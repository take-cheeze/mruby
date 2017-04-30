/*
** vm.c - virtual machine for mruby
**
** See Copyright Notice in mruby.h
*/

#include <stddef.h>
#include <stdarg.h>
#include <math.h>
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>
#include <mruby/irep.h>
#include <mruby/numeric.h>
#include <mruby/proc.h>
#include <mruby/range.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/error.h>
#include <mruby/opcode.h>
#include "value_array.h"
#include <mruby/throw.h>

#ifdef MRB_DISABLE_STDIO
#if defined(__cplusplus)
extern "C" {
#endif
void abort(void);
#if defined(__cplusplus)
}  /* extern "C" { */
#endif
#endif

#define STACK_INIT_SIZE 128
#define CALLINFO_INIT_SIZE 32

#ifndef ENSURE_STACK_INIT_SIZE
#define ENSURE_STACK_INIT_SIZE 16
#endif

#ifndef RESCUE_STACK_INIT_SIZE
#define RESCUE_STACK_INIT_SIZE 16
#endif

/* Define amount of linear stack growth. */
#ifndef MRB_STACK_GROWTH
#define MRB_STACK_GROWTH 128
#endif

/* Maximum mrb_funcall() depth. Should be set lower on memory constrained systems. */
#ifndef MRB_FUNCALL_DEPTH_MAX
#define MRB_FUNCALL_DEPTH_MAX 512
#endif

/* Maximum stack depth. Should be set lower on memory constrained systems.
The value below allows about 60000 recursive calls in the simplest case. */
#ifndef MRB_STACK_MAX
#define MRB_STACK_MAX (0x40000 - MRB_STACK_GROWTH)
#endif

#ifdef VM_DEBUG
# define DEBUG(x) (x)
#else
# define DEBUG(x)
#endif

#define ARGS_PASS_BY_ARRAY -1
#define ARGS_PASS_BY_ARRAY_P(argc) ((argc) < 0)

#define ARENA_RESTORE(mrb,ai) (mrb)->gc.arena_idx = (ai)

#define CALL_PASS_BY_ARRAY 127

#define CONTEXT_MODIFIED_P(c) (!(c)->ci->target_class)

void mrb_method_missing(mrb_state *mrb, mrb_sym name, mrb_value self, mrb_value args);

static inline void
stack_clear(mrb_value *from, size_t count)
{
#ifndef MRB_NAN_BOXING
  const mrb_value mrb_value_zero = { { 0 } };

  while (count-- > 0) {
    *from++ = mrb_value_zero;
  }
#else
  while (count-- > 0) {
    SET_NIL_VALUE(*from);
    from++;
  }
#endif
}

static inline void
stack_copy(mrb_value *dst, const mrb_value *src, size_t size)
{
  while (size-- > 0) {
    *dst++ = *src++;
  }
}

static inline struct REnv*
uvenv(mrb_state *mrb, int up)
{
  struct REnv *e;
  for (e = mrb->c->ci->proc->env; up--; e = (struct REnv*)e->c) {
    if (!e) return NULL;
  }
  return e;
}

static inline mrb_bool
is_strict(mrb_state *mrb, struct REnv *e)
{
  if (MRB_ENV_STACK_SHARED_P(e) && e->target_ci->proc &&
      MRB_PROC_STRICT_P(e->target_ci->proc)) {
    return TRUE;
  }
  return FALSE;
}

static inline struct REnv*
top_env(mrb_state *mrb, struct RProc *proc)
{
  struct REnv *e = proc->env;

  if (is_strict(mrb, e)) return e;
  while (e->c) {
    e = (struct REnv*)e->c;
    if (is_strict(mrb, e)) return e;
  }
  return e;
}

#define CI_ACC_SKIP    -1
#define CI_ACC_DIRECT  -2
#define CI_ACC_RESUMED -3

#define CI_SHARED_STACK(nregs) (-1 - (nregs))

static mrb_callinfo*
stack_expand(mrb_state *mrb, mrb_callinfo *ci, int new_nregs) {
  int const old_nregs = ci->nregs;

  mrb_assert(ci == mrb->c->ci);

  if (old_nregs >= new_nregs) { return ci; }

  ci = (mrb_callinfo*)mrb_realloc(mrb, ci, sizeof(mrb_callinfo) + sizeof(mrb_value) * new_nregs);
  ci->nregs = new_nregs;
  mrb->c->stack = (mrb_value*)((uint8_t*)ci + sizeof(mrb_callinfo));
  stack_clear(mrb->c->stack + old_nregs, new_nregs - old_nregs);

  return mrb->c->ci = ci;
}

static mrb_callinfo*
cipush(mrb_state *mrb, struct RClass *target, mrb_sym mid, struct RProc *p, mrb_bool shared_stack_p)
{
  struct mrb_context *c = mrb->c;
  mrb_callinfo *old_ci = c->ci, *new_ci;
  int nregs;

  if (c->ci_depth >= MRB_FUNCALL_DEPTH_MAX) {
    mrb_exc_raise(mrb, mrb_obj_value(mrb->stack_err));
  }
  c->ci_depth++;

  nregs = MRB_PROC_CFUNC_P(p)? 3 : p->body.irep->nregs;

  // make nregs at least 3 for [`self`, `*`, `&`]
  if (nregs < 3) nregs = 3;

  if (shared_stack_p) {
    old_ci = stack_expand(mrb, old_ci, nregs);
  }

  mrb_assert(nregs >= 3 && nregs < 0x200); // max of GETARG_A()

  new_ci = (mrb_callinfo*)mrb_malloc(mrb,
                                     sizeof(mrb_callinfo) +
                                     sizeof(mrb_value) * (shared_stack_p? 0 : nregs));
  new_ci->ret_ci = old_ci;
  c->ci = new_ci;

  new_ci->eidx = old_ci? old_ci->eidx : 0;
  new_ci->ridx = old_ci? old_ci->ridx : 0;
  new_ci->env = NULL;
  new_ci->pc = NULL;
  new_ci->err = NULL;
  new_ci->proc = p;
  new_ci->acc = 0;
  new_ci->target_class = target;
  new_ci->mid = mid;
  new_ci->nregs = nregs;

  new_ci->stackent = c->stack;
  if (shared_stack_p) {
    new_ci->argc = old_ci->argc;
    new_ci->argv = old_ci->argv;
  }
  else {
    c->stack = (mrb_value*)((uint8_t*)new_ci + sizeof(mrb_callinfo));
    stack_clear(c->stack, nregs);
  }

#if 0
  fprintf(stderr, "cipush ci_depth:%d nregs:%d %s:%s\n",
          c->ci_depth, new_ci->nregs,
          new_ci->target_class? mrb_class_name(mrb, new_ci->target_class) : "unknown",
          mrb_sym2name(mrb, new_ci->mid));
#endif

  if (shared_stack_p) { mrb_assert(c->stack == new_ci->stackent); }

  return new_ci;
}

MRB_API void
mrb_env_unshare(mrb_state *mrb, struct REnv *e)
{
  size_t len = (size_t)MRB_ENV_STACK_LEN(e);
  mrb_value *p;

  if (!MRB_ENV_STACK_SHARED_P(e)) return;

  e->cxt.mid = e->target_ci->mid;
  p = (mrb_value *)mrb_malloc(mrb, sizeof(mrb_value)*len);
  stack_copy(p, e->stack, len);
  e->stack = p;
  MRB_ENV_UNSHARE_STACK(e);
  mrb_write_barrier(mrb, (struct RBasic *)e);
}

static void ecall_current_ci(mrb_state *mrb);

static mrb_code const *
cipop(mrb_state *mrb)
{
  struct mrb_callinfo *old_ci = mrb->c->ci;
  struct REnv *env = old_ci->env;
  mrb_code const *pc = old_ci->pc;

  ecall_current_ci(mrb);

  if (env) {
    mrb_env_unshare(mrb, env);
  }

  if (old_ci->ret_ci) {
    mrb_assert(old_ci->ridx >= old_ci->ret_ci->ridx);
    mrb_assert(old_ci->eidx == old_ci->ret_ci->eidx);
  }

  mrb->c->ci = old_ci->ret_ci;
  mrb->c->stack = old_ci->stackent;
  mrb->c->ci_depth--;

  if (!mrb->c->ci) { mrb_assert(!mrb->c->stack); }

#if 0
  fprintf(stderr, "cipop  ci_depth:%d nregs:%d %s:%s\n",
          mrb->c->ci_depth, old_ci->nregs,
          old_ci->target_class? mrb_class_name(mrb, old_ci->target_class) : "unknown",
          mrb_sym2name(mrb, old_ci->mid));
#endif

  mrb_free(mrb, old_ci);

  return pc;
}

void mrb_exc_set(mrb_state *mrb, mrb_value exc);

static void
ecall(mrb_state *mrb)
{
  mrb_callinfo *ci = mrb->c->ci;
  mrb_value *self = mrb->c->stack;
  struct RObject *exc;
  int ci_depth = mrb->c->ci_depth;
  struct RProc *p = mrb->c->ensure[ci->eidx - 1];

  mrb_assert(p && !MRB_PROC_CFUNC_P(p));
  mrb_assert(!mrb->c->ensure[ci->eidx]);

  mrb->c->ensure[--ci->eidx] = NULL;
  ci = cipush(mrb, p->target_class, 0, p, FALSE);
  ci->acc = CI_ACC_SKIP;
  ci->argc = 0;
  ci->env = p->env;

  exc = mrb->exc; mrb->exc = NULL;

  mrb_run(mrb, p, *self);

  mrb_assert(ci_depth == mrb->c->ci_depth);
  mrb_assert(mrb->c->ci->eidx >= (mrb->c->ci->ret_ci? mrb->c->ci->ret_ci->eidx : 0));
  mrb_assert(!mrb->c->ensure[mrb->c->ci->eidx]);

  if (!mrb->exc) mrb->exc = exc;
}

static void
ecall_current_ci(mrb_state *mrb) {
  mrb_callinfo *ci = mrb->c->ci;
  int const stop_idx = ci->ret_ci? ci->ret_ci->eidx : 0;
  while (ci->eidx > stop_idx) { ecall(mrb); }
  mrb_assert(ci->eidx == stop_idx);
}

#ifndef MRB_FUNCALL_ARGC_MAX
#define MRB_FUNCALL_ARGC_MAX 16
#endif

MRB_API mrb_value
mrb_funcall(mrb_state *mrb, mrb_value self, const char *name, mrb_int argc, ...)
{
  mrb_value argv[MRB_FUNCALL_ARGC_MAX];
  va_list ap;
  mrb_int i;
  mrb_sym mid = mrb_intern_cstr(mrb, name);

  if (argc > MRB_FUNCALL_ARGC_MAX) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Too long arguments. (limit=" MRB_STRINGIZE(MRB_FUNCALL_ARGC_MAX) ")");
  }

  va_start(ap, argc);
  for (i = 0; i < argc; i++) {
    argv[i] = va_arg(ap, mrb_value);
  }
  va_end(ap);
  return mrb_funcall_argv(mrb, self, mid, argc, argv);
}

MRB_API mrb_value
mrb_funcall_with_block(mrb_state *mrb, mrb_value self, mrb_sym mid, mrb_int argc, const mrb_value *argv, mrb_value blk)
{
  mrb_value val;

  if (!mrb->jmp) {
    struct mrb_jmpbuf c_jmp;
    mrb_callinfo *base_ci = mrb->c->ci;

    MRB_TRY(&c_jmp) {
      mrb->jmp = &c_jmp;
      /* recursive call */
      val = mrb_funcall_with_block(mrb, self, mid, argc, argv, blk);
      mrb->jmp = 0;
    }
    MRB_CATCH(&c_jmp) { /* error */
      while (mrb->c->ci != base_ci) {
        cipop(mrb);
      }
      mrb->jmp = 0;
      val = mrb_obj_value(mrb->exc);
    }
    MRB_END_EXC(&c_jmp);
    mrb->jmp = 0;
  }
  else {
    struct RProc *p;
    struct RClass *c;
    mrb_callinfo *ci;
    mrb_value args = mrb_nil_value();

    if (argc < 0) {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "negative argc for funcall (%S)", mrb_fixnum_value(argc));
    }

    c = mrb_class(mrb, self);
    p = mrb_method_search_vm(mrb, &c, mid);
    if (!p) {
      p = mrb_method_search_vm(mrb, &c, mrb_intern_lit(mrb, "method_missing"));

      args = mrb_ary_new_from_values(mrb, argc, argv);
      if (!p) { mrb_method_missing(mrb, mid, self, args); }

      mrb_ary_unshift(mrb, args, mrb_symbol_value(mid));
    }
    else if (argc >= CALL_PASS_BY_ARRAY) {
      args = mrb_ary_new_from_values(mrb, argc, argv);
    }

    ci = cipush(mrb, c, mid, p, FALSE);

    mrb->c->stack[0] = self;
    mrb->c->stack[1] = blk;
    if (mrb_array_p(args)) {
      mrb->c->stack[2] = args;
      ci->argv = mrb->c->stack + 2;
      ci->argc = ARGS_PASS_BY_ARRAY;
    }
    else {
      ci->argc = argc;
      ci->argv = argv;
    }

    if (MRB_PROC_CFUNC_P(p)) {
      int ai = mrb_gc_arena_save(mrb);

      ci->acc = CI_ACC_DIRECT;
      val = p->body.func(mrb, self);
      cipop(mrb);
      mrb_gc_arena_restore(mrb, ai);
    }
    else {
      ci->acc = CI_ACC_SKIP;
      val = mrb_run(mrb, p, self);
    }
  }
  mrb_gc_protect(mrb, val);
  return val;
}

MRB_API mrb_value
mrb_funcall_argv(mrb_state *mrb, mrb_value self, mrb_sym mid, mrb_int argc, const mrb_value *argv)
{
  return mrb_funcall_with_block(mrb, self, mid, argc, argv, mrb_nil_value());
}

mrb_value
mrb_exec_irep(mrb_state *mrb, mrb_value self, struct RProc *p)
{
  mrb_callinfo *ci = mrb->c->ci;

  ci->proc = p;

  if (MRB_PROC_CFUNC_P(p)) {
    ci->target_class = p->target_class;
    return p->body.func(mrb, self);
  }

  ci = cipush(mrb, NULL, 0, p, TRUE);
  ci->pc = p->body.irep->iseq;

  mrb_assert(CONTEXT_MODIFIED_P(mrb->c));

  return self;
}

/* 15.3.1.3.4  */
/* 15.3.1.3.44 */
/*
 *  call-seq:
 *     obj.send(symbol [, args...])        -> obj
 *     obj.__send__(symbol [, args...])      -> obj
 *
 *  Invokes the method identified by _symbol_, passing it any
 *  arguments specified. You can use <code>__send__</code> if the name
 *  +send+ clashes with an existing method in _obj_.
 *
 *     class Klass
 *       def hello(*args)
 *         "Hello " + args.join(' ')
 *       end
 *     end
 *     k = Klass.new
 *     k.send :hello, "gentle", "readers"   #=> "Hello gentle readers"
 */
MRB_API mrb_value
mrb_f_send(mrb_state *mrb, mrb_value self)
{
  mrb_sym name;
  mrb_value block, *argv;
  mrb_int argc;
  struct RProc *p;
  struct RClass *c;
  mrb_callinfo *ci;

  mrb_get_args(mrb, "n*&", &name, &argv, &argc, &block);
  ci = mrb->c->ci;
  if (ci->acc < 0) {
  funcall:
    return mrb_funcall_with_block(mrb, self, name, argc, argv, block);
  }

  c = mrb_class(mrb, self);
  p = mrb_method_search_vm(mrb, &c, name);

  if (!p) {                     /* call method_mising */
    goto funcall;
  }

  ci->mid = name;
  ci->target_class = c;
  /* remove first symbol from arguments */
  if (ARGS_PASS_BY_ARRAY_P(ci->argc)) {
    /* variable length arguments */
    mrb_ary_shift(mrb, *ci->argv);
  } else {
    ci->argc--;
    ci->argv++;
  }

  return mrb_exec_irep(mrb, self, p);
}

static mrb_value
eval_under(mrb_state *mrb, mrb_value self, mrb_value blk, struct RClass *c)
{
  struct RProc *p;
  mrb_callinfo *ci;

  if (mrb_nil_p(blk)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
  }
  p = mrb_proc_ptr(blk);

  ci = mrb->c->ci;
  if (ci->acc == CI_ACC_DIRECT) {
    ci->target_class = c;
    return mrb_yield_cont(mrb, blk, self, 1, &self);
  }

  ci->target_class = c;
  ci->proc = p;
  ci->argc = 1;
  ci->mid = ci->ret_ci->mid;

  mrb->c->stack[0] = self;
  SET_NIL_VALUE(mrb->c->stack[1]); // empty block
  mrb->c->stack[2] = self;

  if (MRB_PROC_CFUNC_P(p)) {
    return p->body.func(mrb, self);
  }

  ci = cipush(mrb, NULL, 0, p, TRUE);
  ci->pc = p->body.irep->iseq;
  ci->ret_ci->argv = mrb->c->stack + 1;
  mrb_assert(CONTEXT_MODIFIED_P(mrb->c));

  return self;
}

/* 15.2.2.4.35 */
/*
 *  call-seq:
 *     mod.class_eval {| | block }  -> obj
 *     mod.module_eval {| | block } -> obj
 *
 *  Evaluates block in the context of _mod_. This can
 *  be used to add methods to a class. <code>module_eval</code> returns
 *  the result of evaluating its argument.
 */
mrb_value
mrb_mod_module_eval(mrb_state *mrb, mrb_value mod)
{
  mrb_value a, b;

  if (mrb_get_args(mrb, "|S&", &a, &b) == 1) {
    mrb_raise(mrb, E_NOTIMP_ERROR, "module_eval/class_eval with string not implemented");
  }
  return eval_under(mrb, mod, b, mrb_class_ptr(mod));
}

/* 15.3.1.3.18 */
/*
 *  call-seq:
 *     obj.instance_eval {| | block }                       -> obj
 *
 *  Evaluates the given block,within  the context of the receiver (_obj_).
 *  In order to set the context, the variable +self+ is set to _obj_ while
 *  the code is executing, giving the code access to _obj_'s
 *  instance variables. In the version of <code>instance_eval</code>
 *  that takes a +String+, the optional second and third
 *  parameters supply a filename and starting line number that are used
 *  when reporting compilation errors.
 *
 *     class KlassWithSecret
 *       def initialize
 *         @secret = 99
 *       end
 *     end
 *     k = KlassWithSecret.new
 *     k.instance_eval { @secret }   #=> 99
 */
mrb_value
mrb_obj_instance_eval(mrb_state *mrb, mrb_value self)
{
  mrb_value a, b;
  mrb_value cv;
  struct RClass *c;

  if (mrb_get_args(mrb, "|S&", &a, &b) == 1) {
    mrb_raise(mrb, E_NOTIMP_ERROR, "instance_eval with string not implemented");
  }
  switch (mrb_type(self)) {
  case MRB_TT_SYMBOL:
  case MRB_TT_FIXNUM:
  case MRB_TT_FLOAT:
    c = 0;
    break;
  default:
    cv = mrb_singleton_class(mrb, self);
    c = mrb_class_ptr(cv);
    break;
  }
  return eval_under(mrb, self, b, c);
}

MRB_API mrb_value
mrb_yield_with_class(mrb_state *mrb, mrb_value b, mrb_int argc, const mrb_value *argv, mrb_value self, struct RClass *c)
{
  struct RProc *p;
  mrb_callinfo *ci;
  mrb_value val;
  int const ci_depth = mrb->c->ci_depth;

  if (mrb_nil_p(b)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
  }
  p = mrb_proc_ptr(b);

  ci = cipush(mrb, c, mrb->c->ci->ret_ci->mid, p, FALSE);
  ci->argc = argc;
  ci->argv = argv;
  ci->acc = CI_ACC_SKIP;

  mrb->c->stack[0] = self;
  SET_NIL_VALUE(mrb->c->stack[1]); // no block

  if (MRB_PROC_CFUNC_P(p)) {
    val = p->body.func(mrb, self);
    cipop(mrb);
  }
  else {
    val = mrb_run(mrb, p, self);
  }

  mrb_assert(ci_depth == mrb->c->ci_depth);

  return val;
}

MRB_API mrb_value
mrb_yield_argv(mrb_state *mrb, mrb_value b, mrb_int argc, const mrb_value *argv)
{
  struct RProc *p = mrb_proc_ptr(b);

  return mrb_yield_with_class(mrb, b, argc, argv, p->env->stack[0], p->target_class);
}

MRB_API mrb_value
mrb_yield(mrb_state *mrb, mrb_value b, mrb_value arg)
{
  struct RProc *p = mrb_proc_ptr(b);

  return mrb_yield_with_class(mrb, b, 1, &arg, p->env->stack[0], p->target_class);
}

mrb_value
mrb_yield_cont(mrb_state *mrb, mrb_value b, mrb_value self, mrb_int argc, const mrb_value *argv)
{
  struct RProc *p;

  if (mrb_nil_p(b) || mrb_type(b) != MRB_TT_PROC) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
  }

  p = mrb_proc_ptr(b);

  mrb->c->ci->argc = ARGS_PASS_BY_ARRAY;
  SET_NIL_VALUE(mrb->c->stack[1]);
  mrb->c->stack[2] = mrb_ary_new_from_values(mrb, argc, argv);
  return mrb_exec_irep(mrb, self, p);
}

typedef enum {
  LOCALJUMP_ERROR_RETURN = 0,
  LOCALJUMP_ERROR_BREAK = 1,
  LOCALJUMP_ERROR_YIELD = 2
} localjump_error_kind;

static void
localjump_error(mrb_state *mrb, localjump_error_kind kind)
{
  char kind_str[3][7] = { "return", "break", "yield" };
  char kind_str_len[] = { 6, 5, 5 };
  static const char lead[] = "unexpected ";
  mrb_value msg;
  mrb_value exc;

  msg = mrb_str_buf_new(mrb, sizeof(lead) + 7);
  mrb_str_cat(mrb, msg, lead, sizeof(lead) - 1);
  mrb_str_cat(mrb, msg, kind_str[kind], kind_str_len[kind]);
  exc = mrb_exc_new_str(mrb, E_LOCALJUMP_ERROR, msg);
  mrb_exc_set(mrb, exc);
}

static void
argnum_error(mrb_state *mrb, mrb_int num)
{
  mrb_value exc;
  mrb_value str;
  mrb_int argc = mrb->c->ci->argc;

  if (ARGS_PASS_BY_ARRAY_P(argc)) {
    mrb_value const args = mrb->c->stack[2];
    if (mrb_array_p(args)) {
      argc = RARRAY_LEN(args);
    }
  }
  if (mrb->c->ci->mid) {
    str = mrb_format(mrb, "'%S': wrong number of arguments (%S for %S)",
                  mrb_sym2str(mrb, mrb->c->ci->mid),
                  mrb_fixnum_value(argc), mrb_fixnum_value(num));
  }
  else {
    str = mrb_format(mrb, "wrong number of arguments (%S for %S)",
                     mrb_fixnum_value(argc), mrb_fixnum_value(num));
  }
  exc = mrb_exc_new_str(mrb, E_ARGUMENT_ERROR, str);
  mrb_exc_set(mrb, exc);
}

#define ERR_PC_SET(mrb, pc) mrb->c->ci->err = pc;
#define ERR_PC_CLR(mrb)     mrb->c->ci->err = 0;
#ifdef MRB_ENABLE_DEBUG_HOOK
#define CODE_FETCH_HOOK(mrb, irep, pc, regs) if ((mrb)->code_fetch_hook) (mrb)->code_fetch_hook((mrb), (irep), (pc), (regs));
#else
#define CODE_FETCH_HOOK(mrb, irep, pc, regs)
#endif

#ifdef MRB_BYTECODE_DECODE_OPTION
#define BYTECODE_DECODER(x) ((mrb)->bytecode_decoder)?(mrb)->bytecode_decoder((mrb), (x)):(x)
#else
#define BYTECODE_DECODER(x) (x)
#endif


#if defined __GNUC__ || defined __clang__ || defined __INTEL_COMPILER
#define DIRECT_THREADED
#endif

#ifndef DIRECT_THREADED

#define INIT_DISPATCH for (;;) { i = BYTECODE_DECODER(*pc); CODE_FETCH_HOOK(mrb, irep, pc, regs); switch (GET_OPCODE(i)) {
#define CASE(op) case op:
#define NEXT pc++; break
#define JUMP break
#define END_DISPATCH }}

#else

#define INIT_DISPATCH JUMP; return mrb_nil_value();
#define CASE(op) L_ ## op:
#define NEXT i=BYTECODE_DECODER(*++pc); CODE_FETCH_HOOK(mrb, irep, pc, regs); goto *optable[GET_OPCODE(i)]
#define JUMP i=BYTECODE_DECODER(*pc); CODE_FETCH_HOOK(mrb, irep, pc, regs); goto *optable[GET_OPCODE(i)]

#define END_DISPATCH

#endif

MRB_API mrb_value
mrb_vm_run(mrb_state *mrb, struct RProc *proc, mrb_value self, unsigned int stack_keep)
{
  mrb_irep *irep = proc->body.irep;
  mrb_value result;
  struct mrb_context *c = mrb->c;

  mrb_assert(!MRB_PROC_CFUNC_P(proc));

  if (!c->stack) {
    cipush(mrb, mrb_class(mrb, self), 0, proc, FALSE);
  }
  c->stack[0] = self;
  result = mrb_vm_exec(mrb, proc, irep->iseq);
  mrb->c = c;
  return result;
}

MRB_API mrb_value
mrb_vm_exec(mrb_state *mrb, struct RProc *proc, mrb_code const *pc)
{
  mrb_irep *irep = proc->body.irep;
  mrb_value *pool = irep->pool;
  mrb_sym *syms = irep->syms;
  mrb_code i;
  int ai = mrb_gc_arena_save(mrb);
  struct mrb_jmpbuf *prev_jmp = mrb->jmp;
  struct mrb_jmpbuf c_jmp;

#ifdef DIRECT_THREADED
  static void *optable[] = {
    &&L_OP_NOP, &&L_OP_MOVE,
    &&L_OP_LOADL, &&L_OP_LOADI, &&L_OP_LOADSYM, &&L_OP_LOADNIL,
    &&L_OP_LOADSELF, &&L_OP_LOADT, &&L_OP_LOADF,
    &&L_OP_GETGLOBAL, &&L_OP_SETGLOBAL, &&L_OP_GETSPECIAL, &&L_OP_SETSPECIAL,
    &&L_OP_GETIV, &&L_OP_SETIV, &&L_OP_GETCV, &&L_OP_SETCV,
    &&L_OP_GETCONST, &&L_OP_SETCONST, &&L_OP_GETMCNST, &&L_OP_SETMCNST,
    &&L_OP_GETUPVAR, &&L_OP_SETUPVAR,
    &&L_OP_JMP, &&L_OP_JMPIF, &&L_OP_JMPNOT,
    &&L_OP_ONERR, &&L_OP_RESCUE, &&L_OP_POPERR, &&L_OP_RAISE, &&L_OP_EPUSH, &&L_OP_EPOP,
    &&L_OP_SEND, &&L_OP_SENDB, &&L_OP_FSEND,
    &&L_OP_CALL, &&L_OP_SUPER, &&L_OP_ARGARY, &&L_OP_ENTER,
    &&L_OP_KARG, &&L_OP_KDICT, &&L_OP_RETURN, &&L_OP_TAILCALL, &&L_OP_BLKPUSH,
    &&L_OP_ADD, &&L_OP_ADDI, &&L_OP_SUB, &&L_OP_SUBI, &&L_OP_MUL, &&L_OP_DIV,
    &&L_OP_EQ, &&L_OP_LT, &&L_OP_LE, &&L_OP_GT, &&L_OP_GE,
    &&L_OP_ARRAY, &&L_OP_ARYCAT, &&L_OP_ARYPUSH, &&L_OP_AREF, &&L_OP_ASET, &&L_OP_APOST,
    &&L_OP_STRING, &&L_OP_STRCAT, &&L_OP_HASH,
    &&L_OP_LAMBDA, &&L_OP_RANGE, &&L_OP_OCLASS,
    &&L_OP_CLASS, &&L_OP_MODULE, &&L_OP_EXEC,
    &&L_OP_METHOD, &&L_OP_SCLASS, &&L_OP_TCLASS,
    &&L_OP_DEBUG, &&L_OP_STOP, &&L_OP_ERR,
  };
#endif

  mrb_bool exc_catched = FALSE;
RETRY_TRY_BLOCK:

  mrb_assert(!MRB_PROC_CFUNC_P(proc));
  mrb_assert(mrb->c->ci);

  MRB_TRY(&c_jmp) {

  if (exc_catched) {
    exc_catched = FALSE;
    goto L_RAISE;
  }
  mrb->jmp = &c_jmp;
  mrb_assert(mrb->c->ci->proc == proc);
  mrb_assert(mrb->c->ci->nregs >= irep->nregs);

#define regs (mrb->c->stack)

#define UPDATE_IREP(i)                          \
  do {                                          \
    irep = (i);                                 \
    pool = irep->pool;                          \
    syms = irep->syms;                          \
  } while(FALSE)                                \

#define UPDATE_PROC(p)                          \
  do {                                          \
    mrb_assert(!MRB_PROC_CFUNC_P(p));           \
    proc = (p);                                 \
    UPDATE_IREP(proc->body.irep);               \
  } while(FALSE)                                \

  INIT_DISPATCH {
    CASE(OP_NOP) {
      /* do nothing */
      NEXT;
    }

    CASE(OP_MOVE) {
      /* A B    R(A) := R(B) */
      regs[GETARG_A(i)] = regs[GETARG_B(i)];
      NEXT;
    }

    CASE(OP_LOADL) {
      /* A Bx   R(A) := Pool(Bx) */
#ifdef MRB_WORD_BOXING
      mrb_value val = pool[GETARG_Bx(i)];
      if (mrb_float_p(val)) {
        val = mrb_float_value(mrb, mrb_float(val));
      }
      regs[GETARG_A(i)] = val;
#else
      regs[GETARG_A(i)] = pool[GETARG_Bx(i)];
#endif
      NEXT;
    }

    CASE(OP_LOADI) {
      /* A sBx  R(A) := sBx */
      SET_INT_VALUE(regs[GETARG_A(i)], GETARG_sBx(i));
      NEXT;
    }

    CASE(OP_LOADSYM) {
      /* A Bx   R(A) := Syms(Bx) */
      SET_SYM_VALUE(regs[GETARG_A(i)], syms[GETARG_Bx(i)]);
      NEXT;
    }

    CASE(OP_LOADSELF) {
      /* A      R(A) := self */
      regs[GETARG_A(i)] = regs[0];
      NEXT;
    }

    CASE(OP_LOADT) {
      /* A      R(A) := true */
      SET_TRUE_VALUE(regs[GETARG_A(i)]);
      NEXT;
    }

    CASE(OP_LOADF) {
      /* A      R(A) := false */
      SET_FALSE_VALUE(regs[GETARG_A(i)]);
      NEXT;
    }

    CASE(OP_GETGLOBAL) {
      /* A Bx   R(A) := getglobal(Syms(Bx)) */
      regs[GETARG_A(i)] = mrb_gv_get(mrb, syms[GETARG_Bx(i)]);
      NEXT;
    }

    CASE(OP_SETGLOBAL) {
      /* setglobal(Syms(Bx), R(A)) */
      mrb_gv_set(mrb, syms[GETARG_Bx(i)], regs[GETARG_A(i)]);
      NEXT;
    }

    CASE(OP_GETSPECIAL) {
      /* A Bx   R(A) := Special[Bx] */
      regs[GETARG_A(i)] = mrb_vm_special_get(mrb, GETARG_Bx(i));
      NEXT;
    }

    CASE(OP_SETSPECIAL) {
      /* A Bx   Special[Bx] := R(A) */
      mrb_vm_special_set(mrb, GETARG_Bx(i), regs[GETARG_A(i)]);
      NEXT;
    }

    CASE(OP_GETIV) {
      /* A Bx   R(A) := ivget(Bx) */
      regs[GETARG_A(i)] = mrb_vm_iv_get(mrb, syms[GETARG_Bx(i)]);
      NEXT;
    }

    CASE(OP_SETIV) {
      /* ivset(Syms(Bx),R(A)) */
      mrb_vm_iv_set(mrb, syms[GETARG_Bx(i)], regs[GETARG_A(i)]);
      NEXT;
    }

    CASE(OP_GETCV) {
      /* A Bx   R(A) := cvget(Syms(Bx)) */
      ERR_PC_SET(mrb, pc);
      regs[GETARG_A(i)] = mrb_vm_cv_get(mrb, syms[GETARG_Bx(i)]);
      ERR_PC_CLR(mrb);
      NEXT;
    }

    CASE(OP_SETCV) {
      /* cvset(Syms(Bx),R(A)) */
      mrb_vm_cv_set(mrb, syms[GETARG_Bx(i)], regs[GETARG_A(i)]);
      NEXT;
    }

    CASE(OP_GETCONST) {
      /* A Bx    R(A) := constget(Syms(Bx)) */
      mrb_value val;
      int a = GETARG_A(i);
      int bx = GETARG_Bx(i);
      mrb_sym sym = syms[bx];

      ERR_PC_SET(mrb, pc);
      val = mrb_vm_const_get(mrb, sym);
      ERR_PC_CLR(mrb);
      regs[a] = val;
      NEXT;
    }

    CASE(OP_SETCONST) {
      /* A Bx   constset(Syms(Bx),R(A)) */
      mrb_vm_const_set(mrb, syms[GETARG_Bx(i)], regs[GETARG_A(i)]);
      NEXT;
    }

    CASE(OP_GETMCNST) {
      /* A Bx   R(A) := R(A)::Syms(Bx) */
      mrb_value val;
      int a = GETARG_A(i);

      ERR_PC_SET(mrb, pc);
      val = mrb_const_get(mrb, regs[a], syms[GETARG_Bx(i)]);
      ERR_PC_CLR(mrb);
      regs[a] = val;
      NEXT;
    }

    CASE(OP_SETMCNST) {
      /* A Bx    R(A+1)::Syms(Bx) := R(A) */
      int a = GETARG_A(i);

      mrb_const_set(mrb, regs[a+1], syms[GETARG_Bx(i)], regs[a]);
      NEXT;
    }

    CASE(OP_GETUPVAR) {
      /* A B C  R(A) := uvget(B,C) */
      mrb_value *regs_a = regs + GETARG_A(i);
      int up = GETARG_C(i);
      struct REnv *e = uvenv(mrb, up);

      if (!e) {
        SET_NIL_VALUE(*regs_a);
      }
      else {
        int idx = GETARG_B(i);
        *regs_a = e->stack[idx];
      }
      NEXT;
    }

    CASE(OP_SETUPVAR) {
      /* A B C  uvset(B,C,R(A)) */
      int up = GETARG_C(i);

      struct REnv *e = uvenv(mrb, up);

      if (e) {
        mrb_value *regs_a = regs + GETARG_A(i);
        int idx = GETARG_B(i);
        e->stack[idx] = *regs_a;
        mrb_write_barrier(mrb, (struct RBasic*)e);
      }
      NEXT;
    }

    CASE(OP_JMP) {
      /* sBx    pc+=sBx */
      pc += GETARG_sBx(i);
      JUMP;
    }

    CASE(OP_JMPIF) {
      /* A sBx  if R(A) pc+=sBx */
      if (mrb_test(regs[GETARG_A(i)])) {
        pc += GETARG_sBx(i);
        JUMP;
      }
      NEXT;
    }

    CASE(OP_JMPNOT) {
      /* A sBx  if !R(A) pc+=sBx */
      if (!mrb_test(regs[GETARG_A(i)])) {
        pc += GETARG_sBx(i);
        JUMP;
      }
      NEXT;
    }

    CASE(OP_ONERR) {
      /* sBx    pc+=sBx on exception */
      if (mrb->c->rsize <= mrb->c->ci->ridx) {
        if (mrb->c->rsize == 0) mrb->c->rsize = RESCUE_STACK_INIT_SIZE;
        else mrb->c->rsize *= 2;
        mrb->c->rescue = (mrb_code const **)mrb_realloc(mrb, mrb->c->rescue, sizeof(mrb_code*) * mrb->c->rsize);
      }
      mrb->c->rescue[mrb->c->ci->ridx++] = pc + GETARG_sBx(i);
      NEXT;
    }

    CASE(OP_RESCUE) {
      /* A B    R(A) := exc; clear(exc); R(B) := matched (bool) */
      int a = GETARG_A(i);
      int b = GETARG_B(i);
      int c = GETARG_C(i);
      mrb_value exc;

      if (c == 0) {
        exc = mrb_obj_value(mrb->exc);
        mrb->exc = 0;
      }
      else {           /* continued; exc taken from R(A) */
        exc = regs[a];
      }
      if (b != 0) {
        mrb_value e = regs[b];
        struct RClass *ec;

        switch (mrb_type(e)) {
        case MRB_TT_CLASS:
        case MRB_TT_MODULE:
          break;
        default:
          mrb_raise(mrb, E_TYPE_ERROR, "class or module required for rescue clause");
          break;
        }
        ec = mrb_class_ptr(e);
        regs[b] = mrb_bool_value(mrb_obj_is_kind_of(mrb, exc, ec));
      }
      if (a != 0 && c == 0) {
        regs[a] = exc;
      }
      NEXT;
    }

    CASE(OP_POPERR) {
      /* A      A.times{rescue_pop()} */
      mrb->c->ci->ridx -= GETARG_A(i);
      NEXT;
    }

    CASE(OP_RAISE) {
      /* A      raise(R(A)) */
      mrb_exc_set(mrb, regs[GETARG_A(i)]);
      goto L_RAISE;
    }

    CASE(OP_EPUSH) {
      /* Bx     ensure_push(SEQ[Bx]) */
      struct RProc *p;

      p = mrb_closure_new(mrb, irep->reps[GETARG_Bx(i)]);
      /* push ensure_stack */
      if (mrb->c->esize <= mrb->c->ci->eidx+1) {
        if (mrb->c->esize == 0) mrb->c->esize = ENSURE_STACK_INIT_SIZE;
        else mrb->c->esize *= 2;
        mrb->c->ensure = (struct RProc **)mrb_realloc(mrb, mrb->c->ensure, sizeof(struct RProc*) * mrb->c->esize);
      }
      mrb->c->ensure[mrb->c->ci->eidx++] = p;
      mrb->c->ensure[mrb->c->ci->eidx] = NULL;
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_EPOP) {
      /* A      A.times{ensure_pop().call} */
      int a = GETARG_A(i);
      mrb_callinfo *ci = mrb->c->ci;
      int n;

      for (n = 0; n < a && (!ci->ret_ci || ci->eidx > ci->ret_ci->eidx); n++) {
        ecall(mrb);
        mrb_assert(ci == mrb->c->ci);
        ARENA_RESTORE(mrb, ai);
      }
      NEXT;
    }

    CASE(OP_LOADNIL) {
      /* A     R(A) := nil */
      int a = GETARG_A(i);

      SET_NIL_VALUE(regs[a]);
      NEXT;
    }

    CASE(OP_SENDB) {
      /* A B C  R(A) := call(R(A),Syms(B),R(A+1),...,R(A+C),&R(A+C+1))*/
      /* fall through */
    };

  L_SEND:
    CASE(OP_SEND) {
      /* A B C  R(A) := call(R(A),Syms(B),R(A+1),...,R(A+C)) */
      int const a = GETARG_A(i);
      int n = GETARG_C(i);
      struct RProc *m;
      struct RClass *c;
      mrb_callinfo *ci;
      mrb_value recv, result, blk;
      mrb_sym mid = syms[GETARG_B(i)];

      recv = regs[a];
      blk = GET_OPCODE(i) == OP_SENDB
            ? regs[(n == CALL_PASS_BY_ARRAY)? a + 2 : a + n + 1]
            : mrb_nil_value();
      c = mrb_class(mrb, recv);
      m = mrb_method_search_vm(mrb, &c, mid);
      if (!m) {
        mrb_value sym = mrb_symbol_value(mid);
        mrb_sym missing = mrb_intern_lit(mrb, "method_missing");

        m = mrb_method_search_vm(mrb, &c, missing);
        if (!m) {
          mrb_method_missing(mrb, mid, recv,
                             n == CALL_PASS_BY_ARRAY
                             ? regs[a+1]
                             : mrb_ary_new_from_values(mrb, n, regs+a+1));
        }

        mid = missing;
        if (n != CALL_PASS_BY_ARRAY) {
          regs[a+1] = mrb_ary_new_from_values(mrb, n, regs+a+1);
          n = CALL_PASS_BY_ARRAY;
        }
        mrb_assert(mrb_array_p(regs[a+1]));
        mrb_ary_unshift(mrb, regs[a+1], sym);
      }

      /* push callinfo */
      ci = cipush(mrb, c, mid, m, FALSE);
      ci->pc = pc + 1;
      ci->acc = a;
      ci->argc = n == CALL_PASS_BY_ARRAY? ARGS_PASS_BY_ARRAY : n;
      ci->argv = ci->stackent + a + 1;
      regs[0] = recv;
      regs[1] = !mrb_nil_p(blk)? mrb_convert_type(mrb, blk, MRB_TT_PROC, "Proc", "to_proc") : blk;

      if (MRB_PROC_CFUNC_P(m)) {
        result = m->body.func(mrb, recv);
        ci = mrb->c->ci;
        mrb_gc_arena_restore(mrb, ai);
        if (mrb->exc) goto L_RAISE;
        if (CONTEXT_MODIFIED_P(mrb->c)) { /* return from context modifying method (resume/yield) */
          if (ci->acc == CI_ACC_RESUMED) {
            mrb->jmp = prev_jmp;
            return result;
          }
          else {
            mrb_assert(!MRB_PROC_CFUNC_P(ci->proc));
            UPDATE_PROC(ci->proc);
          }
        }
        else {
          ci->stackent[ci->acc] = result;
        }
        pc = cipop(mrb);
        JUMP;
      }
      else {
        /* setup environment for calling method */
        UPDATE_PROC(mrb->c->ci->proc = m);
        pc = irep->iseq;
        JUMP;
      }
    }

    CASE(OP_FSEND) {
      /* A B C  R(A) := fcall(R(A),Syms(B),R(A+1),... ,R(A+C-1)) */
      NEXT;
    }

    CASE(OP_CALL) {
      /* A      R(A) := self.call(frame.argc, frame.argv) */
      mrb_callinfo *ci = mrb->c->ci;
      mrb_value recv = regs[0];
      struct RProc *m = mrb_proc_ptr(recv);

      /* replace callinfo */
      ci->target_class = m->target_class;
      ci->proc = m;
      if (m->env) {
        mrb_sym mid;

        mid = MRB_ENV_STACK_SHARED_P(m->env)
              ? m->env->target_ci->mid : m->env->cxt.mid;
        if (mid) ci->mid = mid;
        if (!m->env->stack) m->env->stack = mrb->c->stack;
      }

      /* prepare stack */
      if (MRB_PROC_CFUNC_P(m)) {
        recv = m->body.func(mrb, recv);
        mrb_gc_arena_restore(mrb, ai);
        if (mrb->exc) goto L_RAISE;
        /* pop stackpos */
        ci->stackent[ci->acc] = recv;
        pc = cipop(mrb);
        UPDATE_IREP(mrb->c->ci->proc->body.irep);
        JUMP;
      }
      else {
        /* setup environment for calling method */
        mrb_assert(m->body.irep);

        stack_expand(mrb, ci, m->body.irep->nregs);

        UPDATE_PROC(m);
        pc = irep->iseq;
        if (m->env) { regs[0] = m->env->stack[0]; }
        JUMP;
      }
    }

    CASE(OP_SUPER) {
      /* A C  R(A) := super(R(A+1),... ,R(A+C+1)) */
      mrb_value recv;
      mrb_callinfo *ci = mrb->c->ci;
      struct RProc *m;
      struct RClass *c;
      mrb_sym mid = ci->mid;
      int a = GETARG_A(i);
      int n = GETARG_C(i);
      mrb_value blk;

      if (mid == 0 || CONTEXT_MODIFIED_P(mrb->c)) {
        mrb_value exc;

        exc = mrb_exc_new_str_lit(mrb, E_NOMETHOD_ERROR, "super called outside of method");
        mrb_exc_set(mrb, exc);
        goto L_RAISE;
      }
      recv = regs[0];
      blk = regs[n == CALL_PASS_BY_ARRAY? a+2 : a+n+1];
      c = mrb->c->ci->target_class->super;
      m = mrb_method_search_vm(mrb, &c, mid);
      if (!m) {
        mrb_sym missing = mrb_intern_lit(mrb, "method_missing");
        m = mrb_method_search_vm(mrb, &c, missing);
        if (!m) {
          mrb_method_missing(mrb, mid, recv,
                             n == CALL_PASS_BY_ARRAY
                             ? regs[a+1]
                             : mrb_ary_new_from_values(mrb, n, regs+a+1));
        }
        mid = missing;
        if (n != CALL_PASS_BY_ARRAY) {
          regs[a+1] = mrb_ary_new_from_values(mrb, n, regs+a+1);
          n = CALL_PASS_BY_ARRAY;
        }
        mrb_ary_unshift(mrb, regs[a+1], mrb_symbol_value(ci->mid));
      }

      /* push callinfo */
      ci = cipush(mrb, c, mid, m, FALSE);
      ci->pc = pc + 1;
      ci->argc = n == CALL_PASS_BY_ARRAY? ARGS_PASS_BY_ARRAY : n;
      ci->argv = ci->stackent + a + 1;

      /* prepare stack */
      regs[0] = recv;
      regs[1] = !mrb_nil_p(blk)? mrb_convert_type(mrb, blk, MRB_TT_PROC, "Proc", "to_proc") : blk;

      if (MRB_PROC_CFUNC_P(m)) {
        mrb_value v;

        v = m->body.func(mrb, recv);
        mrb_gc_arena_restore(mrb, ai);
        if (mrb->exc) goto L_RAISE;
        ci = mrb->c->ci;
        if (CONTEXT_MODIFIED_P(mrb->c)) { /* return from context modifying method (resume/yield) */
          if (ci->acc == CI_ACC_RESUMED) {
            mrb->jmp = prev_jmp;
            return v;
          }
          else {
            UPDATE_PROC(ci->ret_ci->proc);
          }
        }
        ci->stackent[a] = v;
        /* pop stackpos */
        pc = cipop(mrb);
        JUMP;
      }
      else {
        /* fill callinfo */
        ci->acc = a;

        /* setup environment for calling method */
        UPDATE_IREP(m->body.irep);
        pc = irep->iseq;
        JUMP;
      }
    }

    CASE(OP_ARGARY) {
      /* A Bx   R(A) := argument array (16=6:1:5:4) */
      int a = GETARG_A(i);
      int bx = GETARG_Bx(i);
      int m1 = (bx>>10)&0x3f;
      int r  = (bx>>9)&0x1;
      int m2 = (bx>>4)&0x1f;
      int lv = (bx>>0)&0xf;
      mrb_value *stack;

      if (mrb->c->ci->mid == 0 || CONTEXT_MODIFIED_P(mrb->c)) {
        mrb_value exc;

      L_NOSUPER:
        exc = mrb_exc_new_str_lit(mrb, E_NOMETHOD_ERROR, "super called outside of method");
        mrb_exc_set(mrb, exc);
        goto L_RAISE;
      }
      if (lv == 0) stack = regs + 2;
      else {
        struct REnv *e = uvenv(mrb, lv-1);
        if (!e) goto L_NOSUPER;
        stack = e->stack + 1;
      }
      if (r == 0) {
        regs[a] = mrb_ary_new_from_values(mrb, m1+m2, stack);
      }
      else {
        mrb_value *pp = NULL;
        struct RArray *rest;
        int len = 0;

        if (mrb_array_p(stack[m1])) {
          struct RArray *ary = mrb_ary_ptr(stack[m1]);

          pp = ary->ptr;
          len = ary->len;
        }
        regs[a] = mrb_ary_new_capa(mrb, m1+len+m2);
        rest = mrb_ary_ptr(regs[a]);
        if (m1 > 0) {
          stack_copy(rest->ptr, stack, m1);
        }
        if (len > 0) {
          stack_copy(rest->ptr+m1, pp, len);
        }
        if (m2 > 0) {
          stack_copy(rest->ptr+m1+len, stack+m1+1, m2);
        }
        rest->len = m1+len+m2;
      }
      regs[a+1] = stack[m1+r+m2];
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_ENTER) {
      /* Ax             arg setup according to flags (23=5:5:1:5:5:1:1) */
      /* number of optional arguments times OP_JMP should follow */
      mrb_aspec ax = GETARG_Ax(i);
      int m1 = MRB_ASPEC_REQ(ax);
      int o  = MRB_ASPEC_OPT(ax);
      int r  = MRB_ASPEC_REST(ax);
      int m2 = MRB_ASPEC_POST(ax);
      /* unused
      int k  = MRB_ASPEC_KEY(ax);
      int kd = MRB_ASPEC_KDICT(ax);
      int b  = MRB_ASPEC_BLOCK(ax);
      */
      int argc = mrb->c->ci->argc;
      mrb_value const *argv = mrb->c->ci->argv;
      int const len = m1 + o + r + m2;
      mrb_value const blk = regs[1];

      mrb_gc_protect(mrb, blk);

      if (ARGS_PASS_BY_ARRAY_P(argc)) {
        struct RArray *ary = mrb_ary_ptr(*argv);
        mrb_assert(mrb_array_p(*argv));
        argv = ary->ptr;
        argc = ary->len;
      }

      if (mrb->c->ci->proc && MRB_PROC_STRICT_P(mrb->c->ci->proc)) {
        if (argc >= 0 && (argc < m1 + m2 || (r == 0 && argc > len))) {
          argnum_error(mrb, m1+m2);
          goto L_RAISE;
        }
      }
      else if (len > 1 && argc == 1 && mrb_array_p(*argv)) {
        argc = mrb_ary_ptr(*argv)->len;
        argv = mrb_ary_ptr(*argv)->ptr;
      }

      regs[len+1] = blk; /* move block */
      if (argc < len) {
        int mlen = m2;
        if (argc < m1+m2) {
          if (m1 < argc)
            mlen = argc - m1;
          else
            mlen = 0;
        }
        value_move(&regs[1], argv, argc-mlen); /* m1 + o */
        if (argc < m1) {
          stack_clear(&regs[argc+1], m1-argc);
        }
        if (mlen) {
          value_move(&regs[len-m2+1], &argv[argc-mlen], mlen);
        }
        if (mlen < m2) {
          stack_clear(&regs[len-m2+mlen+1], m2-mlen);
        }
        if (r) {
          regs[m1+o+1] = mrb_ary_new_capa(mrb, 0);
        }
        if (o == 0 || argc < m1+m2) pc++;
        else
          pc += argc - m1 - m2 + 1;
      }
      else {
        int rnum = 0;
        value_move(&regs[1], argv, m1+o);
        if (r) {
          rnum = argc-m1-o-m2;
          regs[m1+o+1] = mrb_ary_new_from_values(mrb, rnum, argv+m1+o);
        }
        if (m2) {
          if (argc-m2 > m1) {
            value_move(&regs[m1+o+r+1], &argv[m1+o+rnum], m2);
          }
        }
        pc += o + 1;
      }
      mrb->c->ci->argc = len;
      mrb->c->ci->argv = regs + 1;

      JUMP;
    }

    CASE(OP_KARG) {
      /* A B C          R(A) := kdict[Syms(B)]; if C kdict.rm(Syms(B)) */
      /* if C == 2; raise unless kdict.empty? */
      /* OP_JMP should follow to skip init code */
      NEXT;
    }

    CASE(OP_KDICT) {
      /* A C            R(A) := kdict */
      NEXT;
    }

    L_RETURN:
      i = MKOP_AB(OP_RETURN, GETARG_A(i), OP_R_NORMAL);
      /* fall through */
    CASE(OP_RETURN) {
      /* A B     return R(A) (B=normal,in-block return/break) */
      if (mrb->exc) {
        mrb_callinfo *ci;

      L_RAISE:
        ci = mrb->c->ci;
        mrb_obj_iv_ifnone(mrb, mrb->exc, mrb_intern_lit(mrb, "lastpc"), mrb_cptr_value(mrb, (void*)pc));
        mrb_obj_iv_ifnone(mrb, mrb->exc, mrb_intern_lit(mrb, "ciidx"), mrb_fixnum_value(mrb->c->ci_depth));
        if (!ci->ret_ci) {
          goto L_RESCUE;
        }

        // unwind callinfo without rescue block
        for (ci = mrb->c->ci; ci->ret_ci && ci->ridx == ci->ret_ci->ridx; ci = ci->ret_ci, cipop(mrb)) {
          if (ci->acc == CI_ACC_SKIP && prev_jmp) {
            mrb->jmp = prev_jmp;
            cipop(mrb);
            MRB_THROW(prev_jmp);
          }

          if (!ci->ret_ci) {
            if (ci->ridx == 0) {
              if (mrb->c == mrb->root_c) {
                mrb->c->stack = NULL;
                goto L_STOP;
              }
              else {
                struct mrb_context *c = mrb->c;

                mrb->c = c->prev;
                c->prev = NULL;
                goto L_RAISE;
              }
            }
            break;
          }
        }

     L_RESCUE:
        if (ci->ridx == 0) goto L_STOP;
        UPDATE_PROC(ci->proc);
        pc = mrb->c->rescue[--ci->ridx];
        mrb_assert(irep->iseq <= pc && pc < irep->iseq + irep->ilen);
      }
      else {
        mrb_callinfo *ci = mrb->c->ci;
        int acc;
        mrb_value const v = regs[GETARG_A(i)];

        switch (GETARG_B(i)) {
        case OP_R_RETURN:
          /* Fall through to OP_R_NORMAL otherwise */
          if (ci->acc >=0 && proc->env && !MRB_PROC_STRICT_P(proc)) {
            struct REnv *e = top_env(mrb, proc);
            mrb_callinfo *ce = e->target_ci;

            if (!ce->ret_ci || !MRB_ENV_STACK_SHARED_P(e)) {
              localjump_error(mrb, LOCALJUMP_ERROR_RETURN);
              goto L_RAISE;
            }

            for (ci = mrb->c->ci; ci != ce; ci = ci->ret_ci, cipop(mrb)) {
              if (ci->acc < 0) {
                localjump_error(mrb, LOCALJUMP_ERROR_RETURN);
                goto L_RAISE;
              }
            }
            break;
          }
        case OP_R_NORMAL:
          if (!ci->ret_ci) {
            if (!mrb->c->prev) { /* toplevel return */
              localjump_error(mrb, LOCALJUMP_ERROR_RETURN);
              goto L_RAISE;
            }
            if (!mrb->c->prev->ci->ret_ci) {
              mrb_exc_set(mrb, mrb_exc_new_str_lit(mrb, E_FIBER_ERROR, "double resume"));
              goto L_RAISE;
            }
            /* automatic yield at the end */
            mrb->c->status = MRB_FIBER_TERMINATED;
            mrb->c = mrb->c->prev;
            mrb->c->status = MRB_FIBER_RUNNING;
          }
          ci = mrb->c->ci;
          break;
        case OP_R_BREAK:
          if (!proc->env || !MRB_ENV_STACK_SHARED_P(proc->env)) {
            mrb_value exc;

          L_BREAK_ERROR:
            exc = mrb_exc_new_str_lit(mrb, E_LOCALJUMP_ERROR,
                                      "break from proc-closure");
            mrb_exc_set(mrb, exc);
            goto L_RAISE;
          }
          /* break from fiber block */
          if (!mrb->c->ci->ret_ci && mrb->c->ci->pc) {
            struct mrb_context *c = mrb->c;

            mrb->c = c->prev;
            c->prev = NULL;
            ci = mrb->c->ci;
          }
          if (ci->acc < 0) {
            mrb->c->vmexec = FALSE;
            mrb->jmp = prev_jmp;
            return v;
          }
          for (ci = mrb->c->ci; ci->ret_ci != proc->env->target_ci; ci = ci->ret_ci, cipop(mrb)) {
            if (ci->acc == CI_ACC_SKIP) {
              goto L_BREAK_ERROR;
            }
          }
          break;
        default:
          /* cannot happen */
          mrb_assert(FALSE);
          break;
        }
        if (mrb->c->vmexec && CONTEXT_MODIFIED_P(mrb->c)) {
          mrb->c->vmexec = FALSE;
          mrb->jmp = prev_jmp;
          return v;
        }
        ci = mrb->c->ci;
        acc = ci->acc;
        DEBUG(fprintf(stderr, "from :%s\n", mrb_sym2name(mrb, ci->mid)));
        pc = cipop(mrb);
        if (acc == CI_ACC_SKIP || acc == CI_ACC_DIRECT) {
          mrb->jmp = prev_jmp;
          return v;
        }
        UPDATE_PROC(mrb->c->ci->proc);

        regs[acc] = v;
      }
      JUMP;
    }

    CASE(OP_TAILCALL) {
      /* A B C  return call(R(A),Syms(B),R(A+1),... ,R(A+C+1)) */
      int a = GETARG_A(i);
      int n = GETARG_C(i);
      struct RProc *m;
      struct RClass *c;
      mrb_callinfo *ci;
      mrb_value recv;
      mrb_sym mid = syms[GETARG_B(i)];

      recv = regs[a];
      c = mrb_class(mrb, recv);
      m = mrb_method_search_vm(mrb, &c, mid);
      if (!m) {
        mrb_value sym = mrb_symbol_value(mid);
        mrb_sym missing = mrb_intern_lit(mrb, "method_missing");
        m = mrb_method_search_vm(mrb, &c, missing);
        if (!m) {
          mrb_value args;

          if (n == CALL_PASS_BY_ARRAY) {
            args = regs[a+1];
          }
          else {
            args = mrb_ary_new_from_values(mrb, n, regs+a+1);
          }
          mrb_method_missing(mrb, mid, recv, args);
        }
        mid = missing;
        if (n == CALL_PASS_BY_ARRAY) {
          mrb_ary_unshift(mrb, regs[a+1], sym);
        }
        else {
          value_move(regs+a+2, regs+a+1, ++n);
          regs[a+1] = sym;
        }
      }

      /* replace callinfo */
      ci = mrb->c->ci;
      ci->mid = mid;
      ci->target_class = c;
      if (n == CALL_PASS_BY_ARRAY) {
        ci->argc = ARGS_PASS_BY_ARRAY;
      }
      else {
        ci->argc = n;
      }

      /* move stack */
      value_move(mrb->c->stack, &regs[a], ci->argc+1);

      if (MRB_PROC_CFUNC_P(m)) {
        mrb_value v = m->body.func(mrb, recv);
        mrb->c->stack[0] = v;
        mrb_gc_arena_restore(mrb, ai);
        goto L_RETURN;
      }
      else {
        /* setup environment for calling method */
        UPDATE_IREP(m->body.irep);
        pc = irep->iseq;
      }
      JUMP;
    }

    CASE(OP_BLKPUSH) {
      /* A Bx   R(A) := block (16=6:1:5:4) */
      int a = GETARG_A(i);
      int bx = GETARG_Bx(i);
      int m1 = (bx>>10)&0x3f;
      int r  = (bx>>9)&0x1;
      int m2 = (bx>>4)&0x1f;
      int lv = (bx>>0)&0xf;
      mrb_value *stack;

      if (lv == 0) stack = regs + 1;
      else {
        struct REnv *e = uvenv(mrb, lv-1);
        if (!e || !e->target_ci->ret_ci ||
            (!MRB_ENV_STACK_SHARED_P(e) && e->cxt.mid == 0)) {
          localjump_error(mrb, LOCALJUMP_ERROR_YIELD);
          goto L_RAISE;
        }
        stack = e->stack + 1;
      }
      if (mrb_nil_p(stack[m1+r+m2])) {
        localjump_error(mrb, LOCALJUMP_ERROR_YIELD);
        goto L_RAISE;
      }
      regs[a] = stack[m1+r+m2];
      NEXT;
    }

#define TYPES2(a,b) ((((uint16_t)(a))<<8)|(((uint16_t)(b))&0xff))
#define OP_MATH_BODY(op,v1,v2) do {\
  v1(regs[a]) = v1(regs[a]) op v2(regs[a+1]);\
} while(0)

    CASE(OP_ADD) {
      /* A B C  R(A) := R(A)+R(A+1) (Syms[B]=:+,C=1)*/
      int a = GETARG_A(i);

      /* need to check if op is overridden */
      switch (TYPES2(mrb_type(regs[a]),mrb_type(regs[a+1]))) {
      case TYPES2(MRB_TT_FIXNUM,MRB_TT_FIXNUM):
        {
          mrb_int x, y, z;
          mrb_value *regs_a = regs + a;

          x = mrb_fixnum(regs_a[0]);
          y = mrb_fixnum(regs_a[1]);
          if (mrb_int_add_overflow(x, y, &z)) {
            SET_FLOAT_VALUE(mrb, regs_a[0], (mrb_float)x + (mrb_float)y);
            break;
          }
          SET_INT_VALUE(regs[a], z);
        }
        break;
      case TYPES2(MRB_TT_FIXNUM,MRB_TT_FLOAT):
        {
          mrb_int x = mrb_fixnum(regs[a]);
          mrb_float y = mrb_float(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], (mrb_float)x + y);
        }
        break;
      case TYPES2(MRB_TT_FLOAT,MRB_TT_FIXNUM):
#ifdef MRB_WORD_BOXING
        {
          mrb_float x = mrb_float(regs[a]);
          mrb_int y = mrb_fixnum(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], x + y);
        }
#else
        OP_MATH_BODY(+,mrb_float,mrb_fixnum);
#endif
        break;
      case TYPES2(MRB_TT_FLOAT,MRB_TT_FLOAT):
#ifdef MRB_WORD_BOXING
        {
          mrb_float x = mrb_float(regs[a]);
          mrb_float y = mrb_float(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], x + y);
        }
#else
        OP_MATH_BODY(+,mrb_float,mrb_float);
#endif
        break;
      case TYPES2(MRB_TT_STRING,MRB_TT_STRING):
        regs[a] = mrb_str_plus(mrb, regs[a], regs[a+1]);
        break;
      default:
        goto L_SEND;
      }
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_SUB) {
      /* A B C  R(A) := R(A)-R(A+1) (Syms[B]=:-,C=1)*/
      int a = GETARG_A(i);

      /* need to check if op is overridden */
      switch (TYPES2(mrb_type(regs[a]),mrb_type(regs[a+1]))) {
      case TYPES2(MRB_TT_FIXNUM,MRB_TT_FIXNUM):
        {
          mrb_int x, y, z;

          x = mrb_fixnum(regs[a]);
          y = mrb_fixnum(regs[a+1]);
          if (mrb_int_sub_overflow(x, y, &z)) {
            SET_FLOAT_VALUE(mrb, regs[a], (mrb_float)x - (mrb_float)y);
            break;
          }
          SET_INT_VALUE(regs[a], z);
        }
        break;
      case TYPES2(MRB_TT_FIXNUM,MRB_TT_FLOAT):
        {
          mrb_int x = mrb_fixnum(regs[a]);
          mrb_float y = mrb_float(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], (mrb_float)x - y);
        }
        break;
      case TYPES2(MRB_TT_FLOAT,MRB_TT_FIXNUM):
#ifdef MRB_WORD_BOXING
        {
          mrb_float x = mrb_float(regs[a]);
          mrb_int y = mrb_fixnum(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], x - y);
        }
#else
        OP_MATH_BODY(-,mrb_float,mrb_fixnum);
#endif
        break;
      case TYPES2(MRB_TT_FLOAT,MRB_TT_FLOAT):
#ifdef MRB_WORD_BOXING
        {
          mrb_float x = mrb_float(regs[a]);
          mrb_float y = mrb_float(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], x - y);
        }
#else
        OP_MATH_BODY(-,mrb_float,mrb_float);
#endif
        break;
      default:
        goto L_SEND;
      }
      NEXT;
    }

    CASE(OP_MUL) {
      /* A B C  R(A) := R(A)*R(A+1) (Syms[B]=:*,C=1)*/
      int a = GETARG_A(i);

      /* need to check if op is overridden */
      switch (TYPES2(mrb_type(regs[a]),mrb_type(regs[a+1]))) {
      case TYPES2(MRB_TT_FIXNUM,MRB_TT_FIXNUM):
        {
          mrb_int x, y, z;

          x = mrb_fixnum(regs[a]);
          y = mrb_fixnum(regs[a+1]);
          if (mrb_int_mul_overflow(x, y, &z)) {
            SET_FLOAT_VALUE(mrb, regs[a], (mrb_float)x * (mrb_float)y);
            break;
          }
          SET_INT_VALUE(regs[a], z);
        }
        break;
      case TYPES2(MRB_TT_FIXNUM,MRB_TT_FLOAT):
        {
          mrb_int x = mrb_fixnum(regs[a]);
          mrb_float y = mrb_float(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], (mrb_float)x * y);
        }
        break;
      case TYPES2(MRB_TT_FLOAT,MRB_TT_FIXNUM):
#ifdef MRB_WORD_BOXING
        {
          mrb_float x = mrb_float(regs[a]);
          mrb_int y = mrb_fixnum(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], x * y);
        }
#else
        OP_MATH_BODY(*,mrb_float,mrb_fixnum);
#endif
        break;
      case TYPES2(MRB_TT_FLOAT,MRB_TT_FLOAT):
#ifdef MRB_WORD_BOXING
        {
          mrb_float x = mrb_float(regs[a]);
          mrb_float y = mrb_float(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], x * y);
        }
#else
        OP_MATH_BODY(*,mrb_float,mrb_float);
#endif
        break;
      default:
        goto L_SEND;
      }
      NEXT;
    }

    CASE(OP_DIV) {
      /* A B C  R(A) := R(A)/R(A+1) (Syms[B]=:/,C=1)*/
      int a = GETARG_A(i);

      /* need to check if op is overridden */
      switch (TYPES2(mrb_type(regs[a]),mrb_type(regs[a+1]))) {
      case TYPES2(MRB_TT_FIXNUM,MRB_TT_FIXNUM):
        {
          mrb_int x = mrb_fixnum(regs[a]);
          mrb_int y = mrb_fixnum(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], (mrb_float)x / (mrb_float)y);
        }
        break;
      case TYPES2(MRB_TT_FIXNUM,MRB_TT_FLOAT):
        {
          mrb_int x = mrb_fixnum(regs[a]);
          mrb_float y = mrb_float(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], (mrb_float)x / y);
        }
        break;
      case TYPES2(MRB_TT_FLOAT,MRB_TT_FIXNUM):
#ifdef MRB_WORD_BOXING
        {
          mrb_float x = mrb_float(regs[a]);
          mrb_int y = mrb_fixnum(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], x / y);
        }
#else
        OP_MATH_BODY(/,mrb_float,mrb_fixnum);
#endif
        break;
      case TYPES2(MRB_TT_FLOAT,MRB_TT_FLOAT):
#ifdef MRB_WORD_BOXING
        {
          mrb_float x = mrb_float(regs[a]);
          mrb_float y = mrb_float(regs[a+1]);
          SET_FLOAT_VALUE(mrb, regs[a], x / y);
        }
#else
        OP_MATH_BODY(/,mrb_float,mrb_float);
#endif
        break;
      default:
        goto L_SEND;
      }
#ifdef MRB_NAN_BOXING
      if (isnan(mrb_float(regs[a]))) {
        mrb_value v = mrb_float_value(mrb, mrb_float(regs[a]));
        regs[a] = v;
      }
#endif
      NEXT;
    }

    CASE(OP_ADDI) {
      /* A B C  R(A) := R(A)+C (Syms[B]=:+)*/
      int a = GETARG_A(i);

      /* need to check if + is overridden */
      switch (mrb_type(regs[a])) {
      case MRB_TT_FIXNUM:
        {
          mrb_int x = mrb_fixnum(regs[a]);
          mrb_int y = GETARG_C(i);
          mrb_int z;

          if (mrb_int_add_overflow(x, y, &z)) {
            SET_FLOAT_VALUE(mrb, regs[a], (mrb_float)x + (mrb_float)y);
            break;
          }
          SET_INT_VALUE(regs[a], z);
        }
        break;
      case MRB_TT_FLOAT:
#ifdef MRB_WORD_BOXING
        {
          mrb_float x = mrb_float(regs[a]);
          SET_FLOAT_VALUE(mrb, regs[a], x + GETARG_C(i));
        }
#else
        mrb_float(regs[a]) += GETARG_C(i);
#endif
        break;
      default:
        SET_INT_VALUE(regs[a+1], GETARG_C(i));
        i = MKOP_ABC(OP_SEND, a, GETARG_B(i), 1);
        goto L_SEND;
      }
      NEXT;
    }

    CASE(OP_SUBI) {
      /* A B C  R(A) := R(A)-C (Syms[B]=:-)*/
      int a = GETARG_A(i);
      mrb_value *regs_a = regs + a;

      /* need to check if + is overridden */
      switch (mrb_type(regs_a[0])) {
      case MRB_TT_FIXNUM:
        {
          mrb_int x = mrb_fixnum(regs_a[0]);
          mrb_int y = GETARG_C(i);
          mrb_int z;

          if (mrb_int_sub_overflow(x, y, &z)) {
            SET_FLOAT_VALUE(mrb, regs_a[0], (mrb_float)x - (mrb_float)y);
          }
          else {
            SET_INT_VALUE(regs_a[0], z);
          }
        }
        break;
      case MRB_TT_FLOAT:
#ifdef MRB_WORD_BOXING
        {
          mrb_float x = mrb_float(regs[a]);
          SET_FLOAT_VALUE(mrb, regs[a], x - GETARG_C(i));
        }
#else
        mrb_float(regs_a[0]) -= GETARG_C(i);
#endif
        break;
      default:
        SET_INT_VALUE(regs_a[1], GETARG_C(i));
        i = MKOP_ABC(OP_SEND, a, GETARG_B(i), 1);
        goto L_SEND;
      }
      NEXT;
    }

#define OP_CMP_BODY(op,v1,v2) (v1(regs[a]) op v2(regs[a+1]))

#define OP_CMP(op) do {\
  int result;\
  /* need to check if - is overridden */\
  switch (TYPES2(mrb_type(regs[a]),mrb_type(regs[a+1]))) {\
  case TYPES2(MRB_TT_FIXNUM,MRB_TT_FIXNUM):\
    result = OP_CMP_BODY(op,mrb_fixnum,mrb_fixnum);\
    break;\
  case TYPES2(MRB_TT_FIXNUM,MRB_TT_FLOAT):\
    result = OP_CMP_BODY(op,mrb_fixnum,mrb_float);\
    break;\
  case TYPES2(MRB_TT_FLOAT,MRB_TT_FIXNUM):\
    result = OP_CMP_BODY(op,mrb_float,mrb_fixnum);\
    break;\
  case TYPES2(MRB_TT_FLOAT,MRB_TT_FLOAT):\
    result = OP_CMP_BODY(op,mrb_float,mrb_float);\
    break;\
  default:\
    goto L_SEND;\
  }\
  if (result) {\
    SET_TRUE_VALUE(regs[a]);\
  }\
  else {\
    SET_FALSE_VALUE(regs[a]);\
  }\
} while(0)

    CASE(OP_EQ) {
      /* A B C  R(A) := R(A)==R(A+1) (Syms[B]=:==,C=1)*/
      int a = GETARG_A(i);
      if (mrb_obj_eq(mrb, regs[a], regs[a+1])) {
        SET_TRUE_VALUE(regs[a]);
      }
      else {
        OP_CMP(==);
      }
      NEXT;
    }

    CASE(OP_LT) {
      /* A B C  R(A) := R(A)<R(A+1) (Syms[B]=:<,C=1)*/
      int a = GETARG_A(i);
      OP_CMP(<);
      NEXT;
    }

    CASE(OP_LE) {
      /* A B C  R(A) := R(A)<=R(A+1) (Syms[B]=:<=,C=1)*/
      int a = GETARG_A(i);
      OP_CMP(<=);
      NEXT;
    }

    CASE(OP_GT) {
      /* A B C  R(A) := R(A)>R(A+1) (Syms[B]=:>,C=1)*/
      int a = GETARG_A(i);
      OP_CMP(>);
      NEXT;
    }

    CASE(OP_GE) {
      /* A B C  R(A) := R(A)>=R(A+1) (Syms[B]=:>=,C=1)*/
      int a = GETARG_A(i);
      OP_CMP(>=);
      NEXT;
    }

    CASE(OP_ARRAY) {
      /* A B C          R(A) := ary_new(R(B),R(B+1)..R(B+C)) */
      mrb_value v = mrb_ary_new_from_values(mrb, GETARG_C(i), &regs[GETARG_B(i)]);
      regs[GETARG_A(i)] = v;
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_ARYCAT) {
      /* A B            mrb_ary_concat(R(A),R(B)) */
      mrb_value splat = mrb_ary_splat(mrb, regs[GETARG_B(i)]);
      mrb_ary_concat(mrb, regs[GETARG_A(i)], splat);
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_ARYPUSH) {
      /* A B            R(A).push(R(B)) */
      mrb_ary_push(mrb, regs[GETARG_A(i)], regs[GETARG_B(i)]);
      NEXT;
    }

    CASE(OP_AREF) {
      /* A B C          R(A) := R(B)[C] */
      int a = GETARG_A(i);
      int c = GETARG_C(i);
      mrb_value v = regs[GETARG_B(i)];

      if (!mrb_array_p(v)) {
        if (c == 0) {
          regs[GETARG_A(i)] = v;
        }
        else {
          SET_NIL_VALUE(regs[a]);
        }
      }
      else {
        v = mrb_ary_ref(mrb, v, c);
        regs[GETARG_A(i)] = v;
      }
      NEXT;
    }

    CASE(OP_ASET) {
      /* A B C          R(B)[C] := R(A) */
      mrb_ary_set(mrb, regs[GETARG_B(i)], GETARG_C(i), regs[GETARG_A(i)]);
      NEXT;
    }

    CASE(OP_APOST) {
      /* A B C  *R(A),R(A+1)..R(A+C) := R(A) */
      int a = GETARG_A(i);
      mrb_value v = regs[a];
      int pre  = GETARG_B(i);
      int post = GETARG_C(i);
      struct RArray *ary;
      int len, idx;

      if (!mrb_array_p(v)) {
        v = mrb_ary_new_from_values(mrb, 1, &regs[a]);
      }
      ary = mrb_ary_ptr(v);
      len = ary->len;
      if (len > pre + post) {
        regs[a++] = mrb_ary_new_from_values(mrb, len - pre - post, ary->ptr+pre);
        while (post--) {
          regs[a++] = ary->ptr[len-post-1];
        }
      }
      else {
        regs[a++] = mrb_ary_new_capa(mrb, 0);
        for (idx=0; idx+pre<len; idx++) {
          regs[a+idx] = ary->ptr[pre+idx];
        }
        while (idx < post) {
          SET_NIL_VALUE(regs[a+idx]);
          idx++;
        }
      }
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_STRING) {
      /* A Bx           R(A) := str_new(Lit(Bx)) */
      mrb_value str = mrb_str_dup(mrb, pool[GETARG_Bx(i)]);
      regs[GETARG_A(i)] = str;
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_STRCAT) {
      /* A B    R(A).concat(R(B)) */
      mrb_str_concat(mrb, regs[GETARG_A(i)], regs[GETARG_B(i)]);
      NEXT;
    }

    CASE(OP_HASH) {
      /* A B C   R(A) := hash_new(R(B),R(B+1)..R(B+C)) */
      int b = GETARG_B(i);
      int c = GETARG_C(i);
      int lim = b+c*2;
      mrb_value hash = mrb_hash_new_capa(mrb, c);

      while (b < lim) {
        mrb_hash_set(mrb, hash, regs[b], regs[b+1]);
        b+=2;
      }
      regs[GETARG_A(i)] = hash;
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_LAMBDA) {
      /* A b c  R(A) := lambda(SEQ[b],c) (b:c = 14:2) */
      struct RProc *p;
      int c = GETARG_c(i);

      if (c & OP_L_CAPTURE) {
        p = mrb_closure_new(mrb, irep->reps[GETARG_b(i)]);
      }
      else {
        p = mrb_proc_new(mrb, irep->reps[GETARG_b(i)]);
      }
      if (c & OP_L_STRICT) p->flags |= MRB_PROC_STRICT;
      regs[GETARG_A(i)] = mrb_obj_value(p);
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_OCLASS) {
      /* A      R(A) := ::Object */
      regs[GETARG_A(i)] = mrb_obj_value(mrb->object_class);
      NEXT;
    }

    CASE(OP_CLASS) {
      /* A B    R(A) := newclass(R(A),Syms(B),R(A+1)) */
      struct RClass *c = 0, *baseclass;
      int a = GETARG_A(i);
      mrb_value base, super;
      mrb_sym id = syms[GETARG_B(i)];

      base = regs[a];
      super = regs[a+1];
      if (mrb_nil_p(base)) {
        baseclass = mrb->c->ci->proc->target_class;
        if (!baseclass) baseclass = mrb->c->ci->target_class;

        base = mrb_obj_value(baseclass);
      }
      c = mrb_vm_define_class(mrb, base, super, id);
      regs[a] = mrb_obj_value(c);
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_MODULE) {
      /* A B            R(A) := newmodule(R(A),Syms(B)) */
      struct RClass *c = 0, *baseclass;
      int a = GETARG_A(i);
      mrb_value base;
      mrb_sym id = syms[GETARG_B(i)];

      base = regs[a];
      if (mrb_nil_p(base)) {
        baseclass = mrb->c->ci->proc->target_class;
        if (!baseclass) baseclass = mrb->c->ci->target_class;

        base = mrb_obj_value(baseclass);
      }
      c = mrb_vm_define_module(mrb, base, id);
      regs[a] = mrb_obj_value(c);
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_EXEC) {
      /* A Bx   R(A) := blockexec(R(A),SEQ[Bx]) */
      int a = GETARG_A(i);
      mrb_callinfo *ci;
      mrb_value recv = regs[a];
      struct RProc *p;

      /* prepare closure */
      p = mrb_closure_new(mrb, irep->reps[GETARG_Bx(i)]);
      p->target_class = mrb_class_ptr(recv);

      mrb_assert(!MRB_PROC_CFUNC_P(p));

      /* prepare stack */
      ci = cipush(mrb, p->target_class, 0, p, FALSE);
      ci->pc = pc + 1;
      ci->acc = a;
      ci->argc = 0;
      regs[0] = recv;

      UPDATE_IREP(p->body.irep);
      pc = irep->iseq;
      JUMP;
    }

    CASE(OP_METHOD) {
      /* A B            R(A).newmethod(Syms(B),R(A+1)) */
      int a = GETARG_A(i);
      struct RClass *c = mrb_class_ptr(regs[a]);
      struct RProc *p = mrb_proc_ptr(regs[a+1]);

      // fprintf(stderr, "%s %s\n", mrb_class_name(mrb, c), mrb_sym2name(mrb, syms[GETARG_B(i)]));

      mrb_define_method_raw(mrb, c, syms[GETARG_B(i)], p);
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_SCLASS) {
      /* A B    R(A) := R(B).singleton_class */
      regs[GETARG_A(i)] = mrb_singleton_class(mrb, regs[GETARG_B(i)]);
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_TCLASS) {
      /* A      R(A) := target_class */
      if (!mrb->c->ci->target_class) {
        mrb_value exc = mrb_exc_new_str_lit(mrb, E_TYPE_ERROR, "no target class or module");
        mrb_exc_set(mrb, exc);
        goto L_RAISE;
      }
      regs[GETARG_A(i)] = mrb_obj_value(mrb->c->ci->target_class);
      NEXT;
    }

    CASE(OP_RANGE) {
      /* A B C  R(A) := range_new(R(B),R(B+1),C) */
      int b = GETARG_B(i);
      mrb_value val = mrb_range_new(mrb, regs[b], regs[b+1], GETARG_C(i));
      regs[GETARG_A(i)] = val;
      ARENA_RESTORE(mrb, ai);
      NEXT;
    }

    CASE(OP_DEBUG) {
      /* A B C    debug print R(A),R(B),R(C) */
#ifdef MRB_ENABLE_DEBUG_HOOK
      mrb->debug_op_hook(mrb, irep, pc, regs);
#else
#ifndef MRB_DISABLE_STDIO
      printf("OP_DEBUG %d %d %d\n", GETARG_A(i), GETARG_B(i), GETARG_C(i));
#else
      abort();
#endif
#endif
      NEXT;
    }

    CASE(OP_STOP) {
      /*        stop VM */
    L_STOP:
      {
        ecall_current_ci(mrb);
      }
      ERR_PC_CLR(mrb);
      mrb->jmp = prev_jmp;
      if (mrb->exc) {
        return mrb_obj_value(mrb->exc);
      }
      return regs[irep->nlocals];
    }

    CASE(OP_ERR) {
      /* Bx     raise RuntimeError with message Lit(Bx) */
      mrb_value msg = mrb_str_dup(mrb, pool[GETARG_Bx(i)]);
      mrb_value exc;

      if (GETARG_A(i) == 0) {
        exc = mrb_exc_new_str(mrb, E_RUNTIME_ERROR, msg);
      }
      else {
        exc = mrb_exc_new_str(mrb, E_LOCALJUMP_ERROR, msg);
      }
      mrb_exc_set(mrb, exc);
      goto L_RAISE;
    }
  }
  END_DISPATCH;
#undef regs

  }
  MRB_CATCH(&c_jmp) {
    exc_catched = TRUE;
    goto RETRY_TRY_BLOCK;
  }
  MRB_END_EXC(&c_jmp);
}

MRB_API mrb_value
mrb_run(mrb_state *mrb, struct RProc *proc, mrb_value self)
{
  if (ARGS_PASS_BY_ARRAY_P(mrb->c->ci->argc)) {
    return mrb_vm_run(mrb, proc, self, 3); /* receiver, args and block) */
  }
  else {
    return mrb_vm_run(mrb, proc, self, mrb->c->ci->argc + 2); /* argc + 2 (receiver and block) */
  }
}

MRB_API mrb_value
mrb_top_run(mrb_state *mrb, struct RProc *proc, mrb_value self, unsigned int stack_keep)
{
  mrb_callinfo *ci;
  mrb_value v;

  if (mrb->c->ci) {
    return mrb_vm_run(mrb, proc, self, stack_keep);
  }
  ci = cipush(mrb, mrb->object_class, 0, proc, FALSE);
  ci->acc = CI_ACC_SKIP;
  ci->argc = 0;
  v = mrb_vm_run(mrb, proc, self, stack_keep);
  cipop(mrb);

  return v;
}

#if defined(MRB_ENABLE_CXX_EXCEPTION) && defined(__cplusplus)
# if !defined(MRB_ENABLE_CXX_ABI)
} /* end of extern "C" */
# endif
mrb_int mrb_jmpbuf::jmpbuf_id = 0;
# if !defined(MRB_ENABLE_CXX_ABI)
extern "C" {
# endif
#endif

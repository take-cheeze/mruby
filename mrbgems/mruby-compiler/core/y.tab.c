/* A Bison parser, made by Lrama 0.7.0.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* First part of user prologue.  */
#line 7 "mrbgems/mruby-compiler/core/parse.y"

#undef PARSER_DEBUG
#ifdef PARSER_DEBUG
# define YYDEBUG 1
#endif
#define YYSTACK_USE_ALLOCA 1

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/proc.h>
#include <mruby/error.h>
#include <mruby/throw.h>
#include <mruby/string.h>
#include <mruby/dump.h>
#include <mruby/internal.h>
#include "node.h"

#define YYLEX_PARAM p

#define mrbc_malloc(s) mrb_basic_alloc_func(NULL,(s))
#define mrbc_realloc(p,s) mrb_basic_alloc_func((p),(s))
#define mrbc_free(p) mrb_basic_alloc_func((p),0)

typedef mrb_ast_node node;
typedef struct mrb_parser_state parser_state;
typedef struct mrb_parser_heredoc_info parser_heredoc_info;

static int yyparse(parser_state *p);
static int yylex(void *lval, void *lp, parser_state *p);
static void yyerror(void *lp, parser_state *p, const char *s);
static void yywarning(parser_state *p, const char *s);
static void backref_error(parser_state *p, node *n);
static void void_expr_error(parser_state *p, node *n);
static void tokadd(parser_state *p, int32_t c);
static const char* tok(parser_state *p);
static int toklen(parser_state *p);

/* Forward declarations for variable-sized simple node functions */

/* Forward declarations for variable-sized advanced node functions */

/* Helper function to check node type for both traditional and variable-sized nodes */
static mrb_bool node_type_p(node *n, enum node_type type);

#define identchar(c) (ISALNUM(c) || (c) == '_' || !ISASCII(c))

typedef unsigned int stack_type;

#define BITSTACK_PUSH(stack, n) ((stack) = ((stack)<<1)|((n)&1))
#define BITSTACK_POP(stack)     ((stack) = (stack) >> 1)
#define BITSTACK_LEXPOP(stack)  ((stack) = ((stack) >> 1) | ((stack) & 1))
#define BITSTACK_SET_P(stack)   ((stack)&1)

#define COND_PUSH(n)    BITSTACK_PUSH(p->cond_stack, (n))
#define COND_POP()      BITSTACK_POP(p->cond_stack)
#define COND_LEXPOP()   BITSTACK_LEXPOP(p->cond_stack)
#define COND_P()        BITSTACK_SET_P(p->cond_stack)

#define CMDARG_PUSH(n)  BITSTACK_PUSH(p->cmdarg_stack, (n))
#define CMDARG_POP()    BITSTACK_POP(p->cmdarg_stack)
#define CMDARG_LEXPOP() BITSTACK_LEXPOP(p->cmdarg_stack)
#define CMDARG_P()      BITSTACK_SET_P(p->cmdarg_stack)

#define SET_LINENO(c,n) (((struct mrb_ast_var_header*)(c))->lineno = (n))

#define NUM_SUFFIX_R   (1<<0)
#define NUM_SUFFIX_I   (1<<1)

static inline mrb_sym
intern_cstr_gen(parser_state *p, const char *s)
{
  return mrb_intern_cstr(p->mrb, s);
}
#define intern_cstr(s) intern_cstr_gen(p,(s))

static inline mrb_sym
intern_gen(parser_state *p, const char *s, size_t len)
{
  return mrb_intern(p->mrb, s, len);
}
#define intern(s,len) intern_gen(p,(s),(len))

#define intern_op(op) MRB_OPSYM(op)

static mrb_sym
intern_numparam_gen(parser_state *p, int num)
{
  char buf[3];
  buf[0] = '_'; buf[1] = '0'+num; buf[2] = '\0';
  return intern(buf, 2);
}
#define intern_numparam(n) intern_numparam_gen(p,(n))

static void
cons_free_gen(parser_state *p, node *cons)
{
  cons->cdr = p->cells;
  p->cells = cons;
}
#define cons_free(c) cons_free_gen(p, (c))

static void*
parser_palloc(parser_state *p, size_t size)
{
  void *m = mempool_alloc(p->pool, size);

  if (!m) {
    MRB_THROW(p->mrb->jmp);
  }
  return m;
}

#define parser_pfree(ptr) do { if (sizeof(node) <= sizeof(*(ptr))) cons_free((node*)ptr);} while (0)

static node*
cons_gen(parser_state *p, node *car, node *cdr)
{
  struct mrb_ast_node *c;

  /* Try to reuse from free list first - only for 16-byte nodes */
  if (p->cells) {
    c = (struct mrb_ast_node*)p->cells;
    p->cells = p->cells->cdr;
  }
  else {
    c = (struct mrb_ast_node*)parser_palloc(p, sizeof(struct mrb_ast_node));
  }
  c->car = car;
  c->cdr = cdr;
  /* Don't initialize location fields for structure nodes - saves CPU */
  return (node*)c;
}

/* Head-only location optimization: separate functions for head vs structure nodes */
#define cons(a,b) cons_gen(p,(a),(b))           /* Structure nodes - no location */
/* Initialize variable node header */
static void
init_var_header(struct mrb_ast_var_header *header, parser_state *p, enum node_type type)
{
  header->lineno = p->lineno;
  header->filename_index = p->current_filename_index;
  header->node_type = (uint8_t)type;

  /* Handle file boundary edge case */
  if (p->lineno == 0 && p->current_filename_index > 0) {
    header->filename_index--;
  }
}

/* Combined allocate + init header helper */
static inline void*
new_node(parser_state *p, size_t size, enum node_type type)
{
  void *n = parser_palloc(p, size);
  init_var_header((struct mrb_ast_var_header*)n, p, type);
  return n;
}

/* Type-safe macro wrapper for node allocation */
#define NEW_NODE(type_name, node_type) \
  (struct mrb_ast_##type_name##_node*)new_node(p, sizeof(struct mrb_ast_##type_name##_node), node_type)

static node*
list1_gen(parser_state *p, node *a)
{
  return cons(a, 0);
}
#define list1(a) list1_gen(p, (a))

static node*
list2_gen(parser_state *p, node *a, node *b)
{
  return cons(a, cons(b, 0));
}
#define list2(a,b) list2_gen(p, (a),(b))

static node*
list3_gen(parser_state *p, node *a, node *b, node *c)
{
  return cons(a, cons(b, cons(c, 0)));
}
#define list3(a,b,c) list3_gen(p, (a),(b),(c))

static node*
append_gen(parser_state *p, node *a, node *b)
{
  node *c = a;

  if (!a) return b;
  if (!b) return a;
  while (c->cdr) {
    c = c->cdr;
  }
  c->cdr = b;
  return a;
}
#define append(a,b) append_gen(p,(a),(b))
#define push(a,b) append_gen(p,(a),list1(b))

static char*
parser_strndup(parser_state *p, const char *s, size_t len)
{
  char *b = (char*)parser_palloc(p, len+1);

  memcpy(b, s, len);
  b[len] = '\0';
  return b;
}
#undef strndup
#define strndup(s,len) parser_strndup(p, s, len)

static char*
parser_strdup(parser_state *p, const char *s)
{
  return parser_strndup(p, s, strlen(s));
}
#undef strdup
#define strdup(s) parser_strdup(p, s)

static void
dump_int(uint16_t i, char *s)
{
  char *p = s;
  char *t = s;

  while (i > 0) {
    *p++ = (i % 10)+'0';
    i /= 10;
  }
  if (p == s) *p++ = '0';
  *p = 0;
  p--;  /* point the last char */
  while (t < p) {
    char c = *t;
    *t++ = *p;
    *p-- = c;
  }
}

/* xxx ----------------------------- */

static node*
local_switch(parser_state *p)
{
  node *prev = p->locals;

  p->locals = cons(0, 0);
  return prev;
}

static void
local_resume(parser_state *p, node *prev)
{
  p->locals = prev;
}

static void
local_nest(parser_state *p)
{
  p->locals = cons(0, p->locals);
}

static void
local_unnest(parser_state *p)
{
  if (p->locals) {
    p->locals = p->locals->cdr;
  }
}

static mrb_bool
local_var_p(parser_state *p, mrb_sym sym)
{
  const struct RProc *u;
  node *l = p->locals;

  while (l) {
    node *n = l->car;
    while (n) {
      if (node_to_sym(n->car) == sym) return TRUE;
      n = n->cdr;
    }
    l = l->cdr;
  }

  u = p->upper;
  while (u && !MRB_PROC_CFUNC_P(u)) {
    const struct mrb_irep *ir = u->body.irep;
    const mrb_sym *v = ir->lv;
    int i;

    if (v) {
      for (i=0; i+1 < ir->nlocals; i++) {
        if (v[i] == sym) return TRUE;
      }
    }
    if (MRB_PROC_SCOPE_P(u)) break;
    u = u->upper;
  }
  return FALSE;
}

static void
local_add_f(parser_state *p, mrb_sym sym)
{
  if (p->locals) {
    node *n = p->locals->car;
    while (n) {
      if (node_to_sym(n->car) == sym) {
        mrb_int len;
        const char* name = mrb_sym_name_len(p->mrb, sym, &len);
        if (len > 0 && name[0] != '_') {
          yyerror(NULL, p, "duplicated argument name");
          return;
        }
      }
      n = n->cdr;
    }
    p->locals->car = push(p->locals->car, sym_to_node(sym));
  }
}

static void
local_add(parser_state *p, mrb_sym sym)
{
  if (!local_var_p(p, sym)) {
    local_add_f(p, sym);
  }
}

/* allocate register for block */
#define local_add_blk(p) local_add_f(p, 0)

static void
local_add_kw(parser_state *p, mrb_sym kwd)
{
  /* allocate register for keywords hash */
  local_add_f(p, kwd ? kwd : intern_op(pow));
}

static node*
locals_node(parser_state *p)
{
  return p->locals ? p->locals->car : NULL;
}

/* Helper function to check node type for both traditional and variable-sized nodes */
static mrb_bool
node_type_p(node *n, enum node_type type)
{
  if (!n) return FALSE;

  /* Check if this is a variable-sized node */
  struct mrb_ast_var_header *header = (struct mrb_ast_var_header*)n;
  return ((enum node_type)header->node_type == type);
}

/* Helper functions for variable-sized node detection */
static enum node_type
node_type(node *n)
{
  if (!n) return (enum node_type)0;

  /* Try to interpret as variable-sized node */
  struct mrb_ast_var_header *header = (struct mrb_ast_var_header*)n;
  enum node_type type = (enum node_type)header->node_type;

  /* Validate that the node type is within valid range for variable-sized nodes */
  if (type >= NODE_SCOPE && type < NODE_LAST) {
    return type;
  }

  /* If node type is invalid, this is likely a cons-list node */
  /* Return a special sentinel value to indicate cons-list fallback */
  return NODE_LAST; /* Use NODE_LAST as sentinel for cons-list nodes */
}

static void
nvars_nest(parser_state *p)
{
  p->nvars = cons(int_to_node(0), p->nvars);
}

static void
nvars_block(parser_state *p)
{
  p->nvars = cons(int_to_node(-2), p->nvars);
}

static void
nvars_unnest(parser_state *p)
{
  p->nvars = p->nvars->cdr;
}

/* struct: scope_node(locals, body) */
static node*
new_scope(parser_state *p, node *body)
{
  struct mrb_ast_scope_node *scope_node = NEW_NODE(scope, NODE_SCOPE);
  scope_node->locals = locals_node(p);
  scope_node->body = body;
  return (node*)scope_node;
}

/* struct: stmts_node(stmts) - uses cons list */
static node*
new_stmts(parser_state *p, node *body)
{
  struct mrb_ast_stmts_node *n = NEW_NODE(stmts, NODE_STMTS);
  n->stmts = body ? list1(body) : 0;  /* Wrap single statement in cons-list */

  return (node*)n;
}

/* Helper: push statement to stmts node */
static node*
stmts_push(parser_state *p, node *stmts, node *stmt)
{
  struct mrb_ast_stmts_node *n = stmts_node(stmts);
  n->stmts = push(n->stmts, stmt);
  return stmts;
}

/* struct: begin_node(body) */
static node*
new_begin(parser_state *p, node *body)
{
  struct mrb_ast_begin_node *begin_node = NEW_NODE(begin, NODE_BEGIN);
  begin_node->body = body;
  return (node*)begin_node;
}

#define newline_node(n) (n)

/* struct: rescue_node(body, rescue_clauses, else_clause) */
static node*
new_rescue(parser_state *p, node *body, node *resq, node *els)
{
  struct mrb_ast_rescue_node *n = NEW_NODE(rescue, NODE_RESCUE);
  n->body = body;
  n->rescue_clauses = resq;
  n->else_clause = els;

  return (node*)n;
}

static node*
new_mod_rescue(parser_state *p, node *body, node *resq)
{
  return new_rescue(p, body, list1(list3(0, 0, resq)), 0);
}

/* struct: ensure_node(body, ensure_clause) */
static node*
new_ensure(parser_state *p, node *a, node *b)
{
  struct mrb_ast_ensure_node *ensure_node = NEW_NODE(ensure, NODE_ENSURE);
  ensure_node->body = a;
  ensure_node->ensure_clause = b;
  return (node*)ensure_node;
}

/* struct: nil_node() */
static node*
new_nil(parser_state *p)
{
  struct mrb_ast_nil_node *n = NEW_NODE(nil, NODE_NIL);

  return (node*)n;
}

/* struct: true_node() */
static node*
new_true(parser_state *p)
{
  struct mrb_ast_true_node *n = NEW_NODE(true, NODE_TRUE);

  return (node*)n;
}

/* struct: false_node() */
static node*
new_false(parser_state *p)
{
  struct mrb_ast_false_node *n = NEW_NODE(false, NODE_FALSE);

  return (node*)n;
}

/* struct: alias_node(new_name, old_name) */
static node*
new_alias(parser_state *p, mrb_sym a, mrb_sym b)
{
  struct mrb_ast_alias_node *alias_node = NEW_NODE(alias, NODE_ALIAS);
  alias_node->new_name = a;
  alias_node->old_name = b;
  return (node*)alias_node;
}

/* struct: if_node(cond, then_body, else_body) */
static node*
new_if(parser_state *p, node *condition, node *then_body, node *else_body)
{
  void_expr_error(p, condition);

  struct mrb_ast_if_node *n = NEW_NODE(if, NODE_IF);
  n->condition = condition;
  n->then_body = then_body;
  n->else_body = else_body;

  return (node*)n;
}

/* struct: while_node(cond, body) */
static node*
new_while(parser_state *p, node *condition, node *body)
{
  void_expr_error(p, condition);

  struct mrb_ast_while_node *n = NEW_NODE(while, NODE_WHILE);
  n->condition = condition;
  n->body = body;

  return (node*)n;
}

/* struct: until_node(cond, body) */
static node*
new_until(parser_state *p, node *condition, node *body)
{
  void_expr_error(p, condition);

  struct mrb_ast_until_node *n = NEW_NODE(until, NODE_UNTIL);
  n->condition = condition;
  n->body = body;

  return (node*)n;
}

/* struct: while_node(cond, body) */
static node*
new_while_mod(parser_state *p, node *condition, node *body)
{
  node *while_node = new_while(p, condition, body);
  struct mrb_ast_while_node *n = (struct mrb_ast_while_node*)while_node;
  n->header.node_type = NODE_WHILE_MOD;
  return while_node;
}

/* struct: until_node(cond, body) */
static node*
new_until_mod(parser_state *p, node *a, node *b)
{
  node *until_node = new_until(p, a, b);
  struct mrb_ast_until_node *n = (struct mrb_ast_until_node*)until_node;
  n->header.node_type = NODE_UNTIL_MOD;
  return until_node;
}


/* struct: for_node(var, obj, body) */
static node*
new_for(parser_state *p, node *v, node *o, node *b)
{
  void_expr_error(p, o);

  struct mrb_ast_for_node *n = NEW_NODE(for, NODE_FOR);
  n->var = v;
  n->iterable = o;
  n->body = b;

  return (node*)n;
}

/* struct: case_node(expr, when_clauses) - uses cons list */
static node*
new_case(parser_state *p, node *a, node *b)
{
  void_expr_error(p, a);

  struct mrb_ast_case_node *n = NEW_NODE(case, NODE_CASE);
  n->value = a;
  n->body = b;

  return (node*)n;
}

/* Pattern matching case/in expression */
static node*
new_case_match(parser_state *p, node *val, node *in_clauses)
{
  void_expr_error(p, val);

  struct mrb_ast_case_match_node *n = NEW_NODE(case_match, NODE_CASE_MATCH);
  n->value = val;
  n->in_clauses = in_clauses;

  return (node*)n;
}

/* Create value pattern node */
static node*
new_pat_value(parser_state *p, node *val)
{
  struct mrb_ast_pat_value_node *n = NEW_NODE(pat_value, NODE_PAT_VALUE);
  n->value = val;
  return (node*)n;
}

/* Create variable pattern node */
static node*
new_pat_var(parser_state *p, mrb_sym name)
{
  struct mrb_ast_pat_var_node *n = NEW_NODE(pat_var, NODE_PAT_VAR);
  n->name = name;
  /* Register as local variable if not wildcard */
  if (name) {
    local_add(p, name);
  }
  return (node*)n;
}

/* Create pin pattern node (^var) */
static node*
new_pat_pin(parser_state *p, mrb_sym name)
{
  struct mrb_ast_pat_pin_node *n = NEW_NODE(pat_pin, NODE_PAT_PIN);
  n->name = name;
  /* Pin operator references existing variable, does not create new binding */
  return (node*)n;
}

/* Create as pattern node (pattern => var) */
static node*
new_pat_as(parser_state *p, node *pattern, mrb_sym name)
{
  struct mrb_ast_pat_as_node *n = NEW_NODE(pat_as, NODE_PAT_AS);
  n->pattern = pattern;
  n->name = name;
  local_add(p, name);
  return (node*)n;
}

/* Create alternative pattern node (pat1 | pat2) */
static node*
new_pat_alt(parser_state *p, node *left, node *right)
{
  struct mrb_ast_pat_alt_node *n = NEW_NODE(pat_alt, NODE_PAT_ALT);
  n->left = left;
  n->right = right;
  return (node*)n;
}

/* Create array pattern node [a, b, *rest, c] */
static node*
new_pat_array(parser_state *p, node *pre, node *rest, node *post)
{
  struct mrb_ast_pat_array_node *n = NEW_NODE(pat_array, NODE_PAT_ARRAY);
  n->pre = pre;
  n->rest = rest;
  n->post = post;
  return (node*)n;
}

/* Create find pattern node [*pre, elems, *post] */
static node*
new_pat_find(parser_state *p, node *pre, node *elems, node *post)
{
  struct mrb_ast_pat_find_node *n = NEW_NODE(pat_find, NODE_PAT_FIND);
  n->pre = pre;
  n->elems = elems;
  n->post = post;
  return (node*)n;
}

/* Create hash pattern node {a:, b: x, **rest} */
static node*
new_pat_hash(parser_state *p, node *pairs, node *rest)
{
  struct mrb_ast_pat_hash_node *n = NEW_NODE(pat_hash, NODE_PAT_HASH);
  n->pairs = pairs;
  n->rest = rest;
  return (node*)n;
}

/* Create one-line pattern matching node (expr in pattern / expr => pattern) */
static node*
new_match_pat(parser_state *p, node *value, node *pattern, mrb_bool raise_on_fail)
{
  struct mrb_ast_match_pat_node *n = NEW_NODE(match_pat, NODE_MATCH_PAT);
  n->value = value;
  n->pattern = pattern;
  n->raise_on_fail = raise_on_fail;
  return (node*)n;
}

/* Create in-clause node for case/in */
static node*
new_in(parser_state *p, node *pattern, node *guard, node *body, mrb_bool guard_is_unless)
{
  struct mrb_ast_in_node *n = NEW_NODE(in, NODE_IN);
  n->pattern = pattern;
  n->guard = guard;
  n->body = body;
  n->guard_is_unless = guard_is_unless;
  return (node*)n;
}

/* struct: postexe_node(body) */
static node*
new_postexe(parser_state *p, node *a)
{
  struct mrb_ast_postexe_node *postexe_node = NEW_NODE(postexe, NODE_POSTEXE);
  postexe_node->body = a;
  return (node*)postexe_node;
}

/* struct: self_node() */
static node*
new_self(parser_state *p)
{
  struct mrb_ast_self_node *n = NEW_NODE(self, NODE_SELF);

  return (node*)n;
}

/* struct: call_node(receiver, method, args) */
static node*
new_call(parser_state *p, node *receiver, mrb_sym method, node *args, int pass)
{
  /* Calculate size needed (fixed size now) */  struct mrb_ast_call_node *n = NEW_NODE(call, NODE_CALL);
  n->receiver = receiver;
  n->method_name = method;
  n->safe_call = (pass == 0); /* pass == 0 means safe call (&.) */

  /* Store args pointer directly - no need to unpack and repack */
  n->args = args;

  void_expr_error(p, receiver);
  return (node*)n;
}

/* struct: fcall_node(method, args) */
static node*
new_fcall(parser_state *p, mrb_sym b, node *c)
{
  return new_call(p, NULL, b, c, '.');
}

/* (a b . c) */
static node*
new_callargs(parser_state *p, node *a, node *b, node *c)
{
  /* Allocate struct mrb_ast_callargs (fixed size, like new_args) */
  struct mrb_ast_callargs *callargs = (struct mrb_ast_callargs*)parser_palloc(p, sizeof(struct mrb_ast_callargs));

  /* Initialize members directly */
  callargs->regular_args = a;   /* Cons list of regular arguments (preserves splat compatibility) */
  callargs->keyword_args = b;   /* Keyword arguments hash node */
  callargs->block_arg = c;      /* Block argument node */

  /* Return direct cast to node (like new_args) */
  return (node*)callargs;
}

/* struct: super_node(args) */
static node*
new_super(parser_state *p, node *c)
{
  struct mrb_ast_super_node *n = NEW_NODE(super, NODE_SUPER);
  n->args = c;

  return (node*)n;
}

/* struct: zsuper_node() */
static node*
new_zsuper(parser_state *p)
{
  struct mrb_ast_super_node *n = NEW_NODE(super, NODE_ZSUPER);
  n->args = NULL;  /* zsuper initially has no args, but may be added by call_with_block */
  return (node*)n;
}

/* struct: yield_node(args) */
static node*
new_yield(parser_state *p, node *c)
{
  /* Handle callargs structure - direct casting like new_args() */
  if (c) {
    struct mrb_ast_callargs *callargs = (struct mrb_ast_callargs*)c;
    if (callargs->block_arg) {
      yyerror(NULL, p, "both block arg and actual block given");
    }
  }  struct mrb_ast_yield_node *n = NEW_NODE(yield, NODE_YIELD);
  n->args = c;

  return (node*)n;
}

/* struct: return_node(value) */
static node*
new_return(parser_state *p, node *c)
{
  struct mrb_ast_return_node *n = NEW_NODE(return, NODE_RETURN);
  n->args = c;

  return (node*)n;
}

/* struct: break_node(value) */
static node*
new_break(parser_state *p, node *c)
{
  struct mrb_ast_break_node *n = NEW_NODE(break, NODE_BREAK);
  n->value = c;
  return (node*)n;
}

/* struct: defined_node(expr) */
static node*
new_defined(parser_state *p, node *c)
{
  struct mrb_ast_defined_node *n = NEW_NODE(defined, NODE_DEFINED);
  n->expr = c;
  return (node*)n;
}

/* struct: next_node(value) */
static node*
new_next(parser_state *p, node *c)
{
  struct mrb_ast_next_node *n = NEW_NODE(next, NODE_NEXT);
  n->value = c;
  return (node*)n;
}

/* struct: redo_node() */
static node*
new_redo(parser_state *p)
{
  struct mrb_ast_redo_node *n = NEW_NODE(redo, NODE_REDO);
  return (node*)n;
}

/* struct: retry_node() */
static node*
new_retry(parser_state *p)
{
  struct mrb_ast_retry_node *n = NEW_NODE(retry, NODE_RETRY);
  return (node*)n;
}

/* struct: dot2_node(beg, end) */
static node*
new_dot2(parser_state *p, node *a, node *b)
{
  struct mrb_ast_dot2_node *n = NEW_NODE(dot2, NODE_DOT2);
  n->left = a;
  n->right = b;

  return (node*)n;
}

/* struct: dot3_node(beg, end) */
static node*
new_dot3(parser_state *p, node *a, node *b)
{
  struct mrb_ast_dot3_node *n = NEW_NODE(dot3, NODE_DOT3);
  n->left = a;
  n->right = b;

  return (node*)n;
}

/* struct: colon2_node(base, name) */
static node*
new_colon2(parser_state *p, node *b, mrb_sym c)
{
  void_expr_error(p, b);

  struct mrb_ast_colon2_node *colon2_node = NEW_NODE(colon2, NODE_COLON2);
  colon2_node->base = b;
  colon2_node->name = c;
  return (node*)colon2_node;
}

/* struct: colon3_node(name) */
static node*
new_colon3(parser_state *p, mrb_sym c)
{
  struct mrb_ast_colon3_node *colon3_node = NEW_NODE(colon3, NODE_COLON3);
  colon3_node->name = c;
  return (node*)colon3_node;
}

/* struct: and_node(left, right) */
static node*
new_and(parser_state *p, node *a, node *b)
{
  void_expr_error(p, a);

  struct mrb_ast_and_node *n = NEW_NODE(and, NODE_AND);
  n->left = a;
  n->right = b;

  return (node*)n;
}

/* struct: or_node(left, right) */
static node*
new_or(parser_state *p, node *a, node *b)
{
  void_expr_error(p, a);

  struct mrb_ast_or_node *n = NEW_NODE(or, NODE_OR);
  n->left = a;
  n->right = b;

  return (node*)n;
}

/* struct: array_node(elements) - uses cons list */
static node*
new_array(parser_state *p, node *a)
{
  struct mrb_ast_array_node *n = NEW_NODE(array, NODE_ARRAY);
  n->elements = a;

  return (node*)n;
}

/* struct: splat_node(value) */
static node*
new_splat(parser_state *p, node *a)
{
  void_expr_error(p, a);

  struct mrb_ast_splat_node *splat_node = NEW_NODE(splat, NODE_SPLAT);
  splat_node->value = a;
  return (node*)splat_node;
}

/* struct: hash_node(pairs) - uses cons list */
static node*
new_hash(parser_state *p, node *a)
{
  struct mrb_ast_hash_node *n = NEW_NODE(hash, NODE_HASH);
  n->pairs = a;

  return (node*)n;
}

/* (:sym . a) */
/* Symbol node creation - supports both variable and legacy modes */
static node*
new_sym(parser_state *p, mrb_sym sym)
{
  struct mrb_ast_sym_node *n = NEW_NODE(sym, NODE_SYM);
  n->symbol = sym;

  return (node*)n;
}

static node*
new_xvar(parser_state *p, mrb_sym sym, enum node_type type)
{
  struct mrb_ast_var_node *n = NEW_NODE(var, type);
  n->symbol = sym;

  return (node*)n;
}

#define new_lvar(p, sym) new_xvar(p, sym, NODE_LVAR)
#define new_ivar(p, sym) new_xvar(p, sym, NODE_IVAR)
#define new_gvar(p, sym) new_xvar(p, sym, NODE_GVAR)
#define new_cvar(p, sym) new_xvar(p, sym, NODE_CVAR)

static mrb_sym
new_strsym(parser_state *p, node* str)
{
  size_t len = (size_t)str->car;
  const char *s = (const char*)str->cdr;

  return mrb_intern(p->mrb, s, len);
}

/* (:nvar . a) */
static node*
new_nvar(parser_state *p, int num)
{
  int nvar;
  node *nvars = p->nvars->cdr;
  while (nvars) {
    nvar = node_to_int(nvars->car);
    if (nvar == -2) break; /* top of the scope */
    if (nvar > 0) {
      yyerror(NULL, p, "numbered parameter used in outer block");
      break;
    }
    nvars->car = int_to_node(-1);
    nvars = nvars->cdr;
  }
  nvar = node_to_int(p->nvars->car);
  if (nvar == -1) {
    yyerror(NULL, p, "numbered parameter used in inner block");
  }
  else {
    p->nvars->car = int_to_node(nvar > num ? nvar : num);
  }
  struct mrb_ast_nvar_node *n = NEW_NODE(nvar, NODE_NVAR);
  n->num = num;
  return (node*)n;
}

/* struct: const_node(name) */
static node*
new_const(parser_state *p, mrb_sym sym)
{
  struct mrb_ast_const_node *n = NEW_NODE(const, NODE_CONST);
  n->symbol = sym;

  return (node*)n;
}

/* struct: undef_node(syms) - uses cons list */
static node*
new_undef(parser_state *p, node *syms)
{
  struct mrb_ast_undef_node *undef_node = NEW_NODE(undef, NODE_UNDEF);
  undef_node->syms = syms;
  return (node*)undef_node;
}

/* struct: class_node(path, super, body) */
static node*
new_class(parser_state *p, node *c, node *s, node *b)
{
  void_expr_error(p, s);

  struct mrb_ast_class_node *n = NEW_NODE(class, NODE_CLASS);
  n->name = c;
  n->superclass = s;
  n->body = cons(locals_node(p), b);

  return (node*)n;
}

/* struct: sclass_node(obj, body) */
static node*
new_sclass(parser_state *p, node *o, node *b)
{
  void_expr_error(p, o);

  struct mrb_ast_sclass_node *n = NEW_NODE(sclass, NODE_SCLASS);
  n->obj = o;
  n->body = cons(locals_node(p), b);

  return (node*)n;
}

/* struct: module_node(path, body) */
static node*
new_module(parser_state *p, node *m, node *b)
{
  struct mrb_ast_module_node *n = NEW_NODE(module, NODE_MODULE);
  n->name = m;
  n->body = cons(locals_node(p), b);

  return (node*)n;
}

/* struct: def_node(name, args, body) */
static node*
new_def(parser_state *p, mrb_sym name)
{
  struct mrb_ast_def_node *n = NEW_NODE(def, NODE_DEF);
  n->name = name;
  n->args = (struct mrb_ast_args *)int_to_node(p->cmdarg_stack);
  n->locals = local_switch(p);
  n->body = NULL;

  return (node*)n;
}

static void
defn_setup(parser_state *p, node *d, node *a, node *b)
{
  struct mrb_ast_def_node *n = def_node(d);
  node *locals = n->locals;

  n->locals = locals_node(p);
  p->cmdarg_stack = node_to_int(n->args);
  n->args = (struct mrb_ast_args *)a;
  n->body = b;
  local_resume(p, locals);
}

/* struct: sdef_node(obj, name, args, body) */
static node*
new_sdef(parser_state *p, node *o, mrb_sym name)
{
  void_expr_error(p, o);

  struct mrb_ast_sdef_node *sdef_node = NEW_NODE(sdef, NODE_SDEF);
  sdef_node->obj = o;
  sdef_node->name = name;
  sdef_node->args = (struct mrb_ast_args *)int_to_node(p->cmdarg_stack);
  sdef_node->locals = local_switch(p);
  sdef_node->body = NULL;
  return (node*)sdef_node;
}

static void
local_add_margs(parser_state *p, node *n)
{
  while (n) {
    if (node_type(n->car) == NODE_MARG) {
      struct mrb_ast_masgn_node *masgn_n = (struct mrb_ast_masgn_node*)n->car;
      node *rhs = masgn_n->rhs;

      /* For parameter destructuring, rhs contains the locals */
      if (rhs) {
        node *t = rhs;
        while (t) {
          local_add_f(p, node_to_sym(t->car));
          t = t->cdr;
        }
        /* Clear cons list RHS immediately after use */
        masgn_n->rhs = NULL;
      }

      /* Process nested destructuring in lhs components */
      if (masgn_n->pre) {
        local_add_margs(p, masgn_n->pre);
      }
      if (masgn_n->post) {
        local_add_margs(p, masgn_n->post);
      }
    }
    n = n->cdr;
  }
}


static void
local_add_lv(parser_state *p, node *lv)
{
  while (lv) {
    local_add_f(p, node_to_sym(lv->car));
    lv = lv->cdr;
  }
}

/* (m o r m2 tail) */
/* m: (a b c) */
/* o: ((a . e1) (b . e2)) */
/* r: a */
/* m2: (a b c) */
/* b: a */
static node*
new_args(parser_state *p, node *m, node *opt, mrb_sym rest, node *m2, node *tail)
{
  local_add_margs(p, m);
  local_add_margs(p, m2);

  /* Save original optional arguments before processing */
  node *orig_opt = opt;

  /* Process optional arguments (keep original side effects) */
  while (opt) {
    /* opt: (sym . (opt . lv)) -> (sym . opt) */
    local_add_lv(p, opt->car->cdr->cdr);
    opt->car->cdr = opt->car->cdr->car;
    opt = opt->cdr;
  }

  /* Allocate struct mrb_ast_args (no hdr) */
  struct mrb_ast_args *args = (struct mrb_ast_args*)parser_palloc(p, sizeof(struct mrb_ast_args));

  /* Initialize members */
  args->mandatory_args = m;
  args->optional_args = orig_opt;
  args->rest_arg = rest;
  args->post_mandatory_args = m2;

  /* Deconstruct tail cons list: (kws . (kwrest . blk)) */
  if (tail) {
    args->keyword_args = (node*)tail->car;          /* kws */
    args->kwrest_arg = (mrb_sym)(intptr_t)tail->cdr->car; /* kwrest */
    args->block_arg = (mrb_sym)(intptr_t)tail->cdr->cdr;  /* blk */
    cons_free(tail->cdr);
    cons_free(tail);
  }
  else {
    args->keyword_args = NULL;
    args->kwrest_arg = 0;
    args->block_arg = 0;
  }

  return (node*)args;
}

/* struct: args_tail_node(kwargs, kwrest, block) */
static node*
new_args_tail(parser_state *p, node *kws, mrb_sym kwrest, mrb_sym blk)
{
  node *k;

  if (kws || kwrest) {
    local_add_kw(p, kwrest);
  }

  local_add_blk(p);
  if (blk && blk != MRB_SYM(nil)) local_add_f(p, blk);

  /* allocate register for keywords arguments */
  /* order is for Proc#parameters */
  for (k = kws; k; k = k->cdr) {
    if (!k->car->cdr) { /* allocate required keywords - simplified structure: (key . NULL) */
      local_add_f(p, node_to_sym(k->car->car));
    }
  }
  for (k = kws; k; k = k->cdr) {
    if (k->car->cdr) { /* allocate keywords with default - simplified structure: (key . value) */
      local_add_lv(p, k->car->cdr->cdr);  /* value->cdr for default args */
      k->car->cdr = k->car->cdr->car;    /* value->car for default args */
      local_add_f(p, node_to_sym(k->car->car));
    }
  }

  /* Return cons list: (keyword . (kwrest . blk)) */
  return cons(kws, cons(sym_to_node(kwrest), sym_to_node(blk)));
}

/* (kw_sym . def_arg) - simplified from NODE_KW_ARG wrapper */
static node*
new_kw_arg(parser_state *p, mrb_sym kw, node *def_arg)
{
  mrb_assert(kw);
  return cons(sym_to_node(kw), def_arg);
}

/* (:kw_rest_args . a) */
static node*
new_kw_rest_args(parser_state *p, mrb_sym sym)
{
  return sym_to_node(intern_op(pow));  /* Use ** symbol as direct marker */
}

static node*
new_args_dots(parser_state *p, node *m)
{
  mrb_sym r = intern_op(mul);
  mrb_sym k = intern_op(pow);
  mrb_sym b = intern_op(and);
  local_add_f(p, r);
  return new_args(p, m, 0, r, 0, new_args_tail(p, NULL, k, b));
}

/* struct: block_arg_node(value) */
static node*
new_block_arg(parser_state *p, node *a)
{
  struct mrb_ast_block_arg_node *block_arg_node = NEW_NODE(block_arg, NODE_BLOCK_ARG);
  block_arg_node->value = a;
  return (node*)block_arg_node;
}

static node*
setup_numparams(parser_state *p, node *a)
{
  int nvars = node_to_int(p->nvars->car);
  if (nvars > 0) {
    int i;
    mrb_sym sym;
    // Check if any arguments are already defined
    struct mrb_ast_args *args = (struct mrb_ast_args *)a;
    if (a && (args->mandatory_args || args->optional_args || args->rest_arg ||
              args->post_mandatory_args || args->keyword_args || args->kwrest_arg)) {
      yyerror(NULL, p, "ordinary parameter is defined");
    }
    else if (p->locals) {
      /* p->locals should not be NULL unless error happens before the point */
      node* args = 0;
      for (i = nvars; i > 0; i--) {
        char buf[3];

        buf[0] = '_';
        buf[1] = i+'0';
        buf[2] = '\0';
        sym = intern_cstr(buf);
        args = cons(new_lvar(p, sym), args);
        p->locals->car = cons(sym_to_node(sym), p->locals->car);
      }
      a = new_args(p, args, 0, 0, 0, 0);
    }
  }
  return a;
}

/* struct: block_node(args, body) */
static node*
new_block(parser_state *p, node *a, node *b)
{
  a = setup_numparams(p, a);  struct mrb_ast_block_node *n = NEW_NODE(block, NODE_BLOCK);
  n->locals = locals_node(p);
  n->args = (struct mrb_ast_args *)a;
  n->body = b;

  return (node*)n;
}

/* struct: lambda_node(args, body) */
static node*
new_lambda(parser_state *p, node *a, node *b)
{
  a = setup_numparams(p, a);  struct mrb_ast_lambda_node *lambda_node = NEW_NODE(lambda, NODE_LAMBDA);
  lambda_node->locals = locals_node(p);
  lambda_node->args = (struct mrb_ast_args *)a;
  lambda_node->body = b;
  return (node*)lambda_node;
}

/* struct: asgn_node(lhs, rhs) */
static node*
new_asgn(parser_state *p, node *a, node *b)
{
  void_expr_error(p, b);

  struct mrb_ast_asgn_node *n = NEW_NODE(asgn, NODE_ASGN);
  n->lhs = a;
  n->rhs = b;

  return (node*)n;
}

/* Helper function to create MASGN/MARG nodes */
static node*
new_masgn_helper(parser_state *p, node *a, node *b, enum node_type node_type)
{
  struct mrb_ast_masgn_node *n = NEW_NODE(masgn, node_type);

  /* Extract pre, rest, post from cons list structure (a b c) */
  if (a) {
    n->pre = a->car;  /* Pre-splat variables */
    if (a->cdr) {
      n->rest = a->cdr->car;  /* Splat variable (or -1 for anonymous) */
      if (a->cdr->cdr) {
        n->post = a->cdr->cdr->car;  /* Post-splat variables */
        cons_free(a->cdr->cdr);
      }
      else {
        n->post = NULL;
      }
      cons_free(a->cdr);
    }
    else {
      n->rest = NULL;
      n->post = NULL;
    }
    cons_free(a);
  }
  else {
    n->pre = NULL;
    n->rest = NULL;
    n->post = NULL;
  }
  n->rhs = b;

  return (node*)n;
}

/* struct: masgn_node(lhs, rhs) */
static node*
new_masgn(parser_state *p, node *a, node *b)
{
  void_expr_error(p, b);
  return new_masgn_helper(p, a, b, NODE_MASGN);
}

/* (:marg mlhs mrhs) no check - for parameter destructuring */
static node*
new_marg(parser_state *p, node *a)
{
  return new_masgn_helper(p, a, p->locals->car, NODE_MARG);
}

/* struct: op_asgn_node(lhs, op, rhs) */
static node*
new_op_asgn(parser_state *p, node *a, mrb_sym op, node *b)
{
  void_expr_error(p, b);

  struct mrb_ast_op_asgn_node *n = NEW_NODE(op_asgn, NODE_OP_ASGN);
  n->lhs = a;
  n->op = op;
  n->rhs = b;
  return (node*)n;
}

static node*
new_int_n(parser_state *p, int32_t val)
{
  struct mrb_ast_int_node *n = NEW_NODE(int, NODE_INT);
  n->value = val;

  return (node*)n;
}

static node*
new_imaginary(parser_state *p, node *imaginary)
{
  return new_fcall(p, MRB_SYM(Complex),
                   new_callargs(p, list2(new_int_n(p, 0), imaginary), 0, 0));
}

static node*
new_rational(parser_state *p, node *rational)
{
  return new_fcall(p, MRB_SYM(Rational), new_callargs(p, list1(rational), 0, 0));
}

/* Read integer into int32_t with overflow detection */
static mrb_bool
read_int32(const char *p, int base, int32_t *result)
{
  const char *e = p + strlen(p);
  int32_t value = 0;
  mrb_bool neg = FALSE;

  if (base < 2 || base > 16) {
    return FALSE;
  }

  if (*p == '+') {
    p++;
  }
  else if (*p == '-') {
    neg = TRUE;
    p++;
  }

  while (p < e) {
    int n;
    char c = *p;

    /* Skip underscores */
    if (c == '_') {
      p++;
      continue;
    }

    /* Parse digit */
    if (c >= '0' && c <= '9') {
      n = c - '0';
    }
    else if (c >= 'a' && c <= 'f') {
      n = c - 'a' + 10;
    }
    else if (c >= 'A' && c <= 'F') {
      n = c - 'A' + 10;
    }
    else {
      /* Invalid character */
      return FALSE;
    }

    if (n >= base) {
      /* Digit not valid for this base */
      return FALSE;
    }

    /* Check for multiplication overflow */
    if (value > INT32_MAX / base) {
      return FALSE;
    }

    value *= base;

    /* Check for addition overflow */
    if (value > INT32_MAX - n) {
      /* Special case: -INT32_MIN is valid */
      if (neg && value == (INT32_MAX - n + 1) && p + 1 == e) {
        *result = INT32_MIN;
        return TRUE;
      }
      return FALSE;
    }

    value += n;
    p++;
  }

  *result = neg ? -value : value;
  return TRUE;
}

static node*
new_int(parser_state *p, const char *s, int base, int suffix)
{
  int32_t val;
  node* result;

  /* Try to parse as int32_t first */
  if (read_int32(s, base, &val)) {
    result = new_int_n(p, val);
  }
  else {
    /* Big integer - create NODE_BIGINT */
    struct mrb_ast_bigint_node *n = NEW_NODE(bigint, NODE_BIGINT);
    n->string = strdup(s);
    n->base = base;

    result = (node*)n;
  }

  /* Handle suffix modifiers */
  if (suffix & NUM_SUFFIX_R) {
    result = new_rational(p, result);
  }
  if (suffix & NUM_SUFFIX_I) {
    result = new_imaginary(p, result);
  }

  return result;
}

#ifndef MRB_NO_FLOAT
/* struct: float_node(value) */
static node*
new_float(parser_state *p, const char *s, int suffix)
{
  struct mrb_ast_float_node *n = NEW_NODE(float, NODE_FLOAT);
  n->value = strdup(s);

  node* result = (node*)n;

  if (suffix & NUM_SUFFIX_R) {
    result = new_rational(p, result);
  }
  if (suffix & NUM_SUFFIX_I) {
    result = new_imaginary(p, result);
  }
  return result;
}
#endif

/* Create string node from cons list */
/* struct: str_node(str) */
static node*
new_str(parser_state *p, node *a)
{
  struct mrb_ast_str_node *n = NEW_NODE(str, NODE_STR);
  n->list = a;

  return (node*)n;
}

/* struct: xstr_node(str) */
static node*
new_xstr(parser_state *p, node *a)
{
  struct mrb_ast_xstr_node *n = NEW_NODE(xstr, NODE_XSTR);
  n->list = a;
  return (node*)n;
}

/* struct: dsym_node(parts) - uses cons list */
static node*
new_dsym(parser_state *p, node *a)
{
  struct mrb_ast_str_node *n = NEW_NODE(str, NODE_DSYM);
  n->list = a;
  return (node*)n;
}

/* struct: regx_node(pattern, flags, encoding) */
static node*
new_regx(parser_state *p, node *list, const char *flags, const char *encoding)
{
  struct mrb_ast_regx_node *n = NEW_NODE(regx, NODE_REGX);
  n->list = list;
  n->flags = flags;
  n->encoding = encoding;
  return (node*)n;
}

/* struct: back_ref_node(n) */
static node*
new_back_ref(parser_state *p, int n)
{
  struct mrb_ast_back_ref_node *backref_node = NEW_NODE(back_ref, NODE_BACK_REF);
  backref_node->type = n;
  return (node*)backref_node;
}

/* struct: nth_ref_node(n) */
static node*
new_nth_ref(parser_state *p, int n)
{
  struct mrb_ast_nth_ref_node *nthref_node = NEW_NODE(nth_ref, NODE_NTH_REF);
  nthref_node->nth = n;
  return (node*)nthref_node;
}

/* struct: heredoc_node(str) */
static node*
new_heredoc(parser_state *p, struct mrb_parser_heredoc_info **infop)
{
  struct mrb_ast_heredoc_node *n = NEW_NODE(heredoc, NODE_HEREDOC);

  /* Initialize embedded heredoc info struct */
  n->info.allow_indent = FALSE;
  n->info.remove_indent = FALSE;
  n->info.line_head = FALSE;
  n->info.indent = 0;
  n->info.indented = NULL;
  n->info.type = str_not_parsing;  // Will be set by heredoc processing
  n->info.term = NULL;  // Will be set by heredoc processing
  n->info.term_len = 0;
  n->info.doc = NULL;

  /* Return pointer to embedded info if requested */
  *infop = &n->info;

  return (node*)n;
}

static void
new_bv(parser_state *p, mrb_sym id)
{
}

static node*
new_literal_delim(parser_state *p)
{
  return cons((node*)0, (node*)0);
}

/* Helper for creating string representation cons (length . string_ptr) */
static node*
new_str_rep(parser_state *p, const char *str, int len)
{
  return cons(int_to_node(len), (node*)strndup(str, len));
}

/* Helper for creating string representation from current token */
static node*
new_str_tok(parser_state *p)
{
  return new_str_rep(p, tok(p), toklen(p));
}

/* Helper for creating empty string representation */
static node*
new_str_empty(parser_state *p)
{
  return new_str_rep(p, "", 0);
}

/* (:words . a) */
static node*
new_words(parser_state *p, node *a)
{
  struct mrb_ast_words_node *words_node = NEW_NODE(words, NODE_WORDS);
  words_node->args = a;
  return (node*)words_node;
}

/* (:symbols . a) */
static node*
new_symbols(parser_state *p, node *a)
{
  struct mrb_ast_symbols_node *symbols_node = NEW_NODE(symbols, NODE_SYMBOLS);
  symbols_node->args = a;
  return (node*)symbols_node;
}

/* xxx ----------------------------- */

/* (:call a op) */
static node*
call_uni_op(parser_state *p, node *recv, const char *m)
{
  void_expr_error(p, recv);
  return new_call(p, recv, intern_cstr(m), 0, '.');
}

/* (:call a op b) */
static node*
call_bin_op(parser_state *p, node *recv, const char *m, node *arg1)
{
  return new_call(p, recv, intern_cstr(m), new_callargs(p, list1(arg1), 0, 0), '.');
}

static void
args_with_block(parser_state *p, node *a, node *b)
{
  if (b) {
    /* Handle callargs structure - direct casting like new_args() */
    struct mrb_ast_callargs *callargs = (struct mrb_ast_callargs*)a;
    if (callargs->block_arg) {
      yyerror(NULL, p, "both block arg and actual block given");
    }
    callargs->block_arg = b;
  }
}

static void
endless_method_name(parser_state *p, node *defn)
{
  struct mrb_ast_def_node *def = (struct mrb_ast_def_node*)defn;
  mrb_sym sym = def->name;
  mrb_int len;
  const char *name = mrb_sym_name_len(p->mrb, sym, &len);

  if (len > 1 && name[len-1] == '=') {
    for (int i=0; i<len-1; i++) {
      if (!identchar(name[i])) return;
    }
    yyerror(NULL, p, "setter method cannot be defined by endless method definition");
  }
}

static void
call_with_block(parser_state *p, node *a, node *b)
{
  if (!a) return;

  /* Handle direct variable-sized nodes */
  struct mrb_ast_var_header *header = (struct mrb_ast_var_header*)a;

  enum node_type var_type = (enum node_type)header->node_type;
  switch (var_type) {
  case NODE_SUPER:
  case NODE_ZSUPER:
    /* For variable-sized super/zsuper nodes, update the args field directly */
    {
      struct mrb_ast_super_node *super_n = super_node(a);
      if (!super_n->args) {
        super_n->args = new_callargs(p, 0, 0, b);
      }
      else {
        args_with_block(p, super_n->args, b);
      }
    }
    break;
  case NODE_YIELD:
    /* Variable-sized yield nodes should generate an error when given a block */
    yyerror(NULL, p, "block given to yield");
    break;
  case NODE_RETURN:
    /* Variable-sized return nodes - recursively call with args */
    {
      struct mrb_ast_return_node *return_n = return_node(a);
      if (return_n->args != NULL) {
        call_with_block(p, return_n->args, b);
      }
    }
    break;
  case NODE_BREAK:
    /* Variable-sized break nodes - recursively call with value */
    {
      struct mrb_ast_break_node *break_n = (struct mrb_ast_break_node*)a;
      if (break_n->value != NULL) {
        call_with_block(p, break_n->value, b);
      }
    }
    break;
  case NODE_NEXT:
    /* Variable-sized next nodes - recursively call with value */
    {
      struct mrb_ast_next_node *next_n = (struct mrb_ast_next_node*)a;
      if (next_n->value != NULL) {
        call_with_block(p, next_n->value, b);
      }
    }
    break;
  case NODE_CALL:
    /* Variable-sized call nodes - add block to existing args */
    {
      struct mrb_ast_call_node *call = call_node(a);

      if (call->args && callargs_node(call->args)->block_arg) {
        yyerror(NULL, p, "both block arg and actual block given");
        return;
      }

      /* Use existing args and add block */
      if (call->args) {
        /* Modify existing callargs structure to add block */
        args_with_block(p, call->args, b);
      }
      else {
        /* Create new callargs with just the block */
        call->args = new_callargs(p, NULL, NULL, b);
      }
    }
    break;
  default:
    /* For other variable-sized nodes, do nothing */
    break;
  }
}

static node*
new_negate(parser_state *p, node *n)
{
  struct mrb_ast_negate_node *negate_node = NEW_NODE(negate, NODE_NEGATE);
  negate_node->operand = n;
  return (node*)negate_node;
}

static node*
cond(node *n)
{
  return n;
}

static node*
ret_args(parser_state *p, node *n)
{
  /* Handle callargs structure - direct casting like new_args() */
  struct mrb_ast_callargs *callargs = (struct mrb_ast_callargs*)n;
  if (callargs->block_arg) {
    yyerror(NULL, p, "block argument should not be given");
    return NULL;
  }
  if (!callargs->regular_args) return NULL;
  if (!callargs->regular_args->cdr) return callargs->regular_args->car;
  return new_array(p, callargs->regular_args);
}

static void
assignable(parser_state *p, node *lhs)
{
  switch (node_type(lhs)) {
  case NODE_LVAR:
    local_add(p, var_node(lhs)->symbol);
    break;
  case NODE_CONST:
    if (p->in_def)
      yyerror(NULL, p, "dynamic constant assignment");
    break;
  default:
    /* Other node types don't need special handling in assignable */
    break;
  }
}

static node*
var_reference(parser_state *p, node *lhs)
{
  /* Check if this is a variable-sized node */
  if (node_type_p(lhs, NODE_LVAR)) {
    mrb_sym sym = var_node(lhs)->symbol;
    if (!local_var_p(p, sym)) {
      node *n = new_fcall(p, sym, 0);
      /* Don't free variable-sized nodes - they're managed by the parser allocator */
      return n;
    }
  }
  return lhs;
}

static node*
label_reference(parser_state *p, mrb_sym sym)
{
  const char *name = mrb_sym_name(p->mrb, sym);

  if (local_var_p(p, sym)) {
    return new_lvar(p, sym);
  }
  else if (ISUPPER(name[0])) {
    return new_const(p, sym);
  }
  else {
    return new_fcall(p, sym, 0);
  }
}

typedef enum mrb_string_type  string_type;

typedef struct parser_lex_strterm {
  int type;
  int level;
  int term;
  int paren;
  struct parser_lex_strterm *prev;
} parser_lex_strterm;

static parser_lex_strterm*
new_strterm(parser_state *p, string_type type, int term, int paren)
{
  parser_lex_strterm *lex = (parser_lex_strterm*)parser_palloc(p, sizeof(parser_lex_strterm));
  lex->type = type;
  lex->level = 0;
  lex->term = term;
  lex->paren = paren;
  lex->prev = p->lex_strterm;
  return lex;
}

static void
end_strterm(parser_state *p)
{
  parser_lex_strterm *term = p->lex_strterm->prev;
  parser_pfree(p->lex_strterm);
  p->lex_strterm = term;
}

static node*
push_strterm(parser_state *p)
{
  node *n = cons((node*)p->lex_strterm, p->parsing_heredoc);
  p->lex_strterm = NULL;
  return n;
}

static void
pop_strterm(parser_state *p, node *n)
{
  p->lex_strterm = (parser_lex_strterm*)n->car;
  p->parsing_heredoc = n->cdr;
  cons_free(n);
}

static parser_heredoc_info *
parsing_heredoc_info(parser_state *p)
{
  node *nd = p->parsing_heredoc;
  if (nd == NULL) return NULL;
  /* mrb_assert(nd->car->car == NODE_HEREDOC); */
  if (node_type(nd->car) == NODE_HEREDOC) {
    /* Variable-sized heredoc node - return address of embedded info struct */
    struct mrb_ast_heredoc_node *heredoc = (struct mrb_ast_heredoc_node*)nd->car;
    return &heredoc->info;
  }
  return (parser_heredoc_info*)nd->car->cdr;
}

static void
heredoc_treat_nextline(parser_state *p)
{
  if (p->heredocs_from_nextline == NULL) return;
  if (p->parsing_heredoc && p->lex_strterm) {
    append(p->heredocs_from_nextline, p->parsing_heredoc);
  }
  p->parsing_heredoc = p->heredocs_from_nextline;
  p->lex_strterm = new_strterm(p, parsing_heredoc_info(p)->type, 0, 0);
  p->heredocs_from_nextline = NULL;
}

static void
heredoc_end(parser_state *p)
{
  p->parsing_heredoc = p->parsing_heredoc->cdr;
  if (p->parsing_heredoc == NULL) {
    p->lstate = EXPR_BEG;
    end_strterm(p);
  }
  else {
    /* next heredoc */
    p->lex_strterm->type = parsing_heredoc_info(p)->type;
  }
}
#define is_strterm_type(p,str_func) ((p)->lex_strterm->type & (str_func))

static void
prohibit_literals(parser_state *p, node *n)
{
  if (n == 0) {
    yyerror(NULL, p, "can't define singleton method for ().");
  }
  else {
    enum node_type nt = node_type(n);
    switch (nt) {
    case NODE_INT:
    case NODE_STR:
    case NODE_XSTR:
    case NODE_REGX:
    case NODE_FLOAT:
    case NODE_ARRAY:
    case NODE_HEREDOC:
      yyerror(NULL, p, "can't define singleton method for literals");
    default:
      break;
    }
  }
}

/* xxx ----------------------------- */


#line 2070 "mrbgems/mruby-compiler/core/y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG && !defined(yydebug)
extern int yydebug;
#endif


/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    keyword_class = 258,           /* "'class'"  */
    keyword_module = 259,          /* "'module'"  */
    keyword_def = 260,             /* "'def'"  */
    keyword_begin = 261,           /* "'begin'"  */
    keyword_if = 262,              /* "'if'"  */
    keyword_unless = 263,          /* "'unless'"  */
    keyword_while = 264,           /* "'while'"  */
    keyword_until = 265,           /* "'until'"  */
    keyword_for = 266,             /* "'for'"  */
    keyword_undef = 267,           /* "'undef'"  */
    keyword_rescue = 268,          /* "'rescue'"  */
    keyword_ensure = 269,          /* "'ensure'"  */
    keyword_end = 270,             /* "'end'"  */
    keyword_then = 271,            /* "'then'"  */
    keyword_elsif = 272,           /* "'elsif'"  */
    keyword_else = 273,            /* "'else'"  */
    keyword_case = 274,            /* "'case'"  */
    keyword_when = 275,            /* "'when'"  */
    keyword_break = 276,           /* "'break'"  */
    keyword_next = 277,            /* "'next'"  */
    keyword_redo = 278,            /* "'redo'"  */
    keyword_retry = 279,           /* "'retry'"  */
    keyword_in = 280,              /* "'in'"  */
    keyword_do = 281,              /* "'do'"  */
    keyword_do_cond = 282,         /* "'do' for condition"  */
    keyword_do_block = 283,        /* "'do' for block"  */
    keyword_do_LAMBDA = 284,       /* "'do' for lambda"  */
    keyword_return = 285,          /* "'return'"  */
    keyword_yield = 286,           /* "'yield'"  */
    keyword_super = 287,           /* "'super'"  */
    keyword_self = 288,            /* "'self'"  */
    keyword_nil = 289,             /* "'nil'"  */
    keyword_true = 290,            /* "'true'"  */
    keyword_false = 291,           /* "'false'"  */
    keyword_and = 292,             /* "'and'"  */
    keyword_or = 293,              /* "'or'"  */
    keyword_not = 294,             /* "'not'"  */
    keyword_defined = 295,         /* "'defined?'"  */
    modifier_if = 296,             /* "'if' modifier"  */
    modifier_unless = 297,         /* "'unless' modifier"  */
    modifier_while = 298,          /* "'while' modifier"  */
    modifier_until = 299,          /* "'until' modifier"  */
    modifier_rescue = 300,         /* "'rescue' modifier"  */
    keyword_alias = 301,           /* "'alias'"  */
    keyword_BEGIN = 302,           /* "'BEGIN'"  */
    keyword_END = 303,             /* "'END'"  */
    keyword__LINE__ = 304,         /* "'__LINE__'"  */
    keyword__FILE__ = 305,         /* "'__FILE__'"  */
    keyword__ENCODING__ = 306,     /* "'__ENCODING__'"  */
    tIDENTIFIER = 307,             /* "local variable or method"  */
    tFID = 308,                    /* "method"  */
    tGVAR = 309,                   /* "global variable"  */
    tIVAR = 310,                   /* "instance variable"  */
    tCONSTANT = 311,               /* "constant"  */
    tCVAR = 312,                   /* "class variable"  */
    tLABEL_TAG = 313,              /* "label"  */
    tINTEGER = 314,                /* "integer literal"  */
    tFLOAT = 315,                  /* "float literal"  */
    tCHAR = 316,                   /* "character literal"  */
    tXSTRING = 317,                /* tXSTRING  */
    tREGEXP = 318,                 /* tREGEXP  */
    tSTRING = 319,                 /* tSTRING  */
    tSTRING_PART = 320,            /* tSTRING_PART  */
    tSTRING_MID = 321,             /* tSTRING_MID  */
    tNTH_REF = 322,                /* tNTH_REF  */
    tBACK_REF = 323,               /* tBACK_REF  */
    tREGEXP_END = 324,             /* tREGEXP_END  */
    tNUMPARAM = 325,               /* "numbered parameter"  */
    tUPLUS = 326,                  /* "unary plus"  */
    tUMINUS = 327,                 /* "unary minus"  */
    tCMP = 328,                    /* "<=>"  */
    tEQ = 329,                     /* "=="  */
    tEQQ = 330,                    /* "==="  */
    tNEQ = 331,                    /* "!="  */
    tGEQ = 332,                    /* ">="  */
    tLEQ = 333,                    /* "<="  */
    tANDOP = 334,                  /* "&&"  */
    tOROP = 335,                   /* "||"  */
    tMATCH = 336,                  /* "=~"  */
    tNMATCH = 337,                 /* "!~"  */
    tDOT2 = 338,                   /* ".."  */
    tDOT3 = 339,                   /* "..."  */
    tBDOT2 = 340,                  /* tBDOT2  */
    tBDOT3 = 341,                  /* tBDOT3  */
    tAREF = 342,                   /* tAREF  */
    tASET = 343,                   /* tASET  */
    tLSHFT = 344,                  /* "<<"  */
    tRSHFT = 345,                  /* ">>"  */
    tCOLON2 = 346,                 /* "::"  */
    tCOLON3 = 347,                 /* tCOLON3  */
    tOP_ASGN = 348,                /* tOP_ASGN  */
    tASSOC = 349,                  /* "=>"  */
    tLPAREN = 350,                 /* tLPAREN  */
    tLPAREN_ARG = 351,             /* "("  */
    tRPAREN = 352,                 /* ")"  */
    tLBRACK = 353,                 /* "["  */
    tLBRACE = 354,                 /* tLBRACE  */
    tLBRACE_ARG = 355,             /* "{"  */
    tSTAR = 356,                   /* "*"  */
    tPOW = 357,                    /* tPOW  */
    tDSTAR = 358,                  /* "**"  */
    tAMPER = 359,                  /* "&"  */
    tLAMBDA = 360,                 /* "->"  */
    tANDDOT = 361,                 /* "&."  */
    tSYMBEG = 362,                 /* "symbol"  */
    tSTRING_BEG = 363,             /* "string literal"  */
    tXSTRING_BEG = 364,            /* tXSTRING_BEG  */
    tSTRING_DVAR = 365,            /* tSTRING_DVAR  */
    tREGEXP_BEG = 366,             /* tREGEXP_BEG  */
    tWORDS_BEG = 367,              /* tWORDS_BEG  */
    tSYMBOLS_BEG = 368,            /* tSYMBOLS_BEG  */
    tLAMBEG = 369,                 /* tLAMBEG  */
    tHEREDOC_BEG = 370,            /* "here document"  */
    tHEREDOC_END = 371,            /* tHEREDOC_END  */
    tLITERAL_DELIM = 372,          /* tLITERAL_DELIM  */
    tHD_LITERAL_DELIM = 373,       /* tHD_LITERAL_DELIM  */
    tHD_STRING_PART = 374,         /* tHD_STRING_PART  */
    tHD_STRING_MID = 375,          /* tHD_STRING_MID  */
    tLOWEST = 376,                 /* tLOWEST  */
    tUMINUS_NUM = 377,             /* tUMINUS_NUM  */
    tLAST_TOKEN = 378              /* tLAST_TOKEN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 2013 "mrbgems/mruby-compiler/core/parse.y"

    node *nd;
    mrb_sym id;
    int num;
    stack_type stack;
    const struct vtable *vars;

#line 2250 "mrbgems/mruby-compiler/core/y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (parser_state *p);




/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_keyword_class = 3,              /* "'class'"  */
  YYSYMBOL_keyword_module = 4,             /* "'module'"  */
  YYSYMBOL_keyword_def = 5,                /* "'def'"  */
  YYSYMBOL_keyword_begin = 6,              /* "'begin'"  */
  YYSYMBOL_keyword_if = 7,                 /* "'if'"  */
  YYSYMBOL_keyword_unless = 8,             /* "'unless'"  */
  YYSYMBOL_keyword_while = 9,              /* "'while'"  */
  YYSYMBOL_keyword_until = 10,             /* "'until'"  */
  YYSYMBOL_keyword_for = 11,               /* "'for'"  */
  YYSYMBOL_keyword_undef = 12,             /* "'undef'"  */
  YYSYMBOL_keyword_rescue = 13,            /* "'rescue'"  */
  YYSYMBOL_keyword_ensure = 14,            /* "'ensure'"  */
  YYSYMBOL_keyword_end = 15,               /* "'end'"  */
  YYSYMBOL_keyword_then = 16,              /* "'then'"  */
  YYSYMBOL_keyword_elsif = 17,             /* "'elsif'"  */
  YYSYMBOL_keyword_else = 18,              /* "'else'"  */
  YYSYMBOL_keyword_case = 19,              /* "'case'"  */
  YYSYMBOL_keyword_when = 20,              /* "'when'"  */
  YYSYMBOL_keyword_break = 21,             /* "'break'"  */
  YYSYMBOL_keyword_next = 22,              /* "'next'"  */
  YYSYMBOL_keyword_redo = 23,              /* "'redo'"  */
  YYSYMBOL_keyword_retry = 24,             /* "'retry'"  */
  YYSYMBOL_keyword_in = 25,                /* "'in'"  */
  YYSYMBOL_keyword_do = 26,                /* "'do'"  */
  YYSYMBOL_keyword_do_cond = 27,           /* "'do' for condition"  */
  YYSYMBOL_keyword_do_block = 28,          /* "'do' for block"  */
  YYSYMBOL_keyword_do_LAMBDA = 29,         /* "'do' for lambda"  */
  YYSYMBOL_keyword_return = 30,            /* "'return'"  */
  YYSYMBOL_keyword_yield = 31,             /* "'yield'"  */
  YYSYMBOL_keyword_super = 32,             /* "'super'"  */
  YYSYMBOL_keyword_self = 33,              /* "'self'"  */
  YYSYMBOL_keyword_nil = 34,               /* "'nil'"  */
  YYSYMBOL_keyword_true = 35,              /* "'true'"  */
  YYSYMBOL_keyword_false = 36,             /* "'false'"  */
  YYSYMBOL_keyword_and = 37,               /* "'and'"  */
  YYSYMBOL_keyword_or = 38,                /* "'or'"  */
  YYSYMBOL_keyword_not = 39,               /* "'not'"  */
  YYSYMBOL_keyword_defined = 40,           /* "'defined?'"  */
  YYSYMBOL_modifier_if = 41,               /* "'if' modifier"  */
  YYSYMBOL_modifier_unless = 42,           /* "'unless' modifier"  */
  YYSYMBOL_modifier_while = 43,            /* "'while' modifier"  */
  YYSYMBOL_modifier_until = 44,            /* "'until' modifier"  */
  YYSYMBOL_modifier_rescue = 45,           /* "'rescue' modifier"  */
  YYSYMBOL_keyword_alias = 46,             /* "'alias'"  */
  YYSYMBOL_keyword_BEGIN = 47,             /* "'BEGIN'"  */
  YYSYMBOL_keyword_END = 48,               /* "'END'"  */
  YYSYMBOL_keyword__LINE__ = 49,           /* "'__LINE__'"  */
  YYSYMBOL_keyword__FILE__ = 50,           /* "'__FILE__'"  */
  YYSYMBOL_keyword__ENCODING__ = 51,       /* "'__ENCODING__'"  */
  YYSYMBOL_tIDENTIFIER = 52,               /* "local variable or method"  */
  YYSYMBOL_tFID = 53,                      /* "method"  */
  YYSYMBOL_tGVAR = 54,                     /* "global variable"  */
  YYSYMBOL_tIVAR = 55,                     /* "instance variable"  */
  YYSYMBOL_tCONSTANT = 56,                 /* "constant"  */
  YYSYMBOL_tCVAR = 57,                     /* "class variable"  */
  YYSYMBOL_tLABEL_TAG = 58,                /* "label"  */
  YYSYMBOL_tINTEGER = 59,                  /* "integer literal"  */
  YYSYMBOL_tFLOAT = 60,                    /* "float literal"  */
  YYSYMBOL_tCHAR = 61,                     /* "character literal"  */
  YYSYMBOL_tXSTRING = 62,                  /* tXSTRING  */
  YYSYMBOL_tREGEXP = 63,                   /* tREGEXP  */
  YYSYMBOL_tSTRING = 64,                   /* tSTRING  */
  YYSYMBOL_tSTRING_PART = 65,              /* tSTRING_PART  */
  YYSYMBOL_tSTRING_MID = 66,               /* tSTRING_MID  */
  YYSYMBOL_tNTH_REF = 67,                  /* tNTH_REF  */
  YYSYMBOL_tBACK_REF = 68,                 /* tBACK_REF  */
  YYSYMBOL_tREGEXP_END = 69,               /* tREGEXP_END  */
  YYSYMBOL_tNUMPARAM = 70,                 /* "numbered parameter"  */
  YYSYMBOL_tUPLUS = 71,                    /* "unary plus"  */
  YYSYMBOL_tUMINUS = 72,                   /* "unary minus"  */
  YYSYMBOL_tCMP = 73,                      /* "<=>"  */
  YYSYMBOL_tEQ = 74,                       /* "=="  */
  YYSYMBOL_tEQQ = 75,                      /* "==="  */
  YYSYMBOL_tNEQ = 76,                      /* "!="  */
  YYSYMBOL_tGEQ = 77,                      /* ">="  */
  YYSYMBOL_tLEQ = 78,                      /* "<="  */
  YYSYMBOL_tANDOP = 79,                    /* "&&"  */
  YYSYMBOL_tOROP = 80,                     /* "||"  */
  YYSYMBOL_tMATCH = 81,                    /* "=~"  */
  YYSYMBOL_tNMATCH = 82,                   /* "!~"  */
  YYSYMBOL_tDOT2 = 83,                     /* ".."  */
  YYSYMBOL_tDOT3 = 84,                     /* "..."  */
  YYSYMBOL_tBDOT2 = 85,                    /* tBDOT2  */
  YYSYMBOL_tBDOT3 = 86,                    /* tBDOT3  */
  YYSYMBOL_tAREF = 87,                     /* tAREF  */
  YYSYMBOL_tASET = 88,                     /* tASET  */
  YYSYMBOL_tLSHFT = 89,                    /* "<<"  */
  YYSYMBOL_tRSHFT = 90,                    /* ">>"  */
  YYSYMBOL_tCOLON2 = 91,                   /* "::"  */
  YYSYMBOL_tCOLON3 = 92,                   /* tCOLON3  */
  YYSYMBOL_tOP_ASGN = 93,                  /* tOP_ASGN  */
  YYSYMBOL_tASSOC = 94,                    /* "=>"  */
  YYSYMBOL_tLPAREN = 95,                   /* tLPAREN  */
  YYSYMBOL_tLPAREN_ARG = 96,               /* "("  */
  YYSYMBOL_tRPAREN = 97,                   /* ")"  */
  YYSYMBOL_tLBRACK = 98,                   /* "["  */
  YYSYMBOL_tLBRACE = 99,                   /* tLBRACE  */
  YYSYMBOL_tLBRACE_ARG = 100,              /* "{"  */
  YYSYMBOL_tSTAR = 101,                    /* "*"  */
  YYSYMBOL_tPOW = 102,                     /* tPOW  */
  YYSYMBOL_tDSTAR = 103,                   /* "**"  */
  YYSYMBOL_tAMPER = 104,                   /* "&"  */
  YYSYMBOL_tLAMBDA = 105,                  /* "->"  */
  YYSYMBOL_tANDDOT = 106,                  /* "&."  */
  YYSYMBOL_tSYMBEG = 107,                  /* "symbol"  */
  YYSYMBOL_tSTRING_BEG = 108,              /* "string literal"  */
  YYSYMBOL_tXSTRING_BEG = 109,             /* tXSTRING_BEG  */
  YYSYMBOL_tSTRING_DVAR = 110,             /* tSTRING_DVAR  */
  YYSYMBOL_tREGEXP_BEG = 111,              /* tREGEXP_BEG  */
  YYSYMBOL_tWORDS_BEG = 112,               /* tWORDS_BEG  */
  YYSYMBOL_tSYMBOLS_BEG = 113,             /* tSYMBOLS_BEG  */
  YYSYMBOL_tLAMBEG = 114,                  /* tLAMBEG  */
  YYSYMBOL_tHEREDOC_BEG = 115,             /* "here document"  */
  YYSYMBOL_tHEREDOC_END = 116,             /* tHEREDOC_END  */
  YYSYMBOL_tLITERAL_DELIM = 117,           /* tLITERAL_DELIM  */
  YYSYMBOL_tHD_LITERAL_DELIM = 118,        /* tHD_LITERAL_DELIM  */
  YYSYMBOL_tHD_STRING_PART = 119,          /* tHD_STRING_PART  */
  YYSYMBOL_tHD_STRING_MID = 120,           /* tHD_STRING_MID  */
  YYSYMBOL_tLOWEST = 121,                  /* tLOWEST  */
  YYSYMBOL_122_ = 122,                     /* '='  */
  YYSYMBOL_123_ = 123,                     /* '?'  */
  YYSYMBOL_124_ = 124,                     /* ':'  */
  YYSYMBOL_125_ = 125,                     /* '>'  */
  YYSYMBOL_126_ = 126,                     /* '<'  */
  YYSYMBOL_127_ = 127,                     /* '|'  */
  YYSYMBOL_128_ = 128,                     /* '^'  */
  YYSYMBOL_129_ = 129,                     /* '&'  */
  YYSYMBOL_130_ = 130,                     /* '+'  */
  YYSYMBOL_131_ = 131,                     /* '-'  */
  YYSYMBOL_132_ = 132,                     /* '*'  */
  YYSYMBOL_133_ = 133,                     /* '/'  */
  YYSYMBOL_134_ = 134,                     /* '%'  */
  YYSYMBOL_tUMINUS_NUM = 135,              /* tUMINUS_NUM  */
  YYSYMBOL_136_ = 136,                     /* '!'  */
  YYSYMBOL_137_ = 137,                     /* '~'  */
  YYSYMBOL_tLAST_TOKEN = 138,              /* tLAST_TOKEN  */
  YYSYMBOL_139_ = 139,                     /* '{'  */
  YYSYMBOL_140_ = 140,                     /* '}'  */
  YYSYMBOL_141_ = 141,                     /* '['  */
  YYSYMBOL_142_ = 142,                     /* ']'  */
  YYSYMBOL_143_ = 143,                     /* ','  */
  YYSYMBOL_144_ = 144,                     /* '`'  */
  YYSYMBOL_145_ = 145,                     /* '('  */
  YYSYMBOL_146_ = 146,                     /* ')'  */
  YYSYMBOL_147_ = 147,                     /* ';'  */
  YYSYMBOL_148_ = 148,                     /* '.'  */
  YYSYMBOL_149_n_ = 149,                   /* '\n'  */
  YYSYMBOL_YYACCEPT = 150,                 /* $accept  */
  YYSYMBOL_151_1 = 151,                    /* $@1  */
  YYSYMBOL_program = 152,                  /* program  */
  YYSYMBOL_top_compstmt = 153,             /* top_compstmt  */
  YYSYMBOL_top_stmts = 154,                /* top_stmts  */
  YYSYMBOL_top_stmt = 155,                 /* top_stmt  */
  YYSYMBOL_156_2 = 156,                    /* @2  */
  YYSYMBOL_bodystmt = 157,                 /* bodystmt  */
  YYSYMBOL_compstmt = 158,                 /* compstmt  */
  YYSYMBOL_stmts = 159,                    /* stmts  */
  YYSYMBOL_160_3 = 160,                    /* $@3  */
  YYSYMBOL_stmt = 161,                     /* stmt  */
  YYSYMBOL_command_asgn = 162,             /* command_asgn  */
  YYSYMBOL_command_rhs = 163,              /* command_rhs  */
  YYSYMBOL_expr = 164,                     /* expr  */
  YYSYMBOL_165_4 = 165,                    /* $@4  */
  YYSYMBOL_166_5 = 166,                    /* $@5  */
  YYSYMBOL_defn_head = 167,                /* defn_head  */
  YYSYMBOL_168_6 = 168,                    /* $@6  */
  YYSYMBOL_defs_head = 169,                /* defs_head  */
  YYSYMBOL_expr_value = 170,               /* expr_value  */
  YYSYMBOL_command_call = 171,             /* command_call  */
  YYSYMBOL_block_command = 172,            /* block_command  */
  YYSYMBOL_173_7 = 173,                    /* $@7  */
  YYSYMBOL_cmd_brace_block = 174,          /* cmd_brace_block  */
  YYSYMBOL_command = 175,                  /* command  */
  YYSYMBOL_mlhs = 176,                     /* mlhs  */
  YYSYMBOL_mlhs_inner = 177,               /* mlhs_inner  */
  YYSYMBOL_mlhs_basic = 178,               /* mlhs_basic  */
  YYSYMBOL_mlhs_item = 179,                /* mlhs_item  */
  YYSYMBOL_mlhs_list = 180,                /* mlhs_list  */
  YYSYMBOL_mlhs_post = 181,                /* mlhs_post  */
  YYSYMBOL_mlhs_node = 182,                /* mlhs_node  */
  YYSYMBOL_lhs = 183,                      /* lhs  */
  YYSYMBOL_cname = 184,                    /* cname  */
  YYSYMBOL_cpath = 185,                    /* cpath  */
  YYSYMBOL_fname = 186,                    /* fname  */
  YYSYMBOL_fsym = 187,                     /* fsym  */
  YYSYMBOL_undef_list = 188,               /* undef_list  */
  YYSYMBOL_189_8 = 189,                    /* $@8  */
  YYSYMBOL_op = 190,                       /* op  */
  YYSYMBOL_reswords = 191,                 /* reswords  */
  YYSYMBOL_arg = 192,                      /* arg  */
  YYSYMBOL_aref_args = 193,                /* aref_args  */
  YYSYMBOL_arg_rhs = 194,                  /* arg_rhs  */
  YYSYMBOL_paren_args = 195,               /* paren_args  */
  YYSYMBOL_opt_paren_args = 196,           /* opt_paren_args  */
  YYSYMBOL_opt_call_args = 197,            /* opt_call_args  */
  YYSYMBOL_call_args = 198,                /* call_args  */
  YYSYMBOL_199_9 = 199,                    /* @9  */
  YYSYMBOL_command_args = 200,             /* command_args  */
  YYSYMBOL_block_arg = 201,                /* block_arg  */
  YYSYMBOL_opt_block_arg = 202,            /* opt_block_arg  */
  YYSYMBOL_comma = 203,                    /* comma  */
  YYSYMBOL_args = 204,                     /* args  */
  YYSYMBOL_mrhs = 205,                     /* mrhs  */
  YYSYMBOL_primary = 206,                  /* primary  */
  YYSYMBOL_207_10 = 207,                   /* @10  */
  YYSYMBOL_208_11 = 208,                   /* @11  */
  YYSYMBOL_209_12 = 209,                   /* $@12  */
  YYSYMBOL_210_13 = 210,                   /* $@13  */
  YYSYMBOL_211_14 = 211,                   /* @14  */
  YYSYMBOL_212_15 = 212,                   /* @15  */
  YYSYMBOL_213_16 = 213,                   /* $@16  */
  YYSYMBOL_214_17 = 214,                   /* $@17  */
  YYSYMBOL_215_18 = 215,                   /* $@18  */
  YYSYMBOL_216_19 = 216,                   /* $@19  */
  YYSYMBOL_217_20 = 217,                   /* $@20  */
  YYSYMBOL_218_21 = 218,                   /* $@21  */
  YYSYMBOL_219_22 = 219,                   /* @22  */
  YYSYMBOL_220_23 = 220,                   /* @23  */
  YYSYMBOL_221_24 = 221,                   /* @24  */
  YYSYMBOL_222_25 = 222,                   /* @25  */
  YYSYMBOL_primary_value = 223,            /* primary_value  */
  YYSYMBOL_then = 224,                     /* then  */
  YYSYMBOL_do = 225,                       /* do  */
  YYSYMBOL_if_tail = 226,                  /* if_tail  */
  YYSYMBOL_opt_else = 227,                 /* opt_else  */
  YYSYMBOL_for_var = 228,                  /* for_var  */
  YYSYMBOL_f_margs = 229,                  /* f_margs  */
  YYSYMBOL_230_26 = 230,                   /* $@26  */
  YYSYMBOL_block_args_tail = 231,          /* block_args_tail  */
  YYSYMBOL_opt_block_args_tail = 232,      /* opt_block_args_tail  */
  YYSYMBOL_block_param = 233,              /* block_param  */
  YYSYMBOL_opt_block_param = 234,          /* opt_block_param  */
  YYSYMBOL_235_27 = 235,                   /* $@27  */
  YYSYMBOL_block_param_def = 236,          /* block_param_def  */
  YYSYMBOL_opt_bv_decl = 237,              /* opt_bv_decl  */
  YYSYMBOL_bv_decls = 238,                 /* bv_decls  */
  YYSYMBOL_bvar = 239,                     /* bvar  */
  YYSYMBOL_f_larglist = 240,               /* f_larglist  */
  YYSYMBOL_lambda_body = 241,              /* lambda_body  */
  YYSYMBOL_242_28 = 242,                   /* @28  */
  YYSYMBOL_do_block = 243,                 /* do_block  */
  YYSYMBOL_block_call = 244,               /* block_call  */
  YYSYMBOL_method_call = 245,              /* method_call  */
  YYSYMBOL_246_29 = 246,                   /* @29  */
  YYSYMBOL_brace_block = 247,              /* brace_block  */
  YYSYMBOL_248_30 = 248,                   /* @30  */
  YYSYMBOL_case_body = 249,                /* case_body  */
  YYSYMBOL_cases = 250,                    /* cases  */
  YYSYMBOL_in_clauses = 251,               /* in_clauses  */
  YYSYMBOL_252_31 = 252,                   /* $@31  */
  YYSYMBOL_253_32 = 253,                   /* $@32  */
  YYSYMBOL_254_33 = 254,                   /* $@33  */
  YYSYMBOL_p_expr = 255,                   /* p_expr  */
  YYSYMBOL_p_args_head = 256,              /* p_args_head  */
  YYSYMBOL_p_args_post = 257,              /* p_args_post  */
  YYSYMBOL_p_as = 258,                     /* p_as  */
  YYSYMBOL_p_alt = 259,                    /* p_alt  */
  YYSYMBOL_p_value = 260,                  /* p_value  */
  YYSYMBOL_p_array = 261,                  /* p_array  */
  YYSYMBOL_p_array_body = 262,             /* p_array_body  */
  YYSYMBOL_p_array_elems = 263,            /* p_array_elems  */
  YYSYMBOL_p_rest = 264,                   /* p_rest  */
  YYSYMBOL_p_const = 265,                  /* p_const  */
  YYSYMBOL_p_hash = 266,                   /* p_hash  */
  YYSYMBOL_p_hash_body = 267,              /* p_hash_body  */
  YYSYMBOL_p_hash_elems = 268,             /* p_hash_elems  */
  YYSYMBOL_p_hash_elem = 269,              /* p_hash_elem  */
  YYSYMBOL_p_kwrest = 270,                 /* p_kwrest  */
  YYSYMBOL_p_var = 271,                    /* p_var  */
  YYSYMBOL_opt_rescue = 272,               /* opt_rescue  */
  YYSYMBOL_exc_list = 273,                 /* exc_list  */
  YYSYMBOL_exc_var = 274,                  /* exc_var  */
  YYSYMBOL_opt_ensure = 275,               /* opt_ensure  */
  YYSYMBOL_literal = 276,                  /* literal  */
  YYSYMBOL_string = 277,                   /* string  */
  YYSYMBOL_string_fragment = 278,          /* string_fragment  */
  YYSYMBOL_string_rep = 279,               /* string_rep  */
  YYSYMBOL_string_interp = 280,            /* string_interp  */
  YYSYMBOL_281_34 = 281,                   /* @34  */
  YYSYMBOL_xstring = 282,                  /* xstring  */
  YYSYMBOL_regexp = 283,                   /* regexp  */
  YYSYMBOL_heredoc = 284,                  /* heredoc  */
  YYSYMBOL_heredoc_bodies = 285,           /* heredoc_bodies  */
  YYSYMBOL_heredoc_body = 286,             /* heredoc_body  */
  YYSYMBOL_heredoc_string_rep = 287,       /* heredoc_string_rep  */
  YYSYMBOL_heredoc_string_interp = 288,    /* heredoc_string_interp  */
  YYSYMBOL_289_35 = 289,                   /* @35  */
  YYSYMBOL_words = 290,                    /* words  */
  YYSYMBOL_symbol = 291,                   /* symbol  */
  YYSYMBOL_basic_symbol = 292,             /* basic_symbol  */
  YYSYMBOL_sym = 293,                      /* sym  */
  YYSYMBOL_symbols = 294,                  /* symbols  */
  YYSYMBOL_numeric = 295,                  /* numeric  */
  YYSYMBOL_variable = 296,                 /* variable  */
  YYSYMBOL_var_lhs = 297,                  /* var_lhs  */
  YYSYMBOL_var_ref = 298,                  /* var_ref  */
  YYSYMBOL_backref = 299,                  /* backref  */
  YYSYMBOL_superclass = 300,               /* superclass  */
  YYSYMBOL_301_36 = 301,                   /* $@36  */
  YYSYMBOL_f_opt_arglist_paren = 302,      /* f_opt_arglist_paren  */
  YYSYMBOL_f_arglist_paren = 303,          /* f_arglist_paren  */
  YYSYMBOL_f_arglist = 304,                /* f_arglist  */
  YYSYMBOL_f_label = 305,                  /* f_label  */
  YYSYMBOL_f_kw = 306,                     /* f_kw  */
  YYSYMBOL_f_block_kw = 307,               /* f_block_kw  */
  YYSYMBOL_f_block_kwarg = 308,            /* f_block_kwarg  */
  YYSYMBOL_f_kwarg = 309,                  /* f_kwarg  */
  YYSYMBOL_kwrest_mark = 310,              /* kwrest_mark  */
  YYSYMBOL_f_kwrest = 311,                 /* f_kwrest  */
  YYSYMBOL_args_tail = 312,                /* args_tail  */
  YYSYMBOL_opt_args_tail = 313,            /* opt_args_tail  */
  YYSYMBOL_f_args = 314,                   /* f_args  */
  YYSYMBOL_f_bad_arg = 315,                /* f_bad_arg  */
  YYSYMBOL_f_norm_arg = 316,               /* f_norm_arg  */
  YYSYMBOL_f_arg_item = 317,               /* f_arg_item  */
  YYSYMBOL_318_37 = 318,                   /* @37  */
  YYSYMBOL_f_arg = 319,                    /* f_arg  */
  YYSYMBOL_f_opt_asgn = 320,               /* f_opt_asgn  */
  YYSYMBOL_f_opt = 321,                    /* f_opt  */
  YYSYMBOL_f_block_opt = 322,              /* f_block_opt  */
  YYSYMBOL_f_block_optarg = 323,           /* f_block_optarg  */
  YYSYMBOL_f_optarg = 324,                 /* f_optarg  */
  YYSYMBOL_restarg_mark = 325,             /* restarg_mark  */
  YYSYMBOL_f_rest_arg = 326,               /* f_rest_arg  */
  YYSYMBOL_blkarg_mark = 327,              /* blkarg_mark  */
  YYSYMBOL_f_block_arg = 328,              /* f_block_arg  */
  YYSYMBOL_opt_f_block_arg = 329,          /* opt_f_block_arg  */
  YYSYMBOL_singleton = 330,                /* singleton  */
  YYSYMBOL_331_38 = 331,                   /* $@38  */
  YYSYMBOL_assoc_list = 332,               /* assoc_list  */
  YYSYMBOL_assocs = 333,                   /* assocs  */
  YYSYMBOL_assoc = 334,                    /* assoc  */
  YYSYMBOL_operation = 335,                /* operation  */
  YYSYMBOL_operation2 = 336,               /* operation2  */
  YYSYMBOL_operation3 = 337,               /* operation3  */
  YYSYMBOL_dot_or_colon = 338,             /* dot_or_colon  */
  YYSYMBOL_call_op = 339,                  /* call_op  */
  YYSYMBOL_call_op2 = 340,                 /* call_op2  */
  YYSYMBOL_opt_terms = 341,                /* opt_terms  */
  YYSYMBOL_opt_nl = 342,                   /* opt_nl  */
  YYSYMBOL_rparen = 343,                   /* rparen  */
  YYSYMBOL_trailer = 344,                  /* trailer  */
  YYSYMBOL_term = 345,                     /* term  */
  YYSYMBOL_nl = 346,                       /* nl  */
  YYSYMBOL_terms = 347,                    /* terms  */
  YYSYMBOL_none = 348                      /* none  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  107
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   14200

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  150
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  199
/* YYNRULES -- Number of rules.  */
#define YYNRULES  695
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1212

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   378


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     149,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   136,     2,     2,     2,   134,   129,     2,
     145,   146,   132,   130,   143,   131,   148,   133,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   124,   147,
     126,   122,   125,   123,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   141,     2,   142,   128,     2,   144,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   139,   127,   140,   137,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   135,   138
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  2189,  2189,  2189,  2199,  2205,  2209,  2213,  2217,  2223,
    2225,  2224,  2238,  2264,  2270,  2274,  2278,  2282,  2288,  2288,
    2292,  2296,  2300,  2304,  2313,  2322,  2326,  2331,  2332,  2336,
    2340,  2344,  2348,  2351,  2355,  2359,  2363,  2367,  2371,  2376,
    2380,  2389,  2398,  2407,  2416,  2423,  2424,  2428,  2431,  2432,
    2436,  2440,  2444,  2448,  2452,  2452,  2458,  2458,  2464,  2467,
    2477,  2476,  2491,  2500,  2501,  2504,  2505,  2512,  2511,  2526,
    2530,  2535,  2539,  2544,  2548,  2553,  2557,  2561,  2565,  2569,
    2575,  2579,  2585,  2586,  2592,  2596,  2600,  2604,  2608,  2612,
    2616,  2620,  2624,  2628,  2634,  2635,  2641,  2645,  2651,  2655,
    2661,  2665,  2669,  2673,  2677,  2681,  2687,  2693,  2700,  2704,
    2708,  2712,  2716,  2720,  2726,  2732,  2737,  2743,  2747,  2750,
    2754,  2758,  2765,  2766,  2767,  2768,  2773,  2780,  2781,  2784,
    2788,  2788,  2794,  2795,  2796,  2797,  2798,  2799,  2800,  2801,
    2802,  2803,  2804,  2805,  2806,  2807,  2808,  2809,  2810,  2811,
    2812,  2813,  2814,  2815,  2816,  2817,  2818,  2819,  2820,  2821,
    2822,  2823,  2826,  2826,  2826,  2827,  2827,  2828,  2828,  2828,
    2829,  2829,  2829,  2829,  2830,  2830,  2830,  2830,  2831,  2831,
    2831,  2832,  2832,  2832,  2832,  2833,  2833,  2833,  2833,  2834,
    2834,  2834,  2834,  2835,  2835,  2835,  2835,  2836,  2836,  2836,
    2836,  2837,  2837,  2840,  2844,  2848,  2852,  2856,  2860,  2864,
    2869,  2874,  2879,  2883,  2887,  2891,  2895,  2899,  2903,  2907,
    2911,  2915,  2919,  2923,  2927,  2931,  2935,  2939,  2943,  2947,
    2951,  2955,  2959,  2963,  2967,  2971,  2975,  2979,  2983,  2987,
    2991,  2995,  2999,  3003,  3007,  3011,  3015,  3019,  3023,  3027,
    3036,  3045,  3054,  3063,  3069,  3070,  3074,  3078,  3084,  3088,
    3095,  3099,  3108,  3125,  3126,  3129,  3130,  3131,  3135,  3139,
    3145,  3150,  3154,  3158,  3162,  3168,  3168,  3179,  3183,  3189,
    3193,  3199,  3202,  3207,  3211,  3215,  3220,  3224,  3230,  3235,
    3239,  3245,  3246,  3250,  3254,  3255,  3256,  3257,  3258,  3263,
    3262,  3274,  3278,  3273,  3283,  3283,  3287,  3291,  3295,  3299,
    3303,  3307,  3311,  3315,  3319,  3323,  3327,  3331,  3332,  3338,
    3345,  3337,  3358,  3366,  3374,  3374,  3374,  3381,  3381,  3381,
    3388,  3394,  3398,  3407,  3416,  3426,  3428,  3425,  3437,  3435,
    3453,  3458,  3451,  3475,  3473,  3489,  3499,  3510,  3514,  3518,
    3522,  3528,  3535,  3536,  3537,  3540,  3541,  3544,  3545,  3553,
    3554,  3560,  3564,  3567,  3571,  3575,  3579,  3584,  3588,  3592,
    3596,  3602,  3601,  3611,  3615,  3619,  3623,  3629,  3634,  3639,
    3643,  3647,  3651,  3655,  3659,  3663,  3667,  3671,  3675,  3679,
    3683,  3687,  3691,  3695,  3701,  3706,  3713,  3713,  3717,  3722,
    3728,  3732,  3738,  3739,  3742,  3747,  3750,  3754,  3760,  3764,
    3771,  3770,  3787,  3792,  3796,  3801,  3808,  3812,  3816,  3820,
    3824,  3828,  3832,  3836,  3840,  3847,  3846,  3861,  3860,  3876,
    3884,  3893,  3898,  3902,  3902,  3907,  3907,  3912,  3912,  3922,
    3923,  3927,  3931,  3935,  3939,  3943,  3948,  3953,  3961,  3965,
    3972,  3976,  3982,  3983,  3989,  3990,  3996,  3997,  4001,  4005,
    4009,  4013,  4017,  4021,  4025,  4026,  4027,  4034,  4038,  4045,
    4050,  4055,  4060,  4065,  4070,  4078,  4082,  4089,  4093,  4101,
    4105,  4109,  4116,  4120,  4127,  4131,  4135,  4142,  4146,  4155,
    4160,  4168,  4172,  4177,  4184,  4190,  4197,  4200,  4204,  4205,
    4208,  4212,  4215,  4219,  4222,  4223,  4224,  4225,  4228,  4229,
    4235,  4240,  4245,  4250,  4256,  4257,  4263,  4269,  4268,  4280,
    4284,  4290,  4294,  4300,  4309,  4320,  4323,  4324,  4327,  4333,
    4339,  4340,  4343,  4350,  4349,  4364,  4368,  4376,  4380,  4392,
    4399,  4406,  4407,  4408,  4409,  4410,  4414,  4420,  4424,  4432,
    4433,  4434,  4438,  4444,  4448,  4452,  4456,  4460,  4466,  4470,
    4476,  4480,  4484,  4488,  4492,  4496,  4500,  4508,  4515,  4521,
    4522,  4526,  4530,  4529,  4546,  4547,  4550,  4556,  4560,  4566,
    4567,  4571,  4575,  4581,  4587,  4595,  4601,  4608,  4614,  4621,
    4625,  4631,  4635,  4641,  4642,  4645,  4649,  4655,  4659,  4663,
    4667,  4673,  4677,  4682,  4687,  4691,  4695,  4699,  4703,  4707,
    4711,  4715,  4719,  4723,  4727,  4731,  4735,  4739,  4744,  4750,
    4755,  4760,  4765,  4770,  4777,  4781,  4788,  4793,  4792,  4804,
    4808,  4814,  4822,  4830,  4838,  4842,  4848,  4852,  4858,  4859,
    4862,  4867,  4874,  4875,  4878,  4882,  4886,  4892,  4896,  4900,
    4906,  4912,  4912,  4919,  4920,  4926,  4930,  4936,  4942,  4947,
    4951,  4956,  4961,  4977,  4982,  4988,  4989,  4990,  4993,  4994,
    4995,  4996,  4999,  5000,  5001,  5004,  5005,  5008,  5012,  5018,
    5019,  5025,  5026,  5029,  5030,  5033,  5036,  5037,  5038,  5041,
    5042,  5045,  5050,  5053,  5054,  5058
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "\"'class'\"",
  "\"'module'\"", "\"'def'\"", "\"'begin'\"", "\"'if'\"", "\"'unless'\"",
  "\"'while'\"", "\"'until'\"", "\"'for'\"", "\"'undef'\"", "\"'rescue'\"",
  "\"'ensure'\"", "\"'end'\"", "\"'then'\"", "\"'elsif'\"", "\"'else'\"",
  "\"'case'\"", "\"'when'\"", "\"'break'\"", "\"'next'\"", "\"'redo'\"",
  "\"'retry'\"", "\"'in'\"", "\"'do'\"", "\"'do' for condition\"",
  "\"'do' for block\"", "\"'do' for lambda\"", "\"'return'\"",
  "\"'yield'\"", "\"'super'\"", "\"'self'\"", "\"'nil'\"", "\"'true'\"",
  "\"'false'\"", "\"'and'\"", "\"'or'\"", "\"'not'\"", "\"'defined?'\"",
  "\"'if' modifier\"", "\"'unless' modifier\"", "\"'while' modifier\"",
  "\"'until' modifier\"", "\"'rescue' modifier\"", "\"'alias'\"",
  "\"'BEGIN'\"", "\"'END'\"", "\"'__LINE__'\"", "\"'__FILE__'\"",
  "\"'__ENCODING__'\"", "\"local variable or method\"", "\"method\"",
  "\"global variable\"", "\"instance variable\"", "\"constant\"",
  "\"class variable\"", "\"label\"", "\"integer literal\"",
  "\"float literal\"", "\"character literal\"", "tXSTRING", "tREGEXP",
  "tSTRING", "tSTRING_PART", "tSTRING_MID", "tNTH_REF", "tBACK_REF",
  "tREGEXP_END", "\"numbered parameter\"", "\"unary plus\"",
  "\"unary minus\"", "\"<=>\"", "\"==\"", "\"===\"", "\"!=\"", "\">=\"",
  "\"<=\"", "\"&&\"", "\"||\"", "\"=~\"", "\"!~\"", "\"..\"", "\"...\"",
  "tBDOT2", "tBDOT3", "tAREF", "tASET", "\"<<\"", "\">>\"", "\"::\"",
  "tCOLON3", "tOP_ASGN", "\"=>\"", "tLPAREN", "\"(\"", "\")\"", "\"[\"",
  "tLBRACE", "\"{\"", "\"*\"", "tPOW", "\"**\"", "\"&\"", "\"->\"",
  "\"&.\"", "\"symbol\"", "\"string literal\"", "tXSTRING_BEG",
  "tSTRING_DVAR", "tREGEXP_BEG", "tWORDS_BEG", "tSYMBOLS_BEG", "tLAMBEG",
  "\"here document\"", "tHEREDOC_END", "tLITERAL_DELIM",
  "tHD_LITERAL_DELIM", "tHD_STRING_PART", "tHD_STRING_MID", "tLOWEST",
  "'='", "'?'", "':'", "'>'", "'<'", "'|'", "'^'", "'&'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "tUMINUS_NUM", "'!'", "'~'", "tLAST_TOKEN", "'{'",
  "'}'", "'['", "']'", "','", "'`'", "'('", "')'", "';'", "'.'", "'\\n'",
  "$accept", "$@1", "program", "top_compstmt", "top_stmts", "top_stmt",
  "@2", "bodystmt", "compstmt", "stmts", "$@3", "stmt", "command_asgn",
  "command_rhs", "expr", "$@4", "$@5", "defn_head", "$@6", "defs_head",
  "expr_value", "command_call", "block_command", "$@7", "cmd_brace_block",
  "command", "mlhs", "mlhs_inner", "mlhs_basic", "mlhs_item", "mlhs_list",
  "mlhs_post", "mlhs_node", "lhs", "cname", "cpath", "fname", "fsym",
  "undef_list", "$@8", "op", "reswords", "arg", "aref_args", "arg_rhs",
  "paren_args", "opt_paren_args", "opt_call_args", "call_args", "@9",
  "command_args", "block_arg", "opt_block_arg", "comma", "args", "mrhs",
  "primary", "@10", "@11", "$@12", "$@13", "@14", "@15", "$@16", "$@17",
  "$@18", "$@19", "$@20", "$@21", "@22", "@23", "@24", "@25",
  "primary_value", "then", "do", "if_tail", "opt_else", "for_var",
  "f_margs", "$@26", "block_args_tail", "opt_block_args_tail",
  "block_param", "opt_block_param", "$@27", "block_param_def",
  "opt_bv_decl", "bv_decls", "bvar", "f_larglist", "lambda_body", "@28",
  "do_block", "block_call", "method_call", "@29", "brace_block", "@30",
  "case_body", "cases", "in_clauses", "$@31", "$@32", "$@33", "p_expr",
  "p_args_head", "p_args_post", "p_as", "p_alt", "p_value", "p_array",
  "p_array_body", "p_array_elems", "p_rest", "p_const", "p_hash",
  "p_hash_body", "p_hash_elems", "p_hash_elem", "p_kwrest", "p_var",
  "opt_rescue", "exc_list", "exc_var", "opt_ensure", "literal", "string",
  "string_fragment", "string_rep", "string_interp", "@34", "xstring",
  "regexp", "heredoc", "heredoc_bodies", "heredoc_body",
  "heredoc_string_rep", "heredoc_string_interp", "@35", "words", "symbol",
  "basic_symbol", "sym", "symbols", "numeric", "variable", "var_lhs",
  "var_ref", "backref", "superclass", "$@36", "f_opt_arglist_paren",
  "f_arglist_paren", "f_arglist", "f_label", "f_kw", "f_block_kw",
  "f_block_kwarg", "f_kwarg", "kwrest_mark", "f_kwrest", "args_tail",
  "opt_args_tail", "f_args", "f_bad_arg", "f_norm_arg", "f_arg_item",
  "@37", "f_arg", "f_opt_asgn", "f_opt", "f_block_opt", "f_block_optarg",
  "f_optarg", "restarg_mark", "f_rest_arg", "blkarg_mark", "f_block_arg",
  "opt_f_block_arg", "singleton", "$@38", "assoc_list", "assocs", "assoc",
  "operation", "operation2", "operation3", "dot_or_colon", "call_op",
  "call_op2", "opt_terms", "opt_nl", "rparen", "trailer", "term", "nl",
  "terms", "none", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-982)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-696)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -982,  4012,   164,  9524, 11666, 12011,  7819,  -982, 11309, 11309,
    -982,  -982, 11781,  9010,  7432,  9762,  9762,  -982,  -982,  9762,
    4866,  4455,  -982,  -982,  -982,  -982,   -76,    30,  9010,  -982,
      39,  -982,  -982,  -982,  7962,  4316,  -982,  -982,  8105,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,    37, 11428, 11428, 11428,
   11428,   134,  6538,  6658, 10238, 10595,  9294,  -982,  8726,  1029,
     459,   299,  1146,  1263,  -982,   240, 11547, 11428,  -982,   489,
    -982,  1514,  -982,   429,  1949,  1949,  -982,  -982,   167,   115,
    -982,   105, 11896,  -982,   146,  2372,   445,   568,    88,    60,
    -982,   251,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,   214,   248,  -982,   230,    41,  -982,  -982,  -982,  -982,
    -982,  -982,   231,   231,   -76,    30,   746,   835,  -982, 11309,
     288,  6778,   336,  2531,  2531,  -982,   253,  -982,   721,  -982,
    -982,    41,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
      10,    44,    48,   144,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,   170,   172,   173,   181,  -982,   185,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,   197,  5563,   370,   429,  1949,  1949,   295,   265,
     773,   212,   307,   270,   295, 11309, 11309,   875,   375,  -982,
    -982,   944,   387,    55,    68,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  8868,  -982,  -982,   293,  -982,  -982,
    -982,  -982,  -982,  -982,   489,  -982,   334,  -982,   419,  -982,
    -982,   489,  4592,   145, 11428, 11428, 11428, 11428,  -982, 13942,
    -982,  -982,   303,   394,   303,  -982,  -982,  -982,  9881,  -982,
    -982,  9762,  -982,  -982,  -982,  -982,  7432,  7672, 11309,  7672,
    -982,   330,  6898,  -982,   956,   378, 14066, 14066,   564,  9643,
    6538,   338,  1479,  1514,   489,   355,  -982,  6778,   489,   347,
     797,   797,  -982, 13942,   352,   797,  -982,   448, 12126,   359,
     974,   975,  1015,  1459,  -982,  -982,  -982,  -982,  -982,  1297,
    -982,  -982,  -982,  -982,  -982,  -982,   785,  1356,  -982,  -982,
     693,  -982,   358,  -982,  1359,  -982,  1390,   412,   439,  -982,
    -982,  -982,  -982,  7312, 11309, 11309, 11309, 11309,  9643, 11309,
   11309,    63,  -982,  -982,  -982,  -982,   491,   489,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  2598,   436,   438,  5563, 11428,
    -982,   423,   532,   454,  -982,   489,  -982,  -982,  -982,   460,
   11428,  -982,   468,   562,   476,   222,  -982,  -982,   501,  5563,
    -982,  -982, 10714,  -982,  6538,  9409,   486, 10714,  -982, 11428,
   11428, 11428, 11428, 11428, 11428, 11428, 11428, 11428, 11428, 11428,
   11428, 11428, 11428,  -982, 11428, 11428, 11428, 11428, 11428, 11428,
   11428, 11428, 11428, 11428, 11428, 11428,  3499,  -982,  9762,  -982,
    3623,  -982,  -982, 13524,  -982,  -982,  -982,  -982, 11547, 11547,
    -982,   551,  -982,   429,  -982,  1142,  -982,  -982,  -982,  -982,
    -982,  -982, 12406,  9762, 12492,  5563, 11309,  -982,  -982,  -982,
     616,   632,   284,   533,   536,  -982,  5710,   656, 11428, 12578,
    9762, 12664, 11428, 11428,  6151,   658,   658,    87, 12750,  9762,
   12836,  -982,   620,  -982,  6898,   331,  -982,  -982, 10833,   672,
    -982, 11428, 11428, 14004, 14004, 14004, 11428,  -982,  -982, 10000,
    -982, 11428,  -982, 10357,  7552,   544,   489,   303,   303,  -982,
    -982,   704,   550,  -982,  -982,  -982,   704,  -982,  9010,  6271,
     558, 12578, 12664, 11428,  1514,   489,  -982,  -982,  7192,   563,
    -982,  -982,  -982, 10476,  -982,   489, 10595,  -982,  -982,  -982,
    1142,   105, 12126,  -982, 12126, 12922,  9762, 13008,  2079,  -982,
    -982,   567,  -982,  1402,  6898,   785,  -982,  -982,  -982,  -982,
    -982,  -982,  -982, 11428, 11428,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  1483,   489,   489,
     577, 11547,   700, 14004,   546,  -982,  -982,  -982,   213,  -982,
    -982,  2610,  -982, 14004,  2079,  -982,  -982,  1777,  -982,  -982,
    -982, 11547,   707,    43, 11428,  -982, 13658,   303,  -982,   489,
   12126,   583,  -982,  -982,  -982,   683,   623,  2435,  -982,  -982,
    1144,   325,  2013,  3108,  3108,  3108,  3108,  1555,  1555,  3171,
    3296,  3108,  3108, 14066, 14066,  1768,  1768,  2013,   378, 14004,
    1555,  1555,  1656,  1656,  1753,   967,   967,   378,   378,   378,
    5003,  8464,  5277,  8583,  -982,   231,  -982,   614,   303,   596,
    -982,   651,  -982,  -982,  4729,  -982,  -982,  1851,    43,    43,
    -982,  2950,  -982,  -982,  -982,  -982,  -982,   489, 11309,  5563,
    1155,   735,  -982,   231,   629,   231,   765,   704,  9152,  -982,
   10952,   768,  -982, 11428, 11428,   565,  -982,  8224,  8344,   653,
     339,   350,   768,  -982,  -982,  -982,  -982,    59,    82,   655,
      91,    99, 11309,  9010,   674,  2013,   778, 14004,   344,  -982,
   14004, 14004, 14004,   634, 11428, 13942,  -982,   303, 14004,  -982,
    -982,  -982,  -982, 10119, 10357,  -982,  -982,  -982,  -982,   679,
    -982,  -982,    18,  1514,   489,   797,   486,  -982,  1155,   735,
     689,  1156,  1231,  -982,   107,  2079,  -982,   681,  -982,   378,
     378,  -982,  -982,   747,   489,   691,  -982,  -982,  2791,   753,
    4089,  -982,   780,   491,  -982,   454,  -982,   489,  -982,  -982,
     697,   699,   703,  -982,   705,   780,   703,   798, 13596,  -982,
    -982,  2079,  5563,  -982,  -982, 13729, 11071,  -982,  -982, 12126,
    9643, 11547, 11428, 13094,  9762, 13180,  -982,  -982,  -982,   792,
    -982,  -982,   799,  1673,    86,   802,   274,   807,  -982,  2143,
     722,   241,  -982,  -982,   727,   783,  -982,   732,  -982,  -982,
    -982,  -982,  -982,  -982,   787, 11547, 11547,  -982,   551,   696,
   10000, 11547, 11547,  -982,   551,    60,   167,  5563,  6898,    43,
    -982,   489,   873,  -982,  -982,  -982,  -982, 13658,  -982,   805,
    -982,  6418,   878,  -982, 11309,   881,  -982, 11428, 11428,   356,
   11428, 11428,   887,  7045,  7045,   100,   658,  -982,  -982,   190,
    -982, 11190,  5857, 14004,  -982,  7552,   303,  -982,  -982,  -982,
      71,   764,  1426,  5563,  6898,  -982,  -982,  -982,   772,  -982,
    1597,   489, 11428, 11428,  -982,  -982,  2079,  -982,  1777,  -982,
    1777,  -982,  1777,  -982,  -982, 11428, 11428,  -982,  -982,  -982,
   12241,  -982,   775,   454,   777, 12241,  -982,   781,   782,  -982,
     908, 11428, 13800,  -982,  -982, 14004,  5140,  5414,   786,   364,
     446,  2238,  -982,  -982,  -982,  -982,   791,   784,   794,   792,
    -982,   789,   795,  -982,  -982,  -982,  -982,  -982,   806,   810,
    -982,   883,  2238,  2238,   892,   119, 11428, 11428,  -982,  -982,
    -982,  -982,  -982, 11547,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,   945,   824,  6898,  5563,  -982,  -982, 12356,   295,  -982,
    -982,  7045,  -982,  -982,   295,  -982, 11428,  -982,   950,   958,
    -982, 11309, 11309,  6004, 14004,   239,  -982, 10357,  -982,  1727,
     964,   844,  1607,  1607,   986,  -982, 14004, 14004,   703,   842,
     703,   703, 14004, 14004,   861,   864,   951,  1208,   546,  -982,
    -982,  1883,  -982,  1208,  2079,  -982,  1777,  -982,  -982, 13871,
     469,  -982,  -982,  2143,  2238,  -982,   119,  -982,  2238,  -982,
    -982,   857,  -982,  -982,  -982,  -982, 14004, 14004,  -982,  -982,
    -982,  -982,   866,   994,   955,  -982,  1227,   975,  1015,  5563,
    -982,  5710,  -982,  -982,  7045,   295,   295,   166,  -982,  -982,
    -982,  -982,   858,  -982,  -982,  -982,  -982,   874,   874,  1607,
     877,  -982,  1777,  -982,  -982,  -982,  -982,  -982,  -982, 13266,
    -982,   454,   546,  -982,  -982,   885,   886,   889,  -982,   891,
     889,  -982,   893,   905,  -982,   857,  2238,  -982,  -982,  1142,
   13352,  9762, 13438,   632,   565,  1003,  6004,  6004,  2013,  -982,
    1006,  1727,   634,  1607,   874,  1607,   703,   913,   915,  -982,
    2079,  -982,  1777,  -982,  1777,  -982,  1777,  -982,  -982,  2238,
    2143,  -982,  1155,   735,   919,   418,   572,  -982,  -982,  -982,
     166,   166,   531,  -982,  -982,   874,  -982,   889,   920,   889,
     889,   921,  -982,    71,  1052,  1062,   295,  1037,  1040,  -982,
    1777,  -982,  -982,  -982,  2238,  -982,  -982,  6004, 11309, 11309,
     889,   166,   295,   295,  -982,  -982,  6004,  6004,   166,   166,
    -982,  -982
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     0,   299,     0,     0,
     324,   327,     0,     0,   681,   347,   348,   349,   350,   311,
     275,   275,   563,   562,   564,   565,   683,   683,     0,    10,
       0,   567,   566,   568,   553,   667,   555,   554,   557,   556,
     549,   550,   510,   511,   569,   570,   561,     0,     0,     0,
       0,     0,     0,     0,   695,   695,    92,   319,     0,     0,
       0,     0,     0,     0,   525,     0,     0,     0,     3,   681,
       6,     9,    27,    32,   618,   618,    48,    64,    63,     0,
      80,     0,    84,    94,     0,    58,   253,     0,    65,   317,
     291,   292,   508,   293,   294,   295,   506,   505,   537,   507,
     504,   560,     0,   296,   297,   275,     5,     1,     8,   347,
     348,   311,   695,   423,     0,     0,   117,   118,   561,     0,
       0,     0,     0,   618,   618,   120,   571,   351,     0,   560,
     297,     0,   343,   172,   183,   173,   169,   199,   200,   201,
     202,   181,   196,   189,   179,   178,   194,   177,   176,   171,
     197,   170,   184,   188,   190,   182,   175,   191,   198,   193,
     192,   185,   195,   180,   168,   187,   186,   174,   167,   165,
     166,   162,   163,   164,   122,   124,   123,   157,   158,   135,
     136,   137,   144,   141,   143,   138,   139,   159,   160,   145,
     146,   150,   153,   154,   140,   142,   132,   133,   134,   147,
     148,   149,   151,   152,   155,   156,   161,   651,    59,   125,
     126,   650,     0,     0,     0,    62,   618,   618,     0,     0,
       0,   560,     0,   297,     0,     0,     0,   116,     0,   362,
     361,     0,     0,   560,   297,   192,   185,   195,   180,   162,
     163,   164,   122,   123,     0,   127,   129,    20,   128,   528,
     533,   532,   689,   691,   681,   692,     0,   530,     0,   693,
     690,   682,   665,   561,   283,   664,   278,     0,   270,   282,
      78,   274,   695,   508,   695,   655,    79,    77,   695,   264,
     312,     0,    76,   263,   422,    75,   681,     0,     0,     0,
      18,     0,     0,   226,     0,   227,   214,   217,   308,     0,
       0,     0,     0,    15,   681,    82,    14,     0,   681,     0,
     686,   686,   254,     0,     0,   686,   653,     0,     0,    90,
       0,   100,   107,   618,   543,   542,   544,   545,   539,     0,
     541,   540,   512,   517,   516,   519,     0,     0,   514,   521,
       0,   523,     0,   535,     0,   547,     0,   551,   552,    53,
     241,   242,     4,   682,     0,     0,     0,     0,     0,     0,
       0,   625,   621,   620,   619,   622,   623,     0,   627,   639,
     593,   594,   643,   642,   638,   618,     0,   579,     0,   586,
     591,   695,   596,   695,   617,     0,   624,   626,   629,   603,
       0,   636,   603,   641,   603,   646,   600,   575,     0,     0,
     410,   412,     0,    96,     0,    88,    85,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   213,
     216,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   678,   695,   677,
       0,   680,   679,     0,   427,   425,   318,   509,     0,     0,
     416,    69,   316,   340,   117,   118,   119,   551,   552,   579,
     572,   338,     0,   695,     0,     0,     0,   676,   675,    60,
       0,   695,   308,     0,     0,   353,     0,   352,     0,     0,
     695,     0,     0,     0,     0,     0,     0,   308,     0,   695,
       0,   335,     0,   130,     0,     0,   529,   531,     0,     0,
     694,   659,   660,   284,   663,   277,     0,   683,   271,     0,
     280,     0,   272,     0,   681,     0,   681,   695,   695,   265,
     276,   681,     0,   314,    51,   684,   681,    52,     0,     0,
       0,     0,     0,     0,    17,   681,   306,    13,     0,    81,
     302,   305,   309,   688,   255,   687,   688,   257,   310,   654,
     106,    98,     0,    93,     0,     0,   695,     0,   618,   320,
     407,   603,   546,     0,     0,   520,   526,   513,   515,   522,
     524,   536,   548,     0,     0,     7,    21,    22,    23,    24,
      25,    49,    50,   583,   631,   584,   582,     0,   681,   681,
     603,     0,     0,   585,   648,   598,   649,   595,   648,   599,
     580,   602,   610,   632,   602,   613,   640,   602,   615,   645,
     644,     0,     0,   695,   283,    28,    30,     0,    31,   681,
       0,    86,    97,    47,    33,    45,     0,   258,   203,    29,
       0,   297,     0,   231,   236,   237,   238,   233,   235,   245,
     246,   239,   240,   212,   215,   243,   244,     0,   223,   683,
     232,   234,   228,   229,   230,   218,   219,   220,   221,   222,
     668,   673,   669,   674,   421,   275,   419,     0,   695,   668,
     670,   669,   671,   420,   275,   668,   669,   275,   695,   695,
      34,   258,   204,    44,   211,    67,    70,     0,     0,     0,
     117,   118,   121,     0,     0,   695,     0,   681,     0,   300,
     695,   695,   496,     0,     0,   695,   354,   672,   307,     0,
     668,   669,   695,   356,   325,   355,   328,   672,   307,     0,
     668,   669,     0,     0,     0,     0,     0,   282,     0,   331,
     658,   661,   657,   281,   286,   285,   279,   695,   662,   656,
     262,   260,   266,   267,   269,   313,   685,   315,    19,     0,
      26,   210,    83,    16,   681,   686,    99,    91,   103,   105,
       0,   102,   104,   683,     0,   602,   538,     0,   527,   224,
     225,   625,   623,   370,   681,   363,   578,   576,   602,    40,
     249,   345,     0,     0,   592,   695,   647,     0,   601,   630,
     603,   603,   603,   637,   603,   625,   603,    42,   251,   346,
     398,   396,     0,   395,   394,   290,     0,    95,    89,     0,
       0,     0,     0,     0,   695,     0,   460,   461,   462,   494,
     479,   459,     0,     0,     0,   478,   493,     0,    57,     0,
     439,   452,   454,   464,   443,   463,   465,   445,   487,   447,
     456,   458,   457,    55,     0,     0,     0,   418,    73,   424,
     267,     0,     0,   417,    71,   413,    66,     0,     0,   695,
     341,     0,     0,   424,   344,   652,    61,   497,   498,   695,
     499,     0,   695,   359,     0,     0,   357,     0,     0,   424,
       0,     0,     0,     0,     0,   424,     0,   131,   534,     0,
     330,     0,     0,   287,   273,   681,   695,    11,   303,   256,
     101,     0,   400,     0,     0,   321,   518,   371,   368,   628,
       0,   681,     0,     0,   597,   581,   602,   606,   602,   608,
     602,   614,   602,   611,   616,     0,     0,   393,   683,   683,
     588,   589,   695,   695,   378,     0,   634,   378,   378,   376,
       0,   286,   288,    87,    46,   259,   668,   669,     0,   668,
     669,   490,   481,   494,   468,   475,     0,   469,   472,     0,
     483,     0,   484,   486,   477,   492,   491,   466,   440,   441,
     448,     0,     0,     0,     0,     0,     0,     0,    39,   208,
      38,   209,    74,     0,    36,   206,    37,   207,    72,   414,
     415,     0,     0,     0,     0,   573,   339,     0,     0,   501,
     360,     0,    12,   503,     0,   322,     0,   323,     0,     0,
     336,     0,     0,     0,   285,   695,   261,   268,   406,     0,
       0,     0,     0,     0,   366,   577,    41,   250,   603,   603,
     603,   603,    43,   252,     0,     0,     0,   587,   648,   374,
     375,   378,   386,   633,     0,   389,     0,   391,   411,   289,
     424,   489,   467,     0,     0,   482,     0,   449,     0,   453,
     455,   444,   450,   480,   488,   446,   248,   247,    35,   205,
     428,   426,     0,     0,     0,   500,     0,   108,   115,     0,
     502,     0,   326,   329,     0,     0,     0,   695,   430,   431,
     429,   404,   683,   402,   405,   409,   408,   372,   369,     0,
     364,   607,   602,   604,   609,   612,   399,   397,   308,     0,
     590,   695,     0,   377,   384,   378,   378,   378,   635,   378,
     378,   476,   470,   473,   485,   442,     0,    68,   342,   114,
       0,   695,     0,   695,   695,     0,     0,     0,     0,   432,
       0,     0,   401,     0,   367,     0,   603,   672,   307,   373,
       0,   381,     0,   383,     0,   390,     0,   387,   392,     0,
       0,   451,   111,   113,     0,   668,   669,   495,   358,   337,
     695,   695,   433,   332,   403,   365,   605,   378,   378,   378,
     378,   471,   474,   109,     0,     0,     0,     0,     0,   382,
       0,   379,   385,   388,     0,   333,   334,     0,     0,     0,
     378,   695,     0,     0,   380,   434,     0,     0,   695,   695,
     436,   438
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -982,  -982,  -982,   573,  -982,    36,  -982,  -328,   -11,  -982,
    -982,    25,  -198,  -183,    15,  -982,  -982,  1172,  -982,  1739,
      -8,   -64,  -982,  -982,  -450,   157,  1067,  -159,   -16,   -49,
    -301,  -477,   -24,  2484,   -63,  1079,    14,   -15,  -982,  -982,
      42,  -982,  3396,  -982,   876,    69,    56,  -404,   132,  -982,
     -13,  -430,  -225,   391,    49,  -369,    40,  -982,  -982,  -982,
    -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,  -982,
    -982,  -982,  -982,   141,  -208,  -461,   -47,  -607,  -982,  -982,
    -982,   287,   280,  -982,  -564,  -982,  -982,  -285,  -982,   -45,
    -982,  -982,  -982,   247,  -982,  -982,  -982,   -85,  -982,  -484,
    -982,  -212,  -982,  -982,  -982,  -612,  -982,    47,   392,  -982,
     135,  -982,  -982,  -945,  -763,  -982,  -982,  -982,   285,  -809,
    -762,  -982,   -25,  -982,  -982,  -982,  -982,  -982,    72,    57,
    -238,  -982,  -982,  -982,  -982,  -982,  -278,  -982,   855,  -982,
    -982,   512,     2,  -982,  -982,   555,  2320,  2658,  1106,  1906,
    -982,  -982,   -29,   643,     7,   -99,   519,    80,  -982,  -982,
    -982,     8,   559,  -389,  -242,  -958,  -726,  -310,  -982,   221,
    -759,  -561,  -981,    90,  -545,  -982,  -526,  -982,   238,  -376,
    -982,  -982,  -982,   113,  -495,   953,  -235,  -982,  -982,   -66,
    -982,    26,   -17,   203,  -284,   545,  -275,   -32,    -1
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    68,    69,    70,   291,   470,   471,   302,
     528,   303,    72,   624,    73,   647,   632,   216,   698,   217,
     218,    76,    77,   859,   686,    78,    79,   304,    80,    81,
      82,   553,    83,   219,   125,   126,   245,   246,   247,   723,
     663,   210,    85,   309,   628,   664,   280,   515,   516,   281,
     282,   271,   508,   546,   668,   618,    86,   213,   307,   754,
     308,   323,   764,   225,   883,   226,   884,   722,  1084,   689,
     687,   994,   465,   294,   476,   714,   875,  1139,   232,   774,
    1022,  1113,  1042,   928,   802,   929,   803,   901,  1092,  1093,
     559,   905,   613,   401,    88,    89,   679,   452,   678,   499,
    1090,  1140,  1186,  1187,  1188,   828,   829,  1061,   830,   831,
     832,   833,   956,   957,   834,   835,   836,   961,   837,   838,
     839,   840,   701,   869,   998,  1002,    90,    91,    92,   337,
     338,   564,    93,    94,    95,   565,   255,   256,   257,   494,
      96,    97,    98,   331,    99,   100,   221,   222,   103,   223,
     461,   688,   376,   377,   378,   379,   380,   931,   932,   381,
     382,   383,   788,   602,   385,   386,   387,   388,   587,   389,
     390,   391,   936,   937,   392,   393,   394,   395,   396,   595,
     212,   466,   314,   518,   275,   131,   693,   666,   469,   464,
     443,   522,   902,   523,   544,   259,   260,   261,   306
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     106,   224,   349,   605,   446,   608,   254,   599,   285,   287,
     289,   726,   525,   290,   525,   248,   484,   552,   739,   283,
     208,   440,   442,   215,   215,   716,    71,   547,    71,   215,
     248,   549,   319,   406,   667,   843,   305,   353,   629,   108,
     258,   301,   935,   793,   127,   127,   398,   908,   209,   512,
     592,   739,   127,   312,   316,   209,   790,   456,   566,   694,
     958,  1094,   963,  1118,   272,   272,   969,   444,   272,   286,
     209,   612,   330,   397,   397,   791,   709,   757,   794,   736,
    -108,   560,   399,   736,  -111,   719,   444,   273,   273,   279,
     284,   273,   451,  -115,   872,   352,   127,  -424,   876,   568,
     209,  -563,   568,   310,   568,   882,   568,  -113,   568,  1123,
     301,   283,  -114,   889,   857,   858,  -110,   340,   342,   344,
     346,   583,   127,   800,  -112,  -109,   273,   273,   274,   274,
    -559,   399,   274,   589,   453,  -562,   903,   696,   959,  -564,
     -81,   535,    87,   808,    87,   128,   128,   270,   276,   220,
     220,   277,   760,   231,   481,   220,   220,   220,  -563,  -116,
     220,   -95,  -424,   447,   107,   490,  1064,   311,   315,  1118,
     801,   959,   268,   268,   450,   288,   268,  -424,   292,   441,
     445,   279,   284,  1094,   871,   584,   278,   473,   474,   826,
     298,  1138,  -562,    87,   437,   400,  -564,   320,  -100,   445,
     450,   665,  -103,   502,  -668,   674,   475,   220,   677,   623,
    -424,  -107,  -424,  1065,  1181,   397,   397,   485,   486,  -424,
     790,   904,   826,   320,   399,  -105,   960,  -669,   532,   695,
    -106,  1011,  1012,   790,  -102,  -565,   439,   402,  -559,   791,
     215,   215,  -104,  -101,   665,   619,   674,  1064,   403,   739,
     623,   623,   791,   552,   557,   695,   609,   871,   330,   498,
     220,  -567,    87,  -566,  -568,   680,   683,  -116,   407,   551,
     538,   510,  -553,   510,   610,   938,  -557,   519,   545,   545,
     495,   530,   935,   545,   305,   935,   209,   768,   467,   301,
    1122,   789,  -565,   512,  1124,   993,   540,   695,  1100,   347,
     348,   521,   524,   526,   527,  -558,   249,  -558,   965,   250,
     251,   475,    42,   736,   736,    43,   763,   372,  -567,   552,
    -566,  -568,   695,   449,   534,   568,   966,   517,   537,  -553,
     272,   868,   943,  -557,  -108,   971,  -108,   252,   615,   253,
     454,   448,   373,   625,   455,   468,   576,   577,   578,   579,
     273,   498,  -115,   273,    87,   793,   725,  -100,   127,    59,
     475,   862,   341,   483,   333,   334,   220,   220,   972,   215,
     215,   215,   215,  -107,   581,   582,   278,   533,    71,   460,
     596,   621,   596,   580,   625,   625,   563,   478,   305,   575,
    1029,   935,  -115,   301,   274,   457,   458,  1182,   982,   692,
     482,   917,   919,   921,   988,   923,  -114,   924,  1088,   914,
     948,   249,   491,   520,   250,   251,   335,   336,   449,   220,
     736,   570,   220,   333,   334,  1010,   472,   220,   220,   220,
     220,   487,   880,    87,  -110,   268,   493,   519,   268,   498,
      87,    87,   252,   881,   253,   127,   507,  -115,    87,  1006,
     496,   617,   511,   250,   251,   789,   617,   851,   525,   320,
     249,  -110,   519,   250,   251,   705,   359,   360,   789,   529,
     702,   899,  -112,   712,   940,   335,   336,   -80,  -109,   519,
     424,   697,   672,   724,   536,   672,  -110,   507,   519,   542,
     733,   252,   548,   253,    87,   220,   220,   220,   220,    87,
     220,   220,   554,   756,   550,   551,   672,   539,   552,   673,
     273,   541,   894,   748,   573,  1116,   510,   510,  1119,    87,
     892,   339,   739,   672,   333,   334,  1172,   876,   106,   991,
     248,  1089,   672,   673,  -110,   273,  -351,  -110,  -110,   852,
      87,   574,   742,   220,   561,    87,   320,   728,   630,   585,
     673,  -351,   273,   767,    71,   519,  1039,  1040,   591,   673,
    -574,   273,   983,   753,   815,  -110,   594,  -110,  -112,   525,
     209,   551,  -435,  -437,   672,  1020,   335,   336,   665,   220,
     674,   273,   874,   871,   597,   273,  -351,   736,  -112,   630,
     630,  -109,   127,  -351,   127,   268,   590,   598,   782,   672,
     789,   673,   785,   601,   220,   249,    87,   220,   250,   251,
     789,   604,   804,   623,   606,   273,   783,    87,   273,   607,
     268,   220,   737,   611,  1178,    87,   673,   525,   273,   622,
     220,   699,   844,   384,   384,    87,   252,   268,   253,  1101,
    1103,  1104,  1105,  1034,  1035,   700,   268,   623,   370,   371,
     372,   685,   848,   623,   623,   703,   755,   533,   704,   436,
     127,   854,   978,   509,   856,   513,  1073,   510,   984,   986,
      87,   894,   706,   283,   437,   373,   283,   804,   804,    87,
     861,  1013,   384,   384,   562,   713,  -114,   729,  -112,   851,
     741,  -112,  -112,   320,   283,   320,   746,   220,   750,   870,
     873,   543,   930,   215,   873,    87,   -95,  -106,   887,   438,
     765,   873,   866,   268,   886,   781,   439,   740,  -110,  -112,
     778,  -112,   799,   545,   745,   248,   809,  1164,   810,   747,
     853,   789,   220,   855,   847,  1149,   510,   215,   752,  -102,
     209,   359,   360,   279,   852,   811,   279,   625,   779,   617,
     249,   853,   220,   250,   251,   569,   849,  1176,   333,   334,
     551,   320,   847,   477,   279,   209,   459,   459,   797,   477,
     989,   863,  -665,  -112,   249,   384,   384,   250,   251,   561,
     864,   625,   980,   253,   596,   623,   871,   625,   625,   983,
    1079,   776,   777,   890,  -104,   879,  1081,   885,   912,   771,
    1068,   362,   363,   364,   365,   252,   500,   253,   775,   933,
     335,   336,   462,   519,   888,   273,   273,   772,  -109,   897,
     249,   906,   807,   250,   251,   792,  -307,   437,   796,   220,
      87,   900,   786,   789,   910,   944,   786,  -553,   583,  -101,
     916,  -307,   918,   925,   789,   976,   920,   992,   922,   127,
     951,   252,  -553,   253,   964,   952,   896,   672,   804,   967,
    1000,  -666,   463,   220,   479,   970,  1004,   525,   999,   439,
     973,  1003,  1008,  1009,   974,   975,  -307,  1136,  1137,   437,
    -669,  1015,   384,  -307,   673,  -665,   273,  -553,   996,   215,
     907,  -665,  1001,  1021,  -553,   510,  1005,   695,   500,   997,
     865,   249,  1007,   249,   250,   251,   250,   251,   743,   744,
    1018,   977,   586,   249,   480,  1023,   250,   251,  1038,   625,
    1041,   439,   273,  1048,  1044,  1046,  -557,  1053,  1050,  1055,
     600,   596,   596,  1052,   384,  1059,   253,  1054,  1056,   930,
     507,  -557,   930,    87,   252,   930,   253,   930,  1063,  1057,
     320,    87,   630,  1058,   105,   220,   105,   898,  1184,  1185,
    1070,   105,   105,   896,  1071,  1082,  -561,   105,   105,   105,
     127,   268,   105,  1083,  -666,   127,  -557,   909,  1197,  1095,
    -666,  -561,  1072,  -557,  1096,  1102,   630,   220,  1106,  1205,
    1080,  1107,   630,   630,  1206,  1207,  1210,  1211,    87,    87,
    1126,  1141,  1087,  1085,  1086,   105,  1127,  1108,   806,  1128,
    1132,  1129,    87,   930,   873,   220,  -561,  1143,  1169,   105,
    1145,  1173,   934,  -561,    87,    87,   215,   215,  1150,  1152,
     715,   715,  1154,    87,  1156,   488,  1159,   127,   771,   939,
     362,   363,   364,   365,    87,    87,  1111,   531,  1160,   933,
     437,   930,   933,   930,   933,   930,   772,   930,  -668,   850,
    -669,  1183,   437,  1190,  1194,   555,  -560,  1195,  1133,   424,
    1134,  1037,   105,  1135,   105,  1142,  1043,  1196,  1198,   229,
     437,  -560,  1199,   500,   132,   489,   873,  1168,   927,   273,
     500,   930,   439,   332,   333,   334,  1174,   480,  1016,   433,
     434,   435,   749,   990,   439,  1125,  -297,  1060,  1167,   962,
     596,   497,   211,   784,  1025,   556,  -560,   384,  1110,   891,
     933,  -297,   439,  -560,   630,  1170,  1171,     0,   513,  1099,
     519,  1115,   702,   873,    87,    87,     0,  1028,  1076,  1030,
       0,     0,    87,  1031,   841,     0,   335,   336,     0,     0,
       0,     0,   220,   220,    87,     0,  -297,     0,   933,   841,
     933,     0,   933,  -297,   933,     0,   105,     0,     0,   873,
     873,     0,     0,    74,   672,    74,   123,   123,   105,   105,
       0,  -672,  -668,     0,   123,     0,  1201,   842,     0,     0,
    1202,  1203,     0,     0,     0,  1208,  1209,     0,   933,     0,
     873,   673,   842,   273,     0,     0,     0,   873,   873,     0,
     343,   333,   334,   215,   215,   955,     0,  1045,  1047,     0,
      87,   968,    87,     0,    74,    87,     0,     0,   123,     0,
       0,   105,   860,  -308,   105,   813,     0,   841,     0,   105,
     105,   105,   105,  1097,  1098,   105,  -672,  -668,  -308,     0,
     437,     0,   105,   105,   123,     0,     0,  -669,     0,     0,
     105,  -672,  -668,   335,   336,  1117,     0,  1120,     0,     0,
       0,     0,   220,   477,     0,     0,   786,    87,    87,   939,
     842,     0,   939,  -308,   939,   814,     0,  1017,   268,     0,
    -308,     0,   439,    74,  -672,  -668,  -672,  -668,     0,  1109,
    -668,  -668,     0,  -672,  -668,     0,   105,   105,   105,   105,
     105,   105,   105,   105,   437,     0,     0,     0,  1130,     0,
    1144,  1114,  -669,  1146,   682,   684,     0,   345,   333,   334,
       0,   105,   915,   437,     0,   841,     0,  -669,    87,   220,
     220,   841,     0,  1051,     0,     0,     0,    87,    87,   463,
     939,     0,   105,     0,     0,   105,   439,   105,   682,   684,
     105,   562,   333,   334,     0,  1062,  1175,     0,  1131,     0,
    -669,  1177,  -669,  1179,     0,   439,  -669,  1180,   842,  -669,
     335,   336,     0,     0,   842,    74,     0,     0,   939,     0,
     939,   105,   939,     0,   939,  1151,  1153,  1155,     0,  1157,
    1158,   105,   105,     0,     0,     0,   995,     0,     0,   751,
       0,  1200,     0,     0,   335,   336,   105,     0,   105,   105,
     567,   333,   334,   571,   333,   334,     0,     0,   939,   105,
       0,   715,     0,   105,   477,     0,     0,   105,     0,     0,
       0,     0,   105,     0,     0,  1121,   955,   105,     0,     0,
    1062,     0,     0,     0,   572,   333,   334,  1189,  1191,  1192,
    1193,     0,     0,   841,    74,     0,   766,   333,   334,     0,
       0,    74,    74,   335,   336,     0,   335,   336,     0,    74,
    1204,     0,   105,     0,   841,   841,     0,     0,     0,     0,
     123,   105,  -681,  -681,  -681,     0,  -681,  -681,     0,  -681,
       0,     0,     0,     0,  -681,     0,   842,   335,   336,   105,
       0,   361,     0,   362,   363,   364,   365,   105,  1161,   335,
     336,     0,     0,     0,     0,    74,     0,   842,   842,   366,
      74,     0,     0,     0,     0,   771,     0,   362,   363,   364,
     365,     0,   249,   477,   105,   250,   251,     0,     0,   477,
      74,   955,  1121,   772,   368,   354,   355,   356,   357,   358,
     369,   370,   371,   372,   105,   841,   841,     0,     0,     0,
     841,    74,     0,  1019,     0,   253,    74,   123,   368,    74,
       0,     0,     0,     0,   773,     0,  1121,     0,   373,     0,
       0,   374,     0,     0,     0,   249,     0,     0,   250,   251,
       0,     0,     0,     0,   558,     0,     0,     0,   842,   842,
       0,     0,     0,   842,     0,     0,     0,     0,     0,  -681,
      74,    74,     0,     0,     0,  -681,   252,     0,   253,     0,
     477,   477,     0,     0,     0,     0,     0,    74,   841,     0,
       0,   105,   105,     0,   421,   422,     0,     0,    74,   771,
     841,   362,   363,   364,   365,     0,    74,   424,     0,   771,
       0,   362,   363,   364,   365,     0,    74,   772,     0,     0,
       0,   841,   841,     0,     0,   105,     0,   772,     0,     0,
       0,   842,   428,   429,   430,   431,   432,   433,   434,   435,
       0,     0,   368,   842,     0,     0,     0,     0,  1024,     0,
       0,    74,   368,     0,     0,     0,   841,   816,   817,   818,
      74,     0,     0,     0,   842,   842,     0,     0,     0,     0,
       0,   979,   981,     0,   123,   953,   123,   985,   987,   820,
       0,   477,    40,    41,     0,     0,    74,   821,     0,     0,
      75,     0,    75,   124,   124,   421,   422,   477,   477,   842,
       0,   124,     0,   979,   981,   105,   985,   987,   424,     0,
       0,     0,     0,   105,   105,   822,     0,   105,     0,     0,
       0,   823,   824,     0,   825,     0,     0,     0,     0,  1091,
      58,   362,   363,   364,   365,   430,   431,   432,   433,   434,
     435,    75,   123,     0,     0,   124,     0,   772,   105,   105,
       0,   827,     0,     0,   105,   105,     0,     0,   122,     0,
     105,   105,     0,     0,     0,   954,     0,     0,     0,     0,
       0,   124,     0,     0,   105,     0,     0,   105,     0,   795,
       0,   362,   363,   364,   365,     0,   105,   105,     0,     0,
       0,     0,   421,   422,     0,   105,     0,   366,     0,     0,
       0,  -695,     0,     0,     0,   424,   105,   105,     0,  1069,
      75,    74,     0,     0,  -695,  -695,  -695,  -695,  -695,  -695,
     424,  -695,   368,     0,     0,     0,  -695,  -695,  -695,   370,
     371,   372,  1069,   431,   432,   433,   434,   435,  -695,  -695,
       0,     0,  -695,  -695,  -695,  -695,  -695,     0,   431,   432,
     433,   434,   435,     0,     0,     0,   373,   104,     0,   104,
     130,   130,     0,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   361,   105,   362,   363,   364,
     365,     0,  -695,     0,     0,     0,   105,   105,     0,     0,
       0,     0,    75,   366,   105,     0,     0,  -695,   104,     0,
       0,     0,   322,     0,   105,   105,   105,  -695,     0,     0,
    -695,  -695,     0,     0,    74,     0,     0,     0,   368,     0,
       0,   123,    74,    74,   369,   370,   371,   372,   322,     0,
    -695,  -695,     0,     0,     0,     0,   278,  -695,  -695,  -695,
    -695,   361,     0,   362,   363,   364,   365,     0,     0,     0,
       0,     0,   373,     0,     0,   374,     0,    74,     0,   366,
       0,     0,     0,    74,    74,     0,  1112,   104,     0,    74,
      74,    75,   105,   367,   105,     0,     0,   105,    75,    75,
       0,     0,     0,    74,   368,     0,    75,   816,   817,   818,
     369,   370,   371,   372,     0,    74,    74,   124,     0,     0,
       0,     0,     0,     0,    74,   819,     0,     0,     0,   820,
       0,  -695,    40,    41,     0,    74,    74,   821,   373,     0,
       0,   374,     0,     0,   105,     0,     0,     0,     0,   105,
     105,     0,    75,     0,   375,     0,     0,    75,     0,     0,
       0,     0,   123,     0,     0,   822,     0,   123,     0,     0,
       0,   823,   824,     0,   825,     0,   826,    75,     0,   104,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   361,     0,   362,   363,   364,   365,     0,    75,     0,
       0,   827,     0,    75,   124,     0,    75,     0,   122,   366,
     105,   105,   105,     0,     0,    74,     0,     0,     0,   105,
     105,     0,     0,     0,     0,    74,    74,     0,     0,   123,
       0,     0,     0,    74,   368,     0,     0,   816,   817,   818,
     369,   370,   371,   372,     0,    74,     0,    75,    75,     0,
       0,     0,     0,     0,     0,   953,     0,     0,   104,   820,
       0,     0,    40,    41,    75,   104,   104,   821,   373,     0,
       0,   374,     0,   104,     0,    75,     0,     0,     0,     0,
       0,     0,     0,    75,   322,     0,     0,     0,     0,     0,
       0,     0,     0,    75,     0,   822,     0,     0,     0,     0,
       0,   823,   824,     0,   825,     0,     0,     0,     0,     0,
      58,    74,     0,    74,     0,     0,    74,     0,     0,   104,
       0,     0,     0,     0,   104,     0,     0,     0,    75,     0,
       0,   827,   816,   817,   818,     0,     0,    75,   122,     0,
       0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
     953,   124,     0,   124,   820,     0,     0,    40,    41,     0,
       0,     0,   821,    75,     0,   104,     0,     0,    74,    74,
     104,   322,     0,   631,     0,     0,     0,     0,     0,     0,
       0,   101,     0,   101,   129,   129,   129,     0,     0,     0,
     822,     0,   233,     0,     0,     0,   823,   824,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,   631,   631,     0,     0,     0,   124,
       0,     0,     0,     0,     0,     0,   827,     0,     0,    74,
       0,   104,   101,   122,     0,     0,   321,     0,    74,    74,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
     104,     0,     0,     0,     0,     0,     0,   408,     0,     0,
     104,     0,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,     0,
       0,     0,     0,     0,     0,   104,     0,     0,     0,     0,
       0,   101,     0,     0,   104,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   418,   419,   420,     0,   322,     0,
     322,   421,   422,     0,     0,     0,   423,     0,     0,     0,
     104,     0,     0,     0,   424,     0,     0,     0,     0,     0,
     812,     0,     0,     0,     0,    84,     0,    84,     0,     0,
       0,     0,     0,     0,     0,   425,   230,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,     0,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   418,   419,   420,
       0,     0,     0,     0,   421,   422,   322,     0,     0,     0,
       0,     0,     0,   101,     0,     0,    84,   424,     0,     0,
       0,    75,     0,     0,     0,     0,     0,     0,   124,    75,
      75,     0,     0,     0,     0,     0,     0,     0,   425,     0,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
       0,     0,     0,     0,     0,     0,     0,     0,  -282,     0,
       0,     0,     0,   361,    75,   362,   363,   364,   365,     0,
      75,    75,     0,     0,     0,   104,    75,    75,     0,     0,
       0,   366,     0,     0,     0,    84,     0,     0,     0,     0,
      75,     0,   101,     0,     0,   367,     0,     0,     0,   101,
     101,     0,    75,    75,     0,     0,   368,   101,     0,     0,
       0,    75,   369,   370,   371,   372,     0,     0,   321,     0,
       0,     0,    75,    75,     0,     0,     0,     0,     0,     0,
     361,     0,   362,   363,   364,   365,     0,     0,     0,   102,
     373,   102,   361,   374,   362,   363,   364,   365,   366,   124,
       0,     0,     0,   101,   124,     0,   375,     0,   101,     0,
     366,     0,     0,     0,   588,     0,     0,     0,     0,     0,
       0,     0,     0,   368,     0,     0,   787,    84,   101,   369,
     370,   371,   372,     0,     0,   368,     0,     0,   104,     0,
     102,   369,   370,   371,   372,   322,   104,   631,     0,   101,
       0,     0,    75,     0,   101,   321,     0,   373,     0,     0,
     374,     0,    75,    75,     0,     0,   124,     0,     0,   373,
      75,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,   631,    75,     0,     0,     0,     0,   631,   631,     0,
       0,     0,     0,   104,   104,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    84,   104,     0,   102,
       0,     0,     0,    84,    84,   101,     0,     0,     0,   104,
     104,    84,     0,     0,     0,     0,   101,     0,   104,     0,
       0,     0,     0,     0,   101,     0,     0,     0,     0,   104,
     104,     0,     0,     0,   101,     0,     0,     0,    75,     0,
      75,     0,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   130,    84,     0,     0,
       0,   130,    84,   361,     0,   362,   363,   364,   365,   101,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   366,    84,     0,     0,     0,     0,     0,     0,     0,
       0,   102,   321,     0,   321,    75,    75,   911,     0,     0,
       0,     0,     0,    84,   101,     0,   368,     0,    84,   631,
       0,   626,   369,   370,   371,   372,     0,     0,     0,   104,
     104,     0,     0,  1078,     0,     0,     0,   104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
     373,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,   626,   626,     0,     0,    75,     0,     0,     0,
     321,     0,     0,     0,     0,    75,    75,     0,     0,    84,
     102,     0,     0,     0,     0,     0,     0,   102,   102,     0,
      84,     0,     0,     0,     0,   102,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,     0,     0,     0,     0,   104,     0,   104,     0,     0,
     104,     0,     0,     0,     0,   812,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,   102,     0,    84,     0,     0,   102,     0,     0,     0,
       0,     0,    84,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,     0,   102,     0,     0,   421,
     422,     0,   104,   104,     0,     0,     0,     0,    84,     0,
       0,     0,   424,     0,     0,     0,     0,   102,     0,     0,
       0,     0,   102,     0,     0,   102,     0,     0,     0,     0,
       0,     0,     0,   425,     0,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   104,     0,     0,   102,   102,     0,     0,
       0,     0,   104,   104,     0,     0,     0,     0,     0,     0,
       0,     0,   101,   102,     0,     0,     0,     0,     0,   321,
     101,     0,     0,     0,   102,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,     0,     0,   101,   101,     0,
       0,  -696,  -696,  -696,  -696,   413,   414,   102,     0,  -696,
    -696,   101,     0,     0,     0,     0,   102,   421,   422,     0,
       0,     0,     0,   101,   101,     0,     0,     0,     0,     0,
     424,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   102,   101,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,     0,   409,   410,   411,   412,   413,   414,
     129,     0,   417,   418,     0,   129,     0,     0,     0,     0,
     421,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   424,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    84,   626,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,     0,     0,     0,     0,
       0,     0,     0,   101,   101,     0,     0,  1077,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,   626,
       0,     0,     0,   101,     0,   626,   626,     0,     0,     0,
       0,    84,    84,     0,     0,     0,     0,   102,     0,     0,
       0,     0,     0,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,    84,   409,
     410,   411,   412,   413,   414,   415,    84,   417,   418,     0,
       0,     0,     0,     0,     0,   421,   422,    84,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,   101,
       0,   101,     0,     0,   101,     0,     0,     0,     0,     0,
       0,   269,   269,     0,     0,   269,     0,     0,     0,     0,
       0,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   293,   295,   296,   297,     0,     0,     0,
     269,   313,     0,     0,     0,     0,   101,   101,     0,     0,
     102,     0,   350,   351,     0,     0,     0,   626,   102,   102,
       0,     0,     0,     0,     0,     0,     0,    84,    84,     0,
       0,  1075,     0,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     0,     0,   102,     0,     0,     0,     0,     0,   102,
     102,     0,     0,     0,     0,   102,   102,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   101,   101,     0,   102,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   102,   102,     0,     0,     0,     0,     0,     0,     0,
     102,   660,   661,     0,     0,   662,     0,     0,     0,     0,
       0,   102,   102,    84,     0,    84,     0,     0,    84,     0,
     177,   178,   179,   180,   181,   182,   183,   184,     0,     0,
     185,   186,     0,     0,     0,     0,   187,   188,   189,   190,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     191,   192,   193,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,    84,     0,     0,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,     0,   204,   205,     0,     0,     0,
       0,   102,     0,   206,   278,     0,     0,     0,     0,     0,
       0,   102,   102,     0,     0,     0,     0,     0,     0,   102,
     503,   504,   505,   350,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   269,   669,   670,   269,     0,   671,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
      84,    84,     0,     0,   177,   178,   179,   180,   181,   182,
     183,   184,     0,     0,   185,   186,     0,     0,     0,     0,
     187,   188,   189,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   191,   192,   193,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,   102,
       0,     0,   102,     0,     0,     0,     0,     0,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,     0,   204,
     205,     0,     0,     0,     0,     0,     0,   206,   278,     0,
       0,     0,     0,     0,     0,   593,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   603,     0,     0,     0,
       0,     0,     0,     0,   102,   102,     0,     0,   616,     0,
       0,     0,     0,   627,     0,   633,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,   644,   645,   646,     0,
     648,   649,   650,   651,   652,   653,   654,   655,   656,   657,
     658,   659,     0,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   681,   681,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     0,   269,
       0,     0,     0,     0,   102,   102,     0,     0,     0,     0,
       0,     0,     0,     0,   681,     0,   269,     0,   681,   681,
       0,     0,     0,     0,     0,   269,     0,     0,     0,     0,
       0,     0,     0,     0,   727,     0,     0,   730,   731,     0,
       0,     0,   732,     0,     0,   735,     0,   738,     0,   313,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   681,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   735,
       0,     0,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   769,
     770,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   780,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   798,     0,     0,
     805,     0,  -695,     3,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,     0,     0,     0,     0,     0,
       0,    14,     0,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    19,    20,    21,    22,    23,    24,    25,     0,
       0,    26,    27,     0,     0,     0,     0,     0,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,    42,     0,     0,    43,     0,     0,    44,
      45,     0,    46,    47,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   867,    49,    50,   780,
     798,     0,     0,     0,    51,     0,     0,    52,    53,     0,
      54,    55,     0,    56,     0,     0,     0,    57,     0,    58,
      59,    60,     0,    61,    62,    63,     0,    64,  -695,     0,
     893,  -695,  -695,     0,   913,     0,     0,     0,     0,   735,
     313,     0,     0,     0,     0,     0,     0,    65,    66,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -695,
       0,  -695,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,     0,     0,     0,     0,   421,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   942,     0,     0,     0,     0,   681,   945,     0,
     269,     0,   425,     0,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   681,   681,     0,     0,     0,   735,   681,   681,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   681,   681,     0,   681,   681,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1014,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1026,  1027,
       0,     0,     0,     0,     0,     0,  -298,     0,     0,     0,
       0,  1032,  1033,     0,     0,     0,     0,     0,     0,  -298,
    -298,  -298,  -298,  -298,  -298,     0,  -298,  1049,     0,     0,
       0,  -298,     0,  -298,  -298,  -298,     0,     0,     0,     0,
       0,     0,     0,  -298,  -298,     0,     0,  -298,  -298,  -298,
    -298,  -298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1066,  1067,  -298,     0,     0,     0,     0,   681,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -298,
    -298,  -298,  -298,  -298,  -298,  -298,  -298,  -298,  -298,  -298,
    -298,     0,   681,     0,     0,  -298,  -298,  -298,     0,     0,
    -298,     0,     0,   313,     0,     0,  -298,     0,  -298,     0,
       0,     0,  -298,     0,     0,     0,     0,     0,     0,     0,
    -298,     0,  -298,     0,     0,  -298,  -298,     0,     0,  -298,
    -298,  -298,  -298,  -298,  -298,  -298,  -298,  -298,  -298,  -298,
    -298,     0,     0,     0,     0,  -423,  -298,  -298,  -298,  -298,
       0,     0,  -298,  -298,  -298,  -298,     0,     0,  -423,  -423,
    -423,  -423,  -423,  -423,     0,  -423,     0,     0,     0,     0,
    -423,  -423,  -423,  -423,     0,     0,     0,     0,     0,     0,
       0,     0,  -423,  -423,     0,     0,  -423,  -423,  -423,  -423,
    -423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   269,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
       0,     0,     0,     0,  -423,  -423,  -423,     0,     0,  -423,
       0,     0,     0,     0,     0,  -423,     0,  -423,     0,     0,
       0,  -423,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -423,     0,     0,  -423,  -423,     0,     0,  -423,     0,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,
       0,     0,  -553,     0,  -423,  -423,  -423,  -423,  -423,     0,
     278,  -423,  -423,  -423,  -423,  -553,  -553,  -553,  -553,  -553,
    -553,     0,  -553,     0,     0,     0,     0,  -553,     0,  -553,
    -553,     0,     0,     0,     0,     0,     0,     0,     0,  -553,
    -553,     0,     0,  -553,  -553,  -553,  -553,  -553,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     501,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -553,  -553,  -553,  -553,  -553,
    -553,  -553,  -553,  -553,  -553,  -553,  -553,     0,     0,     0,
       0,  -553,  -553,  -553,     0,  -553,  -553,     0,     0,     0,
       0,     0,  -553,     0,  -553,     0,     0,     0,  -553,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -553,     0,
       0,  -553,  -553,     0,  -553,  -553,     0,  -553,  -553,  -553,
    -553,  -553,  -553,  -553,  -553,  -553,  -553,     0,     0,  -695,
       0,     0,  -553,  -553,  -553,  -553,     0,     0,  -553,  -553,
    -553,  -553,  -695,  -695,  -695,  -695,  -695,  -695,     0,  -695,
       0,     0,     0,     0,  -695,  -695,  -695,  -695,     0,     0,
       0,     0,     0,     0,     0,     0,  -695,  -695,     0,     0,
    -695,  -695,  -695,  -695,  -695,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,     0,     0,     0,     0,  -695,  -695,
    -695,     0,     0,  -695,     0,     0,     0,     0,     0,  -695,
       0,  -695,     0,     0,     0,  -695,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -695,     0,     0,  -695,  -695,
       0,     0,  -695,     0,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,     0,     0,  -695,     0,  -695,  -695,
    -695,  -695,  -695,     0,   278,  -695,  -695,  -695,  -695,  -695,
    -695,  -695,  -695,  -695,  -695,     0,  -695,     0,     0,     0,
       0,  -695,     0,  -695,  -695,     0,     0,     0,     0,     0,
       0,     0,     0,  -695,  -695,     0,     0,  -695,  -695,  -695,
    -695,  -695,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -695,
    -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,     0,     0,     0,     0,  -695,  -695,  -695,     0,     0,
    -695,     0,     0,     0,     0,     0,  -695,     0,  -695,     0,
       0,     0,  -695,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -695,     0,     0,  -695,  -695,     0,     0,  -695,
       0,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,  -695,
    -695,     0,     0,  -672,     0,     0,  -695,  -695,  -695,  -695,
       0,   278,  -695,  -695,  -695,  -695,  -672,  -672,  -672,     0,
    -672,  -672,     0,  -672,     0,     0,     0,     0,  -672,  -672,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -672,  -672,     0,     0,  -672,  -672,  -672,  -672,  -672,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,     0,     0,
       0,     0,  -672,  -672,  -672,     0,   845,  -672,     0,     0,
       0,     0,     0,     0,     0,  -672,     0,     0,     0,  -672,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -672,
       0,     0,  -672,  -672,     0,  -111,  -672,     0,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,     0,     0,
    -672,     0,  -672,  -672,  -672,     0,  -103,     0,     0,  -672,
    -672,  -672,  -672,  -672,  -672,  -672,     0,  -672,  -672,     0,
    -672,     0,     0,     0,     0,  -672,  -672,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -672,  -672,     0,
       0,  -672,  -672,  -672,  -672,  -672,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,     0,     0,     0,     0,  -672,
    -672,  -672,     0,   845,  -672,     0,     0,     0,     0,     0,
       0,     0,  -672,     0,     0,     0,  -672,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -672,     0,     0,  -672,
    -672,     0,  -111,  -672,     0,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,  -672,  -672,  -672,     0,     0,  -307,     0,  -672,
    -672,  -672,     0,  -672,     0,     0,  -672,  -672,  -672,  -672,
    -307,  -307,  -307,     0,  -307,  -307,     0,  -307,     0,     0,
       0,     0,  -307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -307,  -307,     0,     0,  -307,  -307,
    -307,  -307,  -307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,     0,     0,     0,     0,  -307,  -307,  -307,     0,
     846,  -307,     0,     0,     0,     0,     0,     0,     0,  -307,
       0,     0,     0,  -307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -307,     0,     0,  -307,  -307,     0,  -113,
    -307,     0,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,     0,     0,  -307,     0,     0,  -307,  -307,     0,
    -105,     0,     0,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
       0,  -307,  -307,     0,  -307,     0,     0,     0,     0,  -307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -307,  -307,     0,     0,  -307,  -307,  -307,  -307,  -307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,     0,
       0,     0,     0,  -307,  -307,  -307,     0,   846,  -307,     0,
       0,     0,     0,     0,     0,     0,  -307,     0,     0,     0,
    -307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -307,     0,     0,  -307,  -307,     0,  -113,  -307,     0,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,     0,
       0,     0,     0,     0,  -307,  -307,     0,  -307,     0,     0,
    -307,  -307,  -307,  -307,   299,     0,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,  -695,  -695,  -695,     0,
       0,  -695,    14,     0,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    19,    20,    21,    22,    23,    24,    25,
       0,     0,    26,    27,     0,     0,     0,     0,     0,    28,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,     0,    40,    41,    42,     0,     0,    43,     0,     0,
      44,    45,     0,    46,    47,    48,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
       0,     0,     0,     0,     0,    51,     0,     0,    52,    53,
       0,    54,    55,     0,    56,     0,     0,     0,    57,     0,
      58,    59,    60,     0,    61,    62,    63,     0,    64,  -695,
       0,     0,  -695,  -695,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    66,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -695,   299,  -695,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,  -695,     0,  -695,  -695,    14,
       0,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      19,    20,    21,    22,    23,    24,    25,     0,     0,    26,
      27,     0,     0,     0,     0,     0,    28,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,     0,    40,
      41,    42,     0,     0,    43,     0,     0,    44,    45,     0,
      46,    47,    48,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,     0,     0,     0,
       0,     0,    51,     0,     0,    52,    53,     0,    54,    55,
       0,    56,     0,     0,     0,    57,     0,    58,    59,    60,
       0,    61,    62,    63,     0,    64,  -695,     0,     0,  -695,
    -695,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -695,   299,  -695,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,  -695,     0,     0,  -695,    14,  -695,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    19,    20,    21,
      22,    23,    24,    25,     0,     0,    26,    27,     0,     0,
       0,     0,     0,    28,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,     0,    40,    41,    42,     0,
       0,    43,     0,     0,    44,    45,     0,    46,    47,    48,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,     0,     0,     0,     0,     0,    51,
       0,     0,    52,    53,     0,    54,    55,     0,    56,     0,
       0,     0,    57,     0,    58,    59,    60,     0,    61,    62,
      63,     0,    64,  -695,     0,     0,  -695,  -695,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -695,   299,  -695,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,  -695,
       0,     0,  -695,    14,     0,    15,    16,    17,    18,  -695,
       0,     0,     0,     0,    19,    20,    21,    22,    23,    24,
      25,     0,     0,    26,    27,     0,     0,     0,     0,     0,
      28,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,     0,    40,    41,    42,     0,     0,    43,     0,
       0,    44,    45,     0,    46,    47,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
      50,     0,     0,     0,     0,     0,    51,     0,     0,    52,
      53,     0,    54,    55,     0,    56,     0,     0,     0,    57,
       0,    58,    59,    60,     0,    61,    62,    63,     0,    64,
    -695,     0,     0,  -695,  -695,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -695,   299,  -695,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,  -695,     0,     0,  -695,
      14,     0,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    19,    20,    21,    22,    23,    24,    25,     0,     0,
      26,    27,     0,     0,     0,     0,     0,    28,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,     0,
      40,    41,    42,     0,     0,    43,     0,     0,    44,    45,
       0,    46,    47,    48,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,     0,     0,
       0,     0,     0,    51,     0,     0,    52,    53,     0,    54,
      55,     0,    56,     0,     0,     0,    57,     0,    58,    59,
      60,     0,    61,    62,    63,     0,    64,  -695,     0,     0,
    -695,  -695,     3,     0,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,    65,    66,    67,     0,
      14,     0,    15,    16,    17,    18,     0,     0,  -695,     0,
    -695,    19,    20,    21,    22,    23,    24,    25,     0,     0,
      26,    27,     0,     0,     0,     0,     0,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,     0,
      40,    41,    42,     0,     0,    43,     0,     0,    44,    45,
       0,    46,    47,    48,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,     0,     0,
       0,     0,     0,    51,     0,     0,    52,    53,     0,    54,
      55,     0,    56,     0,     0,     0,    57,     0,    58,    59,
      60,     0,    61,    62,    63,     0,    64,  -695,     0,     0,
    -695,  -695,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,    66,    67,     0,
       0,  -695,     0,     0,     0,     0,     0,     0,  -695,   299,
    -695,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,     0,  -695,  -695,     0,     0,     0,    14,     0,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    19,    20,
      21,    22,    23,    24,    25,     0,     0,    26,    27,     0,
       0,     0,     0,     0,    28,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,     0,    40,    41,    42,
       0,     0,    43,     0,     0,    44,    45,     0,    46,    47,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
      51,     0,     0,    52,    53,     0,    54,    55,     0,    56,
       0,     0,     0,    57,     0,    58,    59,    60,     0,    61,
      62,    63,     0,    64,  -695,     0,     0,  -695,  -695,   299,
       0,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,     0,     0,    65,    66,    67,     0,    14,     0,    15,
      16,    17,    18,     0,     0,  -695,     0,  -695,    19,    20,
      21,    22,    23,    24,    25,     0,     0,    26,    27,     0,
       0,     0,     0,     0,    28,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,     0,    40,    41,    42,
       0,     0,    43,     0,     0,    44,    45,     0,    46,    47,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
      51,     0,     0,   300,    53,     0,    54,    55,     0,    56,
       0,     0,     0,    57,     0,    58,    59,    60,     0,    61,
      62,    63,     0,    64,  -695,     0,     0,  -695,  -695,  -301,
       0,  -301,  -301,  -301,  -301,  -301,  -301,  -301,  -301,  -301,
    -301,     0,     0,    65,    66,    67,     0,  -301,     0,  -301,
    -301,  -301,  -301,     0,  -695,  -695,     0,  -695,  -301,  -301,
    -301,  -301,  -301,  -301,  -301,     0,     0,  -301,  -301,     0,
       0,     0,     0,     0,  -301,     0,  -301,  -301,  -301,  -301,
    -301,  -301,  -301,  -301,  -301,  -301,     0,  -301,  -301,  -301,
       0,     0,  -301,     0,     0,  -301,  -301,     0,  -301,  -301,
    -301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -301,  -301,     0,     0,     0,     0,     0,
    -301,     0,     0,  -301,  -301,     0,  -301,  -301,     0,  -301,
       0,     0,     0,  -301,     0,  -301,  -301,  -301,     0,  -301,
    -301,  -301,     0,  -301,  -301,     0,     0,  -301,  -301,   299,
       0,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,     0,     0,  -301,  -301,  -301,     0,    14,     0,    15,
      16,    17,    18,     0,  -301,  -301,     0,  -301,    19,    20,
      21,    22,    23,    24,    25,     0,     0,    26,    27,     0,
       0,     0,     0,     0,    28,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,     0,    40,    41,    42,
       0,     0,    43,     0,     0,    44,    45,     0,    46,    47,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
      51,     0,     0,    52,    53,     0,    54,    55,     0,    56,
       0,     0,     0,    57,     0,    58,    59,    60,     0,    61,
      62,    63,     0,    64,  -695,     0,     0,  -695,  -695,   299,
       0,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,     0,     0,    65,    66,    67,     0,    14,     0,    15,
      16,    17,    18,     0,  -695,  -695,     0,  -695,    19,    20,
      21,    22,    23,    24,    25,     0,     0,    26,    27,     0,
       0,     0,     0,     0,    28,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,     0,    40,    41,    42,
       0,     0,    43,     0,     0,    44,    45,     0,    46,    47,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
      51,     0,     0,    52,    53,     0,    54,    55,     0,    56,
       0,     0,     0,    57,     0,    58,    59,    60,     0,    61,
      62,    63,     0,    64,  -695,     0,     0,  -695,  -695,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    67,     0,     0,  -695,     0,
       0,     0,     0,     0,     0,  -695,   299,  -695,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
    -695,     0,     0,     0,    14,     0,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    19,    20,    21,    22,    23,
      24,    25,     0,     0,    26,    27,     0,     0,     0,     0,
       0,    28,     0,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,     0,    40,    41,    42,     0,     0,    43,
       0,     0,    44,    45,     0,    46,    47,    48,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,    50,     0,     0,     0,     0,     0,    51,     0,     0,
      52,    53,     0,    54,    55,     0,    56,     0,     0,     0,
      57,     0,    58,    59,    60,     0,    61,    62,    63,     0,
      64,  -695,     0,     0,  -695,  -695,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,    67,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -695,     0,  -695,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,  -682,  -682,  -682,     0,  -682,
    -682,    14,  -682,    15,    16,    17,    18,  -682,     0,     0,
       0,     0,    19,    20,    21,    22,    23,    24,    25,     0,
       0,    26,    27,     0,     0,     0,     0,     0,    28,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,    42,     0,     0,    43,     0,     0,    44,
      45,     0,    46,    47,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,    51,     0,     0,    52,    53,     0,
      54,    55,     0,    56,     0,     0,     0,    57,     0,    58,
      59,    60,     0,    61,    62,    63,     0,    64,   249,     0,
       0,   250,   251,     0,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,     0,     0,    65,    66,    67,
       0,    14,  -682,    15,    16,    17,    18,     0,  -682,   252,
       0,   253,    19,    20,    21,    22,    23,    24,    25,     0,
       0,    26,    27,     0,     0,     0,     0,     0,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,    42,     0,     0,    43,     0,     0,    44,
      45,     0,    46,    47,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,    51,     0,     0,    52,    53,     0,
      54,    55,     0,    56,     0,     0,     0,    57,     0,    58,
      59,    60,     0,    61,    62,    63,     0,    64,   249,     0,
       0,   250,   251,     0,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    65,    66,    67,
       0,    14,     0,    15,    16,    17,    18,     0,     0,   252,
       0,   253,    19,    20,    21,    22,    23,    24,    25,     0,
       0,    26,    27,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,    42,     0,     0,    43,     0,     0,    44,
      45,     0,    46,    47,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,   214,     0,     0,   121,    53,     0,
      54,    55,     0,     0,     0,     0,     0,    57,     0,    58,
      59,    60,     0,    61,    62,    63,     0,    64,   249,     0,
       0,   250,   251,     0,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    65,    66,    67,
       0,    14,     0,   109,   110,    17,    18,     0,     0,   252,
       0,   253,   111,   112,   113,    22,    23,    24,    25,     0,
       0,   114,   115,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,    42,     0,     0,    43,     0,     0,    44,
      45,     0,    46,    47,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,   214,     0,     0,   121,    53,     0,
      54,    55,     0,     0,     0,     0,     0,    57,     0,    58,
      59,    60,     0,    61,    62,    63,     0,    64,   249,     0,
       0,   250,   251,     0,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    65,   267,    67,
       0,    14,     0,    15,    16,    17,    18,     0,     0,   252,
       0,   253,    19,    20,    21,    22,    23,    24,    25,     0,
       0,    26,    27,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,    42,     0,     0,    43,     0,     0,    44,
      45,     0,    46,    47,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,   214,     0,     0,   121,    53,     0,
      54,    55,     0,     0,     0,     0,     0,    57,     0,    58,
      59,    60,     0,    61,    62,    63,     0,    64,   249,     0,
       0,   250,   251,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,    66,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   253,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,     0,     0,     0,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
       0,     0,     0,     0,     0,   168,   169,   170,   171,   172,
     173,   174,   175,    36,    37,   176,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   118,
     177,   178,   179,   180,   181,   182,   183,   184,     0,     0,
     185,   186,     0,     0,     0,     0,   187,   188,   189,   190,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     191,   192,   193,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,     0,   204,   205,     0,     0,     0,
       0,     0,     0,   206,   207,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,     0,     0,     0,     0,     0,     0,
       0,  -665,     0,  -665,  -665,  -665,  -665,     0,  -665,     0,
       0,     0,  -665,  -665,  -665,  -665,  -665,  -665,  -665,     0,
       0,  -665,  -665,     0,     0,     0,     0,     0,     0,     0,
       0,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
       0,  -665,  -665,  -665,     0,     0,  -665,     0,     0,  -665,
    -665,     0,  -665,  -665,  -665,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -665,  -665,     0,
       0,     0,     0,     0,  -665,     0,     0,  -665,  -665,     0,
    -665,  -665,     0,  -665,     0,  -665,  -665,  -665,     0,  -665,
    -665,  -665,     0,  -665,  -665,  -665,     0,  -665,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -665,  -665,  -665,
       0,  -665,     0,     0,     0,     0,     0,  -665,  -666,  -666,
    -666,  -666,  -666,  -666,  -666,  -666,  -666,     0,     0,     0,
       0,     0,     0,     0,  -666,     0,  -666,  -666,  -666,  -666,
       0,  -666,     0,     0,     0,  -666,  -666,  -666,  -666,  -666,
    -666,  -666,     0,     0,  -666,  -666,     0,     0,     0,     0,
       0,     0,     0,     0,  -666,  -666,  -666,  -666,  -666,  -666,
    -666,  -666,  -666,     0,  -666,  -666,  -666,     0,     0,  -666,
       0,     0,  -666,  -666,     0,  -666,  -666,  -666,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -666,  -666,     0,     0,     0,     0,     0,  -666,     0,     0,
    -666,  -666,     0,  -666,  -666,     0,  -666,     0,  -666,  -666,
    -666,     0,  -666,  -666,  -666,     0,  -666,  -666,  -666,     0,
    -666,     0,     0,     0,     0,     0,     0,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,     0,     0,     0,     0,
    -666,  -666,  -666,  -668,  -666,  -668,  -668,  -668,  -668,     0,
    -666,     0,     0,     0,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,     0,     0,  -668,  -668,     0,     0,     0,     0,     0,
       0,     0,     0,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,     0,  -668,  -668,  -668,     0,     0,  -668,     0,
       0,  -668,  -668,     0,  -668,  -668,  -668,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -668,
    -668,     0,     0,     0,     0,     0,  -668,   877,     0,  -668,
    -668,     0,  -668,  -668,     0,  -668,     0,  -668,  -668,  -668,
       0,  -668,  -668,  -668,     0,  -668,  -668,  -668,     0,  -668,
       0,     0,     0,     0,     0,     0,  -111,  -669,  -669,  -669,
    -669,  -669,  -669,  -669,  -669,  -669,     0,     0,     0,  -668,
    -668,  -668,     0,  -669,     0,  -669,  -669,  -669,  -669,  -668,
       0,     0,     0,     0,  -669,  -669,  -669,  -669,  -669,  -669,
    -669,     0,     0,  -669,  -669,     0,     0,     0,     0,     0,
       0,     0,     0,  -669,  -669,  -669,  -669,  -669,  -669,  -669,
    -669,  -669,     0,  -669,  -669,  -669,     0,     0,  -669,     0,
       0,  -669,  -669,     0,  -669,  -669,  -669,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -669,
    -669,     0,     0,     0,     0,     0,  -669,   878,     0,  -669,
    -669,     0,  -669,  -669,     0,  -669,     0,  -669,  -669,  -669,
       0,  -669,  -669,  -669,     0,  -669,  -669,  -669,     0,  -669,
       0,     0,     0,     0,     0,     0,  -113,  -670,  -670,  -670,
    -670,  -670,  -670,  -670,  -670,  -670,     0,     0,     0,  -669,
    -669,  -669,     0,  -670,     0,  -670,  -670,  -670,  -670,  -669,
       0,     0,     0,     0,  -670,  -670,  -670,  -670,  -670,  -670,
    -670,     0,     0,  -670,  -670,     0,     0,     0,     0,     0,
       0,     0,     0,  -670,  -670,  -670,  -670,  -670,  -670,  -670,
    -670,  -670,     0,  -670,  -670,  -670,     0,     0,  -670,     0,
       0,  -670,  -670,     0,  -670,  -670,  -670,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -670,
    -670,     0,     0,     0,     0,     0,  -670,     0,     0,  -670,
    -670,     0,  -670,  -670,     0,  -670,     0,  -670,  -670,  -670,
       0,  -670,  -670,  -670,     0,  -670,  -670,  -670,     0,  -670,
       0,     0,     0,     0,     0,     0,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,     0,     0,     0,     0,  -670,
    -670,  -670,  -671,     0,  -671,  -671,  -671,  -671,     0,  -670,
       0,     0,     0,  -671,  -671,  -671,  -671,  -671,  -671,  -671,
       0,     0,  -671,  -671,     0,     0,     0,     0,     0,     0,
       0,     0,  -671,  -671,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,     0,  -671,  -671,  -671,     0,     0,  -671,     0,     0,
    -671,  -671,     0,  -671,  -671,  -671,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -671,  -671,
       0,     0,     0,     0,     0,  -671,     0,     0,  -671,  -671,
       0,  -671,  -671,     0,  -671,     0,  -671,  -671,  -671,     0,
    -671,  -671,  -671,     0,  -671,  -671,  -671,     0,  -671,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -671,  -671,
    -671,     0,     0,     0,     0,     0,     0,     0,  -671,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,     0,     0,     0,   157,   158,   159,   235,
     236,   237,   238,   164,   165,   166,   167,     0,     0,     0,
       0,     0,   168,   169,   170,   239,   240,   241,   242,   175,
     324,   325,   243,   326,     0,     0,     0,     0,     0,     0,
     327,     0,     0,     0,     0,     0,   328,   177,   178,   179,
     180,   181,   182,   183,   184,     0,     0,   185,   186,     0,
       0,     0,     0,   187,   188,   189,   190,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   191,   192,   193,
       0,     0,     0,     0,   329,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,     0,   204,   205,     0,     0,     0,     0,     0,     0,
     206,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,     0,     0,     0,   157,   158,
     159,   235,   236,   237,   238,   164,   165,   166,   167,     0,
       0,     0,     0,     0,   168,   169,   170,   239,   240,   241,
     242,   175,   324,   325,   243,   326,     0,     0,     0,     0,
       0,     0,   327,     0,     0,     0,     0,     0,     0,   177,
     178,   179,   180,   181,   182,   183,   184,     0,     0,   185,
     186,     0,     0,     0,     0,   187,   188,   189,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   191,
     192,   193,     0,     0,     0,     0,   492,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,     0,   204,   205,     0,     0,     0,     0,
       0,     0,   206,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,     0,     0,     0,
     157,   158,   159,   235,   236,   237,   238,   164,   165,   166,
     167,     0,     0,     0,     0,     0,   168,   169,   170,   239,
     240,   241,   242,   175,     0,     0,   243,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,   180,   181,   182,   183,   184,     0,
       0,   185,   186,     0,     0,     0,     0,   187,   188,   189,
     190,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   191,   192,   193,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,     0,   204,   205,     0,     0,
       0,     0,     0,     0,   206,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,     0,
       0,     0,   157,   158,   159,   235,   236,   237,   238,   164,
     165,   166,   167,     0,     0,     0,     0,     0,   168,   169,
     170,   239,   240,   241,   242,   175,     0,     0,   243,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,   178,   179,   180,   181,   182,   183,
     184,     0,     0,   185,   186,     0,     0,     0,     0,   187,
     188,   189,   190,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   191,   192,   193,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,     0,   204,   205,
       0,     0,     0,     0,     0,     0,   206,     4,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,     0,
       0,     0,     0,    14,     0,   109,   110,    17,    18,     0,
       0,     0,     0,     0,   111,   112,   113,    22,    23,    24,
      25,     0,     0,   114,   115,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,     0,    40,    41,    42,     0,     0,    43,     0,
       0,    44,    45,     0,   118,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   317,     0,     0,   121,
      53,     0,    54,    55,     0,     0,     0,     0,     0,    57,
       0,    58,    59,    60,     0,    61,    62,    63,     0,    64,
       0,     0,     4,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,     0,     0,     0,     0,    14,   122,
     109,   110,    17,    18,     0,     0,     0,   318,     0,   111,
     112,   113,    22,    23,    24,    25,     0,     0,   114,   115,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,     0,    40,    41,
      42,     0,     0,    43,     0,     0,    44,    45,     0,   118,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,     0,   121,    53,     0,    54,    55,     0,
       0,     0,     0,     0,    57,     0,    58,    59,    60,     0,
      61,    62,    63,     0,    64,     0,     0,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
       0,     0,     0,    14,   122,    15,    16,    17,    18,     0,
       0,     0,   620,     0,    19,    20,    21,    22,    23,    24,
      25,     0,     0,    26,    27,     0,     0,     0,     0,     0,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,     0,    40,    41,    42,     0,     0,    43,     0,
       0,    44,    45,     0,    46,    47,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
      50,     0,     0,     0,     0,     0,    51,     0,     0,    52,
      53,     0,    54,    55,     0,    56,     0,     0,     0,    57,
       0,    58,    59,    60,     0,    61,    62,    63,     0,    64,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    65,
      66,    67,    14,     0,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    19,    20,    21,    22,    23,    24,    25,
       0,     0,    26,    27,     0,     0,     0,     0,     0,    28,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,     0,    40,    41,    42,     0,     0,    43,     0,     0,
      44,    45,     0,    46,    47,    48,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
       0,     0,     0,     0,     0,    51,     0,     0,    52,    53,
       0,    54,    55,     0,    56,     0,     0,     0,    57,     0,
      58,    59,    60,     0,    61,    62,    63,     0,    64,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,     0,    65,    66,
      67,    14,     0,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    19,    20,    21,    22,    23,    24,    25,     0,
       0,   114,   115,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,    33,   262,    35,    36,    37,    38,    39,
       0,    40,    41,    42,     0,     0,    43,     0,     0,    44,
      45,     0,   263,    47,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,   214,     0,     0,   121,    53,     0,
      54,    55,     0,   264,     0,   265,   266,    57,     0,    58,
      59,    60,     0,    61,    62,    63,     0,    64,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,     0,    65,   267,    67,
      14,     0,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    19,    20,    21,    22,    23,    24,    25,     0,     0,
     114,   115,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,   262,    35,    36,    37,    38,    39,     0,
      40,    41,    42,     0,     0,    43,     0,     0,    44,    45,
       0,   263,    47,    48,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,   514,     0,     0,
       0,     0,     0,   214,     0,     0,   121,    53,     0,    54,
      55,     0,   264,     0,   265,   266,    57,     0,    58,    59,
      60,     0,    61,    62,    63,     0,    64,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,     0,    65,   267,    67,    14,
       0,   109,   110,    17,    18,     0,     0,     0,     0,     0,
     111,   112,   113,    22,    23,    24,    25,     0,     0,   114,
     115,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      32,    33,   262,    35,    36,    37,    38,    39,     0,    40,
      41,    42,     0,     0,    43,     0,     0,    44,    45,     0,
     263,    47,    48,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,     0,     0,     0,
       0,     0,   214,     0,     0,   121,    53,     0,    54,    55,
       0,   734,     0,   265,   266,    57,     0,    58,    59,    60,
       0,    61,    62,    63,     0,    64,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,     0,    65,   267,    67,    14,     0,
     109,   110,    17,    18,     0,     0,     0,     0,     0,   111,
     112,   113,    22,    23,    24,    25,     0,     0,   114,   115,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,   262,    35,    36,    37,    38,    39,     0,    40,    41,
      42,     0,     0,    43,     0,     0,    44,    45,     0,   263,
      47,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,   895,     0,     0,     0,     0,
       0,   214,     0,     0,   121,    53,     0,    54,    55,     0,
     734,     0,   265,   266,    57,     0,    58,    59,    60,     0,
      61,    62,    63,     0,    64,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,     0,    65,   267,    67,    14,     0,   109,
     110,    17,    18,     0,     0,     0,     0,     0,   111,   112,
     113,    22,    23,    24,    25,     0,     0,   114,   115,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
     262,    35,    36,    37,    38,    39,     0,    40,    41,    42,
       0,     0,    43,     0,     0,    44,    45,     0,   263,    47,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
     214,     0,     0,   121,    53,     0,    54,    55,     0,   264,
       0,   265,     0,    57,     0,    58,    59,    60,     0,    61,
      62,    63,     0,    64,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,     0,    65,   267,    67,    14,     0,   109,   110,
      17,    18,     0,     0,     0,     0,     0,   111,   112,   113,
      22,    23,    24,    25,     0,     0,   114,   115,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    32,    33,   262,
      35,    36,    37,    38,    39,     0,    40,    41,    42,     0,
       0,    43,     0,     0,    44,    45,     0,   263,    47,    48,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,     0,     0,     0,     0,     0,   214,
       0,     0,   121,    53,     0,    54,    55,     0,     0,     0,
     265,   266,    57,     0,    58,    59,    60,     0,    61,    62,
      63,     0,    64,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,     0,    65,   267,    67,    14,     0,   109,   110,    17,
      18,     0,     0,     0,     0,     0,   111,   112,   113,    22,
      23,    24,    25,     0,     0,   114,   115,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,   262,    35,
      36,    37,    38,    39,     0,    40,    41,    42,     0,     0,
      43,     0,     0,    44,    45,     0,   263,    47,    48,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,   214,     0,
       0,   121,    53,     0,    54,    55,     0,   734,     0,   265,
       0,    57,     0,    58,    59,    60,     0,    61,    62,    63,
       0,    64,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
       0,    65,   267,    67,    14,     0,   109,   110,    17,    18,
       0,     0,     0,     0,     0,   111,   112,   113,    22,    23,
      24,    25,     0,     0,   114,   115,     0,     0,     0,     0,
       0,     0,     0,     0,    31,    32,    33,   262,    35,    36,
      37,    38,    39,     0,    40,    41,    42,     0,     0,    43,
       0,     0,    44,    45,     0,   263,    47,    48,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,    50,     0,     0,     0,     0,     0,   214,     0,     0,
     121,    53,     0,    54,    55,     0,     0,     0,   265,     0,
      57,     0,    58,    59,    60,     0,    61,    62,    63,     0,
      64,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,     0,
      65,   267,    67,    14,     0,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    19,    20,    21,    22,    23,    24,
      25,     0,     0,   114,   115,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,     0,    40,    41,    42,     0,     0,    43,     0,
       0,    44,    45,     0,    46,    47,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
      50,     0,     0,     0,     0,     0,   214,     0,     0,   121,
      53,     0,    54,    55,     0,   614,     0,     0,     0,    57,
       0,    58,    59,    60,     0,    61,    62,    63,     0,    64,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,     0,    65,
     267,    67,    14,     0,   109,   110,    17,    18,     0,     0,
       0,     0,     0,   111,   112,   113,    22,    23,    24,    25,
       0,     0,   114,   115,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,     0,    40,    41,    42,     0,     0,    43,     0,     0,
      44,    45,     0,    46,    47,    48,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
       0,     0,     0,     0,     0,   214,     0,     0,   121,    53,
       0,    54,    55,     0,   264,     0,     0,     0,    57,     0,
      58,    59,    60,     0,    61,    62,    63,     0,    64,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,     0,    65,   267,
      67,    14,     0,   109,   110,    17,    18,     0,     0,     0,
       0,     0,   111,   112,   113,    22,    23,    24,    25,     0,
       0,   114,   115,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,    42,     0,     0,    43,     0,     0,    44,
      45,     0,    46,    47,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,   214,     0,     0,   121,    53,     0,
      54,    55,     0,   614,     0,     0,     0,    57,     0,    58,
      59,    60,     0,    61,    62,    63,     0,    64,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,     0,    65,   267,    67,
      14,     0,   109,   110,    17,    18,     0,     0,     0,     0,
       0,   111,   112,   113,    22,    23,    24,    25,     0,     0,
     114,   115,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,     0,
      40,    41,    42,     0,     0,    43,     0,     0,    44,    45,
       0,    46,    47,    48,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,     0,     0,
       0,     0,     0,   214,     0,     0,   121,    53,     0,    54,
      55,     0,   941,     0,     0,     0,    57,     0,    58,    59,
      60,     0,    61,    62,    63,     0,    64,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,     0,    65,   267,    67,    14,
       0,   109,   110,    17,    18,     0,     0,     0,     0,     0,
     111,   112,   113,    22,    23,    24,    25,     0,     0,   114,
     115,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,     0,    40,
      41,    42,     0,     0,    43,     0,     0,    44,    45,     0,
      46,    47,    48,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,     0,     0,     0,
       0,     0,   214,     0,     0,   121,    53,     0,    54,    55,
       0,   734,     0,     0,     0,    57,     0,    58,    59,    60,
       0,    61,    62,    63,     0,    64,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,     0,    65,   267,    67,    14,     0,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    19,
      20,    21,    22,    23,    24,    25,     0,     0,    26,    27,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,     0,    40,    41,
      42,     0,     0,    43,     0,     0,    44,    45,     0,    46,
      47,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,     0,     0,     0,     0,
       0,   214,     0,     0,   121,    53,     0,    54,    55,     0,
       0,     0,     0,     0,    57,     0,    58,    59,    60,     0,
      61,    62,    63,     0,    64,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,     0,    65,    66,    67,    14,     0,   109,
     110,    17,    18,     0,     0,     0,     0,     0,   111,   112,
     113,    22,    23,    24,    25,     0,     0,   114,   115,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,     0,    40,    41,    42,
       0,     0,    43,     0,     0,    44,    45,     0,    46,    47,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
     214,     0,     0,   121,    53,     0,    54,    55,     0,     0,
       0,     0,     0,    57,     0,    58,    59,    60,     0,    61,
      62,    63,     0,    64,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,     0,    65,   267,    67,    14,     0,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    19,    20,    21,
      22,    23,    24,    25,     0,     0,   114,   115,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,     0,    40,    41,    42,     0,
       0,    43,     0,     0,    44,    45,     0,    46,    47,    48,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,     0,     0,     0,     0,     0,   214,
       0,     0,   121,    53,     0,    54,    55,     0,     0,     0,
       0,     0,    57,     0,    58,    59,    60,     0,    61,    62,
      63,     0,    64,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,     0,    65,   267,    67,    14,     0,   109,   110,    17,
      18,     0,     0,     0,     0,     0,   111,   112,   113,    22,
      23,    24,    25,     0,     0,   114,   115,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,   116,    35,
      36,    37,   117,    39,     0,    40,    41,    42,     0,     0,
      43,     0,     0,    44,    45,     0,   118,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,     0,     0,   120,     0,
       0,   121,    53,     0,    54,    55,     0,     0,     0,     0,
       0,    57,     0,    58,    59,    60,     0,    61,    62,    63,
       0,    64,     0,     0,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,     0,     0,     0,     0,
      14,   122,   109,   110,    17,    18,     0,     0,     0,     0,
       0,   111,   112,   113,    22,    23,    24,    25,     0,     0,
     114,   115,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,     0,
      40,    41,    42,     0,     0,    43,     0,     0,    44,    45,
       0,   227,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   228,     0,     0,    52,    53,     0,    54,
      55,     0,    56,     0,     0,     0,    57,     0,    58,    59,
      60,     0,    61,    62,    63,     0,    64,     0,     0,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,     0,     0,     0,     0,    14,   122,   109,   110,    17,
      18,     0,     0,     0,     0,     0,   111,   112,   113,    22,
      23,    24,    25,     0,     0,   114,   115,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,     0,    40,    41,    42,     0,     0,
      43,     0,     0,    44,    45,     0,   118,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   317,     0,
       0,   404,    53,     0,    54,    55,     0,   405,     0,     0,
       0,    57,     0,    58,    59,    60,     0,    61,    62,    63,
       0,    64,     0,     0,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,     0,     0,     0,     0,
      14,   122,   109,   110,    17,    18,     0,     0,     0,     0,
       0,   111,   112,   113,    22,    23,    24,    25,     0,     0,
     114,   115,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,   116,    35,    36,    37,   117,    39,     0,
      40,    41,    42,     0,     0,    43,     0,     0,    44,    45,
       0,   118,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   120,     0,     0,   121,    53,     0,    54,
      55,     0,     0,     0,     0,     0,    57,     0,    58,    59,
      60,     0,    61,    62,    63,     0,    64,     0,     0,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,     0,     0,     0,     0,    14,   122,   109,   110,    17,
      18,     0,     0,     0,     0,     0,   111,   112,   113,    22,
      23,    24,    25,     0,     0,   114,   115,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,     0,    40,    41,    42,     0,     0,
      43,     0,     0,    44,    45,     0,   118,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   317,     0,
       0,   404,    53,     0,    54,    55,     0,     0,     0,     0,
       0,    57,     0,    58,    59,    60,     0,    61,    62,    63,
       0,    64,     0,     0,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,     0,     0,     0,     0,
      14,   122,   109,   110,    17,    18,     0,     0,     0,     0,
       0,   111,   112,   113,    22,    23,    24,    25,     0,     0,
     114,   115,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,     0,
      40,    41,    42,     0,     0,    43,     0,     0,    44,    45,
       0,   118,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1036,     0,     0,   121,    53,     0,    54,
      55,     0,     0,     0,     0,     0,    57,     0,    58,    59,
      60,     0,    61,    62,    63,     0,    64,     0,     0,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,     0,     0,     0,     0,    14,   122,   109,   110,    17,
      18,     0,     0,     0,     0,     0,   111,   112,   113,    22,
      23,    24,    25,     0,     0,   114,   115,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,     0,    40,    41,    42,     0,     0,
      43,     0,     0,    44,    45,     0,   227,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1074,     0,
       0,   121,    53,     0,    54,    55,     0,     0,   690,   661,
       0,    57,   691,    58,    59,    60,     0,    61,    62,    63,
       0,    64,     0,     0,     0,     0,     0,   177,   178,   179,
     180,   181,   182,   183,   184,     0,     0,   185,   186,     0,
       0,   122,     0,   187,   188,   189,   190,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   191,   192,   193,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,     0,   204,   205,   675,   670,     0,     0,   676,     0,
     206,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,   178,   179,   180,   181,   182,   183,
     184,     0,     0,   185,   186,     0,     0,     0,     0,   187,
     188,   189,   190,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   191,   192,   193,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,     0,   204,   205,
     707,   661,     0,     0,   708,     0,   206,   278,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   177,
     178,   179,   180,   181,   182,   183,   184,     0,     0,   185,
     186,     0,     0,     0,     0,   187,   188,   189,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   191,
     192,   193,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,     0,   204,   205,   710,   670,     0,     0,
     711,     0,   206,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   177,   178,   179,   180,   181,
     182,   183,   184,     0,     0,   185,   186,     0,     0,     0,
       0,   187,   188,   189,   190,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   191,   192,   193,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,     0,
     204,   205,   717,   661,     0,     0,   718,     0,   206,   278,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,   180,   181,   182,   183,   184,     0,
       0,   185,   186,     0,     0,     0,     0,   187,   188,   189,
     190,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   191,   192,   193,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,     0,   204,   205,   720,   670,
       0,     0,   721,     0,   206,   278,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   177,   178,   179,
     180,   181,   182,   183,   184,     0,     0,   185,   186,     0,
       0,     0,     0,   187,   188,   189,   190,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   191,   192,   193,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,     0,   204,   205,   758,   661,     0,     0,   759,     0,
     206,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,   178,   179,   180,   181,   182,   183,
     184,     0,     0,   185,   186,     0,     0,     0,     0,   187,
     188,   189,   190,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   191,   192,   193,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,     0,   204,   205,
     761,   670,     0,     0,   762,     0,   206,   278,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   177,
     178,   179,   180,   181,   182,   183,   184,     0,     0,   185,
     186,     0,     0,     0,     0,   187,   188,   189,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   191,
     192,   193,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,     0,   204,   205,   946,   661,     0,     0,
     947,     0,   206,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   177,   178,   179,   180,   181,
     182,   183,   184,     0,     0,   185,   186,     0,     0,     0,
       0,   187,   188,   189,   190,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   191,   192,   193,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,     0,
     204,   205,   949,   670,     0,     0,   950,     0,   206,   278,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,   180,   181,   182,   183,   184,     0,
       0,   185,   186,     0,     0,     0,     0,   187,   188,   189,
     190,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   191,   192,   193,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,     0,   204,   205,  1147,   661,
       0,     0,  1148,     0,   206,   278,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   177,   178,   179,
     180,   181,   182,   183,   184,     0,     0,   185,   186,     0,
       0,     0,     0,   187,   188,   189,   190,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   191,   192,   193,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,     0,   204,   205,  1162,   661,     0,     0,  1163,     0,
     206,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,   178,   179,   180,   181,   182,   183,
     184,     0,     0,   185,   186,     0,     0,     0,     0,   187,
     188,   189,   190,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   191,   192,   193,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,     0,   204,   205,
    1165,   670,     0,     0,  1166,     0,   206,   278,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   177,
     178,   179,   180,   181,   182,   183,   184,     0,     0,   185,
     186,     0,     0,     0,     0,   187,   188,   189,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   191,
     192,   193,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,     0,   204,   205,   675,   670,     0,     0,
     676,     0,   206,   278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   177,   178,   179,   180,   181,
     182,   183,   184,     0,     0,   185,   186,     0,     0,     0,
       0,   187,   188,   189,   190,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   191,   192,   193,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   926,     0,     0,     0,     0,     0,     0,     0,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,     0,
     204,   205,     0,     0,     0,     0,     0,     0,   206,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,     0,     0,     0,     0,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   425,
       0,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     418,   419,   420,     0,     0,     0,     0,   421,   422,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   425,     0,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,     0,     0,     0,     0,     0,     0,     0,
       0,  -282,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,     0,     0,     0,     0,   421,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   425,     0,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,     0,     0,     0,     0,     0,     0,
       0,     0,  -284,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,     0,     0,     0,     0,   421,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   424,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   425,     0,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   435,     0,     0,     0,     0,     0,
       0,     0,     0,  -285,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,     0,     0,     0,     0,
     421,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   424,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   425,     0,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,     0,     0,     0,     0,
       0,     0,     0,     0,  -287,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   418,   419,   420,     0,     0,     0,
       0,   421,   422,     0,     0,     0,   506,     0,     0,     0,
       0,     0,     0,     0,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   425,     0,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,     0,
       0,     0,     0,   421,   422,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   424,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   425,     0,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,  -696,
    -696,     0,     0,     0,     0,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435
};

static const yytype_int16 yycheck[] =
{
       1,     9,    66,   392,    89,   394,    14,   383,    21,    26,
      27,   495,   287,    28,   289,    13,   224,   318,   513,    20,
       6,    87,    88,     8,     9,   486,     1,   311,     3,    14,
      28,   315,    56,    82,   438,   647,    52,    69,   407,     3,
      14,    52,   801,   604,     4,     5,    75,   773,     6,   274,
     378,   546,    12,    54,    55,    13,   601,   120,   336,   463,
     823,  1019,   824,  1044,    15,    16,   829,    26,    19,   145,
      28,   399,    58,    74,    75,   601,   480,   554,   604,   509,
      25,   323,    75,   513,    25,   489,    26,    15,    16,    20,
      21,    19,   105,    25,   701,    69,    56,    26,   705,   337,
      58,    91,   340,    54,   342,   712,   344,    25,   346,  1054,
     121,   112,    25,   725,   678,   679,    25,    60,    61,    62,
      63,    58,    82,    80,    25,    25,    54,    55,    15,    16,
      93,   124,    19,   375,   119,    91,    29,   465,    52,    91,
     122,   300,     1,   620,     3,     4,     5,    15,    16,     8,
       9,    19,   556,    12,   220,    14,    15,    16,   148,   122,
      19,   143,    91,    91,     0,   231,   975,    54,    55,  1150,
     127,    52,    15,    16,   105,   145,    19,   106,   139,    91,
     139,   112,   113,  1141,    18,   122,   145,   216,   217,   103,
      56,    25,   148,    52,   106,    28,   148,    56,   143,   139,
     131,   436,   143,    58,   145,   440,    16,    66,   443,   407,
     139,   143,   141,   975,  1159,   216,   217,   225,   226,   148,
     765,   114,   103,    82,   217,   143,   140,   145,   294,   464,
     143,    41,    42,   778,   143,    91,   148,   122,    93,   765,
     225,   226,   143,   143,   479,   404,   481,  1056,   143,   744,
     448,   449,   778,   554,   320,   490,    34,    18,   244,    20,
     119,    91,   121,    91,    91,   448,   449,   122,   122,   318,
     302,   272,    91,   274,    52,   801,    91,   278,   310,   311,
     254,   292,  1041,   315,   300,  1044,   244,   565,    91,   300,
    1053,   601,   148,   518,  1056,   859,   307,   532,  1024,    59,
      60,   286,   287,   288,   289,    93,   116,    93,    34,   119,
     120,    16,    61,   743,   744,    64,   558,   104,   148,   620,
     148,   148,   557,    93,   299,   563,    52,   278,   302,   148,
     281,   700,   809,   148,   122,    94,   122,   147,   402,   149,
      52,    93,   129,   407,    56,   148,   354,   355,   356,   357,
     278,    20,   122,   281,   213,   916,    25,   143,   318,   108,
      16,   689,    63,    93,    65,    66,   225,   226,   127,   354,
     355,   356,   357,   143,   359,   360,   145,    93,   353,   126,
     381,   405,   383,   358,   448,   449,   329,   122,   404,   353,
     916,  1150,   122,   404,   281,    59,    60,  1160,   848,   462,
      93,   790,   791,   792,   854,   794,   122,   796,  1015,   785,
     814,   116,    25,   281,   119,   120,   117,   118,    93,   278,
     850,    63,   281,    65,    66,   886,    56,   286,   287,   288,
     289,    56,    93,   292,    16,   278,   143,   438,   281,    20,
     299,   300,   147,    93,   149,   405,   143,   122,   307,    93,
     116,   402,    58,   119,   120,   765,   407,    93,   733,   318,
     116,   122,   463,   119,   120,   476,    37,    38,   778,   139,
     471,   755,   122,   484,   802,   117,   118,   122,   122,   480,
     102,   466,   440,   494,   146,   443,   122,   143,   489,   142,
     507,   147,   140,   149,   353,   354,   355,   356,   357,   358,
     359,   360,   143,   552,    56,   554,   464,   304,   809,   440,
     438,   308,   737,   528,   102,  1041,   517,   518,  1044,   378,
     728,    62,  1017,   481,    65,    66,  1138,  1134,   529,   857,
     528,  1015,   490,   464,   116,   463,    91,   119,   120,    93,
     399,   102,   516,   402,   323,   404,   405,   498,   407,    58,
     481,   106,   480,   564,   529,   556,   932,   933,   122,   490,
     122,   489,    93,   538,   630,   147,   143,   149,   122,   844,
     528,   620,    41,    42,   532,   903,   117,   118,   813,   438,
     815,   509,    17,    18,    52,   513,   141,  1017,    16,   448,
     449,   122,   552,   148,   554,   438,   375,   143,    52,   557,
     910,   532,   594,   143,   463,   116,   465,   466,   119,   120,
     920,   143,   613,   811,    52,   543,    70,   476,   546,   143,
     463,   480,   509,   122,  1150,   484,   557,   902,   556,   143,
     489,    15,   649,    74,    75,   494,   147,   480,   149,  1028,
    1029,  1030,  1031,   928,   929,    13,   489,   845,   102,   103,
     104,   100,   665,   851,   852,   122,   543,    93,   122,    91,
     620,   674,   845,   272,   677,   274,   994,   668,   851,   852,
     529,   896,    16,   674,   106,   129,   677,   678,   679,   538,
     688,   889,   123,   124,    64,    27,   122,    15,   116,    93,
     146,   119,   120,   552,   695,   554,   146,   556,   140,   700,
     701,   310,   801,   688,   705,   564,   143,   143,   723,   141,
     143,   712,   698,   556,   722,    15,   148,   514,   122,   147,
     143,   149,    15,   755,   521,   723,   143,  1131,    45,   526,
     674,  1041,   591,   677,   665,  1111,   737,   722,   535,   143,
     698,    37,    38,   674,    93,   122,   677,   811,   591,   700,
     116,   695,   611,   119,   120,    62,   142,  1146,    65,    66,
     809,   620,   693,   218,   695,   723,   123,   124,   611,   224,
     855,   142,    26,   122,   116,   216,   217,   119,   120,   558,
      15,   845,   846,   149,   785,   983,    18,   851,   852,    93,
     998,   588,   589,    15,   143,   142,  1004,   142,    45,    52,
     983,    54,    55,    56,    57,   147,   261,   149,   587,   801,
     117,   118,    91,   814,   140,   743,   744,    70,   122,   140,
     116,   140,   619,   119,   120,   604,    91,   106,   607,   688,
     689,   142,   594,  1143,   143,   810,   598,    91,    58,   143,
     143,   106,   143,    45,  1154,    58,   143,   858,   143,   809,
      58,   147,   106,   149,    52,    56,   743,   815,   859,    52,
     871,    26,   141,   722,    91,   143,   874,  1142,   869,   148,
     143,   872,   883,   884,    91,   143,   141,  1085,  1086,   106,
     145,   892,   323,   148,   815,   139,   814,   141,    15,   874,
     143,   145,    14,   904,   148,   896,    15,  1132,   353,    94,
     697,   116,    15,   116,   119,   120,   119,   120,   517,   518,
     146,   124,   367,   116,   141,   143,   119,   120,   143,   983,
     143,   148,   850,    15,   143,   143,    91,   143,   142,   140,
     385,   932,   933,   142,   375,    52,   149,   143,   143,  1038,
     143,   106,  1041,   802,   147,  1044,   149,  1046,    56,   143,
     809,   810,   811,   143,     1,   814,     3,   754,  1170,  1171,
      15,     8,     9,   850,   140,    15,    91,    14,    15,    16,
     930,   814,    19,    15,   139,   935,   141,   774,  1186,    15,
     145,   106,   993,   148,   140,   143,   845,   846,   127,  1201,
    1001,   127,   851,   852,  1202,  1203,  1208,  1209,   857,   858,
     143,   143,  1013,  1011,  1012,    52,   140,    56,   617,    15,
    1076,    56,   871,  1112,  1015,   874,   141,   143,    15,    66,
     143,    15,   801,   148,   883,   884,  1011,  1012,   143,   143,
     485,   486,   143,   892,   143,    91,   143,   997,    52,   801,
      54,    55,    56,    57,   903,   904,  1038,    91,   143,  1041,
     106,  1150,  1044,  1152,  1046,  1154,    70,  1156,   145,   668,
     145,   142,   106,   143,   143,    91,    91,    15,  1079,   102,
    1081,   930,   119,  1084,   121,  1092,   935,    15,    41,    12,
     106,   106,    42,   538,     5,   141,  1087,  1134,   801,  1017,
     545,  1190,   148,    64,    65,    66,  1141,   141,   895,   132,
     133,   134,   529,   856,   148,  1058,    91,   972,  1133,   824,
    1111,   256,     6,   594,   911,   141,   141,   558,  1038,   728,
    1112,   106,   148,   148,   983,  1136,  1137,    -1,   737,   143,
    1131,  1041,  1133,  1134,   993,   994,    -1,   916,   997,   918,
      -1,    -1,  1001,   922,   632,    -1,   117,   118,    -1,    -1,
      -1,    -1,  1011,  1012,  1013,    -1,   141,    -1,  1150,   647,
    1152,    -1,  1154,   148,  1156,    -1,   213,    -1,    -1,  1170,
    1171,    -1,    -1,     1,  1132,     3,     4,     5,   225,   226,
      -1,    26,    26,    -1,    12,    -1,  1197,   632,    -1,    -1,
    1198,  1199,    -1,    -1,    -1,  1206,  1207,    -1,  1190,    -1,
    1201,  1132,   647,  1131,    -1,    -1,    -1,  1208,  1209,    -1,
      64,    65,    66,  1198,  1199,   823,    -1,   937,   938,    -1,
    1079,   829,  1081,    -1,    52,  1084,    -1,    -1,    56,    -1,
      -1,   278,   687,    91,   281,    91,    -1,   725,    -1,   286,
     287,   288,   289,  1022,  1023,   292,    91,    91,   106,    -1,
     106,    -1,   299,   300,    82,    -1,    -1,    26,    -1,    -1,
     307,   106,   106,   117,   118,  1044,    -1,  1046,    -1,    -1,
      -1,    -1,  1131,   728,    -1,    -1,  1038,  1136,  1137,  1041,
     725,    -1,  1044,   141,  1046,   141,    -1,   896,  1131,    -1,
     148,    -1,   148,   121,   139,   139,   141,   141,    -1,    91,
     145,   145,    -1,   148,   148,    -1,   353,   354,   355,   356,
     357,   358,   359,   360,   106,    -1,    -1,    -1,    91,    -1,
    1099,  1041,    91,  1102,   448,   449,    -1,    64,    65,    66,
      -1,   378,   787,   106,    -1,   823,    -1,   106,  1197,  1198,
    1199,   829,    -1,   951,    -1,    -1,    -1,  1206,  1207,   141,
    1112,    -1,   399,    -1,    -1,   402,   148,   404,   482,   483,
     407,    64,    65,    66,    -1,   973,  1145,    -1,   141,    -1,
     139,  1150,   141,  1152,    -1,   148,   145,  1156,   823,   148,
     117,   118,    -1,    -1,   829,   213,    -1,    -1,  1150,    -1,
    1152,   438,  1154,    -1,  1156,  1115,  1116,  1117,    -1,  1119,
    1120,   448,   449,    -1,    -1,    -1,   861,    -1,    -1,   533,
      -1,  1190,    -1,    -1,   117,   118,   463,    -1,   465,   466,
      64,    65,    66,    64,    65,    66,    -1,    -1,  1190,   476,
      -1,   886,    -1,   480,   889,    -1,    -1,   484,    -1,    -1,
      -1,    -1,   489,    -1,    -1,  1053,  1054,   494,    -1,    -1,
    1058,    -1,    -1,    -1,    64,    65,    66,  1177,  1178,  1179,
    1180,    -1,    -1,   951,   292,    -1,    64,    65,    66,    -1,
      -1,   299,   300,   117,   118,    -1,   117,   118,    -1,   307,
    1200,    -1,   529,    -1,   972,   973,    -1,    -1,    -1,    -1,
     318,   538,    13,    14,    15,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,   951,   117,   118,   556,
      -1,    52,    -1,    54,    55,    56,    57,   564,  1126,   117,
     118,    -1,    -1,    -1,    -1,   353,    -1,   972,   973,    70,
     358,    -1,    -1,    -1,    -1,    52,    -1,    54,    55,    56,
      57,    -1,   116,   998,   591,   119,   120,    -1,    -1,  1004,
     378,  1159,  1160,    70,    95,    41,    42,    43,    44,    45,
     101,   102,   103,   104,   611,  1053,  1054,    -1,    -1,    -1,
    1058,   399,    -1,   147,    -1,   149,   404,   405,    95,   407,
      -1,    -1,    -1,    -1,   101,    -1,  1194,    -1,   129,    -1,
      -1,   132,    -1,    -1,    -1,   116,    -1,    -1,   119,   120,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,  1053,  1054,
      -1,    -1,    -1,  1058,    -1,    -1,    -1,    -1,    -1,   140,
     448,   449,    -1,    -1,    -1,   146,   147,    -1,   149,    -1,
    1085,  1086,    -1,    -1,    -1,    -1,    -1,   465,  1126,    -1,
      -1,   688,   689,    -1,    89,    90,    -1,    -1,   476,    52,
    1138,    54,    55,    56,    57,    -1,   484,   102,    -1,    52,
      -1,    54,    55,    56,    57,    -1,   494,    70,    -1,    -1,
      -1,  1159,  1160,    -1,    -1,   722,    -1,    70,    -1,    -1,
      -1,  1126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    95,  1138,    -1,    -1,    -1,    -1,   101,    -1,
      -1,   529,    95,    -1,    -1,    -1,  1194,    34,    35,    36,
     538,    -1,    -1,    -1,  1159,  1160,    -1,    -1,    -1,    -1,
      -1,   845,   846,    -1,   552,    52,   554,   851,   852,    56,
      -1,  1186,    59,    60,    -1,    -1,   564,    64,    -1,    -1,
       1,    -1,     3,     4,     5,    89,    90,  1202,  1203,  1194,
      -1,    12,    -1,   877,   878,   802,   880,   881,   102,    -1,
      -1,    -1,    -1,   810,   811,    92,    -1,   814,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,    -1,    -1,    -1,    52,
     107,    54,    55,    56,    57,   129,   130,   131,   132,   133,
     134,    52,   620,    -1,    -1,    56,    -1,    70,   845,   846,
      -1,   128,    -1,    -1,   851,   852,    -1,    -1,   135,    -1,
     857,   858,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,   871,    -1,    -1,   874,    -1,    52,
      -1,    54,    55,    56,    57,    -1,   883,   884,    -1,    -1,
      -1,    -1,    89,    90,    -1,   892,    -1,    70,    -1,    -1,
      -1,     0,    -1,    -1,    -1,   102,   903,   904,    -1,   983,
     121,   689,    -1,    -1,    13,    14,    15,    16,    17,    18,
     102,    20,    95,    -1,    -1,    -1,    25,    26,    27,   102,
     103,   104,  1006,   130,   131,   132,   133,   134,    37,    38,
      -1,    -1,    41,    42,    43,    44,    45,    -1,   130,   131,
     132,   133,   134,    -1,    -1,    -1,   129,     1,    -1,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,   983,    54,    55,    56,
      57,    -1,    91,    -1,    -1,    -1,   993,   994,    -1,    -1,
      -1,    -1,   213,    70,  1001,    -1,    -1,   106,    52,    -1,
      -1,    -1,    56,    -1,  1011,  1012,  1013,   116,    -1,    -1,
     119,   120,    -1,    -1,   802,    -1,    -1,    -1,    95,    -1,
      -1,   809,   810,   811,   101,   102,   103,   104,    82,    -1,
     139,   140,    -1,    -1,    -1,    -1,   145,   146,   147,   148,
     149,    52,    -1,    54,    55,    56,    57,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,   132,    -1,   845,    -1,    70,
      -1,    -1,    -1,   851,   852,    -1,   143,   121,    -1,   857,
     858,   292,  1079,    84,  1081,    -1,    -1,  1084,   299,   300,
      -1,    -1,    -1,   871,    95,    -1,   307,    34,    35,    36,
     101,   102,   103,   104,    -1,   883,   884,   318,    -1,    -1,
      -1,    -1,    -1,    -1,   892,    52,    -1,    -1,    -1,    56,
      -1,   122,    59,    60,    -1,   903,   904,    64,   129,    -1,
      -1,   132,    -1,    -1,  1131,    -1,    -1,    -1,    -1,  1136,
    1137,    -1,   353,    -1,   145,    -1,    -1,   358,    -1,    -1,
      -1,    -1,   930,    -1,    -1,    92,    -1,   935,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,   103,   378,    -1,   213,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    55,    56,    57,    -1,   399,    -1,
      -1,   128,    -1,   404,   405,    -1,   407,    -1,   135,    70,
    1197,  1198,  1199,    -1,    -1,   983,    -1,    -1,    -1,  1206,
    1207,    -1,    -1,    -1,    -1,   993,   994,    -1,    -1,   997,
      -1,    -1,    -1,  1001,    95,    -1,    -1,    34,    35,    36,
     101,   102,   103,   104,    -1,  1013,    -1,   448,   449,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,   292,    56,
      -1,    -1,    59,    60,   465,   299,   300,    64,   129,    -1,
      -1,   132,    -1,   307,    -1,   476,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   484,   318,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   494,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,    -1,    -1,    -1,    -1,
     107,  1079,    -1,  1081,    -1,    -1,  1084,    -1,    -1,   353,
      -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,   529,    -1,
      -1,   128,    34,    35,    36,    -1,    -1,   538,   135,    -1,
      -1,    -1,    -1,    -1,   378,    -1,    -1,    -1,    -1,    -1,
      52,   552,    -1,   554,    56,    -1,    -1,    59,    60,    -1,
      -1,    -1,    64,   564,    -1,   399,    -1,    -1,  1136,  1137,
     404,   405,    -1,   407,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      92,    -1,    12,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   448,   449,    -1,    -1,    -1,   620,
      -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,  1197,
      -1,   465,    52,   135,    -1,    -1,    56,    -1,  1206,  1207,
      -1,    -1,   476,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     484,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
     494,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   689,    -1,
      -1,    -1,    -1,    -1,    -1,   529,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,   538,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    -1,   552,    -1,
     554,    89,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
     564,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    12,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    89,    90,   620,    -1,    -1,    -1,
      -1,    -1,    -1,   213,    -1,    -1,    52,   102,    -1,    -1,
      -1,   802,    -1,    -1,    -1,    -1,    -1,    -1,   809,   810,
     811,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    52,   845,    54,    55,    56,    57,    -1,
     851,   852,    -1,    -1,    -1,   689,   857,   858,    -1,    -1,
      -1,    70,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
     871,    -1,   292,    -1,    -1,    84,    -1,    -1,    -1,   299,
     300,    -1,   883,   884,    -1,    -1,    95,   307,    -1,    -1,
      -1,   892,   101,   102,   103,   104,    -1,    -1,   318,    -1,
      -1,    -1,   903,   904,    -1,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    54,    55,    56,    57,    -1,    -1,    -1,     1,
     129,     3,    52,   132,    54,    55,    56,    57,    70,   930,
      -1,    -1,    -1,   353,   935,    -1,   145,    -1,   358,    -1,
      70,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    86,   213,   378,   101,
     102,   103,   104,    -1,    -1,    95,    -1,    -1,   802,    -1,
      52,   101,   102,   103,   104,   809,   810,   811,    -1,   399,
      -1,    -1,   983,    -1,   404,   405,    -1,   129,    -1,    -1,
     132,    -1,   993,   994,    -1,    -1,   997,    -1,    -1,   129,
    1001,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   845,  1013,    -1,    -1,    -1,    -1,   851,   852,    -1,
      -1,    -1,    -1,   857,   858,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   292,   871,    -1,   121,
      -1,    -1,    -1,   299,   300,   465,    -1,    -1,    -1,   883,
     884,   307,    -1,    -1,    -1,    -1,   476,    -1,   892,    -1,
      -1,    -1,    -1,    -1,   484,    -1,    -1,    -1,    -1,   903,
     904,    -1,    -1,    -1,   494,    -1,    -1,    -1,  1079,    -1,
    1081,    -1,    -1,  1084,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   930,   353,    -1,    -1,
      -1,   935,   358,    52,    -1,    54,    55,    56,    57,   529,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   538,    -1,
      -1,    70,   378,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   213,   552,    -1,   554,  1136,  1137,    86,    -1,    -1,
      -1,    -1,    -1,   399,   564,    -1,    95,    -1,   404,   983,
      -1,   407,   101,   102,   103,   104,    -1,    -1,    -1,   993,
     994,    -1,    -1,   997,    -1,    -1,    -1,  1001,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1013,
     129,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   448,   449,    -1,    -1,  1197,    -1,    -1,    -1,
     620,    -1,    -1,    -1,    -1,  1206,  1207,    -1,    -1,   465,
     292,    -1,    -1,    -1,    -1,    -1,    -1,   299,   300,    -1,
     476,    -1,    -1,    -1,    -1,   307,    -1,    -1,   484,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   494,    -1,
      -1,    -1,    -1,    -1,    -1,  1079,    -1,  1081,    -1,    -1,
    1084,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   689,
      -1,   353,    -1,   529,    -1,    -1,   358,    -1,    -1,    -1,
      -1,    -1,   538,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    -1,   378,    -1,    -1,    89,
      90,    -1,  1136,  1137,    -1,    -1,    -1,    -1,   564,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,   399,    -1,    -1,
      -1,    -1,   404,    -1,    -1,   407,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1197,    -1,    -1,   448,   449,    -1,    -1,
      -1,    -1,  1206,  1207,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   802,   465,    -1,    -1,    -1,    -1,    -1,   809,
     810,    -1,    -1,    -1,   476,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   484,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   494,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   689,    -1,    -1,    -1,   857,   858,    -1,
      -1,    73,    74,    75,    76,    77,    78,   529,    -1,    81,
      82,   871,    -1,    -1,    -1,    -1,   538,    89,    90,    -1,
      -1,    -1,    -1,   883,   884,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   892,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   564,   903,   904,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    73,    74,    75,    76,    77,    78,
     930,    -1,    81,    82,    -1,   935,    -1,    -1,    -1,    -1,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   802,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   810,   811,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   993,   994,    -1,    -1,   997,    -1,    -1,
      -1,  1001,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   845,
      -1,    -1,    -1,  1013,    -1,   851,   852,    -1,    -1,    -1,
      -1,   857,   858,    -1,    -1,    -1,    -1,   689,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   871,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   883,   884,    73,
      74,    75,    76,    77,    78,    79,   892,    81,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,   903,   904,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,  1079,
      -1,  1081,    -1,    -1,  1084,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    16,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    48,    49,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,  1136,  1137,    -1,    -1,
     802,    -1,    66,    67,    -1,    -1,    -1,   983,   810,   811,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   993,   994,    -1,
      -1,   997,    -1,    -1,    -1,  1001,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1013,    -1,    -1,
      -1,    -1,    -1,   845,    -1,    -1,    -1,    -1,    -1,   851,
     852,    -1,    -1,    -1,    -1,   857,   858,  1197,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1206,  1207,    -1,   871,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   883,   884,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     892,    52,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,   903,   904,  1079,    -1,  1081,    -1,    -1,  1084,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1136,  1137,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   983,    -1,   144,   145,    -1,    -1,    -1,    -1,    -1,
      -1,   993,   994,    -1,    -1,    -1,    -1,    -1,    -1,  1001,
     264,   265,   266,   267,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1013,    -1,    -1,   278,    52,    53,   281,    -1,    56,
      -1,  1197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1206,  1207,    -1,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1079,    -1,  1081,
      -1,    -1,  1084,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,    -1,
      -1,    -1,    -1,    -1,    -1,   379,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   390,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1136,  1137,    -1,    -1,   402,    -1,
      -1,    -1,    -1,   407,    -1,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,    -1,
     424,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   435,    -1,    -1,   438,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   448,   449,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1197,    -1,    -1,    -1,   463,
      -1,    -1,    -1,    -1,  1206,  1207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   478,    -1,   480,    -1,   482,   483,
      -1,    -1,    -1,    -1,    -1,   489,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   498,    -1,    -1,   501,   502,    -1,
      -1,    -1,   506,    -1,    -1,   509,    -1,   511,    -1,   513,
     514,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   533,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   543,
      -1,    -1,   546,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   556,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   573,
     574,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   591,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   611,    -1,    -1,
     614,    -1,     0,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   700,    85,    86,   703,
     704,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,
      98,    99,    -1,   101,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,    -1,
     734,   119,   120,    -1,    45,    -1,    -1,    -1,    -1,   743,
     744,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
      -1,   149,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   806,    -1,    -1,    -1,    -1,   811,   812,    -1,
     814,    -1,   123,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   845,   846,    -1,    -1,    -1,   850,   851,   852,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   877,   878,    -1,   880,   881,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   891,    -1,    -1,
      -1,   895,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   912,   913,
      -1,    -1,    -1,    -1,    -1,    -1,     0,    -1,    -1,    -1,
      -1,   925,   926,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      14,    15,    16,    17,    18,    -1,    20,   941,    -1,    -1,
      -1,    25,    -1,    27,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    41,    42,    43,
      44,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   976,   977,    58,    -1,    -1,    -1,    -1,   983,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    -1,  1006,    -1,    -1,    89,    90,    91,    -1,    -1,
      94,    -1,    -1,  1017,    -1,    -1,   100,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,   116,    -1,    -1,   119,   120,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,     0,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,    -1,    13,    14,
      15,    16,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    -1,    41,    42,    43,    44,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1131,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    89,    90,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,   100,    -1,   102,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,   119,   120,    -1,    -1,   123,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,    -1,     0,    -1,   139,   140,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    13,    14,    15,    16,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    -1,    41,    42,    43,    44,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    89,    90,    91,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,    -1,   102,    -1,    -1,    -1,   106,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,   119,   120,    -1,   122,   123,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,     0,
      -1,    -1,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    13,    14,    15,    16,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,
      41,    42,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    89,    90,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   100,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,   119,   120,
      -1,    -1,   123,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,     0,    -1,   139,   140,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    13,
      14,    15,    16,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    41,    42,    43,
      44,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    89,    90,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,   100,    -1,   102,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,   119,   120,    -1,    -1,   123,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,     0,    -1,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    13,    14,    15,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    -1,    41,    42,    43,    44,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    89,    90,    91,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,   119,   120,    -1,   122,   123,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,    -1,
       0,    -1,   139,   140,   141,    -1,   143,    -1,    -1,   146,
     147,   148,   149,    13,    14,    15,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      -1,    41,    42,    43,    44,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    89,
      90,    91,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,   119,
     120,    -1,   122,   123,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,     0,    -1,   139,
     140,   141,    -1,   143,    -1,    -1,   146,   147,   148,   149,
      13,    14,    15,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    41,    42,
      43,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    89,    90,    91,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,   119,   120,    -1,   122,
     123,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,    -1,     0,    -1,    -1,   140,   141,    -1,
     143,    -1,    -1,   146,   147,   148,   149,    13,    14,    15,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    -1,    41,    42,    43,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    89,    90,    91,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,   119,   120,    -1,   122,   123,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,    -1,    -1,
     146,   147,   148,   149,     1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    -1,
      -1,    18,    19,    -1,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,
      -1,    98,    99,    -1,   101,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,   116,
      -1,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,     1,   149,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,    -1,    15,    -1,    17,    18,    19,
      -1,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,    99,
      -1,   101,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,   116,    -1,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,     1,   149,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      -1,    -1,    15,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    95,    96,    -1,    98,    99,    -1,   101,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,   116,    -1,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,     1,   149,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    95,
      96,    -1,    98,    99,    -1,   101,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
     116,    -1,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,     1,   149,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,
      99,    -1,   101,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,    -1,    -1,
     119,   120,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    -1,    -1,   135,   136,   137,    -1,
      19,    -1,    21,    22,    23,    24,    -1,    -1,   147,    -1,
     149,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,
      99,    -1,   101,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,   116,    -1,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,   147,     1,
     149,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    -1,    14,    15,    -1,    -1,    -1,    19,    -1,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,   101,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,    -1,    -1,   119,   120,     1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    -1,    -1,   135,   136,   137,    -1,    19,    -1,    21,
      22,    23,    24,    -1,    -1,   147,    -1,   149,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,   101,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,    -1,    -1,   119,   120,     1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    -1,    -1,   135,   136,   137,    -1,    19,    -1,    21,
      22,    23,    24,    -1,   146,   147,    -1,   149,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,   101,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,    -1,    -1,   119,   120,     1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    -1,    -1,   135,   136,   137,    -1,    19,    -1,    21,
      22,    23,    24,    -1,   146,   147,    -1,   149,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,   101,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,    -1,    -1,   119,   120,     1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    -1,    -1,   135,   136,   137,    -1,    19,    -1,    21,
      22,    23,    24,    -1,   146,   147,    -1,   149,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,   101,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,   116,    -1,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   135,   136,   137,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,   147,     1,   149,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,    -1,
      15,    -1,    -1,    -1,    19,    -1,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      95,    96,    -1,    98,    99,    -1,   101,    -1,    -1,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,   116,    -1,    -1,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,    -1,   149,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,
      98,    99,    -1,   101,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,    -1,
      -1,   119,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,   135,   136,   137,
      -1,    19,   140,    21,    22,    23,    24,    -1,   146,   147,
      -1,   149,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,
      98,    99,    -1,   101,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,    -1,
      -1,   119,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,   135,   136,   137,
      -1,    19,    -1,    21,    22,    23,    24,    -1,    -1,   147,
      -1,   149,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,    -1,
      -1,   119,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,   135,   136,   137,
      -1,    19,    -1,    21,    22,    23,    24,    -1,    -1,   147,
      -1,   149,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,    -1,
      -1,   119,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,   135,   136,   137,
      -1,    19,    -1,    21,    22,    23,    24,    -1,    -1,   147,
      -1,   149,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,   116,    -1,
      -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    21,    22,    23,    24,    -1,    26,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,
      98,    99,    -1,   101,    -1,   103,   104,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
      -1,   139,    -1,    -1,    -1,    -1,    -1,   145,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    21,    22,    23,    24,
      -1,    26,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      95,    96,    -1,    98,    99,    -1,   101,    -1,   103,   104,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    -1,
     135,   136,   137,    19,   139,    21,    22,    23,    24,    -1,
     145,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    95,
      96,    -1,    98,    99,    -1,   101,    -1,   103,   104,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,   122,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,   135,
     136,   137,    -1,    19,    -1,    21,    22,    23,    24,   145,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    95,
      96,    -1,    98,    99,    -1,   101,    -1,   103,   104,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,   122,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,   135,
     136,   137,    -1,    19,    -1,    21,    22,    23,    24,   145,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    95,
      96,    -1,    98,    99,    -1,   101,    -1,   103,   104,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    -1,   135,
     136,   137,    19,    -1,    21,    22,    23,    24,    -1,   145,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,
      -1,    98,    99,    -1,   101,    -1,   103,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
     144,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,   144,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,   144,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,   144,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    95,
      96,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,   135,
      21,    22,    23,    24,    -1,    -1,    -1,   143,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,   135,    21,    22,    23,    24,    -1,
      -1,    -1,   143,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    95,
      96,    -1,    98,    99,    -1,   101,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    -1,    -1,    -1,   135,
     136,   137,    19,    -1,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,
      -1,    98,    99,    -1,   101,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    -1,   135,   136,
     137,    19,    -1,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,
      98,    99,    -1,   101,    -1,   103,   104,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    -1,   135,   136,   137,
      19,    -1,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,
      99,    -1,   101,    -1,   103,   104,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    -1,   135,   136,   137,    19,
      -1,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,    99,
      -1,   101,    -1,   103,   104,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    -1,   135,   136,   137,    19,    -1,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,
     101,    -1,   103,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    -1,   135,   136,   137,    19,    -1,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,   101,
      -1,   103,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    -1,   135,   136,   137,    19,    -1,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    95,    96,    -1,    98,    99,    -1,    -1,    -1,
     103,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    -1,   135,   136,   137,    19,    -1,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    95,    96,    -1,    98,    99,    -1,   101,    -1,   103,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      -1,   135,   136,   137,    19,    -1,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      95,    96,    -1,    98,    99,    -1,    -1,    -1,   103,    -1,
     105,    -1,   107,   108,   109,    -1,   111,   112,   113,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    -1,
     135,   136,   137,    19,    -1,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    95,
      96,    -1,    98,    99,    -1,   101,    -1,    -1,    -1,   105,
      -1,   107,   108,   109,    -1,   111,   112,   113,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    -1,   135,
     136,   137,    19,    -1,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,
      -1,    98,    99,    -1,   101,    -1,    -1,    -1,   105,    -1,
     107,   108,   109,    -1,   111,   112,   113,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    -1,   135,   136,
     137,    19,    -1,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,
      98,    99,    -1,   101,    -1,    -1,    -1,   105,    -1,   107,
     108,   109,    -1,   111,   112,   113,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    -1,   135,   136,   137,
      19,    -1,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,
      99,    -1,   101,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    -1,   135,   136,   137,    19,
      -1,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,    99,
      -1,   101,    -1,    -1,    -1,   105,    -1,   107,   108,   109,
      -1,   111,   112,   113,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    -1,   135,   136,   137,    19,    -1,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,
     111,   112,   113,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    -1,   135,   136,   137,    19,    -1,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    61,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    95,    96,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,
     112,   113,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    -1,   135,   136,   137,    19,    -1,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    95,    96,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,   105,    -1,   107,   108,   109,    -1,   111,   112,
     113,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    -1,   135,   136,   137,    19,    -1,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    -1,
      -1,    95,    96,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,   135,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,
      99,    -1,   101,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,   135,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    95,    96,    -1,    98,    99,    -1,   101,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,   135,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,   135,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    95,    96,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,   135,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    95,    96,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,   108,
     109,    -1,   111,   112,   113,    -1,   115,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,   135,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    95,    96,    -1,    98,    99,    -1,    -1,    52,    53,
      -1,   105,    56,   107,   108,   109,    -1,   111,   112,   113,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    -1,    81,    82,    -1,
      -1,   135,    -1,    87,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,   137,    52,    53,    -1,    -1,    56,    -1,
     144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,   137,
      52,    53,    -1,    -1,    56,    -1,   144,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,   137,    52,    53,    -1,    -1,
      56,    -1,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,   137,    52,    53,    -1,    -1,    56,    -1,   144,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,   137,    52,    53,
      -1,    -1,    56,    -1,   144,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,   137,    52,    53,    -1,    -1,    56,    -1,
     144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,   137,
      52,    53,    -1,    -1,    56,    -1,   144,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,   137,    52,    53,    -1,    -1,
      56,    -1,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,   137,    52,    53,    -1,    -1,    56,    -1,   144,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,   137,    52,    53,
      -1,    -1,    56,    -1,   144,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,   137,    52,    53,    -1,    -1,    56,    -1,
     144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,   137,
      52,    53,    -1,    -1,    56,    -1,   144,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,   137,    52,    53,    -1,    -1,
      56,    -1,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,   144,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    89,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   151,   152,     1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    19,    21,    22,    23,    24,    30,
      31,    32,    33,    34,    35,    36,    39,    40,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      59,    60,    61,    64,    67,    68,    70,    71,    72,    85,
      86,    92,    95,    96,    98,    99,   101,   105,   107,   108,
     109,   111,   112,   113,   115,   135,   136,   137,   153,   154,
     155,   161,   162,   164,   167,   169,   171,   172,   175,   176,
     178,   179,   180,   182,   183,   192,   206,   223,   244,   245,
     276,   277,   278,   282,   283,   284,   290,   291,   292,   294,
     295,   296,   297,   298,   299,   335,   348,     0,   155,    21,
      22,    30,    31,    32,    39,    40,    52,    56,    70,    89,
      92,    95,   135,   167,   169,   184,   185,   206,   223,   296,
     299,   335,   185,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    46,    47,
      48,    49,    50,    51,    52,    53,    56,    71,    72,    73,
      74,    75,    76,    77,    78,    81,    82,    87,    88,    89,
      90,   101,   102,   103,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   136,   137,   144,   145,   186,   190,
     191,   298,   330,   207,    92,   164,   167,   169,   170,   183,
     223,   296,   297,   299,   170,   213,   215,    70,    92,   176,
     183,   223,   228,   296,   299,    33,    34,    35,    36,    49,
      50,    51,    52,    56,   107,   186,   187,   188,   292,   116,
     119,   120,   147,   149,   170,   286,   287,   288,   341,   345,
     346,   347,    52,    70,   101,   103,   104,   136,   175,   192,
     198,   201,   204,   278,   333,   334,   198,   198,   145,   195,
     196,   199,   200,   348,   195,   200,   145,   342,   145,   342,
     187,   156,   139,   192,   223,   192,   192,   192,    56,     1,
      95,   158,   159,   161,   177,   178,   348,   208,   210,   193,
     204,   333,   348,   192,   332,   333,   348,    92,   143,   182,
     223,   296,   299,   211,    54,    55,    57,    64,    70,   108,
     186,   293,    64,    65,    66,   117,   118,   279,   280,    62,
     279,    63,   279,    64,   279,    64,   279,    59,    60,   171,
     192,   192,   341,   347,    41,    42,    43,    44,    45,    37,
      38,    52,    54,    55,    56,    57,    70,    84,    95,   101,
     102,   103,   104,   129,   132,   145,   302,   303,   304,   305,
     306,   309,   310,   311,   312,   314,   315,   316,   317,   319,
     320,   321,   324,   325,   326,   327,   328,   348,   302,   304,
      28,   243,   122,   143,    95,   101,   179,   122,    25,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    89,    90,    94,   102,   123,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    91,   106,   141,   148,
     339,    91,   339,   340,    26,   139,   247,   278,    93,    93,
     195,   200,   247,   164,    52,    56,   184,    59,    60,   303,
     126,   300,    91,   141,   339,   222,   331,    91,   148,   338,
     157,   158,    56,   302,   302,    16,   224,   345,   122,    91,
     141,   339,    93,    93,   224,   170,   170,    56,    91,   141,
     339,    25,   108,   143,   289,   341,   116,   288,    20,   249,
     345,    58,    58,   192,   192,   192,    94,   143,   202,   203,
     348,    58,   202,   203,    86,   197,   198,   204,   333,   348,
     198,   164,   341,   343,   164,   346,   164,   164,   160,   139,
     158,    91,   339,    93,   161,   177,   146,   341,   347,   343,
     158,   343,   142,   203,   344,   347,   203,   344,   140,   344,
      56,   179,   180,   181,   143,    91,   141,   339,   145,   240,
     314,   319,    64,   279,   281,   285,   286,    64,   280,    62,
      63,    64,    64,   102,   102,   155,   170,   170,   170,   170,
     161,   164,   164,    58,   122,    58,   345,   318,    86,   314,
     319,   122,   157,   192,   143,   329,   348,    52,   143,   329,
     345,   143,   313,   192,   143,   313,    52,   143,   313,    34,
      52,   122,   157,   242,   101,   171,   192,   204,   205,   177,
     143,   182,   143,   162,   163,   171,   183,   192,   194,   205,
     223,   299,   166,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   165,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
      52,    53,    56,   190,   195,   336,   337,   197,   204,    52,
      53,    56,   190,   195,   336,    52,    56,   336,   248,   246,
     163,   192,   194,   163,   194,   100,   174,   220,   301,   219,
      52,    56,   184,   336,   197,   336,   157,   164,   168,    15,
      13,   272,   348,   122,   122,   158,    16,    52,    56,   197,
      52,    56,   158,    27,   225,   345,   225,    52,    56,   197,
      52,    56,   217,   189,   158,    25,   249,   192,   204,    15,
     192,   192,   192,   342,   101,   192,   201,   333,   192,   334,
     343,   146,   341,   203,   203,   343,   146,   343,   187,   153,
     140,   194,   343,   161,   209,   333,   179,   181,    52,    56,
     197,    52,    56,   314,   212,   143,    64,   158,   286,   192,
     192,    52,    70,   101,   229,   319,   343,   343,   143,   175,
     192,    15,    52,    70,   306,   311,   328,    86,   312,   317,
     324,   326,   319,   321,   326,    52,   319,   175,   192,    15,
      80,   127,   234,   236,   348,   192,   203,   343,   181,   143,
      45,   122,    45,    91,   141,   339,    34,    35,    36,    52,
      56,    64,    92,    98,    99,   101,   103,   128,   255,   256,
     258,   259,   260,   261,   264,   265,   266,   268,   269,   270,
     271,   291,   295,   255,   342,    93,    93,   195,   200,   142,
     203,    93,    93,   196,   200,   196,   200,   234,   234,   173,
     345,   170,   157,   142,    15,   343,   186,   192,   205,   273,
     348,    18,   227,   348,    17,   226,   227,    93,    93,   142,
      93,    93,   227,   214,   216,   142,   170,   187,   140,   255,
      15,   203,   224,   192,   202,    86,   333,   140,   343,   344,
     142,   237,   342,    29,   114,   241,   140,   143,   316,   343,
     143,    86,    45,    45,   329,   345,   143,   313,   143,   313,
     143,   313,   143,   313,   313,    45,    45,   231,   233,   235,
     305,   307,   308,   311,   319,   320,   322,   323,   326,   328,
     157,   101,   192,   181,   161,   192,    52,    56,   197,    52,
      56,    58,    56,    52,   142,   258,   262,   263,   264,    52,
     140,   267,   268,   270,    52,    34,    52,    52,   258,   264,
     143,    94,   127,   143,    91,   143,    58,   124,   163,   194,
     171,   194,   174,    93,   163,   194,   163,   194,   174,   247,
     243,   157,   158,   234,   221,   345,    15,    94,   274,   348,
     158,    14,   275,   348,   170,    15,    93,    15,   158,   158,
     225,    41,    42,   224,   192,   158,   343,   203,   146,   147,
     157,   158,   230,   143,   101,   343,   192,   192,   319,   326,
     319,   319,   192,   192,   237,   237,    92,   223,   143,   329,
     329,   143,   232,   223,   143,   232,   143,   232,    15,   192,
     142,   258,   142,   143,   143,   140,   143,   143,   143,    52,
     260,   257,   258,    56,   269,   270,   192,   192,   163,   194,
      15,   140,   158,   157,    92,   183,   223,   296,   299,   224,
     158,   224,    15,    15,   218,   170,   170,   158,   227,   249,
     250,    52,   238,   239,   315,    15,   140,   319,   319,   143,
     316,   313,   143,   313,   313,   313,   127,   127,    56,    91,
     307,   311,   143,   231,   232,   323,   326,   319,   322,   326,
     319,   258,   264,   263,   270,   257,   143,   140,    15,    56,
      91,   141,   339,   158,   158,   158,   224,   224,    25,   227,
     251,   143,   342,   143,   319,   143,   319,    52,    56,   329,
     143,   232,   143,   232,   143,   232,   143,   232,   232,   143,
     143,   258,    52,    56,   197,    52,    56,   272,   226,    15,
     158,   158,   255,    15,   239,   319,   313,   319,   326,   319,
     319,   263,   264,   142,   251,   251,   252,   253,   254,   232,
     143,   232,   232,   232,   143,    15,    15,   224,    41,    42,
     319,   158,   170,   170,   232,   251,   224,   224,   158,   158,
     251,   251
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   150,   151,   152,   153,   154,   154,   154,   154,   155,
     156,   155,   157,   158,   159,   159,   159,   159,   160,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   163,   163,   163,   164,   164,
     164,   164,   164,   164,   165,   164,   166,   164,   164,   167,
     168,   169,   170,   171,   171,   172,   172,   173,   174,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     176,   176,   177,   177,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   179,   179,   180,   180,   181,   181,
     182,   182,   182,   182,   182,   182,   182,   182,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   184,   184,   185,
     185,   185,   186,   186,   186,   186,   186,   187,   187,   188,
     189,   188,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   193,   193,   193,   193,   194,   194,
     195,   195,   195,   196,   196,   197,   197,   197,   197,   197,
     198,   198,   198,   198,   198,   199,   200,   201,   201,   202,
     202,   203,   204,   204,   204,   204,   204,   204,   205,   205,
     205,   206,   206,   206,   206,   206,   206,   206,   206,   207,
     206,   208,   209,   206,   210,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   211,
     212,   206,   206,   206,   213,   214,   206,   215,   216,   206,
     206,   206,   206,   206,   206,   217,   218,   206,   219,   206,
     220,   221,   206,   222,   206,   206,   206,   206,   206,   206,
     206,   223,   224,   224,   224,   225,   225,   226,   226,   227,
     227,   228,   228,   229,   229,   229,   229,   229,   229,   229,
     229,   230,   229,   231,   231,   231,   231,   232,   232,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   234,   234,   235,   236,   236,   236,
     237,   237,   238,   238,   239,   239,   240,   240,   241,   241,
     242,   243,   244,   244,   244,   244,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   246,   247,   248,   247,   249,
     250,   250,   251,   252,   251,   253,   251,   254,   251,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   256,   256,
     257,   257,   258,   258,   259,   259,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   261,   261,   262,
     262,   262,   262,   262,   262,   263,   263,   264,   264,   265,
     265,   265,   266,   266,   267,   267,   267,   268,   268,   269,
     269,   270,   270,   270,   271,   272,   272,   273,   273,   273,
     274,   274,   275,   275,   276,   276,   276,   276,   277,   277,
     278,   278,   278,   278,   279,   279,   280,   281,   280,   280,
     280,   282,   282,   283,   283,   284,   285,   285,   286,   286,
     287,   287,   288,   289,   288,   290,   290,   291,   291,   291,
     292,   293,   293,   293,   293,   293,   293,   294,   294,   295,
     295,   295,   295,   296,   296,   296,   296,   296,   297,   297,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   299,
     299,   300,   301,   300,   302,   302,   303,   303,   303,   304,
     304,   304,   304,   305,   305,   306,   306,   307,   307,   308,
     308,   309,   309,   310,   310,   311,   311,   312,   312,   312,
     312,   313,   313,   313,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   315,
     315,   315,   315,   315,   316,   316,   317,   318,   317,   319,
     319,   320,   321,   322,   323,   323,   324,   324,   325,   325,
     326,   326,   327,   327,   328,   328,   328,   329,   329,   329,
     330,   331,   330,   332,   332,   333,   333,   334,   334,   334,
     334,   334,   334,   334,   334,   335,   335,   335,   336,   336,
     336,   336,   337,   337,   337,   338,   338,   339,   339,   340,
     340,   341,   341,   342,   342,   343,   344,   344,   344,   345,
     345,   346,   346,   347,   347,   348
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     1,     3,     2,     1,
       0,     5,     4,     2,     1,     1,     3,     2,     0,     4,
       2,     3,     3,     3,     3,     3,     4,     1,     3,     3,
       3,     3,     1,     3,     3,     6,     5,     5,     5,     5,
       4,     6,     4,     6,     3,     1,     3,     1,     1,     3,
       3,     3,     3,     2,     0,     4,     0,     4,     1,     2,
       0,     5,     1,     1,     1,     1,     4,     0,     5,     2,
       3,     4,     5,     4,     5,     2,     2,     2,     2,     2,
       1,     3,     1,     3,     1,     2,     3,     5,     2,     4,
       2,     4,     1,     3,     1,     3,     2,     3,     1,     2,
       1,     4,     3,     3,     3,     3,     2,     1,     1,     4,
       3,     3,     3,     3,     2,     1,     1,     1,     1,     2,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     6,     5,     5,     5,     5,
       4,     3,     3,     2,     2,     3,     2,     2,     3,     3,
       3,     3,     3,     3,     4,     4,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     3,     3,     3,     3,     6,     6,     4,
       6,     4,     6,     1,     1,     2,     4,     2,     1,     3,
       3,     5,     3,     1,     1,     1,     2,     2,     4,     2,
       1,     2,     2,     4,     1,     0,     2,     2,     1,     2,
       1,     2,     1,     1,     2,     3,     3,     4,     3,     4,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       4,     0,     0,     5,     0,     3,     3,     3,     2,     3,
       3,     1,     2,     4,     3,     4,     2,     1,     2,     0,
       0,     5,     6,     6,     0,     0,     7,     0,     0,     7,
       5,     4,     9,    11,    11,     0,     0,     9,     0,     6,
       0,     0,     8,     0,     5,     4,     4,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     5,     1,
       2,     1,     1,     1,     4,     6,     3,     5,     2,     4,
       1,     0,     4,     4,     2,     2,     1,     2,     0,     6,
       8,     4,     6,     4,     3,     6,     2,     4,     6,     2,
       4,     2,     4,     1,     1,     1,     0,     4,     1,     4,
       1,     4,     1,     3,     1,     1,     4,     1,     3,     3,
       0,     5,     2,     4,     5,     5,     2,     4,     4,     3,
       3,     3,     2,     1,     4,     0,     5,     0,     5,     5,
       1,     1,     1,     0,     6,     0,     8,     0,     8,     1,
       2,     2,     4,     1,     3,     1,     3,     1,     2,     3,
       1,     3,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     3,     2,     1,
       3,     5,     1,     3,     5,     1,     3,     2,     1,     1,
       3,     2,     3,     2,     1,     3,     1,     1,     3,     3,
       2,     2,     2,     1,     1,     6,     1,     1,     1,     1,
       2,     1,     2,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     2,     3,     1,     2,     1,     0,     4,     1,
       2,     2,     3,     2,     3,     1,     1,     2,     1,     2,
       1,     2,     1,     0,     4,     2,     3,     1,     4,     2,
       2,     1,     1,     1,     1,     1,     2,     2,     3,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     0,     4,     1,     1,     3,     5,     3,     1,
       2,     4,     2,     2,     2,     2,     1,     2,     1,     1,
       3,     1,     3,     1,     1,     2,     1,     4,     2,     2,
       1,     2,     1,     0,     6,     8,     4,     6,     4,     6,
       2,     4,     6,     2,     4,     2,     4,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     4,     1,
       3,     2,     2,     2,     1,     3,     1,     3,     1,     1,
       2,     1,     1,     1,     2,     2,     1,     2,     1,     1,
       1,     0,     4,     1,     2,     1,     3,     3,     3,     2,
       2,     3,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     0,     2,     2,     0,     1,     1,     1,
       1,     1,     1,     1,     2,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, p, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc, p)  YY_LOCATION_PRINT(File, *(Loc), p)

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc, p)  YYLOCATION_PRINT(File, &(Loc), p)

#  else

#   define YYLOCATION_PRINT(File, Loc, p) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location, p) \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, p);          \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, parser_state *p)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (p);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
switch (yykind)
    {
      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, parser_state *p)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp, p);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, p);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop, parser_state *p)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top, p)     \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top), p);    \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, parser_state *p)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), p);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule, p) \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, p); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
#ifndef yydebug
int yydebug;
#endif
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location, p)
# define YY_STACK_PRINT(Bottom, Top, p)
# define YY_REDUCE_PRINT(Rule, p)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx, parser_state *p)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, parser_state *p)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (p);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp, p);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (parser_state *p)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
#ifdef __cplusplus
static const YYSTYPE yyval_default = {};
(void) yyval_default;
#else
YY_INITIAL_VALUE (static const YYSTYPE yyval_default;)
#endif
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static const YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;
    YY_USE (yynerrs); /* Silence compiler warning.  */

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */



#line 7304 "mrbgems/mruby-compiler/core/y.tab.c"

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp, p);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, p);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc, p);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc, p);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;


  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn, p);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 2189 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->lstate = EXPR_BEG;
                      if (!p->locals) p->locals = cons(0,0);
                    }
#line 7522 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 3: /* program: $@1 top_compstmt  */
#line 2194 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->tree = new_scope(p, (yyvsp[0].nd));
                    }
#line 7530 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 4: /* top_compstmt: top_stmts opt_terms  */
#line 2200 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 7538 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 5: /* top_stmts: none  */
#line 2206 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_stmts(p, 0);
                    }
#line 7546 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 6: /* top_stmts: top_stmt  */
#line 2210 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_stmts(p, (yyvsp[0].nd));
                    }
#line 7554 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 7: /* top_stmts: top_stmts terms top_stmt  */
#line 2214 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = stmts_push(p, (yyvsp[-2].nd), newline_node((yyvsp[0].nd)));
                    }
#line 7562 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 8: /* top_stmts: error top_stmt  */
#line 2218 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_stmts(p, 0);
                    }
#line 7570 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 10: /* @2: %empty  */
#line 2225 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = local_switch(p);
                      nvars_block(p);
                    }
#line 7579 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 11: /* top_stmt: "'BEGIN'" @2 '{' top_compstmt '}'  */
#line 2230 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[-4]), p, "BEGIN not supported");
                      local_resume(p, (yyvsp[-3].nd));
                      nvars_unnest(p);
                      (yyval.nd) = 0;
                    }
#line 7590 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 12: /* bodystmt: compstmt opt_rescue opt_else opt_ensure  */
#line 2242 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if ((yyvsp[-2].nd)) {
                        (yyval.nd) = new_rescue(p, (yyvsp[-3].nd), (yyvsp[-2].nd), (yyvsp[-1].nd));
                      }
                      else if ((yyvsp[-1].nd)) {
                        yywarning(p, "else without rescue is useless");
                        (yyval.nd) = stmts_push(p, (yyvsp[-3].nd), (yyvsp[-1].nd));
                      }
                      else {
                        (yyval.nd) = (yyvsp[-3].nd);
                      }
                      if ((yyvsp[0].nd)) {
                        if ((yyval.nd)) {
                          (yyval.nd) = new_ensure(p, (yyval.nd), (yyvsp[0].nd));
                        }
                        else {
                          (yyval.nd) = push((yyvsp[0].nd), new_nil(p));
                        }
                      }
                    }
#line 7615 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 13: /* compstmt: stmts opt_terms  */
#line 2265 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 7623 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 14: /* stmts: none  */
#line 2271 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_stmts(p, 0);
                    }
#line 7631 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 15: /* stmts: stmt  */
#line 2275 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_stmts(p, (yyvsp[0].nd));
                    }
#line 7639 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 16: /* stmts: stmts terms stmt  */
#line 2279 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = stmts_push(p, (yyvsp[-2].nd), newline_node((yyvsp[0].nd)));
                    }
#line 7647 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 17: /* stmts: error stmt  */
#line 2283 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_stmts(p, (yyvsp[0].nd));
                    }
#line 7655 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 18: /* $@3: %empty  */
#line 2288 "mrbgems/mruby-compiler/core/parse.y"
                                     {p->lstate = EXPR_FNAME;}
#line 7661 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 19: /* stmt: "'alias'" fsym $@3 fsym  */
#line 2289 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_alias(p, (yyvsp[-2].id), (yyvsp[0].id));
                    }
#line 7669 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 20: /* stmt: "'undef'" undef_list  */
#line 2293 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_undef(p, (yyvsp[0].nd));
                    }
#line 7677 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 21: /* stmt: stmt "'if' modifier" expr_value  */
#line 2297 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_if(p, cond((yyvsp[0].nd)), (yyvsp[-2].nd), 0);
                    }
#line 7685 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 22: /* stmt: stmt "'unless' modifier" expr_value  */
#line 2301 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_if(p, cond((yyvsp[0].nd)), 0, (yyvsp[-2].nd));
                    }
#line 7693 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 23: /* stmt: stmt "'while' modifier" expr_value  */
#line 2305 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if ((yyvsp[-2].nd) && node_type_p((yyvsp[-2].nd), NODE_BEGIN)) {
                        (yyval.nd) = new_while_mod(p, cond((yyvsp[0].nd)), (yyvsp[-2].nd));
                      }
                      else {
                        (yyval.nd) = new_while(p, cond((yyvsp[0].nd)), (yyvsp[-2].nd));
                      }
                    }
#line 7706 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 24: /* stmt: stmt "'until' modifier" expr_value  */
#line 2314 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if ((yyvsp[-2].nd) && node_type_p((yyvsp[-2].nd), NODE_BEGIN)) {
                        (yyval.nd) = new_until_mod(p, cond((yyvsp[0].nd)), (yyvsp[-2].nd));
                      }
                      else {
                        (yyval.nd) = new_until(p, cond((yyvsp[0].nd)), (yyvsp[-2].nd));
                      }
                    }
#line 7719 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 25: /* stmt: stmt "'rescue' modifier" stmt  */
#line 2323 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_mod_rescue(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 7727 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 26: /* stmt: "'END'" '{' compstmt '}'  */
#line 2327 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[-3]), p, "END not supported");
                      (yyval.nd) = new_postexe(p, (yyvsp[-1].nd));
                    }
#line 7736 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 28: /* stmt: mlhs '=' command_call  */
#line 2333 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_masgn(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 7744 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 29: /* stmt: lhs '=' mrhs  */
#line 2337 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_asgn(p, (yyvsp[-2].nd), new_array(p, (yyvsp[0].nd)));
                    }
#line 7752 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 30: /* stmt: mlhs '=' arg  */
#line 2341 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_masgn(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 7760 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 31: /* stmt: mlhs '=' mrhs  */
#line 2345 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_masgn(p, (yyvsp[-2].nd), new_array(p, (yyvsp[0].nd)));
                    }
#line 7768 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 33: /* command_asgn: lhs '=' command_rhs  */
#line 2352 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_asgn(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 7776 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 34: /* command_asgn: var_lhs tOP_ASGN command_rhs  */
#line 2356 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_op_asgn(p, (yyvsp[-2].nd), (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 7784 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 35: /* command_asgn: primary_value '[' opt_call_args ']' tOP_ASGN command_rhs  */
#line 2360 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[-5].nd), intern_op(aref), (yyvsp[-3].nd), '.'), (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 7792 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 36: /* command_asgn: primary_value call_op "local variable or method" tOP_ASGN command_rhs  */
#line 2364 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[-4].nd), (yyvsp[-2].id), 0, (yyvsp[-3].num)), (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 7800 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 37: /* command_asgn: primary_value call_op "constant" tOP_ASGN command_rhs  */
#line 2368 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[-4].nd), (yyvsp[-2].id), 0, (yyvsp[-3].num)), (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 7808 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 38: /* command_asgn: primary_value "::" "constant" tOP_ASGN command_call  */
#line 2372 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[-4]), p, "constant re-assignment");
                      (yyval.nd) = 0;
                    }
#line 7817 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 39: /* command_asgn: primary_value "::" "local variable or method" tOP_ASGN command_rhs  */
#line 2377 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[-4].nd), (yyvsp[-2].id), 0, tCOLON2), (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 7825 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 40: /* command_asgn: defn_head f_opt_arglist_paren '=' command  */
#line 2381 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-3].nd);
                      endless_method_name(p, (yyvsp[-3].nd));
                      void_expr_error(p, (yyvsp[0].nd));
                      defn_setup(p, (yyval.nd), (yyvsp[-2].nd), (yyvsp[0].nd));
                      nvars_unnest(p);
                      p->in_def--;
                    }
#line 7838 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 41: /* command_asgn: defn_head f_opt_arglist_paren '=' command "'rescue' modifier" arg  */
#line 2390 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-5].nd);
                      endless_method_name(p, (yyvsp[-5].nd));
                      void_expr_error(p, (yyvsp[-2].nd));
                      defn_setup(p, (yyval.nd), (yyvsp[-4].nd), new_mod_rescue(p, (yyvsp[-2].nd), (yyvsp[0].nd)));
                      nvars_unnest(p);
                      p->in_def--;
                    }
#line 7851 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 42: /* command_asgn: defs_head f_opt_arglist_paren '=' command  */
#line 2399 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-3].nd);
                      void_expr_error(p, (yyvsp[0].nd));
                      defn_setup(p, (yyval.nd), (yyvsp[-2].nd), (yyvsp[0].nd));
                      nvars_unnest(p);
                      p->in_def--;
                      p->in_single--;
                    }
#line 7864 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 43: /* command_asgn: defs_head f_opt_arglist_paren '=' command "'rescue' modifier" arg  */
#line 2408 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-5].nd);
                      void_expr_error(p, (yyvsp[-2].nd));
                      defn_setup(p, (yyval.nd), (yyvsp[-4].nd), new_mod_rescue(p, (yyvsp[-2].nd), (yyvsp[0].nd)));
                      nvars_unnest(p);
                      p->in_def--;
                      p->in_single--;
                    }
#line 7877 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 44: /* command_asgn: backref tOP_ASGN command_rhs  */
#line 2417 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      backref_error(p, (yyvsp[-2].nd));
                      (yyval.nd) = new_stmts(p, 0);
                    }
#line 7886 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 46: /* command_rhs: command_call "'rescue' modifier" stmt  */
#line 2425 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_mod_rescue(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 7894 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 49: /* expr: expr "'and'" expr  */
#line 2433 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_and(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 7902 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 50: /* expr: expr "'or'" expr  */
#line 2437 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_or(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 7910 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 51: /* expr: "'not'" opt_nl expr  */
#line 2441 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_uni_op(p, cond((yyvsp[0].nd)), "!");
                    }
#line 7918 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 52: /* expr: "'defined?'" opt_nl expr  */
#line 2445 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_defined(p, (yyvsp[0].nd));
                    }
#line 7926 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 53: /* expr: '!' command_call  */
#line 2449 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_uni_op(p, cond((yyvsp[0].nd)), "!");
                    }
#line 7934 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 54: /* $@4: %empty  */
#line 2452 "mrbgems/mruby-compiler/core/parse.y"
                             {p->in_kwarg++;}
#line 7940 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 55: /* expr: arg "=>" $@4 p_expr  */
#line 2453 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* expr => pattern (raises NoMatchingPatternError on failure) */
                      p->in_kwarg--;
                      (yyval.nd) = new_match_pat(p, (yyvsp[-3].nd), (yyvsp[0].nd), TRUE);
                    }
#line 7950 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 56: /* $@5: %empty  */
#line 2458 "mrbgems/mruby-compiler/core/parse.y"
                                 {p->in_kwarg++;}
#line 7956 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 57: /* expr: arg "'in'" $@5 p_expr  */
#line 2459 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* expr in pattern (returns true/false) */
                      p->in_kwarg--;
                      (yyval.nd) = new_match_pat(p, (yyvsp[-3].nd), (yyvsp[0].nd), FALSE);
                    }
#line 7966 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 59: /* defn_head: "'def'" fname  */
#line 2468 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_def(p, (yyvsp[0].id));
                      p->cmdarg_stack = 0;
                      p->in_def++;
                      nvars_block(p);
                    }
#line 7977 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 60: /* $@6: %empty  */
#line 2477 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->lstate = EXPR_FNAME;
                    }
#line 7985 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 61: /* defs_head: "'def'" singleton dot_or_colon $@6 fname  */
#line 2481 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_sdef(p, (yyvsp[-3].nd), (yyvsp[0].id));
                      p->cmdarg_stack = 0;
                      p->in_def++;
                      p->in_single++;
                      nvars_block(p);
                      p->lstate = EXPR_ENDFN; /* force for args */
                    }
#line 7998 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 62: /* expr_value: expr  */
#line 2492 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if (!(yyvsp[0].nd)) (yyval.nd) = new_nil(p);
                      else {
                        (yyval.nd) = (yyvsp[0].nd);
                      }
                    }
#line 8009 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 66: /* block_command: block_call call_op2 operation2 command_args  */
#line 2506 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-3].nd), (yyvsp[-1].id), (yyvsp[0].nd), (yyvsp[-2].num));
                    }
#line 8017 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 67: /* $@7: %empty  */
#line 2512 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_nest(p);
                      nvars_nest(p);
                    }
#line 8026 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 68: /* cmd_brace_block: "{" $@7 opt_block_param compstmt '}'  */
#line 2519 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_block(p, (yyvsp[-2].nd), (yyvsp[-1].nd));
                      local_unnest(p);
                      nvars_unnest(p);
                    }
#line 8036 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 69: /* command: operation command_args  */
#line 2527 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_fcall(p, (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 8044 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 70: /* command: operation command_args cmd_brace_block  */
#line 2531 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      args_with_block(p, (yyvsp[-1].nd), (yyvsp[0].nd));
                      (yyval.nd) = new_fcall(p, (yyvsp[-2].id), (yyvsp[-1].nd));
                    }
#line 8053 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 71: /* command: primary_value call_op operation2 command_args  */
#line 2536 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-3].nd), (yyvsp[-1].id), (yyvsp[0].nd), (yyvsp[-2].num));
                    }
#line 8061 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 72: /* command: primary_value call_op operation2 command_args cmd_brace_block  */
#line 2540 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      args_with_block(p, (yyvsp[-1].nd), (yyvsp[0].nd));
                      (yyval.nd) = new_call(p, (yyvsp[-4].nd), (yyvsp[-2].id), (yyvsp[-1].nd), (yyvsp[-3].num));
                   }
#line 8070 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 73: /* command: primary_value "::" operation2 command_args  */
#line 2545 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-3].nd), (yyvsp[-1].id), (yyvsp[0].nd), tCOLON2);
                    }
#line 8078 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 74: /* command: primary_value "::" operation2 command_args cmd_brace_block  */
#line 2549 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      args_with_block(p, (yyvsp[-1].nd), (yyvsp[0].nd));
                      (yyval.nd) = new_call(p, (yyvsp[-4].nd), (yyvsp[-2].id), (yyvsp[-1].nd), tCOLON2);
                    }
#line 8087 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 75: /* command: "'super'" command_args  */
#line 2554 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_super(p, (yyvsp[0].nd));
                    }
#line 8095 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 76: /* command: "'yield'" command_args  */
#line 2558 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_yield(p, (yyvsp[0].nd));
                    }
#line 8103 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 77: /* command: "'return'" call_args  */
#line 2562 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_return(p, ret_args(p, (yyvsp[0].nd)));
                    }
#line 8111 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 78: /* command: "'break'" call_args  */
#line 2566 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_break(p, ret_args(p, (yyvsp[0].nd)));
                    }
#line 8119 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 79: /* command: "'next'" call_args  */
#line 2570 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_next(p, ret_args(p, (yyvsp[0].nd)));
                    }
#line 8127 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 80: /* mlhs: mlhs_basic  */
#line 2576 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 8135 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 81: /* mlhs: tLPAREN mlhs_inner rparen  */
#line 2580 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 8143 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 83: /* mlhs_inner: tLPAREN mlhs_inner rparen  */
#line 2587 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 8151 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 84: /* mlhs_basic: mlhs_list  */
#line 2593 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 8159 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 85: /* mlhs_basic: mlhs_list mlhs_item  */
#line 2597 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1(push((yyvsp[-1].nd),(yyvsp[0].nd)));
                    }
#line 8167 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 86: /* mlhs_basic: mlhs_list "*" mlhs_node  */
#line 2601 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list2((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 8175 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 87: /* mlhs_basic: mlhs_list "*" mlhs_node ',' mlhs_post  */
#line 2605 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3((yyvsp[-4].nd), (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 8183 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 88: /* mlhs_basic: mlhs_list "*"  */
#line 2609 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list2((yyvsp[-1].nd), new_nil(p));
                    }
#line 8191 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 89: /* mlhs_basic: mlhs_list "*" ',' mlhs_post  */
#line 2613 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3((yyvsp[-3].nd), new_nil(p), (yyvsp[0].nd));
                    }
#line 8199 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 90: /* mlhs_basic: "*" mlhs_node  */
#line 2617 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list2(0, (yyvsp[0].nd));
                    }
#line 8207 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 91: /* mlhs_basic: "*" mlhs_node ',' mlhs_post  */
#line 2621 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3(0, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 8215 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 92: /* mlhs_basic: "*"  */
#line 2625 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list2(0, new_nil(p));
                    }
#line 8223 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 93: /* mlhs_basic: "*" ',' mlhs_post  */
#line 2629 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3(0, new_nil(p), (yyvsp[0].nd));
                    }
#line 8231 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 95: /* mlhs_item: tLPAREN mlhs_inner rparen  */
#line 2636 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_masgn(p, (yyvsp[-1].nd), NULL);
                    }
#line 8239 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 96: /* mlhs_list: mlhs_item ','  */
#line 2642 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[-1].nd));
                    }
#line 8247 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 97: /* mlhs_list: mlhs_list mlhs_item ','  */
#line 2646 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[-1].nd));
                    }
#line 8255 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 98: /* mlhs_post: mlhs_item  */
#line 2652 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 8263 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 99: /* mlhs_post: mlhs_list mlhs_item  */
#line 2656 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 8271 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 100: /* mlhs_node: variable  */
#line 2662 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      assignable(p, (yyvsp[0].nd));
                    }
#line 8279 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 101: /* mlhs_node: primary_value '[' opt_call_args ']'  */
#line 2666 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-3].nd), intern_op(aref), (yyvsp[-1].nd), '.');
                    }
#line 8287 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 102: /* mlhs_node: primary_value call_op "local variable or method"  */
#line 2670 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-2].nd), (yyvsp[0].id), 0, (yyvsp[-1].num));
                    }
#line 8295 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 103: /* mlhs_node: primary_value "::" "local variable or method"  */
#line 2674 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-2].nd), (yyvsp[0].id), 0, tCOLON2);
                    }
#line 8303 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 104: /* mlhs_node: primary_value call_op "constant"  */
#line 2678 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-2].nd), (yyvsp[0].id), 0, (yyvsp[-1].num));
                    }
#line 8311 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 105: /* mlhs_node: primary_value "::" "constant"  */
#line 2682 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if (p->in_def || p->in_single)
                        yyerror(&(yylsp[-2]), p, "dynamic constant assignment");
                      (yyval.nd) = new_colon2(p, (yyvsp[-2].nd), (yyvsp[0].id));
                    }
#line 8321 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 106: /* mlhs_node: tCOLON3 "constant"  */
#line 2688 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if (p->in_def || p->in_single)
                        yyerror(&(yylsp[-1]), p, "dynamic constant assignment");
                      (yyval.nd) = new_colon3(p, (yyvsp[0].id));
                    }
#line 8331 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 107: /* mlhs_node: backref  */
#line 2694 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      backref_error(p, (yyvsp[0].nd));
                      (yyval.nd) = 0;
                    }
#line 8340 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 108: /* lhs: variable  */
#line 2701 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      assignable(p, (yyvsp[0].nd));
                    }
#line 8348 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 109: /* lhs: primary_value '[' opt_call_args ']'  */
#line 2705 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-3].nd), intern_op(aref), (yyvsp[-1].nd), '.');
                    }
#line 8356 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 110: /* lhs: primary_value call_op "local variable or method"  */
#line 2709 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-2].nd), (yyvsp[0].id), 0, (yyvsp[-1].num));
                    }
#line 8364 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 111: /* lhs: primary_value "::" "local variable or method"  */
#line 2713 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-2].nd), (yyvsp[0].id), 0, tCOLON2);
                    }
#line 8372 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 112: /* lhs: primary_value call_op "constant"  */
#line 2717 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-2].nd), (yyvsp[0].id), 0, (yyvsp[-1].num));
                    }
#line 8380 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 113: /* lhs: primary_value "::" "constant"  */
#line 2721 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if (p->in_def || p->in_single)
                        yyerror(&(yylsp[-2]), p, "dynamic constant assignment");
                      (yyval.nd) = new_colon2(p, (yyvsp[-2].nd), (yyvsp[0].id));
                    }
#line 8390 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 114: /* lhs: tCOLON3 "constant"  */
#line 2727 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if (p->in_def || p->in_single)
                        yyerror(&(yylsp[-1]), p, "dynamic constant assignment");
                      (yyval.nd) = new_colon3(p, (yyvsp[0].id));
                    }
#line 8400 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 115: /* lhs: backref  */
#line 2733 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      backref_error(p, (yyvsp[0].nd));
                      (yyval.nd) = 0;
                    }
#line 8409 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 116: /* lhs: "numbered parameter"  */
#line 2738 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[0]), p, "can't assign to numbered parameter");
                    }
#line 8417 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 117: /* cname: "local variable or method"  */
#line 2744 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[0]), p, "class/module name must be CONSTANT");
                    }
#line 8425 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 119: /* cpath: tCOLON3 cname  */
#line 2751 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = cons(int_to_node(1), sym_to_node((yyvsp[0].id)));
                    }
#line 8433 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 120: /* cpath: cname  */
#line 2755 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = cons(int_to_node(0), sym_to_node((yyvsp[0].id)));
                    }
#line 8441 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 121: /* cpath: primary_value "::" cname  */
#line 2759 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[-2].nd));
                      (yyval.nd) = cons((yyvsp[-2].nd), sym_to_node((yyvsp[0].id)));
                    }
#line 8450 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 125: /* fname: op  */
#line 2769 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->lstate = EXPR_ENDFN;
                      (yyval.id) = (yyvsp[0].id);
                    }
#line 8459 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 126: /* fname: reswords  */
#line 2774 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->lstate = EXPR_ENDFN;
                      (yyval.id) = (yyvsp[0].id);
                    }
#line 8468 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 129: /* undef_list: fsym  */
#line 2785 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = cons(sym_to_node((yyvsp[0].id)), 0);
                    }
#line 8476 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 130: /* $@8: %empty  */
#line 2788 "mrbgems/mruby-compiler/core/parse.y"
                                 {p->lstate = EXPR_FNAME;}
#line 8482 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 131: /* undef_list: undef_list ',' $@8 fsym  */
#line 2789 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-3].nd), sym_to_node((yyvsp[0].id)));
                    }
#line 8490 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 132: /* op: '|'  */
#line 2794 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(or);     }
#line 8496 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 133: /* op: '^'  */
#line 2795 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(xor);    }
#line 8502 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 134: /* op: '&'  */
#line 2796 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(and);    }
#line 8508 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 135: /* op: "<=>"  */
#line 2797 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(cmp);    }
#line 8514 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 136: /* op: "=="  */
#line 2798 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(eq);     }
#line 8520 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 137: /* op: "==="  */
#line 2799 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(eqq);    }
#line 8526 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 138: /* op: "=~"  */
#line 2800 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(match);  }
#line 8532 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 139: /* op: "!~"  */
#line 2801 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(nmatch); }
#line 8538 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 140: /* op: '>'  */
#line 2802 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(gt);     }
#line 8544 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 141: /* op: ">="  */
#line 2803 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(ge);     }
#line 8550 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 142: /* op: '<'  */
#line 2804 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(lt);     }
#line 8556 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 143: /* op: "<="  */
#line 2805 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(le);     }
#line 8562 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 144: /* op: "!="  */
#line 2806 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(neq);    }
#line 8568 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 145: /* op: "<<"  */
#line 2807 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(lshift); }
#line 8574 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 146: /* op: ">>"  */
#line 2808 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(rshift); }
#line 8580 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 147: /* op: '+'  */
#line 2809 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(add);    }
#line 8586 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 148: /* op: '-'  */
#line 2810 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(sub);    }
#line 8592 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 149: /* op: '*'  */
#line 2811 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(mul);    }
#line 8598 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 150: /* op: "*"  */
#line 2812 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(mul);    }
#line 8604 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 151: /* op: '/'  */
#line 2813 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(div);    }
#line 8610 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 152: /* op: '%'  */
#line 2814 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(mod);    }
#line 8616 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 153: /* op: tPOW  */
#line 2815 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(pow);    }
#line 8622 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 154: /* op: "**"  */
#line 2816 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(pow);    }
#line 8628 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 155: /* op: '!'  */
#line 2817 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(not);    }
#line 8634 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 156: /* op: '~'  */
#line 2818 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(neg);    }
#line 8640 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 157: /* op: "unary plus"  */
#line 2819 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(plus);   }
#line 8646 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 158: /* op: "unary minus"  */
#line 2820 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(minus);  }
#line 8652 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 159: /* op: tAREF  */
#line 2821 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(aref);   }
#line 8658 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 160: /* op: tASET  */
#line 2822 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(aset);   }
#line 8664 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 161: /* op: '`'  */
#line 2823 "mrbgems/mruby-compiler/core/parse.y"
                                { (yyval.id) = intern_op(tick);   }
#line 8670 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 203: /* arg: lhs '=' arg_rhs  */
#line 2841 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_asgn(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 8678 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 204: /* arg: var_lhs tOP_ASGN arg_rhs  */
#line 2845 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_op_asgn(p, (yyvsp[-2].nd), (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 8686 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 205: /* arg: primary_value '[' opt_call_args ']' tOP_ASGN arg_rhs  */
#line 2849 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[-5].nd), intern_op(aref), (yyvsp[-3].nd), '.'), (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 8694 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 206: /* arg: primary_value call_op "local variable or method" tOP_ASGN arg_rhs  */
#line 2853 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[-4].nd), (yyvsp[-2].id), 0, (yyvsp[-3].num)), (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 8702 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 207: /* arg: primary_value call_op "constant" tOP_ASGN arg_rhs  */
#line 2857 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[-4].nd), (yyvsp[-2].id), 0, (yyvsp[-3].num)), (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 8710 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 208: /* arg: primary_value "::" "local variable or method" tOP_ASGN arg_rhs  */
#line 2861 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[-4].nd), (yyvsp[-2].id), 0, tCOLON2), (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 8718 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 209: /* arg: primary_value "::" "constant" tOP_ASGN arg_rhs  */
#line 2865 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[-4]), p, "constant re-assignment");
                      (yyval.nd) = new_stmts(p, 0);
                    }
#line 8727 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 210: /* arg: tCOLON3 "constant" tOP_ASGN arg_rhs  */
#line 2870 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[-3]), p, "constant re-assignment");
                      (yyval.nd) = new_stmts(p, 0);
                    }
#line 8736 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 211: /* arg: backref tOP_ASGN arg_rhs  */
#line 2875 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      backref_error(p, (yyvsp[-2].nd));
                      (yyval.nd) = new_stmts(p, 0);
                    }
#line 8745 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 212: /* arg: arg ".." arg  */
#line 2880 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_dot2(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 8753 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 213: /* arg: arg ".."  */
#line 2884 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_dot2(p, (yyvsp[-1].nd), new_nil(p));
                    }
#line 8761 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 214: /* arg: tBDOT2 arg  */
#line 2888 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_dot2(p, new_nil(p), (yyvsp[0].nd));
                    }
#line 8769 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 215: /* arg: arg "..." arg  */
#line 2892 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_dot3(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 8777 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 216: /* arg: arg "..."  */
#line 2896 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_dot3(p, (yyvsp[-1].nd), new_nil(p));
                    }
#line 8785 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 217: /* arg: tBDOT3 arg  */
#line 2900 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_dot3(p, new_nil(p), (yyvsp[0].nd));
                    }
#line 8793 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 218: /* arg: arg '+' arg  */
#line 2904 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "+", (yyvsp[0].nd));
                    }
#line 8801 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 219: /* arg: arg '-' arg  */
#line 2908 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "-", (yyvsp[0].nd));
                    }
#line 8809 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 220: /* arg: arg '*' arg  */
#line 2912 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "*", (yyvsp[0].nd));
                    }
#line 8817 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 221: /* arg: arg '/' arg  */
#line 2916 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "/", (yyvsp[0].nd));
                    }
#line 8825 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 222: /* arg: arg '%' arg  */
#line 2920 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "%", (yyvsp[0].nd));
                    }
#line 8833 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 223: /* arg: arg tPOW arg  */
#line 2924 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "**", (yyvsp[0].nd));
                    }
#line 8841 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 224: /* arg: tUMINUS_NUM "integer literal" tPOW arg  */
#line 2928 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_negate(p, call_bin_op(p, (yyvsp[-2].nd), "**", (yyvsp[0].nd)));
                    }
#line 8849 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 225: /* arg: tUMINUS_NUM "float literal" tPOW arg  */
#line 2932 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_negate(p, call_bin_op(p, (yyvsp[-2].nd), "**", (yyvsp[0].nd)));
                    }
#line 8857 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 226: /* arg: "unary plus" arg  */
#line 2936 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_uni_op(p, (yyvsp[0].nd), "+@");
                    }
#line 8865 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 227: /* arg: "unary minus" arg  */
#line 2940 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_negate(p, (yyvsp[0].nd));
                    }
#line 8873 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 228: /* arg: arg '|' arg  */
#line 2944 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "|", (yyvsp[0].nd));
                    }
#line 8881 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 229: /* arg: arg '^' arg  */
#line 2948 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "^", (yyvsp[0].nd));
                    }
#line 8889 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 230: /* arg: arg '&' arg  */
#line 2952 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "&", (yyvsp[0].nd));
                    }
#line 8897 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 231: /* arg: arg "<=>" arg  */
#line 2956 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "<=>", (yyvsp[0].nd));
                    }
#line 8905 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 232: /* arg: arg '>' arg  */
#line 2960 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), ">", (yyvsp[0].nd));
                    }
#line 8913 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 233: /* arg: arg ">=" arg  */
#line 2964 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), ">=", (yyvsp[0].nd));
                    }
#line 8921 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 234: /* arg: arg '<' arg  */
#line 2968 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "<", (yyvsp[0].nd));
                    }
#line 8929 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 235: /* arg: arg "<=" arg  */
#line 2972 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "<=", (yyvsp[0].nd));
                    }
#line 8937 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 236: /* arg: arg "==" arg  */
#line 2976 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "==", (yyvsp[0].nd));
                    }
#line 8945 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 237: /* arg: arg "===" arg  */
#line 2980 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "===", (yyvsp[0].nd));
                    }
#line 8953 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 238: /* arg: arg "!=" arg  */
#line 2984 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "!=", (yyvsp[0].nd));
                    }
#line 8961 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 239: /* arg: arg "=~" arg  */
#line 2988 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "=~", (yyvsp[0].nd));
                    }
#line 8969 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 240: /* arg: arg "!~" arg  */
#line 2992 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "!~", (yyvsp[0].nd));
                    }
#line 8977 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 241: /* arg: '!' arg  */
#line 2996 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_uni_op(p, cond((yyvsp[0].nd)), "!");
                    }
#line 8985 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 242: /* arg: '~' arg  */
#line 3000 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_uni_op(p, cond((yyvsp[0].nd)), "~");
                    }
#line 8993 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 243: /* arg: arg "<<" arg  */
#line 3004 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), "<<", (yyvsp[0].nd));
                    }
#line 9001 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 244: /* arg: arg ">>" arg  */
#line 3008 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[-2].nd), ">>", (yyvsp[0].nd));
                    }
#line 9009 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 245: /* arg: arg "&&" arg  */
#line 3012 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_and(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 9017 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 246: /* arg: arg "||" arg  */
#line 3016 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_or(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 9025 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 247: /* arg: arg '?' arg opt_nl ':' arg  */
#line 3020 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_if(p, cond((yyvsp[-5].nd)), (yyvsp[-3].nd), (yyvsp[0].nd));
                    }
#line 9033 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 248: /* arg: arg '?' arg opt_nl "label" arg  */
#line 3024 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_if(p, cond((yyvsp[-5].nd)), (yyvsp[-3].nd), (yyvsp[0].nd));
                    }
#line 9041 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 249: /* arg: defn_head f_opt_arglist_paren '=' arg  */
#line 3028 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-3].nd);
                      endless_method_name(p, (yyvsp[-3].nd));
                      void_expr_error(p, (yyvsp[0].nd));
                      defn_setup(p, (yyval.nd), (yyvsp[-2].nd), (yyvsp[0].nd));
                      nvars_unnest(p);
                      p->in_def--;
                    }
#line 9054 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 250: /* arg: defn_head f_opt_arglist_paren '=' arg "'rescue' modifier" arg  */
#line 3037 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-5].nd);
                      endless_method_name(p, (yyvsp[-5].nd));
                      void_expr_error(p, (yyvsp[-2].nd));
                      defn_setup(p, (yyval.nd), (yyvsp[-4].nd), new_mod_rescue(p, (yyvsp[-2].nd), (yyvsp[0].nd)));
                      nvars_unnest(p);
                      p->in_def--;
                    }
#line 9067 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 251: /* arg: defs_head f_opt_arglist_paren '=' arg  */
#line 3046 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-3].nd);
                      void_expr_error(p, (yyvsp[0].nd));
                      defn_setup(p, (yyval.nd), (yyvsp[-2].nd), (yyvsp[0].nd));
                      nvars_unnest(p);
                      p->in_def--;
                      p->in_single--;
                    }
#line 9080 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 252: /* arg: defs_head f_opt_arglist_paren '=' arg "'rescue' modifier" arg  */
#line 3055 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-5].nd);
                      void_expr_error(p, (yyvsp[-2].nd));
                      defn_setup(p, (yyval.nd), (yyvsp[-4].nd), new_mod_rescue(p, (yyvsp[-2].nd), (yyvsp[0].nd)));
                      nvars_unnest(p);
                      p->in_def--;
                      p->in_single--;
                    }
#line 9093 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 253: /* arg: primary  */
#line 3064 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 9101 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 255: /* aref_args: args trailer  */
#line 3071 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 9109 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 256: /* aref_args: args comma assocs trailer  */
#line 3075 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-3].nd), new_hash(p, (yyvsp[-1].nd)));
                    }
#line 9117 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 257: /* aref_args: assocs trailer  */
#line 3079 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = cons(new_hash(p, (yyvsp[-1].nd)), 0);
                    }
#line 9125 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 258: /* arg_rhs: arg  */
#line 3085 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 9133 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 259: /* arg_rhs: arg "'rescue' modifier" arg  */
#line 3089 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[-2].nd));
                      (yyval.nd) = new_mod_rescue(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 9142 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 260: /* paren_args: '(' opt_call_args ')'  */
#line 3096 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 9150 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 261: /* paren_args: '(' args comma tBDOT3 rparen  */
#line 3100 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      mrb_sym r = intern_op(mul);
                      mrb_sym k = intern_op(pow);
                      mrb_sym b = intern_op(and);
                      (yyval.nd) = new_callargs(p, push((yyvsp[-3].nd), new_splat(p, new_lvar(p, r))),
                                        list1(cons(new_kw_rest_args(p, 0), new_lvar(p, k))),
                                        new_block_arg(p, new_lvar(p, b)));
                    }
#line 9163 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 262: /* paren_args: '(' tBDOT3 rparen  */
#line 3109 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      mrb_sym r = intern_op(mul);
                      mrb_sym k = intern_op(pow);
                      mrb_sym b = intern_op(and);
                      if (local_var_p(p, r) && local_var_p(p, k) && local_var_p(p, b)) {
                        (yyval.nd) = new_callargs(p, list1(new_splat(p, new_lvar(p, r))),
                                          list1(cons(new_kw_rest_args(p, 0), new_lvar(p, k))),
                                          new_block_arg(p, new_lvar(p, b)));
                      }
                      else {
                        yyerror(&(yylsp[-2]), p, "unexpected argument forwarding ...");
                        (yyval.nd) = 0;
                      }
                    }
#line 9182 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 267: /* opt_call_args: args comma  */
#line 3132 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_callargs(p,(yyvsp[-1].nd),0,0);
                    }
#line 9190 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 268: /* opt_call_args: args comma assocs comma  */
#line 3136 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_callargs(p,(yyvsp[-3].nd),(yyvsp[-1].nd),0);
                    }
#line 9198 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 269: /* opt_call_args: assocs comma  */
#line 3140 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_callargs(p,0,(yyvsp[-1].nd),0);
                    }
#line 9206 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 270: /* call_args: command  */
#line 3146 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = new_callargs(p, list1((yyvsp[0].nd)), 0, 0);
                    }
#line 9215 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 271: /* call_args: args opt_block_arg  */
#line 3151 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_callargs(p, (yyvsp[-1].nd), 0, (yyvsp[0].nd));
                    }
#line 9223 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 272: /* call_args: assocs opt_block_arg  */
#line 3155 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_callargs(p, 0, (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 9231 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 273: /* call_args: args comma assocs opt_block_arg  */
#line 3159 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_callargs(p, (yyvsp[-3].nd), (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 9239 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 274: /* call_args: block_arg  */
#line 3163 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_callargs(p, 0, 0, (yyvsp[0].nd));
                    }
#line 9247 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 275: /* @9: %empty  */
#line 3168 "mrbgems/mruby-compiler/core/parse.y"
                   {
                      (yyval.stack) = p->cmdarg_stack;
                      CMDARG_PUSH(1);
                    }
#line 9256 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 276: /* command_args: @9 call_args  */
#line 3173 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->cmdarg_stack = (yyvsp[-1].stack);
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 9265 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 277: /* block_arg: "&" arg  */
#line 3180 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_block_arg(p, (yyvsp[0].nd));
                    }
#line 9273 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 278: /* block_arg: "&"  */
#line 3184 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_block_arg(p, 0);
                    }
#line 9281 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 279: /* opt_block_arg: comma block_arg  */
#line 3190 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 9289 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 280: /* opt_block_arg: none  */
#line 3194 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = 0;
                    }
#line 9297 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 282: /* args: arg  */
#line 3203 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 9306 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 283: /* args: "*"  */
#line 3208 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1(new_splat(p, new_lvar(p, intern_op(mul))));
                    }
#line 9314 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 284: /* args: "*" arg  */
#line 3212 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1(new_splat(p, (yyvsp[0].nd)));
                    }
#line 9322 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 285: /* args: args comma arg  */
#line 3216 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 9331 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 286: /* args: args comma "*"  */
#line 3221 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), new_splat(p, new_lvar(p, intern_op(mul))));
                    }
#line 9339 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 287: /* args: args comma "*" arg  */
#line 3225 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-3].nd), new_splat(p, (yyvsp[0].nd)));
                    }
#line 9347 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 288: /* mrhs: args comma arg  */
#line 3231 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 9356 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 289: /* mrhs: args comma "*" arg  */
#line 3236 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-3].nd), new_splat(p, (yyvsp[0].nd)));
                    }
#line 9364 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 290: /* mrhs: "*" arg  */
#line 3240 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1(new_splat(p, (yyvsp[0].nd)));
                    }
#line 9372 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 292: /* primary: string  */
#line 3247 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_str(p, (yyvsp[0].nd));
                    }
#line 9380 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 293: /* primary: xstring  */
#line 3251 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_xstr(p, (yyvsp[0].nd));
                    }
#line 9388 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 298: /* primary: "method"  */
#line 3259 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_fcall(p, (yyvsp[0].id), 0);
                    }
#line 9396 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 299: /* @10: %empty  */
#line 3263 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.stack) = p->cmdarg_stack;
                      p->cmdarg_stack = 0;
                    }
#line 9405 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 300: /* primary: "'begin'" @10 bodystmt "'end'"  */
#line 3269 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->cmdarg_stack = (yyvsp[-2].stack);
                      (yyval.nd) = new_begin(p, (yyvsp[-1].nd));
                    }
#line 9414 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 301: /* @11: %empty  */
#line 3274 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.stack) = p->cmdarg_stack;
                      p->cmdarg_stack = 0;
                    }
#line 9423 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 302: /* $@12: %empty  */
#line 3278 "mrbgems/mruby-compiler/core/parse.y"
                           {p->lstate = EXPR_ENDARG;}
#line 9429 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 303: /* primary: "(" @11 compstmt $@12 rparen  */
#line 3279 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->cmdarg_stack = (yyvsp[-3].stack);
                      (yyval.nd) = (yyvsp[-2].nd);
                    }
#line 9438 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 304: /* $@13: %empty  */
#line 3283 "mrbgems/mruby-compiler/core/parse.y"
                              {p->lstate = EXPR_ENDARG;}
#line 9444 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 305: /* primary: "(" $@13 rparen  */
#line 3284 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_nil(p);
                    }
#line 9452 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 306: /* primary: tLPAREN compstmt ')'  */
#line 3288 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 9460 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 307: /* primary: primary_value "::" "constant"  */
#line 3292 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_colon2(p, (yyvsp[-2].nd), (yyvsp[0].id));
                    }
#line 9468 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 308: /* primary: tCOLON3 "constant"  */
#line 3296 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_colon3(p, (yyvsp[0].id));
                    }
#line 9476 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 309: /* primary: "[" aref_args ']'  */
#line 3300 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_array(p, (yyvsp[-1].nd));
                    }
#line 9484 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 310: /* primary: tLBRACE assoc_list '}'  */
#line 3304 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_hash(p, (yyvsp[-1].nd));
                    }
#line 9492 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 311: /* primary: "'return'"  */
#line 3308 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_return(p, 0);
                    }
#line 9500 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 312: /* primary: "'yield'" opt_paren_args  */
#line 3312 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_yield(p, (yyvsp[0].nd));
                    }
#line 9508 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 313: /* primary: "'not'" '(' expr rparen  */
#line 3316 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_uni_op(p, cond((yyvsp[-1].nd)), "!");
                    }
#line 9516 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 314: /* primary: "'not'" '(' rparen  */
#line 3320 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = call_uni_op(p, new_nil(p), "!");
                    }
#line 9524 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 315: /* primary: "'defined?'" '(' expr rparen  */
#line 3324 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_defined(p, (yyvsp[-1].nd));
                    }
#line 9532 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 316: /* primary: operation brace_block  */
#line 3328 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_fcall(p, (yyvsp[-1].id), new_callargs(p, 0, 0, (yyvsp[0].nd)));
                    }
#line 9540 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 318: /* primary: method_call brace_block  */
#line 3333 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      call_with_block(p, (yyvsp[-1].nd), (yyvsp[0].nd));
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 9549 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 319: /* @14: %empty  */
#line 3338 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_nest(p);
                      nvars_nest(p);
                      (yyval.num) = p->lpar_beg;
                      p->lpar_beg = ++p->paren_nest;
                    }
#line 9560 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 320: /* @15: %empty  */
#line 3345 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.stack) = p->cmdarg_stack;
                      p->cmdarg_stack = 0;
                    }
#line 9569 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 321: /* primary: "->" @14 f_larglist @15 lambda_body  */
#line 3350 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->lpar_beg = (yyvsp[-3].num);
                      (yyval.nd) = new_lambda(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                      local_unnest(p);
                      nvars_unnest(p);
                      p->cmdarg_stack = (yyvsp[-1].stack);
                      CMDARG_LEXPOP();
                    }
#line 9582 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 322: /* primary: "'if'" expr_value then compstmt if_tail "'end'"  */
#line 3362 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_if(p, cond((yyvsp[-4].nd)), (yyvsp[-2].nd), (yyvsp[-1].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-5].num));
                    }
#line 9591 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 323: /* primary: "'unless'" expr_value then compstmt opt_else "'end'"  */
#line 3370 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_if(p, cond((yyvsp[-4].nd)), (yyvsp[-1].nd), (yyvsp[-2].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-5].num));
                    }
#line 9600 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 324: /* $@16: %empty  */
#line 3374 "mrbgems/mruby-compiler/core/parse.y"
                                {COND_PUSH(1);}
#line 9606 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 325: /* $@17: %empty  */
#line 3374 "mrbgems/mruby-compiler/core/parse.y"
                                                              {COND_POP();}
#line 9612 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 326: /* primary: "'while'" $@16 expr_value do $@17 compstmt "'end'"  */
#line 3377 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_while(p, cond((yyvsp[-4].nd)), (yyvsp[-1].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-6].num));
                    }
#line 9621 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 327: /* $@18: %empty  */
#line 3381 "mrbgems/mruby-compiler/core/parse.y"
                                {COND_PUSH(1);}
#line 9627 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 328: /* $@19: %empty  */
#line 3381 "mrbgems/mruby-compiler/core/parse.y"
                                                              {COND_POP();}
#line 9633 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 329: /* primary: "'until'" $@18 expr_value do $@19 compstmt "'end'"  */
#line 3384 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_until(p, cond((yyvsp[-4].nd)), (yyvsp[-1].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-6].num));
                    }
#line 9642 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 330: /* primary: "'case'" expr_value opt_terms case_body "'end'"  */
#line 3391 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_case(p, (yyvsp[-3].nd), (yyvsp[-1].nd));
                    }
#line 9650 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 331: /* primary: "'case'" opt_terms case_body "'end'"  */
#line 3395 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_case(p, 0, (yyvsp[-1].nd));
                    }
#line 9658 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 332: /* primary: "'case'" expr_value opt_terms "'in'" p_expr then compstmt in_clauses "'end'"  */
#line 3403 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *in_clause = new_in(p, (yyvsp[-4].nd), NULL, (yyvsp[-2].nd), FALSE);
                      (yyval.nd) = new_case_match(p, (yyvsp[-7].nd), cons(in_clause, (yyvsp[-1].nd)));
                    }
#line 9667 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 333: /* primary: "'case'" expr_value opt_terms "'in'" p_expr "'if' modifier" expr_value then compstmt in_clauses "'end'"  */
#line 3412 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *in_clause = new_in(p, (yyvsp[-6].nd), (yyvsp[-4].nd), (yyvsp[-2].nd), FALSE);
                      (yyval.nd) = new_case_match(p, (yyvsp[-9].nd), cons(in_clause, (yyvsp[-1].nd)));
                    }
#line 9676 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 334: /* primary: "'case'" expr_value opt_terms "'in'" p_expr "'unless' modifier" expr_value then compstmt in_clauses "'end'"  */
#line 3421 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *in_clause = new_in(p, (yyvsp[-6].nd), (yyvsp[-4].nd), (yyvsp[-2].nd), TRUE);
                      (yyval.nd) = new_case_match(p, (yyvsp[-9].nd), cons(in_clause, (yyvsp[-1].nd)));
                    }
#line 9685 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 335: /* $@20: %empty  */
#line 3426 "mrbgems/mruby-compiler/core/parse.y"
                  {COND_PUSH(1);}
#line 9691 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 336: /* $@21: %empty  */
#line 3428 "mrbgems/mruby-compiler/core/parse.y"
                  {COND_POP();}
#line 9697 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 337: /* primary: "'for'" for_var "'in'" $@20 expr_value do $@21 compstmt "'end'"  */
#line 3431 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_for(p, (yyvsp[-7].nd), (yyvsp[-4].nd), (yyvsp[-1].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-8].num));
                    }
#line 9706 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 338: /* @22: %empty  */
#line 3437 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if (p->in_def || p->in_single)
                        yyerror(&(yylsp[-2]), p, "class definition in method body");
                      (yyval.nd) = local_switch(p);
                      nvars_block(p);
                    }
#line 9717 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 339: /* primary: "'class'" cpath superclass @22 bodystmt "'end'"  */
#line 3445 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_class(p, (yyvsp[-4].nd), (yyvsp[-3].nd), (yyvsp[-1].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-5].num));
                      local_resume(p, (yyvsp[-2].nd));
                      nvars_unnest(p);
                    }
#line 9728 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 340: /* @23: %empty  */
#line 3453 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.num) = p->in_def;
                      p->in_def = 0;
                    }
#line 9737 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 341: /* @24: %empty  */
#line 3458 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = cons(local_switch(p), int_to_node(p->in_single));
                      nvars_block(p);
                      p->in_single = 0;
                    }
#line 9747 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 342: /* primary: "'class'" "<<" expr @23 term @24 bodystmt "'end'"  */
#line 3465 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_sclass(p, (yyvsp[-5].nd), (yyvsp[-1].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-7].num));
                      local_resume(p, (yyvsp[-2].nd)->car);
                      nvars_unnest(p);
                      p->in_def = (yyvsp[-4].num);
                      p->in_single = node_to_int((yyvsp[-2].nd)->cdr);
                    }
#line 9760 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 343: /* @25: %empty  */
#line 3475 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if (p->in_def || p->in_single)
                        yyerror(&(yylsp[-1]), p, "module definition in method body");
                      (yyval.nd) = local_switch(p);
                      nvars_block(p);
                    }
#line 9771 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 344: /* primary: "'module'" cpath @25 bodystmt "'end'"  */
#line 3483 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_module(p, (yyvsp[-3].nd), (yyvsp[-1].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-4].num));
                      local_resume(p, (yyvsp[-2].nd));
                      nvars_unnest(p);
                    }
#line 9782 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 345: /* primary: defn_head f_arglist bodystmt "'end'"  */
#line 3493 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-3].nd);
                      defn_setup(p, (yyval.nd), (yyvsp[-2].nd), (yyvsp[-1].nd));
                      nvars_unnest(p);
                      p->in_def--;
                    }
#line 9793 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 346: /* primary: defs_head f_arglist bodystmt "'end'"  */
#line 3503 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-3].nd);
                      defn_setup(p, (yyval.nd), (yyvsp[-2].nd), (yyvsp[-1].nd));
                      nvars_unnest(p);
                      p->in_def--;
                      p->in_single--;
                    }
#line 9805 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 347: /* primary: "'break'"  */
#line 3511 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_break(p, 0);
                    }
#line 9813 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 348: /* primary: "'next'"  */
#line 3515 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_next(p, 0);
                    }
#line 9821 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 349: /* primary: "'redo'"  */
#line 3519 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_redo(p);
                    }
#line 9829 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 350: /* primary: "'retry'"  */
#line 3523 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_retry(p);
                    }
#line 9837 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 351: /* primary_value: primary  */
#line 3529 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                      if (!(yyval.nd)) (yyval.nd) = new_nil(p);
                    }
#line 9846 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 358: /* if_tail: "'elsif'" expr_value then compstmt if_tail  */
#line 3548 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_if(p, cond((yyvsp[-3].nd)), (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 9854 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 360: /* opt_else: "'else'" compstmt  */
#line 3555 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 9862 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 361: /* for_var: lhs  */
#line 3561 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1(list1((yyvsp[0].nd)));
                    }
#line 9870 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 363: /* f_margs: f_arg  */
#line 3568 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3((yyvsp[0].nd),0,0);
                    }
#line 9878 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 364: /* f_margs: f_arg ',' "*" f_norm_arg  */
#line 3572 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3((yyvsp[-3].nd), new_lvar(p, (yyvsp[0].id)), 0);
                    }
#line 9886 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 365: /* f_margs: f_arg ',' "*" f_norm_arg ',' f_arg  */
#line 3576 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3((yyvsp[-5].nd), new_lvar(p, (yyvsp[-2].id)), (yyvsp[0].nd));
                    }
#line 9894 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 366: /* f_margs: f_arg ',' "*"  */
#line 3580 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_add_f(p, intern_op(mul));
                      (yyval.nd) = list3((yyvsp[-2].nd), int_to_node(-1), 0);
                    }
#line 9903 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 367: /* f_margs: f_arg ',' "*" ',' f_arg  */
#line 3585 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3((yyvsp[-4].nd), int_to_node(-1), (yyvsp[0].nd));
                    }
#line 9911 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 368: /* f_margs: "*" f_norm_arg  */
#line 3589 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3(0, new_lvar(p, (yyvsp[0].id)), 0);
                    }
#line 9919 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 369: /* f_margs: "*" f_norm_arg ',' f_arg  */
#line 3593 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3(0, new_lvar(p, (yyvsp[-2].id)), (yyvsp[0].nd));
                    }
#line 9927 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 370: /* f_margs: "*"  */
#line 3597 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_add_f(p, intern_op(mul));
                      (yyval.nd) = list3(0, int_to_node(-1), 0);
                    }
#line 9936 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 371: /* $@26: %empty  */
#line 3602 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_add_f(p, intern_op(mul));
                    }
#line 9944 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 372: /* f_margs: "*" ',' $@26 f_arg  */
#line 3606 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list3(0, int_to_node(-1), (yyvsp[0].nd));
                    }
#line 9952 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 373: /* block_args_tail: f_block_kwarg ',' f_kwrest opt_f_block_arg  */
#line 3612 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, (yyvsp[-3].nd), (yyvsp[-1].id), (yyvsp[0].id));
                    }
#line 9960 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 374: /* block_args_tail: f_block_kwarg opt_f_block_arg  */
#line 3616 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, (yyvsp[-1].nd), 0, (yyvsp[0].id));
                    }
#line 9968 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 375: /* block_args_tail: f_kwrest opt_f_block_arg  */
#line 3620 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, 0, (yyvsp[-1].id), (yyvsp[0].id));
                    }
#line 9976 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 376: /* block_args_tail: f_block_arg  */
#line 3624 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, 0, 0, (yyvsp[0].id));
                    }
#line 9984 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 377: /* opt_block_args_tail: ',' block_args_tail  */
#line 3630 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 9992 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 378: /* opt_block_args_tail: %empty  */
#line 3634 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, 0, 0, 0);
                    }
#line 10000 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 379: /* block_param: f_arg ',' f_block_optarg ',' f_rest_arg opt_block_args_tail  */
#line 3640 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-5].nd), (yyvsp[-3].nd), (yyvsp[-1].id), 0, (yyvsp[0].nd));
                    }
#line 10008 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 380: /* block_param: f_arg ',' f_block_optarg ',' f_rest_arg ',' f_arg opt_block_args_tail  */
#line 3644 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-7].nd), (yyvsp[-5].nd), (yyvsp[-3].id), (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 10016 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 381: /* block_param: f_arg ',' f_block_optarg opt_block_args_tail  */
#line 3648 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-3].nd), (yyvsp[-1].nd), 0, 0, (yyvsp[0].nd));
                    }
#line 10024 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 382: /* block_param: f_arg ',' f_block_optarg ',' f_arg opt_block_args_tail  */
#line 3652 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-5].nd), (yyvsp[-3].nd), 0, (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 10032 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 383: /* block_param: f_arg ',' f_rest_arg opt_block_args_tail  */
#line 3656 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-3].nd), 0, (yyvsp[-1].id), 0, (yyvsp[0].nd));
                    }
#line 10040 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 384: /* block_param: f_arg ',' opt_block_args_tail  */
#line 3660 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-2].nd), 0, 0, 0, (yyvsp[0].nd));
                    }
#line 10048 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 385: /* block_param: f_arg ',' f_rest_arg ',' f_arg opt_block_args_tail  */
#line 3664 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-5].nd), 0, (yyvsp[-3].id), (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 10056 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 386: /* block_param: f_arg opt_block_args_tail  */
#line 3668 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-1].nd), 0, 0, 0, (yyvsp[0].nd));
                    }
#line 10064 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 387: /* block_param: f_block_optarg ',' f_rest_arg opt_block_args_tail  */
#line 3672 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[-3].nd), (yyvsp[-1].id), 0, (yyvsp[0].nd));
                    }
#line 10072 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 388: /* block_param: f_block_optarg ',' f_rest_arg ',' f_arg opt_block_args_tail  */
#line 3676 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[-5].nd), (yyvsp[-3].id), (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 10080 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 389: /* block_param: f_block_optarg opt_block_args_tail  */
#line 3680 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[-1].nd), 0, 0, (yyvsp[0].nd));
                    }
#line 10088 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 390: /* block_param: f_block_optarg ',' f_arg opt_block_args_tail  */
#line 3684 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[-3].nd), 0, (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 10096 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 391: /* block_param: f_rest_arg opt_block_args_tail  */
#line 3688 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, 0, (yyvsp[-1].id), 0, (yyvsp[0].nd));
                    }
#line 10104 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 392: /* block_param: f_rest_arg ',' f_arg opt_block_args_tail  */
#line 3692 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, 0, (yyvsp[-3].id), (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 10112 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 393: /* block_param: block_args_tail  */
#line 3696 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, 0, 0, 0, (yyvsp[0].nd));
                    }
#line 10120 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 394: /* opt_block_param: none  */
#line 3702 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_add_blk(p);
                      (yyval.nd) = 0;
                    }
#line 10129 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 395: /* opt_block_param: block_param_def  */
#line 3707 "mrbgems/mruby-compiler/core/parse.y"
                   {
                      p->cmd_start = TRUE;
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 10138 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 396: /* $@27: %empty  */
#line 3713 "mrbgems/mruby-compiler/core/parse.y"
                      {local_add_blk(p);}
#line 10144 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 397: /* block_param_def: '|' $@27 opt_bv_decl '|'  */
#line 3714 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = 0;
                    }
#line 10152 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 398: /* block_param_def: "||"  */
#line 3718 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_add_blk(p);
                      (yyval.nd) = 0;
                    }
#line 10161 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 399: /* block_param_def: '|' block_param opt_bv_decl '|'  */
#line 3723 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-2].nd);
                    }
#line 10169 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 400: /* opt_bv_decl: opt_nl  */
#line 3729 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = 0;
                    }
#line 10177 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 401: /* opt_bv_decl: opt_nl ';' bv_decls opt_nl  */
#line 3733 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = 0;
                    }
#line 10185 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 404: /* bvar: "local variable or method"  */
#line 3743 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_add_f(p, (yyvsp[0].id));
                      new_bv(p, (yyvsp[0].id));
                    }
#line 10194 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 406: /* f_larglist: '(' f_args opt_bv_decl ')'  */
#line 3751 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-2].nd);
                    }
#line 10202 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 407: /* f_larglist: f_args  */
#line 3755 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 10210 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 408: /* lambda_body: tLAMBEG compstmt '}'  */
#line 3761 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 10218 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 409: /* lambda_body: "'do' for lambda" bodystmt "'end'"  */
#line 3765 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 10226 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 410: /* @28: %empty  */
#line 3771 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_nest(p);
                      nvars_nest(p);
                      (yyval.num) = p->lineno;
                    }
#line 10236 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 411: /* do_block: "'do' for block" @28 opt_block_param bodystmt "'end'"  */
#line 3779 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_block(p,(yyvsp[-2].nd),(yyvsp[-1].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-3].num));
                      local_unnest(p);
                      nvars_unnest(p);
                    }
#line 10247 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 412: /* block_call: command do_block  */
#line 3788 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      call_with_block(p, (yyvsp[-1].nd), (yyvsp[0].nd));
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 10256 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 413: /* block_call: block_call call_op2 operation2 opt_paren_args  */
#line 3793 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-3].nd), (yyvsp[-1].id), (yyvsp[0].nd), (yyvsp[-2].num));
                    }
#line 10264 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 414: /* block_call: block_call call_op2 operation2 opt_paren_args brace_block  */
#line 3797 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-4].nd), (yyvsp[-2].id), (yyvsp[-1].nd), (yyvsp[-3].num));
                      call_with_block(p, (yyval.nd), (yyvsp[0].nd));
                    }
#line 10273 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 415: /* block_call: block_call call_op2 operation2 command_args do_block  */
#line 3802 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-4].nd), (yyvsp[-2].id), (yyvsp[-1].nd), (yyvsp[-3].num));
                      call_with_block(p, (yyval.nd), (yyvsp[0].nd));
                    }
#line 10282 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 416: /* method_call: operation paren_args  */
#line 3809 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_fcall(p, (yyvsp[-1].id), (yyvsp[0].nd));
                    }
#line 10290 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 417: /* method_call: primary_value call_op operation2 opt_paren_args  */
#line 3813 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-3].nd), (yyvsp[-1].id), (yyvsp[0].nd), (yyvsp[-2].num));
                    }
#line 10298 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 418: /* method_call: primary_value "::" operation2 paren_args  */
#line 3817 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-3].nd), (yyvsp[-1].id), (yyvsp[0].nd), tCOLON2);
                    }
#line 10306 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 419: /* method_call: primary_value "::" operation3  */
#line 3821 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-2].nd), (yyvsp[0].id), 0, tCOLON2);
                    }
#line 10314 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 420: /* method_call: primary_value call_op paren_args  */
#line 3825 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-2].nd), MRB_SYM(call), (yyvsp[0].nd), (yyvsp[-1].num));
                    }
#line 10322 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 421: /* method_call: primary_value "::" paren_args  */
#line 3829 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-2].nd), MRB_SYM(call), (yyvsp[0].nd), tCOLON2);
                    }
#line 10330 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 422: /* method_call: "'super'" paren_args  */
#line 3833 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_super(p, (yyvsp[0].nd));
                    }
#line 10338 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 423: /* method_call: "'super'"  */
#line 3837 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_zsuper(p);
                    }
#line 10346 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 424: /* method_call: primary_value '[' opt_call_args ']'  */
#line 3841 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_call(p, (yyvsp[-3].nd), intern_op(aref), (yyvsp[-1].nd), '.');
                    }
#line 10354 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 425: /* @29: %empty  */
#line 3847 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_nest(p);
                      nvars_nest(p);
                      (yyval.num) = p->lineno;
                    }
#line 10364 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 426: /* brace_block: '{' @29 opt_block_param compstmt '}'  */
#line 3854 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_block(p,(yyvsp[-2].nd),(yyvsp[-1].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-3].num));
                      local_unnest(p);
                      nvars_unnest(p);
                    }
#line 10375 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 427: /* @30: %empty  */
#line 3861 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_nest(p);
                      nvars_nest(p);
                      (yyval.num) = p->lineno;
                    }
#line 10385 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 428: /* brace_block: "'do'" @30 opt_block_param bodystmt "'end'"  */
#line 3868 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_block(p,(yyvsp[-2].nd),(yyvsp[-1].nd));
                      SET_LINENO((yyval.nd), (yyvsp[-3].num));
                      local_unnest(p);
                      nvars_unnest(p);
                    }
#line 10396 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 429: /* case_body: "'when'" args then compstmt cases  */
#line 3879 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = cons(cons((yyvsp[-3].nd), (yyvsp[-1].nd)), (yyvsp[0].nd));
                    }
#line 10404 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 430: /* cases: opt_else  */
#line 3885 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      if ((yyvsp[0].nd)) {
                        (yyval.nd) = cons(cons(0, (yyvsp[0].nd)), 0);
                      }
                      else {
                        (yyval.nd) = 0;
                      }
                    }
#line 10417 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 432: /* in_clauses: opt_else  */
#line 3899 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd) ? list1(new_in(p, NULL, NULL, (yyvsp[0].nd), FALSE)) : 0;
                    }
#line 10425 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 433: /* $@31: %empty  */
#line 3902 "mrbgems/mruby-compiler/core/parse.y"
                                    {p->in_kwarg--;}
#line 10431 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 434: /* in_clauses: "'in'" p_expr $@31 then compstmt in_clauses  */
#line 3903 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *in_clause = new_in(p, (yyvsp[-4].nd), NULL, (yyvsp[-1].nd), FALSE);
                      (yyval.nd) = cons(in_clause, (yyvsp[0].nd));
                    }
#line 10440 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 435: /* $@32: %empty  */
#line 3907 "mrbgems/mruby-compiler/core/parse.y"
                                    {p->in_kwarg--;}
#line 10446 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 436: /* in_clauses: "'in'" p_expr $@32 "'if' modifier" expr_value then compstmt in_clauses  */
#line 3908 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *in_clause = new_in(p, (yyvsp[-6].nd), (yyvsp[-3].nd), (yyvsp[-1].nd), FALSE);
                      (yyval.nd) = cons(in_clause, (yyvsp[0].nd));
                    }
#line 10455 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 437: /* $@33: %empty  */
#line 3912 "mrbgems/mruby-compiler/core/parse.y"
                                    {p->in_kwarg--;}
#line 10461 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 438: /* in_clauses: "'in'" p_expr $@33 "'unless' modifier" expr_value then compstmt in_clauses  */
#line 3913 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *in_clause = new_in(p, (yyvsp[-6].nd), (yyvsp[-3].nd), (yyvsp[-1].nd), TRUE);
                      (yyval.nd) = cons(in_clause, (yyvsp[0].nd));
                    }
#line 10470 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 440: /* p_expr: p_args_head p_as  */
#line 3924 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_array(p, push((yyvsp[-1].nd), (yyvsp[0].nd)), 0, 0);
                    }
#line 10478 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 441: /* p_expr: p_args_head p_rest  */
#line 3928 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_array(p, (yyvsp[-1].nd), (yyvsp[0].nd), 0);
                    }
#line 10486 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 442: /* p_expr: p_args_head p_rest ',' p_args_post  */
#line 3932 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_array(p, (yyvsp[-3].nd), (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10494 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 443: /* p_expr: p_rest  */
#line 3936 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_array(p, 0, (yyvsp[0].nd), 0);
                    }
#line 10502 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 444: /* p_expr: p_rest ',' p_args_post  */
#line 3940 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_array(p, 0, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10510 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 445: /* p_expr: p_hash_elems  */
#line 3944 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* Brace-less hash pattern: in a:, b: x */
                      (yyval.nd) = new_pat_hash(p, (yyvsp[0].nd), 0);
                    }
#line 10519 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 446: /* p_expr: p_hash_elems ',' p_kwrest  */
#line 3949 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* Brace-less hash pattern with kwrest: in a:, **rest */
                      (yyval.nd) = new_pat_hash(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10528 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 447: /* p_expr: p_kwrest  */
#line 3954 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* Brace-less kwrest only: in **rest */
                      (yyval.nd) = new_pat_hash(p, 0, (yyvsp[0].nd));
                    }
#line 10537 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 448: /* p_args_head: p_as ','  */
#line 3962 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[-1].nd));
                    }
#line 10545 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 449: /* p_args_head: p_args_head p_as ','  */
#line 3966 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[-1].nd));
                    }
#line 10553 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 450: /* p_args_post: p_as  */
#line 3973 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 10561 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 451: /* p_args_post: p_args_post ',' p_as  */
#line 3977 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10569 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 453: /* p_as: p_alt "=>" "local variable or method"  */
#line 3984 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_as(p, (yyvsp[-2].nd), (yyvsp[0].id));
                    }
#line 10577 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 455: /* p_alt: p_alt '|' p_value  */
#line 3991 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_alt(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10585 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 457: /* p_value: numeric  */
#line 3998 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_value(p, (yyvsp[0].nd));
                    }
#line 10593 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 458: /* p_value: symbol  */
#line 4002 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_value(p, (yyvsp[0].nd));
                    }
#line 10601 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 459: /* p_value: tSTRING  */
#line 4006 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_value(p, new_str(p, list1((yyvsp[0].nd))));
                    }
#line 10609 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 460: /* p_value: "'nil'"  */
#line 4010 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_value(p, new_nil(p));
                    }
#line 10617 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 461: /* p_value: "'true'"  */
#line 4014 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_value(p, new_true(p));
                    }
#line 10625 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 462: /* p_value: "'false'"  */
#line 4018 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_value(p, new_false(p));
                    }
#line 10633 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 463: /* p_value: p_const  */
#line 4022 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_value(p, (yyvsp[0].nd));
                    }
#line 10641 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 466: /* p_value: '^' "local variable or method"  */
#line 4028 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_pin(p, (yyvsp[0].id));
                    }
#line 10649 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 467: /* p_array: "[" p_array_body ']'  */
#line 4035 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 10657 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 468: /* p_array: "[" ']'  */
#line 4039 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_array(p, 0, 0, 0);
                    }
#line 10665 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 469: /* p_array_body: p_array_elems  */
#line 4046 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* Just pre elements, no rest */
                      (yyval.nd) = new_pat_array(p, (yyvsp[0].nd), 0, 0);
                    }
#line 10674 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 470: /* p_array_body: p_array_elems ',' p_rest  */
#line 4051 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* Pre elements + rest, no post */
                      (yyval.nd) = new_pat_array(p, (yyvsp[-2].nd), (yyvsp[0].nd), 0);
                    }
#line 10683 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 471: /* p_array_body: p_array_elems ',' p_rest ',' p_array_elems  */
#line 4056 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* Pre + rest + post */
                      (yyval.nd) = new_pat_array(p, (yyvsp[-4].nd), (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10692 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 472: /* p_array_body: p_rest  */
#line 4061 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* Just rest, no pre or post */
                      (yyval.nd) = new_pat_array(p, 0, (yyvsp[0].nd), 0);
                    }
#line 10701 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 473: /* p_array_body: p_rest ',' p_array_elems  */
#line 4066 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* Rest + post, no pre */
                      (yyval.nd) = new_pat_array(p, 0, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10710 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 474: /* p_array_body: p_rest ',' p_array_elems ',' p_rest  */
#line 4071 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* Find pattern: [*pre, elems, *post] */
                      (yyval.nd) = new_pat_find(p, (yyvsp[-4].nd), (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10719 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 475: /* p_array_elems: p_as  */
#line 4079 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 10727 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 476: /* p_array_elems: p_array_elems ',' p_as  */
#line 4083 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10735 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 477: /* p_rest: "*" "local variable or method"  */
#line 4090 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_var(p, (yyvsp[0].id));
                    }
#line 10743 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 478: /* p_rest: "*"  */
#line 4094 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* Anonymous rest pattern */
                      (yyval.nd) = (node*)-1;
                    }
#line 10752 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 479: /* p_const: "constant"  */
#line 4102 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_const(p, (yyvsp[0].id));
                    }
#line 10760 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 480: /* p_const: p_const "::" "constant"  */
#line 4106 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_colon2(p, (yyvsp[-2].nd), (yyvsp[0].id));
                    }
#line 10768 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 481: /* p_const: tCOLON3 "constant"  */
#line 4110 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_colon3(p, (yyvsp[0].id));
                    }
#line 10776 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 482: /* p_hash: tLBRACE p_hash_body '}'  */
#line 4117 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 10784 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 483: /* p_hash: tLBRACE '}'  */
#line 4121 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_hash(p, 0, 0);
                    }
#line 10792 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 484: /* p_hash_body: p_hash_elems  */
#line 4128 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_hash(p, (yyvsp[0].nd), 0);
                    }
#line 10800 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 485: /* p_hash_body: p_hash_elems ',' p_kwrest  */
#line 4132 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_hash(p, (yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10808 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 486: /* p_hash_body: p_kwrest  */
#line 4136 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_hash(p, 0, (yyvsp[0].nd));
                    }
#line 10816 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 487: /* p_hash_elems: p_hash_elem  */
#line 4143 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 10824 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 488: /* p_hash_elems: p_hash_elems ',' p_hash_elem  */
#line 4147 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 10832 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 489: /* p_hash_elem: "local variable or method" "label" p_as  */
#line 4156 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* {key: pattern} */
                      (yyval.nd) = cons(new_sym(p, (yyvsp[-2].id)), (yyvsp[0].nd));
                    }
#line 10841 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 490: /* p_hash_elem: "local variable or method" "label"  */
#line 4161 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* {key:} shorthand - binds to variable with same name */
                      (yyval.nd) = cons(new_sym(p, (yyvsp[-1].id)), new_pat_var(p, (yyvsp[-1].id)));
                    }
#line 10850 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 491: /* p_kwrest: "**" "local variable or method"  */
#line 4169 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_var(p, (yyvsp[0].id));
                    }
#line 10858 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 492: /* p_kwrest: "**" "'nil'"  */
#line 4173 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* **nil - exact match, no extra keys allowed */
                      (yyval.nd) = (node*)-1;
                    }
#line 10867 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 493: /* p_kwrest: "**"  */
#line 4178 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* ** - anonymous rest, discards extra keys */
                      (yyval.nd) = (node*)-2;
                    }
#line 10876 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 494: /* p_var: "local variable or method"  */
#line 4185 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_pat_var(p, (yyvsp[0].id));
                    }
#line 10884 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 495: /* opt_rescue: "'rescue'" exc_list exc_var then compstmt opt_rescue  */
#line 4193 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1(list3((yyvsp[-4].nd), (yyvsp[-3].nd), (yyvsp[-1].nd)));
                      if ((yyvsp[0].nd)) (yyval.nd) = append((yyval.nd), (yyvsp[0].nd));
                    }
#line 10893 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 497: /* exc_list: arg  */
#line 4201 "mrbgems/mruby-compiler/core/parse.y"
                    {
                        (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 10901 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 500: /* exc_var: "=>" lhs  */
#line 4209 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 10909 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 502: /* opt_ensure: "'ensure'" compstmt  */
#line 4216 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 10917 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 509: /* string: string string_fragment  */
#line 4230 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = append((yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 10925 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 510: /* string_fragment: "character literal"  */
#line 4236 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* tCHAR is (len . str), wrap as cons list */
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 10934 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 511: /* string_fragment: tSTRING  */
#line 4241 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* tSTRING is (len . str), wrap as cons list */
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 10943 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 512: /* string_fragment: "string literal" tSTRING  */
#line 4246 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* $2 is (len . str), wrap as cons list */
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 10952 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 513: /* string_fragment: "string literal" string_rep tSTRING  */
#line 4251 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 10960 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 515: /* string_rep: string_rep string_interp  */
#line 4258 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = append((yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 10968 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 516: /* string_interp: tSTRING_MID  */
#line 4264 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      /* $1 is already in (len . str) format */
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 10977 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 517: /* @34: %empty  */
#line 4269 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push_strterm(p);
                    }
#line 10985 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 518: /* string_interp: tSTRING_PART @34 compstmt '}'  */
#line 4274 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      pop_strterm(p,(yyvsp[-2].nd));
                      /* $1 is already in (len . str) format, create (-1 . node) for expression */
                      node *expr_elem = cons(int_to_node(-1), (yyvsp[-1].nd));
                      (yyval.nd) = list2((yyvsp[-3].nd), expr_elem);
                    }
#line 10996 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 519: /* string_interp: tLITERAL_DELIM  */
#line 4281 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1(new_literal_delim(p));
                    }
#line 11004 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 520: /* string_interp: tHD_LITERAL_DELIM heredoc_bodies  */
#line 4285 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1(new_literal_delim(p));
                    }
#line 11012 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 521: /* xstring: tXSTRING_BEG tXSTRING  */
#line 4291 "mrbgems/mruby-compiler/core/parse.y"
                    {
                        (yyval.nd) = cons((yyvsp[0].nd), (node*)NULL);
                    }
#line 11020 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 522: /* xstring: tXSTRING_BEG string_rep tXSTRING  */
#line 4295 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 11028 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 523: /* regexp: tREGEXP_BEG tREGEXP  */
#line 4301 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *data = (yyvsp[0].nd);  /* ((len . pattern) . (flags . encoding)) */
                      const char *flags = (const char*)data->cdr->car;
                      const char *encoding = (const char*)data->cdr->cdr;
                      /* Use data->car directly as pattern_list: (len . pattern) */
                      node *pattern_list = cons(data->car, (node*)NULL);
                      (yyval.nd) = new_regx(p, pattern_list, flags, encoding);
                    }
#line 11041 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 524: /* regexp: tREGEXP_BEG string_rep tREGEXP  */
#line 4310 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *data = (yyvsp[0].nd);  /* ((len . pattern) . (flags . encoding)) */
                      const char *flags = (const char*)data->cdr->car;
                      const char *encoding = (const char*)data->cdr->cdr;
                      /* Append the pattern from $3->car to the string list $2 */
                      node *complete_list = push((yyvsp[-1].nd), data->car);
                      (yyval.nd) = new_regx(p, complete_list, flags, encoding);
                    }
#line 11054 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 528: /* heredoc_body: tHEREDOC_END  */
#line 4328 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      parser_heredoc_info *info = parsing_heredoc_info(p);
                      info->doc = push(info->doc, new_str_empty(p));
                      heredoc_end(p);
                    }
#line 11064 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 529: /* heredoc_body: heredoc_string_rep tHEREDOC_END  */
#line 4334 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      heredoc_end(p);
                    }
#line 11072 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 532: /* heredoc_string_interp: tHD_STRING_MID  */
#line 4344 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      parser_heredoc_info *info = parsing_heredoc_info(p);
                      info->doc = push(info->doc, (yyvsp[0].nd));
                      heredoc_treat_nextline(p);
                    }
#line 11082 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 533: /* @35: %empty  */
#line 4350 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push_strterm(p);
                    }
#line 11090 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 534: /* heredoc_string_interp: tHD_STRING_PART @35 compstmt '}'  */
#line 4355 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      pop_strterm(p, (yyvsp[-2].nd));
                      parser_heredoc_info *info = parsing_heredoc_info(p);
                      /* $1 is already in (len . str) format, create (-1 . node) for expression */
                      node *expr_elem = cons(int_to_node(-1), (yyvsp[-1].nd));
                      info->doc = push(push(info->doc, (yyvsp[-3].nd)), expr_elem);
                    }
#line 11102 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 535: /* words: tWORDS_BEG tSTRING  */
#line 4365 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_words(p, list1((yyvsp[0].nd)));
                    }
#line 11110 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 536: /* words: tWORDS_BEG string_rep tSTRING  */
#line 4369 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *n = (yyvsp[-1].nd);
                      n = push(n, (yyvsp[0].nd));
                      (yyval.nd) = new_words(p, n);
                    }
#line 11120 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 537: /* symbol: basic_symbol  */
#line 4377 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_sym(p, (yyvsp[0].id));
                    }
#line 11128 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 538: /* symbol: "symbol" "string literal" string_rep tSTRING  */
#line 4381 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *n = (yyvsp[-1].nd);
                      p->lstate = EXPR_ENDARG;
                      if (node_to_int((yyvsp[0].nd)->car) > 0) {
                        n = push(n, (yyvsp[0].nd));
                      }
                      else {
                        cons_free((yyvsp[0].nd));
                      }
                      (yyval.nd) = new_dsym(p, n);
                    }
#line 11144 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 539: /* symbol: "symbol" "numbered parameter"  */
#line 4393 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      mrb_sym sym = intern_numparam((yyvsp[0].num));
                      (yyval.nd) = new_sym(p, sym);
                    }
#line 11153 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 540: /* basic_symbol: "symbol" sym  */
#line 4400 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->lstate = EXPR_END;
                      (yyval.id) = (yyvsp[0].id);
                    }
#line 11162 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 545: /* sym: tSTRING  */
#line 4411 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = new_strsym(p, (yyvsp[0].nd));
                    }
#line 11170 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 546: /* sym: "string literal" tSTRING  */
#line 4415 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = new_strsym(p, (yyvsp[0].nd));
                    }
#line 11178 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 547: /* symbols: tSYMBOLS_BEG tSTRING  */
#line 4421 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_symbols(p, list1((yyvsp[0].nd)));
                    }
#line 11186 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 548: /* symbols: tSYMBOLS_BEG string_rep tSTRING  */
#line 4425 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      node *n = (yyvsp[-1].nd);
                      n = push(n, (yyvsp[0].nd));
                      (yyval.nd) = new_symbols(p, n);
                    }
#line 11196 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 551: /* numeric: tUMINUS_NUM "integer literal"  */
#line 4435 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_negate(p, (yyvsp[0].nd));
                    }
#line 11204 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 552: /* numeric: tUMINUS_NUM "float literal"  */
#line 4439 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_negate(p, (yyvsp[0].nd));
                    }
#line 11212 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 553: /* variable: "local variable or method"  */
#line 4445 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_lvar(p, (yyvsp[0].id));
                    }
#line 11220 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 554: /* variable: "instance variable"  */
#line 4449 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_ivar(p, (yyvsp[0].id));
                    }
#line 11228 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 555: /* variable: "global variable"  */
#line 4453 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_gvar(p, (yyvsp[0].id));
                    }
#line 11236 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 556: /* variable: "class variable"  */
#line 4457 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_cvar(p, (yyvsp[0].id));
                    }
#line 11244 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 557: /* variable: "constant"  */
#line 4461 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_const(p, (yyvsp[0].id));
                    }
#line 11252 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 558: /* var_lhs: variable  */
#line 4467 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      assignable(p, (yyvsp[0].nd));
                    }
#line 11260 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 559: /* var_lhs: "numbered parameter"  */
#line 4471 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[0]), p, "can't assign to numbered parameter");
                    }
#line 11268 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 560: /* var_ref: variable  */
#line 4477 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = var_reference(p, (yyvsp[0].nd));
                    }
#line 11276 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 561: /* var_ref: "numbered parameter"  */
#line 4481 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_nvar(p, (yyvsp[0].num));
                    }
#line 11284 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 562: /* var_ref: "'nil'"  */
#line 4485 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_nil(p);
                    }
#line 11292 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 563: /* var_ref: "'self'"  */
#line 4489 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_self(p);
                    }
#line 11300 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 564: /* var_ref: "'true'"  */
#line 4493 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_true(p);
                    }
#line 11308 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 565: /* var_ref: "'false'"  */
#line 4497 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_false(p);
                    }
#line 11316 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 566: /* var_ref: "'__FILE__'"  */
#line 4501 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      const char *fn = mrb_sym_name_len(p->mrb, p->filename_sym, NULL);
                      if (!fn) {
                        fn = "(null)";
                      }
                      (yyval.nd) = new_str(p, cons(cons(int_to_node(strlen(fn)), (node*)fn), (node*)NULL));
                    }
#line 11328 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 567: /* var_ref: "'__LINE__'"  */
#line 4509 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      char buf[16];

                      dump_int(p->lineno, buf);
                      (yyval.nd) = new_int(p, buf, 10, 0);
                    }
#line 11339 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 568: /* var_ref: "'__ENCODING__'"  */
#line 4516 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_fcall(p, MRB_SYM(__ENCODING__), 0);
                    }
#line 11347 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 571: /* superclass: %empty  */
#line 4526 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = 0;
                    }
#line 11355 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 572: /* $@36: %empty  */
#line 4530 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->lstate = EXPR_BEG;
                      p->cmd_start = TRUE;
                    }
#line 11364 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 573: /* superclass: '<' $@36 expr_value term  */
#line 4535 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 11372 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 576: /* f_arglist_paren: '(' f_args rparen  */
#line 4551 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                      p->lstate = EXPR_BEG;
                      p->cmd_start = TRUE;
                    }
#line 11382 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 577: /* f_arglist_paren: '(' f_arg ',' tBDOT3 rparen  */
#line 4557 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_dots(p, (yyvsp[-3].nd));
                    }
#line 11390 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 578: /* f_arglist_paren: '(' tBDOT3 rparen  */
#line 4561 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_dots(p, 0);
                    }
#line 11398 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 580: /* f_arglist: f_args term  */
#line 4568 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 11406 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 581: /* f_arglist: f_arg ',' tBDOT3 term  */
#line 4572 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_dots(p, (yyvsp[-3].nd));
                    }
#line 11414 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 582: /* f_arglist: "..." term  */
#line 4576 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_dots(p, 0);
                    }
#line 11422 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 583: /* f_label: "local variable or method" "label"  */
#line 4582 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = (yyvsp[-1].id);
                      local_nest(p);
                      p->lstate = EXPR_MID;  /* make newlines significant after label */
                    }
#line 11432 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 584: /* f_label: "numbered parameter" "label"  */
#line 4588 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = intern_numparam((yyvsp[-1].num));
                      local_nest(p);
                      p->lstate = EXPR_MID;  /* make newlines significant after label */
                    }
#line 11442 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 585: /* f_kw: f_label arg  */
#line 4596 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = new_kw_arg(p, (yyvsp[-1].id), cons((yyvsp[0].nd), locals_node(p)));
                      local_unnest(p);
                    }
#line 11452 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 586: /* f_kw: f_label  */
#line 4602 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_kw_arg(p, (yyvsp[0].id), 0);
                      local_unnest(p);
                    }
#line 11461 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 587: /* f_block_kw: f_label primary_value  */
#line 4609 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = new_kw_arg(p, (yyvsp[-1].id), cons((yyvsp[0].nd), locals_node(p)));
                      local_unnest(p);
                    }
#line 11471 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 588: /* f_block_kw: f_label  */
#line 4615 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_kw_arg(p, (yyvsp[0].id), 0);
                      local_unnest(p);
                    }
#line 11480 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 589: /* f_block_kwarg: f_block_kw  */
#line 4622 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 11488 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 590: /* f_block_kwarg: f_block_kwarg ',' f_block_kw  */
#line 4626 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 11496 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 591: /* f_kwarg: f_kw  */
#line 4632 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 11504 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 592: /* f_kwarg: f_kwarg ',' f_kw  */
#line 4636 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 11512 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 595: /* f_kwrest: kwrest_mark "local variable or method"  */
#line 4646 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = (yyvsp[0].id);
                    }
#line 11520 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 596: /* f_kwrest: kwrest_mark  */
#line 4650 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = intern_op(pow);
                    }
#line 11528 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 597: /* args_tail: f_kwarg ',' f_kwrest opt_f_block_arg  */
#line 4656 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, (yyvsp[-3].nd), (yyvsp[-1].id), (yyvsp[0].id));
                    }
#line 11536 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 598: /* args_tail: f_kwarg opt_f_block_arg  */
#line 4660 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, (yyvsp[-1].nd), 0, (yyvsp[0].id));
                    }
#line 11544 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 599: /* args_tail: f_kwrest opt_f_block_arg  */
#line 4664 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, 0, (yyvsp[-1].id), (yyvsp[0].id));
                    }
#line 11552 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 600: /* args_tail: f_block_arg  */
#line 4668 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, 0, 0, (yyvsp[0].id));
                    }
#line 11560 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 601: /* opt_args_tail: ',' args_tail  */
#line 4674 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[0].nd);
                    }
#line 11568 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 602: /* opt_args_tail: ','  */
#line 4678 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, 0, 0, 0);
                    }
#line 11576 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 603: /* opt_args_tail: %empty  */
#line 4682 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args_tail(p, 0, 0, 0);
                    }
#line 11584 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 604: /* f_args: f_arg ',' f_optarg ',' f_rest_arg opt_args_tail  */
#line 4688 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-5].nd), (yyvsp[-3].nd), (yyvsp[-1].id), 0, (yyvsp[0].nd));
                    }
#line 11592 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 605: /* f_args: f_arg ',' f_optarg ',' f_rest_arg ',' f_arg opt_args_tail  */
#line 4692 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-7].nd), (yyvsp[-5].nd), (yyvsp[-3].id), (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 11600 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 606: /* f_args: f_arg ',' f_optarg opt_args_tail  */
#line 4696 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-3].nd), (yyvsp[-1].nd), 0, 0, (yyvsp[0].nd));
                    }
#line 11608 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 607: /* f_args: f_arg ',' f_optarg ',' f_arg opt_args_tail  */
#line 4700 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-5].nd), (yyvsp[-3].nd), 0, (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 11616 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 608: /* f_args: f_arg ',' f_rest_arg opt_args_tail  */
#line 4704 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-3].nd), 0, (yyvsp[-1].id), 0, (yyvsp[0].nd));
                    }
#line 11624 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 609: /* f_args: f_arg ',' f_rest_arg ',' f_arg opt_args_tail  */
#line 4708 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-5].nd), 0, (yyvsp[-3].id), (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 11632 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 610: /* f_args: f_arg opt_args_tail  */
#line 4712 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, (yyvsp[-1].nd), 0, 0, 0, (yyvsp[0].nd));
                    }
#line 11640 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 611: /* f_args: f_optarg ',' f_rest_arg opt_args_tail  */
#line 4716 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[-3].nd), (yyvsp[-1].id), 0, (yyvsp[0].nd));
                    }
#line 11648 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 612: /* f_args: f_optarg ',' f_rest_arg ',' f_arg opt_args_tail  */
#line 4720 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[-5].nd), (yyvsp[-3].id), (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 11656 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 613: /* f_args: f_optarg opt_args_tail  */
#line 4724 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[-1].nd), 0, 0, (yyvsp[0].nd));
                    }
#line 11664 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 614: /* f_args: f_optarg ',' f_arg opt_args_tail  */
#line 4728 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[-3].nd), 0, (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 11672 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 615: /* f_args: f_rest_arg opt_args_tail  */
#line 4732 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, 0, (yyvsp[-1].id), 0, (yyvsp[0].nd));
                    }
#line 11680 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 616: /* f_args: f_rest_arg ',' f_arg opt_args_tail  */
#line 4736 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, 0, (yyvsp[-3].id), (yyvsp[-1].nd), (yyvsp[0].nd));
                    }
#line 11688 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 617: /* f_args: args_tail  */
#line 4740 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_args(p, 0, 0, 0, 0, (yyvsp[0].nd));
                    }
#line 11696 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 618: /* f_args: %empty  */
#line 4744 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_add_f(p, 0);
                      (yyval.nd) = new_args(p, 0, 0, 0, 0, 0);
                    }
#line 11705 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 619: /* f_bad_arg: "constant"  */
#line 4751 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[0]), p, "formal argument cannot be a constant");
                      (yyval.nd) = 0;
                    }
#line 11714 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 620: /* f_bad_arg: "instance variable"  */
#line 4756 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[0]), p, "formal argument cannot be an instance variable");
                      (yyval.nd) = 0;
                    }
#line 11723 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 621: /* f_bad_arg: "global variable"  */
#line 4761 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[0]), p, "formal argument cannot be a global variable");
                      (yyval.nd) = 0;
                    }
#line 11732 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 622: /* f_bad_arg: "class variable"  */
#line 4766 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[0]), p, "formal argument cannot be a class variable");
                      (yyval.nd) = 0;
                    }
#line 11741 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 623: /* f_bad_arg: "numbered parameter"  */
#line 4771 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      yyerror(&(yylsp[0]), p, "formal argument cannot be a numbered parameter");
                      (yyval.nd) = 0;
                    }
#line 11750 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 624: /* f_norm_arg: f_bad_arg  */
#line 4778 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = 0;
                    }
#line 11758 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 625: /* f_norm_arg: "local variable or method"  */
#line 4782 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_add_f(p, (yyvsp[0].id));
                      (yyval.id) = (yyvsp[0].id);
                    }
#line 11767 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 626: /* f_arg_item: f_norm_arg  */
#line 4789 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_lvar(p, (yyvsp[0].id));
                    }
#line 11775 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 627: /* @37: %empty  */
#line 4793 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = local_switch(p);
                    }
#line 11783 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 628: /* f_arg_item: tLPAREN @37 f_margs rparen  */
#line 4797 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = new_marg(p, (yyvsp[-1].nd));
                      local_resume(p, (yyvsp[-2].nd));
                      local_add_f(p, 0);
                    }
#line 11793 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 629: /* f_arg: f_arg_item  */
#line 4805 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 11801 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 630: /* f_arg: f_arg ',' f_arg_item  */
#line 4809 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 11809 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 631: /* f_opt_asgn: "local variable or method" '='  */
#line 4815 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_add_f(p, (yyvsp[-1].id));
                      local_nest(p);
                      (yyval.id) = (yyvsp[-1].id);
                    }
#line 11819 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 632: /* f_opt: f_opt_asgn arg  */
#line 4823 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = cons(sym_to_node((yyvsp[-1].id)), cons((yyvsp[0].nd), locals_node(p)));
                      local_unnest(p);
                    }
#line 11829 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 633: /* f_block_opt: f_opt_asgn primary_value  */
#line 4831 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = cons(sym_to_node((yyvsp[-1].id)), cons((yyvsp[0].nd), locals_node(p)));
                      local_unnest(p);
                    }
#line 11839 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 634: /* f_block_optarg: f_block_opt  */
#line 4839 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 11847 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 635: /* f_block_optarg: f_block_optarg ',' f_block_opt  */
#line 4843 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 11855 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 636: /* f_optarg: f_opt  */
#line 4849 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 11863 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 637: /* f_optarg: f_optarg ',' f_opt  */
#line 4853 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 11871 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 640: /* f_rest_arg: restarg_mark "local variable or method"  */
#line 4863 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      local_add_f(p, (yyvsp[0].id));
                      (yyval.id) = (yyvsp[0].id);
                    }
#line 11880 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 641: /* f_rest_arg: restarg_mark  */
#line 4868 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = intern_op(mul);
                      local_add_f(p, (yyval.id));
                    }
#line 11889 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 644: /* f_block_arg: blkarg_mark "local variable or method"  */
#line 4879 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = (yyvsp[0].id);
                    }
#line 11897 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 645: /* f_block_arg: blkarg_mark "'nil'"  */
#line 4883 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = MRB_SYM(nil);
                    }
#line 11905 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 646: /* f_block_arg: blkarg_mark  */
#line 4887 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = intern_op(and);
                    }
#line 11913 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 647: /* opt_f_block_arg: ',' f_block_arg  */
#line 4893 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = (yyvsp[0].id);
                    }
#line 11921 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 648: /* opt_f_block_arg: ','  */
#line 4897 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = 0;
                    }
#line 11929 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 649: /* opt_f_block_arg: none  */
#line 4901 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.id) = 0;
                    }
#line 11937 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 650: /* singleton: var_ref  */
#line 4907 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      prohibit_literals(p, (yyvsp[0].nd));
                      (yyval.nd) = (yyvsp[0].nd);
                      if (!(yyval.nd)) (yyval.nd) = new_nil(p);
                    }
#line 11947 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 651: /* $@38: %empty  */
#line 4912 "mrbgems/mruby-compiler/core/parse.y"
                      {p->lstate = EXPR_BEG;}
#line 11953 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 652: /* singleton: '(' $@38 expr rparen  */
#line 4913 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      prohibit_literals(p, (yyvsp[-1].nd));
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 11962 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 654: /* assoc_list: assocs trailer  */
#line 4921 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = (yyvsp[-1].nd);
                    }
#line 11970 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 655: /* assocs: assoc  */
#line 4927 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = list1((yyvsp[0].nd));
                    }
#line 11978 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 656: /* assocs: assocs comma assoc  */
#line 4931 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = push((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 11986 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 657: /* assoc: arg "=>" arg  */
#line 4937 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[-2].nd));
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = cons((yyvsp[-2].nd), (yyvsp[0].nd));
                    }
#line 11996 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 658: /* assoc: "local variable or method" "label" arg  */
#line 4943 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = cons(new_sym(p, (yyvsp[-2].id)), (yyvsp[0].nd));
                    }
#line 12005 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 659: /* assoc: "local variable or method" "label"  */
#line 4948 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = cons(new_sym(p, (yyvsp[-1].id)), label_reference(p, (yyvsp[-1].id)));
                    }
#line 12013 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 660: /* assoc: "numbered parameter" "label"  */
#line 4952 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      mrb_sym sym = intern_numparam((yyvsp[-1].num));
                      (yyval.nd) = cons(new_sym(p, sym), label_reference(p, sym));
                    }
#line 12022 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 661: /* assoc: "numbered parameter" "label" arg  */
#line 4957 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = cons(new_sym(p, intern_numparam((yyvsp[-2].num))), (yyvsp[0].nd));
                    }
#line 12031 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 662: /* assoc: string_fragment "label" arg  */
#line 4962 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      if ((yyvsp[-2].nd)->cdr) {
                        /* Multiple fragments - create dynamic symbol */
                        (yyval.nd) = cons(new_dsym(p, (yyvsp[-2].nd)), (yyvsp[0].nd));
                      }
                      else if (node_to_int((yyvsp[-2].nd)->car->car) < 0) {
                        /* Single fragment but it's an expression (-1 . node) - create dynamic symbol */
                        (yyval.nd) = cons(new_dsym(p, (yyvsp[-2].nd)), (yyvsp[0].nd));
                      }
                      else {
                        /* Single string fragment - create simple symbol */
                        (yyval.nd) = cons(new_sym(p, new_strsym(p, (yyvsp[-2].nd)->car)), (yyvsp[0].nd));
                      }
                    }
#line 12051 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 663: /* assoc: "**" arg  */
#line 4978 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      void_expr_error(p, (yyvsp[0].nd));
                      (yyval.nd) = cons(new_kw_rest_args(p, 0), (yyvsp[0].nd));
                    }
#line 12060 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 664: /* assoc: "**"  */
#line 4983 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = cons(new_kw_rest_args(p, 0), new_lvar(p, intern_op(pow)));
                    }
#line 12068 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 677: /* call_op: '.'  */
#line 5009 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.num) = '.';
                    }
#line 12076 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 678: /* call_op: "&."  */
#line 5013 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.num) = 0;
                    }
#line 12084 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 680: /* call_op2: "::"  */
#line 5020 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.num) = tCOLON2;
                    }
#line 12092 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 689: /* term: ';'  */
#line 5041 "mrbgems/mruby-compiler/core/parse.y"
                      {yyerrok;}
#line 12098 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 691: /* nl: '\n'  */
#line 5046 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      p->lineno += (yyvsp[0].num);
                      p->column = 0;
                    }
#line 12107 "mrbgems/mruby-compiler/core/y.tab.c"
    break;

  case 695: /* none: %empty  */
#line 5058 "mrbgems/mruby-compiler/core/parse.y"
                    {
                      (yyval.nd) = 0;
                    }
#line 12115 "mrbgems/mruby-compiler/core/y.tab.c"
    break;


#line 12119 "mrbgems/mruby-compiler/core/y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc, p);

  YYPOPSTACK (yylen);

  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx, p);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx, p);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, p, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, p);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);

  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp, p);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, p);
      YYPOPSTACK (1);

      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp, p);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp, p);


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, p, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, p);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp, p);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, p);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 5062 "mrbgems/mruby-compiler/core/parse.y"

#define pylval  (*((YYSTYPE*)(p->ylval)))

static void
yyerror(void *lp, parser_state *p, const char *s)
{
  char* c;
  size_t n;

  if (! p->capture_errors) {
#ifndef MRB_NO_STDIO
    if (p->filename_sym) {
      const char *filename = mrb_sym_name_len(p->mrb, p->filename_sym, NULL);
      fprintf(stderr, "%s:%d:%d: %s\n", filename, p->lineno, p->column, s);
    }
    else {
      fprintf(stderr, "line %d:%d: %s\n", p->lineno, p->column, s);
    }
#endif
  }
  else if (p->nerr < sizeof(p->error_buffer) / sizeof(p->error_buffer[0])) {
    n = strlen(s);
    c = (char*)parser_palloc(p, n + 1);
    memcpy(c, s, n + 1);
    p->error_buffer[p->nerr].message = c;
    p->error_buffer[p->nerr].lineno = p->lineno;
    p->error_buffer[p->nerr].column = p->column;
  }
  p->nerr++;
}

static void
yyerror_c(parser_state *p, const char *msg, char c)
{
  char buf[256];

  strncpy(buf, msg, sizeof(buf) - 2);
  buf[sizeof(buf) - 2] = '\0';
  strncat(buf, &c, 1);
  yyerror(NULL, p, buf);
}

static void
yywarning(parser_state *p, const char *s)
{
  char* c;
  size_t n;

  if (! p->capture_errors) {
#ifndef MRB_NO_STDIO
    if (p->filename_sym) {
      const char *filename = mrb_sym_name_len(p->mrb, p->filename_sym, NULL);
      fprintf(stderr, "%s:%d:%d: warning: %s\n", filename, p->lineno, p->column, s);
    }
    else {
      fprintf(stderr, "line %d:%d: warning: %s\n", p->lineno, p->column, s);
    }
#endif
  }
  else if (p->nwarn < sizeof(p->warn_buffer) / sizeof(p->warn_buffer[0])) {
    n = strlen(s);
    c = (char*)parser_palloc(p, n + 1);
    memcpy(c, s, n + 1);
    p->warn_buffer[p->nwarn].message = c;
    p->warn_buffer[p->nwarn].lineno = p->lineno;
    p->warn_buffer[p->nwarn].column = p->column;
  }
  p->nwarn++;
}

static void
yywarning_s(parser_state *p, const char *msg, const char *s)
{
  char buf[256];

  strncpy(buf, msg, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';
  strncat(buf, ": ", sizeof(buf) - strlen(buf) - 1);
  strncat(buf, s, sizeof(buf) - strlen(buf) - 1);
  yywarning(p, buf);
}

static void
backref_error(parser_state *p, node *n)
{
  int c;

  c = node_to_int(n->car);

  if (c == NODE_NTH_REF) {
    yyerror_c(p, "can't set variable $", (char)node_to_int(n->cdr)+'0');
  }
  else if (c == NODE_BACK_REF) {
    yyerror_c(p, "can't set variable $", (char)node_to_int(n->cdr));
  }
  else {
    yyerror(NULL, p, "Internal error in backref_error()");
  }
}

static void
void_expr_error(parser_state *p, node *n)
{
  if (n == NULL) return;

  /* Check if this is a variable-sized node first */
  struct mrb_ast_var_header *header = (struct mrb_ast_var_header*)n;
  if (header) {
    /* Handle variable-sized nodes */
    switch ((enum node_type)header->node_type) {
    case NODE_BREAK:
    case NODE_RETURN:
    case NODE_NEXT:
    case NODE_REDO:
    case NODE_RETRY:
      yyerror(NULL, p, "void value expression");
      return;
    case NODE_AND:
    case NODE_OR:
      {
        struct mrb_ast_and_node *and_n = (struct mrb_ast_and_node*)n;
        void_expr_error(p, (node*)and_n->left);
        void_expr_error(p, (node*)and_n->right);
      }
      return;
    case NODE_STMTS:
      {
        struct mrb_ast_stmts_node *stmts = (struct mrb_ast_stmts_node*)n;
        node *last = stmts->stmts;
        if (last) {
          /* Find the last statement in the cons list */
          while (last->cdr) {
            last = last->cdr;
          }
          void_expr_error(p, last->car);
        }
      }
      return;
    case NODE_BEGIN:
      {
        struct mrb_ast_begin_node *begin_n = (struct mrb_ast_begin_node*)n;
        if (begin_n->body) {
          void_expr_error(p, (node*)begin_n->body);
        }
      }
      return;
    default:
      /* Other variable-sized nodes are OK */
      return;
    }
  }

  /* Should not reach here - all nodes should be variable-sized now */
}

static void pushback(parser_state *p, int c);
static mrb_bool peeks(parser_state *p, const char *s);
static mrb_bool skips(parser_state *p, const char *s);

static inline int
nextc0(parser_state *p)
{
  if (p->s && p->s < p->send) {
    return (unsigned char)*p->s++;
  }
  else {
#ifndef MRB_NO_STDIO
    int c;

    if (p->f) {
      c = fgetc(p->f);
      if (!feof(p->f)) return c;
    }
#endif
    return -1;
  }
}

static inline int
nextc(parser_state *p)
{
  int c;

  if (p->pb) {
    node *tmp;

    c = node_to_int(p->pb->car);
    tmp = p->pb;
    p->pb = p->pb->cdr;
    cons_free(tmp);
  }
  else {
    c = nextc0(p);
    if (c < 0) goto eof;
  }
  if (c >= 0) {
    p->column++;
  }
  if (c == '\r') {
    const int lf = nextc0(p);
    if (lf == '\n') {
      return '\n';
    }
    if (lf > 0) pushback(p, lf);
  }
  return c;

  eof:
  if (!p->cxt) return -1;
  else {
    if (p->cxt->partial_hook(p) < 0)
      return -1;                /* end of program(s) */
    return -2;                  /* end of a file in the program files */
  }
}

static void
pushback(parser_state *p, int c)
{
  if (c >= 0) {
    p->column--;
  }
  p->pb = cons(int_to_node(c), p->pb);
}

static void
skip(parser_state *p, char term)
{
  int c;

  for (;;) {
    c = nextc(p);
    if (c < 0) break;
    if (c == term) break;
  }
}

/* Lower-cases a key character and maps '-' to '_', so `frozen-string-literal` also matches. */
static int
magic_key_char(int c)
{
  if (c == '-') return '_';
  if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
  return c;
}

static mrb_bool
magic_ident_char(int c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

/* Sets p->frozen_string_literal from a `frozen_string_literal: true|false` in one comment line (ADR 0546). */
static void
frozen_magic_comment(parser_state *p, const char *s)
{
  static const char key[] = "frozen_string_literal";
  const size_t klen = sizeof(key) - 1;

  for (; *s; s++) {
    size_t i = 0;
    const char *v;

    while (i < klen && s[i] && magic_key_char((unsigned char)s[i]) == key[i]) i++;
    if (i != klen) continue;
    v = s + klen;
    while (*v == ' ' || *v == '\t') v++;
    if (*v != ':') continue;
    v++;
    while (*v == ' ' || *v == '\t') v++;
    if (magic_key_char((unsigned char)v[0]) == 't' && magic_key_char((unsigned char)v[1]) == 'r' &&
        magic_key_char((unsigned char)v[2]) == 'u' && magic_key_char((unsigned char)v[3]) == 'e' &&
        !magic_ident_char((unsigned char)v[4])) {
      p->frozen_string_literal = TRUE;
    }
    else if (magic_key_char((unsigned char)v[0]) == 'f' && magic_key_char((unsigned char)v[1]) == 'a' &&
             magic_key_char((unsigned char)v[2]) == 'l' && magic_key_char((unsigned char)v[3]) == 's' &&
             magic_key_char((unsigned char)v[4]) == 'e' && !magic_ident_char((unsigned char)v[5])) {
      p->frozen_string_literal = FALSE;
    }
    return;
  }
}

/* Consumes one comment line. The line is read into a bounded buffer, so a magic comment
 * must start within the first 255 bytes of its line. */
static void
comment_line(parser_state *p)
{
  char line[256];
  size_t n = 0;
  int c;

  for (;;) {
    c = nextc(p);
    if (c < 0 || c == '\n') break;
    if (n < sizeof(line) - 1) line[n++] = (char)c;
  }
  line[n] = '\0';
  if (!p->token_seen) frozen_magic_comment(p, line);
}

static int
peekc_n(parser_state *p, int n)
{
  node *list = 0;
  int c0;

  do {
    c0 = nextc(p);
    if (c0 == -1) return c0;    /* do not skip partial EOF */
    if (c0 >= 0) --p->column;
    list = push(list, int_to_node(c0));
  } while(n--);
  if (p->pb) {
    p->pb = append(list, p->pb);
  }
  else {
    p->pb = list;
  }
  return c0;
}

static mrb_bool
peek_n(parser_state *p, int c, int n)
{
  return peekc_n(p, n) == c && c >= 0;
}
#define peek(p,c) peek_n((p), (c), 0)

static mrb_bool
peeks(parser_state *p, const char *s)
{
  size_t len = strlen(s);

#ifndef MRB_NO_STDIO
  if (p->f) {
    int n = 0;
    while (*s) {
      if (!peek_n(p, *s++, n++)) return FALSE;
    }
    return TRUE;
  }
  else
#endif
    if (p->s && p->s + len <= p->send) {
      if (memcmp(p->s, s, len) == 0) return TRUE;
    }
  return FALSE;
}

static mrb_bool
skips(parser_state *p, const char *s)
{
  int c;

  for (;;) {
    /* skip until first char */
    for (;;) {
      c = nextc(p);
      if (c < 0) return FALSE;
      if (c == '\n') {
        p->lineno++;
        p->column = 0;
      }
      if (c == *s) break;
    }
    s++;
    if (peeks(p, s)) {
      size_t len = strlen(s);

      while (len--) {
        if (nextc(p) == '\n') {
          p->lineno++;
          p->column = 0;
        }
      }
      return TRUE;
    }
    else {
      s--;
    }
  }
  return FALSE;
}

static int
newtok(parser_state *p)
{
  if (p->tokbuf != p->buf) {
    mrbc_free(p->tokbuf);
    p->tokbuf = p->buf;
    p->tsiz = MRB_PARSER_TOKBUF_SIZE;
  }
  p->tidx = 0;
  return p->column - 1;
}

static void
tokadd(parser_state *p, int32_t c)
{
  char utf8[4];
  int i, len;

  /* mrb_assert(-0x10FFFF <= c && c <= 0xFF); */
  if (c >= 0) {
    /* Single byte from source or non-Unicode escape */
    utf8[0] = (char)c;
    len = 1;
  }
  else {
    /* Unicode character (negative c indicates codepoint) */
    len = (int)mrb_utf8_to_buf(utf8, (uint32_t)(-c));
  }
  if (p->tidx+len >= p->tsiz) {
    if (p->tsiz >= MRB_PARSER_TOKBUF_MAX) {
      p->tidx += len;
      return;
    }
    p->tsiz *= 2;
    if (p->tokbuf == p->buf) {
      p->tokbuf = (char*)mrbc_malloc(p->tsiz);
      memcpy(p->tokbuf, p->buf, MRB_PARSER_TOKBUF_SIZE);
    }
    else {
      p->tokbuf = (char*)mrbc_realloc(p->tokbuf, p->tsiz);
    }
  }
  for (i = 0; i < len; i++) {
    p->tokbuf[p->tidx++] = utf8[i];
  }
}

static int
toklast(parser_state *p)
{
  return p->tokbuf[p->tidx-1];
}

static void
tokfix(parser_state *p)
{
  if (p->tidx >= MRB_PARSER_TOKBUF_MAX) {
    p->tidx = MRB_PARSER_TOKBUF_MAX-1;
    yyerror(NULL, p, "string too long (truncated)");
  }
  p->tokbuf[p->tidx] = '\0';
}

static const char*
tok(parser_state *p)
{
  return p->tokbuf;
}

static int
toklen(parser_state *p)
{
  return p->tidx;
}

#define IS_ARG() (p->lstate == EXPR_ARG || p->lstate == EXPR_CMDARG)
#define IS_END() (p->lstate == EXPR_END || p->lstate == EXPR_ENDARG || p->lstate == EXPR_ENDFN)
#define IS_BEG() (p->lstate == EXPR_BEG || p->lstate == EXPR_MID || p->lstate == EXPR_VALUE || p->lstate == EXPR_CLASS)
#define IS_SPCARG(c) (IS_ARG() && space_seen && !ISSPACE(c))
#define IS_LABEL_POSSIBLE() ((p->lstate == EXPR_BEG && !cmd_state) || IS_ARG() || p->lstate == EXPR_VALUE)
#define IS_LABEL_SUFFIX(n) (peek_n(p, ':',(n)) && !peek_n(p, ':', (n)+1))

static int32_t
scan_oct(const int *start, int len, int *retlen)
{
  const int *s = start;
  int32_t retval = 0;

  /* mrb_assert(len <= 3) */
  while (len-- && *s >= '0' && *s <= '7') {
    retval <<= 3;
    retval |= *s++ - '0';
  }
  *retlen = (int)(s - start);

  return retval;
}

static int32_t
scan_hex(parser_state *p, const int *start, int len, int *retlen)
{
  static const char hexdigit[] = "0123456789abcdef0123456789ABCDEF";
  const int *s = start;
  uint32_t retval = 0;
  char *tmp;

  /* mrb_assert(len <= 8) */
  while (len-- && *s && (tmp = (char*)strchr(hexdigit, *s))) {
    retval <<= 4;
    retval |= (tmp - hexdigit) & 15;
    s++;
  }
  *retlen = (int)(s - start);

  return (int32_t)retval;
}

static int32_t
read_escape_unicode(parser_state *p, int limit)
{
  int buf[9];
  int i;
  int32_t hex;

  /* Look for opening brace */
  i = 0;
  buf[0] = nextc(p);
  if (buf[0] < 0) {
  eof:
    yyerror(NULL, p, "invalid escape character syntax");
    return -1;
  }
  if (ISXDIGIT(buf[0])) {
    /* \uxxxx form */
    for (i=1; i<limit; i++) {
      buf[i] = nextc(p);
      if (buf[i] < 0) goto eof;
      if (!ISXDIGIT(buf[i])) {
        pushback(p, buf[i]);
        break;
      }
    }
  }
  else {
    pushback(p, buf[0]);
  }
  hex = scan_hex(p, buf, i, &i);
  if (i == 0 || hex > 0x10FFFF || (hex & 0xFFFFF800) == 0xD800) {
    yyerror(NULL, p, "invalid Unicode code point");
    return -1;
  }
  return hex;
}

/* Return negative to indicate Unicode code point */
static int32_t
read_escape(parser_state *p)
{
  int32_t c;

  switch (c = nextc(p)) {
  case '\\':/* Backslash */
    return c;

  case 'n':/* newline */
    return '\n';

  case 't':/* horizontal tab */
    return '\t';

  case 'r':/* carriage-return */
    return '\r';

  case 'f':/* form-feed */
    return '\f';

  case 'v':/* vertical tab */
    return '\13';

  case 'a':/* alarm(bell) */
    return '\007';

  case 'e':/* escape */
    return 033;

  case '0': case '1': case '2': case '3': /* octal constant */
  case '4': case '5': case '6': case '7':
  {
    int buf[3];
    int i;

    buf[0] = c;
    for (i=1; i<3; i++) {
      buf[i] = nextc(p);
      if (buf[i] < 0) goto eof;
      if (buf[i] < '0' || '7' < buf[i]) {
        pushback(p, buf[i]);
        break;
      }
    }
    c = scan_oct(buf, i, &i);
  }
  return c;

  case 'x':     /* hex constant */
  {
    int buf[2];
    int i;

    for (i=0; i<2; i++) {
      buf[i] = nextc(p);
      if (buf[i] < 0) goto eof;
      if (!ISXDIGIT(buf[i])) {
        pushback(p, buf[i]);
        break;
      }
    }
    if (i == 0) {
      yyerror(NULL, p, "invalid hex escape");
      return -1;
    }
    return scan_hex(p, buf, i, &i);
  }

  case 'u':     /* Unicode */
    if (peek(p, '{')) {
      /* \u{xxxxxxxx} form */
      nextc(p);
      c = read_escape_unicode(p, 8);
      if (c < 0) return 0;
      if (nextc(p) != '}') goto eof;
    }
    else {
      c = read_escape_unicode(p, 4);
      if (c < 0) return 0;
    }
    return -c;

  case 'b':/* backspace */
    return '\010';

  case 's':/* space */
    return ' ';

  case 'M':
    if ((c = nextc(p)) != '-') {
      yyerror(NULL, p, "Invalid escape character syntax");
      pushback(p, c);
      return '\0';
    }
    if ((c = nextc(p)) == '\\') {
      return read_escape(p) | 0x80;
    }
    else if (c < 0) goto eof;
    else {
      return ((c & 0xff) | 0x80);
    }

  case 'C':
    if ((c = nextc(p)) != '-') {
      yyerror(NULL, p, "Invalid escape character syntax");
      pushback(p, c);
      return '\0';
    }
  case 'c':
    if ((c = nextc(p))== '\\') {
      c = read_escape(p);
    }
    else if (c == '?')
      return 0177;
    else if (c < 0) goto eof;
    return c & 0x9f;

    eof:
  case -1:
  case -2:                      /* end of a file */
    yyerror(NULL, p, "Invalid escape character syntax");
    return '\0';

  default:
    return c;
  }
}

static void
heredoc_count_indent(parser_heredoc_info *hinfo, const char *str, size_t len, size_t spaces, size_t *offset)
{
  size_t indent = 0;
  *offset = 0;
  for (size_t i = 0; i < len; i++) {
    size_t size;
    if (str[i] == '\n')
      break;
    else if (str[i] == '\t')
      size = 8;
    else if (ISSPACE(str[i]))
      size = 1;
    else
      break;
    size_t nindent = indent + size;
    if (nindent > spaces || nindent > hinfo->indent)
      break;
    indent = nindent;
    *offset += 1;
  }
}

static void
heredoc_remove_indent(parser_state *p, parser_heredoc_info *hinfo)
{
  if (!hinfo->remove_indent || hinfo->indent == 0)
    return;
  node *indented, *n, *pair, *escaped, *nspaces;
  const char *str;
  size_t len, spaces, offset, start, end;
  indented = hinfo->indented;
  while (indented) {
    n = indented->car;
    pair = n->car;
    len = (size_t)pair->car;
    str = (char*)pair->cdr;
    escaped = n->cdr->car;
    nspaces = n->cdr->cdr;
    if (escaped) {
      char *newstr = strndup(str, len);
      size_t newlen = 0;
      start = 0;
      while (start < len) {
        end = escaped ? (size_t)escaped->car : len;
        if (end > len) end = len;
        spaces = (size_t)nspaces->car;
        size_t esclen = end - start;
        heredoc_count_indent(hinfo, str + start, esclen, spaces, &offset);
        esclen -= offset;
        memcpy(newstr + newlen, str + start + offset, esclen);
        newlen += esclen;
        start = end;
        if (escaped)
          escaped = escaped->cdr;
        nspaces = nspaces->cdr;
      }
      if (newlen < len)
        newstr[newlen] = '\0';
      pair->car = (node*)newlen;
      pair->cdr = (node*)newstr;
    }
    else {
      spaces = (size_t)nspaces->car;
      heredoc_count_indent(hinfo, str, len, spaces, &offset);
      pair->car = (node*)(len - offset);
      pair->cdr = (node*)(str + offset);
    }
    indented = indented->cdr;
  }
}

static void
heredoc_push_indented(parser_state *p, parser_heredoc_info *hinfo, node *pair, node *escaped, node *nspaces, mrb_bool empty_line)
{
  hinfo->indented = push(hinfo->indented, cons(pair, cons(escaped, nspaces)));
  while (nspaces) {
    size_t tspaces = (size_t)nspaces->car;
    if ((hinfo->indent == ~0U || tspaces < hinfo->indent) && !empty_line)
      hinfo->indent = tspaces;
    nspaces = nspaces->cdr;
  }
}

static int
parse_string(parser_state *p)
{
  int c;
  string_type type = (string_type)p->lex_strterm->type;
  int nest_level = p->lex_strterm->level;
  int beg = p->lex_strterm->paren;
  int end = p->lex_strterm->term;
  parser_heredoc_info *hinfo = (type & STR_FUNC_HEREDOC) ? parsing_heredoc_info(p) : NULL;

  mrb_bool unindent = hinfo && hinfo->remove_indent;
  mrb_bool head = hinfo && hinfo->line_head;
  mrb_bool empty = TRUE;
  size_t spaces = 0;
  size_t pos = -1;
  node *escaped = NULL;
  node *nspaces = NULL;

  if (beg == 0) beg = -3;       /* should never happen */
  if (end == 0) end = -3;
  newtok(p);
  while ((c = nextc(p)) != end || nest_level != 0) {
    pos++;
    if (hinfo && (c == '\n' || c < 0)) {
      mrb_bool line_head;
      tokadd(p, '\n');
      tokfix(p);
      p->lineno++;
      p->column = 0;
      line_head = hinfo->line_head;
      hinfo->line_head = TRUE;
      if (line_head) {
        /* check whether end of heredoc */
        const char *s = tok(p);
        int len = toklen(p);
        if (hinfo->allow_indent) {
          while (ISSPACE(*s) && len > 0) {
            s++;
            len--;
          }
        }
        if (hinfo->term_len > 0 && len-1 == hinfo->term_len && strncmp(s, hinfo->term, len-1) == 0) {
          heredoc_remove_indent(p, hinfo);
          return tHEREDOC_END;
        }
      }
      if (c < 0) {
        char buf[256];
        const char s1[] = "can't find heredoc delimiter \"";
        const char s2[] = "\" anywhere before EOF";

        if (sizeof(s1)+sizeof(s2)+strlen(hinfo->term)+1 >= sizeof(buf)) {
          yyerror(NULL, p, "can't find heredoc delimiter anywhere before EOF");
        }
        else {
          strcpy(buf, s1);
          strcat(buf, hinfo->term);
          strcat(buf, s2);
          yyerror(NULL, p, buf);
        }
        return 0;
      }
      pylval.nd = new_str_tok(p);
      if (unindent && head) {
        nspaces = push(nspaces, int_to_node(spaces));
        heredoc_push_indented(p, hinfo, pylval.nd, escaped, nspaces, empty && line_head);
      }
      return tHD_STRING_MID;
    }
    if (unindent && empty) {
      if (c == '\t')
        spaces += 8;
      else if (ISSPACE(c))
        spaces++;
      else
        empty = FALSE;
    }
    if (c < 0) {
      yyerror(NULL, p, "unterminated string meets end of file");
      return 0;
    }
    else if (c == beg) {
      nest_level++;
      p->lex_strterm->level = nest_level;
    }
    else if (c == end) {
      nest_level--;
      p->lex_strterm->level = nest_level;
    }
    else if (c == '\\') {
      c = nextc(p);
      if (type & STR_FUNC_EXPAND) {
        if (c == end || c == beg) {
          tokadd(p, c);
        }
        else if (c == '\n') {
          p->lineno++;
          p->column = 0;
          if (unindent) {
            nspaces = push(nspaces, int_to_node(spaces));
            escaped = push(escaped, int_to_node(pos));
            pos--;
            empty = TRUE;
            spaces = 0;
          }
          if (type & STR_FUNC_ARRAY) {
            tokadd(p, '\n');
          }
        }
        else if (type & STR_FUNC_REGEXP) {
          tokadd(p, '\\');
          tokadd(p, c);
        }
        else if (c == 'u' && peek(p, '{')) {
          /* \u{xxxx xxxx xxxx} form */
          nextc(p);
          while (1) {
            do c = nextc(p); while (ISSPACE(c));
            if (c == '}') break;
            pushback(p, c);
            c = read_escape_unicode(p, 8);
            if (c < 0) break;
            tokadd(p, -c);
          }
          if (hinfo)
            hinfo->line_head = FALSE;
        }
        else {
          pushback(p, c);
          tokadd(p, read_escape(p));
          if (hinfo)
            hinfo->line_head = FALSE;
        }
      }
      else {
        if (c != beg && c != end) {
          if (c == '\n') {
            p->lineno++;
            p->column = 0;
          }
          if (!(c == '\\' || ((type & STR_FUNC_ARRAY) && ISSPACE(c)))) {
            tokadd(p, '\\');
          }
        }
        tokadd(p, c);
      }
      continue;
    }
    else if ((c == '#') && (type & STR_FUNC_EXPAND)) {
      c = nextc(p);
      if (c == '{') {
        tokfix(p);
        p->lstate = EXPR_BEG;
        p->cmd_start = TRUE;
        pylval.nd = new_str_tok(p);
        if (hinfo) {
          if (unindent && head) {
            nspaces = push(nspaces, int_to_node(spaces));
            heredoc_push_indented(p, hinfo, pylval.nd, escaped, nspaces, FALSE);
          }
          hinfo->line_head = FALSE;
          return tHD_STRING_PART;
        }
        return tSTRING_PART;
      }
      tokadd(p, '#');
      pushback(p, c);
      continue;
    }
    if ((type & STR_FUNC_ARRAY) && ISSPACE(c)) {
      if (toklen(p) == 0) {
        do {
          if (c == '\n') {
            p->lineno++;
            p->column = 0;
            heredoc_treat_nextline(p);
            if (p->parsing_heredoc != NULL) {
              return tHD_LITERAL_DELIM;
            }
          }
          c = nextc(p);
        } while (ISSPACE(c));
        pushback(p, c);
        return tLITERAL_DELIM;
      }
      else {
        pushback(p, c);
        tokfix(p);
        pylval.nd = new_str_tok(p);
        return tSTRING_MID;
      }
    }
    if (c == '\n') {
      p->lineno++;
      p->column = 0;
    }
    tokadd(p, c);
  }

  tokfix(p);
  p->lstate = EXPR_END;
  end_strterm(p);

  if (type & STR_FUNC_XQUOTE) {
    pylval.nd = new_str_tok(p);
    return tXSTRING;
  }

  if (type & STR_FUNC_REGEXP) {
    int f = 0;
    int re_opt;
    int pattern_len = toklen(p);
    char *s = strndup(tok(p), pattern_len);
    char flags[3];
    char *flag = flags;
    char enc = '\0';
    char *encp;
    char *dup;

    newtok(p);
    while (re_opt = nextc(p), re_opt >= 0 && ISALPHA(re_opt)) {
      switch (re_opt) {
      case 'i': f |= 1; break;
      case 'x': f |= 2; break;
      case 'm': f |= 4; break;
      case 'u': f |= 16; break;
      case 'n': f |= 32; break;
      case 'o': break;
      default: tokadd(p, re_opt); break;
      }
    }
    pushback(p, re_opt);
    if (toklen(p)) {
      char msg[128];

      strcpy(msg, "unknown regexp option");
      tokfix(p);
      if (toklen(p) > 1) {
        strcat(msg, "s");
      }
      strcat(msg, " - ");
      strncat(msg, tok(p), sizeof(msg) - strlen(msg) - 1);
      yyerror(NULL, p, msg);
    }
    if (f != 0) {
      if (f & 1) *flag++ = 'i';
      if (f & 2) *flag++ = 'x';
      if (f & 4) *flag++ = 'm';
      if (f & 16) enc = 'u';
      if (f & 32) enc = 'n';
    }
    if (flag > flags) {
      dup = strndup(flags, (size_t)(flag - flags));
    }
    else {
      dup = NULL;
    }
    if (enc) {
      encp = strndup(&enc, 1);
    }
    else {
      encp = NULL;
    }
    pylval.nd = cons(cons(int_to_node(pattern_len), (node*)s), cons((node*)dup, (node*)encp));

    return tREGEXP;
  }
  pylval.nd = new_str_tok(p);

  return tSTRING;
}

static int
number_literal_suffix(parser_state *p)
{
  int c, result = 0;
  node *list = 0;
  int column = p->column;
  int mask = NUM_SUFFIX_R|NUM_SUFFIX_I;

  while ((c = nextc(p)) != -1) {
    list = push(list, int_to_node(c));

    if ((mask & NUM_SUFFIX_I) && c == 'i') {
      result |= (mask & NUM_SUFFIX_I);
      mask &= ~NUM_SUFFIX_I;
      /* r after i, rational of complex is disallowed */
      mask &= ~NUM_SUFFIX_R;
      continue;
    }
    if ((mask & NUM_SUFFIX_R) && c == 'r') {
      result |= (mask & NUM_SUFFIX_R);
      mask &= ~NUM_SUFFIX_R;
      continue;
    }
    if (!ISASCII(c) || ISALPHA(c) || c == '_') {
      p->column = column;
      if (p->pb) {
        p->pb = append(list, p->pb);
      }
      else {
        p->pb = list;
      }
      return 0;
    }
    pushback(p, c);
    break;
  }
  return result;
}

static int
heredoc_identifier(parser_state *p)
{
  int c;
  int type = str_heredoc;
  mrb_bool indent = FALSE;
  mrb_bool squiggly = FALSE;
  mrb_bool quote = FALSE;
  node *newnode;
  parser_heredoc_info *info;

  c = nextc(p);
  if (ISSPACE(c) || c == '=') {
    pushback(p, c);
    return 0;
  }
  if (c == '-' || c == '~') {
    if (c == '-')
      indent = TRUE;
    if (c == '~')
      squiggly = TRUE;
    c = nextc(p);
  }
  if (c == '\'' || c == '"') {
    int term = c;
    if (c == '\'')
      quote = TRUE;
    newtok(p);
    while ((c = nextc(p)) >= 0 && c != term) {
      if (c == '\n') {
        c = -1;
        break;
      }
      tokadd(p, c);
    }
    if (c < 0) {
      yyerror(NULL, p, "unterminated here document identifier");
      return 0;
    }
  }
  else {
    if (c < 0) {
      return 0;                 /* missing here document identifier */
    }
    if (! identchar(c)) {
      pushback(p, c);
      if (indent) pushback(p, '-');
      if (squiggly) pushback(p, '~');
      return 0;
    }
    newtok(p);
    do {
      tokadd(p, c);
    } while ((c = nextc(p)) >= 0 && identchar(c));
    pushback(p, c);
  }
  tokfix(p);
  newnode = new_heredoc(p, &info);
  info->term = strndup(tok(p), toklen(p));
  info->term_len = toklen(p);
  if (! quote)
    type |= STR_FUNC_EXPAND;
  info->type = (string_type)type;
  info->allow_indent = indent || squiggly;
  info->remove_indent = squiggly;
  info->indent = ~0U;
  info->indented = NULL;
  info->line_head = TRUE;
  info->doc = NULL;
  p->heredocs_from_nextline = push(p->heredocs_from_nextline, newnode);
  p->lstate = EXPR_END;

  pylval.nd = newnode;
  return tHEREDOC_BEG;
}

static int
arg_ambiguous(parser_state *p)
{
  yywarning(p, "ambiguous first argument; put parentheses or even spaces");
  return 1;
}

#include "lex.def"

static int
parser_yylex(parser_state *p)
{
  int32_t c;
  int nlines = 1;
  int space_seen = 0;
  int cmd_state;
  enum mrb_lex_state_enum last_state;
  int token_column;

  /* Early termination if too many errors - prevents DoS from malformed input */
  if (p->nerr > 10) {
    return 0;  /* EOF */
  }

  if (p->lex_strterm) {
    if (is_strterm_type(p, STR_FUNC_HEREDOC)) {
      if (p->parsing_heredoc != NULL)
        return parse_string(p);
    }
    else
      return parse_string(p);
  }
  cmd_state = p->cmd_start;
  p->cmd_start = FALSE;
  retry:
  last_state = p->lstate;
  switch (c = nextc(p)) {
  case '\004':  /* ^D */
  case '\032':  /* ^Z */
  case '\0':    /* NUL */
  case -1:      /* end of script. */
    if (p->heredocs_from_nextline)
      goto maybe_heredoc;
    return 0;

  /* white spaces */
  case ' ': case '\t': case '\f': case '\r':
  case '\13':   /* '\v' */
    space_seen = 1;
    goto retry;

  case '#':     /* it's a comment */
    comment_line(p);
    /* fall through */
  case -2:      /* end of a file */
  case '\n':
  maybe_heredoc:
    heredoc_treat_nextline(p);
    p->column = 0;
    switch (p->lstate) {
    case EXPR_BEG:
    case EXPR_FNAME:
    case EXPR_DOT:
    case EXPR_CLASS:
    case EXPR_VALUE:
      p->lineno++;
      if (p->parsing_heredoc != NULL) {
        if (p->lex_strterm) {
          return parse_string(p);
        }
      }
      goto retry;
    default:
      break;
    }
    if (p->parsing_heredoc != NULL) {
      pylval.num = nlines;
      return '\n';
    }
    while ((c = nextc(p))) {
      switch (c) {
      case ' ': case '\t': case '\f': case '\r':
      case '\13': /* '\v' */
        space_seen = 1;
        break;
      case '#': /* comment as a whitespace */
        comment_line(p);
        nlines++;
        break;
      case '.':
        if (!peek(p, '.')) {
          pushback(p, '.');
          p->lineno+=nlines; nlines=1;
          goto retry;
        }
        pushback(p, c);
        goto normal_newline;
      case '&':
        if (peek(p, '.')) {
          pushback(p, '&');
          p->lineno+=nlines; nlines=1;
          goto retry;
        }
        pushback(p, c);
        goto normal_newline;
      case -1:                  /* EOF */
      case -2:                  /* end of a file */
        goto normal_newline;
      default:
        pushback(p, c);
        goto normal_newline;
      }
    }
  normal_newline:
    p->cmd_start = TRUE;
    p->lstate = EXPR_BEG;
    pylval.num = nlines;
    return '\n';

  case '*':
    if ((c = nextc(p)) == '*') {
      if ((c = nextc(p)) == '=') {
        pylval.id = intern_op(pow);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      if (IS_SPCARG(c)) {
        yywarning(p, "'**' interpreted as argument prefix");
        c = tDSTAR;
      }
      else if (IS_BEG()) {
        c = tDSTAR;
      }
      else {
        c = tPOW; /* "**", "argument prefix" */
      }
    }
    else {
      if (c == '=') {
        pylval.id = intern_op(mul);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      if (IS_SPCARG(c)) {
        yywarning(p, "'*' interpreted as argument prefix");
        c = tSTAR;
      }
      else if (IS_BEG()) {
        c = tSTAR;
      }
      else {
        c = '*';
      }
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    return c;

  case '!':
    c = nextc(p);
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
      if (c == '@') {
        return '!';
      }
    }
    else {
      p->lstate = EXPR_BEG;
    }
    if (c == '=') {
      return tNEQ;
    }
    if (c == '~') {
      return tNMATCH;
    }
    pushback(p, c);
    return '!';

  case '=':
    if (p->column == 1) {
      static const char begin[] = "begin";
      static const char end[] = "\n=end";
      if (peeks(p, begin)) {
        c = peekc_n(p, sizeof(begin)-1);
        if (c < 0 || ISSPACE(c)) {
          do {
            if (!skips(p, end)) {
              yyerror(NULL, p, "embedded document meets end of file");
              return 0;
            }
            c = nextc(p);
          } while (!(c < 0 || ISSPACE(c)));
          if (c != '\n') skip(p, '\n');
          p->lineno+=nlines; nlines=1;
          p->column = 0;
          goto retry;
        }
      }
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    if ((c = nextc(p)) == '=') {
      if ((c = nextc(p)) == '=') {
        return tEQQ;
      }
      pushback(p, c);
      return tEQ;
    }
    if (c == '~') {
      return tMATCH;
    }
    else if (c == '>') {
      return tASSOC;
    }
    pushback(p, c);
    return '=';

  case '<':
    c = nextc(p);
    if (c == '<' &&
        p->lstate != EXPR_DOT &&
        p->lstate != EXPR_CLASS &&
        !IS_END() &&
        (!IS_ARG() || space_seen)) {
      int token = heredoc_identifier(p);
      if (token)
        return token;
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
      if (p->lstate == EXPR_CLASS) {
        p->cmd_start = TRUE;
      }
    }
    if (c == '=') {
      if ((c = nextc(p)) == '>') {
        return tCMP;
      }
      pushback(p, c);
      return tLEQ;
    }
    if (c == '<') {
      if ((c = nextc(p)) == '=') {
        pylval.id = intern_op(lshift);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      return tLSHFT;
    }
    pushback(p, c);
    return '<';

  case '>':
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    if ((c = nextc(p)) == '=') {
      return tGEQ;
    }
    if (c == '>') {
      if ((c = nextc(p)) == '=') {
        pylval.id = intern_op(rshift);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      return tRSHFT;
    }
    pushback(p, c);
    return '>';

  case '"':
    p->lex_strterm = new_strterm(p, str_dquote, '"', 0);
    return tSTRING_BEG;

  case '\'':
    p->lex_strterm = new_strterm(p, str_squote, '\'', 0);
    return parse_string(p);

  case '`':
    if (p->lstate == EXPR_FNAME) {
      p->lstate = EXPR_ENDFN;
      return '`';
    }
    if (p->lstate == EXPR_DOT) {
      if (cmd_state)
        p->lstate = EXPR_CMDARG;
      else
        p->lstate = EXPR_ARG;
      return '`';
    }
    p->lex_strterm = new_strterm(p, str_xquote, '`', 0);
    return tXSTRING_BEG;

  case '?':
    if (IS_END()) {
      p->lstate = EXPR_VALUE;
      return '?';
    }
    c = nextc(p);
    if (c < 0) {
      yyerror(NULL, p, "incomplete character syntax");
      return 0;
    }
    if (ISSPACE(c)) {
      if (!IS_ARG()) {
        int c2;
        switch (c) {
        case ' ':
          c2 = 's';
          break;
        case '\n':
          c2 = 'n';
          break;
        case '\t':
          c2 = 't';
          break;
        case '\v':
          c2 = 'v';
          break;
        case '\r':
          c2 = 'r';
          break;
        case '\f':
          c2 = 'f';
          break;
        default:
          c2 = 0;
          break;
        }
        if (c2) {
          char buf[256];
          char cc[] = { (char)c2, '\0' };

          strcpy(buf, "invalid character syntax; use ?\\");
          strncat(buf, cc, 2);
          yyerror(NULL, p, buf);
        }
      }
      ternary:
      pushback(p, c);
      p->lstate = EXPR_VALUE;
      return '?';
    }
    newtok(p);
    /* need support UTF-8 if configured */
    if ((ISALNUM(c) || c == '_')) {
      int c2 = nextc(p);
      pushback(p, c2);
      if ((ISALNUM(c2) || c2 == '_')) {
        goto ternary;
      }
    }
    if (c == '\\') {
      c = read_escape(p);
      tokadd(p, c);
    }
    else {
      tokadd(p, c);
    }
    tokfix(p);
    pylval.nd = new_str_tok(p);
    p->lstate = EXPR_END;
    return tCHAR;

  case '&':
    if ((c = nextc(p)) == '&') {
      p->lstate = EXPR_BEG;
      if ((c = nextc(p)) == '=') {
        pylval.id = intern_op(andand);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      return tANDOP;
    }
    else if (c == '.') {
      p->lstate = EXPR_DOT;
      return tANDDOT;
    }
    else if (c == '=') {
      pylval.id = intern_op(and);
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    pushback(p, c);
    if (IS_SPCARG(c)) {
      yywarning(p, "'&' interpreted as argument prefix");
      c = tAMPER;
    }
    else if (IS_BEG()) {
      c = tAMPER;
    }
    else {
      c = '&';
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    return c;

  case '|':
    if ((c = nextc(p)) == '|') {
      p->lstate = EXPR_BEG;
      if ((c = nextc(p)) == '=') {
        pylval.id = intern_op(oror);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      return tOROP;
    }
    if (c == '=') {
      pylval.id = intern_op(or);
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    pushback(p, c);
    return '|';

  case '+':
    c = nextc(p);
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
      if (c == '@') {
        return tUPLUS;
      }
      pushback(p, c);
      return '+';
    }
    if (c == '=') {
      pylval.id = intern_op(add);
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    if (IS_BEG() || (IS_SPCARG(c) && arg_ambiguous(p))) {
      p->lstate = EXPR_BEG;
      pushback(p, c);
      if (c >= 0 && ISDIGIT(c)) {
        c = '+';
        goto start_num;
      }
      return tUPLUS;
    }
    p->lstate = EXPR_BEG;
    pushback(p, c);
    return '+';

  case '-':
    c = nextc(p);
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
      if (c == '@') {
        return tUMINUS;
      }
      pushback(p, c);
      return '-';
    }
    if (c == '=') {
      pylval.id = intern_op(sub);
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    if (c == '>') {
      p->lstate = EXPR_ENDFN;
      return tLAMBDA;
    }
    if (IS_BEG() || (IS_SPCARG(c) && arg_ambiguous(p))) {
      p->lstate = EXPR_BEG;
      pushback(p, c);
      if (c >= 0 && ISDIGIT(c)) {
        return tUMINUS_NUM;
      }
      return tUMINUS;
    }
    p->lstate = EXPR_BEG;
    pushback(p, c);
    return '-';

  case '.':
    {
      int is_beg = IS_BEG();
      p->lstate = EXPR_MID;
      if ((c = nextc(p)) == '.') {
        if ((c = nextc(p)) == '.') {
          return is_beg ? tBDOT3 : tDOT3;
        }
        pushback(p, c);
        return is_beg ? tBDOT2 : tDOT2;
      }
      pushback(p, c);
      p->lstate = EXPR_BEG;
      if (c >= 0 && ISDIGIT(c)) {
        yyerror(NULL, p, "no .<digit> floating literal anymore; put 0 before dot");
      }
      p->lstate = EXPR_DOT;
      return '.';
    }

    start_num:
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
  {
    int is_float, seen_point, seen_e, nondigit;
    int suffix = 0;

    is_float = seen_point = seen_e = nondigit = 0;
    p->lstate = EXPR_END;
    newtok(p);
    if (c == '-') {
      tokadd(p, c);
      c = nextc(p);
    }
    else if (c == '+') {
      c = nextc(p);
    }
    if (c == '0') {
#define no_digits() do {yyerror(NULL, p,"numeric literal without digits"); return 0;} while (0)
      int start = toklen(p);
      c = nextc(p);
      if (c == 'x' || c == 'X') {
        /* hexadecimal */
        c = nextc(p);
        if (c >= 0 && ISXDIGIT(c)) {
          do {
            if (c == '_') {
              if (nondigit) break;
              nondigit = c;
              continue;
            }
            if (!ISXDIGIT(c)) break;
            nondigit = 0;
            tokadd(p, tolower(c));
          } while ((c = nextc(p)) >= 0);
        }
        pushback(p, c);
        tokfix(p);
        if (toklen(p) == start) {
          no_digits();
        }
        else if (nondigit) goto trailing_uc;
        suffix = number_literal_suffix(p);
        pylval.nd = new_int(p, tok(p), 16, suffix);
        return tINTEGER;
      }
      if (c == 'b' || c == 'B') {
        /* binary */
        c = nextc(p);
        if (c == '0' || c == '1') {
          do {
            if (c == '_') {
              if (nondigit) break;
              nondigit = c;
              continue;
            }
            if (c != '0' && c != '1') break;
            nondigit = 0;
            tokadd(p, c);
          } while ((c = nextc(p)) >= 0);
        }
        pushback(p, c);
        tokfix(p);
        if (toklen(p) == start) {
          no_digits();
        }
        else if (nondigit) goto trailing_uc;
        suffix = number_literal_suffix(p);
        pylval.nd = new_int(p, tok(p), 2, suffix);
        return tINTEGER;
      }
      if (c == 'd' || c == 'D') {
        /* decimal */
        c = nextc(p);
        if (c >= 0 && ISDIGIT(c)) {
          do {
            if (c == '_') {
              if (nondigit) break;
              nondigit = c;
              continue;
            }
            if (!ISDIGIT(c)) break;
            nondigit = 0;
            tokadd(p, c);
          } while ((c = nextc(p)) >= 0);
        }
        pushback(p, c);
        tokfix(p);
        if (toklen(p) == start) {
          no_digits();
        }
        else if (nondigit) goto trailing_uc;
        suffix = number_literal_suffix(p);
        pylval.nd = new_int(p, tok(p), 10, suffix);
        return tINTEGER;
      }
      if (c == '_') {
        /* 0_0 */
        goto octal_number;
      }
      if (c == 'o' || c == 'O') {
        /* prefixed octal */
        c = nextc(p);
        if (c < 0 || c == '_' || !ISDIGIT(c)) {
          no_digits();
        }
      }
      if (c >= '0' && c <= '7') {
        /* octal */
        octal_number:
        do {
          if (c == '_') {
            if (nondigit) break;
            nondigit = c;
            continue;
          }
          if (c < '0' || c > '9') break;
          if (c > '7') goto invalid_octal;
          nondigit = 0;
          tokadd(p, c);
        } while ((c = nextc(p)) >= 0);

        if (toklen(p) > start) {
          pushback(p, c);
          tokfix(p);
          if (nondigit) goto trailing_uc;
          suffix = number_literal_suffix(p);
          pylval.nd = new_int(p, tok(p), 8, suffix);
          return tINTEGER;
        }
        if (nondigit) {
          pushback(p, c);
          goto trailing_uc;
        }
      }
      if (c > '7' && c <= '9') {
        invalid_octal:
        yyerror(NULL, p, "Invalid octal digit");
      }
      else if (c == '.' || c == 'e' || c == 'E') {
        tokadd(p, '0');
      }
      else {
        pushback(p, c);
        suffix = number_literal_suffix(p);
        pylval.nd = new_int(p, "0", 10, suffix);
        return tINTEGER;
      }
    }

    for (;;) {
      switch (c) {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        nondigit = 0;
        tokadd(p, c);
        break;

      case '.':
        if (nondigit) goto trailing_uc;
        if (seen_point || seen_e) {
          goto decode_num;
        }
        else {
          int c0 = nextc(p);
          if (c0 < 0 || !ISDIGIT(c0)) {
            pushback(p, c0);
            goto decode_num;
          }
          c = c0;
        }
        tokadd(p, '.');
        tokadd(p, c);
        is_float++;
        seen_point++;
        nondigit = 0;
        break;

      case 'e':
      case 'E':
        if (nondigit) {
          pushback(p, c);
          c = nondigit;
          goto decode_num;
        }
        if (seen_e) {
          goto decode_num;
        }
        tokadd(p, c);
        seen_e++;
        is_float++;
        nondigit = c;
        c = nextc(p);
        if (c != '-' && c != '+') continue;
        tokadd(p, c);
        nondigit = c;
        break;

      case '_':       /* '_' in number just ignored */
        if (nondigit) goto decode_num;
        nondigit = c;
        break;

      default:
        goto decode_num;
      }
      c = nextc(p);
    }

    decode_num:
    pushback(p, c);
    if (nondigit) {
      trailing_uc:
      yyerror_c(p, "trailing non digit in number: ", (char)nondigit);
    }
    tokfix(p);
    if (is_float) {
#ifdef MRB_NO_FLOAT
      yywarning_s(p, "floating-point numbers are not supported", tok(p));
      pylval.nd = new_int(p, "0", 10, 0);
      return tINTEGER;
#else
      double d;

      if (!mrb_read_float(tok(p), NULL, &d)) {
        yywarning_s(p, "corrupted float value", tok(p));
      }
      suffix = number_literal_suffix(p);
      if (seen_e && (suffix & NUM_SUFFIX_R)) {
        pushback(p, 'r');
        suffix &= ~NUM_SUFFIX_R;
      }
      pylval.nd = new_float(p, tok(p), suffix);
      return tFLOAT;
#endif
    }
    suffix = number_literal_suffix(p);
    pylval.nd = new_int(p, tok(p), 10, suffix);
    return tINTEGER;
  }

  case ')':
  case ']':
    p->paren_nest--;
    /* fall through */
  case '}':
    COND_LEXPOP();
    CMDARG_LEXPOP();
    if (c == ')')
      p->lstate = EXPR_ENDFN;
    else
      p->lstate = EXPR_END;
    return c;

  case ':':
    c = nextc(p);
    if (c == ':') {
      if (IS_BEG() || p->lstate == EXPR_CLASS || IS_SPCARG(-1)) {
        p->lstate = EXPR_BEG;
        return tCOLON3;
      }
      p->lstate = EXPR_DOT;
      return tCOLON2;
    }
    if (!space_seen && IS_END()) {
      pushback(p, c);
      /* In pattern matching context, use EXPR_ARG so newlines are significant */
      p->lstate = p->in_kwarg ? EXPR_ARG : EXPR_BEG;
      return tLABEL_TAG;
    }
    if (IS_END() || ISSPACE(c) || c == '#') {
      pushback(p, c);
      p->lstate = EXPR_BEG;
      return ':';
    }
    pushback(p, c);
    p->lstate = EXPR_FNAME;
    return tSYMBEG;

  case '/':
    if (IS_BEG()) {
      p->lex_strterm = new_strterm(p, str_regexp, '/', 0);
      return tREGEXP_BEG;
    }
    if ((c = nextc(p)) == '=') {
      pylval.id = intern_op(div);
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    pushback(p, c);
    if (IS_SPCARG(c)) {
      p->lex_strterm = new_strterm(p, str_regexp, '/', 0);
      return tREGEXP_BEG;
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    return '/';

  case '^':
    if ((c = nextc(p)) == '=') {
      pylval.id = intern_op(xor);
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    pushback(p, c);
    return '^';

  case ';':
    p->lstate = EXPR_BEG;
    return ';';

  case ',':
    p->lstate = EXPR_BEG;
    return ',';

  case '~':
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      if ((c = nextc(p)) != '@') {
        pushback(p, c);
      }
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    return '~';

  case '(':
    if (IS_BEG()) {
      c = tLPAREN;
    }
    else if (IS_SPCARG(-1)) {
      c = tLPAREN_ARG;
    }
    else if (p->lstate == EXPR_END && space_seen) {
      c = tLPAREN_ARG;
    }
    p->paren_nest++;
    COND_PUSH(0);
    CMDARG_PUSH(0);
    p->lstate = EXPR_BEG;
    return c;

  case '[':
    p->paren_nest++;
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
      p->paren_nest--;
      if ((c = nextc(p)) == ']') {
        if ((c = nextc(p)) == '=') {
          return tASET;
        }
        pushback(p, c);
        return tAREF;
      }
      pushback(p, c);
      return '[';
    }
    else if (IS_BEG()) {
      c = tLBRACK;
    }
    else if (IS_ARG() && space_seen) {
      c = tLBRACK;
    }
    p->lstate = EXPR_BEG;
    COND_PUSH(0);
    CMDARG_PUSH(0);
    return c;

  case '{':
    if (p->lpar_beg && p->lpar_beg == p->paren_nest) {
      p->lstate = EXPR_BEG;
      p->lpar_beg = 0;
      p->paren_nest--;
      COND_PUSH(0);
      CMDARG_PUSH(0);
      return tLAMBEG;
    }
    if (IS_ARG() || p->lstate == EXPR_END || p->lstate == EXPR_ENDFN)
      c = '{';          /* block (primary) */
    else if (p->lstate == EXPR_ENDARG)
      c = tLBRACE_ARG;  /* block (expr) */
    else
      c = tLBRACE;      /* hash */
    COND_PUSH(0);
    CMDARG_PUSH(0);
    p->lstate = EXPR_BEG;
    return c;

  case '\\':
    c = nextc(p);
    if (c == '\n') {
      p->lineno+=nlines; nlines=1;
      p->column = 0;
      space_seen = 1;
      goto retry; /* skip \\n */
    }
    pushback(p, c);
    return '\\';

  case '%':
    if (IS_BEG()) {
      int term;
      int paren;

      c = nextc(p);
      quotation:
      if (c < 0 || !ISALNUM(c)) {
        term = c;
        c = 'Q';
      }
      else {
        term = nextc(p);
        if (ISALNUM(term)) {
          yyerror(NULL, p, "unknown type of %string");
          return 0;
        }
      }
      if (c < 0 || term < 0) {
        yyerror(NULL, p, "unterminated quoted string meets end of file");
        return 0;
      }
      paren = term;
      if (term == '(') term = ')';
      else if (term == '[') term = ']';
      else if (term == '{') term = '}';
      else if (term == '<') term = '>';
      else paren = 0;

      switch (c) {
      case 'Q':
        p->lex_strterm = new_strterm(p, str_dquote, term, paren);
        return tSTRING_BEG;

      case 'q':
        p->lex_strterm = new_strterm(p, str_squote, term, paren);
        return parse_string(p);

      case 'W':
        p->lex_strterm = new_strterm(p, str_dword, term, paren);
        return tWORDS_BEG;

      case 'w':
        p->lex_strterm = new_strterm(p, str_sword, term, paren);
        return tWORDS_BEG;

      case 'x':
        p->lex_strterm = new_strterm(p, str_xquote, term, paren);
        return tXSTRING_BEG;

      case 'r':
        p->lex_strterm = new_strterm(p, str_regexp, term, paren);
        return tREGEXP_BEG;

      case 's':
        p->lex_strterm = new_strterm(p, str_ssym, term, paren);
        return tSYMBEG;

      case 'I':
        p->lex_strterm = new_strterm(p, str_dsymbols, term, paren);
        return tSYMBOLS_BEG;

      case 'i':
        p->lex_strterm = new_strterm(p, str_ssymbols, term, paren);
        return tSYMBOLS_BEG;

      default:
        yyerror(NULL, p, "unknown type of %string");
        return 0;
      }
    }
    if ((c = nextc(p)) == '=') {
      pylval.id = intern_op(mod);
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    if (IS_SPCARG(c)) {
      goto quotation;
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    pushback(p, c);
    return '%';

  case '$':
    p->lstate = EXPR_END;
    token_column = newtok(p);
    c = nextc(p);
    if (c < 0) {
      yyerror(NULL, p, "incomplete global variable syntax");
      return 0;
    }
    switch (c) {
    case '_':     /* $_: last read line string */
      c = nextc(p);
      if (c >= 0 && identchar(c)) { /* if there is more after _ it is a variable */
        tokadd(p, '$');
        tokadd(p, c);
        break;
      }
      pushback(p, c);
      c = '_';
      /* fall through */
    case '~':     /* $~: match-data */
    case '*':     /* $*: argv */
    case '$':     /* $$: pid */
    case '?':     /* $?: last status */
    case '!':     /* $!: error string */
    case '@':     /* $@: error position */
    case '/':     /* $/: input record separator */
    case '\\':    /* $\: output record separator */
    case ';':     /* $;: field separator */
    case ',':     /* $,: output field separator */
    case '.':     /* $.: last read line number */
    case '=':     /* $=: ignorecase */
    case ':':     /* $:: load path */
    case '<':     /* $<: reading filename */
    case '>':     /* $>: default output handle */
    case '\"':    /* $": already loaded files */
      tokadd(p, '$');
      tokadd(p, c);
      tokfix(p);
      pylval.id = intern(tok(p), toklen(p));
      return tGVAR;

    case '-':
      tokadd(p, '$');
      tokadd(p, c);
      c = nextc(p);
      pushback(p, c);
      gvar:
      tokfix(p);
      pylval.id = intern(tok(p), toklen(p));
      return tGVAR;

    case '&':     /* $&: last match */
    case '`':     /* $`: string before last match */
    case '\'':    /* $': string after last match */
    case '+':     /* $+: string matches last pattern */
      if (last_state == EXPR_FNAME) {
        tokadd(p, '$');
        tokadd(p, c);
        goto gvar;
      }
      pylval.nd = new_back_ref(p, c);
      return tBACK_REF;

    case '1': case '2': case '3':
    case '4': case '5': case '6':
    case '7': case '8': case '9':
      do {
        tokadd(p, c);
        c = nextc(p);
      } while (c >= 0 && ISDIGIT(c));
      pushback(p, c);
      if (last_state == EXPR_FNAME) goto gvar;
      tokfix(p);
      {
        mrb_int n;
        if (!mrb_read_int(tok(p), NULL, NULL, &n)) {
          yywarning(p, "capture group index too big; always nil");
          return keyword_nil;
        }
        pylval.nd = new_nth_ref(p, (int)n);
      }
      return tNTH_REF;

    default:
      if (!identchar(c)) {
        pushback(p,  c);
        return '$';
      }
      /* fall through */
    case '0':
      tokadd(p, '$');
    }
    break;

    case '@':
      c = nextc(p);
      token_column = newtok(p);
      tokadd(p, '@');
      if (c == '@') {
        tokadd(p, '@');
        c = nextc(p);
      }
      if (c < 0) {
        if (p->tidx == 1) {
          yyerror(NULL, p, "incomplete instance variable syntax");
        }
        else {
          yyerror(NULL, p, "incomplete class variable syntax");
        }
        return 0;
      }
      else if (ISDIGIT(c)) {
        if (p->tidx == 1) {
          yyerror_c(p, "wrong instance variable name: @", c);
        }
        else {
          yyerror_c(p, "wrong class variable name: @@", c);
        }
        return 0;
      }
      if (!identchar(c)) {
        pushback(p, c);
        return '@';
      }
      break;

    case '_':
      token_column = newtok(p);
      break;

    default:
      if (!identchar(c)) {
        char buf[36];
        const char s[] = "Invalid char in expression: 0x";
        const char hexdigits[] = "0123456789ABCDEF";

        strcpy(buf, s);
        buf[sizeof(s)-1] = hexdigits[(c & 0xf0) >> 4];
        buf[sizeof(s)]   = hexdigits[(c & 0x0f)];
        buf[sizeof(s)+1] = 0;
        yyerror(NULL, p, buf);
        goto retry;
      }

      token_column = newtok(p);
      break;
  }

  do {
    tokadd(p, c);
    c = nextc(p);
    if (c < 0) break;
  } while (identchar(c));
  if (token_column == 0 && toklen(p) == 7 && (c < 0 || c == '\n') &&
      strncmp(tok(p), "__END__", toklen(p)) == 0)
    return -1;

  switch (tok(p)[0]) {
  case '@': case '$':
    pushback(p, c);
    break;
  default:
    if ((c == '!' || c == '?') && !peek(p, '=')) {
      tokadd(p, c);
    }
    else {
      pushback(p, c);
    }
  }
  tokfix(p);
  {
    int result = 0;

    switch (tok(p)[0]) {
    case '$':
      p->lstate = EXPR_END;
      result = tGVAR;
      break;
    case '@':
      p->lstate = EXPR_END;
      if (tok(p)[1] == '@')
        result = tCVAR;
      else
        result = tIVAR;
      break;

    case '_':
      if (toklen(p) == 2 && ISDIGIT(tok(p)[1]) && p->nvars) {
        int n = tok(p)[1] - '0';
        int nvar;

        if (n > 0) {
          nvar = node_to_int(p->nvars->car);
          if (nvar != -2) {     /* numbered parameters never appear on toplevel */
            pylval.num = n;
            p->lstate = EXPR_END;
            return tNUMPARAM;
          }
        }
      }
      /* fall through */
    default:
      if (toklast(p) == '!' || toklast(p) == '?') {
        result = tFID;
      }
      else {
        if (p->lstate == EXPR_FNAME) {
          if ((c = nextc(p)) == '=' && !peek(p, '~') && !peek(p, '>') &&
              (!peek(p, '=') || (peek_n(p, '>', 1)))) {
            result = tIDENTIFIER;
            tokadd(p, c);
            tokfix(p);
          }
          else {
            pushback(p, c);
          }
          if ((c = nextc(p)) == '=' && !peek(p, '~') && !peek(p, '>') &&
              (!peek(p, '=') || (peek_n(p, '>', 1)))) {
            result = tIDENTIFIER;
            tokadd(p, c);
            tokfix(p);
          }
          else {
            pushback(p, c);
          }
        }
        if (result == 0 && ISUPPER(tok(p)[0])) {
          result = tCONSTANT;
        }
        else {
          result = tIDENTIFIER;
        }
      }

      if (IS_LABEL_POSSIBLE()) {
        if (IS_LABEL_SUFFIX(0)) {
          p->lstate = EXPR_END;
          tokfix(p);
          pylval.id = intern(tok(p), toklen(p));
          return tIDENTIFIER;
        }
      }
      if (p->lstate != EXPR_DOT) {
        const struct kwtable *kw;

        /* See if it is a reserved word.  */
        kw = mrb_reserved_word(tok(p), toklen(p));
        if (kw) {
          enum mrb_lex_state_enum state = p->lstate;
          pylval.num = p->lineno;
          p->lstate = kw->state;
          if (state == EXPR_FNAME) {
            pylval.id = intern_cstr(kw->name);
            return kw->id[0];
          }
          if (p->lstate == EXPR_BEG) {
            p->cmd_start = TRUE;
          }
          if (kw->id[0] == keyword_do) {
            if (p->lpar_beg && p->lpar_beg == p->paren_nest) {
              p->lpar_beg = 0;
              p->paren_nest--;
              return keyword_do_LAMBDA;
            }
            if (COND_P()) return keyword_do_cond;
            if (CMDARG_P() && state != EXPR_CMDARG)
              return keyword_do_block;
            if (state == EXPR_ENDARG || state == EXPR_BEG)
              return keyword_do_block;
            return keyword_do;
          }
          if (kw->id[0] == keyword_in) {
            /* Set in_kwarg for pattern matching context */
            p->in_kwarg++;
          }
          if (state == EXPR_BEG || state == EXPR_VALUE || state == EXPR_CLASS)
            return kw->id[0];
          else {
            if (kw->id[0] != kw->id[1])
              p->lstate = EXPR_BEG;
            return kw->id[1];
          }
        }
      }

      if (IS_BEG() || p->lstate == EXPR_DOT || IS_ARG()) {
        if (cmd_state) {
          p->lstate = EXPR_CMDARG;
        }
        else {
          p->lstate = EXPR_ARG;
        }
      }
      else if (p->lstate == EXPR_FNAME) {
        p->lstate = EXPR_ENDFN;
      }
      else {
        p->lstate = EXPR_END;
      }
    }
    {
      mrb_sym ident = intern(tok(p), toklen(p));

      pylval.id = ident;
      if (last_state != EXPR_DOT && ISLOWER(tok(p)[0]) && local_var_p(p, ident)) {
        p->lstate = EXPR_END;
      }
    }
    return result;
  }
}

static int
yylex(void *lval, void *lp, parser_state *p)
{
  int t;

  p->ylval = lval;
  t = parser_yylex(p);
  p->token_seen = TRUE;  /* comments lexed from here on no longer carry the magic (ADR 0546) */
  return t;
}

static void
parser_init_cxt(parser_state *p, mrb_ccontext *cxt)
{
  if (!cxt) return;
  if (cxt->filename) mrb_parser_set_filename(p, cxt->filename);
  if (cxt->lineno) p->lineno = cxt->lineno;
  if (cxt->syms) {
    int i;

    p->locals = cons(0,0);
    for (i=0; i<cxt->slen; i++) {
      local_add_f(p, cxt->syms[i]);
    }
  }
  p->capture_errors = cxt->capture_errors;
  p->no_optimize = cxt->no_optimize;
  p->no_ext_ops = cxt->no_ext_ops;
  p->no_return_value = cxt->no_return_value;
  p->upper = cxt->upper;
  if (cxt->partial_hook) {
    p->cxt = cxt;
  }
}

static void
parser_update_cxt(parser_state *p, mrb_ccontext *cxt)
{
  node *n, *n0;
  int i = 0;

  if (!cxt) return;
  if (!p->tree) return;
  if (!node_type_p(p->tree, NODE_SCOPE)) return;

  /* Extract locals from variable-sized NODE_SCOPE */
  struct mrb_ast_scope_node *scope = scope_node(p->tree);
  n0 = n = scope->locals;
  while (n) {
    i++;
    n = n->cdr;
  }
  cxt->syms = (mrb_sym*)mrbc_realloc(cxt->syms, i*sizeof(mrb_sym));
  cxt->slen = i;
  for (i=0, n=n0; n; i++,n=n->cdr) {
    cxt->syms[i] = node_to_sym(n->car);
  }
}

static void dump_node(mrb_state *mrb, node *tree, int offset);

MRB_API void
mrb_parser_parse(parser_state *p, mrb_ccontext *c)
{
  struct mrb_jmpbuf buf1;
  struct mrb_jmpbuf *prev = p->mrb->jmp;
  p->mrb->jmp = &buf1;

  MRB_TRY(p->mrb->jmp) {
    int n = 1;

    p->cmd_start = TRUE;
    p->in_def = p->in_single = 0;
    p->nerr = p->nwarn = 0;
    p->lex_strterm = NULL;
    parser_init_cxt(p, c);

    n = yyparse(p);
    if (n != 0 || p->nerr > 0) {
      p->tree = 0;
      p->mrb->jmp = prev;
      return;
    }
    parser_update_cxt(p, c);
    if (c && c->dump_result) {
      dump_node(p->mrb, p->tree, 0);
    }
  }
  MRB_CATCH(p->mrb->jmp) {
    p->nerr++;
    if (p->mrb->exc == NULL) {
      yyerror(NULL, p, "memory allocation error");
      p->nerr++;
      p->tree = 0;
    }
  }
  MRB_END_EXC(p->jmp);
  p->mrb->jmp = prev;
}

MRB_API parser_state*
mrb_parser_new(mrb_state *mrb)
{
  mempool *pool;
  parser_state *p;
  static const parser_state parser_state_zero = { 0 };

  pool = mempool_open();
  if (!pool) return NULL;
  p = (parser_state*)mempool_alloc(pool, sizeof(parser_state));
  if (!p) return NULL;

  *p = parser_state_zero;
  p->mrb = mrb;
  p->pool = pool;

  p->s = p->send = NULL;
#ifndef MRB_NO_STDIO
  p->f = NULL;
#endif

  p->cmd_start = TRUE;
  p->in_def = p->in_single = 0;

  p->capture_errors = FALSE;
  p->lineno = 1;
  p->column = 0;
#if defined(PARSER_TEST) || defined(PARSER_DEBUG)
  yydebug = 1;
#endif
  p->tsiz = MRB_PARSER_TOKBUF_SIZE;
  p->tokbuf = p->buf;

  p->lex_strterm = NULL;

  p->current_filename_index = -1;
  p->filename_table = NULL;
  p->filename_table_length = 0;

  return p;
}

MRB_API void
mrb_parser_free(parser_state *p) {
  if (p->tokbuf != p->buf) {
    mrbc_free(p->tokbuf);
  }
  mempool_close(p->pool);
}

MRB_API mrb_ccontext*
mrb_ccontext_new(mrb_state *mrb)
{
  static const mrb_ccontext cc_zero = { 0 };
  mrb_ccontext *cc = (mrb_ccontext*)mrbc_malloc(sizeof(mrb_ccontext));
  *cc = cc_zero;
  return cc;
}

MRB_API void
mrb_ccontext_free(mrb_state *mrb, mrb_ccontext *cxt)
{
  mrbc_free(cxt->filename);
  mrbc_free(cxt->syms);
  mrbc_free(cxt);
}

MRB_API const char*
mrb_ccontext_filename(mrb_state *mrb, mrb_ccontext *c, const char *s)
{
  if (s) {
    size_t len = strlen(s);
    char *p = (char*)mrbc_malloc(len + 1);

    if (p == NULL) return NULL;
    memcpy(p, s, len + 1);
    if (c->filename) {
      mrbc_free(c->filename);
    }
    c->filename = p;
  }
  return c->filename;
}

MRB_API void
mrb_ccontext_partial_hook(mrb_ccontext *c, int (*func)(struct mrb_parser_state*), void *data)
{
  c->partial_hook = func;
  c->partial_data = data;
}

MRB_API void
mrb_ccontext_cleanup_local_variables(mrb_ccontext *c)
{
  if (c->syms) {
    mrbc_free(c->syms);
    c->syms = NULL;
    c->slen = 0;
  }
  c->keep_lv = FALSE;
}

MRB_API void
mrb_parser_set_filename(struct mrb_parser_state *p, const char *f)
{
  mrb_sym sym;
  uint16_t i;
  mrb_sym* new_table;

  sym = mrb_intern_cstr(p->mrb, f);
  p->filename_sym = sym;
  p->lineno = (p->filename_table_length > 0)? 0 : 1;

  for (i = 0; i < p->filename_table_length; i++) {
    if (p->filename_table[i] == sym) {
      p->current_filename_index = i;
      return;
    }
  }

  if (p->filename_table_length == UINT16_MAX) {
    yyerror(NULL, p, "too many files to compile");
    return;
  }
  p->current_filename_index = p->filename_table_length++;

  new_table = (mrb_sym*)parser_palloc(p, sizeof(mrb_sym) * p->filename_table_length);
  if (p->filename_table) {
    memmove(new_table, p->filename_table, sizeof(mrb_sym) * p->current_filename_index);
  }
  p->filename_table = new_table;
  p->filename_table[p->filename_table_length - 1] = sym;
}

MRB_API mrb_sym
mrb_parser_get_filename(struct mrb_parser_state* p, uint16_t idx) {
  if (idx >= p->filename_table_length) return 0;
  else {
    return p->filename_table[idx];
  }
}

#ifndef MRB_NO_STDIO
static struct mrb_parser_state *
mrb_parse_file_continue(mrb_state *mrb, FILE *f, const void *prebuf, size_t prebufsize, mrb_ccontext *c)
{
  parser_state *p;

  p = mrb_parser_new(mrb);
  if (!p) return NULL;
  if (prebuf) {
    p->s = (const char*)prebuf;
    p->send = (const char*)prebuf + prebufsize;
  }
  else {
    p->s = p->send = NULL;
  }
  p->f = f;

  mrb_parser_parse(p, c);
  return p;
}

MRB_API parser_state*
mrb_parse_file(mrb_state *mrb, FILE *f, mrb_ccontext *c)
{
  return mrb_parse_file_continue(mrb, f, NULL, 0, c);
}
#endif

MRB_API parser_state*
mrb_parse_nstring(mrb_state *mrb, const char *s, size_t len, mrb_ccontext *c)
{
  parser_state *p;

  p = mrb_parser_new(mrb);
  if (!p) return NULL;
  p->s = s;
  p->send = s + len;

  mrb_parser_parse(p, c);
  return p;
}

MRB_API parser_state*
mrb_parse_string(mrb_state *mrb, const char *s, mrb_ccontext *c)
{
  return mrb_parse_nstring(mrb, s, strlen(s), c);
}

MRB_API mrb_value
mrb_load_exec(mrb_state *mrb, struct mrb_parser_state *p, mrb_ccontext *c)
{
  struct RClass *target = mrb->object_class;
  struct RProc *proc;
  mrb_value v;
  mrb_int keep = 0;

  if (!p) {
    return mrb_undef_value();
  }
  if (!p->tree || p->nerr) {
    if (c) c->parser_nerr = p->nerr;
    if (p->capture_errors) {
      char buf[256];

      strcpy(buf, "line ");
      dump_int(p->error_buffer[0].lineno, buf+5);
      strcat(buf, ": ");
      strncat(buf, p->error_buffer[0].message, sizeof(buf) - strlen(buf) - 1);
      mrb->exc = mrb_obj_ptr(mrb_exc_new(mrb, E_SYNTAX_ERROR, buf, strlen(buf)));
      mrb_parser_free(p);
      return mrb_undef_value();
    }
    else {
      if (mrb->exc == NULL) {
        mrb->exc = mrb_obj_ptr(mrb_exc_new_lit(mrb, E_SYNTAX_ERROR, "syntax error"));
      }
      mrb_parser_free(p);
      return mrb_undef_value();
    }
  }
  proc = mrb_generate_code(mrb, p);
  mrb_parser_free(p);
  if (proc == NULL) {
    if (mrb->exc == NULL) {
      mrb->exc = mrb_obj_ptr(mrb_exc_new_lit(mrb, E_SCRIPT_ERROR, "codegen error"));
    }
    return mrb_undef_value();
  }
  if (c) {
    if (c->dump_result) mrb_codedump_all(mrb, proc);
    if (c->no_exec) return mrb_obj_value(proc);
    if (c->target_class) {
      target = c->target_class;
    }
    if (c->keep_lv) {
      keep = c->slen + 1;
    }
    else {
      c->keep_lv = TRUE;
    }
  }
  MRB_PROC_SET_TARGET_CLASS(proc, target);
  if (mrb->c->ci) {
    mrb_vm_ci_target_class_set(mrb->c->ci, target);
  }
  v = mrb_top_run(mrb, proc, mrb_top_self(mrb), keep);
  if (mrb->exc) return mrb_nil_value();
  return v;
}

#ifndef MRB_NO_STDIO
MRB_API mrb_value
mrb_load_file_cxt(mrb_state *mrb, FILE *f, mrb_ccontext *c)
{
  return mrb_load_exec(mrb, mrb_parse_file(mrb, f, c), c);
}

MRB_API mrb_value
mrb_load_file(mrb_state *mrb, FILE *f)
{
  return mrb_load_file_cxt(mrb, f, NULL);
}

#define DETECT_SIZE 64

/*
 * In order to be recognized as a `.mrb` file, the following three points must be satisfied:
 * - File starts with "RITE"
 * - At least `sizeof(struct rite_binary_header)` bytes can be read
 * - `NUL` is included in the first 64 bytes of the file
 */
MRB_API mrb_value
mrb_load_detect_file_cxt(mrb_state *mrb, FILE *fp, mrb_ccontext *c)
{
  union {
    char b[DETECT_SIZE];
    struct rite_binary_header h;
  } leading;
  size_t bufsize;

  if (mrb == NULL || fp == NULL) {
    return mrb_nil_value();
  }

  bufsize = fread(leading.b, sizeof(char), sizeof(leading), fp);
  if (bufsize < sizeof(leading.h) ||
      memcmp(leading.h.binary_ident, RITE_BINARY_IDENT, sizeof(leading.h.binary_ident)) != 0 ||
      memchr(leading.b, '\0', bufsize) == NULL) {
    return mrb_load_exec(mrb, mrb_parse_file_continue(mrb, fp, leading.b, bufsize, c), c);
  }
  else {
    mrb_int binsize = bin_to_uint32(leading.h.binary_size);
    mrb_value bin_obj = mrb_str_new(mrb, NULL, binsize);
    uint8_t *bin = (uint8_t*)RSTRING_PTR(bin_obj);
    if ((size_t)binsize > bufsize) {
      memcpy(bin, leading.b, bufsize);
      if (fread(bin + bufsize, binsize - bufsize, 1, fp) == 0) {
        binsize = bufsize;
        /* The error is reported by mrb_load_irep_buf_cxt() */
      }
    }

    mrb_value result = mrb_load_irep_buf_cxt(mrb, bin, binsize, c);
    if (mrb_string_p(bin_obj)) mrb_str_resize(mrb, bin_obj, 0);
    return result;
  }
}
#endif

MRB_API mrb_value
mrb_load_nstring_cxt(mrb_state *mrb, const char *s, size_t len, mrb_ccontext *c)
{
  return mrb_load_exec(mrb, mrb_parse_nstring(mrb, s, len, c), c);
}

MRB_API mrb_value
mrb_load_nstring(mrb_state *mrb, const char *s, size_t len)
{
  return mrb_load_nstring_cxt(mrb, s, len, NULL);
}

MRB_API mrb_value
mrb_load_string_cxt(mrb_state *mrb, const char *s, mrb_ccontext *c)
{
  return mrb_load_nstring_cxt(mrb, s, strlen(s), c);
}

MRB_API mrb_value
mrb_load_string(mrb_state *mrb, const char *s)
{
  return mrb_load_string_cxt(mrb, s, NULL);
}

#ifndef MRB_NO_STDIO

static void
dump_prefix(int offset, uint16_t lineno)
{
  printf("%05d ", lineno);
  while (offset--) {
    putc(' ', stdout);
    putc(' ', stdout);
  }
}

static void
dump_recur(mrb_state *mrb, node *tree, int offset)
{
  while (tree) {
    dump_node(mrb, tree->car, offset);
    tree = tree->cdr;
  }
}

static void
dump_locals(mrb_state *mrb, node *tree, int offset, uint16_t lineno)
{
  if (!tree || (!tree->car && !tree->cdr)) return;

  dump_prefix(offset, lineno);
  printf("locals:\n");
  dump_prefix(offset+1, lineno);
  while (tree) {
    if (tree->car) {
      mrb_sym sym = node_to_sym(tree->car);
      if (sym != 0) {
        const char *name = mrb_sym_name(mrb, sym);
        if (name && strlen(name) > 0 && name[0] != '!' && name[0] != '@' && name[0] != '$') {
          printf(" %s", mrb_sym_dump(mrb, sym));
        }
        else {
          printf(" (invalid symbol: %s)", name ? name : "(null)");
        }
      }
      else {
        printf(" (anonymous)");
      }
    }
    tree = tree->cdr;
  }
  printf("\n");
}

static void
dump_cpath(mrb_state *mrb, node *tree, int offset, uint16_t lineno)
{
  dump_prefix(offset, lineno);
  printf("cpath: ");
  if (!tree) {
    printf("(null)\n");
  }
  else if (node_to_int(tree->car) == 0) {
    printf("(null)\n");
  }
  else if (node_to_int(tree->car) == 1) {
    printf("Object\n");
  }
  else {
    printf("\n");
    dump_node(mrb, tree->car, offset+1);
  }
  dump_prefix(offset, lineno);
  printf("name: %s\n", mrb_sym_dump(mrb, node_to_sym(tree->cdr)));
}

/*
 * This function restores the GC arena on return.
 * For this reason, if a process that further generates an object is
 * performed at the caller, the string pointer returned as the return
 * value may become invalid.
 */
static const char*
str_dump(mrb_state *mrb, const char *str, int len)
{
  int ai = mrb_gc_arena_save(mrb);
  mrb_value s = mrb_str_new(mrb, str, (mrb_int)len);
  s = mrb_str_dump(mrb, s);
  mrb_gc_arena_restore(mrb, ai);
  return RSTRING_PTR(s);
}

static void
dump_str(mrb_state *mrb, node *n, int offset, uint16_t lineno)
{
  while (n) {
    dump_prefix(offset, lineno);
    int len = node_to_int(n->car->car);
    if (len >= 0) {
      printf("str: %s\n", str_dump(mrb, (char*)n->car->cdr, len));
    }
    else {
      printf("interpolation:\n");
      dump_node(mrb, n->car->cdr, offset+1);
    }
    n = n->cdr;
  }
}

static void
dump_args(mrb_state *mrb, struct mrb_ast_args *args, int offset, uint16_t lineno)
{
  if (args->mandatory_args) {
    dump_prefix(offset, lineno);
    printf("mandatory args:\n");
    dump_recur(mrb, args->mandatory_args, offset+1);
  }
  if (args->optional_args) {
    dump_prefix(offset, lineno);
    printf("optional args:\n");
    {
      node *n = args->optional_args;
      while (n) {
        dump_prefix(offset+1, lineno);
        printf("%s=\n", mrb_sym_name(mrb, node_to_sym(n->car->car)));
        dump_node(mrb, n->car->cdr, offset+2);
        n = n->cdr;
      }
    }
  }
  if (args->rest_arg) {
    mrb_sym rest = args->rest_arg;

    dump_prefix(offset, lineno);
    if (rest == MRB_OPSYM(mul))
      printf("rest=*\n");
    else
      printf("rest=*%s\n", mrb_sym_name(mrb, rest));
  }
  if (args->post_mandatory_args) {
    dump_prefix(offset, lineno);
    printf("post mandatory args:\n");
    dump_recur(mrb, args->post_mandatory_args, offset+1);
  }
  if (args->keyword_args) {
    dump_prefix(offset, lineno);
    printf("keyword args:\n");
    {
      node *n = args->keyword_args;
      while (n) {
        dump_prefix(offset+1, lineno);
        printf("%s:\n", mrb_sym_name(mrb, node_to_sym(n->car->car)));
        dump_node(mrb, n->car->cdr, offset+2);
        n = n->cdr;
      }
    }
  }
  if (args->kwrest_arg) {
    mrb_sym rest = args->kwrest_arg;

    dump_prefix(offset, lineno);
    if (rest == MRB_OPSYM(pow))
      printf("kwrest=**\n");
    else
      printf("kwrest=**%s\n", mrb_sym_name(mrb, rest));
  }
  if (args->block_arg) {
    mrb_sym blk = args->block_arg;

    dump_prefix(offset, lineno);
    if (blk == MRB_OPSYM(and))
      printf("blk=&\n");
    else if (blk == MRB_SYM(nil))
      printf("blk=&nil\n");
    else
      printf("blk=&%s\n", mrb_sym_name(mrb, blk));
  }
}

static void
dump_callargs(mrb_state *mrb, node *n, int offset, uint16_t lineno)
{
  if (!n) return;

  struct mrb_ast_callargs *args = (struct mrb_ast_callargs*)n;
  if (args->regular_args) {
    dump_prefix(offset+1, lineno);
    printf("args:\n");
    dump_recur(mrb, args->regular_args, offset+2);
  }
  if (args->keyword_args) {
    dump_prefix(offset+1, lineno);
    printf("kw_args:\n");
    node *kw = args->keyword_args;
    while (kw) {
      dump_prefix(offset+2, lineno);
      printf("key:\n");
      if (node_to_sym(kw->car->car) == MRB_OPSYM(pow)) {
        dump_prefix(offset+3, lineno);
        printf("**:\n");
      }
      else {
        dump_node(mrb, kw->car->car, offset+3);
      }
      dump_prefix(offset+2, lineno);
      printf("value:\n");
      dump_node(mrb, kw->car->cdr, offset+3);
      kw = kw->cdr;
    }
  }
  if (args->block_arg) {
    dump_prefix(offset+1, lineno);
    printf("block:\n");
    dump_node(mrb, args->block_arg, offset+2);
  }
}

#endif

void
dump_node(mrb_state *mrb, node *tree, int offset)
{
#ifndef MRB_NO_STDIO
  enum node_type nodetype;
  uint16_t lineno = 0;

  if (!tree) return;

  /* Extract line number from variable-sized node header */
  if (node_type(tree) != NODE_LAST) {
    lineno = ((struct mrb_ast_var_header*)tree)->lineno;
  }

  dump_prefix(offset, lineno);

  /* All nodes are now variable-sized nodes with headers */
  nodetype = node_type(tree);

  switch (nodetype) {
  /* Variable-sized node cases */
  case NODE_SCOPE:
    printf("NODE_SCOPE:\n");
    if (scope_node(tree)->locals) {
      dump_locals(mrb, scope_node(tree)->locals, offset+1, lineno);
    }
    if (scope_node(tree)->body) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_node(mrb, scope_node(tree)->body, offset+2);
    }
    break;

  case NODE_INT:
    printf("NODE_INT: %d\n", int_node(tree)->value);
    break;

  case NODE_BIGINT:
    printf("NODE_BIGINT: %s (base %d)\n", bigint_node(tree)->string, bigint_node(tree)->base);
    break;

  case NODE_FLOAT:
    printf("NODE_FLOAT: %s\n", float_node(tree)->value);
    break;

  case NODE_STR:
    printf("NODE_STR:\n");
    dump_str(mrb, str_node(tree)->list, offset+1, lineno);
    break;

  case NODE_XSTR:
    printf("NODE_XSTR:\n");
    dump_str(mrb, xstr_node(tree)->list, offset+1, lineno);
    break;

  case NODE_SYM:
    printf("NODE_SYM: %s\n", mrb_sym_dump(mrb, sym_node(tree)->symbol));
    break;

  case NODE_DSYM:
    printf("NODE_DSYM:\n");
    dump_str(mrb, str_node(tree)->list, offset+1, lineno);
    break;

  case NODE_LVAR:
    printf("NODE_LVAR: %s\n", mrb_sym_dump(mrb, var_node(tree)->symbol));
    break;

  case NODE_GVAR:
    printf("NODE_GVAR: %s\n", mrb_sym_dump(mrb, var_node(tree)->symbol));
    break;

  case NODE_IVAR:
    printf("NODE_IVAR: %s\n", mrb_sym_dump(mrb, var_node(tree)->symbol));
    break;

  case NODE_CVAR:
    printf("NODE_CVAR: %s\n", mrb_sym_dump(mrb, var_node(tree)->symbol));
    break;

  case NODE_NVAR:
    printf("NODE_NVAR: %d\n", nvar_node(tree)->num);
    break;

  case NODE_CONST:
    printf("NODE_CONST: %s\n", mrb_sym_dump(mrb, var_node(tree)->symbol));
    break;

  case NODE_CALL:
    printf("NODE_CALL: %s\n", mrb_sym_dump(mrb, call_node(tree)->method_name));
    if (call_node(tree)->receiver) {
      dump_prefix(offset+1, lineno);
      printf("receiver:\n");
      dump_node(mrb, call_node(tree)->receiver, offset+2);
    }
    if (call_node(tree)->args) {
      dump_callargs(mrb, call_node(tree)->args, offset, lineno);
    }
    break;

  case NODE_ARRAY:
    printf("NODE_ARRAY:\n");
    if (array_node(tree)->elements) {
      dump_recur(mrb, array_node(tree)->elements, offset+1);
    }
    break;

  case NODE_TRUE:
    printf("NODE_TRUE\n");
    break;

  case NODE_FALSE:
    printf("NODE_FALSE\n");
    break;

  case NODE_NIL:
    printf("NODE_NIL\n");
    break;

  case NODE_SELF:
    printf("NODE_SELF\n");
    break;

  case NODE_IF:
    printf("NODE_IF:\n");
    if (if_node(tree)->condition) {
      dump_prefix(offset+1, lineno);
      printf("cond:\n");
      dump_node(mrb, if_node(tree)->condition, offset+2);
    }
    if (if_node(tree)->then_body) {
      dump_prefix(offset+1, lineno);
      printf("then:\n");
      dump_node(mrb, if_node(tree)->then_body, offset+2);
    }
    if (if_node(tree)->else_body) {
      dump_prefix(offset+1, lineno);
      printf("else:\n");
      dump_node(mrb, if_node(tree)->else_body, offset+2);
    }
    break;

  case NODE_DEF:
    printf("NODE_DEF: %s\n", mrb_sym_dump(mrb, def_node(tree)->name));
    if (def_node(tree)->args) {
      dump_args(mrb, sdef_node(tree)->args, offset+1, lineno);
    }
    if (def_node(tree)->locals) {
      dump_locals(mrb, def_node(tree)->locals, offset+1, lineno);
    }
    if (def_node(tree)->body) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_node(mrb, def_node(tree)->body, offset+2);
    }
    break;

  case NODE_ASGN:
    printf("NODE_ASGN:\n");
    if (asgn_node(tree)->lhs) {
      dump_prefix(offset+1, lineno);
      printf("lhs:\n");
      dump_node(mrb, asgn_node(tree)->lhs, offset+2);
    }
    if (asgn_node(tree)->rhs) {
      dump_prefix(offset+1, lineno);
      printf("rhs:\n");
      dump_node(mrb, asgn_node(tree)->rhs, offset+2);
    }
    break;

  case NODE_MASGN:
  case NODE_MARG:
    printf("%s:\n", node_type(tree) == NODE_MASGN ? "NODE_MASGN" : "NODE_MARG");
    /* Handle pre-splat variables */
    if (masgn_node(tree)->pre) {
      dump_prefix(offset+1, lineno);
      printf("pre:\n");
      dump_recur(mrb, masgn_node(tree)->pre, offset+2);
    }
    /* Handle splat variable (can be -1 sentinel for anonymous splat) */
    if (masgn_node(tree)->rest) {
      if ((intptr_t)masgn_node(tree)->rest == -1) {
        dump_prefix(offset+1, lineno);
        printf("rest: *<anonymous>\n");
      }
      else {
        dump_prefix(offset+1, lineno);
        printf("rest:\n");
        dump_node(mrb, masgn_node(tree)->rest, offset+2);
      }
    }
    /* Handle post-splat variables */
    if (masgn_node(tree)->post) {
      dump_prefix(offset+1, lineno);
      printf("post:\n");
      dump_recur(mrb, masgn_node(tree)->post, offset+2);
    }
    if (masgn_node(tree)->rhs) {
      dump_prefix(offset+1, lineno);
      printf("rhs:\n");
      dump_node(mrb, masgn_node(tree)->rhs, offset+2);
    }
    break;

  case NODE_RETURN:
    printf("NODE_RETURN:\n");
    if (return_node(tree)->args) {
      dump_node(mrb, return_node(tree)->args, offset);
    }
    break;

  case NODE_BREAK:
    printf("NODE_BREAK:\n");
    if (break_node(tree)->value) {
      dump_prefix(offset+1, lineno);
      printf("value:\n");
      dump_node(mrb, break_node(tree)->value, offset+2);
    }
    break;

  case NODE_NEXT:
    printf("NODE_NEXT:\n");
    if (next_node(tree)->value) {
      dump_prefix(offset+1, lineno);
      printf("value:\n");
      dump_node(mrb, next_node(tree)->value, offset+2);
    }
    break;

  case NODE_NEGATE:
    printf("NODE_NEGATE:\n");
    if (negate_node(tree)->operand) {
      dump_prefix(offset+1, lineno);
      printf("operand:\n");
      dump_node(mrb, negate_node(tree)->operand, offset+2);
    }
    break;

  case NODE_STMTS:
    printf("NODE_STMTS:\n");
    if (stmts_node(tree)->stmts) {
      dump_recur(mrb, stmts_node(tree)->stmts, offset+1);
    }
    break;

  case NODE_BEGIN:
    printf("NODE_BEGIN:\n");
    if (begin_node(tree)->body) {
      dump_node(mrb, begin_node(tree)->body, offset+1);
    }
    break;

  case NODE_RESCUE:
    printf("NODE_RESCUE:\n");
    if (rescue_node(tree)->body) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_node(mrb, rescue_node(tree)->body, offset+2);
    }
    if (rescue_node(tree)->rescue_clauses) {
      node *n2 = rescue_node(tree)->rescue_clauses;
      dump_prefix(offset+1, lineno);
      printf("rescue:\n");
      while (n2) {
        node *n3 = n2->car;
        if (n3->car) {
          dump_prefix(offset+2, lineno);
          printf("handle classes:\n");
          dump_recur(mrb, n3->car, offset+3);
        }
        if (n3->cdr->car) {
          dump_prefix(offset+2, lineno);
          printf("exc_var:\n");
          dump_node(mrb, n3->cdr->car, offset+3);
        }
        if (n3->cdr->cdr->car) {
          dump_prefix(offset+2, lineno);
          printf("rescue body:\n");
          dump_node(mrb, n3->cdr->cdr->car, offset+3);
        }
        n2 = n2->cdr;
      }
    }
    if (rescue_node(tree)->else_clause) {
      dump_prefix(offset+1, lineno);
      printf("else:\n");
      dump_node(mrb, rescue_node(tree)->else_clause, offset+2);
    }
    break;

  case NODE_ENSURE:
    printf("NODE_ENSURE:\n");
    if (ensure_node(tree)->body) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_node(mrb, ensure_node(tree)->body, offset+2);
    }
    if (ensure_node(tree)->ensure_clause) {
      dump_prefix(offset+1, lineno);
      printf("ensure:\n");
      dump_node(mrb, ensure_node(tree)->ensure_clause, offset+2);
    }
    break;

  case NODE_LAMBDA:
    printf("NODE_LAMBDA:\n");
    goto block;

  case NODE_BLOCK:
    printf("NODE_BLOCK:\n");
  block:
    if (block_node(tree)->locals) {
      dump_locals(mrb, block_node(tree)->locals, offset+1, lineno);
    }
    if (block_node(tree)->args) {
      dump_args(mrb, block_node(tree)->args, offset+1, lineno);
    }
    dump_prefix(offset+1, lineno);
    printf("body:\n");
    dump_node(mrb, block_node(tree)->body, offset+2);
    break;

  case NODE_AND:
    printf("NODE_AND:\n");
    dump_node(mrb, and_node(tree)->left, offset+1);
    dump_node(mrb, and_node(tree)->right, offset+1);
    break;

  case NODE_OR:
    printf("NODE_OR:\n");
    dump_node(mrb, or_node(tree)->left, offset+1);
    dump_node(mrb, or_node(tree)->right, offset+1);
    break;

  case NODE_CASE:
    printf("NODE_CASE:\n");
    if (case_node(tree)->value) {
      dump_prefix(offset+1, lineno);
      printf("value:\n");
      dump_node(mrb, case_node(tree)->value, offset+2);
    }
    if (case_node(tree)->body) {
      node *when_node = case_node(tree)->body;
      while (when_node) {
        dump_prefix(offset+1, lineno);
        printf("when:\n");
        node *when_clause = when_node->car;
        if (when_clause && when_clause->car) {
          dump_prefix(offset+2, lineno);
          printf("cond:\n");
          dump_recur(mrb, when_clause->car, offset+3);
        }
        if (when_clause && when_clause->cdr) {
          dump_prefix(offset+2, lineno);
          printf("body:\n");
          dump_node(mrb, when_clause->cdr, offset+3);
        }
        when_node = when_node->cdr;
      }
    }
    break;

  case NODE_WHILE:
    printf("NODE_WHILE:\n");
    goto dump_loop_node;
  case NODE_UNTIL:
    printf("NODE_UNTIL:\n");
    goto dump_loop_node;
  case NODE_WHILE_MOD:
    printf("NODE_WHILE_MOD:\n");
    goto dump_loop_node;
  case NODE_UNTIL_MOD:
    printf("NODE_UNTIL_MOD:\n");

  dump_loop_node:
    dump_prefix(offset+1, lineno);
    printf("cond:\n");
    dump_node(mrb, while_node(tree)->condition, offset+2);
    dump_prefix(offset+1, lineno);
    printf("body:\n");
    dump_node(mrb, while_node(tree)->body, offset+2);
    break;

  case NODE_FOR:
    printf("NODE_FOR:\n");
    if (for_node(tree)->var) {
      dump_prefix(offset+1, lineno);
      printf("var:\n");
      /* FOR_NODE_VAR structure:
       * var_list->car: cons-list of pre-splat variables
       * var_list->cdr->car: splat varnode (not a cons-list)
       * var_list->cdr->cdr->car: cons-list of post-splat variables */
      node *var_list = for_node(tree)->var;
      if (var_list) {
        dump_recur(mrb, var_list->car, offset+2);
        if (var_list && var_list->cdr) {
          /* Second element is a varnode, not a cons-list */
          dump_prefix(offset+1, lineno);
          printf("splat var:\n");
          dump_node(mrb, var_list->cdr->car, offset+2);
          if (var_list->cdr->cdr) {
           /* Third element is a cons-list of post-splat variables */
           dump_prefix(offset+1, lineno);
           printf("post var:\n");
           dump_recur(mrb, var_list->cdr->cdr->car, offset+2);
          }
        }
      }
    }
    if (for_node(tree)->iterable) {
      dump_prefix(offset+1, lineno);
      printf("iterable:\n");
      dump_node(mrb, for_node(tree)->iterable, offset+2);
    }
    if (for_node(tree)->body) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_node(mrb, for_node(tree)->body, offset+2);
    }
    break;

  case NODE_DOT2:
    printf("NODE_DOT2:\n");
    {
      if (dot2_node(tree)->left) {
        dump_prefix(offset+1, lineno);
        printf("left:\n");
        dump_node(mrb, dot2_node(tree)->left, offset+2);
      }
      if (dot2_node(tree)->right) {
        dump_prefix(offset+1, lineno);
        printf("right:\n");
        dump_node(mrb, dot2_node(tree)->right, offset+2);
      }
    }
    break;

  case NODE_DOT3:
    printf("NODE_DOT3:\n");
    {
      if (dot3_node(tree)->left) {
        dump_prefix(offset+1, lineno);
        printf("left:\n");
        dump_node(mrb, dot3_node(tree)->left, offset+2);
      }
      if (dot3_node(tree)->right) {
        dump_prefix(offset+1, lineno);
        printf("right:\n");
        dump_node(mrb, dot3_node(tree)->right, offset+2);
      }
    }
    break;

  case NODE_COLON2:
    printf("NODE_COLON2:\n");
    if (colon2_node(tree)->base) {
      dump_prefix(offset+1, lineno);
      printf("base:\n");
      dump_node(mrb, colon2_node(tree)->base, offset+2);
    }
    dump_prefix(offset+1, lineno);
    printf("name: %s\n", mrb_sym_name(mrb, colon2_node(tree)->name));
    break;

  case NODE_COLON3:
    printf("NODE_COLON3: ::%s\n", mrb_sym_name(mrb, colon3_node(tree)->name));
    break;

  case NODE_HASH:
    printf("NODE_HASH:\n");
    {
      node *pairs = hash_node(tree)->pairs;
      while (pairs) {
        dump_prefix(offset+1, lineno);
        printf("key:\n");
        if (node_to_sym(pairs->car->car) == MRB_OPSYM(pow)) {
          dump_prefix(offset+2, lineno);
          printf("**\n");
        }
        else {
          dump_node(mrb, pairs->car->car, offset+2);
        }
        dump_prefix(offset+1, lineno);
        printf("value:\n");
        dump_node(mrb, pairs->car->cdr, offset+2);
        pairs = pairs->cdr;
      }
    }
    break;

  case NODE_SPLAT:
    printf("NODE_SPLAT:\n");
    dump_node(mrb, splat_node(tree)->value, offset+1);
    break;

  case NODE_OP_ASGN:
    printf("NODE_OP_ASGN:\n");
    dump_prefix(offset+1, lineno);
    printf("lhs:\n");
    dump_node(mrb, op_asgn_node(tree)->lhs, offset+2);
    dump_prefix(offset+1, lineno);
    printf("op='%s' (%d)\n", mrb_sym_name(mrb, op_asgn_node(tree)->op), (int)op_asgn_node(tree)->op);
    dump_node(mrb, op_asgn_node(tree)->rhs, offset+1);
    break;

  case NODE_SUPER:
    printf("NODE_SUPER:\n");
    if (super_node(tree)->args) {
      dump_callargs(mrb, super_node(tree)->args, offset, lineno);
    }
    break;

  case NODE_ZSUPER:
    printf("NODE_ZSUPER:\n");
    if (super_node(tree)->args) {
      dump_callargs(mrb, super_node(tree)->args, offset, lineno);
    }
    break;

  case NODE_YIELD:
    printf("NODE_YIELD:\n");
    if (yield_node(tree)->args) {
      dump_callargs(mrb, yield_node(tree)->args, offset, lineno);
    }
    break;

  case NODE_REDO:
    printf("NODE_REDO\n");
    break;

  case NODE_RETRY:
    printf("NODE_RETRY\n");
    break;

  case NODE_BACK_REF:
    printf("NODE_BACK_REF: $%c\n", back_ref_node(tree)->type);
    break;

  case NODE_NTH_REF:
    printf("NODE_NTH_REF: $%d\n", nth_ref_node(tree)->nth);
    break;

  case NODE_BLOCK_ARG:
    printf("NODE_BLOCK_ARG:\n");
    dump_node(mrb, block_arg_node(tree)->value, offset+1);
    break;

  case NODE_REGX:
    printf("NODE_REGX:\n");
    if (regx_node(tree)->list) {
      dump_str(mrb, regx_node(tree)->list, offset+1, lineno);
    }
    if (regx_node(tree)->flags) {
      dump_prefix(offset+1, lineno);
      printf("flags: %s\n", regx_node(tree)->flags);
    }
    if (regx_node(tree)->encoding) {
      dump_prefix(offset+1, lineno);
      printf("encoding: %s\n", regx_node(tree)->encoding);
    }
    break;

  case NODE_WORDS:
    printf("NODE_WORDS:\n");
    if (words_node(tree)->args) {
      node *list = words_node(tree)->args;
      while (list && list->car) {
        node *item = list->car;
        if (item->car == 0 && item->cdr == 0) {
          /* Skip separator (0 . 0) */
        }
        else if (item->car && item->cdr) {
          /* String item: (len . str) */
          dump_prefix(offset+1, lineno);
          int len = node_to_int(item->car);
          if (len >= 0 && len < 1000 && item->cdr) {
            printf("word: \"%.*s\"\n", len, (char*)item->cdr);
          }
        }
        list = list->cdr;
      }
    }
    break;

  case NODE_SYMBOLS:
    printf("NODE_SYMBOLS:\n");
    if (symbols_node(tree)->args) {
      node *list = symbols_node(tree)->args;
      while (list && list->car) {
        node *item = list->car;
        if (item->car == 0 && item->cdr == 0) {
          /* Skip separator (0 . 0) */
        } else if (item->car && item->cdr) {
          /* String item: (len . str) */
          dump_prefix(offset+1, lineno);
          int len = node_to_int(item->car);
          if (len >= 0 && len < 1000 && item->cdr) {
            printf("symbol: \"%.*s\"\n", len, (char*)item->cdr);
          }
        }
        list = list->cdr;
      }
    }
    break;

  case NODE_ALIAS:
    printf("NODE_ALIAS %s %s:\n",
           mrb_sym_dump(mrb, node_to_sym(tree->car)),
           mrb_sym_dump(mrb, node_to_sym(tree->cdr)));
    break;

  case NODE_UNDEF:
    printf("NODE_UNDEF");
    {
      node *t = tree;
      while (t) {
        printf(" %s", mrb_sym_dump(mrb, node_to_sym(t->car)));
        t = t->cdr;
      }
    }
    printf(":\n");
    break;

  case NODE_CLASS:
    printf("NODE_CLASS:\n");
    if (class_node(tree)->name) {
      dump_cpath(mrb, module_node(tree)->name, offset+1, lineno);
    }
    if (class_node(tree)->superclass) {
      dump_prefix(offset+1, lineno);
      printf("super:\n");
      dump_node(mrb, class_node(tree)->superclass, offset+2);
    }
    if (class_node(tree)->body) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_node(mrb, class_node(tree)->body->cdr, offset+2);
    }
    break;

  case NODE_MODULE:
    printf("NODE_MODULE:\n");
    if (module_node(tree)->name) {
      dump_cpath(mrb, module_node(tree)->name, offset+1, lineno);
    }
    if (module_node(tree)->body) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_node(mrb, module_node(tree)->body->cdr, offset+2);
    }
    break;

  case NODE_SCLASS:
    printf("NODE_SCLASS:\n");
    if (sclass_node(tree)->obj) {
      dump_prefix(offset+1, lineno);
      printf("obj:\n");
      dump_node(mrb, sclass_node(tree)->obj, offset+2);
    }
    if (sclass_node(tree)->body) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_node(mrb, sclass_node(tree)->body->cdr, offset+2);
    }
    break;

  case NODE_SDEF:
    printf("NODE_SDEF: %s\n", mrb_sym_dump(mrb, def_node(tree)->name));
    if (sdef_node(tree)->obj) {
      dump_prefix(offset+1, lineno);
      printf("recv:\n");
      dump_node(mrb, sdef_node(tree)->obj, offset+2);
    }
    if (sdef_node(tree)->args) {
      dump_args(mrb, sdef_node(tree)->args, offset+1, lineno);
    }
    if (sdef_node(tree)->locals) {
      dump_locals(mrb, sdef_node(tree)->locals, offset+1, lineno);
    }
    if (sdef_node(tree)->body) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_node(mrb, sdef_node(tree)->body, offset+2);
    }
    break;

  case NODE_POSTEXE:
    printf("NODE_POSTEXE:\n");
    dump_node(mrb, tree, offset+1);
    break;

  case NODE_HEREDOC:
    printf("NODE_HEREDOC:\n");
    if (heredoc_node(tree)->info.term) {
      dump_prefix(offset+1, lineno);
      printf("terminator: \"%s\"\n", heredoc_node(tree)->info.term);
    }
    if (heredoc_node(tree)->info.doc) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_str(mrb, heredoc_node(tree)->info.doc, offset+2, lineno);
    }
    if (heredoc_node(tree)->info.allow_indent) {
      dump_prefix(offset+1, lineno);
      printf("allow_indent: true\n");
    }
    if (heredoc_node(tree)->info.remove_indent) {
      dump_prefix(offset+1, lineno);
      printf("remove_indent: true\n");
    }
    break;

  case NODE_CASE_MATCH:
    printf("NODE_CASE_MATCH:\n");
    if (case_match_node(tree)->value) {
      dump_prefix(offset+1, lineno);
      printf("value:\n");
      dump_node(mrb, case_match_node(tree)->value, offset+2);
    }
    if (case_match_node(tree)->in_clauses) {
      node *in_clause = case_match_node(tree)->in_clauses;
      while (in_clause) {
        dump_node(mrb, in_clause->car, offset+1);
        in_clause = in_clause->cdr;
      }
    }
    break;

  case NODE_IN:
    printf("NODE_IN:\n");
    if (in_node(tree)->pattern) {
      dump_prefix(offset+1, lineno);
      printf("pattern:\n");
      dump_node(mrb, in_node(tree)->pattern, offset+2);
    }
    if (in_node(tree)->guard) {
      dump_prefix(offset+1, lineno);
      printf("guard (%s):\n", in_node(tree)->guard_is_unless ? "unless" : "if");
      dump_node(mrb, in_node(tree)->guard, offset+2);
    }
    if (in_node(tree)->body) {
      dump_prefix(offset+1, lineno);
      printf("body:\n");
      dump_node(mrb, in_node(tree)->body, offset+2);
    }
    break;

  case NODE_PAT_VALUE:
    printf("NODE_PAT_VALUE:\n");
    if (pat_value_node(tree)->value) {
      dump_node(mrb, pat_value_node(tree)->value, offset+1);
    }
    break;

  case NODE_PAT_VAR:
    if (pat_var_node(tree)->name) {
      printf("NODE_PAT_VAR: %s\n", mrb_sym_dump(mrb, pat_var_node(tree)->name));
    }
    else {
      printf("NODE_PAT_VAR: _ (wildcard)\n");
    }
    break;

  case NODE_PAT_PIN:
    printf("NODE_PAT_PIN: ^%s\n", mrb_sym_dump(mrb, pat_pin_node(tree)->name));
    break;

  case NODE_PAT_AS:
    printf("NODE_PAT_AS: => %s\n", mrb_sym_dump(mrb, pat_as_node(tree)->name));
    if (pat_as_node(tree)->pattern) {
      dump_prefix(offset+1, lineno);
      printf("pattern:\n");
      dump_node(mrb, pat_as_node(tree)->pattern, offset+2);
    }
    break;

  case NODE_PAT_ALT:
    printf("NODE_PAT_ALT:\n");
    if (pat_alt_node(tree)->left) {
      dump_prefix(offset+1, lineno);
      printf("left:\n");
      dump_node(mrb, pat_alt_node(tree)->left, offset+2);
    }
    if (pat_alt_node(tree)->right) {
      dump_prefix(offset+1, lineno);
      printf("right:\n");
      dump_node(mrb, pat_alt_node(tree)->right, offset+2);
    }
    break;

  case NODE_PAT_ARRAY:
    printf("NODE_PAT_ARRAY:\n");
    if (pat_array_node(tree)->pre) {
      dump_prefix(offset+1, lineno);
      printf("pre:\n");
      dump_recur(mrb, pat_array_node(tree)->pre, offset+2);
    }
    if (pat_array_node(tree)->rest) {
      dump_prefix(offset+1, lineno);
      if (pat_array_node(tree)->rest == (node*)-1) {
        printf("rest: * (anonymous)\n");
      }
      else {
        printf("rest:\n");
        dump_node(mrb, pat_array_node(tree)->rest, offset+2);
      }
    }
    if (pat_array_node(tree)->post) {
      dump_prefix(offset+1, lineno);
      printf("post:\n");
      dump_recur(mrb, pat_array_node(tree)->post, offset+2);
    }
    break;

  case NODE_PAT_HASH:
    printf("NODE_PAT_HASH:\n");
    if (pat_hash_node(tree)->pairs) {
      dump_prefix(offset+1, lineno);
      printf("pairs:\n");
      dump_recur(mrb, pat_hash_node(tree)->pairs, offset+2);
    }
    if (pat_hash_node(tree)->rest) {
      dump_prefix(offset+1, lineno);
      if (pat_hash_node(tree)->rest == (node*)-1) {
        printf("rest: **nil\n");
      }
      else {
        printf("rest:\n");
        dump_node(mrb, pat_hash_node(tree)->rest, offset+2);
      }
    }
    break;

  case NODE_MATCH_PAT:
    printf("NODE_MATCH_PAT%s:\n", match_pat_node(tree)->raise_on_fail ? " (=>)" : " (in)");
    dump_prefix(offset+1, lineno);
    printf("value:\n");
    dump_node(mrb, match_pat_node(tree)->value, offset+2);
    dump_prefix(offset+1, lineno);
    printf("pattern:\n");
    dump_node(mrb, match_pat_node(tree)->pattern, offset+2);
    break;

  default:
    /* Fallback: unknown node type - skip like codegen.c does */
    printf("unknown node type %d (0x%x)\n", nodetype, (unsigned)nodetype);
    break;
  }
#endif
}

void
mrb_parser_dump(mrb_state *mrb, node *tree, int offset)
{
  dump_node(mrb, tree, offset);
}

typedef mrb_bool mrb_parser_foreach_top_variable_func(mrb_state *mrb, mrb_sym sym, void *user);
void mrb_parser_foreach_top_variable(mrb_state *mrb, struct mrb_parser_state *p, mrb_parser_foreach_top_variable_func *func, void *user);

void
mrb_parser_foreach_top_variable(mrb_state *mrb, struct mrb_parser_state *p, mrb_parser_foreach_top_variable_func *func, void *user)
{
  const mrb_ast_node *n = p->tree;
  if (node_type_p((node*)n, NODE_SCOPE)) {
    /* Extract locals from variable-sized NODE_SCOPE */
    struct mrb_ast_scope_node *scope = scope_node(n);
    n = scope->locals;
    for (; n; n = n->cdr) {
      mrb_sym sym = node_to_sym(n->car);
      if (sym != 0) {
        if (!func(mrb, sym, user)) break;
      }
    }
  }
}

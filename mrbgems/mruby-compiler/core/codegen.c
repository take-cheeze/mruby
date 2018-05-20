/*
** codegen.c - mruby code generator
**
** See Copyright Notice in mruby.h
*/

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/proc.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include <mruby/debug.h>
#include "node.h"
#include <mruby/opcode.h>
#include <mruby/re.h>
#include <mruby/throw.h>

#define nint(x) ((int)(intptr_t)(x))
#define nchar(x) ((char)(intptr_t)(x))
#define nsym(x) ((mrb_sym)(x))

typedef mrb_ast_node node;
typedef struct mrb_parser_state parser_state;

typedef struct code_generator {
  mrb_state *mrb;
  char *str;
  size_t str_len, str_capa;
} code_generator;

static void append_str(code_generator *g, const char *str) {
  size_t str_len = strlen(str);
  if ((g->str_len + str_len) > g->str_capa) {
    g->str_capa *= 3; // * 1.5
    g->str_capa /= 2;
    g->str = mrb_realloc(g->mrb, g->str, g->str_capa);
  }
  memcpy(g->str + g->str_len, str, str_len);
  g->str_len += str_len;
}

static void append_sym(code_generator *g, GCstr *str) {
  append_str(g, strdata(str));
}

static void
codegen(code_generator *s, node *tree)
{
  int nt = nint(tree->car);
  tree = tree->cdr;
  switch (nt) {
  case NODE_BEGIN:
    if (!tree) {
      append_str(s, " nil ");
      return;
    }
    append_str(s, " do ");
    while (tree) {
      codegen(s, tree->car);
      append_str(s, ";");
      tree = tree->cdr;
    }
    append_str(s, " end ");
    break;

  case NODE_RESCUE:
    {
      int onerr, noexc, exend, pos1, pos2, tmp;
      struct loopinfo *lp;

      if (tree->car == NULL) return;
      onerr = genop(s, MKOP_Bx(OP_ONERR, 0));
      lp = loop_push(s, LOOP_BEGIN);
      lp->pc1 = onerr;
      codegen(s, tree->car);
      pop();
      lp->type = LOOP_RESCUE;
      noexc = genop(s, MKOP_Bx(OP_JMP, 0));
      dispatch(s, onerr);
      tree = tree->cdr;
      exend = 0;
      pos1 = 0;
      if (tree->car) {
        node *n2 = tree->car;
        int exc = cursp();

        genop(s, MKOP_ABC(OP_RESCUE, exc, 0, 0));
        push();
        while (n2) {
          node *n3 = n2->car;
          node *n4 = n3->car;

          if (pos1) dispatch(s, pos1);
          pos2 = 0;
          do {
            if (n4 && n4->car && nint(n4->car->car) == NODE_SPLAT) {
              codegen(s, n4->car);
              genop(s, MKOP_AB(OP_MOVE, cursp(), exc));
              push_n(2); pop_n(2); // space for one arg and a block
              pop();
              genop(s, MKOP_ABC(OP_SEND, cursp(), new_msym(s, mrb_intern_lit(s->mrb, "__case_eqq")), 1));
            }
            else {
              if (n4) {
                codegen(s, n4->car);
              }
              else {
                genop(s, MKOP_ABx(OP_GETCONST, cursp(), new_msym(s, mrb_intern_lit(s->mrb, "StandardError"))));
                push();
              }
              pop();
              genop(s, MKOP_ABC(OP_RESCUE, exc, cursp(), 1));
            }
            distcheck(s, pos2);
            tmp = genop(s, MKOP_AsBx(OP_JMPIF, cursp(), pos2));
            pos2 = tmp;
            if (n4) {
              n4 = n4->cdr;
            }
          } while (n4);
          pos1 = genop(s, MKOP_sBx(OP_JMP, 0));
          dispatch_linked(s, pos2);

          pop();
          if (n3->cdr->car) {
            gen_assignment(s, n3->cdr->car, exc);
          }
          if (n3->cdr->cdr->car) {
            codegen(s, n3->cdr->cdr->car);
            if (val) pop();
          }
          distcheck(s, exend);
          tmp = genop(s, MKOP_sBx(OP_JMP, exend));
          exend = tmp;
          n2 = n2->cdr;
          push();
        }
        if (pos1) {
          dispatch(s, pos1);
          genop(s, MKOP_A(OP_RAISE, exc));
        }
      }
      pop();
      tree = tree->cdr;
      dispatch(s, noexc);
      genop(s, MKOP_A(OP_POPERR, 1));
      if (tree->car) {
        codegen(s, tree->car);
      }
      else if (val) {
        push();
      }
      dispatch_linked(s, exend);
      loop_pop(s);
    }
    break;

  case NODE_ENSURE:
    if (!tree->cdr || !tree->cdr->cdr ||
        (nint(tree->cdr->cdr->car) == NODE_BEGIN &&
         tree->cdr->cdr->cdr)) {
      int idx;
      int epush = s->pc;

      genop(s, MKOP_Bx(OP_EPUSH, 0));
      s->ensure_level++;
      append_str(s, "local _ok, _err = pcall(function()\n");
      codegen(s, tree->car);
      append_str(s, "end);\n");
      scope_body(s, tree->cdr);
      append_str(s, "if !_ok then error(_err) end\n")
    }
    else {                      // empty ensure ignored
      codegen(s, tree->car);
    }
    break;

  case NODE_LAMBDA:
    append_str(s, "(");
    lambda_body(s, tree, TRUE);
    append_str(s, ");");
    break;

  case NODE_BLOCK:
    append_str(s, "(");
    lambda_body(s, tree, TRUE);
    append_str(s, ");");
    break;

  case NODE_IF:
    {
      node *e = tree->cdr->cdr->car;

      if (!tree->car) {
        codegen(s, e);
        return;
      }
      switch (nint(tree->car->car)) {
      case NODE_TRUE:
      case NODE_INT:
      case NODE_STR:
        codegen(s, tree->cdr->car);
        return;
      case NODE_FALSE:
      case NODE_NIL:
        codegen(s, e);
        return;
      }
      append_str(s, "if (");
      codegen(s, tree->car);
      append_sym(s, ") then ");
      codegen(s, tree->cdr->car);
      if (e) {
        append_str(s, " else ")
        codegen(s, e);
      }
      append_str(s, " end ")
    }
    break;

  case NODE_AND:
    append_str(s, "((")
    codegen(s, tree->car);
    append_str(s, ") and (");
    codegen(s, tree->cdr);
    append_str(s, "))");
    break;

  case NODE_OR:
    append_str(s, "((");
    codegen(s, tree->car);
    append_str(s, ") or (");
    codegen(s, tree->cdr);
    append_str(s, "))");
    break;

  case NODE_WHILE:
    append_str(s, " while (");
    codegen(s, tree->car);
    append_str(s, ") do ");
    codegen(s, tree->cdr);
    append_str(s, " end ");
    break;

  case NODE_UNTIL:
    append_str(s, " while not (");
    codegen(s, tree->car);
    append_str(s, ") do ");
    codegen(s, tree->cdr);
    append_str(s, " end ");
    break;

  case NODE_FOR:
    for_body(s, tree);
    if (val) push();
    break;

  case NODE_CASE:
    {
      int head = 0;
      int pos1, pos2, pos3, tmp;
      node *n;

      pos3 = 0;
      if (tree->car) {
        head = cursp();
        codegen(s, tree->car);
      }
      tree = tree->cdr;
      while (tree) {
        n = tree->car->car;
        pos1 = pos2 = 0;
        while (n) {
          codegen(s, n->car);
          if (head) {
            genop(s, MKOP_AB(OP_MOVE, cursp(), head));
            push_n(2); pop_n(2); // space for one arg and a block
            pop();
            if (nint(n->car->car) == NODE_SPLAT) {
              genop(s, MKOP_ABC(OP_SEND, cursp(), new_msym(s, mrb_intern_lit(s->mrb, "__case_eqq")), 1));
            }
            else {
              genop(s, MKOP_ABC(OP_SEND, cursp(), new_msym(s, mrb_intern_lit(s->mrb, "===")), 1));
            }
          }
          else {
            pop();
          }
          distcheck(s, pos2);
          tmp = genop(s, MKOP_AsBx(OP_JMPIF, cursp(), pos2));
          pos2 = tmp;
          n = n->cdr;
        }
        if (tree->car->car) {
          pos1 = genop(s, MKOP_sBx(OP_JMP, 0));
          dispatch_linked(s, pos2);
        }
        codegen(s, tree->car->cdr);
        if (val) pop();
        distcheck(s, pos3);
        tmp = genop(s, MKOP_sBx(OP_JMP, pos3));
        pos3 = tmp;
        if (pos1) dispatch(s, pos1);
        tree = tree->cdr;
      }
      if (val) {
        int pos = cursp();
        genop(s, MKOP_A(OP_LOADNIL, cursp()));
        if (pos3) dispatch_linked(s, pos3);
        if (head) pop();
        if (cursp() != pos) {
          genop(s, MKOP_AB(OP_MOVE, cursp(), pos));
        }
        push();
      }
      else {
        if (pos3) {
          dispatch_linked(s, pos3);
        }
        if (head) {
          pop();
        }
      }
    }
    break;

  case NODE_SCOPE:
    scope_body(s, tree);
    break;

  case NODE_FCALL:
  case NODE_CALL:
    gen_call(s, tree, 0, 0, val, 0);
    break;
  case NODE_SCALL:
    gen_call(s, tree, 0, 0, val, 1);
    break;

  case NODE_DOT2:
    append_str(s, "Range.new((")
    codegen(s, tree->car);
    append_str(s, "), (");
    codegen(s, tree->cdr);
    append_sym(s, "))");
    break;

  case NODE_DOT3:
    append_str(s, "Range.new((")
    codegen(s, tree->car);
    append_str(s, "), (");
    codegen(s, tree->cdr);
    append_sym(s, "), true)");
    break;

  case NODE_COLON2:
    append_str(s, "(");
    codegen(s, tree->car);
    append_str(s, ").[\"");
    append_sym(s, nsym(tree->cdr));
    append_str(s, "\"]");
    break;

  case NODE_COLON3:
    {
      int sym = new_sym(s, nsym(tree));

      genop(s, MKOP_A(OP_OCLASS, cursp()));
      genop(s, MKOP_ABx(OP_GETMCNST, cursp(), sym));
      if (val) push();
    }
    break;

  case NODE_ARRAY:
    {
      int n;

      n = gen_values(s, tree, val, 0);
      if (n >= 0) {
        if (val) {
          pop_n(n);
          genop(s, MKOP_ABC(OP_ARRAY, cursp(), cursp(), n));
          push();
        }
      }
      else if (val) {
        push();
      }
    }
    break;

  case NODE_HASH:
    {
      int len = 0;
      mrb_bool update = FALSE;

      while (tree) {
        codegen(s, tree->car->car);
        codegen(s, tree->car->cdr);
        len++;
        tree = tree->cdr;
        if (val && len == 126) {
          pop_n(len*2);
          genop(s, MKOP_ABC(OP_HASH, cursp(), cursp(), len));
          if (update) {
            pop();
            genop(s, MKOP_ABC(OP_SEND, cursp(), new_msym(s, mrb_intern_lit(s->mrb, "__update")), 1));
          }
          push();
          update = TRUE;
          len = 0;
        }
      }
      if (val) {
        pop_n(len*2);
        genop(s, MKOP_ABC(OP_HASH, cursp(), cursp(), len));
        if (update) {
          pop();
          genop(s, MKOP_ABC(OP_SEND, cursp(), new_msym(s, mrb_intern_lit(s->mrb, "__update")), 1));
        }
        push();
      }
    }
    break;

  case NODE_SPLAT:
    codegen(s, tree);
    break;

  case NODE_ASGN:
    codegen(s, tree->cdr);
    pop();
    gen_assignment(s, tree->car, cursp());
    break;

  case NODE_MASGN:
    {
      int len = 0, n = 0, post = 0;
      node *t = tree->cdr, *p;
      int rhs = cursp();

      if (nint(t->car) == NODE_ARRAY && t->cdr && nosplat(t->cdr)) {
        // fixed rhs
        t = t->cdr;
        while (t) {
          codegen(s, t->car);
          len++;
          t = t->cdr;
        }
        tree = tree->car;
        if (tree->car) {                // pre
          t = tree->car;
          n = 0;
          while (t) {
            if (n < len) {
              gen_assignment(s, t->car, rhs+n);
              n++;
            }
            else {
              genop(s, MKOP_A(OP_LOADNIL, rhs+n));
              gen_assignment(s, t->car, rhs+n);
            }
            t = t->cdr;
          }
        }
        t = tree->cdr;
        if (t) {
          if (t->cdr) {         // post count
            p = t->cdr->car;
            while (p) {
              post++;
              p = p->cdr;
            }
          }
          if (t->car) {         // rest (len - pre - post)
            int rn;

            if (len < post + n) {
              rn = 0;
            }
            else {
              rn = len - post - n;
            }
            genop(s, MKOP_ABC(OP_ARRAY, cursp(), rhs+n, rn));
            gen_assignment(s, t->car, cursp());
            n += rn;
          }
          if (t->cdr && t->cdr->car) {
            t = t->cdr->car;
            while (n<len) {
              gen_assignment(s, t->car, rhs+n);
              t = t->cdr;
              n++;
            }
          }
        }
        pop_n(len);
        if (val) {
          genop(s, MKOP_ABC(OP_ARRAY, rhs, rhs, len));
          push();
        }
      }
      else {
        // variable rhs
        codegen(s, t);
        gen_vmassignment(s, tree->car, rhs);
        if (!val) {
          pop();
        }
      }
    }
    break;

  case NODE_OP_ASGN:
    {
      mrb_sym sym = nsym(tree->cdr->car);
      mrb_int len;
      const char *name = mrb_sym2name_len(s->mrb, sym, &len);
      int idx, callargs = -1, vsp = -1;

      if ((len == 2 && name[0] == '|' && name[1] == '|') &&
          (nint(tree->car->car) == NODE_CONST ||
           nint(tree->car->car) == NODE_CVAR)) {
        int onerr, noexc, exc;
        struct loopinfo *lp;

        onerr = genop(s, MKOP_Bx(OP_ONERR, 0));
        lp = loop_push(s, LOOP_BEGIN);
        lp->pc1 = onerr;
        exc = cursp();
        codegen(s, tree->car);
        lp->type = LOOP_RESCUE;
        genop(s, MKOP_A(OP_POPERR, 1));
        noexc = genop(s, MKOP_Bx(OP_JMP, 0));
        dispatch(s, onerr);
        genop(s, MKOP_ABC(OP_RESCUE, exc, 0, 0));
        genop(s, MKOP_A(OP_LOADF, exc));
        dispatch(s, noexc);
        loop_pop(s);
      }
      else if (nint(tree->car->car) == NODE_CALL) {
        node *n = tree->car->cdr;
        int base, i, nargs = 0;
        callargs = 0;

        if (val) {
          vsp = cursp();
          push();
        }
        codegen(s, n->car);   // receiver
        idx = new_msym(s, nsym(n->cdr->car));
        base = cursp()-1;
        if (n->cdr->cdr->car) {
          nargs = gen_values(s, n->cdr->cdr->car->car, 1);
          if (nargs >= 0) {
            callargs = nargs;
          }
          else { // varargs
            push();
            nargs = 1;
            callargs = CALL_MAXARGS;
          }
        }
        // copy receiver and arguments
        genop(s, MKOP_AB(OP_MOVE, cursp(), base));
        for (i=0; i<nargs; i++) {
          genop(s, MKOP_AB(OP_MOVE, cursp()+i+1, base+i+1));
        }
        push_n(nargs+2);pop_n(nargs+2); // space for receiver, arguments and a block
        genop(s, MKOP_ABC(OP_SEND, cursp(), idx, callargs));
        push();
      }
      else {
        codegen(s, tree->car);
      }
      if (len == 2 &&
          ((name[0] == '|' && name[1] == '|') ||
           (name[0] == '&' && name[1] == '&'))) {
        int pos;

        pop();
        if (val) {
          if (vsp >= 0) {
            genop(s, MKOP_AB(OP_MOVE, vsp, cursp()));
          }
          pos = genop(s, MKOP_AsBx(name[0]=='|'?OP_JMPIF:OP_JMPNOT, cursp(), 0));
        }
        else {
          pos = genop_peep(s, MKOP_AsBx(name[0]=='|'?OP_JMPIF:OP_JMPNOT, cursp(), 0));
        }
        codegen(s, tree->cdr->cdr->car);
        pop();
        if (val && vsp >= 0) {
          genop(s, MKOP_AB(OP_MOVE, vsp, cursp()));
        }
        if (nint(tree->car->car) == NODE_CALL) {
          if (callargs == CALL_MAXARGS) {
            pop();
            genop(s, MKOP_AB(OP_ARYPUSH, cursp(), cursp()+1));
          }
          else {
            pop_n(callargs);
            callargs++;
          }
          pop();
          idx = new_msym(s, attrsym(s, nsym(tree->car->cdr->cdr->car)));
          genop(s, MKOP_ABC(OP_SEND, cursp(), idx, callargs));
        }
        else {
          gen_assignment(s, tree->car, cursp());
        }
        dispatch(s, pos);
        return;
      }
      codegen(s, tree->cdr->cdr->car);
      push(); pop();
      pop(); pop();

      idx = new_msym(s, sym);
      if (len == 1 && name[0] == '+')  {
        genop_peep(s, MKOP_ABC(OP_ADD, cursp(), idx, 1));
      }
      else if (len == 1 && name[0] == '-')  {
        genop_peep(s, MKOP_ABC(OP_SUB, cursp(), idx, 1));
      }
      else if (len == 1 && name[0] == '*')  {
        genop(s, MKOP_ABC(OP_MUL, cursp(), idx, 1));
      }
      else if (len == 1 && name[0] == '/')  {
        genop(s, MKOP_ABC(OP_DIV, cursp(), idx, 1));
      }
      else if (len == 1 && name[0] == '<')  {
        genop(s, MKOP_ABC(OP_LT, cursp(), idx, 1));
      }
      else if (len == 2 && name[0] == '<' && name[1] == '=')  {
        genop(s, MKOP_ABC(OP_LE, cursp(), idx, 1));
      }
      else if (len == 1 && name[0] == '>')  {
        genop(s, MKOP_ABC(OP_GT, cursp(), idx, 1));
      }
      else if (len == 2 && name[0] == '>' && name[1] == '=')  {
        genop(s, MKOP_ABC(OP_GE, cursp(), idx, 1));
      }
      else {
        genop(s, MKOP_ABC(OP_SEND, cursp(), idx, 1));
      }
      if (callargs < 0) {
        gen_assignment(s, tree->car, cursp());
      }
      else {
        if (val && vsp >= 0) {
          genop(s, MKOP_AB(OP_MOVE, vsp, cursp()));
        }
        if (callargs == CALL_MAXARGS) {
          pop();
          genop(s, MKOP_AB(OP_ARYPUSH, cursp(), cursp()+1));
        }
        else {
          pop_n(callargs);
          callargs++;
        }
        pop();
        idx = new_msym(s, attrsym(s,nsym(tree->car->cdr->cdr->car)));
        genop(s, MKOP_ABC(OP_SEND, cursp(), idx, callargs));
      }
    }
    break;

  case NODE_SUPER:
    {
      codegen_scope *s2 = s;
      int lv = 0;
      int n = 0, noop = 0, sendv = 0;

      push();        // room for receiver
      while (!s2->mscope) {
        lv++;
        s2 = s2->prev;
        if (!s2) break;
      }
      genop(s, MKOP_ABx(OP_ARGARY, cursp(), (lv & 0xf)));
      push(); push();         // ARGARY pushes two values
      pop(); pop();
      if (tree) {
        node *args = tree->car;
        if (args) {
          n = gen_values(s, args, 0);
          if (n < 0) {
            n = noop = sendv = 1;
            push();
          }
        }
      }
      if (tree && tree->cdr) {
        codegen(s, tree->cdr);
        pop();
      }
      else {
        genop(s, MKOP_A(OP_LOADNIL, cursp()));
        push(); pop();
      }
      pop_n(n+1);
      if (sendv) n = CALL_MAXARGS;
      genop(s, MKOP_ABC(OP_SUPER, cursp(), 0, n));
      if (val) push();
    }
    break;

  case NODE_ZSUPER:
    {
      codegen_scope *s2 = s;
      int lv = 0, ainfo = 0;

      push();        // room for receiver
      while (!s2->mscope) {
        lv++;
        s2 = s2->prev;
        if (!s2) break;
      }
      if (s2) ainfo = s2->ainfo;
      genop(s, MKOP_ABx(OP_ARGARY, cursp(), (ainfo<<4)|(lv & 0xf)));
      push(); push(); pop();    // ARGARY pushes two values
      if (tree && tree->cdr) {
        codegen(s, tree->cdr);
        pop();
      }
      pop(); pop();
      genop(s, MKOP_ABC(OP_SUPER, cursp(), 0, CALL_MAXARGS));
      if (val) push();
    }
    break;

  case NODE_RETURN:
    if (tree) {
      gen_retval(s, tree);
    }
    else {
      genop(s, MKOP_A(OP_LOADNIL, cursp()));
    }
    if (s->loop) {
      genop(s, MKOP_AB(OP_RETURN, cursp(), OP_R_RETURN));
    }
    else {
      genop_peep(s, MKOP_AB(OP_RETURN, cursp(), OP_R_NORMAL));
    }
    if (val) push();
    break;

  case NODE_YIELD:
    {
      codegen_scope *s2 = s;
      int lv = 0, ainfo = 0;
      int n = 0, sendv = 0;

      while (!s2->mscope) {
        lv++;
        s2 = s2->prev;
        if (!s2) break;
      }
      if (s2) ainfo = s2->ainfo;
      push();
      if (tree) {
        n = gen_values(s, tree, 0);
        if (n < 0) {
          n = sendv = 1;
          push();
        }
      }
      push();pop(); // space for a block
      pop_n(n+1);
      genop(s, MKOP_ABx(OP_BLKPUSH, cursp(), (ainfo<<4)|(lv & 0xf)));
      if (sendv) n = CALL_MAXARGS;
      genop(s, MKOP_ABC(OP_SEND, cursp(), new_msym(s, mrb_intern_lit(s->mrb, "call")), n));
      if (val) push();
    }
    break;

  case NODE_BREAK:
    append_str(s, "error(\"break\")");
    break;

  case NODE_NEXT:
    if (!s->loop) {
      raise_error(s, "unexpected next");
    }
    else if (s->loop->type == LOOP_NORMAL) {
      if (s->ensure_level > s->loop->ensure_level) {
        genop_peep(s, MKOP_A(OP_EPOP, s->ensure_level - s->loop->ensure_level));
      }
      codegen(s, tree);
      distcheck(s, s->loop->pc1 - s->pc);
      genop(s, MKOP_sBx(OP_JMP, s->loop->pc1 - s->pc));
    }
    else {
      if (tree) {
        codegen(s, tree);
        pop();
      }
      else {
        genop(s, MKOP_A(OP_LOADNIL, cursp()));
      }
      genop_peep(s, MKOP_AB(OP_RETURN, cursp(), OP_R_NORMAL));
    }
    if (val) push();
    break;

  case NODE_REDO:
    if (!s->loop || s->loop->type == LOOP_BEGIN || s->loop->type == LOOP_RESCUE) {
      raise_error(s, "unexpected redo");
    }
    else {
      if (s->ensure_level > s->loop->ensure_level) {
        genop_peep(s, MKOP_A(OP_EPOP, s->ensure_level - s->loop->ensure_level));
      }
      distcheck(s, s->loop->pc2 - s->pc);
      genop(s, MKOP_sBx(OP_JMP, s->loop->pc2 - s->pc));
    }
    if (val) push();
    break;

  case NODE_RETRY:
    {
      const char *msg = "unexpected retry";

      if (!s->loop) {
        raise_error(s, msg);
      }
      else {
        struct loopinfo *lp = s->loop;
        int n = 0;

        while (lp && lp->type != LOOP_RESCUE) {
          if (lp->type == LOOP_BEGIN) {
            n++;
          }
          lp = lp->prev;
        }
        if (!lp) {
          raise_error(s, msg);
        }
        else {
          if (n > 0) {
            genop_peep(s, MKOP_A(OP_POPERR, n));
          }
          if (s->ensure_level > lp->ensure_level) {
            genop_peep(s, MKOP_A(OP_EPOP, s->ensure_level - lp->ensure_level));
          }
          distcheck(s, s->loop->pc1 - s->pc);
          genop(s, MKOP_sBx(OP_JMP, lp->pc1 - s->pc));
        }
      }
      if (val) push();
    }
    break;

  case NODE_LVAR:
    if (val) {
      int idx = lv_idx(s, nsym(tree));

      if (idx > 0) {
        genop_peep(s, MKOP_AB(OP_MOVE, cursp(), idx));
      }
      else {
        int lv = 0;
        codegen_scope *up = s->prev;

        while (up) {
          idx = lv_idx(up, nsym(tree));
          if (idx > 0) {
            genop_peep(s, MKOP_ABC(OP_GETUPVAR, cursp(), idx, lv));
            break;
          }
          lv++;
          up = up->prev;
        }
      }
      push();
    }
    break;

  case NODE_GVAR:
    append_str(s, "_G[\"");
    append_sym(s, nsym(tree));
    append_str(s, "\"]");
    break;

  case NODE_IVAR:
    append_str(s, "self[\"");
    append_sym(s, nsym(tree));
    append_str(s, "\"]");
    break;

  case NODE_CVAR:
    if (val) {
      int sym = new_sym(s, nsym(tree));

      genop(s, MKOP_ABx(OP_GETCV, cursp(), sym));
      push();
    }
    break;

  case NODE_CONST:
    append_str(s, "getmetatable(self)[\"");
    append_sym(s, nsym(tree));
    append_str(s, "\"]");
    break;

  case NODE_DEFINED:
    codegen(s, tree);
    break;

  case NODE_BACK_REF:
    {
      char buf[3];
      int sym;

      buf[0] = '$';
      buf[1] = nchar(tree);
      buf[2] = 0;
      append_str(s, "_G[\"");
      append_sym(s, mrb_intern_cstr(s->mrb, buf));
      append_str(s, "\"]");
    }
    break;

  case NODE_NTH_REF:
    {
      mrb_state *mrb = s->mrb;
      mrb_value str;
      int sym;

      str = mrb_format(mrb, "$%S", mrb_fixnum_value(nint(tree)));
      append_str(s, "_G[\"");
      append_sym(s, mrb_intern_cstr(s->mrb, str));
      append_str(s, "\"]");
    }
    break;

  case NODE_ARG:
    // should not happen
    break;

  case NODE_BLOCK_ARG:
    codegen(s, tree);
    break;

  case NODE_INT:
    if (val) {
      char *p = (char*)tree->car;
      int base = nint(tree->cdr->car);
      mrb_int i;
      mrb_code co;
      mrb_bool overflow;

      i = readint_mrb_int(s, p, base, FALSE, &overflow);
#ifndef MRB_WITHOUT_FLOAT
      if (overflow) {
        double f = readint_float(s, p, base);
        int off = new_lit(s, mrb_float_value(s->mrb, f));

        genop(s, MKOP_ABx(OP_LOADL, cursp(), off));
      }
      else
#endif
      {
        if (i < MAXARG_sBx && i > -MAXARG_sBx) {
          co = MKOP_AsBx(OP_LOADI, cursp(), i);
        }
        else {
          int off = new_lit(s, mrb_fixnum_value(i));
          co = MKOP_ABx(OP_LOADL, cursp(), off);
        }
        genop(s, co);
      }
      push();
    }
    break;

#ifndef MRB_WITHOUT_FLOAT
  case NODE_FLOAT:
    if (val) {
      char *p = (char*)tree;
      mrb_float f = mrb_float_read(p, NULL);
      int off = new_lit(s, mrb_float_value(s->mrb, f));

      genop(s, MKOP_ABx(OP_LOADL, cursp(), off));
      push();
    }
    break;
#endif

  case NODE_NEGATE:
    {
      nt = nint(tree->car);
      tree = tree->cdr;
      switch (nt) {
#ifndef MRB_WITHOUT_FLOAT
      case NODE_FLOAT:
        if (val) {
          char *p = (char*)tree;
          mrb_float f = mrb_float_read(p, NULL);
          int off = new_lit(s, mrb_float_value(s->mrb, -f));

          genop(s, MKOP_ABx(OP_LOADL, cursp(), off));
          push();
        }
        break;
#endif

      case NODE_INT:
        if (val) {
          char *p = (char*)tree->car;
          int base = nint(tree->cdr->car);
          mrb_int i;
          mrb_code co;
          mrb_bool overflow;

          i = readint_mrb_int(s, p, base, TRUE, &overflow);
#ifndef MRB_WITHOUT_FLOAT
          if (overflow) {
            double f = readint_float(s, p, base);
            int off = new_lit(s, mrb_float_value(s->mrb, -f));

            genop(s, MKOP_ABx(OP_LOADL, cursp(), off));
          }
          else {
#endif
            if (i < MAXARG_sBx && i > -MAXARG_sBx) {
              co = MKOP_AsBx(OP_LOADI, cursp(), i);
            }
            else {
              int off = new_lit(s, mrb_fixnum_value(i));
              co = MKOP_ABx(OP_LOADL, cursp(), off);
            }
            genop(s, co);
#ifndef MRB_WITHOUT_FLOAT
          }
#endif
          push();
        }
        break;

      default:
        if (val) {
          int sym = new_msym(s, mrb_intern_lit(s->mrb, "-"));

          genop(s, MKOP_ABx(OP_LOADI, cursp(), 0));
          push();
          codegen(s, tree);
          pop(); pop();
          genop(s, MKOP_ABC(OP_SUB, cursp(), sym, 2));
        }
        else {
          codegen(s, tree);
        }
        break;
      }
    }
    break;

  case NODE_STR:
    if (val) {
      char *p = (char*)tree->car;
      size_t len = (intptr_t)tree->cdr;
      int ai = mrb_gc_arena_save(s->mrb);
      int off = new_lit(s, mrb_str_new(s->mrb, p, len));

      mrb_gc_arena_restore(s->mrb, ai);
      genop(s, MKOP_ABx(OP_STRING, cursp(), off));
      push();
    }
    break;

  case NODE_HEREDOC:
    tree = ((struct mrb_parser_heredoc_info *)tree)->doc;
    // fall through
  case NODE_DSTR:
    if (val) {
      node *n = tree;

      if (!n) {
        genop(s, MKOP_A(OP_LOADNIL, cursp()));
        push();
        break;
      }
      codegen(s, n->car);
      n = n->cdr;
      while (n) {
        codegen(s, n->car);
        pop(); pop();
        genop_peep(s, MKOP_AB(OP_STRCAT, cursp(), cursp()+1));
        push();
        n = n->cdr;
      }
    }
    else {
      node *n = tree;

      while (n) {
        if (nint(n->car->car) != NODE_STR) {
          codegen(s, n->car);
        }
        n = n->cdr;
      }
    }
    break;

  case NODE_WORDS:
    gen_literal_array(s, tree, FALSE);
    break;

  case NODE_SYMBOLS:
    gen_literal_array(s, tree, TRUE);
    break;

  case NODE_DXSTR:
    {
      node *n;
      int ai = mrb_gc_arena_save(s->mrb);
      int sym = new_sym(s, mrb_intern_lit(s->mrb, "Kernel"));

      genop(s, MKOP_A(OP_LOADSELF, cursp()));
      push();
      codegen(s, tree->car);
      n = tree->cdr;
      while (n) {
        if (nint(n->car->car) == NODE_XSTR) {
          n->car->car = (struct mrb_ast_node*)(intptr_t)NODE_STR;
          mrb_assert(!n->cdr); // must be the end
        }
        codegen(s, n->car);
        pop(); pop();
        genop_peep(s, MKOP_AB(OP_STRCAT, cursp(), cursp()+1));
        push();
        n = n->cdr;
      }
      push();                   // for block
      pop_n(3);
      sym = new_sym(s, mrb_intern_lit(s->mrb, "`"));
      genop(s, MKOP_ABC(OP_SEND, cursp(), sym, 1));
      if (val) push();
      mrb_gc_arena_restore(s->mrb, ai);
    }
    break;

  case NODE_XSTR:
    {
      char *p = (char*)tree->car;
      size_t len = (intptr_t)tree->cdr;
      int ai = mrb_gc_arena_save(s->mrb);
      int off = new_lit(s, mrb_str_new(s->mrb, p, len));
      int sym;

      genop(s, MKOP_A(OP_LOADSELF, cursp()));
      push();
      genop(s, MKOP_ABx(OP_STRING, cursp(), off));
      push(); push();
      pop_n(3);
      sym = new_sym(s, mrb_intern_lit(s->mrb, "`"));
      genop(s, MKOP_ABC(OP_SEND, cursp(), sym, 1));
      if (val) push();
      mrb_gc_arena_restore(s->mrb, ai);
    }
    break;

  case NODE_REGX:
    if (val) {
      char *p1 = (char*)tree->car;
      char *p2 = (char*)tree->cdr->car;
      char *p3 = (char*)tree->cdr->cdr;
      int ai = mrb_gc_arena_save(s->mrb);
      int sym = new_sym(s, mrb_intern_lit(s->mrb, REGEXP_CLASS));
      int off = new_lit(s, mrb_str_new_cstr(s->mrb, p1));
      int argc = 1;

      genop(s, MKOP_A(OP_OCLASS, cursp()));
      genop(s, MKOP_ABx(OP_GETMCNST, cursp(), sym));
      push();
      genop(s, MKOP_ABx(OP_STRING, cursp(), off));
      push();
      if (p2 || p3) {
        if (p2) { // opt
          off = new_lit(s, mrb_str_new_cstr(s->mrb, p2));
          genop(s, MKOP_ABx(OP_STRING, cursp(), off));
        }
        else {
          genop(s, MKOP_A(OP_LOADNIL, cursp()));
        }
        push();
        argc++;
        if (p3) { // enc
          off = new_lit(s, mrb_str_new(s->mrb, p3, 1));
          genop(s, MKOP_ABx(OP_STRING, cursp(), off));
          push();
          argc++;
        }
      }
      push(); // space for a block
      pop_n(argc+2);
      sym = new_sym(s, mrb_intern_lit(s->mrb, "compile"));
      genop(s, MKOP_ABC(OP_SEND, cursp(), sym, argc));
      mrb_gc_arena_restore(s->mrb, ai);
      push();
    }
    break;

  case NODE_DREGX:
    if (val) {
      node *n = tree->car;
      int ai = mrb_gc_arena_save(s->mrb);
      int sym = new_sym(s, mrb_intern_lit(s->mrb, REGEXP_CLASS));
      int argc = 1;
      int off;
      char *p;

      genop(s, MKOP_A(OP_OCLASS, cursp()));
      genop(s, MKOP_ABx(OP_GETMCNST, cursp(), sym));
      push();
      codegen(s, n->car);
      n = n->cdr;
      while (n) {
        codegen(s, n->car);
        pop(); pop();
        genop_peep(s, MKOP_AB(OP_STRCAT, cursp(), cursp()+1));
        push();
        n = n->cdr;
      }
      n = tree->cdr->cdr;
      if (n->car) { // tail
        p = (char*)n->car;
        off = new_lit(s, mrb_str_new_cstr(s->mrb, p));
        codegen(s, tree->car);
        genop(s, MKOP_ABx(OP_STRING, cursp(), off));
        pop();
        genop_peep(s, MKOP_AB(OP_STRCAT, cursp(), cursp()+1));
        push();
      }
      if (n->cdr->car) { // opt
        char *p2 = (char*)n->cdr->car;
        off = new_lit(s, mrb_str_new_cstr(s->mrb, p2));
        genop(s, MKOP_ABx(OP_STRING, cursp(), off));
        push();
        argc++;
      }
      if (n->cdr->cdr) { // enc
        char *p2 = (char*)n->cdr->cdr;
        off = new_lit(s, mrb_str_new_cstr(s->mrb, p2));
        genop(s, MKOP_ABx(OP_STRING, cursp(), off));
        push();
        argc++;
      }
      push(); // space for a block
      pop_n(argc+2);
      sym = new_sym(s, mrb_intern_lit(s->mrb, "compile"));
      genop(s, MKOP_ABC(OP_SEND, cursp(), sym, argc));
      mrb_gc_arena_restore(s->mrb, ai);
      push();
    }
    else {
      node *n = tree->car;

      while (n) {
        if (nint(n->car->car) != NODE_STR) {
          codegen(s, n->car);
        }
        n = n->cdr;
      }
    }
    break;

  case NODE_SYM:
    append_str(s, "\"");
    append_sym(s, nsym(tree));
    append_str(s, "\"");
    break;

  case NODE_DSYM:
    append_str(s, "intern(");
    codegen(s, tree);
    append_str(s, ")");
    break;

  case NODE_SELF:
    append_str(s, "(self)");
    break;

  case NODE_NIL:
    append_str(s, "(nil)");
    break;

  case NODE_TRUE:
    append_str(s, "(true)");
    break;

  case NODE_FALSE:
    append_str(s, "(false)");
    break;

  case NODE_ALIAS:
    append_str(s, "metatable(self):alias_method(\"");
    append_sym(s, nsym(tree->car));
    append_str(s, "\", \"");
    append_sym(s, nsym(tree->cdr));
    append_str(s, "\")");
    break;

  case NODE_UNDEF:
    append_str(s, "self:undef_method(");
    {
      node *t = tree;
      int num = 0;
      while (t) {
        if (num > 0) {
          append_str(s, ", ");
        }
        append_str(s, "\"");
        append_sym(s, nsym(t->car));
        append_str(s, "\"");
        t = t->cdr;
        num++;
      }
    }
    append_str(s, "\")");
    break;

  case NODE_CLASS:
    append_str(s, "do\nlocal _parent_class = (");
    if (tree->car->car == (node*)0) {
      append_str(s, "self");
    }
    else if (tree->car->car == (node*)1) {
      append_str(s, "Object");
    }
    else {
      codegen(s, tree->car->car);
    }
    append_str(s, ");\n");

    append_str(s, "local _cls = _parent_class[\"");
    append_sym(s, nsym(tree->car->cdr));
    append_str(s, "\"] = {};\n");
    append_str(s, "(");
    scope_body(s, tree->cdr->car);
    append_str(s, ")(_cls)\n end\n");
    break;

  case NODE_MODULE:
    append_str(s, "do\nlocal _parent_module = (");
    if (tree->car->car == (node*)0) {
      append_str(s, "self");
    }
    else if (tree->car->car == (node*)1) {
      append_str(s, "Object");
    }
    else {
      codegen(s, tree->car->car);
    }
    append_str(s, ");\n");

    append_str(s, "local _mod = _parent_module[\"");
    append_sym(s, nsym(tree->car->cdr));
    append_str(s, "\"] = {};\n");
    append_str(s, "(");
    scope_body(s, tree->cdr->car);
    append_str(s, ")(_mod)\n end\n");
    break;

  case NODE_SCLASS:
    append_str(s, "(");
    scope_body(s, tree->cdr->car);
    append_str(s, ")(");
    codegen(s, tree->car);
    append_str(s, ")");
    break;

  case NODE_DEF:
    append_str(s, "getmetatable(self)[\"");
    append_sym(s, nsym(tree->car));
    append_str(s, "\"] = ");
    lambda_body(s, tree->cdr->cdr);
    append_str(s, "; \"");
    append_sym(s, nsym(tree->car));
    append_str(s, "\"");
    break;

  case NODE_SDEF:
    append_str(s, "(");
    codegen(s, tree->car);
    append_str(s, ")[\"");
    append_sym(s, nsym(tree->cdr->car));
    append_str(s, "\"] = ");
    lambda_body(s, tree->cdr->cdr);
    append_str(s, "; \"");
    append_sym(s, nsym(tree->cdr->car));
    append_str(s, "\"");
    break;

  case NODE_POSTEXE:
    codegen(s, tree);
    break;

  default:
    break;
  }
}

MRB_API struct RProc*
mrb_generate_code(mrb_state *mrb, parser_state *p)
{
  code_generator gen = { 0 };
  codegen(&gen, p->tree);
  // return generate_code(mrb, p);
}

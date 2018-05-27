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

static void append_str_len(code_generator *g, const char *str, size_t str_len) {
  if ((g->str_len + str_len) > g->str_capa) {
    g->str_capa *= 3; // * 1.5
    g->str_capa /= 2;
    g->str = mrb_realloc(g->mrb, g->str, g->str_capa);
  }
  memcpy(g->str + g->str_len, str, str_len);
  g->str_len += str_len;
}

static void append_str(code_generator *s, const char *str) {
  append_str_len(s, str, strlen(str));
}

static void append_sym(code_generator *g, GCstr *str) {
  append_str(g, strdata(str));
}

static void codegen(code_generator *g, node *tree);

static void gen_values(code_generator *s, node *tree) {
  for (int n = 0; tree; tree = tree->cdr, ++n) {
    if (n > 0) { append_str(s, ", "); }
    codegen(s, tree->car);
  }
}

static void
gen_assignment(code_generator *s, node *tree)
{
  int type = nint(tree->car);

  tree = tree->cdr;
  switch (type) {
  case NODE_GVAR:
    append_sym(s, nsym(tree));
    break;
  case NODE_LVAR:
    append_sym(s, nsym(tree));
    break;
  case NODE_IVAR:
    append_str(s, "self[\"");
    append_sym(s, nsym(tree));
    append_str(s, "\"]");
    break;
  case NODE_CVAR:
    append_str(s, "getmetatable(self)[\"");
    append_sym(s, nsym(tree));
    append_str(s, "\"]");
    break;
  case NODE_CONST:
    append_str(s, "getmetatable(self)[\"");
    append_sym(s, nsym(tree));
    append_str(s, "\"]");
    break;
  case NODE_COLON2:
    append_sym(s, nsym(tree->cdr));
    append_str(s, ".");
    codegen(s, tree->car);
    break;

  case NODE_CALL:
  case NODE_SCALL:
    append_str(s, " do\nlocal _res = ");
    codegen(s, tree);
    append_str(s, " ;\n _res[\"");
    append_sym(s, nsym(tree->cdr->car));
    append_str(s, "=\"]");
    append_str(s, "(");
    break;

  case NODE_MASGN:
    gen_vmassignment(s, tree->car);
    break;

  /* splat without assignment */
  case NODE_NIL:
    break;

  default:
#ifndef MRB_DISABLE_STDIO
    fprintf(stderr, "unknown lhs %d\n", type);
#endif
    break;
  }
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
    append_str(s, "do\n");
    append_str(s, "lcaol __ok, __err = pcall(function()\n");
    codegen(s, tree->car);
    tree = tree->cdr;
    if (tree->car) {
      node *rescues = tree->car;
      while (rescues) {
        node *rescue = rescues->car;
        node *exc_list = rescue->car;

        if (rescues != tree->car) { append_str(s, " else\n"); }

        append_str(s, "if (");
        do {
          if (exc_list != rescue->car) { append_str(s, " or "); }
          append_str(s, "(");

          if (exc_list && exc_list->car && nint(exc_list->car->car) == NODE_SPLAT) {
            append_str(s, "__err:__case_eqq(");
            codegen(s, exc_list->car);
            append_str(s, ")");
          }
          else {
            if (exc_list) {
              append_str(s, "__err[\"kind_of?\"](__err, (");
              codegen(s, exc_list->car);
              append_str(s, "))");
            }
            else {
              append_str(s, "__err[\"kind_of?\"](__err, StandardError)");
            }
          }
          exc_list = exc_list->cdr;
          append_str(s, ")");
        } while (exc_list);
        append_str(s, ")\n");

        // exc_var
        if (rescue->cdr->car) {
          append_str(s, "local ");
          gen_assignment(s, rescue->cdr->car);
          append_str(s, " = __err;\n");
        }
        if (rescue->cdr->cdr->car) {
          codegen(s, rescue->cdr->cdr->car);
        }
        rescues = rescues->cdr;
      }

      // else
      tree = tree->cdr;
      if (tree->car) {
        append_str(s, " else \n");
        codegen(s, tree->car);
      }

      append_str(s, " end \n");
    }

    append_str(s, ")\n");
    append_str(s, "end\n");
    break;

  case NODE_ENSURE:
    if (!tree->cdr || !tree->cdr->cdr ||
        (nint(tree->cdr->cdr->car) == NODE_BEGIN &&
         tree->cdr->cdr->cdr)) {
      append_str(s, "local _ok, _err = pcall(function()\n");
      codegen(s, tree->car);
      append_str(s, "end);\n");
      codegen(s, tree->cdr);
      append_str(s, "if !_ok then error(_err) end\n");
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
      append_str(s, ") then ");
      codegen(s, tree->cdr->car);
      if (e) {
        append_str(s, " else ");
        codegen(s, e);
      }
      append_str(s, " end ");
    }
    break;

  case NODE_AND:
    append_str(s, "((");
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
    break;

  case NODE_CASE:
    {
      node *expr = NULL;

      append_str(s, " do\n");
      if (tree->car) {
        append_str(s, "local __exp = ");
        codegen(s, tree->car);
        append_str(s, ";\n");
        expr = tree->car;
      }

      for (tree = tree->cdr; tree; tree = tree->cdr) {
        if (tree->car->car) {
          const char *prefix = NULL;
          for (node *n = tree->car->car; n; n = n->cdr) {
            if (!prefix) {
              append_str(s, "if ");
              prefix = " or ";
            } else {
              append_str(s, prefix);
            }
            if (nint(n->car->car) == NODE_SPLAT) {
              append_str(s, "(__exp:__case_eqq(");
              codegen(s, n->car);
              append_str(s, "))");
            }
            else {
              append_str(s, "(__exp[\"===\"](__exp, ");
              codegen(s, n->car);
              append_str(s, "))");
            }
          }
        } else {
          append_str(s, " else\n");
        }

        // body
        codegen(s, tree->car->cdr);
      }
      append_str(s, "  end\nend\n");
    }
    break;

  case NODE_SCOPE:
    scope_body(s, tree);
    break;

  case NODE_FCALL:
  case NODE_CALL:
    gen_call(s, tree, 0, 0, 0);
    break;
  case NODE_SCALL:
    gen_call(s, tree, 0, 0, 1);
    break;

  case NODE_DOT2:
    append_str(s, "Range.new((");
    codegen(s, tree->car);
    append_str(s, "), (");
    codegen(s, tree->cdr);
    append_str(s, "))");
    break;

  case NODE_DOT3:
    append_str(s, "Range.new((");
    codegen(s, tree->car);
    append_str(s, "), (");
    codegen(s, tree->cdr);
    append_str(s, "), true)");
    break;

  case NODE_COLON2:
    append_str(s, "(");
    codegen(s, tree->car);
    append_str(s, ")[\"");
    append_sym(s, nsym(tree->cdr));
    append_str(s, "\"]");
    break;

  case NODE_COLON3:
    append_str(s, "(Object.\"");
    append_sym(s, nsym(tree));
    append_str(s, "\")");
    break;

  case NODE_ARRAY:
    append_str(s, "{");
    gen_values(s, tree);
    append_str(s, "}");
    break;

  case NODE_HASH:
    append_str(s, "{");
    for (int n = 0; tree; tree = tree->cdr, ++n) {
      if (n > 0) { append_str(s, ", "); }
      append_str(s, "[");
      codegen(s, tree->car->car);
      append_str(s, "] = ");
      codegen(s, tree->car->cdr);
    }
    append_str(s, "}");
    break;

  case NODE_SPLAT:
    append_str(s, "{");
    codegen(s, tree);
    append_str(s, "}");
    break;

  case NODE_ASGN:
    gen_assignment(s, tree->car);
    append_str(s, " = ");
    codegen(s, tree->cdr);
    append_str(s, ";\n");
    break;

  case NODE_MASGN:
    {
      int len = 0, n = 0, post = 0;
      node *t = tree->cdr, *p;

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
              gen_assignment(s, t->car);
              n++;
            }
            else {
              genop(s, MKOP_A(OP_LOADNIL, rhs+n));
              gen_assignment(s, t->car);
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
            gen_assignment(s, t->car);
            n += rn;
          }
          if (t->cdr && t->cdr->car) {
            t = t->cdr->car;
            while (n<len) {
              gen_assignment(s, t->car);
              t = t->cdr;
              n++;
            }
          }
        }
      }
      else {
        // variable rhs
        codegen(s, t);
        gen_vmassignment(s, tree->car, rhs);
      }
    }
    break;

  case NODE_OP_ASGN:
    {
      mrb_sym sym = nsym(tree->cdr->car);
      mrb_int len;
      const char *name = mrb_sym2name_len(s->mrb, sym, &len);

      append_str(s, " do\n");

      if (nint(tree->car->car) != NODE_CALL) {
        append_str(s, " local __rhs = ");
        codegen(s, tree->cdr->cdr->car);
        append_str(s, ";\n");

        append_str(s, " local __lhs = ");
        codegen(s, tree->car);
        append_str(s , ";\n");
      }

      if (len == 2 &&
          ((name[0] == '|' && name[1] == '|') ||
           (name[0] == '&' && name[1] == '&'))) {
        append_str(s, " if ");
        if (name[0] == '|') { append_str(s, " !("); }
        append_str(s, "__lhs");
        if (name[0] == '|') { append_str(s, ")"); }
        codegen(s, tree->car);
        append_str(s, " = __lhs ");
        append_sym(s, sym);
        append_str(s, " __rhs;\n");
        append_str(s, " end ");
      }
      else if ((len == 2 && name[0] == '|' && name[1] == '|') &&
          (nint(tree->car->car) == NODE_CONST ||
           nint(tree->car->car) == NODE_CVAR)) {
        append_str(s, "if !(__lhs)\n");
        codegen(s, tree->car);
        append_str(s, " = __lhs");
        append_sym(s, sym);
        append_str(s, " __rhs;\n");
        append_str(s, " end\n");
      }
      else if (nint(tree->car->car) == NODE_CALL) {
        node *n = tree->car->cdr;

        append_str(s, " local __recv = ");
        codegen(s, n->car);
        append_str(s, ";\n");

        append_str(s, "local __lhs = __recv[\"");
        append_sym(s, nsym(n->cdr->car));
        append_str(s, "\"](__recv);\n");

        append_str(s, "__recv[\"");
        append_sym(s, nsym(n->cdr->car));
        append_str(s, "=\"](__recv, ");
        gen_values(s, n->cdr->cdr->car->car);
        append_sym(s, sym);
        append_str(s, " __rhs);");
      }
      else {
        codegen(s, tree->car);
        append_str(s, " = __lhs ");
        append_sym(s, sym);
        append_str(s, " __rhs;\n");
      }

      append_str(s, " end\n");
    }
    break;

  case NODE_SUPER:
    {
      append_str(s, "self:super(__func.name)(self");
      // arguments
      if (tree->car) {
        append_str(s, ", ");
        gen_values(s, tree->car);
      }
      // block
      if (tree && tree->cdr) {
        append_str(s, ", ");
        codegen(s, tree->cdr);
      }
      append_str(s, ")");
    }
    break;

  case NODE_ZSUPER:
    append_str(s, "self:super(__func.name)(self, ..., __blk)");
    break;

  case NODE_RETURN:
    append_str(s, "return (");
    if (tree) {
      append_str(s, "error({\"return\", (");
      codegen(s, tree);
      append_str(s, ")})");
    }
    else {
      append_str(s, "error({\"return\", nil})");
    }
    break;

  case NODE_YIELD:
    append_str(s, " _blk(");
    if (tree) {
      gen_values(s, tree);
    }
    append_str(s, ")");
    break;

  case NODE_BREAK:
    append_str(s, "error(\"break\")");
    break;

  case NODE_NEXT:
    append_str(s, " return (");
    codegen(s, tree);
    append_str(s, ");");
    break;

  case NODE_REDO:
    append_str(s, "error(\"redo\");\n");
    break;

  case NODE_RETRY:
    append_str(s, "error(\"retry\");\n");
    break;

  case NODE_LVAR:
    append_str(s, "(");
    append_sym(s, nsym(tree));
    append_str(s, ")");
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
    append_str(s, "getmetatable(self)[\"");
    append_sym(s, nsym(tree));
    append_str(s, "\"]");
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

      str = mrb_format(mrb, "$%S", mrb_fixnum_value(nint(tree)));
      append_str(s, "_G[\"");
      append_sym(s, mrb_intern_str(s->mrb, str));
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
    {
      char buf[10];
      int base = nint(tree->cdr->car);
      sprintf(buf, "%d", base);
      append_str(s, " tonumber(\"");
      append_str(s, (char*)tree->car);
      append_str(s, "\", ");
      append_str(s, buf);
      append_str(s, ")");
    }
    break;

  case NODE_FLOAT:
    append_str(s, (char*)tree);
    break;

  case NODE_NEGATE:
    append_str(s, " -(");
    codegen(s, tree);
    append_str(s, ")");
    nt = nint(tree->car);
    tree = tree->cdr;
    break;

  case NODE_STR:
    append_str(s, "String:new(\"");
    append_str_len(s, (char*)tree->car, (intptr_t)tree->cdr);
    append_str(s, "\")");
    break;

  case NODE_HEREDOC:
    tree = ((struct mrb_parser_heredoc_info *)tree)->doc;
    // fall through
  case NODE_DSTR:
    {
      node *n = tree;

      append_str(s, "(\"");
      codegen(s, n->car);
      n = n->cdr;
      while (n) {
        append_str(s, "..");
        codegen(s, n->car);
        n = n->cdr;
      }
      append_str(s, "\").to_s()");
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

      append_str(s, "(Kernel[\"`\"](\"");
      codegen(s, tree->car);
      n = tree->cdr;
      while (n) {
        append_str(s, "..");
        if (nint(n->car->car) == NODE_XSTR) {
          n->car->car = (struct mrb_ast_node*)(intptr_t)NODE_STR;
          mrb_assert(!n->cdr); // must be the end
        }
        codegen(s, n->car);
        n = n->cdr;
      }
      append_str(s, "\"))");
    }
    break;

  case NODE_XSTR:
    append_str(s, "self[\"`\"](self, \"");
    append_str_len(s, (char const*)tree->car, nint(tree->cdr));
    append_str(s, "\")");
    break;

  case NODE_REGX:
    {
      char *p1 = (char*)tree->car;
      char *p2 = (char*)tree->cdr->car;
      char *p3 = (char*)tree->cdr->cdr;

      append_str(s, "(Regexp.compile(\"");
      append_str(s, p1);
      append_str(s, "\"");
      if (p2) {
        append_str(s, ", \"");
        append_str(s, p2);
        append_str(s, "\"");

        if (p3) {
          append_str(s, ", \"");
          append_str(s, p3);
          append_str(s, "\"");
        }
      }
      append_str(s, "))");
    }
    break;

  case NODE_DREGX:
    {
      append_str(s, "(Regexp.compile(");
      int num = 0;
      append_str(s, "(");
      for (node *n = tree; n; n = n->cdr, ++num) {
        if (num > 0) { append_str(s, ".."); }
        codegen(s, n->car);
      }
      node *n = tree->cdr->cdr;
      if (n->car) { // tail
        append_str(s, "..");
        codegen(s, tree->cdr->cdr->car);
      }
      append_str(s, ")");

      if (n->cdr->car) { // opt
        char *p2 = (char*)n->cdr->car;
        append_str(s, ", \"");
        append_str(s, p2);
        append_str(s, "\"");

        if (n->cdr->cdr) { // enc
          char *p2 = (char*)n->cdr->cdr;
          append_str(s, ", \"");
          append_str(s, p2);
          append_str(s, "\"");
        }
      }
      append_str(s, "))");
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

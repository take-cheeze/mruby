/*
** rom_graph.c - classes as const data (MRB_ROM_CLASS_GRAPH)
**
** See Copyright Notice in mruby.h
*/

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/gc.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/rom_graph.h>
#include <mruby/internal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mrb_rom_graph_disabled = 0;

#ifdef MRB_ROM_CLASS_GRAPH

struct mrb_mt_tbl *const mrb_rom_graph_no_mt = NULL;

/* One state at a time runs on the graph: its RAM parts are not per state. */
static mrb_state *rom_owner;
static const struct mrb_rom_graph *rom_graph;
/* the graph's const tables (variable.c, IV_ROM_P) */
const void *mrb_rom_iv_lo, *mrb_rom_iv_hi;

const struct mrb_rom_graph*
mrb_rom_graph_claim(mrb_state *mrb)
{
  const struct mrb_rom_graph *g = &mrb_rom_class_graph;
  if (g == NULL || mrb_rom_graph_disabled || rom_graph != NULL) return NULL;
  if (g->abi != MRB_ROM_GRAPH_ABI) return NULL;
  rom_owner = mrb;
  rom_graph = g;
  mrb_rom_iv_lo = g->iv;
  mrb_rom_iv_hi = g->iv + g->niv;
  return g;
}

MRB_API void
mrb_rom_class_frozen(mrb_state *mrb, struct RClass *c)
{
  if (mrb == NULL) mrb = rom_owner;
  mrb_raisef(mrb, E_FROZEN_ERROR, "can't modify %v: it is in the ROM class graph", mrb_obj_value(c));
}


/* COW: a ROM class's const table is copied to the heap on its first store that changes it; the copy is found by the
   const table's address (mrb_rom_graph_iv_redirect), and is a root. Few classes change, so a sorted array does. */
struct rom_ovl {
  const struct mrb_rom_iv *rom;
  struct mrb_rom_iv *t;
};
static struct rom_ovl *rom_ovl;
static int rom_novl, rom_ovl_capa;

static int
ovl_index(const struct mrb_rom_iv *rom)
{
  int lo = 0, hi = rom_novl;
  while (lo < hi) {
    int mid = (lo + hi) / 2;
    if (rom_ovl[mid].rom < rom) lo = mid + 1;
    else hi = mid;
  }
  return lo;
}

struct iv_tbl*
mrb_rom_graph_iv_lookup(struct iv_tbl *t)
{
  const struct mrb_rom_iv *rom = (const struct mrb_rom_iv*)t;
  int i = ovl_index(rom);
  return (i < rom_novl && rom_ovl[i].rom == rom) ? (struct iv_tbl*)rom_ovl[i].t : t;
}

/* `t` (heap) takes the place of const table `rom` from now on. */
void
mrb_rom_graph_iv_cow(mrb_state *mrb, struct iv_tbl *rom, struct iv_tbl *t)
{
  if (rom_novl == rom_ovl_capa) {
    rom_ovl_capa = rom_ovl_capa ? rom_ovl_capa * 2 : 16;
    rom_ovl = (struct rom_ovl*)mrb_realloc(mrb, rom_ovl, sizeof(struct rom_ovl) * rom_ovl_capa);
  }
  int i = ovl_index((const struct mrb_rom_iv*)rom);
  memmove(&rom_ovl[i + 1], &rom_ovl[i], sizeof(struct rom_ovl) * (rom_novl - i));
  rom_ovl[i].rom = (const struct mrb_rom_iv*)rom;
  rom_ovl[i].t = (struct mrb_rom_iv*)t;
  rom_novl++;
}

/* Whether `t` is one of the graph's const tables. */
mrb_bool
mrb_rom_graph_iv_rom_p(const struct iv_tbl *t)
{
  return (const void*)t >= mrb_rom_iv_lo && (const void*)t < mrb_rom_iv_hi;
}

/* The heap objects a ROM class reaches are in its RAM parts only; marked as roots, at the start of marking and again
   in the final phase, since a store into a ROM class has no write barrier (the class is never black). */
void
mrb_rom_graph_mark(mrb_state *mrb)
{
  const struct mrb_rom_graph *g = rom_graph;
  if (g == NULL || rom_owner != mrb) return;
  for (uint32_t i = 0; i < g->nmt; i++) {
    for (mrb_mt_tbl *t = g->mt[i]; t; t = t->next) {
      if (t->alloc & MRB_MT_READONLY_BIT) continue;
      for (int k = 0; k < t->size; k++) {
        if (t->ptr[k].key != 0 && (t->ptr[k].flags & MRB_MT_FUNC) == 0) {
          mrb_gc_mark(mrb, (struct RBasic*)t->ptr[k].val.proc);
        }
      }
    }
  }
  for (int i = 0; i < rom_novl; i++) {
    struct mrb_rom_iv *t = rom_ovl[i].t;
    for (int k = 0; k < t->size; k++) {
      mrb_gc_mark_value(mrb, t->ptr[k]);
    }
  }
}

void mrb_rom_graph_mt_free(mrb_state *mrb, mrb_mt_tbl *t);

/* mrb_close: what the boot hung on the RAM parts. The graph stays claimed: a later state's boot would find the
   method table slots of this one's. */
void
mrb_rom_graph_release(mrb_state *mrb)
{
  const struct mrb_rom_graph *g = rom_graph;
  if (g == NULL || rom_owner != mrb) return;
  for (uint32_t i = 0; i < g->nmt; i++) {
    mrb_rom_graph_mt_free(mrb, g->mt[i]);
    g->mt[i] = NULL;
  }
  for (int i = 0; i < rom_novl; i++) {
    mrb_free(mrb, rom_ovl[i].t->ptr);
    mrb_free(mrb, rom_ovl[i].t);
  }
  mrb_free(mrb, rom_ovl);
  rom_ovl = NULL;
  rom_novl = rom_ovl_capa = 0;
  mrb_rom_iv_lo = mrb_rom_iv_hi = NULL;
  rom_owner = NULL;
}

#endif /* MRB_ROM_CLASS_GRAPH */

MRB_API mrb_bool
mrb_rom_graph_active(mrb_state *mrb)
{
#ifdef MRB_ROM_CLASS_GRAPH
  return rom_graph != NULL && rom_owner == mrb;
#else
  (void)mrb;
  return FALSE;
#endif
}

MRB_API void
mrb_rom_graph_each(mrb_state *mrb, mrb_each_object_callback *func, void *data)
{
#ifndef MRB_ROM_CLASS_GRAPH
  (void)mrb; (void)func; (void)data;
#else
  const struct mrb_rom_graph *g = rom_graph;
  if (g == NULL || rom_owner != mrb) return;
  for (uint32_t i = 0; i < g->nclasses; i++) {
    if (func(mrb, (struct RBasic*)&g->classes[i], data) == MRB_EACH_OBJ_BREAK) return;
  }
#endif
}

/* ---- the dump ---- */

enum dump_iv { DUMP_IV_NONE, DUMP_IV_ATTACHED, DUMP_IV_ROM };

struct dump_class {
  struct RClass *c;
  char *key;          /* sort key: the path, "#<Class:KEY>", or OWNER|POSITION for an iclass */
  int in;             /* in the graph */
  int index;
  int mt;             /* slot index, or -1 */
  enum dump_iv iv;
  int ivx;            /* table index */
};

struct dump {
  mrb_state *mrb;
  mrb_rom_graph_out *out;
  void *ud;
  struct dump_class *v;
  int n, capa;
  char line[512];
};

static void
dump_printf(struct dump *d, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(d->line, sizeof(d->line), fmt, ap);
  va_end(ap);
  d->out(d->ud, d->line);
}

static char*
dump_strdup(mrb_state *mrb, const char *s, size_t len)
{
  char *p = (char*)mrb_malloc(mrb, len + 1);
  memcpy(p, s, len);
  p[len] = '\0';
  return p;
}

static int
dump_collect(mrb_state *mrb, struct RBasic *o, void *data)
{
  struct dump *d = (struct dump*)data;
  if (o->tt != MRB_TT_CLASS && o->tt != MRB_TT_MODULE && o->tt != MRB_TT_SCLASS && o->tt != MRB_TT_ICLASS) {
    return MRB_EACH_OBJ_OK;
  }
  if (d->n == d->capa) {
    d->capa = d->capa ? d->capa * 2 : 256;
    d->v = (struct dump_class*)mrb_realloc(mrb, d->v, sizeof(struct dump_class) * d->capa);
  }
  struct dump_class *e = &d->v[d->n++];
  memset(e, 0, sizeof(*e));
  e->c = (struct RClass*)o;
  e->mt = -1;
  e->ivx = -1;
  return MRB_EACH_OBJ_OK;
}

static struct dump_class*
dump_find(struct dump *d, const struct RClass *c)
{
  for (int i = 0; i < d->n; i++) {
    if (d->v[i].c == c) return &d->v[i];
  }
  return NULL;
}

static int
dump_in(struct dump *d, const struct RClass *c)
{
  struct dump_class *e = dump_find(d, c);
  return e && e->in;
}

/* `path` resolves, constant by constant from Object, to `c`: a definition of it in the next boot finds it. */
static mrb_bool
dump_path_resolves(mrb_state *mrb, const char *path, struct RClass *c)
{
  struct RClass *k = mrb->object_class;
  const char *p = path;
  for (;;) {
    const char *e = strstr(p, "::");
    size_t len = e ? (size_t)(e - p) : strlen(p);
    mrb_sym s = mrb_intern_check(mrb, p, len);
    if (s == 0) return FALSE;
    mrb_value v = mrb_obj_iv_get(mrb, (struct RObject*)k, s);
    /* a top-level name may be BasicObject's own (BasicObject is) */
    for (struct RClass *a = k->super; p == path && mrb_nil_p(v) && a; a = a->super) {
      v = mrb_obj_iv_get(mrb, (struct RObject*)a, s);
    }
    if (mrb_immediate_p(v)) return FALSE;
    if (mrb_type(v) != MRB_TT_CLASS && mrb_type(v) != MRB_TT_MODULE) return FALSE;
    k = mrb_class_ptr(v);
    if (!e) break;
    p = e + 2;
  }
  return k == c;
}

static struct RClass*
dump_attached(mrb_state *mrb, struct RClass *sc)
{
  mrb_value a = mrb_obj_iv_get(mrb, (struct RObject*)sc, MRB_SYM(__attached__));
  if (mrb_immediate_p(a)) return NULL;
  enum mrb_vtype tt = mrb_type(a);
  if (tt != MRB_TT_CLASS && tt != MRB_TT_MODULE && tt != MRB_TT_SCLASS) return NULL;
  return mrb_class_ptr(a);
}

static void
dump_key_sclasses(struct dump *d)
{
  mrb_state *mrb = d->mrb;
  int changed = 1;
  while (changed) {
    changed = 0;
    for (int i = 0; i < d->n; i++) {
      struct dump_class *e = &d->v[i];
      if (e->key || e->c->tt != MRB_TT_SCLASS) continue;
      struct dump_class *a = dump_find(d, dump_attached(mrb, e->c));
      if (a == NULL || a->key == NULL) continue;
      size_t len = strlen(a->key) + 10;
      e->key = (char*)mrb_malloc(mrb, len);
      snprintf(e->key, len, "#<Class:%s>", a->key);
      e->in = 1;
      changed = 1;
    }
  }
}

/* An iclass belongs to the first class above it in a super chain: OWNER|POSITION. */
static void
dump_key_iclasses(struct dump *d)
{
  mrb_state *mrb = d->mrb;
  for (int i = 0; i < d->n; i++) {
    struct dump_class *e = &d->v[i];
    if (e->key == NULL || e->c->tt == MRB_TT_ICLASS) continue;
    int pos = 0;
    for (struct RClass *p = e->c->super; p && p->tt == MRB_TT_ICLASS; p = p->super) {
      struct dump_class *ic = dump_find(d, p);
      if (ic == NULL || ic->key) continue;
      size_t len = strlen(e->key) + 16;
      ic->key = (char*)mrb_malloc(mrb, len);
      snprintf(ic->key, len, "%s|%03d", e->key, pos++);
      ic->in = 1;
    }
  }
}

/* Drops what links outside the graph, until nothing more drops. */
static void
dump_close(struct dump *d)
{
  mrb_state *mrb = d->mrb;
  int changed = 1;
  while (changed) {
    changed = 0;
    for (int i = 0; i < d->n; i++) {
      struct dump_class *e = &d->v[i];
      if (!e->in) continue;
      struct RClass *c = e->c;
      int ok = (c->c == NULL || dump_in(d, c->c)) && (c->super == NULL || dump_in(d, c->super));
      if (ok && c->tt == MRB_TT_SCLASS) ok = dump_in(d, dump_attached(mrb, c));
      if (ok && c->tt == MRB_TT_ICLASS) {
        /* its owner: the class whose chain it is in */
        const char *bar = strchr(e->key, '|');
        ok = FALSE;
        for (int j = 0; bar && j < d->n; j++) {
          struct dump_class *o = &d->v[j];
          if (o->in && o->c->tt != MRB_TT_ICLASS && strlen(o->key) == (size_t)(bar - e->key) &&
              strncmp(o->key, e->key, bar - e->key) == 0) {
            ok = TRUE;
            break;
          }
        }
      }
      if (!ok) {
        e->in = 0;
        changed = 1;
      }
    }
  }
}

static int
dump_cmp(const void *a, const void *b)
{
  const struct dump_class *x = (const struct dump_class*)a, *y = (const struct dump_class*)b;
  if (x->in != y->in) return y->in - x->in;
  if (!x->in) return 0;
  return strcmp(x->key, y->key);
}

/* The C name of a presym, or NULL: MRB_SYM(x) and its relatives cover the names constants and variables have. */
static mrb_bool
dump_word_p(const char *s, size_t len)
{
  if (len == 0 || !(ISALPHA(s[0]) || s[0] == '_')) return FALSE;
  for (size_t i = 1; i < len; i++) {
    if (!(ISALNUM(s[i]) || s[i] == '_')) return FALSE;
  }
  return TRUE;
}

static mrb_bool
dump_sym(mrb_state *mrb, mrb_sym sym, char *buf, size_t size)
{
  if (sym == 0 || sym > MRB_PRESYM_MAX) return FALSE;
  mrb_int len;
  const char *s = mrb_sym_name_len(mrb, sym, &len);
  if (s == NULL || len == 0 || (size_t)len + 16 > size) return FALSE;
  const char *macro = "MRB_SYM";
  size_t off = 0, end = (size_t)len;
  if (len > 2 && s[0] == '@' && s[1] == '@') { macro = "MRB_CVSYM"; off = 2; }
  else if (s[0] == '@') { macro = "MRB_IVSYM"; off = 1; }
  else if (s[0] == '$') { macro = "MRB_GVSYM"; off = 1; }
  else if (s[len-1] == '?') { macro = "MRB_SYM_Q"; end--; }
  else if (s[len-1] == '!') { macro = "MRB_SYM_B"; end--; }
  else if (s[len-1] == '=') { macro = "MRB_SYM_E"; end--; }
  if (!dump_word_p(s + off, end - off)) return FALSE;
  snprintf(buf, size, "%s(%.*s)", macro, (int)(end - off), s + off);
  return TRUE;
}

/* A value the graph can hold: an immediate or a class in it. */
static mrb_bool
dump_value(struct dump *d, mrb_value v, char *buf, size_t size)
{
  char sym[160];
  switch (mrb_type(v)) {
  case MRB_TT_FALSE:
    snprintf(buf, size, mrb_nil_p(v) ? "MRB_ROM_NIL" : "MRB_ROM_FALSE");
    return TRUE;
  case MRB_TT_TRUE:
    snprintf(buf, size, "MRB_ROM_TRUE");
    return TRUE;
  case MRB_TT_INTEGER:
    if (!mrb_immediate_p(v)) return FALSE;
    snprintf(buf, size, "MRB_ROM_FIXNUM(%ld)", (long)mrb_integer(v));
    return TRUE;
  case MRB_TT_SYMBOL:
    if (!dump_sym(d->mrb, mrb_symbol(v), sym, sizeof(sym))) return FALSE;
    snprintf(buf, size, "MRB_ROM_SYMV(%s)", sym);
    return TRUE;
  case MRB_TT_CLASS:
  case MRB_TT_MODULE:
  case MRB_TT_SCLASS: {
    struct dump_class *e = dump_find(d, mrb_class_ptr(v));
    if (e == NULL || !e->in) return FALSE;
    snprintf(buf, size, "MRB_ROM_OBJV(R(%d))", e->index);
    return TRUE;
  }
  default:
    return FALSE;
  }
}

struct dump_iv_arg {
  struct dump *d;
  int total, rom;
  int emit;         /* 0: count; 1: values; 2: keys */
  mrb_bool ram;
};

static int
dump_iv_i(mrb_state *mrb, mrb_sym sym, mrb_value v, void *p)
{
  struct dump_iv_arg *a = (struct dump_iv_arg*)p;
  char key[160], val[200];
  mrb_bool ok = dump_sym(mrb, sym, key, sizeof(key)) && dump_value(a->d, v, val, sizeof(val));
  /* A class that changes later (`ram`) starts with its classes, which a definition looks up, and its name, which only
     its making stored; the boot stores the rest when it gets there, as it does on the heap, so a value made at boot (a
     clock, a seed) is not the dump's. */
  if (ok && a->ram && mrb_immediate_p(v) && sym != MRB_SYM(__classname__) && sym != MRB_SYM(__outer__)) ok = FALSE;
  if (a->emit == 0) {
    a->total++;
    if (ok) a->rom++;
  }
  if (a->emit == 1 && ok) dump_printf(a->d, "  %s,", val);
  if (a->emit == 2 && ok) dump_printf(a->d, "  %s,", key);
  if (a->emit == 3 && !ok) {
    /* the boot stores it: shown so the dumps of two boots compare */
    mrb_int len;
    const char *s = mrb_sym_name_len(mrb, sym, &len);
    dump_printf(a->d, "  /* boot sets %.*s: %s */", (int)len, s ? s : "?",
                mrb_immediate_p(v) ? "immediate" : mrb_obj_classname(mrb, v));
  }
  return 0;
}

static mrb_bool
dump_ram_listed(mrb_state *mrb, struct dump_class *e, const char *const *ram_classes)
{
  if (ram_classes == NULL || (e->c->tt != MRB_TT_CLASS && e->c->tt != MRB_TT_MODULE)) return FALSE;
  for (; *ram_classes; ram_classes++) {
    if (strcmp(*ram_classes, e->key) == 0) return TRUE;
  }
  return FALSE;
}

static void
dump_ivs(struct dump *d, const char *const *ram_classes)
{
  mrb_state *mrb = d->mrb;
  int niv = 0;
  for (int i = 0; i < d->n && d->v[i].in; i++) {
    struct dump_class *e = &d->v[i];
    struct RClass *c = e->c;
    if (c->tt == MRB_TT_ICLASS) continue;
    if (((uintptr_t)c->iv) & 1) {
      e->iv = DUMP_IV_ATTACHED;
      continue;
    }
    struct dump_iv_arg a = { d, 0, 0, 0, dump_ram_listed(mrb, e, ram_classes) };
    mrb_iv_foreach(mrb, mrb_obj_value(c), dump_iv_i, &a);
    if (a.total == 0) continue;
    e->iv = DUMP_IV_ROM;
    e->ivx = niv++;
    dump_printf(d, "/* %d %s: %d of %d */", e->index, e->key, a.rom, a.total);
    if (a.rom > 0) {
      dump_printf(d, "static const struct { mrb_value v[%d]; mrb_sym k[%d]; } ivd_%d = {{", a.rom, a.rom, e->index);
      a.emit = 1;
      mrb_iv_foreach(mrb, mrb_obj_value(c), dump_iv_i, &a);
      dump_printf(d, "}, {");
      a.emit = 2;
      mrb_iv_foreach(mrb, mrb_obj_value(c), dump_iv_i, &a);
      dump_printf(d, "}};");
    }
    a.emit = 3;
    mrb_iv_foreach(mrb, mrb_obj_value(c), dump_iv_i, &a);
  }
  /* one array: a table's address says it is const (variable.c, IV_ROM_P) */
  dump_printf(d, "static const struct mrb_rom_iv iv_tbls[%d] = {", niv ? niv : 1);
  for (int i = 0; i < d->n && d->v[i].in; i++) {
    struct dump_class *e = &d->v[i];
    if (e->iv != DUMP_IV_ROM) continue;
    struct dump_iv_arg a = { d, 0, 0, 0, dump_ram_listed(mrb, e, ram_classes) };
    mrb_iv_foreach(mrb, mrb_obj_value(e->c), dump_iv_i, &a);
    if (a.rom == 0) dump_printf(d, "  { 0, 0, NULL },  /* %d */", e->index);
    else dump_printf(d, "  { %d, %d, (mrb_value*)ivd_%d.v },  /* %d */", a.rom, a.rom, e->index, e->index);
  }
  dump_printf(d, "};");
  d->capa = niv;   /* reused: the table count */
}

static const char*
dump_tt(enum mrb_vtype tt)
{
  switch (tt) {
  case MRB_TT_CLASS: return "MRB_TT_CLASS";
  case MRB_TT_MODULE: return "MRB_TT_MODULE";
  case MRB_TT_SCLASS: return "MRB_TT_SCLASS";
  default: return "MRB_TT_ICLASS";
  }
}

static int
dump_index(struct dump *d, struct RClass *c)
{
  struct dump_class *e = dump_find(d, c);
  return e ? e->index : -1;
}

/* A layer's shape, for comparing two boots' dumps: the method tables themselves are the boot's. */
static void
dump_mt_comment(struct dump *d, struct RClass *c, char *buf, size_t size)
{
  size_t off = 0;
  buf[0] = '\0';
  for (mrb_mt_tbl *t = MRB_CLASS_MT(c); t && off + 24 < size; t = t->next) {
    if (!(t->alloc & MRB_MT_READONLY_BIT) && t->size == 0) continue;
    off += snprintf(buf + off, size - off, " %s%d", (t->alloc & MRB_MT_READONLY_BIT) ? "r" : "w", t->size);
  }
}

static void
dump_classes(struct dump *d, int ncls)
{
  mrb_state *mrb = d->mrb;
  char mtbuf[96];
  dump_printf(d, "static const struct RClass mrb_rom_classes_[%d] = {", ncls);
  for (int i = 0; i < ncls; i++) {
    struct dump_class *e = &d->v[i];
    struct RClass *c = e->c;
    char iv[64], mt[48], sup[32], cls[32];
    switch (e->iv) {
    case DUMP_IV_ATTACHED:
      /* SCLASS_ATTACHED (variable.c): the object, tagged by the low bit */
      snprintf(iv, sizeof(iv), "(struct iv_tbl*)((char*)R(%d) + 1)", dump_index(d, dump_attached(mrb, c)));
      break;
    case DUMP_IV_ROM: snprintf(iv, sizeof(iv), "(struct iv_tbl*)&iv_tbls[%d]", e->ivx); break;
    default: snprintf(iv, sizeof(iv), "NULL"); break;
    }
    if (e->mt >= 0) snprintf(mt, sizeof(mt), "(struct mrb_mt_tbl*)&mt_slot[%d]", e->mt);
    else snprintf(mt, sizeof(mt), "(struct mrb_mt_tbl*)&mrb_rom_graph_no_mt");
    if (c->super) snprintf(sup, sizeof(sup), "R(%d)", dump_index(d, c->super));
    else snprintf(sup, sizeof(sup), "NULL");
    if (c->c) snprintf(cls, sizeof(cls), "R(%d)", dump_index(d, c->c));
    else snprintf(cls, sizeof(cls), "NULL");
    dump_mt_comment(d, c, mtbuf, sizeof(mtbuf));
    dump_printf(d, "  /* %d %s mt:%s */", i, e->key, mtbuf);
    /* MRB_FL_CLASS_IS_INHERITED on every class: a ROM class cannot gain it later, and it only widens what a method
       cache clear clears */
    dump_printf(d, "  { %s, %s, 7, %d, 0x%x, %s, %s, %s },", cls, dump_tt(c->tt), c->frozen ? 1 : 0,
                (unsigned)(c->flags | MRB_FL_CLASS_IS_INHERITED), iv, mt, sup);
  }
  dump_printf(d, "};");
}

static void
dump_free(struct dump *d)
{
  for (int i = 0; i < d->n; i++) mrb_free(d->mrb, d->v[i].key);
  mrb_free(d->mrb, d->v);
}

MRB_API void
mrb_rom_graph_dump(mrb_state *mrb, mrb_rom_graph_out *out, void *ud, const char *const *ram_classes)
{
  struct dump d;
  mrb_bool disabled = mrb->gc.disabled;
  int ai = mrb_gc_arena_save(mrb);

  memset(&d, 0, sizeof(d));
  d.mrb = mrb;
  d.out = out;
  d.ud = ud;
  mrb->gc.disabled = TRUE;
  mrb_rom_graph_each(mrb, dump_collect, &d);
  mrb_objspace_each_objects(mrb, dump_collect, &d);

  /* named classes and modules a definition reopens */
  for (int i = 0; i < d.n; i++) {
    struct dump_class *e = &d.v[i];
    if (e->c->tt != MRB_TT_CLASS && e->c->tt != MRB_TT_MODULE) continue;
    if (!MRB_ROM_CLASS_P(e->c) && !(e->c->flags & MRB_FL_CLASS_DEFINED)) continue;
    mrb_value path = mrb_class_path(mrb, e->c);
    if (!mrb_string_p(path)) continue;
    e->key = dump_strdup(mrb, RSTRING_PTR(path), RSTRING_LEN(path));
    if (strlen(e->key) != (size_t)RSTRING_LEN(path) || !dump_path_resolves(mrb, e->key, e->c)) {
      mrb_free(mrb, e->key);
      e->key = NULL;
      continue;
    }
    e->in = 1;
  }
  dump_key_sclasses(&d);
  dump_key_iclasses(&d);
  dump_close(&d);
  qsort(d.v, d.n, sizeof(struct dump_class), dump_cmp);
  int ncls = 0;
  while (ncls < d.n && d.v[ncls].in) {
    d.v[ncls].index = ncls;
    ncls++;
  }

  /* method table slots: each class or module with a table, and every module; an iclass shares its module's */
  int nmt = 0;
  for (int i = 0; i < ncls; i++) {
    struct RClass *c = d.v[i].c;
    if (c->tt == MRB_TT_ICLASS && !(c->flags & MRB_FL_CLASS_IS_ORIGIN)) continue;
    if (MRB_CLASS_MT(c) || c->tt == MRB_TT_MODULE) d.v[i].mt = nmt++;
  }
  for (int i = 0; i < ncls; i++) {
    struct RClass *c = d.v[i].c;
    if (c->tt != MRB_TT_ICLASS || (c->flags & MRB_FL_CLASS_IS_ORIGIN)) continue;
    struct RClass *m = c->c;
    MRB_CLASS_ORIGIN(m);
    struct dump_class *me = dump_find(&d, m);
    d.v[i].mt = me ? me->mt : -1;
  }

  dump_printf(&d, "/* mruby ROM class graph (mrb_rom_graph_dump): %d classes, %d method table slots */", ncls, nmt);
  dump_printf(&d, "#ifndef MRB_ROM_CLASS_GRAPH");
  dump_printf(&d, "#define MRB_ROM_CLASS_GRAPH");
  dump_printf(&d, "#endif");
  dump_printf(&d, "#include <mruby.h>");
  dump_printf(&d, "#include <mruby/class.h>");
  dump_printf(&d, "#include <mruby/presym.h>");
  dump_printf(&d, "#include <mruby/rom_graph.h>");
  /* the words in the tables are this build's */
  dump_printf(&d, "_Static_assert(sizeof(mrb_value) == %d && sizeof(mrb_int) == %d && sizeof(struct RClass) == %d &&",
              (int)sizeof(mrb_value), (int)sizeof(mrb_int), (int)sizeof(struct RClass));
  dump_printf(&d, "  WORDBOX_FIXNUM_SHIFT == %d && WORDBOX_SYMBOL_SHIFT == %d && WORDBOX_SYMBOL_FLAG == %d && "
              "MRB_PRESYM_MAX == %d, \"the graph is compiled as the dump's build was\");", WORDBOX_FIXNUM_SHIFT,
              WORDBOX_SYMBOL_SHIFT, WORDBOX_SYMBOL_FLAG, (int)MRB_PRESYM_MAX);
  dump_printf(&d, "static const struct RClass mrb_rom_classes_[%d];", ncls);
  dump_printf(&d, "#define R(i) ((struct RClass*)&mrb_rom_classes_[i])");
  dump_printf(&d, "static struct mrb_mt_tbl *mt_slot[%d];", nmt ? nmt : 1);
  dump_ivs(&d, ram_classes);
  int niv = d.capa;
  dump_classes(&d, ncls);
  dump_printf(&d, "const struct mrb_rom_graph mrb_rom_class_graph = {");
  dump_printf(&d, "  MRB_ROM_GRAPH_ABI, %d, %d, %d, (struct RClass*)mrb_rom_classes_, mt_slot, iv_tbls,",
              ncls, nmt, niv);
  dump_printf(&d, "  R(%d), R(%d), R(%d), R(%d)", dump_index(&d, mrb_class_real(mrb->object_class->super)),
              dump_index(&d, mrb->object_class), dump_index(&d, mrb->module_class), dump_index(&d, mrb->class_class));
  dump_printf(&d, "};");

  dump_free(&d);
  mrb_gc_arena_restore(mrb, ai);
  mrb->gc.disabled = disabled;
}

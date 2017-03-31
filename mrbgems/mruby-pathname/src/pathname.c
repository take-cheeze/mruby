#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include <string.h>

#define mrb_cIO mrb_obj_value(mrb_class_get(mrb, "IO"))
#define mrb_cFile mrb_obj_value(mrb_class_get(mrb, "File"))
#define mrb_cDir mrb_obj_value(mrb_class_get(mrb, "Dir"))
#define mrb_mFileTest mrb_obj_value(mrb_module_get(mrb, "FileTest"))

static mrb_value
get_strpath(mrb_state *mrb, mrb_value obj)
{
    mrb_value strpath;
    strpath = mrb_iv_get(mrb, obj, mrb_intern_lit(mrb, "@path"));
    if (!mrb_string_p(strpath))
        mrb_raise(mrb, E_TYPE_ERROR, "unexpected @path");
    return strpath;
}

static void
set_strpath(mrb_state *mrb, mrb_value obj, mrb_value val)
{
    mrb_iv_set(mrb, obj, mrb_intern_lit(mrb, "@path"), val);
}

/*
 * Create a Pathname object from the given String (or String-like object).
 * If +path+ contains a NULL character (<tt>\0</tt>), an ArgumentError is raised.
 */
static mrb_value
path_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value arg, str;
    mrb_get_args(mrb, "o", &arg);
    if (mrb_string_p(arg)) {
        str = arg;
    }
    else {
        str = mrb_convert_type(mrb, arg, MRB_TT_UNDEF, "Pathname", "to_path");
        if (mrb_nil_p(str))
            str = arg;
        str = mrb_check_string_type(mrb, str);
    }
    if (memchr(RSTRING_PTR(str), '\0', RSTRING_LEN(str)))
        mrb_raise(mrb, E_ARGUMENT_ERROR, "pathname contains null byte");
    str = mrb_obj_dup(mrb, str);

    set_strpath(mrb, self, str);
    // OBJ_INFECT(self, str);
    return self;
}

/*
 * call-seq:
 *   pathname.freeze -> obj
 *
 * Freezes this Pathname.
 *
 * See Object.freeze.
 */
static mrb_value
path_freeze(mrb_state *mrb, mrb_value self)
{
    // mrb_call_super(0, 0);
    mrb_funcall(mrb, self, "freeze", 0);
    mrb_funcall(mrb, get_strpath(mrb, self), "freeze", 0);
    return self;
}

/*
 * call-seq:
 *   pathname.taint -> obj
 *
 * Taints this Pathname.
 *
 * See Object.taint.
 */
/*
static mrb_value
path_taint(mrb_state *mrb, mrb_value self)
{
    mrb_call_super(0, 0);
    mrb_obj_taint(get_strpath(mrb, self));
    return self;
}
*/

/*
 * call-seq:
 *   pathname.untaint -> obj
 *
 * Untaints this Pathname.
 *
 * See Object.untaint.
 */
/*
static mrb_value
path_untaint(mrb_state *mrb, mrb_value self)
{
    mrb_call_super(0, 0);
    mrb_obj_untaint(get_strpath(mrb, self));
    return self;
}
*/

/*
 *  Compare this pathname with +other+.  The comparison is string-based.
 *  Be aware that two different paths (<tt>foo.txt</tt> and <tt>./foo.txt</tt>)
 *  can refer to the same file.
 */
static mrb_value
path_eq(mrb_state *mrb, mrb_value self)
{
    mrb_value other;
    mrb_get_args(mrb, "o", &other);
    if (!mrb_obj_is_kind_of(mrb, other, mrb_class_get(mrb, "Pathname")))
        return mrb_false_value();
    return mrb_bool_value(mrb_str_equal(mrb, get_strpath(mrb, self), get_strpath(mrb, other)));
}

/*
 *  Provides a case-sensitive comparison operator for pathnames.
 *
 *	Pathname.new('/usr') <=> Pathname.new('/usr/bin')
 *	    #=> -1
 *	Pathname.new('/usr/bin') <=> Pathname.new('/usr/bin')
 *	    #=> 0
 *	Pathname.new('/usr/bin') <=> Pathname.new('/USR/BIN')
 *	    #=> 1
 *
 *  It will return +-1+, +0+ or +1+ depending on the value of the left argument
 *  relative to the right argument. Or it will return +nil+ if the arguments
 *  are not comparable.
 */
static mrb_value
path_cmp(mrb_state *mrb, mrb_value self)
{
    mrb_value other, s1, s2;
    char *p1, *p2;
    char *e1, *e2;
    mrb_get_args(mrb, "o", &other);
    if (!mrb_obj_is_kind_of(mrb, other, mrb_class_get(mrb, "Pathname")))
        return mrb_nil_value();
    s1 = get_strpath(mrb, self);
    s2 = get_strpath(mrb, other);
    p1 = RSTRING_PTR(s1);
    p2 = RSTRING_PTR(s2);
    e1 = p1 + RSTRING_LEN(s1);
    e2 = p2 + RSTRING_LEN(s2);
    while (p1 < e1 && p2 < e2) {
        int c1, c2;
        c1 = (unsigned char)*p1++;
        c2 = (unsigned char)*p2++;
        if (c1 == '/') c1 = '\0';
        if (c2 == '/') c2 = '\0';
        if (c1 != c2) {
            return mrb_fixnum_value(c1 < c2 ? -1 : 1);
        }
    }
    if (p1 < e1)
        return mrb_fixnum_value(1);
    if (p2 < e2)
        return mrb_fixnum_value(-1);
    return mrb_fixnum_value(0);
}

/* :nodoc: */
static mrb_value
path_hash(mrb_state *mrb, mrb_value self)
{
    return mrb_fixnum_value(mrb_str_hash(mrb, get_strpath(mrb, self)));
}

/*
 *  call-seq:
 *    pathname.to_s             -> string
 *    pathname.to_path          -> string
 *
 *  Return the path as a String.
 *
 *  to_path is implemented so Pathname objects are usable with File.open, etc.
 */
static mrb_value
path_to_s(mrb_state *mrb, mrb_value self)
{
    return mrb_obj_dup(mrb, get_strpath(mrb, self));
}

/* :nodoc: */
static mrb_value
path_inspect(mrb_state *mrb, mrb_value self)
{
    mrb_value c = mrb_class_path(mrb, mrb_obj_class(mrb, self));
    mrb_value str = get_strpath(mrb, self);
    return mrb_format(mrb, "#<%S:%S>", c, mrb_inspect(mrb, str));
}

/*
 * Return a pathname which is substituted by String#sub.
 *
 *	path1 = Pathname.new('/usr/bin/perl')
 *	path1.sub('perl', 'ruby')
 *	    #=> #<Pathname:/usr/bin/ruby>
 */
static mrb_value
path_sub(mrb_state *mrb, mrb_value self)
{
    mrb_value str = get_strpath(mrb, self);
    mrb_int argc;
    mrb_value *argv;
    mrb_value block;

    mrb_get_args(mrb, "*&", &argv, &argc, &block);

    str = mrb_funcall_with_block(mrb, str, mrb_intern_lit(mrb, "sub"), argc, argv, block);

    return mrb_class_new_instance(mrb, 1, &str, mrb_obj_class(mrb, self));
}

/*
 * Return a pathname with +repl+ added as a suffix to the basename.
 *
 * If self has no extension part, +repl+ is appended.
 *
 *	Pathname.new('/usr/bin/shutdown').sub_ext('.rb')
 *	    #=> #<Pathname:/usr/bin/shutdown.rb>
 */
/*
static mrb_value
path_sub_ext(mrb_state *mrb, mrb_value self)
{
    mrb_value str = get_strpath(mrb, self);
    mrb_value str2;
    long extlen;
    const char *ext;
    const char *p;
    mrb_value repl;

    mrb_get_args(mrb, "S", &repl);

    p = RSTRING_PTR(str);
    extlen = RSTRING_LEN(str);
    ext = ruby_enc_find_extname(p, &extlen, mrb_enc_get(str));
    if (ext == NULL) {
        ext = p + RSTRING_LEN(str);
    }
    else if (extlen <= 1) {
        ext += extlen;
    }
    str2 = mrb_str_subseq(str, 0, ext-p);
    mrb_str_append(mrb, str2, repl);
    // OBJ_INFECT(str2, str);
    return mrb_class_new_instance(mrb, 1, &str2, mrb_obj_class(mrb, self));
}
*/

/* Facade for File */

/*
 * Returns the real (absolute) pathname for +self+ in the actual
 * filesystem.
 *
 * Does not contain symlinks or useless dots, +..+ and +.+.
 *
 * All components of the pathname must exist when this method is
 * called.
 *
 */
static mrb_value
path_realpath(mrb_state *mrb, mrb_value self)
{
    mrb_value basedir, str;
    mrb_get_args(mrb, "|S", &basedir);
    str = mrb_funcall(mrb, mrb_cFile, "realpath", 2, get_strpath(mrb, self), basedir);
    return mrb_class_new_instance(mrb, 1, &str, mrb_obj_class(mrb, self));
}

/*
 * Returns the real (absolute) pathname of +self+ in the actual filesystem.
 *
 * Does not contain symlinks or useless dots, +..+ and +.+.
 *
 * The last component of the real pathname can be nonexistent.
 */
static mrb_value
path_realdirpath(mrb_state *mrb, mrb_value self)
{
    mrb_value basedir, str;
    mrb_get_args(mrb, "|S", &basedir);
    str = mrb_funcall(mrb, mrb_cFile, "realdirpath", 2, get_strpath(mrb, self), basedir);
    return mrb_class_new_instance(mrb, 1, &str, mrb_obj_class(mrb, self));
}

/*
 * call-seq:
 *   pathname.each_line {|line| ... }
 *   pathname.each_line(sep=$/ [, open_args]) {|line| block }     -> nil
 *   pathname.each_line(limit [, open_args]) {|line| block }      -> nil
 *   pathname.each_line(sep, limit [, open_args]) {|line| block } -> nil
 *   pathname.each_line(...)                                      -> an_enumerator
 *
 * Iterates over each line in the file and yields a String object for each.
 */
static mrb_value
path_each_line(mrb_state *mrb, mrb_value self)
{
    mrb_value args[4];
    int n;
    mrb_value block;

    n = mrb_get_args(mrb, "&|ooo", &block, &args[1], &args[2], &args[3]);

    args[0] = get_strpath(mrb, self);
    return mrb_funcall_with_block(mrb, mrb_cIO, mrb_intern_lit(mrb, "foreach"), 1+n, args, block);
}

/*
 * call-seq:
 *   pathname.read([length [, offset]]) -> string
 *   pathname.read([length [, offset]], open_args) -> string
 *
 * Returns all data from the file, or the first +N+ bytes if specified.
 *
 * See IO.read.
 *
 */
static mrb_value
path_read(mrb_state *mrb, mrb_value self)
{
    mrb_value args[4];
    int n;

    args[0] = get_strpath(mrb, self);

    n = mrb_get_args(mrb, "|ooo", &args[1], &args[2], &args[3]);
    return mrb_funcall_argv(mrb, mrb_cIO, mrb_intern_lit(mrb, "read"), 1+n, args);
}

/*
 * call-seq:
 *   pathname.binread([length [, offset]]) -> string
 *
 * Returns all the bytes from the file, or the first +N+ if specified.
 *
 * See IO.binread.
 *
 */
static mrb_value
path_binread(mrb_state *mrb, mrb_value self)
{
    mrb_value args[3];
    int n;

    args[0] = get_strpath(mrb, self);
    n = mrb_get_args(mrb, "|oo", &args[1], &args[2]);
    return mrb_funcall_argv(mrb, mrb_cIO, mrb_intern_lit(mrb, "binread"), 1+n, args);
}

/*
 * call-seq:
 *   pathname.write(string, [offset] )   => fixnum
 *   pathname.write(string, [offset], open_args )   => fixnum
 *
 * Writes +contents+ to the file.
 *
 * See IO.write.
 *
 */
static mrb_value
path_write(mrb_state *mrb, mrb_value self)
{
    mrb_value args[4];
    int n;

    args[0] = get_strpath(mrb, self);
    n = mrb_get_args(mrb, "|ooo", &args[1], &args[2], &args[3]);
    return mrb_funcall_argv(mrb, mrb_cIO, mrb_intern_lit(mrb, "write"), 1+n, args);
}

/*
 * call-seq:
 *   pathname.binwrite(string, [offset] )   => fixnum
 *   pathname.binwrite(string, [offset], open_args )   => fixnum
 *
 * Writes +contents+ to the file, opening it in binary mode.
 *
 * See IO.binwrite.
 *
 */
static mrb_value
path_binwrite(mrb_state *mrb, mrb_value self)
{
    mrb_value args[4];
    int n;

    args[0] = get_strpath(mrb, self);
    n = mrb_get_args(mrb, "|ooo", &args[1], &args[2], &args[3]);
    return mrb_funcall_argv(mrb, mrb_cIO, mrb_intern_lit(mrb, "binwrite"), 1+n, args);
}

/*
 * call-seq:
 *   pathname.readlines(sep=$/ [, open_args])     -> array
 *   pathname.readlines(limit [, open_args])      -> array
 *   pathname.readlines(sep, limit [, open_args]) -> array
 *
 * Returns all the lines from the file.
 *
 * See IO.readlines.
 *
 */
static mrb_value
path_readlines(mrb_state *mrb, mrb_value self)
{
    mrb_value args[4];
    int n;

    args[0] = get_strpath(mrb, self);
    n = mrb_get_args(mrb, "|ooo", &args[1], &args[2], &args[3]);
    return mrb_funcall_argv(mrb, mrb_cIO, mrb_intern_lit(mrb, "readlines"), 1+n, args);
}

/*
 * call-seq:
 *   pathname.sysopen([mode, [perm]])  -> fixnum
 *
 * See IO.sysopen.
 *
 */
static mrb_value
path_sysopen(mrb_state *mrb, mrb_value self)
{
    mrb_value args[3];
    int n;

    args[0] = get_strpath(mrb, self);
    n = mrb_get_args(mrb, "|oo", &args[1], &args[2]);
    return mrb_funcall_argv(mrb, mrb_cIO, mrb_intern_lit(mrb, "sysopen"), 1+n, args);
}

/*
 * call-seq:
 *   pathname.atime	-> time
 *
 * Returns the last access time for the file.
 *
 * See File.atime.
 */
static mrb_value
path_atime(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_cFile, "atime", 1, get_strpath(mrb, self));
}

#if defined(HAVE_STRUCT_STAT_ST_BIRTHTIMESPEC) || defined(_WIN32)
/*
 * call-seq:
 *   pathname.birthtime	-> time
 *
 * Returns the birth time for the file.
 * If the platform doesn't have birthtime, raises NotImplementedError.
 *
 * See File.birthtime.
 */
static mrb_value
path_birthtime(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_cFile, mrb_intern_lit(mrb, "birthtime"), 1, get_strpath(mrb, self));
}
#else
# define path_birthtime mrb_f_notimplement
#endif

/*
 * call-seq:
 *   pathname.ctime	-> time
 *
 * Returns the last change time, using directory information, not the file itself.
 *
 * See File.ctime.
 */
static mrb_value
path_ctime(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_cFile, "ctime", 1, get_strpath(mrb, self));
}

/*
 * call-seq:
 *   pathname.mtime	-> time
 *
 * Returns the last modified time of the file.
 *
 * See File.mtime.
 */
static mrb_value
path_mtime(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_cFile, "mtime", 1, get_strpath(mrb, self));
}

/*
 * call-seq:
 *   pathname.chmod	-> integer
 *
 * Changes file permissions.
 *
 * See File.chmod.
 */
static mrb_value
path_chmod(mrb_state *mrb, mrb_value self)
{
    mrb_value mode;
    mrb_get_args(mrb, "o", &mode);
    return mrb_funcall(mrb, mrb_cFile, "chmod", 2, mode, get_strpath(mrb, self));
}

/*
 * call-seq:
 *   pathname.lchmod	-> integer
 *
 * Same as Pathname.chmod, but does not follow symbolic links.
 *
 * See File.lchmod.
 */
static mrb_value
path_lchmod(mrb_state *mrb, mrb_value self)
{
    mrb_value mode;
    mrb_get_args(mrb, "o", &mode);
    return mrb_funcall(mrb, mrb_cFile, "lchmod", 2, mode, get_strpath(mrb, self));
}

/*
 * call-seq:
 *   pathname.chown	-> integer
 *
 * Change owner and group of the file.
 *
 * See File.chown.
 */
static mrb_value
path_chown(mrb_state *mrb, mrb_value self)
{
    mrb_value owner, group;
    mrb_get_args(mrb, "oo", &owner, &group);
    return mrb_funcall(mrb, mrb_cFile, "chown", 3, owner, group, get_strpath(mrb, self));
}

/*
 * call-seq:
 *   pathname.lchown	-> integer
 *
 * Same as Pathname.chown, but does not follow symbolic links.
 *
 * See File.lchown.
 */
static mrb_value
path_lchown(mrb_state *mrb, mrb_value self)
{
    mrb_value owner, group;
    mrb_get_args(mrb, "oo", &owner, &group);
    return mrb_funcall(mrb, mrb_cFile, "lchown", 3, owner, group, get_strpath(mrb, self));
}

/*
 * call-seq:
 *    pathname.fnmatch(pattern, [flags])        -> string
 *    pathname.fnmatch?(pattern, [flags])       -> string
 *
 * Return +true+ if the receiver matches the given pattern.
 *
 * See File.fnmatch.
 */
static mrb_value
path_fnmatch(mrb_state *mrb, mrb_value self)
{
    mrb_value str = get_strpath(mrb, self);
    mrb_value pattern, flags;
    if (mrb_get_args(mrb, "o|o", &pattern, &flags) == 1)
        return mrb_funcall(mrb, mrb_cFile, "fnmatch", 2, pattern, str);
    else
        return mrb_funcall(mrb, mrb_cFile, "fnmatch", 3, pattern, str, flags);
}

/*
 * call-seq:
 *   pathname.ftype	-> string
 *
 * Returns "type" of file ("file", "directory", etc).
 *
 * See File.ftype.
 */
static mrb_value
path_ftype(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_cFile, "ftype", 1, get_strpath(mrb, self));
}

/*
 * call-seq:
 *   pathname.make_link(old)
 *
 * Creates a hard link at _pathname_.
 *
 * See File.link.
 */
static mrb_value
path_make_link(mrb_state *mrb, mrb_value self)
{
    mrb_value old;
    mrb_get_args(mrb, "o", &old);
    return mrb_funcall(mrb, mrb_cFile, "link", 2, old, get_strpath(mrb, self));
}

/*
 * Opens the file for reading or writing.
 *
 * See File.open.
 */
static mrb_value
path_open(mrb_state *mrb, mrb_value self)
{
    mrb_value args[4];
    int n;
    mrb_value block;

    args[0] = get_strpath(mrb, self);
    n = mrb_get_args(mrb, "&|ooo", &block, &args[1], &args[2], &args[3]);
    return mrb_funcall_with_block(mrb, mrb_cFile, mrb_intern_lit(mrb, "open"), 1+n, args, block);
}

/*
 * Read symbolic link.
 *
 * See File.readlink.
 */
static mrb_value
path_readlink(mrb_state *mrb, mrb_value self)
{
    mrb_value str;
    str = mrb_funcall(mrb, mrb_cFile, "readlink", 1, get_strpath(mrb, self));
    return mrb_class_new_instance(mrb, 1, &str, mrb_obj_class(mrb, self));
}

/*
 * Rename the file.
 *
 * See File.rename.
 */
static mrb_value
path_rename(mrb_state *mrb, mrb_value self)
{
    mrb_value to;
    mrb_get_args(mrb, "o", &to);
    return mrb_funcall(mrb, mrb_cFile, "rename", 2, get_strpath(mrb, self), to);
}

/*
 * Returns a File::Stat object.
 *
 * See File.stat.
 */
static mrb_value
path_stat(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_cFile, "stat", 1, get_strpath(mrb, self));
}

/*
 * See File.lstat.
 */
static mrb_value
path_lstat(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_cFile, "lstat", 1, get_strpath(mrb, self));
}

/*
 * call-seq:
 *   pathname.make_symlink(old)
 *
 * Creates a symbolic link.
 *
 * See File.symlink.
 */
static mrb_value
path_make_symlink(mrb_state *mrb, mrb_value self)
{
    mrb_value old;
    mrb_get_args(mrb, "o", &old);
    return mrb_funcall(mrb, mrb_cFile, "symlink", 2, old, get_strpath(mrb, self));
}

/*
 * Truncates the file to +length+ bytes.
 *
 * See File.truncate.
 */
static mrb_value
path_truncate(mrb_state *mrb, mrb_value self)
{
    mrb_value length;
    mrb_get_args(mrb, "o", &length);
    return mrb_funcall(mrb, mrb_cFile, "truncate", 2, get_strpath(mrb, self), length);
}

/*
 * Update the access and modification times of the file.
 *
 * See File.utime.
 */
static mrb_value
path_utime(mrb_state *mrb, mrb_value self)
{
    mrb_value atime, mtime;
    mrb_get_args(mrb, "oo", &atime, &mtime);
    return mrb_funcall(mrb, mrb_cFile, "utime", 3, atime, mtime, get_strpath(mrb, self));
}

/*
 * Returns the last component of the path.
 *
 * See File.basename.
 */
static mrb_value
path_basename(mrb_state *mrb, mrb_value self)
{
    mrb_value str = get_strpath(mrb, self);
    mrb_value fext;
    if (mrb_get_args(mrb, "|o", &fext) == 0)
        str = mrb_funcall(mrb, mrb_cFile, "basename", 1, str);
    else
        str = mrb_funcall(mrb, mrb_cFile, "basename", 2, str, fext);
    return mrb_class_new_instance(mrb, 1, &str, mrb_obj_class(mrb, self));
}

/*
 * Returns all but the last component of the path.
 *
 * See File.dirname.
 */
static mrb_value
path_dirname(mrb_state *mrb, mrb_value self)
{
    mrb_value str = get_strpath(mrb, self);
    str = mrb_funcall(mrb, mrb_cFile, "dirname", 1, str);
    return mrb_class_new_instance(mrb, 1, &str, mrb_obj_class(mrb, self));
}

/*
 * Returns the file's extension.
 *
 * See File.extname.
 */
static mrb_value
path_extname(mrb_state *mrb, mrb_value self)
{
    mrb_value str = get_strpath(mrb, self);
    return mrb_funcall(mrb, mrb_cFile, "extname", 1, str);
}

/*
 * Returns the absolute path for the file.
 *
 * See File.expand_path.
 */
static mrb_value
path_expand_path(mrb_state *mrb, mrb_value self)
{
    mrb_value str = get_strpath(mrb, self);
    mrb_value dname;
    if (mrb_get_args(mrb, "|o", &dname) == 0)
        str = mrb_funcall(mrb, mrb_cFile, "expand_path", 1, str);
    else
        str = mrb_funcall(mrb, mrb_cFile, "expand_path", 2, str, dname);
    return mrb_class_new_instance(mrb, 1, &str, mrb_obj_class(mrb, self));
}

/*
 * Returns the #dirname and the #basename in an Array.
 *
 * See File.split.
 */
static mrb_value
path_split(mrb_state *mrb, mrb_value self)
{
    mrb_value str = get_strpath(mrb, self);
    mrb_value ary, dirname, basename;
    mrb_value results[2];
    ary = mrb_funcall(mrb, mrb_cFile, "split", 1, str);
    ary = mrb_check_array_type(mrb, ary);
    dirname = mrb_ary_entry(ary, 0);
    basename = mrb_ary_entry(ary, 1);
    results[0] = mrb_class_new_instance(mrb, 1, &dirname, mrb_obj_class(mrb, self));
    results[1] = mrb_class_new_instance(mrb, 1, &basename, mrb_obj_class(mrb, self));
    return mrb_ary_new_from_values(mrb, 2, results);
}

/*
 * See FileTest.blockdev?.
 */
static mrb_value
path_blockdev_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "blockdev?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.chardev?.
 */
static mrb_value
path_chardev_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "chardev?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.executable?.
 */
static mrb_value
path_executable_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "executable?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.executable_real?.
 */
static mrb_value
path_executable_real_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "executable_real?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.exist?.
 */
static mrb_value
path_exist_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "exist?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.grpowned?.
 */
static mrb_value
path_grpowned_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "grpowned?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.directory?.
 */
static mrb_value
path_directory_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "directory?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.file?.
 */
static mrb_value
path_file_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "file?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.pipe?.
 */
static mrb_value
path_pipe_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "pipe?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.socket?.
 */
static mrb_value
path_socket_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "socket?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.owned?.
 */
static mrb_value
path_owned_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "owned?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.readable?.
 */
static mrb_value
path_readable_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "readable?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.world_readable?.
 */
static mrb_value
path_world_readable_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "world_readable?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.readable_real?.
 */
static mrb_value
path_readable_real_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "readable_real?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.setuid?.
 */
static mrb_value
path_setuid_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "setuid?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.setgid?.
 */
static mrb_value
path_setgid_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "setgid?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.size.
 */
static mrb_value
path_size(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "size", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.size?.
 */
static mrb_value
path_size_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "size?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.sticky?.
 */
static mrb_value
path_sticky_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "sticky?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.symlink?.
 */
static mrb_value
path_symlink_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "symlink?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.writable?.
 */
static mrb_value
path_writable_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "writable?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.world_writable?.
 */
static mrb_value
path_world_writable_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "world_writable?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.writable_real?.
 */
static mrb_value
path_writable_real_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "writable_real?", 1, get_strpath(mrb, self));
}

/*
 * See FileTest.zero?.
 */
static mrb_value
path_zero_p(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_mFileTest, "zero?", 1, get_strpath(mrb, self));
}

/*
 * Tests the file is empty.
 *
 * See Dir#empty? and FileTest.empty?.
 */
static mrb_value
path_empty_p(mrb_state *mrb, mrb_value self)
{

    mrb_value path = get_strpath(mrb, self);
    if (mrb_bool(mrb_funcall(mrb, mrb_mFileTest, "directory?", 1, path)))
        return mrb_funcall(mrb, mrb_cDir, "empty?", 1, path);
    else
        return mrb_funcall(mrb, mrb_mFileTest, "empty?", 1, path);
}

/*
static mrb_value
glob_i(MRB_BLOCK_CALL_FUNC_ARGLIST(elt, klass))
{
    return mrb_yield(mrb, mrb_class_new_instance(mrb, 1, &elt, klass));
}
*/

/*
 * Returns or yields Pathname objects.
 *
 *  Pathname.glob("config/" "*.rb")
 *	#=> [#<Pathname:config/environment.rb>, #<Pathname:config/routes.rb>, ..]
 *
 * See Dir.glob.
 */
/*
static mrb_value
path_s_glob(mrb_state *mrb, mrb_value klass)
{
    mrb_value args[2];
    int n;

    n = mrb_get_args(mrb, "o|o", &args[0], &args[1]);
    if (mrb_block_given_p()) {
        return mrb_block_call(mrb_cDir, mrb_intern_lit(mrb, "glob"), n, args, glob_i, klass);
    }
    else {
        mrb_value ary;
        long i;
        ary = mrb_funcall_argv(mrb, mrb_cDir, mrb_intern_lit(mrb, "glob"), n, args);
        ary = mrb_convert_type(mrb, ary, MRB_TT_ARRAY, "Array", "to_ary");
        for (i = 0; i < RARRAY_LEN(ary); i++) {
            mrb_value elt = mrb_ary_ref(mrb, ary, i);
            elt = mrb_class_new_instance(1, &elt, klass);
            mrb_ary_set(mrb, ary, i, elt);
        }
        return ary;
    }
}
*/

/*
 * Returns the current working directory as a Pathname.
 *
 *	Pathname.getwd
 *	    #=> #<Pathname:/home/zzak/projects/ruby>
 *
 * See Dir.getwd.
 */
static mrb_value
path_s_getwd(mrb_state *mrb, mrb_value klass)
{
    mrb_value str;
    str = mrb_funcall(mrb, mrb_cDir, "getwd", 0);
    return mrb_class_new_instance(mrb, 1, &str, mrb_class_ptr(klass));
}

/*
 * Return the entries (files and subdirectories) in the directory, each as a
 * Pathname object.
 *
 * The results contains just the names in the directory, without any trailing
 * slashes or recursive look-up.
 *
 *   pp Pathname.new('/usr/local').entries
 *   #=> [#<Pathname:share>,
 *   #    #<Pathname:lib>,
 *   #    #<Pathname:..>,
 *   #    #<Pathname:include>,
 *   #    #<Pathname:etc>,
 *   #    #<Pathname:bin>,
 *   #    #<Pathname:man>,
 *   #    #<Pathname:games>,
 *   #    #<Pathname:.>,
 *   #    #<Pathname:sbin>,
 *   #    #<Pathname:src>]
 *
 * The result may contain the current directory <code>#<Pathname:.></code> and
 * the parent directory <code>#<Pathname:..></code>.
 *
 * If you don't want +.+ and +..+ and
 * want directories, consider Pathname#children.
 */
static mrb_value
path_entries(mrb_state *mrb, mrb_value self)
{
    struct RClass *klass;
    mrb_value str, ary;
    long i;
    klass = mrb_obj_class(mrb, self);
    str = get_strpath(mrb, self);
    ary = mrb_funcall(mrb, mrb_cDir, "entries", 1, str);
    ary = mrb_convert_type(mrb, ary, MRB_TT_ARRAY, "Array", "to_ary");
    for (i = 0; i < RARRAY_LEN(ary); i++) {
	mrb_value elt = mrb_ary_ref(mrb, ary, i);
        elt = mrb_class_new_instance(mrb, 1, &elt, klass);
        mrb_ary_set(mrb, ary, i, elt);
    }
    return ary;
}

/*
 * Create the referenced directory.
 *
 * See Dir.mkdir.
 */
static mrb_value
path_mkdir(mrb_state *mrb, mrb_value self)
{
    mrb_value str = get_strpath(mrb, self);
    mrb_value vmode;
    if (mrb_get_args(mrb, "|o", &vmode) == 0)
        return mrb_funcall(mrb, mrb_cDir, "mkdir", 1, str);
    else
        return mrb_funcall(mrb, mrb_cDir, "mkdir", 2, str, vmode);
}

/*
 * Remove the referenced directory.
 *
 * See Dir.rmdir.
 */
static mrb_value
path_rmdir(mrb_state *mrb, mrb_value self)
{
    return mrb_funcall(mrb, mrb_cDir, "rmdir", 1, get_strpath(mrb, self));
}

/*
 * Opens the referenced directory.
 *
 * See Dir.open.
 */
static mrb_value
path_opendir(mrb_state *mrb, mrb_value self)
{
    mrb_value args[1];
    mrb_value block;

    args[0] = get_strpath(mrb, self);
    mrb_get_args(mrb, "&", &block);
    return mrb_funcall_with_block(mrb, mrb_cDir, mrb_intern_lit(mrb, "open"), 1, args, block);
}

/*
static mrb_value
each_entry_i(MRB_BLOCK_CALL_FUNC_ARGLIST(elt, klass))
{
    return mrb_yield(mrb_class_new_instance(1, &elt, klass));
}
*/

/*
 * Iterates over the entries (files and subdirectories) in the directory,
 * yielding a Pathname object for each entry.
 */
/*
static mrb_value
path_each_entry(mrb_state *mrb, mrb_value self)
{
    mrb_value args[1];

    args[0] = get_strpath(mrb, self);
    return mrb_block_call(mrb_cDir, mrb_intern_lit(mrb, "foreach"), 1, args, each_entry_i, mrb_obj_class(mrb, self));
}
*/

/*
static mrb_value
unlink_body(mrb_state *mrb, mrb_value str)
{
    return mrb_funcall(mrb, mrb_cDir, "unlink", 1, str);
}

static mrb_value
unlink_rescue(mrb_state *mrb, mrb_value str, mrb_value errinfo)
{
    return mrb_funcall(mrb, mrb_cFile, "unlink", 1, str);
}
*/

/*
 * Removes a file or directory, using File.unlink if +self+ is a file, or
 * Dir.unlink as necessary.
 */
/*
static mrb_value
path_unlink(mrb_state *mrb, mrb_value self)
{
    mrb_value eENOTDIR = mrb_const_get(mrb, mrb_obj_value(mrb_class_get(mrb, "Errno")), mrb_intern_lit(mrb, "ENOTDIR"));
    mrb_value str = get_strpath(mrb, self);
    return mrb_rescue2(unlink_body, str, unlink_rescue, str, eENOTDIR, (mrb_value)0);
}
*/

/*
 * :call-seq:
 *  Pathname(path)  -> pathname
 *
 * Creates a new Pathname object from the given string, +path+, and returns
 * pathname object.
 *
 * In order to use this constructor, you must first require the Pathname
 * standard library extension.
 *
 *	require 'pathname'
 *	Pathname("/home/zzak")
 *	#=> #<Pathname:/home/zzak>
 *
 * See also Pathname::new for more information.
 */
/*
static mrb_value
path_f_pathname(mrb_state *mrb, mrb_value self, mrb_value str)
{
    return mrb_class_new_instance(mrb, 1, &str, mrb_class_get(mrb, "Pathname"));
}
*/

/*
 *
 * Pathname represents the name of a file or directory on the filesystem,
 * but not the file itself.
 *
 * The pathname depends on the Operating System: Unix, Windows, etc.
 * This library works with pathnames of local OS, however non-Unix pathnames
 * are supported experimentally.
 *
 * A Pathname can be relative or absolute.  It's not until you try to
 * reference the file that it even matters whether the file exists or not.
 *
 * Pathname is immutable.  It has no method for destructive update.
 *
 * The goal of this class is to manipulate file path information in a neater
 * way than standard Ruby provides.  The examples below demonstrate the
 * difference.
 *
 * *All* functionality from File, FileTest, and some from Dir and FileUtils is
 * included, in an unsurprising way.  It is essentially a facade for all of
 * these, and more.
 *
 * == Examples
 *
 * === Example 1: Using Pathname
 *
 *   require 'pathname'
 *   pn = Pathname.new("/usr/bin/ruby")
 *   size = pn.size              # 27662
 *   isdir = pn.directory?       # false
 *   dir  = pn.dirname           # Pathname:/usr/bin
 *   base = pn.basename          # Pathname:ruby
 *   dir, base = pn.split        # [Pathname:/usr/bin, Pathname:ruby]
 *   data = pn.read
 *   pn.open { |f| _ }
 *   pn.each_line { |line| _ }
 *
 * === Example 2: Using standard Ruby
 *
 *   pn = "/usr/bin/ruby"
 *   size = File.size(pn)        # 27662
 *   isdir = File.directory?(pn) # false
 *   dir  = File.dirname(pn)     # "/usr/bin"
 *   base = File.basename(pn)    # "ruby"
 *   dir, base = File.split(pn)  # ["/usr/bin", "ruby"]
 *   data = File.read(pn)
 *   File.open(pn) { |f| _ }
 *   File.foreach(pn) { |line| _ }
 *
 * === Example 3: Special features
 *
 *   p1 = Pathname.new("/usr/lib")   # Pathname:/usr/lib
 *   p2 = p1 + "ruby/1.8"            # Pathname:/usr/lib/ruby/1.8
 *   p3 = p1.parent                  # Pathname:/usr
 *   p4 = p2.relative_path_from(p3)  # Pathname:lib/ruby/1.8
 *   pwd = Pathname.pwd              # Pathname:/home/gavin
 *   pwd.absolute?                   # true
 *   p5 = Pathname.new "."           # Pathname:.
 *   p5 = p5 + "music/../articles"   # Pathname:music/../articles
 *   p5.cleanpath                    # Pathname:articles
 *   p5.realpath                     # Pathname:/home/gavin/articles
 *   p5.children                     # [Pathname:/home/gavin/articles/linux, ...]
 *
 * == Breakdown of functionality
 *
 * === Core methods
 *
 * These methods are effectively manipulating a String, because that's
 * all a path is.  None of these access the file system except for
 * #mountpoint?, #children, #each_child, #realdirpath and #realpath.
 *
 * - +
 * - #join
 * - #parent
 * - #root?
 * - #absolute?
 * - #relative?
 * - #relative_path_from
 * - #each_filename
 * - #cleanpath
 * - #realpath
 * - #realdirpath
 * - #children
 * - #each_child
 * - #mountpoint?
 *
 * === File status predicate methods
 *
 * These methods are a facade for FileTest:
 * - #blockdev?
 * - #chardev?
 * - #directory?
 * - #executable?
 * - #executable_real?
 * - #exist?
 * - #file?
 * - #grpowned?
 * - #owned?
 * - #pipe?
 * - #readable?
 * - #world_readable?
 * - #readable_real?
 * - #setgid?
 * - #setuid?
 * - #size
 * - #size?
 * - #socket?
 * - #sticky?
 * - #symlink?
 * - #writable?
 * - #world_writable?
 * - #writable_real?
 * - #zero?
 *
 * === File property and manipulation methods
 *
 * These methods are a facade for File:
 * - #atime
 * - #birthtime
 * - #ctime
 * - #mtime
 * - #chmod(mode)
 * - #lchmod(mode)
 * - #chown(owner, group)
 * - #lchown(owner, group)
 * - #fnmatch(pattern, *args)
 * - #fnmatch?(pattern, *args)
 * - #ftype
 * - #make_link(old)
 * - #open(*args, &block)
 * - #readlink
 * - #rename(to)
 * - #stat
 * - #lstat
 * - #make_symlink(old)
 * - #truncate(length)
 * - #utime(atime, mtime)
 * - #basename(*args)
 * - #dirname
 * - #extname
 * - #expand_path(*args)
 * - #split
 *
 * === Directory methods
 *
 * These methods are a facade for Dir:
 * - Pathname.glob(*args)
 * - Pathname.getwd / Pathname.pwd
 * - #rmdir
 * - #entries
 * - #each_entry(&block)
 * - #mkdir(*args)
 * - #opendir(*args)
 *
 * === IO
 *
 * These methods are a facade for IO:
 * - #each_line(*args, &block)
 * - #read(*args)
 * - #binread(*args)
 * - #readlines(*args)
 * - #sysopen(*args)
 *
 * === Utilities
 *
 * These methods are a mixture of Find, FileUtils, and others:
 * - #find(&block)
 * - #mkpath
 * - #rmtree
 * - #unlink / #delete
 *
 *
 * == Method documentation
 *
 * As the above section shows, most of the methods in Pathname are facades.  The
 * documentation for these methods generally just says, for instance, "See
 * FileTest.writable?", as you should be familiar with the original method
 * anyway, and its documentation (e.g. through +ri+) will contain more
 * information.  In some cases, a brief description will follow.
 */
void
mrb_mruby_pathname_gem_init(mrb_state *mrb)
{
    struct RClass *pathname = mrb_define_class(mrb, "Pathname", mrb->object_class);
    mrb_define_method(mrb, pathname, "initialize", path_initialize, 1);
    mrb_define_method(mrb, pathname, "freeze", path_freeze, 0);
    // mrb_define_method(mrb, pathname, "taint", path_taint, 0);
    // mrb_define_method(mrb, pathname, "untaint", path_untaint, 0);
    mrb_define_method(mrb, pathname, "==", path_eq, 1);
    mrb_define_method(mrb, pathname, "===", path_eq, 1);
    mrb_define_method(mrb, pathname, "eql?", path_eq, 1);
    mrb_define_method(mrb, pathname, "<=>", path_cmp, 1);
    mrb_define_method(mrb, pathname, "hash", path_hash, 0);
    mrb_define_method(mrb, pathname, "to_s", path_to_s, 0);
    mrb_define_method(mrb, pathname, "to_path", path_to_s, 0);
    mrb_define_method(mrb, pathname, "inspect", path_inspect, 0);
    mrb_define_method(mrb, pathname, "sub", path_sub, -1);
    // mrb_define_method(mrb, pathname, "sub_ext", path_sub_ext, 1);
    mrb_define_method(mrb, pathname, "realpath", path_realpath, -1);
    mrb_define_method(mrb, pathname, "realdirpath", path_realdirpath, -1);
    mrb_define_method(mrb, pathname, "each_line", path_each_line, -1);
    mrb_define_method(mrb, pathname, "read", path_read, -1);
    mrb_define_method(mrb, pathname, "binread", path_binread, -1);
    mrb_define_method(mrb, pathname, "readlines", path_readlines, -1);
    mrb_define_method(mrb, pathname, "write", path_write, -1);
    mrb_define_method(mrb, pathname, "binwrite", path_binwrite, -1);
    mrb_define_method(mrb, pathname, "sysopen", path_sysopen, -1);
    mrb_define_method(mrb, pathname, "atime", path_atime, 0);
    // mrb_define_method(mrb, pathname, "birthtime", path_birthtime, 0);
    mrb_define_method(mrb, pathname, "ctime", path_ctime, 0);
    mrb_define_method(mrb, pathname, "mtime", path_mtime, 0);
    mrb_define_method(mrb, pathname, "chmod", path_chmod, 1);
    mrb_define_method(mrb, pathname, "lchmod", path_lchmod, 1);
    mrb_define_method(mrb, pathname, "chown", path_chown, 2);
    mrb_define_method(mrb, pathname, "lchown", path_lchown, 2);
    mrb_define_method(mrb, pathname, "fnmatch", path_fnmatch, -1);
    mrb_define_method(mrb, pathname, "fnmatch?", path_fnmatch, -1);
    mrb_define_method(mrb, pathname, "ftype", path_ftype, 0);
    mrb_define_method(mrb, pathname, "make_link", path_make_link, 1);
    mrb_define_method(mrb, pathname, "open", path_open, -1);
    mrb_define_method(mrb, pathname, "readlink", path_readlink, 0);
    mrb_define_method(mrb, pathname, "rename", path_rename, 1);
    mrb_define_method(mrb, pathname, "stat", path_stat, 0);
    mrb_define_method(mrb, pathname, "lstat", path_lstat, 0);
    mrb_define_method(mrb, pathname, "make_symlink", path_make_symlink, 1);
    mrb_define_method(mrb, pathname, "truncate", path_truncate, 1);
    mrb_define_method(mrb, pathname, "utime", path_utime, 2);
    mrb_define_method(mrb, pathname, "basename", path_basename, -1);
    mrb_define_method(mrb, pathname, "dirname", path_dirname, 0);
    mrb_define_method(mrb, pathname, "extname", path_extname, 0);
    mrb_define_method(mrb, pathname, "expand_path", path_expand_path, -1);
    mrb_define_method(mrb, pathname, "split", path_split, 0);
    mrb_define_method(mrb, pathname, "blockdev?", path_blockdev_p, 0);
    mrb_define_method(mrb, pathname, "chardev?", path_chardev_p, 0);
    mrb_define_method(mrb, pathname, "executable?", path_executable_p, 0);
    mrb_define_method(mrb, pathname, "executable_real?", path_executable_real_p, 0);
    mrb_define_method(mrb, pathname, "exist?", path_exist_p, 0);
    mrb_define_method(mrb, pathname, "grpowned?", path_grpowned_p, 0);
    mrb_define_method(mrb, pathname, "directory?", path_directory_p, 0);
    mrb_define_method(mrb, pathname, "file?", path_file_p, 0);
    mrb_define_method(mrb, pathname, "pipe?", path_pipe_p, 0);
    mrb_define_method(mrb, pathname, "socket?", path_socket_p, 0);
    mrb_define_method(mrb, pathname, "owned?", path_owned_p, 0);
    mrb_define_method(mrb, pathname, "readable?", path_readable_p, 0);
    mrb_define_method(mrb, pathname, "world_readable?", path_world_readable_p, 0);
    mrb_define_method(mrb, pathname, "readable_real?", path_readable_real_p, 0);
    mrb_define_method(mrb, pathname, "setuid?", path_setuid_p, 0);
    mrb_define_method(mrb, pathname, "setgid?", path_setgid_p, 0);
    mrb_define_method(mrb, pathname, "size", path_size, 0);
    mrb_define_method(mrb, pathname, "size?", path_size_p, 0);
    mrb_define_method(mrb, pathname, "sticky?", path_sticky_p, 0);
    mrb_define_method(mrb, pathname, "symlink?", path_symlink_p, 0);
    mrb_define_method(mrb, pathname, "writable?", path_writable_p, 0);
    mrb_define_method(mrb, pathname, "world_writable?", path_world_writable_p, 0);
    mrb_define_method(mrb, pathname, "writable_real?", path_writable_real_p, 0);
    mrb_define_method(mrb, pathname, "zero?", path_zero_p, 0);
    mrb_define_method(mrb, pathname, "empty?", path_empty_p, 0);
    // mrb_define_singleton_method(mrb, (struct RObject*)pathname, "glob", path_s_glob, -1);
    mrb_define_singleton_method(mrb, (struct RObject*)pathname, "getwd", path_s_getwd, 0);
    mrb_define_singleton_method(mrb, (struct RObject*)pathname, "pwd", path_s_getwd, 0);
    mrb_define_method(mrb, pathname, "entries", path_entries, 0);
    mrb_define_method(mrb, pathname, "mkdir", path_mkdir, -1);
    mrb_define_method(mrb, pathname, "rmdir", path_rmdir, 0);
    mrb_define_method(mrb, pathname, "opendir", path_opendir, 0);
    // mrb_define_method(mrb, pathname, "each_entry", path_each_entry, 0);
    // mrb_define_method(mrb, pathname, "unlink", path_unlink, 0);
    // mrb_define_method(mrb, pathname, "delete", path_unlink, 0);
    // mrb_undef_method(mrb, pathname, "=~");
    // mrb_define_global_function(mrb, "Pathname", path_f_pathname, 1);
}

void
mrb_mruby_pathname_gem_final(mrb_state *mrb)
{
}

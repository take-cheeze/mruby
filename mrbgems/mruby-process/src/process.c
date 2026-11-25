/*
** mruby-process - Process.clock_gettime and Process.pid
**
** POSIX only. Clock IDs are Process::CLOCK_REALTIME, CLOCK_MONOTONIC and
** CLOCK_PROCESS_CPUTIME_ID (when the platform has them). Units follow CRuby:
** :float_second (default), :float_millisecond, :float_microsecond,
** :second, :millisecond, :microsecond, :nanosecond.
**
** [ MIT license: https://www.opensource.org/licenses/mit-license.php ]
*/

#include <time.h>
#include <unistd.h>
#include <string.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/error.h>
#include <mruby/numeric.h>
#include <mruby/string.h>

#ifdef _WIN32
void mrb_mruby_process_gem_init(mrb_state *mrb) { (void)mrb; }
void mrb_mruby_process_gem_final(mrb_state *mrb) { (void)mrb; }
#else

static mrb_value
proc_clock_gettime(mrb_state *mrb, mrb_value self)
{
  mrb_int clk = CLOCK_REALTIME;
  mrb_sym unit = mrb_intern_lit(mrb, "float_second");
  struct timespec ts;
  mrb_value ret;
  mrb_int sec, nsec;

  mrb_get_args(mrb, "|in", &clk, &unit);
  if (clock_gettime((clockid_t)clk, &ts) != 0) {
    mrb_sys_fail(mrb, "clock_gettime");
  }
  sec = (mrb_int)ts.tv_sec;
  nsec = (mrb_int)ts.tv_nsec;

  if (unit == mrb_intern_lit(mrb, "float_second")) {
    return mrb_float_value(mrb, (mrb_float)ts.tv_sec + (mrb_float)ts.tv_nsec / 1e9);
  }
  if (unit == mrb_intern_lit(mrb, "float_millisecond")) {
    return mrb_float_value(mrb, (mrb_float)ts.tv_sec * 1e3 + (mrb_float)ts.tv_nsec / 1e6);
  }
  if (unit == mrb_intern_lit(mrb, "float_microsecond")) {
    return mrb_float_value(mrb, (mrb_float)ts.tv_sec * 1e6 + (mrb_float)ts.tv_nsec / 1e3);
  }
  if (unit == mrb_intern_lit(mrb, "second")) {
    return mrb_fixnum_value(sec);
  }
  if (unit == mrb_intern_lit(mrb, "millisecond")) {
    ret = mrb_fixnum_value(sec * 1000 + nsec / 1000000);
    return ret;
  }
  if (unit == mrb_intern_lit(mrb, "microsecond")) {
    return mrb_fixnum_value(sec * 1000000 + nsec / 1000);
  }
  if (unit == mrb_intern_lit(mrb, "nanosecond")) {
    return mrb_fixnum_value(sec * 1000000000 + nsec);
  }
  mrb_raisef(mrb, E_ARGUMENT_ERROR, "unexpected unit: %S", mrb_symbol_value(unit));
  return mrb_nil_value();
}

static mrb_value
proc_pid(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value((mrb_int)getpid());
}

void
mrb_mruby_process_gem_init(mrb_state *mrb)
{
  struct RClass *process = mrb_define_module(mrb, "Process");

  mrb_define_const(mrb, process, "CLOCK_REALTIME", mrb_fixnum_value((mrb_int)CLOCK_REALTIME));
  mrb_define_const(mrb, process, "CLOCK_MONOTONIC", mrb_fixnum_value((mrb_int)CLOCK_MONOTONIC));
#ifdef CLOCK_PROCESS_CPUTIME_ID
  mrb_define_const(mrb, process, "CLOCK_PROCESS_CPUTIME_ID", mrb_fixnum_value((mrb_int)CLOCK_PROCESS_CPUTIME_ID));
#endif

  mrb_define_module_function(mrb, process, "clock_gettime", proc_clock_gettime, MRB_ARGS_OPT(2));
  mrb_define_module_function(mrb, process, "pid", proc_pid, MRB_ARGS_NONE());
}

void
mrb_mruby_process_gem_final(mrb_state *mrb)
{
}
#endif

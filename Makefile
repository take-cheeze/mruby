# mruby is using Rake (http://rake.rubyforge.org) as a build tool.
# We provide a minimalistic version called minirake inside of our
# codebase.

BUILD_DIR = $(abspath ./build/minirake)
SRC_DIR = $(abspath .)

MKDIR_P = mkdir -p
DOWNLOADER = wget -O
TAR = tar
# DOWNLOADER = curl -o

MINIRAKE := $(BUILD_DIR)/bin/minirake
MRBC := $(BUILD_DIR)/bin/mrbc
BIN_MRUBY := $(BUILD_DIR)/bin/mruby

DEP_FILE := $(SRC_DIR)/mrbgems/mruby-bin-minirake/.dep_gems.txt
DEPS := $(shell cat $(DEP_FILE))
SYM_DEPS := $(subst -,_, $(DEPS))

NON_CORE_MRBGEMS_DIR := $(patsubst %/,%,$(dir $(BUILD_DIR)))/mrbgems
MRBGEM_CLONE_RESULT := $(shell $(SRC_DIR)/tasks/clone_minirake_dependencies.sh $(NON_CORE_MRBGEMS_DIR))

MRBGEM_DIRS := $(foreach gem,$(DEPS),$(if $(wildcard \
  $(SRC_DIR)/mrbgems/$(gem)/mrbgem.rake), \
  $(SRC_DIR)/mrbgems/$(gem), \
  $(NON_CORE_MRBGEMS_DIR)/$(gem)))
MRBGEM_C_SRCS := $(foreach dir,$(MRBGEM_DIRS),$(wildcard $(dir)/src/*.c)) $(SRC_DIR)/mrblib/init_mrblib.c
MRBGEM_CXX_SRCS := $(foreach dir,$(MRBGEM_DIRS),$(wildcard $(dir)/src/*.cxx))
MRBGEM_CPP_SRCS := $(foreach dir,$(MRBGEM_DIRS),$(wildcard $(dir)/src/*.cpp))

MRBGEM_OBJS := \
  $(subst $(SRC_DIR),$(BUILD_DIR), \
    $(patsubst %.c,%.o,$(MRBGEM_C_SRCS)) \
    $(patsubst %.cxx,%.o,$(MRBGEM_CXX_SRCS)) \
    $(patsubst %.cpp,%.o,$(MRBGEM_CPP_SRCS))) \
  $(addprefix $(BUILD_DIR)/mrbgems/,$(addsuffix /gem_init.o, $(DEPS))) \
  $(BUILD_DIR)/mrbgems/gems_init.o \
  $(BUILD_DIR)/mrblib.o \
  $(BUILD_DIR)/mrbgems/mruby-mrbgem-require/mrbgem_require.o \
  $(BUILD_DIR)/mrbgems/mruby-rbconfig/rbconfig.o \

CORE_SRCS := $(wildcard src/*.c) $(wildcard $(SRC_DIR)/mrbgems/mruby-compiler/core/*.c)
MRBLIB_SRCS := $(wildcard $(SRC_DIR)/mrblib/*.rb)
MINIRAKE_SRCS := $(wildcard $(SRC_DIR)/mrbgems/mruby-bin-minirake/tools/minirake/*.rb)

CORE_OBJS := $(subst $(SRC_DIR),$(BUILD_DIR),$(patsubst %.c,%.o,$(CORE_SRCS))) $(BUILD_DIR)/core/y.tab.o
CLI_MAIN_OBJS := $(BUILD_DIR)/tasks/cli_main.o
MRBC_OBJS := $(BUILD_DIR)/mrbgems/mruby-bin-mrbc/tools/mrbc/mrbc.o
BIN_MRUBY_OBJS := $(BUILD_DIR)/mrbgems/mruby-bin-mruby/tools/mruby/mruby.o

LIBMRUBY_CORE := $(BUILD_DIR)/lib/libmruby_core.a
LIBMRUBY := $(BUILD_DIR)/lib/libmruby.a

ONIGMO_VERSION := 6.1.1
ONIGMO_URL := https://github.com/k-takata/Onigmo/releases/download/Onigmo-$(ONIGMO_VERSION)/onigmo-$(ONIGMO_VERSION).tar.gz
ONIGMO_ARCHIVE := $(abspath $(BUILD_DIR)/Onigmo-$(ONIGMO_VERSION).tar.gz)
ONIGMO_DIR := $(abspath $(BUILD_DIR)/Onigmo-$(ONIGMO_VERSION))
ONIGMO_HEADER := $(ONIGMO_DIR)/build/include/onigmo.h
ONIGMO_LIB := $(ONIGMO_DIR)/build/lib/libonigmo.a

LIBYAML_VERSION := 0.1.6
LIBYAML_URL := http://pyyaml.org/download/libyaml/yaml-$(LIBYAML_VERSION).tar.gz
LIBYAML_ARCHIVE := $(abspath $(BUILD_DIR)/libyaml-$(LIBYAML_VERSION).tar.gz)
LIBYAML_DIR := $(abspath $(BUILD_DIR)/yaml-$(LIBYAML_VERSION))
LIBYAML_HEADER := $(LIBYAML_DIR)/build/include/yaml.h
LIBYAML_LIB := $(LIBYAML_DIR)/build/lib/libyaml.a

CPPFLAGS += -I$(SRC_DIR)/include \
  $(addprefix -I,$(wildcard $(addsuffix /include,$(MRBGEM_DIRS)))) \

# -I$(SRC_DIR)/mrbgems/mruby-compiler/core \
# -I$(SRC_DIR)/src \

CLI_LIBS := $(ONIGMO_LIB) $(LIBYAML_LIB)

MRBCFLAGS := -g

all : $(MINIRAKE)
	$(MINIRAKE)
.PHONY : all


test : | all $(BIN_MRUBY)
	$(MINIRAKE) test
.PHONY : test

clean :
ifeq ($(wildcard $(MINIRAKE)),)
	$(RM) -r $(BUILD_DIR)
else
	$(MINIRAKE) clean
endif
.PHONY : clean

$(BUILD_DIR)/build/mrbgems/mruby-onig-regexp/src/mruby_onig_regexp.o : CPPFLAGS += -I$(dir $(ONIGMO_HEADER)) -DHAVE_ONIGMO_H
$(BUILD_DIR)/build/mrbgems/mruby-onig-regexp/src/mruby_onig_regexp.o : $(ONIGMO_HEADER)
$(ONIGMO_HEADER) : $(ONIGMO_LIB)
$(ONIGMO_LIB) : $(ONIGMO_DIR)
	@$(MKDIR_P) $(dir $@)
	cd $(ONIGMO_DIR) && \
	./configure --disable-shared --enable-static --prefix=$(ONIGMO_DIR)/build && \
	make install
$(ONIGMO_DIR) : $(ONIGMO_ARCHIVE)
	@$(MKDIR_P) $(dir $@)
	$(TAR) xf $< -C $(dir $@)
$(ONIGMO_ARCHIVE) :
	@$(MKDIR_P) $(dir $@)
	$(DOWNLOADER) $@ $(ONIGMO_URL)

$(BUILD_DIR)/build/mrbgems/mruby-yaml/src/yaml.o : CPPFLAGS += -I$(dir $(LIBYAML_HEADER))
$(BUILD_DIR)/build/mrbgems/mruby-yaml/src/yaml.o : $(LIBYAML_HEADER)
$(LIBYAML_HEADER) : $(LIBYAML_LIB)
$(LIBYAML_LIB) : $(LIBYAML_DIR)
	@$(MKDIR_P) $(dir $@)
	cd $(LIBYAML_DIR) && \
	./configure --disable-shared --enable-static --prefix=$(LIBYAML_DIR)/build && \
	make install
$(LIBYAML_DIR) : $(LIBYAML_ARCHIVE)
	@$(MKDIR_P) $(dir $@)
	$(TAR) xf $< -C $(dir $@)
$(LIBYAML_ARCHIVE) :
	@$(MKDIR_P) $(dir $@)
	$(DOWNLOADER) $@ $(LIBYAML_URL)

$(BUILD_DIR)/build/mrbgems/mruby-file-stat/src/file-stat.o : CPPFLAGS += -I$(BUILD_DIR)/build/mrbgems/mruby-file-stat
$(BUILD_DIR)/build/mrbgems/mruby-file-stat/src/file-stat.o : $(BUILD_DIR)/build/mrbgems/mruby-file-stat/config.h
$(BUILD_DIR)/build/mrbgems/mruby-file-stat/config.h :
	@$(MKDIR_P) $(dir $@)
	cd $(dir $@) && $(SRC_DIR)/build/mrbgems/mruby-file-stat/configure

$(BUILD_DIR)/build/mrbgems/mruby-dir/src/dir.o : CPPFLAGS += -I$(SRC_DIR)/src
$(BUILD_DIR)/core/y.tab.o : CPPFLAGS += -I$(SRC_DIR)/mrbgems/mruby-compiler/core
$(BUILD_DIR)/build/mrbgems/mruby-require/src/require.o : CPPFLAGS += -I$(SRC_DIR)/src
$(BUILD_DIR)/build/mrbgems/mruby-pack/src/pack.o : CPPFLAGS += -I$(SRC_DIR)/src

# build `mruby` command
$(MINIRAKE) : $(BUILD_DIR)/minirake.o $(CLI_MAIN_OBJS) $(LIBMRUBY) $(CLI_LIBS)
	@$(MKDIR_P) $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/minirake.c : $(MINIRAKE_SRCS) | $(MRBC)
	@$(MKDIR_P) $(dir $@)
	$(MRBC) $(MRBCFLAGS) -Bmrb_main_irep -o $@ $^

$(LIBMRUBY) : $(CORE_OBJS) $(MRBGEM_OBJS)
	@$(MKDIR_P) $(dir $@)
	$(AR) rs $@ $^

$(BUILD_DIR)/mrblib.c : $(MRBLIB_SRCS) | $(MRBC)
	@$(MKDIR_P) $(dir $@)
	$(MRBC) $(MRBCFLAGS) -Bmrblib_irep -o $@ $^

MRBGEM_REQUIRE_PREFIX := mrb_ary_push(mrb, ary, mrb_str_new_lit(mrb, "
MRBGEM_REQUIRE_SUFFIX := "));\n

define MRBGEM_REQUIRE
#include <mruby.h>
#include <mruby/array.h>

mrb_value mrbgem_require_gems(mrb_state *mrb) {
  mrb_value ary = mrb_ary_new_capa(mrb, $(words $(DEPS)));

  $(addprefix $(MRBGEM_REQUIRE_PREFIX),$(addsuffix $(MRBGEM_REQUIRE_SUFFIX),$(subst mruby-,,$(DEPS))))

  return ary;
}
endef
export MRBGEM_REQUIRE

$(info $(shell echo "$${MRBGEM_REQUIRE}"))

$(BUILD_DIR)/mrbgems/mruby-mrbgem-require/mrbgem_require.c : | $(DEP_FILE) Makefile
	@$(MKDIR_P) $(dir $@)
	printf "$${MRBGEM_REQUIRE}" > $@

RBCONFIG_HOST_OS := $(shell echo `uname -s`-`uname -r` | tr A-Z a-z)
RBCONFIG_RUBY_INSTALL_NAME := $(notdir $(BIN_MRUBY))
RBCONFIG_BINDIR := $(abspath $(dir $(BIN_MRUBY)))
RUBY_PLATFORM := $(shell echo `uname -m`-`uname -s` | tr A-Z a-z)

# generate RbConfig for minirake
define RBCONFIG_SRC
#include <mruby.h>
#include <mruby/hash.h>

mrb_value
mrb_rbconfig_hash(mrb_state *mrb)
{
  mrb_value ret = mrb_hash_new(mrb);

  mrb_hash_set(mrb, ret, mrb_str_new_lit(mrb, "host_os"), mrb_str_new_lit(mrb, "$(RBCONFIG_HOST_OS)"));
  mrb_hash_set(mrb, ret, mrb_str_new_lit(mrb, "ruby_install_name"),
               mrb_str_new_lit(mrb, "$(RBCONFIG_RUBY_INSTALL_NAME)"));
  mrb_hash_set(mrb, ret, mrb_str_new_lit(mrb, "bindir"), mrb_str_new_lit(mrb, "$(RBCONFIG_BINDIR)"));

  return ret;
}

void
mrb_rbconfig_define_const(mrb_state *mrb)
{
  mrb_define_global_const(mrb, "RUBY_PLATFORM", mrb_str_new_lit(mrb, "$(RUBY_PLATFORM)"));
}
endef
export RBCONFIG_SRC

$(BUILD_DIR)/mrbgems/mruby-rbconfig/rbconfig.c : | $(SRC_DIR)/Makefile
	@$(MKDIR_P) $(dir $@)
	printf "$${RBCONFIG_SRC}" > $@

# generate gem_init.c for initializing all mrbgem
define GEMS_INIT_SOURCE
#include <mruby.h>

$(addprefix void GENERATED_TMP_mrb_, $(addsuffix _gem_init(mrb_state*);\n, $(SYM_DEPS)))
$(addprefix void GENERATED_TMP_mrb_, $(addsuffix _gem_final(mrb_state*);\n, $(SYM_DEPS)))

static void
mrb_final_mrbgems(mrb_state *mrb) {
  $(addprefix GENERATED_TMP_mrb_, $(addsuffix _gem_final(mrb);\n, $(SYM_DEPS)))
}

void
mrb_init_mrbgems(mrb_state *mrb) {
  $(addprefix GENERATED_TMP_mrb_, $(addsuffix _gem_init(mrb);\n, $(SYM_DEPS)))
  mrb_state_atexit(mrb, mrb_final_mrbgems);
}
endef
export GEMS_INIT_SOURCE

$(BUILD_DIR)/mrbgems/gems_init.c : | $(DEP_FILE) $(SRC_DIR)/Makefile
	@$(MKDIR_P) $(dir $@)
	printf "$${GEMS_INIT_SOURCE}" > $@

# generate gem_init.c for each mrbgem
define GEM_INIT
$(eval sym = $(subst -,_,$(notdir $(1))))

define GEM_INIT_SOURCE_$(sym)
#include <mruby.h>
#include <mruby/irep.h>
#include <stdlib.h>

void mrb_$(sym)_gem_init(mrb_state *mrb);
void mrb_$(sym)_gem_final(mrb_state *mrb);

void GENERATED_TMP_mrb_$(sym)_gem_init(mrb_state *mrb) {
  mrb_int ai = mrb_gc_arena_save(mrb);
#ifndef MRBGEM_NO_C_INIT
  mrb_$(sym)_gem_init(mrb);
#endif
#ifndef MRBGEM_NO_IREP
  mrb_load_irep(mrb, gem_mrblib_irep_$(sym));
#endif
  if (mrb->exc) {
    mrb_print_error(mrb);
    exit(EXIT_FAILURE);
  }
  mrb_gc_arena_restore(mrb, ai);
}

void GENERATED_TMP_mrb_$(sym)_gem_final(mrb_state *mrb) {
#ifndef MRBGEM_NO_C_INIT
  mrb_$(sym)_gem_final(mrb);
#endif
}
endef
export GEM_INIT_SOURCE_$(sym)

$(sym)_RBFILES := $(wildcard $(1)/mrblib/*.rb)
$(sym)_SRCS := $(wildcard $(1)/src/*.c $(1)/src/*.cxx $(1)/src/*.cpp)

$(BUILD_DIR)/mrbgems/$(notdir $(1))/gem_init.c : $$($(sym)_RBFILES) | $(SRC_DIR)/Makefile $(MRBC)
	@$(MKDIR_P) $$(dir $$@)
ifeq ($$($(sym)_RBFILES),)
	echo "#define MRBGEM_NO_IREP 1" > $$@
else
	$(MRBC) $(MRBCFLAGS) -Bgem_mrblib_irep_$(sym) -o $$@ $$^
endif
ifeq ($$($(sym)_SRCS),)
	echo "#define MRBGEM_NO_C_INIT 1" >> $$@
endif
	printf "$$$${GEM_INIT_SOURCE_$(sym)}" >> $$@
endef

$(foreach gem,$(MRBGEM_DIRS),$(eval $(call GEM_INIT,$(gem))))

# build `mrbc` command
$(MRBC) : $(LIBMRUBY_CORE) $(MRBC_OBJS)
	@$(MKDIR_P) $(dir $@)
	$(CC) -o $@ $^

$(LIBMRUBY_CORE) : $(CORE_OBJS)
	@$(MKDIR_P) $(dir $@)
	$(AR) rs $@ $^

# build `mruby` command
$(BIN_MRUBY) : $(BIN_MRUBY_OBJS) $(LIBMRUBY) $(CLI_LIBS)
	@$(MKDIR_P) $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/core/y.tab.c : $(SRC_DIR)/mrbgems/mruby-compiler/core/parse.y
	@$(MKDIR_P) $(dir $@)
	$(YACC) -o $@ $<

$(BUILD_DIR)/%.o : %.cxx
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o : %.cpp
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o : %.c
	@$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

ifneq ($(wildcard $(dir $(BUILD_DIR))minirake_dep.mak),)
include $(dir $(BUILD_DIR))minirake_dep.mak
endif

<!-- summary: ROM Class Graph: the boot's classes as const data -->

# ROM Class Graph

`MRB_ROM_CLASS_GRAPH` lets a program take the class, module, singleton class
and iclass objects its boot makes from const data instead of the GC heap.
It is for a closed program whose boot is the same every run, on a target
where RAM is scarce and flash is not.

## Making a graph

1. Build with `MRB_ROM_CLASS_GRAPH` and boot as usual, then call
   `mrb_rom_graph_dump(mrb, out, ud, ram_classes)`. It writes C, a line at a
   time, to `out`.
2. Compile that C and link it into the same program. `mrb_init_class` takes
   `mrb_rom_class_graph` (a weak symbol) when it is there.
3. Boot again and dump again: the two dumps are the same file when the graph
   is what the boot makes.

`ram_classes` names the classes and modules (by path) whose constants or
class-level instance variables change after the boot.

## What is in it

- The classes and modules a definition by name made (`mrb_define_class`,
  `mrb_define_module` and their `_under`/`vm` forms, Ruby `class`/`module`)
  that their path still reaches, their singleton classes, and the iclasses in
  their ancestor chains. A class made otherwise (`Class.new`, `Struct.new`)
  stays on the heap, and so does a singleton class of any other object.
- Each one is a `const struct RClass` painted `MRB_GC_RED`.
- Its constants and instance variables, in a const table: the immediates
  (and classes in the graph) whose names and symbols are presyms. The boot
  stores the rest. A class `ram_classes` names starts with its classes and its
  name only, so a value its boot makes (a clock) is not the dump's.
- A store that changes a const table copies it to the heap first; the copy
  takes its place from then on.
- A class that has methods has a RAM slot for its method table chain; the
  boot installs its tables there (`MRB_CLASS_MT`). An iclass shares its
  module's slot.

## Limits

- One `mrb_state` at a time runs on the graph; another opens with heap
  classes. A state closed after using it leaves the graph used.
- A ROM class's structure is fixed: a method defined on one that had no
  method table in the dump, a new included module, a prepend, a freeze, or a
  singleton class it did not have raise `FrozenError`.
- The graph must come from the same program: the dump asserts the word
  layout it was made with, and its names are presym macros, so a stale graph
  fails to compile or to boot.
- `mrb_objspace_each_objects` does not see ROM classes; `mrb_rom_graph_each`
  does.

Set `mrb_rom_graph_disabled` before `mrb_open` to keep a linked graph out.

# Compiler-Implementation-in-C

Course compiler project: **code generation** modules written in C for translating a parsed syntax tree into intermediate / assembly-style output.

## Description

This repository contains three incremental parts of a compiler back-end (`Part 1.c`, `Part 2.c`, `Part 3.c`). Each file implements `code_recur` / related routines that walk an AST (`treenode`) and emit code for language constructs (identifiers, control flow, arrays, `switch`, `main`, etc.).

The files are **not standalone programs** — they plug into the university compiler framework via `CodeGenerator.h` and shared parser/AST types from the course.

## Technologies

- C
- Course-provided compiler headers (`CodeGenerator.h`, AST definitions)

## Repository contents

| File | Scope (progression) |
|------|---------------------|
| `Part 1.c` | Core code generation: symbols, basic expressions, `if` / `for`, identifiers |
| `Part 2.c` | Extended constructs and control-flow handling |
| `Part 3.c` | Further features including constant folding helpers (`is_constant`, `calculate_value`) |

## Project structure

```
Compiler-Implementation-in-C/
├── Part 1.c
├── Part 2.c
├── Part 3.c
├── .gitignore
└── README.md
```

## Prerequisites

- GCC or another C compiler
- **`CodeGenerator.h`** and the rest of the course compiler skeleton (parser, AST headers, linker stubs) — **not included in this repo**

## How to build / run

These sources are meant to be **linked into the full course compiler project**, not compiled alone.

Typical course workflow:

1. Copy the appropriate `Part N.c` logic into your course `CodeGenerator` module (or replace the provided stub).
2. Build the full compiler from the course Makefile / project layout.
3. Run the compiler on course test inputs:

```bash
./compiler input_file.src
```

Exact command depends on your course framework naming.

## Example (course context)

```bash
# After integrating with course headers and building the full compiler:
./compiler examples/program1.src
# Emitted assembly / intermediate file per course spec
```

## Notes / limitations

- **`#include "CodeGenerator.h"`** — header and AST types are required; cloning only these three `.c` files will not compile by themselves.
- File names contain spaces (`Part 1.c`, …) — quote paths in shell commands.
- `Part 3.c` is the most complete version; earlier parts are milestones from the same assignment series.
- No lexer/parser sources here — only the code-generation stages.

## Author

Mustafa Waked — Compiler course (University of Haifa)

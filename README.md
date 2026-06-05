# Compiler-Implementation-in-C

Course compiler project: **code generation** modules written in C for translating a parsed syntax tree into intermediate / assembly-style output.

## What this project does

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
├── scripts/verify-structure.ps1
├── .gitignore
└── README.md
```

## Prerequisites

- GCC or another C compiler (Linux, macOS, or WSL on Windows)
- **`CodeGenerator.h`** and the rest of the course compiler skeleton (parser, AST headers, linker stubs) — **not included in this repo**

## Installation

```bash
git clone https://github.com/Mustafa-Waked/Compiler-Implementation-in-C.git
cd Compiler-Implementation-in-C
```

Obtain `CodeGenerator.h` and the full course compiler project from your course materials.

## Build

These sources are meant to be **linked into the full course compiler project**, not compiled alone.

Typical course workflow:

1. Copy the appropriate `Part N.c` logic into your course `CodeGenerator` module (or replace the provided stub).
2. Build the full compiler from the course Makefile / project layout.
3. Run the compiler on course test inputs.

```bash
# After integrating with course headers and building the full compiler:
make
./compiler examples/program1.src
```

Exact command depends on your course framework naming.

On Windows without WSL, use **WSL** or **MinGW** — native `gcc` is often not installed by default.

## Run

```bash
./compiler input_file.src
```

Emitted assembly / intermediate output follows the course specification.

## Example

```bash
# Integrate Part 3.c into the course CodeGenerator, then:
make clean && make
./compiler examples/program1.src
```

## Verify this clone (structure only)

```powershell
.\scripts\verify-structure.ps1
```

This checks that all three part files exist and include `CodeGenerator.h`. It does **not** compile without the course framework.

## Expected output

When integrated with the full compiler, running on a `.src` test file produces assembly or intermediate code per the course assignment spec.

## Troubleshooting

| Problem | Likely cause | Fix |
|---------|--------------|-----|
| `CodeGenerator.h: No such file` | Header not in repo | Copy from course compiler skeleton; this repo is only the code-gen modules. |
| Undefined reference to `treenode` / `leafnode` | Missing AST headers | Link against full course project, not these `.c` files alone. |
| `gcc` not found (Windows) | No C toolchain | Install WSL (`sudo apt install build-essential`) or MinGW-w64. |
| Spaces in filenames | Shell parsing | Quote paths: `gcc -c "Part 1.c"` |
| Which part to use? | Milestone progression | **`Part 3.c`** is the most complete; earlier parts are stepping stones. |

## Notes / limitations

- **`#include "CodeGenerator.h"`** — header and AST types are required; cloning only these three `.c` files will not compile by themselves.
- File names contain spaces (`Part 1.c`, …) — quote paths in shell commands.
- No lexer/parser sources here — only the code-generation stages.

## Author

Mustafa Waked — Compiler course (University of Haifa)

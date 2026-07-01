# Axiom

Axiom is a reactive **Model-Driven Engineering (MDE)** framework written in modern C++23.

It compiles a domain-specific language (DSL) for hierarchical state machines through a compiler-style pipeline into readable and efficient C++ code.

---

## Features

### Current Milestone

- Lexer with source locations and diagnostics
- Handwritten recursive-descent parser
- Basic parser recovery
- Immutable AST nodes
- AST debug printer
- Semantic validation
  - Duplicate state detection
  - Unknown transition endpoint detection
- C++ code generation
  - Enum-based state representation
  - Switch-based dispatch generation
- Command-line interface
  - `parse`
  - `ast`
  - `generate`

### Planned

- Incremental dependency graphs
- Intermediate Representation (IR) lowering
- Transformation passes
- GUI tooling
- Live editing and hot reload
- Language server support

---

## Requirements

- CMake 3.30+
- C++23 compiler
  - macOS: Apple Clang (via Xcode Command Line Tools)
- Ninja (required on macOS setup)

---

## Quick Start

Clone the repository:

```bash
git clone <repository-url>
cd Axiom
```

### macOS Setup (First Time Only)

Install required tools:

```bash
xcode-select --install
```

Install build tools:

```bash
brew install cmake ninja
```

### Configure & Build

Debug build:

```bash
cmake --preset debug
cmake --build build-debug
```

Release build:

```bash
cmake --preset release
cmake --build build-release
```

### Run the CLI

After building:

Debug build:
```bash
./build-debug/axiom parse examples/wafer_stage.ax
./build-debug/axiom ast examples/wafer_stage.ax
./build-debug/axiom generate examples/wafer_stage.ax -o generated
```

Release build:
```bash
./build-release/axiom parse examples/wafer_stage.ax
./build-release/axiom ast examples/wafer_stage.ax
./build-release/axiom generate examples/wafer_stage.ax -o generated
```

### Build Presets

Axiom uses macOS-native CMake presets.

#### Debug build

```bash
cmake --preset debug
cmake --build build-debug
```

#### Release build

```bash
cmake --preset release
cmake --build build-release
```

---

# CLI Usage

## Parse a source file

```bash
axiom parse examples/wafer_stage.ax
```

## Print the AST

```bash
axiom ast examples/wafer_stage.ax
```

## Generate C++ output

```bash
axiom generate examples/wafer_stage.ax -o generated
```

---

## Project Layout

| Module | Responsibility |
|--------|----------------|
| axiom-core | Diagnostics, sources, utilities, Result<T> |
| axiom-ast | AST node definitions and printers |
| axiom-lexer | Tokenization |
| axiom-parser | Recursive-descent parser |
| axiom-semantic | Semantic analysis and symbol tables |
| axiom-ir | Intermediate representation (planned) |
| axiom-transform | Transformation passes (planned) |
| axiom-codegen | C++ code generation |
| axiom-runtime | Shared runtime support |
| axiom-cli | Command-line frontend |

---

## Documentation

Coding conventions are documented in:

`docs/CODING_STANDARDS.md`

### API Documentation

Generate API documentation using Doxygen:

```bash
cd docs
doxygen Doxyfile
```

Then open:

```bash
build/docs/html/index.html
```

Requires Doxygen:

https://www.doxygen.nl/

---

## Developer Workflow

Typical workflow:

```bash
cmake --preset debug
cmake --build build-debug
```

Rebuild after changes:

```bash
cmake --build build-debug
```

For release builds:

```bash
cmake --preset release
cmake --build build-release
```

---

## Notes

- CMake presets are configured for macOS development.
- Ninja is required for the build system.
- Debug builds include symbols and are suitable for development.
- Release builds are optimized for performance.

---

## License

TBD

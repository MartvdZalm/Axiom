Here’s a cleaner, more professional README rewrite that improves onboarding, removes machine-specific paths, and presents the project more like a modern compiler/framework project.

# Axiom

Axiom is a reactive **Model-Driven Engineering (MDE)** framework written in modern C++23.

It compiles a domain-specific language (DSL) for hierarchical state machines through a compiler-style pipeline into readable and efficient C++ code.

---

# Features

## Current Milestone

* Lexer with source locations and diagnostics
* Handwritten recursive-descent parser
* Basic parser recovery
* Immutable AST nodes
* AST debug printer
* Semantic validation

  * Duplicate state detection
  * Unknown transition endpoint detection
* C++ code generation

  * Enum-based state representation
  * Switch-based dispatch generation
* Command-line interface

  * `parse`
  * `ast`
  * `generate`

## Planned

* Incremental dependency graphs
* Intermediate Representation (IR) lowering
* Transformation passes
* GUI tooling
* Live editing and hot reload
* Language server support

---

# Requirements

* CMake 3.30+
* C++23 compiler

  * MSVC 2022
  * Clang 18+
  * GCC 14+
* [vcpkg](https://vcpkg.io/)

---

# Quick Start

Clone the repository:

```bash
git clone <repository-url>
cd Axiom
```

Set the `VCPKG_ROOT` environment variable.

## Windows (PowerShell)

```powershell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
```

## Linux / macOS

```bash
export VCPKG_ROOT="$HOME/vcpkg"
```

Configure and build:

```bash
cmake --preset default
cmake --build build --config Release
```

Run tests:

```bash
ctest --preset default
```

---

# Build Presets

Axiom ships with predefined CMake presets.

## Default Visual Studio Build

```bash
cmake --preset default
cmake --build build --config Release
```

## Ninja Release Build

```bash
cmake --preset ninja-release
cmake --build build-ninja
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

# Project Layout

| Module            | Responsibility                               |
| ----------------- | -------------------------------------------- |
| `axiom-core`      | Diagnostics, sources, utilities, `Result<T>` |
| `axiom-ast`       | AST node definitions and printers            |
| `axiom-lexer`     | Tokenization                                 |
| `axiom-parser`    | Recursive-descent parser                     |
| `axiom-semantic`  | Semantic analysis and symbol tables          |
| `axiom-ir`        | Intermediate representation (planned)        |
| `axiom-transform` | Transformation passes (planned)              |
| `axiom-codegen`   | C++ code generation                          |
| `axiom-runtime`   | Shared runtime support                       |
| `axiom-cli`       | Command-line frontend                        |

---

# Documentation

Coding conventions are documented in:

```text
docs/CODING_STANDARDS.md
```

## API Documentation

Generate API documentation using Doxygen:

```bash
cd docs
doxygen Doxyfile
```

Open:

```text
build/docs/html/index.html
```

Requires [Doxygen](https://www.doxygen.nl/) to be installed.

---

# Development Notes

`CMakePresets.json` is platform-independent and uses the `VCPKG_ROOT` environment variable.

Machine-specific overrides should be placed in:

```text
CMakeUserPresets.json
```

This file should not be committed to source control.

---

# License

TBD

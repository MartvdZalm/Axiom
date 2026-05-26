# Axiom Coding Standards

## Ownership and memory

- No raw owning pointers; use `std::unique_ptr` by default.
- `std::shared_ptr` only when ownership is genuinely shared.
- Prefer value semantics and contiguous storage; arena allocators may be introduced for AST storage in later milestones.

## Errors and diagnostics

- Internal hot paths avoid exceptions; surface failures through `axiom::core::Result<T>` and `DiagnosticBag`.
- Every user-facing error includes buffer id, line, and column via `SourceLocation`.
- Do not mutate AST nodes during semantic analysis.

## Architecture boundaries

| Layer      | May depend on                         | Must not depend on        |
|-----------|----------------------------------------|---------------------------|
| core      | STL, fmt                               | ast, lexer, parser, ...   |
| ast       | core                                   | parser, semantic, codegen |
| lexer     | core                                   | parser, ast (node types)  |
| parser    | core, lexer, ast                       | semantic, codegen         |
| semantic  | core, ast                              | codegen                   |
| codegen   | core, ast, semantic                    | parser internals          |

## Style

- C++23, extensions off, aggressive `const` correctness.
- Avoid macros; prefer `enum class` and `constexpr`.
- Avoid virtual dispatch in lexer/parser hot paths.
- Match surrounding naming: `snake_case` files, `PascalCase` types, `snake_case` functions.

## CMake

- One target per module; use `axiom::module` ALIAS targets.
- `INTERFACE` libraries for header-only modules; `STATIC` when `.cpp` translation units exist.
- Apply `axiom_apply_warnings()` to all compiled targets.

## Formatting and analysis

- `.clang-format` — LLVM-based style, 100-column limit.
- `clang-tidy` and `cppcheck` recommended in CI (hooks optional locally).

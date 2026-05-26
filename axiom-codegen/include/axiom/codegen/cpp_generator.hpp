#pragma once

/// @file cpp_generator.hpp
/// @brief Lowers validated state machines to C++ source files.

#include <axiom/ast/nodes.hpp>
#include <axiom/core/result.hpp>
#include <axiom/semantic/analyzer.hpp>

#include <string>
#include <vector>

namespace axiom::codegen {

/// @brief One emitted file (header or translation unit) from code generation.
struct GeneratedFile {
    std::string path;
    std::string contents;
};

/// @brief Complete output of a generate pass (typically `.hpp` + `.cpp` per machine).
struct GeneratedUnit {
    std::vector<GeneratedFile> files;
};

/// @brief Emits readable C++23 from a validated AST and semantic model.
///
/// Milestone 1 maps each @ref axiom::ast::MachineDecl to an `enum class` for states
/// and events plus switch-based `transition` dispatch. IR lowering will sit between
/// semantic analysis and this generator in a later milestone.
///
/// Pipeline: @ref axiom::semantic::SemanticModel + AST → **CppGenerator** → @ref GeneratedUnit.
class CppGenerator {
public:
    [[nodiscard]] core::Result<GeneratedUnit>
    generate(const ast::TranslationUnit& unit, const semantic::SemanticModel& model) const;

private:
    [[nodiscard]] GeneratedUnit generate_machine(const ast::MachineDecl& machine) const;
    [[nodiscard]] std::string sanitize_symbol(std::string_view name) const;
};

} // namespace axiom::codegen

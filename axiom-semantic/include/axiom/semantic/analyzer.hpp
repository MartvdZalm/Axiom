#pragma once

/// @file analyzer.hpp
/// @brief Read-only semantic validation of the AST.

#include <axiom/ast/nodes.hpp>
#include <axiom/core/result.hpp>
#include <axiom/semantic/symbol_table.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace axiom::semantic {

/// @brief Output of semantic analysis: one symbol table per machine name.
struct SemanticModel {
    std::unordered_map<std::string, MachineSymbolTable> machines;
};

/// @brief Validates an AST and builds symbol tables without mutating nodes.
///
/// Checks duplicate states, unknown transition endpoints, and records inferred
/// events. Failures are reported through @ref axiom::core::Result and
/// @ref axiom::core::DiagnosticBag.
///
/// Pipeline: @ref axiom::ast::TranslationUnit → **SemanticAnalyzer** → @ref SemanticModel.
class SemanticAnalyzer {
public:
    [[nodiscard]] core::Result<SemanticModel>
    analyze(std::uint32_t buffer_id, const ast::TranslationUnit& unit);

private:
    void analyze_machine(std::uint32_t buffer_id, const ast::MachineDecl& machine, SemanticModel& out);

    core::DiagnosticBag diagnostics_;
};

} // namespace axiom::semantic

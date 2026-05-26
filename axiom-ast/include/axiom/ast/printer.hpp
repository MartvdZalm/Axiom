#pragma once

/// @file printer.hpp
/// @brief Human-readable AST dump for debugging and tooling.

#include <axiom/ast/nodes.hpp>

#include <string>

namespace axiom::ast {

/// @brief Serializes the AST to a stable, indented text representation.
///
/// Intended for developer inspection rather than round-tripping: the CLI `ast`
/// command and unit tests use this output to verify parse results without invoking
/// the full code generator.
///
/// Does not use @ref AstVisitor internally in Milestone 1; a visitor-based printer
/// may replace the ad hoc walk when more node kinds appear.
class AstPrinter {
public:
    /// @brief Prints an entire @ref TranslationUnit (all machines).
    [[nodiscard]] std::string print(const TranslationUnit& unit) const;

    /// @brief Prints a single @ref MachineDecl subtree.
    [[nodiscard]] std::string print(const MachineDecl& machine) const;
};

} // namespace axiom::ast

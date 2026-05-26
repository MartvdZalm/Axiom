#pragma once

/// @file visitor.hpp
/// @brief Extensible traversal interface over AST nodes.

#include <axiom/ast/nodes.hpp>

namespace axiom::ast {

/// @brief Abstract visitor for read-only AST traversal.
///
/// Provides a stable extension point for passes that need structured walks over the
/// tree (pretty printers, linters, future IR builders) without scattering `switch`
/// logic across the codebase.
///
/// @note Milestone 1 uses @ref AstPrinter for debugging; concrete visitors will
///       multiply as IR and transform passes land.
///
/// Typical usage: subclass, implement `visit_*` hooks, dispatch from a driver that
/// walks @ref TranslationUnit → @ref MachineDecl → states and transitions.
struct AstVisitor {
    virtual ~AstVisitor() = default;

    /// @brief Called for each @ref MachineDecl in a translation unit.
    virtual void visit_machine(const MachineDecl& machine) = 0;

    /// @brief Called for each @ref StateDecl within a machine.
    virtual void visit_state(const StateDecl& state) = 0;

    /// @brief Called for each @ref TransitionDecl within a machine.
    virtual void visit_transition(const TransitionDecl& transition) = 0;
};

} // namespace axiom::ast

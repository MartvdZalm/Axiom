#pragma once

/// @file nodes.hpp
/// @brief AST node definitions for the Axiom state-machine DSL.
///
/// The AST is the structured output of parsing (`.ax` source). It is immutable in
/// spirit: downstream phases read nodes without mutating them. Semantic analysis
/// validates the tree; code generation lowers it to C++.
///
/// Pipeline position:
/// @code
///   Source (.ax) → Lexer → Parser → AST → Semantic Analysis → Code Generation
/// @endcode

#include <axiom/core/source_location.hpp>

#include <memory>
#include <string>
#include <vector>

namespace axiom::ast {

/// @brief Symbolic name for an event referenced by the state machine.
///
/// In the current DSL grammar, events appear inline on `transition` lines
/// (`... on EventName`) rather than as separate top-level declarations. This node
/// type exists for explicit `event` declarations in future grammar revisions and for
/// passes that materialize inferred event symbols during semantic analysis.
///
/// Consumed by: semantic analysis (symbol tables), future IR lowering.
struct EventDecl {
    core::SourceSpan span{};
    std::string name;
};

/// @brief A named state within a machine body.
///
/// Corresponds to a `state Name` line in the DSL. States are local to their parent
/// @ref MachineDecl and are referenced by name from @ref TransitionDecl nodes.
///
/// Consumed by: semantic analysis (duplicate detection), code generation (enum values).
struct StateDecl {
    core::SourceSpan span{};
    std::string name;
};

/// @brief A directed edge triggered by an event between two states.
///
/// Corresponds to `transition Source -> Target on Event` in the DSL. The parser
/// stores state and event names as strings; semantic analysis resolves them against
/// the parent machine's state table.
///
/// Consumed by: semantic analysis (unknown state checks), code generation (switch arms).
struct TransitionDecl {
    core::SourceSpan span{};
    std::string source_state;
    std::string target_state;
    std::string event_name;
};

/// @brief A hierarchical state machine declared in the Axiom DSL.
///
/// Corresponds to a top-level `machine Name { ... }` block. Aggregates all states
/// and transitions for one machine. A translation unit may contain multiple machines.
///
/// Produced by: @ref axiom::parser::Parser.
/// Consumed by: semantic analysis, code generation (one generated C++ type per machine).
struct MachineDecl {
    core::SourceSpan span{};
    std::string name;
    std::vector<StateDecl> states;
    std::vector<TransitionDecl> transitions;
};

/// @brief Root of the AST for a single compilation input.
///
/// Owns the list of top-level @ref MachineDecl nodes parsed from one source buffer.
/// This is the primary handle passed from the parser to semantic analysis and,
/// in Milestone 1, directly to the C++ code generator.
struct TranslationUnit {
    core::SourceSpan span{};
    std::vector<MachineDecl> machines;
};

} // namespace axiom::ast

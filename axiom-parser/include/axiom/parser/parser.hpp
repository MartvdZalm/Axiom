#pragma once

/// @file parser.hpp
/// @brief Builds @ref axiom::ast::TranslationUnit from a token stream.

#include <axiom/ast/nodes.hpp>
#include <axiom/core/result.hpp>
#include <axiom/core/source_manager.hpp>
#include <axiom/lexer/lexer.hpp>

namespace axiom::parser {

/// @brief Handwritten recursive-descent parser for the Axiom DSL.
///
/// Consumes @ref axiom::lexer::Token values and produces an AST. Uses predictive
/// parsing (no generated tables) and basic synchronization on errors inside machine
/// bodies so multiple diagnostics can be reported per file.
///
/// Pipeline: @ref axiom::lexer::Lexer → **Parser** → @ref axiom::ast::TranslationUnit.
class Parser {
public:
    Parser(core::SourceManager& sources, std::uint32_t buffer_id);

    /// @brief Parses one translation unit (all top-level `machine` declarations).
    [[nodiscard]] core::Result<ast::TranslationUnit> parse_translation_unit();

private:
    [[nodiscard]] ast::MachineDecl parse_machine();
    [[nodiscard]] ast::StateDecl parse_state();
    [[nodiscard]] ast::TransitionDecl parse_transition();

    [[nodiscard]] lexer::Token current() const;
    [[nodiscard]] bool check(lexer::TokenKind kind) const;
    bool match(lexer::TokenKind kind);
    lexer::Token advance();
    lexer::Token expect(lexer::TokenKind kind, const char* context);

    void synchronize_within_machine();
    void report_error(const lexer::Token& at, std::string message);

    core::SourceManager& sources_;
    std::uint32_t buffer_id_;
    lexer::Lexer lexer_;
    lexer::Token current_;
    core::DiagnosticBag diagnostics_;
};

} // namespace axiom::parser

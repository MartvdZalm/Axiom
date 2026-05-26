#pragma once

/// @file lexer.hpp
/// @brief Scans Axiom DSL source into a stream of tokens.

#include <axiom/core/diagnostic.hpp>
#include <axiom/core/source_manager.hpp>
#include <axiom/lexer/token.hpp>

namespace axiom::lexer {

/// @brief UTF-8-aware scanner for a single @ref axiom::core::SourceManager buffer.
///
/// First phase of the compiler pipeline. Skips whitespace and `//` comments, recognizes
/// keywords and punctuation, and records @ref core::SourceSpan on every token for the
/// parser and diagnostic engine.
///
/// Design notes:
/// - Hand-written (no lexer generator) for predictable errors and zero code-gen deps.
/// - Byte-oriented with UTF-8 continuation handling; lexemes are `string_view` slices.
/// - Invalid characters produce @ref TokenKind::Invalid and an entry in @ref diagnostics().
///
/// Pipeline: @ref SourceManager → **Lexer** → @ref axiom::parser::Parser → AST.
class Lexer {
public:
    /// @brief Constructs a lexer positioned at the start of @p buffer_id.
    Lexer(const core::SourceManager& sources, std::uint32_t buffer_id);

    /// @brief Returns the next token and advances the scan position.
    [[nodiscard]] Token next();

    /// @brief Looks ahead one token without consuming it.
    [[nodiscard]] Token peek();

    /// @brief Discards the current peeked or next token (implements parser lookahead).
    void consume();

    /// @brief Lexical errors accumulated since construction or last clear.
    [[nodiscard]] const core::DiagnosticBag& diagnostics() const noexcept { return diagnostics_; }

private:
    [[nodiscard]] char current() const;
    [[nodiscard]] char peek_char(std::size_t ahead = 1) const;
    void advance();
    void skip_whitespace_and_comments();
    [[nodiscard]] Token make_token(TokenKind kind, std::size_t start, std::size_t end);
    [[nodiscard]] Token lex_identifier_or_keyword(std::size_t start);
    [[nodiscard]] TokenKind keyword_kind(std::string_view text) const;
    [[nodiscard]] core::SourceLocation location_at(std::size_t offset) const;

    const core::SourceManager& sources_;
    std::uint32_t buffer_id_;
    std::string_view source_;
    std::size_t pos_{0};
    std::size_t line_{1};
    std::size_t column_{1};
    bool has_peek_{false};
    Token peeked_{};
    core::DiagnosticBag diagnostics_;
};

} // namespace axiom::lexer

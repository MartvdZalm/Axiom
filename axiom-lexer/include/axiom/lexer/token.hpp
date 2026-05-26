#pragma once

/// @file token.hpp
/// @brief Lexical token with source span and zero-copy lexeme.

#include <axiom/core/source_location.hpp>
#include <axiom/lexer/token_kind.hpp>

#include <string_view>

namespace axiom::lexer {

/// @brief One lexical unit with kind, source span, and text.
///
/// `lexeme` is a non-owning view into the @ref axiom::core::SourceManager buffer;
/// it remains valid until that buffer is destroyed. The parser consumes tokens via
/// @ref Lexer::next and never retains lexemes across buffer reloads.
struct Token {
    TokenKind kind{TokenKind::EndOfFile};
    core::SourceSpan span{};
    std::string_view lexeme{};

    [[nodiscard]] constexpr bool is(TokenKind k) const noexcept { return kind == k; }
    [[nodiscard]] constexpr bool is_eof() const noexcept { return kind == TokenKind::EndOfFile; }
    [[nodiscard]] constexpr bool is_invalid() const noexcept { return kind == TokenKind::Invalid; }
};

/// @brief Stable string name for a @ref TokenKind (diagnostics, tests).
[[nodiscard]] const char* token_kind_name(TokenKind kind);

} // namespace axiom::lexer

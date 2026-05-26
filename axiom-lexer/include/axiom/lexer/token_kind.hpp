#pragma once

/// @file token_kind.hpp
/// @brief Classification of lexical units for the Axiom DSL.

#include <cstdint>

namespace axiom::lexer {

/// @brief Discriminant for all tokens produced by @ref Lexer.
///
/// Keywords are distinct kinds (not generic identifiers) so the recursive-descent
/// parser can branch predictably without a symbol table at parse time.
enum class TokenKind : std::uint8_t {
    EndOfFile,     ///< Sentinel past the last character.
    Identifier,    ///< User-defined names (machines, states, events).
    MachineKw,     ///< `machine`
    StateKw,       ///< `state`
    TransitionKw,  ///< `transition`
    OnKw,          ///< `on` (transition trigger)
    LBrace,        ///< `{`
    RBrace,        ///< `}`
    Arrow,         ///< `->`
    Invalid,       ///< Lexically invalid input; usually followed by a diagnostic.
};

} // namespace axiom::lexer

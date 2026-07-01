#include <axiom/lexer/token.hpp>

namespace axiom::lexer
{

const char* token_kind_name(TokenKind kind)
{
    switch (kind)
    {
    case TokenKind::EndOfFile:
        return "EOF";
    case TokenKind::Identifier:
        return "identifier";
    case TokenKind::MachineKw:
        return "machine";
    case TokenKind::StateKw:
        return "state";
    case TokenKind::TransitionKw:
        return "transition";
    case TokenKind::OnKw:
        return "on";
    case TokenKind::LBrace:
        return "{";
    case TokenKind::RBrace:
        return "}";
    case TokenKind::Arrow:
        return "->";
    case TokenKind::Invalid:
        return "invalid";
    }
    return "unknown";
}

} // namespace axiom::lexer

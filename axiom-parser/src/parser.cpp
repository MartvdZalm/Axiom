#include <axiom/parser/parser.hpp>

#include <fmt/format.h>

namespace axiom::parser {

Parser::Parser(core::SourceManager& sources, std::uint32_t buffer_id)
    : sources_(sources), buffer_id_(buffer_id), lexer_(sources, buffer_id) {
    current_ = lexer_.next();
}

lexer::Token Parser::current() const { return current_; }

bool Parser::check(lexer::TokenKind kind) const { return current_.kind == kind; }

bool Parser::match(lexer::TokenKind kind) {
    if (!check(kind)) {
        return false;
    }
    advance();
    return true;
}

lexer::Token Parser::advance() {
    const auto previous = current_;
    current_ = lexer_.next();
    return previous;
}

lexer::Token Parser::expect(lexer::TokenKind kind, const char* context) {
    if (!check(kind)) {
        report_error(
            current_,
            fmt::format("expected {} while parsing {}", lexer::token_kind_name(kind), context));
        return current_;
    }
    return advance();
}

void Parser::report_error(const lexer::Token& at, std::string message) {
    diagnostics_.add_error(buffer_id_, at.span.begin, std::move(message));
}

void Parser::synchronize_within_machine() {
    while (!current_.is_eof()) {
        if (current_.is(lexer::TokenKind::RBrace)) {
            return;
        }
        if (current_.is(lexer::TokenKind::StateKw) || current_.is(lexer::TokenKind::TransitionKw) ||
            current_.is(lexer::TokenKind::MachineKw)) {
            return;
        }
        advance();
    }
}

core::Result<ast::TranslationUnit> Parser::parse_translation_unit() {
    ast::TranslationUnit unit{};
    const auto unit_begin = current_.span.begin;

    while (!check(lexer::TokenKind::EndOfFile)) {
        if (check(lexer::TokenKind::MachineKw)) {
            unit.machines.push_back(parse_machine());
            continue;
        }

        if (current_.is_invalid()) {
            advance();
            synchronize_within_machine();
            continue;
        }

        report_error(current_, "expected top-level machine declaration");
        advance();
        synchronize_within_machine();
    }

    unit.span = core::SourceSpan{unit_begin, current_.span.begin};
    diagnostics_.merge(lexer_.diagnostics());

    if (diagnostics_.has_errors()) {
        return core::Result<ast::TranslationUnit>::fail(std::move(diagnostics_));
    }
    return core::Result<ast::TranslationUnit>::ok(std::move(unit));
}

ast::MachineDecl Parser::parse_machine() {
    const auto begin = current_.span.begin;
    expect(lexer::TokenKind::MachineKw, "machine");

    const auto name_tok = expect(lexer::TokenKind::Identifier, "machine name");
    ast::MachineDecl machine{};
    machine.name = std::string(name_tok.lexeme);
    machine.span.begin = begin;

    expect(lexer::TokenKind::LBrace, "machine body");

    while (!check(lexer::TokenKind::RBrace) && !check(lexer::TokenKind::EndOfFile)) {
        if (check(lexer::TokenKind::StateKw)) {
            machine.states.push_back(parse_state());
            continue;
        }

        if (match(lexer::TokenKind::TransitionKw)) {
            machine.transitions.push_back(parse_transition());
            continue;
        }

        if (current_.is_invalid()) {
            advance();
            synchronize_within_machine();
            continue;
        }

        report_error(current_, "expected 'state' or 'transition' in machine body");
        advance();
        synchronize_within_machine();
    }

    const auto close = expect(lexer::TokenKind::RBrace, "machine body");
    machine.span.end = close.span.end;
    return machine;
}

ast::StateDecl Parser::parse_state() {
    const auto begin = current_.span.begin;
    expect(lexer::TokenKind::StateKw, "state");
    const auto name_tok = expect(lexer::TokenKind::Identifier, "state name");
    return ast::StateDecl{core::SourceSpan{begin, name_tok.span.end}, std::string(name_tok.lexeme)};
}

ast::TransitionDecl Parser::parse_transition() {
    const auto begin = current_.span.begin;

    const auto source_tok = expect(lexer::TokenKind::Identifier, "transition source state");
    expect(lexer::TokenKind::Arrow, "transition");
    const auto target_tok = expect(lexer::TokenKind::Identifier, "transition target state");
    expect(lexer::TokenKind::OnKw, "transition");
    const auto event_tok = expect(lexer::TokenKind::Identifier, "transition event");

    ast::TransitionDecl transition{};
    transition.source_state = std::string(source_tok.lexeme);
    transition.target_state = std::string(target_tok.lexeme);
    transition.event_name = std::string(event_tok.lexeme);
    transition.span = core::SourceSpan{begin, event_tok.span.end};
    return transition;
}

} // namespace axiom::parser

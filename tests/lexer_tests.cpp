#include <axiom/lexer/lexer.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>

using namespace axiom;

TEST_CASE("lexer tokenizes wafer stage keywords and punctuation", "[lexer]")
{
    const std::string source = R"(
machine WaferStage {
  state Idle
  transition Idle -> Moving on StartMove
}
)";

    core::SourceManager sources;
    const auto buffer = sources.add_memory_buffer("test.ax", source);

    lexer::Lexer lex(sources, buffer);
    const auto machine = lex.next();
    REQUIRE(machine.is(lexer::TokenKind::MachineKw));

    const auto name = lex.next();
    REQUIRE(name.is(lexer::TokenKind::Identifier));
    REQUIRE(name.lexeme == "WaferStage");

    const auto lbrace = lex.next();
    REQUIRE(lbrace.is(lexer::TokenKind::LBrace));

    const auto state_kw = lex.next();
    REQUIRE(state_kw.is(lexer::TokenKind::StateKw));
}

TEST_CASE("lexer reports invalid characters", "[lexer]")
{
    core::SourceManager sources;
    const auto buffer = sources.add_memory_buffer("bad.ax", "@");

    lexer::Lexer lex(sources, buffer);
    const auto invalid = lex.next();
    REQUIRE(invalid.is(lexer::TokenKind::Invalid));
    REQUIRE(lex.diagnostics().has_errors());
}

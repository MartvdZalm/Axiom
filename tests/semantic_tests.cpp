#include <axiom/parser/parser.hpp>
#include <axiom/semantic/analyzer.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace axiom;

TEST_CASE("semantic analysis rejects duplicate states", "[semantic]") {
    core::SourceManager sources;
    const auto buffer = sources.add_memory_buffer(
        "dup.ax",
        "machine M { state A state A }");

    parser::Parser parser(sources, buffer);
    const auto parsed = parser.parse_translation_unit();
    REQUIRE(parsed.succeeded());

    semantic::SemanticAnalyzer analyzer;
    const auto result = analyzer.analyze(buffer, parsed.value());
    REQUIRE_FALSE(result.succeeded());
    REQUIRE(result.diagnostics().has_errors());
}

TEST_CASE("semantic analysis rejects unknown transition state", "[semantic]") {
    core::SourceManager sources;
    const auto buffer = sources.add_memory_buffer(
        "unknown.ax",
        "machine M { state A transition A -> Missing on E }");

    parser::Parser parser(sources, buffer);
    const auto parsed = parser.parse_translation_unit();
    REQUIRE(parsed.succeeded());

    semantic::SemanticAnalyzer analyzer;
    const auto result = analyzer.analyze(buffer, parsed.value());
    REQUIRE_FALSE(result.succeeded());
}

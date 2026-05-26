#include <axiom/parser/parser.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace axiom;

namespace {

constexpr const char* kWaferStage = R"(machine WaferStage {
  state Idle
  state Moving
  transition Idle -> Moving on StartMove
  transition Moving -> Idle on Stop
})";

} // namespace

TEST_CASE("parser builds machine AST", "[parser]") {
    core::SourceManager sources;
    const auto buffer = sources.add_memory_buffer("wafer.ax", kWaferStage);

    parser::Parser parser(sources, buffer);
    const auto result = parser.parse_translation_unit();
    REQUIRE(result.succeeded());
    REQUIRE(result.value().machines.size() == 1);

    const auto& machine = result.value().machines.front();
    REQUIRE(machine.name == "WaferStage");
    REQUIRE(machine.states.size() == 2);
    REQUIRE(machine.transitions.size() == 2);
}

TEST_CASE("parser recovers from malformed machine body", "[parser]") {
    core::SourceManager sources;
    const auto buffer = sources.add_memory_buffer(
        "broken.ax",
        "machine M { state A @@ transition A -> B on E }");

    parser::Parser parser(sources, buffer);
    const auto result = parser.parse_translation_unit();
    REQUIRE_FALSE(result.succeeded());
}

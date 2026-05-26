#include <axiom/codegen/cpp_generator.hpp>
#include <axiom/parser/parser.hpp>
#include <axiom/semantic/analyzer.hpp>

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>

using namespace axiom;

namespace {

std::string read_fixture(const char* relative) {
    const std::filesystem::path path =
        std::filesystem::path(AXIOM_TEST_DATA_DIR) / relative;
    std::ifstream in(path);
    REQUIRE(in);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

} // namespace

TEST_CASE("codegen snapshot for WaferStage", "[codegen]") {
    const auto source = read_fixture("wafer_stage.ax");
    const auto expected_header = read_fixture("expected/WaferStage.hpp.snap");
    const auto expected_source = read_fixture("expected/WaferStage.cpp.snap");

    core::SourceManager sources;
    const auto buffer = sources.add_memory_buffer("wafer.ax", source);

    parser::Parser parser(sources, buffer);
    const auto parsed = parser.parse_translation_unit();
    REQUIRE(parsed.succeeded());

    semantic::SemanticAnalyzer analyzer;
    const auto semantic = analyzer.analyze(buffer, parsed.value());
    REQUIRE(semantic.succeeded());

    codegen::CppGenerator generator;
    const auto generated = generator.generate(parsed.value(), semantic.value());
    REQUIRE(generated.succeeded());
    REQUIRE(generated.value().files.size() == 2);

    REQUIRE(generated.value().files[0].contents == expected_header);
    REQUIRE(generated.value().files[1].contents == expected_source);
}

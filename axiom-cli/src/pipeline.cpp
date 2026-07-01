#include <axiom/cli/pipeline.hpp>
#include <axiom/parser/parser.hpp>
#include <axiom/semantic/analyzer.hpp>

namespace axiom::cli
{

Pipeline::Pipeline(core::SourceManager& sources) : sources_(sources)
{
}

core::Result<ast::TranslationUnit> Pipeline::parse_file(const std::string& path)
{
    diagnostics_.clear();

    const auto loaded = sources_.load_file(path);
    if (!loaded)
    {
        diagnostics_.add_error(0, {}, loaded.error());
        return core::Result<ast::TranslationUnit>::fail(std::move(diagnostics_));
    }

    last_buffer_ = *loaded;
    parser::Parser parser(sources_, last_buffer_);
    auto result = parser.parse_translation_unit();
    if (!result.succeeded())
    {
        diagnostics_.merge(result.diagnostics());
        return core::Result<ast::TranslationUnit>::fail(std::move(diagnostics_));
    }
    return result;
}

core::Result<semantic::SemanticModel> Pipeline::analyze(const ast::TranslationUnit& unit,
                                                        std::uint32_t buffer_id)
{
    semantic::SemanticAnalyzer analyzer{};
    auto result = analyzer.analyze(buffer_id, unit);
    if (!result.succeeded())
    {
        diagnostics_.merge(result.diagnostics());
        return core::Result<semantic::SemanticModel>::fail(std::move(diagnostics_));
    }
    return result;
}

core::Result<codegen::GeneratedUnit> Pipeline::generate(const ast::TranslationUnit& unit,
                                                        const semantic::SemanticModel& model)
{
    codegen::CppGenerator generator{};
    return generator.generate(unit, model);
}

core::Result<CompileArtifacts> Pipeline::compile(const CompileInput& input)
{
    diagnostics_.clear();

    auto parsed = parse_file(input.source_path);
    if (!parsed.succeeded())
    {
        return core::Result<CompileArtifacts>::fail(std::move(diagnostics_));
    }

    auto analyzed = analyze(parsed.value(), last_buffer_);
    if (!analyzed.succeeded())
    {
        return core::Result<CompileArtifacts>::fail(std::move(diagnostics_));
    }

    auto generated = generate(parsed.value(), analyzed.value());
    if (!generated.succeeded())
    {
        return core::Result<CompileArtifacts>::fail(std::move(diagnostics_));
    }

    CompileArtifacts artifacts{};
    artifacts.buffer_id = last_buffer_;
    artifacts.ast = std::move(parsed.value());
    artifacts.semantic = std::move(analyzed.value());
    artifacts.generated = std::move(generated.value());
    return core::Result<CompileArtifacts>::ok(std::move(artifacts));
}

} // namespace axiom::cli

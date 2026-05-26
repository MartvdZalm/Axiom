#pragma once

#include <axiom/ast/nodes.hpp>
#include <axiom/codegen/cpp_generator.hpp>
#include <axiom/core/result.hpp>
#include <axiom/core/source_manager.hpp>
#include <axiom/semantic/analyzer.hpp>

#include <string>

namespace axiom::cli {

struct CompileInput {
    std::string source_path;
};

struct CompileArtifacts {
    std::uint32_t buffer_id{0};
    ast::TranslationUnit ast{};
    semantic::SemanticModel semantic{};
    codegen::GeneratedUnit generated{};
};

class Pipeline {
public:
    explicit Pipeline(core::SourceManager& sources);

    [[nodiscard]] core::Result<ast::TranslationUnit> parse_file(const std::string& path);
    [[nodiscard]] core::Result<semantic::SemanticModel>
    analyze(const ast::TranslationUnit& unit, std::uint32_t buffer_id);
    [[nodiscard]] core::Result<codegen::GeneratedUnit>
    generate(const ast::TranslationUnit& unit, const semantic::SemanticModel& model);

    [[nodiscard]] core::Result<CompileArtifacts> compile(const CompileInput& input);

    [[nodiscard]] const core::SourceManager& sources() const noexcept { return sources_; }
    [[nodiscard]] core::DiagnosticBag& diagnostics() noexcept { return diagnostics_; }
    [[nodiscard]] const core::DiagnosticBag& diagnostics() const noexcept { return diagnostics_; }

private:
    core::SourceManager& sources_;
    core::DiagnosticBag diagnostics_;
    std::uint32_t last_buffer_{0};
};

} // namespace axiom::cli

#pragma once

/// @file diagnostic.hpp
/// @brief Structured compiler messages and aggregation for pipeline stages.

#include <axiom/core/source_location.hpp>

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace axiom::core {

class SourceManager;

/// @brief Severity of a compiler diagnostic.
enum class DiagnosticSeverity : std::uint8_t {
    Note,    ///< Informational; does not fail the compilation.
    Warning, ///< Potential issue; compilation may still succeed.
    Error,   ///< Fatal for the current phase; fails @ref Result.
};

/// @brief A single message anchored to a source location.
///
/// Lexer, parser, and semantic phases append diagnostics instead of throwing.
/// Each diagnostic references a @ref SourceManager buffer id so messages can be
/// rendered with file/line/column context.
struct Diagnostic {
    DiagnosticSeverity severity{DiagnosticSeverity::Error};
    std::string message;
    std::uint32_t buffer_id{0};
    SourceLocation location{};
};

/// @brief Collects diagnostics produced during one pipeline step or compilation.
///
/// Stages merge bags as needed (e.g. parser absorbs lexer errors). @ref Result
/// carries a bag on failure so the CLI can print all issues in one pass.
class DiagnosticBag {
public:
    void add(Diagnostic diag);

    /// @brief Convenience helper for error-level messages.
    void add_error(std::uint32_t buffer_id, SourceLocation loc, std::string message);

    /// @brief Convenience helper for warning-level messages.
    void add_warning(std::uint32_t buffer_id, SourceLocation loc, std::string message);

    [[nodiscard]] const std::vector<Diagnostic>& all() const noexcept { return items_; }

    /// @brief True if any stored diagnostic has @ref DiagnosticSeverity::Error.
    [[nodiscard]] bool has_errors() const noexcept;

    [[nodiscard]] bool empty() const noexcept { return items_.empty(); }

    /// @brief Renders all diagnostics using @p sources for location formatting.
    [[nodiscard]] std::string format(const SourceManager& sources) const;

    /// @brief Appends diagnostics from another bag (e.g. lexer → parser handoff).
    void merge(const DiagnosticBag& other);

    void clear() noexcept { items_.clear(); }

private:
    std::vector<Diagnostic> items_;
};

} // namespace axiom::core

#pragma once

/// @file source_location.hpp
/// @brief Source coordinates shared across lexer, parser, and diagnostics.

#include <cstdint>
#include <string>
#include <string_view>

namespace axiom::core {

/// @brief A single point in a source buffer (byte offset plus line/column).
///
/// Offsets are byte indices into UTF-8 source text; line and column are maintained
/// for human-readable diagnostics. All AST nodes and tokens carry spans built from
/// these locations.
struct SourceLocation {
    std::uint32_t offset{0};
    std::uint32_t line{1};
    std::uint32_t column{1};

    constexpr SourceLocation() = default;
    constexpr SourceLocation(std::uint32_t off, std::uint32_t ln, std::uint32_t col)
        : offset(off), line(ln), column(col) {}
};

/// @brief Half-open source range `[begin, end)` for AST nodes and tokens.
struct SourceSpan {
    SourceLocation begin{};
    SourceLocation end{};

    constexpr SourceSpan() = default;
    constexpr SourceSpan(SourceLocation b, SourceLocation e) : begin(b), end(e) {}
};

/// @brief View of one loaded source file or in-memory buffer.
///
/// `text` is non-owning; lifetime is guaranteed by @ref SourceManager for the
/// duration of a compilation.
struct SourceBuffer {
    std::string_view text;
    std::string file_path;

    [[nodiscard]] constexpr bool empty() const noexcept { return text.empty(); }
    [[nodiscard]] std::size_t size() const noexcept { return text.size(); }
};

} // namespace axiom::core

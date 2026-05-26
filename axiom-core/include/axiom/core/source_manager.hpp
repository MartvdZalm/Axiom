#pragma once

/// @file source_manager.hpp
/// @brief Owns source text and resolves locations for diagnostics and lexing.

#include <axiom/core/source_location.hpp>

#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace axiom::core {

/// @brief Central registry of source buffers for one compilation session.
///
/// Every phase that needs file contents or location formatting goes through the
/// manager. Lexers hold a buffer id; diagnostics store `(buffer_id, SourceLocation)`
/// pairs that the manager can turn into `path:line:column` strings.
///
/// Pipeline role: created by the CLI/driver, passed to lexer and parser, referenced
/// when formatting @ref DiagnosticBag output.
class SourceManager {
public:
    /// @brief Registers in-memory source (tests, REPL) and returns a stable buffer id.
    std::uint32_t add_memory_buffer(std::string name, std::string content);

    /// @brief Reads a file from disk into a new buffer.
    /// @return Buffer id on success, or an error message if the file cannot be read.
    [[nodiscard]] std::expected<std::uint32_t, std::string> load_file(const std::string& path);

    /// @brief Returns buffer metadata, or `std::nullopt` if @p id is invalid.
    [[nodiscard]] std::optional<SourceBuffer> buffer(std::uint32_t id) const;

    /// @brief Full text of a buffer; empty view if @p id is invalid.
    [[nodiscard]] std::string_view text(std::uint32_t id) const;

    /// @brief Display name (usually path) for diagnostics.
    [[nodiscard]] std::string_view file_name(std::uint32_t id) const;

    /// @brief Extracts the exact substring covered by @p span within @p buffer_id.
    [[nodiscard]] std::string_view slice(std::uint32_t buffer_id, SourceSpan span) const;

    /// @brief Formats `file_name:line:column` for a location in a buffer.
    [[nodiscard]] std::string format_location(std::uint32_t buffer_id, SourceLocation loc) const;

private:
    struct Entry {
        std::string name;
        std::string content;
    };

    std::vector<Entry> buffers_;
};

} // namespace axiom::core

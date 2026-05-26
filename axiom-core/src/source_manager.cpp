#include <axiom/core/source_manager.hpp>

#include <fmt/format.h>

#include <fstream>
#include <sstream>

namespace axiom::core {

std::uint32_t SourceManager::add_memory_buffer(std::string name, std::string content) {
    const auto id = static_cast<std::uint32_t>(buffers_.size());
    buffers_.push_back(Entry{std::move(name), std::move(content)});
    return id;
}

std::expected<std::uint32_t, std::string> SourceManager::load_file(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        return std::unexpected(fmt::format("could not open '{}'", path));
    }

    std::ostringstream ss;
    ss << stream.rdbuf();
    return add_memory_buffer(path, ss.str());
}

std::optional<SourceBuffer> SourceManager::buffer(std::uint32_t id) const {
    if (id >= buffers_.size()) {
        return std::nullopt;
    }
    const auto& entry = buffers_[id];
    return SourceBuffer{entry.content, entry.name};
}

std::string_view SourceManager::text(std::uint32_t id) const {
    if (id >= buffers_.size()) {
        return {};
    }
    return buffers_[id].content;
}

std::string_view SourceManager::file_name(std::uint32_t id) const {
    if (id >= buffers_.size()) {
        return {};
    }
    return buffers_[id].name;
}

std::string_view SourceManager::slice(std::uint32_t buffer_id, SourceSpan span) const {
    const auto src = text(buffer_id);
    const auto begin = static_cast<std::size_t>(span.begin.offset);
    const auto end = static_cast<std::size_t>(span.end.offset);
    if (begin > src.size() || end > src.size() || begin > end) {
        return {};
    }
    return src.substr(begin, end - begin);
}

std::string SourceManager::format_location(std::uint32_t buffer_id, SourceLocation loc) const {
    return fmt::format("{}:{}:{}", file_name(buffer_id), loc.line, loc.column);
}

} // namespace axiom::core

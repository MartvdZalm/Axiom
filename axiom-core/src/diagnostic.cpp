#include <axiom/core/diagnostic.hpp>
#include <axiom/core/source_manager.hpp>
#include <fmt/format.h>

namespace axiom::core
{

namespace
{

const char* severity_label(DiagnosticSeverity s)
{
    switch (s)
    {
    case DiagnosticSeverity::Note:
        return "note";
    case DiagnosticSeverity::Warning:
        return "warning";
    case DiagnosticSeverity::Error:
        return "error";
    }
    return "error";
}

} // namespace

void DiagnosticBag::add(Diagnostic diag)
{
    items_.push_back(std::move(diag));
}

void DiagnosticBag::add_error(std::uint32_t buffer_id, SourceLocation loc, std::string message)
{
    add(Diagnostic{DiagnosticSeverity::Error, std::move(message), buffer_id, loc});
}

void DiagnosticBag::add_warning(std::uint32_t buffer_id, SourceLocation loc, std::string message)
{
    add(Diagnostic{DiagnosticSeverity::Warning, std::move(message), buffer_id, loc});
}

void DiagnosticBag::merge(const DiagnosticBag& other)
{
    for (const auto& d : other.all())
    {
        add(d);
    }
}

bool DiagnosticBag::has_errors() const noexcept
{
    for (const auto& d : items_)
    {
        if (d.severity == DiagnosticSeverity::Error)
        {
            return true;
        }
    }
    return false;
}

std::string DiagnosticBag::format(const SourceManager& sources) const
{
    std::string out;
    for (const auto& d : items_)
    {
        const auto loc = sources.format_location(d.buffer_id, d.location);
        out += fmt::format("{}: {}: {}\n", loc, severity_label(d.severity), d.message);
    }
    return out;
}

} // namespace axiom::core

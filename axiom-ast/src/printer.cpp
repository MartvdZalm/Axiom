#include <axiom/ast/printer.hpp>
#include <fmt/format.h>

namespace axiom::ast
{

namespace
{

std::string indent(int level)
{
    return std::string(static_cast<std::size_t>(level) * 2, ' ');
}

} // namespace

std::string AstPrinter::print(const TranslationUnit& unit) const
{
    std::string out = "TranslationUnit\n";
    for (const auto& machine : unit.machines)
    {
        out += print(machine);
    }
    return out;
}

std::string AstPrinter::print(const MachineDecl& machine) const
{
    std::string out = fmt::format("  MachineDecl name={}\n", machine.name);
    for (const auto& state : machine.states)
    {
        out += fmt::format("    StateDecl name={}\n", state.name);
    }
    for (const auto& transition : machine.transitions)
    {
        out += fmt::format("    TransitionDecl {} -> {} on {}\n", transition.source_state,
                           transition.target_state, transition.event_name);
    }
    return out;
}

} // namespace axiom::ast

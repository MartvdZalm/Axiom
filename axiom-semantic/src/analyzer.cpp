#include <axiom/semantic/analyzer.hpp>

#include <fmt/format.h>
#include <unordered_set>

namespace axiom::semantic {

core::Result<SemanticModel>
SemanticAnalyzer::analyze(std::uint32_t buffer_id, const ast::TranslationUnit& unit) {
    SemanticModel model{};
    diagnostics_.clear();

    for (const auto& machine : unit.machines) {
        analyze_machine(buffer_id, machine, model);
    }

    if (diagnostics_.has_errors()) {
        return core::Result<SemanticModel>::fail(std::move(diagnostics_));
    }
    return core::Result<SemanticModel>::ok(std::move(model));
}

void SemanticAnalyzer::analyze_machine(
    std::uint32_t buffer_id,
    const ast::MachineDecl& machine,
    SemanticModel& out) {
    MachineSymbolTable table{};

    for (std::size_t i = 0; i < machine.states.size(); ++i) {
        const auto& state = machine.states[i];
        if (!table.declare_state(state.name, i)) {
            diagnostics_.add_error(
                buffer_id,
                state.span.begin,
                fmt::format("duplicate state '{}'", state.name));
        }
    }

    std::unordered_set<std::string> referenced_events;
    for (const auto& transition : machine.transitions) {
        referenced_events.insert(transition.event_name);
    }
    for (const auto& event_name : referenced_events) {
        table.declare_event(event_name);
    }

    for (const auto& transition : machine.transitions) {
        if (!table.has_state(transition.source_state)) {
            diagnostics_.add_error(
                buffer_id,
                transition.span.begin,
                fmt::format("unknown source state '{}'", transition.source_state));
        }
        if (!table.has_state(transition.target_state)) {
            diagnostics_.add_error(
                buffer_id,
                transition.span.begin,
                fmt::format("unknown target state '{}'", transition.target_state));
        }
    }

    out.machines.emplace(machine.name, std::move(table));
}

} // namespace axiom::semantic

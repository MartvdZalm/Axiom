#include <axiom/semantic/symbol_table.hpp>

namespace axiom::semantic {

bool MachineSymbolTable::declare_state(std::string name, std::size_t ast_index) {
    if (state_index_.contains(name)) {
        return false;
    }
    const auto index = states_.size();
    state_index_[name] = index;
    states_.push_back(StateSymbol{std::move(name), ast_index});
    return true;
}

void MachineSymbolTable::declare_event(std::string name) {
    if (event_index_.contains(name)) {
        return;
    }
    const auto index = events_.size();
    event_index_[name] = index;
    events_.push_back(EventSymbol{std::move(name)});
}

bool MachineSymbolTable::has_state(std::string_view name) const {
    return state_index_.contains(std::string(name));
}

bool MachineSymbolTable::has_event(std::string_view name) const {
    return event_index_.contains(std::string(name));
}

} // namespace axiom::semantic

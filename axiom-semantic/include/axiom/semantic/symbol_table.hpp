#pragma once

/// @file symbol_table.hpp
/// @brief Per-machine symbol tables produced during semantic analysis.

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace axiom::semantic {

/// @brief A resolved state symbol with its index in the machine's state list.
struct StateSymbol {
    std::string name;
    std::size_t ast_index{0};
};

/// @brief An event symbol inferred from transition clauses.
struct EventSymbol {
    std::string name;
};

/// @brief Name lookup tables for a single @ref axiom::ast::MachineDecl.
///
/// Built by @ref SemanticAnalyzer; consumed by codegen and future semantic passes.
class MachineSymbolTable {
public:
    /// @return False if @p name is already declared in this machine.
    bool declare_state(std::string name, std::size_t ast_index);
    void declare_event(std::string name);

    [[nodiscard]] bool has_state(std::string_view name) const;
    [[nodiscard]] bool has_event(std::string_view name) const;
    [[nodiscard]] const std::vector<StateSymbol>& states() const noexcept { return states_; }
    [[nodiscard]] const std::vector<EventSymbol>& events() const noexcept { return events_; }

private:
    std::unordered_map<std::string, std::size_t> state_index_;
    std::vector<StateSymbol> states_;
    std::unordered_map<std::string, std::size_t> event_index_;
    std::vector<EventSymbol> events_;
};

} // namespace axiom::semantic

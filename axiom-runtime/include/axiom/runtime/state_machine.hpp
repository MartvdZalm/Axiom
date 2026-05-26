#pragma once

// Shared runtime helpers for generated machines (expanded in later milestones).

namespace axiom::runtime {

template<typename State, typename Event>
struct StateMachineTraits {
    static State initial();
    static State transition(State current, Event event);
};

} // namespace axiom::runtime

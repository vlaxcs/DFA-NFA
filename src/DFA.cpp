#include <cassert>

#include <DFA.h>
#include <format>
#include <Setup.h>
#include <UserWarn.h>

DFA::DFA(const std::string& file) {
    const Setup setup(file);
    setSigma(setup.getSigma());
    setStates(setup.getStates());
    setTransitions(setup.getTransitions());
    setStartState();
    validate();
}

void DFA::validate() {
    for (const auto& state : states) {
        for (const auto& symbol : sigma) {
            auto transitionsWithSymbol = state->transitions.equal_range(symbol);
            size_t count = std::distance(transitionsWithSymbol.first, transitionsWithSymbol.second);
            if (count > 1) {
                UserWarn(std::format("There are mutliple states leading from {} with symbol {}", state->name, symbol));
            }
        }
    }
}

bool DFA::process(const std::string& word) const {
    auto currentState = startState;
    assert(currentState != nullptr);

    if (word.empty()) {
        return currentState->final;
    }

    for (const auto &symbol : word) {
        auto transitionsWithSymbol = currentState->transitions.equal_range(symbol);
        if (transitionsWithSymbol.first == transitionsWithSymbol.second) {
            return false;
        }
        currentState = transitionsWithSymbol.first->second;
    }

    return currentState->final;
}
#include <cassert>

#include <DFA.h>
#include <Setup.h>
#include <UserWarn.h>

DFA::DFA(const std::string& file) {
    const Setup setup(file);
    setSigma(setup.getSigma());
    setStates(setup.getStates());
    setTransitions(setup.getTransitions());
    setStartState();
    if (this->states.empty()) {
        UserWarn("There are no states declared for this DFA");
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
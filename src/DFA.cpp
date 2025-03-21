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
        if (!currentState->transitions.contains(symbol)) {
            return false;
        }
        currentState = currentState->transitions.at(symbol);
    }

    return currentState->final;
}
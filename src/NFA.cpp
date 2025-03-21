#include <cassert>

#include <NFA.h>
#include <Setup.h>
#include <UserWarn.h>

NFA::NFA(const std::string& file) {
    const Setup setup(file);
    setSigma(setup.getSigma());
    setStates(setup.getStates());
    setTransitions(setup.getTransitions());
    setStartState();
    if (this->states.empty()) {
        UserWarn("There are no states declared for this NFA");
    }
}

bool NFA::process(const std::string& word) const{
    std::vector<std::shared_ptr<State>> currentStates = {startState};
    assert(currentStates[0] != nullptr);

    for (const auto & symbol : word) {
        std::vector<std::shared_ptr<State>> newStates;
        for (const auto & state : currentStates) {
            auto transitionsWithSymbol = state->transitions.equal_range(symbol);
            for (auto it = transitionsWithSymbol.first; it != transitionsWithSymbol.second; ++it) {
                newStates.push_back(it->second);
            }
        }
        currentStates = newStates;
    }

    for (const auto & state : currentStates) {
        if (state->final) {
            return true;
        }
    }
    return false;
}
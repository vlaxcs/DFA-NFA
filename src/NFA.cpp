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

    if (word.empty()) {
        return currentStates[0]->final;
    }

    for (const auto symbol : word) {
        std::vector<std::shared_ptr<State>> newStates;
        for (const auto& state : currentStates) {
            if (state->transitions.contains(symbol)) {
                newStates.push_back(state->transitions.at(symbol));
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
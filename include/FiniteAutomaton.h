#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

#include <State.h>

class FiniteAutomaton {
protected:
    std::unordered_set<char> sigma;
    std::vector<std::shared_ptr<State>> states;
    std::unordered_map<std::string, std::shared_ptr<State>> stateMap;
    std::shared_ptr<State> startState = nullptr;

    void setSigma(std::vector<std::string> const& sigma_);
    void setStates(const std::vector<std::string>& stateLines);
    void setTransitions(std::vector<std::string> const& transitions);
    void setStartState();

public:
    ~FiniteAutomaton() = default;
};
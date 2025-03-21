#pragma once

#include <FiniteAutomaton.h>

class DFA : public FiniteAutomaton {
public:
    explicit DFA(const std::string& file);
    bool process(const std::string& word) const;
    ~DFA() = default;
};
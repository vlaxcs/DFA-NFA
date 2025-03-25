#pragma once

#include <FiniteAutomaton.h>

class DFA : public FiniteAutomaton {
    void validate();
public:
    explicit DFA(const std::string& file);
    bool process(const std::string& word) const;
    ~DFA() = default;
};
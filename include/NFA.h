#pragma once

#include <FiniteAutomaton.h>

class NFA : public FiniteAutomaton {
public:
    explicit NFA(const std::string& file);
    bool process(const std::string& word) const;
    ~NFA() = default;
};
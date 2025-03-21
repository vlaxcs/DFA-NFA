#pragma once

#include <memory>
#include <unordered_map>

struct State {
    std::string name;
    bool initial = false;
    bool final = false;
    std::unordered_map<char, std::shared_ptr<State>> transitions;
};
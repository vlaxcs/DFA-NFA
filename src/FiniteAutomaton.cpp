#include <sstream>

#include <FiniteAutomaton.h>
#include <State.h>
#include <UserWarn.h>

void FiniteAutomaton::setSigma(std::vector<std::string> const& sigma_) {
     for (const auto& line : sigma_) {
         this->sigma.insert(line[0]);
     }
}

void FiniteAutomaton::setStates(const std::vector<std::string>& stateLines) {
     bool hasInitialState = false, hasFinalState = false;
     for (const auto& line : stateLines) {
         auto newState = std::make_shared<State>();

         std::istringstream iss(line);
         std::string token;

         while (std::getline(iss, token, ',')) {
             token.erase(0, token.find_first_not_of(' '));
             token.erase(token.find_last_not_of(' ') + 1);

             if (token == "S") {
                 if (hasInitialState) {
                     UserWarn("Initial state should be unique", line);
                 }
                 hasInitialState = true;
                 newState->initial = true;
             } else if (token == "F") {
                 newState->final = true;
                 hasFinalState = true;
             } else {
                 newState->name = token;
             }
         }

         if (newState->name.empty()) {
             UserWarn("State must have a name", line);
             continue;
         }

         this->stateMap[newState->name] = newState;
         this->states.push_back(newState);
     }

     if (!hasFinalState) {
         UserWarn("At least one final state required");
     }
 }

 void FiniteAutomaton::setTransitions(std::vector<std::string> const& transitions) {
     for (const auto& line : transitions) {
         std::istringstream iss(line);
         std::string fromState, toState, token;
         char symbol;

         int tokenCount = 0;
         while (std::getline(iss, token, ',')) {
             token.erase(0, token.find_first_not_of(' '));
             token.erase(token.find_last_not_of(' ') + 1);
             switch (tokenCount++) {
                 case 0: fromState = token; break;
                 case 1: if (token.length() > 1) UserWarn("The symbol should be an unique character", line); symbol = token[0]; break;
                 case 2: toState = token; break;
                 default: break;
             }
         }

         if (this->stateMap.contains(fromState) && this->stateMap.contains(toState)) {
             this->stateMap[fromState]->transitions[symbol] = stateMap[toState];
         } else {
             UserWarn("There are undefined states", line);
         }
     }
 }

void FiniteAutomaton::setStartState() {
    for (const auto & state : states) {
        if (state->initial) {
        startState = state;
        break;
        }
    }
}
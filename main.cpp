#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <string>
#include <crtdbg.h>
#include <filesystem>

struct State {
    std::string name;
    bool initial = false;
    bool final = false;
    std::unordered_map<char, State*> transitions;
};

class UserWarn {
public:
    explicit UserWarn(const std::string& reason) {
        std::cerr << "Configuration error: " << reason << '\n';
        exit(1);
    }

    explicit UserWarn(const std::string& reason, const std::string& line) {
        std::cout << "Issue at: " << line << '\n';
        std::cerr << "Configuration error: " << reason << '\n';
        exit(1);
    }
};

class Setup {
private:
    std::vector<std::string> sigma;
    std::vector<std::string> states;
    std::vector<std::string> transitions;

public:
    explicit Setup(const std::string& file) {
        std::ifstream f(file);
        if (!f.is_open()) {
            UserWarn("Provided file does not exist: " + file);
        }

        std::string line;
        bool readSigma(false), readStates(false), readTransitions(false);
        while (std::getline(f, line)) {
            if (line.find("Sigma") != std::string::npos) {
                readSigma = true;
                readStates = readTransitions = false;
                continue;
            } else if (line.find("States") != std::string::npos) {
                readStates = true;
                readSigma = readTransitions = false;
                continue;
            } else if (line.find("Transitions") != std::string::npos) {
                readTransitions = true;
                readSigma = readStates = false;
                continue;
            } else if (line.find("End") != std::string::npos) {
                readSigma = readStates = readTransitions = false;
                continue;
            } else if (line.starts_with("#")) {
                continue;
            }

            if (readSigma) {
                sigma.push_back(line);
            } else if (readStates) {
                states.push_back(line);
            } else if (readTransitions) {
                transitions.push_back(line);
            }
        }

        f.close();
    }

    [[nodiscard]] const std::vector<std::string>& getSigma() const {
        return sigma;
    }
    [[nodiscard]] const std::vector<std::string>& getStates() const {
        return states;
    }
    [[nodiscard]] const std::vector<std::string>& getTransitions() const {
        return transitions;
    }
};

class FiniteAutomaton {
protected:
    std::vector<char> sigma;
    std::vector<State*> states;
    std::unordered_map<std::string, State*> stateMap;

    // Checks if Sigma contains given symbol
    bool inSigma(const char symbol) const {
        for (const auto & chr : this->sigma) {
            if (chr == symbol) {
                return true;
            }
        }
        return false;
    }

    void setSigma(std::vector<std::string> const& sigma) {
        for (const auto& line : sigma) {
            this->sigma.push_back(line[0]);
        }
    }

    void setStates(std::vector<std::string> const& states) {
        bool hasInitialState = false, hasFinalState = false;
        for (const auto& line : states) {
            auto* newState = new State(); // baga unique pointer

            char str[line.size() + 1];
            std::strcpy(str, line.c_str());
            const char* token = strtok(str, ", ");
            while (token != nullptr) {
                std::string tokenStr = token;
                if (tokenStr == "S") {
                    // The DFA should have a unique initial state
                    if (hasInitialState) {
                        UserWarn("Initial state should be unique", line);
                    }
                    hasInitialState = true;
                    newState->initial = true;
                } else if (tokenStr == "F") {
                    newState->final = true;
                    hasFinalState = true;
                } else {
                    newState->name = tokenStr;
                }
                token = strtok(nullptr, ", ");
            }

            this->stateMap[newState->name] = newState;
            this->states.push_back(newState);
            newState = nullptr;
            delete newState;
        }

        // The DFA must have at least one final state
        if (!hasFinalState) {
            UserWarn("At least one final state required");
        }
    }

    void setTransitions(std::vector<std::string> const& transitions) {
        for (const auto& line : transitions) {
            char str[line.size() + 1];
            std::strcpy(str, line.c_str());
            const char* token = strtok(str, ", ");
            std::string fromState, toState;
            char symbol = '-';

            int tokenCount = 0;
            while (token != nullptr) {
                switch (tokenCount++) {
                    case 0: fromState = token; break;
                    case 1: if (strlen(token) > 1) UserWarn("The symbol should be an unique character", line); symbol = token[0]; break;
                    case 2: toState = token; break;
                    default: break;
                }
                token = strtok(nullptr, ", ");
            }

            if (!inSigma(symbol)) {
                UserWarn("Symbol does not occur in given alphabet", line);
            }

            // Checks if our stateMap contains given fromState and toState
            if (this->stateMap.contains(fromState) && this->stateMap.contains(toState)) {
                this->stateMap[fromState]->transitions[symbol] = stateMap[toState];

            } else {
                UserWarn("There are undefined states", line);
            }
        }
    }

public:
    ~FiniteAutomaton() {
        for (const auto & state : states) {
            delete state;
        }
    }
};

class DFA : public FiniteAutomaton {
public:
    explicit DFA(const std::string& file) {
        const Setup setup(file);
        setSigma(setup.getSigma());
        setStates(setup.getStates());
        setTransitions(setup.getTransitions());
        if (this->states.empty()) {
            UserWarn("There are no states declared for this DFA");
        }
    }

    bool process(const std::string& word) const{
        const State* currentState = nullptr;
        for (const auto & state : states) {
            if (state->initial) {
                currentState = state;
                break;
            }
        }

        if (word.empty()) {
            return currentState != nullptr && currentState->final;
        }

        for (auto symbol : word) {
            if (currentState == nullptr || !currentState->transitions.contains(symbol)) {
                return false;
            }
            currentState = currentState->transitions.at(symbol);
        }

        return currentState != nullptr && currentState->final;
    }
};

class NFA : public FiniteAutomaton {
public:
    explicit NFA(const std::string& file) {
        const Setup setup(file);
        setSigma(setup.getSigma());
        setStates(setup.getStates());
        setTransitions(setup.getTransitions());
        if (this->states.empty()) {
            UserWarn("There are no states declared for this NFA");
        }
    }

    bool process(const std::string& word) const{
        const State* currentState = nullptr;
        for (const auto & state : states) {
            if (state->initial) {
                currentState = state;
                break;
            }
        }

        if (word.empty()) {
            return currentState != nullptr && currentState->final;
        }

        for (char symbol : word) {
            if (currentState == nullptr || !currentState->transitions.contains(symbol)) {
                return false;
            }
            currentState = currentState->transitions.at(symbol);
        }
        return currentState != nullptr && currentState->final;
    }
};

template <typename FA>
class Test {
private:
    std::vector<std::string> words;
    std::string configPath;

    void setWords(const std::string& filename) {
        std::ifstream f(filename);
        std::string line;
        if (!f.is_open()) {
            UserWarn("Provided file does not exist: " + filename);
        }
        while (std::getline(f, line)) {
            this->words.push_back(line);
        }
        f.close();
    }

    void setConfigPath() {
       this->configPath= typeid(FA).name() + 1;
        this->configPath += "/";
    }

    static void getResult(const FA& custom, const std::string& word) {
        std::cout << "Word: " << word << ": ";
        if (custom.process(word)) {
            std::cout << "Accepted!" << std::endl;
        }
        else {
            std::cout << "Rejected!" << std::endl;
        }
    }

public:
    explicit Test(const std::string& filename) {
        setWords(filename);
        setConfigPath();
    }

    void run(){
        for (const auto& entry : std::filesystem::directory_iterator(this->configPath)) {
            if (entry.is_regular_file()) {
                std::string currentConfig = this->configPath + entry.path().filename().string();
                std::cout << "\nConfiguration: " << currentConfig << '\n';
                FA custom(currentConfig);
                for (const auto& word : words) {
                    getResult(custom, word);
                }
            }
        }
    }

    ~Test(){std::cout<<"Test finished";};
};

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Test<DFA> t1("words.in");
    t1.run();
    // Test<NFA> t2("words.in");
    // t2.run();
    return 0;
}

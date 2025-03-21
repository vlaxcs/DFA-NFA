#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <cassert>
#include <unordered_set>

struct State {
    std::string name;
    bool initial = false;
    bool final = false;
    std::unordered_map<char, std::shared_ptr<State>> transitions;
};

class UserWarn {
private:
    static void configurationError(const std::string& reason, const std::string& line = "") {
        if (!line.empty()) {
            std::cout << "Issue at: " << line << std::endl;
        }
        std::cerr << "Configuration error: " << reason << std::endl;
        exit(1);
    }

public:
    explicit UserWarn(const std::string& reason) {
       configurationError(reason);
    }

    explicit UserWarn(const std::string& reason, const std::string& line) {
        configurationError(reason, line);
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
    std::unordered_set<char> sigma;
    std::vector<std::shared_ptr<State>> states;
    std::unordered_map<std::string, std::shared_ptr<State>> stateMap;
    std::shared_ptr<State> startState = nullptr;

    /*  Checks if Sigma contains given symbol
        bool inSigma(const char symbol) const {
        return sigma.contains(symbol);
    }*/

    void setSigma(std::vector<std::string> const& sigma) {
        for (const auto& line : sigma) {
            this->sigma.insert(line[0]);
        }
    }

    void setStates(const std::vector<std::string>& stateLines) {
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

        // Any Finite Automaton must have at least one final state
        if (!hasFinalState) {
            UserWarn("At least one final state required");
        }
    }

    void setTransitions(std::vector<std::string> const& transitions) {
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

            /*In our case, invalid symbols will be rejected in processing
            if (!inSigma(symbol)) {
                UserWarn("Symbol does not occur in given alphabet", line);
            }*/

            // Checks if our stateMap contains given fromState and toState
            if (this->stateMap.contains(fromState) && this->stateMap.contains(toState)) {
                this->stateMap[fromState]->transitions[symbol] = stateMap[toState];
            } else {
                UserWarn("There are undefined states", line);
            }
        }
    }

    void setStartState() {
        for (const auto & state : states) {
            if (state->initial) {
                startState = state;
                break;
            }
        }
    }

public:
    ~FiniteAutomaton() = default;
};

class DFA : public FiniteAutomaton {
public:
    explicit DFA(const std::string& file) {
        const Setup setup(file);
        setSigma(setup.getSigma());
        setStates(setup.getStates());
        setTransitions(setup.getTransitions());
        setStartState();
        if (this->states.empty()) {
            UserWarn("There are no states declared for this DFA");
        }
    }

    bool process(const std::string& word) const{
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

    ~DFA() = default;
};

class NFA : public FiniteAutomaton {
public:
    explicit NFA(const std::string& file) {
        const Setup setup(file);
        setSigma(setup.getSigma());
        setStates(setup.getStates());
        setTransitions(setup.getTransitions());
        setStartState();
        if (this->states.empty()) {
            UserWarn("There are no states declared for this NFA");
        }
    }

    bool process(const std::string& word) const{
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

    ~NFA() = default;
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

    std::string getConfigPath() {
        return this->configPath;
    }

    static bool getResult(const FA& custom, const std::string& word) {
        return custom.process(word);
    }

public:
    explicit Test(const std::string& filename) {
        setWords(filename);
        setConfigPath();
    }

    void run(){
        std::string currentPath = getConfigPath();
        for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
            if (entry.is_regular_file()) {
                std::string currentConfig = currentPath + entry.path().filename().string();
                std::cout << std::endl << "Configuration: " << currentConfig << std::endl;
                FA custom(currentConfig);
                for (const auto& word : words) {
                    std::cout << "Word: " << word << ": ";
                    getResult(custom, word) ? std::cout << "Accepted!" << std::endl : std::cout << "Rejected!" << std::endl;
                }
            }
        }
    }

    ~Test() = default;
};

int main() {
    Test<DFA> t1("words.in");
    t1.run();
    Test<NFA> t2("words.in");
    t2.run();
    return 0;
}
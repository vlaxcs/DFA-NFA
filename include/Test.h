#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#include <UserWarn.h>
#include <DFA.h>
#include <NFA.h>

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
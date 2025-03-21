cmake_minimum_required(VERSION 3.30)
project(DFA_NFA)

set(CMAKE_CXX_STANDARD 20)

add_executable(DFA_NFA src/main.cpp src/UserWarn.cpp src/Setup.cpp src/FiniteAutomaton.cpp src/DFA.cpp src/NFA.cpp)
target_include_directories(DFA_NFA PRIVATE include)
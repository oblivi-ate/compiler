# compiler

compile:
g++ token.cpp dfa.cpp scanner.cpp -o scanner -std=c++17

run:
./scanner test_scanner.pc
#ifndef SCANNER_H_
#define SCANNER_H_

#include "scanner.h"
#include "dfa.h"
#include <iostream>
#include <fstream>
#include <sstream>

class Scanner {
public:
    Scanner(std::istream& in) : in(in) {}

    void scan(Token_List* token_list);

private:
    std::istream& in; // 输入流
    char currentChar;  // 当前字符
    int position = 0; // 当前位置
};


#endif
#include "scanner.h"
#include "dfa.h"
#include "token.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Scanner::scan(Token_List *token_list)
{
    initdfa();              // 初始化dfa状态
    currentChar = in.get(); // 读取第一个字符
    while (true)
    {
        Transition(currentChar, token_list, in.eof());
        if (currentstate == ERROR)
        {
            std::cerr << "Error occurred during scanning." << std::endl;
            std::cerr << std::endl;
            return;
        }
        if (!in.eof())
        {
            currentChar = in.get();
        }
        else
        {
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) // check if the file is open
    {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }
    Token *header = new Token(ST, "Start of the scanner");
    Token_List *token_list = new Token_List(*header, 0); // 用于存储token的链表

    Scanner scanner(inputFile);
    scanner.scan(token_list);

    printToken(token_list);

    inputFile.close();

    return 0;
}
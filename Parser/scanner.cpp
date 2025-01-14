#include "scanner.h"
#include "dfa.h"
#include "token.h"
#include <iostream>
#include <fstream>
#include <sstream>


void Scanner::scan(Token_List *token_list)
{
    initdfa();              
    currentChar = in.get(); 
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

int doScan(int argc, char *argv[], Token_List *&tk){
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
    Token_List *token_list = new Token_List(*header, 0); 

    Scanner scanner(inputFile);
    scanner.scan(token_list);
    
    // delete white space tokens or ENTER tokens
    Token_List *temp = token_list;
    while (temp != nullptr)
    {
        if (temp->token.type == TokenType::WS || temp->token.type == TokenType::ENTER)
        {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;       
        }
        temp = temp->next;
    }
    
    // delete header
    token_list = token_list->next;
    token_list->prev = nullptr;
    

    // delete EOF token
    temp = token_list;
    while (temp->next != nullptr)
    {
        temp = temp->next;
    }
    temp->prev->next = nullptr;
    delete temp;
    
    tk = token_list;


    inputFile.close();

    return 0;
}
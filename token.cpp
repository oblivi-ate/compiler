#include "token.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

std::unordered_map<TokenType, std::string> token_table = {
    {TokenType::IF, "IF"},
    {TokenType::ELSE, "ELSE"},
    {TokenType::WHILE, "WHILE"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::INT, "INT"},
    {TokenType::VOID, "VOID"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::TIMES, "TIMES"},
    {TokenType::DIVIDE, "DIVIDE"},
    {TokenType::MOD, "MOD"},
    {TokenType::LT, "LT"},
    {TokenType::LTE, "LTE"},
    {TokenType::GT, "GT"},
    {TokenType::GTE, "GTE"},
    {TokenType::EQ, "EQ"},
    {TokenType::NEQ, "NEQ"},
    {TokenType::ASSIGN, "ASSIGN"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::LPAREN, "LPAREN"},
    {TokenType::RPAREN, "RPAREN"},
    {TokenType::LBRACKET, "LBRACKET"},
    {TokenType::RBRACKET, "RBRACKET"},
    {TokenType::LBRACE, "LBRACE"},
    {TokenType::RBRACE, "RBRACE"},
    {TokenType::COMMENT, "COMMENT"},
    {TokenType::DIGIT, "DIGIT"},
    {TokenType::LETTER, "LETTER"},
    {TokenType::NUMBER, "NUMBER"},
    {TokenType::WS, "WS"},
    {TokenType::ID, "ID"},
    {TokenType::INDENT, "INDENT"},
    {TokenType::ERROR, "ERROR"},
    {TokenType::ENTER, "ENTER"},
    {TokenType::ENDFILE, "ENDFILE"},
    {TokenType::START, "START"},
    // ...
};

void addToken(Token_List *token_list, Token token, int line)
{
    Token_List *new_token = new Token_List(token, line);
    token_list->next = new_token;
    new_token->prev = token_list;
    token_list = new_token;
}

void printToken(Token_List *token_list)
{
    Token_List *current = token_list;
    while (current != nullptr)
    {  
        std::cout << "Line " << current->line << " --- " << token_table[current->token.type] << " : " << current->token.info << std::endl;  
        current = current->next;
    }
}

void testToken()
{
    Token_List *token_list = new Token_List(Token(ERROR, "error"), -1);
    addToken(token_list, Token(IF, "if"), 1);
    printToken(token_list);
    printf("Test passed.\n");
}

// int main()
// {
//     testToken();
//     return 0;
// }

#include "token.h"
#include <iostream>
#include <string>

std::string TokenName[34] = {
    // keywords
    "IF", "ELSE", "WHILE", "RETURN", "INT", "VOID", 
    // special symbols
    "PLUS", "MINUS", "TIMES", "DIVIDE", // + - * /
    "LT", "LTE", "GT", "GTE", "EQ", "NEQ", "ASSIGN", // < <= > >= == != =
    "SEMICOLON", "COMMA", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "LBRACE", "RBRACE", // ; , ( ) [ ] { }
    // identifiers
    "ID", 
    // numbers and strings
    "NUMBER", "LETTER",
    // white space
    "WS",
    // indentation
    "INDENT", "DEDENT",
    // error
    "ERROR",
    // new line
    "ENTER",
    // end of file
    "ENDFILE",
    // ...
};

Token::Token(TokenType type, std::string info)
{
    this->type = type;
    this->info = info;
}


#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>

enum TokenType 
{
    // keywords
    IF, ELSE, WHILE, RETURN, INT, VOID, 
    // special symbols
    PLUS, MINUS, TIMES, DIVIDE, MOD // + - * / % 
    LT, LTE, GT, GTE, EQ, NEQ, ASSIGN, // < <= > >= == != =
    SEMICOLON, COMMA, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE, // ; , ( ) [ ] { }
    // COMMENT
    COMMENT, 
    // numbers and strings
    NUMBER, LETTER,
    // white space
    WS,
    // indentation
    INDENT, DEDENT,
    // error
    ERROR,
    // new line
    ENTER,
    // end of file
    ENDFILE,
    // ...

};

struct Token 
{
    TokenType type;
    std::string info;
    
    Token(TokenType type, std::string info){}
};

typedef struct Token_List{
    Token token;
    struct Token_List *prev;
    struct Token_List *next;
    // constructor of null token
    Token_List(Token token) : token(token), prev(nullptr), next(nullptr) {}
};


#endif

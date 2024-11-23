#ifndef TOKEN_H_
#define TOKEN_H_
#include <string>

enum TokenType 
{
    // keywords
    IF, ELSE, WHILE, RETURN, INT, VOID, 
    // special symbols
    PLUS, MINUS, TIMES, DIVIDE, MOD, // + - * / % 
    LT, LTE, GT, GTE, EQ, NEQ, ASSIGN, // < <= > >= == != =
    SEMICOLON, COMMA, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE, COLON, // ; , ( ) [ ] { } :
    // COMMENT
    COMMENT, 
    // numbers and strings
    NUMBER,
    // white space
    WS,
    // identifier
    ID,
    // indentation
    INDENT,
    // error
    ERR,
    // new line
    ENTER,
    // end of file
    ENDFILE,
    // start of the scanner
    ST,
    // ...

};

struct Token 
{
    TokenType type;
    std::string info;
    Token(TokenType type, std::string info) : type(type), info(info) {}
    bool operator==(const Token &other) const {
        // Define equality based on your fields
        return type == other.type;
    }
};

namespace std {
    template <>
    struct hash<Token> {
        size_t operator()(const Token &token) const {
            return std::hash<int>()(token.type);
        }
    };
}

struct Token_List{
    int line;
    Token token;
    struct Token_List *prev;
    struct Token_List *next;
    // constructor of token list
    Token_List(Token token, int line) : line(line), token(token), prev(nullptr), next(nullptr) {}
};

void addToken(Token_List *token_list, Token token, int line);
void printToken(Token_List *token_list);

#endif

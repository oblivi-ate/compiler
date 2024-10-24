#include "dfa.h"
#include "token.h"
#include <unordered_map>
#include <utility>
#include <string>

STATE currentstate = START;
int currentpos = 0;
std::string info = "";

std::unordered_map<Token, std::string> keywords = {
    {Token(IF, "if"), "if"},
    {Token(ELSE, "else"), "else"},
    {Token(WHILE, "while"), "while"},
    {Token(RETURN, "return"), "return"},
    {Token(INT, "int"), "int"},
    {Token(VOID, "void"), "void"},
    {Token(PLUS, "+"), "+"},
    {Token(MINUS, "-"), "-"},
    {Token(TIMES, "*"), "*"},
    {Token(DIVIDE, "/"), "/"},
    {Token(LT, "<"), "<"},
    {Token(LTE, "<="), "<="},
    {Token(GT, ">"), ">"},
    {Token(GTE, ">="), ">="},
    {Token(EQ, "=="), "=="},
    {Token(NEQ, "!="), "!="},
    {Token(ASSIGN, "="), "="},
    {Token(SEMICOLON, ";"), ";"},
    {Token(COMMA, ","), ","},
    {Token(LPAREN, "("), "("},
    {Token(RPAREN, ")"), ")"},
    {Token(LBRACKET, "["), "["},
    {Token(RBRACKET, "]"), "]"},
    {Token(LBRACE, "{"), "{"},
    {Token(RBRACE, "}"), "}"},
    {Token(ID, "id"), "id"},
    {Token(NUMBER, "number"), "number"},
    {Token(LETTER, "letter"), "letter"},
    {Token(WS, "ws"), "ws"},
    {Token(INDENT, "indent"), "indent"},
    {Token(DEDENT, "dedent"), "dedent"},
    {Token(ENTER, "enter"), "enter"},
    {Token(ENDFILE, "endfile"), "endfile"},

    // ...  
};


void addToken(Token_List *token_list, Token token)
{
    Token_List *new_token = new Token_List(token);
    token_list->next = new_token;
    new_token->prev = token_list;
    token_list = new_token;
}


void TransitionStart(char c, Token_List *tk, bool is_EOF){
    info = "";
    if(is_EOF){
        currentstate = DONE;
        currentpos++;
        addToken(tk, Token(ENDFILE, ""));
        return;
    }
    switch (c)
    {
    case '+' | '-' | '*' | ';' | ',' | '(' | ')' | '[' | ']' | '{' | '}':
        currentstate = DONE;
        currentpos++;
        addToken(tk, Token(ENDFILE, ""));
        break;
    case '=':
        currentstate = READ_EQ;
        currentpos++;
        break;
    case '>':
        currentstate = READ_GT;
        currentpos++;
        break;
    case '!':
        currentstate = READ_NOT;
        currentpos++;
        break;
    case '<':
        currentstate = READ_LT;
        currentpos++;
        break;
    case '/':
        //
    case '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9':
        currentstate = IN_NUM;
        currentpos++;
        info += c;
        break;
    default:
        
    }

}

void initdfa()
{
    // initialize the dfa
    currentstate = START;
    currentpos = 0;
}

#include "dfa.h"
#include "token.h"
#include <unordered_map>
#include <utility>
#include <string>

STATE currentstate = START;
int currentpos = 0;
int line = 1;
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
    {Token(MOD, "MOD"), "mod"},
    {Token(ENTER, "enter"), "enter"},
    {Token(ENDFILE, "endfile"), "endfile"},

    // ...
};

void Transition(char c, Token_List *tk, bool is_EOF)
{
    if (is_EOF)
    {
        currentstate = DONE;
        currentpos++;
        addToken(tk, Token(ENDFILE, ""), line);
        return;
    }
    if (currentstate == START || currentstate == NOT_DONE || currentstate == DONE)
    {
        switch (c)
        {
        case '+':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(PLUS, "PLUS"), line);
            break;
        case '-':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(MINUS, "MINUS"), line);
            break;
        case '*':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(TIMES, "TIMES"), line);
            break;
        case '%':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(MOD, "MOD"), line);
            break;
        case ';':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(SEMICOLON, "SEMICOLON"), line);
            break;
        case ',':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(COMMA, "COMMA"), line);
            break;
        case '(':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(LPAREN, "LPAREN"), line);
            break;
        case ')':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(RPAREN, "RPAREN"), line);
            break;
        case '[':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(LBRACKET, "LBRACKET"), line);
            break;
        case ']':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(RBRACKET, "RBRACKET"), line);
            break;
        case '{':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(LBRACE, "LBRACE"), line);
            break;
        case '}':
            currentstate = DONE;
            currentpos++;
            addToken(tk, Token(RBRACE, "RBRACE"), line);
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
            currentstate = READ_OVER;
            currentpos++;
        case '#':
            currentstate = COMMENTING;
            currentpos++;
            //
        case '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9':
            currentstate = IN_NUM;
            currentpos++;
            info += c;
            break;
        case 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' | 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z':
            currentstate = IN_LET;
            currentpos++;
            info += c;
            break;
        default: // 当输入其他字符时直接报错return，在main函数循环中请加入一个判断token类型是否为error的语句，如果是则退出循环
            currentstate = ERROR;
            currentpos++;
            addToken(tk, Token((TokenType)ERROR, "ERROR! Illegal character!"), line);
            return;
        }
    }
    if (currentstate == READ_EQ && is_EOF)
    { // 当已经输入一个‘=’时
        switch (c)
        {
        case '=': // 第二个 ‘=’ 传入， 状态为DONE，在tokenlist中加入以EQ为Tokentype的新节点
            currentstate = DONE;
            addToken(tk, Token(EQ, ""), line);
            currentpos++;
            break;
        default: // 没有接收到第二个‘=’状态为NOT_DONE
            currentstate = NOT_DONE;
            addToken(tk, Token(ASSIGN, "ASSIGN"), line); // 将之前录入的单个引号打入链表
            break;
        }
        if (currentstate == READ_GT && is_EOF)
        {
            switch (c)
            {
            case '=':
                currentstate = DONE;
                addToken(tk, Token(GTE, "GREATER THAN OR EQUAL TO"), line);
                currentpos++;
                break;
            default:
                currentstate = NOT_DONE;
                addToken(tk, Token(GT, "GREATER"), line);
                break;
            }
        }
        if (currentstate == READ_LT && is_EOF)
        {
            switch (c)
            {
            case '=':
                currentstate = DONE;
                addToken(tk, Token(LTE, "LESS THAN OR EQUAL TO"), line);
                currentpos++;
                break;
            default:
                currentstate = NOT_DONE;
                addToken(tk, Token(LT, "LESS THAN"), line);
                break;
            }
        }
        if (currentstate == READ_NOT && is_EOF)
        {
            switch (c)
            {
            case '=':
                currentstate = DONE;
                addToken(tk, Token(NEQ, "NOT EQULA TO"), line);
                currentpos++;
                break;
            default: // 若读取感叹号！之后没有等号则直接报错，函数直接返回 。
                currentstate = ERROR;
                addToken(tk, Token((TokenType)ERROR, "ERROR! There are only single ! appeared."), line);
                return;
            }
        }
        if (currentstate == IN_NUM && is_EOF)
        { // 前一个是数字的情况
            switch (c)
            {
            case '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9':
                currentstate = IN_NUM;
                currentpos++;
                info += c;
                break;
            default:
                currentstate = NOT_DONE;
                addToken(tk, Token(NUMBER, info), line);
                info = "";
                Transition(c, tk, is_EOF);
                break;
            }
        }
        if (currentstate == IN_LET && is_EOF)
        { // 前一个是字母的情况
            switch (c)
            {
            case 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' | 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z':
                currentstate = IN_LET;
                currentpos++;
                info += c;
                break;
            default:
                currentstate = NOT_DONE;
                if (info == "if") {
                    addToken(tk, Token(IF, "IF"), line);
                } else if (info == "else") {
                    addToken(tk, Token(ELSE, "ELSE"), line);
                } else if (info == "while") {
                    addToken(tk, Token(WHILE, "WHILE"), line);
                } else if (info == "return") {
                    addToken(tk, Token(RETURN, "RETURN"), line);
                } else if (info == "int") {
                    addToken(tk, Token(INT, "INT"), line);
                } else if (info == "void") {
                    addToken(tk, Token(VOID, "VOID"), line);
                } else {
                    addToken(tk, Token(LETTER, info), line);
                }
                info = "";
                break;
            }
        }
        if (currentstate == READ_OVER && is_EOF)
        { // 前一个是斜杠的情况
            switch (c)
            {
            case '/':
                currentstate = COMMENTING; // COMMENTING是#评论和//评论的情况
                currentpos++;
                break;
            case '*':
                currentstate = INCOMMENT; // INCOMMENT是/*评论的情况
                currentpos++;
                break;
            default:
                currentstate = DONE;
                addToken(tk, Token(DIVIDE, "DIVIDE"), line);
                break;
            }
        }
        if (currentstate == COMMENTING && is_EOF)
        {
            switch (c)
            {
            case '\n':
                currentstate = DONE;
                line++;
                addToken(tk, Token(COMMENT, info), line);
                info = "";
                currentpos++;
                break;
            default:
                info += c;
                currentpos++;
                break;
            }
        }
        if (currentstate == INCOMMENT && is_EOF)
        {
            switch (c)
            {
            case '*':
                currentstate = WAIT_OVER;
                currentpos++;
                break;
            default:
                info += c;
                currentpos++;
                break;
            }
        }
        if (currentstate == WAIT_OVER && is_EOF)
        {
            switch (c)
            {
            case '*':
                currentpos++;
                info += "*";
                break;
            case '/':
                currentstate = DONE;
                currentpos++;
                addToken(tk, Token(COMMENT, info), line);
                info = "";
            default:
                currentstate = INCOMMENT;
                info += "*";
                info += c;
                currentpos++;
                break;
            }
        }
        return;
    }
}

void initdfa()
{
    // initialize the dfa
    currentstate = (STATE)START;
    currentpos = 0;
}
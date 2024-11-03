#include "dfa.h"
#include "token.h"
#include <unordered_map>
#include <utility>
#include <string>

STATE currentstate;
int line;
std::string info;

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
    printf("currentstate: %d\n", currentstate);
    if (is_EOF) // read the end of file
    {
        if(currentstate == INCOMMENT || currentstate == WAIT_OVER || currentstate == COMMENTING)
        {
            currentstate = (STATE)ERROR;
            addToken(tk, Token((TokenType)ERROR, "ERROR! Comment is not closed!"), line);
            return;
        }
        currentstate = DONE;
        addToken(tk, Token(ENDFILE, ""), line);
        return;
    }
    if (currentstate == DONE) // DONE->START
    {
        currentstate = START;
        Transition(c, tk, is_EOF);
        return;
    }
    if (currentstate == (STATE)START) // initial state
    {
        switch (c)
        {
        case '+':
            currentstate = DONE;
            addToken(tk, Token(PLUS, "PLUS"), line);
            return;
        case '-':
            currentstate = DONE;
            addToken(tk, Token(MINUS, "MINUS"), line);
            return;
        case '*':
            currentstate = DONE;
            addToken(tk, Token(TIMES, "TIMES"), line);
            return;
        case '%':
            currentstate = DONE;
            addToken(tk, Token(MOD, "MOD"), line);
            return;
        case ';':
            currentstate = DONE;
            addToken(tk, Token(SEMICOLON, "SEMICOLON"), line);
            return;
        case ',':
            currentstate = DONE;
            addToken(tk, Token(COMMA, "COMMA"), line);
            return;
        case '(':
            currentstate = DONE;
            addToken(tk, Token(LPAREN, "LPAREN"), line);
            return;
        case ')':
            currentstate = DONE;
            addToken(tk, Token(RPAREN, "RPAREN"), line);
            return;
        case '[':
            currentstate = DONE;
            addToken(tk, Token(LBRACKET, "LBRACKET"), line);
            return;
        case ']':
            currentstate = DONE;
            addToken(tk, Token(RBRACKET, "RBRACKET"), line);
            return;
        case '{':
            currentstate = DONE;
            addToken(tk, Token(LBRACE, "LBRACE"), line);
            return;
        case '}':
            currentstate = DONE;
            addToken(tk, Token(RBRACE, "RBRACE"), line);
            return;
        case '\n':
            currentstate = DONE;
            addToken(tk, Token(ENTER, "ENTER"), line);
            line++;
            return;
        case ' ':
            currentstate = DONE;
            addToken(tk, Token(WS, "WS"), line);
            return;
        case '=':
            currentstate = READ_EQ;
            return;
        case '>':
            currentstate = READ_GT;
            return;
        case '!':
            currentstate = READ_NOT;
            return;
        case '<':
            currentstate = READ_LT;
            return;
        case '/':
            currentstate = READ_OVER;
            info += c; 
            return;
        case '#':
            currentstate = COMMENTING;
            info += c;
            return;
            //
        case '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9':
            currentstate = IN_NUM;
            info += c;
            break;
        case 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' | 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z':
            currentstate = IN_LET;
            info += c;
            return;
        default: // 当输入其他字符时直接报错return，在main函数循环中请加入一个判断token类型是否为error的语句，如果是则退出循环
            currentstate = (STATE)ERROR;

            addToken(tk, Token((TokenType)ERROR, "ERROR! Illegal character!"), line);
            return;
        }
    }
    if (currentstate == READ_EQ && is_EOF) // 当已经输入一个‘=’时
    {
        switch (c)
        {
        case '=': // 第二个 ‘=’ 传入， 状态为DONE，在tokenlist中加入以EQ为Tokentype的新节点
            currentstate = DONE;
            addToken(tk, Token(EQ, ""), line);
            return;
        default: // 没有接收到第二个‘=’状态为NOT_DONE
            currentstate = NOT_DONE;
            addToken(tk, Token(ASSIGN, "ASSIGN"), line); // 将之前录入的单个引号打入链表
            TransitionNotDone(c, tk, is_EOF);
            return;
        }
    }
    if (currentstate == READ_GT && is_EOF)
    {
        switch (c)
        {
        case '=':
            currentstate = DONE;
            addToken(tk, Token(GTE, "GREATER THAN OR EQUAL TO"), line);
            return;
        default:
            currentstate = NOT_DONE;
            addToken(tk, Token(GT, "GREATER"), line);
            TransitionNotDone(c, tk, is_EOF);
            return;
        }
    }
    else if (currentstate == READ_LT && is_EOF)
    {
        switch (c)
        {
        case '=':
            currentstate = DONE;
            addToken(tk, Token(LTE, "LESS THAN OR EQUAL TO"), line);
            return;
        default:
            currentstate = NOT_DONE;
            addToken(tk, Token(LT, "LESS THAN"), line);
            TransitionNotDone(c, tk, is_EOF);
            return;
        }
    }
    else if (currentstate == READ_NOT)
    {
        switch (c)
        {
        case '=':
            currentstate = DONE;
            addToken(tk, Token(NEQ, "NOT EQULA TO"), line);
            return;
        default: // 若读取感叹号！之后没有等号则直接报错，函数直接返回 。
            currentstate = (STATE)ERROR;
            addToken(tk, Token((TokenType)ERROR, "ERROR! There are only single ! appeared."), line);
            return;
        }
    }
    else if (currentstate == IN_NUM) // 前一个是数字的情况
    {
        switch (c)
        {
        case '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9':
            currentstate = IN_NUM;
            info += c;
            return;
        default:
            currentstate = NOT_DONE;
            addToken(tk, Token(NUMBER, info), line);
            info = "";
            TransitionNotDone(c, tk, is_EOF);
            return;
        }
    }
    else if (currentstate == IN_LET) // 前一个是字母的情况
    { 
        switch (c)
        {
        case 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' | 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z':
            currentstate = IN_LET;
            info += c;
            return;
        default:
            currentstate = NOT_DONE;
            if (info == "if")
            {
                addToken(tk, Token(IF, "IF"), line);
            }
            else if (info == "else")
            {
                addToken(tk, Token(ELSE, "ELSE"), line);
            }
            else if (info == "while")
            {
                addToken(tk, Token(WHILE, "WHILE"), line);
            }
            else if (info == "return")
            {
                addToken(tk, Token(RETURN, "RETURN"), line);
            }
            else if (info == "int")
            {
                addToken(tk, Token(INT, "INT"), line);
            }
            else if (info == "void")
            {
                addToken(tk, Token(VOID, "VOID"), line);
            }
            else
            {
                addToken(tk, Token(LETTER, info), line);
            }
            info = "";
            return;
        }
    }
    else if (currentstate == READ_OVER) // 前一个是斜杠的情况
    { 
        switch (c)
        {
        case '/':
            currentstate = COMMENTING; // COMMENTING是#评论和//评论的情况
            info += c;
            return;
        case '*':
            currentstate = INCOMMENT; // INCOMMENT是/*评论的情况
            info += c;
            return;
        default:
            currentstate = NOT_DONE;
            addToken(tk, Token(DIVIDE, "DIVIDE"), line);
            info = "";
            TransitionNotDone(c, tk, is_EOF);
            return;
        }
    }
    else if (currentstate == COMMENTING)
    {
        switch (c)
        {
        case '\n':
            currentstate = DONE;
            addToken(tk, Token(COMMENT, info), line);
            line++;
            info = "";
            return;
        default:
            info += c;
            return;
        }
    }
    if (currentstate == INCOMMENT)
    {
        switch (c)
        {
        case '*':
            info += c;
            currentstate = WAIT_OVER;
            return;
        default:
            info += c;
            return;
        }
    }
    if (currentstate == WAIT_OVER)
    {
        switch (c)
        {
        case '*':
            info += c;
            return;
        case '/':
            currentstate = DONE;
            info += c;
            addToken(tk, Token(COMMENT, info), line);
            info = "";
        default:
            currentstate = INCOMMENT;
            info += c;
            break;
        }
    }
    return;
}

void TransitionNotDone(char c, Token_List *tk, bool is_EOF)
{
    currentstate = (STATE)START;
    Transition(c, tk, is_EOF);
}
void initdfa() // initialize the dfa
{
    currentstate = (STATE)START;
    line = 1;
    info = "";
}
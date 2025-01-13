#include "dfa.h"
#include "token.h"
#include <unordered_map>
#include <utility>
#include <string>
#include <iostream>

STATE currentstate;
int line;
std::string info;

void Transition(char c, Token_List *tk, bool is_EOF)
{
    
    if (is_EOF) // read the end of file
    {
        if(currentstate == INCOMMENT || currentstate == WAIT_OVER || currentstate == COMMENTING)
        {
            currentstate = ERROR;
            addToken(tk, Token(ERR, "ERROR! Comment is not closed!"), line);
            return;
        }
        currentstate = DONE;
        addToken(tk, Token(ENDFILE, "The end of file"), line);
        return;
    }
    if (currentstate == DONE) // DONE->START
    {
        currentstate = START;
        Transition(c, tk, is_EOF);
        return;
    }
    else if (currentstate == START) // initial state
    {
        switch (c)
        {
        case '+':
            currentstate = DONE;
            addToken(tk, Token(PLUS, "+"), line);
            return;
        case '-':
            currentstate = DONE;
            addToken(tk, Token(MINUS, "-"), line);
            return;
        case '*':
            currentstate = DONE;
            addToken(tk, Token(TIMES, "*"), line);
            return;
        case '%':
            currentstate = DONE;
            addToken(tk, Token(MOD, "%"), line);
            return;
        case ';':
            currentstate = DONE;
            addToken(tk, Token(SEMICOLON, ";"), line);
            return;
        case ',':
            currentstate = DONE;
            addToken(tk, Token(COMMA, ","), line);
            return;
        case '(':
            currentstate = DONE;
            addToken(tk, Token(LPAREN, "("), line);
            return;
        case ')':
            currentstate = DONE;
            addToken(tk, Token(RPAREN, ")"), line);
            return;
        case '[':
            currentstate = DONE;
            addToken(tk, Token(LBRACKET, "["), line);
            return;
        case ']':
            currentstate = DONE;
            addToken(tk, Token(RBRACKET, "]"), line);
            return;
        case '{':
            currentstate = DONE;
            addToken(tk, Token(LBRACE, "{"), line);
            return;
        case '}':
            currentstate = DONE;
            addToken(tk, Token(RBRACE, "}"), line);
            return;
        case '\n':
            currentstate = DONE;
            addToken(tk, Token(ENTER, "line break"), line);
            line++;
            return;
        case ' ':
            currentstate = DONE;
            addToken(tk, Token(WS, "white space"), line);
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
        case ':':
            currentstate = DONE;
            addToken(tk, Token(COLON, ":"), line);
            return;
        case '0' ... '9':
            currentstate = IN_NUM;
            info += c;
            return;
        case 'A' ... 'Z':
            currentstate = IN_ID;
            info += c;
            return;
        case 'a' ... 'z':
            currentstate = IN_ID;
            info += c;
            return;
        default: 
            currentstate = ERROR;
            addToken(tk, Token(ERR, "ERROR! Illegal character!"), line);
            return;
        }
    }
    else if (currentstate == READ_EQ) 
    {
        switch (c)
        {
        case '=': 
            currentstate = DONE;
            addToken(tk, Token(EQ, "=="), line);
            return;
        default: 
            currentstate = NOT_DONE;
            addToken(tk, Token(ASSIGN, "="), line); 
            TransitionNotDone(c, tk, is_EOF);
            return;
        }
    }
    else if (currentstate == READ_GT)
    {
        switch (c)
        {
        case '=':
            currentstate = DONE;
            addToken(tk, Token(GTE, ">="), line);
            return;
        default:
            currentstate = NOT_DONE;
            addToken(tk, Token(GT, ">"), line);
            TransitionNotDone(c, tk, is_EOF);
            return;
        }
    }
    else if (currentstate == READ_LT)
    {
        switch (c)
        {
        case '=':
            currentstate = DONE;
            addToken(tk, Token(LTE, "<="), line);
            return;
        default:
            currentstate = NOT_DONE;
            addToken(tk, Token(LT, "<"), line);
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
            addToken(tk, Token(NEQ, "!="), line);
            return;
        default: 
            currentstate = ERROR;
            addToken(tk, Token(ERR, "ERROR! There are only single ! appeared."), line);
            return;
        }
    }
    else if (currentstate == IN_NUM) 
    {
        switch (c)
        {
        case '0' ... '9':
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
    else if (currentstate == IN_ID) 
    { 
        switch (c)
        {
        case '0' ... '9':
            currentstate = IN_ID;
            info += c;
            return;
        case 'A' ... 'Z':
            currentstate = IN_ID;
            info += c;
            return;
        case 'a' ... 'z':
            currentstate = IN_ID;
            info += c;
            return;
        default:
            currentstate = NOT_DONE;
            if (info == "if")
            {
                addToken(tk, Token(IF, "if"), line);
            }
            else if (info == "else")
            {
                addToken(tk, Token(ELSE, "else"), line);
            }
            else if (info == "while")
            {
                addToken(tk, Token(WHILE, "while"), line);
            }
            else if (info == "return")
            {
                addToken(tk, Token(RETURN, "return"), line);
            }
            else if (info == "int")
            {
                addToken(tk, Token(INT, "int"), line);
            }
            else if (info == "void")
            {
                addToken(tk, Token(VOID, "void"), line);
            }
            else if(info == "def")
            {
                addToken(tk, Token(DEF, "def"), line);
            }
            else
            {
                addToken(tk, Token(ID, info), line);
            }
            info = "";
            TransitionNotDone(c, tk, is_EOF);
            return;
        }
    }
    else if (currentstate == READ_OVER) 
    { 
        switch (c)
        {
        case '/':
            currentstate = COMMENTING; 
            info += c;
            return;
        case '*':
            currentstate = INCOMMENT; 
            info += c;
            return;
        default:
            currentstate = NOT_DONE;
            addToken(tk, Token(DIVIDE, "/"), line);
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
    else if (currentstate == INCOMMENT)
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
    else if (currentstate == WAIT_OVER)
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
            return;
        default:
            currentstate = INCOMMENT;
            info += c;
            return;
        }
    }
    return;
}

void TransitionNotDone(char c, Token_List *tk, bool is_EOF)
{
    currentstate = START;
    Transition(c, tk, is_EOF);
}
void initdfa() // initialize the dfa
{
    currentstate = (STATE)START;
    line = 1;
    info = "";
}
#include "dfa.h"
#include "token.h"
#include <unordered_map>
#include <utility>
#include <string>

STATE currentstate;
int line;
std::string info;

void Transition(char c, Token_List *tk, bool is_EOF)
{
    
    if (is_EOF) // read the end of file
    {
        printf("currentchar: %c\n", c);
        if(currentstate == INCOMMENT || currentstate == WAIT_OVER || currentstate == COMMENTING)
        {
            currentstate = ERROR;
            addToken(tk, Token(ERR, "ERROR! Comment is not closed!"), line);
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
            printf("info: %s\n", info.c_str());
            return;
        default: // �����������ַ�ʱֱ�ӱ���return����main����ѭ���������һ���ж�token�����Ƿ�Ϊerror����䣬��������˳�ѭ��
            currentstate = ERROR;
            addToken(tk, Token(ERR, "ERROR! Illegal character!"), line);
            return;
        }
    }
    if (currentstate == READ_EQ) // ���Ѿ�����һ����=��ʱ
    {
        switch (c)
        {
        case '=': // �ڶ��� ��=�� ���룬 ״̬ΪDONE����tokenlist�м�����EQΪTokentype���½ڵ�
            currentstate = DONE;
            addToken(tk, Token(EQ, "=="), line);
            return;
        default: // û�н��յ��ڶ�����=��״̬ΪNOT_DONE
            currentstate = NOT_DONE;
            addToken(tk, Token(ASSIGN, "="), line); // ��֮ǰ¼��ĵ������Ŵ�������
            TransitionNotDone(c, tk, is_EOF);
            return;
        }
    }
    if (currentstate == READ_GT)
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
        default: // ����ȡ��̾�ţ�֮��û�еȺ���ֱ�ӱ�������ֱ�ӷ��� ��
            currentstate = ERROR;
            addToken(tk, Token(ERR, "ERROR! There are only single ! appeared."), line);
            return;
        }
    }
    else if (currentstate == IN_NUM) // ǰһ�������ֵ����
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
    else if (currentstate == IN_LET) // ǰһ������ĸ�����
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
            else
            {
                addToken(tk, Token(LETTER, info), line);
            }
            info = "";
            return;
        }
    }
    else if (currentstate == READ_OVER) // ǰһ����б�ܵ����
    { 
        switch (c)
        {
        case '/':
            currentstate = COMMENTING; // COMMENTING��#���ۺ�//���۵����
            info += c;
            return;
        case '*':
            currentstate = INCOMMENT; // INCOMMENT��/*���۵����
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
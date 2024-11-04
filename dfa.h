#ifndef DFA_H_
#define DFA_H_
#include "token.h"

enum STATE
{
    START,     // initial state
    READ_EQ,   // read '='
    READ_LT,   // read '<'
    READ_GT,   // read '>'
    READ_NOT,  // read '!'
    READ_OVER, // read '/'
    IN_NUM,    // in number
    IN_ID,     // in identifier
    INCOMMENT, // for /* */comment
    COMMENTING,// for # and // comment
    WAIT_OVER, // wait for the end of comment
    DONE,      // final state
    NOT_DONE,  // not done, yet
    ERROR,     // error state
};

extern STATE currentstate;
extern int line;
extern std::string info;

void initdfa();
void Transition(char c, Token_List *tk, bool is_EOF);
void TransitionNotDone(char c, Token_List *tk, bool is_EOF);

#endif

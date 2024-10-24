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
    INCOMMENT, // in comment
    WAIT_OVER, // wait for the end of comment
    DONE,      // final state
    ERROR,     // error state
    UNGET      // unget the last character
};

void initdfa();

void dfa(std::string str, Token_List *token_list);

#endif
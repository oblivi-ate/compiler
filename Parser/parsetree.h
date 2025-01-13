#ifndef PARSERTREE_H
#define PARSERTREE_H

#include "token.h"
#define MAX_CHILDREN 4

enum NodeKind
{
    NULL_ND, STMT, EXPR, PARAM, DCL
};

enum StmtKind
{
    SLCT_STMT, WHILE_STMT, FOR_STMT, EXPR_STMT, CMPD_STMT, RTN_STMT, NULL_STMT
};

enum ExprKind
{
    OP_EXPR, CONST_EXPR, ID_EXPR, CALL_EXPR, ARRAY_EXPR
};

enum ParamKind
{
    VAR_PARAM, ARRAY_PARAM, VOID_PARAM
};

enum DclKind{
    VAR_DCL, ARRAY_DCL, FUN_DCL
};

enum ExprType
{
    Void, Int
};  

struct TreeNode
{
    int line;

    TreeNode *lSibling;
    TreeNode *rSibling;
    TreeNode *parent;
    TreeNode *child[MAX_CHILDREN];

    NodeKind nodekind;
    union Kind {StmtKind stmt; ExprKind expr; ParamKind param; DclKind dcl; } kind;
    union {
        union {
            TokenType op;             // used by Op_EXPR
            int val;                  // used by Const_EXPR,
            const char * name;        // used by ID_EXPR, Call_EXPR, Array_EXPR
        } exprAttr;                 
        struct { 
            ExprType type;            // used by all dcl and param
            const char * name;        // used by all dcl and param
            int size;                 // used by array declaration
        } dclAttr;                  
    } attr; 

    ExprType type; 

    // constructor of tree node 
    TreeNode()
    {
        for (int i = 0; i < MAX_CHILDREN; i++)
        {
            child[i] = nullptr;
        }
    }
};



#endif //PARSERTREE_H
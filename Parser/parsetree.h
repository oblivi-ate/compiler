#ifndef PARSETREE_H
#define PARSETREE_H

enum NodeKind
{
    STMT, EXPR, PARAM, LIST
};

enum StmtKind
{
    IF, ELSE, ELIF, WHILE, RETURN
};

enum ExprKind
{
    NUM, STR, ID, OP
};

enum ParamKind
{
    
};

enum ListKind{
    STMT, PARAM, ELIF, ARG
};

enum ExprType
{
    VOID, INT, STR
};  

struct TreeNode
{
    NodeKind nodekind;
    union Kind { StmtKind stmt; ExprKind expr; ParamKind param; ListKind list; } kind;
    TreeNode *child[3];
    int line;
    // constructor of tree node
    TreeNode()
    {
        for (int i = 0; i < 3; i++)
        {
            child[i] = nullptr;
        }
    }
};


#endif PARSETREE_H

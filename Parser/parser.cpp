#include "parser.h"
#include "parsetree.h"
#include "token.h"
#include "scanner.cpp"

#include <iostream>

Token_List *pos; // get result from scanner
void get_token(int argc, char *argv[], Token_List *pos)
{
    doScan(argc, argv, pos);
    return;
}

Token_List *next_pos()
{
    pos = pos->next;
    return pos;
}

Token_List *prev_pos()
{
    pos = pos->prev;
    return pos;
}

TreeNode* if_stmt()
{
    TreeNode* root = new TreeNode();

    root->nodekind = NodeKind::STMT;
    root->kind.stmt = StmtKind::EXPR;
    root->line = pos->line;
    root->child[0] = if_clause();
    root->child[1] = elif_clause();
    root->child[2] = else_clause();

    return root;
}

TreeNode* if_clause()
{
    TreeNode* root = new TreeNode();

    if (pos->token.type != TokenType::IF)
    {
        throw "Syntax Error: Expecting 'if' at line " + std::to_string(pos->line);
    }

    root->nodekind = NodeKind::STMT;
    root->kind.stmt = StmtKind::IF;
    root->line = pos->line;

    next_pos();
    root->child[0] = expression();

    if (pos->token.type != TokenType::COLON)
    {
        throw "Syntax Error: Expecting ':' at line " + std::to_string(pos->line);
    }

    next_pos();
    root->child[1] = stmt_list();

    return root;
}

TreeNode* elif_clause()
{
    TreeNode* root = new TreeNode();

    if (pos->token.type != TokenType::ELSE)
    {
        return nullptr;
    }

    next_pos();
    if(pos->token.type != TokenType::IF)
    {
        prev_pos();
        return nullptr;
    }

    next_pos();
    root->child[0] = if_stmt(); // else if -> else if_stmt

    return root;
}

TreeNode* else_clause()
{
    TreeNode* root = new TreeNode();

    if (pos->token.type != TokenType::ELSE)
    {
        return nullptr;
    }

    root->nodekind = NodeKind::STMT;
    root->kind.stmt = StmtKind::ELSE;
    root->line = pos->line;

    next_pos();
    if (pos->token.type != TokenType::COLON)
    {
        throw "Syntax Error: Expecting ':' at line " + std::to_string(pos->line);
    }

    next_pos();
    if(pos->token.type != TokenType::ENTER)
    {
        throw "Syntax Error: Expecting new line at line " + std::to_string(pos->line);
    }

    next_pos();
    root->child[0] = stmt_list();

    return root;
}

TreeNode* expression()
{
    
}

TreeNode* stmt_list()
{   
    TreeNode* root = new TreeNode();

}



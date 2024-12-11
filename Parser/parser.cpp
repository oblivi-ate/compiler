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

TreeNode* program()
{
    TreeNode *root = new TreeNode();
    root->nodekind = NodeKind::LIST;
    root->kind.list = ListKind::STMT;
    root->line = pos->line;

    root->child[0] = declaration_list();

    return root;
}

TreeNode* declaration_list()
{
    TreeNode *root = new TreeNode();
    root->nodekind = NodeKind::LIST;
    root->kind.list = ListKind::STMT;
    root->line = pos->line;

    while (pos->token.type != TokenType::ENDFILE)
    {
        root->child[0] = declaration();
        root->child[1] = declaration_list();
    }

    return root;
}

TreeNode* declaration(){

}

TreeNode* var_declaration(){

}

TreeNode* type_specifier(){

}

TreeNode* fun_declaration(){

}

TreeNode* params(){

}

TreeNode* param_list(){

}

TreeNode* param(){

}

TreeNode* compound_stmt(){

}

TreeNode* local_declarations(){

}

TreeNode* statement_list(){

}

TreeNode* stmt(){

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
    root->child[1] = stmt();

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
    root->child[0] = if_stmt(); // else if --> else if_stmt

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
    root->child[0] = statement_list();

    return root;
}

TreeNode* expression()
{
    TreeNode* root = new TreeNode();

    root->nodekind = NodeKind::EXPR;
    root->kind.expr = ExprKind::OP;
    root->line = pos->line;

    if(pos->token.type == TokenType::ID)
    {
        root->child[0] = var();
        if(pos->token.type != TokenType::ASSIGN)
        {
            throw "Syntax Error: Expecting '=' at line " + std::to_string(pos->line);
        }
        next_pos();
        root->child[1] = expression();
    }
    else
    {
        root->child[0] = simple_expr();
    }

    return root;
}

TreeNode* var(){
    TreeNode* root = new TreeNode();

    if(pos->token.type != TokenType::ID)
    {
        throw "Syntax Error: Expecting identifier at line " + std::to_string(pos->line);
    }

    root->nodekind = NodeKind::EXPR;
    root->kind.expr = ExprKind::ID;
    root->line = pos->line;

    next_pos();
    if(pos->token.type == TokenType::LBRACKET)
    {
        next_pos();
        root->child[0] = expression();
        if(pos->token.type != TokenType::RBRACKET)
        {
            throw "Syntax Error: Expecting ']' at line " + std::to_string(pos->line);
        }
        next_pos();
    }

    return root;
}

TreeNode* simple_expr()
{
    TreeNode* root = new TreeNode();

}

TreeNode* additive_expr()
{
    TreeNode* root = new TreeNode();

}

TreeNode* relop(){

}

TreeNode* addop(){

}

TreeNode* term(){

}

TreeNode* mulop(){

}

TreeNode* factor(){

}

TreeNode* call(){

}

TreeNode* arg(){

}

TreeNode* arg_list(){

}



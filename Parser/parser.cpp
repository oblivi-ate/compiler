#include "parser.h"
#include "parsetree.h"
#include "token.h"

Token_List* pos = ; // get result from scanner

Token_List* next_pos(){
    pos = pos->next;
    return pos;
}

Token_List* prev_pos(){
    pos = pos->prev;
    return pos;
}



TreeNode expr_stmt(){
    TreeNode root = TreeNode();

    root.nodekind = NodeKind::STMT;
    root.kind.stmt = StmtKind::EXPR;
    root.line = pos->line;
    root.child[0] = if_clause();
    root.child[1] = elif_clause_list();
    root.child[2] = else_clause();

    return root;
}

TreeNode if_clause(){
    TreeNode root = TreeNode();

    root.nodekind = NodeKind::STMT;
    root.kind.stmt = StmtKind::IF;
    root.line = pos->line;
    
    if(pos->token.type != TokenType::IF){
        throw "Syntax Error: Expecting 'if' at line " + std::to_string(pos->line);
    }
    next_pos();
    root.child[0] = expression();
    

    return root;
}

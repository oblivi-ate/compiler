#ifndef PARSER_H
#define PARSER_H

#include "parsetree.h"

// declare the function in parser.cpp
TreeNode *program(bool &status);
TreeNode *declaration_list(bool &status);
TreeNode *declaration(bool &status);
TreeNode *var_declaration(bool &status);
TreeNode *type_specifier(bool &status);
TreeNode *fun_declaration(bool &status);
TreeNode *params(bool &status);
TreeNode *param_list(bool &status);
TreeNode *param(bool &status);
TreeNode *compound_stmt(bool &status);
TreeNode *local_declarations(bool &status);
TreeNode *statement_list(bool &status);
TreeNode *stmt(bool &status);
TreeNode *expression_stmt(bool &status);
TreeNode *selection_stmt(bool &status);
TreeNode *iteration_stmt(bool &status);
TreeNode *return_stmt(bool &status);
TreeNode *expression(bool &status);
TreeNode *var(bool &status);
TreeNode *simple_expr(bool &status);
TreeNode *relop(bool &status);
TreeNode *additive_expr(bool &status);
TreeNode *addop(bool &status);
TreeNode *term(bool &status);
TreeNode *mulop(bool &status);
TreeNode *factor(bool &status);
TreeNode *call(bool &status);
TreeNode *args(bool &status);
TreeNode *arg_list(bool &status);


int doParser(int argc, char *argv[], TreeNode *root);

#endif 
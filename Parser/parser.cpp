#include "parser.h"
#include "parsetree.h"
#include "token.h"
#include "scanner.cpp"

#include <iostream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>


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


//WK
bool check(TokenType type) {
    if (pos->token.type == type) {
        next_pos();
        return true;
    }
    return false;
}
void expect(TokenType type, const std::string &msg) {
    if (pos->token.type != type) {
        throw std::runtime_error(msg + " at line " + std::to_string(pos->line));
    }
    next_pos();
}
std::string identifier() {
    expect(TokenType::ID, "Syntax Error: Expecting identifier");
    std::string id = pos->token.info;  // Get the string value from info element of the current Token
    next_pos();  // Move to the next Token
    return id;
}
int integer() {
    expect(TokenType::NUMBER, "Syntax Error: Expecting integer");
    int num = std::stoi(pos->token.info);  // Converts a string to an integer
    next_pos(); 
    return num;
}
//WK end

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

//WK
TreeNode* declaration(){
    if (check(TokenType::DEF)) { // function declaration
        return fun_declaration();
    } else if (check(TokenType::INT) || check(TokenType::FLOAT)) { // Variable declaration
        return var_declaration();
    }
    return nullptr;
}

TreeNode* var_declaration(){
    TreeNode *root = new TreeNode();
    root->nodekind = TreeNode::NodeKind::DECL;
    root->line = pos->line;

    // Assuming the type specifier is already parsed and stored somewhere, here I just get the identifier directly
    std::string var_name = identifier();

    // Checking semicolons
    expect(TokenType::SEMICOLON, "Syntax Error: Expecting ';'");

    // Store the variable name in the declared name
    root->kind.decl.name = var_name;
    return root;
}

TreeNode* type_specifier(){
    TreeNode* root = new TreeNode();
    root->nodekind = NodeKind::DECL;
    // Let type specifiers be simple type keywords such as int, float, and so on
    if (check(TokenType::INT)) {
        root->kind.decl.type = ExprType::INT;
    } else if (check(TokenType::FLOAT)) {
        root->kind.decl.type = ExprType::STR;
    } else {
        throw std::runtime_error("Syntax Error: Unknown type at line " + std::to_string(pos->line));
    }
    return root;
}

TreeNode* fun_declaration(){
    TreeNode *root = new TreeNode();
    root->nodekind = NodeKind::DECL;  // Set the node type to declaration
    root->line = pos->line;

    std::string func_name = identifier();  // Getting the function name

    expect(TokenType::LPAREN, "Syntax Error: Expecting '('");
    TreeNode *params = param_list();  // Parsing the argument list
    expect(TokenType::RPAREN, "Syntax Error: Expecting ')'");
    expect(TokenType::LBRACE, "Syntax Error: Expecting '{'");
    TreeNode *compound = compound_stmt();  // Parsing compound statements
    expect(TokenType::RBRACE, "Syntax Error: Expecting '}'");

    root->kind.decl.name = func_name;  // Store the function name in the declared name

    // Add child nodes to the child array (changed to this because of root error)
    root->child[0] = params;
    root->child[1] = compound;

    return root;
}

TreeNode* params(){
// This function is implemented in param list
}

TreeNode* param_list(){
    TreeNode* root = new TreeNode();
    root->nodekind = NodeKind::LIST;
    root->kind.list = ListKind::PARAM;
    root->line = pos->line;

    while (pos->token.type == TokenType::ID) {
        TreeNode* param = new TreeNode();
        param->nodekind = NodeKind::PARAM;
        param->kind.param = identifier();  // Suppose ParamKind can store parameter names
        root->child.push_back(param);  // Use the vector's push back method
        if (!check(TokenType::COMMA)) break;
    }

    return root;
}

TreeNode* param(){
// This function is implemented in param list
}

TreeNode* compound_stmt(){
    TreeNode* root = new TreeNode();
    root->nodekind = NodeKind::STMT;
    root->kind.stmt = StmtKind::COMPOUND;//COMPOUND refers to the compound statement node in {}
    root->line = pos->line;

    while (pos->token.type != TokenType::RBRACE) {
        root->child.push_back(stmt());  // Add a statement to the list of child nodes
    }

    return root;
}

TreeNode* local_declarations(){
// This function is implemented in compound stmt
}

TreeNode* statement_list(){
    TreeNode* root = new TreeNode();
    root->nodekind = NodeKind::LIST;
    root->kind.list = ListKind::STMT;
    root->line = pos->line;
    while (pos->token.type != TokenType::RBRACE) {
        root->child.push_back(stmt());
    }
    return root;
}

TreeNode* stmt() {
    if (check(TokenType::IF)) {
        return if_stmt();
    } else if (check(TokenType::WHILE)) {
        return while_stmt();
    } else if (check(TokenType::RETURN)) {
        return return_stmt();
    } else {
        return expression_stmt();
    }
}

TreeNode* expression_stmt() {
    TreeNode* root = new TreeNode();
    root->nodekind = NodeKind::STMT;
    root->kind.stmt = StmtKind::EXPR_STMT;//Used to identify Expression statements
    root->line = pos->line;
    root->child.push_back(expression());
    expect(TokenType::SEMICOLON, "Syntax Error: Expecting ';'");
    return root;
}

TreeNode* while_stmt() {
    TreeNode* root = new TreeNode();
    root->nodekind = NodeKind::STMT;
    root->kind.stmt = StmtKind::WHILE;
    root->line = pos->line;
    expect(TokenType::WHILE, "Syntax Error: Expecting 'while'");
    expect(TokenType::LPAREN, "Syntax Error: Expecting '('");
    root->child.push_back(expression());
    expect(TokenType::RPAREN, "Syntax Error: Expecting ')'");
    expect(TokenType::COLON, "Syntax Error: Expecting ':'");
    root->child.push_back(stmt());
    return root;
}

TreeNode* return_stmt() {
    TreeNode* root = new TreeNode();
    root->nodekind = NodeKind::STMT;
    root->kind.stmt = StmtKind::RETURN;
    root->line = pos->line;
    expect(TokenType::RETURN, "Syntax Error: Expecting 'return'");
    if (pos->token.type != TokenType::SEMICOLON) {
        root->child.push_back(expression());
    }
    expect(TokenType::SEMICOLON, "Syntax Error: Expecting ';'");
    return root;
}

//WK end

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

//WK
TreeNode* additive_expr()
{
    TreeNode* root = new TreeNode();
    root->nodekind = NodeKind::EXPR;
    root->kind.expr = ExprKind::OP;
    root->line = pos->line;

    TreeNode* left = term();  // 解析乘法和除法表达式
    root->child.push_back(left);

    while (pos->token.type == TokenType::PLUS || pos->token.type == TokenType::MINUS) {
        TokenType op = pos->token.type;
        next_pos();  // 跳过操作符
        TreeNode* right = term();  // 解析下一个乘法和除法表达式
        TreeNode* bin_node = new TreeNode();
        bin_node->nodekind = NodeKind::EXPR;
        bin_node->kind.expr = ExprKind::OP;
        bin_node->line = pos->line;
        bin_node->child.push_back(left);
        bin_node->child.push_back(right);
        left = bin_node;  // 更新左侧表达式
    }

    root->child.push_back(left);
    return root;

}
//WK end

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



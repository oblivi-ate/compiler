#include "parser.h"
#include "parsetree.h"
#include "token.h"
#include "scanner.h"

#include <iostream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>



Token_List *pos; // get result from scanner

void test(std::string s)
{
    std::cout << "Parser: " << s << std::endl;
}

void test(){
    std::cout << pos->token.info << std::endl;
}

void get_parser_token(int argc, char *argv[])
{
    doScan(argc, argv, pos);
    return;
}

Token_List *next_pos()
{
    if (pos->next == nullptr)
    {
        return pos;
    }
    pos = pos->next;
    return pos;
}

Token_List *prev_pos()
{
    if (pos->prev == nullptr)
    {
        return pos;
    }
    pos = pos->prev;
    return pos;
}

bool check(TokenType type)
{
    if (pos->token.type == type)
    {
        next_pos();
        return true;
    }
    return false;
}

void expect(const std::string &msg)
{
    throw std::runtime_error(msg + " at line " + std::to_string(pos->line));
}

std::string identifier()
{
    std::string id = pos->token.info; // Get the string value from info element of the current Token
    next_pos();                       // Move to the next Token
    return id;
}
int integer()
{
    int num = std::stoi(pos->token.info); // Converts a string to an integer
    next_pos();
    return num;
}

TreeNode *program(bool &status) // program → declaration-list
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = declaration_list(s), s == true)
    {
        status = true;
        return result;
    }

    status = false;
    return nullptr;
}

TreeNode *declaration_list(bool &status) // declaration-list → declaration | declaration declaration-list
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = declaration(s), s == true)
    {
        if (result->child[1] = declaration_list(s), s == true)
        {

            status = true;
            return result;
        }
        else
        {
            status = true;
            return result;
        }
    }

    status = false;
    return nullptr;
}

TreeNode *declaration(bool &status) // declaration → var-declaration | fun-declaration
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = var_declaration(s), s == true)
    {
        status = true;
        return result;
    }
    else if (result->child[0] = fun_declaration(s), s == true)
    {
        status = true;
        return result;
    }

    status = false;
    return nullptr;
}

TreeNode *var_declaration(bool &status) //  var-declaration → type-specifier ID ; | type-specifier ID [ NUM ] ;
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::DCL;
    result->line = pos->line;

    if (result->child[0] = type_specifier(s), s == true)
    {
        if (check(TokenType::ID))
        {
            result->kind.dcl = DclKind::VAR_DCL;
            prev_pos();
            result->attr.dclAttr.name = pos->token.info.c_str();
            next_pos();
            if (check(TokenType::LBRACKET))
            {
                result->kind.dcl = DclKind::ARRAY_DCL;
                if (check(TokenType::NUMBER))
                {
                    prev_pos();
                    result->attr.dclAttr.size = integer();
                    next_pos();
                    if (check(TokenType::RBRACKET))
                    {
                        if (check(TokenType::SEMICOLON))
                        {
                            status = true;
                            return result;
                        }
                        else
                        {
                            expect("Syntax Error: Expecting ';'");
                        }
                    }
                    else
                    {
                        expect("Syntax Error: Expecting ']'");
                    }
                }
                else
                {
                    expect("Syntax Error: Expecting 'NUMBER'");
                }
            }
            else if (check(TokenType::LPAREN))
            {
                prev_pos(); // back to LPAREN
                prev_pos(); // back to ID
                prev_pos(); // back to type-specifier
                status = false;
                result->nodekind = NodeKind::NULL_ND;
                return nullptr;
            }
            else
            {
                if (check(TokenType::SEMICOLON))
                {
                    status = true;
                    return result;
                }
                else
                {
                    expect("Syntax Error: Expecting ';'");
                }
            }
        }
        else
        {
            expect("Syntax Error: Expecting 'ID'");
        }
    }

    status = false;
    result->nodekind = NodeKind::NULL_ND;
    return nullptr;
}

TreeNode *type_specifier(bool &status) // type-specifier → int | void
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::EXPR;
    result->kind.expr = ExprKind::TYPE_EXPR;
    result->line = pos->line;

    if (check(TokenType::INT))
    {
        result->attr.dclAttr.type = ExprType::Int;
        status = true;
        return result;
    }
    else if (check(TokenType::VOID))
    {
        result->attr.dclAttr.type = ExprType::Void;
        status = true;
        return result;
    }

    status = false;
    return nullptr;
}

TreeNode *fun_declaration(bool &status) // fun-declaration → type-specifier ID ( params ) compound-stmt | def ID (params) compound-stmt
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::DCL;
    result->line = pos->line;

    if (result->child[0] = type_specifier(s), s == true)
    {
        if (check(TokenType::ID))
        {
            result->kind.dcl = DclKind::FUN_DCL;
            prev_pos();
            result->attr.dclAttr.name = pos->token.info.c_str();
            next_pos();
            if (check(TokenType::LPAREN))
            {
                if (result->child[1] = params(s), s == true)
                {
                    if (check(TokenType::RPAREN))
                    {
                        if (check(TokenType::LBRACE))
                        {

                            if (result->child[2] = compound_stmt(s), s == true)
                            {
                                if (check(TokenType::RBRACE))
                                {
                                    if (check(TokenType::SEMICOLON))
                                    {
                                        status = true;
                                        return result;
                                    }
                                    else
                                    {
                                        expect("Syntax Error: Expecting ';'");
                                    }
                                }
                                else
                                {
                                    expect("Syntax Error: Expecting '}'");
                                }
                            }
                        }
                        else
                        {
                            expect("Syntax Error: Expecting '{'");
                        }
                    }
                    else
                    {
                        expect("Syntax Error: Expecting ')'");
                    }
                }
            }
            else
            {
                expect("Syntax Error: Expecting '('");
            }
        }
        else if (check(TokenType::DEF))
        {
            if (check(TokenType::ID))
            {
                result->kind.dcl = DclKind::FUN_DCL;
                prev_pos();
                result->attr.dclAttr.name = pos->token.info.c_str();
                next_pos();
                if (check(TokenType::LPAREN))
                {
                    if (result->child[1] = params(s), s == true)
                    {
                        if (check(TokenType::RPAREN))
                        {
                            if (check(TokenType::LBRACE))
                            {

                                if (result->child[2] = compound_stmt(s), s == true)
                                {
                                    if (check(TokenType::RBRACE))
                                    {
                                        if (check(TokenType::SEMICOLON))
                                        {
                                            status = true;
                                            return result;
                                        }
                                        else
                                        {
                                            expect("Syntax Error: Expecting ';'");
                                        }
                                    }
                                    else
                                    {
                                        expect("Syntax Error: Expecting '}'");
                                    }
                                }
                            }
                            else
                            {
                                expect("Syntax Error: Expecting '{'");
                            }
                        }
                        else
                        {
                            expect("Syntax Error: Expecting ')'");
                        }
                    }
                }
                else
                {
                    expect("Syntax Error: Expecting '('");
                }
            }
        }
    }
    result->nodekind = NodeKind::NULL_ND;
    status = false;
    return nullptr;
}
TreeNode *params(bool &status) // params → void | param-list
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = param_list(s), s == true)
    {
        status = true;
        return result;
    }

    status = true;
    return result;
}

TreeNode *param_list(bool &status) // param-list → param | param , param-list
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = param(s), s == true)
    {
        if (check(TokenType::COMMA))
        {
            if (result->child[1] = param_list(s), s == true)
            {
                status = true;
                return result;
            }
        }
        else
        {
            status = true;
            return result;
        }
    }

    status = false;
    return nullptr;
}

TreeNode *param(bool &status) // param → type-specifier ID | type-specifier ID [ ]
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::PARAM;
    result->line = pos->line;

    if (result->child[0] = type_specifier(s), s == true)
    {
        if (check(TokenType::ID))
        {
            result->attr.dclAttr.name = pos->token.info.c_str();
            if (check(TokenType::LBRACKET))
            {
                if (check(TokenType::RBRACKET))
                {
                    status = true;
                    return result;
                }
                else
                {
                    expect("Syntax Error: Expecting ']'");
                }
            }
            else
            {
                status = true;
                return result;
            }
        }
    }

    status = false;
    return nullptr;
}

TreeNode *compound_stmt(bool &status) // compound-stmt → local-declarations statement-list
{
    bool s = false;
    TreeNode *root = new TreeNode();
    root->nodekind = NodeKind::STMT;
    root->kind.stmt = StmtKind::CMPD_STMT;
    root->line = pos->line;

    if (check(TokenType::RBRACE))
    {
        prev_pos();
        status = false;
        return nullptr;
    }
    if (root->child[0] = local_declarations(s), s == true)
    {
        if (root->child[1] = statement_list(s), s == true)
        {
            status = true;
            return root;
        }
        else
        {
            expect("Syntax Error: Expecting statement_list");
        }
    }
    status = false;
    return nullptr;
}

TreeNode *local_declarations(bool &status) // local-declarations → var-declarations local-declaration | empty
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = var_declaration(s), s == true)
    {
        if (result->child[1] = local_declarations(s), s == true)
        {
            status = true;
            return result;
        }
        else{
            prev_pos();
        }
    }

    status = true;
    return result;
}

TreeNode *statement_list(bool &status) // statement-list → statement statement-list | empty
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = stmt(s), s == true)
    {
        if (result->child[1] = statement_list(s), s == true)
        {
            status = true;
            return result;
        }
        else
        {
            status = true;
            return result;
        }
    }

    status = false;
    return nullptr;
}

TreeNode *stmt(bool &status) // statement → expression-stmt | compound-stmt | selection-stmt | iteration-stmt | return-stmt
{
    if (pos->token.type == TokenType::IF)
    {
        return selection_stmt(status);
    }
    else if (pos->token.type == TokenType::WHILE)
    {
        return iteration_stmt(status);
    }
    else if (pos->token.type == TokenType::RETURN)
    {
        return return_stmt(status);
    }
    else if (pos->token.type == TokenType::LBRACE)
    {
        return compound_stmt(status);
    }
    else
    {
        return expression_stmt(status);
    }

    return nullptr;
}

TreeNode *expression_stmt(bool &status) // expression-stmt → expression ; | ;
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::STMT;
    result->kind.stmt = StmtKind::EXPR_STMT;
    result->line = pos->line;

    if (check(TokenType::SEMICOLON))
    {
        status = true;
        return result;
    }
    else if (result->child[0] = expression(s), s == true)
    {
        if (check(TokenType::SEMICOLON))
        {
            status = true;
            return result;
        }
    }
    status = false;
    return nullptr;
}

TreeNode *selection_stmt(bool &status) // selection-stmt → if ( expression ) { stmt_list } | if ( expression ) { stmt_list } else { stmt_list } | if ( expression ) : { stmt_list } | if ( expression ) : { stmt_list } else : { stmt_list }
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::STMT;
    result->kind.stmt = StmtKind::SLCT_STMT;
    result->line = pos->line;

    if (check(TokenType::IF))
    {
        if (check(TokenType::LPAREN))
        {
            if (result->child[0] = expression(s), s == true)
            {
                if (check(TokenType::RPAREN))
                {
                    check(TokenType::COLON);
                    if (check(TokenType::LBRACE))
                    {
                        if (result->child[1] = statement_list(s), s == true)
                        {
                            if (check(TokenType::RBRACE))
                            {
                                if (check(TokenType::ELSE))
                                {
                                    check(TokenType::COLON);
                                    if (check(TokenType::LBRACE))
                                    {
                                        if (result->child[2] = statement_list(s), s == true)
                                        {
                                            if (check(TokenType::RBRACE))
                                            {
                                                status = true;
                                                return result;
                                            }
                                            else
                                            {
                                                expect("Syntax Error: Expecting '}'");
                                            }
                                        }
                                    }
                                    else
                                    {
                                        expect("Syntax Error: Expecting '{'");
                                    }
                                }
                                else
                                {
                                    status = true;
                                    return result;
                                }
                            }
                            else
                            {
                                expect("Syntax Error: Expecting '}'");
                            }
                        }
                    }
                    else
                    {
                        expect("Syntax Error: Expecting '{'");
                    }
                }
            }
        }
    }

    status = false;
    return nullptr;
}

TreeNode *iteration_stmt(bool &status) // iteration-stmt → while ( expression ) { stmt_list } | while ( expression ) : { stmt_list }
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::STMT;
    result->kind.stmt = StmtKind::WHILE_STMT;
    result->line = pos->line;

    if (check(TokenType::WHILE))
    {
        if (check(TokenType::LPAREN))
        {
            if (result->child[0] = expression(s), s == true)
            {
                if (check(TokenType::RPAREN))
                {
                    if (check(TokenType::LBRACE))
                    {
                        check(TokenType::COLON);
                        if (result->child[1] = statement_list(s), s == true)
                        {
                            if (check(TokenType::RBRACE))
                            {
                                status = true;
                                return result;
                            }
                            else
                            {
                                expect("Syntax Error: Expecting '}'");
                            }
                        }
                    }
                    else
                    {
                        expect("Syntax Error: Expecting '{'");
                    }
                }
                else
                {
                    expect("Syntax Error: Expecting ')'");
                }
            }
        }
    }

    status = false;
    return nullptr;
}

TreeNode *return_stmt(bool &status) // return-stmt → return ; | return expression ;
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::STMT;
    result->kind.stmt = StmtKind::RTN_STMT;
    result->line = pos->line;

    if (check(TokenType::RETURN))
    {
        if (check(TokenType::SEMICOLON))
        {
            status = true;
            return result;
        }
        else if (result->child[0] = expression(s), s == true)
        {
            if (check(TokenType::SEMICOLON))
            {
                status = true;
                return result;
            }
            else
            {
                expect("Syntax Error: Expecting ';'");
            }
        }
        else
        {
            expect("Syntax Error: Expecting ';'");
        }
    }

    status = false;
    return nullptr;
}

TreeNode *expression(bool &status) // expression → var = expression | simple-exprssion
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = var(s), s == true)
    {
        if (check(TokenType::ASSIGN))
        {
            if (result->child[1] = expression(s), s == true)
            {
                status = true;
                return result;
            }
            else
            {
                prev_pos(); // back to ASSIGN
            }
        }
        else
        {
            prev_pos(); // back to var
        }
    }
    else if (result->child[0] = simple_expr(s), s == true)
    {
        status = true;
        return result;
    }

    status = false;
    return nullptr;
}

TreeNode *var(bool &status) // var → ID | ID [ expression ]
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::EXPR;
    result->kind.expr = ExprKind::ID_EXPR;
    result->line = pos->line;

    if (pos->token.type == TokenType::ID)
    {
        result->attr.exprAttr.name = pos->token.info.c_str();
        next_pos();
        if (check(TokenType::LBRACKET))
        {
            if (result->child[0] = expression(s), s == true)
            {
                if (check(TokenType::RBRACKET))
                {
                    status = true;
                    return result;
                }
            }
        }
        else
        {
            status = true;
            return result;
        }
    }

    status = false;
    return nullptr;
}

TreeNode *simple_expr(bool &status) // simple-expression → additive-expression | additive-expression relop simple-expression
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    
    if (result->child[0] = additive_expr(s), s == true)
    {

        if (result->child[1] = relop(s), s == true)
        {

            if (result->child[2] = simple_expr(s), s == true)
            {
                status = true;
                return result;
            }
        }
        else
        {
            status = true;
            return result;
        }
    }

    status = false;
    return nullptr;
}

TreeNode *relop(bool &status) // relop → <= | < | > | >= | == | !=
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::EXPR;
    result->kind.expr = ExprKind::OP_EXPR;
    result->line = pos->line;

    if (check(TokenType::LTE) || check(TokenType::LT) || check(TokenType::GT) || check(TokenType::GTE) || check(TokenType::EQ) || check(TokenType::NEQ))
    {
        result->attr.exprAttr.op = pos->token.type;
        status = true;
        return result;
    }

    status = false;
    return nullptr;
}

TreeNode *additive_expr(bool &status) // additive_expr → term | term addop additive_expr
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = term(s), s == true)
    {
        if (check(TokenType::PLUS) || check(TokenType::MINUS))
        {
            if (result->child[1] = addop(s), s == true)
            {
                if (result->child[2] = additive_expr(s), s == true)
                {
                    status = true;
                    return result;
                }
            }
        }
        else
        {
            status = true;
            return result;
        }
    }

    status = false;
    return nullptr;
}

TreeNode *addop(bool &status) // addop → + | -
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::EXPR;
    result->kind.expr = ExprKind::OP_EXPR;
    result->line = pos->line;

    if (check(TokenType::PLUS) || check(TokenType::MINUS))
    {
        result->attr.exprAttr.op = pos->token.type;
        status = true;
        return result;
    }

    status = false;
    expect("Syntax Error: Expecting '+' or '-'");
    return nullptr;
}

TreeNode *term(bool &status) // term → factor | factor mulop term
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;
    if (result->child[0] = factor(s), s == true)
    {
        if (check(TokenType::TIMES) || check(TokenType::DIVIDE))
        {
            if (result->child[1] = mulop(s), s == true)
            {
                if (result->child[2] = term(s), s == true)
                {
                    status = true;
                    return result;
                }
            }
        }
        else
        {
            status = true;
            return result;
        }
    }

    status = false;
    return nullptr;
}

TreeNode *mulop(bool &status) // mulop → * | /
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::EXPR;
    result->kind.expr = ExprKind::OP_EXPR;
    result->line = pos->line;

    if (check(TokenType::TIMES) || check(TokenType::DIVIDE))
    {
        result->attr.exprAttr.op = pos->token.type;
        status = true;
        return result;
    }

    status = false;
    expect("Syntax Error: Expecting '*' or '/'");
    return nullptr;
}

TreeNode *factor(bool &status) // factor → ( expression ) | NUM | var | call
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->line = pos->line;

    if (check(TokenType::LPAREN))
    {
        if (result->child[0] = expression(s), s == true)
        {
            if (check(TokenType::RPAREN))
            {
                result->nodekind = NodeKind::NULL_ND;
                status = true;
                return result;
            }
        }
    }
    else if (pos->token.type == TokenType::NUMBER)
    {
        result->nodekind = NodeKind::EXPR;
        result->kind.expr = ExprKind::CONST_EXPR;
        result->attr.exprAttr.val = integer();
        status = true;

        return result;
    }
    else if (pos->token.type == TokenType::ID)
    {
        result->nodekind = NodeKind::NULL_ND;
        if (result->child[0] = var(s), s == true)
        {
            status = true;
            return result;
        }
        if (result->child[0] = call(s), s == true)
        {
            status = true;
            return result;
        }
    }

    status = false;
    return nullptr;
}

TreeNode *call(bool &status) // call → ID (args)
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::EXPR;
    result->kind.expr = ExprKind::CALL_EXPR;
    result->line = pos->line;
    result->attr.exprAttr.name = identifier().c_str();

    if (check(TokenType::LPAREN))
    {
        if (result->child[0] = args(s), s == true)
        {
            if (check(TokenType::RPAREN))
            {
                status = true;
                return result;
            }
            else
            {
                expect("Syntax Error: Expecting ')'");
            }
        }
    }

    status = false;
    return nullptr;
}

TreeNode *args(bool &status) // args → arg_list | empty
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = arg_list(s), s == true)
    {
        status = true;
        return result;
    }

    status = false;
    return result;
}

TreeNode *arg_list(bool &status) // arg_list → expression | expression, arg_list
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (result->child[0] = expression(s), s == true)
    {
        if (check(TokenType::COMMA))
        {
            if (result->child[1] = arg_list(s), s == true)
            {
                status = true;
                return result;
            }
        }
        else
        {
            status = true;
            return result;
        }
    }

    status = false;
    return nullptr;
}

void printTree(TreeNode *root)
{
    std::vector<TreeNode *> queue;
    queue.push_back(root);
    while (!queue.empty())
    {
        TreeNode *node = queue.front();
        queue.erase(queue.begin());
        std::cout << "NodeKind: " << node->nodekind << std::endl;
        std::cout << "Line: " << node->line << std::endl;
        if (node->nodekind == NodeKind::DCL && node->kind.dcl == DclKind::VAR_DCL)
        {
            std::cout << "Name: " << node->attr.dclAttr.name << std::endl;
        }
        for (int i = 0; i < MAX_CHILDREN; i++)
        {
            if (node->child[i] != nullptr)
            {
                queue.push_back(node->child[i]);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    std::cout << "Parser" << std::endl;
    get_parser_token(argc, argv);
    printToken(pos);
    bool status = false;
    TreeNode *root = program(status);
    if (status == true)
    {
        std::cout << "Parsing Successful" << std::endl;
        // printTree(root);
    }
    else
    {
        std::cout << "Parsing Failed" << std::endl;
    }
    // print tree

    printTree(root);

    return 0;
}
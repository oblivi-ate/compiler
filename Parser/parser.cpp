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
void get_parser_token(int argc, char *argv[], Token_List *pos)
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

    if(result->child[0] = var_declaration(s), s == true)
    {
        status = true;
        return result;
    }
    else if(result->child[0] = fun_declaration(s), s == true)
    {
        status = true;
        return result;
    }

    status = false;
    return nullptr;
}

TreeNode *var_declaration(bool &status) //  var-declaration → type-specifier ID | type-specifier ID [ NUM ]
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::DCL;
    result->line = pos->line;

    if (result->child[0] = type_specifier(s), s == true)
    {
        if (check(TokenType::ID))
        {
            result->attr.dclAttr.name = pos->token.info.c_str();
            if (check(TokenType::LBRACKET))
            {
                if (check(TokenType::NUMBER))
                {
                    result->attr.dclAttr.size = integer();
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
                    expect("Syntax Error: Expecting 'NUM'");
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
            expect("Syntax Error: Expecting 'ID'");
        }
    }

    status = false;
    return nullptr;
}

TreeNode *type_specifier(bool &status) // type-specifier → int | void
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::DCL;
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
            result->attr.dclAttr.name = pos->token.info.c_str();
            if (check(TokenType::LPAREN))
            {
                if (result->child[1] = params(s), s == true)
                {
                    if (check(TokenType::RPAREN))
                    {
                        if (result->child[2] = compound_stmt(s), s == true)
                        {
                            status = true;
                            return result;
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
    else if(check(TokenType::DEF))
    {
        if (check(TokenType::ID))
        {
            result->attr.dclAttr.name = pos->token.info.c_str();
            if (check(TokenType::LPAREN))
            {
                if (result->child[1] = params(s), s == true)
                {
                    if (check(TokenType::RPAREN))
                    {
                        if (result->child[2] = compound_stmt(s), s == true)
                        {
                            status = true;
                            return result;
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

TreeNode *params(bool &status) // params → void | param-list 
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if (check(TokenType::VOID))
    {
        status = true;
        return result;
    }
    else if (result->child[0] = param_list(s), s == true)
    {
        status = true;
        return result;
    }

    status = false;
    return nullptr;
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

    if(result->child[0] = type_specifier(s), s == true)
    {
        if(check(TokenType::ID))
        {
            result->attr.dclAttr.name = pos->token.info.c_str();
            if(check(TokenType::LBRACKET))
            {
                if(check(TokenType::RBRACKET))
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

TreeNode *compound_stmt(bool &status)
{
    bool s = false;
    TreeNode *root = new TreeNode();
    root->nodekind = NodeKind::STMT;
    root->kind.stmt = StmtKind::CMPD_STMT;
    root->line = pos->line;

    if (check(TokenType::LBRACE))
    {
        root->child[0] = local_declarations(s);
        root->child[1] = statement_list(s);
        if (check(TokenType::RBRACE))
        {
            status = true;
            return root;
        }
        else
        {
            expect("Syntax Error: Expecting '}'");
        }
    }
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
    }

    status = false;
    return result;
}

TreeNode *statement_list(bool &status) // statement-list → statement statement-list | empty
{
    bool s = false;
    TreeNode *result = new TreeNode();
    result->nodekind = NodeKind::NULL_ND;
    result->line = pos->line;

    if(result->child[0] = stmt(s), s == true)
    {
        if(result->child[1] = statement_list(s), s == true)
        {
            status = true;
            return result;
        }
    }

    status = false;
    return result;
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

TreeNode *selection_stmt(bool &status) // selection-stmt → if ( expression ) statement | if ( expression ) statement else statement | if ( expression ) : statement | if ( expression ) : statement else : statement
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
                    if (result->child[1] = stmt(s), s == true)
                    {
                        if (check(TokenType::ELSE))
                        {
                            check(TokenType::COLON);
                            if (result->child[2] = stmt(s), s == true)
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
                }
            }
        }
    }

    status = false;
    return nullptr;
}

TreeNode *iteration_stmt(bool &status) // iteration-stmt → while ( expression ) statement | while ( expression ) : statement
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
                    if (result->child[1] = stmt(s), s == true)
                    {
                        status = true;
                        return result;
                    }
                    else if (check(TokenType::COLON))
                    {
                        if (result->child[1] = stmt(s), s == true)
                        {
                            status = true;
                            return result;
                        }
                    }
                    else
                    {
                        expect("Syntax Error: Expecting ':'");
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
    expect("Syntax Error: Expecting 'ID'");
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
    expect("Syntax Error: Expecting '<=', '<', '>', '>=', '==', or '!='");
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


int main(int argc, char *argv[])
{
    get_parser_token(argc, argv, pos);
    bool status = false;
    TreeNode *root = program(status);
    if (status == true)
    {
        std::cout << "Parsing Successful" << std::endl;
        //printTree(root);
    }
    return 0;
}
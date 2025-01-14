#include "parser_print.h"

#include <iostream>

#define INDENT_GAP 2

const char* token_type_to_string(TokenType type) {
    switch (type) {
    case TokenType::PLUS: return "+";
    case TokenType::MINUS: return "-";
    case TokenType::TIMES: return "*";
    case TokenType::DIVIDE: return "/";
    case TokenType::ASSIGN: return "=";
    case TokenType::LTE: return "<=";
    case TokenType::LT: return "<";
    case TokenType::GT: return ">";
    case TokenType::GTE: return ">=";
    case TokenType::EQ: return "==";
    case TokenType::NEQ: return "!=";
    default: return "Unknown Token";
    }
}

static void print_expr_type(ExprType t) {
    switch (t) {
    case ExprType::Void: printf("void"); break;
    case ExprType::Int: printf("int"); break;
    default: printf("Error ExpType"); break;
    }
}

static void print_spaces(int indentNum) {
    for (int i = 0; i < indentNum; i++) {
        printf(" ");
    }
}

void print_tree(TreeNode *tree) {
    static int indentNum = 0;
    indentNum += INDENT_GAP;

    while (tree != NULL) {
        print_spaces(indentNum);
        switch (tree->nodekind) {
        case NodeKind::DCL:
            printf("Declare: ");
            printf(" %s ", tree->attr.dclAttr.name);
            switch (tree->kind.dcl) {
            case DclKind::ARRAY_DCL: printf("[%d]\n", tree->attr.dclAttr.size); break;
            case DclKind::FUN_DCL: printf("function with parameters:\n"); break;
            case DclKind::VAR_DCL: printf("\n"); break;
            default: printf("Unknown DclNode kind\n"); break;
            }
            break;
        case NodeKind::PARAM:
            printf("Parameter: ");
            print_expr_type(tree->attr.dclAttr.type);
            if (tree->attr.dclAttr.type != ExprType::Void) {
                printf(" %s", tree->attr.dclAttr.name);
                if (tree->kind.param == ParamKind::ARRAY_PARAM)
                    printf(" [ ]");
            }
            printf("\n");
            break;
        case NodeKind::STMT:
            switch (tree->kind.stmt) {
            case StmtKind::SLCT_STMT: printf("If "); break;
            case StmtKind::WHILE_STMT: printf("while stmt: \n"); break;
            case StmtKind::FOR_STMT: printf("for stmt: \n"); break;
            case StmtKind::EXPR_STMT: printf("Expression stmt: \n"); break;
            case StmtKind::CMPD_STMT: printf("Compound Stmt:\n"); break;
            case StmtKind::RTN_STMT: printf("Return \n"); break;
            case StmtKind::NULL_STMT: printf("Null statement: ;\n"); break;
            default: printf("Unknown StmtNode kind\n"); break;
            }
            if (tree->kind.stmt == StmtKind::SLCT_STMT && tree->child[2] != NULL) {
                printf(" with ELSE \n");
            } else if (tree->kind.stmt == StmtKind::SLCT_STMT) {
                printf(" without ELSE \n");
            }
            break;
        case NodeKind::EXPR:
            switch (tree->kind.expr) {
            case ExprKind::OP_EXPR: {
                printf("Operator: ");
                if (tree->attr.exprAttr.op == TokenType::LBRACKET)
                    printf("[] index operator");
                else
                    printf("%s", token_type_to_string(tree->attr.exprAttr.op));
                printf("\n");
                break;
            }
            case ExprKind::CONST_EXPR: printf("Const: %d\n", tree->attr.exprAttr.val); break;
            case ExprKind::ID_EXPR: printf("ID: %s\n", tree->attr.exprAttr.name); break;
            case ExprKind::CALL_EXPR: printf("Call function: %s, with arguments:\n", tree->attr.exprAttr.name); break;
            case ExprKind::ARRAY_EXPR: printf("Array: %s\n", tree->attr.exprAttr.name); break;
            case ExprKind::TYPE_EXPR: printf("Type: "); print_expr_type(tree->attr.dclAttr.type); printf("\n"); break;
            default: printf("Unknown ExpNode kind\n"); break;
            }
            break;
        default:
            std::cout << tree->info << std::endl;
            break;
        }
        for (int i = 0; i < MAX_CHILDREN; i++) {
            print_tree(tree->child[i]);
        }
        tree = tree->rSibling;
    }
    indentNum -= INDENT_GAP;
}
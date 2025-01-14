#ifndef PARSER_PRINT_H
#define PARSER_PRINT_H

#include "parser.h"  // Ensure this includes the definition of TreeNode

void print_tree(TreeNode *);
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

#endif PARSER_PRINT_H
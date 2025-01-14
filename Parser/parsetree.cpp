// #include <sstream>
// #include <unordered_map>
// #include <iomanip>
// #include <iostream>
// #include "parsetree.h"
// #include "token.h"

// // 定义 TokenType 对应的字符串名称
// static std::string tokenNames[] = {
//     "IF", "ELSE", "WHILE", "RETURN", "INT", "VOID", 
//     "PLUS", "MINUS", "TIMES", "DIVIDE", "MOD", 
//     "LT", "LTE", "GT", "GTE", "EQ", "NEQ", "ASSIGN", 
//     "SEMICOLON", "COMMA", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "LBRACE", "RBRACE", "COLON", 
//     "COMMENT", 
//     "NUMBER",
//     "WS",
//     "ID",
//     "INDENT",
//     "ERR",
//     "ENTER",
//     "ENDFILE",
//     "ST",
//     "DEF"
// };

// // 定义 NodeKind 对应的字符串名称
// static std::string nodeKindNames[] = {
//     "NULL_ND", "STMT", "EXPR", "PARAM", "DCL"
// };

// // 定义 StmtKind 对应的字符串名称
// static std::string stmtKindNames[] = {
//     "SLCT_STMT", "WHILE_STMT", "FOR_STMT", "EXPR_STMT", "CMPD_STMT", "RTN_STMT", "NULL_STMT"
// };

// // 定义 ExprKind 对应的字符串名称
// static std::string exprKindNames[] = {
//     "OP_EXPR", "CONST_EXPR", "ID_EXPR", "CALL_EXPR", "ARRAY_EXPR"
// };

// // 定义 ParamKind 对应的字符串名称
// static std::string paramKindNames[] = {
//     "VAR_PARAM", "ARRAY_PARAM", "VOID_PARAM"
// };

// // 定义 DclKind 对应的字符串名称
// static std::string dclKindNames[] = {
//     "VAR_DCL", "ARRAY_DCL", "FUN_DCL"
// };

// // 定义 ExprType 对应的字符串名称
// static std::string exprTypeNames[] = {
//     "Void", "Int"
// };

// // TreeNode 构造函数实现
// TreeNode::TreeNode()
//     : line(0), lSibling(nullptr), rSibling(nullptr), parent(nullptr), nodekind(NULL_ND), type(Void)
// {
//     for (int i = 0; i < MAX_CHILDREN; ++i) {
//         child[i] = nullptr;
//     }
//     kind.stmt = StmtKind::NULL_STMT;
//     attr.dclAttr = { Void, nullptr, 0 };
// }

// // TreeNode 析构函数实现
// TreeNode::~TreeNode()
// {
//     // 根据 nodekind 清理不同的联合体成员
//     if (nodekind == STMT) {
//         if (kind.stmt == StmtKind::SLCT_STMT) {
//             if (attr.dclAttr.name) {
//                 delete[] attr.dclAttr.name;
//                 attr.dclAttr.name = nullptr;
//             }
//         }
//     }
//     else if (nodekind == PARAM) {
//         if (attr.dclAttr.name) {
//             delete[] attr.dclAttr.name;
//             attr.dclAttr.name = nullptr;
//         }
//     }
//     else if (nodekind == EXPR) {
//         switch (kind.expr) {
//             case ExprKind::ID_EXPR:
//             case ExprKind::CALL_EXPR:
//             case ExprKind::ARRAY_EXPR:
//                 if (attr.exprAttr.name) {
//                     delete[] attr.exprAttr.name;
//                     attr.exprAttr.name = nullptr;
//                 }
//                 break;
//             case ExprKind::CONST_EXPR:
//                 break;
//             case ExprKind::OP_EXPR:
//                 break;
//             default:
//                 break;
//         }
//     }

//     // 额外的清理，如果有的话
//     if (type == Int || type == Void) {
//         // 根据需要添加更多的清理逻辑
//     }
// }

// // 重载输出流运算符，用于打印 TreeNode 信息
// std::ostream& operator<<(std::ostream& os, const TreeNode& t)
// {
//     static int spaceNum = 0;
//     os << std::string(spaceNum, ' ');
//     os << t.line << ": " << nodeKindNames[static_cast<int>(t.nodekind)] << " ";

//     switch (t.nodekind)
//     {
//         case STMT:
//             os << stmtKindNames[static_cast<int>(t.kind.stmt)] << " ";
//             if (t.kind.stmt == StmtKind::SLCT_STMT || t.kind.stmt == StmtKind::WHILE_STMT) {
//                 os << (t.attr.dclAttr.name ? t.attr.dclAttr.name : "null") << " "
//                    << exprTypeNames[static_cast<int>(t.attr.dclAttr.type)] << " "
//                    << t.attr.dclAttr.size << " ";
//             }
//             break;
//         case EXPR:
//             os << exprKindNames[static_cast<int>(t.kind.expr)] << " ";
//             switch (t.kind.expr) {
//                 case ExprKind::OP_EXPR:
//                     os << tokenNames[static_cast<int>(t.attr.exprAttr.op)] << " ";
//                     break;
//                 case ExprKind::CONST_EXPR:
//                     os << t.attr.exprAttr.val << " ";
//                     break;
//                 case ExprKind::ID_EXPR:
//                 case ExprKind::CALL_EXPR:
//                 case ExprKind::ARRAY_EXPR:
//                     os << (t.attr.exprAttr.name ? t.attr.exprAttr.name : "null") << " ";
//                     break;
//                 default:
//                     break;
//             }
//             break;
//         case PARAM:
//             os << paramKindNames[static_cast<int>(t.kind.param)] << " ";
//             os << (t.attr.dclAttr.name ? t.attr.dclAttr.name : "null") << " "
//                << exprTypeNames[static_cast<int>(t.attr.dclAttr.type)] << " "
//                << t.attr.dclAttr.size << " ";
//             break;
//         case DCL:
//             os << dclKindNames[static_cast<int>(t.kind.dcl)] << " ";
//              break;
//         case NULL_ND:
//         default:
//             os << "UNKNOWN_NODE ";
//             break;
//     }

//     os << std::endl;
//     spaceNum += 2;
//     for (int i = 0; i < MAX_CHILDREN; ++i) {
//         if (t.child[i]) {
//             os << *(t.child[i]);
//         }
//     }
//     spaceNum -= 2;
//     if (t.rSibling) {
//         os << *(t.rSibling);
//     }
//     return os;
// }
       


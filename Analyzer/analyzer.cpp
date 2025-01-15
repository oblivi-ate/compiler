#include <cstring>
#include <stdexcept>
#include <algorithm>
#include "analyzer.h"

using namespace std::string_literals;


void SemanticAnalyser::reportError(int line, const char *msg)
{
    fprintf(stderr, "Line %d:\x001b[31mERROR\x001b[0m: %s\n", line, msg);
    _error = true;
}

void SemanticAnalyser::reportError(int line, const std::string &msg)
{
    fprintf(stderr, "Line %d:\x001b[31mERROR\x001b[0m: %s\n", line, msg.c_str());
    _error = true;
}

SemanticAnalyser::SemanticAnalyser(TreeNode *tr)
{
    parseTree = tr;
    symTable = genSymbolTableTop();
}

SymbolTable *SemanticAnalyser::getSymbolTable()
{
    return symTable;
}

SymbolTable *SemanticAnalyser::genSymbolTableTop()
{
    SymbolTable *st = new SymbolTable(nullptr);
    genSymbolTable(parseTree, st);
    return st;
}

void SemanticAnalyser::genSymbolTable(TreeNode *tr, SymbolTable *st)
{
    if (tr == nullptr)
    {
        return;
    }

    if (tr->nodekind == NodeKind::DCL)
    {
        if (tr->kind.dcl == DclKind::FUN_DCL)
        {
            st->add_symbol(tr->attr.dclAttr.name, "function", tr);
        }
        else if (tr->kind.dcl == DclKind::VAR_DCL)
        {
            st->add_symbol(tr->attr.dclAttr.name, "variable", tr);
        }
        else if (tr->kind.dcl == DclKind::ARRAY_EXPR_DCL)
        {
            st->add_symbol(tr->attr.dclAttr.name, "array", tr);
        }
    }
    else{
        return;
    }


    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        genSymbolTable(tr->child[i], st);
    }

    genSymbolTable(tr->rSibling, st);
}

SymbolTable *SemanticAnalyser::genSymbolTable()
{
    SymbolTable *st = new SymbolTable(nullptr);
    genSymbolTable(parseTree, st);
    return st;
}


bool SemanticAnalyser::error()
{
    return _error;
}
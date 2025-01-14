#include <cstring>
#include <stdexcept>
#include <algorithm>
#include "analyzer.h"

using namespace std::string_literals;

using std::make_shared;

void SemanticAnalyser::reportError(int line, const char* msg)
{
	fprintf(stderr, "Line %d:\x001b[31mERROR\x001b[0m: %s\n", line, msg);
	_error = true;
}

void SemanticAnalyser::reportError(int line, const std::string &msg)
{
	fprintf(stderr, "Line %d:\x001b[31mERROR\x001b[0m: %s\n", line, msg.c_str());
	_error = true;
}

SemanticAnalyser::SemanticAnalyser(TreeNode* tr)
{
    parseTree = tr;
    symTable = genSymbolTableTop();
}

SymbolTable* SemanticAnalyser::getSymbolTable()
{
    return symTable;
}

SymbolTable* SemanticAnalyser::genSymbolTableTop()
{
    SymbolTable* st = new SymbolTable(nullptr);
    genSymbolTable(parseTree, st);
    return st;
}


void SemanticAnalyser::assignTypes()
{
	assignTypes(parseTree);
}

bool SemanticAnalyser::error()
{
	return _error;
}
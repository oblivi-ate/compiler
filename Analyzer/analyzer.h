#ifndef S_ANALYSER_H
#define S_ANALYSER_H

#include "parsetree.h"
#include "symbol_table.h"

class SemanticAnalyser
{
private:
	TreeNode* parseTree; // this will be modified by Analyser
	SymbolTable* symTable;

	bool _error = false;
	
	SymbolTable* genSymbolTableTop();

	void genSymbolTable(TreeNode* tr, SymbolTable* st);

	SymbolTable* genSymbolTable();
	int assignTypes(TreeNode *tr);

	void reportError(int line, const char* msg);
	void reportError(int line, const std::string& msg);
public:
	SemanticAnalyser(TreeNode*);

	SymbolTable* getSymbolTable();
	void assignTypes();

	bool error();

};

#endif
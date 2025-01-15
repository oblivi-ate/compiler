#include "symbol_table.h"
#include "analyzer.h"
#include "parser.h"

#include <iostream>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    TreeNode* root = new TreeNode();
    doParser(argc, argv, root);
    if (!root) {
        std::cerr << "Error: Unable to parse file." << std::endl;
        return 1;
    }

    SemanticAnalyser analyser(root);
    SymbolTable* symTable = analyser.getSymbolTable();

    //print the symbol table
    symTable->print_table();

    // Check for errors
    if (analyser.error()) {
        std::cerr << "Semantic analysis failed." << std::endl;
        return 1;
    }

    std::cout << "Semantic analysis succeeded." << std::endl;
    return 0;
    


}
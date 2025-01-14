### Group member contribution
YU QIANSHUO : symbol_table(.cpp & .h) <br>
WANG KUN, ZHENG AO: analyzer(.cpp & .h) <br>


#### Compile
gcc version 8.0 or above is required for the code to compile. <br>
` g++ token.cpp dfa.cpp scanner.cpp parser.cpp parsetree.cpp parser_print.cpp symbol_table.cpp analyzer.cpp driver.cpp -o parser -std=c++17 `

#### Run
`./parser test.pyc`


### Analyzer Summary

#### symbol_table.cpp
symbol_table.cpp implements the functionality for managing a symbol table in a compiler. It includes the implementation of the PycSymRef and PycSymbol classes, which represent references to symbols and the symbols themselves, respectively. The SymbolTable class manages a hierarchy of symbol tables, allowing for the lookup of symbols by name. The lookup method searches for a symbol in the current table and, if not found, recursively searches in the parent table. The check_local method checks if a symbol is defined in the current table. The print_table method prints the contents of the symbol table, including symbol names, types, declaration lines, and reference lines, in a formatted manner. This functionality is crucial for semantic analysis in the compiler, ensuring that symbols are correctly identified and their scopes are managed.

#### symbol_table.h
symbol_table.h defines the symbol table for a compiler. It includes PycSymRef for tracking symbol references and PycSymbol for representing symbols with their declaration, type, and references. The SymbolTable manages a hierarchy of tables, enabling scoped symbol lookups with lookup and check_local methods. It also features print_table for debugging. This header is essential for semantic analysis, ensuring proper symbol management during compilation.

#### s_analyzer.cpp
s_analyser.cpp implements semantic analysis for a compiler. It includes the SemanticAnalyser class, which performs tasks such as error reporting, symbol table generation, and type assignment. The reportError method logs errors with line numbers. The genSymbolTableTop and genSymbolTable methods create and populate the symbol table, handling declarations, function definitions, and parameter lists. The assignTypes method traverses the abstract syntax tree (AST) to assign types to expressions, ensuring type consistency and reporting mismatches. This class is essential for ensuring that the program's syntax adheres to semantic rules, preparing the AST for further processing like code generation.

#### s_analyzer.h
s_analyser.h defines the SemanticAnalyser class for performing semantic analysis in a compiler. It includes methods for generating a symbol table, assigning types to expressions, and reporting errors. The genSymbolTableTop and genSymbolTable methods create and populate the symbol table, handling declarations and function definitions. The assignTypes method traverses the abstract syntax tree (AST) to ensure type consistency, reporting mismatches. The reportError methods log errors with line numbers. This class is crucial for validating the program's syntax against semantic rules and preparing the AST for code generation.

#### sa_driver.cpp
sa_driver.cpp is the main driver for the semantic analysis phase of a compiler. It integrates the scanner, parser, and semantic analyzer. The run_scanner_and_parser function checks if the input file exists, runs the scanner to tokenize the file, and processes the token list to handle newlines. It then uses the Parser to parse the tokens into an abstract syntax tree (AST). If parsing succeeds, it initializes a SemanticAnalyser to generate the symbol table and assign types to expressions. It prints the symbol table and the AST if no errors occur. The main function either takes a filename from the command line or prompts the user for input. This file orchestrates the flow from source code to a semantically analyzed AST.


### Group member contribution
YU QIANSHUO : parsetree(.cpp & .h) <br>
WANG KUN, ZHENG AO: parser(.cpp & .h), parser_print(.cpp & .h) <br>

#### Compile
`g++ token.cpp dfa.cpp scanner.cpp parser.cpp parsetree.cpp parser_print.cpp -o parser -std=c++17`

#### Run
`./parser test_correct.pyc`
`./parser test_wrong.pyc`



### parser Summary
#### some parser tree sections in parser.cpp
The parser tree section of the parser.cpp file uses the TreeNode structure to construct the Abstract Syntax Tree (AST). Functions like program, declaration_list, and declaration parse source code recursively, creating and linking nodes. The expect function handles errors, while the main function starts parsing and outputs the results in preparation for semantic analysis and code generation.
#### File: parsetree.cpp
This file implements the TreeNode class, which represents nodes in the Abstract Syntax Tree (AST). It includes constructors and destructors for managing node properties and memory. The file also overloads the << operator to print node information, facilitating debugging and verification of the AST structure.
#### File: parsetree.h
This header file defines the TreeNode structure and related enums (NodeKind, StmtKind, ExprKind, ParamKind, DclKind, ExprType). It sets up the structure for AST nodes, including node type, child pointers, and attributes, providing the foundation for building and managing the AST.
#### parser.cpp
parser.cpp implements the construction of the Abstract Syntax Tree (AST) for a compiler. It defines the TreeNode structure to represent nodes in the AST, each containing properties like node type, child pointers, and line numbers. The file includes a series of recursive parsing functions such as program, declaration_list, and declaration to parse source code and build the AST by creating and linking nodes. The check function verifies if the current token matches the expected type, while the expect function throws errors for unexpected tokens. The main function initiates parsing by calling program and outputs whether the parsing was successful, setting the stage for subsequent semantic analysis and code generation.
#### parser.h
parser.h is a header file that declares functions used for parsing source code and constructing an Abstract Syntax Tree (AST). It includes declarations for functions like program, declaration_list, declaration, and others, which are defined in parser.cpp. These functions handle different parts of the source code, from top-level program structures to individual statements and expressions. The file also includes the necessary header parsetree.h to access the TreeNode structure and related enums. This setup allows the compiler to parse the source code recursively, building the AST by creating and linking TreeNode instances, which are essential for subsequent semantic analysis and code generation.
#### parser_print.cpp
parser_print.cpp implements functionality to print the Abstract Syntax Tree (AST) in a readable format. It includes a function print_tree that recursively traverses the AST, printing each node's details such as node type, expression type, and relevant attributes like variable names or operator types. The output is indented to reflect the tree structure, making it easier to visualize the hierarchy of the source code's syntax elements. This tool is invaluable for debugging and verifying the correctness of the parsing process in compiler development.
#### parser_print.h
parser_print.h is a header file that provides functionality for printing the Abstract Syntax Tree (AST). It includes the declaration of the print_tree function, which is used to print the AST nodes in a structured format. Additionally, it defines a helper function token_type_to_string that converts token types to their corresponding string representations. This function is useful for printing operator tokens in a human-readable form. The header ensures that the TreeNode structure is included, which is necessary for the print_tree function to access and print the AST nodes. This file is essential for debugging and visualizing the AST during the development of a compiler.





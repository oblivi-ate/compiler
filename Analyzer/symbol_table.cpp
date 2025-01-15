// symbol_table.cpp

#include "symbol_table.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

//define posttable
PycNode::PycNode(char type, int pos)
    : nodeType(type), nextPostable(nullptr), next(nullptr)
{
    nextPostable = new Postable{nullptr, pos};
}

// define symbol table
Symbol_table::Symbol_table(char somethingChar)
    : table(nullptr), something(somethingChar), next(nullptr)
{
}

//hash function
unsigned long hash_function(char nodeType) {
    unsigned long hash = 5381;
    hash = ((hash << 5) + hash) + nodeType; // hash * 33 + nodeType
    return hash;
}

// get hash value
unsigned long get_symbol_hash(char nodeType) {
    return hash_function(nodeType);
}

// print table
void traverse_symbol_table(const Symbol_table* table) {
    while (table != nullptr) {
        std::cout << "Symbol Table: " << table 
                  << ", Something: " << table->something << std::endl;
        const PycNode* node = table->table;
        while (node != nullptr) {
            std::cout << "\tPycNode: nodeType: " << node->nodeType << std::endl;
            const Postable* post = node->nextPostable;
            while (post != nullptr) {
                std::cout << "\t\tPostable: table: " << post->table 
                          << ", position: " << post->position << std::endl;
                post = post->table;
            }
            node = node->next;
        }
        table = table->next;
    }
}

// add new node
void add_symbol(Symbol_table* table, char nodeType, int position) {
    if (table == nullptr) {
        std::cerr << "Error: Symbol table is NULL." << std::endl;
        return;
    }

 
    PycNode* newNode = new PycNode(nodeType, position);
    if (!newNode) {
        std::cerr << "Error: Unable to allocate memory for PycNode." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    newNode->next = table->table;
    table->table = newNode;
}
//create new table
Symbol_table* create_symbol_table(char somethingChar) {
    Symbol_table* table = new Symbol_table(somethingChar);
    if (!table) {
        std::cerr << "Error: Unable to allocate memory for Symbol_table." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return table;
}
//destroy table
void destroy_symbol_table(Symbol_table* table) {
    while (table != nullptr) {
        PycNode* node = table->table;
        while (node != nullptr) {
            Postable* post = node->nextPostable;
            while (post != nullptr) {
                Postable* tempPost = post;
                post = post->table;
                delete tempPost;
            }
            PycNode* tempNode = node;
            node = node->next;
            delete tempNode;
        }
        Symbol_table* tempTable = table;
        table = table->next;
        delete tempTable;
    }
}

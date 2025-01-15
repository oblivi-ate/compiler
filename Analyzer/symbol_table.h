#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <list>
#include <unordered_map>
#include <memory>
#include <string>
#include "parsetree.h"
struct Postable{
    struct Postable* table;
    int position;
};

struct PycNode {
    char nodeType;
    struct Postabel *next
    struct PycNode* next;
} ;

struct Symbol_table{
    struct PycNode* table;
    char something;
    struct Symbol_tabel* next;
};

unsigned long hash_function(char nodeType);

void traverse_symbol_table(struct Symbol_table* table);

void add_symbol(struct Symbol_table* table, char nodeType, int position);

unsigned long get_symbol_hash(char nodeType);

Symbol_table* create_symbol_table(char somethingChar);

void destroy_symbol_table(Symbol_table* table);


#endif 

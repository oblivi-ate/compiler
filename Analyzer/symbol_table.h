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


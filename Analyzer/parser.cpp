#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>


unsigned long hash_function(char nodeType) {
    unsigned long hash = 5381;
    hash = ((hash << 5) + hash) + nodeType; 
    return hash;
}


void traverse_symbol_table(struct Symbol_table* table) {
    while (table != NULL) {
        printf("Symbol Table: %p, something: %c\n", (void*)table, table->something);
        struct PycNode* node = table->table;
        while (node != NULL) {
            printf("\tPycNode: nodeType: %c\n", node->nodeType);
            struct Postable* post = node->nextPostable;
            while (post != NULL) {
                printf("\t\tPostable: table: %p, position: %d\n", (void*)post->table, post->position);
                post = post->table;
            }
            node = node->next;
        }
        table = table->next;
    }
}


unsigned long get_symbol_hash(char nodeType) {
    return hash_function(nodeType);
}


void add_symbol(struct Symbol_table* table, char nodeType, int position) {
    if (table == NULL) return;

  
    struct PycNode* newNode = (struct PycNode*)malloc(sizeof(struct PycNode));
    if (!newNode) {
        fprintf(stderr, "Error: Unable to allocate memory for PycNode.\n");
        exit(EXIT_FAILURE);
    }
    newNode->nodeType = nodeType;
    newNode->nextPostable = NULL;
    newNode->next = table->table;
    table->table = newNode;


    struct Postable* newPostable = (struct Postable*)malloc(sizeof(struct Postable));
    if (!newPostable) {
        fprintf(stderr, "Error: Unable to allocate memory for Postable.\n");
        exit(EXIT_FAILURE);
    }
    newPostable->table = NULL;
    newPostable->position = position;

    
    newNode->nextPostable = newPostable;
}


struct Symbol_table* create_symbol_table(char something) {
    struct Symbol_table* table = (struct Symbol_table*)malloc(sizeof(struct Symbol_table));
    if (!table) {
        fprintf(stderr, "Error: Unable to allocate memory for Symbol_table.\n");
        exit(EXIT_FAILURE);
    }
    table->table = NULL;
    table->something = something;
    table->next = NULL;
    return table;
}




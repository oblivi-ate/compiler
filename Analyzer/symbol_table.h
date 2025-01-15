#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <string>
#include <list>
#include <unordered_map>
#include <memory>
#include "parsetree.h"


struct SymbolTable;
struct PycSymbol;
struct PycSymRef;


struct PycSymRef {
    PycSymbol* parent;       
    TreeNode* refNode;       
    PycSymRef* next;         

    PycSymRef(PycSymbol* p, TreeNode* rN)
        : parent(p), refNode(rN), next(nullptr) {}
};


struct PycSymbol {
    SymbolTable* parent;     
    PycSymRef* refs;          
    TreeNode* declNode;    
    std::string typeName;    
    PycSymbol* next;     

    PycSymbol(SymbolTable* p, TreeNode* dN, const std::string& type)
        : parent(p), refs(nullptr), declNode(dN), typeName(type), next(nullptr) {}
};


struct SymbolTable {
private:
    static int internal_id; 

public:
    const int id;            
    TreeNode* tableNode;   

  
    std::shared_ptr<SymbolTable> lower;   
    std::shared_ptr<SymbolTable> next;    
    std::weak_ptr<SymbolTable> upper;     

   
    PycSymbol* symbols;

   
    SymbolTable(TreeNode* t);
    SymbolTable(TreeNode* t, int id);

  
    void add_symbol(const std::string& name, const std::string& type, TreeNode* declNode);

   
    PycSymbol* lookup(const std::string& name);

    bool check_local(const std::string& name);

  
    void print_table();
};


struct SymbolTableManager {
    std::shared_ptr<SymbolTable> current;  
    std::shared_ptr<SymbolTable> global;  
};


extern SymbolTableManager symTableManager;

#endif 

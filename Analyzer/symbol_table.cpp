#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "symbol_table.h"


int SymbolTable::internal_id = 0;


SymbolTableManager symTableManager = { nullptr, nullptr };


SymbolTable::SymbolTable(TreeNode* t)
    : id(internal_id++), tableNode(t), symbols(nullptr)
{
   
    if (symTableManager.current) {
        upper = symTableManager.current;
    } else {
        upper.reset(); 
    }
    lower = nullptr;
    next = nullptr;

    symTableManager.current = std::make_shared<SymbolTable>(*this);
}

SymbolTable::SymbolTable(TreeNode* t, int id)
    : id(id), tableNode(t), symbols(nullptr)
{
    internal_id = id + 1;
    if (symTableManager.current) {
        upper = symTableManager.current;
    } else {
        upper.reset();
    }
    lower = nullptr;
    next = nullptr;

    symTableManager.current = std::make_shared<SymbolTable>(*this);
}


void SymbolTable::add_symbol(const std::string& name, const std::string& type, TreeNode* declNode) {
    if (check_local(name)) {
        std::cerr << "Error: Symbol '" << name << "' already defined in the current scope." << std::endl;
        return;
    }

    PycSymbol* newSym = new PycSymbol(this, declNode, type);
    newSym->next = symbols;
    symbols = newSym;
}


PycSymbol* SymbolTable::lookup(const std::string& name) {
 
    PycSymbol* sym = symbols;
    while (sym) {
        if (sym->typeName == name) { 
            return sym;
        }
        sym = sym->next;
    }

    
    if (!upper.expired()) {
        std::shared_ptr<SymbolTable> upperShared = upper.lock();
        return upperShared->lookup(name);
    } else {
        throw std::invalid_argument("Symbol '" + name + "' not found!");
    }
}

bool SymbolTable::check_local(const std::string& name) {
    PycSymbol* sym = symbols;
    while (sym) {
        if (sym->typeName == name) {
            return true;
        }
        sym = sym->next;
    }
    return false;
}


void SymbolTable::print_table() {
    static bool headerPrinted = false;
    if (!headerPrinted) {
        std::cout << std::left
                  << std::setw(10) << "TableID"
                  << std::setw(20) << "Name"
                  << std::setw(20) << "Type"
                  << std::setw(10) << "DeclLine"
                  << "Refs" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        headerPrinted = true;
    }

    std::cout << std::left
              << std::setw(10) << id;

    PycSymbol* sym = symbols;
    while (sym) {
        std::cout << std::left
                  << std::setw(20) << sym->typeName
                  << std::setw(20) << sym->typeName  
                  << std::setw(10) << (sym->declNode ? sym->declNode->line : -1);

     
        PycSymRef* ref = sym->refs;
        while (ref) {
            std::cout << ref->refNode->line << " ";
            ref = ref->next;
        }
        std::cout << std::endl;

        sym = sym->next;
    }

    
    if (lower) {
        lower->print_table();
    }

 
    if (next) {
        next->print_table();
    }
}

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "symbol_table.h"

// 符号类型名称数组，用于打印符号类型
static std::string exprTypeNames[] = {
    "TBD", "Void", "Int"
};

// PycSymRef 构造函数实现
PycSymRef::PycSymRef(PycSymbol* p, TreeNode* rN)
    : parent(p), refNode(rN)
{
}

// PycSymbol 构造函数实现
PycSymbol::PycSymbol(SymbolTable* p, TreeNode* dN, const std::string& type)
    : parent(p), declNode(dN), typeName(type)
{
}

// 初始化静态成员变量
int SymbolTable::internal_id = 0;

// SymbolTable 构造函数实现（自动分配 ID）
SymbolTable::SymbolTable(TreeNode* t)
    : tableNode(t), id(internal_id++), lower(nullptr), next(nullptr), nextAttachPoint(&lower)
{
}

// SymbolTable 构造函数实现（指定 ID）
SymbolTable::SymbolTable(TreeNode* t, int id)
    : tableNode(t), id(id), lower(nullptr), next(nullptr), nextAttachPoint(&lower)
{
    SymbolTable::internal_id = id + 1;
}

// 查找符号实现
PycSymbol& SymbolTable::lookup(const std::string& name)
{
    auto it = hashTable.find(name);
    if (it == hashTable.end())
    {
        if (auto upperShared = upper.lock())
        {
            return upperShared->lookup(name);
        }
        else
        {
            throw std::invalid_argument("Symbol \"" + name + "\" not found!");
        }
    }
    return it->second;
}

// 检查当前符号表是否包含指定符号
bool SymbolTable::check_local(const std::string& name)
{
    return hashTable.find(name) != hashTable.end();
}

// 打印符号表内容实现
void SymbolTable::print_table()
{
    static int flag = 0;
    if (!flag)
    {
        std::cout << std::left
                  << std::setw(6) << "Table"
                  << std::setw(20) << "Name"
                  << std::setw(20) << "Type"
                  << std::setw(5) << "Decl"
                  << "Refs" << std::endl;
        std::cout << std::left
                  << std::setw(6) << "ID"
                  << std::setw(20) << ""
                  << std::setw(20) << ""
                  << std::setw(5) << "Line"
                  << "Lines" << std::endl;
        std::cout << std::left
                  << std::setw(6) << "----"
                  << std::setw(20) << "----"
                  << std::setw(20) << "----"
                  << std::setw(5) << "----"
                  << "----" << std::endl;
    }

    std::string idField = std::to_string(id);
    for (const auto& [name, sym] : hashTable)
    {
        std::cout << std::left
                  << std::setw(6) << idField
                  << std::setw(20) << name
                  << std::setw(20) << sym.typeName;

        TreeNode* tr = sym.declNode;
        std::cout << std::setw(5) << tr->line << " ";

        // 打印引用行号
        for (const auto& ref : sym.refs)
        {
            std::cout << ref.refNode->line << " ";
        }
        std::cout << std::endl;
    }

    flag++;
    if (lower)
    {
        lower->print_table();
    }
    if (next)
    {
        next->print_table();
    }
    flag--;
}


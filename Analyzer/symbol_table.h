#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <list>
#include <unordered_map>
#include <memory>
#include <string>
#include "parsetree.h"

struct PycSymbol;

// 符号引用结构体，记录符号被引用的位置
struct PycSymRef
{
    PycSymbol* parent;   // 指向对应的符号
    TreeNode* refNode;   // 引用发生的语法树节点

    PycSymRef(PycSymbol* p, TreeNode* rN);
};

// 符号引用列表类型别名
using PycSymRefList = std::list<PycSymRef>;

// 符号表前向声明
struct SymbolTable;

// 符号结构体，表示一个符号（变量、函数等）
struct PycSymbol
{
    SymbolTable* parent;     // 所属的符号表
    PycSymRefList refs;      // 符号引用列表
    TreeNode* declNode;      // 符号的声明节点
    std::string typeName;    // 符号的类型名称

    PycSymbol(SymbolTable* p, TreeNode* dN, const std::string& type);

    // 添加符号引用
    template <class... Types>
    void emplace_ref(Types... args)
    {
        refs.emplace_back(this, std::forward<Types>(args)...);
    }
};

// 符号表结构体
struct SymbolTable
{
private:
    static int internal_id;  // 静态变量，用于生成唯一的符号表 ID

public:
    const int id;             // 符号表的唯一标识符，构造后不可修改
    TreeNode* tableNode;     // 符号表对应的语法树节点

    // 树状结构相关指针
    std::shared_ptr<SymbolTable>* nextAttachPoint;
    std::shared_ptr<SymbolTable> lower, next;
    std::weak_ptr<SymbolTable> upper, prev;

    // 哈希表，存储符号名与符号的映射
    std::unordered_map<std::string, PycSymbol> hashTable;

    // 构造函数
    SymbolTable(TreeNode* t);
    SymbolTable(TreeNode* t, int id);

    // 添加新符号
    template <class... Types>
    void emplace_symbol(const std::string& name, const std::string& type, Types... args)
    {
        hashTable.emplace(name, PycSymbol(this, std::forward<Types>(args)..., type));
    }

    // 查找符号，支持作用域链查找
    PycSymbol& lookup(const std::string& name);

    // 检查当前符号表是否包含指定符号
    bool check_local(const std::string& name);

    // 打印符号表内容，便于调试
    void print_table();
};

// 共享符号表类型别名
using SharedSymbolTable = std::shared_ptr<SymbolTable>;

#endif // SYMBOL_TABLE_H_


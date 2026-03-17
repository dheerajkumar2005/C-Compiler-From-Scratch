#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

#include "SemanticError.hpp"
#include "utils.hpp"

enum class Entry_Kind
{
    VARIABLE,
    FUNCTION,
    PARAMETER,
};

// std::ostream &operator<<(std::ostream &os, const Entry_Kind &ek);

enum class Scope_Kind
{
    GLOBAL,
    FUNCTION,
    // later: BLOCK
};

// std::ostream &operator<<(std::ostream &os, const Scope_Kind &sk);

struct Func_Signature
{
    std::string name;
    Type return_type;
    std::vector<Type> param_types;

    std::vector<std::string> param_names;

    Func_Signature(const std::string &name, Type return_type);

    void add_param(const std::string &param_name, Type type);

    bool operator==(const Func_Signature &other) const;
    bool operator!=(const Func_Signature &other) const;
};

struct Symbol_Table_Entry
{

    Entry_Kind kind;
    Type type;
    int size;
    int offset;
    Func_Signature *func_sig; // nullptr for non-functions

    Symbol_Table_Entry(Entry_Kind kind, Type type, Func_Signature *func_sig = nullptr);
};

struct Scope
{
    Scope_Kind kind;
    Scope *parent_scope;

    std::unordered_map<std::string, Symbol_Table_Entry *> sym_tab;
    Func_Signature *func_sig; // nullptr for non-functions

    Scope(Scope_Kind kind, Scope *parent_scope = nullptr, Func_Signature *func_sig = nullptr);
};

#endif

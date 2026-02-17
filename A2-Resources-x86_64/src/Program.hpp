#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

#include "Ast.hpp"
#include "SemanticError.hpp"

class Symbol_Table
{
public:
    std::unordered_map<std::string, Type> sym_tab;
};

class Func_Signature
{
public:
    std::string name;
    Type return_type;
    std::vector<Type> param_types;

    Func_Signature(const std::string &name, Type return_type);
    void add_param(Type type);

    bool operator==(const Func_Signature &other) const;
    bool operator!=(const Func_Signature &other) const;
};

class Func_Table
{
public:
    std::unordered_map<std::string, Procedure *> func_tab;
    // void insert_decl(const std::string &id, Func_Signature *func_sig);
};

class Scope
{
public:
    Symbol_Table symbol_table;
    Scope *parent_scope;

    Scope(Scope *parent_scope = nullptr);
    virtual ~Scope() = 0;
};

class Procedure : public Scope
{
public:
    Func_Signature *func_signature;
    std::vector<Ast *> body;
    Procedure(Scope *parent_scope = nullptr, Func_Signature *func_signature = nullptr);
};

class Program : public Scope
{
public:
    Func_Table func_table;
};

#endif

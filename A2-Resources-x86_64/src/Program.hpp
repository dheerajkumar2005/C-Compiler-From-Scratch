// TODO: Write constructors

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
    std::unordered_map<std::string, Type> sym_tab;

public:
    void insert(const std::string &id, Type type);
};

class Func_Signature
{
public:
    std::string name;
    Type return_type;
    std::vector<Type> param_types;

    Func_Signature(const std::string &name, Type return_type);
};

class Func_Table
{
public:
    std::unordered_map<std::string, Func_Signature *> func_tab;
    void insert(const std::string &id, Func_Signature* func_sig);
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

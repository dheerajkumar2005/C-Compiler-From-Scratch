// TODO: Write constructors

#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

#include "Ast.hpp"

class Symbol_Table
{
    std::unordered_map<std::string, Type> m;
};

class Func_Signature
{
    std::string name;
    Type return_type;
    std::vector<Type> param_types;
};

class Func_Table
{
    std::unordered_map<std::string, Func_Signature *> m;
};

class Scope
{
protected:
    Symbol_Table symbol_table;
    Scope *parent_scope;

public:
    virtual ~Scope() = 0;
};

class Procedure : public Scope
{
protected:
    Func_Signature *func_signature;
    std::vector<Ast *> body;

public:
};

class Program : public Scope
{
protected:
    Func_Table func_table;
};

#endif

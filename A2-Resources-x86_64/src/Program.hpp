// TODO: Finish this file

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

class Scope
{
protected:
    Symbol_Table symbol_table;
    Scope *parent_scope;

public:
    // Scope(Symbol_Table symbol_table, Scope *parent_scope = nullptr);
    virtual ~Scope() = 0;
};

class Procedure : public Scope
{
protected:
    Type return_type;
    std::vector<Ast *> body;

    // For printing ONLY
    std::vector<std::pair<std::string, Type>> params;

public:
};

class Program : public Scope
{
    // std::map<std::string, Type> global_symbol_table;
    // std::map<std::string, Procedure *> procedures;
};

#endif

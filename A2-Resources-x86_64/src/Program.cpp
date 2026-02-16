#include "Program.hpp"

Scope::Scope(Scope *parent_scope)
    : symbol_table(), parent_scope(parent_scope)
{
}

Procedure::Procedure(Scope *parent_scope, Func_Signature *func_signature = nullptr)
    : Scope(parent_scope), func_signature(func_signature), body()
{
}

Func_Signature::Func_Signature(const std::string &name, Type return_type)
    : name(name), return_type(return_type), param_types()
{
}

void Symbol_Table::insert(const std::string &id, Type type)
{
    if (sym_tab.find(id) != sym_tab.end())
    {
        throw new SemanticError("Expected only one declaration per identifier: " + id);
    }
    sym_tab[id] = type;
}
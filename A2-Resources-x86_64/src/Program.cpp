#include "Program.hpp"

Scope::Scope(Scope *parent_scope)
    : symbol_table(), parent_scope(parent_scope)
{
}

Procedure::Procedure(Scope *parent_scope, Func_Signature *func_signature)
    : Scope(parent_scope), func_signature(func_signature), body()
{
}

Func_Signature::Func_Signature(const std::string &name, Type return_type)
    : name(name), return_type(return_type), param_types(), param_names()
{
}

void Func_Signature::add_param(const std::string &param_name, Type type)
{
    param_types.push_back(type);
    param_names.push_back(param_name);
}

bool Func_Signature::operator==(const Func_Signature &other) const
{
    return return_type == other.return_type && param_types == other.param_types;
}

bool Func_Signature::operator!=(const Func_Signature &other) const
{
    return !(*this == other);
}

ParserContext::ParserContext()
    : program_ptr(nullptr), func_ptr(nullptr)
{
}

Scope::~Scope()
{
}

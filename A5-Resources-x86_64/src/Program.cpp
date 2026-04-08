#include "Program.hpp"

// std::ostream &operator<<(std::ostream &os, const Entry_Kind &ek)
// {
//     if (ek == Entry_Kind::FUNCTION)
//     {
//         os << "FUNCTION";
//     }
//     else if (ek == Entry_Kind::PARAMETER)
//     {
//         os << "PARAMETER";
//     }
//     else if (ek == Entry_Kind::VARIABLE)
//     {
//         os << "VARIABLE";
//     }
//     else
//     {
//         throw_SemanticError("Got weird Entry_Kind");
//     }
//     return os;
// }

// std::ostream &operator<<(std::ostream &os, const Scope_Kind &sk)
// {
//     if (sk == Scope_Kind::FUNCTION)
//     {
//         os << "FUNCTION";
//     }
//     else if (sk == Scope_Kind::GLOBAL)
//     {
//         os << "GLOBAL";
//     }
//     else
//     {
//         throw_SemanticError("Got weird Scope_Kind");
//     }
//     return os;
// }

Symbol_Table_Entry::Symbol_Table_Entry(Entry_Kind _kind, Type _type)
    : kind(_kind), type(_type)
{
}

Symbol_Table_Entry::~Symbol_Table_Entry()
{
}

Data_Entry::Data_Entry(Entry_Kind _kind, Type _type, int _size, int _offset)
    : Symbol_Table_Entry(_kind, _type), size(_size), offset(_offset)
{
}

Scope::Scope(Scope_Kind kind, Scope *parent_scope, Func_Signature *func_sig)
    : param_offset(8), local_offset(0), kind(kind), parent_scope(parent_scope), func_sig(func_sig)
{
}

void Scope::add_param(Type param_type, const std::string &param_name)
{
    int param_size = get_size(param_type);

    // No other params with the same name
    if (sym_tab.find(param_name) != sym_tab.end())
    {
        throw_SemanticError("Param name matches previous param: " + param_name);
        return;
    }

    sym_tab[param_name] = new Data_Entry(Entry_Kind::PARAMETER, param_type, param_size, param_offset);
    param_offset += param_size;
}

void Scope::add_local(Type local_type, const std::string &local_name)
{
    int local_size = get_size(local_type);

    // Cannot clash with the function name
    if (kind == Scope_Kind::FUNCTION && local_name == func_sig->name)
    {
        throw_SemanticError("Local variable name matches function name: " + local_name);
        return;
    }

    // No local variable or params with the same name
    if (sym_tab.find(local_name) != sym_tab.end())
    {
        throw_SemanticError("Local variable name matches previously declared variable: " + local_name);
        return;
    }

    local_offset -= local_size;
    sym_tab[local_name] = new Data_Entry(Entry_Kind::VARIABLE, local_type, local_size, local_offset);
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

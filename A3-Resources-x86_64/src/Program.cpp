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

Symbol_Table_Entry::Symbol_Table_Entry(Entry_Kind kind, Type type, Func_Signature *func_sig)
    : kind(kind), type(type), size(0), offset(0), func_sig(func_sig)
{
}

std::string Symbol_Table_Entry::to_string() const{
    // Temp Hardcoded VAR and offset
    return "<"+type_to_string(type)+">"+" Entity Type: VAR (No offset assigned yet)";
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

Scope::Scope(Scope_Kind kind, Scope *parent_scope, Func_Signature *func_sig)
    : kind(kind), parent_scope(parent_scope), func_sig(func_sig)
{
}

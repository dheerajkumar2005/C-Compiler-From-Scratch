// TODO: Complete this file

#include "Ast.hpp"

Expression_Ast::Expression_Ast(Type type) : type(type)
{
}

Type Expression_Ast::get_type() const
{
    return type;
}

Base_Expr_Ast::Base_Expr_Ast(Type type) : Expression_Ast(type)
{
}

// Name_Expr_Ast::Name_Expr_Ast(char *var_name)
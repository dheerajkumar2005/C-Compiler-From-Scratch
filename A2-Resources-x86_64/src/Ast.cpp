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

Name_Expr_Ast::Name_Expr_Ast(std::string *id, Type type)
    : Base_Expr_Ast(type), var_name(*id)
{
}

Read_Stmt_Ast::Read_Stmt_Ast(Name_Expr_Ast *var)
    : var(var)
{
    // TODO: Semantic checks
}

Write_Stmt_Ast::Write_Stmt_Ast(Expression_Ast *expr)
    : expr(expr)
{
    // TODO: Semantic checks
}
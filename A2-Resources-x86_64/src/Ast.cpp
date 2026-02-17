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
    if(type == Type::VOID){
        throw_SemanticError("can't have void types in variable declarations");
    }
}

Read_Stmt_Ast::Read_Stmt_Ast(Name_Expr_Ast *var)
    : var(var)
{
    // TODO: Semantic checks
    if(var->get_type() != Type::INT && var->get_type() != Type::FLOAT){
        throw_SemanticError("can read only int or float");
    }
}

Write_Stmt_Ast::Write_Stmt_Ast(Expression_Ast *expr)
    : expr(expr)
{
    // TODO: Semantic checks
    Type type = expr->get_type();
    if(type == Type::VOID || type == Type::BOOL){
        throw_SemanticError("can't print bool or void");
    }

}

Int_Expr_Ast::Int_Expr_Ast(int ival)
    : Base_Expr_Ast(Type::INT), ival(ival)
{

}

Float_Expr_Ast::Float_Expr_Ast(float fval)
    : Base_Expr_Ast(Type::FLOAT), fval(fval)
{
}

String_Expr_Ast::String_Expr_Ast(char *_sval)
    : Base_Expr_Ast(Type::STR), sval(_sval)
{
}

Unary_Expr_AST::Unary_Expr_AST(Type type, Unary_Operator op, Expression_Ast *opd1)
    : Expression_Ast(type), op(op), opd1(opd1)
{
}

UMinus_Expr_Ast::UMinus_Expr_Ast(Expression_Ast *opd1)
    : Unary_Expr_AST(opd1->get_type(), Unary_Operator::NEGATE, opd1)
{
}

Binary_Expr_Ast::Binary_Expr_Ast(Type type, Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2)
    : Expression_Ast(type), op(op), opd1(opd1), opd2(opd2)
{
}

Boolean_Expr_Ast::Boolean_Expr_Ast(Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(Type::BOOL, op, opd1, opd2)
{
}

Div_Expr_Ast::Div_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::DIVIDE, opd1, opd2)
{
}

Minus_Expr_Ast::Minus_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::SUBTRACT, opd1, opd2)
{
}

Mult_Expr_Ast::Mult_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::MULTIPLY, opd1, opd2)
{
}

Plus_Expr_Ast::Plus_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::ADD, opd1, opd2)
{
}

Relational_Expr_Ast::Relational_Expr_Ast(Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(Type::BOOL, op, opd1, opd2)
{
}

Ternary_Expr_Ast::Ternary_Expr_Ast(Type type, Ternary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2, Expression_Ast *opd3)
    : Expression_Ast(type), op(op), opd1(opd1), opd2(opd2), opd3(opd3)
{
}

Conditional_Expr_Ast::Conditional_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2, Expression_Ast *opd3)
    : Ternary_Expr_Ast(Type::BOOL, Ternary_Operator::QUESTION_MARK_COLON, opd1, opd2, opd3)
{
}

Assignment_Stmt_Ast::Assignment_Stmt_Ast(Name_Expr_Ast *lhs, Expression_Ast *rhs)
    : Statement_Ast(), lhs(lhs), rhs(rhs)
{
}

Logical_Not_Expr_Ast::Logical_Not_Expr_Ast(Expression_Ast *opd1)
    : Unary_Expr_AST(Type::BOOL, Unary_Operator::LOGICAL_NOT, opd1)
{
}

Ast::~Ast()
{
}

Base_Expr_Ast::~Base_Expr_Ast()
{
}

Unary_Expr_AST::~Unary_Expr_AST()
{
}

Binary_Expr_Ast::~Binary_Expr_Ast()
{
}

Ternary_Expr_Ast::~Ternary_Expr_Ast()
{
}

Statement_Ast::~Statement_Ast() {}

void Name_Expr_Ast::print_ast() const
{
    std::cout << var_name << "_<" << get_type() << ">";
}

void Int_Expr_Ast::print_ast() const
{
    std::cout << "Num : " << ival << "<" << get_type() << ">";
}

void Float_Expr_Ast::print_ast() const
{
    std::cout << "Num : " << std::fixed << std::setprecision(2) << fval << "<" << get_type() << ">";
}

void String_Expr_Ast::print_ast() const
{
    std::cout << "String : \"" << sval << "\"<" << get_type() << ">";
}


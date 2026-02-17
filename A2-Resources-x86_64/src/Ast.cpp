// TODO: Complete this file

#include "Ast.hpp"

Ast::Ast()
{
}

Statement_Ast::Statement_Ast()
    : Ast()
{
}

Expression_Ast::Expression_Ast(Type type)
    : Ast(), type(type)
{
}

Type Expression_Ast::get_type() const
{
    return type;
}

Base_Expr_Ast::Base_Expr_Ast(Type type)
    : Expression_Ast(type)
{
}

Name_Expr_Ast::Name_Expr_Ast(std::string *id, Type type)
    : Base_Expr_Ast(type), var_name(*id)
{
    if (type == Type::VOID)
    {
        throw_SemanticError("can't have void types in variable declarations");
    }
}

Read_Stmt_Ast::Read_Stmt_Ast(Name_Expr_Ast *var)
    : Statement_Ast(), var(var)
{
    if (var->get_type() != Type::INT && var->get_type() != Type::FLOAT)
    {
        throw_SemanticError("can read only int or float");
    }
}

Write_Stmt_Ast::Write_Stmt_Ast(Expression_Ast *expr)
    : Statement_Ast(), expr(expr)
{
    // TODO: Semantic checks
    Type type = expr->get_type();
    if (type == Type::VOID || type == Type::BOOL)
    {
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
    if (opd1->get_type() != Type::INT && opd1->get_type() != Type::FLOAT)
    {
        throw_SemanticError("uminus can only have int or float argument");
    }
}

Binary_Expr_Ast::Binary_Expr_Ast(Type type, Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2)
    : Expression_Ast(type), op(op), opd1(opd1), opd2(opd2)
{
}

bool are_same_type(Expression_Ast *opd1, Expression_Ast *opd2)
{
    return opd1->get_type() == opd2->get_type();
}

bool is_numeric_type(Expression_Ast *opd)
{
    return opd->get_type() == Type::FLOAT || opd->get_type() == Type::INT;
}

Boolean_Expr_Ast::Boolean_Expr_Ast(Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(Type::BOOL, op, opd1, opd2)
{
    if (!are_same_type(opd1, opd2))
    {
        throw_SemanticError("Both operands should be of the same type");
    }
    if (opd1->get_type() != Type::BOOL)
    {
        throw_SemanticError("Operand of Boolean expr should be bool");
    }
}

Div_Expr_Ast::Div_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::DIVIDE, opd1, opd2)
{
    if (!are_same_type(opd1, opd2))
    {
        throw_SemanticError("Both operands should be of the same type");
    }
    else if (!is_numeric_type(opd1))
    {
        throw_SemanticError("Operand of arith expr should be int/float");
    }
}

Minus_Expr_Ast::Minus_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::SUBTRACT, opd1, opd2)
{
    if (!are_same_type(opd1, opd2))
    {
        throw_SemanticError("Both operands should be of the same type");
    }
    else if (!is_numeric_type(opd1))
    {
        throw_SemanticError("Operand of arith expr should be int/float");
    }
}

Mult_Expr_Ast::Mult_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::MULTIPLY, opd1, opd2)
{
    if (!are_same_type(opd1, opd2))
    {
        throw_SemanticError("Both operands should be of the same type");
    }
    else if (!is_numeric_type(opd1))
    {
        throw_SemanticError("Operand of arith expr should be int/float");
    }
}

Plus_Expr_Ast::Plus_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::ADD, opd1, opd2)
{
    if (!are_same_type(opd1, opd2))
    {
        throw_SemanticError("Both operands should be of the same type");
    }
    else if (!is_numeric_type(opd1))
    {
        throw_SemanticError("Operand of arith expr should be int/float");
    }
}

Relational_Expr_Ast::Relational_Expr_Ast(Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2)
    : Binary_Expr_Ast(Type::BOOL, op, opd1, opd2)
{
    if (op != Binary_Operator::LT && op != Binary_Operator::LE && op != Binary_Operator::GT && op != Binary_Operator::GE && op != Binary_Operator::NE && op != Binary_Operator::EQ)
    {
        throw_SemanticError("Relational expression ast needs comparison operator");
    }
    else if (!are_same_type(opd1, opd2))
    {
        throw_SemanticError("Both operands should be of the same type");
    }
    else if (!is_numeric_type(opd1))
    {
        throw_SemanticError("Operand of comparison expr should be int/float");
    }
}

Ternary_Expr_Ast::Ternary_Expr_Ast(Type type, Ternary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2, Expression_Ast *opd3)
    : Expression_Ast(type), op(op), opd1(opd1), opd2(opd2), opd3(opd3)
{
}

Conditional_Expr_Ast::Conditional_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2, Expression_Ast *opd3)
    : Ternary_Expr_Ast(opd2->get_type(), Ternary_Operator::QUESTION_MARK_COLON, opd1, opd2, opd3)
{
    if (opd1->get_type() != Type::BOOL)
    {
        throw_SemanticError("comparison part should be of type bool");
    }
    else if (opd2->get_type() != opd3->get_type())
    {
        throw_SemanticError("both parts should be of same type in ternary expr");
    }
    else if (opd2->get_type() == Type::VOID)
    {
        throw_SemanticError("can't have type void in expr");
    }
}

Assignment_Stmt_Ast::Assignment_Stmt_Ast(Name_Expr_Ast *lhs, Expression_Ast *rhs)
    : Statement_Ast(), lhs(lhs), rhs(rhs)
{
    if (lhs->get_type() != rhs->get_type())
    {
        throw_SemanticError("both sides of assign stmt should have same type");
    }
    if (lhs->get_type() == Type::VOID)
    {
        throw_SemanticError("cannot assign to type VOID");
    }
}

Logical_Not_Expr_Ast::Logical_Not_Expr_Ast(Expression_Ast *opd1)
    : Unary_Expr_AST(Type::BOOL, Unary_Operator::LOGICAL_NOT, opd1)
{
    if (opd1->get_type() != Type::BOOL)
    {
        throw_SemanticError("operand of logical not should be of type bool");
    }
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

Statement_Ast::~Statement_Ast()
{
}

Expression_Ast::~Expression_Ast()
{
}

std::ostream &operator<<(std::ostream &os, Type t)
{
    if (t == Type::VOID)
    {
        os << "void";
    }
    else if (t == Type::INT)
    {
        os << "int";
    }
    else if (t == Type::FLOAT)
    {
        os << "float";
    }
    else if (t == Type::STR)
    {
        os << "string";
    }
    else if (t == Type::BOOL)
    {
        os << "bool";
    }
    else
    {
        throw_SemanticError("Unexpected type");
    }
    return os;
}

std::string type_to_string(Type type)
{
    std::ostringstream oss;
    oss << type;
    return oss.str();
}

std::ostream &operator<<(std::ostream &os, Binary_Operator op)
{   
    if(op == Binary_Operator::ADD){
        os << "Plus";
    }
    else if(op == Binary_Operator::SUBTRACT){
        os << "Minus";
    }
    else if(op == Binary_Operator::MULTIPLY){
        os << "Mult";
    }
    else if(op == Binary_Operator::DIVIDE){
        os << "Div";
    }
    else if (op == Binary_Operator::LT)
    {
        os << "LT";
    }
    else if (op == Binary_Operator::LE)
    {
        os << "LE";
    }
    else if (op == Binary_Operator::GT)
    {
        os << "GT";
    }
    else if (op == Binary_Operator::GE)
    {
        os << "GE";
    }
    else if (op == Binary_Operator::NE)
    {
        os << "NE";
    }
    else if (op == Binary_Operator::EQ)
    {
        os << "EQ";
    }
    else if (op == Binary_Operator::LOGICAL_AND)
    {
        os << "AND";
    }
    else if (op == Binary_Operator::LOGICAL_OR)
    {
        os << "OR";
    }
    else
    {
        throw_SemanticError("Unexpected type");
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, Unary_Operator op){
    if( op == Unary_Operator::LOGICAL_NOT){
        os << "NOT";
    }
    else if(op == Unary_Operator::NEGATE){
        os << "Uminus";
    }
    else{
        throw_SemanticError("Unexpected type");
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, Ternary_Operator op){
    if( op == Ternary_Operator::QUESTION_MARK_COLON){
        os << "?";
    }
    else{
        throw_SemanticError("Unexpected type");
    }
}


std::string op_to_string(Binary_Operator op)
{
    std::ostringstream oss;
    oss << op;
    return oss.str();
}

std::string op_to_string(Unary_Operator op)
{
    std::ostringstream oss;
    oss << op;
    return oss.str();
}

std::string op_to_string(Unary_Operator op)
{
    std::ostringstream oss;
    oss << op;
    return oss.str();
}

std::string Name_Expr_Ast::to_string() const
{
    return "Name : " + var_name + "_<" + type_to_string(get_type()) + ">";
}

std::string Int_Expr_Ast::to_string() const
{
    return "Num : " + std::to_string(ival) + "<" + type_to_string(get_type()) + ">";
}

std::string Float_Expr_Ast::to_string() const
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << fval;
    return "Num : " + out.str() + "<" + type_to_string(get_type()) + ">";
}

std::string String_Expr_Ast::to_string() const
{
    return "String : " + sval + "<" + type_to_string(get_type()) + ">";
}

std::string UMinus_Expr_Ast::to_string() const
{
    return "Arith: Uminus<"+type_to_string(get_type())+">\nL_Opd ("+opd1->to_string()+")";
}

std::string Logical_Not_Expr_Ast::to_string() const
{
    return "Condition: NOT<bool>\nL_Opd ("+ opd1->to_string()+")";   
}

std::string Boolean_Expr_Ast::to_string() const
{  
    return "Condition: "++"<bool>\nL_Opd ("+ opd1->to_string()+")"; 
}

std::string Div_Expr_Ast::to_string() const
{
}

std::string Minus_Expr_Ast::to_string() const
{
}

std::string Mult_Expr_Ast::to_string() const
{
}

std::string Plus_Expr_Ast::to_string() const
{
}

std::string Relational_Expr_Ast::to_string() const
{
}

std::string Conditional_Expr_Ast::to_string() const
{
    // return "\nCondition: " + 
}

std::string Assignment_Stmt_Ast::to_string() const
{
    return "Asgn:\nLHS (" + lhs->to_string() + ")\nRHS (" + rhs->to_string() + ")";
}

std::string Read_Stmt_Ast::to_string() const
{
    return "Read: " + var->to_string();
}

std::string Write_Stmt_Ast::to_string() const
{
    return "Write: " + expr->to_string();
}

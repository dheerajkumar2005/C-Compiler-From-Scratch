#include "utils.hpp"

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
    if (op == Binary_Operator::ADD)
    {
        os << "Plus";
    }
    else if (op == Binary_Operator::SUBTRACT)
    {
        os << "Minus";
    }
    else if (op == Binary_Operator::MULTIPLY)
    {
        os << "Mult";
    }
    else if (op == Binary_Operator::DIVIDE)
    {
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

std::ostream &operator<<(std::ostream &os, Unary_Operator op)
{
    if (op == Unary_Operator::LOGICAL_NOT)
    {
        os << "NOT";
    }
    else if (op == Unary_Operator::NEGATE)
    {
        os << "Uminus";
    }
    else
    {
        throw_SemanticError("Unexpected type");
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, Ternary_Operator op)
{
    if (op == Ternary_Operator::QUESTION_MARK_COLON)
    {
        os << "?";
    }
    else
    {
        throw_SemanticError("Unexpected type");
    }
    return os;
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

std::string op_to_string(Ternary_Operator op)
{
    std::ostringstream oss;
    oss << op;
    return oss.str();
}
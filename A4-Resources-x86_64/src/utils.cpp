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

std::ostream &operator<<(std::ostream &os, TAC_Operator op)
{
    if (op == TAC_Operator::LOGICAL_NOT)
    {
        os << "!";
    }
    else if (op == TAC_Operator::NEGATE)
    {
        os << "-";
    }
    else if (op == TAC_Operator::ADD)
    {
        os << "+";
    }
    else if (op == TAC_Operator::SUBTRACT)
    {
        os << "-";
    }
    else if (op == TAC_Operator::MULTIPLY)
    {
        os << "*";
    }
    else if (op == TAC_Operator::DIVIDE)
    {
        os << "/";
    }
    else if (op == TAC_Operator::LT)
    {
        os << "<";
    }
    else if (op == TAC_Operator::LE)
    {
        os << "<=";
    }
    else if (op == TAC_Operator::GT)
    {
        os << ">";
    }
    else if (op == TAC_Operator::GE)
    {
        os << ">=";
    }
    else if (op == TAC_Operator::NE)
    {
        os << "!=";
    }
    else if (op == TAC_Operator::EQ)
    {
        os << "==";
    }
    else if (op == TAC_Operator::LOGICAL_AND)
    {
        os << "&&";
    }
    else if (op == TAC_Operator::LOGICAL_OR)
    {
        os << "||";
    }
    else if (op == TAC_Operator::NOP)
    {
        throw_SemanticError("Did not expect NOP");
    }
    else
    {
        throw_SemanticError("Unexpected type");
    }
    return os;
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

std::ostream &operator<<(std::ostream &os, RTL_Operator op)
{
    if (op == RTL_Operator::NEGATE)
    {
        os << "uminus";
    }
    else if (op == RTL_Operator::LOGICAL_NOT)
    {
        os << "not";
    }
    else if (op == RTL_Operator::ADD)
    {
        os << "add";
    }
    else if (op == RTL_Operator::SUBTRACT)
    {
        os << "sub";
    }
    else if (op == RTL_Operator::MULTIPLY)
    {
        os << "mul";
    }
    else if (op == RTL_Operator::DIVIDE)
    {
        os << "div";
    }
    else if (op == RTL_Operator::LOGICAL_AND)
    {
        os << "and";
    }
    else if (op == RTL_Operator::LOGICAL_OR)
    {
        os << "or";
    }
    else if (op == RTL_Operator::LT)
    {
        os << "slt";
    }
    else if (op == RTL_Operator::LE)
    {
        os << "sle";
    }
    else if (op == RTL_Operator::GT)
    {
        os << "sgt";
    }
    else if (op == RTL_Operator::GE)
    {
        os << "sge";
    }
    else if (op == RTL_Operator::NE)
    {
        os << "sne";
    }
    else if (op == RTL_Operator::EQ)
    {
        os << "seq";
    }
    else
    {
        throw_SemanticError("Unexpected RTL Operator: " + op_to_string(op));
    }

    return os;
}

std::string op_to_string(TAC_Operator op)
{
    std::ostringstream oss;
    oss << op;
    return oss.str();
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

std::string op_to_string(RTL_Operator op)
{
    std::ostringstream oss;
    oss << op;
    return oss.str();
}

std::string op_float_to_string(RTL_Operator op)
{
    std::string result;
    if (op == RTL_Operator::NEGATE)
    {
        result = "uminus.d";
    }
    else if (op == RTL_Operator::LOGICAL_NOT)
    {
        result = "not";
    }
    else if (op == RTL_Operator::ADD)
    {
        result = "add.d";
    }
    else if (op == RTL_Operator::SUBTRACT)
    {
        result = "sub.d";
    }
    else if (op == RTL_Operator::MULTIPLY)
    {
        result = "mul.d";
    }
    else if (op == RTL_Operator::DIVIDE)
    {
        result = "div.d";
    }
    else if (op == RTL_Operator::LOGICAL_AND)
    {
        result = "and";
    }
    else if (op == RTL_Operator::LOGICAL_OR)
    {
        result = "or";
    }
    else if (op == RTL_Operator::LT)
    {
        result = "slt.d";
    }
    else if (op == RTL_Operator::LE)
    {
        result = "sle.d";
    }
    else if (op == RTL_Operator::EQ)
    {
        result = "seq.d";
    }
    else
    {
        throw_SemanticError("Unexpected RTL float Operator: " + op_to_string(op));
    }
    return result;
}

TAC_Operator binary_to_tac(Binary_Operator op)
{
    if (op == Binary_Operator::ADD)
    {
        return TAC_Operator::ADD;
    }
    else if (op == Binary_Operator::SUBTRACT)
    {
        return TAC_Operator::SUBTRACT;
    }
    else if (op == Binary_Operator::MULTIPLY)
    {
        return TAC_Operator::MULTIPLY;
    }
    else if (op == Binary_Operator::DIVIDE)
    {
        return TAC_Operator::DIVIDE;
    }
    else if (op == Binary_Operator::LT)
    {
        return TAC_Operator::LT;
    }
    else if (op == Binary_Operator::LE)
    {
        return TAC_Operator::LE;
    }
    else if (op == Binary_Operator::GT)
    {
        return TAC_Operator::GT;
    }
    else if (op == Binary_Operator::GE)
    {
        return TAC_Operator::GE;
    }
    else if (op == Binary_Operator::NE)
    {
        return TAC_Operator::NE;
    }
    else if (op == Binary_Operator::EQ)
    {
        return TAC_Operator::EQ;
    }
    else if (op == Binary_Operator::LOGICAL_AND)
    {
        return TAC_Operator::LOGICAL_AND;
    }
    else if (op == Binary_Operator::LOGICAL_OR)
    {
        return TAC_Operator::LOGICAL_OR;
    }
    else
    {
        throw_SemanticError("Unexpected type");
        return TAC_Operator::NOP;
    }
}

TAC_Operator unary_to_tac(Unary_Operator op)
{
    if (op == Unary_Operator::LOGICAL_NOT)
    {
        return TAC_Operator::LOGICAL_NOT;
    }
    else if (op == Unary_Operator::NEGATE)
    {
        return TAC_Operator::NEGATE;
    }
    else
    {
        throw_SemanticError("Unexpected type");
        return TAC_Operator::NOP;
    }
}

RTL_Operator invert_op(RTL_Operator op)
{
    RTL_Operator result = RTL_Operator::EQ;
    if (op == RTL_Operator::GE)
    {
        result = RTL_Operator::LT;
    }
    else if (op == RTL_Operator::GT)
    {
        result = RTL_Operator::LE;
    }
    else if (op == RTL_Operator::NE)
    {
        result = RTL_Operator::EQ;
    }
    else
    {
        throw_SemanticError("New operator that is not expected here");
    }
    return result;
}

RTL_Operator tac_to_rtl(TAC_Operator op)
{
    if (op == TAC_Operator::NOP)
    {
        throw SemanticError("Cannot have NOP in RTL");
    }
    else if (op == TAC_Operator::NEGATE)
    {
        return RTL_Operator::NEGATE;
    }
    else if (op == TAC_Operator::LOGICAL_NOT)
    {
        return RTL_Operator::LOGICAL_NOT;
    }
    else if (op == TAC_Operator::ADD)
    {
        return RTL_Operator::ADD;
    }
    else if (op == TAC_Operator::SUBTRACT)
    {
        return RTL_Operator::SUBTRACT;
    }
    else if (op == TAC_Operator::MULTIPLY)
    {
        return RTL_Operator::MULTIPLY;
    }
    else if (op == TAC_Operator::DIVIDE)
    {
        return RTL_Operator::DIVIDE;
    }
    else if (op == TAC_Operator::LOGICAL_AND)
    {
        return RTL_Operator::LOGICAL_AND;
    }
    else if (op == TAC_Operator::LOGICAL_OR)
    {
        return RTL_Operator::LOGICAL_OR;
    }
    else if (op == TAC_Operator::LT)
    {
        return RTL_Operator::LT;
    }
    else if (op == TAC_Operator::LE)
    {
        return RTL_Operator::LE;
    }
    else if (op == TAC_Operator::GT)
    {
        return RTL_Operator::GT;
    }
    else if (op == TAC_Operator::GE)
    {
        return RTL_Operator::GE;
    }
    else if (op == TAC_Operator::NE)
    {
        return RTL_Operator::NE;
    }
    else if (op == TAC_Operator::EQ)
    {
        return RTL_Operator::EQ;
    }
    else
    {
        throw_SemanticError("Unexpected TAC Operator: " + op_to_string(op));
    }

    return RTL_Operator::ADD; // dummy
}

std::string rtl_priority_to_register(int priority)
{
    if (priority == 1)
    {
        return "v0";
    }
    else if (priority <= 11)
    {
        return "t" + std::to_string(priority - 2);
    }
    else if (priority <= 19)
    {
        return "s" + std::to_string(priority - 12);
    }
    else if (priority <= 34)
    {
        return "f" + std::to_string((priority - 19) * 2);
    }
    else if (priority == PRIORITY_A0)
    {
        return "a0";
    }
    else if (priority == PRIORITY_F0)
    {
        return "f0";
    }
    else if (priority == PRIORITY_V1)
    {
        return "v1";
    }
    else
    {
        throw_SemanticError("This register doesn't exist currently, I have no clue why is it used\n");
        return "";
    }
}

bool is_relational_op(RTL_Operator op)
{
    return op == RTL_Operator::LT || op == RTL_Operator::LE || op == RTL_Operator::GT || op == RTL_Operator::GE || op == RTL_Operator::EQ || op == RTL_Operator::NE;
}
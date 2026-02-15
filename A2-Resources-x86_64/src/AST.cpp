#include "AST.hpp"
#include "SymTabEntry.hpp"

bool isNumericType(Type t)
{
    return t == Type::INT || t == Type::FLOAT;
}

bool isParametricType(Type t)
{
    return t == Type::INT || t == Type::FLOAT || t == Type::STR || t == Type::BOOL;
}

ExprAST::ExprAST(Type type, Operator op, ExprAST *opd1, ExprAST *opd2, ExprAST *opd3)
    : AST(), type(type), op(op), opd1(opd1), opd2(opd2), opd3(opd3)
{
    if (op == Operator::NOP)
    {
        if (opd1 || opd2 || opd3)
        {
            throw SemanticError("Expected 0 operands");
        }
    }
    else if (op == Operator::NEGATE || op == Operator::LOGICAL_NOT)
    {
        if (!opd1 || opd2 || opd3)
        {
            throw SemanticError("Expected 1 operand");
        }
        if (op == Operator::NEGATE)
        {
            if (!isNumericType(opd1->type))
            {
                throw SemanticError("Expected operand of type INT or FLOAT");
            }
        }
        else // op == Operator::LOGICAL_NOT
        {
            if (opd1->type != Type::BOOL)
            {
                throw SemanticError("Expected operand of type BOOL");
            }
        }
        type = opd1->type;
    }
    else if (op == Operator::QUESTION_MARK_COLON)
    {
        if (!opd1 || !opd2 || !opd3)
        {
            throw SemanticError("Expected 3 operands");
        }
        if (opd1->type != Type::BOOL || !isParametricType(opd2->type) || opd2->type != opd3->type)
        {
            throw SemanticError("Incorrect types of the 3 operands");
        }
        type = opd2->type;
    }
    else // (binary) arithmetic, logical and comparison operators
    {
        if (!opd1 || !opd2 || opd3)
        {
            throw SemanticError("Expected 2 operands");
        }
        if (opd1->type != opd2->type)
        {
            throw SemanticError("Expected both operands to have the same type");
        }
        if (op == Operator::LOGICAL_AND || op == Operator::LOGICAL_OR)
        {
            if (opd1->type != Type::BOOL)
            {
                throw SemanticError("Expected both operands to be of type BOOL");
            }
            type = Type::BOOL;
        }
        else // arithmetic/comparison operators
        {
            if (!isNumericType(opd1->type))
            {
                throw SemanticError("Expected both operands to be of type INT or FLOAT");
            }
            if (op == Operator::ADD || op == Operator::SUBTRACT || op == Operator::MULTIPLY || op == Operator::DIVIDE)
            {
                type = opd1->type;
            }
            else
            {
                type = Type::BOOL;
            }
        }
    }
}

IntLiteralAST::IntLiteralAST(int ival)
    : ExprAST(Type::INT), ival(ival)
{
}

FloatLiteralAST::FloatLiteralAST(float fval)
    : ExprAST(Type::FLOAT), fval(fval)
{
}

StrLiteralAST::StrLiteralAST(char *_sval)
    : ExprAST(Type::STR), sval(_sval)
{
}

void VarAST::set_sym_tab_entry_ptr(SymTabEntry *ste_ptr)
{
    sym_tab_entry_ptr = ste_ptr;
    type = sym_tab_entry_ptr->type;
}

VarAST::VarAST(char *var_name) : ExprAST(), sym_tab_entry_ptr(nullptr)
{
    std::string key(var_name);

    // Check local scope first, then global scope
    auto it_local = SymTabEntry::local_scope.find(key);
    if (it_local != SymTabEntry::local_scope.end())
    {
        set_sym_tab_entry_ptr(it_local->second);
        return;
    }

    auto it_global = SymTabEntry::global_scope.find(key);
    if (it_global != SymTabEntry::global_scope.end())
    {
        set_sym_tab_entry_ptr(it_global->second);
        return;
    }
}

AssignAST::AssignAST(VarAST *lhs, ExprAST *rhs)
    : AST(), lhs(lhs), rhs(rhs)
{
}

ReadAST::ReadAST(VarAST *var)
    : AST(), var(var)
{
}

WriteAST::WriteAST(ExprAST *expr)
    : AST(), expr(expr)
{
}

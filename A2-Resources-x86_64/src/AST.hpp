#ifndef AST_HPP
#define AST_HPP

#include <string>

#include "SymTabEntry.hpp"
#include "SemanticError.hpp"

enum class Operator
{
    NOP, // for literals/variables
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    NEGATE,
    QUESTION_MARK_COLON,
    LOGICAL_AND,
    LOGICAL_OR,
    LOGICAL_NOT,
    LT,
    LE,
    GT,
    GE,
    NE,
    EQ,
};

class AST
{
};

// LValueAST comes later

class ExprAST : public AST
{
    Operator op;
    ExprAST *opd1;
    ExprAST *opd2;
    ExprAST *opd3;

protected:
    Type type;

public:
    ExprAST(Type type = Type::UNKNOWN, Operator op = Operator::NOP, ExprAST *opd1 = nullptr, ExprAST *opd2 = nullptr, ExprAST *opd3 = nullptr);
};

class IntLiteralAST : public ExprAST
{
    int ival;

public:
    IntLiteralAST(int ival);
};

class FloatLiteralAST : public ExprAST
{
    float fval;

public:
    FloatLiteralAST(float fval);
};

class StrLiteralAST : public ExprAST
{
    std::string sval;

public:
    StrLiteralAST(char *_sval);
};

class VarAST : public ExprAST
{
    SymTabEntry *sym_tab_entry_ptr;

    void set_sym_tab_entry_ptr(SymTabEntry *ste_ptr);

public:
    VarAST(char *var_name);
};

class AssignAST : public AST
{
    VarAST *lhs;
    ExprAST *rhs;

public:
    AssignAST(VarAST *lhs, ExprAST *rhs);
};

class ReadAST : public AST
{
    VarAST *var;

public:
    ReadAST(VarAST *var);
};

class WriteAST : public AST
{
    ExprAST *expr;

public:
    WriteAST(ExprAST *expr);
};

#endif

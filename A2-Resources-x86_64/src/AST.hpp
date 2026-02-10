#ifndef AST_HPP
#define AST_HPP

#include <string>

#include "SymTabEntry.hpp"

enum class Operator {
    NOP, // for literals/variables
    PLUS,
    MINUS,
    MULT,
    DIV,
    UMINUS,
    QUESTION_MARK_COLON,
    AND,
    OR,
    NOT,
    LESS_THAN,
    LESS_THAN_EQUAL,
    GREATER_THAN,
    GREATER_THAN_EQUAL,
    NOT_EQUAL,
    EQUAL,
};

class AST {

};

// LValueAST comes later

class ExprAST : public AST  {
    Type type;
    Operator op;
    ExprAST *operand1;
    ExprAST *operand2;
    ExprAST *operand3;

public:
    ExprAST(Type type, Operator op, ExprAST *operand1, ExprAST *operand2, ExprAST *operand3);
};

class IntLiteralAST : public ExprAST  {
    int ival;

public:
    IntLiteralAST(int ival): ExprAST(Type::INT), ival(ival) {}
};

class FloatLiteralAST : public ExprAST  {
    float fval;

    FloatLiteralAST(float fval): RValueAST(Type::FLOAT), fval(fval) {}
};

class StrLiteralAST : public ExprAST  {
    std::string sval;

public:
    StrLiteralAST(char *_sval) : RValueAST(Type::STR), sval(_sval) {}
};

class VarAST : public ExprAST  {
    SymTabEntry *sym_tab_entry_ptr;

    void set_sym_tab_entry_ptr(SymTabEntry *ste_ptr);

public:
    VarAST(char *var_name);
};

class AssignAST : public AST {
    VarAST *lhs;
    ExprAST *rhs;

public:
    AssignAST(VarAST *lhs, ExprAST *rhs) : AST(), lhs(lhs), rhs(rhs) {}
};

class ReadAST : public AST  {
    VarAST *var;

public:
    ReadAST(VarAST *var) : AST(), var(var) {}
};

class WriteAST : public AST  {
    ExprAST *expr;

public:
    WriteAST(ExprAST *expr);
};

#endif

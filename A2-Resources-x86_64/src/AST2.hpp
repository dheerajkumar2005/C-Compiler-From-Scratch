#ifndef AST_HPP
#define AST_HPP

#include <string>

#include "SymTabEntry.hpp"
#include "SemanticError.hpp"

enum class Operator{
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

class AST{
    virtual void print_AST();
};

class Expr_AST : public AST {
    Type type;
};


class Base_Expr_AST : public Expr_AST{

};

class Var_AST : public Base_Expr_AST{
    SymTabEntry *sym_tab_entry_ptr;
    void set_sym_tab_entry_ptr(SymTabEntry* ste_ptr);

    public:
        Var_AST(char* var_name);
        void print_AST();

};

class Int_Num_Expr_AST : public Base_Expr_AST{
    int ival;

    public:
        Int_Num_Expr_AST(int ival);
        void print_AST();
};

class Float_Num_Expr_AST : public Base_Expr_AST{
    float fval; // For this it should be rounded off to 2 decimals

    public:
        Float_Num_Expr_AST(float fval);
        void print_AST();
};

class String_Expr_AST : public Base_Expr_AST{
    std::string sval;

    public:
        String_Expr_AST(char* _sval);
        void print_AST();
};


class Unary_Expr_AST : public Expr_AST{

};

class Uminus_Expr_AST : public Expr_AST{
    Type type;
    Expr_AST* L_opd;

    public:
        Uminus_Expr_AST(Expr_AST* _L_opd);
        void print_AST();
};

class Binary_Expr_AST : public Expr_AST{
    Type type;
    Operator op;
    Expr_AST* L_opd;
    Expr_AST* R_opd;

    public:
        Binary_Expr_AST(Operator op, Expr_AST* L_opd, Expr_AST* R_opd);
        void print_AST();
};


class Ternary_Expr_AST : public Expr_AST{
    Type type;
    Expr_AST* cond;
    Expr_AST* true_part;
    Expr_AST* false_part;

    public:
        Ternary_Expr_AST(Expr_AST* cond, Expr_AST* true_part, Expr_AST* false_part);
        void print_AST();
};


class Stmt_AST : public AST{

};

class Assign_AST : public Stmt_AST{
    Var_AST* lhs;
    Expr_AST* rhs;

    public:
        Assign_AST(Var_AST *lhs, Expr_AST *rhs);
        void print_AST();

};

class Read_AST : public Stmt_AST{
    Var_AST *var;

    public:
        Read_AST(Var_AST *var);
        void print_AST();
};

class Write_AST : public Stmt_AST{
    Expr_AST *expr;
    public:
        Write_AST(Expr_AST *expr);
        void print_AST();
};








#endif
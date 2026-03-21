#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <ostream>
#include <iomanip>
#include <sstream>

#include "TAC.hpp"

extern std::ostream *astout;
extern std::ostream *tacout;

class Ast
{
public:
    Code *code;

    Ast();
    virtual ~Ast() = 0;
    virtual std::string to_string() const = 0;
};

class Expression_Ast : public Ast
{
public:
    Type type;
    TAC_Operand *place;

    Expression_Ast(Type type);
    virtual ~Expression_Ast() = 0;

    Type get_type() const;
};

class Base_Expr_Ast : public Expression_Ast
{
public:
    Base_Expr_Ast(Type type);
    virtual ~Base_Expr_Ast() = 0;
};

class Name_Expr_Ast : public Base_Expr_Ast
{
public:
    std::string var_name;
    Scope *declaring_scope;

    Name_Expr_Ast(std::string *id, Scope *declaring_scope, Type type);
    ~Name_Expr_Ast() override = default;

    virtual std::string to_string() const override final;
};

class Int_Expr_Ast : public Base_Expr_Ast
{
protected:
    int ival;

public:
    Int_Expr_Ast(int ival);
    ~Int_Expr_Ast() override = default;

    virtual std::string to_string() const override final;
};

class Float_Expr_Ast : public Base_Expr_Ast
{
protected:
    float fval;

public:
    Float_Expr_Ast(float fval);
    ~Float_Expr_Ast() = default;

    virtual std::string to_string() const override final;
};

class String_Expr_Ast : public Base_Expr_Ast
{
protected:
    std::string sval;

public:
    String_Expr_Ast(char *_sval);
    ~String_Expr_Ast() = default;

    virtual std::string to_string() const override;
};

class Unary_Expr_AST : public Expression_Ast
{
protected:
    Unary_Operator op;
    Expression_Ast *opd1;

public:
    Unary_Expr_AST(Type type, Unary_Operator op, Expression_Ast *opd1);
    virtual ~Unary_Expr_AST() = 0;
};

class UMinus_Expr_Ast : public Unary_Expr_AST
{
public:
    UMinus_Expr_Ast(Expression_Ast *opd1);
    ~UMinus_Expr_Ast() = default;

    virtual std::string to_string() const override final;
};

class Logical_Not_Expr_Ast : public Unary_Expr_AST
{
public:
    Logical_Not_Expr_Ast(Expression_Ast *opd1);
    ~Logical_Not_Expr_Ast() = default;

    virtual std::string to_string() const override final;
};

class Binary_Expr_Ast : public Expression_Ast
{
protected:
    Binary_Operator op;
    Expression_Ast *opd1;
    Expression_Ast *opd2;

public:
    Binary_Expr_Ast(Type type, Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2);
    virtual ~Binary_Expr_Ast() = 0;
};

class Boolean_Expr_Ast : public Binary_Expr_Ast
{
public:
    Boolean_Expr_Ast(Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2);
    ~Boolean_Expr_Ast() = default;

    virtual std::string to_string() const override final;
};

class Div_Expr_Ast : public Binary_Expr_Ast
{
public:
    Div_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2);
    ~Div_Expr_Ast() = default;

    virtual std::string to_string() const override final;
};

class Minus_Expr_Ast : public Binary_Expr_Ast
{
public:
    Minus_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2);
    ~Minus_Expr_Ast() = default;

    virtual std::string to_string() const override final;
};

class Mult_Expr_Ast : public Binary_Expr_Ast
{
public:
    Mult_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2);
    ~Mult_Expr_Ast() = default;

    virtual std::string to_string() const override final;
};

class Plus_Expr_Ast : public Binary_Expr_Ast
{
public:
    Plus_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2);
    ~Plus_Expr_Ast() = default;

    virtual std::string to_string() const override final;
};

class Relational_Expr_Ast : public Binary_Expr_Ast
{
public:
    Relational_Expr_Ast(Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2);
    ~Relational_Expr_Ast() = default;

    virtual std::string to_string() const override final;
};

class Ternary_Expr_Ast : public Expression_Ast
{
protected:
    Ternary_Operator op;
    Expression_Ast *opd1;
    Expression_Ast *opd2;
    Expression_Ast *opd3;

public:
    Ternary_Expr_Ast(Type type, Ternary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2, Expression_Ast *opd3);
    virtual ~Ternary_Expr_Ast() = 0;
};

class Conditional_Expr_Ast : public Ternary_Expr_Ast
{
public:
    Conditional_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2, Expression_Ast *opd3);
    ~Conditional_Expr_Ast() = default;

    virtual std::string to_string() const override final;
};

class Statement_Ast : public Ast
{
public:
    Statement_Ast();
    virtual ~Statement_Ast() = 0;
};

class Assignment_Stmt_Ast : public Statement_Ast
{
protected:
    Name_Expr_Ast *lhs;
    Expression_Ast *rhs;

public:
    Assignment_Stmt_Ast(Name_Expr_Ast *lhs, Expression_Ast *rhs);
    ~Assignment_Stmt_Ast() = default;

    virtual std::string to_string() const override final;
};

class Read_Stmt_Ast : public Statement_Ast
{
protected:
    Name_Expr_Ast *var;

public:
    Read_Stmt_Ast(Name_Expr_Ast *var);
    ~Read_Stmt_Ast() = default;

    virtual std::string to_string() const override final;
};

class Write_Stmt_Ast : public Statement_Ast
{
protected:
    Expression_Ast *expr;

public:
    Write_Stmt_Ast(Expression_Ast *expr);
    ~Write_Stmt_Ast() = default;

    virtual std::string to_string() const override final;
};

class Compound_Stmt_Ast : public Statement_Ast
{
public:
    std::vector<Statement_Ast *> *stmts;

    Compound_Stmt_Ast(Statement_Ast *stmt);
    Compound_Stmt_Ast(std::vector<Statement_Ast *> *stmts);
    ~Compound_Stmt_Ast() = default;

    virtual std::string to_string() const override final;
};

class If_Stmt_Ast : public Statement_Ast
{
public:
    Boolean_Expr_Ast *predicate;
    Compound_Stmt_Ast *if_clause;
    Compound_Stmt_Ast *else_clause;

    If_Stmt_Ast(Boolean_Expr_Ast *predicate, Statement_Ast *_if_clause, Statement_Ast *_else_clause = nullptr);
    ~If_Stmt_Ast() = default;

    virtual std::string to_string() const override final;
};

#endif

#ifndef AST_HPP
#define AST_HPP

enum class Type {
    UNKNOWN, // default type
	INT,
	FLOAT,
	STR,
    BOOL,
};

enum class Operator {
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

class AssignAST : public AST {
    VarAST *lhs;
    ExprAST *rhs;

public:
    AssignAST(VarAST *lhs, ExprAST *rhs) : AST(), lhs(lhs), rhs(rhs) {}
};

class ReadAST : public AST final {
    VarAST *var;

public:
    ReadAST(VarAST *var) : AST(), var(var) {}
};

class WriteAST : public AST final {
    ExprAST *expr;

public:
    WriteAST(ExprAST *expr);
};

// LValueAST comes later

class RValueAST : public AST {
protected:
    Type type;

public:
    RValueAST(Type type) : type(type) {}
    RValueAST() : RValueAST(Type::UNKNOWN) {}
};

class IntLiteralAST : public RValueAST final {
    int ival;

public:
    IntLiteralAST(int ival): RValueAST(Type::INT), ival(ival) {}
};

class FloatLiteralAST : public RValueAST final {
    float fval;

    FloatLiteralAST(float fval): RValueAST(Type::FLOAT), fval(fval) {}
};

class StrLiteralAST : public RValueAST final {
    std::string sval;

public:
    StrLiteralAST(char *_sval) : RValueAST(Type::STR), sval(_sval)
};

class VarAST : public RValueAST final {
    SymTabEntry *sym_tab_entry_ptr;

    set_sym_tab_entry_ptr(SymTabEntry *ste_ptr);

public:
    VarAST(char *var_name);
};

class ExprAST : public RValueAST final {
    Operator op;
    RValueAST *operand1, operand2, operand3;

public:
    ExprAST(Operator op, RValueAST *operand1, RValueAST *operand2, RValueAST *operand3) : RValueAST(), op(op), operand1(operand1), operand2(operand2), operand3(operand3) {}
    ExprAST(Operator op, RValueAST *operand1, RValueAST *operand2) : ExprAST(op, operand1, operand2, nullptr) {}
    ExprAST(Operator op, RValueAST *operand1) : ExprAST(op, operand1, nullptr) {}
};

#endif

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

struct AST {

};

struct AssignAST : public AST {
    VarAST *lhs;
    ExprAST *rhs;

    AssignAST(VarAST *lhs, ExprAST *rhs) : AST(), lhs(lhs), rhs(rhs) {}
};

struct ReadAST : public AST {
    VarAST *var;

    ReadAST(VarAST *var) : AST(), var(var) {}
};

struct WriteAST : public AST {
    ExprAST *expr;

    WriteAST(ExprAST *expr);
};

// LValueAST comes later

struct RValueAST : public AST {
    // TODO: Add fields/methods as required
    // TODO: Add atleast one pure virtual function
    Type type;
    
    RValueAST(Type type) : type(type) {}
    RValueAST() : RValueAST(Type::UNKNOWN) {}
    virtual ~RValueAST() = default;
};

struct IntLiteralAST : public RValueAST {
    int ival;

    IntLiteralAST(int ival): RValueAST(Type::INT), ival(ival) {}
};

struct FloatLiteralAST : public RValueAST {
    float fval;

    FloatLiteralAST(float fval): RValueAST(Type::FLOAT), fval(fval) {}
};

struct StrLiteralAST : public RValueAST {
    std::string sval;

    StrLiteralAST(char *_sval) : RValueAST(Type::STR), sval(_sval)
};

struct VarAST : public RValueAST {
    SymTabEntry *sym_tab_entry_ptr;

    VarAST(char *var_name);
};

struct ExprAST : public RValueAST {
    Operator op;
    RValueAST *operand1, operand2, operand3;

    ExprAST(Operator op, RValueAST *operand1, RValueAST *operand2, RValueAST *operand3) : RValueAST(), op(op), operand1(operand1), operand2(operand2), operand3(operand3) {}
    ExprAST(Operator op, RValueAST *operand1, RValueAST *operand2) : ExprAST(op, operand1, operand2, nullptr) {}
    ExprAST(Operator op, RValueAST *operand1) : ExprAST(op, operand1, nullptr) {}
};

#endif

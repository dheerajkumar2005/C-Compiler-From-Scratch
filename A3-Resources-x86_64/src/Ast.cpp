#include "Ast.hpp"

Ast::Ast() : code(nullptr)
{
}

Ast::~Ast()
{
}

Expression_Ast::Expression_Ast(Type type)
    : Ast(), type(type), place(nullptr)
{
}

Expression_Ast::~Expression_Ast()
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

Base_Expr_Ast::~Base_Expr_Ast()
{
}

Name_Expr_Ast::Name_Expr_Ast(std::string *id, Scope *declaring_scope, Type type)
    : Base_Expr_Ast(type), var_name(*id), declaring_scope(declaring_scope)
{
    if (type == Type::VOID)
    {
        throw_SemanticError("can't have void types in variable declarations");
    }
}

std::string Name_Expr_Ast::to_string() const
{
    return "Name : " + var_name + "_<" + type_to_string(get_type()) + ">";
}

Int_Expr_Ast::Int_Expr_Ast(int ival)
    : Base_Expr_Ast(Type::INT), ival(ival)
{
    this->place = new Int_Const_TAC_Operand(ival);
}

std::string Int_Expr_Ast::to_string() const
{
    return "Num : " + std::to_string(ival) + "<" + type_to_string(get_type()) + ">";
}

Float_Expr_Ast::Float_Expr_Ast(float fval)
    : Base_Expr_Ast(Type::FLOAT), fval(fval)
{
    this->place = new Float_Const_TAC_Operand(fval);
}

std::string Float_Expr_Ast::to_string() const
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << fval;
    return "Num : " + out.str() + "<" + type_to_string(get_type()) + ">";
}

String_Expr_Ast::String_Expr_Ast(char *_sval)
    : Base_Expr_Ast(Type::STR), sval(_sval)
{
    this->place = new String_Const_TAC_operand(_sval);
}

std::string String_Expr_Ast::to_string() const
{
    return "String : " + sval + "<" + type_to_string(get_type()) + ">";
}

Unary_Expr_AST::Unary_Expr_AST(Type type, Unary_Operator op, Expression_Ast *opd1)
    : Expression_Ast(type), op(op), opd1(opd1)
{
    Temporary_TAC_Operand *t1 = new Temporary_TAC_Operand();

    Assignment_TAC_Statement *c1 = new Assignment_TAC_Statement(t1, op, opd1->place);

    code = new Code();
    code->append_list(opd1->code);
    code->append_statement(c1);

    place = t1;
}

Unary_Expr_AST::~Unary_Expr_AST()
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

std::string UMinus_Expr_Ast::to_string() const
{
    return "\nArith: Uminus<" + type_to_string(get_type()) + ">\nL_Opd (" + opd1->to_string() + ")";
}

Logical_Not_Expr_Ast::Logical_Not_Expr_Ast(Expression_Ast *opd1)
    : Unary_Expr_AST(Type::BOOL, Unary_Operator::LOGICAL_NOT, opd1)
{
    if (opd1->get_type() != Type::BOOL)
    {
        throw_SemanticError("operand of logical not should be of type bool");
    }
}

std::string Logical_Not_Expr_Ast::to_string() const
{
    return "\nCondition: NOT<bool>\nL_Opd (" + opd1->to_string() + ")";
}

Binary_Expr_Ast::Binary_Expr_Ast(Type type, Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2)
    : Expression_Ast(type), op(op), opd1(opd1), opd2(opd2)
{
    Temporary_TAC_Operand *t1 = new Temporary_TAC_Operand();

    Assignment_TAC_Statement *c1 = new Assignment_TAC_Statement(t1, op, opd1->place, opd2->place);

    code = new Code();
    // Can evaluate in any order
    code->append_list(opd1->code);
    code->append_list(opd2->code);
    code->append_statement(c1);

    place = t1;
}

Binary_Expr_Ast::~Binary_Expr_Ast()
{
}

bool are_same_type(Expression_Ast *opd1, Expression_Ast *opd2)
{
    return opd1->get_type() == opd2->get_type();
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

std::string Boolean_Expr_Ast::to_string() const
{
    return "\nCondition: " + op_to_string(op) + "<bool>\nL_Opd (" + opd1->to_string() + ")\nR_Opd (" + opd2->to_string() + ")";
}

bool is_numeric_type(Expression_Ast *opd)
{
    return opd->get_type() == Type::FLOAT || opd->get_type() == Type::INT;
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

std::string Div_Expr_Ast::to_string() const
{
    return "\nArith: Div<" + type_to_string(get_type()) + ">\nL_Opd (" + opd1->to_string() + ")\nR_Opd (" + opd2->to_string() + ")";
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

std::string Minus_Expr_Ast::to_string() const
{
    return "\nArith: Minus<" + type_to_string(get_type()) + ">\nL_Opd (" + opd1->to_string() + ")\nR_Opd (" + opd2->to_string() + ")";
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

std::string Mult_Expr_Ast::to_string() const
{
    return "\nArith: Mult<" + type_to_string(get_type()) + ">\nL_Opd (" + opd1->to_string() + ")\nR_Opd (" + opd2->to_string() + ")";
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

std::string Plus_Expr_Ast::to_string() const
{
    return "\nArith: Plus<" + type_to_string(get_type()) + ">\nL_Opd (" + opd1->to_string() + ")\nR_Opd (" + opd2->to_string() + ")";
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

std::string Relational_Expr_Ast::to_string() const
{
    return "\nCondition: " + op_to_string(op) + "<bool>\nL_Opd (" + opd1->to_string() + ")\nR_Opd (" + opd2->to_string() + ")";
}

Ternary_Expr_Ast::Ternary_Expr_Ast(Type type, Ternary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2, Expression_Ast *opd3)
    : Expression_Ast(type), op(op), opd1(opd1), opd2(opd2), opd3(opd3)
{
}

Ternary_Expr_Ast::~Ternary_Expr_Ast()
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

    Temporary_TAC_Operand *t1 = new Temporary_TAC_Operand();
    Shared_Temporary_TAC_Operand *t2 = new Shared_Temporary_TAC_Operand();

    TAC_Label *l1 = new TAC_Label();
    TAC_Label *l2 = new TAC_Label();

    Assignment_TAC_Statement *c1 = new Assignment_TAC_Statement(t1, Unary_Operator::LOGICAL_NOT, opd1->place);
    If_Goto_TAC_Statement *c2 = new If_Goto_TAC_Statement(t1, l1);
    Assignment_TAC_Statement *c3 = new Assignment_TAC_Statement(t2, opd2->place);
    Goto_TAC_Statement *c4 = new Goto_TAC_Statement(l2);
    Label_TAC_Statement *c5 = new Label_TAC_Statement(l1);
    Assignment_TAC_Statement *c6 = new Assignment_TAC_Statement(t2, opd3->place);
    Label_TAC_Statement *c7 = new Label_TAC_Statement(l2);

    code = new Code();
    code->append_list(opd1->code);
    code->append_statement(c1);
    code->append_statement(c2);
    code->append_list(opd2->code);
    code->append_statement(c3);
    code->append_statement(c4);
    code->append_statement(c5);
    code->append_list(opd3->code);
    code->append_statement(c6);
    code->append_statement(c7);

    place = t2;
}

std::string Conditional_Expr_Ast::to_string() const
{
    return opd1->to_string() + "\nTrue_Part (" + opd2->to_string() + ")\nFalse_Part (" + opd3->to_string() + ")";
}

Statement_Ast::Statement_Ast()
    : Ast()
{
}

Statement_Ast::~Statement_Ast()
{
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

    Variable_TAC_Operand *id = new Variable_TAC_Operand(&lhs->var_name, lhs->declaring_scope);

    Assignment_TAC_Statement *c1 = new Assignment_TAC_Statement(id, rhs->place);

    code = new Code();
    code->append_statement(c1);
}

std::string Assignment_Stmt_Ast::to_string() const
{
    return "Asgn:\nLHS (" + lhs->to_string() + ")\nRHS (" + rhs->to_string() + ")";
}

Read_Stmt_Ast::Read_Stmt_Ast(Name_Expr_Ast *var)
    : Statement_Ast(), var(var)
{
    if (var->get_type() != Type::INT && var->get_type() != Type::FLOAT)
    {
        throw_SemanticError("can read only int or float");
    }

    Variable_TAC_Operand *id = new Variable_TAC_Operand(&var->var_name, var->declaring_scope);

    IO_TAC_Statement *c1 = new IO_TAC_Statement(IO_Kind::READ, id);

    code = new Code();
    code->append_statement(c1);
}

std::string Read_Stmt_Ast::to_string() const
{
    return "Read: " + var->to_string();
}

Write_Stmt_Ast::Write_Stmt_Ast(Expression_Ast *expr)
    : Statement_Ast(), expr(expr)
{
    Type type = expr->get_type();
    if (type == Type::VOID || type == Type::BOOL)
    {
        throw_SemanticError("can't print bool or void");
    }

    IO_TAC_Statement *c1 = new IO_TAC_Statement(IO_Kind::WRITE, expr->place);

    code = new Code();
    code->append_list(expr->code);
    code->append_statement(c1);
}

std::string Write_Stmt_Ast::to_string() const
{
    return "Write: " + expr->to_string();
}

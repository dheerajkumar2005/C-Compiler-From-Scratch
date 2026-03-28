#include "Ast.hpp"

Ast::Ast()
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
        return;
    }

    place = new Variable_TAC_Operand(id, declaring_scope);
}

Code *Name_Expr_Ast::codegen()
{
    return nullptr;
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

Code *Int_Expr_Ast::codegen()
{
    return nullptr;
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

Code *Float_Expr_Ast::codegen()
{
    return nullptr;
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

Code *String_Expr_Ast::codegen()
{
    return nullptr;
}

std::string String_Expr_Ast::to_string() const
{
    return "String : " + sval + "<" + type_to_string(get_type()) + ">";
}

Unary_Expr_AST::Unary_Expr_AST(Type type, Unary_Operator op, Expression_Ast *opd1)
    : Expression_Ast(type), op(op), opd1(opd1)
{
}

Unary_Expr_AST::~Unary_Expr_AST()
{
}

Code *Unary_Expr_AST::codegen()
{
    Code *code = new Code();

    code->append_list(opd1->codegen());

    place = new Temporary_TAC_Operand();
    code->append_statement(new Assignment_TAC_Statement(place, op, opd1->place));

    return code;
}

UMinus_Expr_Ast::UMinus_Expr_Ast(Expression_Ast *opd1)
    : Unary_Expr_AST(opd1->get_type(), Unary_Operator::NEGATE, opd1)
{
    if (opd1->get_type() != Type::INT && opd1->get_type() != Type::FLOAT)
    {
        throw_SemanticError("uminus can only have int or float argument");
        return;
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
        return;
    }
}

std::string Logical_Not_Expr_Ast::to_string() const
{
    return "\nCondition: NOT<bool>\nL_Opd (" + opd1->to_string() + ")";
}

Binary_Expr_Ast::Binary_Expr_Ast(Type type, Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2)
    : Expression_Ast(type), op(op), opd1(opd1), opd2(opd2)
{
}

Binary_Expr_Ast::~Binary_Expr_Ast()
{
}

Code *Binary_Expr_Ast::codegen()
{
    Code *code = new Code();

    // Can evaluate in any order
    code->append_list(opd1->codegen());

    code->append_list(opd2->codegen());

    place = new Temporary_TAC_Operand();
    code->append_statement(new Assignment_TAC_Statement(place, op, opd1->place, opd2->place));

    return code;
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
        return;
    }
    if (opd1->get_type() != Type::BOOL)
    {
        throw_SemanticError("Operand of Boolean expr should be bool");
        return;
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
        return;
    }
    else if (!is_numeric_type(opd1))
    {
        throw_SemanticError("Operand of arith expr should be int/float");
        return;
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
        return;
    }
    if (!is_numeric_type(opd1))
    {
        throw_SemanticError("Operand of arith expr should be int/float");
        return;
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
        return;
    }
    if (!is_numeric_type(opd1))
    {
        throw_SemanticError("Operand of arith expr should be int/float");
        return;
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
        return;
    }
    if (!is_numeric_type(opd1))
    {
        throw_SemanticError("Operand of arith expr should be int/float");
        return;
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
        return;
    }
    if (!are_same_type(opd1, opd2))
    {
        throw_SemanticError("Both operands should be of the same type");
        return;
    }
    if (!is_numeric_type(opd1))
    {
        throw_SemanticError("Operand of comparison expr should be int/float");
        return;
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
        return;
    }
    if (opd2->get_type() != opd3->get_type())
    {
        throw_SemanticError("both parts should be of same type in ternary expr");
        return;
    }
    if (opd2->get_type() == Type::VOID)
    {
        throw_SemanticError("can't have type void in expr");
        return;
    }
}

Code *Conditional_Expr_Ast::codegen()
{
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

    Code *code = new Code();
    code->append_list(opd1->codegen());
    code->append_statement(c1);
    code->append_statement(c2);
    code->append_list(opd2->codegen());
    code->append_statement(c3);
    code->append_statement(c4);
    code->append_statement(c5);
    code->append_list(opd3->codegen());
    code->append_statement(c6);
    code->append_statement(c7);

    place = t2;
}

std::string Conditional_Expr_Ast::to_string() const
{
    return opd1->to_string() + "\nTrue_Part (" + opd2->to_string() + ")\nFalse_Part (" + opd3->to_string() + ")";
}

Statement_Ast::Statement_Ast(RegisterTracker *register_tracker)
    : Ast(), register_tracker(register_tracker)
{
}

Statement_Ast::~Statement_Ast()
{
}

Assignment_Stmt_Ast::Assignment_Stmt_Ast(Name_Expr_Ast *lhs, Expression_Ast *rhs, RegisterTracker *register_tracker)
    : Statement_Ast(register_tracker), lhs(lhs), rhs(rhs)
{
    if (lhs->get_type() != rhs->get_type())
    {
        throw_SemanticError("both sides of assign stmt should have same type");
        return;
    }
    if (lhs->get_type() == Type::VOID)
    {
        throw_SemanticError("cannot assign to type VOID");
        return;
    }
}

Code *Assignment_Stmt_Ast::codegen()
{
    Variable_TAC_Operand *id = new Variable_TAC_Operand(&lhs->var_name, lhs->declaring_scope);

    Assignment_TAC_Statement *c1 = new Assignment_TAC_Statement(id, rhs->place);

    Code *code = new Code();
    code->append_list(rhs->codegen());
    code->append_statement(c1);

    return code;
}

RTL_Code *Assignment_Stmt_Ast::rtlgen()
{
    RTL_Code *rtl_code = new RTL_Code();
    for (auto tac_stmt_ptr : *(codegen()->stmt_list))
    {
        RTL_Code *stmt_code = tac_stmt_ptr->to_rtl(register_tracker, lhs->type, rhs->type);
        rtl_code->append_list(stmt_code);
    }
}

std::string Assignment_Stmt_Ast::to_string() const
{
    return "Asgn:\nLHS (" + lhs->to_string() + ")\nRHS (" + rhs->to_string() + ")";
}

Read_Stmt_Ast::Read_Stmt_Ast(Name_Expr_Ast *var, RegisterTracker *register_tracker)
    : Statement_Ast(register_tracker), var(var)
{
    if (var->get_type() != Type::INT && var->get_type() != Type::FLOAT)
    {
        throw_SemanticError("can read only int or float");
        return;
    }
}

Code *Read_Stmt_Ast::codegen()
{
    Variable_TAC_Operand *id = new Variable_TAC_Operand(&var->var_name, var->declaring_scope);

    IO_TAC_Statement *c1 = new IO_TAC_Statement(IO_Kind::READ, id);

    Code *code = new Code();
    code->append_statement(c1);

    return code;
}

RTL_Code *Read_Stmt_Ast::rtlgen()
{
    return codegen()->stmt_list->front()->to_rtl(register_tracker, var->type, Type::VOID);
}

std::string Read_Stmt_Ast::to_string() const
{
    return "Read: " + var->to_string();
}

Write_Stmt_Ast::Write_Stmt_Ast(Expression_Ast *expr, RegisterTracker *register_tracker)
    : Statement_Ast(register_tracker), expr(expr)
{
    Type type = expr->get_type();
    if (type == Type::VOID || type == Type::BOOL)
    {
        throw_SemanticError("can't print bool or void");
        return;
    }
}

Code *Write_Stmt_Ast::codegen()
{
    IO_TAC_Statement *c1 = new IO_TAC_Statement(IO_Kind::WRITE, expr->place);

    Code *code = new Code();
    code->append_list(expr->codegen());
    code->append_statement(c1);

    return code;
}

RTL_Code *Write_Stmt_Ast::rtlgen()
{
    return codegen()->stmt_list->front()->to_rtl(register_tracker, expr->type, Type::VOID);
}

std::string Write_Stmt_Ast::to_string() const
{
    return "Write: " + expr->to_string();
}

Compound_Stmt_Ast::Compound_Stmt_Ast(std::vector<Statement_Ast *> *stmts)
    : Statement_Ast(register_tracker), stmts(stmts)
{
}

Code *Compound_Stmt_Ast::codegen()
{
    Code *code = new Code();
    for (auto stmt : *stmts)
    {
        code->append_list(stmt->codegen());
    }

    return code;
}

RTL_Code *Compound_Stmt_Ast::rtlgen()
{
    RTL_Code *rtl_code = new RTL_Code();
    for (auto stmt : *stmts)
    {
        rtl_code->append_list(stmt->rtlgen());
    }

    return rtl_code;
}

std::string Compound_Stmt_Ast::to_string() const
{
    std::string result;

    for (auto stmt : *stmts)
    {
        result += "\n" + stmt->to_string();
    }

    return result;
}

// TODO
If_Stmt_Ast::If_Stmt_Ast(Expression_Ast *predicate, Statement_Ast *if_clause, Statement_Ast *else_clause)
    : Statement_Ast(register_tracker), predicate(predicate), if_clause(if_clause), else_clause(else_clause)
{
    if (predicate->type != Type::BOOL)
    {
        throw_SemanticError("Expected predicate of type BOOL, got: " + type_to_string(predicate->type));
        return;
    }

    // t1 = new Temporary_TAC_Operand();

    // l1 = new TAC_Label();

    // c1 = new Assignment_TAC_Statement(t1, Unary_Operator::LOGICAL_NOT, predicate->place);
    // c2 = new If_Goto_TAC_Statement(t1, l1);
    // c3 = new Goto_TAC_Statement(l1);
    // c4 = new Label_TAC_Statement(l1);

    // code = new Code();
    // code->append_list(predicate->code);
    // code->append_statement(c1);
    // code->append_statement(c2);
    // code->append_list(if_clause->code);
    // code->append_statement(c3);
    // code->append_statement(c4);
}

// If_Stmt_Ast::If_Stmt_Ast(If_Stmt_Ast *unmatched_if, Statement_Ast *_else_clause)
//     : Statement_Ast(), predicate(unmatched_if->predicate), if_clause(unmatched_if->if_clause), else_clause(_else_clause)
// {
//     if (unmatched_if->else_clause)
//     {
//         throw_SemanticError("Cannot have 2 else clauses!");
//     }
//     if (!elseca)
//         code = unmatched_if->code;
//     if (else_clause)
//     {
//         TAC_Statement *c0 = code->pop_statement();

//         TAC_Label *l2 = new TAC_Label();
//         Label_TAC_Statement *c5 = new Label_TAC_Statement(l2);
//         code->append_statement(c5);

//         code->append_list(else_clause->code);

//         code->append_statement(c0);
//     }
// }

Code *If_Stmt_Ast::codegen()
{
    Code *code = new Code();

    code->append_list(predicate->codegen());

    TAC_Operand *t = new Temporary_TAC_Operand();
    code->append_statement(new Assignment_TAC_Statement(t, Unary_Operator::LOGICAL_NOT, predicate->place));

    TAC_Label *l = new TAC_Label();
    code->append_statement(new If_Goto_TAC_Statement(t, l));

    code->append_list(if_clause->code);
}

std::string If_Stmt_Ast::to_string() const
{
    std::string result = "If:\nCondition (" + predicate->to_string() + ")\nThen (" + if_clause->to_string() + ")";
    if (else_clause)
    {
        result += "\nElse (" + else_clause->to_string() + ")";
    }
    return result;
}

While_Stmt_Ast::While_Stmt_Ast(Expression_Ast *_predicate, Statement_Ast *_body)
    : Statement_Ast(), predicate(_predicate), body(_body)
{
    if (predicate->type != Type::BOOL)
    {
        throw_SemanticError("Expected predicate of type BOOL, got: " + type_to_string(predicate->type));
        return;
    }

    Temporary_TAC_Operand *t1 = new Temporary_TAC_Operand();

    TAC_Label *l1 = new TAC_Label();
    TAC_Label *l2 = new TAC_Label();

    Label_TAC_Statement *c1 = new Label_TAC_Statement(l1);
    Assignment_TAC_Statement *c2 = new Assignment_TAC_Statement(t1, Unary_Operator::LOGICAL_NOT, predicate->place);
    If_Goto_TAC_Statement *c3 = new If_Goto_TAC_Statement(t1, l2);
    Goto_TAC_Statement *c4 = new Goto_TAC_Statement(l1);
    Label_TAC_Statement *c5 = new Label_TAC_Statement(l2);

    code = new Code();
    code->append_statement(c1);
    code->append_list(predicate->code);
    code->append_statement(c2);
    code->append_statement(c3);
    code->append_list(body->code);
    code->append_statement(c4);
    code->append_statement(c5);
}

std::string While_Stmt_Ast::to_string() const
{
    std::string result = "While:\nCondition (" + predicate->to_string() + ")\nBody (\n" + body->to_string() + ")";
    return result;
}

Do_While_Stmt_Ast::Do_While_Stmt_Ast(Expression_Ast *predicate, Statement_Ast *body)
    : Statement_Ast(), predicate(predicate), body(body)
{
    if (predicate->type != Type::BOOL)
    {
        throw_SemanticError("Expected predicate of type BOOL, got: " + type_to_string(predicate->type));
        return;
    }
    TAC_Label *l1 = new TAC_Label();
    // Temporary_TAC_Operand *t1 = new Temporary_TAC_Operand();
    Label_TAC_Statement *c1 = new Label_TAC_Statement(l1);
    If_Goto_TAC_Statement *c2 = new If_Goto_TAC_Statement(predicate->place, l1);

    code = new Code();
    code->append_statement(c1);
    code->append_list(body->code);
    code->append_list(predicate->code);
    code->append_statement(c2);
}

std::string Do_While_Stmt_Ast::to_string() const
{
    std::string result = "Do:\nBody (\n" + body->to_string() + ")\nWhile Condition (" + predicate->to_string() + ")";
    return result;
}

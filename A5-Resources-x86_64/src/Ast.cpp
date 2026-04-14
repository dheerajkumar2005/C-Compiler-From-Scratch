#include "Ast.hpp"

Ast::Ast(Scope *_eval_scope) : code(nullptr), eval_scope(_eval_scope)
{
}

Ast::~Ast()
{
}

Code *Ast::get_code()
{
    if (!code)
    {
        code = codegen();
    }

    return code;
}

Expression_Ast::Expression_Ast(Type type, Scope *_eval_scope)
    : Ast(_eval_scope), type(type), place(nullptr)
{
}

Expression_Ast::~Expression_Ast()
{
}

Type Expression_Ast::get_type() const
{
    return type;
}

Base_Expr_Ast::Base_Expr_Ast(Type type, Scope *_eval_scope)
    : Expression_Ast(type, _eval_scope)
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

    place = new Variable_TAC_Operand(type, id, declaring_scope);
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
    this->place = new String_Const_TAC_Operand(_sval);
}

Code *String_Expr_Ast::codegen()
{
    return nullptr;
}

std::string String_Expr_Ast::to_string() const
{
    return "String : " + sval + "<" + type_to_string(get_type()) + ">";
}

Function_Call_Ast::Function_Call_Ast(std::string name, Func_Signature *sig, std::vector<Expression_Ast *> *a, Scope *_eval_scope)
    : Base_Expr_Ast(sig->return_type, _eval_scope), func_name(name), func_sig(sig), args(a)
{
    if (func_name != func_sig->name)
    {
        throw_SemanticError("Expected same function name in both places");
    }
}

Code *Function_Call_Ast::codegen()
{
    Code *result = new Code();

    Type return_type = func_sig->return_type;
    if (return_type != Type::VOID)
    {
        place = new Temporary_TAC_Operand(return_type);
    }

    std::vector<TAC_Operand *> operands;
    for (Expression_Ast *arg : *args)
    {
        result->append_list(arg->get_code());
        operands.push_back(arg->place);
    }

    result->append_statement(new Call_TAC_Statement(eval_scope, func_name, operands, place));
    return result;
}

std::string Function_Call_Ast::to_string() const
{
    std::string result = "FN CALL: " + func_name + "(\n";
    for (Expression_Ast *arg : *args)
    {
        result += arg->to_string();
    }
    result += ")";
    return result;
}

Unary_Expr_AST::Unary_Expr_AST(Type type, Unary_Operator op, Expression_Ast *opd1, Scope *_eval_scope)
    : Expression_Ast(type, _eval_scope), op(op), opd1(opd1)
{
}

Unary_Expr_AST::~Unary_Expr_AST()
{
}

Code *Unary_Expr_AST::codegen()
{
    Code *code = new Code();

    code->append_list(opd1->get_code());

    place = new Temporary_TAC_Operand(type);
    code->append_statement(new Assignment_TAC_Statement(eval_scope, place, op, opd1->place));

    return code;
}

UMinus_Expr_Ast::UMinus_Expr_Ast(Expression_Ast *opd1, Scope *_eval_scope)
    : Unary_Expr_AST(opd1->get_type(), Unary_Operator::NEGATE, opd1, _eval_scope)
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

Logical_Not_Expr_Ast::Logical_Not_Expr_Ast(Expression_Ast *opd1, Scope *_eval_scope)
    : Unary_Expr_AST(Type::BOOL, Unary_Operator::LOGICAL_NOT, opd1, _eval_scope)
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

Binary_Expr_Ast::Binary_Expr_Ast(Type type, Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2, Scope *_eval_scope)
    : Expression_Ast(type, _eval_scope), op(op), opd1(opd1), opd2(opd2)
{
}

Binary_Expr_Ast::~Binary_Expr_Ast()
{
}

Code *Binary_Expr_Ast::codegen()
{
    Code *code = new Code();

    // Can evaluate in any order
    code->append_list(opd1->get_code());

    code->append_list(opd2->get_code());

    place = new Temporary_TAC_Operand(type);
    code->append_statement(new Assignment_TAC_Statement(eval_scope, place, op, opd1->place, opd2->place));

    return code;
}

bool are_same_type(Expression_Ast *opd1, Expression_Ast *opd2)
{
    return opd1->get_type() == opd2->get_type();
}

Boolean_Expr_Ast::Boolean_Expr_Ast(Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2, Scope *_eval_scope)
    : Binary_Expr_Ast(Type::BOOL, op, opd1, opd2, _eval_scope)
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

Div_Expr_Ast::Div_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2, Scope *_eval_scope)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::DIVIDE, opd1, opd2, _eval_scope)
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

Minus_Expr_Ast::Minus_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2, Scope *_eval_scope)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::SUBTRACT, opd1, opd2, _eval_scope)
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

Mult_Expr_Ast::Mult_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2, Scope *_eval_scope)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::MULTIPLY, opd1, opd2, _eval_scope)
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

Plus_Expr_Ast::Plus_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2, Scope *_eval_scope)
    : Binary_Expr_Ast(opd1->get_type(), Binary_Operator::ADD, opd1, opd2, _eval_scope)
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

Relational_Expr_Ast::Relational_Expr_Ast(Binary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2, Scope *_eval_scope)
    : Binary_Expr_Ast(Type::BOOL, op, opd1, opd2, _eval_scope)
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

Ternary_Expr_Ast::Ternary_Expr_Ast(Type type, Ternary_Operator op, Expression_Ast *opd1, Expression_Ast *opd2, Expression_Ast *opd3, Scope *_eval_scope)
    : Expression_Ast(type, _eval_scope), op(op), opd1(opd1), opd2(opd2), opd3(opd3)
{
}

Ternary_Expr_Ast::~Ternary_Expr_Ast()
{
}

Conditional_Expr_Ast::Conditional_Expr_Ast(Expression_Ast *opd1, Expression_Ast *opd2, Expression_Ast *opd3, Scope *_eval_scope)
    : Ternary_Expr_Ast(opd2->get_type(), Ternary_Operator::QUESTION_MARK_COLON, opd1, opd2, opd3, _eval_scope)
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
    Code *predicate_expr = opd1->get_code();

    place = new Shared_Temporary_TAC_Operand(opd2->type);
    eval_scope->add_local(place->type, place->to_string());

    TAC_Label *l1 = new TAC_Label();
    TAC_Label *l2 = new TAC_Label();

    Code *then_expr = opd2->get_code();
    Code *else_expr = opd3->get_code();

    Temporary_TAC_Operand *t1 = new Temporary_TAC_Operand(opd1->type);

    Code *code = new Code();
    code->append_list(predicate_expr);
    code->append_statement(new Assignment_TAC_Statement(eval_scope, t1, Unary_Operator::LOGICAL_NOT, opd1->place));
    code->append_statement(new If_Goto_TAC_Statement(eval_scope, t1, l1));
    code->append_list(then_expr);
    code->append_statement(new Assignment_TAC_Statement(eval_scope, place, opd2->place));
    code->append_statement(new Goto_TAC_Statement(l2));
    code->append_statement(new Label_TAC_Statement(l1));
    code->append_list(else_expr);
    code->append_statement(new Assignment_TAC_Statement(eval_scope, place, opd3->place));
    code->append_statement(new Label_TAC_Statement(l2));

    return code;
}

std::string Conditional_Expr_Ast::to_string() const
{
    return opd1->to_string() + "\nTrue_Part (" + opd2->to_string() + ")\nFalse_Part (" + opd3->to_string() + ")";
}

Statement_Ast::Statement_Ast(Scope *_eval_scope)
    : Ast(_eval_scope)
{
}

Statement_Ast::~Statement_Ast()
{
}

RTL_Code *Statement_Ast::rtlgen(RegisterTracker *register_tracker)
{
    RTL_Code *rtl_code = new RTL_Code();
    for (auto tac_stmt_ptr : *(get_code()->stmt_list))
    {
        rtl_code->append_list(tac_stmt_ptr->to_rtl(register_tracker));
    }

    return rtl_code;
}

RTL_Code *Statement_Ast::get_rtl(RegisterTracker *register_tracker)
{
    if (!rtl_code)
    {
        rtl_code = rtlgen(register_tracker);
    }

    return rtl_code;
}

Assignment_Stmt_Ast::Assignment_Stmt_Ast(Scope *_eval_scope, Name_Expr_Ast *lhs, Expression_Ast *rhs)
    : Statement_Ast(_eval_scope), lhs(lhs), rhs(rhs)
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
    Code *code = new Code();

    code->append_list(rhs->get_code());

    Variable_TAC_Operand *id = new Variable_TAC_Operand(lhs->type, &lhs->var_name, lhs->declaring_scope);
    code->append_statement(new Assignment_TAC_Statement(eval_scope, id, rhs->place));

    return code;
}

std::string Assignment_Stmt_Ast::to_string() const
{
    return "Asgn:\nLHS (" + lhs->to_string() + ")\nRHS (" + rhs->to_string() + ")";
}

Read_Stmt_Ast::Read_Stmt_Ast(Scope *_eval_scope, Name_Expr_Ast *var)
    : Statement_Ast(_eval_scope), var(var)
{
    if (var->get_type() != Type::INT && var->get_type() != Type::FLOAT)
    {
        throw_SemanticError("can read only int or float");
        return;
    }
}

Code *Read_Stmt_Ast::codegen()
{
    Code *code = new Code();

    Variable_TAC_Operand *id = new Variable_TAC_Operand(var->type, &var->var_name, var->declaring_scope);
    code->append_statement(new IO_TAC_Statement(eval_scope, IO_Kind::READ, id));

    return code;
}

std::string Read_Stmt_Ast::to_string() const
{
    return "Read: " + var->to_string();
}

Write_Stmt_Ast::Write_Stmt_Ast(Scope *_eval_scope, Expression_Ast *expr)
    : Statement_Ast(_eval_scope), expr(expr)
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
    Code *code = new Code();

    code->append_list(expr->get_code());

    code->append_statement(new IO_TAC_Statement(eval_scope, IO_Kind::WRITE, expr->place));

    return code;
}

std::string Write_Stmt_Ast::to_string() const
{
    return "Write: " + expr->to_string();
}

Compound_Stmt_Ast::Compound_Stmt_Ast(Scope *_eval_scope, std::vector<Statement_Ast *> *stmts)
    : Statement_Ast(_eval_scope), stmts(stmts)
{
}

Code *Compound_Stmt_Ast::codegen()
{
    Code *code = new Code();
    for (auto stmt : *stmts)
    {
        code->append_list(stmt->get_code());
    }

    return code;
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

If_Stmt_Ast::If_Stmt_Ast(Scope *_eval_scope, Expression_Ast *predicate, Statement_Ast *if_clause, Statement_Ast *else_clause)
    : Statement_Ast(_eval_scope), predicate(predicate), if_clause(if_clause), else_clause(else_clause)
{
    if (predicate->type != Type::BOOL)
    {
        throw_SemanticError("Expected predicate of type BOOL, got: " + type_to_string(predicate->type));
        return;
    }
}

Code *If_Stmt_Ast::codegen()
{
    Code *code = new Code();

    code->append_list(predicate->get_code());

    Code *if_clause_code = if_clause->get_code();

    TAC_Operand *t = new Temporary_TAC_Operand(Type::BOOL);
    code->append_statement(new Assignment_TAC_Statement(eval_scope, t, Unary_Operator::LOGICAL_NOT, predicate->place));

    if (else_clause)
    {
        TAC_Label *l_end = new TAC_Label();
        TAC_Label *l_false = new TAC_Label();

        code->append_statement(new If_Goto_TAC_Statement(eval_scope, t, l_false));

        code->append_list(if_clause_code);

        code->append_statement(new Goto_TAC_Statement(l_end));

        code->append_statement(new Label_TAC_Statement(l_false));

        code->append_list(else_clause->get_code());

        code->append_statement(new Label_TAC_Statement(l_end));
    }
    else
    {
        TAC_Label *l_false = new TAC_Label();
        code->append_statement(new If_Goto_TAC_Statement(eval_scope, t, l_false));

        code->append_list(if_clause_code);

        code->append_statement(new Goto_TAC_Statement(l_false));
        code->append_statement(new Label_TAC_Statement(l_false));
    }

    return code;
}

std::string If_Stmt_Ast::to_string() const
{
    std::string result = "If:\nCondition (" + predicate->to_string() + ")\nThen (\n" + if_clause->to_string() + ")";
    if (else_clause)
    {
        result += "\nElse (\n" + else_clause->to_string() + ")";
    }
    return result;
}

While_Stmt_Ast::While_Stmt_Ast(Scope *_eval_scope, Expression_Ast *_predicate, Statement_Ast *_body)
    : Statement_Ast(_eval_scope), predicate(_predicate), body(_body)
{
    if (predicate->type != Type::BOOL)
    {
        throw_SemanticError("Expected predicate of type BOOL, got: " + type_to_string(predicate->type));
        return;
    }
}

Code *While_Stmt_Ast::codegen()
{
    Code *predicate_code = predicate->get_code();
    Code *body_code = body->get_code();

    Code *code = new Code();

    TAC_Label *l1 = new TAC_Label();
    code->append_statement(new Label_TAC_Statement(l1));

    code->append_list(predicate_code);

    Temporary_TAC_Operand *t1 = new Temporary_TAC_Operand(Type::BOOL);
    code->append_statement(new Assignment_TAC_Statement(eval_scope, t1, Unary_Operator::LOGICAL_NOT, predicate->place));

    TAC_Label *l2 = new TAC_Label();
    code->append_statement(new If_Goto_TAC_Statement(eval_scope, t1, l2));

    code->append_list(body_code);

    code->append_statement(new Goto_TAC_Statement(l1));

    code->append_statement(new Label_TAC_Statement(l2));

    return code;
}

std::string While_Stmt_Ast::to_string() const
{
    std::string result = "While:\nCondition (" + predicate->to_string() + ")\nBody (\n" + body->to_string() + ")";
    return result;
}

Do_While_Stmt_Ast::Do_While_Stmt_Ast(Scope *_eval_scope, Expression_Ast *predicate, Statement_Ast *body)
    : Statement_Ast(_eval_scope), predicate(predicate), body(body)
{
    if (predicate->type != Type::BOOL)
    {
        throw_SemanticError("Expected predicate of type BOOL, got: " + type_to_string(predicate->type));
        return;
    }
}

Code *Do_While_Stmt_Ast::codegen()
{
    Code *body_code = body->get_code();
    Code *predicate_code = predicate->get_code();

    Code *code = new Code();

    TAC_Label *l1 = new TAC_Label();
    code->append_statement(new Label_TAC_Statement(l1));

    code->append_list(body_code);

    code->append_list(predicate_code);

    code->append_statement(new If_Goto_TAC_Statement(eval_scope, predicate->place, l1));

    return code;
}

std::string Do_While_Stmt_Ast::to_string() const
{
    std::string result = "Do:\nBody (\n" + body->to_string() + ")\nWhile Condition (" + predicate->to_string() + ")";
    return result;
}

Return_Stmt_Ast::Return_Stmt_Ast(Scope *_eval_scope, Expression_Ast *_expression, TAC_Label *_return_label, Shared_Temporary_TAC_Operand *_return_stemp)
    : Statement_Ast(_eval_scope), expression(_expression), return_label(_return_label), return_stemp(_return_stemp)
{
}

Code *Return_Stmt_Ast::codegen()
{
    Code *code = new Code();
    code->append_list(expression->get_code());
    code->append_statement(new Assignment_TAC_Statement(eval_scope, return_stemp, expression->place));
    code->append_statement(new Goto_TAC_Statement(return_label));
    return code;
}

std::string Return_Stmt_Ast::to_string() const
{
    return "Return: " + expression->to_string();
}

Call_Stmt_Ast::Call_Stmt_Ast(Scope *_eval_scope, Function_Call_Ast *call)
    : Statement_Ast(_eval_scope), func_call(call)
{
}

Code *Call_Stmt_Ast::codegen()
{
    return func_call->get_code();
}

std::string Call_Stmt_Ast::to_string() const
{
    return func_call->to_string();
}

Function_Ast::Function_Ast(Func_Signature *_func_sig)
    : Statement_Ast(), func_sig(_func_sig), body(), return_label(nullptr), return_stemp(nullptr)
{
    Type return_type = func_sig->return_type;
    if (return_type != Type::VOID)
    {
        return_label = new TAC_Label();
        return_stemp = new Shared_Temporary_TAC_Operand(return_type);
    }
}

void Function_Ast::set_scope(Scope *_eval_scope)
{
    eval_scope = _eval_scope;
}

void Function_Ast::add_stmt(Statement_Ast *stmt)
{
    body.push_back(stmt);
}

Code *Function_Ast::codegen()
{
    Code *code = new Code();
    for (auto stmt : body)
    {
        code->append_list(stmt->get_code());
    }
    if (func_sig->return_type != Type::VOID)
    {
        code->append_statement(new Label_TAC_Statement(return_label));
        code->append_statement(new Return_TAC_Statement(eval_scope, return_stemp));
    }
    return code;
}

std::string Function_Ast::to_string() const
{
    std::string result;
    result += "**PROCEDURE: " + func_sig->name + "\n";
    result += "Return Type: <" + type_to_string(func_sig->return_type) + ">\n";
    result += "Formal Parameters: \n";

    int num_params = func_sig->param_types.size();
    for (int i = 0; i < num_params; i++)
    {
        result += func_sig->param_names[i] + "_ Type:<" + type_to_string(func_sig->param_types[i]) + ">\n";
    }

    result += "**BEGIN: Abstract Syntax Tree\n";
    for (auto stmt : body)
    {
        result += stmt->to_string() + "\n";
    }
    result += "**END: Abstract Syntax Tree\n";

    return result;
}

Function_Entry::Function_Entry(Type _return_type, Func_Signature *_func_sig)
    : Symbol_Table_Entry(Entry_Kind::FUNCTION, _return_type), func_sig(_func_sig), definition(new Function_Ast(_func_sig))
{
    if (!func_sig || _return_type != func_sig->return_type)
    {
        throw_SemanticError("Expected consistency of return types");
    }
}

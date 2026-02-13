#include "AST.hpp"
#include "SymTabEntry.hpp"

ExprAST::ExprAST(Type type, Operator op, ExprAST *operand1, ExprAST *operand2, ExprAST *operand3)
    : AST(), type(type), op(op), operand1(operand1), operand2(operand2), operand3(operand3)
{
    // TODO: Add type checking (semantic analysis) code here
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

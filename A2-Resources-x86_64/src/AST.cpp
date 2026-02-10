#include "AST.hpp"
#include "SymTabEntry.hpp"

ExprAST::ExprAST(Type type = Type::UNKNOWN, Operator op = Operator::NOP, ExprAST *operand1 = nullptr, ExprAST *operand2 = nullptr, ExprAST *operand3 = nullptr)
    : AST(), type(type), op(op), operand1(operand1), operand2(operand2), operand3(operand3)
{
    // TODO: Add type checking (semantic analysis) code here 
}

void VarAST::set_sym_tab_entry_ptr(SymTabEntry *ste_ptr) {
    sym_tab_entry_ptr = ste_ptr;
    type = sym_tab_entry_ptr->type;
}

VarAST::VarAST(char *var_name) : ExprAST(), sym_tab_entry_ptr(nullptr) {
    std::string key(var_name);

    // Check local scope first, then global scope
    auto it_local = SymTabEntry::local_scope.find(key);
    if(it_local != SymTabEntry::local_scope.end()) {
        set_sym_tab_entry_ptr(it_local->second);
        return;
    }

    auto it_global = SymTabEntry::global_scope.find(key);
    if(it_global != SymTabEntry::global_scope.end()) {
        set_sym_tab_entry_ptr(it_global->second);
        return;
    }
}

AssignAST::AssignAST() {
    
}

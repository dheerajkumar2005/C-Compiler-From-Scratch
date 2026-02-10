#include "AST.hpp"
#include "SymTabEntry.hpp"

VarAST::set_sym_tab_entry_ptr(SymTabEntry *ste_ptr) {
    sym_tab_entry_ptr = ste_ptr;
    type = sym_tab_entry_ptr->type;
}

VarAST::VarAST(char *var_name) : RValueAST(), sym_tab_entry_ptr(nullptr) {
    std::string key(var_name);

    // Check local scope first, then global scope
    auto it_local = SymTabEntry::local_scope.find(key);
    if(it_local != SymTabEntry::local_scope.end()) {
        set_sym_tab_entry_ptr(it->second);
        return;
    }

    auto it_global = SymTabEntry::global_scope.find(key);
    if(it_global != SymTabEntry::global_scope.end()) {
        set_sym_tab_entry_ptr(it->second);
        return;
    }
}

#include "support.hpp"

IdentifierList *accumulate_var_decl_item_list(std::string *identifier)
{
    return new std::vector<std::string *>{identifier};
}

IdentifierList *accumulate_var_decl_item_list(IdentifierList *identifiers, std::string *identifier)
{
    identifiers->push_back(identifier);
    return identifiers;
}

void process_var_decl_stmt(Scope *curr_scope, Type type, IdentifierList *identifiers)
{
    auto &sym_tab = curr_scope->sym_tab;
    for (const auto &id : *identifiers)
    {
        // Cannot clash with the function name
        if (curr_scope->kind == Scope_Kind::FUNCTION && *id == curr_scope->func_sig->name)
        {
            throw_SemanticError("Local variable name matches function name: " + *id);
        }

        // No local variable or params with the same name
        if (sym_tab.find(*id) != sym_tab.end())
        {
            throw_SemanticError("Local variable name matches previously declared variable: " + *id);
        }

        sym_tab[*id] = new Symbol_Table_Entry(Entry_Kind::VARIABLE, type);
    }
}

Name_Expr_Ast *process_variable_name(Scope *curr_scope, std::string *id)
{
    while (curr_scope)
    {
        auto &sym_tab = curr_scope->sym_tab;
        if (sym_tab.find(*id) != sym_tab.end() && sym_tab[*id]->kind != Entry_Kind::FUNCTION)
        {
            return new Name_Expr_Ast(id, curr_scope, sym_tab[*id]->type);
        }

        curr_scope = curr_scope->parent_scope;
    }

    throw_SemanticError("Expected variable declaration before usage: " + *id);
    return nullptr;
}

FormalParam *accumulate_formal_param(Type type, std::string *id)
{
    return new std::pair<Type, std::string *>(type, id);
}

FormalParamList *accumulate_formal_param_list(FormalParam *formal_param)
{
    return new std::vector<FormalParam *>{formal_param};
}

FormalParamList *accumulate_formal_param_list(FormalParamList *formal_param_list, FormalParam *formal_param)
{
    formal_param_list->push_back(formal_param);
    return formal_param_list;
}

Scope *make_func_scope(Scope *curr_scope, Func_Signature *func_sig)
{
    Scope *next_scope = new Scope(Scope_Kind::FUNCTION, curr_scope, func_sig);

    // Add the params to the new symtab
    const auto &param_types = func_sig->param_types;
    const auto &param_names = func_sig->param_names;
    const int num_params = param_types.size();

    auto &sym_tab = next_scope->sym_tab;
    for (int i = 0; i < num_params; i++)
    {
        Symbol_Table_Entry *ste = new Symbol_Table_Entry(Entry_Kind::PARAMETER, param_types[i]);

        // No other params with the same name
        if (sym_tab.find(param_names[i]) != sym_tab.end())
        {
            throw_SemanticError("Param name matches previous param: " + param_names[i]);
        }

        sym_tab[param_names[i]] = ste;
    }

    return next_scope;
}

Func_Signature *get_func_sig(std::string func_name, Type return_type, FormalParamList *formal_param_list)
{
    Func_Signature *func_sig = new Func_Signature(func_name, return_type);
    if (formal_param_list)
    {
        for (const auto &formal_param : *formal_param_list)
        {
            func_sig->add_param(*(formal_param->second), formal_param->first);
        }
    }

    return func_sig;
}

void process_func_decl(Scope *curr_scope, FuncHeader *func_header, FormalParamList *formal_param_list)
{
    std::string func_name = *(func_header->second);
    Type return_type = func_header->first;

    // This name does not already exist in this scope
    // The grammar ensures that functions only exist in the global scope
    // NOTE: We don't allow function overloading
    auto &sym_tab = curr_scope->sym_tab;
    if (sym_tab.find(func_name) != sym_tab.end())
    {
        throw_SemanticError("Expected single declaration of variable/function with name: " + func_name);
    }

    Func_Signature *func_sig = get_func_sig(func_name, return_type, formal_param_list);

    // Add it to the symbol table
    sym_tab[func_name] = new Symbol_Table_Entry(Entry_Kind::FUNCTION, return_type, func_sig);
}

Func_Signature *process_func_def(Scope *curr_scope, FuncHeader *func_header, FormalParamList *formal_param_list)
{
    std::string func_name = *(func_header->second);
    Type return_type = func_header->first;
    Func_Signature *func_sig = get_func_sig(func_name, return_type, formal_param_list);

    // If no symtab entry exists, add it
    // Otherwise, check if the function signatures match
    auto &sym_tab = curr_scope->sym_tab;
    if (sym_tab.find(func_name) != sym_tab.end())
    {
        Symbol_Table_Entry *existing_func = sym_tab[func_name];
        if (*(existing_func->func_sig) != *func_sig)
        {
            throw_SemanticError("Expected function definition to match declaration: " + func_name);
        }
    }
    else
    {
        process_func_decl(curr_scope, func_header, formal_param_list);
    }

    return func_sig;
}

StatementList *accumulate_stmt_list(StatementList *stmt_list, Statement_Ast *stmt)
{
    stmt_list->push_back(stmt);
    return stmt_list;
}

StatementList *accumulate_stmt_list()
{
    return new std::vector<Statement_Ast *>();
}

void ast_print_func_sig(Scope *func)
{
    if (func->kind != Scope_Kind::FUNCTION)
    {
        throw_SemanticError("Not a function!");
    }

    Func_Signature *func_signature = func->func_sig;

    std::string func_name = func_signature->name;
    Type return_type = func_signature->return_type;
    std::vector<std::string> param_names = func_signature->param_names;
    std::vector<Type> param_types = func_signature->param_types;

    *astout << "**PROCEDURE: " << func_name << std::endl;
    *astout << "Return Type: <" << return_type << ">" << std::endl;
    *astout << "Formal Parameters: " << std::endl;
    for (int i = 0; i < param_names.size(); i++)
    {
        *astout << param_names[i] << "_ Type:<" << param_types[i] << ">" << std::endl;
    }
}

void ast_print_stmt_list(StatementList *stmt_list)
{
    *astout << "**BEGIN: Abstract Syntax Tree" << std::endl;
    if (stmt_list)
    {
        for (auto stmt_ast : *stmt_list)
        {
            if (stmt_ast)
            {
                *astout << stmt_ast->to_string() << std::endl;
            }
        }
    }
    *astout << "**END: Abstract Syntax Tree" << std::endl;
}

void tac_print_func_sig(Scope *func)
{
    if (func->kind != Scope_Kind::FUNCTION)
    {
        throw_SemanticError("Not a function!");
    }

    Func_Signature *func_signature = func->func_sig;

    std::string func_name = func_signature->name;
    Type return_type = func_signature->return_type;
    std::vector<std::string> param_names = func_signature->param_names;
    std::vector<Type> param_types = func_signature->param_types;

    *tacout << "**PROCEDURE: " << func_name << std::endl;
}

void tac_print_stmt_list(StatementList *stmt_list)
{
    *tacout << "**BEGIN: Three Address Code Statements" << std::endl;
    if (stmt_list)
    {
        for (auto stmt_ast : *stmt_list)
        {
            if (stmt_ast && stmt_ast->code)
            {
                *tacout << stmt_ast->code->to_string() << std::endl;
            }
        }
    }
    *tacout << "**END: Three Address Code Statements" << std::endl;
}

#include "support.hpp"

IdentifierList *accumulate_var_decl_item_list(std::string *identifier)
{
    if (!sa_parse)
    {
        return new std::vector<std::string *>{identifier};
    }
    return nullptr;
}

IdentifierList *accumulate_var_decl_item_list(IdentifierList *identifiers, std::string *identifier)
{
    if (!sa_parse)
    {
        identifiers->push_back(identifier);
        return identifiers;
    }
    return nullptr;
}

void process_var_decl_stmt(Scope *curr_scope, Type type, IdentifierList *identifiers)
{
    if (!sa_parse)
    {
        auto &sym_tab = curr_scope->sym_tab;
        for (const auto &id : *identifiers)
        {
            curr_scope->add_local(type, *id);
        }
    }
}

Name_Expr_Ast *process_variable_name(Scope *curr_scope, std::string *id)
{
    if (!sa_parse)
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
    }
    return nullptr;
}

FormalParam *accumulate_formal_param(Type type, std::string *id)
{
    if (!sa_parse)
    {
        return new std::pair<Type, std::string *>(type, id);
    }
    return nullptr;
}

FormalParam *accumulate_func_header(Type type, std::string *id)
{
    if (!sa_parse)
    {
        // For some unfathomable reason, functions other than main are appended with `_` in the name
        // Like what even is this shit bro
        if (*id != "main")
        {
            *id += "_";
        }
        return new std::pair<Type, std::string *>(type, id);
    }
    return nullptr;
}

FormalParamList *accumulate_formal_param_list(FormalParam *formal_param)
{
    if (!sa_parse)
    {
        return new std::vector<FormalParam *>{formal_param};
    }
    return nullptr;
}

FormalParamList *accumulate_formal_param_list(FormalParamList *formal_param_list, FormalParam *formal_param)
{
    if (!sa_parse)
    {
        formal_param_list->push_back(formal_param);
        return formal_param_list;
    }
    return nullptr;
}

Scope *make_func_scope(Scope *curr_scope, Func_Signature *func_sig)
{
    if (!sa_parse)
    {
        Scope *next_scope = new Scope(Scope_Kind::FUNCTION, curr_scope, func_sig);

        // Add the params to the new symtab
        const auto &param_types = func_sig->param_types;
        const auto &param_names = func_sig->param_names;
        const int num_params = param_types.size();

        auto &sym_tab = next_scope->sym_tab;
        for (int i = 0; i < num_params; i++)
        {
            next_scope->add_param(param_types[i], param_names[i]);
        }

        return next_scope;
    }
    return nullptr;
}

Func_Signature *get_func_sig(std::string func_name, Type return_type, FormalParamList *formal_param_list)
{
    if (!sa_parse)
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
    return nullptr;
}

void process_func_decl(Scope *curr_scope, FuncHeader *func_header, FormalParamList *formal_param_list)
{
    if (!sa_parse)
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
            return;
        }

        // All kinds of functions are allowed

        Func_Signature *func_sig = get_func_sig(func_name, return_type, formal_param_list);

        // Add it to the symbol table
        sym_tab[func_name] = new Symbol_Table_Entry(func_sig);
    }
}

Func_Signature *make_func_sig(Scope *curr_scope, FuncHeader *func_header, FormalParamList *formal_param_list)
{
    if (!sa_parse)
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
                return nullptr;
            }
        }
        else
        {
            process_func_decl(curr_scope, func_header, formal_param_list);
        }

        return func_sig;
    }
    return nullptr;
}

Expression_Ast *process_predicate(Expression_Ast *expr)
{
    if (!sa_parse)
    {
        if (expr->type == Type::BOOL)
        {
            return expr;
        }
    }
    return nullptr;
}

If_Stmt_Ast *add_else_clause(If_Stmt_Ast *unmatched_if, Statement_Ast *else_clause)
{
    if (unmatched_if->else_clause || !else_clause)
    {
        throw_SemanticError("Expected unmatched_if to not have an else clause and else_clause to be non-null");
    }

    unmatched_if->else_clause = else_clause;
    return unmatched_if;
}

StatementList *accumulate_stmt_list(StatementList *stmt_list, Statement_Ast *stmt)
{
    if (!sa_parse)
    {
        stmt_list->push_back(stmt);
        return stmt_list;
    }
    return nullptr;
}

StatementList *accumulate_stmt_list()
{
    if (!sa_parse)
    {
        return new std::vector<Statement_Ast *>();
    }
    return nullptr;
}

bool is_empty(StatementList *stmt_list)
{
    return !stmt_list || stmt_list->empty();
}

std::string ast_print_func_sig(Scope *func)
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

    std::string result;
    result += "**PROCEDURE: " + func_name + "\n";
    result += "Return Type: <" + type_to_string(return_type) + ">\n";
    result += "Formal Parameters: \n";
    for (int i = 0; i < param_names.size(); i++)
    {
        result += param_names[i] + "_ Type:<" + type_to_string(param_types[i]) + ">\n";
    }
    return result;
}

std::string ast_print_stmt_list(StatementList *stmt_list)
{
    std::string result;
    result += "**BEGIN: Abstract Syntax Tree\n";
    if (stmt_list)
    {
        for (auto stmt_ast : *stmt_list)
        {
            if (stmt_ast)
            {
                result += stmt_ast->to_string() + "\n";
            }
        }
    }
    result += "**END: Abstract Syntax Tree\n";
    return result;
}

std::string *ast_print_func(Scope *func, StatementList *stmt_list)
{
    std::string *ptr = new std::string;
    *ptr += ast_print_func_sig(func);
    *ptr += ast_print_stmt_list(stmt_list);
    return ptr;
}

std::string tac_print_func_sig(Scope *func)
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

    std::string result;
    result += "**PROCEDURE: " + func_name + "\n";
    return result;
}

std::string tac_print_stmt_list(StatementList *stmt_list)
{
    std::string result;
    result += "**BEGIN: Three Address Code Statements\n";
    if (stmt_list)
    {
        for (auto stmt_ast : *stmt_list)
        {
            if (stmt_ast)
            {
                Code *code = stmt_ast->get_code();
                if (code)
                {
                    result += code->to_string() + "\n";
                }
            }
        }
    }
    result += "**END: Three Address Code Statements\n";
    return result;
}

std::string *tac_print_func(Scope *func, StatementList *stmt_list)
{
    std::string *ptr = new std::string;
    *ptr += tac_print_func_sig(func);
    *ptr += tac_print_stmt_list(stmt_list);
    return ptr;
}

std::string rtl_print_func_sig(Scope *func)
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

    std::string result;
    result += "**PROCEDURE: " + func_name + "\n";
    return result;
}

std::string rtl_print_stmt_list(StatementList *stmt_list, RegisterTracker *register_tracker)
{
    std::string result;
    result += "**BEGIN: RTL Statements\n";
    if (stmt_list)
    {
        for (auto stmt_ast : *stmt_list)
        {
            if (stmt_ast)
            {
                RTL_Code *rtl_code = stmt_ast->get_rtl(register_tracker);
                if (rtl_code)
                {
                    result += rtl_code->to_string() + "\n";
                }
            }
        }
    }
    result += "**END: RTL Statements\n";
    return result;
}

std::string *rtl_print_func(Scope *func, StatementList *stmt_list, RegisterTracker *register_tracker)
{
    std::string *ptr = new std::string;
    *ptr += rtl_print_func_sig(func);
    *ptr += rtl_print_stmt_list(stmt_list, register_tracker);

    return ptr;
}

FunctionDefinition *process_func_def(std::string *func_name, std::string *ast, std::string *tac, std::string *rtl)
{
    if (!sa_parse)
    {
        return new std::vector<std::string *>{func_name, ast, tac, rtl};
    }
    return nullptr;
}

FunctionDefinitionList *accumulate_func_def(FunctionDefinitionList *func_def_list, FunctionDefinition *func_def)
{
    if (!sa_parse)
    {
        func_def_list->push_back(func_def);
        return func_def_list;
    }
    return nullptr;
}

FunctionDefinitionList *accumulate_func_def(FunctionDefinition *func_def)
{
    if (!sa_parse)
    {
        return new std::vector<FunctionDefinition *>{func_def};
    }
    return nullptr;
}

void print_func_def_list(FunctionDefinitionList *func_def_list_ptr, int show_ast, int show_tac, int show_rtl)
{
    std::vector<std::pair<std::string, std::vector<std::string *>>> func_def_list;
    for (auto func_def_ptr : *func_def_list_ptr)
    {
        std::vector<std::string *> func_def;
        for (int i = 1; i < (*func_def_ptr).size(); i++)
        {
            func_def.push_back((*func_def_ptr)[i]);
        }
        std::string func_name = *((*func_def_ptr)[0]);
        func_def_list.emplace_back(func_name, func_def);
    }

    // Sort by function name
    std::sort(func_def_list.begin(), func_def_list.end());

    if (show_ast)
    {
        for (const auto &[_, func_def] : func_def_list)
        {
            *astout << *(func_def[0]);
        }
    }

    if (show_tac)
    {
        for (const auto &[_, func_def] : func_def_list)
        {
            *tacout << *(func_def[1]);
        }
    }

    if (show_rtl)
    {
        for (const auto &[_, func_def] : func_def_list)
        {
            *rtlout << *(func_def[2]);
        }
    }
}

#include "support.hpp"

TAC_Label *return_label = nullptr;
Shared_Temporary_TAC_Operand *return_stemp = nullptr;
Scope* curr_scope = new Scope(Scope_Kind::GLOBAL);

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

void process_var_decl_stmt(Type type, IdentifierList *identifiers)
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

Name_Expr_Ast *process_variable_name(std::string *id)
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

ActualParamList *accumulate_actual_param_list(ActualParam *arg)
{
    return new std::vector<ActualParam *>{arg};
}

ActualParamList *accumulate_actual_param_list(ActualParamList *args, ActualParam *arg)
{
    args->push_back(arg);
    return args;
}

Scope *make_func_scope(Func_Signature *func_sig)
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

void process_func_decl(FuncHeader *func_header, FormalParamList *formal_param_list)
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
        // This will immediately allocate a return label and stemp for non-void functions
        sym_tab[func_name] = new Function_Entry(return_type, func_sig);
    }
}

// TODO
Func_Signature *make_func_sig(FuncHeader *func_header, FormalParamList *formal_param_list)
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
            Function_Entry *existing_func = dynamic_cast<Function_Entry *>(sym_tab[func_name]);
            if (!existing_func || *(existing_func->func_sig) != *func_sig)
            {
                throw_SemanticError("Expected function definition to match declaration: " + func_name);
                return nullptr;
            }
        }
        else
        {
            process_func_decl(func_header, formal_param_list);
        }

        // Setup the return label and stemp
        Function_Entry *fe = dynamic_cast<Function_Entry *>(sym_tab[func_name]);
        return_label = fe->definition->return_label;
        return_stemp = fe->definition->return_stemp;

        return func_sig;
    }
    return nullptr;
}

void process_body(Scope *parent_scope, StatementList *body)
{
    const std::string &func_name = curr_scope->func_sig->name;
    auto &sym_tab = parent_scope->sym_tab;
    if (sym_tab.find(func_name) == sym_tab.end())
    {
        throw_SemanticError("Expected to see child function in the symtab of parent function");
        return;
    }

    auto entry = dynamic_cast<Function_Entry *>(sym_tab[func_name]);
    if (!entry)
    {
        throw_SemanticError("Expected to see entry of type function: " + func_name);
    }

    for (auto stmt : *body)
    {
        entry->definition->add_stmt(stmt);
    }
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

Function_Call_Ast *process_func_call(std::string *name_ptr, ActualParamList *args)
{
    std::string name = *name_ptr;
    if (name != "main")
    {
        name += "_";
    }

    // NOTE: Since I know that the function being called should be in the global scope
    Scope *parent_scope = curr_scope->parent_scope;
    if (!parent_scope || parent_scope->kind != Scope_Kind::GLOBAL)
    {
        throw_SemanticError("Expected to have a global scope as the parent");
    }

    auto &sym_tab = parent_scope->sym_tab;
    if (sym_tab.find(name) == sym_tab.end() || sym_tab[name]->kind != Entry_Kind::FUNCTION)
    {
        throw_SemanticError("No function declared with the name: " + name);
    }

    Function_Entry *fe = dynamic_cast<Function_Entry *>(sym_tab[name]);
    if (!fe)
    {
        throw_SemanticError("All you had to do was, STAY!");
    }

    // TODO: Check if the types of the args matches
    const auto &param_types = fe->func_sig->param_types;

    const int num_params = param_types.size();
    const int num_args = args->size();
    if (num_params != num_args)
    {
        throw_SemanticError("Number of args passed to " + name + " is incorrect.");
    }

    for (int i = 0; i < num_params; i++)
    {
        if (param_types[i] != (*args)[i]->type)
        {
            throw_SemanticError("Param #" + std::to_string(i + 1) + ": Type mismatch");
        }
    }

    return new Function_Call_Ast(name, fe->func_sig, args, curr_scope);
}

Call_Stmt_Ast *process_call_stmt(Function_Call_Ast *call)
{
    return new Call_Stmt_Ast(curr_scope, call);
}

void reset_temps()
{
    Temporary_TAC_Operand::reset_temp_count();
    Shared_Temporary_TAC_Operand::reset_stemp_count();
}

void print_func_def_list()
{
    for (const auto &[func_name, symtab_entry] : curr_scope->sym_tab)
    {
        auto func_entry = dynamic_cast<Function_Entry *>(symtab_entry);
        if (!func_entry)
        {
            continue;
        }

        if (show_ast)
        {
            *astout << func_entry->definition->to_string() << "\n";
        }
        if (show_tac)
        {
            *tacout << "**PROCEDURE: " + func_name + "\n";
            *tacout << "**BEGIN: Three Address Code Statements\n";
            *tacout << func_entry->definition->get_code()->to_string() << "\n";
            *tacout << "**END: Three Address Code Statements\n";
        }
        if (show_rtl)
        {
            *rtlout << func_entry->definition->get_rtl(new RegisterTracker())->to_string() << "\n";
        }
    }
}
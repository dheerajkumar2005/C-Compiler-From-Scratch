#include "support.hpp"

IdentifierList *process_var_decl_item_list(std::string *identifier)
{
    return new std::vector<std::string *>{identifier};
}

IdentifierList *process_var_decl_item_list(IdentifierList *identifiers, std::string *identifier)
{
    identifiers->push_back(identifier);
    return identifiers;
}

DeclStmt process_var_decl_stmt(Type type, IdentifierList *identifiers)
{
    return {type, identifiers};
}

DeclStmtList *process_var_decl_stmt_list(DeclStmt var_decl_stmt)
{
    return new std::vector<DeclStmt>{var_decl_stmt};
}

DeclStmtList *process_var_decl_stmt_list(DeclStmtList *var_decl_stmts, DeclStmt var_decl_stmt)
{
    var_decl_stmts->push_back(var_decl_stmt);
    return var_decl_stmts;
}

void set_procedure_context(ParserContext *context, Type type, std::string *identifier)
{
    // NOTE: This is temporary
    if (type != Type::VOID || *identifier != "main")
    {
        throw new SemanticError("Expected 'void main'");
    }

    // Setting context: If already created this procedure object, don't create it again
    if (context->program_ptr->func_table.func_tab.find(*identifier) != context->program_ptr->func_table.func_tab.end())
    {
        context->func_ptr = context->program_ptr->func_table.func_tab[*identifier];
    }
    else
    {
        Func_Signature *func_sig = new Func_Signature(*identifier, type);
        context->func_ptr = new Procedure(context->program_ptr, func_sig);
    }
}

void add_to_local_sym_tab(Procedure *procedure, DeclStmtList *decl_stmt_list)
{
    for (const auto &[type, idListPtr] : *decl_stmt_list)
    {
        for (const auto &id : *idListPtr)
        {
            // Function has a different name
            if (*id == procedure->func_signature->name)
            {
                throw new SemanticError("Local variable name matches function name: " + *id);
            }

            // No local variable with the same name
            if (procedure->symbol_table.sym_tab.find(*id) != procedure->symbol_table.sym_tab.end())
            {
                throw new SemanticError("Local varibale name matches previously declared variable: " + *id);
            }

            procedure->symbol_table.sym_tab[*id] = type;
        }
    }
}

void add_to_global_sym_tab(Program *program, DeclStmt decl_stmt)
{
    for (const auto &id : *decl_stmt.second)
    {
        // No global variable with the same name
        if (program->symbol_table.sym_tab.find(*id) != program->symbol_table.sym_tab.end())
        {
            throw new SemanticError("Expected single declaration of a global variable with name: " + *id);
        }

        // No function with the same name
        if (program->func_table.func_tab.find(*id) != program->func_table.func_tab.end())
        {
            throw new SemanticError("Cannot declare a variable with the same name as a function: " + *id);
        }

        program->symbol_table.sym_tab[*id] = decl_stmt.first;
    }
}

Name_Expr_Ast *process_variable_name(Procedure *func_ptr, std::string *id)
{
    if (func_ptr->symbol_table.sym_tab.find(*id) == func_ptr->symbol_table.sym_tab.end())
    {
        throw new SemanticError("Expected variable declaration before usage: " + *id);
    }
    return new Name_Expr_Ast(id, func_ptr->symbol_table.sym_tab[*id]);
}

FormalParam process_formal_param(Type type, std::string *id)
{
    return {type, id};
}

FormalParamList *process_formal_param_list(FormalParam formal_param)
{
    return new std::vector<FormalParam>{formal_param};
}

FormalParamList *process_formal_param_list(FormalParamList *formal_param_list, FormalParam formal_param)
{
    formal_param_list->push_back(formal_param);
    return formal_param_list;
}

void process_func_decl(ParserContext *context, FuncHeader func_header, FormalParamList *formal_param_list)
{
    Type return_type = func_header.first;
    std::string func_name = *(func_header.second);
    Func_Signature *func_sig = new Func_Signature(func_name, return_type);

    if (formal_param_list)
    {
        for (const auto &formal_param : *formal_param_list)
        {
            func_sig->add_param(formal_param.first);
        }
    }

    // No global variable with this name
    const auto &global_sym_tab = context->program_ptr->symbol_table.sym_tab;
    if (global_sym_tab.find(func_name) != global_sym_tab.end())
    {
        throw new SemanticError("Function name matches global variable name: " + func_name);
    }

    // No other function with the same name
    auto &func_tab = context->program_ptr->func_table.func_tab;
    if (func_tab.find(func_name) != func_tab.end())
    {
        throw new SemanticError("Expected only one function with name: " + func_name);
    }

    // NOTE: We assume no function overloading
    Procedure *func_ptr = context->func_ptr;
    func_tab[func_name] = func_ptr;

    // Insert all params as local variables in the function's symbol table
    auto &local_sym_tab = func_ptr->symbol_table.sym_tab;
    if (formal_param_list)
    {
        for (const auto &formal_param : *formal_param_list)
        {
            local_sym_tab[*formal_param.second] = formal_param.first;
        }
    }
}

void process_func_def(ParserContext *context, FuncHeader func_header, FormalParamList *formal_param_list)
{
    // TODO: DRY

    Type return_type = func_header.first;
    std::string func_name = *(func_header.second);
    Func_Signature *func_sig = new Func_Signature(func_name, return_type);

    if (formal_param_list)
    {
        for (const auto &formal_param : *formal_param_list)
        {
            func_sig->add_param(formal_param.first);
        }
    }

    const auto &global_sym_tab = context->program_ptr->symbol_table.sym_tab;
    if (global_sym_tab.find(func_name) != global_sym_tab.end())
    {
        throw new SemanticError("Function name matches global variable name: " + func_name);
    }

    auto &func_tab = context->program_ptr->func_table.func_tab;
    if (func_tab.find(func_name) != func_tab.end())
    {
        Procedure *existing_procedure = func_tab[func_name];
        if (*(existing_procedure->func_signature) != *func_sig)
        {
            throw new SemanticError("Expected function definition to match declaration: " + func_name);
        }
    }

    else
    {
        process_func_decl(context, func_header, formal_param_list);
    }
}

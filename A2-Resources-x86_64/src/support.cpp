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

void set_context_to_main(ParserContext *context, Type type, std::string *identifier)
{
    if (type != Type::VOID || *identifier != "main")
    {
        throw new SemanticError("Expected 'void main'");
    }

    Func_Signature *func_sig = new Func_Signature(*identifier, type);
    Procedure *procedure = new Procedure(context->program_ptr, func_sig);
    context->main_func_ptr = procedure;
}

void add_to_local_sym_tab(Procedure *procedure, DeclStmtList *decl_stmt_list)
{
    for (const auto &[type, idListPtr] : *decl_stmt_list)
    {
        for (const auto &id : *idListPtr)
        {
            if (*id == procedure->func_signature->name)
            {
                throw new SemanticError("Local variable name matches function name: " + *id);
            }
            procedure->symbol_table.insert(*id, type);
        }
    }
}

void add_to_global_sym_tab(Program *program, DeclStmtList *decl_stmt_list)
{
    // TODO
}
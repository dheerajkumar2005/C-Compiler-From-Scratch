#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include <vector>
#include <string>

#include "Ast.hpp"
#include "ParserContext.hpp"
#include "SemanticError.hpp"
#include "Program.hpp"

using IdentifierList = std::vector<std::string *>;
using DeclStmt = std::pair<Type, IdentifierList *>;
using DeclStmtList = std::vector<DeclStmt>;

IdentifierList *process_var_decl_item_list(std::string *identifier);
IdentifierList *process_var_decl_item_list(IdentifierList *identifiers, std::string *identifier);

DeclStmt process_var_decl_stmt(Type type, IdentifierList *identifiers);

DeclStmtList *process_var_decl_stmt_list(DeclStmt var_decl_stmt);
DeclStmtList *process_var_decl_stmt_list(DeclStmtList *var_decl_stmts, DeclStmt var_decl_stmt);

void set_context_to_main(ParserContext *context, Type type, std::string *identifier);

void add_to_local_sym_tab(Procedure *procedure, DeclStmtList *decl_stmt_list);
void add_to_global_sym_tab(Program *program, DeclStmtList *decl_stmt_list);

#endif

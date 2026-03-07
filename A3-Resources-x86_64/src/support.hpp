#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include <vector>
#include <string>

#include "Ast.hpp"

#include "SemanticError.hpp"
#include "Program.hpp"
#include "Errors.hpp"

using IdentifierList = std::vector<std::string *>;
using DeclStmt = std::pair<Type, IdentifierList *>;
using DeclStmtList = std::vector<DeclStmt *>;

using FormalParam = std::pair<Type, std::string *>;
using FormalParamList = std::vector<FormalParam *>;

using FuncHeader = std::pair<Type, std::string *>;

using StatementList = std::vector<Statement_Ast *>;

IdentifierList *process_var_decl_item_list(std::string *identifier);
IdentifierList *process_var_decl_item_list(IdentifierList *identifiers, std::string *identifier);

DeclStmt *process_var_decl_stmt(Type type, IdentifierList *identifiers);

DeclStmtList *process_var_decl_stmt_list(DeclStmt *var_decl_stmt);
DeclStmtList *process_var_decl_stmt_list(DeclStmtList *var_decl_stmts, DeclStmt *var_decl_stmt);

void set_procedure_context(ParserContext *context, Type type, std::string *identifier);

void add_to_local_sym_tab(Procedure *procedure, DeclStmtList *decl_stmt_list);
void add_to_global_sym_tab(Program *program, DeclStmt *decl_stmt);

Name_Expr_Ast *process_variable_name(ParserContext *context, std::string *identifier);

FormalParam *process_formal_param(Type type, std::string *id);

FormalParamList *process_formal_param_list(FormalParam *formal_param);
FormalParamList *process_formal_param_list(FormalParamList *formal_param_list, FormalParam *formal_param);

void process_func_decl(ParserContext *context, FuncHeader *func_header, FormalParamList *formal_param_list = nullptr);
void process_func_def(ParserContext *context, FuncHeader *func_header, FormalParamList *formal_param_list = nullptr);

StatementList *process_stmt_list(StatementList *stmt_list, Statement_Ast *stmt);
StatementList *process_stmt_list();

void print_func_sig(Procedure *func_ptr);
void print_stmt_ast_list(StatementList *stmt_list);

#endif

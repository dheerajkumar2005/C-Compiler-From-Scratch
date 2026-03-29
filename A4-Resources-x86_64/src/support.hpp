#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include <vector>
#include <string>

#include "Ast.hpp"

#include "SemanticError.hpp"
#include "Program.hpp"
#include "Errors.hpp"

extern int sa_parse;

using IdentifierList = std::vector<std::string *>;
using DeclStmt = std::pair<Type, IdentifierList *>;
using DeclStmtList = std::vector<DeclStmt *>;

using FormalParam = std::pair<Type, std::string *>;
using FormalParamList = std::vector<FormalParam *>;

using FuncHeader = std::pair<Type, std::string *>;

using StatementList = std::vector<Statement_Ast *>;

IdentifierList *accumulate_var_decl_item_list(std::string *identifier);
IdentifierList *accumulate_var_decl_item_list(IdentifierList *identifiers, std::string *identifier);

void process_var_decl_stmt(Scope *curr_scope, Type type, IdentifierList *identifiers);

Name_Expr_Ast *process_variable_name(Scope *curr_scope, std::string *identifier);

FormalParam *accumulate_formal_param(Type type, std::string *id);

FormalParamList *accumulate_formal_param_list(FormalParam *formal_param);
FormalParamList *accumulate_formal_param_list(FormalParamList *formal_param_list, FormalParam *formal_param);

Scope *make_func_scope(Scope *curr_scope, Func_Signature *func_sig);

void process_func_decl(Scope *curr_scope, FuncHeader *func_header, FormalParamList *formal_param_list = nullptr);
Func_Signature *process_func_def(Scope *curr_scope, FuncHeader *func_header, FormalParamList *formal_param_list = nullptr);

Expression_Ast *process_predicate(Expression_Ast *expr);

If_Stmt_Ast *add_else_clause(If_Stmt_Ast *unmatched_if, Statement_Ast *else_clause);

StatementList *accumulate_stmt_list(StatementList *stmt_list, Statement_Ast *stmt);
StatementList *accumulate_stmt_list();

bool is_empty(StatementList *stmt_list);

void ast_print_func_sig(Scope *func);
void ast_print_stmt_list(StatementList *stmt_list);

void tac_print_func_sig(Scope *func);
void tac_print_stmt_list(StatementList *stmt_list);

void rtl_print_func_sig(Scope *func);
void rtl_print_stmt_list(StatementList *stmt_list, RegisterTracker *register_tracker);

#endif

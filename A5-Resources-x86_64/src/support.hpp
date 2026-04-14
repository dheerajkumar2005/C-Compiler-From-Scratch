#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include <vector>
#include <string>
#include <algorithm>

#include "Ast.hpp"
#include "SemanticError.hpp"
#include "Program.hpp"
#include "Errors.hpp"

extern int sa_parse;
extern int show_ast;
extern int show_tac;
extern int show_rtl;

extern TAC_Label *return_label;
extern Shared_Temporary_TAC_Operand *return_stemp;
extern Scope* curr_scope;

using IdentifierList = std::vector<std::string *>;
using DeclStmt = std::pair<Type, IdentifierList *>;
using DeclStmtList = std::vector<DeclStmt *>;

using FormalParam = std::pair<Type, std::string *>;
using FormalParamList = std::vector<FormalParam *>;

using ActualParam = Expression_Ast;
using ActualParamList = std::vector<ActualParam *>;

using FuncHeader = std::pair<Type, std::string *>;

using StatementList = std::vector<Statement_Ast *>;

IdentifierList *accumulate_var_decl_item_list(std::string *identifier);
IdentifierList *accumulate_var_decl_item_list(IdentifierList *identifiers, std::string *identifier);

void process_var_decl_stmt(Type type, IdentifierList *identifiers);

Name_Expr_Ast *process_variable_name(std::string *identifier);

FormalParam *accumulate_formal_param(Type type, std::string *id);
FormalParam *accumulate_func_header(Type type, std::string *id);

FormalParamList *accumulate_formal_param_list(FormalParam *formal_param);
FormalParamList *accumulate_formal_param_list(FormalParamList *formal_param_list, FormalParam *formal_param);

ActualParamList *accumulate_actual_param_list(ActualParam *arg);
ActualParamList *accumulate_actual_param_list(ActualParamList *args, ActualParam *arg);

Scope *make_func_scope(Func_Signature *func_sig);

void process_func_decl(FuncHeader *func_header, FormalParamList *formal_param_list = nullptr);
Func_Signature *make_func_sig(FuncHeader *func_header, FormalParamList *formal_param_list = nullptr);

void process_body(Scope *parent_scope, StatementList *body);

Expression_Ast *process_predicate(Expression_Ast *expr);

If_Stmt_Ast *add_else_clause(If_Stmt_Ast *unmatched_if, Statement_Ast *else_clause);

StatementList *accumulate_stmt_list(StatementList *stmt_list, Statement_Ast *stmt);
StatementList *accumulate_stmt_list();

Function_Call_Ast *process_func_call(std::string *name, ActualParamList *args);
Call_Stmt_Ast *process_call_stmt(Function_Call_Ast *call);

void print_func_def_list();

#endif

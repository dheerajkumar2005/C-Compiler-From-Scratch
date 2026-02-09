%{
    #include <stdio.h>
    #include <stdlib.h>

    #include "AST.hpp"
    #include "support.hpp"
%}

// TODO: Add types for AST nodes, variables (pointers to symtab entry), ...
%union {
    AssignAST *asgn;
    ReadAST *read;
    WriteAST *write;
    ExprAST *expr;
    IntLiteralAST *ival;
    FloatLiteralAST *fval;
    StrLiteralAST *sval;
    VarAST *var;
}

// Terminals (and optionally their types)
%token VOID
%token INTEGER
%token STRING
%token FLOAT
%token BOOL
%token <var> NAME
%token READ
%token WRITE
%token <ival> INT_NUM
%token <fval> FLOAT_NUM
%token <sval> STR_CONST
%token PLUS
%token MINUS
%token MULT
%token DIV
%token ASSIGN_OP
%token NOT
%token AND
%token OR
%token QUESTION_MARK
%token COLON
%token LESS_THAN
%token LESS_THAN_EQUAL
%token GREATER_THAN
%token GREATER_THAN_EQUAL
%token EQUAL
%token NOT_EQUAL
%token SEMICOLON
%token COMMA
%token LEFT_ROUND_BRACKET
%token RIGHT_ROUND_BRACKET
%token LEFT_CURLY_BRACKET
%token RIGHT_CURLY_BRACKET

// Non-terminals and their types
%type <write> print_statement
%type <read> read_statement
%type <expr> expression
%type <expr> rel_expression
%type <var> variable_as_operand
%type <var> variable_name
%type <expr> constant_as_operand

%start program

// Disambiguation
// Arithmetic operators
%left PLUS MINUS
%left MULT DIV
%right UMINUS // NOTE: %right so that --x is parsed as (-(-x))

// Logical operators
// Unintentional precendence between arithmetic and logical operators established
// But it is fine since such expressions are semantically invalid in all interpretations
%left OR
%left AND
%right NOT

%%

program
    : func_def
    | var_decl_stmt_list func_def
    | func_decl func_def
    | var_decl_stmt_list func_decl func_def
    | func_decl var_decl_stmt_list func_def
    | var_decl_stmt_list func_decl var_decl_stmt_list func_def
;

func_decl
    : func_header LEFT_ROUND_BRACKET formal_param_list RIGHT_ROUND_BRACKET SEMICOLON
    | func_header LEFT_ROUND_BRACKET RIGHT_ROUND_BRACKET SEMICOLON
;

func_header
    : named_type NAME
;

func_def
    : func_header LEFT_ROUND_BRACKET formal_param_list RIGHT_ROUND_BRACKET LEFT_CURLY_BRACKET optional_local_var_decl_stmt_list statement_list RIGHT_CURLY_BRACKET
    | func_header LEFT_ROUND_BRACKET RIGHT_ROUND_BRACKET LEFT_CURLY_BRACKET optional_local_var_decl_stmt_list statement_list RIGHT_CURLY_BRACKET
;

formal_param_list
    : formal_param_list COMMA formal_param
    | formal_param
;

formal_param
    : param_type NAME
;

param_type
    : INTEGER
    | FLOAT
    | BOOL
    | STRING
;

statement_list
    : statement_list statement
    | 
;

statement
    : assignment_statement
    | print_statement
    | read_statement
;

optional_local_var_decl_stmt_list
    : 
    | var_decl_stmt_list
;

var_decl_stmt_list
    : var_decl_stmt
    | var_decl_stmt_list var_decl_stmt
;

var_decl_stmt
    : param_type var_decl_item_list SEMICOLON
;

var_decl_item_list
    : var_decl_item_list COMMA var_decl_item
    | var_decl_item
;

var_decl_item
    : NAME
;

named_type
    : INTEGER
    | FLOAT
    | VOID
    | STRING
    | BOOL
;

assignment_statement
    : variable_as_operand ASSIGN_OP expression SEMICOLON { $$ = process_assignment($1, $3); }
;

print_statement
    : WRITE expression SEMICOLON { $$ = process_write($2); }
;

read_statement
    : READ variable_name SEMICOLON { $$ = process_read($2); }
;

expression
    : expression PLUS expression { $$ = process_expr(Operator::PLUS, $1, $3); }
    | expression MINUS expression { $$ = process_expr(Operator::MINUS, $1, $3); }
    | expression MULT expression { $$ = process_expr(Operator::MULT, $1, $3); }
    | expression DIV expression { $$ = process_expr(Operator::DIV, $1, $3); }
    | MINUS expression %prec UMINUS { $$ = process_expr(Operator::UMINUS, $2); }
    | LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET { $$ = $1; }
    | expression QUESTION_MARK expression COLON expression { $$ = process_expr(Operator::QUESTION_MARK_COLON, $1, $3, $5); }
    | expression AND expression { $$ = process_expr(Operator::AND, $1, $3); } 
    | expression OR expression { $$ = process_expr(Operator::OR, $1, $3); }
    | NOT expression { $$ = process_expr(Operator::NOT, $1, $3); }
    | rel_expression { $$ = $1; }
    | variable_as_operand { $$ = $1; }
    | constant_as_operand { $$ = $1; }
;

rel_expression
    : expression LESS_THAN expression { $$ = process_expr(Operator::LESS_THAN, $1, $3); }
    | expression LESS_THAN_EQUAL expression { $$ = process_expr(Operator::LESS_THAN_EQUAL, $1, $3); }
    | expression GREATER_THAN expression { $$ = process_expr(Operator::GREATER_THAN, $1, $3); }
    | expression GREATER_THAN_EQUAL expression { $$ = process_expr(Operator::GREATER_THAN_EQUAL, $1, $3); }
    | expression NOT_EQUAL expression { $$ = process_expr(Operator::NOT_EQUAL, $1, $3); }
    | expression EQUAL expression { $$ = process_expr(Operator::EQUAL, $1, $3); }
;

variable_as_operand
    : variable_name { $$ = $1; }
;

variable_name
    : NAME { $$ = $1; }
;

constant_as_operand
    : INT_NUM { $$ = $1; }
    | FLOAT_NUM { $$ = $1; }
    | STR_CONST { $$ = $1; }
;

%%

%{
    #include <stdio.h>
    #include <stdlib.h>
%}

%union {
    int ival;
    float fval;
    char *sval;
}

%token VOID
%token INTEGER
%token STRING
%token FLOAT
%token BOOL
%token <sval> NAME
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
    : variable_as_operand ASSIGN_OP expression SEMICOLON
;

print_statement
    : WRITE expression SEMICOLON
;

read_statement
    : READ variable_name SEMICOLON
;

expression
    : expression PLUS expression
    | expression MINUS expression
    | expression MULT expression
    | expression DIV expression
    | MINUS expression %prec UMINUS
    | LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET
    | expression QUESTION_MARK expression COLON expression
    | expression AND expression
    | expression OR expression
    | NOT expression
    | rel_expression
    | variable_as_operand
    | constant_as_operand
;

rel_expression
    : expression LESS_THAN expression
    | expression LESS_THAN_EQUAL expression
    | expression GREATER_THAN expression
    | expression GREATER_THAN_EQUAL expression
    | expression NOT_EQUAL expression
    | expression EQUAL expression

variable_as_operand
    : variable_name
;

variable_name
    : NAME
;

constant_as_operand
    : INT_NUM
    | FLOAT_NUM
    | STR_CONST
;

%%

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
%token SEMICOLON
%token COMMA
%token LEFT_ROUND_BRACKET
%token RIGHT_ROUND_BRACKET
%token LEFT_CURLY_BRACKET
%token RIGHT_CURLY_BRACKET

%type <ival> expression

%start statement_list

%left PLUS MINUS
%left MULT DIV
%right UMINUS

%%

statement_list
    : statement_list statement
    | statement

statement
    : expression SEMICOLON

expression
    : expression PLUS expression { $$ = $1 + $3; printf("Found a PLUS expression with value %d\n", $$); }
    | expression MINUS expression { $$ = $1 - $3; printf("Found a MINUS expression with value %d\n", $$); }
    | expression MULT expression { $$ = $1 * $3; printf("Found a MULT expression with value %d\n", $$); }
    | expression DIV expression { $$ = $1 / $3; printf("Found a DIV expression with value %d\n", $$); }
    | MINUS expression %prec UMINUS { $$ = -$2; printf("Found a UMINUS expression with value %d\n", $$); }
    | LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET { $$ = $2; printf("Found an expression with value %d\n", $$); }
    | INT_NUM { $$ = $1; printf("Found an integer with value %d\n", $$); }
;

%%

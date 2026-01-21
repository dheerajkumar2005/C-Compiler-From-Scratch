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
%token ASSIGN_OP
%token SEMICOLON
%token COMMA
%token LEFT_ROUND_BRACKET
%token RIGHT_ROUND_BRACKET
%token LEFT_CURLY_BRACKET
%token RIGHT_CURLY_BRACKET

%start PROGRAM

%%

PROGRAM : PROGRAM STATEMENT | STATEMENT

STATEMENT 
    : TYPE NAME SEMICOLON // DECLARATION
        { printf("Found a declaration for %s\n", $2); }
    | NAME ASSIGN_OP LITERAL SEMICOLON // ASSIGNMENT
        { printf("Found an assignment for %s\n", $1); }

TYPE : INTEGER | STRING | FLOAT | BOOL

LITERAL 
    : INT_NUM { printf("Found an int literal %i\n", $1); } 
    | FLOAT_NUM { printf("Found a float literal %f\n", $1); }
    | STR_CONST { printf("Found a string literal %s\n", $1); }
%%

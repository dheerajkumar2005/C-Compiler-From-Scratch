%option noyywrap
%{
    #include "y.tab.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    // TODO: Need to add tokens for L2
%}

DIGIT [0-9]
LETTER [a-zA-Z_]

%%

"void" { return VOID; }
"int" { return INTEGER; }
"read" { return READ; }
"print" { return WRITE; }
"string" { return STRING; }
"float" { return FLOAT; }
"bool" { return BOOL; }

{LETTER}({LETTER}|{DIGIT})* { yylval.sval = strdup(yytext); return NAME; }

{DIGIT}+ { yylval.ival = atoi(yytext); return INT_NUM; }
({DIGIT}+\.{DIGIT}*)|(\.{DIGIT}+) { yylval.fval = atof(yytext); return FLOAT_NUM; }
\"[^\"]*\" { yylval.sval = strdup(yytext); return STR_CONST; }

"=" { return ASSIGN_OP; }

"+" { return PLUS; }
"-" { return MINUS; }
"*" { return MULT; }
"/" { return DIV; }

";" { return SEMICOLON; }
"," { return COMMA; }
"(" { return LEFT_ROUND_BRACKET; }
")" { return RIGHT_ROUND_BRACKET; }
"{" { return LEFT_CURLY_BRACKET; }
"}" { return RIGHT_CURLY_BRACKET; }

\/\/[^\n]* ;

[ \n]+ ;

%%

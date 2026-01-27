%option noyywrap
%option yylineno
%{
    #include "y.tab.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    extern int show_tokens;
    extern int scanner_error;
%}

DIGIT [0-9]
LETTER [a-zA-Z_]

%%

"void" { 
    if(show_tokens) {
        fprintf(yyout, "Token Name: VOID\tLexeme: void\tLineno: %d\n", yylineno); 
    }
    return VOID; 
}
"int" { 
    if(show_tokens) {
        fprintf(yyout, "Token Name: INTEGER\tLexeme: int\tLineno: %d\n", yylineno);
    }
    return INTEGER; 
}
"read" { 
    if(show_tokens) {
        fprintf(yyout, "Token Name: READ\tLexeme: read\tLineno: %d\n", yylineno);
    }
    return READ; 
}
"print" { 
    if(show_tokens) {
        fprintf(yyout, "Token Name: WRITE\tLexeme: print\tLineno: %d\n", yylineno); 
    }
    return WRITE; 
}
"string" { 
    if(show_tokens) {
        fprintf(yyout, "Token Name: STRING\tLexeme: string\tLineno: %d\n", yylineno);
    } 
    return STRING; 
}
"float" { 
    if(show_tokens) {
        fprintf(yyout, "Token Name: FLOAT\tLexeme: float\tLineno: %d\n", yylineno); 
    }
    return FLOAT; 
}
"bool" { 
    if(show_tokens) {
        fprintf(yyout, "Token Name: BOOL\tLexeme: bool\tLineno: %d\n", yylineno); 
    }
    return BOOL; 
}

{LETTER}({LETTER}|{DIGIT})* { 
    yylval.sval = strdup(yytext); 
    if(show_tokens) {
        fprintf(yyout, "Token Name: NAME\tLexeme: %s\tLineno: %d\n", yylval.sval, yylineno); 
    }
    return NAME;
}

{DIGIT}+ {
    yylval.ival = atoi(yytext); 
    if(show_tokens) {
        fprintf(yyout, "Token Name: INT_NUM\tLexeme: %d\tLineno: %d\n", yylval.ival, yylineno); 
    }
    return INT_NUM; 
}
({DIGIT}+\.{DIGIT}*)|(\.{DIGIT}+) { 
    yylval.fval = atof(yytext);
    if(show_tokens) {
        fprintf(yyout, "Token Name: FLOAT_NUM\tLexeme: %f\tLineno: %d\n", yylval.fval, yylineno); 
    }
    return FLOAT_NUM; }
\"[^\"]*\" { 
    yylval.sval = strdup(yytext);
    if(show_tokens) {
        fprintf(yyout, "Token Name: STR_CONST\tLexeme: %s\tLineno: %d\n", yylval.sval, yylineno); 
    }
    return STR_CONST; 
}

"=" {
    if(show_tokens) {
        fprintf(yyout, "Token Name: ASSIGN_OP\tLexeme: =\tLineno: %d\n", yylineno); 
    } 
    return ASSIGN_OP; 
}

"+" {
    if(show_tokens) {
        fprintf(yyout, "Token Name: PLUS\tLexeme: +\tLineno: %d\n", yylineno); 
    } 
    return PLUS; 
}
"-" {
    if(show_tokens) {
        fprintf(yyout, "Token Name: MINUS\tLexeme: -\tLineno: %d\n", yylineno); 
    } 
    return MINUS; 
}
"*" {
    if(show_tokens) {
        fprintf(yyout, "Token Name: MULT\tLexeme: *\tLineno: %d\n", yylineno); 
    } 
    return MULT; 
}
"/" { 
    if(show_tokens) {
        fprintf(yyout, "Token Name: DIV\tLexeme: /\tLineno: %d\n", yylineno); 
    }
    return DIV; 
}

";" {
    if(show_tokens) {
        fprintf(yyout, "Token Name: SEMICOLON\tLexeme: ;\tLineno: %d\n", yylineno); 
    } 
    return SEMICOLON; 
}
"," {
    if(show_tokens) {
        fprintf(yyout, "Token Name: COMMA\tLexeme: ,\tLineno: %d\n", yylineno); 
    } 
    return COMMA; 
}
"(" {
    if(show_tokens) {
        fprintf(yyout, "Token Name: LEFT_ROUND_BRACKET\tLexeme: \tLineno: %d\n", yylineno); 
    } 
    return LEFT_ROUND_BRACKET; 
}
")" {
    if(show_tokens) {
        fprintf(yyout, "Token Name: RIGHT_ROUND_BRACKET\tLexeme: \tLineno: %d\n", yylineno); 
    }  
    return RIGHT_ROUND_BRACKET; 
}
"{" {
    if(show_tokens) {
        fprintf(yyout, "Token Name: LEFT_CURLY_BRACKET\tLexeme: \tLineno: %d\n", yylineno); 
    } 
    return LEFT_CURLY_BRACKET; 
}
"}" {
    if(show_tokens) {
        fprintf(yyout, "Token Name: RIGHT_CURLY_BRACKET\tLexeme: \tLineno: %d\n", yylineno); 
    } 
    return RIGHT_CURLY_BRACKET; 
}

\/\/[^\n]* ;

[ \n]+ ;
. {
    scanner_error = 1;
}

%%

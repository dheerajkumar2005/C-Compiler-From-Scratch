%option noyywrap
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    enum Token {
        VOID = 256, INTEGER, READ, WRITE, STRING, FLOAT, BOOL,
        NAME,
        INT_NUM, FLOAT_NUM, STR_CONST,
        ASSIGN_OP,
        SEMICOLON, COMMA, 
        LEFT_ROUND_BRACKET, RIGHT_ROUND_BRACKET,
        LEFT_CURLY_BRACKET, RIGHT_CURLY_BRACKET
    };
%}

/* Definitions */
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

{LETTER}({LETTER}|{DIGIT})* { return NAME; }

{DIGIT}+ { return INT_NUM; }
({DIGIT}+\.{DIGIT}*)|(\.{DIGIT}*) { return FLOAT_NUM; }
"\"[^\"\n\r]*\"" { return STR_CONST; }

"=" { return ASSIGN_OP; }

";" { return SEMICOLON; }
"," { return COMMA; }
"(" { return LEFT_ROUND_BRACKET; }
")" { return RIGHT_ROUND_BRACKET; }
"{" { return LEFT_CURLY_BRACKET; }
"}" { return RIGHT_CURLY_BRACKET; }

%%

int main() {
    int tok;
    while((tok = yylex()) != 0) {
        printf("TOKEN = %d, LEXEME = \"%s\"\n", tok, yytext);
    }
    return 0;
}

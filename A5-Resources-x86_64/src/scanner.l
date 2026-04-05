%{
    #include <stdio.h>
    #include "Ast.hpp"
    #include "parse.tab.h"

    #define YY_DECL int yylex(Scope *context, RegisterTracker *register_tracker)

    extern int show_tokens;
    extern int scanner_error;

    void display_token(const char *token_name, const char *lexeme) {
        if(show_tokens) {
            fprintf(yyout, "Token Name: %s\tLexeme: %s\tLineno: %d\n", token_name, lexeme, yylineno);
        }
    }
%}


%option noyywrap
%option yylineno

DIGIT [0-9]
LETTER [a-zA-Z_]

%%

"void" { 
    display_token("VOID", "void");
    return VOID; 
}
"int" { 
    display_token("INTEGER", "int");
    return INTEGER; 
}
"read" { 
    display_token("READ", "read");
    return READ; 
}
"print" { 
    display_token("WRITE", "print");
    return WRITE; 
}
"string" { 
    display_token("STRING", "string");
    return STRING; 
}
"float" { 
    display_token("FLOAT", "float");
    return FLOAT; 
}
"bool" { 
    display_token("BOOL", "bool");
    return BOOL; 
}
"if" {
    display_token("IF", "if");
    return IF;
}
"else" {
    display_token("ELSE", "else");
    return ELSE;
}
"do" {
    display_token("DO", "do");
    return DO;
}
"while" {
    display_token("WHILE", "while");
    return WHILE;
}
"return" {
    display_token("RETURN", "return");
    return RETURN;
}

{LETTER}({LETTER}|{DIGIT})* { 
    // FIXED
    display_token("NAME", yytext); 
    yylval.identifier = new std::string(yytext); 
    return NAME;
}

{DIGIT}+ {
    // FIXED
    display_token("INT_NUM", yytext);
    yylval.iptr = new Int_Expr_Ast(atoi(yytext));
    return INT_NUM; 
}
({DIGIT}+\.{DIGIT}*)|(\.{DIGIT}+) { 
    // FIXED
    display_token("FLOAT_NUM", yytext);
    yylval.fptr = new Float_Expr_Ast(atof(yytext));
    return FLOAT_NUM; 
}
\"[^\"]*\" {
    // FIXED
    display_token("STR_CONST", yytext); 
    yylval.sptr = new String_Expr_Ast(yytext);
    return STR_CONST; 
}

"=" {
    display_token("ASSIGN_OP", "=");
    return ASSIGN_OP; 
}

"+" {
    display_token("PLUS", "+"); 
    return PLUS; 
}
"-" {
    display_token("MINUS", "-");
    return MINUS; 
}
"*" {
    display_token("MULT", "*");
    return MULT; 
}
"/" { 
    display_token("DIV", "/");
    return DIV; 
}

"!" {
    display_token("NOT", "!");
    return NOT;
}
"&&" {
    display_token("AND", "&&");
    return AND;
}
"||" {
    display_token("OR", "||");
    return OR;
}

"?" {
    display_token("QUESTION_MARK", "?");
    return QUESTION_MARK;
}
":" {
    display_token("COLON", ":");
    return COLON;
}
"<" {
    display_token("LESS_THAN", "<");
    return LESS_THAN;
}
"<=" {
    display_token("LESS_THAN_EQUAL", "<=");
    return LESS_THAN_EQUAL;
}
">" {
    display_token("GREATER_THAN", ">");
    return GREATER_THAN;
}
">=" {
    display_token("GREATER_THAN_EQUAL", ">=");
    return GREATER_THAN_EQUAL;
}
"==" {
    display_token("EQUAL", "==");
    return EQUAL;
}
"!=" {
    display_token("NOT_EQUAL", "!=");
    return NOT_EQUAL;
}

";" {
    display_token("SEMICOLON", ";");
    return SEMICOLON; 
}
"," {
    display_token("COMMA", ",");
    return COMMA; 
}
"(" {
    display_token("LEFT_ROUND_BRACKET", "(");
    return LEFT_ROUND_BRACKET; 
}
")" {
    display_token("RIGHT_ROUND_BRACKET", ")");  
    return RIGHT_ROUND_BRACKET; 
}
"{" {
    display_token("LEFT_CURLY_BRACKET", "{");
    return LEFT_CURLY_BRACKET; 
}
"}" {
    display_token("RIGHT_CURLY_BRACKET", "}");
    return RIGHT_CURLY_BRACKET; 
}

\/\/[^\n]* ;

[ \n\t]+ ;
. {
    fprintf(stderr, "Bad char: %d\n", yytext[0]);
    scanner_error = 1;
    return yytext[0];
}

%%

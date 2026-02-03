/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    VOID = 258,                    /* VOID  */
    INTEGER = 259,                 /* INTEGER  */
    STRING = 260,                  /* STRING  */
    FLOAT = 261,                   /* FLOAT  */
    BOOL = 262,                    /* BOOL  */
    NAME = 263,                    /* NAME  */
    READ = 264,                    /* READ  */
    WRITE = 265,                   /* WRITE  */
    INT_NUM = 266,                 /* INT_NUM  */
    FLOAT_NUM = 267,               /* FLOAT_NUM  */
    STR_CONST = 268,               /* STR_CONST  */
    PLUS = 269,                    /* PLUS  */
    MINUS = 270,                   /* MINUS  */
    MULT = 271,                    /* MULT  */
    DIV = 272,                     /* DIV  */
    ASSIGN_OP = 273,               /* ASSIGN_OP  */
    SEMICOLON = 274,               /* SEMICOLON  */
    COMMA = 275,                   /* COMMA  */
    LEFT_ROUND_BRACKET = 276,      /* LEFT_ROUND_BRACKET  */
    RIGHT_ROUND_BRACKET = 277,     /* RIGHT_ROUND_BRACKET  */
    LEFT_CURLY_BRACKET = 278,      /* LEFT_CURLY_BRACKET  */
    RIGHT_CURLY_BRACKET = 279,     /* RIGHT_CURLY_BRACKET  */
    UMINUS = 280                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define VOID 258
#define INTEGER 259
#define STRING 260
#define FLOAT 261
#define BOOL 262
#define NAME 263
#define READ 264
#define WRITE 265
#define INT_NUM 266
#define FLOAT_NUM 267
#define STR_CONST 268
#define PLUS 269
#define MINUS 270
#define MULT 271
#define DIV 272
#define ASSIGN_OP 273
#define SEMICOLON 274
#define COMMA 275
#define LEFT_ROUND_BRACKET 276
#define RIGHT_ROUND_BRACKET 277
#define LEFT_CURLY_BRACKET 278
#define RIGHT_CURLY_BRACKET 279
#define UMINUS 280

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 6 "parser.y"

    int ival;
    float fval;
    char *sval;

#line 123 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

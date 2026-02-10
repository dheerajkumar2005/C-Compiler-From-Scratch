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
/* "%code requires" blocks.  */
#line 1 "parser.y"

    #include "AST.hpp"

#line 53 "y.tab.h"

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
    NOT = 274,                     /* NOT  */
    AND = 275,                     /* AND  */
    OR = 276,                      /* OR  */
    QUESTION_MARK = 277,           /* QUESTION_MARK  */
    COLON = 278,                   /* COLON  */
    LESS_THAN = 279,               /* LESS_THAN  */
    LESS_THAN_EQUAL = 280,         /* LESS_THAN_EQUAL  */
    GREATER_THAN = 281,            /* GREATER_THAN  */
    GREATER_THAN_EQUAL = 282,      /* GREATER_THAN_EQUAL  */
    EQUAL = 283,                   /* EQUAL  */
    NOT_EQUAL = 284,               /* NOT_EQUAL  */
    SEMICOLON = 285,               /* SEMICOLON  */
    COMMA = 286,                   /* COMMA  */
    LEFT_ROUND_BRACKET = 287,      /* LEFT_ROUND_BRACKET  */
    RIGHT_ROUND_BRACKET = 288,     /* RIGHT_ROUND_BRACKET  */
    LEFT_CURLY_BRACKET = 289,      /* LEFT_CURLY_BRACKET  */
    RIGHT_CURLY_BRACKET = 290,     /* RIGHT_CURLY_BRACKET  */
    UMINUS = 291                   /* UMINUS  */
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
#define NOT 274
#define AND 275
#define OR 276
#define QUESTION_MARK 277
#define COLON 278
#define LESS_THAN 279
#define LESS_THAN_EQUAL 280
#define GREATER_THAN 281
#define GREATER_THAN_EQUAL 282
#define EQUAL 283
#define NOT_EQUAL 284
#define SEMICOLON 285
#define COMMA 286
#define LEFT_ROUND_BRACKET 287
#define RIGHT_ROUND_BRACKET 288
#define LEFT_CURLY_BRACKET 289
#define RIGHT_CURLY_BRACKET 290
#define UMINUS 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 5 "parser.y"

    AST *stmt;
    AssignAST *asgn;
    ReadAST *read;
    WriteAST *write;
    ExprAST *expr;
    IntLiteralAST *iptr;
    FloatLiteralAST *fptr;
    StrLiteralAST *sptr;
    VarAST *var;

#line 157 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1





# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 108 "y.tab.c"

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

#line 212 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_VOID = 3,                       /* VOID  */
  YYSYMBOL_INTEGER = 4,                    /* INTEGER  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_FLOAT = 6,                      /* FLOAT  */
  YYSYMBOL_BOOL = 7,                       /* BOOL  */
  YYSYMBOL_NAME = 8,                       /* NAME  */
  YYSYMBOL_READ = 9,                       /* READ  */
  YYSYMBOL_WRITE = 10,                     /* WRITE  */
  YYSYMBOL_INT_NUM = 11,                   /* INT_NUM  */
  YYSYMBOL_FLOAT_NUM = 12,                 /* FLOAT_NUM  */
  YYSYMBOL_STR_CONST = 13,                 /* STR_CONST  */
  YYSYMBOL_PLUS = 14,                      /* PLUS  */
  YYSYMBOL_MINUS = 15,                     /* MINUS  */
  YYSYMBOL_MULT = 16,                      /* MULT  */
  YYSYMBOL_DIV = 17,                       /* DIV  */
  YYSYMBOL_ASSIGN_OP = 18,                 /* ASSIGN_OP  */
  YYSYMBOL_NOT = 19,                       /* NOT  */
  YYSYMBOL_AND = 20,                       /* AND  */
  YYSYMBOL_OR = 21,                        /* OR  */
  YYSYMBOL_QUESTION_MARK = 22,             /* QUESTION_MARK  */
  YYSYMBOL_COLON = 23,                     /* COLON  */
  YYSYMBOL_LESS_THAN = 24,                 /* LESS_THAN  */
  YYSYMBOL_LESS_THAN_EQUAL = 25,           /* LESS_THAN_EQUAL  */
  YYSYMBOL_GREATER_THAN = 26,              /* GREATER_THAN  */
  YYSYMBOL_GREATER_THAN_EQUAL = 27,        /* GREATER_THAN_EQUAL  */
  YYSYMBOL_EQUAL = 28,                     /* EQUAL  */
  YYSYMBOL_NOT_EQUAL = 29,                 /* NOT_EQUAL  */
  YYSYMBOL_SEMICOLON = 30,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 31,                     /* COMMA  */
  YYSYMBOL_LEFT_ROUND_BRACKET = 32,        /* LEFT_ROUND_BRACKET  */
  YYSYMBOL_RIGHT_ROUND_BRACKET = 33,       /* RIGHT_ROUND_BRACKET  */
  YYSYMBOL_LEFT_CURLY_BRACKET = 34,        /* LEFT_CURLY_BRACKET  */
  YYSYMBOL_RIGHT_CURLY_BRACKET = 35,       /* RIGHT_CURLY_BRACKET  */
  YYSYMBOL_UMINUS = 36,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 37,                  /* $accept  */
  YYSYMBOL_program = 38,                   /* program  */
  YYSYMBOL_func_decl = 39,                 /* func_decl  */
  YYSYMBOL_func_header = 40,               /* func_header  */
  YYSYMBOL_func_def = 41,                  /* func_def  */
  YYSYMBOL_formal_param_list = 42,         /* formal_param_list  */
  YYSYMBOL_formal_param = 43,              /* formal_param  */
  YYSYMBOL_param_type = 44,                /* param_type  */
  YYSYMBOL_statement_list = 45,            /* statement_list  */
  YYSYMBOL_statement = 46,                 /* statement  */
  YYSYMBOL_optional_local_var_decl_stmt_list = 47, /* optional_local_var_decl_stmt_list  */
  YYSYMBOL_var_decl_stmt_list = 48,        /* var_decl_stmt_list  */
  YYSYMBOL_var_decl_stmt = 49,             /* var_decl_stmt  */
  YYSYMBOL_var_decl_item_list = 50,        /* var_decl_item_list  */
  YYSYMBOL_var_decl_item = 51,             /* var_decl_item  */
  YYSYMBOL_named_type = 52,                /* named_type  */
  YYSYMBOL_assignment_statement = 53,      /* assignment_statement  */
  YYSYMBOL_print_statement = 54,           /* print_statement  */
  YYSYMBOL_read_statement = 55,            /* read_statement  */
  YYSYMBOL_expression = 56,                /* expression  */
  YYSYMBOL_rel_expression = 57,            /* rel_expression  */
  YYSYMBOL_variable_as_operand = 58,       /* variable_as_operand  */
  YYSYMBOL_variable_name = 59,             /* variable_name  */
  YYSYMBOL_constant_as_operand = 60        /* constant_as_operand  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   215

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  117

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   291


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    81,    81,    82,    83,    84,    85,    86,    90,    91,
      95,    99,   100,   104,   105,   109,   113,   114,   115,   116,
     120,   121,   126,   127,   128,   132,   133,   137,   138,   142,
     146,   147,   151,   155,   156,   157,   158,   159,   163,   167,
     171,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   191,   192,   193,   194,   195,   196,
     200,   204,   208,   209,   210
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "VOID", "INTEGER",
  "STRING", "FLOAT", "BOOL", "NAME", "READ", "WRITE", "INT_NUM",
  "FLOAT_NUM", "STR_CONST", "PLUS", "MINUS", "MULT", "DIV", "ASSIGN_OP",
  "NOT", "AND", "OR", "QUESTION_MARK", "COLON", "LESS_THAN",
  "LESS_THAN_EQUAL", "GREATER_THAN", "GREATER_THAN_EQUAL", "EQUAL",
  "NOT_EQUAL", "SEMICOLON", "COMMA", "LEFT_ROUND_BRACKET",
  "RIGHT_ROUND_BRACKET", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET",
  "UMINUS", "$accept", "program", "func_decl", "func_header", "func_def",
  "formal_param_list", "formal_param", "param_type", "statement_list",
  "statement", "optional_local_var_decl_stmt_list", "var_decl_stmt_list",
  "var_decl_stmt", "var_decl_item_list", "var_decl_item", "named_type",
  "assignment_statement", "print_statement", "read_statement",
  "expression", "rel_expression", "variable_as_operand", "variable_name",
  "constant_as_operand", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-73)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     204,   -73,   -73,   -73,   -73,   -73,     9,   204,    -4,   -73,
      47,   204,   -73,    48,   -73,    -1,   -73,   204,    33,   -73,
      32,   -73,   204,   -73,   -73,   -73,    41,   -73,   -73,   -73,
     -73,   -73,    23,   -25,   -73,    64,   -73,    47,   -73,   204,
      15,    45,   -73,   208,   208,    31,   -73,   -73,   -73,    39,
     -73,   208,   -73,   -73,   208,    25,   -73,   -73,    75,    56,
     -73,   -73,   -73,   -73,   -73,    66,   -73,    42,    67,   -73,
     -73,   -73,    56,    56,    56,    85,   -73,   -73,   -73,    56,
     -73,   -73,   159,   177,    65,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,   -73,   102,
     -73,   149,   149,   159,   159,   177,   169,   119,   135,   135,
     135,   135,   135,   135,   -73,    56,   135
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    35,    16,    19,    17,    18,     0,     0,     0,     2,
       0,     0,    27,     0,     1,     0,     4,     0,     0,    32,
       0,    31,     0,     3,    28,    10,     0,     6,    16,    19,
      17,    18,     0,     0,    14,     0,    29,     0,     5,     0,
       0,     0,     9,    25,     0,     0,    15,    30,     7,     0,
      21,    26,    13,     8,    25,     0,    21,    61,     0,     0,
      12,    20,    22,    23,    24,     0,    60,     0,     0,    62,
      63,    64,     0,     0,     0,     0,    51,    52,    53,     0,
      11,    40,    45,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,     0,
      46,    41,    42,    43,    44,    48,    49,     0,    54,    55,
      56,    57,    59,    58,    38,     0,    47
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -73,   -73,    84,    59,    19,    70,    60,   -15,    52,   -73,
      49,     5,    -7,   -73,    83,   -73,   -73,   -73,   -73,   -72,
     -73,   -13,    63,   -73
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     6,     7,    15,     9,    33,    34,    10,    55,    61,
      50,    51,    12,    20,    21,    13,    62,    63,    64,    75,
      76,    77,    66,    78
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      82,    83,    84,    35,    24,    11,    44,    99,    45,    14,
      24,    35,    17,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    16,    39,    18,    35,
      23,    26,    24,    57,    58,    59,    27,    28,    29,    30,
      31,    38,    65,   116,    24,    28,    29,    30,    31,    43,
      57,    58,    59,    42,    65,    19,    25,    43,    48,     8,
      60,    53,    36,    37,    57,    54,    32,    69,    70,    71,
       8,    72,    46,    54,    40,    73,    44,    80,    49,    85,
      86,    87,    88,    57,    79,    89,    90,    91,    74,    92,
      93,    94,    95,    96,    97,    22,    41,    81,   100,    85,
      86,    87,    88,    56,    52,    89,    90,    91,    67,    92,
      93,    94,    95,    96,    97,    98,    85,    86,    87,    88,
      47,    68,    89,    90,    91,     0,    92,    93,    94,    95,
      96,    97,   114,    85,    86,    87,    88,     0,     0,    89,
      90,    91,   115,    92,    93,    94,    95,    96,    97,    85,
      86,    87,    88,     0,     0,    89,    90,    91,     0,    92,
      93,    94,    95,    96,    97,    87,    88,     0,     0,    89,
      90,    91,     0,    92,    93,    94,    95,    96,    97,    89,
      90,    91,     0,    92,    93,    94,    95,    96,    97,    89,
       0,    91,     0,    92,    93,    94,    95,    96,    97,    91,
       0,    92,    93,    94,    95,    96,    97,     1,     2,     3,
       4,     5,    28,    29,    30,    31
};

static const yytype_int8 yycheck[] =
{
      72,    73,    74,    18,    11,     0,    31,    79,    33,     0,
      17,    26,     7,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     7,    22,    32,    44,
      11,    32,    39,     8,     9,    10,    17,     4,     5,     6,
       7,    22,    55,   115,    51,     4,     5,     6,     7,    34,
       8,     9,    10,    30,    67,     8,     8,    34,    39,     0,
      35,    30,    30,    31,     8,    34,    33,    11,    12,    13,
      11,    15,     8,    34,    33,    19,    31,    35,    33,    14,
      15,    16,    17,     8,    18,    20,    21,    22,    32,    24,
      25,    26,    27,    28,    29,    11,    26,    30,    33,    14,
      15,    16,    17,    54,    44,    20,    21,    22,    56,    24,
      25,    26,    27,    28,    29,    30,    14,    15,    16,    17,
      37,    58,    20,    21,    22,    -1,    24,    25,    26,    27,
      28,    29,    30,    14,    15,    16,    17,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    14,
      15,    16,    17,    -1,    -1,    20,    21,    22,    -1,    24,
      25,    26,    27,    28,    29,    16,    17,    -1,    -1,    20,
      21,    22,    -1,    24,    25,    26,    27,    28,    29,    20,
      21,    22,    -1,    24,    25,    26,    27,    28,    29,    20,
      -1,    22,    -1,    24,    25,    26,    27,    28,    29,    22,
      -1,    24,    25,    26,    27,    28,    29,     3,     4,     5,
       6,     7,     4,     5,     6,     7
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    38,    39,    40,    41,
      44,    48,    49,    52,     0,    40,    41,    48,    32,     8,
      50,    51,    39,    41,    49,     8,    32,    41,     4,     5,
       6,     7,    33,    42,    43,    44,    30,    31,    41,    48,
      33,    42,    30,    34,    31,    33,     8,    51,    41,    33,
      47,    48,    43,    30,    34,    45,    47,     8,     9,    10,
      35,    46,    53,    54,    55,    58,    59,    45,    59,    11,
      12,    13,    15,    19,    32,    56,    57,    58,    60,    18,
      35,    30,    56,    56,    56,    14,    15,    16,    17,    20,
      21,    22,    24,    25,    26,    27,    28,    29,    30,    56,
      33,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    30,    23,    56
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    38,    38,    38,    38,    38,    38,    39,    39,
      40,    41,    41,    42,    42,    43,    44,    44,    44,    44,
      45,    45,    46,    46,    46,    47,    47,    48,    48,    49,
      50,    50,    51,    52,    52,    52,    52,    52,    53,    54,
      55,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    57,    57,    57,    57,    57,    57,
      58,    59,    60,    60,    60
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     2,     3,     3,     4,     5,     4,
       2,     8,     7,     3,     1,     2,     1,     1,     1,     1,
       2,     0,     1,     1,     1,     0,     1,     1,     2,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       3,     3,     3,     3,     3,     2,     3,     5,     3,     3,
       2,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 22: /* statement: assignment_statement  */
#line 126 "parser.y"
                           { (yyval.stmt) = (yyvsp[0].asgn); }
#line 1350 "y.tab.c"
    break;

  case 23: /* statement: print_statement  */
#line 127 "parser.y"
                      { (yyval.stmt) = (yyvsp[0].write); }
#line 1356 "y.tab.c"
    break;

  case 24: /* statement: read_statement  */
#line 128 "parser.y"
                     { (yyval.stmt) = (yyvsp[0].read); }
#line 1362 "y.tab.c"
    break;

  case 29: /* var_decl_stmt: param_type var_decl_item_list SEMICOLON  */
#line 142 "parser.y"
                                              { /* TODO: Make SymTabEntry here */ }
#line 1368 "y.tab.c"
    break;

  case 38: /* assignment_statement: variable_as_operand ASSIGN_OP expression SEMICOLON  */
#line 163 "parser.y"
                                                         { (yyval.asgn) = new AssignAST((yyvsp[-3].var), (yyvsp[-1].expr)); }
#line 1374 "y.tab.c"
    break;

  case 39: /* print_statement: WRITE expression SEMICOLON  */
#line 167 "parser.y"
                                 { (yyval.write) = new WriteAST((yyvsp[-1].expr)); }
#line 1380 "y.tab.c"
    break;

  case 40: /* read_statement: READ variable_name SEMICOLON  */
#line 171 "parser.y"
                                   { (yyval.read) = new ReadAST((yyvsp[-1].var)); }
#line 1386 "y.tab.c"
    break;

  case 41: /* expression: expression PLUS expression  */
#line 175 "parser.y"
                                 { (yyval.expr) = new ExprAST(Operator::PLUS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1392 "y.tab.c"
    break;

  case 42: /* expression: expression MINUS expression  */
#line 176 "parser.y"
                                  { (yyval.expr) = new ExprAST(Operator::MINUS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1398 "y.tab.c"
    break;

  case 43: /* expression: expression MULT expression  */
#line 177 "parser.y"
                                 { (yyval.expr) = new ExprAST(Operator::MULT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1404 "y.tab.c"
    break;

  case 44: /* expression: expression DIV expression  */
#line 178 "parser.y"
                                { (yyval.expr) = new ExprAST(Operator::DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1410 "y.tab.c"
    break;

  case 45: /* expression: MINUS expression  */
#line 179 "parser.y"
                                    { (yyval.expr) = new ExprAST(Operator::UMINUS, (yyvsp[0].expr)); }
#line 1416 "y.tab.c"
    break;

  case 46: /* expression: LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET  */
#line 180 "parser.y"
                                                        { (yyval.expr) = (yyvsp[-1].expr); }
#line 1422 "y.tab.c"
    break;

  case 47: /* expression: expression QUESTION_MARK expression COLON expression  */
#line 181 "parser.y"
                                                           { (yyval.expr) = new ExprAST(Operator::QUESTION_MARK_COLON, (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1428 "y.tab.c"
    break;

  case 48: /* expression: expression AND expression  */
#line 182 "parser.y"
                                { (yyval.expr) = new ExprAST(Operator::AND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1434 "y.tab.c"
    break;

  case 49: /* expression: expression OR expression  */
#line 183 "parser.y"
                               { (yyval.expr) = new ExprAST(Operator::OR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1440 "y.tab.c"
    break;

  case 50: /* expression: NOT expression  */
#line 184 "parser.y"
                     { (yyval.expr) = new ExprAST(Operator::NOT, (yyvsp[0].expr)); }
#line 1446 "y.tab.c"
    break;

  case 51: /* expression: rel_expression  */
#line 185 "parser.y"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 1452 "y.tab.c"
    break;

  case 52: /* expression: variable_as_operand  */
#line 186 "parser.y"
                          { (yyval.expr) = (yyvsp[0].var); }
#line 1458 "y.tab.c"
    break;

  case 53: /* expression: constant_as_operand  */
#line 187 "parser.y"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 1464 "y.tab.c"
    break;

  case 54: /* rel_expression: expression LESS_THAN expression  */
#line 191 "parser.y"
                                      { (yyval.expr) = new ExprAST(Operator::LESS_THAN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1470 "y.tab.c"
    break;

  case 55: /* rel_expression: expression LESS_THAN_EQUAL expression  */
#line 192 "parser.y"
                                            { (yyval.expr) = new ExprAST(Operator::LESS_THAN_EQUAL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1476 "y.tab.c"
    break;

  case 56: /* rel_expression: expression GREATER_THAN expression  */
#line 193 "parser.y"
                                         { (yyval.expr) = new ExprAST(Operator::GREATER_THAN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1482 "y.tab.c"
    break;

  case 57: /* rel_expression: expression GREATER_THAN_EQUAL expression  */
#line 194 "parser.y"
                                               { (yyval.expr) = new ExprAST(Operator::GREATER_THAN_EQUAL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1488 "y.tab.c"
    break;

  case 58: /* rel_expression: expression NOT_EQUAL expression  */
#line 195 "parser.y"
                                      { (yyval.expr) = new ExprAST(Operator::NOT_EQUAL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1494 "y.tab.c"
    break;

  case 59: /* rel_expression: expression EQUAL expression  */
#line 196 "parser.y"
                                  { (yyval.expr) = new ExprAST(Operator::EQUAL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1500 "y.tab.c"
    break;

  case 60: /* variable_as_operand: variable_name  */
#line 200 "parser.y"
                    { (yyval.var) = (yyvsp[0].var); }
#line 1506 "y.tab.c"
    break;

  case 61: /* variable_name: NAME  */
#line 204 "parser.y"
           { (yyval.var) = (yyvsp[0].var); }
#line 1512 "y.tab.c"
    break;

  case 62: /* constant_as_operand: INT_NUM  */
#line 208 "parser.y"
              { (yyval.expr) = (yyvsp[0].iptr); }
#line 1518 "y.tab.c"
    break;

  case 63: /* constant_as_operand: FLOAT_NUM  */
#line 209 "parser.y"
                { (yyval.expr) = (yyvsp[0].fptr); }
#line 1524 "y.tab.c"
    break;

  case 64: /* constant_as_operand: STR_CONST  */
#line 210 "parser.y"
                { (yyval.expr) = (yyvsp[0].sptr); }
#line 1530 "y.tab.c"
    break;


#line 1534 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 213 "parser.y"


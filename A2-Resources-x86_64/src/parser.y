// TODO: Fix this file based on the changes in Ast.hpp and Program.hpp
%{
    #include "support.hpp"
    #include "Ast.hpp"
    #include "ParserContext.hpp"
    
    extern "C" int yylex(void);
    extern "C" int yyparse(ParserContext *);
    extern "C" void yyerror(const char *s);
%}

%parse-param { ParserContext *context }
%lex-param { ParserContext *context }

%union {
    Stmt_AST *stmt;
    Assign_AST *asgn;
    Read_AST *read;
    Write_AST *write;
    Expr_AST *expr;
    Var_AST *var;
    Binary_Expr_AST *rel;

    Type type;
    std::string *identifier;
    IdentifierList *identifier_list;
    DeclStmt decl_stmt;
    DeclStmtList *decl_stmt_list;

    Base_Expr_AST *constant;
    Int_Expr_AST *iptr;
    Float_Expr_AST *fptr;
    String_Expr_AST *sptr;

    // TODO: Add fields for parameter and return types
}

// Terminals (and optionally their types)
%token VOID
%token INTEGER
%token STRING
%token FLOAT
%token BOOL
%token READ
%token WRITE
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

%type <decl_stmt_list> var_decl_stmt_list
%type <decl_stmt> var_decl_stmt
%type <type> named_type
%type <identifier> var_decl_item
%type <identifier_list> var_decl_item_list

%token <identifier> NAME
%type <constant> constant_as_operand
%token <iptr> INT_NUM
%token <fptr> FLOAT_NUM
%token <sptr> STR_CONST

// Non-terminals and their types
%type <stmt> statement
%type <asgn> assignment_statement
%type <write> print_statement
%type <read> read_statement
%type <expr> expression
%type <rel> rel_expression
%type <var> variable_as_operand
%type <var> variable_name

%start program

// Disambiguation
%right QUESTION_MARK COLON
%left OR
%left AND
%left EQUAL NOT_EQUAL
%left LESS_THAN LESS_THAN_EQUAL GREATER_THAN GREATER_THAN_EQUAL
%left PLUS MINUS
%left MULT DIV
%right NOT UMINUS

%%

program
    : global_decl_stmt_list func_def_list
    | func_def_list
;

global_decl_stmt_list
    : global_decl_stmt_list func_decl
    | global_decl_stmt_list var_decl_stmt
    | var_decl_stmt
    | func_decl
;

func_decl
    : func_header LEFT_ROUND_BRACKET formal_param_list RIGHT_ROUND_BRACKET SEMICOLON
    | func_header LEFT_ROUND_BRACKET RIGHT_ROUND_BRACKET SEMICOLON
;

func_def_list
    : func_def_list func_def
    | func_def

func_header
    : named_type NAME { set_context_to_main(context, $1, $2); }
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
    : assignment_statement { $$ = $1; }
    | print_statement { $$ = $1; }
    | read_statement { $$ = $1; }
;

optional_local_var_decl_stmt_list
    : 
    | var_decl_stmt_list { add_to_local_sym_tab(context->main_func_ptr, $1); }
;

/* DONE */
var_decl_stmt_list
    : var_decl_stmt { $$ = process_var_decl_stmt_list($1); }
    | var_decl_stmt_list var_decl_stmt { $$ = process_var_decl_stmt_list($1, $2); }
;

/* DONE */
var_decl_stmt
    : named_type var_decl_item_list SEMICOLON { $$ = process_var_decl_stmt($1, $2); }
;

/* DONE */
var_decl_item_list
    : var_decl_item_list COMMA var_decl_item { $$ = process_var_decl_item_list($1, $3); }
    | var_decl_item { $$ = process_var_decl_item_list($1); }
;

/* DONE */
var_decl_item
    : NAME { $$ = $1; }
;

named_type
    : INTEGER { $$ = Type::INT; }
    | FLOAT { $$ = Type::FLOAT; }
    | VOID { $$ = Type::VOID; }
    | STRING { $$ = Type::STR; }
    | BOOL { $$ = Type::BOOL; }
;

assignment_statement
    : variable_as_operand ASSIGN_OP expression SEMICOLON { $$ = new AssignAST($1, $3); }
;

print_statement
    : WRITE expression SEMICOLON { $$ = new WriteAST($2); }
;

read_statement
    : READ variable_name SEMICOLON { $$ = new ReadAST($2); }
;

expression
    : expression PLUS expression { $$ = process_expr(Operator::ADD, $1, $3); }
    | expression MINUS expression { $$ = process_expr(Operator::SUBTRACT, $1, $3); }
    | expression MULT expression { $$ = process_expr(Operator::MULTIPLY, $1, $3); }
    | expression DIV expression { $$ = process_expr(Operator::DIVIDE, $1, $3); }
    | MINUS expression %prec UMINUS { $$ = process_expr(Operator::NEGATE, $2); }
    | LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET { $$ = $2; }
    | expression QUESTION_MARK expression COLON expression { $$ = process_expr(Operator::QUESTION_MARK_COLON, $1, $3, $5); }
    | expression AND expression { $$ = process_expr(Operator::LOGICAL_AND, $1, $3); } 
    | expression OR expression { $$ = process_expr(Operator::LOGICAL_OR, $1, $3); }
    | NOT expression { $$ = process_expr(Operator::LOGICAL_NOT, $2); }
    | rel_expression { $$ = $1; }
    | variable_as_operand { $$ = $1; }
    | constant_as_operand { $$ = $1; }
;

rel_expression
    : expression LESS_THAN expression { $$ = process_expr(Operator::LT, $1, $3); }
    | expression LESS_THAN_EQUAL expression { $$ = process_expr(Operator::LE, $1, $3); }
    | expression GREATER_THAN expression { $$ = process_expr(Operator::GT, $1, $3); }
    | expression GREATER_THAN_EQUAL expression { $$ = process_expr(Operator::GE, $1, $3); }
    | expression NOT_EQUAL expression { $$ = process_expr(Operator::NE, $1, $3); }
    | expression EQUAL expression { $$ = process_expr(Operator::EQ, $1, $3); }
;

variable_as_operand
    : variable_name { $$ = $1; }
;

variable_name
    : NAME { $$ = $1; }
;

constant_as_operand
    : INT_NUM { $$ = $1; }
    | FLOAT_NUM { $$ = $1; }
    | STR_CONST { $$ = $1; }
;

%%

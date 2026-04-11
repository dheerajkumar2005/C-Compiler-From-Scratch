%code requires {
    #include "support.hpp"
    #include "Ast.hpp"
    #include "Program.hpp"
    
    extern "C" int yylex(Scope *);
    extern "C" int yyparse(Scope *);
    extern "C" void yyerror(Scope *, const char *);
}
%{
    #include "Program.hpp" 
    #include "TAC.hpp"

    extern int sa_parse;
    
    extern TAC_Label *return_label;
    extern Shared_Temporary_TAC_Operand *return_stemp;
%}

%parse-param { Scope *curr_scope }
%lex-param { Scope *curr_scope }

%union {
    StatementList *stmt_list;
    Statement_Ast *stmt;
    Assignment_Stmt_Ast *asgn;
    Read_Stmt_Ast *read;
    Write_Stmt_Ast *write;
    Return_Stmt_Ast *_return;
    Call_Stmt_Ast *call_stmt;
    Expression_Ast *expr;
    Name_Expr_Ast *var;
    Relational_Expr_Ast *rel;
    Function_Call_Ast *call_ast;

    Type type;
    std::string *identifier;
    IdentifierList *identifier_list;
    DeclStmt *decl_stmt;
    DeclStmtList *decl_stmt_list;

    FormalParam *formal_param;
    FormalParamList *formal_param_list;

    FuncHeader *func_header;

    Base_Expr_Ast *constant;
    Int_Expr_Ast *iptr;
    Float_Expr_Ast *fptr;
    String_Expr_Ast *sptr;

    Compound_Stmt_Ast *compound_stmt;
    If_Stmt_Ast *if_stmt;

    While_Stmt_Ast *while_stmt;

    Do_While_Stmt_Ast *do_while_stmt;

    std::vector<Expression_Ast *> *arg_list;
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
%token IF
%token ELSE
%token DO
%token WHILE
%token RETURN

%type <func_header> func_header
%type <formal_param_list> formal_param_list
%type <formal_param> formal_param
%type <type> param_type

%type <type> named_type
%type <identifier> var_decl_item
%type <identifier_list> var_decl_item_list

%token <identifier> NAME
%type <constant> constant_as_operand
%token <iptr> INT_NUM
%token <fptr> FLOAT_NUM
%token <sptr> STR_CONST

// Non-terminals and their types
%type <stmt_list> statement_list
%type <stmt> statement
%type <asgn> assignment_statement
%type <write> print_statement
%type <read> read_statement
%type <call_stmt> call_statement
%type <call_ast> func_call
%type <_return> return_statement
%type <expr> expression if_condition actual_arg
%type <rel> rel_expression
%type <var> variable_as_operand variable_name
%type <compound_stmt> compound_statement
%type <if_stmt> unmatched_if if_statement
%type <do_while_stmt> do_while_statement
%type <while_stmt> while_statement
%type <arg_list> actual_arg_list non_empty_arg_list

%start program

// Disambiguation
%nonassoc lower_than_else
%nonassoc ELSE

%right QUESTION_MARK COLON
%left OR
%left AND
%right NOT
%nonassoc NOT_EQUAL EQUAL LESS_THAN LESS_THAN_EQUAL GREATER_THAN GREATER_THAN_EQUAL
%left PLUS MINUS
%left MULT DIV
%right UMINUS

%%

program
    : global_decl_stmt_list func_def_list { print_func_def_list(curr_scope); }
    | func_def_list { print_func_def_list(curr_scope); }
;

global_decl_stmt_list
    : global_decl_stmt_list func_decl
    | global_decl_stmt_list var_decl_stmt
    | var_decl_stmt
    | func_decl
;

func_decl
    : func_header LEFT_ROUND_BRACKET formal_param_list RIGHT_ROUND_BRACKET SEMICOLON { process_func_decl(curr_scope, $1, $3); }
    | func_header LEFT_ROUND_BRACKET RIGHT_ROUND_BRACKET SEMICOLON { process_func_decl(curr_scope, $1); }
;

func_def_list
    : func_def_list func_def  
    | func_def 

/* NOTE: formal param and func header look the same */
func_header
    : named_type NAME { $$ = accumulate_func_header($1, $2); } 
;

func_def
    : func_header LEFT_ROUND_BRACKET formal_param_list RIGHT_ROUND_BRACKET 
    { 
        if(!sa_parse) 
        {
            // Add it to old symtab or match with existing signature
            Func_Signature *func_sig = make_func_sig(curr_scope, $1, $3);

            // Push the new scope
            curr_scope = make_func_scope(curr_scope, func_sig);

            // return_label = func_sig->
        }
    }
    LEFT_CURLY_BRACKET optional_local_var_decl_stmt_list statement_list RIGHT_CURLY_BRACKET 
    {
        if (!sa_parse)
        {
            Scope *parent_scope = curr_scope->parent_scope;
            process_body(parent_scope, curr_scope, $8);
            curr_scope = parent_scope;
        }
    }
    
    | func_header LEFT_ROUND_BRACKET RIGHT_ROUND_BRACKET 
    {
        if (!sa_parse) 
        {
            // Add it to old symtab or match with existing signature
            Func_Signature *func_sig = make_func_sig(curr_scope, $1);

            // Push the new scope
            curr_scope = make_func_scope(curr_scope, func_sig);
        }
    } 
    LEFT_CURLY_BRACKET optional_local_var_decl_stmt_list statement_list RIGHT_CURLY_BRACKET 
    {
        if (!sa_parse)
        {
            Scope *parent_scope = curr_scope->parent_scope;
            process_body(parent_scope, curr_scope, $7);
            curr_scope = parent_scope;
        }
    }
;

formal_param_list
    : formal_param_list COMMA formal_param { $$ = accumulate_formal_param_list($1, $3); }
    | formal_param { $$ = accumulate_formal_param_list($1); }
;

formal_param
    : param_type NAME { $$ = accumulate_formal_param($1, $2); }
;

param_type
    : INTEGER { $$ = Type::INT; }
    | FLOAT { $$ = Type::FLOAT; }
    | STRING { $$ = Type::STR; }
    | BOOL { $$ = Type::BOOL; } 
;

statement_list
    : statement_list statement { $$ = accumulate_stmt_list($1, $2); }
    | { $$ = accumulate_stmt_list(); }
;

statement
    : assignment_statement { $$ = $1; }
    | if_statement { $$ = $1; }
    | do_while_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    | compound_statement { $$ = $1; }
    | print_statement { $$ = $1; }
    | read_statement { $$ = $1; }
    | call_statement { $$ = $1; }
    | return_statement { $$ = $1; }
;

call_statement
    : func_call SEMICOLON { $$ = sa_parse ? nullptr : process_call_stmt($1); }
;

func_call
    : NAME LEFT_ROUND_BRACKET actual_arg_list RIGHT_ROUND_BRACKET { $$ = sa_parse ? nullptr : new Function_Call_Ast(*$1, $3); }
;

actual_arg_list
    : non_empty_arg_list { $$ = sa_parse ? nullptr : $1; }
    | { $$ = sa_parse ? nullptr : new std::vector<Expression_Ast *>(); }
;

non_empty_arg_list
    : non_empty_arg_list COMMA actual_arg {
        if(sa_parse) 
        {
            $1->push_back($3);
            $$ = $1;
        } 
        else 
        {
            $$ = nullptr;
        }
    }
    | actual_arg {
        if(sa_parse)
        {
            $$ = new std::vector<Expression_Ast *>();
            $$->push_back($1);
        } 
        else 
        {
            $$ = nullptr;
        }
    }
;

actual_arg
    : expression { $$ = sa_parse ? nullptr : $1; }
;

return_statement
    : RETURN expression SEMICOLON { $$ = new Return_Stmt_Ast($2, return_label, return_stemp); }
;

optional_local_var_decl_stmt_list
    : 
    | var_decl_stmt_list
;

var_decl_stmt_list
    : var_decl_stmt
    | var_decl_stmt_list var_decl_stmt
;

var_decl_stmt
    : named_type var_decl_item_list SEMICOLON { process_var_decl_stmt(curr_scope, $1, $2); }
;

var_decl_item_list
    : var_decl_item_list COMMA var_decl_item { $$ = accumulate_var_decl_item_list($1, $3); }
    | var_decl_item { $$ = accumulate_var_decl_item_list($1); }
;

var_decl_item
    : NAME { $$ = $1; }
;

/* FIXED */
named_type
    : INTEGER { $$ = Type::INT; }
    | FLOAT { $$ = Type::FLOAT; }
    | VOID { $$ = Type::VOID; }
    | STRING { $$ = Type::STR; }
    | BOOL { $$ = Type::BOOL; }
;

assignment_statement
    : variable_as_operand ASSIGN_OP expression SEMICOLON { $$ = sa_parse ? nullptr : new Assignment_Stmt_Ast($1, $3); }
    | variable_as_operand ASSIGN_OP func_call SEMICOLON { $$ = sa_parse ? nullptr : new Assignment_Stmt_Ast($1, $3); }
;

if_condition
    : LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET { $$ = process_predicate($2); }
;

unmatched_if
    : IF if_condition statement { $$ = sa_parse ? nullptr : new If_Stmt_Ast($2, $3); }
;

if_statement
    : unmatched_if ELSE statement { $$ = sa_parse ? nullptr : add_else_clause($1, $3); }
    | unmatched_if %prec lower_than_else { $$ = sa_parse ? nullptr : $1; }
;

do_while_statement
    : DO statement WHILE LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET SEMICOLON { $$ = sa_parse ? nullptr : new Do_While_Stmt_Ast($5, $2); }
;

while_statement
    : WHILE LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET statement { $$ = sa_parse ? nullptr : new While_Stmt_Ast($3, $5); }
;

compound_statement
    : LEFT_CURLY_BRACKET statement_list RIGHT_CURLY_BRACKET { $$ = sa_parse ? nullptr : new Compound_Stmt_Ast($2);  }

print_statement
    : WRITE expression SEMICOLON { $$ = sa_parse ? nullptr : new Write_Stmt_Ast($2); }
;

read_statement
    : READ variable_name SEMICOLON { $$ = sa_parse ? nullptr : new Read_Stmt_Ast($2); }
;

expression
    : expression PLUS expression { $$ = sa_parse ? nullptr : new Plus_Expr_Ast($1, $3); }
    | expression MINUS expression { $$ = sa_parse ? nullptr : new Minus_Expr_Ast($1, $3); }
    | expression MULT expression { $$ = sa_parse ? nullptr : new Mult_Expr_Ast($1, $3); }
    | expression DIV expression { $$ = sa_parse ? nullptr : new Div_Expr_Ast($1, $3); }
    | MINUS expression %prec UMINUS { $$ = sa_parse ? nullptr : new UMinus_Expr_Ast($2); }
    | LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET { $$ = $2; }
    | expression QUESTION_MARK expression COLON expression { $$ = sa_parse ? nullptr : new Conditional_Expr_Ast(curr_scope, $1, $3, $5); }
    | expression AND expression { $$ = sa_parse ? nullptr : new Boolean_Expr_Ast(Binary_Operator::LOGICAL_AND, $1, $3); } 
    | expression OR expression { $$ = sa_parse ? nullptr : new Boolean_Expr_Ast(Binary_Operator::LOGICAL_OR, $1, $3); }
    | NOT expression { $$ = sa_parse ? nullptr : new Logical_Not_Expr_Ast($2); }
    | rel_expression { $$ = $1; }
    | variable_as_operand { $$ = $1; }
    | constant_as_operand { $$ = $1; }
;

rel_expression
    : expression LESS_THAN expression { $$ = sa_parse ? nullptr : new Relational_Expr_Ast(Binary_Operator::LT, $1, $3); }
    | expression LESS_THAN_EQUAL expression { $$ = sa_parse ? nullptr : new Relational_Expr_Ast(Binary_Operator::LE, $1, $3); }
    | expression GREATER_THAN expression { $$ = sa_parse ? nullptr : new Relational_Expr_Ast(Binary_Operator::GT, $1, $3); }
    | expression GREATER_THAN_EQUAL expression { $$ = sa_parse ? nullptr : new Relational_Expr_Ast(Binary_Operator::GE, $1, $3); }
    | expression NOT_EQUAL expression { $$ = sa_parse ? nullptr : new Relational_Expr_Ast(Binary_Operator::NE, $1, $3); }
    | expression EQUAL expression { $$ = sa_parse ? nullptr : new Relational_Expr_Ast(Binary_Operator::EQ, $1, $3); }
;

variable_as_operand
    : variable_name { $$ = $1; }
;

variable_name
    : NAME { $$ = process_variable_name(curr_scope, $1); }
;

constant_as_operand
    : INT_NUM { $$ = $1; }
    | FLOAT_NUM { $$ = $1; }
    | STR_CONST { $$ = $1; }
;

%%

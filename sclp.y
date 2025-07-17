%{
#include "common-headers.hh"

extern "C" void yyerror(char *s);

extern int yylex(void);
extern func_signature* already_existing_func;
extern map<string, vector<AST_Statement*>> all_ast_statements;
extern bool stop_after_parse;
extern vector<pair<func_signature*, pair<bool,bool>>> all_func_signatures;
extern map<string, int> addr_mapping;
extern int string_const_rtl_opd_number;
extern int saved_tac_temp_count;
%}

%union{
    string* str;
    vec_var_type* var_type;
    int integer;
    double decimal_num;
    AST_Statement* stmt;
    AST_Expr* expr;
    AST_Var* var;
    func_signature* func_sig;
    pair<int, string*>* header;
    st_list_vector* st_vector;
    expr_list_vector* expr_vector;
}

%type <str> NAME STR_CONST var_decl
%type <integer> INT_NUM nm_tp param_tp
%type <decimal_num> FLOAT_NUM
%type <var_type> var_decl_list formal_param formal_param_list optional_local_var_decl_st_list var_decl_st var_decl_st_list
%type <var> var_name
%type <stmt> ass_st read_st print_st compound_st while_st do_while_st if_st st call_st return_st
%type <expr> const_as_op if_cond relational_expression expression actual_arg func_call
%type <func_sig> func_signature func_signature_with_optional_local_var
%type <header> func_header
%type <st_vector> st_list
%type <expr_vector> actual_arg_list non_empty_arg_list

%token WHILE
%token IF
%token DO
%token ELSE
%token INTEGER
%token FLOAT
%token BOOL
%token STRING
%token VOID
%token WRITE
%token READ
%token NAME
%token RETURN
%token ASSIGN_OP
%token GREATER_THAN
%token LESS_THAN
%token GREATER_THAN_EQUAL
%token LESS_THAN_EQUAL
%token NOT_EQUAL
%token EQUAL
%token PLUS
%token MINUS
%token MULT
%token DIV
%token AND
%token OR
%token NOT
%token ADDRESSOF
%token LEFT_CURLY_BRACKET
%token RIGHT_CURLY_BRACKET
%token LEFT_ROUND_BRACKET
%token RIGHT_ROUND_BRACKET
%token LEFT_SQUARE_BRACKET
%token RIGHT_SQUARE_BRACKET
%token SEMICOLON
%token COMMA
%token QUESTION_MARK
%token COLON
%token FLOAT_NUM
%token INT_NUM
%token STR_CONST
%token ERROR

%right IF ELSE
%right QUESTION_MARK COLON
%left OR
%left AND
%right NOT
%left NOT_EQUAL EQUAL
%left GREATER_THAN GREATER_THAN_EQUAL LESS_THAN LESS_THAN_EQUAL
%left PLUS MINUS
%left MULT DIV
%right UMINUS

%start Run
%%
Run
    : global_decl_st_list func_def_list
    | func_def_list
    ;

global_decl_st_list
    : global_decl_st_list func_decl
    | global_decl_st_list var_decl_st           {if (!stop_after_parse) add_variables_to_global_symtab($2);}
    | var_decl_st                               {if (!stop_after_parse) add_variables_to_global_symtab($1);}
    | func_decl
    ;

func_decl
    : func_signature                            {if (!stop_after_parse) push_func_to_vector($1, true);}
      SEMICOLON                                 {if (!stop_after_parse) {already_existing_func = $1; vector<AST_Statement*>* empty_vec = new vector<AST_Statement*>(); if ($1->func_name != "main") all_ast_statements[$1->func_name + "_"] = *empty_vec; else all_ast_statements[$1->func_name] = *empty_vec;}}
    ;

func_def_list
    : func_def_list func_def
    | func_def
    ;

func_header
    : nm_tp NAME                                {if (!stop_after_parse) $$ = new pair<int, string*>($1, $2);}
    ;

func_def
    : func_signature_with_optional_local_var st_list RIGHT_CURLY_BRACKET
                                                {
                                                    if (!stop_after_parse)
                                                    {
                                                        if ($1->func_name != "main") all_ast_statements[$1->func_name + "_"] = $2->statements; else all_ast_statements[$1->func_name] = $2->statements;
                                                    }
                                                }
    ;

func_signature_with_optional_local_var
    : func_signature                            {if (!stop_after_parse) push_func_to_vector($1, false);}
      LEFT_CURLY_BRACKET optional_local_var_decl_st_list
                                                {if (!stop_after_parse) {add_variables_to_local_symtab($1->param_list);
                                                add_variables_to_local_symtab($4); $$ = $1;}}
    ;

func_signature
    : func_header LEFT_ROUND_BRACKET formal_param_list RIGHT_ROUND_BRACKET
                                                {if (!stop_after_parse) $$ = new func_signature(static_cast<data_type>($1->first), $1->second, $3);}
    | func_header LEFT_ROUND_BRACKET RIGHT_ROUND_BRACKET
                                                {if (!stop_after_parse) $$ = new func_signature(static_cast<data_type>($1->first), $1->second);}
    ;

formal_param_list
    : formal_param_list COMMA formal_param      {if (!stop_after_parse) {$1->append_another($3); $$ = $1;}}
    | formal_param                              {if (!stop_after_parse) $$ = $1;}
    ;

formal_param
    : param_tp NAME                             {if (!stop_after_parse) $$ = new vec_var_type($2, $1);}
    ;

param_tp
    : INTEGER                                   {if (!stop_after_parse) $$ = INTEGER__h;}
    | FLOAT                                     {if (!stop_after_parse) $$ = FLOAT__h;}
    | STRING                                    {if (!stop_after_parse) $$ = STRING__h;}
    | BOOL                                      {if (!stop_after_parse) $$ = BOOL__h;}
    ;

st_list
    : st_list st                                {if (!stop_after_parse) {$1->append($2); $$ = $1;}}
    | %empty                                    {if (!stop_after_parse) $$ = new st_list_vector();}
    ;

st
    : ass_st                                    {if (!stop_after_parse) $$ = $1;}
    | if_st                                     {if (!stop_after_parse) $$ = $1;}
    | do_while_st                               {if (!stop_after_parse) $$ = $1;}
    | while_st                                  {if (!stop_after_parse) $$ = $1;}
    | compound_st                               {if (!stop_after_parse) $$ = $1;}
    | print_st                                  {if (!stop_after_parse) $$ = $1;}
    | read_st                                   {if (!stop_after_parse) $$ = $1;}
    | call_st                                   {if (!stop_after_parse) $$ = $1;}
    | return_st                                 {if (!stop_after_parse) $$ = $1;}
    ;

call_st
    : func_call SEMICOLON                       {if (!stop_after_parse) $$ = new AST_Call_Statement($1);}
    ;

func_call
    : NAME LEFT_ROUND_BRACKET actual_arg_list RIGHT_ROUND_BRACKET
                                                {if (!stop_after_parse) $$ = new AST_Function_Call_Expr($1, $3);}
    ;

actual_arg_list
    : non_empty_arg_list                        {if (!stop_after_parse) $$ = $1;}
    | %empty                                    {if (!stop_after_parse) $$ = new expr_list_vector();}
    ;

non_empty_arg_list
    : non_empty_arg_list COMMA actual_arg       {if (!stop_after_parse) {$1->append($3); $$ = $1;}}
    | actual_arg                                {if (!stop_after_parse) {$$ = new expr_list_vector(); $$->append($1);}}
    ;

actual_arg
    : expression                                {if (!stop_after_parse) $$ = $1;}
    ;

return_st
    : RETURN expression SEMICOLON               {if (!stop_after_parse) $$ = new AST_Return_Statement($2);}
    ;

optional_local_var_decl_st_list
    : %empty                                    {if (!stop_after_parse) $$ = new vec_var_type();}
    | var_decl_st_list                          {if (!stop_after_parse) $$ = $1;}
    ;

var_decl_st_list
    : var_decl_st                               {if (!stop_after_parse) $$ = $1;}
    | var_decl_st_list var_decl_st              {if (!stop_after_parse) {$1->append_another($2); $$ = $1;}}
    ;

var_decl_st
    : nm_tp var_decl_list SEMICOLON             {if (!stop_after_parse) {$2->set_type_to_all($1); $$ = $2;}}
    ;

var_decl_list
    : var_decl_list COMMA var_decl              {if (!stop_after_parse) {$1->add_name($3); $$ = $1;}}
    | var_decl                                  {if (!stop_after_parse) $$ = new vec_var_type($1);}
    ;

var_decl
    : NAME                                      {if (!stop_after_parse) $$ = $1;}
    ;

nm_tp
    : INTEGER                                   {if (!stop_after_parse) $$ = INTEGER__h;}
    | FLOAT                                     {if (!stop_after_parse) $$ = FLOAT__h;}
    | VOID                                      {if (!stop_after_parse) $$ = VOID__h;}
    | STRING                                    {if (!stop_after_parse) $$ = STRING__h;}
    | BOOL                                      {if (!stop_after_parse) $$ = BOOL__h;}
    ;

ass_st
    : var_name ASSIGN_OP expression SEMICOLON   {if (!stop_after_parse) $$ = new AST_Assign_Statement($1, $3);}
    | var_name ASSIGN_OP func_call SEMICOLON    {if (!stop_after_parse) $$ = new AST_Assign_Statement($1, $3);}
    ;

if_st
    : IF if_cond st ELSE st                     {if (!stop_after_parse) $$ = new AST_If_Statement($2, $3, $5);}
    | IF if_cond st                             {if (!stop_after_parse) $$ = new AST_If_Statement($2, $3);}
    ;

do_while_st
    : DO st WHILE LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET SEMICOLON
                                                {if (!stop_after_parse) $$ = new AST_Do_While_Statement($5, $2);}
    ;

while_st
    : WHILE LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET st
                                                {if (!stop_after_parse) $$ = new AST_While_Statement($3, $5);}
    ;

compound_st
    : LEFT_CURLY_BRACKET st_list RIGHT_CURLY_BRACKET
                                                {if (!stop_after_parse) $$ = new AST_Compound_Statement($2);}
    ;

print_st
    : WRITE expression SEMICOLON                {if (!stop_after_parse) $$ = new AST_Print_Statement($2);}
    ;

read_st
    : READ var_name SEMICOLON                   {if (!stop_after_parse) $$ = new AST_Read_Statement($2);}
    ;

expression
    : expression PLUS expression                {if (!stop_after_parse) $$ = new AST_Arith_Expr($1, $3, PLUS__h);}
    | expression MINUS expression               {if (!stop_after_parse) $$ = new AST_Arith_Expr($1, $3, MINUS__h);}
    | expression MULT expression                {if (!stop_after_parse) $$ = new AST_Arith_Expr($1, $3, MULT__h);}
    | expression DIV expression                 {if (!stop_after_parse) $$ = new AST_Arith_Expr($1, $3, DIV__h);}
    | MINUS expression %prec UMINUS             {if (!stop_after_parse) $$ = new AST_Uminus_Expr($2);}
    | LEFT_ROUND_BRACKET expression RIGHT_ROUND_BRACKET
                                                {if (!stop_after_parse) $$ = $2;}
    | expression AND expression                 {if (!stop_after_parse) $$ = new AST_Logical_Expr($1, $3, AND__h);}
    | expression OR expression                  {if (!stop_after_parse) $$ = new AST_Logical_Expr($1, $3, OR__h);}
    | NOT expression                            {if (!stop_after_parse) $$ = new AST_Not_Expr($2);}
    | relational_expression                     {if (!stop_after_parse) $$ = $1;}
    | var_name                                  {if (!stop_after_parse) $$ = new AST_Name_Expr($1);}
    | const_as_op                               {if (!stop_after_parse) $$ = $1;}
    ;

relational_expression
    : expression LESS_THAN expression           {if (!stop_after_parse) $$ = new AST_Rel_Expr($1, $3, LESS_THAN__h);}
    | expression LESS_THAN_EQUAL expression     {if (!stop_after_parse) $$ = new AST_Rel_Expr($1, $3, LESS_THAN_EQUAL__h);}
    | expression GREATER_THAN expression        {if (!stop_after_parse) $$ = new AST_Rel_Expr($1, $3, GREATER_THAN__h);}
    | expression GREATER_THAN_EQUAL expression  {if (!stop_after_parse) $$ = new AST_Rel_Expr($1, $3, GREATER_THAN_EQUAL__h);}
    | expression NOT_EQUAL expression           {if (!stop_after_parse) $$ = new AST_Rel_Expr($1, $3, NOT_EQUAL__h);}
    | expression EQUAL expression               {if (!stop_after_parse) $$ = new AST_Rel_Expr($1, $3, EQUAL__h);}
    ;

var_name
    : NAME                                      {if (!stop_after_parse) $$ = new AST_Var($1);}
    ;

const_as_op
    : INT_NUM                                   {if (!stop_after_parse) $$ = new AST_Number_Expr<int>($1, INTEGER__h);}
    | FLOAT_NUM                                 {if (!stop_after_parse) $$ = new AST_Number_Expr<double>($1, FLOAT__h);}
    | STR_CONST                                 {
                                                    if (!stop_after_parse)
                                                    {
                                                        $$ = new AST_String_Expr($1, STRING__h);
                                                        if (addr_mapping.count(*($1)) == 0)
                                                        {
                                                            addr_mapping[*($1)] = string_const_rtl_opd_number++;
                                                        }
                                                    }
                                                }
    ;
%%
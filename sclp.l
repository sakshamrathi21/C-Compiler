%{
#include "common-headers.hh"
#include "y.tab.h"

void store_token_name (string token_name, char* lexeme, int lineno);

extern FILE* yyin;
%}

%option noyywrap

digit [0-0]
alpha [a-zA-Z_]
plus [+]
minus [-]
mult [*]
div [/]
semicolon [;]
assign_op [=]
dot [.]
ap ["]
%%
{div}{div}.*                    {}
int                             {store_token_name("INTEGER", yytext, yylineno) return INTEGER;}
bool                            {store_token_name ("BOOL", yytext, yylineno); return BOOL;}
float                            {store_token_name ("FLOAT", yytext, yylineno); return FLOAT;}
void                            {store_token_name ("VOID", yytext, yylineno); return VOID;}
read                            {store_token_name ("READ", yytext, yylineno); return READ;}
do                            {store_token_name ("DO", yytext, yylineno); return DO;}
while                            {store_token_name ("WHILE", yytext, yylineno); return WHILE;}
string                            {store_token_name ("STRING", yytext, yylineno); return STRING;}
return                            {store_token_name ("RETURN", yytext, yylineno); return RETURN;}
if                            {store_token_name ("IF", yytext, yylineno); return IF;}
else                            {store_token_name ("ELSE", yytext, yylineno); return ELSE;}
print                            {store_token_name ("WRITE", yytext, yylineno); return WRITE;}
{alpha}({alpha}|{digit})*        {yylval.str = new string(yytext); store_token_name("NAME", yytext, yylineno); return NAME;}
{digit}+                         {yylval.integer = atoi(yytext); store_token_name("INT_NUM", yytext, yylineno); return INT_NUM;}
{plus}                            {store_token_name ("PLUS", yytext, yylineno); return PLUS;}      
{minus}                            {store_token_name ("MINUS", yytext, yylineno); return MINUS;} 
{mult}                            {store_token_name ("MULT", yytext, yylineno); return MULT;} 
{div}                            {store_token_name ("DIV", yytext, yylineno); return DIV;}
{semicolon}                            {store_token_name ("SEMICOLON", yytext, yylineno); return SEMICOLON;}
{digit}*{dot}{digit}+|{digit}+{dot}{digit}* {yylval.decimal_num = atof(yytext); store_token_name("FLOAT_NUM", yytext, yylineno); return FLOAT_NUM;}
{ap}[^"\n]*{ap}                         {yylval.str = new string(yytext); store_token_name("STR_CONST", yytext, yylineno) return STR_CONST;}
{assign_op}{assign_op}                  {store_token_name("EQUAL", yytext, yylineno); return EQUAL;}
"!="                                    {store_token_name("NOT_EQUAL", yytext, yylineno); return NOT_EQUAL;}
{assign_op}                             {store_token_name("ASSIGN_OP", yytext, yylineno); return ASSIGN_OP;}
">"                            {store_token_name("GREATER_THAN", yytext, yylineno); return GREATER_THAN;}
"<"                            {store_token_name("LESS_THAN", yytext, yylineno); return LESS_THAN;}
"<="                            {store_token_name("LESS_THAN_EQUAL", yytext, yylineno); return LESS_THAN_EQUAL;}
">="                            {store_token_name("GREATER_THAN_EQUAL", yytext, yylineno); return GREATER_THAN_EQUAL;}
"("                            {store_token_name("LEFT_ROUND_BRACKET", yytext, yylineno); return LEFT_ROUND_BRACKET;}
")"                            {store_token_name("RIGHT_ROUND_BRACKET", yytext, yylineno); return RIGHT_ROUND_BRACKET;}
"!"                            {store_token_name("NOT", yytext, yylineno); return NOT;}
"&&"                            {store_token_name("AND", yytext, yylineno); return AND;}
"||"                            {store_token_name("OR", yytext, yylineno); return OR;}
"&"                            {store_token_name("ADDRESSOF", yytext, yylineno); return ADDRESSOF;}
"{"                            {store_token_name("LEFT_CURLY_BRACKET", yytext, yylineno); return LEFT_CURLY_BRACKET;}
"}"                            {store_token_name("RIGHT_CURLY_BRACKET", yytext, yylineno); return RIGHT_CURLY_BRACKET;}
"["                            {store_token_name("LEFT_SQUARE_BRACKET", yytext, yylineno); return LEFT_SQUARE_BRACKET;}
"]"                            {store_token_name("RIGHT_SQUARE_BRACKET", yytext, yylineno); return RIGHT_SQUARE_BRACKET;}
":"                            {store_token_name("COLON", yytext, yylineno); return COLON;}
"?"                            {store_token_name("QUESTION_MARK", yytext, yylineno); return QUESTION_MARK;}
","                            {store_token_name("COMMA", yytext, yylineno); return COMMA;}
[ \t]+                          {}
\n                              {}
.                               {throw_error_and_exit("Invalid Lexeme Detected -- Lexical Error");}
%%
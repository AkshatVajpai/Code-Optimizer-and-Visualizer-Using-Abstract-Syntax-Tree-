%code requires {
    #include "ast.h"
}

%{
#include <stdio.h>
#include "ast.h"

extern int yylex();
extern char* yytext;
extern int yylineno;

void yyerror(const char* s) { 
    fprintf(stderr, "Parse error: %s\n", s); 
    fprintf(stderr, "Line %d: ", yylineno);
    fprintf(stderr, "Unexpected token: %s\n", yytext);
}

ASTNode* ast_root;
%}

%union {
    int ival;
    char* str;
    ASTNode* node;
}

%token <ival> NUMBER
%token <str> IDENTIFIER
%token <str> STRING

%token KW_INT KW_IF KW_FOR KW_RETURN KW_ELSE
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON ASSIGN COMMA
%token PLUS MINUS MUL DIV LT GT LE GE EQ NE AND OR NOT
%token INCR DECR

%type <node> stmt stmt_list compound_stmt expr expr_list decl_stmt
               if_stmt for_stmt return_stmt function type program for_init
               else_stmt

%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MUL DIV
%right NOT
%right INCR DECR

%start program

%%

program:
      function                          { ast_root = $1; }
    ;

function:
      type IDENTIFIER LPAREN RPAREN compound_stmt
                                        { $$ = make_function_node($2, $5); }
    ;

type:
      KW_INT                            { $$ = make_type_node("int"); }
    ;

stmt_list:
      stmt                              { $$ = $1; }
    | stmt_list stmt                    { $$ = make_seq_node($1, $2); }
    ;

compound_stmt:
      LBRACE stmt_list RBRACE           { $$ = $2; }
    | LBRACE RBRACE                     { $$ = NULL; }
    ;

stmt:
      decl_stmt                         { $$ = $1; }
    | expr SEMICOLON                    { $$ = $1; }
    | if_stmt                           { $$ = $1; }
    | for_stmt                          { $$ = $1; }
    | return_stmt                       { $$ = $1; }
    | compound_stmt                     { $$ = $1; }
    ;

decl_stmt:
      KW_INT IDENTIFIER ASSIGN expr SEMICOLON
                                        { $$ = make_decl_node($2, $4); }
    | KW_INT IDENTIFIER SEMICOLON       { $$ = make_decl_node($2, NULL); }
    ;

if_stmt:
      KW_IF LPAREN expr RPAREN compound_stmt else_stmt
                                        { 
                                          if ($6) {
                                              ASTNode* if_node = make_if_node($3, $5);
                                              if_node->next = $6;  // Link else statement to if node
                                              $$ = if_node;
                                          } else {
                                              $$ = make_if_node($3, $5);
                                          }
                                        }
    ;

else_stmt:
      KW_ELSE compound_stmt             { $$ = $2; }
    | /* empty */                       { $$ = NULL; }
    ;

for_init:
      KW_INT IDENTIFIER ASSIGN expr     { $$ = make_decl_node($2, $4); }
    | KW_INT IDENTIFIER                 { $$ = make_decl_node($2, NULL); }
    | expr                              { $$ = $1; }
    | /* empty */                       { $$ = NULL; }
    ;

for_stmt:
      KW_FOR LPAREN for_init SEMICOLON expr SEMICOLON expr RPAREN compound_stmt
                                        { $$ = make_for_node($3, $5, $7, $9); }
    ;

return_stmt:
      KW_RETURN expr SEMICOLON          { $$ = make_return_node($2); }
    | KW_RETURN SEMICOLON               { $$ = make_return_node(NULL); }
    ;

expr:
      expr PLUS expr                    { $$ = make_binop_node('+', $1, $3); }
    | expr MINUS expr                   { $$ = make_binop_node('-', $1, $3); }
    | expr MUL expr                     { $$ = make_binop_node('*', $1, $3); }
    | expr DIV expr                     { $$ = make_binop_node('/', $1, $3); }
    | expr LT expr                      { $$ = make_binop_node('<', $1, $3); }
    | expr GT expr                      { $$ = make_binop_node('>', $1, $3); }
    | expr LE expr                      { $$ = make_binop_node('L', $1, $3); }
    | expr GE expr                      { $$ = make_binop_node('G', $1, $3); }
    | expr EQ expr                      { $$ = make_binop_node('=', $1, $3); }
    | expr NE expr                      { $$ = make_binop_node('!', $1, $3); }
    | expr AND expr                     { $$ = make_binop_node('&', $1, $3); }
    | expr OR expr                      { $$ = make_binop_node('|', $1, $3); }
    | NOT expr                          { $$ = make_unary_node("!", $2); }
    | IDENTIFIER INCR                   { $$ = make_unary_node("++", make_var_node($1)); }
    | IDENTIFIER DECR                   { $$ = make_unary_node("--", make_var_node($1)); }
    | NUMBER                            { $$ = make_int_node($1); }
    | STRING                            { $$ = make_string_node($1); }
    | IDENTIFIER                        { $$ = make_var_node($1); }
    | IDENTIFIER LPAREN RPAREN          { $$ = make_func_call_node($1, NULL); }
    | IDENTIFIER LPAREN expr_list RPAREN
                                        { $$ = make_func_call_node($1, $3); }
    | LPAREN expr RPAREN                { $$ = $2; }
    ;

expr_list:
      expr                              { $$ = make_expr_list_node($1, NULL); }
    | expr_list COMMA expr              { $$ = make_expr_list_node($3, $1); }
    ;

%%
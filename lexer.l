%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

extern int yylineno;
%}

%option yylineno

%%

[ \t\r]                 ; /* ignore whitespace and carriage returns */
\n                      ; /* ignore newlines */
"//".*\n               ; /* ignore single line comments */
"/*"([^*]|"*"+[^*/])*"*"+"/"  ; /* ignore multi-line comments */
^#.*\n                 ; /* ignore preprocessor directives */

"int"                   { printf("[LEX] Found int keyword\n"); return KW_INT; }
"if"                    { printf("[LEX] Found if keyword\n"); return KW_IF; }
"else"                  { printf("[LEX] Found else keyword\n"); return KW_ELSE; }
"for"                   { printf("[LEX] Found for keyword\n"); return KW_FOR; }
"return"                { printf("[LEX] Found return keyword\n"); return KW_RETURN; }
"printf"                { printf("[LEX] Found printf identifier\n"); return IDENTIFIER; }

"=="                    { return EQ; }
"!="                    { return NE; }
"<="                    { return LE; }
">="                    { return GE; }
"&&"                    { return AND; }
"||"                    { return OR; }
"!"                     { return NOT; }

"++"                    { return INCR; }
"--"                    { return DECR; }

"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MUL; }
"/"                     { return DIV; }
"<"                     { return LT; }
">"                     { return GT; }

"="                     { return ASSIGN; }
";"                     { return SEMICOLON; }
","                     { return COMMA; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
"{"                     { return LBRACE; }
"}"                     { return RBRACE; }

[0-9]+                  { printf("[LEX] Found number: %s\n", yytext); yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("[LEX] Found identifier: %s\n", yytext); yylval.str = strdup(yytext); return IDENTIFIER; }
\"([^\\\"]|\\.)*\"      { 
                          printf("[LEX] Found string: %s\n", yytext);
                          yylval.str = strdup(yytext);
                          // Remove quotes
                          yylval.str[strlen(yylval.str)-1] = '\0';
                          memmove(yylval.str, yylval.str+1, strlen(yylval.str));
                          return STRING; 
                        }

.                       { 
                          fprintf(stderr, "Lexical error: unexpected character '%s' (ASCII: %d) at line %d\n", 
                                 yytext, yytext[0], yylineno);
                          return yytext[0]; 
                        }

%%

int yywrap(void) {
    return 1;
}
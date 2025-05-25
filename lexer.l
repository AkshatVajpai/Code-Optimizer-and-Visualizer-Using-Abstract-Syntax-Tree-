%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

extern int yylineno;
%}

%option yylineno

%%

[ \t]                   ; /* ignore whitespace */
\n                      ; /* ignore newlines */
"//".*\n               ; /* ignore single line comments */
"/*"([^*]|"*"+[^*/])*"*"+"/"  ; /* ignore multi-line comments */

"int"                   { return KW_INT; }
"if"                    { return KW_IF; }
"else"                  { return KW_ELSE; }
"for"                   { return KW_FOR; }
"return"                { return KW_RETURN; }

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

[0-9]+                  { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = strdup(yytext); return IDENTIFIER; }
\"([^\\\"]|\\.)*\"      { 
                          yylval.str = strdup(yytext);
                          // Remove quotes
                          yylval.str[strlen(yylval.str)-1] = '\0';
                          memmove(yylval.str, yylval.str+1, strlen(yylval.str));
                          return STRING; 
                        }

.                       { 
                          fprintf(stderr, "Lexical error: unexpected character '%s' at line %d\n", 
                                 yytext, yylineno);
                          return yytext[0]; 
                        }

%%

int yywrap(void) {
    return 1;
}
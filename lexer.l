%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXTOKENLEN 40

extern FILE *yyin;
extern FILE *yyout;
extern char* yytext;

int lineNum = 1;
int countErrorsLexer;
char tokenString[MAXTOKENLEN];
%}

%option noyywrap

DIGITO    [0-9]
NUMBER    {DIGITO}+
LETRA     [a-zA-Z]
IDENT     {LETRA}({LETRA}|{DIGITO})*
NEWLINE   \n
WHITESPACE [ \t]+
COMMENT    "/*"(.|\n)*"*/"


%%

"else"            { return ELSE; }
"if"              { return IF; }
"int"             { return INT; }
"return"          { return RETURN; }
"void"            { return VOID; }
"while"           { return WHILE; }

"+"               { return PLUS; }
"-"               { return MINUS; }
"*"               { return MULT; }
"/"               { return DIV; }
"<"               { return SMAL; }
"<="              { return SMALEQ; }
">"               { return GREAT; }
">="              { return GREATEQ; }
"=="              { return EQ; }
"!="              { return DIFF; }
"="               { return ASSIGN; }
";"               { return SEMICOL; }
","               { return COMMA; }
"("               { return LPAREN; }
")"               { return RPAREN; }
"["               { return LBRACKET; }
"]"               { return RBRACKET; }
"{"               { return LBRACE; }
"}"               { return RBRACE; }

{COMMENT}         { /* Ignorar */ }
{NUMBER}          { yylval = atoi(yytext); return NUM; }
{IDENT}           { strncpy(tokenString, yytext, MAXTOKENLEN); return ID; }

{WHITESPACE}      { /* Ignorar */ }
{NEWLINE}         { lineNum++; }

.                 {
    fprintf(stderr, "ERRO LÉXICO: \"%s\" ", yytext);
    fprintf(stderr, "LINHA: %d\n", lineNum);
    return -1;
}

<<EOF>>           { return 0; }

%%

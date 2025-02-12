%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXTOKENLEN 40

extern FILE *yyin;
extern FILE *yyout;
extern char* yytext;

int lineNum;
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

"/*" {
    int c, next_char;
    while ((c = input()) != EOF) {
        if (c == '\n') {
            lineNum++;
        } else if (c == '*') {
            next_char = input(); // Lê o próximo caractere após '*'
            if (next_char == '/') {
                break; // Fecha o comentário
            } else {
                unput(next_char); // Devolve o caractere lido
            }
        }
    }
    if (c == EOF) {
        fprintf(stderr, "ERRO LÉXICO: Comentário não fechado");
        fprintf(stderr, " LINHA: %d\n", lineNum);
        countErrorsLexer++;
    }
}

{NUMBER}          { yylval = atoi(yytext); return NUM; }
{IDENT}           { strncpy(tokenString, yytext, MAXTOKENLEN); return ID; }

{WHITESPACE}      { /* Ignorar */ }
{NEWLINE}         { lineNum++; }

.                 {
    fprintf(stderr, "ERRO LÉXICO: \"%s\"", yytext);
    fprintf(stderr, " LINHA: %d\n", lineNum);
    countErrorsLexer++;
}

<<EOF>>           { return 0; }

%%

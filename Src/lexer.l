%{
#include "parser.tab.h"  // Deve vir primeiro
#include "../Include/globals.h"


extern YYSTYPE yylval;
extern char* yytext;

int lineNum = 1;
int countErrorsLexer = 1;

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
        fprintf(stderr, ANSI_COLOR_PURPLE "ERRO LÉXICO: Comentário não fechado" ANSI_COLOR_RESET);
        fprintf(stderr, ANSI_COLOR_RED " LINHA: %d\n" ANSI_COLOR_RESET, lineNum);
        countErrorsLexer++;
    }
}



{NUMBER}          { yylval = atoi(yytext); return NUM; }
{IDENT}           { strncpy(tokenString, yytext, MAXTOKENLEN); return ID; }

{WHITESPACE}      { /* Ignorar */ }
{NEWLINE}         { lineNum++; }

.                 {
    fprintf(stderr, ANSI_COLOR_PURPLE "ERRO LÉXICO: " ANSI_COLOR_RESET "\"%s\"", yytext);
    fprintf(stderr, ANSI_COLOR_RED " LINHA: %d\n" ANSI_COLOR_RESET, lineNum);
    countErrorsLexer++;
}

<<EOF>>           { return 0; }

%%

// Função auxiliar mantida se necessário
TokenType getToken(void) {
    return yylex();
}
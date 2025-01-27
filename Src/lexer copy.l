%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// extern YYSTYPE yylval;

int lineNum = 1; // Definição da variável lineNum
int countErrorsLexer = 1;

%}
%option noyywrap

/* Definições regulares */
DIGITO [0-9]
NUMBER {DIGITO}+

LETRA [a-zA-Z]
IDENT {LETRA}({LETRA}|{DIGITO})*

NEWLINE \n

/* Espaços em branco. O espaço em branco é composto por branco, mudanças de linha e tabulações.
É ignorado, exceto quando é usado como separador de ID, NUMs e palavras-chave. */
WHITESPACE [ ]+
TAB \t

%%

"else"            {printf("Token: ELSE\n");}
"if"              {printf("Token: IF\n");}
"int"             {printf("Token: INT\n");}
"return"          {printf("Token: RETURN\n");}
"void"            {printf("Token: VOID\n");}
"while"           {printf("Token: WHILE\n");}

\+               {printf("Token: PLUS\n");}
\-               {printf("Token: MINUS\n");}
\*               {printf("Token: MULT\n");}
\/               {printf("Token: DIV\n");}
\<               {printf("Token: SMAL\n");}
\<=              {printf("Token: SMALEQ\n");}
\>               {printf("Token: GREAT\n");}
\>\=             {printf("Token: GREATEQ\n");}
\=\=             {printf("Token: EQ\n");}
\!\=             {printf("Token: DIFF\n");}
\=               {printf("Token: ASSIGN\n");}
;                {printf("Token: SEMICOL\n");}
\,               {printf("Token: COMMA\n");}
\(               {printf("Token: LPAREN\n");}
\)               {printf("Token: RPAREN\n");}
\[               {printf("Token: LBRACKET\n");}
\]               {printf("Token: RBRACKET\n");}
\{               {printf("Token: LBRACE\n");}
\}               {printf("Token: RBRACE\n");}

{NUMBER}         {printf("Token: NUMBER(%s)\n", yytext);}
{IDENT}          {printf("Token: ID(%s)\n", yytext);}

{WHITESPACE}     { /* Ignorar espaços em branco */ }
{TAB}           { /* Ignorar tabulações */ }
{NEWLINE}        {lineNum++;}

.               {printf("ERRO LÉXICO: '%s' na linha %d\n", yytext, lineNum);}

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            printf("Erro ao abrir arquivo %s\n", argv[1]);
            return 1;
        }
    }
    
    printf("Iniciando análise léxica...\n");
    yylex();
    printf("\nTotal de linhas analisadas: %d\n", lineNum);
    
    return 0;
}
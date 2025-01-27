%{
#include "../../Include/globals.h"
#include "parser.tab.h"  // Incluir o arquivo de cabeçalho gerado pelo Bison

extern YYSTYPE yylval;

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

"else"            {return ELSE;}
"if"              {return IF;}
"int"             {return INT;}
"return"          {return RETURN;}
"void"            {return VOID;}
"while"           {return WHILE;}

\+               {return PLUS;}
\-               {return MINUS;}
\*               {return MULT;}
\/               {return DIV;}
\<               {return SMAL;}
\<=              {return SMALEQ;}
\>               {return GREAT;}
\>\=              {return GREATEQ;}
\=\=              {return EQ;}
\!\=              {return DIFF;}
\=               {return ASSIGN;}
;               {return SEMICOL;}
\,               {return COMMA;}
\(               {return LPAREN;}
\)               {return RPAREN;}
\[               {return LBRACKET;}
\]               {return RBRACKET;}
\{               {return LBRACE;}
\}               {return RBRACE;}

\/\*              {
                        while (1) {
                            int c = input();
                            if (c == EOF) {
                                fprintf(yyout, "ERROR%d", countErrorsLexer);
                                fprintf(yyout, "ERRO LÉXICO: \"%s\" ", yytext);
                                fprintf(yyout, "LINHA: %d.\n", lineNum);
                                countErrorsLexer++;
                                
                                break;
                            } else if(c == '\n') {
                                fprintf(yyout, "\n");
                                lineNum++;
                            } else if (c == '*') {
                                int next = input();
                                if (next == '/') {
                                    break;
                                } else {
                                    unput(next);
                                }
                            }
                        }
                    }
                    
\*\/          {fprintf(yyout, "ERROR%d", countErrorsLexer);
                printf("ERRO LÉXICO: \"%s\" ", yytext);
                printf("LINHA: %d.\n", lineNum);
                countErrorsLexer++;}

{NUMBER}            {yylval = atoi(yytext); return NUM;}
{IDENT}             {strncpy(tokenString, yytext, MAXTOKENLEN); return ID;}

{WHITESPACE}         { /* Ignorar espaços em branco */ }
{TAB}               { /* Ignorar tabulações */ }
{NEWLINE}            {lineNum++;}

.                 {fprintf(yyout, "ERROR%d", countErrorsLexer);
                    printf(ANSI_COLOR_PURPLE "ERRO LÉXICO: " ANSI_COLOR_RESET "\"%s\" ", yytext);
                    printf(ANSI_COLOR_RED "LINHA: %d.\n" ANSI_COLOR_RESET, lineNum);
                    countErrorsLexer++;}
<<EOF>>           {return ENDOFFILE;}

%%

/* Rotina auxiliar */

TokenType getToken(void){

    static int firstTime = TRUE;

    if (firstTime){
        firstTime = FALSE;
        lineNum++;
        yyin = arquivoEntrada;
        yyout = arquivoSaida;

    }

    TokenType currentToken = yylex();

    if (currentToken == ID){
        strncpy(tokenString, yytext, MAXTOKENLEN);
    }

    return currentToken;    
}

/* Função que verifica se o usuário deseja compilar um arquivo ou escrever o código diretamente no terminal */
/* Deve-se utilizar parâmetros na linha de comandos para selecionar.
    Para a entrada 1, deve-se dar de entrada o programa pelo terminal e a saída será no terminal.
    Para a entrada 2, a entrada será passada como parâmetro e a saída será no terminal.
    Para a entrada 3, a entrada e saída serão passadas como parâmetro.
*/
void formaEntrada(int argc, char **argv){
    if (argc == 1){
        yyin = stdin;
        yyout = stdout;
    } else if (argc == 2){
        yyin = fopen(argv[1], "r");
        yyout = stdout;
    } else if (argc == 3){
        yyin = fopen(argv[1], "r");
        yyout = fopen(argv[2], "w");
    } else {
        fprintf(stderr, "Uso: %s [arquivo_entrada] [arquivo_saida]\n", argv[0]);
        exit(1);
    }
}
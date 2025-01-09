/*
Projeto de Compiladores - Analisador Léxico
    Discentes: Fernando Daniel Marcelino e Marcos Vinicius Gasparoto Bauab
    Docente: Dr. Luiz Eduardo Galvão Martins

Atividade da disciplina de Compiladores, como parte avaliativa para o projeto de um compilador para a linguagem C-.
*/

%{

#include "../../Include/globals.h"

int lineNum = 1; // Definição da variável lineNum
int countErrors = 1;

char tokenString[MAXTOKENLEN+1]; // Definição do lexema do identificador ou palavra reservada

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

"else"            {fprintf(yyout, "ELSE");}
"if"              {fprintf(yyout, "IF");}
"int"             {fprintf(yyout, "INT");}
"return"          {fprintf(yyout, "RETURN");}
"void"            {fprintf(yyout, "VOID");}
"while"           {fprintf(yyout, "WHILE");}

\+               {fprintf(yyout, "PLUS");}
\-               {fprintf(yyout, "MINUS");}
\*               {fprintf(yyout, "MULT");}
\/               {fprintf(yyout, "DIV");}
\<               {fprintf(yyout, "SMAL");}
\<=              {fprintf(yyout, "SMALEQ");}
\>               {fprintf(yyout, "GREAT");}
\>\=              {fprintf(yyout, "GREATEQ");}
\=\=              {fprintf(yyout, "EQ");}
\!\=              {fprintf(yyout, "DIFF");}
\=               {fprintf(yyout, "ASSIGN");}
;               {fprintf(yyout, "SEMICOL");}
\,               {fprintf(yyout, "COMMA");}
\(               {fprintf(yyout, "LPAREN");}
\)               {fprintf(yyout, "RPAREN");}
\[               {fprintf(yyout, "LBRACKET");}
\]               {fprintf(yyout, "RBRACKET");}
\{               {fprintf(yyout, "LBRACE");}
\}               {fprintf(yyout, "RBRACE");}
\/\*              {
                        while (1) {
                            int c = input();
                            if (c == EOF) {
                                fprintf(yyout, "ERROR%d", countErrors);
                                fprintf(yyout, "ERRO LÉXICO: \"%s\" ", yytext);
                                fprintf(yyout, "LINHA: %d.\n", lineNum);
                                countErrors++;
                                
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
\*\/          {fprintf(yyout, "ERROR%d", countErrors);
                printf("ERRO LÉXICO: \"%s\" ", yytext);
                printf("LINHA: %d.\n", lineNum);
                countErrors++;}


{NUMBER}            {fprintf(yyout, "NUM");}
{IDENT}             {fprintf(yyout, "ID");}

{WHITESPACE}         {fprintf(yyout, " ");}
{TAB}               {fprintf(yyout, "\t");}
{NEWLINE}            {lineNum++; fprintf(yyout, "\n");}

.                 {fprintf(yyout, "ERROR%d", countErrors);
                    printf(ANSI_COLOR_PURPLE "ERRO LÉXICO: " ANSI_COLOR_RESET "\"%s\" ", yytext);
                    printf(ANSI_COLOR_RED "LINHA: %d.\n" ANSI_COLOR_RESET, lineNum);
                    countErrors++;}
<<EOF>>           {fprintf(yyout, "ENDOFFILE"); return 0;}

%%

/* Rotina auxiliar */

int main(int argc, char *argv[]) {
    FILE *inputFile;
    FILE *outputFile;

    if (argc == 3) {
        inputFile = fopen(argv[1], "r");
        outputFile = fopen(argv[2], "w");

        // Abrir o arquivo de entrada
        if (inputFile) {
            yyin = inputFile;
        } else {
            perror(argv[0]);
            return EXIT_FAILURE;
        }

        // Abrir o arquivo de saída
        if (outputFile) {
            yyout = outputFile;
        } else {
            perror("Erro ao abrir arquivo de saída");
            fclose(inputFile); // Fecha o arquivo de entrada antes de sair
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return EXIT_FAILURE;
    }

    yylex(); // Inicia o analisador léxico

    fclose(inputFile); // Fecha o arquivo de entrada
    fclose(outputFile); // Fecha o arquivo de saída

    return EXIT_SUCCESS;
}
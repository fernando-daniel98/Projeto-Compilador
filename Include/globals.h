#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define MAXTOKENLEN 40
#define MAXIDLEN 33
#define MAXSTACK 30

extern char tokenString[MAXTOKENLEN];

// Cores ANSI
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GRAY     "\x1b[0;37m"
#define ANSI_COLOR_WHITE    "\x1b[1;37m"
#define ANSI_COLOR_RESET    "\x1b[0m"
#define ANSI_COLOR_PURPLE   "\x1b[0;35m"
#define ANSI_COLOR_GREEN    "\x1b[0;32m"

// Variáveis globais
extern int lineNum;
extern int countErrorsLexer;
extern int countErrorsParser;
extern int countErrorsSemantic;
extern FILE *arquivoEntrada;
extern FILE *arquivoSaida;
extern char tokenString[MAXTOKENLEN];

// Tipos
typedef int TokenType;  // Usará tokens gerados pelo Bison

// Protótipos de funções
void formaEntrada(int argc, char **argv);

#endif
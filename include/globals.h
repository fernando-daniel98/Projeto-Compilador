#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>

// Constantes globais
#define MAXCHILDREN 3
#define MAXNOLIN 40
#define MAXTOKENLEN 40

// Coloracoes em ANSI
#define ANSI_COLOR_RESET    "\e[0m"
#define ANSI_COLOR_RED      "\e[0;31m" // Erro léxico
#define ANSI_COLOR_YELLOW   "\e[0;33m" // Erro sintático
#define ANSI_COLOR_PURPLE   "\e[0;35m" // Erro semântico
#define ANSI_COLOR_WHITE    "\e[1;37m" // Num da linha
#define ANSI_COLOR_GREEN    "\e[1;32m" // Recuperação de erro

// Variáveis externas do compilador
extern FILE *yyin;
extern FILE *yyout;
extern int lexical_errors;
extern int syntax_errors;
extern int semantic_errors;

// ExpType é usado para verificação de tipos
typedef enum {Void, Integer} ExpType;

#endif
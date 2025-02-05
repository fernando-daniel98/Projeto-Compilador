#include <stdio.h>
#include <stdlib.h>  // Para exit()
#include "../Include/globals.h"

extern FILE *yyin, *yyout;
char tokenString[MAXTOKENLEN];  // Add this line to define tokenString

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
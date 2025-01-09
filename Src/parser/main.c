#include "../../Include/globals.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return EXIT_FAILURE;
    }

    arquivoEntrada = fopen(argv[1], "r");
    if (!arquivoEntrada) {
        perror("Erro ao abrir arquivo de entrada");
        return EXIT_FAILURE;
    }

    arquivoSaida = fopen(argv[2], "w");
    if (!arquivoSaida) {
        perror("Erro ao abrir arquivo de saída");
        fclose(arquivoEntrada);
        return EXIT_FAILURE;
    }

    yyin = arquivoEntrada;
    yyout = arquivoSaida;

    // Iniciar o parser
    yyparse();

    fclose(arquivoEntrada);
    fclose(arquivoSaida);

    return EXIT_SUCCESS;
}
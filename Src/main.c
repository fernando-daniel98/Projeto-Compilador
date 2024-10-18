#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {

    printf("Teste de leitura: ");
    char *nome = malloc(100*sizeof(char));

    if (fgets(nome, 100, stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        free(nome);
        return 1;
    }

    // Remover o \n do final da string
    nome[strcspn(nome, "\n")] = 0;

    time_t tempo = time(NULL);
    printf("Hello, %s! Data e hora: %s", nome, ctime(&tempo));

    free(nome);

    return 0;
}
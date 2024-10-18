#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 256

int main() {

    char *buffer;

    buffer = (char *) malloc(BUFFER_SIZE * sizeof(char));

    FILE *fd;

    fd = fopen("arqTeste.c", "r");

    if (fd == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    printf("Exibindo tudo o que está no arquivo:\n");

    while (fgets(buffer, BUFFER_SIZE, fd) != NULL) {
        printf("%s", buffer);
    }

    fclose(fd);
    free(buffer);

    return 0;
}
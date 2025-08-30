#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label.h"

// Vetor de labels
VETOR_LABEL * vetorLabel = NULL;

// Inicializa o vetor de labels
void inicializaLabels(){
    vetorLabel = (VETOR_LABEL *)malloc(sizeof(VETOR_LABEL));
    vetorLabel->tamanho = -1;
    vetorLabel->vetor = NULL;
}

// Cria um novo no de label
LABEL * criarNoLabel(char* id, int endereco){
    LABEL * novoNoLabel = (LABEL *)malloc(sizeof(LABEL));
    novoNoLabel->id = strdup(id);
    novoNoLabel->endereco = endereco;
    novoNoLabel->prox = NULL;
    return novoNoLabel;
}

// Adiciona uma label ao vetor de labels
void adicionarLabel(char* id, int endereco){
    LABEL * novoNoLabel = criarNoLabel(id, endereco);
    if(vetorLabel->tamanho == -1){
        vetorLabel->vetor = novoNoLabel;
        vetorLabel->tamanho = 0;
        return;
    }

    LABEL * aux = vetorLabel->vetor;
    while(aux->prox != NULL){
        aux = aux->prox;
    }
    aux->prox = novoNoLabel;
    vetorLabel->tamanho++;
}

// Retorna o endereco de uma label
int getEnderecoLabel(char* id){
    LABEL * aux = vetorLabel->vetor;
    while(aux != NULL){
        if(!strcmp(aux->id, id)){
            return aux->endereco;
        }
        aux = aux->prox;
    }
    return -1;
}

// Libera o vetor de labels
void liberarLabels(){
    LABEL * aux = vetorLabel->vetor;
    while(aux != NULL){
        LABEL * aux2 = aux->prox;
        free(aux);
        aux = aux2;
    }
    free(vetorLabel);
}

// Imprime o vetor de labels
void imprimirLabels(){
    LABEL * aux = vetorLabel->vetor;

    printf("\n============= Vetor de labels =============\n");
    while(aux != NULL){
        printf("%s, Endereco: %d\n", aux->id, aux->endereco);
        aux = aux->prox;
    }
}
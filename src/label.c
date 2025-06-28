/* Arquivo para armazenar as funcoes para a estrutura
de dados que armazena as labels e seus enederecos de memoria */

// TODO: Modificar o vetor de labels para armazenar um char ao inves de int

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/globals.h"
#include "../include/label.h"

// Vetor de labels
VETOR_LABEL *vetorLabel = NULL;

// Inicializa o vetor de labels
void inicializaLabels(){
    vetorLabel = (VETOR_LABEL *)malloc(sizeof(VETOR_LABEL));
    if(vetorLabel == NULL){
        fprintf(stderr, "Erro ao alocar memoria para o vetor de labels\n");
        exit(1);
    }
    vetorLabel->vetor = NULL;
    vetorLabel->tamanho = 0;
}

// Cria um novo no de label
Label * criarNoLabel(char* id, int endereco){
    Label * novoNoLabel = (Label *)malloc(sizeof(Label));
    novoNoLabel->id = strdup(id);
    novoNoLabel->endereco = endereco;
    novoNoLabel->prox = NULL;
    return novoNoLabel;
}

// Adiciona uma label ao vetor de labels
void adicionarLabel(char* id, int endereco){
    if(vetorLabel == NULL){
        inicializaLabels();
    }
    Label * novoNoLabel = criarNoLabel(id, endereco);
    if(vetorLabel->vetor == NULL){
        vetorLabel->vetor = novoNoLabel;
        vetorLabel->tamanho++;
        return;
    }
    Label * aux = vetorLabel->vetor;
    while(aux->prox != NULL){
        aux = aux->prox;
    }
    aux->prox = novoNoLabel;
    vetorLabel->tamanho++;
}

// Retorna o endereco de uma label
int getEnderecoLabel(char* id){
    if(vetorLabel == NULL || vetorLabel->vetor == NULL) return -1;

    Label * aux = vetorLabel->vetor;
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
    if(vetorLabel == NULL) return;

    Label * aux = vetorLabel->vetor;
    Label * prox;
    while(aux != NULL){
        Label * aux2 = aux->prox;
        free(aux->id);
        free(aux);
        aux = aux2;
    }
    free(vetorLabel);
    vetorLabel = NULL;
}

// Imprime o vetor de labels
void imprimirLabels(){
    if(vetorLabel == NULL || vetorLabel->vetor == NULL){
        printf("Nao ha labels para imprimir\n");
        return;
    }
    printf("\n");
    Label * aux = vetorLabel->vetor;
    printf("Labels:\n");
    while(aux != NULL){
        printf("%s, Endereco: %d\n", aux->id, aux->endereco);
        aux = aux->prox;
    }
}
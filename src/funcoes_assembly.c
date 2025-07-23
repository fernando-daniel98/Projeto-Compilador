#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/assembler.h"
#include "../include/globals.h" // Adicionado para yyout
#include "../include/memoria.h" // Adicionado para inicializa_memoria, vetorMemoria, funcaoAtual

ASSEMBLY ** instrucoesAssembly = NULL;
int indiceAssembly = 0;

void inicializaAssembly(){
    instrucoesAssembly = (ASSEMBLY **) malloc(sizeof(ASSEMBLY *) * MAX_ASSEMBLY);
    if(instrucoesAssembly == NULL){
        printf("Erro ao alocar memoria para o vetor de instrucoes assembly\n");
        exit(1);
    }
    for(int i = 0; i < MAX_ASSEMBLY; i++){
        instrucoesAssembly[i] = NULL;
    }

    indiceAssembly = 0;

    inicializaLabels();
    inicializa_memoria(&vetorMemoria);
    funcaoAtual = vetorMemoria.funcoes;
}

ASSEMBLY * criarNoAssembly(tipoInstrucao tipo, char *nome){
    ASSEMBLY * novoNoAssembly = (ASSEMBLY *)malloc(sizeof(ASSEMBLY));
    novoNoAssembly->tipo = tipo;

    switch (tipo){
    case typeR:
        novoNoAssembly->tipoR = (TIPO_R *)malloc(sizeof(TIPO_R));
        novoNoAssembly->tipoR->nome = nome;
        novoNoAssembly->tipoR->rd = -1;
        novoNoAssembly->tipoR->rs = -1;
        novoNoAssembly->tipoR->rt = -1;
        break;

    case typeI:
        novoNoAssembly->tipoI = (TIPO_I *)malloc(sizeof(TIPO_I));
        novoNoAssembly->tipoI->nome = nome;
        novoNoAssembly->tipoI->rs = -1;
        novoNoAssembly->tipoI->rt = -1;
        novoNoAssembly->tipoI->imediato = -1;
        novoNoAssembly->tipoI->label = -1;
        break;

    case typeJ:
        novoNoAssembly->tipoJ = (TIPO_J *)malloc(sizeof(TIPO_J));
        novoNoAssembly->tipoJ->nome = nome;
        novoNoAssembly->tipoJ->labelImediato = NULL;
        break;
    
    case typeLabel:
        novoNoAssembly->tipoLabel = (TIPO_LABEL *)malloc(sizeof(TIPO_LABEL));
        novoNoAssembly->tipoLabel->nome = nome;
        novoNoAssembly->tipoLabel->endereco = -1;
        novoNoAssembly->tipoLabel->boolean = -1;
        break;
    }

    return novoNoAssembly;
}

// Liberar vetor de instrucoes assembly
void liberarAssembly(){
    for(int i = 0; i < indiceAssembly; i++){
        if(instrucoesAssembly[i]->tipo == typeR)
            free(instrucoesAssembly[i]->tipoR);
        else if(instrucoesAssembly[i]->tipo == typeI)
            free(instrucoesAssembly[i]->tipoI);
        else if(instrucoesAssembly[i]->tipo == typeJ)
            free(instrucoesAssembly[i]->tipoJ);
        else if(instrucoesAssembly[i]->tipo == typeLabel){
            free(instrucoesAssembly[i]->tipoLabel);
            if(instrucoesAssembly[i]->tipoLabel->boolean == 1)
                free(instrucoesAssembly[i]->tipoLabel->nome);
        }
        free(instrucoesAssembly[i]);
    }
    free(instrucoesAssembly);
}

void tipo_reg(int reg){
    switch(reg){
    case $zero:
        fprintf(yyout, "$zero");
        break;
    case $v0:
        fprintf(yyout, "$v0");
        break;
    case $a0:
        fprintf(yyout, "$a0");
        break;
    case $s0:
        fprintf(yyout, "$s0");
        break;
    case $fp:
        fprintf(yyout, "$fp");
        break;
    case $sp:
        fprintf(yyout, "$sp");
        break;
    case $ra:
        fprintf(yyout, "$ra");
        break;
    case $temp:
        fprintf(yyout, "$t8");
        break;
    case $pilha:
        fprintf(yyout, "$k0");
        break;
    default:
        if (reg >= 8 && reg <= 15) {
            fprintf(yyout, "$t%d", reg - 8);
        } else if (reg == 25) {
            fprintf(yyout, "$t9");
        }
        else {
            fprintf(yyout, "$%d", reg);
        }
        break;
    }
}

void imprimirAssembly(){
    if(instrucoesAssembly == NULL){
        printf("Nao ha instrucoes assembly para imprimir\n");
        return;
    }
    fprintf(yyout, "============== Assembly ==============\n");
    for(int i = 0; i < indiceAssembly; i++){
        if(instrucoesAssembly[i] != NULL){
            if(instrucoesAssembly[i]->tipo == typeR){
                fprintf(yyout, "%s ", instrucoesAssembly[i]->tipoR->nome);
                tipo_reg(instrucoesAssembly[i]->tipoR->rd);
                fprintf(yyout, ", ");
                tipo_reg(instrucoesAssembly[i]->tipoR->rs);
                fprintf(yyout, ", ");
                tipo_reg(instrucoesAssembly[i]->tipoR->rt);
                fprintf(yyout, "\n");
            }
            else if(instrucoesAssembly[i]->tipo == typeI){
                if (strcmp(instrucoesAssembly[i]->tipoI->nome, "beq") == 0 || strcmp(instrucoesAssembly[i]->tipoI->nome, "bne") == 0) {
                    fprintf(yyout, "%s ", instrucoesAssembly[i]->tipoI->nome);
                    tipo_reg(instrucoesAssembly[i]->tipoI->rs);
                    fprintf(yyout, ", ");
                    tipo_reg(instrucoesAssembly[i]->tipoI->rt);
                    fprintf(yyout, ", Label%d\n", instrucoesAssembly[i]->tipoI->label);
                } else if (strcmp(instrucoesAssembly[i]->tipoI->nome, "lw") == 0 || strcmp(instrucoesAssembly[i]->tipoI->nome, "sw") == 0) {
                    fprintf(yyout, "%s ", instrucoesAssembly[i]->tipoI->nome);
                    tipo_reg(instrucoesAssembly[i]->tipoI->rt);
                    fprintf(yyout, ", %d(", instrucoesAssembly[i]->tipoI->imediato);
                    tipo_reg(instrucoesAssembly[i]->tipoI->rs);
                    fprintf(yyout, ")\n");
                } else {
                    fprintf(yyout, "%s ", instrucoesAssembly[i]->tipoI->nome);
                    tipo_reg(instrucoesAssembly[i]->tipoI->rt);
                    fprintf(yyout, ", ");
                    tipo_reg(instrucoesAssembly[i]->tipoI->rs);
                    fprintf(yyout, ", %d\n", instrucoesAssembly[i]->tipoI->imediato);
                }
            }
            else if(instrucoesAssembly[i]->tipo == typeJ){
                if (strcmp(instrucoesAssembly[i]->tipoJ->nome, "syscall") == 0 || strcmp(instrucoesAssembly[i]->tipoJ->nome, "halt") == 0) {
                    fprintf(yyout, "%s\n", instrucoesAssembly[i]->tipoJ->nome);
                } else {
                    fprintf(yyout, "%s %s\n", instrucoesAssembly[i]->tipoJ->nome, instrucoesAssembly[i]->tipoJ->labelImediato);
                }
            }
            else if(instrucoesAssembly[i]->tipo == typeLabel){
                fprintf(yyout, "%s:\n", instrucoesAssembly[i]->tipoLabel->nome);
            }
        }
    }
}

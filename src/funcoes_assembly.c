#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/assembler.h"
#include "../include/globals.h" // Adicionado para yyout
#include "../include/memoria.h" // Adicionado para inicializa_memoria, vetorMemoria, funcaoAtual

ASSEMBLY ** instrucoesAssembly = NULL;
int indiceAssembly = 0;

void inicializaAssembly(){
    instrucoesAssembly = (ASSEMBLY **) malloc(sizeof(ASSEMBLY *) * MAX_ASSEMBLY); // Corrigido para MAX_ASSEMBLY
    if(instrucoesAssembly == NULL){
        printf("Erro ao alocar memoria para o vetor de instrucoes assembly\n");
        exit(1);
    }
    for(int i = 0; i < MAX_ASSEMBLY; i++){ // Corrigido para MAX_ASSEMBLY
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
        fprintf(yyout, "$zero"); // Corrigido para yyout
        break;
    // Adicionando os outros registradores que estavam faltando no switch original,
    // mas que são definidos e podem ser usados.
    case $v0:
        fprintf(yyout, "$v0"); // Corrigido para yyout
        break;
    case $a0:
        fprintf(yyout, "$a0"); // Corrigido para yyout
        break;
    case $s0:
        fprintf(yyout, "$s0"); // Corrigido para yyout
        break;
    case $fp:
        fprintf(yyout, "$fp"); // Corrigido para yyout
        break;
    case $sp:
        fprintf(yyout, "$sp"); // Corrigido para yyout
        break;
    case $ra:
        fprintf(yyout, "$ra"); // Corrigido para yyout
        break;
    case $temp: // Assumindo que $temp (24) é o que se quer imprimir como $t8
        fprintf(yyout, "$t8"); // Corrigido para yyout
        break;
    case $pilha: // Assumindo que $pilha (26) é o que se quer imprimir como $k0
        fprintf(yyout, "$k0"); // Corrigido para yyout
        break;
    // Casos para $t0-$t7 (8-15), $s0-$s7 (16-23), $t8-$t9 (24-25)
    // O código original em assembler.c usa valores diretos para registradores
    // que são passados como instrucao->operX->val.
    // A função tipo_reg precisa cobrir todos os registradores usados.
    // Por simplicidade, vamos adicionar os que estavam explicitamente nos erros.
    // Se outros registradores (e.g. $t1, $t2) forem usados e precisarem de nomes
    // específicos, eles devem ser adicionados aqui.
    // Para registradores temporários genéricos $t0-$t7, $t8, $t9
    default:
        if (reg >= 8 && reg <= 15) { // $t0 - $t7
            fprintf(yyout, "$t%d", reg - 8); // Corrigido para yyout
        } else if (reg == 25) { // $t9 (usado como temp_reg_neq)
            fprintf(yyout, "$t9"); // Corrigido para yyout
        }
        // Adicionar mais casos conforme necessário ou um default genérico
        else {
            fprintf(yyout, "$%d", reg); // Imprime como número se não mapeado
        }
        break;
    }
}

// Mostrar as instrucoes em assembly
void imprimirAssembly(){
    if(instrucoesAssembly == NULL){
        printf("Nao ha instrucoes assembly para imprimir\n");
        return;
    }
    fprintf(yyout, "============== Assembly ==============\n"); // Corrigido para yyout
    for(int i = 0; i < indiceAssembly; i++){
        if(instrucoesAssembly[i] != NULL){
            if(instrucoesAssembly[i]->tipo == typeR){
                fprintf(yyout, "%s ", instrucoesAssembly[i]->tipoR->nome); // Corrigido para yyout
                tipo_reg(instrucoesAssembly[i]->tipoR->rd);
                fprintf(yyout, ", "); // Corrigido para yyout
                tipo_reg(instrucoesAssembly[i]->tipoR->rs);
                fprintf(yyout, ", "); // Corrigido para yyout
                tipo_reg(instrucoesAssembly[i]->tipoR->rt);
                fprintf(yyout, "\n"); // Corrigido para yyout
            }
            else if(instrucoesAssembly[i]->tipo == typeI){
                // Para beq/bne, o formato é: beq rs, rt, Label
                // Para lw/sw/addi/ori, o formato é: op rt, rs, imediato ou op rt, imediato(rs)
                if (strcmp(instrucoesAssembly[i]->tipoI->nome, "beq") == 0 || strcmp(instrucoesAssembly[i]->tipoI->nome, "bne") == 0) {
                    fprintf(yyout, "%s ", instrucoesAssembly[i]->tipoI->nome); // Corrigido para yyout
                    tipo_reg(instrucoesAssembly[i]->tipoI->rs); // rs
                    fprintf(yyout, ", "); // Corrigido para yyout
                    tipo_reg(instrucoesAssembly[i]->tipoI->rt); // rt
                    fprintf(yyout, ", Label%d\n", instrucoesAssembly[i]->tipoI->label); // Corrigido para yyout
                } else if (strcmp(instrucoesAssembly[i]->tipoI->nome, "lw") == 0 || strcmp(instrucoesAssembly[i]->tipoI->nome, "sw") == 0) {
                    fprintf(yyout, "%s ", instrucoesAssembly[i]->tipoI->nome); // Corrigido para yyout
                    tipo_reg(instrucoesAssembly[i]->tipoI->rt); // rt (destino para lw, fonte para sw)
                    fprintf(yyout, ", %d(", instrucoesAssembly[i]->tipoI->imediato); // Corrigido para yyout
                    tipo_reg(instrucoesAssembly[i]->tipoI->rs); // rs (base)
                    fprintf(yyout, ")\n"); // Corrigido para yyout
                } else { // addi, subi, ori, xori, etc.
                    fprintf(yyout, "%s ", instrucoesAssembly[i]->tipoI->nome); // Corrigido para yyout
                    tipo_reg(instrucoesAssembly[i]->tipoI->rt); // rt (destino)
                    fprintf(yyout, ", "); // Corrigido para yyout
                    tipo_reg(instrucoesAssembly[i]->tipoI->rs); // rs (fonte)
                    fprintf(yyout, ", %d\n", instrucoesAssembly[i]->tipoI->imediato); // Corrigido para yyout
                }
            }
            else if(instrucoesAssembly[i]->tipo == typeJ){
                if (strcmp(instrucoesAssembly[i]->tipoJ->nome, "syscall") == 0 || strcmp(instrucoesAssembly[i]->tipoJ->nome, "halt") == 0) {
                    fprintf(yyout, "%s\n", instrucoesAssembly[i]->tipoJ->nome); // Corrigido para yyout
                } else {
                    fprintf(yyout, "%s %s\n", instrucoesAssembly[i]->tipoJ->nome, instrucoesAssembly[i]->tipoJ->labelImediato); // Corrigido para yyout
                }
            }
            else if(instrucoesAssembly[i]->tipo == typeLabel){
                fprintf(yyout, "%s:\n", instrucoesAssembly[i]->tipoLabel->nome); // Corrigido para yyout
            }
        }
    }
}

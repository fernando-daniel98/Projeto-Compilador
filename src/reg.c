#include <stdio.h>
#include <string.h>
#include <math.h>
// CORREÇÃO: Ajustar caminhos dos includes
#include "../include/globals.h"
#include "../include/codeGen.h"
#include "../include/reg.h"

// IMPORTANTE: Usar valores do reg.h (COMPATÍVEL COM EDUARDO)
// #define MAX_REG 23 // Já definido em reg.h - Registradores t0-t22 IGUAL AO EDUARDO
// #define MAX_REG_DESCARTE 10000000 // Já definido em reg.h

// CORREÇÃO: Usar MAXTOKENLEN
char stringTemp[MAXTOKENLEN] = "Temporario"; // Nome para identificar variaveis temporarias

/* Essa variavel sera importante para o discarte dos registradores, ja
que registradores usados pela ultima vez a algum tempo serao eliminados primeiro
do que os usados recentemente, para tentar evitar conflitos em instrucoes */
int totalReg = 1;

int totalRegEmUso = 0; 

// Estrutura REG já definida em reg.h

REG listaReg[MAX_REG]; // Lista encadeada com os registradores

// Funcao para inicializar o vetor de registradores
void inicializaReg(){
    for(int i = 0; i < MAX_REG; i++){
        listaReg[i].numReg = i;
        listaReg[i].nomeVar = NULL;
        strcpy(listaReg[i].escopo, "");
        listaReg[i].descarte = 0;
        listaReg[i].ultimoUso = 0;  // Inicializar campo ultimoUso
    }
}

// Adiciona uma variavel em um registrador
int adicionarVarReg(char* nomeVar, char* escopo){
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar == NULL){
            listaReg[i].nomeVar = nomeVar;
            strcpy(listaReg[i].escopo, escopo);
            listaReg[i].descarte = 0;
            listaReg[i].ultimoUso = totalReg;  // Atualizar último uso
            totalReg++;
            totalRegEmUso++;
            return i;
        }
    }
    return -1; // Nao foi possivel adicionar a variavel em nenhum registrador
}

// Adiciona uma variavel temporaria em um registrador, normalmente utilizada em operacoes
int adicionaTempReg(){
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar == NULL){
            listaReg[i].nomeVar = stringTemp;
            strcpy(listaReg[i].escopo, funcName); // funcName é global do codeGen.h
            listaReg[i].descarte = totalReg;
            listaReg[i].ultimoUso = totalReg; // CORRIGIDO: usar tempo atual, não antigo
            totalReg++;
            totalRegEmUso++;
            return i;
        }
    }
    return -1; // Nao foi possivel adicionar a variavel em nenhum registrador
}

int buscarVarReg(char* nomeVar, char* escopo){
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar != NULL){
            if(strcmp(listaReg[i].nomeVar, nomeVar) == 0 && strcmp(listaReg[i].escopo, escopo) == 0){
                // Aplica a mesma ideia do algoritmo LRU visto em SO
                atualizarUltimoUso(i);
                return i;
            }
        }
    }

    return -1; // Nao foi possivel encontrar a variavel em nenhum registrador
}

// Funcao para mostrar os registradores e suas informacoes na tela do usuario
void mostrarReg(){
    int cont = 0;
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar != NULL){
            fprintf(yyout, "t%d: %s, %s, uso=%d\n", listaReg[i].numReg, listaReg[i].nomeVar, listaReg[i].escopo, listaReg[i].ultimoUso);
        }
    }
    
    fprintf(yyout, "\nRegistradores Livres: %d.\n", MAX_REG - totalRegEmUso);
}

// Funcao para descartar um registrador usando algoritmo LRU (Least Recently Used)
int descartarReg(){
    int menorUltimoUso = totalReg + 1; // Inicializa com valor maior que qualquer último uso
    int regDescartado = -1;

    // Encontra o registrador com menor ultimoUso (mais antigo)
    for(int i = 0; i < MAX_REG; i++){	
        if(listaReg[i].nomeVar != NULL && strcmp(listaReg[i].nomeVar, stringTemp) == 0){ 
            // Apenas considera registradores temporários para descarte
            if(listaReg[i].ultimoUso < menorUltimoUso){
                menorUltimoUso = listaReg[i].ultimoUso;
                regDescartado = i;
            }
        }
    }

    // Se não encontrou temporários, descarta qualquer registrador (caso extremo)
    if(regDescartado == -1){
        for(int i = 0; i < MAX_REG; i++){	
            if(listaReg[i].nomeVar != NULL){ 
                if(listaReg[i].ultimoUso < menorUltimoUso){
                    menorUltimoUso = listaReg[i].ultimoUso;
                    regDescartado = i;
                }
            }
        }
    }

    if(regDescartado == -1){
        fprintf(stderr, ANSI_COLOR_RED "ERRO: Nao foi possivel descartar nenhum registrador\n" ANSI_COLOR_RESET);
        return -1;
    }

    // Limpa o registrador descartado
    listaReg[regDescartado].nomeVar = NULL;
    strcpy(listaReg[regDescartado].escopo, "");
    listaReg[regDescartado].descarte = 0;
    listaReg[regDescartado].ultimoUso = 0;
    totalRegEmUso--;
    

    if(DEBUG_MODE){
        fprintf(stderr, ANSI_COLOR_PURPLE "WARNING: " ANSI_COLOR_RESET "Descartado registrador t%d\n", regDescartado);
    }
    
    return regDescartado;
}

/* Funcao para verificar se a variavel ja esta em um registrador
Se nao estiver, adiciona a variavel em um registrador
Se estiver, retorna o numero do registrador em que a variavel esta
Se nao for possivel adicionar a variavel em um registrador, retorna -1 */
int verificacaoRegistradores(char *lexema, char* escopo, int boolTemp){
    int reg;

    if(boolTemp == 0){
        if((reg = (buscarVarReg(lexema, escopo))) == -1){
            if(totalRegEmUso == MAX_REG){
                if((reg = descartarReg()) == -1){
                    fprintf(stderr, ANSI_COLOR_RED "ERRO: Nao foi possivel descartar nenhum registrador\n" ANSI_COLOR_RESET);
                    return -1;
                }
            }
            
            if((reg = adicionarVarReg(lexema, escopo)) == -1){
                fprintf(stderr, ANSI_COLOR_RED "Erro ao adicionar variavel no vetor de registradores\n" ANSI_COLOR_RESET);
            }
        }
        
        return reg;
    }
    if(totalRegEmUso == MAX_REG){
        if((reg = descartarReg()) == -1){
            fprintf(stderr, ANSI_COLOR_RED "ERRO: Nao foi possivel descartar nenhum registrador\n" ANSI_COLOR_RESET);
            return -1;
        }
    }
    reg = adicionaTempReg();  // CORRIGIDO: sempre salva o resultado
    if(reg == -1){
        fprintf(stderr, ANSI_COLOR_RED "Erro ao adicionar variavel no vetor de registradores\n" ANSI_COLOR_RESET);
    }

    return reg;
    

}

// Funcao para atualizar o ultimo uso de um registrador (para LRU)
void atualizarUltimoUso(int regNum){
    if(regNum >= 0 && regNum < MAX_REG && listaReg[regNum].nomeVar != NULL){
        listaReg[regNum].ultimoUso = totalReg++;
    }
}
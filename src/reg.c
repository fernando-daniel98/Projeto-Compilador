#include <stdio.h>
#include <string.h>
#include <math.h>
// CORREÇÃO: Ajustar caminhos dos includes
#include "../include/globals.h"
#include "../include/codeGen.h"
#include "../include/reg.h"

#define MAX_REG 55 // Registradores t0-t54 (seguindo lógica do Eduardo mas com mais regs)
#define MAX_REG_DESCARTE 10000000 // Numero maximo de registradores que podem ser descartados

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
                return i;
            }
        }
    }

    return -1; // Nao foi possivel encontrar a variavel em nenhum registrador
}

// Funcao para mostrar os registradores e suas informacoes na tela do usuario
void mostrarReg(){
    int cont = 0;
    fprintf(yyout, "\n============== Registradores ===============\n");
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar != NULL){
            fprintf(yyout, "t%d: %s, %s, %d\n", listaReg[i].numReg, listaReg[i].nomeVar, listaReg[i].escopo, listaReg[i].descarte);
        }
    }
    
    fprintf(yyout, "\n");
    fprintf(yyout, "Registradores Livres: %d.\n", MAX_REG - totalRegEmUso);

}

// Funcao para descartar um registrador que nao esta sendo mais utilizado no momento
int descartarReg(){
    float menor = INFINITY; // Inicializa com um valor alto
    int regDescartado = -1;

    for(int i = 0; i < MAX_REG; i++){	
        if(listaReg[i].descarte > 0){ // Apenas considera registradores marcados para descarte
            if(menor > listaReg[i].descarte){
                menor = listaReg[i].descarte;
                regDescartado = i;
            }
        }
    }

    if(menor == MAX_REG_DESCARTE){
        fprintf(stderr, ANSI_COLOR_RED "ERRO: Nao foi possivel descartar nenhum registrador\n" ANSI_COLOR_RESET);

        return -1;
    }

    listaReg[regDescartado].nomeVar = NULL;
    strcpy(listaReg[regDescartado].escopo, "");
    listaReg[regDescartado].descarte = 0;
    totalRegEmUso--;
    

    if(DEBUG_MODE){
        fprintf(stderr, ANSI_COLOR_PURPLE "WARNING: " ANSI_COLOR_RESET "Descartado registrador t%d\n", regDescartado);
    }

    // Adicionar uma impressão de aviso, mesmo sem DEBUG_MODE, pode ser útil
    // fprintf(stderr, ANSI_COLOR_PURPLE "INFO: " ANSI_COLOR_RESET "Descartado registrador t%d\n", regDescartado);

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
    if((reg = adicionaTempReg()) == -1){
        fprintf(stderr, ANSI_COLOR_RED "Erro ao adicionar variavel no vetor de registradores\n" ANSI_COLOR_RESET);
    }

    return reg;
    

}
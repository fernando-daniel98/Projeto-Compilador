#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/globals.h"
#include "../include/codeGen.h"
#include "../include/reg.h"

char stringTemp[MAXTOKENLEN] = "Temporario";

int totalReg = 1;

int totalRegEmUso = 0; 

REG listaReg[MAX_REG];

void inicializaReg(){
    for(int i = 0; i < MAX_REG; i++){
        listaReg[i].numReg = i;
        listaReg[i].nomeVar = NULL;
        strcpy(listaReg[i].escopo, "");
        listaReg[i].descarte = 0;
        listaReg[i].ultimoUso = 0;
    }
}

int adicionarVarReg(char* nomeVar, char* escopo){
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar == NULL){
            listaReg[i].nomeVar = nomeVar;
            strcpy(listaReg[i].escopo, escopo);
            listaReg[i].descarte = 0;
            listaReg[i].ultimoUso = totalReg;
            totalReg++;
            totalRegEmUso++;
            return i;
        }
    }
    return -1;
}

int adicionaTempReg(){
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar == NULL){
            listaReg[i].nomeVar = stringTemp;
            strcpy(listaReg[i].escopo, funcName);
            listaReg[i].descarte = totalReg;
            listaReg[i].ultimoUso = totalReg;
            totalReg++;
            totalRegEmUso++;
            return i;
        }
    }
    return -1;
}

int buscarVarReg(char* nomeVar, char* escopo){
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar != NULL){
            if(strcmp(listaReg[i].nomeVar, nomeVar) == 0 && strcmp(listaReg[i].escopo, escopo) == 0){
                atualizarUltimoUso(i);
                return i;
            }
        }
    }

    return -1;
}

void mostrarReg(){
    int cont = 0;
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar != NULL){
            fprintf(yyout, "t%d: %s, %s, uso=%d\n", listaReg[i].numReg, listaReg[i].nomeVar, listaReg[i].escopo, listaReg[i].ultimoUso);
        }
    }
    
    fprintf(yyout, "\nRegistradores Livres: %d.\n", MAX_REG - totalRegEmUso);
}

int descartarReg(){
    int menorUltimoUso = totalReg + 1;
    int regDescartado = -1;

    for(int i = 0; i < MAX_REG; i++){	
        if(listaReg[i].nomeVar != NULL && strcmp(listaReg[i].nomeVar, stringTemp) == 0){ 
            if(listaReg[i].ultimoUso < menorUltimoUso){
                menorUltimoUso = listaReg[i].ultimoUso;
                regDescartado = i;
            }
        }
    }

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
    reg = adicionaTempReg();
    if(reg == -1){
        fprintf(stderr, ANSI_COLOR_RED "Erro ao adicionar variavel no vetor de registradores\n" ANSI_COLOR_RESET);
    }

    return reg;
    

}

void atualizarUltimoUso(int regNum){
    if(regNum >= 0 && regNum < MAX_REG && listaReg[regNum].nomeVar != NULL){
        listaReg[regNum].ultimoUso = totalReg++;
    }
}
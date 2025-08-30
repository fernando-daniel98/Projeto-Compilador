#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "codeGen.h"

#define MAX_REG 22 // Numero maximo de registradores
#define MAX_REG_DESCARTE 10000000 // Numero maximo de registradores que podem ser descartados

char stringTemp[MAXLEXEMA] = "Temporario"; // Nome para identificar variaveis temporarias

/* Essa variavel sera importante para o discarte dos registradores, ja
que registradores usados pela ultima vez a algum tempo serao eliminados primeiro
do que os usados recentemente, para tentar evitar conflitos em instrucoes */
int totalReg = 1;

int totalRegEmUso = 0; 

typedef struct reg{
    int numReg;
    char* nomeVar;
    char escopo[MAXLEXEMA];
    int descarte; // Diz se o registrador pode ser descartado (1, 2, ..., n) ou nao (0)
    int ultimoUso; // Timestamp do ultimo uso para algoritmo LRU
}REG;

REG listaReg[MAX_REG]; // Lista encadeada com os registradores

// Funcao para inicializar o vetor de registradores
void inicializaReg(){
    for(int i = 0; i < MAX_REG; i++){
        listaReg[i].numReg = i;
        listaReg[i].nomeVar = NULL;
        strcpy(listaReg[i].escopo, "");
        listaReg[i].descarte = 0;
        listaReg[i].ultimoUso = 0;
    }
}

// Adiciona uma variavel em um registrador
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
    return -1; // Nao foi possivel adicionar a variavel em nenhum registrador
}

// Adiciona uma variavel temporaria em um registrador, normalmente utilizada em operacoes
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
    return -1; // Nao foi possivel adicionar a variavel em nenhum registrador
}

int buscarVarReg(char* nomeVar, char* escopo){
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar != NULL){
            if(strcmp(listaReg[i].nomeVar, nomeVar) == 0 && strcmp(listaReg[i].escopo, escopo) == 0){
                listaReg[i].ultimoUso = totalReg; // Atualiza ultimo uso para LRU
                totalReg++;
                return i;
            }
        }
    }

    return -1; // Nao foi possivel encontrar a variavel em nenhum registrador
}

// Funcao para mostrar os registradores e suas informacoes na tela do usuario
void mostrarReg(){
    int cont = 0;
    printf("\n============== Registradores ===============\n");
    for(int i = 0; i < MAX_REG; i++){
        if(listaReg[i].nomeVar != NULL){
            printf("t%d: %s, %s, %d, uso:%d\n", listaReg[i].numReg, listaReg[i].nomeVar, listaReg[i].escopo, listaReg[i].descarte, listaReg[i].ultimoUso);
        }
    }
    printf(ANSI_COLOR_PURPLE);
    printf("%d Registradores Livres\n\n", MAX_REG - totalRegEmUso);
    printf(ANSI_COLOR_RESET);
}

// Funcao para descartar um registrador usando algoritmo LRU (Least Recently Used)
int descartarReg(){
    int menorUltimoUso = totalReg + 1;
    int regDescartado = -1;

    // Primeiro: tenta descartar entre registradores temporarios (stringTemp)
    for(int i = 0; i < MAX_REG; i++){	
        if(listaReg[i].nomeVar != NULL && strcmp(listaReg[i].nomeVar, stringTemp) == 0){ 
            if(listaReg[i].ultimoUso < menorUltimoUso){
                menorUltimoUso = listaReg[i].ultimoUso;
                regDescartado = i;
            }
        }
    }

    // Segundo: se nao encontrou temporarios, descarta qualquer registrador
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

    // Se ainda nao encontrou, erro
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
        if(listaReg[regDescartado].nomeVar == stringTemp) {
            fprintf(stderr, ANSI_COLOR_PURPLE "LRU DESCARTE: " ANSI_COLOR_RESET "Registrador t%d (TEMPORARIO, ultimoUso:%d) descartado\n", 
                    regDescartado, menorUltimoUso);
        } else {
            fprintf(stderr, ANSI_COLOR_PURPLE "LRU DESCARTE: " ANSI_COLOR_RESET "Registrador t%d (VARIAVEL '%s', ultimoUso:%d) descartado\n", 
                    regDescartado, listaReg[regDescartado].nomeVar ? listaReg[regDescartado].nomeVar : "NULL", menorUltimoUso);
        }
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
                    printf(ANSI_COLOR_RED);
                    printf("ERRO: Nao foi possivel descartar nenhum registrador\n");
                    printf(ANSI_COLOR_RESET);
                    return -1;
                }
            }
            
            if((reg = adicionarVarReg(lexema, escopo)) == -1){
                printf(ANSI_COLOR_RED);
                printf("Erro ao adicionar variavel no vetor de registradores");
                printf(ANSI_COLOR_RESET);
            }
        }

        return reg;
    }

    if(totalRegEmUso == MAX_REG){
        if((reg = descartarReg()) == -1){
            printf(ANSI_COLOR_RED);
            printf("ERRO: Nao foi possivel descartar nenhum registrador\n");
            printf(ANSI_COLOR_RESET);
            return -1;
        }
    }
    if((reg = adicionaTempReg()) == -1){
        printf(ANSI_COLOR_RED);
        printf("Erro ao adicionar variavel no vetor de registradores");
        printf(ANSI_COLOR_RESET);
    }

    return reg;
    

}
#ifndef _REG_H_
#define _REG_H_

#include "globals.h"

#define MAX_REG 23
#define TOTAL_REG 32
#define MAX_REG_DESCARTE 10000000

#define REG_ZERO 31
#define REG_RA 30
#define REG_FP 29
#define REG_SP 28
#define REG_TEMP 27
#define REG_PILHA 26
#define REG_S2 25
#define REG_S1 24
#define REG_S0 23

#define REG_T_BASE 0
#define REG_T_MAX 22

typedef struct reg {
    int numReg;
    char* nomeVar;
    char escopo[MAXTOKENLEN];
    int descarte;
    int ultimoUso;
} REG;

extern REG listaReg[MAX_REG];
extern int totalReg;
extern int totalRegEmUso;

void inicializaReg();
int adicionarVarReg(char* nomeVar, char* escopo);
int adicionaTempReg();
int buscarVarReg(char* nomeVar, char* escopo);
void mostrarReg();
int descartarReg();
int verificacaoRegistradores(char *lexema, char* escopo, int boolTemp);
void atualizarUltimoUso(int regNum);

#endif

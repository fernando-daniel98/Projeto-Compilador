#ifndef _REG_H_
#define _REG_H_

#include "globals.h"

// Definições de registradores COMPATÍVEIS com Eduardo (32 registradores total)
#define MAX_REG 23          // Registradores t0-t22 disponíveis (23 total) IGUAL AO EDUARDO
#define TOTAL_REG 32        // Total de registradores físicos IGUAL AO EDUARDO
#define MAX_REG_DESCARTE 10000000

// Registradores especiais EXATOS como Eduardo
#define REG_ZERO 31         // Registrador sempre zero (Eduardo: 31)
#define REG_RA 30           // Return Address (Eduardo: 30)
#define REG_FP 29           // Frame Pointer (Eduardo: 29)
#define REG_SP 28           // Stack Pointer (Eduardo: 28)
#define REG_TEMP 27         // Registrador temporário especial (Eduardo: 27)
#define REG_PILHA 26        // Registrador para pilha de parâmetros (Eduardo: 26)
#define REG_S2 25           // Saved register 2 (Eduardo: 25)
#define REG_S1 24           // Saved register 1 (Eduardo: 24)
#define REG_S0 23           // Saved register 0 / $t23 (Eduardo: 23)

// Mapeamento dos registradores temporários EXATO como Eduardo (t0-t22)
#define REG_T_BASE 0        // Registradores t0 até t22 (0-22)
#define REG_T_MAX 22        // Último registrador temporário IGUAL AO EDUARDO

// Estrutura para gerenciar registradores
typedef struct reg {
    int numReg;                     // Número do registrador físico
    char* nomeVar;                  // Nome da variável armazenada
    char escopo[MAXTOKENLEN];       // Escopo da variável
    int descarte;                   // Prioridade para descarte (0 = não pode descartar)
    int ultimoUso;                  // Timestamp do último uso
} REG;

// Variáveis globais para gerenciamento de registradores
extern REG listaReg[MAX_REG];
extern int totalReg;
extern int totalRegEmUso;

// Funções para gerenciamento de registradores
void inicializaReg();
int adicionarVarReg(char* nomeVar, char* escopo);
int adicionaTempReg();
int buscarVarReg(char* nomeVar, char* escopo);
void mostrarReg();
int descartarReg();
int verificacaoRegistradores(char *lexema, char* escopo, int boolTemp);
void atualizarUltimoUso(int regNum);  // Nova função para LRU

#endif // _REG_H_

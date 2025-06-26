#ifndef _REG_H_
#define _REG_H_

#include "globals.h"

// Definições de registradores para sua arquitetura (64 registradores)
#define MAX_REG 55          // Registradores t0-t54 disponíveis (55 total)
#define TOTAL_REG 64        // Total de registradores físicos (como Eduardo tinha 32)
#define MAX_REG_DESCARTE 10000000

// Registradores especiais da sua arquitetura (baseado no Eduardo)
#define REG_ZERO 63         // Registrador sempre zero (Eduardo: 31)
#define REG_RA 62           // Return Address (Eduardo: 30)
#define REG_FP 61           // Frame Pointer (Eduardo: 29)
#define REG_SP 60           // Stack Pointer (Eduardo: 28)
#define REG_TEMP 59         // Registrador temporário especial (Eduardo: 27)
#define REG_PILHA 58        // Registrador para pilha de parâmetros (Eduardo: 26)
#define REG_S2 57           // Saved register 2 (Eduardo: 25)
#define REG_S1 56           // Saved register 1 (Eduardo: 24)
#define REG_S0 55           // Saved register 0 (Eduardo: 23)

// Mapeamento dos registradores temporários (t0-t54)
#define REG_T_BASE 0        // Registradores t0 até t54 (0-54)
#define REG_T_MAX 54        // Último registrador temporário

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

#endif // _REG_H_

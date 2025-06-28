#ifndef BINARIO_H
#define BINARIO_H 1

#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

/* Estruturas binárias otimizadas para o processador MIPS-64 modificado
   As structs são declaradas com bit fields para economizar memória
   e facilitar a escrita binária direta no arquivo */

/* Formato R-Type: 32 bits
   opcode(6) | rs(6) | rt(6) | rd(6) | shamt(2) | funct(6) */
typedef struct {
    unsigned int funct:6;    // bits 5-0: função específica
    unsigned int shamt:2;    // bits 7-6: shift amount (reduzido para 2 bits)
    unsigned int rd:6;       // bits 13-8: registrador destino
    unsigned int rt:6;       // bits 19-14: registrador fonte 2
    unsigned int rs:6;       // bits 25-20: registrador fonte 1
    unsigned int opcode:6;   // bits 31-26: código da operação
} BIN_R;

/* Formato I-Type: 32 bits
   opcode(6) | rs(6) | rt(6) | immediate(14) */
typedef struct {
    unsigned int immediate:14; // bits 13-0: valor imediato ou offset (expandido para 14 bits)
    unsigned int rt:6;         // bits 19-14: registrador destino/fonte
    unsigned int rs:6;         // bits 25-20: registrador base
    unsigned int opcode:6;     // bits 31-26: código da operação
} BIN_I;

/* Formato J-Type: 32 bits
   opcode(6) | address(26) */
typedef struct {
    unsigned int address:26;   // bits 25-0: endereço de salto (expandido para 26 bits)
    unsigned int opcode:6;     // bits 31-26: código da operação
} BIN_J;

// Funções de conversão
unsigned int get_opcode(char* nome, tipoInstrucao tipo);
unsigned int get_funct(char* nome);
unsigned int get_register(int reg);
unsigned int get_shamt(int shamt);
unsigned int get_immediate(int imediato);
unsigned int get_address(char* label);

// Funções de geração binária
BIN_R* binarioNop(void);
BIN_R* binarioR(ASSEMBLY* instrucao);
BIN_I* binarioI(ASSEMBLY* instrucao);
BIN_J* binarioJ(ASSEMBLY* instrucao);

// Funções de saída
void printBits(size_t const size, void const * const ptr, FILE* arquivo);
void binario(FILE* arquivo);
void binario_debug(FILE* arquivo);
void salvarBinario(const char* nomeArquivo);
void salvarBinarioDebug(const char* nomeArquivo);

#endif /* BINARIO_H */

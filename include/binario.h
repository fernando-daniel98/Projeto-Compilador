#ifndef BINARIO_H
#define BINARIO_H 1

#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

typedef struct {
    unsigned int funct:6;
    unsigned int shamt:5;
    unsigned int rd:5;
    unsigned int rt:5;
    unsigned int rs:5;
    unsigned int opcode:6;
} BIN_R;

typedef struct {
    unsigned int immediate:16;
    unsigned int rt:5;
    unsigned int rs:5;
    unsigned int opcode:6;
} BIN_I;

typedef struct {
    unsigned int address:26;
    unsigned int opcode:6;
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

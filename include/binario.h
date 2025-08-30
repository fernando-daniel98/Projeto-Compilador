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

void binario(FILE* arquivo);
void binario_debug(FILE* arquivo);


#endif /* BINARIO_H */
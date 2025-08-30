#ifndef Assembly_H
#define Assembly_H 1

#include "label.h"

#define MAX_CHAR_NOME 5
#define MAX_ASSEMBLY 10000

typedef enum{
    typeR,
    typeI,
    typeJ, 
    typeLabel 
} tipoInstrucao;

typedef struct tipoR{
    char *nome;
    int rd;
    int rt;
    int rs;
    int shamt;
} TIPO_R;

typedef struct tipoI{
    char *nome;
    int rs;
    int rt;
    int imediato;
    int label;
} TIPO_I;

typedef struct tipoJ{
    char *nome;
    char *labelImediato;
} TIPO_J;

typedef struct tipoLabel{
    int boolean; 
    char *nome; 
    int endereco; 
} TIPO_LABEL;

typedef struct assembly{
    tipoInstrucao tipo;         // Tipo da instrucao
    TIPO_I * tipoI;             // struct do tipo I
    TIPO_R * tipoR;             // struct do tipo R
    TIPO_J * tipoJ;             // struct do tipo J
    TIPO_LABEL * tipoLabel;     // struct do tipo Label
} ASSEMBLY;

// Vetor para armazenar as instrucoes em assembly
extern ASSEMBLY ** instrucoesAssembly;
extern int indiceAssembly; // Indice para o vetor de instrucoes assembly

void assembly();
void inicializaAssembly();
ASSEMBLY * criarNoAssembly(tipoInstrucao tipo, char *nome);
void imprimirAssembly();
void liberarAssembly();

#endif
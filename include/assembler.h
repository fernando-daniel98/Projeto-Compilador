#ifndef Assembly_H
#define Assembly_H 1

// Inclusao de bibliotecas
#include "label.h" // Mantido, pode ser necessário para TIPO_LABEL ou outros usos futuros
#include "globals.h" // Adicionado para yyout, caso seja usado aqui no futuro, e para consistência

// Capacidade maxima para o vetor dos nomes das instrucoes
#define MAX_CHAR_NOME 5
#define MAX_ASSEMBLY 10000

// Definições de registradores MIPS (valores numéricos)
#define $zero 0   // Registrador sempre zero
#define $at 1     // Assembler Temporary
#define $v0 2     // Return value 0
#define $v1 3     // Return value 1
#define $a0 4     // Argument 0
#define $a1 5     // Argument 1
#define $a2 6     // Argument 2
#define $a3 7     // Argument 3
#define $t0 8     // Temporary 0
#define $t1 9     // Temporary 1
#define $t2 10    // Temporary 2
#define $t3 11    // Temporary 3
#define $t4 12    // Temporary 4
#define $t5 13    // Temporary 5
#define $t6 14    // Temporary 6
#define $t7 15    // Temporary 7
#define $s0 16    // Saved 0
#define $s1 17    // Saved 1
#define $s2 18    // Saved 2
#define $s3 19    // Saved 3
#define $s4 20    // Saved 4
#define $s5 21    // Saved 5
#define $s6 22    // Saved 6
#define $s7 23    // Saved 7
#define $t8 24    // Temporary 8
#define $t9 25    // Temporary 9
#define $k0 26    // Kernel 0
#define $k1 27    // Kernel 1
#define $gp 28    // Global Pointer
#define $sp 29    // Stack Pointer
#define $fp 30    // Frame Pointer
#define $ra 31    // Return Address

// Registradores customizados do seu código original
#define $temp 59
#define $pilha 58

typedef enum{
    typeR, // Instrucoes do tipo R
    typeI, // Instrucoes do tipo I
    typeJ, // Instrucoes do tipo J
    typeLabel // Label de funcoes ou de pulos
} tipoInstrucao;

/* Struct para armazenar as informacoes
de instrucoes do tipo R */
typedef struct tipoR{
    char *nome; // Nome da instrucao
    int rd; // Registrador destino
    int rt; // Registrador fonte
    int rs; // Registrador fonte
} TIPO_R;

/* Struct para armazenar as informacoes
de instrucoes do tipo I */
typedef struct tipoI{
    char *nome; // Nome da instrucao
    int rs; // Registrador fonte
    int rt; // Registrador destino
    int imediato; // Valor imediato
    int label; // Label para o branch
} TIPO_I;

/* Struct para armazenar as informacoes 
de instrucoes do tipo J */
typedef struct tipoJ{
    char *nome; // Nome da instrucao
    char *labelImediato; // Nome da Label para o jump
} TIPO_J;

typedef struct tipoLabel{
    int boolean; // Booleano para verificar se eh label(1) ou funcao(0)
    char *nome; // Nome da label
    int endereco; // Endereco da label
} TIPO_LABEL;


typedef struct assembly{
    tipoInstrucao tipo; // Tipo da instrucao
    TIPO_I * tipoI; // Ponteiro para a struct do tipo I
    TIPO_R * tipoR; // Ponteiro para a struct do tipo R
    TIPO_J * tipoJ; // Ponteiro para a struct do tipo J
    TIPO_LABEL * tipoLabel; // Ponteiro para a struct do tipo Label
} ASSEMBLY;

// Vetor para armazenar as instrucoes em assembly
extern ASSEMBLY ** instrucoesAssembly;
extern int indiceAssembly; // Indice para o vetor de instrucoes assembly

// Funcoes para a geracao do codigo assembly
void assembly();
void inicializaAssembly();
ASSEMBLY * criarNoAssembly(tipoInstrucao tipo, char *nome);
void imprimirAssembly();
void liberarAssembly();

#endif
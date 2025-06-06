#ifndef Assembly_H
#define Assembly_H 1

// Inclusao de bibliotecas
#include "label.h" // Mantido, pode ser necessário para TIPO_LABEL ou outros usos futuros
#include "globals.h" // Adicionado para yyout, caso seja usado aqui no futuro, e para consistência

// Capacidade maxima para o vetor dos nomes das instrucoes
#define MAX_CHAR_NOME 5
#define MAX_ASSEMBLY 10000

// Definições de registradores MIPS (valores numéricos)
#define $zero 0     // Registrador zero constante
#define $v0 2       // Valor de retorno de função, código de syscall
#define $a0 4       // Argumento de função/syscall
#define $sp 29      // Stack pointer
#define $fp 30      // Frame pointer
#define $ra 31      // Return address
#define $s0 16      // Registrador salvo (usado como base para globais no código original)
#define $temp 24    // Registrador temporário ($t8) - usado em assembler.c
#define $pilha 26   // Registrador para a "pilha de parâmetros" (usando $k0) - usado em assembler.c
// Para temp_reg_neq, ele é definido localmente em opRelacionais em assembler.c.
// Se for necessário em funcoes_assembly.c, pode ser adicionado aqui ou passado.

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
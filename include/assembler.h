#ifndef Assembly_H
#define Assembly_H 1

#include "label.h"
#include "globals.h"

#define MAX_CHAR_NOME 5
#define MAX_ASSEMBLY 10000

#define $zero 31
#define $ra 30
#define $fp 29
#define $sp 28
#define $temp 27
#define $pilha 26
#define $s2 25
#define $s1 24
#define $s0 23

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
void imprimirAssemblyParaArquivo(FILE* arquivo); // Nova função que aceita FILE*
void liberarAssembly();
void resolverLabels();  // Nova função para resolver referencias de labels
void imprimirAssemblySemLabels(); // Imprimir assembly sem labels e com referências resolvidas
void imprimirAssemblySemLabelsParaArquivo(FILE* arquivo); // Nova função que aceita FILE*
void salvarAssemblyLimpo(const char* nomeArquivo); // Salvar assembly em arquivo sem números de linha
void salvarAssemblySemLabelsArquivo(const char* nomeArquivo); // Salvar assembly sem labels em arquivo
void salvarAssemblyPuro(const char* nomeArquivo); // Salvar assembly puro sem números de linha (último print CA)

// Funcao para mapear numero do registrador para nome legivel
const char* getRegisterName(int regNum);

#endif
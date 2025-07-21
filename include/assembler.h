#ifndef Assembly_H
#define Assembly_H 1

// Inclusao de bibliotecas
#include "label.h" // Mantido, pode ser necessário para TIPO_LABEL ou outros usos futuros
#include "globals.h" // Adicionado para yyout, caso seja usado aqui no futuro, e para consistência

// Capacidade maxima para o vetor dos nomes das instrucoes
#define MAX_CHAR_NOME 5
#define MAX_ASSEMBLY 10000

// Definições de registradores COMPATÍVEIS com o Eduardo (32 registradores)
// Mapeamento EXATO da arquitetura MIPS-like do Eduardo
#define $zero 31   // Registrador sempre zero (read-only)
#define $ra 30     // Return Address (endereço de retorno)
#define $fp 29     // Frame Pointer (ponteiro do frame atual)
#define $sp 28     // Stack Pointer (ponteiro da pilha)
#define $temp 27   // Registrador temporário especial
#define $pilha 26  // Registrador para pilha de parâmetros
#define $s2 25     // Saved register 2
#define $s1 24     // Saved register 1
#define $s0 23     // Saved register 0 (BASE DE MEMÓRIA - Eduardo usa $t23)

// Registradores temporários t0-t22 (EXATO como Eduardo)
// Mapeamento direto: $t{n} = n (para n de 0 até 22)
// IMPORTANTE: Eduardo usa $t23 como base de memória!

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
#ifndef CODEGEN_H
#define CODEGEN_H

#include "globals.h"

// Tipos de operação TAC
typedef enum {
    TAC_ADD,        // +
    TAC_SUB,        // -
    TAC_MULT,       // *
    TAC_DIV,        // /
    TAC_LT,         // <
    TAC_LTE,        // <=
    TAC_GT,         // >
    TAC_GTE,        // >=
    TAC_EQ,         // ==
    TAC_NEQ,        // !=
    TAC_ASSIGN,     // =
    TAC_LABEL,      // etiqueta
    TAC_GOTO,       // desvio incondicional
    TAC_IF,         // if-goto
    TAC_IFFALSE,    // iffalse-goto
    TAC_RETURN,     // return
    TAC_PARAM,      // parâmetro
    TAC_CALL,       // chamada de função
    TAC_ARG,        // argumento
    TAC_FUNC,       // declaração de função
    TAC_END,        // fim de função
    TAC_ARRAY_INDEX,// acesso a elemento de array a[i]
    TAC_ARRAY_ASSIGN// atribuição para elemento de array a[i] = x
} TACOp;

// Estrutura para instrução de código de três endereços
typedef struct TACInstr {
    TACOp op;           // operador
    char *arg1;         // primeiro operando
    char *arg2;         // segundo operando
    char *result;       // resultado
    struct TACInstr *next; // próxima instrução
} TACInstr;

// Gerador de código TAC
typedef struct {
    TACInstr *head;     // primeira instrução
    TACInstr *tail;     // última instrução
    int tempCount;      // contador para variáveis temporárias
    int labelCount;     // contador para etiquetas
} TACGenerator;

// Inicializa o gerador de código TAC
void initTAC(void);

// Libera os recursos do gerador de código TAC
void finalizeTAC(void);

// Gera código TAC a partir da árvore sintática
void generateTAC(TreeNode *syntaxTree);

// Imprime o código TAC gerado
void printTAC(FILE *out);

// Gera um novo nome para variável temporária
char *newTemp(void);

// Gera um novo rótulo
char *newLabel(void);

// Adiciona uma instrução TAC
TACInstr *addTACInstr(TACOp op, char *arg1, char *arg2, char *result);

// Função para converter enum TACOp para string
const char *tacOpToString(TACOp op);

#endif

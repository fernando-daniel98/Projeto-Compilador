#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include "globals.h"

// Tipos de operação TAC (Three Address Code)
typedef enum {
    ADD,            // +
    SUB,            // -
    MULT,           // *
    DIV,            // /
    LT,             // <
    LTE,            // <=
    GT,             // >
    GTE,            // >=
    EQ,             // ==
    NEQ,            // !=
    ASSIGN,         // =
    OP_DESCON,      // operador desconhecido
    LABEL,          // etiqueta
    GOTO,           // desvio incondicional
    IF,             // if-goto
    IFFALSE,        // iffalse-goto
    RETURN,         // return
    PARAM,          // parâmetro
    CALL,           // chamada de função
    ARG,            // argumento
    FUNC,           // declaração de função
    END,            // fim de função
    ARRAY_INDEX,    // acesso a elemento de array a[i]
    ARRAY_ASSIGN,   // atribuição para elemento de array a[i] = x
} typeOperations;

// Estrutura para instrução de código de três endereços
typedef struct TACInstr {
    typeOperations op;      // operação
    char *arg1;             // primeiro operando
    char *arg2;             // segundo operando
    char *result;           // resultado
} quadruple;

// Funções criadas
void percorreAAS(TreeNode* node);

#endif
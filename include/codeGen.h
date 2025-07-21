#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include "globals.h"
#include "syntax_tree.h"
#include "tab.h"

#define MAX_LEN_CODE_INTERMEDIATE 1000

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
    IFF,            // if-false-goto (COMPATÍVEL COM EDUARDO)
    RET,            // return (COMPATÍVEL COM EDUARDO)
    PARAM,          // parâmetro
    CALL,           // chamada de função
    ARG,            // argumento
    FUNC,           // declaração de função
    END,            // fim de função
    ARRAY_INDEX,    // acesso a elemento de array a[i]
    ARRAY_ASSIGN,   // atribuição para elemento de array a[i] = x
    ALLOC,          // Para "ALLOC"
    LOADI,          // Para "LOADI" (Load Immediate)
    LOAD,           // Para "LOAD"
    STORE,          // Para "STORE"
    HALT            // Para "HALT" (finalização do programa)
} typeOperations;

// Tipos de enderecos
typedef enum {Vazio, IntConst, String} tipoEndereco;

/* Estrutura de enderecos, em que armazena o seu tipo e o
valor determinado pelo mesmo, ou seja, numero ou char* */
typedef struct endereco{
    tipoEndereco tipo;
    int val;
    int boolReg; //Diz se o endereco eh um numero (0), Reg (1) ou label (2)
    char* nome;
} ENDERECO;

// Estrutura para instrução de código de três endereços
typedef struct instruction {
    typeOperations operation;   // operação
    ENDERECO * oper1;           // primeiro operando
    ENDERECO * oper2;           // segundo operando
    ENDERECO * oper3;           // resultado
} quadruple;


extern quadruple ** intermediateCode;
extern int adressCounter;
extern int numReg;
extern int numLabel;
extern char funcName[MAXTOKENLEN];

// Funções criadas
int createIntermediateCode(TreeNode *tree, PnoIdentificador* symbolTable);
void freeIntermediateCode(); // Removido parâmetro, operará em globais

// Funções para inicialização e manipulação de estruturas
void alocateIntermediateCode();
int initIntermediateCode();
quadruple* criaInstrucao(typeOperations operation);

// Funções para manipulação de declarações
void insertDeclarationIf(TreeNode *tree, PnoIdentificador* symbtable);
void insertDeclarationWhile(TreeNode *tree, PnoIdentificador* symbtable);
void insertDeclarationReturn(TreeNode *tree, PnoIdentificador* symbtable);
void insertDeclarationVar(TreeNode *tree, PnoIdentificador* symbtable);
void insertDeclarationAssign(TreeNode *tree, PnoIdentificador* symbtable);
void criarCodigoIntermediario(TreeNode *tree, PnoIdentificador* symbtable, int flag);

// Funções para manipulação de expressões
void insertExpressionOp(TreeNode *tree, PnoIdentificador* symbtable);
void insertExpressionRel(TreeNode *tree, PnoIdentificador* symbtable);
void insertExpressionConst(TreeNode *tree, PnoIdentificador* symbtable); // Corrigido typo aqui (removida a duplicata "inserExpressionConst")
void insertExpressionId(TreeNode *tree, PnoIdentificador* symbtable);
void insertExpressionCall(TreeNode *tree, PnoIdentificador* symbtable);

// Funções para geração do código intermediário
ENDERECO* criaEndereco(tipoEndereco tipo, int val, char* nome, int boolReg);
void adicionaQuadrupla(typeOperations op, ENDERECO* oper1, ENDERECO* oper2, ENDERECO* oper3);

// Funções para registradores
void inicializaReg();
int adicionarVarReg(char* nomeVar, char* escopo);
int adicionaTempReg();
int buscarVarReg(char* nomeVar, char* escopo);
void mostrarReg();
int descartarReg();
int verificacaoRegistradores(char *lexema, char* escopo, int boolTemp);

// Funções auxiliares para impressão
void imprimeCodigoIntermediario();
const char* getTACOperationName(typeOperations op);
typeOperations mapOperatorToTAC(int op);

// Função para mapear numero do registrador para nome legivel (declaração compartilhada)
const char* getRegisterName(int regNum);

#endif
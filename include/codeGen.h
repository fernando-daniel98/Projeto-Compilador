#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include "globals.h"
#include "syntax_tree.h"
#include "symbol_table.h"

#define MAX_LEN_CODE_INTERMEDIATE 100

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

extern quadruple ** intermediateCode; // vetor de código intermediário
extern int adressCounter; // contador de endereços
extern int numReg; // número do registrador
extern int numLabel; // número do label
extern char funcName[MAXTOKENLEN]; // nome da função

// Funções criadas
void percorreAAS(TreeNode* node);
int createIntermediateCode(TreeNode *tree, PnoIdentificador* symbolTable);
void freeIntermediateCode(quadruple **codeIntermediate);
void insertQuadruple(TreeNode *tree, PnoIdentificador* symbtable, int flagCall);
void insertExpression(TreeNode *tree, PnoIdentificador* symbtable);
void insertStatement(TreeNode *tree, PnoIdentificador* symbtable);

// Funções para inicialização e manipulação de estruturas
quadruple **alocateIntermediateCode();
int initIntermediateCode();
quadruple* criaInstrucao(char* operation);
int verificacaoRegistradores(void* param1, void* param2, int increment);

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
void insertExpressionConst(TreeNode *tree, PnoIdentificador* symbtable);
void inserExpressionConst(TreeNode *tree, PnoIdentificador* symbtable);
void insertExpressionId(TreeNode *tree, PnoIdentificador* symbtable);
void insertExpressionCall(TreeNode *tree, PnoIdentificador* symbtable);

// Funções para geração do código intermediário
ENDERECO* criaEndereco(tipoEndereco tipo, int val, char* nome, int boolReg);
void adicionaQuadrupla(typeOperations op, ENDERECO* oper1, ENDERECO* oper2, ENDERECO* oper3);
void codIntConstExpression(TreeNode *tree);
void codIntIdExpression(TreeNode *tree);
void codIntCallExpression(TreeNode *tree, PnoIdentificador* symbtable);
void codIntAssignExpression(TreeNode *tree, PnoIdentificador* symbtable);

void imprimeCodigoIntermediario();
const char* getTACOperationName(typeOperations op);
typeOperations mapOperatorToTAC(int op);

#endif
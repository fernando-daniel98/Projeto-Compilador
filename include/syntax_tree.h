#ifndef _SYNTAX_TREE_H_
#define _SYNTAX_TREE_H_

#include "globals.h"

// Definições de tipos para a árvore sintática
typedef enum {StatementK, ExpressionK, TypeK} NodeKind;

typedef enum {
    IfK,
    WhileK,
    ReturnINT,
    ReturnVOID,
    NuloDecl,
    VarDeclK,
    VetDeclK,
    FunDeclK,
    VarParamK,
    VetParamK,
    ParamVoid
} StatementKind;

typedef enum {
    OpK, 
    OpRel, 
    ConstK, 
    IdK, 
    AtivK, 
    VetorK, 
    AssignK,
    NuloExp
} ExpressionKind;

typedef enum {
    PLUS_OP, MINUS_OP, MULT_OP, DIV_OP,
    LT_OP, LTE_OP, GT_OP, GTE_OP,
    EQ_OP, NEQ_OP, ASSIGN_OP
} OperatorType;

// Estrutura do nó da árvore sintática
typedef struct treeNode {
    struct treeNode * child[MAXCHILDREN];
    struct treeNode * sibling;
    int lineno;
    NodeKind nodekind;
    union { StatementKind stmt; ExpressionKind exp;} kind;
    union { int op;
            int val;
            char *name; } attr;
    ExpType type; /* for type checking of exps */
} TreeNode;

extern TreeNode *syntaxTree;

// Funções para manipulação da árvore sintática
TreeNode* newNode(NodeKind nodekind);
void printTree(TreeNode* tree);
void printSpaces(void);
void freeTree(TreeNode* tree);
void printNodeInfo(FILE* output, TreeNode* node);
void printTreeHierarchy(FILE* output, TreeNode* tree, int level, const char* prefix);
void displayTreeHierarchy(TreeNode* tree, FILE* output);
int isUnaryNegative(TreeNode* tree);

#endif
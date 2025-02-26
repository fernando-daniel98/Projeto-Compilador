#ifndef _GLOBALS_H_
#define _GLOBALS_H_

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum {StatementK,ExpressionK} NodeKind;

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

/* ExpType is used for type checking */
typedef enum {Void,Integer} ExpType;

#define MAXCHILDREN 3
#define MAXNOLIN 20

typedef struct treeNode
   {

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

typedef enum {
        PLUS_OP, MINUS_OP, MULT_OP, DIV_OP,
        LT_OP, LTE_OP, GT_OP, GTE_OP,
        EQ_OP, NEQ_OP, ASSIGN_OP
} OperatorType;

TreeNode* newNode(NodeKind nodekind);

typedef struct noIdentificador{
        char nomeIdentificador[MAXNOLIN];
        StatementKind tipoIdentificador;
        char escopo[MAXNOLIN];
        ExpType tipoDado;
        int linhas[MAXNOLIN];
        struct noIdentificador *prox, *ant;
    } noIdentificador;
    
typedef noIdentificador *PnoIdentificador;

extern PnoIdentificador* symbolTable;

void printTree(TreeNode* tree);
void printSpaces(void);
void freeTree(TreeNode* tree);

void deleteSymTab(void);
void buildSymTabFromTree(TreeNode* tree);
extern char currentScope[MAXNOLIN];
void mostraTabelaSimbolos(PnoIdentificador *tabelaHash);

#endif
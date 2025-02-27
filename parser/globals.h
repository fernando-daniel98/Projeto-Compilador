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
#define MAXNOLIN 40
#define MAXTOKENLEN 40


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
        char nomeIdentificador[MAXTOKENLEN];
        StatementKind tipoIdentificador;
        char escopo[MAXTOKENLEN];
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
extern char currentScope[MAXTOKENLEN];
void mostraTabelaSimbolos(PnoIdentificador *tabelaHash);
void checkAndSetType(TreeNode *node);

// Coloracoes em ANSI
#define ANSI_COLOR_RED      "\x1b[31m" // Erro léxico
#define ANSI_COLOR_GRAY     "\e[0;37m"
#define ANSI_COLOR_WHITE    "\e[1;37m" // Num da linha
#define ANSI_COLOR_RESET    "\e[0m"
#define ANSI_COLOR_PURPLE   "\e[0;35m" // Erro semântico
#define ANSI_COLOR_GREEN    "\e[0;32m" // Erro sintático

#endif
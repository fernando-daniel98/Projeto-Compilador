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

extern int lexical_errors;
extern int syntax_errors;
extern int semantic_errors;

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

PnoIdentificador* inicializaTabela(void);
void deleteSymTab(void);
void buildSymTabFromTree(TreeNode* tree);
extern char currentScope[MAXTOKENLEN];
void mostraTabelaSimbolos(PnoIdentificador *tabelaHash);
void checkAssignmentTypes(TreeNode* node);
void checkMainFunction(void);
int verifyRedeclaration(char *nome, char *escopo);

// Coloracoes em ANSI
#define ANSI_COLOR_RESET    "\e[0m"
#define ANSI_COLOR_RED      "\e[0;31m" // Erro léxico
#define ANSI_COLOR_YELLOW   "\e[0;33m" // Erro sintático
#define ANSI_COLOR_PURPLE   "\e[0;35m" // Erro semântico
#define ANSI_COLOR_WHITE    "\e[1;37m" // Num da linha
#define ANSI_COLOR_GREEN    "\e[1;32m" // Recuperação de erro

void generateDotFile(TreeNode* tree, const char* filename);

#endif
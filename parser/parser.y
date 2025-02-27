%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

extern FILE *yyin;
extern FILE *yyout;
extern char *yytext;

extern int lineNum;

extern int yylex(void);
void yyerror(const char *s);

static TreeNode *savedTree;

void debug_print(const char* msg) {
    printf("DEBUG: %s\n", msg);
    fflush(stdout);
}

%}

%code requires {
    #include "globals.h"    /* Add this include here as well */
}

%union {
    TreeNode *node;
    ExpType type;
}

/* Declare types for all non-terminals that return a node */
%type <node> programa
%type <node> declaracao_lista declaracao
%type <node> var_declaracao fun_declaracao
%type <node> param_lista param
%type <node> composto_decl local_declaracoes statement_lista
%type <node> statement expressao_decl selecao_decl iteracao_decl
%type <node> retorno_decl expressao var
%type <node> simples_expressao soma_expressao termo fator
%type <node> ativacao args arg_lista params relacional soma mult

/* Declare type for tipo_especificador */
%type <type> tipo_especificador

/* Declare types for tokens */
%token <node> ID NUM
%token <node> IF ELSE WHILE RETURN
%token <node> PLUS MINUS MULT DIV
%token <node> SMAL SMALEQ GREAT GREATEQ EQ DIFF ASSIGN

/* Tokens without values */
%token INT VOID
%token SEMICOL COMMA
%token LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%left ASSIGN
%left EQ DIFF SMAL SMALEQ GREAT GREATEQ
%left PLUS MINUS
%left MULT DIV
%right UMINUS

%%

programa: declaracao_lista { savedTree = $1;} ;

declaracao_lista: 
    declaracao_lista declaracao 
        { 
            TreeNode* t = $1;
            if (t != NULL)
            { 
                while (t->sibling != NULL) {
                    t = t->sibling;
                }
                t->sibling = $2;
                $$ = $1;
            }
            else $$ = $2;
        }
    | declaracao { $$ = $1; }
;

declaracao: 
    var_declaracao { $$ = $1;}
    | fun_declaracao { $$ = $1;}
;

var_declaracao: 
    tipo_especificador ID SEMICOL
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VarDeclK;
        t->attr.name = $2->attr.name;  // Copy the ID name
        t->type = $1;  // Cast back to ExpType
        free($2);  // Free the ID node since we copied its data
        $$ = t;
    }
    | 
    tipo_especificador ID LBRACKET NUM RBRACKET SEMICOL
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VetDeclK;       // Vector declaration
        t->attr.name = $2->attr.name;  // Copy array name
        t->type = $1;         // Cast back to ExpType
        t->child[0] = $4;             // Store size of array (NUM node)
        free($2);                      // Free the ID node
        $$ = t;
    }
;

tipo_especificador:
    INT  { $$ = Integer; }
    | VOID { $$ = Void; }
;

fun_declaracao: 
    tipo_especificador ID LPAREN params RPAREN composto_decl {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = FunDeclK;
        t->attr.name = $2->attr.name;
        t->type = $1;
        t->child[0] = $4;  // params
        t->child[1] = $6;  // compound statement
        t->lineno = $2->lineno;  // Set line number from the ID token
        free($2);
        $$ = t;
    }
;

params: 
    param_lista { $$ = $1; }
    | VOID 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = ParamVoid;
        $$ = t;
    }
;

param_lista: 
    param_lista COMMA param 
    {
        TreeNode* t = $1;
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = $3;
            $$ = $1;
        }
        else $$ = $3;
    }
    | param { $$ = $1; }
;

param: 
    tipo_especificador ID 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VarParamK;

        if ($2 && $2->attr.name) {
            t->attr.name = strdup($2->attr.name);
        }

        t->type = $1;
        
        free($2);
        
        $$ = t;
    }
    | tipo_especificador ID LBRACKET RBRACKET 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VetParamK;
        
        if ($2 && $2->attr.name) {
            t->attr.name = strdup($2->attr.name);
        } 
        
        t->type = $1;
        
        free($2);
        $$ = t;
    }
;

composto_decl: 
    LBRACE local_declaracoes statement_lista RBRACE
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = NuloDecl;
        t->child[0] = $2;  // local declarations
        t->child[1] = $3;  // statements
        $$ = t;
    }
;

local_declaracoes: 
    local_declaracoes var_declaracao 
    {
        TreeNode* t = $1;
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = $2;
            $$ = $1;
        }
        else $$ = $2;
    }
    | /* vazio */ { $$ = NULL; }
;

statement_lista: 
    statement_lista statement 
    {
        TreeNode* t = $1;
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = $2;
            $$ = $1;
        }
        else $$ = $2;
    }
    | /* vazio */ { $$ = NULL; }
    | error SEMICOL
        {
            yyerrok;
            printf("Erro recuperado. Continuando...\n");
            $$ = NULL;
        }
;

statement: 
    expressao_decl { $$ = $1; }
    | composto_decl { $$ = $1; }
    | selecao_decl { $$ = $1; }
    | iteracao_decl { $$ = $1; }
    | retorno_decl { $$ = $1; }
;

expressao_decl: 
    expressao SEMICOL { $$ = $1; }
    | SEMICOL { $$ = NULL; }
;

selecao_decl: 
    IF LPAREN expressao RPAREN statement %prec LOWER_THAN_ELSE 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = IfK;
        t->child[0] = $3;  /* condition */
        t->child[1] = $5;  /* if-part */
        t->child[2] = NULL; /* else-part is null */
        $$ = t;
    }
    | IF LPAREN expressao RPAREN statement ELSE statement 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = IfK;
        t->child[0] = $3;  /* condition */
        t->child[1] = $5;  /* if-part */
        t->child[2] = $7;  /* else-part */
        $$ = t;
    }
;

iteracao_decl: 
    WHILE LPAREN expressao RPAREN statement 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = WhileK;
        t->child[0] = $3;  /* condition */
        t->child[1] = $5;  /* body */
        $$ = t;
    }
;

retorno_decl: 
    RETURN SEMICOL 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = ReturnVOID;
        $$ = t;
    }
    | RETURN expressao SEMICOL 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = ReturnINT;
        t->child[0] = $2;  /* return value */
        $$ = t;
    }
;

expressao: 
    var ASSIGN expressao 
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = AssignK;
        t->child[0] = $1;
        t->child[1] = $3;
        $$ = t;
    }
    | simples_expressao { $$ = $1; }
;

var: 
    ID { $$ = $1; }
    | ID LBRACKET expressao RBRACKET 
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = VetorK;
        t->attr.name = $1->attr.name;
        t->child[0] = $3;  // index expression
        free($1);
        $$ = t;
    }
;

simples_expressao: 
    soma_expressao relacional soma_expressao 
    {
        TreeNode* t = $2;  // reuse relational operator node
        t->child[0] = $1;
        t->child[1] = $3;
        $$ = t;
    }
    | soma_expressao { $$ = $1; }
;

relacional: 
    SMAL { $$ = $1; }
    | 
    SMALEQ { $$ = $1; }
    | 
    GREAT { $$ = $1; }
    | 
    GREATEQ { $$ = $1; }
    | 
    EQ { $$ = $1; }
    | 
    DIFF { $$ = $1; }
;

soma_expressao: 
    soma_expressao soma termo 
    {
        TreeNode* t = $2;  // reuse operator node
        t->child[0] = $1;
        t->child[1] = $3;
        $$ = t;
    }
    | termo { $$ = $1; }
;

soma: 
    PLUS { $$ = $1; }
    | 
    MINUS { $$ = $1; }
;

termo: 
    termo mult fator 
    {
        TreeNode* t = $2;
        t->child[0] = $1;
        t->child[1] = $3;
        $$ = t;
    }
    | fator { $$ = $1; }
;

mult: 
    MULT { $$ = $1; }
    | 
    DIV { $$ = $1; } 
;

fator: 
    LPAREN expressao RPAREN { $$ = $2; }
    | var { $$ = $1; }
    | ativacao { $$ = $1; }
    | NUM { $$ = $1; }
    | MINUS fator %prec UMINUS 
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = OpK;
        t->child[0] = $2;
        $$ = t;
    }
;

ativacao: 
    ID LPAREN args RPAREN 
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = AtivK;
        t->attr.name = $1->attr.name;
        t->child[0] = $3;  // arguments
        free($1);
        $$ = t;
    }
;

args: 
    arg_lista { $$ = $1; }
    | /* vazio */ { $$ = NULL; }
;

arg_lista: 
    arg_lista COMMA expressao 
    {
        TreeNode* t = $1;
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = $3;
            $$ = $1;
        }
        else $$ = $3;
    }
    | expressao { $$ = $1; }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, ANSI_COLOR_GREEN "ERRO SINTÁTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", yytext);
    fprintf(stderr, ANSI_COLOR_GREEN "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET "\n", lineNum);
}

int formaEntrada(int argc, char **argv) {
    if (argc == 1) {
        yyin = stdin;
        yyout = stdout;
    } else if (argc == 2) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
            return 1;
        }
        yyout = stdout;
    } else if (argc == 3) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
            return 1;
        }
        yyout = fopen(argv[2], "w");
        if (yyout == NULL) {
            fprintf(stderr, "Error: Cannot open output file %s\n", argv[2]);
            fclose(yyin);
            return 1;
        }
    } else {
        fprintf(stderr, "Usage: %s [input_file] [output_file]\n", argv[0]);
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    printf("Starting program...\n");
    fflush(stdout);  // Force print

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    printf("Opening file: %s\n", argv[1]);
    fflush(stdout);

    // Check if formaEntrada is successful
    if (formaEntrada(argc, argv) != 0) {
        fprintf(stderr, "Error opening files\n");
        return 1;
    }

    // Verify file was opened successfully
    if (yyin == NULL) {
        fprintf(stderr, "Failed to open input file\n");
        return 1;
    }

    printf("File opened successfully\n");
    fflush(stdout);
    
    printf("Starting parse...\n");
    fflush(stdout);

    int parseResult = yyparse();
    
    printf("Parse completed with result: %d\n", parseResult);
    fflush(stdout);

    if (parseResult != 0) {
        fprintf(stderr, "Parsing failed!\n");
        if (yyin != stdin) fclose(yyin);
        if (yyout != stdout) fclose(yyout);
        return 1;
    }

    printf("Checking syntax tree...\n");
    fflush(stdout);

    if (savedTree != NULL) {        
        printf("\nBuilding symbol table...\n");
        buildSymTabFromTree(savedTree);
        
        printf("\nSymbol Table Contents:\n");
        mostraTabelaSimbolos(symbolTable);

        printf("\nSyntax tree created successfully!\n");
        printTree(savedTree);
        
    } else {
        fprintf(stderr, "Empty syntax tree!\n");
        if (yyin != stdin) fclose(yyin);
        if (yyout != stdout) fclose(yyout);
        return 1;
    }

    printf("Cleaning up...\n");
    fflush(stdout);

    if(savedTree != NULL)
        freeTree(savedTree);
        
    if(symbolTable != NULL)
        deleteSymTab();

    if (yyin != stdin) fclose(yyin);
    if (yyout != stdout) fclose(yyout);

    printf("Program completed successfully\n");
    fflush(stdout);
    return 0;
}
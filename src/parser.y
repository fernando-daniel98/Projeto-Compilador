%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/globals.h"
#include "../include/syntax_tree.h"

extern FILE *yyin;
extern FILE *yyout;
extern char *yytext;

extern int lineNum;
extern int inFunctionScope;

extern int yylex(void);
void yyerror(const char *s);

int syntax_errors = 0;
extern int lexical_errors;
int semantic_errors = 0;

static TreeNode *savedTree;

%}

%code requires {
    #include "../include/globals.h"
    #include "../include/syntax_tree.h"
}

%union {
    TreeNode *node;
    ExpType type;
}

%define parse.error verbose


%type <node> programa
%type <node> declaracao_lista declaracao
%type <node> var_declaracao fun_declaracao
%type <node> param_lista param
%type <node> composto_decl local_declaracoes statement_lista
%type <node> statement expressao_decl selecao_decl iteracao_decl
%type <node> retorno_decl expressao var
%type <node> simples_expressao soma_expressao termo fator
%type <node> ativacao args arg_lista params relacional soma mult

%type <type> tipo_especificador

%token <node> ID NUM
%token <node> IF ELSE WHILE RETURN
%token <node> PLUS MINUS MULT DIV
%token <node> SMAL SMALEQ GREAT GREATEQ EQ DIFF ASSIGN

%token INT VOID ERROR
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
        t->attr.name = $2->attr.name;
        t->type = $1;
        free($2);
        $$ = t;
    }
    | tipo_especificador error SEMICOL {
        yyerrok;
        fprintf(stderr, ANSI_COLOR_GREEN "RECUPERAÇÃO DE ERRO: " ANSI_COLOR_RESET "Ignorando declaração inválida\n");
        $$ = NULL;
    }
    | tipo_especificador ID LBRACKET NUM RBRACKET SEMICOL
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VetDeclK;
        t->attr.name = $2->attr.name;
        t->type = $1;
        t->child[0] = $4;
        free($2);
        $$ = t;
    }
    | tipo_especificador error RBRACKET SEMICOL {
        yyerrok;
        fprintf(stderr, ANSI_COLOR_GREEN "RECUPERAÇÃO DE ERRO: " ANSI_COLOR_RESET "Ignorando declaração de vetor inválida\n");
        $$ = NULL;
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
        TreeNode* first_param = $1;
        
        TreeNode* t = first_param;
        while (t->sibling != NULL) {
            t = t->sibling;
        }
        
        t->sibling = $3;
        
        $$ = first_param;
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
            t->child[0] = $2;
            t->child[1] = $3;
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
    | error SEMICOL { 
        yyerrok; 
        fprintf(stderr, ANSI_COLOR_GREEN "RECUPERAÇÃO DE ERRO: " ANSI_COLOR_RESET "Sincronizando em ';'\n"); 
        $$ = NULL; 
    }
;

selecao_decl: 
    IF LPAREN expressao RPAREN statement %prec LOWER_THAN_ELSE 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = IfK;
        t->child[0] = $3;
        t->child[1] = $5;
        t->child[2] = NULL;
        $$ = t;
    }
    | IF LPAREN expressao RPAREN statement ELSE statement 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = IfK;
        t->child[0] = $3;
        t->child[1] = $5;
        t->child[2] = $7;
        $$ = t;
    }
;

iteracao_decl: 
    WHILE LPAREN expressao RPAREN statement 
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = WhileK;
        t->child[0] = $3;
        t->child[1] = $5;
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
        t->child[0] = $2;
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
        t->child[0] = $3;
        free($1);
        $$ = t;
    }
;

simples_expressao: 
    soma_expressao relacional soma_expressao 
    {
        TreeNode* t = $2;
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
        TreeNode* t = $2;
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
        t->attr.op = MINUS_OP;
        t->child[0] = $2;
        t->child[1] = NULL;
        $$ = t;
    }
;

ativacao: 
    ID LPAREN args RPAREN 
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = AtivK;
        t->attr.name = $1->attr.name;
        t->child[0] = $3;
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
    fprintf(stderr, ANSI_COLOR_YELLOW "ERRO SINTÁTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", yytext);
    fprintf(stderr, ANSI_COLOR_YELLOW "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET " | %s\n", lineNum, s);
    
    syntax_errors++;
}

TreeNode *parse(void) {
    savedTree = NULL;
    
    int parseResult = yyparse();
    
    if(parseResult == 0){
        fprintf(yyout, "\nSyntax tree generation completed successfully\n");
        fflush(yyout);
    }
    else{
        fprintf(yyout, "\nUnable to generate syntax tree due to errors in the source program\n");
        fflush(yyout);
    }

    
    return savedTree;
}
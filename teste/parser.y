%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
extern FILE *yyout;
extern char* yytext;

// extern int lineNum;
extern int countErrorsLexer;

extern int yylex(void);
void yyerror(const char *s);
int countErrorsParser = 1;
%}

%token NUM ID IF ELSE INT RETURN VOID WHILE
%token PLUS MINUS MULT DIV SMAL SMALEQ GREAT GREATEQ EQ DIFF ASSIGN SEMICOL COMMA 
%token LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%left ASSIGN
%left EQ DIFF SMAL SMALEQ GREAT GREATEQ
%left PLUS MINUS
%left MULT DIV
%right UMINUS

%%

programa: declaracao_lista;

declaracao_lista: 
    declaracao_lista declaracao 
    | declaracao 
;

declaracao: 
    var_declaracao 
    | fun_declaracao 
;

var_declaracao: 
    tipo_especificador ID SEMICOL 
    | tipo_especificador ID LBRACKET NUM RBRACKET SEMICOL 
;

tipo_especificador: 
    INT 
    | VOID 
;

fun_declaracao: 
    tipo_especificador ID LPAREN params RPAREN composto_decl 
;

params: 
    param_lista 
    | VOID 
;

param_lista: 
    param_lista COMMA param 
    | param 
;

param: 
    tipo_especificador ID 
    | tipo_especificador ID LBRACKET RBRACKET 
;

composto_decl: 
    LBRACE local_declaracoes statement_lista RBRACE 
;

local_declaracoes: 
    local_declaracoes var_declaracao 
    | /* vazio */ 
;

statement_lista: 
    statement_lista statement 
    | /* vazio */ 
;

statement: 
    expressao_decl 
    | composto_decl 
    | selecao_decl 
    | iteracao_decl 
    | retorno_decl 
;

expressao_decl: 
    expressao SEMICOL 
    | SEMICOL 
;

selecao_decl: 
    IF LPAREN expressao RPAREN statement %prec LOWER_THAN_ELSE 
    | IF LPAREN expressao RPAREN statement ELSE statement 
;

iteracao_decl: 
    WHILE LPAREN expressao RPAREN statement 
;

retorno_decl: 
    RETURN SEMICOL 
    | RETURN expressao SEMICOL 
;

expressao: 
    var ASSIGN expressao 
    | simples_expressao 
;

var: 
    ID 
    | ID LBRACKET expressao RBRACKET 
;

simples_expressao: 
    soma_expressao relacional soma_expressao 
    | soma_expressao 
;

relacional: 
    SMAL | SMALEQ | GREAT | GREATEQ | EQ | DIFF 
;

soma_expressao: 
    soma_expressao soma termo 
    | termo 
;

soma: 
    PLUS | MINUS 
;

termo: 
    termo mult fator 
    | fator 
;

mult: 
    MULT | DIV 
;

fator: 
    LPAREN expressao RPAREN 
    | var 
    | ativacao 
    | NUM 
    | MINUS fator %prec UMINUS 
;

ativacao: 
    ID LPAREN args RPAREN 
;

args: 
    arg_lista 
    | /* vazio */ 
;

arg_lista: 
    arg_lista COMMA expressao 
    | expressao 
;

%%

void yyerror(const char *s) {
    printf( "ERRO SINTÁTICO: \"%s\" \n", yytext);
    /* printf( "LINHA: %d\n" , lineNum); */
    countErrorsParser++;
}

void formaEntrada(int argc, char **argv){
    if (argc == 1){
        yyin = stdin;
        yyout = stdout;
    } else if (argc == 2){
        yyin = fopen(argv[1], "r");
        yyout = stdout;
    } else if (argc == 3){
        yyin = fopen(argv[1], "r");
        yyout = fopen(argv[2], "w");
    } else {
        fprintf(stderr, "Uso: %s [arquivo_entrada] [arquivo_saida]\n", argv[0]);
        exit(1);
    }
}

int main(int argc, char **argv) {

    formaEntrada(argc, argv);

    yyparse();
    return 0;
}
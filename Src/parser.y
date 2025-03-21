%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char* yytext;       // Uncommented line
extern int yylex(void);    // Uncommented line
void yyerror(const char *s);

extern int lineNum = 1;

%}

%token NUM ID IF ELSE INT RETURN VOID WHILE ERROR
%token PLUS MINUS MULT DIV SMAL SMALEQ GREAT GREATEQ EQ DIFF ASSIGN SEMICOL COMMA 
%token LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%left ASSIGN
%left EQ DIFF SMAL SMALEQ GREAT GREATEQ
%left PLUS MINUS
%left MULT DIV

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
    printf("ERRO SINTÁTICO: \"%s\" ", yytext);
    printf("LINHA: %d\n", lineNum);
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}
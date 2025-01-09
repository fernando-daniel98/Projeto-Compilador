%{
#include "../../Include/globals.h"

int countErrorsParser = 1;

static int yylex(void);
void yyerror(char *s);
int yyparse(void);


%}

%union {
    int num;
    char *id;
}

%token <num> NUM
%token <id> ID
%token ENDOFFILE ERROR IF ELSE INT RETURN VOID WHILE
%token PLUS MINUS MULT DIV SMAL SMALEQ GREAT GREATEQ EQ DIFF ASSIGN SEMICOL COMMA LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE BEGCOMM CLOSCOMM

%left ELSE
%left PLUS MINUS
%left MULT DIV
%left SMAL SMALEQ GREAT GREATEQ EQ DIFF
%right ASSIGN

%%

/* Regras de gramática aqui */
programa: declaracao_lista
        ;

declaracao_lista: declaracao_lista declaracao
                | declaracao
                ;

declaracao: var_declaracao
            | fun_declaracao
            ;

var_declaracao: tipo_especificador ID SEMICOL
                | tipo_especificador ID LBRACKET NUM RBRACKET SEMICOL
                ;

tipo_especificador: INT
                    | VOID
                    ;

fun_declaracao: tipo_especificador ID LPAREN params RPAREN composto_decl
                ;

params: param_lista
        | VOID
        ;

param_lista: param_lista COMMA param
            | param
            ;

param: tipo_especificador ID
        | tipo_especificador ID LBRACKET RBRACKET
        ;

composto_decl: LBRACE local_declaracoes statement_lista RBRACE
                ;

local_declaracoes: local_declaracoes var_declaracao
                | vazio
                ;

statement_lista: statement_lista statement
                | vazio
                ;

statement: expressao_decl
            | composto_decl
            | selecao_decl
            | iteracao_decl
            | retorno_decl
            ;

expressao_decl: expressao SEMICOL
                | SEMICOL
                ;

selecao_decl: IF LPAREN expressao RPAREN statement %prec ELSE
            | IF LPAREN expressao RPAREN statement ELSE statement
            ;

iteracao_decl: WHILE LPAREN expressao RPAREN statement
                ;

retorno_decl: RETURN SEMICOL
            | RETURN expressao SEMICOL
            ;

expressao: var ASSIGN expressao
            | simples_expressao
            ;

var: ID
    | ID LBRACKET expressao RBRACKET
    ;

simples_expressao: soma_expressao relacional soma_expressao
                | soma_expressao
                ;

relacional: SMAL
            | SMALEQ
            | GREAT
            | GREATEQ
            | EQ
            | DIFF
            ;

soma_expressao: soma_expressao soma termo
                | termo
                ;

soma: PLUS
    | MINUS
    ;

termo: termo mult fator
        | fator
        ;

mult: MULT
    | DIV
    ;

fator: LPAREN expressao RPAREN
        | var
        | ativacao
        | NUM
        ;

ativacao: ID LPAREN args RPAREN
        ;

args: arg_lista
    | vazio
    ;

arg_lista: arg_lista COMMA expressao
        | expressao
        ;

vazio: /* vazio */


%%

void yyerror(char *s) {
    printf(ANSI_COLOR_GREEN "ERRO SINTÁTICO: " ANSI_COLOR_RESET "\"%s\" ", yytext);
    printf(ANSI_COLOR_RED "LINHA: %d.\n" ANSI_COLOR_RESET, lineNum);
    countErrorsParser++;
}


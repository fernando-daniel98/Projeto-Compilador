%{
#include "../../Include/globals.h"

int countErrorsParser = 1;

static int yylex(void);
void yyerror(char *s);
int yyparse(void);


%}

%token NUM
%token ID
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

declaracao_lista: declaracao_lista declaracao { $$ = $1; }
                | declaracao { $$ = $1; }
                ;

declaracao: var_declaracao { $$ = $1; }
            | fun_declaracao { $$ = $1; }
            ;

var_declaracao: tipo_especificador ID SEMICOL { $$ = $1; }
                | tipo_especificador ID LBRACKET NUM RBRACKET SEMICOL { $$ = $1; }
                ;

tipo_especificador: INT
                    | VOID
                    ;

fun_declaracao: tipo_especificador ID LPAREN params RPAREN composto_decl { $$ = $1; }
                ;

params: param_lista { $$ = $1; }
        | VOID { $$ = $1; }
        ;

param_lista: param_lista COMMA param {
                if ($$ != NULL) {
                    $$ = $1;
                }
                else {
                    $$ = $3;
                }
            }
            | param { $$ = $1; }
            ;

param: tipo_especificador ID { $$ = $1; }
        | tipo_especificador ID LBRACKET RBRACKET { $$ = $1; }
        ;

composto_decl: LBRACE local_declaracoes statement_lista RBRACE {
                if ($$ != NULL) {
                    $$ = $2;
                }
                else {
                    $$ = $3;
                }
            }
                ;

local_declaracoes: local_declaracoes var_declaracao{
                if ($1 != NULL) {
                    $$ = $1;
                }
                else {
                    $$ = $2;
                }
            }
                | vazio { $$ = $1; }
                ;

statement_lista: statement_lista statement {
                if ($1 != NULL) {
                    $$ = $1;
                }
                else {
                    $$ = $2;
                }
            }
                | vazio { $$ = $1; }
                ;

statement: expressao_decl { $$ = $1; }
            | composto_decl { $$ = $1; }
            | selecao_decl { $$ = $1; }
            | iteracao_decl { $$ = $1; }
            | retorno_decl { $$ = $1; }
            ;

expressao_decl: expressao SEMICOL { $$ = $1; }
                | SEMICOL { $$ = $1; }
                ;

selecao_decl: IF LPAREN expressao RPAREN statement %prec ELSE {
                if ($$ != NULL) {
                    $$ = $4;
                }
                else {
                    $$ = $5;
                }
        }
            | IF LPAREN expressao RPAREN statement ELSE statement {
                if ($$ != NULL) {
                    $$ = $4;
                }
                else {
                    $$ = $6;
                }
            }
            ;

iteracao_decl: WHILE LPAREN expressao RPAREN statement {
                if ($$ != NULL) {
                    $$ = $4;
                }
                else {
                    $$ = $5;
                }
                }
                ;

retorno_decl: RETURN SEMICOL { $$ = $1; }
            | RETURN expressao SEMICOL
            ;

expressao: var ASSIGN expressao { $$ = $1; }
            | simples_expressao { $$ = $1; }
            ;

var: ID { $$ = $1; }
    | ID LBRACKET expressao RBRACKET {
        if ($$ != NULL) {
            $$ = $1;
        }
        else {
            $$ = $3;
        }
    }
    ;

simples_expressao: soma_expressao relacional soma_expressao {
                if ($$ != NULL) {
                    $$ = $1;
                }
                else {
                    $$ = $3;
                }
            }
                | soma_expressao { $$ = $1; }
                ;

relacional: SMAL
            | SMALEQ
            | GREAT
            | GREATEQ
            | EQ
            | DIFF
            ;

soma_expressao: soma_expressao soma termo {
                $$ = $2;
}
                | termo { $$ = $1; }
                ;

soma: PLUS { $$ = $1; }
    | MINUS { $$ = $1; }
    ;

termo: termo mult fator {
                if ($$ != NULL) {
                    $$ = $2;
                }
                else {
                    $$ = $3;
                }
            }
        | fator { $$ = $1; }
        ;

mult: MULT { $$ = $1; }
    | DIV { $$ = $1; }
    ;

fator: LPAREN expressao RPAREN { $$ = $2; }
        | var { $$ = $1; }
        | ativacao { $$ = $1; }
        | NUM { $$ = $1; }
        ;

ativacao: ID LPAREN args RPAREN { $$ = $1; }
        ;

args: arg_lista { $$ = $1; }
    | vazio { $$ = $1; }
    ;

arg_lista: arg_lista COMMA expressao { $$ = $1; }
        | expressao { $$ = $1; }
        ;

vazio: /* vazio */ { $$ = NULL; }
        ;


%%

void yyerror(char *s) {
    printf(ANSI_COLOR_GREEN "ERRO SINTÁTICO: " ANSI_COLOR_RESET "\"%s\" ", yytext);
    printf(ANSI_COLOR_RED "LINHA: %d.\n" ANSI_COLOR_RESET, lineNum);
    countErrorsParser++;
}


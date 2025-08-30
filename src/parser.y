%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"

#define YYSTYPE PONTEIRONO
#define MAX_NOS 10000

static int yylex(void);
void yyerror(char* s);
int yyparse(void);

enum yytokentype auxErro;

int syntax_errors = 0;

//No raiz da arvore sintatica
PONTEIRONO arvoreSintatica;


void mostraArvore(PONTEIRONO raiz, int num);
enum yytokentype getToken(void);
PONTEIRONO parse(void);

char auxLexema[MAXLEXEMA];
PONTEIRONO nos[MAX_NOS];
int qntNos = 0;

%}
/*
Declaracao dos tokens que serao utilizados durante o processo de analise
sintatica. 
*/

%token NUM SOMA SUB MULT DIV INT
%token ID VOID WHILE ELSE IF ABREPARENTESES FECHAPARENTESES
%token RETURN COMMA ABRECHAVES FECHACHAVES SEMICOLON
%token ATRIB ABRECOLCHETES FECHACOLCHETES
%token EQ NEQ LT LET GT GET ERRO

/* Precedências para resolver o conflito shift/reduce do "dangling else" */
%nonassoc IFX
%nonassoc ELSE


%%

programa			: declaracao_lista {
                        arvoreSintatica = $1;
                    }

                    ;
            
declaracao_lista	: declaracao_lista declaracao {  
                        if($1 != NULL){
                            $$ = $1;
                            adicionaIrmao($$, $2);
                        }
                        else{
                            $$ = $2;
                        }
                      }
                    | declaracao {$$ = $1;}
                    ;

declaracao			: var_declaracao {$$ = $1;}
                    | fun_declaracao {$$ = $1;}				
                    ;

var_declaracao		: tipo_especificador ID SEMICOLON {
                        $$ = $1;
                        $$->tipo = DECLARACAO;
                        $$->tipoDeclaracao = VarDeclK;
                        $$->numLinha = lineNum;
                    
                        PONTEIRONO aux = novoNo();

                        strcpy(aux->lexema, pilha[indPilha]);
                        indPilha--;

                        //strcpy(aux->lexema, id);
                        adicionaFilho($$, aux);

                        nos[qntNos] = aux;
                        qntNos++;

                        }
                    | tipo_especificador error SEMICOLON    {
                        yyerrok;
                        printf(ANSI_COLOR_GREEN "RECUPERAÇÃO DE ERRO: " ANSI_COLOR_RESET "Ignorando declaração inválida\n");
                        $$ = NULL;
                    }
                    | tipo_especificador ID ABRECOLCHETES NUM FECHACOLCHETES SEMICOLON {
                        $$ = $1;
                        $$->tipo = DECLARACAO;
                        $$->tipoDeclaracao = VetDeclK;
                        $$->numLinha = lineNum;

                        PONTEIRONO aux = novoNo();
                        PONTEIRONO aux2 = novoNo();
                        
                        strcpy(aux->lexema, pilha[indPilha]);
                        indPilha--;

                        nos[qntNos] = aux;
                        qntNos++;

                        strcpy(aux2->lexema, pilha[indPilha]);
                        indPilha--;
                        
                        adicionaFilho($$, aux2);
                        adicionaFilho($$, aux);

                        nos[qntNos] = aux2;
                        qntNos++;
                    }
                    | tipo_especificador error FECHACOLCHETES SEMICOLON {
                        yyerrok;
                        printf(ANSI_COLOR_GREEN "RECUPERAÇÃO DE ERRO: " ANSI_COLOR_RESET "Ignorando declaração inválida\n");
                        $$ = NULL;
                    }
                    ;

tipo_especificador 	: INT {
                        $$ = novoNo();
                        strcpy($$->lexema, "INT");
                        $$->numLinha = lineNum;

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    | VOID {
                        $$ = novoNo();
                        strcpy($$->lexema, "VOID");
                        $$->numLinha = lineNum;

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    ;
                    
                    /* Trocar o ID da funcao por um IDFUNC */
fun_declaracao		: tipo_especificador fun_id ABREPARENTESES params FECHAPARENTESES composto_decl { 
                        //strcpy(auxLexema, "")
                        $$ = $1;

                        adicionaFilho($$, $4);
                        adicionaFilho($$, $2);
                        adicionaFilho($2, $6);
                        
                        $$->tipo = DECLARACAO;
                        $$->tipoDeclaracao = FunDeclK;
                    }
                    ;

fun_id				: ID {
                        $$ = novoNo();
                        
                        strcpy($$->lexema, pilha[indPilha]);
                        indPilha--;

                        //strcpy($$->lexema, auxNome);
                        $$->numLinha = lineNum;

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    ;	

params				: param_lista {$$ = $1;}
                    | VOID {
                        $$ = novoNo();
                        $$->tipo = DECLARACAO;
                        $$->numLinha = lineNum;
                        $$->tipoDeclaracao = ParamVoid;
                        strcpy($$->lexema, "VOID");

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    ;

param_lista			: param_lista COMMA param {
                        if($1 != NULL){
                            $$ = $1;
                            adicionaIrmao($$, $3);
                        }
                        else{
                            $$ = $3;
                        }						
                    }
                    | param {$$ = $1;}
                    ;

param				: tipo_especificador ID {
                        $$ = $1;
                        $$->tipo = DECLARACAO;
                        $$->numLinha = lineNum;
                        $$->tipoDeclaracao = VarParamK;

                        PONTEIRONO aux = novoNo();

                        strcpy(aux->lexema, pilha[indPilha]);
                        indPilha--;
                    
                        //strcpy(aux->lexema, id);
                        adicionaFilho($$, aux);

                        nos[qntNos] = aux;
                        qntNos++;

                    }
                    | tipo_especificador ID ABRECOLCHETES FECHACOLCHETES {
                        $$ = $1;
                        $$->tipo = DECLARACAO;
                        $$->numLinha = lineNum;
                        $$->tipoDeclaracao = VetParamK;
                        PONTEIRONO aux = novoNo();

    
                        strcpy(aux->lexema, pilha[indPilha]);
                        indPilha--;

                        //strcpy(aux->lexema, id);
                        adicionaFilho($$, aux);		

                        nos[qntNos] = aux;
                        qntNos++;	
                    }
                    ;

composto_decl		: ABRECHAVES local_declaracoes statement_lista FECHACHAVES { 
                        if($2 != NULL){
                            $$ = $2;
                            adicionaIrmao($$, $3);
                        }
                        else{
                            $$ = $3;
                        }
                    }
                    ;

local_declaracoes 	: local_declaracoes var_declaracao {
                        if($1 != NULL){
                            $$ = $1;
                            adicionaIrmao($$, $2);
                        }
                        else{
                            $$ = $2;
                        }

                    }
                    | %empty {$$ = NULL;}
                    ;

statement_lista 	: statement_lista statement {
                        if($1 != NULL){
                            $$ = $1;
                            adicionaIrmao($$, $2);
                        }
                        else{
                            $$ = $2;
                        }
                    }
                    | %empty {$$ = NULL;}
                    ;
            
statement			: expressao_decl {$$ = $1;}
                    | composto_decl {$$ = $1;}
                    | selecao_decl {$$ = $1;}
                    | iteracao_decl {$$ = $1;}
                    | retorno_decl {$$ = $1;}
                    ;

expressao_decl		: expressao SEMICOLON {$$ = $1;}
                    | SEMICOLON {$$ = NULL;}
                    | error SEMICOLON {
                        yyerrok;
                        printf(ANSI_COLOR_GREEN "RECUPERAÇÃO DE ERRO: " ANSI_COLOR_RESET "Sincronizando em ';'\n");
                        $$ = NULL;
                    }
                    ;
            
selecao_decl		: IF ABREPARENTESES expressao FECHAPARENTESES statement %prec IFX {
                        $$ = novoNo();
                        strcpy($$->lexema, "IF");
                        $$->tipo = DECLARACAO;
                        $$->numLinha = lineNum;
                        $$->tipoDeclaracao = IfK;

                        adicionaFilho($$, $3);
                        adicionaFilho($$, $5);		

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    | IF ABREPARENTESES expressao FECHAPARENTESES statement ELSE statement {
                        $$ = novoNo();
                        strcpy($$->lexema, "IF");
                        $$->tipo = DECLARACAO;
                        $$->numLinha = lineNum;
                        $$->tipoDeclaracao = IfK;

                        adicionaFilho($$, $3);
                        adicionaFilho($$, $5);		
                        adicionaFilho($$, $7);

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    ;
            
iteracao_decl		: WHILE ABREPARENTESES expressao FECHAPARENTESES statement {
                        $$ = novoNo();
                        strcpy($$->lexema, "WHILE");
                        $$->tipo = DECLARACAO;
                        $$->numLinha = lineNum;
                        $$->tipoDeclaracao = WhileK;

                        adicionaFilho($$, $3);
                        adicionaFilho($$, $5);

                        nos[qntNos] = $$;
                        qntNos++;

                    }
                    ;
            
retorno_decl		: RETURN SEMICOLON { 
                        $$ = novoNo();
                        $$->tipo = DECLARACAO;
                        $$->numLinha = lineNum;
                        $$->tipoDeclaracao = ReturnVOID;
                        strcpy($$->lexema, "ReturnVOID");

                        nos[qntNos] = $$;
                        qntNos++;
                        
                    }
                    | RETURN expressao SEMICOLON {
                        $$ = novoNo();
                        $$->tipo = DECLARACAO;
                        $$->numLinha = lineNum;
                        $$->tipoDeclaracao = ReturnINT;
                        strcpy($$->lexema, "ReturnINT");

                        adicionaFilho($$, $2);

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    ;

expressao			: var ATRIB expressao {
                        $$ = novoNo();
                        strcpy($$->lexema, "=");
                        $$->tipo = EXPRESSAO;
                        $$->numLinha = lineNum;
                        $$->tipoExpressao = AssignK;

                        adicionaFilho($$, $1);
                        adicionaFilho($$, $3);

                        nos[qntNos] = $$;
                        qntNos++;

                    }
                    | simples_expressao {$$ = $1;}
                    ;

var 				: ID {
                        $$ = novoNo();
                        $$->tipo = EXPRESSAO;
                        $$->numLinha = lineNum;
                        $$->tipoExpressao = IdK;
                        

                        strcpy($$->lexema, pilha[indPilha]);
                        /*
                        FILE * arquivoAux = fopen("arquivoAux.txt", "a+");
                        fprintf(arquivoAux, "%s\n", $$->lexema);
                        fclose(arquivoAux);
                        */
                        indPilha--;
                        //strcpy($$->lexema, id);

                        nos[qntNos] = $$;
                        qntNos++;

                    }
                    | ID ABRECOLCHETES expressao FECHACOLCHETES{
                        $$ = novoNo();

                        $$->tipo = EXPRESSAO;
                        $$->numLinha = lineNum;
                        $$->tipoExpressao = VetorK;
                        
                        //strcpy($$->lexema, id);
                        strcpy($$->lexema, pilha[indPilha]);
                        indPilha--;

                        adicionaFilho($$, $3);

                        nos[qntNos] = $$;
                        qntNos++;
                        
                    }
                    ;
            
simples_expressao	: soma_expressao relacional soma_expressao {
                        $$ = $2;
                        $$->tipo = EXPRESSAO;
                        $$->numLinha = lineNum;
                        $$->tipoExpressao = OpRel;

                        adicionaFilho($$, $1);
                        adicionaFilho($$, $3);	
                        
                    }
                    | soma_expressao {$$ = $1;}
                    ;
        
relacional			: operador_relacional {
                        $$ = $1;
                    }
                    ;

operador_relacional	: EQ {
                        $$ = novoNo();
                        strcpy($$->lexema, "==");

                        nos[qntNos] = $$;
                        qntNos++;
                        
                    }

                    | NEQ {
                        $$ = novoNo();
                        strcpy($$->lexema, "!=");

                        nos[qntNos] = $$;
                        qntNos++;
                        
                    }

                    | LT {
                        $$ = novoNo();
                        strcpy($$->lexema, "<");

                        nos[qntNos] = $$;
                        qntNos++;
                        
                    }
                    
                    | GT {
                        $$ = novoNo();
                        strcpy($$->lexema, ">");

                        nos[qntNos] = $$;
                        qntNos++;
                        
                    }
                    
                    | LET {
                        $$ = novoNo();
                        strcpy($$->lexema, "<=");

                        nos[qntNos] = $$;
                        qntNos++;
                        
                    }

                    | GET {
                        $$ = novoNo();
                        strcpy($$->lexema, ">=");

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    ;


soma_expressao		: soma_expressao soma termo {
                        $$ = $2;
                        $$->tipo = EXPRESSAO;
                        $$->numLinha = lineNum;
                        $$->tipoExpressao = OpK;

                        adicionaFilho($$, $1);
                        adicionaFilho($$, $3);
                    }
                    | termo {$$ = $1;}
                    ;
            
soma				: SOMA {
                        $$ = novoNo();
                        strcpy($$->lexema, "+");

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    | SUB {
                        $$ = novoNo();
                        strcpy($$->lexema, "-");

                        nos[qntNos] = $$;
                        qntNos++;
                    } 
                    ;
            
termo				: termo mult fator {
                        $$ = $2;
                        $$->tipo = EXPRESSAO;
                        $$->numLinha = lineNum;
                        $$->tipoExpressao = OpK;

                        adicionaFilho($$, $1);
                        adicionaFilho($$, $3);
                        
                    }
                    | fator {$$ = $1;}
                    ;
            
mult				: MULT {
                        $$ = novoNo();
                        strcpy($$->lexema, "*");

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    | DIV {
                        $$ = novoNo();
                        strcpy($$->lexema, "/");

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    ;
            
fator				: ABREPARENTESES expressao FECHAPARENTESES  {$$ = $2;}
                    | var {$$ = $1;}
                    | ativacao {$$ = $1;}
                    | NUM { 
                        $$ = novoNo();
                        $$->tipo = EXPRESSAO;
                        $$->numLinha = lineNum;
                        $$->tipoExpressao = ConstK;

                        strcpy($$->lexema, pilha[indPilha]);
                        indPilha--;

                        //strcpy($$->lexema, auxNome);

                        nos[qntNos] = $$;
                        qntNos++;
                    }
                    ;

ativacao 			: fun_id ABREPARENTESES args FECHAPARENTESES {
                        $$ = $1;
                        $$->tipo = EXPRESSAO;
                        $$->numLinha = lineNum;
                        $$->tipoExpressao = AtivK;
                        adicionaFilho($$, $3);
                    }
                    ;
            
args 				: arg_lista {$$ = $1;}
                    | %empty {$$ = NULL;}
                    ;
            
arg_lista			: arg_lista COMMA expressao {
                        if($1 != NULL){
                            $$ = $1;
                            adicionaIrmao($$, $3);
                        } else $$ = $3;
                    }
                    | expressao {
                        $$ = $1;
                    }
                    ;			


%%

//Funcao para mostrar o erro sintatico do codigo
void yyerror(char *s) {
    printf(ANSI_COLOR_YELLOW "ERRO SINTÁTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", yytext);
    printf(ANSI_COLOR_YELLOW "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET " | %s\n", lineNum, s);
    syntax_errors++;
}

int yylex(void)
{ return (auxErro = getToken()); }

PONTEIRONO parse(void)
{ 
    yyparse();
    return arvoreSintatica;
}


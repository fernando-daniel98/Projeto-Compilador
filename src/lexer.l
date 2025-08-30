%{

#include <string.h>
#include "parser.h"
#include "globals.h"
#include "tab.h"

int lineNum;
char stringAux[500];
char auxNome[MAXLEXEMA];
char id[MAXLEXEMA];
int firstTime = 1;

int lexical_errors = 0;

int tabelaNomes(enum yytokentype *token);
void copiar(FILE* velho);
void mostrarTela(char palavra[]);


int indPilha = -1;
char pilha[4][MAXLEXEMA];

%}

%option noyywrap

DIGIT       [0-9]
NUMBER      {DIGIT}+
LETRA       [a-zA-Z]
IDENT       {LETRA}({LETRA}|{DIGIT})*
NEWLINE     \n
WHITESPACE  [ \t]+

%%

{WHITESPACE} {/*Ignora as entradas com espaços e tabulacoes*/}

"/*"		{
          char c, aux = 'x'; 
          do{ 
            c = input();
            if (c == EOF) break;

            if (c == '\n') {
                lineNum++;
            }
            if (c == '*'){
                aux = input();
            }
          } while (aux != '/');
        }

{NEWLINE} {
            lineNum++;
          }
   
   
{NUMBER} {
            if(strlen(yytext) >= MAXLEXEMA){
                printf(ANSI_COLOR_PURPLE "WARNING: Digito muito grande - " ANSI_COLOR_RESET ); 
                yytext[MAXLEXEMA-1] = '\0';
                printf("valor reduzido para apenas %s\n", yytext);
            }
            
            //strcpy(auxNome, yytext);
            
            indPilha++;
            strcpy(pilha[indPilha], yytext);

              return NUM; 
         }

{IDENT} {
            enum yytokentype token;
            if((tabelaNomes(&token)) == 0){
                token = ID;
            }
            
            /* Caso o ID possua um tamanho maior do que o trabalhado,
            seu conteudo sera cortado para poder ser utilizado na linguagem */
            if(strlen(yytext) >= MAXLEXEMA){
                printf(ANSI_COLOR_PURPLE "WARNING: ID muito grande - " ANSI_COLOR_RESET ); 
                yytext[MAXLEXEMA-1] = '\0';
                printf("nome reduzido para '%s'\n", yytext);
            }

            if(token == ID){
                indPilha++;
                bzero(pilha[indPilha], MAXLEXEMA);
                strcpy(pilha[indPilha], yytext);
            }
            return token;
         }
        
"(" {
    return ABREPARENTESES;
   }

")" {
    return FECHAPARENTESES;
   } 

"[" {
    return ABRECOLCHETES;
   }	      
          
          
"]" {
    return FECHACOLCHETES;
   }	      

          
"{" {
    return ABRECHAVES;
   }
          
"}" {
    return FECHACHAVES;	
   }

          
"=" {
    return ATRIB;
  }	
    
"," {
    return COMMA;
  }	
  
";" {
    return SEMICOLON;
  }
  
"+" {
    return SOMA;
   }	
   
"-" {
    return SUB;
   }  
   
"*" {
    return MULT;
   }	
   
"/" {
    return DIV;
   } 

"==" {
    return EQ;
   }

"!=" {
    return NEQ;
    }

"<" {
    return LT;
  }

">" {
    return GT;
  }

"<=" {
        return LET;
      }

">=" {	
        return GET;
      }
    
<<EOF>> {
        return 0;
    }

":qw" {if(arquivoEntrada == stdin) return 0;}

. {
    printf(ANSI_COLOR_RED "\nERRO LÉXICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", yytext);
    printf(ANSI_COLOR_RED "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET "\n", lineNum);
    lexical_errors++;
    return ERRO;
  }

%%

//Funcao que verifica se o id eh uma palavra reservada
int tabelaNomes(enum yytokentype *token){
    char aux[20];
    int flag = 0;
    
    if(strcmp(yytext, "if") == 0){
        strcpy(aux, "if");
        *token = IF;
        flag =  1;
    }
    else if(strcmp(yytext, "int") == 0){
        strcpy(aux, "int");
        *token = INT;
        flag =  1;
    }
    else if(strcmp(yytext, "else") == 0){
        strcpy(aux, "else");
        *token = ELSE;
        flag =  1;
    }
    else if(strcmp(yytext, "return") == 0){
        strcpy(aux, "return");
        *token = RETURN;
        flag =  1;
    }
    else if(strcmp(yytext, "void") == 0){
        strcpy(aux, "void");
        *token = VOID;
        flag =  1;
    }
    else if(strcmp(yytext, "while") == 0){
        strcpy(aux, "while");
        *token = WHILE;
        flag =  1;
    }
    return flag;
}

//Mostra a linha inteira na tela
void mostrarTela(char palavra[]){
    int i = 0;
    char ch;
    
    fprintf(arquivoSaida, "%d: ", lineNum);
    ch = palavra[i];
    while(ch == '\t' || ch == ' '){
        i++;
        ch = palavra[i];
    }
    for (; i < strlen(palavra); i++){
        ch = palavra[i];
        if(ch != '\n') fprintf(arquivoSaida, "%c", palavra[i]);
    }
    fprintf(arquivoSaida, "\n");
}

//Funcao que copia o arquivo de entrada para um arquivo de saida extra
void copiar(FILE* velho){
    FILE *novo;
    novo=fopen("copia.txt","w");
    if (novo==NULL){
        printf("Erro de abertura\n");
        exit(1);
    }

    char letra;
    while((letra = fgetc(velho)) != EOF) fprintf(novo, "%c", letra);

    fclose(novo);
    return;
}

//Funcao que retorna o proximo token
enum yytokentype getToken(void)
{ 	
    if(firstTime == 1){
        firstTime = 0;
        yyin = arquivoEntrada;

        if(arquivoEntrada == stdin){
            fprintf(arquivoSaida, "Para sair, digite :qw e aperte Enter\n");
            fprintf(arquivoSaida, "Digite o codigo fonte:\n\n");
            copiaArquivo = NULL;
        }
        else{
            copiar(arquivoEntrada);
            rewind(arquivoEntrada);
            copiaArquivo = fopen("copia.txt", "r");
        }

        // Inicializa pilha de ID
        for (int i = 0; i < 4; i++){
            bzero(pilha[i], MAXLEXEMA);	
        }
    }

    enum yytokentype currentToken;

    currentToken = yylex();
    if(currentToken == ID) strcpy(id, yytext);
    return currentToken;
}


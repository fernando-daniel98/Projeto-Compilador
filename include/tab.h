#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include "globals.h"

// Estrutura para a tabela de símbolos
typedef struct noIdentificador{
    char nomeIdentificador[MAXTOKENLEN];
    StatementKind tipoIdentificador;
    char escopo[MAXTOKENLEN];
    ExpType tipoDado;
    int linhas[MAXNOLIN];
    struct noIdentificador *prox, *ant;
} noIdentificador;
    
typedef noIdentificador *PnoIdentificador;

// Protótipos das funções relacionadas à tabela de símbolos
PnoIdentificador* inicializaTabela(void);
void initSymbolTable(void);
void deleteSymTab(void);
void adicionaIdentificarTabela(PnoIdentificador *tabelaHash, char *nomeIdentificador, StatementKind tipoIdenficador, char *escopo, ExpType tipoDado, int linha);
PnoIdentificador removeIdentificadorTabela(PnoIdentificador *tabelaHash, char *nomeIdentificador);
PnoIdentificador buscaIdentificadorTabela(PnoIdentificador tabelaHash[], char *nomeIdentificador, char *escopo);
void liberandoTabelaSimbolos(PnoIdentificador *tabelaHash);
const char* getStatementKindName(StatementKind kind);
const char* getExpTypeName(ExpType type);
void mostraTabelaSimbolos(PnoIdentificador *tabelaHash);
int verifyRedeclaration(char *nome, char *escopo);

// Variáveis externas
extern PnoIdentificador* symbolTable;
extern char currentScope[MAXTOKENLEN];
extern int inFunctionScope;

#endif
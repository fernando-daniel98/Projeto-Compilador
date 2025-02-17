#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"

#define MAXTAMTABELA 17
#define SHIFT 4

PnoIdentificador* symbolTable = NULL;
char currentScope[MAXNOLIN];

int hash(char * key){
    unsigned long int temp = 0;
    unsigned int i = 0;

    while (key[i] != '\0'){
        temp = ((temp << SHIFT) + key[i++]) % MAXTAMTABELA;
            // O operador << faz deslocamentos de bits para a esquerda no valor de temp (4 bits).
            // Este recurso está sendo usado para evitar sobrecarga em temp.
    }

    return temp;
}

PnoIdentificador* inicializaTabela(){
    PnoIdentificador* init = (PnoIdentificador *) malloc(sizeof(PnoIdentificador *)*MAXTAMTABELA);

    for(int i =0; i < MAXTAMTABELA; i++){
        init[i] = NULL;}

    return init;
}

void adicionaLinhaIdentificador(PnoIdentificador item, int linha){
    if (item == NULL) return;
    
    // Procura a primeira posição vazia no vetor de linhas
    int i = 0;
    while(i < MAXNOLIN && item->linhas[i] != 0){
        i++;
    }
    
    // Se ainda houver espaço, adiciona a linha
    if(i < MAXNOLIN){
        item->linhas[i] = linha;
    }
}

void adicionaIdentificarTabela(PnoIdentificador *tabelaHash, char *nomeIdentificador, StatementKind tipoIdenficador, char *escopo, ExpType tipoDado, int linha){
    int posicao = hash(nomeIdentificador);
    
    // Caso a posicao da tabela esteja vazia, ou seja o identificador ainda não foi inserido
    if(tabelaHash[posicao] == NULL){
        tabelaHash[posicao] = (PnoIdentificador) malloc(sizeof(struct noIdentificador));
        tabelaHash[posicao]->prox = NULL;
        tabelaHash[posicao]->ant = NULL;
        tabelaHash[posicao]->tipoIdentificador = tipoIdenficador;
        tabelaHash[posicao]->tipoDado = tipoDado;
        adicionaLinhaIdentificador(tabelaHash[posicao], linha);
        strcpy(tabelaHash[posicao]->nomeIdentificador, nomeIdentificador);
        strcpy(tabelaHash[posicao]->escopo, escopo);
    }
    else {
        // Caso a posicao da tabela já esteja ocupada, procura se o identificador já existe
        PnoIdentificador aux = tabelaHash[posicao];
        
        // Procura se já existe um identificador com mesmo nome e escopo
        while(aux != NULL) {
            if(strcmp(aux->nomeIdentificador, nomeIdentificador) == 0 && 
               strcmp(aux->escopo, escopo) == 0) {
                // Se encontrou, apenas adiciona a nova linha
                adicionaLinhaIdentificador(aux, linha);
                return;
            }
            aux = aux->prox;
        }

        // Se não encontrou, cria um novo nó
        aux = tabelaHash[posicao];
        while(aux->prox != NULL) {
            aux = aux->prox;
        }

        aux->prox = (PnoIdentificador) malloc(sizeof(struct noIdentificador));
        aux->prox->ant = aux;
        aux->prox->prox = NULL;
        aux->prox->tipoIdentificador = tipoIdenficador;
        aux->prox->tipoDado = tipoDado;
        adicionaLinhaIdentificador(aux->prox, linha);
        strcpy(aux->prox->nomeIdentificador, nomeIdentificador);
        strcpy(aux->prox->escopo, escopo);
    }
}

PnoIdentificador removeIdentificadorTabela(PnoIdentificador *tabelaHash, char *nomeIdentificador){
    int posicao = hash(nomeIdentificador);

    if(tabelaHash[posicao] == NULL){
        return NULL;
    }
    else{
        PnoIdentificador aux = tabelaHash[posicao];
        PnoIdentificador ant = NULL;

        while(aux != NULL && strcmp(aux->nomeIdentificador, nomeIdentificador) != 0){
            ant = aux;
            aux = aux->prox;
        }

        if(aux == NULL){
            return NULL;
        }
        else{
            if(ant == NULL){
                tabelaHash[posicao] = aux->prox;
            }
            else{
                ant->prox = aux->prox;
            }

            return aux;
        }
    }
}

PnoIdentificador buscaIdentificadorTabela(PnoIdentificador tabelaHash[], char *nomeIdentificador){
    int posicao = hash(nomeIdentificador);

    if(tabelaHash[posicao] == NULL){
        return NULL;
    }
    else{
        PnoIdentificador aux = tabelaHash[posicao];

        while(aux != NULL && strcmp(aux->nomeIdentificador, nomeIdentificador) != 0){
            aux = aux->prox;
        }

        return aux;
    }
}

void liberandoTabelaSimbolos(PnoIdentificador *tabelaHash){
    if(tabelaHash == NULL) return;

    for(int i = 0; i < MAXTAMTABELA; i++){
        PnoIdentificador atual = tabelaHash[i];

        while(atual != NULL){
            PnoIdentificador temp = atual;
            atual = atual->prox;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }

    free(tabelaHash);
}

const char* getStatementKindName(StatementKind kind) {
    switch(kind) {
        case IfK: return "IfK";
        case WhileK: return "WhileK";
        case ReturnINT: return "ReturnINT";
        case ReturnVOID: return "ReturnVOID";
        case NuloDecl: return "NuloDecl";
        case VarDeclK: return "VarDeclK";
        case VetDeclK: return "VetDeclK";
        case FunDeclK: return "FunDeclK";
        case VarParamK: return "VarParamK";
        case VetParamK: return "VetParamK";
        case ParamVoid: return "ParamVoid";
        default: return "Unknown";
    }
}

const char* getExpTypeName(ExpType type) {
    switch(type) {
        case Void: return "Void";
        case Integer: return "Integer";
        default: return "Unknown";
    }
}

/* Modify mostraTabelaSimbolos function */
void mostraTabelaSimbolos(PnoIdentificador *tabelaHash) {
    printf("\nSYMBOL TABLE\n");
    printf("-------------\n");
    for(int i = 0; i < MAXTAMTABELA; i++) {
        PnoIdentificador atual = tabelaHash[i];
        while(atual != NULL) {
            printf("Name: %-15s | ", atual->nomeIdentificador);
            printf("Type: %-12s | ", getStatementKindName(atual->tipoIdentificador));
            printf("Scope: %-10s | ", atual->escopo);
            printf("Data Type: %-8s | ", getExpTypeName(atual->tipoDado));
            printf("Lines: ");
            for(int j = 0; j < MAXNOLIN; j++) {
                if(atual->linhas[j] != 0) {
                    printf("%d ", atual->linhas[j]);
                }
            }
            printf("\n");
            atual = atual->prox;
        }
    }
    printf("-------------\n\n");
}

void insertNode(TreeNode* node) {
    if (node == NULL) return;
    
    switch (node->nodekind) {
        case StatementK:
            switch (node->kind.stmt) {
                case VarDeclK:
                    adicionaIdentificarTabela(symbolTable, 
                                            node->attr.name,
                                            VarDeclK,
                                            currentScope,
                                            node->type,
                                            node->lineno);
                    break;
                    
                case VetDeclK:
                    adicionaIdentificarTabela(symbolTable, 
                                            node->attr.name,
                                            VetDeclK,
                                            currentScope,
                                            node->type,
                                            node->lineno);
                    break;
                    
                case FunDeclK:
                    // Save old scope
                    char oldScope[MAXNOLIN];
                    strcpy(oldScope, currentScope);
                    
                    // Set new scope
                    strcpy(currentScope, node->attr.name);
                    
                    // Add function to symbol table
                    adicionaIdentificarTabela(symbolTable, 
                                            node->attr.name,
                                            FunDeclK,
                                            "global",  // functions are always global
                                            node->type,
                                            node->lineno);
                    
                    // Process function body
                    for (int i = 0; i < MAXCHILDREN; i++) {
                        buildSymTabFromTree(node->child[i]);
                    }
                    
                    // Restore old scope
                    strcpy(currentScope, oldScope);
                    return;  // Already processed children
                    
                case VarParamK:
                    adicionaIdentificarTabela(symbolTable, 
                                            node->attr.name,
                                            VarParamK,
                                            currentScope,
                                            node->type,
                                            node->lineno);
                    break;
                    
                case VetParamK:
                    adicionaIdentificarTabela(symbolTable, 
                                            node->attr.name,
                                            VetParamK,
                                            currentScope,
                                            node->type,
                                            node->lineno);
                    break;
            }
            break;
            
        case ExpressionK:
            switch (node->kind.exp) {
                case IdK:
                    // For variable uses, we just add the line number
                    PnoIdentificador existing = buscaIdentificadorTabela(symbolTable, node->attr.name);
                    if (existing != NULL) {
                        adicionaLinhaIdentificador(existing, node->lineno);
                    } else {
                        // Could add error handling for undeclared variables
                        printf("Warning: Undeclared identifier %s at line %d\n", 
                               node->attr.name, node->lineno);
                    }
                    break;
                    
                case AtivK:
                    // For function calls, add line number to function entry
                    existing = buscaIdentificadorTabela(symbolTable, node->attr.name);
                    if (existing != NULL) {
                        adicionaLinhaIdentificador(existing, node->lineno);
                    } else {
                        printf("Warning: Undeclared function %s at line %d\n", 
                               node->attr.name, node->lineno);
                    }
                    break;
            }
            break;
    }
    
    // Process siblings and children
    for (int i = 0; i < MAXCHILDREN; i++) {
        buildSymTabFromTree(node->child[i]);
    }
    if (node->sibling != NULL) {
        buildSymTabFromTree(node->sibling);
    }
}

void buildSymTabFromTree(TreeNode* tree) {
    // Initialize symbol table if not already initialized
    if (symbolTable == NULL) {
        symbolTable = inicializaTabela();
        strcpy(currentScope, "global");
    }
    
    // Start processing from root
    insertNode(tree);
}

// Function to clean up
void deleteSymTab(void) {
    if (symbolTable != NULL) {
        liberandoTabelaSimbolos(symbolTable);
        symbolTable = NULL;
    }
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/globals.h"

#define MAXTAMTABELA 29
#define SHIFT 4

extern int semantic_errors;

int inFunctionScope = 0; // Flag para indicar se estamos dentro de uma função

PnoIdentificador* symbolTable = NULL;
char currentScope[MAXTOKENLEN];

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

// Adicione esta função ao arquivo tab.c
void initSymbolTable(void) {
    if (symbolTable == NULL) {
        symbolTable = inicializaTabela();
    }
    strcpy(currentScope, "global");
    inFunctionScope = 0;
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
    
    char key[MAXNOLIN * 2];

    if (strcmp(escopo, "global") == 0) {
        snprintf(key, sizeof(key), "%s", nomeIdentificador);
    } else {
        snprintf(key, sizeof(key), "%s@%s", nomeIdentificador, escopo);
    }
    int posicao = hash(key);
    
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

PnoIdentificador buscaIdentificadorTabela(PnoIdentificador tabelaHash[], char *nomeIdentificador, char *escopo) {
    char key[MAXNOLIN * 2];
    if (strcmp(escopo, "global") == 0) {
        snprintf(key, sizeof(key), "%s", nomeIdentificador);
    } else {
        snprintf(key, sizeof(key), "%s@%s", nomeIdentificador, escopo);
    }
    int posicao = hash(key);

    if (tabelaHash[posicao] == NULL) {
        return NULL;
    } else {
        PnoIdentificador aux = tabelaHash[posicao];
        while (aux != NULL) {
            if (strcmp(aux->nomeIdentificador, nomeIdentificador) == 0 && strcmp(aux->escopo, escopo) == 0) {
                return aux;
            }
            aux = aux->prox;
        }
        return NULL;
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
    fprintf(yyout, "\nSYMBOL TABLE\n");
    fprintf(yyout, "-------------\n");
    for(int i = 0; i < MAXTAMTABELA; i++) {
        PnoIdentificador atual = tabelaHash[i];
        while(atual != NULL) {
            fprintf(yyout, "Name: %-20s | ", atual->nomeIdentificador);
            fprintf(yyout, "Type: %-12s | ", getStatementKindName(atual->tipoIdentificador));
            fprintf(yyout, "Scope: %-20s | ", atual->escopo);
            fprintf(yyout, "Data Type: %-8s | ", getExpTypeName(atual->tipoDado));
            fprintf(yyout, "Lines: ");
            for(int j = 0; j < MAXNOLIN; j++) {
                if(atual->linhas[j] != 0) {
                    fprintf(yyout, "%d ", atual->linhas[j]);
                }
            }
            fprintf(yyout, "\n");
            atual = atual->prox;
        }
    }
    fprintf(yyout, "-------------\n\n");
}

// Função para verificar se existe pelo menos um comando return em uma função
int hasReturnStatement(TreeNode* funcBody) {
    if (funcBody == NULL) return 0;
    
    // Se este nó é um return
    if (funcBody->nodekind == StatementK && 
        (funcBody->kind.stmt == ReturnINT || funcBody->kind.stmt == ReturnVOID)) {
        return 1;
    }
    
    // Verificar nos filhos
    for (int i = 0; i < MAXCHILDREN; i++) {
        if (funcBody->child[i] != NULL && hasReturnStatement(funcBody->child[i])) {
            return 1;
        }
    }
    
    // Verificar nos irmãos
    if (funcBody->sibling != NULL && hasReturnStatement(funcBody->sibling)) {
        return 1;
    }
    
    return 0;
}

// Verifica se uma função tem return do tipo correto
void checkFunctionReturnType(TreeNode* functionNode) {
    if (functionNode == NULL || functionNode->nodekind != StatementK || functionNode->kind.stmt != FunDeclK) 
        return;
    
    // Pular a verificação para a função main, que é tratada separadamente
    if (functionNode->attr.name != NULL && strcmp(functionNode->attr.name, "main") == 0) 
        return;
    
    // O corpo da função está em child[1]
    TreeNode* funcBody = functionNode->child[1];
    
    // Se a função deve retornar um valor (tipo não void)
    if (functionNode->type == Integer) {
        // Verificar se existe pelo menos um return na função
        if (!hasReturnStatement(funcBody)) {
            fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", functionNode->attr.name);
            fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", functionNode->lineno);
            fprintf(stderr, "FUNÇÃO COM TIPO DE RETORNO INT NÃO TEM COMANDO RETURN.\n");
            semantic_errors++;
        }
    }
}

void buildSymTabFromTree(TreeNode* tree) {
    // Initialize symbol table if not already initialized
    if (symbolTable == NULL) {
        symbolTable = inicializaTabela();
        strcpy(currentScope, "global");
        inFunctionScope = 0;
    }
    
    if (tree == NULL) return;
    
    // Processar o nó atual
    if (tree->nodekind == StatementK) {
        switch (tree->kind.stmt) {
            case VarDeclK:
            case VetDeclK:
                // VERIFICAÇÃO: Variáveis não podem ser do tipo void
                if (tree->type == Void) {
                    fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", tree->attr.name);
                    fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", tree->lineno);
                    fprintf(stderr, "VARIÁVEIS NÃO PODEM SER DO TIPO VOID.\n");
                    semantic_errors++;
                } 
                else {
                    // Determinar o escopo apropriado
                    char* scopeToUse = (inFunctionScope == 0) ? "global" : currentScope;
                    
                    // 1. Verificar se já existe uma função com este nome (sempre no escopo global)
                    PnoIdentificador existingFunc = buscaIdentificadorTabela(symbolTable, tree->attr.name, "global");
                    if (existingFunc != NULL && existingFunc->tipoIdentificador == FunDeclK) {
                        fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", tree->attr.name);
                        fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", tree->lineno);
                        fprintf(stderr, "NOME JÁ UTILIZADO COMO FUNÇÃO.\n");
                        semantic_errors++;
                    }
                    // 2. Verificar redeclaração no mesmo escopo
                    else if (verifyRedeclaration(tree->attr.name, scopeToUse)) {
                        fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", tree->attr.name);
                        fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", tree->lineno);
                        fprintf(stderr, "IDENTIFICADOR JÁ DECLARADO NO MESMO ESCOPO.\n");
                        semantic_errors++;
                    } 
                    else {
                        // Se passou por todas as verificações, adicionar à tabela
                        adicionaIdentificarTabela(symbolTable, 
                                              tree->attr.name,
                                              tree->kind.stmt,
                                              scopeToUse,
                                              tree->type,
                                              tree->lineno);
                    }
                }
                break;
                
            case FunDeclK:
                // 1. Verificar se já existe uma variável global com este nome
                PnoIdentificador existingVar = buscaIdentificadorTabela(symbolTable, tree->attr.name, "global");
                if (existingVar != NULL && 
                    (existingVar->tipoIdentificador == VarDeclK || existingVar->tipoIdentificador == VetDeclK)) {
                    fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", tree->attr.name);
                    fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", tree->lineno);
                    fprintf(stderr, "NOME JÁ UTILIZADO COMO VARIÁVEL GLOBAL.\n");
                    semantic_errors++;
                }
                // 2. Verificar se já existe uma função com este nome
                else if (verifyRedeclaration(tree->attr.name, "global")) {
                    fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", tree->attr.name);
                    fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", tree->lineno);
                    fprintf(stderr, "FUNÇÃO COM MESMO NOME JÁ DECLARADA.\n");
                    semantic_errors++;
                } 
                else {
                    // Se passou por todas as verificações, adicionar à tabela
                    adicionaIdentificarTabela(symbolTable, 
                                          tree->attr.name,
                                          FunDeclK,
                                          "global",
                                          tree->type,
                                          tree->lineno);
                }
                
                // Continuar processando o corpo da função (mesma lógica anterior)
                char oldScope[MAXTOKENLEN];
                strcpy(oldScope, currentScope);
                strcpy(currentScope, tree->attr.name);
                inFunctionScope++;
                
                for (int i = 0; i < MAXCHILDREN; i++) {
                    buildSymTabFromTree(tree->child[i]);
                }
                
                // Verificar se a função tem return adequado depois de processar todo o corpo
                checkFunctionReturnType(tree);
                
                strcpy(currentScope, oldScope);
                inFunctionScope--;
                break;
                
            case VarParamK:
            case VetParamK:
                // Aqui também podemos verificar conflito com função, mas normalmente
                // parâmetros são sempre locais à função e não podem conflitar com funções
                
                // Verificar redeclaração no mesmo escopo (mesma função)
                if (verifyRedeclaration(tree->attr.name, currentScope)) {
                    fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", tree->attr.name);
                    fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", tree->lineno);
                    fprintf(stderr, "PARÂMETRO COM MESMO NOME JÁ DECLARADO.\n");
                    semantic_errors++;
                } else {
                    adicionaIdentificarTabela(symbolTable, 
                                          tree->attr.name,
                                          tree->kind.stmt,
                                          currentScope,
                                          tree->type,
                                          tree->lineno);
                }
                break;
                
            default:
                // Para outros tipos de declarações, apenas processar os filhos
                break;
        }
    } 
    else if (tree->nodekind == ExpressionK) {
        switch (tree->kind.exp) {
            case IdK: {
                // Verificar se o identificador existe no escopo atual
                PnoIdentificador existing = buscaIdentificadorTabela(symbolTable, tree->attr.name, currentScope);
                if (existing == NULL) {
                    // Verificar no escopo global se não for encontrado no escopo atual
                    existing = buscaIdentificadorTabela(symbolTable, tree->attr.name, "global");
                }
                if (existing != NULL) {
                    adicionaLinhaIdentificador(existing, tree->lineno);
                } else {
                    // Erro de identificador não declarado
                    fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", tree->attr.name);
                    fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", tree->lineno);
                    fprintf(stderr, "VARIÁVEL NÃO DECLARADA.\n");
                    semantic_errors++;
                }
                break;
            }
            
            case VetorK: {
                // Verificar se o vetor existe no escopo atual
                PnoIdentificador existing = buscaIdentificadorTabela(symbolTable, tree->attr.name, currentScope);
                if (existing == NULL) {
                    existing = buscaIdentificadorTabela(symbolTable, tree->attr.name, "global");
                }
                if (existing != NULL) {
                    adicionaLinhaIdentificador(existing, tree->lineno);
                } else {
                    fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", tree->attr.name);
                    fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", tree->lineno);
                    fprintf(stderr, "VETOR NÃO DECLARADO.\n");
                    semantic_errors++;
                }
                break;
            }

            case AssignK:
                // Adicione aqui a verificação de tipos para atribuições
                checkAssignmentTypes(tree);
                break;
            
            case AtivK: {
                // Funções são sempre globais
                PnoIdentificador existing = buscaIdentificadorTabela(symbolTable, tree->attr.name, "global");
                if (existing != NULL) {
                    // Verificar se a função foi chamada antes de ser declarada
                    // A primeira linha no array é a linha de declaração da função
                    int declarationLine = existing->linhas[0];
                    
                    if (declarationLine > tree->lineno && existing->tipoIdentificador == FunDeclK) {
                        fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", tree->attr.name);
                        fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", tree->lineno);
                        fprintf(stderr, "FUNÇÃO CHAMADA ANTES DE SER DECLARADA (declarada na linha %d).\n", declarationLine);
                        semantic_errors++;
                    }
                    
                    adicionaLinhaIdentificador(existing, tree->lineno);
                } else {
                    // Excepcionalmente as funções input e output não precisam ser declaradas
                    if(strcmp(tree->attr.name, "input") != 0 && strcmp(tree->attr.name, "output") != 0) {
                        fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", tree->attr.name);
                        fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", tree->lineno);
                        fprintf(stderr, "FUNÇÃO NÃO DECLARADA.\n");
                        semantic_errors++;
                    }
                }
                break;
            }
        }
    }
    
    // Processar os irmãos e filhos (código existente)
    if (tree->sibling != NULL) {
        buildSymTabFromTree(tree->sibling);
    }
    
    if (tree->nodekind != StatementK || tree->kind.stmt != FunDeclK) {
        for (int i = 0; i < MAXCHILDREN; i++) {
            if (tree->child[i] != NULL) {
                buildSymTabFromTree(tree->child[i]);
            }
        }
    }
}

void checkAssignmentTypes(TreeNode* node) {
    if (node == NULL || node->nodekind != ExpressionK || node->kind.exp != AssignK)
        return;
    
    // Nó de atribuição tem child[0] como variável (esquerda) e child[1] como expressão (direita)
    TreeNode* leftSide = node->child[0];
    TreeNode* rightSide = node->child[1];
    
    if (leftSide == NULL || rightSide == NULL)
        return;
    
    // Obter o tipo da variável do lado esquerdo
    ExpType leftType = Integer; // Padrão para variáveis em C-
    
    // Se o lado esquerdo for um identificador, buscar seu tipo na tabela
    if (leftSide->nodekind == ExpressionK && 
        (leftSide->kind.exp == IdK || leftSide->kind.exp == VetorK)) {
        
        // Procurar primeiro no escopo atual
        PnoIdentificador leftVar = buscaIdentificadorTabela(symbolTable, leftSide->attr.name, currentScope);
        
        // Se não encontrou, procurar no escopo global
        if (leftVar == NULL) {
            leftVar = buscaIdentificadorTabela(symbolTable, leftSide->attr.name, "global");
        }
        
        if (leftVar != NULL) {
            leftType = leftVar->tipoDado; // Usar o tipo da variável na tabela de símbolos
        }
    }
    
    // Verificar se o lado direito é uma chamada de função
    if (rightSide->nodekind == ExpressionK && rightSide->kind.exp == AtivK) {
        // Procurar a função na tabela de símbolos (funções são sempre globais)
        PnoIdentificador func = buscaIdentificadorTabela(symbolTable, rightSide->attr.name, "global");
        
        // Exceção para as funções built-in
        if (strcmp(rightSide->attr.name, "input") == 0) {
            // input() retorna int
            ExpType rightType = Integer;
            // Sem verificação necessária, input sempre retorna int
        } 
        else if (strcmp(rightSide->attr.name, "output") == 0) {
            // output() retorna void, então isso é um erro
            fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", "output");
            fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", node->lineno);
            fprintf(stderr, "FUNÇÃO VOID NÃO PODE SER ATRIBUÍDA.\n");
            semantic_errors++;
        }
        else if (func != NULL) {
            // Verificar se a função retorna void
            if (func->tipoDado == Void && leftType == Integer) {
                fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", rightSide->attr.name);
                fprintf(stderr, ANSI_COLOR_PURPLE "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET ". ", node->lineno);
                fprintf(stderr, "FUNÇÃO VOID NÃO PODE SER ATRIBUÍDA A VARIÁVEL.\n");
                semantic_errors++;
            }
        }
    }
}

// Function to clean up
void deleteSymTab(void) {
    if (symbolTable != NULL) {
        liberandoTabelaSimbolos(symbolTable);
        symbolTable = NULL;
    }
}

// Verificar se a função main foi declarada
void checkMainFunction() {
    // A função main deve estar no escopo global
    PnoIdentificador mainFunc = buscaIdentificadorTabela(symbolTable, "main", "global");
    
    if (mainFunc == NULL) {
        fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET);
        fprintf(stderr, "FUNÇÃO MAIN NÃO DECLARADA. TODO PROGRAMA C- DEVE TER UMA FUNÇÃO MAIN.\n");
        semantic_errors++;
        return;
    }
    
    // Verificar se é realmente uma função e não uma variável
    if (mainFunc->tipoIdentificador != FunDeclK) {
        fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET);
        fprintf(stderr, "MAIN DEVE SER UMA FUNÇÃO, NÃO UMA VARIÁVEL.\n");
        semantic_errors++;
        return;
    }
    
    // Verificar se a função main tem tipo de retorno void (opcionalmente)
    if (mainFunc->tipoDado != Void) {
        fprintf(stderr, ANSI_COLOR_PURPLE "ERRO SEMÂNTICO: " ANSI_COLOR_RESET);
        fprintf(stderr, "FUNÇÃO MAIN DEVE TER TIPO DE RETORNO VOID.\n");
        semantic_errors++;
    }
}

int verifyRedeclaration(char *nome, char *escopo) {
    PnoIdentificador existing = buscaIdentificadorTabela(symbolTable, nome, escopo);
    if (existing != NULL) {
        return 1;
    }
    return 0;
}
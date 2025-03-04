#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "globals.h"

// Funções de análise semântica
void initSymbolTable(void);
void buildSymTabFromTree(TreeNode *);
void checkMainFunction(void);
void mostraTabelaSimbolos(PnoIdentificador *);
void deleteSymTab(void);

#endif
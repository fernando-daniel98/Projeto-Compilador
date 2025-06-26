#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/globals.h"
#include "../include/codeGen.h"
#include "../include/syntax_tree.h"
#include "../include/tab.h"
#include "../include/util.h"
// #include "../include/reg.h"


// Estrutura global para instrução de código intermediário
quadruple **intermediateCode = NULL;

int adressCounter = 0;
int numReg = 1;
int numLabel = 0;
int aux = 0;

quadruple *funcLabel = NULL;
char funcName[MAXTOKENLEN] = "global";


// Função para alocar os nós do código intermediário
void alocateIntermediateCode() {
    intermediateCode = (quadruple **) malloc(MAX_LEN_CODE_INTERMEDIATE * sizeof(quadruple *));

    for (int i = 0; i < MAX_LEN_CODE_INTERMEDIATE; i++) {
        intermediateCode[i] = NULL;
    }

    inicializaReg();
}

// Função para desalocar o código intermediário
void freeIntermediateCode() {
    if (intermediateCode == NULL) {
        return; 
    }

    for (int i = 0; i < adressCounter; i++) {
        if (intermediateCode[i] != NULL) {
            if (intermediateCode[i]->oper1 != NULL) {
                if (intermediateCode[i]->oper1->tipo == String && intermediateCode[i]->oper1->nome != NULL) {
                    free(intermediateCode[i]->oper1->nome);
                    intermediateCode[i]->oper1->nome = NULL;
                }
                free(intermediateCode[i]->oper1);
                intermediateCode[i]->oper1 = NULL;
            }

            if (intermediateCode[i]->oper2 != NULL) {
                if (intermediateCode[i]->oper2->tipo == String && intermediateCode[i]->oper2->nome != NULL) {
                    free(intermediateCode[i]->oper2->nome);
                    intermediateCode[i]->oper2->nome = NULL;
                }
                free(intermediateCode[i]->oper2);
                intermediateCode[i]->oper2 = NULL;
            }

            if (intermediateCode[i]->oper3 != NULL) {
                if (intermediateCode[i]->oper3->tipo == String && intermediateCode[i]->oper3->nome != NULL) {
                    free(intermediateCode[i]->oper3->nome);
                    intermediateCode[i]->oper3->nome = NULL;
                }
                free(intermediateCode[i]->oper3);
                intermediateCode[i]->oper3 = NULL;
            }

            free(intermediateCode[i]);
            intermediateCode[i] = NULL; 
        }
    }
    
    free(intermediateCode);
    intermediateCode = NULL; 

    // Reseta os contadores globais
    adressCounter = 0;
}

// Função que inicia o código intermediário
int initIntermediateCode() {
    alocateIntermediateCode();

    return intermediateCode == NULL ? -1 : 0;
}

ENDERECO* criaEndereco(tipoEndereco tipo, int val, char* nome, int boolReg) {
    ENDERECO* endereco = (ENDERECO*) malloc(sizeof(ENDERECO));
    if (endereco == NULL) {
        fprintf(stderr, "Falha ao alocar memória para ENDERECO.\n");
        return NULL;
    }
    
    if(tipo == IntConst) {
        endereco->tipo = IntConst;
        endereco->val = val;
        endereco->boolReg = boolReg;
        endereco->nome = NULL;
    }
    else if(tipo == String) {
        endereco->tipo = String;
        endereco->val = 0;
        if (nome != NULL) {
            endereco->nome = strdup(nome);
            if (endereco->nome == NULL) {
                fprintf(stderr, "Falha ao alocar memória para nome em ENDERECO (strdup).\n");
                free(endereco);
                return NULL;
            }
        } else {
            endereco->nome = NULL;
        }
    }
    else {
        endereco->tipo = Vazio;
        endereco->val = 0;
        endereco->nome = NULL;
    }

    return endereco;
}

quadruple* criaInstrucao(typeOperations op_enum) {
    
    quadruple* instrucao = (quadruple*) malloc(sizeof(struct instruction));
    
    if(instrucao == NULL) {
        fprintf(stderr, "Falha ao alocar memória para instrução de quadrupla.\n");
        return NULL;
    }

    instrucao->operation = op_enum; 
    instrucao->oper1 = NULL;
    instrucao->oper2 = NULL;
    instrucao->oper3 = NULL;

    return instrucao;
}

void adicionaQuadrupla(typeOperations op, ENDERECO* oper1, ENDERECO* oper2, ENDERECO* oper3) {
    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Código intermediário atingiu o tamanho máximo.\n");
        return;
    }
    
    quadruple* newQuad = criaInstrucao(op);
    if (newQuad == NULL) {
        fprintf(stderr, "Falha ao alocar memória para nova quádrupla em adicionaQuadrupla.\n");
        return;
    }
    
    newQuad->oper1 = oper1;
    newQuad->oper2 = oper2;
    newQuad->oper3 = oper3;
    
    intermediateCode[adressCounter++] = newQuad;
}

void insertDeclarationWhile(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple* label_cond_quad = NULL;
    quadruple* iff_quad = NULL;
    quadruple* goto_cond_quad = NULL;
    quadruple* label_fim_loop_quad = NULL;

    int label_cond_num = numLabel++;
    int label_fim_loop_num = numLabel++;
    int reg_cond_result;

    label_cond_quad = criaInstrucao(LABEL);
    label_cond_quad->oper1 = criaEndereco(IntConst, label_cond_num, NULL, 2); 
    label_cond_quad->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    label_cond_quad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    intermediateCode[adressCounter++] = label_cond_quad;

    criarCodigoIntermediario(tree->child[0], symbTable, 1);
    reg_cond_result = numReg; 

    iff_quad = criaInstrucao(IFFALSE); 
    iff_quad->oper1 = criaEndereco(IntConst, reg_cond_result, NULL, 1); 
    iff_quad->oper2 = criaEndereco(IntConst, label_fim_loop_num, NULL, 2); 
    iff_quad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    intermediateCode[adressCounter++] = iff_quad;

    criarCodigoIntermediario(tree->child[1], symbTable, 1); 

    goto_cond_quad = criaInstrucao(GOTO);
    goto_cond_quad->oper1 = criaEndereco(IntConst, label_cond_num, NULL, 2); 
    goto_cond_quad->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    goto_cond_quad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    intermediateCode[adressCounter++] = goto_cond_quad;

    label_fim_loop_quad = criaInstrucao(LABEL);
    label_fim_loop_quad->oper1 = criaEndereco(IntConst, label_fim_loop_num, NULL, 2); 
    label_fim_loop_quad->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    label_fim_loop_quad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    intermediateCode[adressCounter++] = label_fim_loop_quad;
}

void insertDeclarationReturn(TreeNode *tree, PnoIdentificador* symbTable) {
    quadruple* instrucaoReturn = NULL;

    instrucaoReturn = criaInstrucao(RETURN);
    
    if(tree->kind.stmt == ReturnINT) {
        criarCodigoIntermediario(tree->child[0], symbTable, 1);
        instrucaoReturn->oper1 = criaEndereco(IntConst, numReg, NULL, 1);
    }
    else { 
        instrucaoReturn->oper1 = criaEndereco(Vazio, 0, NULL, 0);
    }

    instrucaoReturn->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    instrucaoReturn->oper3 = criaEndereco(Vazio, 0, NULL, 0);

    intermediateCode[adressCounter++] = instrucaoReturn;
}

void insertDeclarationVar(TreeNode *tree, PnoIdentificador* symbTable) {
    quadruple* instrucaoVar = NULL;

    instrucaoVar = criaInstrucao(ALLOC);
    if (instrucaoVar == NULL) {
        fprintf(stderr, "Falha ao criar instrução ALLOC em insertDeclarationVar.\n");
        return;
    }
    instrucaoVar ->oper1 = criaEndereco(String, 0, tree->attr.name, 0);
    if (instrucaoVar->oper1 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para oper1 em insertDeclarationVar.\n");
        free(instrucaoVar);
        return;
    }

    PnoIdentificador no = buscaIdentificadorTabela(symbTable, tree->attr.name, funcName); 

    if(no == NULL) {
        no = buscaIdentificadorTabela(symbTable, tree->attr.name, "global");
    }

    if(no == NULL) {
        instrucaoVar->oper2 = criaEndereco(String, 0, funcName, 0);
        fprintf(stderr, "Variável '%s' não encontrada na tabela de símbolos (linha %d). Assumindo escopo '%s'.\n", tree->attr.name, tree->lineno, funcName);
    }
    else if (strcmp(no->escopo, "global") == 0){
        instrucaoVar->oper2 = criaEndereco(String, 0, "global", 0);
    }
    else {
        instrucaoVar->oper2 = criaEndereco(String, 0, no->escopo, 0); 
    }


    instrucaoVar -> oper3 = criaEndereco(Vazio, 0, NULL, 0);
    if (instrucaoVar->oper3 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para oper3 em insertDeclarationVar.\n");
        if(instrucaoVar->oper1->nome) free(instrucaoVar->oper1->nome);
        free(instrucaoVar->oper1);
        if(instrucaoVar->oper2 && instrucaoVar->oper2->nome) free(instrucaoVar->oper2->nome);
        free(instrucaoVar->oper2); 
        free(instrucaoVar);
        return;
    }


    if(tree -> kind.stmt == VetDeclK) {
        if (tree->child[0] != NULL && tree->child[0]->kind.exp == ConstK) {
             instrucaoVar->oper3->tipo = IntConst;
             instrucaoVar->oper3->val = tree->child[0]->attr.val;
        } else {
             fprintf(stderr, "Declaração de vetor '%s' (linha %d) sem tamanho constante.\n", tree->attr.name, tree->lineno);
             instrucaoVar->oper3->tipo = IntConst;
             instrucaoVar->oper3->val = 1; 
        }
    }

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido em insertDeclarationVar.\n");
        if(instrucaoVar->oper1->nome) free(instrucaoVar->oper1->nome);
        free(instrucaoVar->oper1);
        if(instrucaoVar->oper2) { if(instrucaoVar->oper2->nome) free(instrucaoVar->oper2->nome); free(instrucaoVar->oper2); }
        if(instrucaoVar->oper3) { if(instrucaoVar->oper3->nome) free(instrucaoVar->oper3->nome); free(instrucaoVar->oper3); }
        free(instrucaoVar);
        return;
    }
    intermediateCode[adressCounter++] = instrucaoVar;
}

void insertDeclarationIf(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple* instrucaoIFF = NULL;
    quadruple* instrucaoGOTO = NULL;
    quadruple* label_else_inicio = NULL; 
    quadruple* label_fim_if = NULL;     

    int else_inicio_label_num;
    int fim_if_label_num;

    criarCodigoIntermediario(tree->child[0], symbTable, 1);

    else_inicio_label_num = numLabel++; 

    instrucaoIFF = criaInstrucao(IFFALSE);
    instrucaoIFF->oper1 = criaEndereco(IntConst, numReg, NULL, 1); 
    instrucaoIFF->oper2 = criaEndereco(IntConst, else_inicio_label_num, NULL, 2); 
    instrucaoIFF->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    intermediateCode[adressCounter++] = instrucaoIFF;

    criarCodigoIntermediario(tree->child[1], symbTable, 1);

    if(tree->child[2] != NULL){ 
        fim_if_label_num = numLabel++; 

        instrucaoGOTO = criaInstrucao(GOTO);
        instrucaoGOTO->oper1 = criaEndereco(IntConst, fim_if_label_num, NULL, 2);
        instrucaoGOTO->oper2 = criaEndereco(Vazio, 0, NULL, 0);
        instrucaoGOTO->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        intermediateCode[adressCounter++] = instrucaoGOTO;

        label_else_inicio = criaInstrucao(LABEL);
        label_else_inicio->oper1 = criaEndereco(IntConst, else_inicio_label_num, NULL, 2);
        label_else_inicio->oper2 = criaEndereco(Vazio, 0, NULL, 0);
        label_else_inicio->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        intermediateCode[adressCounter++] = label_else_inicio;

        criarCodigoIntermediario(tree->child[2], symbTable, 1);

        label_fim_if = criaInstrucao(LABEL);
        label_fim_if->oper1 = criaEndereco(IntConst, fim_if_label_num, NULL, 2);
        label_fim_if->oper2 = criaEndereco(Vazio, 0, NULL, 0);
        label_fim_if->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        intermediateCode[adressCounter++] = label_fim_if;
    }
    else{ 
        label_fim_if = criaInstrucao(LABEL); 
        label_fim_if->oper1 = criaEndereco(IntConst, else_inicio_label_num, NULL, 2);
        label_fim_if->oper2 = criaEndereco(Vazio, 0, NULL, 0);
        label_fim_if->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        intermediateCode[adressCounter++] = label_fim_if;
    }
}

void insertDeclarationAssign(TreeNode *tree, PnoIdentificador* symbTable){
    if (tree == NULL || tree->child[0] == NULL || tree->child[1] == NULL) {
        fprintf(stderr, "Erro interno: Nó de atribuição inválido ou filhos ausentes na linha %d.\n", tree ? tree->lineno : -1);
        return;
    }

    TreeNode *lhs = tree->child[0]; 
    TreeNode *rhs = tree->child[1]; 
    quadruple* instrucaoStore = NULL;
    ENDERECO *op1_store = NULL, *op2_store = NULL, *op3_store = NULL;
    int reg_rhs_val;    
    int reg_index_val;  

    criarCodigoIntermediario(rhs, symbTable, 1); 
    reg_rhs_val = numReg; 

    op2_store = criaEndereco(IntConst, reg_rhs_val, NULL, 1); 
    if (op2_store == NULL) {
        fprintf(stderr, "Falha ao criar endereço para o valor do RHS na atribuição, linha %d.\n", tree->lineno);
        return;
    }

    if (lhs->kind.exp == IdK) { 
        op1_store = criaEndereco(String, 0, lhs->attr.name, 0); 
        if (op1_store == NULL) {
            fprintf(stderr, "Falha ao criar endereço para variável LHS '%s' na atribuição, linha %d.\n", lhs->attr.name, tree->lineno);
            if(op2_store->nome) free(op2_store->nome); free(op2_store); 
            return;
        }
        op3_store = criaEndereco(Vazio, 0, NULL, 0);
        if (op3_store == NULL) {
            fprintf(stderr, "Falha ao criar endereço vazio para LHS (IdK) na atribuição, linha %d.\n", tree->lineno);
            if(op1_store->nome) free(op1_store->nome); free(op1_store);
            if(op2_store->nome) free(op2_store->nome); free(op2_store);
            return;
        }
    } else if (lhs->kind.exp == VetorK) { 
        op1_store = criaEndereco(String, 0, lhs->attr.name, 0); 
        if (op1_store == NULL) {
            fprintf(stderr, "Falha ao criar endereço para vetor LHS '%s' na atribuição, linha %d.\n", lhs->attr.name, tree->lineno);
            if(op2_store->nome) free(op2_store->nome); free(op2_store);
            return;
        }
        criarCodigoIntermediario(lhs->child[0], symbTable, 1);
        reg_index_val = numReg; 

        op3_store = criaEndereco(IntConst, reg_index_val, NULL, 1); 
        if (op3_store == NULL) {
            fprintf(stderr, "Falha ao criar endereço para o índice do vetor LHS '%s' na atribuição, linha %d.\n", lhs->attr.name, tree->lineno);
            if(op1_store->nome) free(op1_store->nome); free(op1_store);
            if(op2_store->nome) free(op2_store->nome); free(op2_store);
            return;
        }
    } else {
        fprintf(stderr, "Lado esquerdo inválido (não é IdK nem VetorK) na atribuição, linha %d.\n", tree->lineno);
        if(op2_store->nome) free(op2_store->nome); free(op2_store); 
        return;
    }

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido antes de adicionar instrução STORE, linha %d.\n", tree->lineno);
        if(op1_store) { if(op1_store->nome) free(op1_store->nome); free(op1_store); }
        if(op2_store) { if(op2_store->nome) free(op2_store->nome); free(op2_store); } 
        if(op3_store) { if(op3_store->nome) free(op3_store->nome); free(op3_store); } 
        return;
    }

    instrucaoStore = criaInstrucao(STORE);
    if (instrucaoStore == NULL) {
        fprintf(stderr, "Falha ao criar instrução STORE na atribuição, linha %d.\n", tree->lineno);
        if(op1_store) { if(op1_store->nome) free(op1_store->nome); free(op1_store); }
        if(op2_store) { if(op2_store->nome) free(op2_store->nome); free(op2_store); }
        if(op3_store) { if(op3_store->nome) free(op3_store->nome); free(op3_store); }
        return;
    }

    instrucaoStore->oper1 = op1_store; 
    instrucaoStore->oper2 = op2_store; 
    instrucaoStore->oper3 = op3_store; 

    intermediateCode[adressCounter++] = instrucaoStore;
}

void insertDeclarationFunc(TreeNode *tree, PnoIdentificador* symbolTable){
    TreeNode* paramNode = tree->child[0];
    int numParams = 0;

    strncpy(funcName, tree->attr.name, MAXTOKENLEN -1);
    funcName[MAXTOKENLEN -1] = '\0';

    TreeNode* tempParamNode = tree->child[0];
    while (tempParamNode != NULL && tempParamNode->kind.stmt != ParamVoid) {
        numParams++;
        tempParamNode = tempParamNode->sibling;
    }

    quadruple* funcQuad = criaInstrucao(FUNC); 
    if (funcQuad == NULL) {
        fprintf(stderr, "Falha ao criar instrução FUNC para %s.\n", tree->attr.name);
        strcpy(funcName, "global"); 
        return;
    }

    if (tree->type == Integer) {
        funcQuad->oper1 = criaEndereco(String, 0, "INT", 0);
    } else if (tree->type == Void) {
        funcQuad->oper1 = criaEndereco(String, 0, "VOID", 0);
    } else { 
        funcQuad->oper1 = criaEndereco(String, 0, "UNKNOWN_RET_TYPE", 0);
    }

    funcQuad->oper2 = criaEndereco(String, 0, tree->attr.name, 0);
    if (funcQuad->oper2 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para nome da função %s.\n", tree->attr.name);
        if(funcQuad->oper1 && funcQuad->oper1->nome) free(funcQuad->oper1->nome); free(funcQuad->oper1);
        free(funcQuad);
        strcpy(funcName, "global"); 
        return;
    }

    funcQuad->oper3 = criaEndereco(IntConst, numParams, NULL, 0);
    if (funcQuad->oper3 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para número de parâmetros da função %s.\n", tree->attr.name);
        if(funcQuad->oper1 && funcQuad->oper1->nome) free(funcQuad->oper1->nome); free(funcQuad->oper1);
        if(funcQuad->oper2 && funcQuad->oper2->nome) free(funcQuad->oper2->nome); free(funcQuad->oper2);
        free(funcQuad);
        strcpy(funcName, "global"); 
        return;
    }
    
    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido ao declarar função %s.\n", tree->attr.name);
        if(funcQuad->oper1 && funcQuad->oper1->nome) free(funcQuad->oper1->nome); free(funcQuad->oper1);
        if(funcQuad->oper2 && funcQuad->oper2->nome) free(funcQuad->oper2->nome); free(funcQuad->oper2);
        if(funcQuad->oper3) free(funcQuad->oper3);
        free(funcQuad);
        strcpy(funcName, "global"); 
        return;
    }
    intermediateCode[adressCounter++] = funcQuad;

    paramNode = tree->child[0]; 
    while (paramNode != NULL && paramNode->kind.stmt != ParamVoid) { 
        quadruple* argQuad = criaInstrucao(ARG);
        if (argQuad == NULL) {
            fprintf(stderr, "Falha ao criar instrução ARG para função %s, parâmetro %s.\n", tree->attr.name, paramNode->attr.name ? paramNode->attr.name : "NOME_PARAM_NULO");
            paramNode = paramNode->sibling;
            continue;
        }
        
        // oper1: Tipo do parâmetro (INT ou VET)
        if (paramNode->kind.stmt == VarParamK) {
            if (paramNode->type == Integer) {
                argQuad->oper1 = criaEndereco(String, 0, "INT", 0);
            } else { 
                argQuad->oper1 = criaEndereco(String, 0, "UNKNOWN_VAR_TYPE", 0);
            }
        } else if (paramNode->kind.stmt == VetParamK) {
            argQuad->oper1 = criaEndereco(String, 0, "VET", 0);
        } else {
            argQuad->oper1 = criaEndereco(String, 0, "UNKNOWN_PARAM_KIND", 0);
        }
        
        // oper2: Nome do parâmetro
        argQuad->oper2 = criaEndereco(String, 0, paramNode->attr.name, 0);
        
        // oper3: Escopo (nome da função)
        argQuad->oper3 = criaEndereco(String, 0, tree->attr.name, 0);
        
        if (argQuad->oper1 == NULL || argQuad->oper2 == NULL || argQuad->oper3 == NULL) {
            fprintf(stderr, "Falha ao criar um dos endereços para ARG da função %s, parâmetro %s.\n", tree->attr.name, paramNode->attr.name ? paramNode->attr.name : "NOME_PARAM_NULO");
            if(argQuad->oper1) { if(argQuad->oper1->nome) free(argQuad->oper1->nome); free(argQuad->oper1); }
            if(argQuad->oper2) { if(argQuad->oper2->nome) free(argQuad->oper2->nome); free(argQuad->oper2); }
            if(argQuad->oper3) { if(argQuad->oper3->nome) free(argQuad->oper3->nome); free(argQuad->oper3); }
            free(argQuad);
            paramNode = paramNode->sibling;
            continue; 
        }

        if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
            fprintf(stderr, "Limite do código intermediário atingido ao declarar argumento para %s.\n", tree->attr.name);
            if(argQuad->oper1) { if(argQuad->oper1->nome) free(argQuad->oper1->nome); free(argQuad->oper1); }
            if(argQuad->oper2) { if(argQuad->oper2->nome) free(argQuad->oper2->nome); free(argQuad->oper2); }
            if(argQuad->oper3) { if(argQuad->oper3->nome) free(argQuad->oper3->nome); free(argQuad->oper3); }
            free(argQuad);
            strcpy(funcName, "global"); 
            return;
        }
        intermediateCode[adressCounter++] = argQuad;
        paramNode = paramNode->sibling;
    }

    // Criar quadruplas para LOAD dos parâmetros (carregar valor/endereço em registradores).
    TreeNode* paramChild = tree->child[0];
    while (paramChild != NULL && paramChild->kind.stmt != ParamVoid) { // Condição corrigida
        quadruple* loadQuad = criaInstrucao(LOAD);
        if (loadQuad == NULL) {
            fprintf(stderr, "Falha ao criar instrução LOAD para parâmetro %s da função %s.\n", 
                    paramChild->attr.name ? paramChild->attr.name : "NOME_PARAM_NULO", tree->attr.name);
            paramChild = paramChild->sibling;
            continue;
        }

        // oper1: Registrador de destino
        int reg_dest = verificacaoRegistradores(paramChild->attr.name, tree->attr.name, 0); // 0 para não temporário
        if (reg_dest == -1) {
            // verificacaoRegistradores já imprime erro se não conseguir alocar/descartar.
            // Apenas liberar o loadQuad e continuar.
            free(loadQuad); 
            paramChild = paramChild->sibling;
            continue;
        }
        loadQuad->oper1 = criaEndereco(IntConst, reg_dest, NULL, 1); // boolReg = 1 para registrador

        // oper2: Nome do parâmetro (fonte do valor/endereço)
        loadQuad->oper2 = criaEndereco(String, 0, paramChild->attr.name, 0);
        
        // oper3: Vazio (LOAD de variável/parâmetro não usa o terceiro operando como índice aqui)
        loadQuad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        
        if (loadQuad->oper1 == NULL || loadQuad->oper2 == NULL || loadQuad->oper3 == NULL) {
            fprintf(stderr, "Falha ao criar um dos endereços para LOAD do parâmetro %s da função %s.\n", 
                    paramChild->attr.name ? paramChild->attr.name : "NOME_PARAM_NULO", tree->attr.name);
            if(loadQuad->oper1) { free(loadQuad->oper1); } // É IntConst, não tem nome
            if(loadQuad->oper2) { if(loadQuad->oper2->nome) free(loadQuad->oper2->nome); free(loadQuad->oper2); }
            if(loadQuad->oper3) { free(loadQuad->oper3); } // É Vazio, não tem nome
            free(loadQuad);
            paramChild = paramChild->sibling;
            continue; 
        }

        if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
            fprintf(stderr, "Limite do código intermediário atingido ao criar LOAD para parâmetro %s da função %s.\n", 
                    paramChild->attr.name ? paramChild->attr.name : "NOME_PARAM_NULO", tree->attr.name);
            if(loadQuad->oper1) { free(loadQuad->oper1); }
            if(loadQuad->oper2) { if(loadQuad->oper2->nome) free(loadQuad->oper2->nome); free(loadQuad->oper2); }
            if(loadQuad->oper3) { free(loadQuad->oper3); }
            free(loadQuad);
            strcpy(funcName, "global"); 
            return; // Mantendo o return em caso de estouro, conforme código original do usuário.
        }
        intermediateCode[adressCounter++] = loadQuad;
        paramChild = paramChild->sibling;
    }

    if (tree->child[1] != NULL) {
        if (tree->child[1]->child[0] != NULL) { 
            criarCodigoIntermediario(tree->child[1]->child[0], symbolTable, 0);
        }
        if (tree->child[1]->child[1] != NULL) { 
            criarCodigoIntermediario(tree->child[1]->child[1], symbolTable, 0);
        }
    }

    quadruple* endFuncQuad = criaInstrucao(END);
    if (endFuncQuad == NULL) {
        fprintf(stderr, "Falha ao criar instrução END para função %s.\n", tree->attr.name);
        return;
    }
    endFuncQuad->oper1 = criaEndereco(String, 0, tree->attr.name, 0); 
    if (endFuncQuad->oper1 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para nome da função (END) para %s.\n", tree->attr.name);
        free(endFuncQuad);
        return;
    }
    endFuncQuad->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    if (endFuncQuad->oper2 == NULL) {
        fprintf(stderr, "Falha ao criar endereço vazio (oper2 END) para função %s.\n", tree->attr.name);
        if(endFuncQuad->oper1->nome) free(endFuncQuad->oper1->nome); free(endFuncQuad->oper1);
        free(endFuncQuad);
        return;
    }
    endFuncQuad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    if (endFuncQuad->oper3 == NULL) {
        fprintf(stderr, "Falha ao criar endereço vazio (oper3 END) para função %s.\n", tree->attr.name);
        if(endFuncQuad->oper1->nome) free(endFuncQuad->oper1->nome); free(endFuncQuad->oper1);
        free(endFuncQuad->oper2);
        free(endFuncQuad);
        return;
    }

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido ao finalizar função %s.\n", tree->attr.name);
        if(endFuncQuad->oper1->nome) free(endFuncQuad->oper1->nome); free(endFuncQuad->oper1);
        free(endFuncQuad->oper2);
        free(endFuncQuad->oper3);
        free(endFuncQuad);
        return;
    }
    intermediateCode[adressCounter++] = endFuncQuad;

    strcpy(funcName, "global");
}

void insertExpressionOp(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple* op = NULL;
    typeOperations tacOp = mapOperatorToTAC(tree->attr.op);
    int reg_child0_val, reg_child1_val, reg_result_val;

    op = criaInstrucao(tacOp);
    if (op == NULL) {
        fprintf(stderr, "Falha ao criar instrução para operador %s na linha %d.\n", operatorToString(tree->attr.op), tree->lineno);
        return;
    }

    criarCodigoIntermediario(tree->child[0], symbTable, 1);
    reg_child0_val = numReg; 
    op->oper1 = criaEndereco(IntConst, reg_child0_val, NULL, 1);

    if (tree->child[1] != NULL) {
        criarCodigoIntermediario(tree->child[1], symbTable, 1);
        reg_child1_val = numReg; 
        op->oper2 = criaEndereco(IntConst, reg_child1_val, NULL, 1);
    } else {
        op->oper2 = criaEndereco(Vazio, 0, NULL, 0); 
    }

    reg_result_val = verificacaoRegistradores(NULL, NULL, 1); 
    numReg = reg_result_val; 

    op->oper3 = criaEndereco(IntConst, reg_result_val, NULL, 1);

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido em operação, linha %d.\n", tree->lineno);
        if(op->oper1) free(op->oper1);
        if(op->oper2) free(op->oper2);
        if(op->oper3) free(op->oper3);
        free(op);
        return;
    }
    intermediateCode[adressCounter++] = op;
}

void insertExpressionRel(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple * op = NULL;
    typeOperations tacOp = mapOperatorToTAC(tree->attr.op);
    int reg_child0_val, reg_child1_val, reg_result_val;

    op = criaInstrucao(tacOp);
    if (op == NULL) {
        fprintf(stderr, "Falha ao criar instrução para operador relacional %s na linha %d.\n", operatorToString(tree->attr.op), tree->lineno);
        return;
    }

    criarCodigoIntermediario(tree->child[0], symbTable, 1);
    reg_child0_val = numReg; 

    criarCodigoIntermediario(tree->child[1], symbTable, 1);
    reg_child1_val = numReg; 

    op->oper1 = criaEndereco(IntConst, reg_child0_val, NULL, 1);
    op->oper2 = criaEndereco(IntConst, reg_child1_val, NULL, 1);

    reg_result_val = verificacaoRegistradores(NULL, NULL, 1);
    numReg = reg_result_val; 

    op->oper3 = criaEndereco(IntConst, reg_result_val, NULL, 1);

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido em operação relacional, linha %d.\n", tree->lineno);
        if(op->oper1) free(op->oper1);
        if(op->oper2) free(op->oper2);
        if(op->oper3) free(op->oper3);
        free(op);
        return;
    }
    intermediateCode[adressCounter++] = op;
}

void insertExpressionConst(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple* constant = NULL;

    // Para o caso da constante ser zero, reservei o endereço 63 do registrador
    if (tree->attr.val == 0) {
        numReg = 63; // Registrador 31 é reservado para zero
        return;
    }
    
    numReg = verificacaoRegistradores(NULL, NULL, 1); 
        
    constant = criaInstrucao(LOADI); 
    if (constant == NULL) {
        fprintf(stderr, "Falha ao criar instrução LOADI em constante, linha %d.\n", tree->lineno);
        return;
    }
    constant->oper1 = criaEndereco(IntConst, numReg, NULL, 1);      
    constant->oper2 = criaEndereco(IntConst, tree->attr.val, NULL, 0); 
    constant->oper3 = criaEndereco(Vazio, 0, NULL, 0);              

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido em constante, linha %d.\n", tree->lineno);
        if(constant->oper1) free(constant->oper1);
        if(constant->oper2) free(constant->oper2);
        if(constant->oper3) free(constant->oper3);
        free(constant);
        return;
    }
    intermediateCode[adressCounter++] = constant;
}

void insertExpressionId(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple* instrucaoId = NULL;
    ENDERECO *op1 = NULL, *op2 = NULL, *op3 = NULL;

    if (tree == NULL) {
        fprintf(stderr, "Erro interno: Nó da árvore nulo em insertExpressionId.\n");
        return;
    }
    if (tree->nodekind != ExpressionK || (tree->kind.exp != VetorK && tree->kind.exp != IdK)) {
        fprintf(stderr, "Erro interno: Nó inesperado em insertExpressionId, linha %d.\n", tree->lineno);
        return;
    }

    if(tree->kind.exp == VetorK){

        if (tree->child[0] == NULL) {
            fprintf(stderr, ANSI_COLOR_RED "ERRO: " ANSI_COLOR_RESET);
            fprintf(stderr, "Acesso a vetor '%s' (linha %d) sem expressão de índice.\n", tree->attr.name, tree->lineno);
            return; 
        }

        instrucaoId = criaInstrucao(LOAD);
        if (instrucaoId == NULL) {
            fprintf(stderr, "Falha ao criar instrução LOAD para vetor '%s', linha %d.\n", tree->attr.name, tree->lineno);
            return;
        }

        // Processa a expressão do índice. O resultado estará em 'numReg'.
        criarCodigoIntermediario(tree->child[0], symbTable, 1); 
        op3 = criaEndereco(IntConst, numReg, NULL, 1); // oper3 é o registrador do índice
        if (op3 == NULL) {
            fprintf(stderr, "Falha ao criar endereço para oper3 (índice do vetor) para LOAD de '%s', linha %d.\n", tree->attr.name, tree->lineno);
            free(instrucaoId);
            return;
        }
        // aux = numReg; // A variável global 'aux' parece ser usada para 'vetorIndice' em outros contextos.
                       // Se for para o LOAD, o índice já está em op3. Se for para um STORE subsequente,
                       // a função de STORE deve recalcular ou obter o índice.
                       // Para o LOAD, 'aux' não é diretamente usada aqui.

        // Obtém um novo registrador para ser o destino do LOAD.
        // O '1' em verificacaoRegistradores indica que é para um temporário.
        int destReg = verificacaoRegistradores(NULL, NULL, 1); 
        if (destReg == -1) {
            fprintf(stderr, ANSI_COLOR_RED "ERRO: " ANSI_COLOR_RESET);
            fprintf(stderr, "Falha ao obter registrador de destino para LOAD de vetor '%s', linha %d.\n", tree->attr.name, tree->lineno);
            // verificacaoRegistradores deve imprimir seu próprio erro detalhado.
            if(op3->nome) free(op3->nome); free(op3); // op3 é IntConst, nome é NULL
            free(instrucaoId);
            return;
        }
        numReg = destReg; // Atualiza numReg para o registrador de destino do LOAD
        
        op1 = criaEndereco(IntConst, numReg, NULL, 1); // oper1 é o registrador de destino
        op2 = criaEndereco(String, 0, tree->attr.name, 0); // oper2 é o nome base do vetor

        if (op1 == NULL || op2 == NULL) {
            fprintf(stderr, "Falha ao criar endereços para oper1 ou oper2 para LOAD de vetor '%s', linha %d.\n", tree->attr.name, tree->lineno);
            if(op1) free(op1);
            if(op2 && op2->nome) free(op2->nome); if(op2) free(op2);
            if(op3) free(op3); // op3 é IntConst, nome é NULL
            free(instrucaoId);
            return;
        }
        instrucaoId->oper1 = op1;
        instrucaoId->oper2 = op2;
        instrucaoId->oper3 = op3;

    }
    else if(tree->kind.exp == IdK){  
        PnoIdentificador varEscopo = NULL;
        varEscopo = buscaIdentificadorTabela(symbTable, tree->attr.name, funcName);
        char* escopoParaVerificacao = funcName;

        if(varEscopo == NULL){
            varEscopo = buscaIdentificadorTabela(symbTable, tree->attr.name, "global");
            if (varEscopo != NULL) {
                escopoParaVerificacao = "global";
            }
        } else {
            escopoParaVerificacao = varEscopo->escopo; // Usar o escopo encontrado
        }

        if(varEscopo == NULL){
            fprintf(stderr, ANSI_COLOR_RED "ERRO: " ANSI_COLOR_RESET);
            fprintf(stderr, "Variável '%s' (linha %d) não encontrada na tabela de símbolos.\n", tree->attr.name, tree->lineno);
            numReg = -1; // Sinaliza erro para a lógica de adição da quádrupla
            // Não criar instrucaoId aqui, pois a variável não existe.
        } else {
            // O '0' em verificacaoRegistradores indica que é para uma variável (não temporário).
            int destReg = verificacaoRegistradores(tree->attr.name, escopoParaVerificacao, 0);
            if (destReg == -1) {
                fprintf(stderr, ANSI_COLOR_RED "ERRO: " ANSI_COLOR_RESET);
                fprintf(stderr, "Falha ao obter registrador para variável '%s' no escopo '%s', linha %d.\n", tree->attr.name, escopoParaVerificacao, tree->lineno);
                // verificacaoRegistradores deve imprimir seu próprio erro detalhado.
                numReg = -1; // Sinaliza erro
            } else {
                numReg = destReg; // Atualiza numReg para o registrador de destino do LOAD

                instrucaoId = criaInstrucao(LOAD);
                if (instrucaoId == NULL) {
                    fprintf(stderr, "Falha ao criar instrução LOAD para Id '%s', linha %d.\n", tree->attr.name, tree->lineno);
                    numReg = -1; // Sinaliza erro
                } else {
                    op1 = criaEndereco(IntConst, numReg, NULL, 1);
                    op2 = criaEndereco(String, 0, tree->attr.name, 0);
                    op3 = criaEndereco(Vazio, 0, NULL, 0);

                    if (op1 == NULL || op2 == NULL || op3 == NULL) {
                        fprintf(stderr, "Falha ao criar endereços para LOAD de Id '%s', linha %d.\n", tree->attr.name, tree->lineno);
                        if(op1) free(op1);
                        if(op2 && op2->nome) free(op2->nome); if(op2) free(op2);
                        if(op3) free(op3);
                        free(instrucaoId);
                        instrucaoId = NULL; // Garante que não será adicionado
                        numReg = -1; // Sinaliza erro
                    } else {
                        instrucaoId->oper1 = op1;
                        instrucaoId->oper2 = op2;
                        instrucaoId->oper3 = op3;
                    }
                }
            }
        }
    }

    // Adiciona a instrução ao código intermediário se tudo ocorreu bem
    if(instrucaoId != NULL && numReg != -1){ 
        if (adressCounter < MAX_LEN_CODE_INTERMEDIATE) {
            intermediateCode[adressCounter++] = instrucaoId;
        } else {
            fprintf(stderr, "Limite do código intermediário atingido ao adicionar LOAD para '%s', linha %d.\n", tree->attr.name, tree->lineno);
            // Libera a instrução e seus operandos, pois não será adicionada
            if(instrucaoId->oper1) free(instrucaoId->oper1); // Nome é NULL para IntConst
            if(instrucaoId->oper2 && instrucaoId->oper2->nome) free(instrucaoId->oper2->nome);
            if(instrucaoId->oper2) free(instrucaoId->oper2);
            if(instrucaoId->oper3) free(instrucaoId->oper3); // Nome é NULL para IntConst/Vazio
            free(instrucaoId);
        }
    } else if (instrucaoId != NULL && numReg == -1) {
        // Se instrucaoId foi criada mas houve erro (numReg == -1), libera
        // (Este caso pode ser redundante se as falhas anteriores já limpam instrucaoId e o setam para NULL)
        fprintf(stderr, "INFO: Instrução LOAD para '%s' (linha %d) não adicionada devido a erro anterior na obtenção de registrador.\n", tree->attr.name, tree->lineno);
        if(instrucaoId->oper1) free(instrucaoId->oper1);
        if(instrucaoId->oper2 && instrucaoId->oper2->nome) free(instrucaoId->oper2->nome);
        if(instrucaoId->oper2) free(instrucaoId->oper2);
        if(instrucaoId->oper3) free(instrucaoId->oper3);
        free(instrucaoId);
    }
    // Se varEscopo == NULL (para IdK), instrucaoId não foi nem criada, então não há o que liberar aqui.
    // numReg já está -1.
}

void insertExpressionCall(TreeNode *tree, PnoIdentificador* symbTable) {
    quadruple* instrucaoCall = NULL;
    quadruple* instrucaoParam = NULL;
    TreeNode* argNode = tree->child[0]; 

    int numParam = 0; 

    instrucaoCall = criaInstrucao(CALL);
    if (instrucaoCall == NULL) {
        fprintf(stderr, "Falha ao criar instrução CALL para %s, linha %d.\n", tree->attr.name, tree->lineno);
        return;
    }
    instrucaoCall->oper1 = criaEndereco(String, 0, tree->attr.name, 0); 
    if (instrucaoCall->oper1 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para nome da função (CALL oper1) para %s, linha %d.\n", tree->attr.name, tree->lineno);
        free(instrucaoCall);
        return;
    }

    while(argNode !=  NULL){
        instrucaoParam = criaInstrucao(PARAM);
        if (instrucaoParam == NULL) {
            fprintf(stderr, "Falha ao criar instrução PARAM para %s, argumento %d, linha %d.\n", tree->attr.name, numParam + 1, argNode->lineno);
            if(instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome);
            free(instrucaoCall->oper1);
            free(instrucaoCall);
            return;
        }
        
        if (argNode->nodekind == ExpressionK) {
            if (argNode->kind.exp == IdK) {
                PnoIdentificador idInfo = buscaIdentificadorTabela(symbTable, argNode->attr.name, funcName);
                if (idInfo == NULL) {
                    idInfo = buscaIdentificadorTabela(symbTable, argNode->attr.name, "global");
                }

                if (idInfo != NULL && (idInfo->tipoIdentificador == VetDeclK || idInfo->tipoIdentificador == VetParamK)) {
                    int reg_vet_addr = verificacaoRegistradores(argNode->attr.name, funcName, 0); // 0 para não temporário
                    if (reg_vet_addr != -1) {
                        instrucaoParam->oper1 = criaEndereco(IntConst, reg_vet_addr, NULL, 1); 
                        instrucaoParam->oper2 = criaEndereco(String, 0, "VET", 0);      
                        instrucaoParam->oper3 = criaEndereco(String, 0, argNode->attr.name, 0);
                    } else {
                        // Erro já impresso por verificacaoRegistradores.
                        // Deixar oper1, oper2, oper3 como NULL para serem pegos pela verificação de erro abaixo.
                        fprintf(stderr, "INFO: Não foi possível alocar registrador para vetor '%s' (linha %d) na chamada de função. Quádrupla PARAM não será gerada corretamente.\n", argNode->attr.name, argNode->lineno);
                    }
                } else { // Não é um vetor conhecido ou é uma variável simples
                    criarCodigoIntermediario(argNode, symbTable, 1); 
                    instrucaoParam->oper1 = criaEndereco(IntConst, numReg, NULL, 1); 
                    instrucaoParam->oper2 = criaEndereco(String, 0, "INT", 0); 
                    instrucaoParam->oper3 = criaEndereco(Vazio, 0, NULL, 0);
                }
            } else { // Não é IdK (ex: ConstK, OpK)
                criarCodigoIntermediario(argNode, symbTable, 1); 
                instrucaoParam->oper1 = criaEndereco(IntConst, numReg, NULL, 1); 
                instrucaoParam->oper2 = criaEndereco(String, 0, "INT", 0); 
                instrucaoParam->oper3 = criaEndereco(Vazio, 0, NULL, 0);
            }
        } else { // Não é ExpressionK (caso de erro, mas mantendo a lógica original)
            fprintf(stderr, "Argumento de função inválido (não é ExpressionK) para %s, linha %d.\n", tree->attr.name, argNode->lineno);
            criarCodigoIntermediario(argNode, symbTable, 1); 
            instrucaoParam->oper1 = criaEndereco(IntConst, numReg, NULL, 1);
            instrucaoParam->oper2 = criaEndereco(String, 0, "UNKNOWN_ARG_TYPE", 0);
            instrucaoParam->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        }
        
        if (instrucaoParam->oper1 == NULL || instrucaoParam->oper2 == NULL || instrucaoParam->oper3 == NULL) {
             fprintf(stderr, "Falha ao criar um dos endereços para PARAM na chamada de %s, linha %d. O parâmetro pode não ter sido processado corretamente.\n", tree->attr.name, argNode->lineno);
             // Limpeza dos operandos que possam ter sido alocados
             if(instrucaoParam->oper1) { if(instrucaoParam->oper1->tipo == String && instrucaoParam->oper1->nome) free(instrucaoParam->oper1->nome); free(instrucaoParam->oper1); instrucaoParam->oper1 = NULL;}
             if(instrucaoParam->oper2) { if(instrucaoParam->oper2->tipo == String && instrucaoParam->oper2->nome) free(instrucaoParam->oper2->nome); free(instrucaoParam->oper2); instrucaoParam->oper2 = NULL;}
             if(instrucaoParam->oper3) { if(instrucaoParam->oper3->tipo == String && instrucaoParam->oper3->nome) free(instrucaoParam->oper3->nome); free(instrucaoParam->oper3); instrucaoParam->oper3 = NULL;}
             free(instrucaoParam); // Libera a instrução PARAM em si
             
             // Libera a instrução CALL e retorna, pois a chamada está incompleta/corrompida
             if(instrucaoCall->oper1 && instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome); 
             if(instrucaoCall->oper1) free(instrucaoCall->oper1);
            // oper2 e oper3 de instrucaoCall ainda não foram alocados neste ponto do loop,
            // mas é bom verificar se a lógica mudar no futuro.
             if(instrucaoCall->oper2) free(instrucaoCall->oper2);
             if(instrucaoCall->oper3) free(instrucaoCall->oper3);
             free(instrucaoCall);
             return;
        }

        if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
            fprintf(stderr, "Limite do código intermediário atingido em parâmetro de chamada para %s, linha %d.\n", tree->attr.name, argNode->lineno);
            if(instrucaoParam->oper1) { if(instrucaoParam->oper1->tipo == String && instrucaoParam->oper1->nome) free(instrucaoParam->oper1->nome); free(instrucaoParam->oper1); }
            if(instrucaoParam->oper2) { if(instrucaoParam->oper2->tipo == String && instrucaoParam->oper2->nome) free(instrucaoParam->oper2->nome); free(instrucaoParam->oper2); }
            if(instrucaoParam->oper3) { if(instrucaoParam->oper3->tipo == String && instrucaoParam->oper3->nome) free(instrucaoParam->oper3->nome); free(instrucaoParam->oper3); }
            free(instrucaoParam);
            if(instrucaoCall->oper1 && instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome); 
            if(instrucaoCall->oper1) free(instrucaoCall->oper1);
            // oper2 e oper3 de instrucaoCall ainda não foram alocados
            free(instrucaoCall);
            return;
        }
        intermediateCode[adressCounter++] = instrucaoParam;
        
        argNode = argNode->sibling;
        numParam++;
    }
    
    instrucaoCall->oper2 = criaEndereco(IntConst, numParam, NULL, 0);
    if (instrucaoCall->oper2 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para numParam (CALL oper2) para %s, linha %d.\n", tree->attr.name, tree->lineno);
        if(instrucaoCall->oper1 && instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome); free(instrucaoCall->oper1);
        free(instrucaoCall);
        return;
    }
    
    PnoIdentificador funcInfo = buscaIdentificadorTabela(symbTable, tree->attr.name, "global");
    
    if (funcInfo != NULL) {
        if (funcInfo->tipoDado == Void) {
            instrucaoCall->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        } else if (funcInfo->tipoDado == Integer) {
            numReg = verificacaoRegistradores(NULL, NULL, 1); 
            instrucaoCall->oper3 = criaEndereco(IntConst, numReg, NULL, 1); 
        } else {
            fprintf(stderr, "Função '%s' (linha %d) na tabela de símbolos com tipo de dado desconhecido %d. Assumindo retorno void.\n", tree->attr.name, tree->lineno, funcInfo->tipoDado);
            instrucaoCall->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        }
    } else {
        fprintf(stderr, "Função '%s' (linha %d) não encontrada na tabela de símbolos. Assumindo retorno void.\n", tree->attr.name, tree->lineno);
        instrucaoCall->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    }

    if (instrucaoCall->oper3 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para valor de retorno (CALL oper3) para %s, linha %d.\n", tree->attr.name, tree->lineno);
        if(instrucaoCall->oper1 && instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome); free(instrucaoCall->oper1);
        if(instrucaoCall->oper2) free(instrucaoCall->oper2);
        free(instrucaoCall);
        return;
    }

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido em chamada de função %s, linha %d.\n", tree->attr.name, tree->lineno);
        if(instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome); free(instrucaoCall->oper1);
        if(instrucaoCall->oper2) free(instrucaoCall->oper2);
        if(instrucaoCall->oper3) free(instrucaoCall->oper3); 
        free(instrucaoCall);
        return;
    }
    intermediateCode[adressCounter++] = instrucaoCall;
}

void criarCodigoIntermediario(TreeNode *tree, PnoIdentificador* symbTable, int flagSibling) {

    if (tree == NULL) {
        return;
    }

    switch (tree->nodekind) {
        case StatementK:
            switch(tree->kind.stmt) {
                case IfK:
                    insertDeclarationIf(tree, symbTable);
                    break;
                case WhileK:
                    insertDeclarationWhile(tree, symbTable);
                    break;
                case ReturnINT:
                case ReturnVOID:
                    insertDeclarationReturn(tree, symbTable);
                    break;
                case VarDeclK:
                case VetDeclK:
                    insertDeclarationVar(tree, symbTable);
                    break;
                case FunDeclK:
                    insertDeclarationFunc(tree, symbTable);
                    break;
                case NuloDecl: 
                    if (tree->child[0] != NULL) { 
                        criarCodigoIntermediario(tree->child[0], symbTable, 0);
                    }
                    if (tree->child[1] != NULL) { 
                        criarCodigoIntermediario(tree->child[1], symbTable, 0);
                    }
                    break;
                default:
                    break;
            }
            break;
        case ExpressionK:
            switch(tree->kind.exp) {
                case OpK:
                    insertExpressionOp(tree, symbTable);
                    break;
                case OpRel:
                    insertExpressionRel(tree, symbTable);
                    break;
                case ConstK:
                    insertExpressionConst(tree, symbTable);
                    break;
                case IdK:
                case VetorK:
                    insertExpressionId(tree, symbTable);
                    break;
                case AtivK: 
                    insertExpressionCall(tree, symbTable);
                    break;
                case AssignK:
                    insertDeclarationAssign(tree, symbTable); 
                    break;
                default:
                    fprintf(stderr, "Geração de Código: Tipo de nó de expressão %d desconhecido, linha %d.\n", tree->kind.exp, tree->lineno); 
                    break;
            }
            break;
        default:
            fprintf(stderr, "Geração de Código: Tipo de nó %d desconhecido, linha %d.\n",tree->nodekind, tree->lineno);
            break;
    }

    if(!flagSibling) {
        criarCodigoIntermediario(tree->sibling, symbTable, 0); 
    }
}

int createIntermediateCode(TreeNode *tree, PnoIdentificador* symbTable){

    if(tree == NULL || symbTable == NULL) {
        fprintf(stderr, "Geração de Código: Árvore sintática ou tabela de símbolos nula.\n");
        return -2;
    }

    int checkErrorAllocatedMemoryIC = initIntermediateCode();

    if(checkErrorAllocatedMemoryIC == -1) {
        fprintf(stderr, "Geração de Código: Falha ao alocar memória para código intermediário.\n");
        return -1;
    }

    criarCodigoIntermediario(tree, symbTable, 0);

    return 0; 
}

const char* getTACOperationName(typeOperations op) {
    switch (op) {
        // Operações aritméticas
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MULT: return "MULT";
        case DIV: return "DIV";
        // Operações relacionais
        case LT: return "LT";
        case LTE: return "LTE";
        case GT: return "GT";
        case GTE: return "GTE";
        case EQ: return "EQ";
        case NEQ: return "NEQ";
        // Atribuição
        case ASSIGN: return "ASSIGN";
        // Controle de fluxo
        case LABEL: return "LABEL";
        case GOTO: return "GOTO";
        case IF: return "IF";
        case IFFALSE: return "IFFALSE";
        case RETURN: return "RETURN";
        case PARAM: return "PARAM";
        case CALL: return "CALL";
        case ARG: return "ARG";
        case FUNC: return "FUN";
        case END: return "END";
        case ARRAY_INDEX: return "ARRAY_INDEX";
        case ARRAY_ASSIGN: return "ARRAY_ASSIGN";
        // Novas operações mapeadas
        case ALLOC: return "ALLOC";
        case LOADI: return "LOADI";
        case LOAD: return "LOAD";
        case STORE: return "STORE";
        // Outros
        case OP_DESCON: return "OP_DESCON";
        case HALT: return "HALT";
        default:
            return "UNKNOWN_OP";
    }
}

typeOperations mapOperatorToTAC(int op) {
    switch (op) {
        case PLUS_OP: return ADD;
        case MINUS_OP: return SUB;
        case MULT_OP: return MULT;
        case DIV_OP: return DIV;
        case LT_OP: return LT;
        case LTE_OP: return LTE;
        case GT_OP: return GT;
        case GTE_OP: return GTE;
        case EQ_OP: return EQ;
        case NEQ_OP: return NEQ;
        case ASSIGN_OP: return ASSIGN;
        default: return OP_DESCON; 
    }
}

void imprimeCodigoIntermediario() {    
    for(int i = 0; i < MAX_LEN_CODE_INTERMEDIATE && intermediateCode[i] != NULL; i++){
        
        fprintf(yyout, "(%s, ", getTACOperationName(intermediateCode[i]->operation));
        
        if(intermediateCode[i]->oper1 != NULL){
            if(intermediateCode[i]->oper1->tipo == IntConst){
                if(intermediateCode[i]->oper1->boolReg == 1){
                    fprintf(yyout, "%s, ", getRegisterName(intermediateCode[i]->oper1->val));
                }
                else if(intermediateCode[i]->oper1->boolReg == 2){
                    fprintf(yyout, "L%d, ", intermediateCode[i]->oper1->val);
                }
                else{
                    fprintf(yyout, "%d, ", intermediateCode[i]->oper1->val);
                }
            }
            else if(intermediateCode[i]->oper1->tipo == String){
                fprintf(yyout, "%s, ", intermediateCode[i]->oper1->nome);
            }
            else{
                fprintf(yyout, "-, ");
            }
        }
        else{
            fprintf(yyout, "-, ");
        }
        if(intermediateCode[i]->oper2 != NULL){
            if(intermediateCode[i]->oper2->tipo == IntConst){
                if(intermediateCode[i]->oper2->boolReg == 1){
                    fprintf(yyout, "%s, ", getRegisterName(intermediateCode[i]->oper2->val));
                }
                else if(intermediateCode[i]->oper2->boolReg == 2){
                    fprintf(yyout, "L%d, ", intermediateCode[i]->oper2->val);
                }
                else{
                    fprintf(yyout, "%d, ", intermediateCode[i]->oper2->val);
                }
            }
            else if(intermediateCode[i]->oper2->tipo == String){
                fprintf(yyout, "%s, ", intermediateCode[i]->oper2->nome);
            }
            else{
                fprintf(yyout, "-, ");
            }
        }
        else{
            fprintf(yyout, "-, ");
        }
        if(intermediateCode[i]->oper3 != NULL){
            if(intermediateCode[i]->oper3->tipo == IntConst){
                if(intermediateCode[i]->oper3->boolReg == 1)
                    fprintf(yyout, "%s", getRegisterName(intermediateCode[i]->oper3->val));
                else
                    fprintf(yyout, "%d", intermediateCode[i]->oper3->val);
            }
            else if(intermediateCode[i]->oper3->tipo == String){
                fprintf(yyout, "%s", intermediateCode[i]->oper3->nome);
            }
            else{
                fprintf(yyout, "-");
            }
        }
        else{
            fprintf(yyout, "-");
        }

        fprintf(yyout, ")\n");
    }
}


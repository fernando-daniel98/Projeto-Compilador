#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/globals.h"
#include "../include/codeGen.h"
#include "../include/syntax_tree.h"
#include "../include/symbol_table.h"
#include "../include/util.h"

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

    // initRegs();
}

// Função para desalocar o código intermediário
void freeIntermediateCode() {
    if (intermediateCode == NULL) {
        return; 
    }

    for (int i = 0; i < adressCounter; i++) {
        if (intermediateCode[i] != NULL) {
            // Liberar operando 1
            if (intermediateCode[i]->oper1 != NULL) {
                if (intermediateCode[i]->oper1->tipo == String && intermediateCode[i]->oper1->nome != NULL) {
                    free(intermediateCode[i]->oper1->nome);
                    intermediateCode[i]->oper1->nome = NULL;
                }
                free(intermediateCode[i]->oper1);
                intermediateCode[i]->oper1 = NULL;
            }

            // Liberar operando 2
            if (intermediateCode[i]->oper2 != NULL) {
                if (intermediateCode[i]->oper2->tipo == String && intermediateCode[i]->oper2->nome != NULL) {
                    free(intermediateCode[i]->oper2->nome);
                    intermediateCode[i]->oper2->nome = NULL;
                }
                free(intermediateCode[i]->oper2);
                intermediateCode[i]->oper2 = NULL;
            }

            // Liberar operando 3
            if (intermediateCode[i]->oper3 != NULL) {
                if (intermediateCode[i]->oper3->tipo == String && intermediateCode[i]->oper3->nome != NULL) {
                    free(intermediateCode[i]->oper3->nome);
                    intermediateCode[i]->oper3->nome = NULL;
                }
                free(intermediateCode[i]->oper3);
                intermediateCode[i]->oper3 = NULL;
            }

            // Liberar a quádrupla em si
            free(intermediateCode[i]);
            intermediateCode[i] = NULL; // Definir como NULL no array
        }
    }
    
    // Liberar o array de ponteiros para quádruplas
    free(intermediateCode);
    intermediateCode = NULL; 

    // Resetar contadores globais
    adressCounter = 0;
}

void freeIntermediateCode2(quadruple **intermediateCode) {
    for (int i = 0; i < MAX_LEN_CODE_INTERMEDIATE && intermediateCode[i] != NULL; i++) {
        if (intermediateCode[i] != NULL) {
            if (intermediateCode[i]->oper1 != NULL) {
                if (intermediateCode[i]->oper1->tipo == String && intermediateCode[i]->oper1->nome != NULL) {
                    free(intermediateCode[i]->oper1->nome);
                }
                free(intermediateCode[i]->oper1);
            }
            if (intermediateCode[i]->oper2 != NULL) {
                if (intermediateCode[i]->oper2->tipo == String && intermediateCode[i]->oper2->nome != NULL) {
                    free(intermediateCode[i]->oper2->nome);
                }
                free(intermediateCode[i]->oper2);
            }
            if (intermediateCode[i]->oper3 != NULL) {
                if (intermediateCode[i]->oper3->tipo == String && intermediateCode[i]->oper3->nome != NULL) {
                    free(intermediateCode[i]->oper3->nome);
                }
                free(intermediateCode[i]->oper3);
            }
            free(intermediateCode[i]);
        }
    }
    free(intermediateCode);
}

// Função que inicia o código intermediário
int initIntermediateCode() {
    alocateIntermediateCode();

    return intermediateCode == NULL ? -1 : 0;

    // initRegisters();
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

    instrucao->operation = op_enum; // Atribui o enum diretamente
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

    // 1. Label para o início da avaliação da condição (L_cond)
    label_cond_quad = criaInstrucao(LABEL);
    label_cond_quad->oper1 = criaEndereco(IntConst, label_cond_num, NULL, 2); // boolReg = 2 para Label
    label_cond_quad->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    label_cond_quad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    intermediateCode[adressCounter++] = label_cond_quad;

    // 2. Gera código para a CONDIÇÃO (tree->child[0])
    criarCodigoIntermediario(tree->child[0], symbTable, 1);
    reg_cond_result = numReg; 

    // 3. Cria a instrução IFFFALSE
    iff_quad = criaInstrucao(IFFALSE); 
    iff_quad->oper1 = criaEndereco(IntConst, reg_cond_result, NULL, 1); 
    iff_quad->oper2 = criaEndereco(IntConst, label_fim_loop_num, NULL, 2); 
    iff_quad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    intermediateCode[adressCounter++] = iff_quad;

    // 4. Gera código para o CORPO do loop (tree->child[1])
    criarCodigoIntermediario(tree->child[1], symbTable, 1); 

    // 5. Adiciona GOTO para L_cond (volta para reavaliar a condição)
    goto_cond_quad = criaInstrucao(GOTO);
    goto_cond_quad->oper1 = criaEndereco(IntConst, label_cond_num, NULL, 2); 
    goto_cond_quad->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    goto_cond_quad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    intermediateCode[adressCounter++] = goto_cond_quad;

    // 6. Adiciona o rótulo para o fim do loop (L_fim_loop)
    label_fim_loop_quad = criaInstrucao(LABEL);
    label_fim_loop_quad->oper1 = criaEndereco(IntConst, label_fim_loop_num, NULL, 2); // boolReg = 2 para Label
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
    else { // ReturnVOID
        instrucaoReturn->oper1 = criaEndereco(Vazio, 0, NULL, 0);
    }

    instrucaoReturn->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    instrucaoReturn->oper3 = criaEndereco(Vazio, 0, NULL, 0);

    intermediateCode[adressCounter++] = instrucaoReturn;
}

void insertDeclarationVar(TreeNode *tree, PnoIdentificador* symbTable) {
    quadruple* instrucaoVar = NULL;
    quadruple* param = NULL;

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

    // Busca o nó na tabela de símbolos para saber o escopo
    PnoIdentificador no = buscaIdentificadorTabela(symbTable, tree->attr.name, funcName); 

    if(no == NULL) {
        // Tenta no escopo global se não achou no da função
        no = buscaIdentificadorTabela(symbTable, tree->attr.name, "global");
    }

    if(no == NULL) {
        // Assume escopo da função atual se não encontrado
        instrucaoVar->oper2 = criaEndereco(String, 0, funcName, 0);
        fprintf(stderr, "Variável '%s' não encontrada na tabela de símbolos (linha %d). Assumindo escopo '%s'.\n", tree->attr.name, tree->lineno, funcName);
    }
    else if (strcmp(no->escopo, "global") == 0){
        instrucaoVar->oper2 = criaEndereco(String, 0, "global", 0);
    }
    else {
        instrucaoVar->oper2 = criaEndereco(String, 0, no->escopo, 0); // Usa o escopo encontrado
    }


    instrucaoVar -> oper3 = criaEndereco(Vazio, 0, NULL, 0);
    if (instrucaoVar->oper3 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para oper3 em insertDeclarationVar.\n");
        if(instrucaoVar->oper1->nome) free(instrucaoVar->oper1->nome);
        free(instrucaoVar->oper1);
        if(instrucaoVar->oper2 && instrucaoVar->oper2->nome) free(instrucaoVar->oper2->nome);
        free(instrucaoVar->oper2); // Supondo que oper2 pode ter sido alocado e precisa ser liberado
        free(instrucaoVar);
        return;
    }


    if(tree -> kind.stmt == VetDeclK) {
        // Tamanho do vetor em child[0]
        if (tree->child[0] != NULL && tree->child[0]->kind.exp == ConstK) {
             instrucaoVar->oper3->tipo = IntConst;
             instrucaoVar->oper3->val = tree->child[0]->attr.val;
        } else {
             fprintf(stderr, "Declaração de vetor '%s' (linha %d) sem tamanho constante.\n", tree->attr.name, tree->lineno);
             instrucaoVar->oper3->tipo = IntConst;
             instrucaoVar->oper3->val = 1; // Tamanho padrão ou indicativo de erro
        }
    }

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido em insertDeclarationVar.\n");
        // Libera recursos alocados para instrucaoVar
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
    quadruple* label_else_inicio = NULL; // Label para o início do bloco ELSE
    quadruple* label_fim_if = NULL;     // Label para o fim de toda a estrutura IF

    int else_inicio_label_num;
    int fim_if_label_num;

    // 1. Gera código para a CONDIÇÃO (tree->child[0])
    criarCodigoIntermediario(tree->child[0], symbTable, 1);

    // Label para onde o IFFFALSE (if-false) vai pular
    else_inicio_label_num = numLabel++; 

    // 2. Cria e adiciona a instrução IFFFALSE
    instrucaoIFF = criaInstrucao(IFFALSE);
    instrucaoIFF->oper1 = criaEndereco(IntConst, numReg, NULL, 1); 
    instrucaoIFF->oper2 = criaEndereco(IntConst, else_inicio_label_num, NULL, 2); 
    instrucaoIFF->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    intermediateCode[adressCounter++] = instrucaoIFF;

    // 3. Gera código para o bloco THEN (tree->child[1])
    criarCodigoIntermediario(tree->child[1], symbTable, 1);

    if(tree->child[2] != NULL){ // Se existe um bloco ELSE
        fim_if_label_num = numLabel++; // Label para o final de toda a instrução IF

        // 4a. GOTO para pular o bloco ELSE
        instrucaoGOTO = criaInstrucao(GOTO);
        instrucaoGOTO->oper1 = criaEndereco(IntConst, fim_if_label_num, NULL, 2);
        instrucaoGOTO->oper2 = criaEndereco(Vazio, 0, NULL, 0);
        instrucaoGOTO->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        intermediateCode[adressCounter++] = instrucaoGOTO;

        // 5a. LABEL para o início do bloco ELSE
        label_else_inicio = criaInstrucao(LABEL);
        label_else_inicio->oper1 = criaEndereco(IntConst, else_inicio_label_num, NULL, 2);
        label_else_inicio->oper2 = criaEndereco(Vazio, 0, NULL, 0);
        label_else_inicio->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        intermediateCode[adressCounter++] = label_else_inicio;

        // 6a. Gera código para o bloco ELSE (tree->child[2])
        criarCodigoIntermediario(tree->child[2], symbTable, 1);

        // 7a. Adiciona o LABEL para o fim de toda a estrutura IF
        label_fim_if = criaInstrucao(LABEL);
        label_fim_if->oper1 = criaEndereco(IntConst, fim_if_label_num, NULL, 2);
        label_fim_if->oper2 = criaEndereco(Vazio, 0, NULL, 0);
        label_fim_if->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        intermediateCode[adressCounter++] = label_fim_if;
    }
    else{ // Se NÃO existe um bloco ELSE
        // 4b. Adiciona o LABEL para o fim do IF
        label_fim_if = criaInstrucao(LABEL); 
        label_fim_if->oper1 = criaEndereco(IntConst, else_inicio_label_num, NULL, 2);
        label_fim_if->oper2 = criaEndereco(Vazio, 0, NULL, 0);
        label_fim_if->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        intermediateCode[adressCounter++] = label_fim_if;
    }
}

void insertDeclarationAssign(TreeNode *tree, PnoIdentificador* symbTable){
    TreeNode *lhs = tree->child[0]; // Lado esquerdo: IdK ou VetorK
    TreeNode *rhs = tree->child[1]; // Lado direito: Expressão
    quadruple* instrucaoStore = NULL;
    ENDERECO *op1_store = NULL, *op2_store = NULL, *op3_store = NULL;
    int reg_rhs_val;
    int reg_index_val;

    // 1. Gerar código para o lado direito (RHS). Resultado em 'numReg'.
    criarCodigoIntermediario(rhs, symbTable, 1);
    reg_rhs_val = numReg;

    // 2. Criar operando 2 do STORE (fonte dos dados).
    op2_store = criaEndereco(IntConst, reg_rhs_val, NULL, 1);
    if (op2_store == NULL) {
        fprintf(stderr, "Falha ao criar endereço para RHS em atribuição, linha %d.\n", tree->lineno);
        return;
    }

    // 3. Determinar operandos 1 e 3 do STORE com base no LHS.
    if (lhs->kind.exp == IdK) { // LHS é variável simples
        op1_store = criaEndereco(String, 0, lhs->attr.name, 0); 
        if (op1_store == NULL) {
            fprintf(stderr, "Falha ao criar endereço para LHS (IdK) em atribuição, linha %d.\n", tree->lineno);
            free(op2_store);
            return;
        }
        op3_store = criaEndereco(Vazio, 0, NULL, 0); // Sem índice
        if (op3_store == NULL) {
            fprintf(stderr, "Falha ao criar endereço vazio para LHS (IdK) em atribuição, linha %d.\n", tree->lineno);
            if(op1_store->nome) free(op1_store->nome);
            free(op1_store);
            free(op2_store);
            return;
        }
    } else if (lhs->kind.exp == VetorK) { // LHS é elemento de vetor
        op1_store = criaEndereco(String, 0, lhs->attr.name, 0); 
        if (op1_store == NULL) {
            fprintf(stderr, "Falha ao criar endereço para LHS (VetorK nome) em atribuição, linha %d.\n", tree->lineno);
            free(op2_store);
            return;
        }
        // Gerar código para a expressão do índice. Resultado em 'numReg'.
        criarCodigoIntermediario(lhs->child[0], symbTable, 1);
        reg_index_val = numReg;
        op3_store = criaEndereco(IntConst, reg_index_val, NULL, 1); // Registrador do índice
        if (op3_store == NULL) {
            fprintf(stderr, "Falha ao criar endereço para LHS (VetorK índice) em atribuição, linha %d.\n", tree->lineno);
            if(op1_store->nome) free(op1_store->nome);
            free(op1_store);
            free(op2_store);
            return;
        }
    } else {
        fprintf(stderr, "Lado esquerdo inválido na atribuição, linha %d.\n", tree->lineno);
        free(op2_store); 
        return;
    }

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido em insertDeclarationAssign.\n");
        // Libera recursos alocados
        if(instrucaoStore->oper1) free(instrucaoStore->oper1);
        if(instrucaoStore->oper2) free(instrucaoStore->oper2);
        if(instrucaoStore->oper3) free(instrucaoStore->oper3);
        free(instrucaoStore);
        return;
    }

    // 4. Criar a instrução STORE.
    instrucaoStore = criaInstrucao(STORE);
    if (instrucaoStore == NULL) {
        fprintf(stderr, "Falha ao criar instrução STORE em atribuição, linha %d.\n", tree->lineno);
        // Libera recursos alocados
        if(op1_store) { if(op1_store->nome) free(op1_store->nome); free(op1_store); }
        if(op2_store) free(op2_store);
        if(op3_store) free(op3_store);
        return;
    }

    instrucaoStore->oper1 = op1_store;
    instrucaoStore->oper2 = op2_store;
    instrucaoStore->oper3 = op3_store;

    // 5. Adicionar a instrução STORE ao código intermediário.
    intermediateCode[adressCounter++] = instrucaoStore;
}

void insertDeclarationFunc(TreeNode *tree, PnoIdentificador* symbolTable){
    // Processar parâmetros
    TreeNode* paramNode = tree->child[0];
    int numParams = 0;

    // Atualiza o nome da função atual
    strncpy(funcName, tree->attr.name, MAXTOKENLEN -1);
    funcName[MAXTOKENLEN -1] = '\0';

    while (paramNode != NULL && paramNode->kind.stmt != ParamVoid) { 
        numParams++;
        quadruple* argQuad = criaInstrucao(ARG);
        
        argQuad->oper1 = criaEndereco(String, 0, paramNode->attr.name, 0);
        
        if (paramNode->type == Integer) {
            argQuad->oper2 = criaEndereco(String, 0, "INT", 0);
        } else if (paramNode->type == Void) { 
            argQuad->oper2 = criaEndereco(String, 0, "VOID", 0);
        } else {
            argQuad->oper2 = criaEndereco(String, 0, "UNKNOWN_TYPE", 0); 
        }
        
        argQuad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        
        if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
            fprintf(stderr, "Limite do código intermediário atingido ao declarar argumento para %s.\n", tree->attr.name);
            // Libera recursos alocados
            if(argQuad->oper1) { if(argQuad->oper1->nome) free(argQuad->oper1->nome); free(argQuad->oper1); }
            if(argQuad->oper2) { if(argQuad->oper2->nome) free(argQuad->oper2->nome); free(argQuad->oper2); }
            if(argQuad->oper3) free(argQuad->oper3); // Vazio, sem nome
            free(argQuad);
            return;
        }
        intermediateCode[adressCounter++] = argQuad;
        paramNode = paramNode->sibling;
    }

    // Quádrupla da função
    quadruple* funcQuad = criaInstrucao(FUNC); 
    if (funcQuad == NULL) {
        fprintf(stderr, "Falha ao criar instrução FUNC para %s.\n", tree->attr.name);
        return;
    }

    // oper1: tipo de retorno da função (INT ou VOID)
    if (tree->type == Integer) {
        funcQuad->oper1 = criaEndereco(String, 0, "INT", 0);
    } else if (tree->type == Void) {
        funcQuad->oper1 = criaEndereco(String, 0, "VOID", 0);
    }

    // oper2: nome da função
    funcQuad->oper2 = criaEndereco(String, 0, tree->attr.name, 0);
    if (funcQuad->oper2 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para nome da função %s.\n", tree->attr.name);
        if(funcQuad->oper1 && funcQuad->oper1->nome) free(funcQuad->oper1->nome); free(funcQuad->oper1);
        free(funcQuad);
        return;
    }

    // oper3: número de parâmetros (calculado acima)
    funcQuad->oper3 = criaEndereco(IntConst, numParams, NULL, 0);
    if (funcQuad->oper3 == NULL) {
        fprintf(stderr, "Falha ao criar endereço para número de parâmetros da função %s.\n", tree->attr.name);
        if(funcQuad->oper1 && funcQuad->oper1->nome) free(funcQuad->oper1->nome); free(funcQuad->oper1);
        if(funcQuad->oper2 && funcQuad->oper2->nome) free(funcQuad->oper2->nome); free(funcQuad->oper2);
        free(funcQuad);
        return;
    }
    
    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido ao declarar função %s.\n", tree->attr.name);
        // Libera recursos alocados
        if(funcQuad->oper1 && funcQuad->oper1->nome) free(funcQuad->oper1->nome); free(funcQuad->oper1);
        if(funcQuad->oper2 && funcQuad->oper2->nome) free(funcQuad->oper2->nome); free(funcQuad->oper2);
        if(funcQuad->oper3) free(funcQuad->oper3);
        free(funcQuad);
        return;
    }
    intermediateCode[adressCounter++] = funcQuad;

    // Processa o corpo da função (child[1] -> NuloDecl)
    if (tree->child[1] != NULL) {
        if (tree->child[1]->child[0] != NULL) { // Declarações locais
            criarCodigoIntermediario(tree->child[1]->child[0], symbolTable, 0);
        }
        if (tree->child[1]->child[1] != NULL) { // Lista de instruções
            criarCodigoIntermediario(tree->child[1]->child[1], symbolTable, 0);
        }
    }

    // Adiciona a instrução END
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
        // Libera recursos alocados
        if(endFuncQuad->oper1->nome) free(endFuncQuad->oper1->nome); free(endFuncQuad->oper1);
        free(endFuncQuad->oper2);
        free(endFuncQuad->oper3);
        free(endFuncQuad);
        return;
    }
    intermediateCode[adressCounter++] = endFuncQuad;

    // Restaura o escopo para global
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

    // Avalia o primeiro operando (child[0])
    criarCodigoIntermediario(tree->child[0], symbTable, 1);
    reg_child0_val = numReg; 
    op->oper1 = criaEndereco(IntConst, reg_child0_val, NULL, 1);

    // Avalia o segundo operando (child[1]), se existir
    if (tree->child[1] != NULL) {
        criarCodigoIntermediario(tree->child[1], symbTable, 1);
        reg_child1_val = numReg; 
        op->oper2 = criaEndereco(IntConst, reg_child1_val, NULL, 1);
    } else {
        // Operador unário
        op->oper2 = criaEndereco(Vazio, 0, NULL, 0); 
    }

    // Novo registrador para o resultado
    reg_result_val = verificacaoRegistradores(NULL, NULL, 1); 
    numReg = reg_result_val; // Atualiza numReg global

    op->oper3 = criaEndereco(IntConst, reg_result_val, NULL, 1);

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido em operação, linha %d.\n", tree->lineno);
        // Libera recursos alocados
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

    // Avalia operandos
    criarCodigoIntermediario(tree->child[0], symbTable, 1);
    reg_child0_val = numReg; 

    criarCodigoIntermediario(tree->child[1], symbTable, 1);
    reg_child1_val = numReg; 

    op->oper1 = criaEndereco(IntConst, reg_child0_val, NULL, 1);
    op->oper2 = criaEndereco(IntConst, reg_child1_val, NULL, 1);

    // Resultado booleano em um registrador
    reg_result_val = verificacaoRegistradores(NULL, NULL, 1);
    numReg = reg_result_val; 

    op->oper3 = criaEndereco(IntConst, reg_result_val, NULL, 1);

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido em operação relacional, linha %d.\n", tree->lineno);
        // Libera recursos alocados
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
    
    // Valor da constante em tree->attr.val.
    
    // Otimização para constante 0 (comentada para garantir LOADI)
    // if(tree->attr.val == 0){ 
    //     numReg = 31; 
    //     return;
    // }

    numReg = verificacaoRegistradores(NULL, NULL, 1); // Novo registrador temporário
        
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
        // Libera recursos alocados
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
    
    if(tree->kind.exp == VetorK){
        instrucaoId = criaInstrucao(LOAD);

        criarCodigoIntermediario(tree->child[0], symbTable, 1); // Índice do vetor
        instrucaoId->oper3 = criaEndereco(IntConst, numReg, NULL, 1);
        aux = numReg;

        numReg = verificacaoRegistradores(NULL, NULL, 1); // Registrador para o valor carregado
        
        instrucaoId->oper1 = criaEndereco(IntConst, numReg, NULL, 1); 
        instrucaoId->oper2 = criaEndereco(String, 0, tree->attr.name, 0);

    }
    else if(tree->kind.exp == IdK){  
        PnoIdentificador varEscopo = buscaIdentificadorTabela(symbTable, tree->attr.name, funcName);
        if(varEscopo == NULL){
            varEscopo = buscaIdentificadorTabela(symbTable, tree->attr.name, "global");
        }

        if(varEscopo == NULL){
            printf(ANSI_COLOR_RED "ERRO: " ANSI_COLOR_RESET);
            printf("Escopo da variavel '%s' nao encontrada", tree->attr.name);
            numReg = -1; // Indica erro
        }
        else if(!strcmp(varEscopo->escopo, "global")){
            numReg = verificacaoRegistradores(tree->attr.name, "global", 0);
        }
        else{
            numReg = verificacaoRegistradores(tree->attr.name, funcName, 0);
        }
        
        instrucaoId = criaInstrucao(LOAD);
        instrucaoId->oper1 = criaEndereco(IntConst, numReg, NULL, 1);
        instrucaoId->oper2 = criaEndereco(String, 0, tree->attr.name, 0);
        instrucaoId->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    }

    if(instrucaoId != NULL && numReg != -1){ // Adiciona apenas se não houve erro de escopo
        intermediateCode[adressCounter++] = instrucaoId;
    } else if (instrucaoId != NULL && numReg == -1) {
        // Libera instrucaoId se foi alocada mas não será usada devido a erro
        if(instrucaoId->oper1) free(instrucaoId->oper1);
        if(instrucaoId->oper2 && instrucaoId->oper2->nome) free(instrucaoId->oper2->nome);
        if(instrucaoId->oper2) free(instrucaoId->oper2);
        if(instrucaoId->oper3) free(instrucaoId->oper3);
        free(instrucaoId);
    }
}

void insertExpressionCall(TreeNode *tree, PnoIdentificador* symbTable) {
    quadruple* instrucaoCall = NULL;
    quadruple* instrucaoParam = NULL;
    TreeNode* argNode = tree->child[0]; // Nó do argumento atual

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
            // Libera recursos alocados
            if(instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome);
            free(instrucaoCall->oper1);
            free(instrucaoCall);
            return;
        }
        
        // Verifica o tipo do nó do argumento
        if (argNode->nodekind == ExpressionK) {
            if (argNode->kind.exp == IdK) {
                // Argumento é IdK (variável simples ou array)
                PnoIdentificador idInfo = buscaIdentificadorTabela(symbTable, argNode->attr.name, funcName);
                if (idInfo == NULL) {
                    idInfo = buscaIdentificadorTabela(symbTable, argNode->attr.name, "global");
                }

                if (idInfo != NULL && (idInfo->tipoIdentificador == VetDeclK || idInfo->tipoIdentificador == VetParamK)) {
                    // Array sendo passado
                    instrucaoParam->oper1 = criaEndereco(String, 0, argNode->attr.name, 0); 
                    instrucaoParam->oper2 = criaEndereco(String, 0, "VET_PARAM", 0);      
                    instrucaoParam->oper3 = criaEndereco(Vazio, 0, NULL, 0);
                } else {
                    // Variável simples
                    criarCodigoIntermediario(argNode, symbTable, 1); 
                    instrucaoParam->oper1 = criaEndereco(IntConst, numReg, NULL, 1);
                    instrucaoParam->oper2 = criaEndereco(String, 0, "INT", 0); 
                    instrucaoParam->oper3 = criaEndereco(Vazio, 0, NULL, 0);
                }
            } else {
                // Argumento é expressão complexa
                criarCodigoIntermediario(argNode, symbTable, 1); 
                instrucaoParam->oper1 = criaEndereco(IntConst, numReg, NULL, 1);
                instrucaoParam->oper2 = criaEndereco(String, 0, "INT", 0); 
                instrucaoParam->oper3 = criaEndereco(Vazio, 0, NULL, 0);
            }
        } else {
            fprintf(stderr, "Argumento de função inválido (não é ExpressionK) para %s, linha %d.\n", tree->attr.name, argNode->lineno);
            criarCodigoIntermediario(argNode, symbTable, 1); 
            instrucaoParam->oper1 = criaEndereco(IntConst, numReg, NULL, 1);
            instrucaoParam->oper2 = criaEndereco(String, 0, "UNKNOWN_ARG_TYPE", 0);
            instrucaoParam->oper3 = criaEndereco(Vazio, 0, NULL, 0);
        }
        
        if (instrucaoParam->oper1 == NULL || instrucaoParam->oper2 == NULL || instrucaoParam->oper3 == NULL) {
             fprintf(stderr, "Falha ao criar um dos endereços para PARAM na chamada de %s, linha %d.\n", tree->attr.name, argNode->lineno);
             // Libera recursos alocados
             if(instrucaoParam->oper1) { if(instrucaoParam->oper1->tipo == String && instrucaoParam->oper1->nome) free(instrucaoParam->oper1->nome); free(instrucaoParam->oper1); }
             if(instrucaoParam->oper2) { if(instrucaoParam->oper2->tipo == String && instrucaoParam->oper2->nome) free(instrucaoParam->oper2->nome); free(instrucaoParam->oper2); }
             if(instrucaoParam->oper3) { if(instrucaoParam->oper3->tipo == String && instrucaoParam->oper3->nome) free(instrucaoParam->oper3->nome); free(instrucaoParam->oper3); }
             free(instrucaoParam);
             if(instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome); free(instrucaoCall->oper1);
             free(instrucaoCall);
             return;
        }

        if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
            fprintf(stderr, "Limite do código intermediário atingido em parâmetro de chamada para %s, linha %d.\n", tree->attr.name, argNode->lineno);
            // Libera recursos alocados
            if(instrucaoParam->oper1->tipo == String && instrucaoParam->oper1->nome) free(instrucaoParam->oper1->nome); free(instrucaoParam->oper1);
            if(instrucaoParam->oper2->tipo == String && instrucaoParam->oper2->nome) free(instrucaoParam->oper2->nome); free(instrucaoParam->oper2);
            if(instrucaoParam->oper3->tipo == String && instrucaoParam->oper3->nome) free(instrucaoParam->oper3->nome); free(instrucaoParam->oper3); // Embora Vazio aqui
            free(instrucaoParam);
            // Liberar instrucaoCall também
            if(instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome); free(instrucaoCall->oper1);
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
        // Libera recursos alocados
        if(instrucaoCall->oper1 && instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome); free(instrucaoCall->oper1);
        // oper3 ainda não alocado, instrucaoCall->oper2 é o que falhou
        free(instrucaoCall);
        return;
    }
    
    PnoIdentificador funcInfo = buscaIdentificadorTabela(symbTable, tree->attr.name, "global");
    
    // Determinar oper3 (valor de retorno)
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
        // Libera recursos alocados
        if(instrucaoCall->oper1 && instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome); free(instrucaoCall->oper1);
        if(instrucaoCall->oper2) free(instrucaoCall->oper2);
        // instrucaoCall->oper3 é o que falhou
        free(instrucaoCall);
        return;
    }

    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Limite do código intermediário atingido em chamada de função %s, linha %d.\n", tree->attr.name, tree->lineno);
        // Libera recursos alocados
        if(instrucaoCall->oper1->nome) free(instrucaoCall->oper1->nome); free(instrucaoCall->oper1);
        if(instrucaoCall->oper2) free(instrucaoCall->oper2);
        if(instrucaoCall->oper3) free(instrucaoCall->oper3); // oper3 pode ser IntConst ou Vazio
        free(instrucaoCall);
        return;
    }
    intermediateCode[adressCounter++] = instrucaoCall;
}

void criarCodigoIntermediario(TreeNode *tree, PnoIdentificador* symbTable, int flagCall) {

    if (tree == NULL) {
        return;
    }

    // Verifica o tipo de nó e insere a instrução correspondente
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
                case NuloDecl: // Bloco de código
                    if (tree->child[0] != NULL) { // Declarações locais
                        criarCodigoIntermediario(tree->child[0], symbTable, 0);
                    }
                    if (tree->child[1] != NULL) { // Lista de instruções
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
                case AtivK: // Chamada de função
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

    // Se flagCall for 0, processa os irmãos.
    if(!flagCall) {
        criarCodigoIntermediario(tree->sibling, symbTable, 0); 
    }
}

int createIntermediateCode(TreeNode *tree, PnoIdentificador* symbTable){

    if(tree == NULL || symbTable == NULL) {
        fprintf(stderr, "Geração de Código: Árvore sintática ou tabela de símbolos nula.\n");
        return -2;
    }

    // Inicializa o código intermediário
    int checkErrorAllocatedMemoryIC = initIntermediateCode();

    if(checkErrorAllocatedMemoryIC == -1) {
        fprintf(stderr, "Geração de Código: Falha ao alocar memória para código intermediário.\n");
        return -1;
    }

    // Insere as instruções no código intermediário
    // Chamada inicial para a lista de declarações globais (flagCall = 0)
    criarCodigoIntermediario(tree, symbTable, 0);

    return 0; // Success
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
        default: return OP_DESCON; // Operador desconhecido
    }
}

void imprimeCodigoIntermediario() {    
    for(int i = 0; i < MAX_LEN_CODE_INTERMEDIATE && intermediateCode[i] != NULL; i++){
        
        fprintf(yyout, "%s, ", getTACOperationName(intermediateCode[i]->operation));
        
        if(intermediateCode[i]->oper1 != NULL){
            if(intermediateCode[i]->oper1->tipo == IntConst){
                if(intermediateCode[i]->oper1->boolReg == 1){
                    fprintf(yyout, "$t%d, ", intermediateCode[i]->oper1->val);
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
                    fprintf(yyout, "$t%d, ", intermediateCode[i]->oper2->val);
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
                    fprintf(yyout, "$t%d\n", intermediateCode[i]->oper3->val);
                else
                    fprintf(yyout, "%d\n", intermediateCode[i]->oper3->val);
            }
            else if(intermediateCode[i]->oper3->tipo == String){
                fprintf(yyout, "%s\n", intermediateCode[i]->oper3->nome);
            }
            else{
                fprintf(yyout, "-\n");
            }
        }
        else{
            fprintf(yyout, "-\n");
        }
    }
}


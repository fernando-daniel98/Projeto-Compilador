#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/globals.h"
#include "../include/codeGen.h"
#include "../include/syntax_tree.h"
#include "../include/symbol_table.h"

// Estrutura para instrução de código intermediário
quadruple **intermediateCode = NULL;

int adressCounter = 0;
int numReg = 1;
int numLabel = 0;

quadruple *funcLabel = NULL;
char funcName[MAXTOKENLEN] = "global";


// Função para alocar os nós do código intermediário
quadruple **alocateIntermediateCode() {
    quadruple **codeIntermediate = (quadruple **) malloc(MAX_LEN_CODE_INTERMEDIATE * sizeof(quadruple *));

    for (int i = 0; i < MAX_LEN_CODE_INTERMEDIATE; i++) {
        codeIntermediate[i] = (quadruple *) malloc(sizeof(quadruple));
        codeIntermediate[i]->oper1 = NULL;
        codeIntermediate[i]->oper2 = NULL;
        codeIntermediate[i]->oper3 = NULL;
    }

    return codeIntermediate;
}

// Função para desalocar o código intermediário
void freeIntermediateCode(quadruple **codeIntermediate) {
    for (int i = 0; i < MAX_LEN_CODE_INTERMEDIATE; i++) {
        if (codeIntermediate[i] != NULL) {
            free(codeIntermediate[i]->oper1);
            free(codeIntermediate[i]->oper2);
            free(codeIntermediate[i]->oper3);
            free(codeIntermediate[i]);
        }
    }
    free(codeIntermediate);
}

// Função que inicia o código intermediário
int initIntermediateCode() {
    intermediateCode = alocateIntermediateCode();

    return intermediateCode == NULL ? -1 : 0;
}

// Cria um novo endereço 
ENDERECO* criaEndereco(tipoEndereco tipo, int val, char* nome, int boolReg) {
    ENDERECO* endereco = (ENDERECO*) malloc(sizeof(ENDERECO));
    
    if(tipo == IntConst) {
        endereco->tipo = IntConst;
        endereco->val = val;
        endereco->boolReg = boolReg;
        endereco->nome = NULL;
    }
    else if(tipo == String) {
        endereco->tipo = String;
        endereco->val = 0;
        endereco->nome = strdup(nome);
    }
    else {
        endereco->tipo = Vazio;
        endereco->val = 0;
        endereco->nome = NULL;
    }

    return endereco;
}

// Adiciona uma quadrupla ao código intermediário na posição atual
void adicionaQuadrupla(typeOperations op, ENDERECO* oper1, ENDERECO* oper2, ENDERECO* oper3) {
    if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
        fprintf(stderr, "Error: Código intermediário atingiu o tamanho máximo.\n");
        return;
    }
    
    intermediateCode[adressCounter]->operation = op;
    intermediateCode[adressCounter]->oper1 = oper1;
    intermediateCode[adressCounter]->oper2 = oper2;
    intermediateCode[adressCounter]->oper3 = oper3;
    
    adressCounter++;
}

void insertDeclarationIf(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple* instrucaoOp = NULL;
    quadruple* instrucaoGoto = NULL;
    quadruple* instrucaoLabel1 = NULL;
    quadruple* instrucaoLabel2 = NULL;
    quadruple* instrucaoLabel3 = NULL;
    quadruple* instrucaoIF = NULL;

    criarCodigoIntermediario(tree->child[0], symbTable, 1);

    /* Cria a instrucao para o IF false */
    instrucaoIF = criaInstrucao("IFF");
    instrucaoIF->oper1 = criaEndereco(IntConst, numReg, NULL, 1);
    instrucaoIF->oper2 = criaEndereco(IntConst, numLabel, NULL, 2);
    instrucaoIF->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    intermediateCode[adressCounter] = instrucaoIF;
    adressCounter++;

    /* Cria a instrucao para o label */
    instrucaoLabel1 = criaInstrucao("LABEL");
    instrucaoLabel1->oper1 = criaEndereco(IntConst, numLabel, NULL, 2);
    instrucaoLabel1->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    instrucaoLabel1->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    numLabel++;

    instrucaoGoto = criaInstrucao("GOTO");
    instrucaoGoto->oper1 = criaEndereco(IntConst, numLabel, NULL, 2);
    instrucaoGoto->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    instrucaoGoto->oper3 = criaEndereco(Vazio, 0, NULL, 0);

    instrucaoLabel2 = criaInstrucao("LABEL");
    instrucaoLabel2->oper1 = criaEndereco(IntConst, numLabel, NULL, 2);
    instrucaoLabel2->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    instrucaoLabel2->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    
    numLabel++;

    /* Avanca para o filho do IF caso a operacao seja verdadeira */
    criarCodigoIntermediario(tree->child[1], symbTable, 1);

    if(tree->child[2] != NULL){

        intermediateCode[adressCounter++] = instrucaoGoto;
        intermediateCode[adressCounter++] = instrucaoLabel1;

        /*  Avanca para o filho do ELSE caso a operacao seja falsa (Else) 
            Se nao tiver nenhum Else, ela logo ira dar um return de volta para 
            essa funcao */
        criarCodigoIntermediario(tree->child[2], symbTable, 1);

        intermediateCode[adressCounter++] = instrucaoLabel2;
    }
    else{
        intermediateCode[adressCounter++] = instrucaoLabel1;
    }
}

void insertDeclarationWhile(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple* instrucaoIFF = NULL;
    quadruple* instrucaoGOTO = NULL;
    quadruple* instrucaoLabel1 = NULL;
    quadruple* instrucaoLabel2 = NULL;
    
    instrucaoLabel1 = criaInstrucao("LABEL");
    instrucaoLabel1->oper1 = criaEndereco(IntConst, numLabel, NULL, 2);
    instrucaoLabel1->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    instrucaoLabel1->oper3 = criaEndereco(Vazio, 0, NULL, 0);

    intermediateCode[adressCounter++] = instrucaoLabel1;

    instrucaoGOTO = criaInstrucao("GOTO");
    instrucaoGOTO->oper1 = criaEndereco(IntConst, numLabel, NULL, 2);
    instrucaoGOTO->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    instrucaoGOTO->oper3 = criaEndereco(Vazio, 0, NULL, 0);

    numLabel++;

    instrucaoLabel2 = criaInstrucao("LABEL");
    instrucaoLabel2->oper1 = criaEndereco(IntConst, numLabel, NULL, 2);
    instrucaoLabel2->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    instrucaoLabel2->oper3 = criaEndereco(Vazio, 0, NULL, 0);
    
    numLabel++;

    /* Avanca para o filho do WHILE caso a operacao seja verdadeira */
    criarCodigoIntermediario(tree->child[0], symbTable, 1);

    instrucaoIFF = criaInstrucao("IFF");
    
    instrucaoIFF->oper2 = criaEndereco(IntConst, numLabel-1, NULL, 2);
    instrucaoIFF->oper3 = criaEndereco(Vazio, 0, NULL, 0);

    criarCodigoIntermediario(tree->child[1], symbTable, 1);

    instrucaoIFF->oper1 = criaEndereco(IntConst, numReg, NULL, 1);

    intermediateCode[adressCounter++] = instrucaoIFF;

    criarCodigoIntermediario(tree->child[1], symbTable, 1);

    intermediateCode[adressCounter++] = instrucaoGOTO;

    intermediateCode[adressCounter++] = instrucaoLabel2;
}

void insertDeclarationReturn(TreeNode *tree, PnoIdentificador* symbTable) {
    quadruple* instrucaoReturn = NULL;
    quadruple* goTo = NULL;

    instrucaoReturn = criaInstrucao("RETURN");
    
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

    goTo = criaInstrucao("GOTO");
    goTo->oper1 = criaEndereco(IntConst, numLabel, NULL, 2);
    goTo->oper2 = criaEndereco(Vazio, 0, NULL, 0);
    goTo->oper3 = criaEndereco(Vazio, 0, NULL, 0);

    intermediateCode[adressCounter++] = goTo;
}

void insertDeclarationVar(TreeNode *tree, PnoIdentificador* symbTable) {
    quadruple* instrucaoVar = NULL;
    quadruple* param = NULL;

    instrucaoVar = criaInstrucao("ALLOC");
    instrucaoVar ->oper1 = criaEndereco(String, 0, tree->attr.name, 0);

    // Busca o nó na tabela de símbolos para saber o escopo
    PnoIdentificador no = searchSymbolTable(symbTable, tree->attr.name);

    if(no == NULL) {
        instrucaoVar->oper2 = criaEndereco(String, 0, "escopo", 0);
    }
    else if (strcmp(no->escopo, "global") == 0){
        instrucaoVar->oper2 = criaEndereco(String, 0, "global", 0);
    }
    else {
        instrucaoVar->oper2 = criaEndereco(String, 0, funcName, 0);
    }

    instrucaoVar -> oper3 = criaEndereco(Vazio, 0, NULL, 0);

    if(tree -> kind.stmt == VetDeclK) {
        instrucaoVar->oper3->tipo = IntConst;
        instrucaoVar->oper3->val = atoi(tree->child[1]->attr.name); 
    }

    intermediateCode[adressCounter++] = instrucaoVar;
}

void insertExpressionOp(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple* op = NULL;
    char NomeOp[MAXTOKENLEN];

    strcpy(NomeOp, tree->attr.name);

    if(strcmp(NomeOp, "+") == 0){
        op = criaInstrucao("ADD");
    }
    else if(strcmp(NomeOp, "-") == 0){
        op = criaInstrucao("SUB");
    }
    else if(strcmp(NomeOp, "*") == 0){
        op = criaInstrucao("MULT");
    }
    else if(strcmp(NomeOp, "/") == 0){
        op = criaInstrucao("DIV");
    }

    criarCodigoIntermediario(tree->child[0], symbTable, 1);
    op->oper1 = criaEndereco(IntConst, numReg, NULL, 1);

    criarCodigoIntermediario(tree->child[1], symbTable, 1);
    op->oper2 = criaEndereco(IntConst, numReg, NULL, 1);

    numReg = verificacaoRegistradores(NULL, NULL, 1);

    op->oper3 = criaEndereco(IntConst, numReg, NULL, 1);

    intermediateCode[adressCounter++] = op;
}

void insertExpressionRel(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple * op = NULL;
    
    if(strcmp(tree->attr.name, "==") == 0){
        op = criaInstrucao("EQ");
    }
    else if(strcmp(tree->attr.name, "!=") == 0){
        op = criaInstrucao("NEQ");
    }
    else if(strcmp(tree->attr.name, ">") == 0){
        op = criaInstrucao("GT");
    }
    else if(strcmp(tree->attr.name, "<") == 0){
        op = criaInstrucao("LT");
    }
    else if(strcmp(tree->attr.name, ">=") == 0){
        op = criaInstrucao("GET");
    }
    else if(strcmp(tree->attr.name, "<=") == 0){
        op = criaInstrucao("LET");
    } 
    
    criarCodigoIntermediario(tree->child[0], symbTable, 1);
    op->oper1 = criaEndereco(IntConst, numReg, NULL, 1);
    
    criarCodigoIntermediario(tree->child[1], symbTable, 1);
    op->oper2 = criaEndereco(IntConst, numReg, NULL, 1);

    numReg = verificacaoRegistradores(NULL, NULL, 1);

    op->oper3 = criaEndereco(IntConst, numReg, NULL, 1);

    intermediateCode[adressCounter++] = op;
}

void inserExpressionConst(TreeNode *tree, PnoIdentificador* symbTable){
    quadruple * constant = NULL;

    if(strcmp(tree->attr.name, "0") == 0){
        numReg = 31;
        return;
    }

    /* Parei aqui */
    // numReg = 
}




// Função que insere as instruções no código intermediário
void insertQuadruple(TreeNode *tree, PnoIdentificador* symbTable, int flagCall) {
    if (tree == NULL) return;

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
                default:
                    fprintf(stderr, "Tipo de nó de declaração desconhecido, linha %d\n", tree->lineno);
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
                    fprintf(stderr, "Tipo de nó de expressão desconhecido, linha %d\n", tree->lineno);
                break;
            }
            break;
        default:
            fprintf(stderr, "Tipo de nó desconhecido, linha %d\n", tree->lineno);
            break;
    }

    if(!flagCall)
        insertQuadruple(tree->sibling, symbTable, 1);
    
}

// Função que insere os elementos ao código intermediário
int createIntermediateCode(TreeNode *tree, PnoIdentificador* symbTable){
    fprintf(stderr, "\nDEBUG: Iniciando geração de código intermediário\n");
    
    if(tree == NULL || symbTable == NULL) {
        fprintf(stderr, "Error: tree or symbolTable is NULL\n");
        return -2; // Error with AS or ST
    }

    // Inicializa o código intermediário
    fprintf(stderr, "DEBUG: Alocando memória para código intermediário\n");
    int checkErrorAllocatedMemoryIC = initIntermediateCode();

    if(checkErrorAllocatedMemoryIC == -1) {
        fprintf(stderr, "Error: Could not allocate memory for intermediate code\n");
        return -1; // Error in allocating memory
    }

    // Insere as instruções no código intermediário
    fprintf(stderr, "DEBUG: Iniciando percorrimento recursivo da árvore sintática\n");
    insertQuadruple(tree, symbTable, 1);
    fprintf(stderr, "DEBUG: Percorrimento da árvore sintática finalizado\n");

    return 0; // Success
}

// Função auxiliar para mapear OperatorType para typeOperations e retornar o nome da operação TAC
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
        case FUNC: return "FUNC";
        case END: return "END";
        case ARRAY_INDEX: return "ARRAY_INDEX";
        case ARRAY_ASSIGN: return "ARRAY_ASSIGN";
        // Outros
        case OP_DESCON: return "OP_DESCON (operador desconhecido)";
        default: return "defaultInGetTACOperationName";
    }
}

// Função para mapear OperatorType para typeOperations
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

// Função para imprimir as quadruplas geradas no código intermediário
void imprimeCodigoIntermediario() {
    fprintf(yyout, "\n============== Código Intermediário (Quadruplas) ===============\n");
    for(int i = 0; i < adressCounter; i++) {
        // Imprime número da instrução
        fprintf(yyout, "[%d] ", i);
        
        // Imprime a operação
        fprintf(yyout, "%s, ", getTACOperationName(intermediateCode[i]->operation));
        
        // Imprime o primeiro operando
        if(intermediateCode[i]->oper1 != NULL) {
            if(intermediateCode[i]->oper1->tipo == IntConst) {
                if(intermediateCode[i]->oper1->boolReg == 1) {
                    fprintf(yyout, "$t%d, ", intermediateCode[i]->oper1->val);
                } else if(intermediateCode[i]->oper1->boolReg == 2) {
                    fprintf(yyout, "L%d, ", intermediateCode[i]->oper1->val);
                } else {
                    fprintf(yyout, "%d, ", intermediateCode[i]->oper1->val);
                }
            } else if(intermediateCode[i]->oper1->tipo == String) {
                fprintf(yyout, "%s, ", intermediateCode[i]->oper1->nome);
            } else {
                fprintf(yyout, "-, ");
            }
        } else {
            fprintf(yyout, "-, ");
        }
        
        // Imprime o segundo operando
        if(intermediateCode[i]->oper2 != NULL) {
            if(intermediateCode[i]->oper2->tipo == IntConst) {
                if(intermediateCode[i]->oper2->boolReg == 1) {
                    fprintf(yyout, "$t%d, ", intermediateCode[i]->oper2->val);
                } else if(intermediateCode[i]->oper2->boolReg == 2) {
                    fprintf(yyout, "L%d, ", intermediateCode[i]->oper2->val);
                } else {
                    fprintf(yyout, "%d, ", intermediateCode[i]->oper2->val);
                }
            } else if(intermediateCode[i]->oper2->tipo == String) {
                fprintf(yyout, "%s, ", intermediateCode[i]->oper2->nome);
            } else {
                fprintf(yyout, "-, ");
            }
        } else {
            fprintf(yyout, "-, ");
        }
        
        // Imprime o terceiro operando
        if(intermediateCode[i]->oper3 != NULL) {
            if(intermediateCode[i]->oper3->tipo == IntConst) {
                if(intermediateCode[i]->oper3->boolReg == 1) {
                    fprintf(yyout, "$t%d\n", intermediateCode[i]->oper3->val);
                } else if(intermediateCode[i]->oper3->boolReg == 2) {
                    fprintf(yyout, "L%d\n", intermediateCode[i]->oper3->val);
                } else {
                    fprintf(yyout, "%d\n", intermediateCode[i]->oper3->val);
                }
            } else if(intermediateCode[i]->oper3->tipo == String) {
                fprintf(yyout, "%s\n", intermediateCode[i]->oper3->nome);
            } else {
                fprintf(yyout, "-\n");
            }
        } else {
            fprintf(yyout, "-\n");
        }
    }
    
    fprintf(yyout, "============== Fim do Código Intermediário ===============\n");
}
 
// Função para criar uma instrução de quadrupla com base no nome da operação
quadruple* criaInstrucao(char* operation) {
    quadruple* instrucao = (quadruple*) malloc(sizeof(quadruple));
    
    if (instrucao == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para instrucao\n");
        return NULL;
    }
    
    // Mapear string de operação para o tipo enum correspondente
    if (strcmp(operation, "ADD") == 0) {
        instrucao->operation = ADD;
    } else if (strcmp(operation, "SUB") == 0) {
        instrucao->operation = SUB;
    } else if (strcmp(operation, "MULT") == 0) {
        instrucao->operation = MULT;
    } else if (strcmp(operation, "DIV") == 0) {
        instrucao->operation = DIV;
    } else if (strcmp(operation, "LT") == 0) {
        instrucao->operation = LT;
    } else if (strcmp(operation, "LTE") == 0 || strcmp(operation, "LET") == 0) {
        instrucao->operation = LTE;
    } else if (strcmp(operation, "GT") == 0) {
        instrucao->operation = GT;
    } else if (strcmp(operation, "GTE") == 0 || strcmp(operation, "GET") == 0) {
        instrucao->operation = GTE;
    } else if (strcmp(operation, "EQ") == 0) {
        instrucao->operation = EQ;
    } else if (strcmp(operation, "NEQ") == 0) {
        instrucao->operation = NEQ;
    } else if (strcmp(operation, "ASSIGN") == 0) {
        instrucao->operation = ASSIGN;
    } else if (strcmp(operation, "LABEL") == 0) {
        instrucao->operation = LABEL;
    } else if (strcmp(operation, "GOTO") == 0) {
        instrucao->operation = GOTO;
    } else if (strcmp(operation, "IF") == 0) {
        instrucao->operation = IF;
    } else if (strcmp(operation, "IFFALSE") == 0 || strcmp(operation, "IFF") == 0) {
        instrucao->operation = IFFALSE;
    } else if (strcmp(operation, "RETURN") == 0) {
        instrucao->operation = RETURN;
    } else if (strcmp(operation, "PARAM") == 0) {
        instrucao->operation = PARAM;
    } else if (strcmp(operation, "CALL") == 0) {
        instrucao->operation = CALL;
    } else if (strcmp(operation, "ARG") == 0) {
        instrucao->operation = ARG;
    } else if (strcmp(operation, "FUNC") == 0) {
        instrucao->operation = FUNC;
    } else if (strcmp(operation, "END") == 0) {
        instrucao->operation = END;
    } else if (strcmp(operation, "ARRAY_INDEX") == 0) {
        instrucao->operation = ARRAY_INDEX;
    } else if (strcmp(operation, "ARRAY_ASSIGN") == 0) {
        instrucao->operation = ARRAY_ASSIGN;
    } else {
        instrucao->operation = OP_DESCON;
        fprintf(stderr, "Aviso: Operação desconhecida: %s\n", operation);
    }
    
    instrucao->oper1 = NULL;
    instrucao->oper2 = NULL;
    instrucao->oper3 = NULL;
    
    return instrucao;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/codeGen.h"
#include "../include/assembler.h"
#include "../include/memoria.h"
#include "../include/label.h"

ASSEMBLY **instrucoesAssembly = NULL;
int indiceAssembly = 0;
static char nomeFuncaoAtual[256] = "";

const char* getRegisterName(int regNum) {
    static char tempNames[4][32];
    static int nameIndex = 0;
    
    switch (regNum) {
        case 31: return "$zero";
        case 30: return "$ra";
        case 29: return "$fp";
        case 28: return "$sp";
        case 27: return "$temp";
        case 26: return "$pilha";
        case 25: return "$s2";
        case 24: return "$s1";
        case 23: return "$s0";       // CORREÇÃO: Adicionar $s0
        
        default:
            if (regNum >= 0 && regNum <= 22) {
                nameIndex = (nameIndex + 1) % 4;
                snprintf(tempNames[nameIndex], sizeof(tempNames[nameIndex]), "$t%d", regNum);
                return tempNames[nameIndex];
            } else {
                nameIndex = (nameIndex + 1) % 4;
                snprintf(tempNames[nameIndex], sizeof(tempNames[nameIndex]), "$reg%d", regNum);
                return tempNames[nameIndex];
            }
    }
}

void inicializaAssembly() {
    instrucoesAssembly = (ASSEMBLY **) malloc(MAX_ASSEMBLY * sizeof(ASSEMBLY *));
    for (int i = 0; i < MAX_ASSEMBLY; i++) {
        instrucoesAssembly[i] = NULL;
    }
    indiceAssembly = 0;
    strcpy(nomeFuncaoAtual, "");
    
    inicializaLabels();
}

ASSEMBLY* criarNoAssembly(tipoInstrucao tipo, char *nome) {
    ASSEMBLY* novaInstrucao = (ASSEMBLY*) malloc(sizeof(ASSEMBLY));
    novaInstrucao->tipo = tipo;
    
    novaInstrucao->tipoI = NULL;
    novaInstrucao->tipoR = NULL;
    novaInstrucao->tipoJ = NULL;
    novaInstrucao->tipoLabel = NULL;
    
    switch (tipo) {
        case typeR:
            novaInstrucao->tipoR = (TIPO_R*) malloc(sizeof(TIPO_R));
            novaInstrucao->tipoR->nome = strdup(nome);
            novaInstrucao->tipoR->rd = 0;
            novaInstrucao->tipoR->rs = 0;
            novaInstrucao->tipoR->rt = 0;
            break;
            
        case typeI:
            novaInstrucao->tipoI = (TIPO_I*) malloc(sizeof(TIPO_I));
            novaInstrucao->tipoI->nome = strdup(nome);
            novaInstrucao->tipoI->rs = 0;
            novaInstrucao->tipoI->rt = 0;
            novaInstrucao->tipoI->imediato = 0;
            novaInstrucao->tipoI->label = 0;
            break;
            
        case typeJ:
            novaInstrucao->tipoJ = (TIPO_J*) malloc(sizeof(TIPO_J));
            novaInstrucao->tipoJ->nome = strdup(nome);
            novaInstrucao->tipoJ->labelImediato = NULL;
            break;
            
        case typeLabel:
            novaInstrucao->tipoLabel = (TIPO_LABEL*) malloc(sizeof(TIPO_LABEL));
            novaInstrucao->tipoLabel->nome = strdup(nome);
            novaInstrucao->tipoLabel->endereco = 0;
            novaInstrucao->tipoLabel->boolean = 0;
            break;
    }
    
    return novaInstrucao;
}

int opAritmeticos(quadruple* instrucao, ASSEMBLY** novaInstrucao) {
    if (instrucao->operation == ADD) {
        *novaInstrucao = criarNoAssembly(typeR, "add");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
    }
    else if (instrucao->operation == SUB) {
        *novaInstrucao = criarNoAssembly(typeR, "sub");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
    }
    else if (instrucao->operation == MULT) {
        *novaInstrucao = criarNoAssembly(typeR, "mult");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
    }
    else if (instrucao->operation == DIV) {
        *novaInstrucao = criarNoAssembly(typeR, "div");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
    }
    else {
        return 0;
    }
    return 1;
}

int opRelacionais(quadruple* instrucao, ASSEMBLY** novaInstrucao) {
    if (instrucao->operation == EQ) {
        *novaInstrucao = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
        instrucoesAssembly[indiceAssembly++] = *novaInstrucao;

        *novaInstrucao = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper2->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper1->val;
        instrucoesAssembly[indiceAssembly++] = *novaInstrucao;
        *novaInstrucao = criarNoAssembly(typeI, "xori");
        (*novaInstrucao)->tipoI->rt = instrucao->oper3->val;
        (*novaInstrucao)->tipoI->rs = instrucao->oper3->val;
        (*novaInstrucao)->tipoI->imediato = 1;
    }
    else if (instrucao->operation == LT) {
        *novaInstrucao = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
    }
    else if (instrucao->operation == NEQ) {
        *novaInstrucao = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
        instrucoesAssembly[indiceAssembly++] = *novaInstrucao;

        *novaInstrucao = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper2->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper1->val;
    }
    else if (instrucao->operation == GT) {
        *novaInstrucao = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper2->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper1->val;
    }
    else if (instrucao->operation == GTE) {
        *novaInstrucao = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
        instrucoesAssembly[indiceAssembly++] = *novaInstrucao;

        int rd = (*novaInstrucao)->tipoR->rd;
        *novaInstrucao = criarNoAssembly(typeI, "xori");
        (*novaInstrucao)->tipoI->rt = rd;
        (*novaInstrucao)->tipoI->rs = rd;
        (*novaInstrucao)->tipoI->imediato = 1;
    }
    else if (instrucao->operation == LTE) {
        *novaInstrucao = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper2->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper1->val;
        instrucoesAssembly[indiceAssembly++] = *novaInstrucao;

        int rd = (*novaInstrucao)->tipoR->rd;
        *novaInstrucao = criarNoAssembly(typeI, "xori");
        (*novaInstrucao)->tipoI->rt = rd;
        (*novaInstrucao)->tipoI->rs = rd;
        (*novaInstrucao)->tipoI->imediato = 1;
    }
    else {
        return 0;
    }
    return 1;
}

// Função principal para gerar assembly completo
void geraAssemblyCompleto(quadruple* instrucao) {
    ASSEMBLY* novaInstrucao = NULL;
    
    if (instrucao == NULL) return;
    
    // Operações aritméticas
    if (opAritmeticos(instrucao, &novaInstrucao)) {
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // Operações relacionais
    else if (opRelacionais(instrucao, &novaInstrucao)) {    
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // ASSIGN (atribuição)
    else if (instrucao->operation == ASSIGN) {
        novaInstrucao = criarNoAssembly(typeR, "add");
        novaInstrucao->tipoR->rd = instrucao->oper1->val;    
        novaInstrucao->tipoR->rs = $zero;                    
        novaInstrucao->tipoR->rt = instrucao->oper2->val;    
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // LOADI (carregar imediato)
    else if (instrucao->operation == LOADI) {
        novaInstrucao = criarNoAssembly(typeI, "ori");
        novaInstrucao->tipoI->rt = instrucao->oper1->val;    
        novaInstrucao->tipoI->rs = $zero;                    
        novaInstrucao->tipoI->imediato = instrucao->oper2->val;
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else if (instrucao->operation == ALLOC) {
        if (instrucao->oper1 && instrucao->oper1->nome && funcaoAtual) {
            if (instrucao->oper3 && instrucao->oper3->val > 1) {
                int tamanho = instrucao->oper3->val;
                for (int i = 0; i < tamanho; i++) {
                    char* nome_posicao = malloc(strlen(instrucao->oper1->nome) + 20);
                    if (i == 0) {
                        strcpy(nome_posicao, instrucao->oper1->nome);
                    } else {
                        sprintf(nome_posicao, "%s[%d]", instrucao->oper1->nome, i);
                    }
                    insere_variavel(funcaoAtual, nome_posicao, vetor);
                }
            } else {
                insere_variavel(funcaoAtual, instrucao->oper1->nome, inteiro);
            }
        }
    }
    // FUNC (declaração de função) - precisa vir antes de ARG
    else if (instrucao->operation == FUNC) {
        if (instrucao->oper2 && instrucao->oper2->nome) {
            strcpy(nomeFuncaoAtual, instrucao->oper2->nome);
            novaInstrucao = criarNoAssembly(typeLabel, instrucao->oper2->nome);
            novaInstrucao->tipoLabel->boolean = 0; // é função (não label)
            
            // Adicionar função ao sistema de labels (mapear nome da função -> linha assembly)
            adicionarLabel(instrucao->oper2->nome, indiceAssembly);
            
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            
            // Atualizar função atual no sistema de memória
            funcaoAtual = insere_funcao(&vetorMemoria, instrucao->oper2->nome);
            
            // Se for main, inicializar registradores especiais usando cálculos dinâmicos
            if (strcmp(instrucao->oper2->nome, "main") == 0) {
                // ===============================================
                // INICIALIZAÇÃO DINÂMICA DA MAIN
                // CORREÇÃO: Inicializar $s0 primeiro (base do sistema)
                // ===============================================
                
                // NOVO: Inicializar $s0 com valor base do sistema (endereço base da memória)
                novaInstrucao = criarNoAssembly(typeI, "ori");
                novaInstrucao->tipoI->rt = $s0;      // $s0 = base do sistema
                novaInstrucao->tipoI->rs = $zero;    // $zero
                novaInstrucao->tipoI->imediato = 1000;  // endereço base da memória
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // Inicializar $fp dinamicamente
                // ori $fp $zero (tamanho_global + offset_fp)
                novaInstrucao = criarNoAssembly(typeI, "ori");
                novaInstrucao->tipoI->rt = $fp;
                novaInstrucao->tipoI->rs = $zero;
                // Cálculo dinâmico: tamanho das globais + offset da função atual
                int fp_value = buscar_funcao(&vetorMemoria, "global")->tamanho + get_fp(funcaoAtual);
                novaInstrucao->tipoI->imediato = fp_value;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // add $fp $fp $s0 (somar com base do sistema)
                novaInstrucao = criarNoAssembly(typeR, "add");
                novaInstrucao->tipoR->rd = $fp;
                novaInstrucao->tipoR->rs = $fp;
                novaInstrucao->tipoR->rt = $s0;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // Inicializar $sp dinamicamente
                // ori $sp $zero (tamanho_global + offset_sp)
                novaInstrucao = criarNoAssembly(typeI, "ori");
                novaInstrucao->tipoI->rt = $sp;
                novaInstrucao->tipoI->rs = $zero;
                int tamanho_global = buscar_funcao(&vetorMemoria, "global")->tamanho;
                int sp_base = 24;
                int sp_value = tamanho_global + sp_base;
                
                novaInstrucao->tipoI->imediato = sp_value;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // add $sp $sp $s0 (somar com base do sistema)
                novaInstrucao = criarNoAssembly(typeR, "add");
                novaInstrucao->tipoR->rd = $sp;
                novaInstrucao->tipoR->rs = $sp;
                novaInstrucao->tipoR->rt = $s0;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // Inicializar pilha de parâmetros dinamicamente
                // add $pilha $zero $s0 (definir base da pilha)
                novaInstrucao = criarNoAssembly(typeR, "add");
                novaInstrucao->tipoR->rd = $pilha;
                novaInstrucao->tipoR->rs = $zero;
                novaInstrucao->tipoR->rt = $s0;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // subi $pilha $pilha 5 (ajustar posição inicial da pilha)
                novaInstrucao = criarNoAssembly(typeI, "subi");
                novaInstrucao->tipoI->rt = $pilha;
                novaInstrucao->tipoI->rs = $pilha;
                novaInstrucao->tipoI->imediato = 5;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            } else {
                // ===============================================
                // INÍCIO DE FUNÇÃO REGULAR - Salvar $ra
                // Sempre salvar $ra em offset calculado
                // ===============================================
                
                // sw $ra offset($fp) - salva return address no frame atual
                VARIAVEL* var_ra = get_variavel(funcaoAtual, "Endereco Retorno");
                int offset_ra = var_ra ? get_fp_relation(funcaoAtual, var_ra) + instrucao->oper3->val : 4;
                
                novaInstrucao = criarNoAssembly(typeI, "sw");
                novaInstrucao->tipoI->rt = $ra;    // $ra (62) - return address a ser salvo
                novaInstrucao->tipoI->rs = $fp;    // $fp (61) - frame pointer base
                novaInstrucao->tipoI->imediato = offset_ra; // offset calculado dinamicamente
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            }
        }
    }
    // ARG (argumento de função)
    else if (instrucao->operation == ARG) {
        if (instrucao->oper2 && instrucao->oper2->nome && funcaoAtual) {
            TIPO_VAR tipo = inteiroArg;
            
            if (instrucao->oper1 && strcmp(instrucao->oper1->nome, "VET") == 0) {
                tipo = vetorArg;
            }
            
            insere_variavel(funcaoAtual, instrucao->oper2->nome, tipo);
        }
        
        // ARG é tratado em tempo de compilação
        // Não gerar código assembly - apenas configurar memória
    }
    else if (instrucao->operation == IFF) {
        novaInstrucao = criarNoAssembly(typeI, "beq");
        novaInstrucao->tipoI->rs = $zero;
        novaInstrucao->tipoI->rt = instrucao->oper1->val;
        novaInstrucao->tipoI->label = instrucao->oper2->val;
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // LABEL
    else if (instrucao->operation == LABEL) {
        
        char labelName[32];
        sprintf(labelName, "Label %d", instrucao->oper1->val);
        novaInstrucao = criarNoAssembly(typeLabel, labelName);
        novaInstrucao->tipoLabel->endereco = instrucao->oper1->val;
        novaInstrucao->tipoLabel->boolean = 1; // é label (não função)
        // Adicionar label ao sistema de labels (mapear nome da label -> linha assembly)
        adicionarLabel(labelName, indiceAssembly);
        
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // ===============================================
    // RETURN (retorno de função com valor)
    // ===============================================
    else if (instrucao->operation == RET) {
        if (strcmp(nomeFuncaoAtual, "main") != 0) {
            // ===============================================
            // SEQUÊNCIA SOFISTICADA DE RETORNO COM VALOR:
            // 1. Carregar endereço do vínculo de controle da função anterior
            // 2. Salvar valor de retorno no endereço correto do frame anterior
            // ===============================================
            
            // 1. lw $temp offset($fp) - carregar vínculo de controle da função anterior
            VARIAVEL* var_controle = get_variavel(funcaoAtual, "Vinculo Controle");
            int offset_controle = var_controle ? get_fp_relation(funcaoAtual, var_controle) : 0;
            
            novaInstrucao = criarNoAssembly(typeI, "lw");
            novaInstrucao->tipoI->rt = $temp;   // $temp (59) - receberá o endereço do frame anterior
            novaInstrucao->tipoI->rs = $fp;     // $fp (61) - frame pointer atual
            novaInstrucao->tipoI->imediato = offset_controle; // offset do vínculo de controle
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            
            // 2. sw valor_retorno 2($temp) - salvar valor no frame anterior
            // Offset 2 é a posição padrão para "Valor de Retorno" no frame anterior
            novaInstrucao = criarNoAssembly(typeI, "sw");
            novaInstrucao->tipoI->rs = $temp;   // $temp (59) - endereço base do frame anterior
            novaInstrucao->tipoI->rt = instrucao->oper1->val; // registrador com valor de retorno
            novaInstrucao->tipoI->imediato = 2; // offset 2 = posição "Valor de Retorno"
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
    }
    else if (instrucao->operation == PARAM) {
        // ===============================================
        // PARAM: Salvar parâmetro na pilha (endereço já foi carregado por LOAD)
        // ===============================================
        
        // Gerar instrução sw para salvar parâmetro na pilha
        novaInstrucao = criarNoAssembly(typeI, "sw");
        novaInstrucao->tipoI->rs = $pilha;
        novaInstrucao->tipoI->rt = instrucao->oper1->val;
        // Usar o tamanho atual da pilha de parâmetros como offset (ANTES de incrementar)
        novaInstrucao->tipoI->imediato = buscar_funcao(&vetorMemoria, "parametros")->tamanho;
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        
        // Incrementar contador de parâmetros APÓS usar o offset
        insere_variavel(buscar_funcao(&vetorMemoria, "parametros"), "Param", inteiro);
    }
    // LOAD (carregar de memória)
    else if (instrucao->operation == LOAD) {
        if (instrucao->oper3 && instrucao->oper3->nome && strcmp(instrucao->oper3->nome, "BASE") == 0) {
            // LOAD especial para carregar endereço base
            // Determinar se é variável global ou local ANTES de gerar instrução
            int offset = 0;
            if (instrucao->oper2 && instrucao->oper2->nome) {
                VARIAVEL* var = get_variavel(funcaoAtual, instrucao->oper2->nome);
                if (var) {
                    offset = get_fp_relation(funcaoAtual, var);
                    
                    novaInstrucao = criarNoAssembly(typeI, "addi");
                    novaInstrucao->tipoI->rt = instrucao->oper1->val;
                    novaInstrucao->tipoI->rs = var->bool_global ? $s0 : $fp;  // Escolher base correta
                    novaInstrucao->tipoI->imediato = offset;
                    instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                } else {
                    // Variável não encontrada, assumir local
                    novaInstrucao = criarNoAssembly(typeI, "addi");
                    novaInstrucao->tipoI->rt = instrucao->oper1->val;
                    novaInstrucao->tipoI->rs = $fp;
                    novaInstrucao->tipoI->imediato = 0;
                    instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                }
            } else {
                // Sem nome de variável, usar base padrão
                novaInstrucao = criarNoAssembly(typeI, "addi");
                novaInstrucao->tipoI->rt = instrucao->oper1->val;
                novaInstrucao->tipoI->rs = $fp;
                novaInstrucao->tipoI->imediato = 0;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            }
            return;
        }
        
        else if (instrucao->oper3 && 
            (instrucao->oper3->boolReg == 1 || 
             (instrucao->oper3->nome && strcmp(instrucao->oper3->nome, "-") != 0 && strcmp(instrucao->oper3->nome, "BASE") != 0))) {
            // ===============================================
            // LOAD INDEXADO A VETOR: var = array[index]
            // EXCLUINDO "BASE" que é tratado acima
            // ===============================================
            
            ASSEMBLY* loadBase = criarNoAssembly(typeI, "lw");
            loadBase->tipoI->rt = $temp;
            
            if (instrucao->oper2 && instrucao->oper2->nome) {
                VARIAVEL* var = get_variavel(funcaoAtual, instrucao->oper2->nome);
                if (var && var->bool_global) {
                    free(loadBase->tipoI->nome);
                    loadBase->tipoI->nome = strdup("addi");
                    loadBase->tipoI->rs = $s0;
                    loadBase->tipoI->imediato = 0;
                } else {
                    int offset = var ? get_fp_relation(funcaoAtual, var) : 0;
                    loadBase->tipoI->rs = $fp;
                    loadBase->tipoI->imediato = offset;
                }
            } else {
                loadBase->tipoI->rs = $fp;
                loadBase->tipoI->imediato = 0;
            }
            instrucoesAssembly[indiceAssembly++] = loadBase;
            
            // 2. Adicionar índice ao endereço base: add $temp, $temp, index
            ASSEMBLY* addIndex = criarNoAssembly(typeR, "add");
            addIndex->tipoR->rd = $temp;      // destino: $temp
            addIndex->tipoR->rs = $temp;      // operando 1: $temp (base)
            addIndex->tipoR->rt = instrucao->oper3->val; // operando 2: índice
            instrucoesAssembly[indiceAssembly++] = addIndex;
            
            novaInstrucao = criarNoAssembly(typeI, "lw");
            novaInstrucao->tipoI->rt = instrucao->oper1->val;
            novaInstrucao->tipoI->rs = $temp;
            novaInstrucao->tipoI->imediato = 0;
            
        } else {
            // ===============================================
            // LOAD SIMPLES: var = value ou reg = endereco_vetor
            // ===============================================
            
            // Calcular offset dinâmico baseado na variável
            int offset = 0;
            const char* instrucao_nome = "lw";  // padrão: load value
            
            if (instrucao->oper2 && instrucao->oper2->nome) {
                VARIAVEL* var = get_variavel(funcaoAtual, instrucao->oper2->nome);
                if (var) {
                    offset = get_fp_relation(funcaoAtual, var);
                    
                    // CORREÇÃO CRÍTICA: Para parâmetros vetoriais, carregar ENDEREÇO (addi) em vez de VALOR (lw)
                    if (var->tipo == vetorArg) {
                        instrucao_nome = "addi";  // carregar endereço para vetores passados como parâmetro
                    }
                    
                    novaInstrucao = criarNoAssembly(typeI, instrucao_nome);
                    novaInstrucao->tipoI->rt = instrucao->oper1->val;
                    novaInstrucao->tipoI->rs = var->bool_global ? $s0 : $fp;
                    novaInstrucao->tipoI->imediato = offset;
                } else {
                    // Variável não encontrada, usar load value padrão
                    novaInstrucao = criarNoAssembly(typeI, "lw");
                    novaInstrucao->tipoI->rt = instrucao->oper1->val;
                    novaInstrucao->tipoI->rs = $fp;
                    novaInstrucao->tipoI->imediato = 0;
                }
            } else {
                // Sem nome de variável, usar load value padrão
                novaInstrucao = criarNoAssembly(typeI, "lw");
                novaInstrucao->tipoI->rt = instrucao->oper1->val;
                novaInstrucao->tipoI->rs = $fp;
                novaInstrucao->tipoI->imediato = 0;
            }
            // IMPORTANTE: Adicionar instrução apenas para LOAD simples (não BASE nem indexado)
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
    }
    // STORE (salvar na memória)
    else if (instrucao->operation == STORE) {
        // Verificar se é acesso indexado a vetor (STORE var, value, index_reg)
        // Se oper3 existe e não é "-", então é acesso indexado
        if (instrucao->oper3 && 
            (instrucao->oper3->boolReg == 1 || 
             (instrucao->oper3->nome && strcmp(instrucao->oper3->nome, "-") != 0))) {
            // ===============================================
            // STORE INDEXADO A VETOR: a[index] = value
            // 1. lw $temp, offset($fp)  // carrega endereço base do array
            // 2. add $temp, $temp, index // adiciona índice ao endereço base  
            // 3. sw value, 0($temp)     // armazena valor na posição calculada
            // ===============================================
            
            // 1. Carregar endereço base do vetor em $temp
            ASSEMBLY* loadBase = criarNoAssembly(typeI, "lw");
            loadBase->tipoI->rt = $temp;  // $temp receberá a base do vetor
            
            if (instrucao->oper1 && instrucao->oper1->nome) {
                VARIAVEL* var = get_variavel(funcaoAtual, instrucao->oper1->nome);
                if (var && var->bool_global) {
                    // Para vetores globais, CALCULAR endereço base (não carregar conteúdo)
                    free(loadBase->tipoI->nome);  // libera o nome original ("lw")
                    loadBase->tipoI->nome = strdup("addi");
                    loadBase->tipoI->rs = $s0;
                    loadBase->tipoI->imediato = 0;
                } else {
                    // Para vetores locais/parâmetros, carregar endereço
                    int offset = var ? get_fp_relation(funcaoAtual, var) : 0;
                    loadBase->tipoI->rs = $fp;
                    loadBase->tipoI->imediato = offset;
                }
            } else {
                loadBase->tipoI->rs = $fp;
                loadBase->tipoI->imediato = 0;
            }
            instrucoesAssembly[indiceAssembly++] = loadBase;
            
            // 2. Adicionar índice ao endereço base: add $temp, $temp, index
            ASSEMBLY* addIndex = criarNoAssembly(typeR, "add");
            addIndex->tipoR->rd = $temp;      // destino: $temp
            addIndex->tipoR->rs = $temp;      // operando 1: $temp (base)
            addIndex->tipoR->rt = instrucao->oper3->val; // operando 2: índice
            instrucoesAssembly[indiceAssembly++] = addIndex;
            
            // 3. Armazenar valor na posição calculada: sw value, 0($temp)
            novaInstrucao = criarNoAssembly(typeI, "sw");
            novaInstrucao->tipoI->rt = instrucao->oper2->val;    // fonte (valor)
            novaInstrucao->tipoI->rs = $temp;                    // endereço calculado
            novaInstrucao->tipoI->imediato = 0;                  // offset 0
            
        } else {
            // ===============================================
            // STORE SIMPLES: var = value
            // ===============================================
            novaInstrucao = criarNoAssembly(typeI, "sw");
            novaInstrucao->tipoI->rt = instrucao->oper2->val;    // fonte
            
            // Calcular offset dinâmico baseado na variável
            int offset = 0;
            if (instrucao->oper1 && instrucao->oper1->nome) {
                VARIAVEL* var = get_variavel(funcaoAtual, instrucao->oper1->nome);
                if (var) {
                    offset = get_fp_relation(funcaoAtual, var);
                    novaInstrucao->tipoI->rs = var->bool_global ? $s0 : $fp;
                } else {
                    novaInstrucao->tipoI->rs = $fp;
                }
            } else {
                novaInstrucao->tipoI->rs = $fp;
            }
            
            novaInstrucao->tipoI->imediato = offset;
        }
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // GOTO (salto incondicional)
    else if (instrucao->operation == GOTO) {
        char labelName[32];
        sprintf(labelName, "Label %d", instrucao->oper1->val);
        novaInstrucao = criarNoAssembly(typeJ, "j");
        novaInstrucao->tipoJ->labelImediato = strdup(labelName);
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // CALL (chamada de função)
    else if (instrucao->operation == CALL) {
        if (instrucao->oper1 && instrucao->oper1->nome) {
            if (strcmp(instrucao->oper1->nome, "output") == 0) {
                // ===============================================
                // FUNÇÃO DE OUTPUT
                // ===============================================
                
                // Limpar temporários usados na chamada
                apagar_temp(buscar_funcao(&vetorMemoria, "parametros"));
                
                // lw $temp offset($pilha) - carregar parâmetro da pilha
                novaInstrucao = criarNoAssembly(typeI, "lw");
                novaInstrucao->tipoI->rt = $temp;
                novaInstrucao->tipoI->rs = $pilha;
                novaInstrucao->tipoI->imediato = buscar_funcao(&vetorMemoria, "parametros")->tamanho;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // out $zero $temp 0 - mostrar valor para o usuário
                novaInstrucao = criarNoAssembly(typeI, "out");
                novaInstrucao->tipoI->rs = $zero;
                novaInstrucao->tipoI->rt = $temp;
                novaInstrucao->tipoI->imediato = 0;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                return; // Não precisa fazer mais nada
            } else if (strcmp(instrucao->oper1->nome, "input") == 0) {
                // ===============================================
                // FUNÇÃO DE INPUT
                // ===============================================
                
                // in $reg $zero 0 - ler valor do usuário
                novaInstrucao = criarNoAssembly(typeI, "in");
                novaInstrucao->tipoI->rt = instrucao->oper3->val;  // registrador destino
                novaInstrucao->tipoI->rs = $zero;                  // fonte sempre $zero
                novaInstrucao->tipoI->imediato = 0;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                return; // Não precisa fazer mais nada
            } else {
                // ===============================================
                // CHAMADA DE FUNÇÃO
                // Sistema completo de transferência de parâmetros e controle
                // ===============================================
                
                // 1. TRANSFERIR PARÂMETROS DA PILHA PARA O STACK FRAME
                // Processar todos os parâmetros de trás para frente
                for(int i = instrucao->oper2->val; i > 0; i--) {
                    // Limpar temporários usados na chamada anterior
                    apagar_temp(buscar_funcao(&vetorMemoria, "parametros"));
                    
                    // lw $temp offset($pilha) - carregar parâmetro da pilha
                    novaInstrucao = criarNoAssembly(typeI, "lw");
                    novaInstrucao->tipoI->rt = $temp;
                    novaInstrucao->tipoI->rs = $pilha;
                    novaInstrucao->tipoI->imediato = buscar_funcao(&vetorMemoria, "parametros")->tamanho;
                    instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                    
                    // sw $temp i($sp) - salvar parâmetro no novo frame
                    novaInstrucao = criarNoAssembly(typeI, "sw");
                    novaInstrucao->tipoI->rs = $sp;
                    novaInstrucao->tipoI->rt = $temp;
                    novaInstrucao->tipoI->imediato = i;
                    instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                }
                
                // 2. CONFIGURAR VÍNCULO DE CONTROLE
                // add $temp $fp $zero - armazenar $fp atual em $temp
                novaInstrucao = criarNoAssembly(typeR, "add");
                novaInstrucao->tipoR->rt = $zero;  // $zero (63)
                novaInstrucao->tipoR->rs = $fp;    // $fp (61) - frame pointer atual
                novaInstrucao->tipoR->rd = $temp;  // $temp (59) - temporário
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // addi $temp $temp offset - ajustar para "Vínculo Controle"
                VARIAVEL* var_controle = get_variavel(funcaoAtual, "Vinculo Controle");
                int offset_controle = var_controle ? get_fp_relation(funcaoAtual, var_controle) : 0;
                novaInstrucao = criarNoAssembly(typeI, "addi");
                novaInstrucao->tipoI->rt = $temp;
                novaInstrucao->tipoI->rs = $temp;
                novaInstrucao->tipoI->imediato = offset_controle;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // sw $temp offset($sp) - salvar vínculo de controle no novo frame
                novaInstrucao = criarNoAssembly(typeI, "sw");
                novaInstrucao->tipoI->rt = $temp;
                novaInstrucao->tipoI->rs = $sp;
                novaInstrucao->tipoI->imediato = instrucao->oper2->val + 1;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // 3. CRESCER A PILHA DINAMICAMENTE
                // addi $fp $fp 25 - incrementar frame pointer
                int sp_base = 24;
                int incremento = sp_base + 1;
                
                novaInstrucao = criarNoAssembly(typeI, "addi");
                novaInstrucao->tipoI->rt = $fp;
                novaInstrucao->tipoI->rs = $fp;
                novaInstrucao->tipoI->imediato = incremento;  // 25
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // addi $sp $sp 25 - incrementar stack pointer
                novaInstrucao = criarNoAssembly(typeI, "addi");
                novaInstrucao->tipoI->rt = $sp;
                novaInstrucao->tipoI->rs = $sp;
                novaInstrucao->tipoI->imediato = incremento;  // 25
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // 4. CHAMAR A FUNÇÃO
                // jal função - jump and link (salva PC+1 em $ra automaticamente)
                novaInstrucao = criarNoAssembly(typeJ, "jal");
                novaInstrucao->tipoJ->labelImediato = strdup(instrucao->oper1->nome);
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // ===============================================
                // RETORNO DA FUNÇÃO - Restaurar estado anterior
                // ===============================================
                
                // 5. REDUZIR A PILHA DINAMICAMENTE
                // subi $fp $fp 25 - restaurar frame pointer anterior
                int decremento = 25;
                
                novaInstrucao = criarNoAssembly(typeI, "subi");
                novaInstrucao->tipoI->rt = $fp;
                novaInstrucao->tipoI->rs = $fp;
                novaInstrucao->tipoI->imediato = decremento;  // 25
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // subi $sp $sp 25 - restaurar stack pointer anterior
                novaInstrucao = criarNoAssembly(typeI, "subi");
                novaInstrucao->tipoI->rt = $sp;
                novaInstrucao->tipoI->rs = $sp;
                novaInstrucao->tipoI->imediato = decremento;  // 25
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // 6. CARREGAR VALOR DE RETORNO SE NECESSÁRIO
                if (instrucao->oper3 && instrucao->oper3->tipo != Vazio && instrucao->oper3->val != -1) {
                    // lw reg offset($fp) - carregar valor de retorno
                    VARIAVEL* var_retorno = get_variavel(funcaoAtual, "Valor Retorno");
                    int offset_retorno = var_retorno ? get_fp_relation(funcaoAtual, var_retorno) : 2;
                    
                    novaInstrucao = criarNoAssembly(typeI, "lw");
                    novaInstrucao->tipoI->rt = instrucao->oper3->val;
                    novaInstrucao->tipoI->rs = $fp;
                    novaInstrucao->tipoI->imediato = offset_retorno;
                    instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                }
            }
        }
    }
    // ===============================================
    // END (fim de função) - RETORNO
    // ===============================================
    else if (instrucao->operation == END) {
        if (instrucao->oper1 && instrucao->oper1->nome) {
            if (strcmp(instrucao->oper1->nome, "main") == 0) {
                // Fim da main - não fazer nada (main termina com halt)
                return;
            }
            
            // ===============================================
            // SEQUÊNCIA SOFISTICADA DE RETORNO DE FUNÇÃO:
            // 1. Restaurar $ra que foi salvo dinamicamente no início da função
            // 2. Usar jr $zero $ra $zero para retornar
            // ===============================================
            
            // 1. lw $ra offset($fp) - restaurar Return Address
            // O offset é calculado dinamicamente baseado na variável "Endereco Retorno"
            VARIAVEL* var_ra = get_variavel(funcaoAtual, "Endereco Retorno");
            int offset_ra = var_ra ? get_fp_relation(funcaoAtual, var_ra) : 1;
            
            novaInstrucao = criarNoAssembly(typeI, "lw");
            novaInstrucao->tipoI->rs = $fp;    
            novaInstrucao->tipoI->rt = $ra;    
            novaInstrucao->tipoI->imediato = offset_ra; 
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            
            novaInstrucao = criarNoAssembly(typeR, "jr");
            novaInstrucao->tipoR->rs = $ra;    
            novaInstrucao->tipoR->rd = $zero;  
            novaInstrucao->tipoR->rt = $zero;  
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
    }
    // HALT
    else if (instrucao->operation == HALT) {
        novaInstrucao = criarNoAssembly(typeJ, "halt");
        novaInstrucao->tipoJ->labelImediato = strdup("$zero");
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else {
        // Instrução não implementada
        printf("Assembly: Instrução não implementada: %s\n", getTACOperationName(instrucao->operation));
    }
}

// Função principal para gerar assembly completo
void assembly() {
    // Inicializar sistema de memória
    inicializa_memoria(&vetorMemoria);
    
    inicializaAssembly();
    
    // Gerar instruções de assembly
    
    // Jump para main no início
    ASSEMBLY *jumpMain = criarNoAssembly(typeJ, "j");
    jumpMain->tipoJ->labelImediato = strdup("main");
    instrucoesAssembly[indiceAssembly++] = jumpMain;
    
    // Processar todas as instruções de código intermediário
    for (int i = 0; i < adressCounter; i++) {
        if (intermediateCode[i] != NULL) {
            geraAssemblyCompleto(intermediateCode[i]);
        }
    }
    
    // Resolver todas as referencias de labels após gerar o assembly
    resolverLabels();
}

// Função para imprimir o assembly gerado
void imprimirAssembly() {
    
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL) continue;
        
        switch (instr->tipo) {
            case typeR:
                printf("%d:  \t%s %s %s %s\n", i,
                       instr->tipoR->nome,
                       getRegisterName(instr->tipoR->rd),
                       getRegisterName(instr->tipoR->rs), 
                       getRegisterName(instr->tipoR->rt));
                break;
                
            case typeI:
                if (instr->tipoI->label != 0) {
                    // USAR NOME DA LABEL EM VEZ DE NÚMERO
                    char labelName[32];
                    sprintf(labelName, "Label %d", instr->tipoI->label);
                    printf("%d:  \t%s %s %s %s\n", i,
                           instr->tipoI->nome,
                           getRegisterName(instr->tipoI->rt),
                           getRegisterName(instr->tipoI->rs),
                           labelName);
                } else {
                    // Tratamento específico para diferentes tipos de instruções I
                    if (strcmp(instr->tipoI->nome, "ori") == 0 || strcmp(instr->tipoI->nome, "xori") == 0 ||
                        strcmp(instr->tipoI->nome, "addi") == 0 || strcmp(instr->tipoI->nome, "subi") == 0 ||
                        strcmp(instr->tipoI->nome, "andi") == 0 || strcmp(instr->tipoI->nome, "slti") == 0) {
                        // Formato ori/xori/addi: instrução $rt $rs imediato  
                        printf("%d:  \t%s %s %s %d\n", i,
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rt),
                               getRegisterName(instr->tipoI->rs),
                               instr->tipoI->imediato);
                    } else if (strcmp(instr->tipoI->nome, "out") == 0) {
                        // Para output: out $rs $rt imediato
                        printf("%d:  \t%s %s %s %d\n", i,
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rs),
                               getRegisterName(instr->tipoI->rt),
                               instr->tipoI->imediato);
                    } else if (strcmp(instr->tipoI->nome, "in") == 0) {
                        // Para input: in $rt $rs imediato
                        printf("%d:  \t%s %s %s %d\n", i,
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rt),
                               getRegisterName(instr->tipoI->rs),
                               instr->tipoI->imediato);
                    } else if (strcmp(instr->tipoI->nome, "beq") == 0 && instr->tipoI->label != 0) {
                        // Para beq com label, mostrar nome da label
                        char labelName[32];
                        sprintf(labelName, "Label %d", instr->tipoI->label);
                        printf("%d:  \t%s %s %s %s\n", i,
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rs),
                               getRegisterName(instr->tipoI->rt),
                               labelName);
                    } else {
                        // Formato padrão load/store: instrução reg offset($reg)
                        if (instr->tipoI->imediato == 0) {
                            printf("%d:  \t%s %s 0(%s)\n", i,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs));
                        } else {
                            printf("%d:  \t%s %s %d(%s)\n", i,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   instr->tipoI->imediato,
                                   getRegisterName(instr->tipoI->rs));
                        }
                    }
                }
                break;
                
            case typeJ:
                printf("%d:  \t%s %s\n", i,
                       instr->tipoJ->nome,
                       instr->tipoJ->labelImediato);
                break;
                
            case typeLabel:
                if (strncmp(instr->tipoLabel->nome, "#", 1) == 0) {
                    printf("%s\n", instr->tipoLabel->nome);
                } else {
                    // Formato: N:  nome: (dois espaços após o número)
                    printf("%d:  %s:\n", i, instr->tipoLabel->nome);
                }
                break;
        }
    }

    // Mapeamento de labels
    imprimirLabels();
}

// Nova função que aceita FILE* como parâmetro para redirecionamento
void imprimirAssemblyParaArquivo(FILE* arquivo) {
    if (arquivo == NULL) arquivo = stdout;
    
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL) continue;
        
        switch (instr->tipo) {
            case typeR:
                fprintf(arquivo, "%d:  \t%s %s %s %s\n", i,
                       instr->tipoR->nome,
                       getRegisterName(instr->tipoR->rd),
                       getRegisterName(instr->tipoR->rs), 
                       getRegisterName(instr->tipoR->rt));
                break;
                
            case typeI:
                if (instr->tipoI->label != 0) {
                    char labelName[32];
                    sprintf(labelName, "Label %d", instr->tipoI->label);
                    fprintf(arquivo, "%d:  \t%s %s %s %s\n", i,
                           instr->tipoI->nome,
                           getRegisterName(instr->tipoI->rt),
                           getRegisterName(instr->tipoI->rs),
                           labelName);
                } else {
                    // Instruções com imediato
                    if (strcmp(instr->tipoI->nome, "sw") == 0 || strcmp(instr->tipoI->nome, "lw") == 0) {
                        fprintf(arquivo, "%d:  \t%s %s %d(%s)\n", i,
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rt),
                               instr->tipoI->imediato,
                               getRegisterName(instr->tipoI->rs));
                    } else {
                        // Outras instruções I
                        if (strcmp(instr->tipoI->nome, "ori") == 0 || strcmp(instr->tipoI->nome, "xori") == 0 ||
                            strcmp(instr->tipoI->nome, "addi") == 0 || strcmp(instr->tipoI->nome, "subi") == 0 ||
                            strcmp(instr->tipoI->nome, "andi") == 0 || strcmp(instr->tipoI->nome, "slti") == 0) {
                            fprintf(arquivo, "%d:  \t%s %s %s %d\n", i,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs),
                                   instr->tipoI->imediato);
                        } else if (strcmp(instr->tipoI->nome, "out") == 0) {
                            // Para output: out $rs $rt imediato (Eduardo usa rs=temp, rt=zero)
                            fprintf(arquivo, "%d:  \t%s %s %s %d\n", i,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rs),
                                   getRegisterName(instr->tipoI->rt),
                                   instr->tipoI->imediato);
                        } else if (strcmp(instr->tipoI->nome, "in") == 0) {
                            // Para input: in $rt $rs imediato (Eduardo usa rt=destino, rs=zero)
                            fprintf(arquivo, "%d:  \t%s %s %s %d\n", i,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs),
                                   instr->tipoI->imediato);
                        } else if (strcmp(instr->tipoI->nome, "beq") == 0 && instr->tipoI->label != 0) {
                            // Para beq com label, mostrar nome da label (COMPATÍVEL COM EDUARDO)
                            char labelName[32];
                            sprintf(labelName, "Label %d", instr->tipoI->label);
                            fprintf(arquivo, "%d:  \t%s %s %s %s\n", i,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rs),
                                   getRegisterName(instr->tipoI->rt),
                                   labelName);
                        } else {
                            fprintf(arquivo, "%d:  \t%s %s %s %d\n", i,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs),
                                   instr->tipoI->imediato);
                        }
                    }
                }
                break;
                
            case typeJ:
                fprintf(arquivo, "%d:  \t%s %s\n", i,
                       instr->tipoJ->nome,
                       instr->tipoJ->labelImediato);
                break;
                
            case typeLabel:
                if (strncmp(instr->tipoLabel->nome, "#", 1) == 0) {
                    fprintf(arquivo, "%s\n", instr->tipoLabel->nome);
                } else {
                    fprintf(arquivo, "%d:  %s:\n", i, instr->tipoLabel->nome);
                }
                break;
        }
    }
    
    // Usar a função original de impressão de labels redirecionando stdout temporariamente
    FILE *original_stdout = stdout;
    stdout = arquivo;
    imprimirLabels();
    stdout = original_stdout;
}

// Função para liberar memória do assembly
void liberarAssembly() {
    if (instrucoesAssembly == NULL) return;
    
    for (int i = 0; i < indiceAssembly; i++) {
        if (instrucoesAssembly[i] != NULL) {
            ASSEMBLY* instr = instrucoesAssembly[i];
            
            if (instr->tipoR != NULL) {
                if (instr->tipoR->nome != NULL) free(instr->tipoR->nome);
                free(instr->tipoR);
            }
            if (instr->tipoI != NULL) {
                if (instr->tipoI->nome != NULL) free(instr->tipoI->nome);
                free(instr->tipoI);
            }
            if (instr->tipoJ != NULL) {
                if (instr->tipoJ->nome != NULL) free(instr->tipoJ->nome);
                if (instr->tipoJ->labelImediato != NULL) free(instr->tipoJ->labelImediato);
                free(instr->tipoJ);
            }
            if (instr->tipoLabel != NULL) {
                if (instr->tipoLabel->nome != NULL) free(instr->tipoLabel->nome);
                free(instr->tipoLabel);
            }
            
            free(instr);
        }
    }
    
    free(instrucoesAssembly);
    instrucoesAssembly = NULL;
    indiceAssembly = 0;
    
    // Liberar sistema de labels
    liberarLabels();
}

// Função para resolver referencias de labels (converter nomes para endereços)
void resolverLabels() {
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL) continue;
        
        // Para instruções beq, NÃO resolver o label - manter nome da label como Eduardo
        if (instr->tipo == typeI && instr->tipoI->label != 0) {
            if (strcmp(instr->tipoI->nome, "beq") == 0) {
                // Para beq, manter o número da label para impressão posterior
                // NÃO calcular offset - Eduardo mantém nome da label
                continue; // Pular resolução para beq
            }
            
            char labelName[32];
            sprintf(labelName, "Label %d", instr->tipoI->label);
            int endereco = getEnderecoLabel(labelName);
            if (endereco != -1) {
                // Calcular offset relativo (endereço destino - instrução atual - 1)
                instr->tipoI->imediato = endereco - i - 1;
                instr->tipoI->label = 0; // Limpar flag de label apenas para não-beq
            } else {
                printf("AVISO: Label '%s' não encontrada\n", labelName);
            }
        }
        
        // Resolver referencias em instruções tipo J (j, jal com label)
        if (instr->tipo == typeJ && instr->tipoJ->labelImediato != NULL) {
            int endereco = getEnderecoLabel(instr->tipoJ->labelImediato);
            if (endereco != -1) {
                // Para instruções J, podemos atualizar um campo ou manter o nome
                // Por enquanto, vamos manter o nome mas podemos adicionar lógica específica
            } else {
                printf("AVISO: Label '%s' não encontrada\n", instr->tipoJ->labelImediato);
            }
        }
    }
}

// Função para imprimir assembly sem labels e com referências resolvidas
void imprimirAssemblySemLabels() {    
    if (instrucoesAssembly == NULL || indiceAssembly == 0) {
        printf("Nenhuma instrucao de assembly para imprimir.\n");
        return;
    }
    
    // Criar mapeamento de linha antiga -> linha nova (sem labels)
    int *mapeamentoLinhas = (int*)malloc(indiceAssembly * sizeof(int));
    int linhaAtual = 0;
    
    // Primeiro passo: mapear quais linhas não são labels/funções
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL) {
            mapeamentoLinhas[i] = -1; // Linha vazia
            continue;
        }
        
        // Verificar se é um label ou função (typeLabel)
        if (instr->tipo == typeLabel) {
            mapeamentoLinhas[i] = -1; // Marcar para remoção
        } else {
            mapeamentoLinhas[i] = linhaAtual++; // Nova numeração
        }
    }
    
    // Segundo passo: imprimir apenas instruções que não são labels
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL || mapeamentoLinhas[i] == -1) continue;
        
        int novaLinha = mapeamentoLinhas[i];
        
        switch (instr->tipo) {
            case typeR:
                printf("%d:  \t%s %s %s %s\n", novaLinha,
                       instr->tipoR->nome,
                       getRegisterName(instr->tipoR->rd),
                       getRegisterName(instr->tipoR->rs), 
                       getRegisterName(instr->tipoR->rt));
                break;
                
            case typeI:
                if (instr->tipoI->label != 0) {
                    // Resolver referência de label para número de linha
                    char labelName[32];
                    sprintf(labelName, "Label %d", instr->tipoI->label);
                    int enderecoOriginal = getEnderecoLabel(labelName);
                    
                    if (enderecoOriginal != -1 && enderecoOriginal < indiceAssembly) {
                        int novaLinhaDestino = mapeamentoLinhas[enderecoOriginal];
                        if (novaLinhaDestino != -1) {
                            // Calcular offset relativo na nova numeração
                            int offset = novaLinhaDestino - novaLinha - 1;
                            printf("%d:  \t%s %s %s %d\n", novaLinha,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs),
                                   offset);
                        } else {
                            printf("%d:  \t%s %s %s Label_%d_removida\n", novaLinha,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs),
                                   instr->tipoI->label);
                        }
                    } else {
                        printf("%d:  \t%s %s %s Label_%d_nao_encontrada\n", novaLinha,
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rt),
                               getRegisterName(instr->tipoI->rs),
                               instr->tipoI->label);
                    }
                } else {
                    // Tratar instruções normais
                    if (strcmp(instr->tipoI->nome, "ori") == 0 || strcmp(instr->tipoI->nome, "xori") == 0 ||
                        strcmp(instr->tipoI->nome, "addi") == 0 || strcmp(instr->tipoI->nome, "subi") == 0 ||
                        strcmp(instr->tipoI->nome, "andi") == 0 || strcmp(instr->tipoI->nome, "slti") == 0) {
                        printf("%d:  \t%s %s %s %d\n", novaLinha,
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rt),
                               getRegisterName(instr->tipoI->rs),
                               instr->tipoI->imediato);
                    } else {
                        if (instr->tipoI->imediato == 0) {
                            printf("%d:  \t%s %s 0(%s)\n", novaLinha,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs));
                        } else {
                            printf("%d:  \t%s %s %d(%s)\n", novaLinha,
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   instr->tipoI->imediato,
                                   getRegisterName(instr->tipoI->rs));
                        }
                    }
                }
                break;
                
            case typeJ:
                // Resolver referência de função para número de linha
                if (instr->tipoJ->labelImediato != NULL) {
                    // Tratamento especial para halt que não deveria ter referência de label
                    if (strcmp(instr->tipoJ->nome, "halt") == 0) {
                        printf("%d:  \t%s\n", novaLinha, instr->tipoJ->nome);
                    } else {
                        int enderecoOriginal = getEnderecoLabel(instr->tipoJ->labelImediato);
                        
                        if (enderecoOriginal != -1 && enderecoOriginal < indiceAssembly) {
                            int novaLinhaDestino = mapeamentoLinhas[enderecoOriginal];
                            if (novaLinhaDestino != -1) {
                                printf("%d:  \t%s %d\n", novaLinha,
                                       instr->tipoJ->nome,
                                       novaLinhaDestino);
                            } else {
                                // A função foi removida, buscar a próxima instrução válida
                                int proximaInstrucao = -1;
                                for (int j = enderecoOriginal + 1; j < indiceAssembly; j++) {
                                    if (mapeamentoLinhas[j] != -1) {
                                        proximaInstrucao = mapeamentoLinhas[j];
                                        break;
                                    }
                                }
                                if (proximaInstrucao != -1) {
                                    printf("%d:  \t%s %d\n", novaLinha,
                                           instr->tipoJ->nome,
                                           proximaInstrucao);
                                } else {
                                    printf("%d:  \t%s %s_removida\n", novaLinha,
                                           instr->tipoJ->nome,
                                           instr->tipoJ->labelImediato);
                                }
                            }
                        } else {
                            printf("%d:  \t%s %s_nao_encontrada\n", novaLinha,
                                   instr->tipoJ->nome,
                                   instr->tipoJ->labelImediato);
                        }
                    }
                } else {
                    printf("%d:  \t%s\n", novaLinha, instr->tipoJ->nome);
                }
                break;
                
            case typeLabel:
                // Labels não são impressos nesta função
                break;
        }
    }
    
    free(mapeamentoLinhas);
}

// Nova função que aceita FILE* como parâmetro para redirecionamento
void imprimirAssemblySemLabelsParaArquivo(FILE* arquivo) {
    if (arquivo == NULL) arquivo = stdout;
    
    if (instrucoesAssembly == NULL || indiceAssembly == 0) {
        fprintf(arquivo, "Nenhuma instrucao de assembly para imprimir.\n");
        return;
    }
    
    // Criar mapeamento de linha antiga -> linha nova (sem labels)
    int *mapeamentoLinhas = (int*)malloc(indiceAssembly * sizeof(int));
    int linhaAtual = 0;
    
    // Primeiro passo: mapear linhas, excluindo labels
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr != NULL && instr->tipo != typeLabel) {
            mapeamentoLinhas[i] = linhaAtual++;
        } else {
            mapeamentoLinhas[i] = -1; // Labels não têm linha no assembly sem labels
        }
    }
    
    // Segundo passo: imprimir instruções resolvendo referências de labels
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL || instr->tipo == typeLabel) continue;
        
        int novaLinha = mapeamentoLinhas[i];
        
        switch (instr->tipo) {
            case typeR:
                fprintf(arquivo, "%d:  \t%s %s %s %s\n", novaLinha,
                       instr->tipoR->nome,
                       getRegisterName(instr->tipoR->rd),
                       getRegisterName(instr->tipoR->rs), 
                       getRegisterName(instr->tipoR->rt));
                break;
                
            case typeI:
                if (instr->tipoI->label != 0) {
                    // Resolver referência do label sem usar variáveis indisponíveis
                    fprintf(arquivo, "%d:  \t%s %s %s Label%d\n", novaLinha,
                           instr->tipoI->nome,
                           getRegisterName(instr->tipoI->rt),
                           getRegisterName(instr->tipoI->rs),
                           instr->tipoI->label);
                } else {
                    // Instruções normais
                    if (strcmp(instr->tipoI->nome, "sw") == 0 || strcmp(instr->tipoI->nome, "lw") == 0) {
                        fprintf(arquivo, "%d:  \t%s %s %d(%s)\n", novaLinha,
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rt),
                               instr->tipoI->imediato,
                               getRegisterName(instr->tipoI->rs));
                    } else {
                        fprintf(arquivo, "%d:  \t%s %s %s %d\n", novaLinha,
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rt),
                               getRegisterName(instr->tipoI->rs),
                               instr->tipoI->imediato);
                    }
                }
                break;
                
            case typeJ:
                if (instr->tipoJ->labelImediato != NULL) {
                    // Resolver referência do label sem usar getEnderecoLabel
                    fprintf(arquivo, "%d:  \t%s %s\n", novaLinha,
                           instr->tipoJ->nome,
                           instr->tipoJ->labelImediato);
                } else {
                    fprintf(arquivo, "%d:  \t%s\n", novaLinha,
                           instr->tipoJ->nome);
                }
                break;
        }
    }
    
    free(mapeamentoLinhas);
}

// Função para salvar assembly limpo em arquivo (sem números de linha)
void salvarAssemblyLimpo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível criar o arquivo %s\n", nomeArquivo);
        return;
    }
    
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL) continue;
        
        switch (instr->tipo) {
            case typeR:
                fprintf(arquivo, "%s %s %s %s\n",
                       instr->tipoR->nome,
                       getRegisterName(instr->tipoR->rd),
                       getRegisterName(instr->tipoR->rs), 
                       getRegisterName(instr->tipoR->rt));
                break;
                
            case typeI:
                if (instr->tipoI->label != 0) {
                    fprintf(arquivo, "%s %s %s Label %d\n",
                           instr->tipoI->nome,
                           getRegisterName(instr->tipoI->rt),
                           getRegisterName(instr->tipoI->rs),
                           instr->tipoI->label);
                } else {
                    // Tratar instruções especiais como ori e xori que têm formato diferente
                    if (strcmp(instr->tipoI->nome, "ori") == 0 || strcmp(instr->tipoI->nome, "xori") == 0) {
                        // Formato ori/xori: ori $rt $rs imediato  
                        fprintf(arquivo, "%s %s %s %d\n",
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rt),
                               getRegisterName(instr->tipoI->rs),
                               instr->tipoI->imediato);
                    } else {
                        // Formato padrão: instrução reg offset($reg)
                        if (instr->tipoI->imediato == 0) {
                            fprintf(arquivo, "%s %s 0(%s)\n",
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs));
                        } else {
                            fprintf(arquivo, "%s %s %d(%s)\n",
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   instr->tipoI->imediato,
                                   getRegisterName(instr->tipoI->rs));
                        }
                    }
                }
                break;
                
            case typeJ:
                fprintf(arquivo, "%s %s\n",
                       instr->tipoJ->nome,
                       instr->tipoJ->labelImediato);
                break;
                
            case typeLabel:
                if (strncmp(instr->tipoLabel->nome, "#", 1) == 0) {
                    fprintf(arquivo, "%s\n", instr->tipoLabel->nome);
                } else {
                    // Formato: nome: (sem número da linha)
                    fprintf(arquivo, "%s:\n", instr->tipoLabel->nome);
                }
                break;
        }
    }
    
    fclose(arquivo);
}

// Função para salvar assembly sem labels em arquivo (apenas instruções e números de linha)
void salvarAssemblySemLabelsArquivo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível criar o arquivo %s\n", nomeArquivo);
        return;
    }
    
    // Criar mapeamento de linha antiga -> linha nova (sem labels)
    int *mapeamentoLinhas = (int*)malloc(indiceAssembly * sizeof(int));
    int linhaAtual = 0;
    
    // Primeiro passo: mapear quais linhas não são labels/funções
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL) {
            mapeamentoLinhas[i] = -1; // Linha vazia
            continue;
        }
        
        // Verificar se é um label ou função (typeLabel)
        if (instr->tipo == typeLabel) {
            mapeamentoLinhas[i] = -1; // Marcar para remoção
        } else {
            mapeamentoLinhas[i] = linhaAtual++; // Nova numeração
        }
    }
    
    // Segundo passo: salvar apenas instruções que não são labels
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL || mapeamentoLinhas[i] == -1) continue;
        
        switch (instr->tipo) {
            case typeR:
                fprintf(arquivo, "%s %s %s %s\n",
                       instr->tipoR->nome,
                       getRegisterName(instr->tipoR->rd),
                       getRegisterName(instr->tipoR->rs), 
                       getRegisterName(instr->tipoR->rt));
                break;
                
            case typeI:
                if (instr->tipoI->label != 0) {
                    // Resolver referência de label para número de linha
                    char labelName[32];
                    sprintf(labelName, "Label %d", instr->tipoI->label);
                    int enderecoOriginal = getEnderecoLabel(labelName);
                    
                    if (enderecoOriginal != -1 && enderecoOriginal < indiceAssembly) {
                        int novaLinhaDestino = mapeamentoLinhas[enderecoOriginal];
                        if (novaLinhaDestino != -1) {
                            // Calcular offset relativo na nova numeração
                            int novaLinha = mapeamentoLinhas[i];
                            int offset = novaLinhaDestino - novaLinha - 1;
                            fprintf(arquivo, "%s %s %s %d\n",
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs),
                                   offset);
                        } else {
                            fprintf(arquivo, "%s %s %s Label_%d_removida\n",
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs),
                                   instr->tipoI->label);
                        }
                    }
                } else {
                    // Tratar instruções normais
                    if (strcmp(instr->tipoI->nome, "ori") == 0 || strcmp(instr->tipoI->nome, "xori") == 0 ||
                        strcmp(instr->tipoI->nome, "addi") == 0 || strcmp(instr->tipoI->nome, "subi") == 0 ||
                        strcmp(instr->tipoI->nome, "andi") == 0 || strcmp(instr->tipoI->nome, "slti") == 0) {
                        fprintf(arquivo, "%s %s %s %d\n",
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rt),
                               getRegisterName(instr->tipoI->rs),
                               instr->tipoI->imediato);
                    } else {
                        if (instr->tipoI->imediato == 0) {
                            fprintf(arquivo, "%s %s 0(%s)\n",
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs));
                        } else {
                            fprintf(arquivo, "%s %s %d(%s)\n",
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   instr->tipoI->imediato,
                                   getRegisterName(instr->tipoI->rs));
                        }
                    }
                }
                break;
                
            case typeJ:
                // Resolver referência de função para número de linha
                if (instr->tipoJ->labelImediato != NULL) {
                    // Tratamento especial para halt que não deveria ter referência de label
                    if (strcmp(instr->tipoJ->nome, "halt") == 0) {
                        fprintf(arquivo, "%s\n", instr->tipoJ->nome);
                    } else {
                        int enderecoOriginal = getEnderecoLabel(instr->tipoJ->labelImediato);
                        
                        if (enderecoOriginal != -1 && enderecoOriginal < indiceAssembly) {
                            int novaLinhaDestino = mapeamentoLinhas[enderecoOriginal];
                            if (novaLinhaDestino != -1) {
                                fprintf(arquivo, "%s %d\n",
                                       instr->tipoJ->nome,
                                       novaLinhaDestino);
                            } else {
                                // A função foi removida, buscar a próxima instrução válida
                                int proximaInstrucao = -1;
                                for (int j = enderecoOriginal + 1; j < indiceAssembly; j++) {
                                    if (mapeamentoLinhas[j] != -1) {
                                        proximaInstrucao = mapeamentoLinhas[j];
                                        break;
                                    }
                                }
                                if (proximaInstrucao != -1) {
                                    fprintf(arquivo, "%s %d\n",
                                           instr->tipoJ->nome,
                                           proximaInstrucao);
                                } else {
                                    fprintf(arquivo, "%s %s_removida\n",
                                           instr->tipoJ->nome,
                                           instr->tipoJ->labelImediato);
                                }
                            }
                        }
                    }
                } else {
                    fprintf(arquivo, "%s\n", instr->tipoJ->nome);
                }
                break;
                
            case typeLabel:
                // Labels não são salvos nesta função
                break;
        }
        
    }
    
    free(mapeamentoLinhas);
    fclose(arquivo);
}

// Função para salvar assembly puro sem números de linha
void salvarAssemblyPuro(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível criar o arquivo %s\n", nomeArquivo);
        return;
    }
    
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL) continue;
        
        switch (instr->tipo) {
            case typeR:
                fprintf(arquivo, "%s %s %s %s\n",
                       instr->tipoR->nome,
                       getRegisterName(instr->tipoR->rd),
                       getRegisterName(instr->tipoR->rs), 
                       getRegisterName(instr->tipoR->rt));
                break;
                
            case typeI:
                if (instr->tipoI->label != 0) {
                    fprintf(arquivo, "%s %s %s Label %d\n",
                           instr->tipoI->nome,
                           getRegisterName(instr->tipoI->rt),
                           getRegisterName(instr->tipoI->rs),
                           instr->tipoI->label);
                } else {
                    // Tratar instruções especiais como ori e xori que têm formato diferente
                    if (strcmp(instr->tipoI->nome, "ori") == 0 || strcmp(instr->tipoI->nome, "xori") == 0) {
                        // Formato ori/xori: ori $rt $rs imediato  
                        fprintf(arquivo, "%s %s %s %d\n",
                               instr->tipoI->nome,
                               getRegisterName(instr->tipoI->rt),
                               getRegisterName(instr->tipoI->rs),
                               instr->tipoI->imediato);
                    } else {
                        // Formato padrão: instrução reg offset($reg)
                        if (instr->tipoI->imediato == 0) {
                            fprintf(arquivo, "%s %s 0(%s)\n",
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   getRegisterName(instr->tipoI->rs));
                        } else {
                            fprintf(arquivo, "%s %s %d(%s)\n",
                                   instr->tipoI->nome,
                                   getRegisterName(instr->tipoI->rt),
                                   instr->tipoI->imediato,
                                   getRegisterName(instr->tipoI->rs));
                        }
                    }
                }
                break;
                
            case typeJ:
                if (instr->tipoJ->labelImediato != NULL) {
                    // Tratamento especial para halt que não deveria ter referência de label
                    if (strcmp(instr->tipoJ->nome, "halt") == 0) {
                        fprintf(arquivo, "%s\n", instr->tipoJ->nome);
                    } else {
                        fprintf(arquivo, "%s %s\n",
                               instr->tipoJ->nome,
                               instr->tipoJ->labelImediato);
                    }
                } else {
                    fprintf(arquivo, "%s\n", instr->tipoJ->nome);
                }
                break;
                
            case typeLabel:
                // Labels são incluídos no assembly puro (diferente do assembly sem labels)
                if (strncmp(instr->tipoLabel->nome, "#", 1) == 0) {
                    fprintf(arquivo, "%s\n", instr->tipoLabel->nome);
                } else {
                    // Formato: nome: (sem número da linha)
                    fprintf(arquivo, "%s:\n", instr->tipoLabel->nome);
                }
                break;
        }
    }
    
    fclose(arquivo);
}

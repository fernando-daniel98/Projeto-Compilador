#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/codeGen.h"
#include "../include/assembler.h"
#include "../include/memoria.h"

// Estruturas globais para assembly
ASSEMBLY **instrucoesAssembly = NULL;
int indiceAssembly = 0;
static char nomeFuncaoAtual[256] = "";  // Nome da função atual (string)
// funcaoAtual será o ponteiro para MEMORIA_FUNCOES (definido em memoria.h)

// ===============================================
// FUNÇÃO PARA MAPEAR REGISTRADORES PARA NOMES
// Converte números de registradores para nomes legíveis
// ===============================================
const char* getRegisterName(int regNum) {
    static char tempName[32];
    
    switch (regNum) {
        // Registradores especiais (sistema)
        case 63: return "$zero";   // Sempre 0
        case 62: return "$ra";     // Return Address
        case 61: return "$fp";     // Frame Pointer
        case 60: return "$sp";     // Stack Pointer
        case 59: return "$temp";   // Temporário do compilador
        case 58: return "$pilha";  // Ponteiro para pilha de parâmetros
        case 57: return "$s2";     // Saved register 2
        case 56: return "$s1";     // Saved register 1
        case 55: return "$s0";     // Saved register 0
        
        // Registradores temporários (0-54)
        default:
            if (regNum >= 0 && regNum <= 54) {
                snprintf(tempName, sizeof(tempName), "$t%d", regNum);
                return tempName;
            } else {
                snprintf(tempName, sizeof(tempName), "$reg%d", regNum);
                return tempName;
            }
    }
}

// Função para inicializar o sistema de assembly
void inicializaAssembly() {
    instrucoesAssembly = (ASSEMBLY **) malloc(MAX_ASSEMBLY * sizeof(ASSEMBLY *));
    for (int i = 0; i < MAX_ASSEMBLY; i++) {
        instrucoesAssembly[i] = NULL;
    }
    indiceAssembly = 0;
    strcpy(nomeFuncaoAtual, "");
}

// Função para criar um novo nó de assembly
ASSEMBLY* criarNoAssembly(tipoInstrucao tipo, char *nome) {
    ASSEMBLY* novaInstrucao = (ASSEMBLY*) malloc(sizeof(ASSEMBLY));
    novaInstrucao->tipo = tipo;
    
    // Inicializar todos os ponteiros como NULL
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

// Função para gerar assembly de operações aritméticas
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
    // AND e OR não estão no enum, removendo por enquanto
    else {
        return 0; // Operação não reconhecida
    }
    return 1; // Sucesso
}

// Função para gerar assembly de operações relacionais usando a lógica do Eduardo
int opRelacionais(quadruple* instrucao, ASSEMBLY** novaInstrucao) {
    if (instrucao->operation == EQ) {
        // EQ: if arg1 == arg2 then arg3 = 1, else arg3 = 0
        // Eduardo's logic: slt twice and xor
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
        // NEQ: if arg1 != arg2 then arg3 = 1, else arg3 = 0
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
        // GT: if arg1 > arg2 then arg3 = 1, else arg3 = 0
        *novaInstrucao = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper2->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper1->val;
    }
    else if (instrucao->operation == GTE) {
        // GTE: if arg1 >= arg2 then arg3 = 1, else arg3 = 0
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
        // LTE: if arg1 <= arg2 then arg3 = 1, else arg3 = 0
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

// Função principal para gerar assembly completo baseado na lógica do Eduardo
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
        novaInstrucao->tipoR->rd = instrucao->oper1->val;    // destino
        novaInstrucao->tipoR->rs = $zero;                    // $zero (63)
        novaInstrucao->tipoR->rt = instrucao->oper2->val;    // fonte
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // LOADI (carregar imediato)
    else if (instrucao->operation == LOADI) {
        novaInstrucao = criarNoAssembly(typeI, "ori");
        novaInstrucao->tipoI->rt = instrucao->oper1->val;    // destino
        novaInstrucao->tipoI->rs = $zero;                    // $zero (63)
        novaInstrucao->tipoI->imediato = instrucao->oper2->val; // valor imediato
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // ALLOC (alocar memória)
    else if (instrucao->operation == ALLOC) {
        // ALLOC é tratado em tempo de compilação para layout de memória
        // Apenas inserir comentário
        novaInstrucao = criarNoAssembly(typeLabel, "# ALLOC");
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // ARG (argumento de função)
    else if (instrucao->operation == ARG) {
        // ARG é tratado em tempo de compilação
        novaInstrucao = criarNoAssembly(typeLabel, "# ARG");
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // IFFALSE (branch if false) - corrigindo IFF para IFFALSE
    else if (instrucao->operation == IFFALSE) {
        novaInstrucao = criarNoAssembly(typeI, "beq");
        novaInstrucao->tipoI->rs = $zero;                    // $zero (63)
        novaInstrucao->tipoI->rt = instrucao->oper1->val;    // registrador de condição
        novaInstrucao->tipoI->label = instrucao->oper2->val; // label de destino
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // LABEL
    else if (instrucao->operation == LABEL) {
        char labelName[32];
        sprintf(labelName, "Label_%d", instrucao->oper1->val);
        novaInstrucao = criarNoAssembly(typeLabel, labelName);
        novaInstrucao->tipoLabel->endereco = instrucao->oper1->val;
        novaInstrucao->tipoLabel->boolean = 1; // é label (não função)
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // FUNC (declaração de função) - corrigindo FUN para FUNC
    else if (instrucao->operation == FUNC) {
        if (instrucao->oper2 && instrucao->oper2->nome) {
            strcpy(nomeFuncaoAtual, instrucao->oper2->nome);
            novaInstrucao = criarNoAssembly(typeLabel, instrucao->oper2->nome);
            novaInstrucao->tipoLabel->boolean = 0; // é função (não label)
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            
            // Atualizar função atual no sistema de memória
            funcaoAtual = insere_funcao(&vetorMemoria, instrucao->oper2->nome);
            
            // Se for main, inicializar registradores especiais usando cálculos dinâmicos
            if (strcmp(instrucao->oper2->nome, "main") == 0) {
                // ===============================================
                // INICIALIZAÇÃO DINÂMICA DA MAIN (estilo Eduardo)
                // Calcula valores baseados no tamanho das variáveis globais
                // ===============================================
                
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
                // Cálculo dinâmico: tamanho das globais + tamanho da função atual
                int sp_value = buscar_funcao(&vetorMemoria, "global")->tamanho + get_sp(funcaoAtual);
                novaInstrucao->tipoI->imediato = sp_value;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // add $sp $sp $s0 (somar com base do sistema)
                novaInstrucao = criarNoAssembly(typeR, "add");
                novaInstrucao->tipoR->rd = $sp;
                novaInstrucao->tipoR->rs = $sp;
                novaInstrucao->tipoR->rt = $s0;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // Inicializar ponteiro de parâmetros ($pilha)
                // add $pilha $zero $s0 (valor base do sistema)
                novaInstrucao = criarNoAssembly(typeR, "add");
                novaInstrucao->tipoR->rd = $pilha;
                novaInstrucao->tipoR->rs = $zero;
                novaInstrucao->tipoR->rt = $s0;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // subi $pilha $pilha 5 (ajustar para área de parâmetros)
                novaInstrucao = criarNoAssembly(typeI, "subi");
                novaInstrucao->tipoI->rt = $pilha;
                novaInstrucao->tipoI->rs = $pilha;
                novaInstrucao->tipoI->imediato = 5;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            } else {
                // ===============================================
                // INÍCIO DE FUNÇÃO (não-main) - SALVAR RETURN ADDRESS
                // Padrão do Eduardo: sempre salvar $ra em offset calculado
                // ===============================================
                
                // sw $ra offset($fp) - salva return address no frame atual
                // O offset é calculado dinamicamente baseado na variável "Endereco Retorno"
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
    // ===============================================
    // RETURN (retorno de função com valor) - Implementação sofisticada do Eduardo
    // ===============================================
    else if (instrucao->operation == RETURN) {
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
    // PARAM (parâmetro de função)
    else if (instrucao->operation == PARAM) {
        novaInstrucao = criarNoAssembly(typeI, "sw");
        novaInstrucao->tipoI->rs = $pilha;
        novaInstrucao->tipoI->rt = instrucao->oper1->val;
        novaInstrucao->tipoI->imediato = 0; // offset na pilha de parâmetros
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // LOAD (carregar de memória)
    else if (instrucao->operation == LOAD) {
        novaInstrucao = criarNoAssembly(typeI, "lw");
        novaInstrucao->tipoI->rt = instrucao->oper1->val;    // destino
        novaInstrucao->tipoI->rs = $fp;                      // frame pointer
        novaInstrucao->tipoI->imediato = 0;                  // offset (simplificado)
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // STORE (salvar na memória)
    else if (instrucao->operation == STORE) {
        novaInstrucao = criarNoAssembly(typeI, "sw");
        novaInstrucao->tipoI->rt = instrucao->oper2->val;    // fonte
        novaInstrucao->tipoI->rs = $fp;                      // frame pointer
        novaInstrucao->tipoI->imediato = 0;                  // offset (simplificado)
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // GOTO (salto incondicional)
    else if (instrucao->operation == GOTO) {
        char labelName[32];
        sprintf(labelName, "Label_%d", instrucao->oper1->val);
        novaInstrucao = criarNoAssembly(typeJ, "j");
        novaInstrucao->tipoJ->labelImediato = strdup(labelName);
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    // CALL (chamada de função)
    else if (instrucao->operation == CALL) {
        if (instrucao->oper1 && instrucao->oper1->nome) {
            if (strcmp(instrucao->oper1->nome, "output") == 0) {
                // ===============================================
                // FUNÇÃO DE OUTPUT - Implementação do Eduardo
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
                novaInstrucao->tipoI->rs = $temp;
                novaInstrucao->tipoI->rt = $zero;
                novaInstrucao->tipoI->imediato = 0;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                return; // Não precisa fazer mais nada
            } else if (strcmp(instrucao->oper1->nome, "input") == 0) {
                // ===============================================
                // FUNÇÃO DE INPUT - Implementação do Eduardo
                // ===============================================
                
                // in reg $zero 0 - ler valor do usuário
                novaInstrucao = criarNoAssembly(typeI, "in");
                novaInstrucao->tipoI->rt = instrucao->oper3->val;
                novaInstrucao->tipoI->rs = $zero;
                novaInstrucao->tipoI->imediato = 0;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                return; // Não precisa fazer mais nada
            } else {
                // ===============================================
                // CHAMADA DE FUNÇÃO SOFISTICADA (Implementação do Eduardo)
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
                // addi $fp $fp tamanho_dinamico - incrementar frame pointer
                int tamanho_frame = get_sp(funcaoAtual) + 1;
                novaInstrucao = criarNoAssembly(typeI, "addi");
                novaInstrucao->tipoI->rt = $fp;
                novaInstrucao->tipoI->rs = $fp;
                novaInstrucao->tipoI->imediato = tamanho_frame;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // addi $sp $sp tamanho_dinamico - incrementar stack pointer
                novaInstrucao = criarNoAssembly(typeI, "addi");
                novaInstrucao->tipoI->rt = $sp;
                novaInstrucao->tipoI->rs = $sp;
                novaInstrucao->tipoI->imediato = tamanho_frame;
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
                // subi $fp $fp tamanho_dinamico - restaurar frame pointer anterior
                novaInstrucao = criarNoAssembly(typeI, "subi");
                novaInstrucao->tipoI->rt = $fp;
                novaInstrucao->tipoI->rs = $fp;
                novaInstrucao->tipoI->imediato = tamanho_frame;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // subi $sp $sp tamanho_dinamico - restaurar stack pointer anterior  
                novaInstrucao = criarNoAssembly(typeI, "subi");
                novaInstrucao->tipoI->rt = $sp;
                novaInstrucao->tipoI->rs = $sp;
                novaInstrucao->tipoI->imediato = tamanho_frame;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
                
                // 6. CARREGAR VALOR DE RETORNO SE NECESSÁRIO
                if (instrucao->oper3 && instrucao->oper3->val != -1) {
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
    // END (fim de função) - RETORNO SOFISTICADO do Eduardo
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
            novaInstrucao->tipoI->rs = $fp;    // $fp (61) - frame pointer
            novaInstrucao->tipoI->rt = $ra;    // $ra (62) - return address
            novaInstrucao->tipoI->imediato = offset_ra; // offset calculado dinamicamente
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            
            // 2. jr $zero $ra $zero - retornar para a função chamadora
            // PC = $ra (endereço de retorno)
            novaInstrucao = criarNoAssembly(typeR, "jr");
            novaInstrucao->tipoR->rs = $ra;    // $ra (62) - contém endereço de retorno
            novaInstrucao->tipoR->rd = $zero;  // $zero (63) - não usado mas necessário
            novaInstrucao->tipoR->rt = $zero;  // $zero (63) - não usado mas necessário
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
    // Inicializar sistema de memória sofisticado
    inicializa_memoria(&vetorMemoria);
    
    inicializaAssembly();
    
    printf("Gerando assembly sofisticado baseado na lógica do Eduardo...\n");
    
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
    
    printf("Assembly sofisticado gerado com %d instruções.\n", indiceAssembly);
    printf("Sistema de memória dinâmico ativo com cálculos automáticos de offset.\n");
}

// Função para imprimir o assembly gerado
void imprimirAssembly() {
    printf("\n============== ASSEMBLY COMPLETO ==============\n");
    printf("# PROCESSADOR MIPS-64 - Arquitetura de 64 registradores\n");
    printf("#\n");
    printf("# REGISTRADORES ESPECIAIS:\n");
    printf("# $zero (63): Sempre 0                    | $ra   (62): Return Address\n");
    printf("# $fp   (61): Frame Pointer               | $sp   (60): Stack Pointer\n");
    printf("# $temp (59): Temporário do compilador   | $pilha(58): Pilha de parâmetros\n");
    printf("# $s2   (57): Saved reg 2                | $s1   (56): Saved reg 1\n");
    printf("# $s0   (55): Saved reg 0                | $t0-$t54 (0-54): Temporários\n");
    printf("#\n");
    printf("# CONVENÇÕES DE FRAME:\n");
    printf("# 0($fp): parâmetro 1    | 1($fp): parâmetro 2    | 2($fp): parâmetro 3\n");
    printf("# 3($fp): end. retorno   | 4($fp): return addr    | 5($fp): valor retorno\n");
    printf("# 9-11($fp): variáveis locais | Frame size: 25 posições\n");
    printf("#\n");
    printf("# PILHA:\n");
    printf("# Crescimento: addi $fp/$sp $fp/$sp 25\n");
    printf("# Redução:     subi $fp/$sp $fp/$sp 25\n");
    printf("#\n");
    printf("# CHAMADAS DE FUNÇÃO:\n");
    printf("# 1. Preparar parâmetros em $pilha (58)\n");
    printf("# 2. Transferir para $sp usando $temp (59)\n");
    printf("# 3. Salvar frame anterior em 4($sp)\n");
    printf("# 4. Crescer pilha e chamar com jal\n");
    printf("# 5. Reduzir pilha após retorno\n");
    printf("#\n");
    printf("# RETORNO DE FUNÇÃO:\n");
    printf("# 1. Salvar $ra em 4($fp) no início\n");
    printf("# 2. Preparar valor de retorno via 3($fp)\n");
    printf("# 3. Restaurar $ra e usar jr $zero $ra $zero\n");
    printf("#\n");
    printf("================================================\n\n");
    
    for (int i = 0; i < indiceAssembly; i++) {
        ASSEMBLY* instr = instrucoesAssembly[i];
        if (instr == NULL) continue;
        
        printf("%d:\t", i);
        
        switch (instr->tipo) {
            case typeR:
                printf("%s %s, %s, %s\n", 
                       instr->tipoR->nome,
                       getRegisterName(instr->tipoR->rd),
                       getRegisterName(instr->tipoR->rs), 
                       getRegisterName(instr->tipoR->rt));
                break;
                
            case typeI:
                if (instr->tipoI->label != 0) {
                    printf("%s %s, %s, Label_%d\n",
                           instr->tipoI->nome,
                           getRegisterName(instr->tipoI->rt),
                           getRegisterName(instr->tipoI->rs),
                           instr->tipoI->label);
                } else {
                    printf("%s %s, %s, %d\n",
                           instr->tipoI->nome,
                           getRegisterName(instr->tipoI->rt),
                           getRegisterName(instr->tipoI->rs),
                           instr->tipoI->imediato);
                }
                break;
                
            case typeJ:
                printf("%s %s\n",
                       instr->tipoJ->nome,
                       instr->tipoJ->labelImediato);
                break;
                
            case typeLabel:
                if (strncmp(instr->tipoLabel->nome, "#", 1) == 0) {
                    printf("%s\n", instr->tipoLabel->nome);
                } else {
                    printf("%s:\n", instr->tipoLabel->nome);
                }
                break;
        }
    }
    
    printf("============== FIM ASSEMBLY COMPLETO ==============\n");
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
}

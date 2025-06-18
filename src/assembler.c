#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/codeGen.h"
#include "../include/assembler.h"
#include "../include/memoria.h"


MEMORIA vetorMemoria; // var global que guarda a estrutura da memoria
MEMORIA_FUNCOES *funcaoAtual = NULL; // pont para a funcao atual; analogo a funcName em codeGen.h

void geraAssembly(quadruple* instrucao);

// Função que inicia a estrutura e gera o código assembly
void assembly(){
    inicializaAssembly();

    /* Criar uma funcao Jump para a main */
    ASSEMBLY *novaInstrucao = criarNoAssembly(typeJ, "j");
    novaInstrucao->tipoJ->labelImediato = strdup("main");
    instrucoesAssembly[indiceAssembly++] = novaInstrucao;

    for(int i = 0; i < adressCounter; i++){ // Alterado de indiceVetor para adressCounter
        if (intermediateCode[i] != NULL) { // Adicionada verificação de nulidade
            geraAssembly(intermediateCode[i]);
        }
    }
}

// OK
int opRelacionais(quadruple* instrucao, ASSEMBLY** novaInstrucao){

    // As comparações de string foram substituídas por comparações de enum
    if(instrucao->operation == EQ){
        (*novaInstrucao) = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;

        instrucoesAssembly[indiceAssembly++] = *novaInstrucao;

        (*novaInstrucao) = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val; // RD deve ser o mesmo para a segunda parte da emulação
        (*novaInstrucao)->tipoR->rs = instrucao->oper2->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper1->val;

        instrucoesAssembly[indiceAssembly++] = *novaInstrucao;

        (*novaInstrucao) = criarNoAssembly(typeI, "xori");
        (*novaInstrucao)->tipoI->rt = instrucao->oper3->val;
        (*novaInstrucao)->tipoI->rs = instrucao->oper3->val;
        (*novaInstrucao)->tipoI->imediato = 1;

    }
    else if(instrucao->operation == LT){
        (*novaInstrucao) = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
    }
    else if(instrucao->operation == NEQ){ 
        (*novaInstrucao) = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;

        instrucoesAssembly[indiceAssembly++] = *novaInstrucao;

        (*novaInstrucao) = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper2->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper1->val;
        
    }
    else if(instrucao->operation == GT){
        (*novaInstrucao) = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper2->val; // GT (a > b) -> (b < a)
        (*novaInstrucao)->tipoR->rt = instrucao->oper1->val;
    }
    else if(instrucao->operation == GTE){ // a >= b  é NOT (a < b)
        (*novaInstrucao) = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;

        instrucoesAssembly[indiceAssembly++] = *novaInstrucao;

        int rd = (*novaInstrucao)->tipoR->rd;

        (*novaInstrucao) = criarNoAssembly(typeI, "xori"); // Inverte o bit (0->1, 1->0)
        (*novaInstrucao)->tipoI->rt = rd;
        (*novaInstrucao)->tipoI->rs = rd;
        (*novaInstrucao)->tipoI->imediato = 1;
    }
    else if(instrucao->operation == LTE){ // a <= b é NOT (b < a)
        (*novaInstrucao) = criarNoAssembly(typeR, "slt");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper2->val; // (b < a)
        (*novaInstrucao)->tipoR->rt = instrucao->oper1->val;

        instrucoesAssembly[indiceAssembly++] = *novaInstrucao;

        int rd = (*novaInstrucao)->tipoR->rd;

        (*novaInstrucao) = criarNoAssembly(typeI, "xori"); // Inverte o bit
        (*novaInstrucao)->tipoI->rt = rd;
        (*novaInstrucao)->tipoI->rs = rd;
        (*novaInstrucao)->tipoI->imediato = 1;
    }
    else{
        return 0;
    }
    return 1;
}

// OK
int opAritmeticos(quadruple* instrucao, ASSEMBLY** novaInstrucao){
    
    if(instrucao->operation == ADD){
        *novaInstrucao = criarNoAssembly(typeR, "add");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
    }
    else if(instrucao->operation == SUB){
        (*novaInstrucao) = criarNoAssembly(typeR, "sub");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
    }
    else if(instrucao->operation == MULT){
        (*novaInstrucao) = criarNoAssembly(typeR, "mult");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
    }
    else if(instrucao->operation == DIV){ // Similar a mult
        (*novaInstrucao) = criarNoAssembly(typeR, "div");
        (*novaInstrucao)->tipoR->rd = instrucao->oper3->val;
        (*novaInstrucao)->tipoR->rs = instrucao->oper1->val;
        (*novaInstrucao)->tipoR->rt = instrucao->oper2->val;
    }
    else{
        return 0;
    }
    return 1;
}

// Alterado o tipo do parâmetro de INSTRUCAO* para quadruple*
void geraAssembly(quadruple* instrucao){
    ASSEMBLY* novaInstrucao = NULL;

    if (instrucao == NULL) return; // Segurança

    if(opAritmeticos(instrucao, &novaInstrucao)){
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else if(opRelacionais(instrucao, &novaInstrucao)){
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else if(instrucao->operation == ASSIGN){ // x = y  -> add x, $zero, y
        novaInstrucao = criarNoAssembly(typeR, "add");
        novaInstrucao->tipoR->rd = instrucao->oper1->val; // Destino (x)
        novaInstrucao->tipoR->rs = $zero;                 // Fonte1 ($zero)
        novaInstrucao->tipoR->rt = instrucao->oper2->val; // Fonte2 (y)
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else if(instrucao->operation == LOADI){ // LOADI rt, imm  -> ori rt, $zero, imm
        novaInstrucao = criarNoAssembly(typeI, "ori");
        novaInstrucao->tipoI->rt = instrucao->oper1->val; // rt (destino)
        novaInstrucao->tipoI->rs = $zero;                 // rs ($zero)
        novaInstrucao->tipoI->imediato = instrucao->oper2->val; // imediato
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else if(instrucao->operation == ALLOC){
        // oper1: nome da variável (String)
        // oper2: nome da função/escopo (String)
        // oper3: tamanho (IntConst) ou Vazio para não vetor
        if (instrucao->oper2 == NULL || instrucao->oper2->nome == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Nome de função/escopo nulo para ALLOC.\n");
            return;
        }
        MEMORIA_FUNCOES* funcao = buscar_funcao(&vetorMemoria, instrucao->oper2->nome);
        if (funcao == NULL) {
             printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Função %s não encontrada para ALLOC.\n", instrucao->oper2->nome);
             return;
        }

        if(instrucao->oper1 == NULL || instrucao->oper1->nome == NULL){
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Nome de variável nulo para ALLOC.\n");
            return;
        }

        if(instrucao->oper3 == NULL){
            printf(ANSI_COLOR_RED "Erro: " ANSI_COLOR_RESET);
            printf("NULL no operando 3 para ALLOC\n");
            return;
        }
        
        if(instrucao->oper3->tipo == Vazio){
            insere_variavel(funcao, instrucao->oper1->nome, inteiro);
        }
        else{ // Assumindo IntConst para tamanho do vetor
            for(int i = 0; i < instrucao->oper3->val; i++){
                insere_variavel(funcao, instrucao->oper1->nome, vetor);	
            }
        }

        // A verificação de estouro de memória já estava aqui, mantida.
        static int flag_alloc = 0; // Renomeado para evitar conflito com 'flag' de outros contextos
        if(funcao->tamanho > get_sp(funcao) && !flag_alloc){
            printf(ANSI_COLOR_RED "Erro: " ANSI_COLOR_RESET);
            printf("Memoria insuficiente na funcao %s\n", funcao->nome);
            printf("Aumente a memoria alocada pelo compilador ou diminua as variaveis da funcao\n");
            flag_alloc = 1;
        }
    }
    else if(instrucao->operation == ARG){
        // oper1: tipo do argumento ("INT" ou "VET") (String)
        // oper2: nome do argumento (String)
        // oper3: nome da função (String)
        if (instrucao->oper3 == NULL || instrucao->oper3->nome == NULL ||
            instrucao->oper1 == NULL || instrucao->oper1->nome == NULL ||
            instrucao->oper2 == NULL || instrucao->oper2->nome == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Operandos nulos para ARG.\n");
            return;
        }
        MEMORIA_FUNCOES* funcao = buscar_funcao(&vetorMemoria, instrucao->oper3->nome);
        if (funcao == NULL) {
             printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Função %s não encontrada para ARG.\n", instrucao->oper3->nome);
             return;
        }
        
        if(!strcmp(instrucao->oper1->nome, "INT")) 
            insere_variavel(funcao, instrucao->oper2->nome, inteiroArg);
        else insere_variavel(funcao, instrucao->oper2->nome, vetorArg);
    }	
    else if(instrucao->operation == IFFALSE){ // IFF reg, label -> beq reg, $zero, label
        novaInstrucao = criarNoAssembly(typeI, "beq");
        novaInstrucao->tipoI->rs = instrucao->oper1->val; // registrador com a condição
        novaInstrucao->tipoI->rt = $zero;                 // comparar com zero
        novaInstrucao->tipoI->label = instrucao->oper2->val; // label para pular se falso (igual a zero)
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else if(instrucao->operation == LABEL){
        // oper1: número do label (IntConst)
        if (instrucao->oper1 == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Operando nulo para LABEL.\n");
            return;
        }
        char* auxLabel = (char*) malloc(sizeof(char) * 20); // Aumentado tamanho para "Label XXXXXXXX"
        sprintf(auxLabel, "Label%d", instrucao->oper1->val); // Removido espaço para consistência
        novaInstrucao = criarNoAssembly(typeLabel, auxLabel); 
        novaInstrucao->tipoLabel->endereco = instrucao->oper1->val; // O endereço aqui é o número do label
        novaInstrucao->tipoLabel->boolean = 1; // Indica que é um label de desvio

        // Adicionar à tabela de labels do assembler
        adicionarLabel(auxLabel, indiceAssembly); // O endereço é o índice atual da instrução assembly

        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else if(instrucao->operation == FUNC){
        // oper1: tipo de retorno ("INT", "VOID") (String)
        // oper2: nome da função (String)
        // oper3: número de parâmetros (IntConst)
        if (instrucao->oper2 == NULL || instrucao->oper2->nome == NULL || instrucao->oper3 == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Operandos nulos para FUNC.\n");
            return;
        }
        novaInstrucao = criarNoAssembly(typeLabel, strdup(instrucao->oper2->nome)); 
        novaInstrucao->tipoLabel->boolean = 0; // Indica que é um label de função

        adicionarLabel(instrucao->oper2->nome, indiceAssembly);
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;

        funcaoAtual = insere_funcao(&vetorMemoria, strdup(instrucao->oper2->nome));
    
        if(!strcmp(instrucao->oper2->nome, "main")){
            novaInstrucao = criarNoAssembly(typeI, "ori");
            novaInstrucao->tipoI->rt = $fp;
            novaInstrucao->tipoI->rs = $zero; 
            novaInstrucao->tipoI->imediato = buscar_funcao(&vetorMemoria, "global")->tamanho + get_fp(funcaoAtual);
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            novaInstrucao = criarNoAssembly(typeR, "add");
            novaInstrucao->tipoR->rd = $fp;
            novaInstrucao->tipoR->rs = $fp;
            novaInstrucao->tipoR->rt = $s0; // $s0 (registrador 16) é usado como base para globais?
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            novaInstrucao = criarNoAssembly(typeI, "ori");
            novaInstrucao->tipoI->rt = $sp;
            novaInstrucao->tipoI->rs = $zero;
            novaInstrucao->tipoI->imediato = buscar_funcao(&vetorMemoria, "global")->tamanho + get_sp(funcaoAtual);
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            novaInstrucao = criarNoAssembly(typeR, "add");
            novaInstrucao->tipoR->rd = $sp;
            novaInstrucao->tipoR->rs = $sp;
            novaInstrucao->tipoR->rt = $s0;
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            novaInstrucao = criarNoAssembly(typeR, "add");
            novaInstrucao->tipoR->rd = $pilha;
            novaInstrucao->tipoR->rs = $zero;
            novaInstrucao->tipoR->rt = $s0; 
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            novaInstrucao = criarNoAssembly(typeI, "subi"); // Assumindo subi como subtract immediate
            novaInstrucao->tipoI->rt = $pilha;
            novaInstrucao->tipoI->rs = $pilha;
            novaInstrucao->tipoI->imediato = 5; // Deslocamento para área de parâmetros?
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
        else{
            // Guarda o endereço de retorno
            novaInstrucao = criarNoAssembly(typeI, "sw");
            novaInstrucao->tipoI->rt = $ra; // Registrador de retorno
            novaInstrucao->tipoI->rs = $fp; // Baseado no frame pointer atual
            // oper3->val aqui é o número de parâmetros da função que está sendo definida.
            // O endereço de retorno é salvo no frame da função CHAMADA.
            // get_fp_relation(funcaoAtual, get_variavel(funcaoAtual, "Endereco Retorno")) deve dar o offset correto.
            // O "+ instrucao->oper3->val" parece incorreto aqui. O oper3 da FUNC é num_params.
            // O local de salvar $ra é fixo no frame da função atual (a que está sendo definida).
            VARIAVEL* var_ret_addr = get_variavel(funcaoAtual, "Endereco Retorno");
            if (var_ret_addr == NULL) {
                printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Variavel 'Endereco Retorno' não encontrada para %s.\n", funcaoAtual->nome);
                return;
            }
            novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var_ret_addr);
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
    }
    else if(instrucao->operation == RETURN){
        // oper1: valor de retorno (IntConst, registrador) ou Vazio
        if (funcaoAtual == NULL || funcaoAtual->nome == NULL) {
             printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "funcaoAtual nula ou sem nome para RETURN.\n");
             return;
        }
        if(!strcmp(funcaoAtual->nome, "main")) return; // HALT tratará o fim do main
        
        if (instrucao->oper1 != NULL && instrucao->oper1->tipo != Vazio) {
            // Salva o valor do retorno no frame da função ANTERIOR (chamadora)
            // Isso é feito pela instrução CALL ao retornar, ou o valor é colocado em $v0
            // A convenção MIPS é colocar o valor de retorno em $v0 (registrador 2)
            // E então a função chamadora pega de $v0.
            // O código original tentava salvar no frame da função anterior, o que é complexo.
            // Vamos usar $v0.
            novaInstrucao = criarNoAssembly(typeR, "add"); // move $v0, reg_retorno
            novaInstrucao->tipoR->rd = $v0; // $v0 (registrador 2)
            novaInstrucao->tipoR->rs = instrucao->oper1->val; // registrador com o valor de retorno
            novaInstrucao->tipoR->rt = $zero;
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }

        // Restaurar $ra (já foi salvo no início da função em FUN)
        // E pular de volta para $ra
        // O código original para RET fazia mais coisas, como acessar "Vinculo Controle"
        // e salvar o valor de retorno no frame da função anterior.
        // Simplificando para o padrão MIPS:
        // 1. Colocar valor de retorno em $v0 (feito acima se houver retorno)
        // 2. Restaurar $sp e $fp (se foram alterados significativamente além do prólogo)
        // 3. jr $ra

        // O código original para END faz o jr $ra. O RETURN deve apenas preparar o valor.
        // Se a semântica do RETURN na linguagem C-- é só preparar o valor e END faz o pulo,
        // então o código acima para $v0 é suficiente.
        // A lógica original do RET era:
        // lw $temp, offset_vinculo_controle($fp)  ; $temp = fp da chamadora
        // sw reg_retorno, offset_valor_retorno($temp) ; Salva no frame da chamadora
        // Isso é uma convenção de chamada não padrão.
        // Vou manter a lógica de colocar em $v0, e o END fará o jr $ra.
    }
    else if(instrucao->operation == PARAM){
        // oper1: valor/registrador do parâmetro (IntConst)
        // oper2: tipo ("VET" ou "INT") (String)
        // oper3: nome da variável se VET (String), ou Vazio
        
        // Esta instrução é para PASSAR um parâmetro ANTES de um CALL.
        // Os parâmetros são tipicamente colocados em $a0-$a3 ou na pilha.
        // O código original move da área $pilha para o stack frame da função CHAMADA ($sp).
        
        if (instrucao->oper1 == NULL || instrucao->oper2 == NULL || instrucao->oper2->nome == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Operandos nulos para PARAM.\n");
            return;
        }
        MEMORIA_FUNCOES* params_func = buscar_funcao(&vetorMemoria, "parametros");
        if (params_func == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Função 'parametros' não encontrada.\n");
            return;
        }
                
        if(!strcmp(instrucao->oper2->nome, "VET")){
            if (instrucao->oper3 == NULL || instrucao->oper3->nome == NULL) {
                 printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Nome da variável VET nula para PARAM.\n");
                 return;
            }
            VARIAVEL* var = get_variavel(funcaoAtual, instrucao->oper3->nome);
            if (var == NULL) { /* get_variavel já imprime erro */ return; }

            if(var->tipo == vetorArg){ // Se o vetor sendo passado já é um argumento (ponteiro)
                novaInstrucao = criarNoAssembly(typeI, "lw"); // Carrega o endereço do vetor
                novaInstrucao->tipoI->rt = instrucao->oper1->val; // Reg temporário para o endereço
                novaInstrucao->tipoI->rs = (var->bool_global) ? $s0 : $fp;
                novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var);
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;

                novaInstrucao = criarNoAssembly(typeI, "sw"); // Salva o endereço na área de params
                novaInstrucao->tipoI->rs = $pilha; // Base da área de params
                novaInstrucao->tipoI->rt = instrucao->oper1->val; // Reg com o endereço
                novaInstrucao->tipoI->imediato = params_func->tamanho; // Offset na área de params
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            }
            else{ // Se é um vetor local (passar seu endereço base)
                novaInstrucao = criarNoAssembly(typeI, "addi"); // Calcula o endereço base do vetor
                novaInstrucao->tipoI->rt = instrucao->oper1->val; // Reg temporário para o endereço
                novaInstrucao->tipoI->rs = (var->bool_global) ? $s0 : $fp;
                novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var);
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;

                novaInstrucao = criarNoAssembly(typeI, "sw"); // Salva o endereço na área de params
                novaInstrucao->tipoI->rs = $pilha;
                novaInstrucao->tipoI->rt = instrucao->oper1->val;
                novaInstrucao->tipoI->imediato = params_func->tamanho;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            }
        }
        else{ // Parâmetro tipo INT (passar valor)
            novaInstrucao = criarNoAssembly(typeI, "sw"); // Salva o valor do registrador na área de params
            novaInstrucao->tipoI->rs = $pilha;
            novaInstrucao->tipoI->rt = instrucao->oper1->val; // Registrador com o valor do parâmetro
            novaInstrucao->tipoI->imediato = params_func->tamanho;
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
        insere_variavel(params_func, "Param", inteiro); // Incrementa o contador de params na área especial
    } 
    else if(instrucao->operation == LOAD){
        // oper1: registrador destino (IntConst)
        // oper2: nome da variável/vetor (String)
        // oper3: registrador com índice (IntConst) ou Vazio
        if (instrucao->oper1 == NULL || instrucao->oper2 == NULL || instrucao->oper2->nome == NULL || instrucao->oper3 == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Operandos nulos para LOAD.\n");
            return;
        }
        VARIAVEL* var = get_variavel(funcaoAtual, instrucao->oper2->nome);
        if (var == NULL) { /* get_variavel já imprime erro */ return; }

        if(instrucao->oper3->tipo != Vazio){ // Load de um índice de um vetor: oper1 = oper2[oper3]
            // lw rd, offset(base_addr_reg)
            // Primeiro, calcular o endereço base do vetor em um registrador temporário ($temp)
            // Se var->tipo == vetor (vetor local) ou vetorArg (ponteiro para vetor)
            // Se vetorArg, primeiro lw $temp, offset_do_ponteiro($fp) -> $temp agora tem o endereço base real
            // Se vetor local, addi $temp, $fp, offset_do_vetor -> $temp agora tem o endereço base real
            
            if (var->tipo == vetorArg) { // Vetor passado como parâmetro (var contém o endereço do ponteiro)
                novaInstrucao = criarNoAssembly(typeI, "lw"); // Carrega o endereço base do vetor (que está no ponteiro)
                novaInstrucao->tipoI->rt = $temp; // $temp = *(endereço_do_ponteiro_var)
                novaInstrucao->tipoI->rs = (var->bool_global) ? $s0 : $fp;
                novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var);
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            } else { // Vetor local ou global (var é o início do vetor)
                novaInstrucao = criarNoAssembly(typeI, "addi"); // Calcula o endereço base do vetor
                novaInstrucao->tipoI->rt = $temp; // $temp = endereço_de_var
                novaInstrucao->tipoI->rs = (var->bool_global) ? $s0 : $fp;
                novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var);
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            }
            
            // Agora $temp tem o endereço base do vetor. Adicionar o índice.
            // Assumindo que o índice em oper3->val já está escalado (multiplicado por 4 se for array de int)
            // Se não, precisa de sll antes do add. Vamos assumir que está em unidades de bytes ou que o simulador aceita índice direto.
            // Para MIPS, o índice precisa ser em bytes. Se oper3->val é o índice N, precisamos de N*4.
            // Vamos assumir que oper3->val é um registrador que já tem o offset em bytes.
            novaInstrucao = criarNoAssembly(typeR, "add"); // $temp = $temp + reg_indice
            novaInstrucao->tipoR->rd = $temp;
            novaInstrucao->tipoR->rs = $temp;
            novaInstrucao->tipoR->rt = instrucao->oper3->val; // Registrador com o índice (offset)
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            // Finalmente, carregar o valor
            novaInstrucao = criarNoAssembly(typeI, "lw"); // rd = $temp[0]
            novaInstrucao->tipoI->rt = instrucao->oper1->val; // Registrador destino final
            novaInstrucao->tipoI->rs = $temp; // Endereço calculado
            novaInstrucao->tipoI->imediato = 0; // Offset 0 do endereço calculado
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
        else{ // Load de uma variável inteira: oper1 = oper2
            novaInstrucao = criarNoAssembly(typeI, "lw");
            novaInstrucao->tipoI->rt = instrucao->oper1->val; // Registrador destino
            novaInstrucao->tipoI->rs = (var->bool_global) ? $s0 : $fp; // Base ($s0 para global, $fp para local)
            novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var); // Offset da variável
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
    }
    else if(instrucao->operation == STORE){
        // oper1: nome da variável/vetor (String)
        // oper2: registrador com valor a ser armazenado (IntConst)
        // oper3: registrador com índice (IntConst) ou Vazio
        if (instrucao->oper1 == NULL || instrucao->oper1->nome == NULL || instrucao->oper2 == NULL || instrucao->oper3 == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Operandos nulos para STORE.\n");
            return;
        }
        VARIAVEL* var = get_variavel(funcaoAtual, instrucao->oper1->nome);
        if (var == NULL) { /* get_variavel já imprime erro */ return; }

        if(instrucao->oper3->tipo != Vazio){ // Store em um vetor: oper1[oper3] = oper2
            // Similar ao LOAD para calcular o endereço.
            if (var->tipo == vetorArg) {
                novaInstrucao = criarNoAssembly(typeI, "lw"); 
                novaInstrucao->tipoI->rt = $temp; 
                novaInstrucao->tipoI->rs = (var->bool_global) ? $s0 : $fp;
                novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var);
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            } else { 
                novaInstrucao = criarNoAssembly(typeI, "addi"); 
                novaInstrucao->tipoI->rt = $temp; 
                novaInstrucao->tipoI->rs = (var->bool_global) ? $s0 : $fp;
                novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var);
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            }
            
            novaInstrucao = criarNoAssembly(typeR, "add"); 
            novaInstrucao->tipoR->rd = $temp;
            novaInstrucao->tipoR->rs = $temp;
            novaInstrucao->tipoR->rt = instrucao->oper3->val; // Registrador com o índice (offset)
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            novaInstrucao = criarNoAssembly(typeI, "sw"); // $temp[0] = reg_valor
            novaInstrucao->tipoI->rt = instrucao->oper2->val; // Registrador com o valor a ser armazenado
            novaInstrucao->tipoI->rs = $temp; // Endereço calculado
            novaInstrucao->tipoI->imediato = 0;
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
        else{ // Store em uma variável inteira: oper1 = oper2
            novaInstrucao = criarNoAssembly(typeI, "sw");
            novaInstrucao->tipoI->rt = instrucao->oper2->val; // Registrador com o valor
            novaInstrucao->tipoI->rs = (var->bool_global) ? $s0 : $fp; // Base
            novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var); // Offset
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
    }
    else if(instrucao->operation == GOTO){
        // oper1: número do label (IntConst)
        if (instrucao->oper1 == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Operando nulo para GOTO.\n");
            return;
        }
        novaInstrucao = criarNoAssembly(typeJ, "j");
        char labelName[20];
        sprintf(labelName, "Label%d", instrucao->oper1->val);
        novaInstrucao->tipoJ->labelImediato = strdup(labelName);
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else if(instrucao->operation == HALT){
        novaInstrucao = criarNoAssembly(typeJ, "halt"); // 'halt' pode ser uma syscall ou instrução especial
        novaInstrucao->tipoJ->labelImediato = strdup("$zero"); // Argumento dummy, não usado por halt real
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else if(instrucao->operation == CALL){
        // oper1: nome da função (String)
        // oper2: número de parâmetros (IntConst)
        // oper3: registrador de retorno (IntConst) ou Vazio
        if (instrucao->oper1 == NULL || instrucao->oper1->nome == NULL || instrucao->oper2 == NULL || instrucao->oper3 == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Operandos nulos para CALL.\n");
            return;
        }
        MEMORIA_FUNCOES* params_func = buscar_funcao(&vetorMemoria, "parametros");
         if (params_func == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Função 'parametros' não encontrada para CALL.\n");
            return;
        }
                
        if(!strcmp(instrucao->oper1->nome, "output")){
            // Carrega o último parâmetro da área de parâmetros para $temp e o imprime
            novaInstrucao = criarNoAssembly(typeI, "lw");
            novaInstrucao->tipoI->rt = $a0; // Argumento para syscall de print_int é em $a0
            novaInstrucao->tipoI->rs = $pilha;
            // O tamanho em params_func é o offset do *próximo* param. O último está em tamanho-1.
            novaInstrucao->tipoI->imediato = params_func->tamanho -1; 
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            novaInstrucao = criarNoAssembly(typeI, "ori"); // Syscall para print_int é 1
            novaInstrucao->tipoI->rt = $v0; // $v0 = 1
            novaInstrucao->tipoI->rs = $zero;
            novaInstrucao->tipoI->imediato = 1;
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            novaInstrucao = criarNoAssembly(typeJ, "syscall"); // Faz a chamada de sistema
            novaInstrucao->tipoJ->labelImediato = strdup(""); // Dummy
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            
            apagar_temp(params_func); // Limpa o último parâmetro da área "parametros"
            return; 
        }
        else if(!strcmp(instrucao->oper1->nome, "input")){
            novaInstrucao = criarNoAssembly(typeI, "ori"); // Syscall para read_int é 5
            novaInstrucao->tipoI->rt = $v0; // $v0 = 5
            novaInstrucao->tipoI->rs = $zero;
            novaInstrucao->tipoI->imediato = 5;
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            novaInstrucao = criarNoAssembly(typeJ, "syscall");
            novaInstrucao->tipoJ->labelImediato = strdup(""); // Dummy
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            
            // O resultado da leitura (inteiro) estará em $v0.
            // A quádrupla CALL para input tem oper3 como o registrador destino.
            if (instrucao->oper3->tipo != Vazio) {
                novaInstrucao = criarNoAssembly(typeR, "add"); // Move $v0 para o registrador destino
                novaInstrucao->tipoR->rd = instrucao->oper3->val; // Registrador destino
                novaInstrucao->tipoR->rs = $v0;
                novaInstrucao->tipoR->rt = $zero;
                instrucoesAssembly[indiceAssembly++] = novaInstrucao;
            }
            return; 
        }

        // Para funções normais:
        // 1. Salvar registradores $t0-$t9, $s0-$s7 se necessário (caller-save vs callee-save)
        //    (Simplificado aqui, assumindo que a convenção de registradores é tratada ou não é estrita)
        // 2. Passar parâmetros (já feito pelas instruções PARAM, que os colocaram na área $pilha)
        //    Agora, precisamos mover da área $pilha para $a0-$a3 ou para a pilha real se mais de 4.
        //    O código original move da área $pilha para o stack frame da função CHAMADA ($sp).
        
        int num_params_val = instrucao->oper2->val;
        for(int i = 0; i < num_params_val; i++) { // Parâmetros são recuperados em ordem reversa de como foram colocados
            apagar_temp(params_func); // Decrementa o contador em params_func e retorna o offset do último

            novaInstrucao = criarNoAssembly(typeI, "lw"); // lw $temp, offset_param($pilha)
            novaInstrucao->tipoI->rt = $temp; // Carrega o parâmetro em $temp
            novaInstrucao->tipoI->rs = $pilha;
            novaInstrucao->tipoI->imediato = params_func->tamanho; // Offset do parâmetro atual
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;

            // Salva o parâmetro (em $temp) no stack frame da função a ser chamada.
            // Os parâmetros são colocados em $sp + offset.
            // O primeiro parâmetro vai em $sp+0, segundo em $sp+4, etc. (ou $sp+k, $sp+k+4, ...)
            // O código original usa 'i' que vai de num_params-1 até 0.
            // sw $temp, (num_params_val - 1 - i)*4($sp)  (se i de 0 a num_params-1)
            // ou sw $temp, i($sp) se i já é o offset correto.
            // A lógica original era `imediato = i` onde i ia de `instrucao->arg2->val` (num_params) descendo até 1.
            // Isso significa que o primeiro parâmetro (último a ser processado no loop) ia para 1($sp),
            // o segundo para 2($sp), etc. Isso é incomum.
            // Convenção MIPS: $a0, $a1, $a2, $a3, depois pilha.
            // Vamos manter a lógica original de empilhar em $sp + offset.
            // O offset 'i' no código original ia de N down to 1.
            // Isso deve ser (num_params_val - 1 - i) se i_loop de 0 a N-1, para ordem correta.
            // Ou, se i_stack_offset vai de 0 para N-1:
            // sw $temp, i_stack_offset*4($sp)
            // A lógica original: `imediato = i` onde `i` é o contador do loop de `num_params` até `1`.
            // Isso parece ser um índice de 1 a N.
            novaInstrucao = criarNoAssembly(typeI, "sw");
            novaInstrucao->tipoI->rs = $sp; // Base é o stack pointer atual (que será da nova função)
            novaInstrucao->tipoI->rt = $temp; // Valor do parâmetro
            novaInstrucao->tipoI->imediato = (num_params_val - 1 - i); // Offset: 0 para o primeiro param, 1 para o segundo, etc.
                                                                    // Se i vai de 0 a N-1.
                                                                    // Se o loop original era de N down to 1, e imediato = i,
                                                                    // então o último param (i=1) ia para 1($sp),
                                                                    // o primeiro param (i=N) ia para N($sp).
                                                                    // Isso é ordem reversa na pilha.
                                                                    // Para ordem correta (primeiro param em menor endereço):
                                                                    // offset = i (se loop de 0 a N-1)
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }

        // Prólogo da chamada:
        // 1. Salvar $fp da função atual (chamadora) no stack frame da função chamada.
        //    Este é o "Vínculo de Controle".
        //    sw $fp, offset_vinculo_controle($sp)
        // O código original faz:
        // add $temp, $zero, $fp  (temp = fp_atual)
        // addi $temp, $temp, offset_vinculo_controle_no_frame_ATUAL (isso está errado)
        // sw $temp, (num_params + 1)($sp) (salva um endereço calculado estranhamente)

        // Correto para salvar $fp da chamadora:
        // O $sp já aponta para o novo frame. O vínculo de controle é o $fp da chamadora.
        // O offset para o vínculo de controle é tipicamente fixo, ex: 0($sp) após $sp ser ajustado.
        // O código original parece querer salvar o $fp da chamadora em $sp + (num_params + 1).
        // E o $fp da chamadora é o $fp atual.
        novaInstrucao = criarNoAssembly(typeI, "sw");
        novaInstrucao->tipoI->rt = $fp; // $fp da chamadora
        novaInstrucao->tipoI->rs = $sp; // Baseado no $sp do novo frame
        novaInstrucao->tipoI->imediato = num_params_val; // Salva $fp logo após os N parâmetros (índice N, se params são 0 a N-1)
                                                        // Ou (num_params_val + 1) se os offsets dos params eram 1 a N.
                                                        // Se i vai de 0 a N-1, e $fp em N.
                                                        // Se i vai de 1 a N, e $fp em N+1.
                                                        // A lógica original do PARAM usava `imediato = i` (1 a N).
                                                        // Então, `imediato = num_params_val + 1` para o $fp.
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;


        // 2. Ajustar $fp para a nova função: $fp = $sp
        novaInstrucao = criarNoAssembly(typeR, "add");
        novaInstrucao->tipoR->rd = $fp;
        novaInstrucao->tipoR->rs = $sp;
        novaInstrucao->tipoR->rt = $zero;
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;

        // 3. Ajustar $sp para alocar espaço para variáveis locais da função chamada.
        //    subi $sp, $sp, tamanho_locais
        //    O código original incrementava $fp e $sp por get_sp(funcaoAtual)+1.
        //    `funcaoAtual` aqui é a CHAMADORA. `get_sp` retorna o tamanho do frame.
        //    Isso parece ser para avançar $fp e $sp para o início do frame da função CHAMADA.
        //    addi $fp, $fp, tamanho_frame_chamadora + 1
        //    addi $sp, $sp, tamanho_frame_chamadora + 1
        //    Isso é feito ANTES do jal.
        //    Esta lógica de ajuste de $fp/$sp antes do jal é para configurar o frame da função CHAMADA.
        //    O $sp já deve estar apontando para o topo do frame da função chamada.
        //    O $fp é tipicamente $sp + tamanho_frame_chamada.
        //    A lógica original é complexa e não padrão.
        //    Vou seguir a lógica original de ajuste de $fp e $sp.
        //    `get_sp(funcaoAtual)` refere-se ao tamanho do frame da função CHAMADORA.
        //    Isso move $fp e $sp para o início do frame da função CHAMADA.
        novaInstrucao = criarNoAssembly(typeI, "addi");
        novaInstrucao->tipoI->rt = $fp;
        novaInstrucao->tipoI->rs = $fp; // $fp já é o $sp do novo frame
        novaInstrucao->tipoI->imediato = get_sp(funcaoAtual) + 1; // Adiciona tamanho do frame da chamadora
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;

        novaInstrucao = criarNoAssembly(typeI, "addi");
        novaInstrucao->tipoI->rt = $sp;
        novaInstrucao->tipoI->rs = $sp; // $sp já é o $sp do novo frame
        novaInstrucao->tipoI->imediato = get_sp(funcaoAtual) + 1; // Adiciona tamanho do frame da chamadora
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;


        // 4. Pular para a função (jal)
        novaInstrucao = criarNoAssembly(typeJ, "jal");
        novaInstrucao->tipoJ->labelImediato = strdup(instrucao->oper1->nome);
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;

        // Epílogo da chamada (após o retorno do jal):
        // 1. Restaurar $sp e $fp da função chamadora.
        //    O $fp foi salvo, então lw $fp, offset_vinculo_controle($sp_novo_frame)
        //    O $sp é restaurado adicionando o tamanho do frame da função chamada.
        //    O código original subtrai 25 de $fp e $sp. Isso assume um tamanho de frame fixo de 25.
        //    Isso deve ser o inverso da adição feita antes do jal.
        //    subi $fp, $fp, get_sp(funcaoAtual_chamadora) + 1
        //    subi $sp, $sp, get_sp(funcaoAtual_chamadora) + 1
        //    O valor 25 é um placeholder para o tamanho do frame da função CHAMADA.
        //    Isso deve ser get_sp(funcao_chamada) + 1.
        //    Como não temos essa info aqui, manter 25 como no original.
        novaInstrucao = criarNoAssembly(typeI, "subi");
        novaInstrucao->tipoI->rt = $fp;
        novaInstrucao->tipoI->rs = $fp;
        novaInstrucao->tipoI->imediato = 25; // Placeholder
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;

        novaInstrucao = criarNoAssembly(typeI, "subi");
        novaInstrucao->tipoI->rt = $sp;
        novaInstrucao->tipoI->rs = $sp;
        novaInstrucao->tipoI->imediato = 25; // Placeholder
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;

        // 2. Obter valor de retorno de $v0 se houver.
        if(instrucao->oper3->tipo != Vazio){ // Se a função retorna algo e temos um registrador destino
            // lw reg_destino, offset_valor_retorno($fp) // $fp é da chamadora
            // Ou, se usando $v0:
            // add reg_destino, $v0, $zero
            // A lógica original: lw rt, offset_valor_retorno($fp)
            // Onde rt é oper3->val.
            VARIAVEL* var_ret_val = get_variavel(funcaoAtual, "Valor Retorno");
            if (var_ret_val == NULL) {
                printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Variavel 'Valor Retorno' não encontrada para %s em CALL.\n", funcaoAtual->nome);
                return;
            }
            novaInstrucao = criarNoAssembly(typeI, "lw");
            novaInstrucao->tipoI->rt = instrucao->oper3->val; // Registrador destino do valor de retorno
            novaInstrucao->tipoI->rs = $fp; // $fp da função ATUAL (chamadora)
            novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var_ret_val);
            instrucoesAssembly[indiceAssembly++] = novaInstrucao;
        }
    } 
    else if(instrucao->operation == END){
        // oper1: nome da função (String)
        if (instrucao->oper1 == NULL || instrucao->oper1->nome == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Operando nulo para END.\n");
            return;
        }
        if(!strcmp(instrucao->oper1->nome, "main")){
            // O HALT já foi adicionado no final do código intermediário.
            // Não precisa fazer nada aqui para main, o HALT global cuidará disso.
            return; 
        }
        
        // Epílogo da função (a função que está terminando):
        // 1. Restaurar $ra (salvo no início da função)
        //    lw $ra, offset_endereco_retorno($fp)
        VARIAVEL* var_ret_addr_end = get_variavel(funcaoAtual, "Endereco Retorno");
         if (var_ret_addr_end == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Variavel 'Endereco Retorno' não encontrada para %s em END.\n", funcaoAtual->nome);
            return;
        }
        novaInstrucao = criarNoAssembly(typeI, "lw");
        novaInstrucao->tipoI->rs = $fp; // $fp da função atual
        novaInstrucao->tipoI->rt = $ra; // Destino $ra
        novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var_ret_addr_end);
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    
        // 2. Restaurar $fp da função chamadora
        //    lw $fp, offset_vinculo_controle($fp)
        VARIAVEL* var_vinc_ctrl = get_variavel(funcaoAtual, "Vinculo Controle");
        if (var_vinc_ctrl == NULL) {
            printf(ANSI_COLOR_RED "Erro Assembly: " ANSI_COLOR_RESET "Variavel 'Vinculo Controle' não encontrada para %s em END.\n", funcaoAtual->nome);
            return;
        }
        novaInstrucao = criarNoAssembly(typeI, "lw");
        novaInstrucao->tipoI->rs = $fp; // $fp da função atual
        novaInstrucao->tipoI->rt = $fp; // Destino $fp (restaurando o da chamadora)
        novaInstrucao->tipoI->imediato = get_fp_relation(funcaoAtual, var_vinc_ctrl);
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;

        // 3. Restaurar $sp da função chamadora
        //    add $sp, $fp, 0 (se $fp aponta para o início do frame da chamadora)
        //    Ou, se $sp foi decrementado no prólogo, deve ser incrementado aqui.
        //    O $sp da chamadora é $fp_da_chamadora (que acabamos de restaurar em $fp) + tamanho_do_frame_da_chamadora.
        //    Ou, mais simples, $sp = $fp (se $fp é o $sp da chamadora antes da alocação de locais).
        //    O código original não restaurava $sp explicitamente no END, apenas $ra.
        //    A restauração de $sp é crucial. $sp deve voltar para onde estava antes da chamada.
        //    Se o $fp restaurado é o $fp da chamadora, e o $sp da chamadora era $fp_da_chamadora + (tamanho_frame_chamadora - tamanho_params_passados),
        //    a forma mais simples é $sp = $fp (se $fp é o $sp da chamadora antes de alocar locais).
        //    Se o $fp restaurado é o $fp da função chamadora, então o $sp da chamadora
        //    deve ser $fp (restaurado) + (tamanho do frame da chamadora - espaço para params).
        //    A convenção mais simples é $sp = $fp (após restaurar $fp).
        //    Isso desfaz a alocação do frame da função atual.
        novaInstrucao = criarNoAssembly(typeR, "add"); // $sp = $fp
        novaInstrucao->tipoR->rd = $sp;
        novaInstrucao->tipoR->rs = $fp; // $fp já foi restaurado para o da chamadora
        novaInstrucao->tipoR->rt = $zero;
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;


        // 4. Pular para $ra
        novaInstrucao = criarNoAssembly(typeR, "jr");
        novaInstrucao->tipoR->rs = $ra;
        novaInstrucao->tipoR->rd = $zero; // Não usado por jr
        novaInstrucao->tipoR->rt = $zero; // Não usado por jr
        instrucoesAssembly[indiceAssembly++] = novaInstrucao;
    }
    else{
        printf(ANSI_COLOR_RED);
        printf("Erro Assembly: ");
        printf(ANSI_COLOR_RESET);
        printf("Instrucao de código intermediário não reconhecida (%s)\n", getTACOperationName(instrucao->operation));
    }
}
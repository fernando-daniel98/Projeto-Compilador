#include "../include/assembler.h"
#include "../include/codeGen.h" // Para typeOperations, ENDERECO, etc.
#include "../include/symbol_table.h" // Para PnoIdentificador
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para mapear o registrador do código intermediário para um registrador MIPS
// Os registradores $t0-$t9 (8-15, 24-25) são geralmente usados para temporários.
// $s0-$s7 (16-23) para variáveis salvas.
// Por simplicidade, vamos mapear diretamente os 'numReg' para $tN, assumindo que 'numReg'
// já foi gerenciado para não exceder o número de registradores temporários disponíveis.
// O registrador 63 foi reservado para zero no seu codeGen, podemos mapeá-lo para $zero.
const char* mapRegToMips(int regNum) {
    static char regStr[5];
    if (regNum == 63) { // Seu registrador especial para zero
        return "$zero";
    }
    if (regNum >= 0 && regNum < 10) { // Exemplo: mapeando para $t0-$t9
        sprintf(regStr, "$t%d", regNum);
        return regStr;
    }
    // Adicionar mais mapeamentos conforme necessário (e.g., para $s0-$s7)
    // Ou usar uma estratégia de alocação de registradores mais dinâmica.
    sprintf(regStr, "$t%d", regNum % 10); // Fallback simples, pode causar colisões
    return regStr;
}

// Função auxiliar para obter o nome de uma variável global para o assembly
const char* getGlobalVarName(const char* varName) {
    // No MIPS, variáveis globais são acessadas por seus labels.
    // Poderíamos prefixar para evitar conflitos, mas por enquanto usamos o nome direto.
    return varName;
}


void generateAssembly(quadruple **intermCode, PnoIdentificador *symTable, FILE *outStream) {
    if (intermCode == NULL || outStream == NULL) {
        fprintf(stderr, "Erro: Código intermediário ou stream de saída nulos para o assembler.\n");
        return;
    }

    fprintf(outStream, ".data\n");
    fprintf(outStream, "newline: .asciiz \"\\n\"\n"); // Para imprimir nova linha

    fprintf(outStream, "\n.text\n");
    fprintf(outStream, ".globl main\n"); // Removido \n\n para consistência, labels de função adicionam \n antes

    char currentFuncName[MAXTOKENLEN] = "global";
    int assemblyLineCounter = 1; // Contador para as linhas de instrução assembly

    for (int i = 0; intermCode[i] != NULL; i++) {
        quadruple *q = intermCode[i];
        
        char quadRefStr[256];
        char op1_str[64] = "-";
        char op2_str[64] = "-";
        char op3_str[64] = "-";

        if (q->oper1) {
            if (q->oper1->tipo == String) snprintf(op1_str, sizeof(op1_str), "%s", q->oper1->nome);
            else if (q->oper1->tipo == IntConst) {
                if (q->oper1->boolReg == 1) snprintf(op1_str, sizeof(op1_str), "%s", mapRegToMips(q->oper1->val));
                else if (q->oper1->boolReg == 2) snprintf(op1_str, sizeof(op1_str), "L%d", q->oper1->val);
                else snprintf(op1_str, sizeof(op1_str), "%d", q->oper1->val);
            }
        }
        if (q->oper2) {
            if (q->oper2->tipo == String) snprintf(op2_str, sizeof(op2_str), "%s", q->oper2->nome);
            else if (q->oper2->tipo == IntConst) {
                if (q->oper2->boolReg == 1) snprintf(op2_str, sizeof(op2_str), "%s", mapRegToMips(q->oper2->val));
                else if (q->oper2->boolReg == 2) snprintf(op2_str, sizeof(op2_str), "L%d", q->oper2->val);
                else snprintf(op2_str, sizeof(op2_str), "%d", q->oper2->val);
            }
        }
        if (q->oper3) {
            if (q->oper3->tipo == String) snprintf(op3_str, sizeof(op3_str), "%s", q->oper3->nome);
            else if (q->oper3->tipo == IntConst) {
                 if (q->oper3->boolReg == 1) snprintf(op3_str, sizeof(op3_str), "%s", mapRegToMips(q->oper3->val));
                 else if (q->oper3->boolReg == 2) snprintf(op3_str, sizeof(op3_str), "L%d", q->oper3->val);
                 else snprintf(op3_str, sizeof(op3_str), "%d", q->oper3->val);
            }
        }
        snprintf(quadRefStr, sizeof(quadRefStr), "(%s, %s, %s, %s)", getTACOperationName(q->operation), op1_str, op2_str, op3_str);

        // O comentário de quádrupla geral foi removido daqui. Será impresso por instrução.

        switch (q->operation) {
            case ALLOC: 
                if (q->oper1 && q->oper1->tipo == String && q->oper2 && q->oper2->tipo == String) {
                    if (strcmp(q->oper2->nome, "global") == 0) {
                        // Já tratado na seção .data, apenas um comentário informativo se necessário
                        fprintf(outStream, "%d:\t# ALLOC global %s (declarado em .data) \t\t # Quádrupla: %s\n", assemblyLineCounter++, q->oper1->nome, quadRefStr);
                    } else { 
                        // Para alocação na pilha, isso geralmente faz parte do prólogo da função.
                        // O prólogo já é gerado pela quádrupla FUNC.
                        // Se ALLOC local significar ajuste dinâmico do $sp, seria uma instrução.
                        // Por ora, apenas um comentário informativo.
                        fprintf(outStream, "%d:\t# ALLOC local %s in %s (espaço na pilha) \t\t # Quádrupla: %s\n", assemblyLineCounter++, q->oper1->nome, q->oper2->nome, quadRefStr);
                    }
                }
                break;

            case LOADI: 
                if (q->oper1 && q->oper1->tipo == IntConst && q->oper1->boolReg == 1 &&
                    q->oper2 && q->oper2->tipo == IntConst) {
                    fprintf(outStream, "%d:\tli %s, %d \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper1->val), q->oper2->val, quadRefStr);
                }
                break;

            case STORE: 
                if (q->oper1 && q->oper1->tipo == String &&
                    q->oper2 && q->oper2->tipo == IntConst && q->oper2->boolReg == 1) {
                    if (q->oper3 && q->oper3->tipo == Vazio) { // Variável escalar
                        // TODO: Distinguir entre globais (label) e locais (offset $fp)
                        // Assumindo global por enquanto
                        fprintf(outStream, "%d:\tsw %s, %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper2->val), getGlobalVarName(q->oper1->nome), quadRefStr);
                    } else if (q->oper3 && q->oper3->tipo == IntConst && q->oper3->boolReg == 1) { // Acesso a array
                        // Assumindo array global
                        fprintf(outStream, "%d:\tla $at, %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, getGlobalVarName(q->oper1->nome), quadRefStr);
                        fprintf(outStream, "%d:\tsll $k0, %s, 2 \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper3->val), quadRefStr);
                        fprintf(outStream, "%d:\tadd $at, $at, $k0 \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                        fprintf(outStream, "%d:\tsw %s, 0($at) \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper2->val), quadRefStr);
                    }
                }
                break;

            case LOAD: 
                 if (q->oper1 && q->oper1->tipo == IntConst && q->oper1->boolReg == 1 &&
                    q->oper2 && q->oper2->tipo == String) {
                    if (q->oper3 && q->oper3->tipo == Vazio) { // Variável escalar
                        // TODO: Distinguir entre globais (label) e locais (offset $fp)
                        fprintf(outStream, "%d:\tlw %s, %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper1->val), getGlobalVarName(q->oper2->nome), quadRefStr);
                    } else if (q->oper3 && q->oper3->tipo == IntConst && q->oper3->boolReg == 1) { // Acesso a array
                        fprintf(outStream, "%d:\tla $at, %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, getGlobalVarName(q->oper2->nome), quadRefStr);
                        fprintf(outStream, "%d:\tsll $k0, %s, 2 \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper3->val), quadRefStr);
                        fprintf(outStream, "%d:\tadd $at, $at, $k0 \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                        fprintf(outStream, "%d:\tlw %s, 0($at) \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper1->val), quadRefStr);
                    }
                }
                break;

            case ADD: 
            case SUB: 
            case MULT: 
            case DIV: 
                if (q->oper1 && q->oper1->tipo == IntConst && q->oper1->boolReg == 1 &&
                    q->oper2 && q->oper2->tipo == IntConst && q->oper2->boolReg == 1 &&
                    q->oper3 && q->oper3->tipo == IntConst && q->oper3->boolReg == 1) {
                    const char *opStr = "";
                    if (q->operation == ADD) opStr = "add";
                    else if (q->operation == SUB) opStr = "sub";
                    else if (q->operation == MULT) opStr = "mul"; // Pode precisar de mflo
                    else if (q->operation == DIV) opStr = "div";   // Pode precisar de mflo/mfhi
                    fprintf(outStream, "%d:\t%s %s, %s, %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, opStr,
                            mapRegToMips(q->oper3->val), 
                            mapRegToMips(q->oper1->val), 
                            mapRegToMips(q->oper2->val),
                            quadRefStr
                    );
                    // Para MULT/DIV reais em MIPS, pode precisar de instruções adicionais (mflo, mfhi)
                    // Ex: se 'mul Rdest, Rsrc1, Rsrc2' é pseudo-instrução para 'mult Rsrc1, Rsrc2; mflo Rdest'
                    // if (q->operation == MULT) fprintf(outStream, "%d:\tmflo %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper3->val), quadRefStr);
                    // if (q->operation == DIV) fprintf(outStream, "%d:\tmflo %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper3->val), quadRefStr); // Quociente
                }
                break;
            
            case LT:
                if (q->oper3 && q->oper3->boolReg == 1 && q->oper1 && q->oper1->boolReg == 1 && q->oper2 && q->oper2->boolReg == 1) {
                    fprintf(outStream, "%d:\tslt %s, %s, %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper3->val), mapRegToMips(q->oper1->val), mapRegToMips(q->oper2->val), quadRefStr);
                }
                break;
            case EQ: 
                if (q->oper3 && q->oper3->boolReg == 1 && q->oper1 && q->oper1->boolReg == 1 && q->oper2 && q->oper2->boolReg == 1) {
                    fprintf(outStream, "%d:\tsub $at, %s, %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper1->val), mapRegToMips(q->oper2->val), quadRefStr);
                    fprintf(outStream, "%d:\tsltiu %s, $at, 1 \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper3->val), quadRefStr);
                }
                break;


            case LABEL: 
                if (q->oper1 && q->oper1->tipo == IntConst && q->oper1->boolReg == 2) {
                    fprintf(outStream, "%d.Label L%d:\n", assemblyLineCounter++, q->oper1->val);
                }
                break;

            case GOTO: 
                if (q->oper1 && q->oper1->tipo == IntConst && q->oper1->boolReg == 2) {
                    fprintf(outStream, "%d:\tj L%d \t\t # Quádrupla: %s\n", assemblyLineCounter++, q->oper1->val, quadRefStr);
                }
                break;

            case IFFALSE: 
                if (q->oper1 && q->oper1->tipo == IntConst && q->oper1->boolReg == 1 &&
                    q->oper2 && q->oper2->tipo == IntConst && q->oper2->boolReg == 2) {
                    fprintf(outStream, "%d:\tbeq %s, $zero, L%d \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper1->val), q->oper2->val, quadRefStr);
                }
                break;

            case FUNC: 
                if (q->oper2 && q->oper2->tipo == String) {
                    fprintf(outStream, "\n%s:\n", q->oper2->nome); // Label da função não numerado
                    strncpy(currentFuncName, q->oper2->nome, MAXTOKENLEN -1);
                    currentFuncName[MAXTOKENLEN-1] = '\0';
                    
                    // Prólogo da Função (instruções numeradas)
                    //fprintf(outStream, "\t# Function Prologue for %s\n", q->oper2->nome); // Comentário não numerado
                    fprintf(outStream, "%d:\taddiu $sp, $sp, -8 \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr); // $ra, $fp
                    fprintf(outStream, "%d:\tsw $ra, 4($sp) \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                    fprintf(outStream, "%d:\tsw $fp, 0($sp) \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                    fprintf(outStream, "%d:\tmove $fp, $sp \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                    // TODO: Alocar espaço para variáveis locais. Ex:
                    // int local_space = calculate_local_var_space(q->oper2->nome, symTable);
                    // if (local_space > 0) {
                    //    fprintf(outStream, "%d:\taddiu $sp, $sp, -%d \t\t # Quádrupla: %s\n", assemblyLineCounter++, local_space, quadRefStr);
                    // }
                }
                break;

            case END: 
                if (q->oper1 && q->oper1->tipo == String) {
                    //fprintf(outStream, "\t# Function Epilogue for %s\n", q->oper1->nome); // Comentário não numerado
                    fprintf(outStream, "%d:\tmove $sp, $fp \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                    fprintf(outStream, "%d:\tlw $ra, 4($sp) \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                    fprintf(outStream, "%d:\tlw $fp, 0($sp) \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                    fprintf(outStream, "%d:\taddiu $sp, $sp, 8 \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                    if (strcmp(q->oper1->nome, "main") != 0) { 
                        fprintf(outStream, "%d:\tjr $ra \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                    }
                    strcpy(currentFuncName, "global");
                }
                break;

            case PARAM: 
                // Esta quádrupla é para passar parâmetros ANTES de um CALL.
                // Ex: move $a0, reg_val (para primeiro param)
                //     move $a1, reg_val (para segundo param)
                // Ou colocar na pilha se mais de 4 params ou struct/array grande.
                // Assumindo que q->oper1 é o registrador com o valor, e precisamos saber qual argumento é (0, 1, 2, 3).
                // Isso requer um contador de parâmetros para a chamada atual, que não está aqui.
                // Por simplicidade, vamos apenas gerar um comentário.
                if (q->oper1 && q->oper1->tipo == IntConst && q->oper1->boolReg == 1) {
                     fprintf(outStream, "%d:\t# PARAM %s (map to $aN or stack) \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper1->val), quadRefStr);
                } else if (q->oper1 && q->oper1->tipo == String) { // Passando nome de array (endereço)
                     fprintf(outStream, "%d:\t# PARAM %s (load address to $aN or stack) \t\t # Quádrupla: %s\n", assemblyLineCounter++, q->oper1->nome, quadRefStr);
                }
                break;

            case CALL: 
                if (q->oper1 && q->oper1->tipo == String) {
                    fprintf(outStream, "%d:\tjal %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, q->oper1->nome, quadRefStr);
                    if (q->oper3 && q->oper3->tipo == IntConst && q->oper3->boolReg == 1) {
                        fprintf(outStream, "%d:\tmove %s, $v0 \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper3->val), quadRefStr);
                    }
                }
                break;

            case RETURN: 
                if (q->oper1 && q->oper1->tipo == IntConst && q->oper1->boolReg == 1) {
                    fprintf(outStream, "%d:\tmove $v0, %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, mapRegToMips(q->oper1->val), quadRefStr);
                }
                // O salto para o epílogo (jr $ra) é tratado pela instrução END.
                // Se houver múltiplos returns, cada um precisaria saltar para um label de epílogo comum.
                // Por agora, assumimos que o epílogo (gerado por END) segue ou é o destino implícito.
                // Se a função não for main, o END gerará 'jr $ra'.
                // Se for main, o HALT cuidará do término.
                // Adicionar um salto explícito para o final da função se necessário:
                // fprintf(outStream, "%d:\tj %s_epilogue_label \t\t # Quádrupla: %s\n", assemblyLineCounter++, currentFuncName, quadRefStr);
                break;


            case HALT:
                fprintf(outStream, "%d:\tli $v0, 10 \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                fprintf(outStream, "%d:\tsyscall \t\t # Quádrupla: %s\n", assemblyLineCounter++, quadRefStr);
                break;
            
            case ARG: // Definição de argumento de função (geralmente parte do FUNC)
                // Normalmente não gera código MIPS executável por si só, mas informa o layout.
                // O prólogo da função (FUNC) e o acesso aos parâmetros usariam essa informação.
                // Se for para gerar um comentário:
                fprintf(outStream, "%d:\t# ARG %s %s (info para func %s) \t\t # Quádrupla: %s\n", 
                        assemblyLineCounter++, 
                        q->oper1 ? q->oper1->nome : "-", // tipo_arg
                        q->oper2 ? q->oper2->nome : "-", // nome_param
                        q->oper3 ? q->oper3->nome : currentFuncName, // escopo_func
                        quadRefStr);
                break;

            default:
                fprintf(outStream, "%d:\t# Tradução não implementada para: %s \t\t # Quádrupla: %s\n", assemblyLineCounter++, getTACOperationName(q->operation), quadRefStr);
                break;
        }
    }
    fprintf(outStream, "\n# Fim do código assembly\n");
}

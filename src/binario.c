#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/codeGen.h"
#include "../include/assembler.h"
#include "../include/binario.h"
#include "../include/label.h"

// ===============================================
// MAPEAMENTO DE OPCODES CONFORME ESPECIFICAÇÃO JSON
// ===============================================

unsigned int get_opcode(char* nome, tipoInstrucao tipo) {
    if (tipo == typeR) {
        return 0b000000;
    }
    
    if (!strcmp(nome, "lw")) return 0b100011;
    if (!strcmp(nome, "sw")) return 0b101011;
    if (!strcmp(nome, "addi")) return 0b001000;
    if (!strcmp(nome, "subi")) return 0b001001;
    if (!strcmp(nome, "andi")) return 0b001100;
    if (!strcmp(nome, "ori")) return 0b001101;
    if (!strcmp(nome, "xori")) return 0b101101;
    if (!strcmp(nome, "beq")) return 0b000100;
    if (!strcmp(nome, "bneq")) return 0b000101;
    if (!strcmp(nome, "slti")) return 0b001010;
    if (!strcmp(nome, "in")) return 0b011111;
    if (!strcmp(nome, "out")) return 0b011110;
    
    if (!strcmp(nome, "j")) return 0b000010;
    if (!strcmp(nome, "jal")) return 0b000011;
    if (!strcmp(nome, "halt")) return 0b111111;
    
    printf("ERRO: Opcode não encontrado para instrução: %s\n", nome);
    return 0b111111;
}

// ===============================================
// MAPEAMENTO DE FUNCTIONS PARA INSTRUÇÕES R-TYPE
// ===============================================

unsigned int get_funct(char* nome) {
    if (!strcmp(nome, "add")) return 0b100000;
    if (!strcmp(nome, "sub")) return 0b100010;
    if (!strcmp(nome, "and")) return 0b100100;
    if (!strcmp(nome, "or")) return 0b100101;
    if (!strcmp(nome, "jr")) return 0b001000;
    if (!strcmp(nome, "jalr")) return 0b001001;
    if (!strcmp(nome, "slt")) return 0b101010;
    if(!strcmp(nome, "nor")) return 0b100111;
    if (!strcmp(nome, "sll")) return 0b000000;
    if (!strcmp(nome, "srl")) return 0b000010;
    if (!strcmp(nome, "div")) return 0b011010;
    if (!strcmp(nome, "mult")) return 0b011000;
    if(!strcmp(nome, "xor")) return 0b101101;
    printf("ERRO: Function code não encontrado para instrução R-Type: %s\n", nome);
    return 0b000000;
}

// ===============================================
// FUNÇÕES AUXILIARES
// ===============================================

unsigned int get_register(int reg){
    return reg;
}

unsigned int get_shamt(int shamt){
    return shamt;
}

unsigned int get_immediate(int imediato){
    return imediato;
}	

unsigned int get_address(char* label){
    return getEnderecoLabel(label);
}

// ===============================================
// GERAÇÃO DE INSTRUÇÕES BINÁRIAS
// ===============================================

BIN_R* binarioNop(void) {
    BIN_R* bin = (BIN_R*)malloc(sizeof(BIN_R));
    if (bin == NULL) {
        printf("ERRO: Falha na alocação de memória para NOP\n");
        return NULL;
    }
    
    bin->opcode = 0b000000;
    bin->rs = $zero;
    bin->rt = $zero;
    bin->rd = $zero;
    bin->shamt = 0;
    bin->funct = 0b100000;
    
    return bin;
}

BIN_R* binarioR(ASSEMBLY* instrucao) {
    if (instrucao == NULL || instrucao->tipoR == NULL) {
        printf("ERRO: Instrução R-Type inválida\n");
        return binarioNop();
    }
    
    BIN_R* bin = (BIN_R*)malloc(sizeof(BIN_R));
    if (bin == NULL) {
        printf("ERRO: Falha na alocação de memória para instrução R-Type\n");
        return binarioNop();
    }
    
    bin->opcode = get_opcode(instrucao->tipoR->nome, instrucao->tipo);
    bin->rs = get_register(instrucao->tipoR->rs);
    bin->rt = get_register(instrucao->tipoR->rt);
    bin->rd = get_register(instrucao->tipoR->rd);
    bin->shamt = get_shamt(0);
    bin->funct = get_funct(instrucao->tipoR->nome);
    
    return bin;
}

BIN_I* binarioI(ASSEMBLY* instrucao){
    BIN_I* bin = (BIN_I*)malloc(sizeof(BIN_I));
    bin->opcode = get_opcode(instrucao->tipoI->nome, instrucao->tipo);
    bin->rs = get_register(instrucao->tipoI->rs);
    bin->rt = get_register(instrucao->tipoI->rt);
    
    if(!strcmp(instrucao->tipoI->nome, "bne") || !strcmp(instrucao->tipoI->nome, "beq")){
        char label[26];
        sprintf(label, "Label %d", instrucao->tipoI->label);
        bin->immediate = get_address(label);
    }
    else{
        bin->immediate = get_immediate(instrucao->tipoI->imediato);
    }

    return bin;
}

BIN_J* binarioJ(ASSEMBLY* instrucao){
    BIN_J* bin = (BIN_J*)malloc(sizeof(BIN_J));
    bin->opcode = get_opcode(instrucao->tipoJ->nome, instrucao->tipo);
    bin->address = get_address(instrucao->tipoJ->labelImediato);
    return bin;
}

// ===============================================
// FUNÇÕES DE SAÍDA BINÁRIA
// ===============================================

void printBits(size_t const size, void const * const ptr, FILE* arquivo) {
    if (arquivo == NULL || ptr == NULL) {
        printf("ERRO: Arquivo ou ponteiro NULL em printBits\n");
        return;
    }
    
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    // Imprimir bits do MSB para LSB (big-endian)
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            fprintf(arquivo, "%u", byte);
        }
    }
}

void binario(FILE* arquivo) {
    if (arquivo == NULL) {
        printf("ERRO: Arquivo NULL fornecido para geração binária\n");
        return;
    }
    
    if (instrucoesAssembly == NULL) {
        printf("ERRO: Array de instruções assembly não inicializado\n");
        return;
    }
    
    BIN_I* binI;
    BIN_J* binJ;
    BIN_R* binR;
    
    for (int i = 0; i < indiceAssembly; i++) {
        if (instrucoesAssembly[i] == NULL) {
            binR = binarioNop();
            if (binR != NULL) {
                printBits(sizeof(*binR), binR, arquivo);
                free(binR);
            }
            fprintf(arquivo, "\n");
            continue;
        }
        
        switch (instrucoesAssembly[i]->tipo) {
            case typeR:
                binR = binarioR(instrucoesAssembly[i]);
                if (binR != NULL) {
                    printBits(sizeof(*binR), binR, arquivo);
                    free(binR);
                }
                break;
                
            case typeI:
                binI = binarioI(instrucoesAssembly[i]);
                if (binI != NULL) {
                    printBits(sizeof(*binI), binI, arquivo);
                    free(binI);
                }
                break;
                
            case typeJ:
                binJ = binarioJ(instrucoesAssembly[i]);
                if (binJ != NULL) {
                    printBits(sizeof(*binJ), binJ, arquivo);
                    free(binJ);
                }
                break;
                
            case typeLabel:
                // Labels não geram código, usar NOP como placeholder
                binR = binarioNop();
                if (binR != NULL) {
                    printBits(sizeof(*binR), binR, arquivo);
                    free(binR);
                }
                break;
                
            default:
                binR = binarioNop();
                if (binR != NULL) {
                    printBits(sizeof(*binR), binR, arquivo);
                    free(binR);
                }
                break;
        }
        fprintf(arquivo, "\n");
    }
}

void binario_debug(FILE* arquivo) {
    if (arquivo == NULL) {
        printf("ERRO: Arquivo NULL fornecido para debug binário\n");
        return;
    }
    
    if (instrucoesAssembly == NULL) {
        printf("ERRO: Array de instruções assembly não inicializado\n");
        return;
    }
    
    BIN_I* binI;
    BIN_J* binJ;
    BIN_R* binR;
    
    for (int i = 0; i < indiceAssembly; i++) {
        fprintf(arquivo, "%d:\t", i);
        
        if (instrucoesAssembly[i] == NULL) {
            binR = binarioNop();
            if (binR != NULL) {
                printBits(sizeof(*binR), binR, arquivo);
                fprintf(arquivo, " : nop (NULL instruction)");
                free(binR);
            }
            fprintf(arquivo, "\n");
            continue;
        }
        
        switch (instrucoesAssembly[i]->tipo) {
            case typeR:
                binR = binarioR(instrucoesAssembly[i]);
                if (binR != NULL) {
                    printBits(sizeof(*binR), binR, arquivo);
                    fprintf(arquivo, " : %s", instrucoesAssembly[i]->tipoR->nome);
                    free(binR);
                }
                break;
                
            case typeI:
                binI = binarioI(instrucoesAssembly[i]);
                if (binI != NULL) {
                    printBits(sizeof(*binI), binI, arquivo);
                    fprintf(arquivo, " : %s", instrucoesAssembly[i]->tipoI->nome);
                    free(binI);
                }
                break;
                
            case typeJ:
                binJ = binarioJ(instrucoesAssembly[i]);
                if (binJ != NULL) {
                    printBits(sizeof(*binJ), binJ, arquivo);
                    fprintf(arquivo, " : %s", instrucoesAssembly[i]->tipoJ->nome);
                    free(binJ);
                }
                break;
                
            case typeLabel:
                binR = binarioNop();
                if (binR != NULL) {
                    printBits(sizeof(*binR), binR, arquivo);
                    fprintf(arquivo, " : nop (label: %s)", 
                           instrucoesAssembly[i]->tipoLabel->nome);
                    free(binR);
                }
                break;
                
            default:
                fprintf(arquivo, "UNKNOWN_TYPE : erro");
                break;
        }
        fprintf(arquivo, "\n");
    }
}

void salvarBinario(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("ERRO: Não foi possível criar o arquivo %s\n", nomeArquivo);
        return;
    }
    
    binario(arquivo);
    fclose(arquivo);
}

void salvarBinarioDebug(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("ERRO: Não foi possível criar o arquivo %s\n", nomeArquivo);
        return;
    }
    
    binario_debug(arquivo);
    fclose(arquivo);
}

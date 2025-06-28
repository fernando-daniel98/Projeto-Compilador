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
        return 0b000000; // Todas as instruções R-Type têm opcode 000000
    }
    
    // Instruções I-Type (conforme architeture.json)
    if (!strcmp(nome, "lw")) return 0b100011;        // 0x23
    if (!strcmp(nome, "sw")) return 0b101011;        // 0x2B
    if (!strcmp(nome, "addi")) return 0b001000;      // 0x08
    if (!strcmp(nome, "subi")) return 0b001001;      // 0x09
    if (!strcmp(nome, "andi")) return 0b001100;      // 0x0C
    if (!strcmp(nome, "ori")) return 0b001101;       // 0x0D
    if (!strcmp(nome, "xori")) return 0b001110;      // 0x0E
    if (!strcmp(nome, "beq")) return 0b000100;       // 0x04
    if (!strcmp(nome, "bneq")) return 0b000101;      // 0x05
    if (!strcmp(nome, "slti")) return 0b001010;      // 0x0A
    if (!strcmp(nome, "in")) return 0b011111;        // 0x1F
    if (!strcmp(nome, "out")) return 0b011110;       // 0x1E
    
    // Instruções J-Type (conforme architeture.json)
    if (!strcmp(nome, "j")) return 0b000010;         // 0x02
    if (!strcmp(nome, "jal")) return 0b000011;       // 0x03
    if (!strcmp(nome, "halt")) return 0b111111;      // 0x3F
    
    // Instrução não reconhecida
    printf("ERRO: Opcode não encontrado para instrução: %s\n", nome);
    return 0b111111; // Código de erro (halt)
}

// ===============================================
// MAPEAMENTO DE FUNCTIONS PARA INSTRUÇÕES R-TYPE
// ===============================================

unsigned int get_funct(char* nome) {
    // Instruções R-Type (conforme architeture.json)
    if (!strcmp(nome, "add")) return 0b100000;       // 0x20
    if (!strcmp(nome, "sub")) return 0b100010;       // 0x22
    if (!strcmp(nome, "and")) return 0b100100;       // 0x24
    if (!strcmp(nome, "or")) return 0b100101;        // 0x25
    if (!strcmp(nome, "jr")) return 0b001000;        // 0x08
    if (!strcmp(nome, "jlr")) return 0b001001;       // 0x09 (jump and link register)
    if (!strcmp(nome, "slt")) return 0b101010;       // 0x2A
    if (!strcmp(nome, "sll")) return 0b000000;       // 0x00
    if (!strcmp(nome, "srl")) return 0b000010;       // 0x02
    if (!strcmp(nome, "div")) return 0b011010;       // 0x1A
    if (!strcmp(nome, "mult")) return 0b011000;      // 0x18
    
    // Instrução não reconhecida
    printf("ERRO: Function code não encontrado para instrução R-Type: %s\n", nome);
    return 0b000000; // NOP como padrão
}

// ===============================================
// FUNÇÕES AUXILIARES
// ===============================================

unsigned int get_register(int reg) {
    // Verificar se o registrador está no range válido (0-63)
    if (reg >= 0 && reg <= 63) {
        return (unsigned int)reg;
    }
    
    printf("ERRO: Registrador inválido: %d (deve estar entre 0-63)\n", reg);
    return 63; // Retorna $zero como padrão seguro
}

unsigned int get_shamt(int shamt) {
    // Shift amount limitado a 2 bits (0-3) conforme especificação
    if (shamt >= 0 && shamt <= 3) {
        return (unsigned int)shamt;
    }
    
    printf("AVISO: Shift amount %d limitado a 2 bits (max 3)\n", shamt);
    return (unsigned int)(shamt & 0x03); // Mascara para 2 bits
}

unsigned int get_immediate(int imediato) {
    // Valor imediato limitado a 14 bits (0-16383) conforme especificação
    if (imediato >= 0 && imediato <= 16383) {
        return (unsigned int)imediato;
    }
    
    // Para valores negativos, usar complemento de 2
    if (imediato >= -8192 && imediato < 0) {
        return (unsigned int)(imediato & 0x3FFF); // Mascara para 14 bits
    }
    
    printf("AVISO: Valor imediato %d limitado a 14 bits (±8192)\n", imediato);
    return (unsigned int)(imediato & 0x3FFF); // Mascara para 14 bits
}

unsigned int get_address(char* label) {
    if (label == NULL) {
        printf("ERRO: Label NULL fornecida\n");
        return 0;
    }
    
    int endereco = getEnderecoLabel(label);
    if (endereco == -1) {
        printf("AVISO: Label '%s' não encontrada, usando endereço 0\n", label);
        return 0;
    }
    
    // Endereço limitado a 26 bits (0-67108863) conforme especificação
    if (endereco >= 0 && endereco <= 67108863) {
        return (unsigned int)endereco;
    }
    
    printf("AVISO: Endereço %d limitado a 26 bits\n", endereco);
    return (unsigned int)(endereco & 0x3FFFFFF); // Mascara para 26 bits
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
    
    // NOP: add $zero, $zero, $zero
    bin->opcode = 0b000000;  // R-Type
    bin->rs = 63;            // $zero
    bin->rt = 63;            // $zero
    bin->rd = 63;            // $zero
    bin->shamt = 0;          // Sem shift
    bin->funct = 0b100000;   // ADD
    
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
    bin->shamt = get_shamt(0); // Shamt sempre 0 para instruções básicas
    bin->funct = get_funct(instrucao->tipoR->nome);
    
    return bin;
}

BIN_I* binarioI(ASSEMBLY* instrucao) {
    if (instrucao == NULL || instrucao->tipoI == NULL) {
        printf("ERRO: Instrução I-Type inválida\n");
        return NULL;
    }
    
    BIN_I* bin = (BIN_I*)malloc(sizeof(BIN_I));
    if (bin == NULL) {
        printf("ERRO: Falha na alocação de memória para instrução I-Type\n");
        return NULL;
    }
    
    bin->opcode = get_opcode(instrucao->tipoI->nome, instrucao->tipo);
    bin->rs = get_register(instrucao->tipoI->rs);
    bin->rt = get_register(instrucao->tipoI->rt);
    
    // Tratamento especial para branches com labels
    if ((!strcmp(instrucao->tipoI->nome, "beq") || !strcmp(instrucao->tipoI->nome, "bneq")) 
        && instrucao->tipoI->label != 0) {
        // Converter número da label para string e buscar endereço
        char labelName[32];
        sprintf(labelName, "Label %d", instrucao->tipoI->label);
        
        int enderecoLabel = getEnderecoLabel(labelName);
        if (enderecoLabel != -1) {
            // Calcular offset relativo (endereço destino - PC atual - 1)
            // Nota: o PC atual seria a posição desta instrução no assembly
            bin->immediate = get_immediate(enderecoLabel);
        } else {
            printf("AVISO: Label %s não encontrada para branch\n", labelName);
            bin->immediate = 0;
        }
    } else {
        // Usar valor imediato direto
        bin->immediate = get_immediate(instrucao->tipoI->imediato);
    }
    
    return bin;
}

BIN_J* binarioJ(ASSEMBLY* instrucao) {
    if (instrucao == NULL || instrucao->tipoJ == NULL) {
        printf("ERRO: Instrução J-Type inválida\n");
        return NULL;
    }
    
    BIN_J* bin = (BIN_J*)malloc(sizeof(BIN_J));
    if (bin == NULL) {
        printf("ERRO: Falha na alocação de memória para instrução J-Type\n");
        return NULL;
    }
    
    bin->opcode = get_opcode(instrucao->tipoJ->nome, instrucao->tipo);
    
    // Tratamento especial para halt (sem endereço)
    if (!strcmp(instrucao->tipoJ->nome, "halt")) {
        bin->address = 0;
    } else if (instrucao->tipoJ->labelImediato != NULL) {
        bin->address = get_address(instrucao->tipoJ->labelImediato);
    } else {
        printf("AVISO: Instrução J-Type sem endereço: %s\n", instrucao->tipoJ->nome);
        bin->address = 0;
    }
    
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

#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include "codeGen.h" // Para quadruple e outras definições
#include <stdio.h>

// Função principal para gerar o código assembly
// intermediateCode: array de quádruplas
// symbolTable: tabela de símbolos (pode ser útil para informações globais)
// outStream: stream de saída para o código assembly (ex: arquivo .s)
void generateAssembly(quadruple **intermediateCode, PnoIdentificador *symbolTable, FILE *outStream);

#endif

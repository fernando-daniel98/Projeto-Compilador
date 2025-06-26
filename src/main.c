#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>
#include "../include/globals.h"
#include "../include/syntax_tree.h"
#include "../include/parser.h"
#include "../include/tab.h"
#include "../include/semantic.h"
#include "../include/codeGen.h"
#include "../include/assembler.h"     // Descomentado para testar assembler simples

// Variáveis externas necessárias
extern FILE *yyin;
extern FILE *yyout;
extern int lineNum;
extern int lexical_errors;
extern int syntax_errors;
extern int semantic_errors;

int formaEntrada(int argc, char **argv) {
    if (argc == 1) {
        yyin = stdin;
        yyout = stdout;
    } else if (argc == 2) {
        yyin = fopen(argv[1], "r");
        
        if (yyin == NULL) {
            fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
            return 1;
        }
        
        // Saída principal vai para stdout, para ser redirecionada pelo Makefile
        yyout = stdout;

        // Mensagem informativa para o log (via stdout redirecionado)
        fprintf(yyout, "Processing input file: %s\n", argv[1]);
        // A mensagem sobre onde o log será salvo é agora tratada pelo Makefile.

    } else if (argc == 3) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
            return 1;
        }
        yyout = fopen(argv[2], "w");
        if (yyout == NULL) {
            fprintf(stderr, "Error: Cannot open output file %s\n", argv[2]);
            fclose(yyin);
            return 1;
        }
        fprintf(yyout, "Processing input file: %s\n", argv[1]);
        fprintf(yyout, "Output directed to: %s\n", argv[2]);
    } else {
        fprintf(stderr, "Usage: %s [input_file] [output_file]\n", argv[0]);
        fprintf(stderr, "If [output_file] is omitted, output goes to stdout.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {

    // Verificar número de argumentos
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file> [output_file]\n", argv[0]);
        fprintf(stderr, "If [output_file] is omitted, output goes to stdout.\n");
        return 1;
    }

    if (formaEntrada(argc, argv) != 0) {
        fprintf(stderr, "Error: Could not open or configure files.\n");
        return 1;
    }

    // Verificar arquivo de entrada
    if (yyin == NULL) {
        // A mensagem de erro já é impressa por formaEntrada ou fopen
        return 1;
    }
    
    fprintf(stderr, "Starting syntax analysis...\n");

    TreeNode *syntaxTree = NULL;

    syntaxTree = parse();

    if (syntaxTree != NULL) {
        fprintf(stderr, "Syntax analysis completed.\n\n");
        
        fprintf(stderr, "Starting semantic analysis...\n");

        initSymbolTable();
        buildSymTabFromTree(syntaxTree);
        
        checkMainFunction();

        fprintf(stderr, "Semantic analysis completed.\n\n");

        mostraTabelaSimbolos(symbolTable);

        fprintf(yyout, "SYNTAX TREE\n");
        fprintf(yyout, "-------------\n");
        printTree(syntaxTree);
        fprintf(yyout, "-------------\n\n");
        
    } else {
        fprintf(stderr, "Syntax analysis failed. See previous errors.\n\n");
    }

    fprintf(stderr, "\n");

    // Verificar se houve erros antes de prosseguir para a geração de código.
    // Se houver erros, não faz sentido gerar o código intermediário.
    // Portanto, a fase de síntese não será iniciada.
    int icStatus;

    if (lexical_errors > 0 || syntax_errors > 0 || semantic_errors > 0 || syntaxTree == NULL) {
        fprintf(stderr, "Skipping intermediate code generation due to errors or missing syntax tree.\n\n");
    } else {
        fprintf(stderr, "Starting intermediate code generation...\n");

        fprintf(yyout, "INTERMEDIATE CODE\n");
        fprintf(yyout, "-------------\n");
        
        icStatus = createIntermediateCode(syntaxTree, symbolTable);

        // Inserindo a quadrupla do HALT no final do código intermediário
        // Somente se não houver erros da geração do código intermediário
        if(icStatus == 0){
            quadruple *haltQuad = criaInstrucao(HALT);
            if (haltQuad == NULL) {
                fprintf(stderr, "Failed to create HALT instruction.\n");

                icStatus = -1;
            } else {
                haltQuad->oper1 = criaEndereco(Vazio, 0, NULL, 0);
                haltQuad->oper2 = criaEndereco(Vazio, 0, NULL, 0);
                haltQuad->oper3 = criaEndereco(Vazio, 0, NULL, 0);
                
                if (adressCounter >= MAX_LEN_CODE_INTERMEDIATE) {
                    fprintf(stderr, "Intermediate code limit reached while adding HALT instruction.\n");
                    free(haltQuad);
                } else {
                    intermediateCode[adressCounter++] = haltQuad;
                }
            }
        }
        
        // Verificar o resultado da geração de código
        if (icStatus == 0) {
            fprintf(stderr, "Intermediate code generation completed.\n\n");
            
            imprimeCodigoIntermediario();
            
        } else if (icStatus == -1) {
            fprintf(stderr, "Intermediate code generation failed: Memory allocation error.\n");
        } else if (icStatus == -2) {
            fprintf(stderr, "Intermediate code generation failed: Invalid syntax tree or symbol table.\n");
        } else {
            fprintf(stderr, "Intermediate code generation failed with unknown error code: %d.\n", icStatus);
        }
        
        fprintf(yyout, "-------------\n\n");
    }

    // Liberação das estruturas que não serão mais utilizadas.
    if (syntaxTree != NULL){
        freeTree(syntaxTree);
    }

    if (symbolTable != NULL){
        deleteSymTab();
    }

    fprintf(yyout, "\n\n");
    mostrarReg();
    fprintf(yyout, "\n\n");

    // Testando assembler simples
    assembly();
    imprimirAssembly();

    if (intermediateCode != NULL){
        freeIntermediateCode();
    }

    // Fechando os arquivos
    if (yyin != stdin) fclose(yyin);
    if (yyout != stdout && yyout != NULL) fclose(yyout);

    fprintf(stderr, "Files closed.\n");

    if (lexical_errors > 0 || syntax_errors > 0 || semantic_errors > 0) {
        fprintf(stderr, "Compilation finished with errors:\n");
        if (lexical_errors > 0) fprintf(stderr, "- Lexical errors: %d\n", lexical_errors);
        if (syntax_errors > 0) fprintf(stderr, "- Syntax errors: %d\n", syntax_errors);
        if (semantic_errors > 0) fprintf(stderr, "- Semantic errors: %d\n\n", semantic_errors);
        return 1;
    }
    else {
        fprintf(stderr, "Compilation finished successfully.\n");
    }

    return 0;
}
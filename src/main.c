#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>
#include "../include/globals.h"
#include "../include/syntax_tree.h"
#include "../include/parser.h"
#include "../include/symbol_table.h"
#include "../include/semantic.h"
#include "../include/codeGen.h"

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

    // Verificar arquivo de entrada (yyin é configurado em formaEntrada)
    if (yyin == NULL) {
        // A mensagem de erro já é impressa por formaEntrada ou fopen
        return 1;
    }
    
    fprintf(stderr, "Starting syntax analysis...\n");

    TreeNode *syntaxTree = NULL;

    // Análises léxica e sintática
    syntaxTree = parse();

    if (syntaxTree != NULL) {
        fprintf(stderr, "Syntax analysis completed.\n\n");
        
        // Análise semântica
        fprintf(stderr, "Starting semantic analysis...\n");

        initSymbolTable();
        buildSymTabFromTree(syntaxTree);
        
        // Verificar se a função main está presente no arquivo de entrada
        checkMainFunction();

        // Fim da análise semântica
        fprintf(stderr, "Semantic analysis completed.\n\n");

        // Mostrando tabela de símbolos
        mostraTabelaSimbolos(symbolTable);

        // Mostrando a árvore sintática
        fprintf(yyout, "SYNTAX TREE\n");
        fprintf(yyout, "-------------\n");
        printTree(syntaxTree);
        // printNodeInfo(yyout, syntaxTree); // Chamada anterior
        // displayTreeHierarchy(syntaxTree, yyout); // Nova chamada para a árvore hierárquica
        fprintf(yyout, "-------------\n\n");
        
    } else {
        fprintf(stderr, "Syntax analysis failed. See previous errors.\n\n");
        // Se não há árvore sintática, não há como prosseguir para semântica ou geração de código.
        // Os erros léxicos e sintáticos já devem ter sido reportados.
        // A limpeza e fechamento de arquivos ocorrerão no final.
    }

    fprintf(stderr, "\n");

    // Verificar se houve erros antes de prosseguir para a geração de código.
    // Se houver erros, não faz sentido gerar o código intermediário.
    // Portanto, a fase de síntese não será iniciada.
    if (lexical_errors > 0 || syntax_errors > 0 || semantic_errors > 0 || syntaxTree == NULL) {
        fprintf(stderr, "Skipping intermediate code generation due to errors or missing syntax tree.\n\n");
    } else {
        // Geração de código intermediário
        fprintf(stderr, "Starting intermediate code generation...\n");

        fprintf(yyout, "INTERMEDIATE CODE\n");
        fprintf(yyout, "-------------\n");
        
        // Chamar o gerador de código intermediário
        int icStatus = createIntermediateCode(syntaxTree, symbolTable);

        // Inserindo a quadrupla do HALT no final do código intermediário
        // Somente se não houver erros da geração do código intermediário
        if(icStatus == 0){
            quadruple *haltQuad = criaInstrucao(HALT);
            if (haltQuad == NULL) {
                fprintf(stderr, "Failed to create HALT instruction.\n");

                // Erro de alocação de memória
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
            
            // Imprimir as quadruplas geradas no arquivo de saída
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

    // Liberação das estruturas utilizadas.
    if (syntaxTree != NULL){
        freeTree(syntaxTree);
    }

    if (symbolTable != NULL){
        deleteSymTab();
    }

    if (intermediateCode != NULL){
        freeIntermediateCode();
    }

    // Fechando os arquivos
    if (yyin != stdin) fclose(yyin);
    if (yyout != stdout && yyout != NULL) fclose(yyout);

    fprintf(yyout, "Compilation process completed and files closed.\n\n");

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
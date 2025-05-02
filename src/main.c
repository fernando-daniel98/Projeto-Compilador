#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>
#include "../include/globals.h"
#include "../include/parser.h"
#include "../include/semantic.h"
#include "../include/codeGen.h"

// Variáveis externas necessárias
extern FILE *yyin;
extern FILE *yyout;
extern int lineNum;
extern int lexical_errors;
extern int syntax_errors;
extern int semantic_errors;

int dotFileFlag = 0;

void generateFileName(char *buffer, size_t bufferSize, const char *directory, 
                        const char *baseName, const char *extension) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestr[20];

    // Formato do timestamp: dd-mm-HHhMMmin
    strftime(timestr, sizeof(timestr), "%d-%m-%Hh%Mmin", t);

    // Criar nome do arquivo: diretório/timestamp_nomearquivo.extensão
    snprintf(buffer, bufferSize, "%s/%s_%s%s", 
                directory, timestr, basename((char*)baseName), extension);
}

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
        
        // Configura saída para o log
        char logPath[256];
        generateFileName(logPath, sizeof(logPath), "output/logs", argv[1], ".log");
        yyout = fopen(logPath, "w");

        if (yyout == NULL) {
            fprintf(stderr, "Alert: Could not create log file. Using stdout.\n");
            yyout = stdout;
        } else {
            fprintf(yyout, "File found: %s\n", argv[1]);
            fprintf(yyout, "Log will be saved to: %s\n", logPath);
        }

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
    } else {
        fprintf(stderr, "Usage: %s [input_file] [output_file]\n", argv[0]);
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {

    fprintf(stderr, "Starting program...\n");
    fflush(stderr);

    // Verificar número de argumentos
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s [dot_flag] [tac_flag] <input_file>\n", argv[0]);
        fprintf(stderr, "dot_flag: 1 to generate DOT file, 0 to skip (optional)\n");
        return 1;
    }

    char *inputFile;
    
    // Processar os argumentos
    if (argc == 3) {
        // Se temos 3 argumentos, o segundo é a flag DOT e o terceiro é o arquivo
        if (strcmp(argv[1], "0") == 0) {
            dotFileFlag = 0;
        } else if (strcmp(argv[1], "1") == 0) {
            dotFileFlag = 1;
        } else {
            fprintf(stderr, "Error: Invalid DOT flag. Use 0 or 1.\n");
            return 1;
        }
        inputFile = argv[2];
    } else {
        // Se temos 2 argumentos, o segundo é o arquivo (sem flags especificadas)
        dotFileFlag = 0; // Valor padrão
        inputFile = argv[1];
    }

    fprintf(stderr, "Opening file: %s in tests path\n\n", inputFile);
    if (dotFileFlag) {
        fprintf(stderr, "DOT file generation: enabled\n");
    } else {
        fprintf(stderr, "DOT file generation: disabled\n");
    }
    
    fflush(stderr);

    // Modificar a chamada para formaEntrada para usar inputFile
    char *args[2] = {argv[0], inputFile};
    if (formaEntrada(2, args) != 0) {
        fprintf(stderr, "Error opening files\n");
        return 1;
    }

    // Verificar arquivo
    if (yyin == NULL) {
        fprintf(stderr, "Failed to open input file\n");
        return 1;
    }
    else {
        fprintf(stderr, "Input file opened successfully\n");
    }
    
    fprintf(stderr, "Starting syntax analysis...\n");
    fflush(stderr);

    TreeNode *syntaxTree = NULL;

    // Fase 1: Análise sintática
    syntaxTree = parse();

    if (syntaxTree != NULL) {
        fprintf(stderr, "\nSyntax analysis completed successfully\n");
        fflush(stderr);
        
        // Fase 2: Análise semântica
        fprintf(stderr, "\nStarting semantic analysis...\n");
        fflush(stderr);

        initSymbolTable();
        buildSymTabFromTree(syntaxTree);
        
        // Verificar se houve erros semânticos
        if (semantic_errors == 0) {
            fprintf(stderr, "\nSemantic analysis completed successfully\n");
            
        } else {
            fprintf(stderr, "\nSemantic analysis completed with %d errors\n", semantic_errors);
        }
        
        // Verificar função main
        checkMainFunction();
        
        // Mostrar tabela de símbolos
        fprintf(stderr, "\nGenerating symbol table...\n");
        mostraTabelaSimbolos(symbolTable);
        fprintf(stderr, "Symbol table generation complete\n");

        // Mostrando a árvore sintática
        fprintf(stderr, "\nGenerating syntax tree output...\n");

        fprintf(yyout, "\nSYNTAX TREE 1\n");
        fprintf(yyout, "-------------\n");
        printTree(syntaxTree);
        fprintf(yyout, "-------------\n\n");

        // fprintf(yyout, "\nSYNTAX TREE 2\n");
        // fprintf(yyout, "-------------\n");
        // displayTreeHierarchy(syntaxTree, yyout);
        // fprintf(yyout, "-------------\n\n");
        fprintf(stderr, "Syntax tree output complete\n");

        
        if (dotFileFlag) {
            fprintf(stderr, "\nGenerating DOT file for AST visualization...\n");
            char dotFilePath[256];
            generateFileName(dotFilePath, sizeof(dotFilePath), "output/ast", inputFile, ".dot");
            generateDotFile(syntaxTree, dotFilePath);
            fprintf(stderr, "AST in DOT format saved to %s\n", dotFilePath);
        } else {
            fprintf(stderr, "\nDOT file generation skipped as requested\n");
        }
        
    } else {
        fprintf(stderr, "\nSyntax analysis failed to produce valid syntax tree\n");
        fprintf(stderr, "This may be due to syntax errors in the input file\n");
        if (yyin != stdin) fclose(yyin);
        if (yyout != stdout) fclose(yyout);
        return 1;
    }

    // Fase 3: Geração de código intermediário
    fprintf(stderr, "\nStarting intermediate code generation...\n");
    fflush(stderr);

    percorreAAS(syntaxTree);

    fprintf(stderr, "\nCleaning up resources...\n");

    if (syntaxTree != NULL)
        freeTree(syntaxTree);

    if (symbolTable != NULL)
        deleteSymTab();

    // Fechar arquivos
    if (yyin != stdin) fclose(yyin);
    if (yyout != stdout) fclose(yyout);

    fprintf(yyout, "Compilation process completed\n");
    fflush(yyout);

    if (lexical_errors > 0 || syntax_errors > 0 || semantic_errors > 0) {
        fprintf(stderr, "\nCompilation completed with errors:\n");
        fprintf(stderr, "- Lexical errors: %d\n", lexical_errors);
        fprintf(stderr, "- Syntax errors: %d\n", syntax_errors);
        fprintf(stderr, "- Semantic errors: %d\n", semantic_errors);
        return 1;
    }
    else {
        fprintf(stderr, "\nCompilation completed successfully with no errors\n");
    }

    return 0;
}
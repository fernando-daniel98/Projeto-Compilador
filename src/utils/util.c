#include <stdio.h>
#include "../include/globals.h"
#include <stdlib.h>
#include <string.h>

static int indentno = 0; /* number of spaces to indent */

/* macros to increase/decrease indentation */
#define INDENT indentno+=1
#define UNINDENT indentno-=1

/* printSpaces indents by printing spaces */
void printSpaces(void) {
    for (int i = 0; i < indentno; i++)
        fprintf(yyout, "\t");
}

/* Function to convert ExpType to string */
const char* expTypeToString(ExpType type) {
    switch(type) {
        case Void: return "void";
        case Integer: return "int";
        default: return "unknown";
    }
}

/* Function to convert operator to string */
const char* operatorToString(int op) {
    switch(op) {
        case PLUS_OP: return "+";
        case MINUS_OP: return "-";
        case MULT_OP: return "*";
        case DIV_OP: return "/";
        case LT_OP: return "<";
        case LTE_OP: return "<=";
        case GT_OP: return ">";
        case GTE_OP: return ">=";
        case EQ_OP: return "==";
        case NEQ_OP: return "!=";
        case ASSIGN_OP: return "=";
        default: return "?";
    }
}

/* Função para verificar se um nó é um operador unário negativo */
int isUnaryNegative(TreeNode* tree) {
    return (tree != NULL && 
            tree->nodekind == ExpressionK && 
            tree->kind.exp == OpK && 
            tree->attr.op == MINUS_OP &&
            tree->child[1] == NULL);
}
/* printTree recursively prints the syntax tree */
void printTree(TreeNode* tree) {
    INDENT;
    while (tree != NULL) {
        printSpaces();
        if (tree->nodekind == StatementK) {
            switch (tree->kind.stmt) {
                case IfK:
                    fprintf(yyout, "If\n");
                    break;
                case WhileK:
                    fprintf(yyout, "While\n");
                    break;
                case ReturnINT:
                    fprintf(yyout, "Return (int)\n");
                    break;
                case ReturnVOID:
                    fprintf(yyout, "Return (void)\n");
                    break;
                case NuloDecl:
                    fprintf(yyout, "Compound Statement\n");
                    break;
                case VarDeclK:
                    fprintf(yyout, "Var Declaration: %s (type: %s)\n", 
                        tree->attr.name, 
                        expTypeToString(tree->type));
                    break;
                case VetDeclK:
                    fprintf(yyout, "Vector Declaration: %s (type: %s)\n", 
                        tree->attr.name, 
                        expTypeToString(tree->type));
                    break;
                case FunDeclK:
                    fprintf(yyout, "Function Declaration: %s (return type: %s)\n", 
                        tree->attr.name, 
                        expTypeToString(tree->type));
                    break;
                case VarParamK:
                    fprintf(yyout, "Parameter: %s (type: %s)\n", 
                        tree->attr.name, 
                        expTypeToString(tree->type));
                    break;
                case VetParamK:
                    fprintf(yyout, "Vector Parameter: %s (type: %s)\n", 
                        tree->attr.name, 
                        expTypeToString(tree->type));
                    break;
                case ParamVoid:
                    fprintf(yyout, "Void Parameter\n");
                    break;
                default:
                    fprintf(yyout, "Unknown StatementK node kind\n");
                    break;
            }
        }
        else if (tree->nodekind == ExpressionK) {
            switch (tree->kind.exp) {
                case OpK:
                    if (isUnaryNegative(tree)) {
                        fprintf(yyout, "Operator: unary %s\n", operatorToString(tree->attr.op));
                    } else {
                        fprintf(yyout, "Operator: %s\n", operatorToString(tree->attr.op));
                    }
                    break;
                case OpRel:
                    fprintf(yyout, "Relational: %s\n", operatorToString(tree->attr.op));
                    break;
                case ConstK:
                    fprintf(yyout, "Const: %d\n", tree->attr.val);
                    break;
                case IdK:
                    fprintf(yyout, "Id: %s\n", tree->attr.name);
                    break;
                case AtivK:
                    fprintf(yyout, "Function Call: %s\n", tree->attr.name);
                    break;
                case VetorK:
                    fprintf(yyout, "Vector: %s\n", tree->attr.name);
                    break;
                case AssignK:
                    fprintf(yyout, "Assign\n");
                    break;
                case NuloExp:
                    fprintf(yyout, "Null Expression\n");
                    break;
                default:
                    fprintf(yyout, "Unknown ExpressionK node kind\n");
                    break;
            }
        }
        else {
            fprintf(yyout, "Unknown node kind\n");
        }

        /* Print children */
        for (int i = 0; i < MAXCHILDREN; i++) {
            if (tree->child[i] != NULL) {
                printTree(tree->child[i]);
            }
        }
        
        tree = tree->sibling;
    }
    UNINDENT;
}

void freeTree(TreeNode *tree) {
    if (tree == NULL) return;
    
    // Primeiro liberar os filhos (profundidade)
    for (int i = 0; i < MAXCHILDREN; i++) {
        if (tree->child[i] != NULL) {
            freeTree(tree->child[i]);
            tree->child[i] = NULL; // Evitar uso após liberação
        }
    }
    
    // Em seguida, liberar os irmãos (largura)
    if (tree->sibling != NULL) {
        freeTree(tree->sibling);
        tree->sibling = NULL; // Evitar uso após liberação
    }
    
    // Por fim, liberar o nó atual
    free(tree);
}

// Variável para rastrear os IDs dos nós na representação DOT
static int nodeCount = 0;

/* Gera o código DOT para um nó da árvore */
void generateDotNode(FILE* dotFile, TreeNode* tree, int nodeId) {
    if (tree == NULL) return;
    
    // Determinar a cor e forma do nó com base no tipo
    const char* shape = "ellipse";
    const char* color = "black";
    const char* style = "filled";
    const char* fillcolor = "white";
    
    // Definir cores diferentes para tipos diferentes de nós
    if (tree->nodekind == StatementK) {
        switch (tree->kind.stmt) {
            case IfK:
                fillcolor = "lightblue";
                break;
            case WhileK:
                fillcolor = "lightgreen";
                break;
            case ReturnINT:
                fillcolor = "pink";
                break;
            case ReturnVOID:
                fillcolor = "lightsalmon";
                break;
            case VarDeclK:
                fillcolor = "lightgray";
                break;
            case FunDeclK:
                fillcolor = "gold";
                shape = "box";
                break;
            default:
                fillcolor = "white";
                break;
        }
    } else if (tree->nodekind == ExpressionK) {
        switch (tree->kind.exp) {
            case OpK:
                if (isUnaryNegative(tree)) {
                    fillcolor = "salmon";
                } else {
                    fillcolor = "lightcoral";
                }
                break;
            case ConstK:
                fillcolor = "lightsalmon";
                break;
            case IdK:
                fillcolor = "lightcyan";
                break;
            case AtivK:
                fillcolor = "plum";
                break;
            case AssignK:
                fillcolor = "lightyellow";
                break;
            default:
                fillcolor = "white";
                break;
        }
    }
    
    // Gerar o rótulo do nó
    fprintf(dotFile, "  node%d [shape=%s, style=\"%s\", color=%s, fillcolor=\"%s\", label=\"", nodeId, shape, style, color, fillcolor);
    
    // Adicionar informações sobre o nó com base em seu tipo
    if (tree->nodekind == StatementK) {
        switch (tree->kind.stmt) {
            case IfK:
                fprintf(dotFile, "If");
                break;
            case WhileK:
                fprintf(dotFile, "While");
                break;
            case ReturnINT:
                fprintf(dotFile, "Return (int)");
                break;
            case ReturnVOID:
                fprintf(dotFile, "Return (void)");
                break;
            case NuloDecl:
                fprintf(dotFile, "Compound Statement");
                break;
            case VarDeclK:
                fprintf(dotFile, "VarDecl: %s\\nType: %s", tree->attr.name, expTypeToString(tree->type));
                break;
            case VetDeclK:
                // Obter o tamanho do vetor a partir do nó filho
                int arraySize = 0;
                if (tree->child[0] && tree->child[0]->nodekind == ExpressionK && 
                    tree->child[0]->kind.exp == ConstK) {
                    arraySize = tree->child[0]->attr.val;
                }
                fprintf(dotFile, "ArrayDecl: %s[%d]\\nType: %s", tree->attr.name, arraySize, expTypeToString(tree->type));
                break;
            case FunDeclK:
                fprintf(dotFile, "FunDecl: %s\\nType: %s", tree->attr.name, expTypeToString(tree->type));
                break;
            case VarParamK:
                fprintf(dotFile, "VarParam: %s\\nType: %s", tree->attr.name, expTypeToString(tree->type));
                break;
            case VetParamK:
                fprintf(dotFile, "ArrayParam: %s[]\\nType: %s", tree->attr.name, expTypeToString(tree->type));
                break;
            case ParamVoid:
                fprintf(dotFile, "Void Parameter");
                break;
            default:
                fprintf(dotFile, "Unknown Statement");
                break;
        }
    } else if (tree->nodekind == ExpressionK) {
        switch (tree->kind.exp) {
            case OpK:
                if (isUnaryNegative(tree)) {
                    fprintf(dotFile, "Op: unary %s", operatorToString(tree->attr.op));
                } else {
                    fprintf(dotFile, "Op: %s", operatorToString(tree->attr.op));
                }
                break;
            case OpRel:
                fprintf(dotFile, "Rel: %s", operatorToString(tree->attr.op));
                break;
            case ConstK:
                fprintf(dotFile, "Const: %d", tree->attr.val);
                break;
            case IdK:
                fprintf(dotFile, "Id: %s", tree->attr.name);
                break;
            case VetorK:
                fprintf(dotFile, "Array: %s", tree->attr.name);
                break;
            case AtivK:
                fprintf(dotFile, "Call: %s", tree->attr.name);
                break;
            case AssignK:
                fprintf(dotFile, "Assign");
                break;
            default:
                fprintf(dotFile, "Unknown Expression");
                break;
        }
    }
    
    // Adicionar o número da linha
    fprintf(dotFile, "\\nLine: %d", tree->lineno);
    
    fprintf(dotFile, "\"];\n");
}

/* Função recursiva para gerar o código DOT para toda a árvore */
int generateDotTree(FILE* dotFile, TreeNode* tree, int parentId) {
    if (tree == NULL) return parentId;
    
    int currentId = nodeCount++;
    generateDotNode(dotFile, tree, currentId);
    
    // Conectar ao nó pai, se existir
    if (parentId >= 0) {
        fprintf(dotFile, "  node%d -> node%d;\n", parentId, currentId);
    }
    
    // Processar filhos
    for (int i = 0; i < MAXCHILDREN; i++) {
        if (tree->child[i] != NULL) {
            generateDotTree(dotFile, tree->child[i], currentId);
        }
    }
    
    // Processar irmãos
    if (tree->sibling != NULL) {
        return generateDotTree(dotFile, tree->sibling, parentId);
    }
    
    return currentId;
}

/* Função principal para gerar o arquivo DOT */
void generateDotFile(TreeNode* tree, const char* filename) {
    FILE* dotFile = fopen(filename, "w");
    if (dotFile == NULL) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filename);
    }
    
    // Resetar contador de nós
    nodeCount = 0;
    
    // Escrever o cabeçalho do arquivo DOT
    fprintf(dotFile, "digraph AST {\n");
    fprintf(dotFile, "  node [fontname=\"Arial\"];\n");
    fprintf(dotFile, "  edge [fontname=\"Arial\"];\n");
    fprintf(dotFile, "  rankdir=\"TB\";\n");
    fprintf(dotFile, "  splines=true;\n\n");
    
    // Gerar a árvore
    generateDotTree(dotFile, tree, -1);
    
    // Finalizar o arquivo DOT
    fprintf(dotFile, "}\n");
    fclose(dotFile);
}
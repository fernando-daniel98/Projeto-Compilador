#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/globals.h"
#include "../include/syntax_tree.h"

static int indentno = -1; // começa com -1 para que a primeira chamada de printSpaces imprima 0 espaços

#define INDENT indentno+=1
#define UNINDENT indentno-=1

void printSpaces(void) {
    for (int i = 0; i < indentno; i++)
        fprintf(yyout, "\t");
}

const char* expTypeToString(ExpType type) {
    switch(type) {
        case Void: return "void";
        case Integer: return "int";
        default: return "unknown";
    }
}

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
                    break;
                default:
                    fprintf(yyout, "Unknown ExpressionK node kind\n");
                    break;
            }
        }
        else {
            fprintf(yyout, "Unknown node kind\n");
        }

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
    
    for(int i = 0; i < MAXCHILDREN; i++) {
        freeTree(tree->child[i]); // Liberar os filhos
    }
    freeTree(tree->sibling); // Liberar o irmão
    
    // Por fim, liberar o nó atual
    free(tree);
}

void printNodeInfo(FILE* output, TreeNode* node) {
    if (node == NULL) {
        fprintf(output, "NULL");
        return;
    }
    
    if (node->nodekind == StatementK) {
        switch (node->kind.stmt) {
            case IfK: fprintf(output, "If"); break;
            case WhileK: fprintf(output, "While"); break;
            case ReturnINT: fprintf(output, "Return(int)"); break;
            case ReturnVOID: fprintf(output, "Return(void)"); break;
            case NuloDecl: fprintf(output, "CompoundStmt"); break;
            case VarDeclK: fprintf(output, "VarDecl: %s", node->attr.name); break;
            case VetDeclK: fprintf(output, "ArrayDecl: %s", node->attr.name); break;
            case FunDeclK: fprintf(output, "FunDecl: %s", node->attr.name); break;
            case VarParamK: fprintf(output, "Param: %s", node->attr.name); break;
            case VetParamK: fprintf(output, "ArrayParam: %s", node->attr.name); break;
            case ParamVoid: fprintf(output, "VoidParam"); break;
            default: fprintf(output, "UnknownStmt"); break;
        }
    } else if (node->nodekind == ExpressionK) {
        switch (node->kind.exp) {
            case OpK:
                if (isUnaryNegative(node))
                    fprintf(output, "UnaryOp: %s", operatorToString(node->attr.op));
                else
                    fprintf(output, "Op: %s", operatorToString(node->attr.op));
                break;
            case OpRel: fprintf(output, "RelOp: %s", operatorToString(node->attr.op)); break;
            case ConstK: fprintf(output, "Const: %d", node->attr.val); break;
            case IdK: fprintf(output, "Id: %s", node->attr.name); break;
            case AtivK: fprintf(output, "Call: %s", node->attr.name); break;
            case VetorK: fprintf(output, "Array: %s", node->attr.name); break;
            case AssignK: fprintf(output, "Assign"); break;
            default: fprintf(output, "UnknownExp"); break;
        }
    } else {
        fprintf(output, "Unknown");
    }
    
    fprintf(output, " (linha %d)", node->lineno);
}

void printTreeHierarchy(FILE* output, TreeNode* tree, int level, const char* prefix) {
    static int nodeCounter = 0;
    
    if (tree == NULL) return;
    
    // Atribuir ID ao nó atual
    int currentNodeId = ++nodeCounter;
    
    // Imprimir informações do nó atual com indentação
    fprintf(output, "%s[Nó %d] ", prefix, currentNodeId);
    printNodeInfo(output, tree);
    fprintf(output, "\n");
    
    // Preparar prefixo para filhos
    char childPrefix[256];
    snprintf(childPrefix, sizeof(childPrefix), "%s│   ", prefix);
    
    // Listar todos os filhos
    for (int i = 0; i < MAXCHILDREN; i++) {
        if (tree->child[i] != NULL) {
            fprintf(output, "%s├── Filho %d: ", prefix, i);
            printNodeInfo(output, tree->child[i]);
            fprintf(output, "\n");
            
            // Recursivamente processar cada filho
            printTreeHierarchy(output, tree->child[i], level + 1, childPrefix);
        }
    }
    
    // Listar irmão, se houver
    if (tree->sibling != NULL) {
        fprintf(output, "%s└── Irmão: ", prefix);
        printNodeInfo(output, tree->sibling);
        fprintf(output, "\n");
        
        // Preparar prefixo para irmãos
        char siblingPrefix[256];
        snprintf(siblingPrefix, sizeof(siblingPrefix), "%s    ", prefix);
        
        // Recursivamente processar o irmão
        printTreeHierarchy(output, tree->sibling, level, siblingPrefix);
    }
}

void displayTreeHierarchy(TreeNode* tree, FILE* output) {
    if (output == NULL) {
        output = stdout;
    }
    printTreeHierarchy(output, tree, 0, "");
}
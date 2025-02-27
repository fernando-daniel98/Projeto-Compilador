#include <stdio.h>
#include "globals.h"
#include <stdlib.h>
#include <string.h>

static int indentno = 0; /* number of spaces to indent */

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
void printSpaces(void) {
    for (int i = 0; i < indentno; i++)
        printf("\t");
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
        default: return "unknown op";
    }
}

/* printTree recursively prints the syntax tree */
void printTree(TreeNode* tree) {
    INDENT;
    while (tree != NULL) {
        printSpaces();
        if (tree->nodekind == StatementK) {
            switch (tree->kind.stmt) {
                case IfK:
                    printf("If\n");
                    break;
                case WhileK:
                    printf("While\n");
                    break;
                case ReturnINT:
                    printf("Return (int)\n");
                    break;
                case ReturnVOID:
                    printf("Return (void)\n");
                    break;
                case NuloDecl:
                    printf("Compound Statement\n");
                    break;
                case VarDeclK:
                    printf("Var Declaration: %s (type: %s)\n", 
                        tree->attr.name, 
                        expTypeToString(tree->type));
                    break;
                case VetDeclK:
                    printf("Vector Declaration: %s (type: %s)\n", 
                        tree->attr.name, 
                        expTypeToString(tree->type));
                    break;
                case FunDeclK:
                    printf("Function Declaration: %s (return type: %s)\n", 
                        tree->attr.name, 
                        expTypeToString(tree->type));
                    break;
                case VarParamK:
                    printf("Parameter: %s (type: %s)\n", 
                        tree->attr.name, 
                        expTypeToString(tree->type));
                    break;
                case VetParamK:
                    printf("Vector Parameter: %s (type: %s)\n", 
                        tree->attr.name, 
                        expTypeToString(tree->type));
                    break;
                case ParamVoid:
                    printf("Void Parameter\n");
                    break;
                default:
                    printf("Unknown StatementK node kind\n");
                    break;
            }
        }
        else if (tree->nodekind == ExpressionK) {
            switch (tree->kind.exp) {
                case OpK:
                    printf("Operator: %s\n", operatorToString(tree->attr.op));
                    break;
                case OpRel:
                    printf("Relational: %s\n", operatorToString(tree->attr.op));
                    break;
                case ConstK:
                    printf("Const: %d\n", tree->attr.val);
                    break;
                case IdK:
                    printf("Id: %s\n", tree->attr.name);
                    break;
                case AtivK:
                    printf("Function Call: %s\n", tree->attr.name);
                    break;
                case VetorK:
                    printf("Vector: %s\n", tree->attr.name);
                    break;
                case AssignK:
                    printf("Assign\n");
                    break;
                case NuloExp:
                    printf("Null Expression\n");
                    break;
                default:
                    printf("Unknown ExpressionK node kind\n");
                    break;
            }
        }
        else {
            printf("Unknown node kind\n");
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

void freeTree(TreeNode* tree) {
    for (int i = 0; i < MAXCHILDREN; i++) {
        if (tree->child[i] != NULL) {
            freeTree(tree->child[i]);
        }
    }
    if (tree->sibling != NULL) {
        freeTree(tree->sibling);
    }
    free(tree);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/globals.h"
#include "../include/codeGen.h"

// Função auxiliar para mapear OperatorType para typeOperations e retornar o nome da operação TAC
const char* getTACOperationName(typeOperations op) {
    switch (op) {
        // Operações aritméticas
        case ADD: return "ADD (+)";
        case SUB: return "SUB (-)";
        case MULT: return "MULT (*)";
        case DIV: return "DIV (/)";
        // Operações relacionais
        case LT: return "LT (<)";
        case LTE: return "LTE (<=)";
        case GT: return "GT (>)";
        case GTE: return "GTE (>=)";
        case EQ: return "EQ (==)";
        case NEQ: return "NEQ (!=)";
        // Atribuição
        case ASSIGN: return "ASSIGN (=)";
        // Controle de fluxo
        case LABEL: return "LABEL";
        case GOTO: return "GOTO";
        case IF: return "IF";
        case IFFALSE: return "IFFALSE";
        case RETURN: return "RETURN";
        case PARAM: return "PARAM";
        case CALL: return "CALL";
        case ARG: return "ARG";
        case FUNC: return "FUNC";
        case END: return "END";
        case ARRAY_INDEX: return "ARRAY_INDEX";
        case ARRAY_ASSIGN: return "ARRAY_ASSIGN";
        // Outros
        case OP_DESCON: return "OP_DESCON (operador desconhecido)";
        default: return "defaultInGetTACOperationName";
    }
}

// Função para mapear OperatorType para typeOperations
typeOperations mapOperatorToTAC(int op) {
    switch (op) {
        case PLUS_OP: return ADD;
        case MINUS_OP: return SUB;
        case MULT_OP: return MULT;
        case DIV_OP: return DIV;
        case LT_OP: return LT;
        case LTE_OP: return LTE;
        case GT_OP: return GT;
        case GTE_OP: return GTE;
        case EQ_OP: return EQ;
        case NEQ_OP: return NEQ;
        case ASSIGN_OP: return ASSIGN;
        default: return OP_DESCON; // Operador desconhecido
    }
}

// Percorre a árvore sintática e mostra o tipo de operação TAC
void percorreAAS(TreeNode* node) {
    if (node == NULL) return;

    // Verifica o número de linha (evita linhas zeradas ou inválidas)
    int linhaNode = (node->lineno > 0) ? node->lineno : 0;
    
    // Verifica o tipo de nó e sua operação
    if (node->nodekind == ExpressionK) {
        switch (node->kind.exp) {
            case OpK:
            case OpRel:
                typeOperations tacOp = mapOperatorToTAC(node->attr.op);
                printf("Linha %d: Operação TAC: %s", linhaNode, getTACOperationName(tacOp));
                
                // Mostrar os operandos se disponíveis
                if (node->child[0] != NULL) {
                    printf("\n  Operando Esquerdo: ");
                    if (node->child[0]->nodekind == ExpressionK) {
                        if (node->child[0]->kind.exp == IdK && node->child[0]->attr.name) {
                            printf("Variável '%s'", node->child[0]->attr.name);
                        } else if (node->child[0]->kind.exp == ConstK) {
                            printf("Constante %d", node->child[0]->attr.val);
                        } else {
                            printf("(Subexpressão)");
                        }
                    }
                }
                
                if (node->child[1] != NULL) {
                    printf("\n  Operando Direito: ");
                    if (node->child[1]->nodekind == ExpressionK) {
                        if (node->child[1]->kind.exp == IdK && node->child[1]->attr.name) {
                            printf("Variável '%s'", node->child[1]->attr.name);
                        } else if (node->child[1]->kind.exp == ConstK) {
                            printf("Constante %d", node->child[1]->attr.val);
                        } else {
                            printf("(Subexpressão)");
                        }
                    }
                }
                
                printf("\n");
                break;
            
            case AssignK:
                printf("Linha %d: Operação TAC: %s", linhaNode, getTACOperationName(ASSIGN));
                
                // Mostrar variável destino
                if (node->child[0] != NULL && node->child[0]->nodekind == ExpressionK) {
                    if (node->child[0]->kind.exp == IdK && node->child[0]->attr.name) {
                        printf("\n  Destino: Variável '%s'", node->child[0]->attr.name);
                    } else if (node->child[0]->kind.exp == VetorK && node->child[0]->attr.name) {
                        printf("\n  Destino: Vetor '%s'", node->child[0]->attr.name);
                    }
                }
                
                // Mostrar valor atribuído
                if (node->child[1] != NULL) {
                    printf("\n  Valor: ");
                    if (node->child[1]->nodekind == ExpressionK) {
                        if (node->child[1]->kind.exp == IdK && node->child[1]->attr.name) {
                            printf("Variável '%s'", node->child[1]->attr.name);
                        } else if (node->child[1]->kind.exp == ConstK) {
                            printf("Constante %d", node->child[1]->attr.val);
                        } else if (node->child[1]->kind.exp == AtivK && node->child[1]->attr.name) {
                            printf("Chamada de função '%s'", node->child[1]->attr.name);
                        } else {
                            printf("(Expressão)");
                        }
                    }
                }
                
                printf("\n");
                break;
            
            case AtivK:
                printf("Linha %d: Operação TAC: %s (chamada de função", linhaNode, getTACOperationName(CALL));
                if (node->attr.name) {
                    printf(" '%s')", node->attr.name);
                } else {
                    printf(")");
                }
                
                // Mostrar argumentos da chamada
                if (node->child[0] != NULL) {
                    printf("\n  Argumentos:");
                    TreeNode* argNode = node->child[0];
                    int argCount = 0;
                    
                    while (argNode != NULL) {
                        printf("\n  Arg %d: ", ++argCount);
                        if (argNode->nodekind == ExpressionK) {
                            if (argNode->kind.exp == IdK && argNode->attr.name) {
                                printf("Variável '%s'", argNode->attr.name);
                            } else if (argNode->kind.exp == ConstK) {
                                printf("Constante %d", argNode->attr.val);
                            } else {
                                printf("(Expressão)");
                            }
                        }
                        argNode = argNode->sibling;
                    }
                }
                
                printf("\n");
                break;
            
            case VetorK:
                printf("Linha %d: Operação TAC: %s", linhaNode, getTACOperationName(ARRAY_INDEX));
                if (node->attr.name) {
                    printf(" (vetor: '%s')", node->attr.name);
                }
                
                // Mostrar índice do acesso ao vetor
                if (node->child[0] != NULL) {
                    printf("\n  Índice: ");
                    if (node->child[0]->nodekind == ExpressionK) {
                        if (node->child[0]->kind.exp == IdK && node->child[0]->attr.name) {
                            printf("Variável '%s'", node->child[0]->attr.name);
                        } else if (node->child[0]->kind.exp == ConstK) {
                            printf("Constante %d", node->child[0]->attr.val);
                        } else {
                            printf("(Expressão)");
                        }
                    }
                }
                
                printf("\n");
                break;
                
            case IdK:
                // Os identificadores já são mostrados como operandos de outras operações
                break;
                
            case ConstK:
                // As constantes já são mostradas como operandos de outras operações
                break;
                
            default:
                break;
        }
    } else if (node->nodekind == StatementK) {
        switch (node->kind.stmt) {
            case IfK:
                printf("Linha %d: Operação TAC: %s (condicional)\n", linhaNode, getTACOperationName(IF));
                
                // Mostrar condição do if
                if (node->child[0] != NULL) {
                    printf("  Condição: ");
                    if (node->child[0]->nodekind == ExpressionK) {
                        if (node->child[0]->kind.exp == OpRel) {
                            typeOperations relOp = mapOperatorToTAC(node->child[0]->attr.op);
                            printf("Operação %s\n", getTACOperationName(relOp));
                        } else {
                            printf("(Expressão)\n");
                        }
                    }
                }
                break;
            
            case WhileK:
                printf("Linha %d: Operação TAC: %s (laço while)\n", linhaNode, getTACOperationName(GOTO));
                
                // Mostrar condição do while
                if (node->child[0] != NULL) {
                    printf("  Condição: ");
                    if (node->child[0]->nodekind == ExpressionK) {
                        if (node->child[0]->kind.exp == OpRel) {
                            typeOperations relOp = mapOperatorToTAC(node->child[0]->attr.op);
                            printf("Operação %s\n", getTACOperationName(relOp));
                        } else {
                            printf("(Expressão)\n");
                        }
                    }
                }
                break;
            
            case ReturnINT:
            case ReturnVOID:
                printf("Linha %d: Operação TAC: %s", linhaNode, getTACOperationName(RETURN));
                
                // Mostrar valor de retorno se for ReturnINT
                if (node->kind.stmt == ReturnINT && node->child[0] != NULL) {
                    printf("\n  Valor de retorno: ");
                    if (node->child[0]->nodekind == ExpressionK) {
                        if (node->child[0]->kind.exp == IdK && node->child[0]->attr.name) {
                            printf("Variável '%s'", node->child[0]->attr.name);
                        } else if (node->child[0]->kind.exp == ConstK) {
                            printf("Constante %d", node->child[0]->attr.val);
                        } else {
                            printf("(Expressão)");
                        }
                    }
                }
                
                printf("\n");
                break;
            
            case FunDeclK:
                printf("Linha %d: Operação TAC: %s (declaração de função '%s')\n", 
                       linhaNode, 
                       getTACOperationName(FUNC), 
                       node->attr.name ? node->attr.name : "anônima");
                break;
                
            case VarParamK:
            case VetParamK:
                printf("Linha %d: Operação TAC: %s (parâmetro '%s')\n", 
                       linhaNode, 
                       getTACOperationName(PARAM),
                       node->attr.name ? node->attr.name : "anônimo");
                break;
                
            default:
                break;
        }
    }

    // Percorre filhos
    for (int i = 0; i < MAXCHILDREN; i++) {
        percorreAAS(node->child[i]);
    }
    // Percorre irmãos
    percorreAAS(node->sibling);
}
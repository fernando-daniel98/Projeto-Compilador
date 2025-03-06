#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/codeGen.h"
#include "../../include/globals.h"
#include "../../include/util.h"

// Gerador global de código TAC
static TACGenerator tacGen;

// Inicializa o gerador de código TAC
void initTAC(void) {
    tacGen.head = NULL;
    tacGen.tail = NULL;
    tacGen.tempCount = 0;
    tacGen.labelCount = 0;
}

// Libera os recursos do gerador de código TAC
void finalizeTAC(void) {
    if (tacGen.head == NULL) {
        return; // Já está vazio, nada a fazer
    }
    
    TACInstr *current = tacGen.head;
    TACInstr *next;
    
    while (current != NULL) {
        next = current->next; // Salva o próximo antes de liberar o atual
        
        // Não tentamos liberar as strings para evitar problemas com ponteiros compartilhados
        // Apenas liberamos a estrutura da instrução
        free(current);
        
        current = next;
    }
    
    // Reinicializa o gerador
    tacGen.head = NULL;
    tacGen.tail = NULL;
    // Não reiniciamos os contadores para evitar conflitos caso o gerador seja usado novamente
}

// Gera um novo nome para variável temporária
char *newTemp(void) {
    char *temp = (char *)malloc(15);
    sprintf(temp, "t%d", tacGen.tempCount++);
    return temp;
}

// Gera um novo rótulo
char *newLabel(void) {
    char *label = (char *)malloc(15);
    sprintf(label, "L%d", tacGen.labelCount++);
    return label;
}

// Adiciona uma instrução TAC
TACInstr *addTACInstr(TACOp op, char *arg1, char *arg2, char *result) {
    TACInstr *instr = (TACInstr *)malloc(sizeof(TACInstr));
    instr->op = op;
    instr->arg1 = arg1;
    instr->arg2 = arg2;
    instr->result = result;
    instr->next = NULL;
    
    if (tacGen.head == NULL) {
        tacGen.head = tacGen.tail = instr;
    } else {
        tacGen.tail->next = instr;
        tacGen.tail = instr;
    }
    
    return instr;
}

// Converte operador de C- para operador TAC
TACOp convertOp(int op) {
    switch (op) {
        case PLUS_OP: return TAC_ADD;
        case MINUS_OP: return TAC_SUB;
        case MULT_OP: return TAC_MULT;
        case DIV_OP: return TAC_DIV;
        case LT_OP: return TAC_LT;
        case LTE_OP: return TAC_LTE;
        case GT_OP: return TAC_GT;
        case GTE_OP: return TAC_GTE;
        case EQ_OP: return TAC_EQ;
        case NEQ_OP: return TAC_NEQ;
        case ASSIGN_OP: return TAC_ASSIGN;
        default: return TAC_ASSIGN; // Default
    }
}

// Função para criar uma cópia de uma string
char *copyString(const char *s) {
    if (s == NULL) return NULL;
    char *copy = malloc(strlen(s) + 1);
    strcpy(copy, s);
    return copy;
}

// Função auxiliar para gerar código TAC para expressões
char *genExpr(TreeNode *tree) {
    if (tree == NULL) return NULL;
    
    char *result;
    char *arg1, *arg2;
    
    switch (tree->nodekind) {
        case ExpressionK:
            switch (tree->kind.exp) {
                case OpK:
                    if (isUnaryNegative(tree)) {
                        arg1 = genExpr(tree->child[0]);
                        result = newTemp();
                        addTACInstr(TAC_SUB, "0", arg1, result);
                        return result;
                    } else {
                        arg1 = genExpr(tree->child[0]);
                        arg2 = genExpr(tree->child[1]);
                        result = newTemp();
                        addTACInstr(convertOp(tree->attr.op), arg1, arg2, result);
                        return result;
                    }
                    
                case OpRel:
                    arg1 = genExpr(tree->child[0]);
                    arg2 = genExpr(tree->child[1]);
                    result = newTemp();
                    addTACInstr(convertOp(tree->attr.op), arg1, arg2, result);
                    return result;
                    
                case ConstK:
                    // Converter o valor inteiro para string
                    result = (char *)malloc(20);
                    sprintf(result, "%d", tree->attr.val);
                    return result;
                    
                case IdK:
                    // Retornar o nome do identificador
                    return copyString(tree->attr.name);
                    
                case VetorK:
                    // Acesso a elemento de array: a[i]
                    arg1 = copyString(tree->attr.name);
                    arg2 = genExpr(tree->child[0]); // índice
                    result = newTemp();
                    addTACInstr(TAC_ARRAY_INDEX, arg1, arg2, result);
                    return result;
                    
                case AssignK:
                    // Atribuição: x = expr ou a[i] = expr
                    if (tree->child[0]->kind.exp == VetorK) {
                        // Array assignment: a[i] = expr
                        arg1 = copyString(tree->child[0]->attr.name); // array name
                        arg2 = genExpr(tree->child[0]->child[0]); // index
                        result = genExpr(tree->child[1]); // expression
                        addTACInstr(TAC_ARRAY_ASSIGN, arg1, arg2, result);
                        return result;
                    } else {
                        // Simple assignment: x = expr
                        arg1 = genExpr(tree->child[1]);
                        result = copyString(tree->child[0]->attr.name);
                        addTACInstr(TAC_ASSIGN, arg1, NULL, result);
                        return copyString(result);
                    }
                    
                case AtivK: {
                    // Chamada de função: f(args)
                    int argCount = 0;
                    TreeNode *arg = tree->child[0];
                    
                    // Processar argumentos na ordem reversa (empilhar)
                    // Primeiro, conte o número de argumentos
                    TreeNode *countArg = arg;
                    while (countArg != NULL) {
                        argCount++;
                        countArg = countArg->sibling;
                    }
                    
                    // Armazenar argumentos em uma pilha (array)
                    char **args = (char **)malloc(argCount * sizeof(char *));
                    int i = 0;
                    while (arg != NULL) {
                        args[i++] = genExpr(arg);
                        arg = arg->sibling;
                    }
                    
                    // Adicionar instruções de parâmetro na ordem correta
                    for (i = 0; i < argCount; i++) {
                        addTACInstr(TAC_PARAM, args[i], NULL, NULL);
                    }
                    
                    free(args); // Liberar o array, não as strings (estão na TAC)
                    
                    // Função call
                    result = newTemp();
                    char *funcName = copyString(tree->attr.name);
                    char *argCountStr = (char *)malloc(10);
                    sprintf(argCountStr, "%d", argCount);
                    addTACInstr(TAC_CALL, funcName, argCountStr, result);
                    return result;
                }
                
                default:
                    return copyString("unknown");
            }
            
        default:
            return copyString("unknown");
    }
}

// Função recursiva para gerar código TAC para declarações
void genStmt(TreeNode *tree) {
    if (tree == NULL) return;
    
    char *result, *arg1, *arg2, *label, *labelElse, *labelEnd;
    
    switch (tree->nodekind) {
        case StatementK:
            switch (tree->kind.stmt) {
                case IfK:
                    // Gera código para a condição
                    arg1 = genExpr(tree->child[0]);
                    
                    // Cria rótulos para as instruções de desvio
                    labelElse = newLabel();
                    
                    if (tree->child[2] != NULL) {
                        // If-then-else
                        labelEnd = newLabel();
                        
                        // Se condição é falsa, vai para o else
                        addTACInstr(TAC_IFFALSE, arg1, NULL, labelElse);
                        
                        // Gera código para o bloco then
                        genStmt(tree->child[1]);
                        
                        // Ao final do then, salta para o final
                        addTACInstr(TAC_GOTO, NULL, NULL, labelEnd);
                        
                        // Insere o rótulo do else
                        addTACInstr(TAC_LABEL, NULL, NULL, labelElse);
                        
                        // Gera código para o bloco else
                        genStmt(tree->child[2]);
                        
                        // Insere o rótulo do fim do if
                        addTACInstr(TAC_LABEL, NULL, NULL, labelEnd);
                    } else {
                        // If-then (sem else)
                        
                        // Se condição é falsa, vai para o fim
                        addTACInstr(TAC_IFFALSE, arg1, NULL, labelElse);
                        
                        // Gera código para o bloco then
                        genStmt(tree->child[1]);
                        
                        // Insere o rótulo do fim do if
                        addTACInstr(TAC_LABEL, NULL, NULL, labelElse);
                    }
                    break;
                    
                case WhileK:
                    // Cria rótulos para o início do while e para o fim
                    label = newLabel();
                    labelEnd = newLabel();
                    
                    // Insere o rótulo do início
                    addTACInstr(TAC_LABEL, NULL, NULL, label);
                    
                    // Gera código para a condição
                    arg1 = genExpr(tree->child[0]);
                    
                    // Se condição é falsa, vai para o fim
                    addTACInstr(TAC_IFFALSE, arg1, NULL, labelEnd);
                    
                    // Gera código para o corpo do while
                    genStmt(tree->child[1]);
                    
                    // Volta ao início do while
                    addTACInstr(TAC_GOTO, NULL, NULL, label);
                    
                    // Insere o rótulo do fim do while
                    addTACInstr(TAC_LABEL, NULL, NULL, labelEnd);
                    break;
                    
                case ReturnINT:
                    // Retorno com valor
                    arg1 = genExpr(tree->child[0]);
                    addTACInstr(TAC_RETURN, arg1, NULL, NULL);
                    break;
                    
                case ReturnVOID:
                    // Retorno sem valor
                    addTACInstr(TAC_RETURN, NULL, NULL, NULL);
                    break;
                    
                case FunDeclK: {
                    // Declaração de função
                    char *funcName = copyString(tree->attr.name);
                    addTACInstr(TAC_FUNC, funcName, NULL, NULL);
                    
                    // Processar parâmetros
                    TreeNode *param = tree->child[0];
                    int paramCount = 0;
                    while (param != NULL) {
                        if (param->nodekind == StatementK && 
                            (param->kind.stmt == VarParamK || param->kind.stmt == VetParamK)) {
                            char *paramName = copyString(param->attr.name);
                            addTACInstr(TAC_ARG, paramName, NULL, NULL);
                            paramCount++;
                        }
                        param = param->sibling;
                    }
                    
                    // Processar corpo da função
                    genStmt(tree->child[1]);
                    
                    // Fim da função
                    addTACInstr(TAC_END, funcName, NULL, NULL);
                    break;
                }
                
                case NuloDecl:
                    // Bloco composto
                    // Processar declarações locais
                    if (tree->child[0] != NULL) {
                        genStmt(tree->child[0]);
                    }
                    
                    // Processar lista de statements
                    if (tree->child[1] != NULL) {
                        genStmt(tree->child[1]);
                    }
                    break;
                    
                case VarDeclK:
                    // Variáveis já são tratadas na tabela de símbolos
                    // Não precisamos gerar código TAC para declarações
                    break;
                    
                case VetDeclK:
                    // Arrays já são tratados na tabela de símbolos
                    // Não precisamos gerar código TAC para declarações
                    break;
                    
                default:
                    // Outros tipos de declaração
                    break;
            }
            break;
            
        case ExpressionK:
            // Expressão isolada (como statement)
            genExpr(tree);
            break;
    }
    
    // Processar nó irmão
    if (tree->sibling != NULL) {
        genStmt(tree->sibling);
    }
}

// Gera código TAC a partir da árvore sintática
void generateTAC(TreeNode *syntaxTree) {
    initTAC(); // Inicializa o gerador
    genStmt(syntaxTree); // Gerar código para a árvore
}

// Converte enum TACOp para string
const char *tacOpToString(TACOp op) {
    switch (op) {
        case TAC_ADD: return "ADD";
        case TAC_SUB: return "SUB";
        case TAC_MULT: return "MULT";
        case TAC_DIV: return "DIV";
        case TAC_LT: return "LT";
        case TAC_LTE: return "LTE";
        case TAC_GT: return "GT";
        case TAC_GTE: return "GTE";
        case TAC_EQ: return "EQ";
        case TAC_NEQ: return "NEQ";
        case TAC_ASSIGN: return "ASSIGN";
        case TAC_LABEL: return "LABEL";
        case TAC_GOTO: return "GOTO";
        case TAC_IF: return "IF";
        case TAC_IFFALSE: return "IFFALSE";
        case TAC_RETURN: return "RETURN";
        case TAC_PARAM: return "PARAM";
        case TAC_CALL: return "CALL";
        case TAC_ARG: return "ARG";
        case TAC_FUNC: return "FUNC";
        case TAC_END: return "END";
        case TAC_ARRAY_INDEX: return "ARRAY_INDEX";
        case TAC_ARRAY_ASSIGN: return "ARRAY_ASSIGN";
        default: return "UNKNOWN";
    }
}

// Imprime o código TAC gerado
void printTAC(FILE *out) {
    TACInstr *current = tacGen.head;
    int lineNum = 1;
    
    fprintf(out, "\nTHREE ADDRESS CODE\n");
    fprintf(out, "--------------------\n");
    
    while (current != NULL) {
        fprintf(out, "%3d: ", lineNum++);
        
        switch (current->op) {
            case TAC_LABEL:
                fprintf(out, "%s:", current->result);
                break;
                
            case TAC_GOTO:
                fprintf(out, "goto %s", current->result);
                break;
                
            case TAC_IF:
                fprintf(out, "if %s goto %s", current->arg1, current->result);
                break;
                
            case TAC_IFFALSE:
                fprintf(out, "iffalse %s goto %s", current->arg1, current->result);
                break;
                
            case TAC_ASSIGN:
                fprintf(out, "%s := %s", current->result, current->arg1);
                break;
                
            case TAC_ADD:
            case TAC_SUB:
            case TAC_MULT:
            case TAC_DIV:
            case TAC_LT:
            case TAC_LTE:
            case TAC_GT:
            case TAC_GTE:
            case TAC_EQ:
            case TAC_NEQ:
                fprintf(out, "%s := %s %s %s", 
                        current->result, current->arg1, 
                        tacOpToString(current->op), current->arg2);
                break;
                
            case TAC_RETURN:
                if (current->arg1)
                    fprintf(out, "return %s", current->arg1);
                else
                    fprintf(out, "return");
                break;
                
            case TAC_PARAM:
                fprintf(out, "param %s", current->arg1);
                break;
                
            case TAC_CALL:
                fprintf(out, "%s := call %s, %s", 
                        current->result, current->arg1, current->arg2);
                break;
                
            case TAC_FUNC:
                fprintf(out, "function %s:", current->arg1);
                break;
                
            case TAC_END:
                fprintf(out, "end function %s", current->arg1);
                break;
                
            case TAC_ARG:
                fprintf(out, "arg %s", current->arg1);
                break;
                
            case TAC_ARRAY_INDEX:
                fprintf(out, "%s := %s[%s]", current->result, current->arg1, current->arg2);
                break;
                
            case TAC_ARRAY_ASSIGN:
                fprintf(out, "%s[%s] := %s", current->arg1, current->arg2, current->result);
                break;
                
            default:
                fprintf(out, "unknown operation");
                break;
        }
        
        fprintf(out, "\n");
        current = current->next;
    }
    
    fprintf(out, "--------------------\n\n");
}

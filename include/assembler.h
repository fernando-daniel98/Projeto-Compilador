#ifndef Assembly_H
#define Assembly_H 1

/*
 * ==========================================
 * PROCESSADOR MIPS-64 - REGISTRADORES ESPECIAIS
 * ==========================================
 * 
 * REGISTRADORES RESERVADOS (Sistema):
 * - $zero (63): Sempre contém 0 (0x3F em hex, 111111 em binário)
 * - $ra   (62): Return Address - endereço de retorno (0x3E em hex, 111110 em binário)
 * - $fp   (61): Frame Pointer - início do frame da função (0x3D em hex, 111101 em binário)
 * - $sp   (60): Stack Pointer - fim/topo da pilha (0x3C em hex, 111100 em binário)
 * - $temp (59): Temporário do compilador (0x3B em hex, 111011 em binário)
 * - $pilha(58): Ponteiro para pilha de parâmetros (0x3A em hex, 111010 em binário)
 * - $s2   (57): Registrador salvo (0x39 em hex, 111001 em binário)
 * - $s1   (56): Registrador salvo (0x38 em hex, 111000 em binário)
 * - $s0   (55): Registrador salvo (0x37 em hex, 110111 em binário)
 * 
 * REGISTRADORES DE USO GERAL:
 * - $t0-$t54 (0-54): Registradores temporários para uso geral
 * 
 * CONVENÇÕES DE FRAME DE FUNÇÃO:
 * Offsets relativos ao $fp (Frame Pointer):
 * 0($fp): parâmetro 1 da função
 * 1($fp): parâmetro 2 da função
 * 2($fp): parâmetro 3 da função
 * 3($fp): endereço onde armazenar valor de retorno (ponteiro)
 * 4($fp): return address ($ra) - SEMPRE salvo aqui
 * 5($fp): valor de retorno da função (se aplicável)
 * 6-8($fp): variáveis locais adicionais
 * 9-11($fp): variáveis locais temporárias
 * 
 * PADRÃO DE GERENCIAMENTO DA PILHA:
 * - Tamanho padrão do frame: 25 posições
 * - Crescimento da pilha: 
 *   addi $fp $fp 25  (cresce frame pointer)
 *   addi $sp $sp 25  (cresce stack pointer)
 * - Redução da pilha:
 *   subi $fp $fp 25  (reduz frame pointer)
 *   subi $sp $sp 25  (reduz stack pointer)
 * 
 * SEQUÊNCIA PADRÃO DE CHAMADA DE FUNÇÃO:
 * 1. Preparar parâmetros na $pilha (reg 58):
 *    sw $param1 0($pilha)
 *    sw $param2 1($pilha)
 *    sw $param3 2($pilha)
 * 
 * 2. Transferir parâmetros para a stack:
 *    lw $temp 2($pilha); sw $temp 3($sp)  # param 3
 *    lw $temp 1($pilha); sw $temp 2($sp)  # param 2
 *    lw $temp 0($pilha); sw $temp 1($sp)  # param 1
 * 
 * 3. Salvar endereço do frame anterior:
 *    add $temp $fp $zero; addi $temp $temp 3
 *    sw $temp 4($sp)
 * 
 * 4. Crescer a pilha e chamar:
 *    addi $fp $fp 25; addi $sp $sp 25
 *    jal função
 * 
 * 5. Restaurar após retorno:
 *    subi $fp $fp 25; subi $sp $sp 25
 * 
 * SEQUÊNCIA PADRÃO DE RETORNO DE FUNÇÃO:
 * 1. Salvar $ra no início da função:
 *    sw $ra 4($fp)
 * 
 * 2. Preparar valor de retorno (se aplicável):
 *    lw $temp 3($fp)          # endereço onde salvar
 *    sw $valor_retorno 2($temp)
 * 
 * 3. Restaurar $ra e retornar:
 *    lw $ra 4($fp)
 *    jr $zero $ra $zero
 */

// Inclusao de bibliotecas
#include "label.h" // Mantido, pode ser necessário para TIPO_LABEL ou outros usos futuros
#include "globals.h" // Adicionado para yyout, caso seja usado aqui no futuro, e para consistência

// Capacidade maxima para o vetor dos nomes das instrucoes
#define MAX_CHAR_NOME 5
#define MAX_ASSEMBLY 10000

// Definições de registradores para sua arquitetura (64 registradores)
// Mapeamento baseado na lógica do Eduardo, mas ajustado para 64 regs
#define $zero 63   // Registrador sempre zero (Eduardo: 31)
#define $ra 62     // Return Address (Eduardo: 30)
#define $fp 61     // Frame Pointer (Eduardo: 29)
#define $sp 60     // Stack Pointer (Eduardo: 28)
#define $temp 59   // Registrador temporário especial (Eduardo: 27)
#define $pilha 58  // Registrador para pilha de parâmetros (Eduardo: 26)
#define $s2 57     // Saved register 2 (Eduardo: 25)
#define $s1 56     // Saved register 1 (Eduardo: 24)
#define $s0 55     // Saved register 0 (Eduardo: 23)

// Registradores temporários t0-t54 (Eduardo tinha t0-t22)
// Mapeamento direto: $t{n} = n (para n de 0 até 54)

typedef enum{
    typeR, // Instrucoes do tipo R
    typeI, // Instrucoes do tipo I
    typeJ, // Instrucoes do tipo J
    typeLabel // Label de funcoes ou de pulos
} tipoInstrucao;

/* Struct para armazenar as informacoes
de instrucoes do tipo R */
typedef struct tipoR{
    char *nome; // Nome da instrucao
    int rd; // Registrador destino
    int rt; // Registrador fonte
    int rs; // Registrador fonte
} TIPO_R;

/* Struct para armazenar as informacoes
de instrucoes do tipo I */
typedef struct tipoI{
    char *nome; // Nome da instrucao
    int rs; // Registrador fonte
    int rt; // Registrador destino
    int imediato; // Valor imediato
    int label; // Label para o branch
} TIPO_I;

/* Struct para armazenar as informacoes 
de instrucoes do tipo J */
typedef struct tipoJ{
    char *nome; // Nome da instrucao
    char *labelImediato; // Nome da Label para o jump
} TIPO_J;

typedef struct tipoLabel{
    int boolean; // Booleano para verificar se eh label(1) ou funcao(0)
    char *nome; // Nome da label
    int endereco; // Endereco da label
} TIPO_LABEL;


typedef struct assembly{
    tipoInstrucao tipo; // Tipo da instrucao
    TIPO_I * tipoI; // Ponteiro para a struct do tipo I
    TIPO_R * tipoR; // Ponteiro para a struct do tipo R
    TIPO_J * tipoJ; // Ponteiro para a struct do tipo J
    TIPO_LABEL * tipoLabel; // Ponteiro para a struct do tipo Label
} ASSEMBLY;

// Vetor para armazenar as instrucoes em assembly
extern ASSEMBLY ** instrucoesAssembly;
extern int indiceAssembly; // Indice para o vetor de instrucoes assembly

// Funcoes para a geracao do codigo assembly
void assembly();
void inicializaAssembly();
ASSEMBLY * criarNoAssembly(tipoInstrucao tipo, char *nome);
void imprimirAssembly();
void liberarAssembly();
void resolverLabels();  // Nova função para resolver referencias de labels
void imprimirAssemblySemLabels(); // Imprimir assembly sem labels e com referências resolvidas
void salvarAssemblyLimpo(const char* nomeArquivo); // Salvar assembly em arquivo sem números de linha
void salvarAssemblySemLabelsArquivo(const char* nomeArquivo); // Salvar assembly sem labels em arquivo
void salvarAssemblyPuro(const char* nomeArquivo); // Salvar assembly puro sem números de linha (último print CA)

// Funcao para mapear numero do registrador para nome legivel
const char* getRegisterName(int regNum);

#endif
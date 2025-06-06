#ifndef _LABEL_H_ 
#define _LABEL_H_ 1

typedef struct label{
    char* id;
    int endereco;
    struct label *prox;
} Label;

typedef struct vetorLabel{
    Label *vetor;
    int tamanho;
} VETOR_LABEL;

extern VETOR_LABEL *vetorLabel;

void inicializaLabels();
Label * criarNoLabel(char* id, int endereco);
void adicionarLabel(char* id, int endereco);
int getEnderecoLabel(char* id);
void liberarLabels();
void imprimirLabels();

#endif
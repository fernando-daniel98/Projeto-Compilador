# Projeto-Compilador
Projeto final da disciplina de Compiladores da Universidade Federal de São Paulo.

## Integrantes
Os integrantes deste projeto são:
- [Fernando Daniel Marcelino](https://github.com/fernando-daniel98)
- [Marcos Vinicius Gasparoto Bauab](https://github.com/MarcosBauab)

## Tecnologias


## Execução
Para executar o projeto, basta rodar o comando:

bison -d -v parser.y

flex lexer.l

gcc -c lex.yy.c

g++ -o compilador lex.yy.o parser.tab.c lex.yy.c -lfl

./ compilador entrada.txt >

## Funcionalidades

## Estrutura do Projeto
```

```

## Referências
As referências utilizadas para a realização deste projeto foram:

# Projeto-Compilador
Projeto final da disciplina de Compiladores da Universidade Federal de São Paulo.

## Integrantes
Os integrantes deste projeto são:
- [Fernando Daniel Marcelino](https://github.com/fernando-daniel98)
- [Marcos Vinicius Gasparoto Bauab](https://github.com/MarcosBauab)

## Tecnologias


## Execução
Para executar o projeto, basta rodar o comando:

bison -d -v <nome_do_arquivo>.y

flex <nome_do_arquivo>.l

gcc -c lex.yy.c

g++ -o <nome_do_executavel> <nome_do_compilado>.o <nome_do_arquivo>.tab.c lex.yy.c -lfl

./ <nome_do_executavel> < <nome_do_arquivo_de_entrada>.txt > <nome_do_arquivo_de_saida>.txt

## Funcionalidades

## Estrutura do Projeto
```

```

## Referências
As referências utilizadas para a realização deste projeto foram:

%{
#include <stdlib.h>
#include <stdio.h>

#define NUM 300
#define ID  301
#define FIM 399

%}

DIGITO [0-9]
LETRA [a-zA-Z]

%%

{DIGITO}+ {
    printf("NUM");
    fflush(stdout); // Garante que a saída seja exibida imediatamente
}

{LETRA}({LETRA}|{DIGITO})* {
    printf("ID");
    fflush(stdout);
}

. {
    putchar(yytext[0]); // Passa caracteres que não são IDs ou números diretamente
}

<<EOF>> {
    printf("FIM\n");
    return FIM;
}

%%

int main(int argc, char *argv[])
{
    FILE *f_in;

    if (argc == 2)
    {
        if ((f_in = fopen(argv[1], "r")))  yyin = f_in;
        else
        {
            perror(argv[0]);
            return EXIT_FAILURE;
        }
    }
    else
        yyin = stdin;
  
    yylex(); // Inicia o analisador léxico

    return EXIT_SUCCESS;
}

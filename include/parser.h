#ifndef PARSER_H
#define PARSER_H

#include "globals.h"

// Protótipo para a função de parsing
TreeNode *parse(void);

// Protótipos de funções do parser
int yyparse(void);
void yyerror(const char *s);

#endif
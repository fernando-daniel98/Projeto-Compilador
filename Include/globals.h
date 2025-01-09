#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define MAXTOKENLEN 40
#define MAXIDLEN 33
#define MAXSTACK 30


// Cores em ANSI
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GRAY     "\x1b[0;37m"
#define ANSI_COLOR_WHITE    "\x1b[1;37m"
#define ANSI_COLOR_RESET    "\x1b[0m"
#define ANSI_COLOR_PURPLE   "\x1b[0;35m"
#define ANSI_COLOR_GREEN    "\x1b[0;32m"

extern int lineNum; /* source line number for listing */
extern int countErrorsLexer; /* number of errors in the lexer */
extern int countErrorsParser; /* number of errors in the parser */
extern int countErrorsSemantic; /* number of errors in the semantic analysis */

extern FILE *arquivoEntrada; /* source code text file */
extern FILE *arquivoSaida; /* output code text file */


typedef enum 
    /* book-keeping tokens */
   {ENDOFFILE = 300, ERROR,

    /* reserved words */
    // if  else  int  return  void  while
    IF, ELSE, INT, RETURN , VOID, WHILE,

    /* multicharacter tokens */
    // ID  NUM
    ID, NUM,

    /* Símbolos especiais */ 
    // +  -  *  /  <  <=  >  >=  ==  !=  =  ;  ,  (  )  [  ]  {  }  /*  */
    PLUS, MINUS, MULT, DIV, SMAL, SMALEQ, GREAT, GREATEQ, EQ, DIFF, ASSIGN, SEMICOL, COMMA, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE, BEGCOMM, CLOSCOMM
} TokenType;

#endif
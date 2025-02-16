%{
#include "parser.tab.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXTOKENLEN 40

extern FILE *yyin;
extern FILE *yyout;
extern char* yytext;

int lineNum = 1;
char tokenString[MAXTOKENLEN];

/* Helper function to create new nodes */
TreeNode* newNode(NodeKind nodekind) {
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    if (t == NULL) {
        fprintf(stderr, "Out of memory error at line %d\n", lineNum);
        exit(1);
    }
    for (int i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = nodekind;
    t->lineno = lineNum;
    return t;
}
%}

%option noyywrap

DIGITO    [0-9]
NUMBER    {DIGITO}+
LETRA     [a-zA-Z]
IDENT     {LETRA}({LETRA}|{DIGITO})*
NEWLINE   \n
WHITESPACE [ \t]+
COMMENT    "/*"(.|\n)*"*/"

%%

"else"            { return ELSE; }
"if"              { 
                    TreeNode* node = newNode(StatementK);
                    node->kind.stmt = IfK;
                    yylval.node = node;
                    return IF; 
                  }
"int"             { return INT; }
"return"          { 
                    TreeNode* node = newNode(StatementK);
                    node->kind.stmt = ReturnINT;  // or ReturnVOID based on context
                    yylval.node = node;
                    return RETURN; 
                  }
"void"            { return VOID; }
"while"           { 
                    TreeNode* node = newNode(StatementK);
                    node->kind.stmt = WhileK;
                    yylval.node = node;
                    return WHILE; 
                  }
"+"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpK;
                    node->attr.op = PLUS_OP;
                    yylval.node = node;
                    return PLUS; 
                  }
"-"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpK;
                    node->attr.op = MINUS_OP;
                    yylval.node = node;
                    return MINUS; 
                  }
"*"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpK;
                    node->attr.op = MULT_OP;
                    yylval.node = node;
                    return MULT; 
                  }
"/"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpK;
                    node->attr.op = DIV_OP;
                    yylval.node = node;
                    return DIV; 
                  }
"<"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = LT_OP;
                    yylval.node = node;
                    return SMAL; 
                  }
"<="              { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = LTE_OP;
                    yylval.node = node;
                    return SMALEQ; 
                  }
">"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = GT_OP;
                    yylval.node = node;
                    return GREAT; 
                  }
">="              { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = GTE_OP;
                    yylval.node = node;
                    return GREATEQ; 
                  }
"=="              { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = EQ_OP;
                    yylval.node = node;
                    return EQ; 
                  }
"!="              { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = NEQ_OP;
                    yylval.node = node;
                    return DIFF; 
                  }
"="               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = AssignK;
                    node->attr.op = ASSIGN_OP;
                    yylval.node = node;
                    return ASSIGN; 
                  }
";"               { return SEMICOL; }
","               { printf("virgula \n"); fflush(stdout);  return COMMA; }
"("               { return LPAREN; }
")"               { return RPAREN; }
"["               { return LBRACKET; }
"]"               { return RBRACKET; }
"{"               { return LBRACE; }
"}"               { return RBRACE; }
{COMMENT}         { /* Ignorar */ }
{NUMBER}          { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = ConstK;
                    node->attr.val = atoi(yytext);
                    node->type = Integer;
                    yylval.node = node;
                    return NUM;
                  }
{IDENT}           { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = IdK;
                    node->attr.name = strdup(yytext);
                    yylval.node = node;
                    return ID;  
                  }
{WHITESPACE}      { /* Ignorar */ }
{NEWLINE}         { lineNum++; }
.                 {
                    fprintf(stderr, "ERRO LÉXICO: \"%s\" ", yytext);
                    fprintf(stderr, "LINHA: %d\n", lineNum);
                    return -1;
                  }
<<EOF>>           { return 0; }
%%
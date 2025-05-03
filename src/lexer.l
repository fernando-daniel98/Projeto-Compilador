%{
#include "../build/parser.tab.h"
#include "../include/globals.h"
#include "../include/syntax_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char* yytext;

int lexical_errors = 0;

int lineNum = 1;

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

%%

"/*"            {
                  char c, prev = 0;
                  while ((c = input())) {
                      if (c == '/' && prev == '*')
                          break;
                      if (c == '\n')
                          lineNum++;
                      prev = c;
                  }
                }
"else"            { return ELSE; }
"if"              { 
                    TreeNode* node = newNode(StatementK);
                    node->kind.stmt = IfK;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return IF; 
                  }
"int"             { return INT; }
"return"          { 
                    TreeNode* node = newNode(StatementK);
                    node->kind.stmt = ReturnINT;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return RETURN; 
                  }
"void"            { return VOID; }
"while"           { 
                    TreeNode* node = newNode(StatementK);
                    node->kind.stmt = WhileK;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return WHILE; 
                  }
"+"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpK;
                    node->attr.op = PLUS_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return PLUS; 
                  }
"-"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpK;
                    node->attr.op = MINUS_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return MINUS; 
                  }
"*"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpK;
                    node->attr.op = MULT_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return MULT; 
                  }
"/"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpK;
                    node->attr.op = DIV_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return DIV; 
                  }
"<"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = LT_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return SMAL; 
                  }
"<="              { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = LTE_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return SMALEQ; 
                  }
">"               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = GT_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return GREAT; 
                  }
">="              { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = GTE_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return GREATEQ; 
                  }
"=="              { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = EQ_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return EQ; 
                  }
"!="              { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = OpRel;
                    node->attr.op = NEQ_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return DIFF; 
                  }
"="               { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = AssignK;
                    node->attr.op = ASSIGN_OP;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return ASSIGN; 
                  }
";"               { return SEMICOL; }
","               { return COMMA; }
"("               { return LPAREN; }
")"               { return RPAREN; }
"["               { return LBRACKET; }
"]"               { return RBRACKET; }
"{"               { return LBRACE; }
"}"               { return RBRACE; }
{NUMBER}          { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = ConstK;
                    node->attr.val = atoi(yytext);
                    node->type = Integer;
                    node->lineno = lineNum;
                    yylval.node = node;
                    return NUM;
                  }
{IDENT}           { 
                    TreeNode* node = newNode(ExpressionK);
                    node->kind.exp = IdK;
                    node->attr.name = strdup(yytext);
                    node->lineno = lineNum;
                    yylval.node = node;
                    return ID;  
                  }
{WHITESPACE}      { /* Ignorar */ }
{NEWLINE}         { lineNum++; }
.                 {
                    fprintf(stderr, ANSI_COLOR_RED "ERRO LÉXICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", yytext);
                    fprintf(stderr, ANSI_COLOR_RED "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET "\n", lineNum);
                    lexical_errors++;
                    return ERROR;
                  }
<<EOF>>           { return 0; }
%%
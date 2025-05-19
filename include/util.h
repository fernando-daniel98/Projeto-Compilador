#ifndef UTIL_H
#define UTIL_H

#include "globals.h"
#include <stdlib.h>

int isUnaryNegative(TreeNode *tree);

/* Function to convert operator to string */
const char* operatorToString(int op);

#endif // UTIL_H
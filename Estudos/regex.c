#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

// https://www.geeksforgeeks.org/regular-expressions-in-c/

/*
Criação de Expressão Regular Para compilar ou criar a expressão regular, a função regcomp() é usada. Ela recebe três argumentos:
    Sintaxe: regcomp(®regex, expression, flag);

1. regex is a pointer to a memory location where expression is matched and stored.
2. expression is a string type
3. flag to specify the type of compilation

Return Value: This returns the value as shown below: 
0: when successful compilation is done.
Error_code: When there is unsuccessful compilation of the expression.

*/

/*
Matching of Pattern using Regular Expression 
The regexec() function is used to match a string against a pattern. It takes in five arguments: 
 

A precompiled pattern
A string in which the pattern needs to be searched for.
Information regarding the location of matches.
Flags to specify a change in the matching behavior.
Syntax: 
 

regexec(&regex, expression, 0, NULL, 0);
where, regex = precompiled pattern, 
expression = pattern to be match in regex, 
NULL = Information regarding location of the matches. 
flag = to specify the change in matching behaviour
Return Value: This returns the value as shown below: 
 

0: If there is a match.
REG_NOMATCH: If there is no match.
*/

// C program to illustrate the regexec() function
#include <regex.h>
#include <stdio.h>

// Function to print the result
void print_result(int value)
{

	// If pattern found
	if (value == 0) {
		printf("Pattern found.\n");
	}

	// If pattern not found
	else if (value == REG_NOMATCH) {
		printf("Pattern not found.\n");
	}

	// If error occurred during Pattern
	// matching
	else {
		printf("An error occurred.\n");
	}
}

// Driver Code
int main()
{

	// Variable to store initial regex()
	regex_t reegex;

	// Variable for return type
	int value;
	int value2;

    printf("Resultado 1:\n");

	// Creation of regEx
	value = regcomp( &reegex, "[:word:]", 0);

    if(!value)
        printf("1- RegEx compiled successfully.\n");
    else
        printf("1- Compilation Error.\n");

	// Comparing pattern "GeeksforGeeks" with
	// string in reg
	value = regexec( &reegex, "Fernando Daniel Marcelino", 0, NULL, 0);

	// Print the results
	print_result(value);


    printf("\n\nResultado 2:\n");

	// Creation of regEx
	value2 = regcomp( &reegex, "[:number:]", 0);

    if(!value2)
        printf("2- RegEx compiled successfully.\n");
    else
        printf("2- Compilation Error.\n");

	// Comparing pattern "Geeks"
	// with string in reg
	value2 = regexec( &reegex, "156490", 0, NULL, 0);

	print_result(value2);

	return 0;
}

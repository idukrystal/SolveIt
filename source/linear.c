#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define true 1
#define false 0
#define MEM_ERROR_MSG "memory problem encounterd"
#define NULL_ARG_MSG "null value passed as argument"
#define MEM_ERROR_CODE 1

// Function to remove spaces from a string
char **removeSpaces(char **eqnPtr) {
    int len = strlen(*eqnPtr);
    int i, j = 0;
    int spaces = 0;

    if (eqnPtr == NULL || *eqnPtr == NULL) {
      perror(NULL_ARG_MSG);
      return NULL;
    }
    for (i = 0; i < len; i++)
      if ((*eqnPtr)[i] == ' ')
	spaces++;
    char *newEqn = malloc(sizeof(char) * (len - spaces + 1));
    if (newEqn != NULL) {
      for (i = 0; i < len; i++)
	if ((*eqnPtr)[i] != ' ') {
	  newEqn[j++] = (*eqnPtr)[i];
	}
      newEqn[j] = '\0'; // Null-terminate the resulting string
    } else {
      perror(MEM_ERROR_MSG);
    }
    free(*eqnPtr);
    *eqnPtr = newEqn;
    return eqnPtr;
}

char **split(char **string, const char delimiter, int *numParts)
{
    // Initialize variables
    int numTokens = 0;

    // Copy the input string because strtok modifies it
    char *inputCopy = strdup(inputString);
    if (inputCopy == NULL)
    {
        perror(MEM_ERROR_MSG);
        return NULL;
    }

    // Tokenize the input string using strtok
    char *token = strtok(inputCopy, &delimiter);
    while (token != NULL)
    {
        // Allocate memory for the new token
        char *newToken = strdup(token);
        if (newToken == NULL)
        {
            perror("Memory allocation failed");
            free(inputCopy); // Free the inputCopy before returning
            return NULL;
        }

        // Add the new token to the result array
        numTokens++;
        resultArray = (char **)realloc(resultArray, sizeof(char *) * numTokens);
        if (resultArray == NULL)
        {
            perror("Memory allocation failed");
            free(newToken);  // Free the newToken before returning
            free(inputCopy); // Free the inputCopy before returning
            return NULL;
        }
        resultArray[numTokens - 1] = newToken;

        // Get the next token
        token = strtok(NULL, &delimiter);
    }

    // Free the copied input string and update the number of parts
    free(inputCopy);
    *numParts = numTokens;

    return resultArray;
}

char findVariable(const char *equation)
{
    int len = strlen(equation);
    char variable = '\0'; // Initialize variable to null character

    for (int i = 0; i < len; i++)
    {
        char currentChar = equation[i];

        if ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z'))
        {
            // Check if the current character is a letter (a-z or A-Z)
            if (variable == '\0')
            {
                // If variable is not set yet, set it to the current character
                variable = currentChar;
            }
            else if (variable != currentChar)
            {
                // If variable is already set to a different character, return an error
                printf("Error: Multiple variables found in the equation.\n");
                return '\0';
            }
        }
    }

    if (variable == '\0')
    {
        // No variable found in the equation
        printf("Error: No variable found in the equation.\n");
    }

    return variable;
}

simplify(char *eqn) {
  char variable = findVariable(eqn);
}

void expand(char *eqn, int start, int stop) {
    puts(">>>>>>>>>>\n");
    char curr;
    int brace = 0;
    bool inbrace = false;
    printf("%s  %d %d\n", eqn, start, stop);
    for (int i = start, j = 0; i <= stop; i++)
    {
        curr = eqn[i];
	printf("%c  %d  %d\n", curr, j, i);
        if (curr == '(') {
            if (!inbrace) {
                inbrace = true;
		printf("%c \n", 'i');
                j = i;
            }
            brace++;
        }
        if (curr == ')') {
            brace--;
            if (brace == 0) {
                inbrace = false;
                expand(eqn, j + 1,i - 1);
            } else if (brace < 0) {
                perror("Eqn not formated well");
            }
        }
    }
    puts(">>>>>>>>>>\n");
    
}

char *solve(const char *eqn)
{
  char *eqnDup = strdup(eqn);
  if (
      eqnDup == NULL ||
      removeSpaces(&eqnDup) == NULL
  ) {
    perror(MEM_ERROR_MSG);
    exit(MEM_ERROR_CODE);
  };
  split(eqnDup, '=');
  return eqnDup;
}

int main()
{
    char *strp = "(3x + 2) = (6-(45 + 3))";
    char *stp = strdup(strp);
    solve(stp);
    //expand(str, 0, strlen(str) - 1)
}

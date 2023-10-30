#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten/emscripten.h>
#include "solveit/basics.h"
#include "solveit/simulteneous.h"

#define EXTERN


EXTERN EMSCRIPTEN_KEEPALIVE char *solve(char *eqn) {
  int unknowns = 0;
  char *res = malloc(sizeof(char) * 1000);
  printf("equation at start\n%s\n", eqn);
  removeSpaces(&eqn);
  substitute(&eqn, "−", "-");
  standadize(&eqn);
  printf("about to fix\n");
  fixUtf8(&eqn);
  removeSpaces(&eqn);
  printf(">> %s\n", eqn);
  char **system = toSystem(eqn, &unknowns);
  double *matrix[unknowns];
  char ** vars = malloc(sizeof(char *) * unknowns);
  for (int i = 0; i < unknowns; i++)
    vars[i] = NULL;
  for (int i = 0; i < unknowns; i++) {
    matrix[i] = parse(system[i], unknowns, vars);
  }
  calculate(matrix, unknowns);
  for (int i = 0; i < 1000; i++) {
    res[i] = '\0';
  }
  for (int i = 0; i < unknowns; i++) {
    strcat(res, vars[i]);
    strcat(res, " = ");
    strcat(res, toString(matrix[i][unknowns]));
    strcat(res, "\n");
  }
  printf("res in c code is   %s\n", res);
  return res;
}

EXTERN EMSCRIPTEN_KEEPALIVE char *getInstruction() {
  char *tmp = "Enter Each Equation on it own Line";
  int size = strlen(tmp) + 1;
  char *inst = malloc(sizeof(char) * size);
  for (int i = 0; i < size; i++) {
    inst[i] = tmp[i];
  }
  return inst;
}

EXTERN EMSCRIPTEN_KEEPALIVE char *getHint() {
  char *tmp = "2x + 4 = 10";
  int size = strlen(tmp) + 1;
  char *hint = malloc(sizeof(char) * size);
  for (int i = 0; i < size; i++)
    hint[i] = tmp[i];
  return hint;
}

int main() {
  printf("simulteneous solver is ready\n");
}

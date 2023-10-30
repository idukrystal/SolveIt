#include <emscripten/emscripten.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./solveit/calculate.h"
#include "./solveit/types.h"

#define EXTERN

extern bool error;
extern char *errorMsg;

#define errorReturn				\
  if (error) {					\
    error = false;				\
    return errorMsg;				\
  }						\

EXTERN EMSCRIPTEN_KEEPALIVE char *solve(char *eqn)  {
  printf("start %s\n", eqn);
  removeSpaces(&eqn);
  errorReturn
  printf("after remove space  %s\n", eqn);
  standadize(&eqn);
  errorReturn
  printf("âtfter standadize %s\n", eqn);
  int ulen = utf8Len(eqn);
  normalize(&eqn, ulen);
  errorReturn
  printf("after normalize %s\n", eqn);
  int len = strlen(eqn);
  mulBrackets(&eqn);
  errorReturn
  printf("after mul brackets  %s\n", eqn);
  ulen = utf8Len(eqn);
  double ans = calculate(eqn , 0, ulen);
  errorReturn
  return toString(ans);
}

EXTERN EMSCRIPTEN_KEEPALIVE char *getInstruction() {
  char *tmp = "Enter Expression here";
  int size = strlen(tmp) + 1;
  char *inst = malloc(sizeof(char) * size);
  for (int i = 0; i < size; i++) {
    inst[i] = tmp[i];
  }
  return inst;
}

EXTERN EMSCRIPTEN_KEEPALIVE char *getHint() {
  char *tmp = "2 + 3(2)^2";
  int size = strlen(tmp) + 1;
  char *hint = malloc(sizeof(char) * size);
  for (int i = 0; i < size; i++)
    hint[i] = tmp[i];
  return hint;
}

int main(void) {
  /*char *eqn = "3(2)5";
  char *eq = malloc(sizeof(char) * 11);
  for (int i = 0; i <= 10; i++)
  eq[i] = eqn[i];*/
  printf("Basic Calculator is Ready\n");
  //printf("solved  %s\n", solve(eq));
  return 0;
}

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <emscripten/emscripten.h>

#define EXTERN

char *res;

EXTERN EMSCRIPTEN_KEEPALIVE char *solve(char *eqn) {
  char* msg = "qudraticing ";
  int len = strlen(msg) + strlen(eqn);
  res = malloc(sizeof(char) * (len + 1));
  strcat(res, msg);
  strcat(res, eqn);
  return res;
}

int main() {
  printf("quadratic solver is ready\n");
}

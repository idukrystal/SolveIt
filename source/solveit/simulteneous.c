#include "basics.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void print(double **matrix, int size) {
  for (int i = 0; i < size; i++) {
    for(int j = 0; j < size + 1; j++) {
      printf("  %lf", matrix[i][j]);
    }
    putchar('\n');
  }
  putchar('\n');
}

void calculate(double **matrix, int size) {
  double div, mul;
  printf(" calculate start %d\n", size);
  print(matrix, size);
  for (int i = 0; i < size; i++) {
    div = matrix[i][i];
    for(int j = 0; j < (size  + 1); j++) {
      matrix[i][j] /= div;
    }
    print(matrix, size);
    for (int j = 0; j < size; j++) {
      if (j == i)
        continue;
      mul = matrix[j][i];
      for (int k = 0; k < (size + 1); k++) {
        printf("before %lf %lf\n", matrix[j][k], mul);
        matrix[j][k] = matrix[j][k] - (mul*(matrix[i][k]));
      }
      print(matrix, size);
    }
  }
  print(matrix, size);
}

void addEqn(char *str, char **sys, int from, int to, int ind) {
  printf("add %d  %d %d\n", from, to, ind);
  char *eqn = malloc(sizeof(char) * (to - from + 1));
  int j = 0;
  if (eqn == NULL) {
    exit(3);
  }
  for (int i = from; i < to; i++) {
    eqn[j] = str[i];
    j++;
  }
  eqn[j] = '\0';
  printf("%s\n", eqn);
  *sys  = eqn;
}

int countLines(char *eqn) {
  unsigned char c;
  int count = 0, len = strlen(eqn);
  for(int i = 0; i < len; i++) {
    c = eqn[i];
    if (c == '\n')
      count++;
  }
  return count + 1;
}
char ** toSystem(char *eqnStr, int *unknowns) {
  int len = strlen(eqnStr), start = 0, index = 0;
  int count = countLines(eqnStr);
  char **system = malloc(sizeof(char*) * count);
  unsigned char c;
  for (int i = 0; i <=  len; i++) {
    c = eqnStr[i];
    if (c == '\n' || c == '\0') {
      addEqn(eqnStr, (system+index), start, i, index);
      index++;
      start = i + 1;
    }
  }
  *unknowns = index;
  return system;
}

double getConst(char *eqn, int start, int stop, char *var){
  unsigned char c;
  int sign = (eqn[start] == '-') ? -1 : 1;
  for (int i = start; i < stop; i++) {
    c = eqn[i];
    if (c >= '0' && c <= '9') {
      start = i;
      while (((c >= '0' && c <= '9') || c == '.') && i < stop && c != '=') {
        i++;
        c = eqn[i];
      }
      return convertToDouble(eqn, start, i) * sign;
    }
  }
  return (var) ?  1.0 * sign: 0;
}

char * getStringAt(char *eqn, int from, int to) {
  char *str = malloc(sizeof(char) * (to - from + 1));
  int j = 0;
  
  if (str == NULL)
    _perror("Memory Issues\n");

  
  for (int i = from; i < to; i++) {
    printf("£££ %d %d\n", i, j);
    str[j] = eqn[i];
    j++;
  }
  str[j] = '\0';
  return str;
}

char * getVar(char *eqn, int start, int stop) {
  unsigned char c;

  if (eqn[start] == '+' || eqn[start] == '-' || eqn[start] == '=')
    start += 1;
  for (int i = start; i < stop; i++) {
    c = eqn[i];
    if (!(c >= '0' && c<= '9') && !(c == '.')) {
      start = i;
      while (!(c >= '0' && c <= '9')  && (i < stop)) {
        i++;
        c = eqn[i];
      }
      return getStringAt(eqn, start, i);
    }
  }
  return NULL;
}

int addOrGet(char **vars, char *var, int unknowns) {
  int i = 0;
  char *cur;
  cur = vars[i];
  printf(" add/get %s unknowns %d i %d cur %s\n", var, unknowns, i, cur);
  while (cur != NULL) {
    if (strcmp(var, cur) == 0) {
      return i;
    }
    i++;
    if (i >=  unknowns)
      _perror("extra var dectected -> %s\n", var);
    cur = vars[i];
    printf(" add/get %s unknowns %d i %d cur %s\n", var, unknowns, i, cur);
  }
  printf(" i is %d\n", i);
  vars[i] = var;
  return i;
}
double *parse(char *eqn, int unknowns, char **vars) {
  printf("parse %s\n", eqn);
  int start = 0, len = strlen(eqn);
  double cons;
  bool rhs = false;
  unsigned char c;
  char *var = NULL;
  double *row = malloc(sizeof(double) * (unknowns + 1));

  int index = 0;
  for (int i = 0; i < (unknowns + 1); i++)
    row[i] = 0;
  for (int i = 1; i <=  len; i++) {
    c = eqn[i];
    printf(" parseing i %d  c %c eqn %s\n", i, c, eqn);
    if (c == '+' || c == '-' || c == '\0' || c == '=') {
      printf(" parse before getVar  i %d, var %s\n", i, var);
      var = getVar(eqn, start, i);
      printf(" parse before getConst i %d, var %s\n", i, var);
      cons =  getConst(eqn, start, i, var);
      printf(" parse i %d, var %s\n", i, var);
      if (var != NULL) {
	if (rhs)
	  cons *= -1;
	row[addOrGet(vars,var, unknowns)] += cons;
        printf("**** %s   %lf \n",var,  cons);
      }
      else {
        printf("////a   %d  %c %lf %lf\n", i, eqn[start], cons, row[unknowns]);
	if (rhs) {
	  row[unknowns] += cons;
	  printf("////b   %d  %c %lf %lf\n", i, eqn[start], cons, row[unknowns]);
	}
	else {
	  row[unknowns] += ((cons) * -1);
	  printf("////c   %d  %c %lf %lf\n", i, eqn[start], cons, row[unknowns]);
	}
	printf("////d   %d  %c %lf %lf\n", i, eqn[start], cons, row[unknowns]);
      }
      start = i;
      if (c == '=')
	rhs = true;
    }
  }
  return row;
  printf(" ###     %lf  >> \n", cons);
}

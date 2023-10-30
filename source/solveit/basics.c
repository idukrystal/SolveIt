#ifndef ERROR
#include "error.h"
#define ERROR
#endif

#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "basics.h"
#include <stdarg.h>

#define loopMul(brac, param, prev, less, rep)	   \
  len = strlen(eqn);					   \
  ulen = utf8Len(eqn);					   \
  for (param) {                                    \
    c = getUtf8at(eqn, i, 0);					     \
    printf(" error %d %d %d %d\n", i, j, c, len);		      \
    if ( c == brac ) {                                                \
      printf("found %c  at %d\n", c, i);                             \
      if (contains(operators,					     \
		   getUtf8at(eqn, prev 1, 0),			     \
		   noOfOperators)) {				     \
	mul = false;                                                 \
      }								     \
      if (getUtf8at(eqn, prev 1, 0)  == brac) {			     \
        mul = false;                                                 \
      }/*                                                              \
      if (less 1){						     \
	printf("prev 2      %d\n", prev 2);			     \
        if (contains(                                                \
                     operators,                                      \
                     getUtf8at(eqn, prev 2, 0),                       \
                     noOfOperators                                   \
                    )                                                \
        ) {                                                          \
          mul = false;                                               \
        }                                                            \
	}  							      \
      if (less 2){                                                    \
        if (getUtf8at(eqn, prev 3, 0) == 8226) {		      \
          mul = false;                                              \
        }                                                            \
	}	*/						     \
      if (mul) {                                                     \
        replaceUtf8(eqnPtr,  i, i + 1, rep, ulen);                       \
        len = strlen(*eqnPtr);                                       \
        mulBrackets(eqnPtr);                                         \
        return *eqnPtr;                                              \
      }                                                              \
      mul = true;                                                    \
    }                                                                \
  }                                                                  \

const int noOfOperators = 8;
const unsigned char operators[] = {'-', '+', '*', '/', 247, 215, 183, '^'};

void _perror(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  putc('\n', stderr);
  error = true;
  errorMsg = malloc(sizeof(char) * strlen(fmt));
  for(int i = 0; i < strlen(fmt); i++)
    errorMsg[i] = fmt[i];
  //exit(EXIT_FAILURE);
}

int utf8Len(char *eqn) {
  int len = 0;
  unsigned char c  = eqn[0];
  for (int i = 0; c != 0; c = eqn[i]) {
    if (c < 128) {
      len++;
      i += 1;
    }
    if (c >= 192 && c <= 223) {
      len++;
      i += 2;
    }
    if (c >= 224) {
      len++;
      i += 3;
    }
  }
  printf("lening %s ++ %d\n", eqn, len);
  return len;
}

int getUtf8at(char *eqn, int index, int start) {
  int curIndex = -1;
  unsigned char c = eqn[0];
  for (int i = 0; c != 0; c = eqn[i]) {
    if (c < 128) {
      curIndex++;
      i += 1;
      if (curIndex == start + index) {
        return c;
      }
    }
    else if (c >= 192 && c <= 223) {
      curIndex++;
      i += 2;
      if (curIndex == start + index) {
        int a = 192 ^ c;
        int b = 128 ^ eqn[i - 1];
        return (a << 6) + b;
      }
    }
    else if (c >= 224 && c <= 239) {
      curIndex++;
      if (curIndex == start + index) {
        int a = 224 ^ c;
        int b = 128 ^ eqn[i + 1];
        int c = 128 ^ eqn[i + 2];
        return (a << 12) + (b << 6) + c;
      }
      i+=3;
    }
    else {
      _perror("utf type not supported\n");
    }
  }
  return -1;
}

bool contains(unsigned char *list, unsigned char val, int len) {

  for (int i = 0; i < len; i++) {
    printf("contain  %d %d\n", val, list[i]);
    if (list[i] == val){
      printf(" found\n");
      return true;
    }
  }
  printf("  not found \n");
  return false;
}

double convertToDouble(char *eqn, int start, int stop) {
  printf(" in convert  %s %d %d\n", eqn, start, stop);
  double res = 0;
  double exp;
  int decStart = stop;
  for (int i = start; i < stop; i++) {
    int c = getUtf8at(eqn, i - start, start);
    if (c == '.') {
      if (decStart != stop) {
        _perror("multiple .\n");
      }
      decStart = i;
    }
  }
  exp = decStart - start -1;

  for (int i = start; i < stop; i++) {
    int c = getUtf8at(eqn, i - start, start);
    if (c == '.') {
      continue;
    }
    if (c >= '0' && c <= '9') {
      res += (c - '0') * pow(10.0, (exp--));
    }
    else {
      _perror("=>utf-8(%d) is not allowed ", c);
    }
  }
  printf(">> %lf\n", res);
  return res;
}

char *replace(char **eqnPtr, int from, int to, char *with, int len){
  char *eqn = *eqnPtr;
  int wlen = strlen(with);
  char *newEqn = malloc(sizeof(char) * (len - (to - from -  1 - wlen)));
  if (newEqn == NULL) {
    free(eqn);
    _perror("no more memory\n");
  }
  for (int i = 0, j = 0; i < len; i++, j++) {
    if (i == from) {
      for (int k = 0; k < wlen; k++, j++) {
        newEqn[j] = with[k];
      }
      i = to - 1;
      j--;
      printf(" %d  %d %s  >>\n", i, j, newEqn);
      continue;
    }
    newEqn[j] = eqn[i];
    printf(" %d  %d %s  >>\n", i, j, newEqn);
  }
  newEqn[len - (to - from - 1 - wlen) - 1] = '\0';
  free(eqn);
  *eqnPtr = newEqn;
  return *eqnPtr;
}


char *replaceUtf8(char **eqnPtr, int from, int to, char *with, int ulen) {
  int i = 0, j = 0, start = 0, stop = 0;
  unsigned char c;
  printf("repl8  %d %d %d\n", from, to, ulen);
  while(i < ulen) {
    c = getUtf8at(*eqnPtr, i, 0);
    if (i == from) {
      start = j;
    }
    if (i == to) {
      stop = j;
      break;
    }
    if (c < 128)
      j += 1;
    if (c >= 128)
      j += 2;
    i++;
  }
  printf("repl8  %d %d %d\n", start, stop, ulen);
  return replace(eqnPtr, start, stop, with, strlen(*eqnPtr));
}

char *mulBrackets(char **eqnPtr) {
  char *eqn = *eqnPtr;
  unsigned char c;
  int prev, len , ulen, j;
  bool mul = true;

  loopMul('(', int i = 1; i < ulen; i++, i-, i>, "·(")
    loopMul(')', int i = ulen-2; i >= 0; i--, i+1+0*,(len-(i+1))>, ")·")
  return *eqnPtr;
}

char *standadize(char **eqnPtr) {

  int sign = 1, len = strlen(*eqnPtr), j, c;
  char *eqn = *eqnPtr;
    for (int i = 0; i < len; i++) {
      c = eqn[i];;
      if (c == '-' || c == '+') {
        j = i;
        while (c == '-' || c == '+') {
          if (c == '-')
            sign *= -1;
          j++ ;
          c = eqn[j];
        }
        if (j != (i + 1)) {
          if (sign == -1)
            replace(eqnPtr, i, j, "-", len);
          else
            replace(eqnPtr, i, j, "+", len);
          standadize(eqnPtr);
          return *eqnPtr;
        }
        sign = 1;
        i = --j;
      }
    }
    return *eqnPtr;
}

char *normalize(char **eqnPtr, int ulen) {
  char *eqn = *eqnPtr;
  char c, next;
  int len = strlen(eqn);
  if (eqn[0] == '/' || eqn[0] == '*')
    _perror("stray utf(%d) \n", eqn[0]);
  if (eqn[len - 1] == '/' || eqn[len - 1] == '*')
    _perror("stray utf(%d) \n", eqn[0]);
  if (
          getUtf8at(eqn, 0, 0) == 247 ||
          getUtf8at(eqn, 0, 0) == 215
          )
    _perror("stray utf(%d) \n", getUtf8at(eqn, 0, 0));
  if (
      getUtf8at(eqn, 0, ulen - 1) == 247 ||
      getUtf8at(eqn, 0, ulen -1) == 215
      )
    _perror("stray utf(%d) \n", getUtf8at(eqn, ulen - 1, 0));
  if (eqn[0] == '-') {
    printf(" ...... 1 start\n");
    char *newEqn = malloc(sizeof(char) * (len + 4));
    bool closed = false;
    if (newEqn == NULL)
      _perror("memory error \n");                                                                             newEqn[0] = '(';
    newEqn[1] = eqn[0];
    newEqn[len + 4] = '\0';
    for (int i = 1, j = 2; i <= len; i++, j++) {
      c = eqn[i];
      if (!(isalnum(c) || c  == '.' || closed)) {
        printf(" na %c\n", c);
        newEqn[j] = ')';
        closed = true;
        j ++ ;
      }
      newEqn[j] = eqn[i];
      printf(" ...... %s fill\n", newEqn);
    }
    printf(" %s   %s\n", eqn, newEqn);
    free(eqn);
    *eqnPtr = newEqn;
    printf(" %s   %s\n", eqn, newEqn);
  }
  return eqn;
}

char *removeSpaces(char **eqnPtr) {
  int len = strlen(*eqnPtr);
  int spaces = 0;
  for (int i = 0; i < len; i++) {
    if ((*eqnPtr)[i] == ' ') {
      spaces++;
    }
  }
  if (spaces > 0) {
    char* newEqn = malloc(sizeof(char) * (len - spaces +1) );
    if (newEqn == NULL) {
      _perror("Can't get enogh Memory \n");
    }

    int j = 0;
    for (int i = 0; i <  len; i++) {
      if ((*eqnPtr)[i] != ' ') {
        newEqn[j] = (*eqnPtr)[i];
        j++;
      }
    }
    newEqn[j] = '\0';
    free(*eqnPtr);
    *eqnPtr = newEqn;
  }
  return *eqnPtr;
}

char *toString(double ans) {
  char *res = malloc(sizeof(char) * 50);
  sprintf(res, "%f", ans);
  return res;
}

char *substitute(char **eqn, char *old, char *new) {
  int len = strlen(*eqn), len2 = strlen(old), len3 = strlen(new), start;
  for (int i = 0, j = 0; i < len; i++) {
    printf(" substitute i %d  %c %d\n", i, (*eqn)[i], (*eqn)[i]);
    start = i;
    while ((*eqn)[i] == old[j]) {
      printf(" substiyute i %d %c j %d  %c\n", i , (*eqn)[i], j, old[j]);
      j++;
      i++;
    }
    if (j == len2) {
      replace(eqn, start, i, new, len);
      i = (i - (len2 - len3)) - 1;
      j = 0;
      len = strlen(*eqn);
    }
  }
  return *eqn;
}

char *fixUtf8(char **eqn) {
  int len = strlen(*eqn);
  for (int i = 0; i < len; i++) {
    printf("fix i %d  %d\n", i, (*eqn)[i]);
    if ((*eqn)[i] > 127 || (*eqn)[i] < 0) {
      replace(eqn, i, i+1, " ", len);
      return fixUtf8(eqn);
    }
  }
  return *eqn;
}

/**
int main(void) {
  char *input = "14 ÷ 3 × 2";
  char *eqn = malloc(sizeof(char)*(strlen(input) +1));
  for (int i = 0; i < strlen(input); i++)
    eqn[i] = input[i];
  printf(" before substitute %s\n", eqn);
  fixUtf8(&eqn);
  printf(" after substitute %s\n", eqn);
}

**/

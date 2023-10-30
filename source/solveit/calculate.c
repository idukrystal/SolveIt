#include "calculate.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define loop(opr, len, soln)				 \
for (int i = start; i < stop; i++) {                     \
  unsigned char  c = getUtf8at(eqn, i - start, start);   \
  oprs = opr;						 \
  printf("----  %d\n", c);				 \
  if (c == '(') {                                        \
    bDepth++;                                            \
    inBracket = true;                                    \
  }                                                      \
  if (c == ')') {                                        \
    bDepth--;                                            \
    if (bDepth == 0){                                    \
      inBracket = false;                                 \
    }                                                    \
    if (bDepth < 0) {                                    \
      _perror("dangling )");                             \
    }                                                    \
  }							 \
  if (contains(oprs, c, len) && !inBracket) {		 \
    last = i;                                            \
    found = true;                                        \
  }                                                      \
 }							 \
if (inBracket)                                           \
  _perror("cant find ')' \n");                           \
if (found) {                                             \
  found = false;                                         \
  return soln;						 \
 }                                                       \


double calculate(char *eqn, int start, int stop) {
  bool inBracket = false;
  int bDepth = 0;
  int len = strlen(eqn);
  int last = 0;
  bool found = false;
  unsigned char *oprs;
  printf (">>>   calc %s >> %d  >> %d\n", eqn, start, stop);
  unsigned char ops1[] = {'+', '-'};
  loop(ops1, 2,
       (eqn[last] == '+') ? calculate(eqn, start, last) + calculate(eqn, last + 1, stop):
  calculate(eqn, start, last) - calculate(eqn, last + 1, stop);
       );
  //loop('+', calculate(eqn, start, last) + calculate(eqn, last + 1, stop));

  unsigned char ops2[] = {215, '*'};
  loop(ops2, 2,
       calculate(eqn, start, last) * calculate(eqn, last + 1, stop)
  );

 
  //loop({'*'}, calculate(eqn, start, last) * calculate(eqn, last + 1, stop));

  unsigned char ops3[] = {247};
  loop(ops3, 1,
       calculate(eqn, start, last) / calculate(eqn, last + 1, stop)
       );

  unsigned char ops4[] = {'/'};
  loop(ops4, 1,
       calculate(eqn, start, last) / calculate(eqn, last + 1, stop)
       );
  //loop({'/'}, calculate(eqn, start, last) / calculate(eqn, last + 1, stop));

  unsigned char ops5[] = {183};
  loop(ops5, 1,
       calculate(eqn, start, last) * calculate(eqn, last + 1, stop)
       );
  unsigned char ops6[] = {'^'};
  loop(ops6, 1,
       pow(calculate(eqn, start, last), calculate(eqn, last + 1, stop))
       );

  if (getUtf8at(eqn, 0, start) == '(')
    return calculate(eqn, start + 1, stop - 1);

  return convertToDouble(eqn, start, stop);
}

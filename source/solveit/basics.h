#include "types.h"

void _perror(const char *fmt, ...);

int utf8Len(char *eqn);

int getUtf8at(char *eqn, int index, int start);

bool contains(unsigned char *, unsigned char val, int len);

double convertToDouble(char *eqn, int start, int stop);

char *replace(char **eqnPtr, int from, int to, char *with, int len);

char *mulBrackets(char **eqnPtr);

char *standadize(char **eqnPtr);

char *normalize(char **eqnPtr, int ulen);

char *removeSpaces(char **eqnPtr);

char *toString(double ans);

char *substitute(char **eqn, char *old, char *new);

char *fixUtf8(char **eqn);



void print(double **matrix, int);

void calculate(double **matrix, int);

void addEqn(char *str, char **sys, int from, int to, int ind);

char ** toSystem(char *eqnStr, int *unknowns);

double getConst(char *eqn, int start, int stop, char *);

char * getVar(char *eqn, int start, int stop);

int addOrGet(char **vars, char *var, int unknowns);

double *parse(char *eqn, int unknowns, char **vars);

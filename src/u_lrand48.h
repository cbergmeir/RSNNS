#ifndef U_LRAND_HEADER
#define U_LRAND_HEADER

long u_lrand48(void);
void u_srand48(long seedval);
double u_drand48(void);


void u_setCurrentSeedVal(long seedval);
long u_getCurrentSeedVal();

#endif

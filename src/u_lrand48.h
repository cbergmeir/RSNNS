#ifndef U_LRAND_HEADER
#define U_LRAND_HEADER

long snns_lrand48(void);
void snns_srand48(long seedval);
double snns_drand48(void);


void u_setCurrentSeedVal(long seedval);
long u_getCurrentSeedVal();

#endif

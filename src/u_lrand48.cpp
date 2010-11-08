#include <stdlib.h>

#include "u_lrand48.h"

long u_currentSeedVal = 0;

long snns_lrand48(void)
{
    return (long) rand();
}

void snns_srand48(long seedval)
{
    srand(seedval);
}

double snns_drand48(void)
{
//RAND_MAX 0x7fffffff
    return((double) (rand() & RAND_MAX)
	   / (double) RAND_MAX);
}

void u_setCurrentSeedVal(long seedval)
{
  u_currentSeedVal = seedval;
}

long u_getCurrentSeedVal()
{
  return(u_currentSeedVal);
}


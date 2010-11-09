#include <stdlib.h>

#include "u_lrand48.h"

long u_currentSeedVal = 0;

long u_lrand48(void)
{
    return (long) rand();
}

void u_srand48(long seedval)
{
    srand(seedval);
}

double u_drand48(void)
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


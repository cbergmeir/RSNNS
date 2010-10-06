/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/lrand48.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : lrand48 functions
  NOTES          : 

  AUTHOR         : Michael Vogt
  DATE           : 2.3.98

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.2 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:49 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#if ! defined(HAVE_LRAND48) || HAVE_LRAND48 == 0
#include <stdlib.h>

long lrand48(void)
{
    return (long) rand();
}

void srand48(long seedval)
{
    srand(seedval);
}

double drand48(void)
{
    return((double) (rand() & 0x7fffffff)
	   / (double) 0x7fffffff);
}

#endif /* ! HAVE_LRAND48 */

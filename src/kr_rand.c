/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_rand.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel randomize functions for MS-DOS
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 30.06.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:44 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#ifdef  __MSDOS__

#ifndef  MSDOS_RAND_FUNCS
#define  MSDOS_RAND_FUNCS

#include <stdlib.h>

void srand48(long seedval)
{
        srand((int) seedval);
}

long lrand48(void )
{
        return ((long) rand());
}

double drand48(void)
{
        return ((double) rand() / (double) RAND_MAX);
}

#endif
#endif

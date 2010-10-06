/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_rand.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel randomize functions for MS-DOS
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 30.06.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:05 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifdef  __MSDOS__

#ifndef  MSDOS_RAND_FUNCS
#define  MSDOS_RAND_FUNCS

void srand48(long seedval);
long lrand48(void);
double drand48(void);

#endif
#endif

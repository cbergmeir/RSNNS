/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/lrand48.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : lrand48 functions
  NOTES          : 

  AUTHOR         : Michael Vogt
  DATE           : 2.3.98

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.2 $
  LAST CHANGE    : $Date: 1998/03/03 17:54:29 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#if ! defined(HAVE_LRAND48) || HAVE_LRAND48 == 0
#ifndef _LRAND48_DEFINED_
#define _LRAND48_DEFINED_

extern long lrand48(void);
extern void srand48(long seedval);
extern double drand48(void);

#endif /* _LRAND48_DEFINED_ */
#endif /* ! HAVE_LRAND48 */

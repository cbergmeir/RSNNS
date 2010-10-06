/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/alloca.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : alloca
  NOTES          : 

  AUTHOR         : Michael Vogt
  DATE           : 2.3.98

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.3 $
  LAST CHANGE    : $Date: 1998/03/03 17:54:26 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#if ! defined(HAVE_ALLOCA) || HAVE_ALLOCA == 0
#ifndef _ALLOCA_DEFINED_
#define _ALLOCA_DEFINED_

#include <sys/types.h>
extern char *alloca(size_t size);

#endif /* _ALLOCA_DEFINED_ */
#endif /* ! HAVE_ALLOCA */

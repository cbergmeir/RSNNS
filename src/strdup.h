/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/strdup.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : System V Library Function strdup.
  NOTES          : The strdup function is missing in the ULTRIX-32 operating system
                    environment.

  AUTHOR         : Niels Mache
  DATE           : 30.07.90

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/02 17:57:56 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#if ! defined(HAVE_STRDUP) || HAVE_STRDUP == 0
#ifndef _STRDUP_DEFINED_
#define _STRDUP_DEFINED_

extern char *strdup(const char *str);

#endif /* _STRDUP_DEFINED_ */
#endif /* ! HAVE_STRDUP */

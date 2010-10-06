/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/alloca.c,v $
  SHORTNAME      : alloca.c
  SNNS VERSION   : 4.2

  PURPOSE        : alloca
  NOTES          : 

  AUTHOR         : Michael Vogt
  DATE           : 3.398

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.3 $
  LAST CHANGE    : $Date: 1998/03/03 15:59:55 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#if ! defined(HAVE_ALLOCA) || HAVE_ALLOCA == 0
#include <stdlib.h>
#include <stdio.h>

/* this is only a poor implementaion of alloca, which never gives
   memory back to the system */

char *alloca(size_t size)
{
    char *p;

    p = (char *) malloc(size);
    if (!p)
    {
	fprintf(stderr, "%s line %d: can't get memory for alloca\n",
		__FILE__, __LINE__);
	exit(1);
    }
    return p;
}

#endif /* ! HAVE_ALLOCA */

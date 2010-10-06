/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/strdup.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : strdup.
  NOTES          : 

  AUTHOR         : Niels Mache
  DATE           : 30.07.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/03/03 15:59:58 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#if ! defined(HAVE_STRDUP) || HAVE_STRDUP == 0
#include <string.h>
#include <stdlib.h>

char *strdup(const char *source)
{
    int len;
    char *copy;
    
    len = strlen(source) + 1;
    if ((copy = malloc(len * sizeof(char))))
	strcpy(copy, source);

    return(copy);
}

#endif /* ! HAVE_STRDUP */

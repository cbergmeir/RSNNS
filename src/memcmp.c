/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/memcmp.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : memcmp function
  NOTES          : 

  AUTHOR         : Michael Vogt
  DATE           : 2.3.98

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.3 $
  LAST CHANGE    : $Date: 1998/03/03 17:54:30 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
    unsigned const char *p1 = s1;
    unsigned const char *p2 = s2;

    while (n>0 && *p1 == *p2)
    {
	p1++;
	p2++;
	n--;
    }

    return (n>0) ? ((*p1 < *p2) ? -1 : 1) : 0;
}

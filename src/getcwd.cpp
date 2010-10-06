/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/getcwd.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : getcwd
  NOTES          : 

  AUTHOR         : Guenter Mamier
  DATE           : 2.3.98

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.3 $
  LAST CHANGE    : $Date: 1998/03/03 15:59:56 $

    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#if ! defined(HAVE_GETCWD) || HAVE_GETCWD == 0
#include <stdlib.h>

/* unfortunately there is no easy way to locate the current directory
   if getcwd is not present. Even getwd will not work since getcwd
   seems to be more standardized than getwd. So we better set the
   current pathname to "" instead of trying tricks like 'pwd' */

#define _DEFAULT_CWD_NAME ""

char *getcwd(char *buf, size_t size)
{
    if (buf == NULL)
	buf = malloc(size);

    if (buf != NULL)
	strncpy(buf, _DEFAULT_CWD_NAME, size);

    return buf;
}

#undef _DEFAULT_CWD_NAME

#endif /* ! HAVE_GETCWD */

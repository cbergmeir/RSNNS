/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/getcwd.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : getcwd
  NOTES          : 

  AUTHOR         : Guenter Mamier
  DATE           : 2.3.98

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.3 $
  LAST CHANGE    : $Date: 1998/03/03 17:54:27 $

    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#if ! defined(HAVE_GETCWD) || HAVE_GETCWD == 0
#ifndef _GETCWD_DEFINED_
#define _GETCWD_DEFINED_

#include <sys/types.h>
extern char *getcwd(char *buf, size_t size);

#endif /* _GETCWD_DEFINED_ */
#endif /* ! HAVE_GETCWD */

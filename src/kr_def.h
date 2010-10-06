/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_def.h,v $
  SHORTNAME      : def.h
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Defaults
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 01.03.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef KR_DEFAULTS

#define KR_DEFAULTS

#ifndef NULL
#include <stdio.h>	/*  include "stdio.h" if the NULL-pointer isn't
			    defined  */
#endif


/*  default constants for block memory allocation in # of components
*/
#ifdef __BORLANDC__

#define  UNIT_BLOCK     100
#define  LINK_BLOCK     500
#define  SITE_BLOCK     20
#define  NTABLE_BLOCK   50
#define  STABLE_BLOCK   20
#define  PATTERN_BLOCK  50

#else
#ifdef  __ENZO__

/* A population of networks doesn't allow to use this much memory ... */

#define  UNIT_BLOCK     300
#define  LINK_BLOCK     1000
#define  SITE_BLOCK     20
#define  NTABLE_BLOCK   50
#define  STABLE_BLOCK   20
#define  PATTERN_BLOCK  200

#else

#define  UNIT_BLOCK     1000
#define  LINK_BLOCK     5000
#define  SITE_BLOCK     200
#define  NTABLE_BLOCK   500
#define  STABLE_BLOCK   200
#define  PATTERN_BLOCK  200

#endif /*__ENZO__*/
#endif /*__BORLANDC__*/

/*  Unit/Site Default Initialisations
*/

#define     DEF_OUT         0
#define     DEF_I_ACT       0
#define     DEF_BIAS        0

#define     DEF_SUBNET_NO   0
#define     DEF_LAYER_NO    1
#define     DEF_POS_X       0
#define     DEF_POS_Y       0

#define     DEF_POS_Z       0


#define     DEF_STYPE       UFLAG_TTYP_HIDD


/*#################################################

GROUP: File I/O Constants

#################################################*/

/*  max. line length for the kernel input/output files
*/
#define  MAX_LIN_LEN    250
#define  BEST_LINE_LEN  132



#endif

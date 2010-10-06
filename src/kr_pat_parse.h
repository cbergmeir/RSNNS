/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_pat_parse.h,v $
  SHORTNAME      : pat_parse
  SNNS VERSION   : 4.2

  PURPOSE        : parser for new pattern format; bison format 
  NOTES          : impossible to use with yacc

  AUTHOR         : Michael Vogt
  DATE           : 10.9.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:59 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _KR_PAT_PARSE_DEFINED_
#define _KR_PAT_PARSE_DEFINED_

/* begin global definition section */

#define CURRENT_VERSION_V       4  /* current version of pattern file        */
#define CURRENT_VERSION_R       2  /* format                                 */

extern int parse_pattern_file(int *set, np_pattern_set_info *this_set);


/* end global definition section */


#endif


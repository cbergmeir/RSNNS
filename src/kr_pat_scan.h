/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_pat_scan.h,v $
  SHORTNAME      : pattern scanner
  SNNS VERSION   : 4.2

  PURPOSE        : scanner for pattern files
  NOTES          : preliminary

  AUTHOR         : Michael Vogt
  DATE           : 10.9.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/31 13:08:00 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _KR_PAT_SCAN_DEFINED_
#define _KR_PAT_SCAN_DEFINED_

/* begin global definition section */

extern int pplex(void);
extern void scanner_await_pattern_end(void);
extern void scanner_await_no_class(void);
extern void scanner_await_class(void);
extern void scanner_init_scanner(FILE *in_file);

/* end global definition section */


#endif


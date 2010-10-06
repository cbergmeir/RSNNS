/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_pat_scan.ph,v $
  SHORTNAME      : pattern scanner
  SNNS VERSION   : 4.2

  PURPOSE        : scanner for pattern files
  NOTES          : preliminary

  AUTHOR         : Michael Vogt
  DATE           : 10.9.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/31 13:08:02 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _KR_PAT_SCAN_DEFINED_
#define _KR_PAT_SCAN_DEFINED_

/* begin global definition section */

int pplex(void);
void scanner_await_pattern_end(void);
void scanner_await_no_class(void);
void scanner_await_class(void);
void scanner_init_scanner(FILE *in_file);

/* end global definition section */

/* begin private definition section */

#define yylval pplval

static char *s_dotpos;
static int scanner_pattern_end_flag;
static int scanner_no_class_flag;
static int numbers_as_names;

/* end private definition section */

#endif


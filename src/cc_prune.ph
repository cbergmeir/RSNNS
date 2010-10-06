/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_prune.ph,v $
  SHORTNAME      : cc_prune.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          : 

  AUTHOR         : Juergen Gatter
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : 

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _CC_PRUNE_DEFINED_
#define  _CC_PRUNE_DEFINED_
/* begin global definition section */
float cc_getPruningError
      (int pruneFunc,int StartPattern,int EndPattern,int mode);

void cc_pruneNet (int StartPattern, int EndPattern, int pruneFunc);

void cc_remove_Unit(void);

/* end   global definition section */

/* begin privat definition section */
float cc_calculatePruneError(int prune_func,int p,int n,float sse);

void cc_killLink (int source, int target);

/* end privat definition section */

#endif /* _CC_PRUNE_DEFINED_ */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/prun_f.h,v $
  SHORTNAME      : prun_f.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file for pruning algorithms
  NOTES          :

  AUTHOR         : Tobias Schreiner
  DATE           : 13.3.94

  CHANGED BY     : Jochen Biedermann, 07.07.94
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _PRUN_F_DEFINED_
#define  _PRUN_F_DEFINED_

#define PR_ALL_PATTERNS -1

extern int pr_trainCycles, pr_retrainCycles, pr_recreate, pr_refresh, 
           pr_inputPruning, pr_hiddenPruning;

extern float pr_maxErrorInc, pr_acceptedError, pr_minError,
             pr_obs_initParameter;

extern void pr_checkDeadUnits (void);

extern krui_err pr_calcMeanDeviation (int pattern, float *sum_error);

extern krui_err PRUNE_Mag (int pattern);

extern krui_err PRUNE_OBD (int pattern);

extern krui_err PRUNE_OBS (int pattern);

extern krui_err PRUNE_Skeletonization (int pattern);

extern krui_err PRUNE_Noncontributing (int pattern);

extern void pr_obs_setInitParameter (float param);

extern krui_err pr_callPrunFunc (int pattern);

extern void pr_setInputPruning (int value);

extern void pr_setHiddenPruning (int value);

#endif /* _PRUN_F_DEFINED_ */

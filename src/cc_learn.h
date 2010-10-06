/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_learn.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Michael Schmalzl
  DATE           : 5.2.92

  CHANGED BY     : Michael Schmalzl
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:25:57 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _CC_LEARN_DEFINED_
#define  _CC_LEARN_DEFINED_

/* begin global definition section */
extern krui_err LEARN_CC(int StartPattern, int EndPattern,
                      float *ParameterInArray, int NoOfInParams,
                      float **ParameterOutArray, int *NoOfOutParams);
extern krui_err LEARN_CasCor(int StartPattern, int EndPattern,
                      float *ParameterInArray, int NoOfInParams,
                      float **ParameterOutArray, int *NoOfOutParams);
extern krui_err TEST_CasCor(int start_pattern, int end_pattern,
			float *parameterInArray, int NoOfInParams,
			float **parameterOutArray, int *NoOfOutParams);

/* end   global definition section */

/* begin privat definition section */
/* end privat definition section */

#endif /* _CC_LEARN_DEFINED_ */

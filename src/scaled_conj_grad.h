/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/scaled_conj_grad.h,v $
  SHORTNAME      : scaled_conj_grad.h
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Learning Functions
  NOTES          : with following learning functions:
	           - scaled conjugated gradient

  AUTHOR         : Bruno Orsier
  DATE           : 01.10.96

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:38 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _SCALED_CONJ_GRAD_DEFINED_
#define _SCALED_CONJ_GRAD_DEFINED_

extern krui_err LEARN_SCG(int start_pattern, int end_pattern, 
			  float *parameterInArray, int NoOfInParams, 
			  float **parameterOutArray, int *NoOfOutParams) ;

extern krui_err TEST_SCG(int start_pattern, int end_pattern,
			 float *parameterInArray, int NoOfInParams,
			 float **parameterOutArray, int *NoOfOutParams);
#endif

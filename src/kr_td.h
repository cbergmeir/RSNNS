/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_td.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Time delay learning and propagation functions
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 20.02.93

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:08 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _KR_TD_DEFINED_
#define  _KR_TD_DEFINED_

extern void  propagateTDNetForward(int pattern_no, int sub_pat_no);

extern krui_err LEARN_TDbackprop(int start_pattern, int end_pattern, 
				 float parameterInArray[], int NoOfInParams,
				 float * *parameterOutArray, 
				 int *NoOfOutParams );

extern krui_err LEARN_TDBP_McClelland(int start_pattern, int end_pattern, 
				      float parameterInArray[], 
				      int NoOfInParams,
				      float * *parameterOutArray, 
				      int *NoOfOutParams);

extern krui_err TEST_TDbackprop(int start_pattern, int end_pattern, 
				 float parameterInArray[], int NoOfInParams,
				 float * *parameterOutArray, 
				 int *NoOfOutParams );
 
#endif 

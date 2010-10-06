/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/stochastic_learn_f.h,v $
  SHORTNAME      : stochastic_learn_f.h
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Learning Functions
  NOTES          : with following learning functions: 
                   - Monte-Carlo
                   - Simulated Annealing

  AUTHOR         : Joachim Danz
  DATE           : 27.04.94

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 1.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:41 $

             Copyright (c) 1994-95 Joachim Danz, TH Darmstadt, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG


******************************************************************************/
#ifndef _STOCHASTIC_LEARN_F_DEFINED_
#define _STOCHASTIC_LEARN_F_DEFINED_

extern krui_err LEARN_MonteCarlo(int start_pattern, int end_pattern, 
				 float *parameterInArray, int NoOfInParams, 
				 float **parameterOutArray, int *NoOfOutParams);
extern krui_err TEST_MonteCarlo(int start_pattern, int end_pattern, 
				float *parameterInArray, int NoOfInParams, 
				float **parameterOutArray, int *NoOfOutParams);
extern krui_err LEARN_SimulatedAnnealingSS(int start_pattern, int end_pattern, 
					   float *parameterInArray, 
					   int NoOfInParams, 
					   float **parameterOutArray, 
					   int *NoOfOutParams);
extern krui_err LEARN_SimulatedAnnealingWTA(int start_pattern, int end_pattern,
					    float *parameterInArray, 
					    int NoOfInParams, 
					    float **parameterOutArray, 
					    int *NoOfOutParams);
extern krui_err LEARN_SimulatedAnnealingWWTA(int start_pattern, int end_pattern,
					     float *parameterInArray, 
					     int NoOfInParams, 
					     float **parameterOutArray, 
					     int *NoOfOutParams);
extern krui_err TEST_SimulatedAnnealingSS(int start_pattern, int end_pattern, 
					  float *parameterInArray, 
					  int NoOfInParams, 
					  float **parameterOutArray, 
					  int *NoOfOutParams);
extern krui_err TEST_SimulatedAnnealingWTA(int start_pattern, int end_pattern, 
					   float *parameterInArray, 
					   int NoOfInParams, 
					   float **parameterOutArray, 
					   int *NoOfOutParams);
extern krui_err TEST_SimulatedAnnealingWWTA(int start_pattern, int end_pattern,
					    float *parameterInArray, 
					    int NoOfInParams, 
					    float **parameterOutArray, 
					    int *NoOfOutParams);

#endif 

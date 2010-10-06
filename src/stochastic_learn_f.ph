/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/stochastic_learn_f.ph,v $
  SHORTNAME      : stochastic_learn_f.ph
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Learning Functions
  NOTES          : with following learning functions: 
                   - Monte-Carlo
                   - Simulated Annealing
	           
  AUTHOR         : Joachim Danz
  DATE           : 27.04.94

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 1.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:42 $

             Copyright (c) 1994-95 Joachim Danz, TH Darmstadt, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _STOCHASTIC_LEARN_F_DEFINED_
#define _STOCHASTIC_LEARN_F_DEFINED_

/* begin global definition section */
krui_err LEARN_MonteCarlo(int start_pattern, int end_pattern, 
			  float *parameterInArray, int NoOfInParams, 
			  float **parameterOutArray, int *NoOfOutParams);
krui_err TEST_MonteCarlo(int start_pattern, int end_pattern, 
			 float *parameterInArray, int NoOfInParams, 
			 float **parameterOutArray, int *NoOfOutParams);
krui_err LEARN_SimulatedAnnealing(int start_pattern, int end_pattern, 
				  float *parameterInArray, int NoOfInParams, 
				  float **parameterOutArray, 
				  int *NoOfOutParams , int errorFunction);
krui_err LEARN_SimulatedAnnealingSS(int start_pattern, int end_pattern, 
				    float *parameterInArray, int NoOfInParams, 
				    float **parameterOutArray, 
				    int *NoOfOutParams);
krui_err LEARN_SimulatedAnnealingWTA(int start_pattern, int end_pattern, 
				     float *parameterInArray, int NoOfInParams,
				     float **parameterOutArray, 
				     int *NoOfOutParams);
krui_err LEARN_SimulatedAnnealingWWTA(int start_pattern, int end_pattern, 
				      float *parameterInArray, 
				      int NoOfInParams, 
				      float **parameterOutArray, 
				      int *NoOfOutParams);
krui_err TEST_SimulatedAnnealing(int start_pattern, int end_pattern, 
				 float *parameterInArray, int NoOfInParams, 
				 float **parameterOutArray, int *NoOfOutParams,
				 int errorFunction);
krui_err TEST_SimulatedAnnealingSS(int start_pattern, int end_pattern, 
				   float *parameterInArray, int NoOfInParams, 
				   float **parameterOutArray, 
				   int *NoOfOutParams);
krui_err TEST_SimulatedAnnealingWTA(int start_pattern, int end_pattern, 
				    float *parameterInArray, int NoOfInParams, 
				    float **parameterOutArray, 
				    int *NoOfOutParams);
krui_err TEST_SimulatedAnnealingWWTA(int start_pattern, int end_pattern, 
				     float *parameterInArray, int NoOfInParams,
				     float **parameterOutArray, 
				     int *NoOfOutParams);
float calculate_WTA_error (int pattern_no, int sub_pat_n);
float calculate_w_WTA_error (int pattern_no, int sub_pat_n);
float calculate_SS_error (int pattern_no, int sub_pat_n);


/* end global definition section */

/* begin private definition section */
 
#define  NET_ERROR( param )      param[ 0 ]  /* returns the net error */
#define  LEARN_PARAM1( param )   param[ 0 ]  /* contains 1st learning param */
#define  LEARN_PARAM2( param )   param[ 1 ]  /* contains 2nd learning param */
#define  LEARN_PARAM3( param )   param[ 2 ]  /* contains 3rd learning param */
#define  LEARN_PARAM4( param )   param[ 3 ]  /* contains 4th learning param */ 
#define  LEARN_PARAM5( param )   param[ 4 ]  /* contains 5th learning param */
#define  SS_ERROR   1
#define  WTA_ERROR  2
#define  WWTA_ERROR 3
 
#endif 






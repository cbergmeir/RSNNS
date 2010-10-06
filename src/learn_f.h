/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/learn_f.h,v $
  SHORTNAME      : learn_f
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Learning Functions
  NOTES          : with following learning functions:
	           - Backpropagation
	           - Backpropagation with momentum term
                   - Quickprop
	           - Counterpropagation
	           - BackPercolation

  AUTHOR         : Niels Mache
  DATE           : 01.10.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.16 $
  LAST CHANGE    : $Date: 1998/04/08 09:26:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _LEARN_F_DEFINED_
#define  _LEARN_F_DEFINED_

extern krui_err LEARN_HEBB (int start_pattern, int end_pattern,
			    float *parameterInArray, int NoOfInParams, 
			    float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_RM_delta(int start_pattern, int end_pattern,
			       float parameterInArray[], int NoOfInParams,
			       float * *parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_ART1(int start_pattern, int end_pattern,
			   float parameterInArray[], int NoOfInParams,
			   float * *parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_ART2(int start_pattern, int end_pattern,
			   float parameterInArray[], int NoOfInParams,
			   float * *parameterOutArray, int *NoOfOutParams );

extern krui_err LEARN_ARTMAP(int start_pattern, int end_pattern,
			     float parameterInArray[], int NoOfInParams,
			     float * *parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_kohonen(int start_pattern, int end_pattern,
			      float parameterInArray[], int NoOfInParams,
			      float * *parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_backprop(int start_pattern, int end_pattern, 
			       float *parameterInArray, int NoOfInParams, 
			       float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_backpropBatch(int start_pattern, int end_pattern, 
				    float *parameterInArray, int NoOfInParams,
				    float **parameterOutArray, 
				    int *NoOfOutParams);

extern krui_err LEARN_backpropMomentum(int start_pattern, int end_pattern, 
				       float *parameterInArray, 
				       int NoOfInParams, 
				       float **parameterOutArray, 
				       int *NoOfOutParams);

extern krui_err LEARN_backpropWeightDecay (int start_pattern, 
					   int end_pattern, 
					   float *parameterInArray, 
					   int NoOfInParams, 
					   float **parameterOutArray,
					   int *NoOfOutParams); 

extern krui_err  LEARN_backpropChunk(int start_pattern, int end_pattern, 
				     float *parameterInArray, int NoOfInParams,
				     float **parameterOutArray, 
				     int *NoOfOutParams);

extern krui_err  LEARN_backpropJogChunk(int start_pattern, int end_pattern, 
					float *parameterInArray, int NoOfInParams,
					float **parameterOutArray, 
					int *NoOfOutParams);

extern krui_err  LEARN_backpropClassJogChunk(int start_pattern, int end_pattern, 
					     float *parameterInArray, int NoOfInParams,
					     float **parameterOutArray, 
					     int *NoOfOutParams);

extern krui_err LEARN_quickprop(int start_pattern, int end_pattern, 
				float *parameterInArray, int NoOfInParams, 
				float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_CPN(int start_pattern, int end_pattern, 
			  float *parameterInArray, int NoOfInParams, 
			  float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_perc(int start_pattern, int end_pattern, 
			   float *parameterInArray, int NoOfInParams, 
			   float **parameterOutArray, int *NoOfOutParams);

extern krui_err RbfLearnClean(void);

extern krui_err RbfLearnForward(int pattern_no, int sub_pat_no);

extern float RbfLearnAdjustDelta(float para_center, float para_bias, 
				 float para_weight, float para_pain, 
				 float para_momentum, float para_delta_max, 
				 int learn_mask);

extern void RbfLearnAdjustWeights(float para_center, float para_bias, 
				  float para_weight, float para_momentum);

extern krui_err RbfTopoCheck(void);

extern krui_err LEARN_RBF(int start_pattern, int end_pattern, 
			   float *parameterInArray, int NoOfInParams, 
			   float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_RBF_DDA(int start_pattern, int end_pattern, 
		   float  *parameterInArray,  int  NoOfInParams, 
		   float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_rprop(int start_pattern, int end_pattern, 
			    float *parameterInArray, int NoOfInParams, 
			    float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_RpropMAP(int start_pattern, int end_pattern, 
			       float *parameterInArray, int NoOfInParams, 
			       float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_BPTT(int start_pattern, int end_pattern, 
			   float *parameterInArray, int NoOfInParams, 
			   float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_BBPTT(int start_pattern, int end_pattern, 
			    float *parameterInArray, int NoOfInParams, 
			    float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_QPTT(int start_pattern, int end_pattern, 
			   float *parameterInArray, int NoOfInParams, 
			   float **parameterOutArray, int *NoOfOutParams);

extern krui_err LEARN_JE_Backprop(int start_pattern, int end_pattern,
				  float *parameterInArray, int NoOfInParams,
				  float **parameterOutArray,int *NoOfOutParams);

extern krui_err LEARN_JE_BackpropMomentum(int start_pattern, int end_pattern,
					  float *parameterInArray,
					  int NoOfInParams,
					  float **parameterOutArray,
					  int *NoOfOutParams);

extern krui_err LEARN_JE_Quickprop(int start_pattern, int end_pattern,
				   float *parameterInArray, int NoOfInParams,
				   float **parameterOutArray, 
				   int *NoOfOutParams);

extern krui_err LEARN_JE_Rprop(int start_pattern, int end_pattern,
			       float  *parameterInArray, int NoOfInParams,
			       float **parameterOutArray, int *NoOfOutParams);

extern krui_err spanning_tree(void);

extern void propagateNetForward(int pattern_no, int sub_pat_no);

extern float propagateNetBackwardBatch(int pattern_no, int sub_pat_no,
				       float delta_max,
				       Patterns alternative_out_pat, 
				       int alternative_size);

extern krui_err clearDeltas(void);
extern krui_err clearAllDeltas(void);

extern krui_err TEST_backprop(int start_pattern, int end_pattern, 
			       float *parameterInArray, int NoOfInParams, 
			       float **parameterOutArray, int *NoOfOutParams);

extern krui_err TEST_backpropMomentum(int start_pattern, int end_pattern, 
			       float *parameterInArray, int NoOfInParams, 
			       float **parameterOutArray, int *NoOfOutParams);

extern krui_err TEST_JE_Backprop(int start_pattern, int end_pattern,
				  float *parameterInArray, int NoOfInParams,
				  float **parameterOutArray,int *NoOfOutParams);

extern krui_err TEST_JE_BackpropMomentum(int start_pattern, int end_pattern,
				  float *parameterInArray, int NoOfInParams,
				  float **parameterOutArray,int *NoOfOutParams);

extern krui_err TEST_rprop(int start_pattern, int end_pattern, 
			    float *parameterInArray, int NoOfInParams, 
			    float **parameterOutArray, int *NoOfOutParams);

extern krui_err TEST_MAP(int start_pattern, int end_pattern, 
			 float *parameterInArray, int NoOfInParams, 
			 float **parameterOutArray, int *NoOfOutParams);

extern krui_err TEST_JE_Rprop(int start_pattern, int end_pattern,
			       float  *parameterInArray, int NoOfInParams,
			       float **parameterOutArray, int *NoOfOutParams);

extern krui_err TEST_BPTT(int start_pattern, int end_pattern, 
			   float *parameterInArray, int NoOfInParams, 
			   float **parameterOutArray, int *NoOfOutParams);

#endif 



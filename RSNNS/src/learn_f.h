/************************************************************************************

   This file is part of SnnsCLib, a fork of the kernel and parts of the gui of 
   the Stuttgart Neural Network Simulator (SNNS), version 4.3.

   The file's original version is part of SNNS 4.3. It's source code can be found at

   http://www.ra.cs.uni-tuebingen.de/SNNS/

   SNNS 4.3 is under the license LGPL v2. We note that source code files of SNNS 4.3 
   state as version "4.2". Base of this fork is SNNS 4.3 with a reverse-applied 
   python patch (see http://developer.berlios.de/projects/snns-dev/).

   SnnsCLib was developed in 2010 by Christoph Bergmeir under supervision of 
   José M. Benítez, both affiliated to DiCITS Lab, Sci2s group, DECSAI, 
   University of Granada

   Changes done to the original code were performed with the objective to
   port it from C to C++ and to encapsulate all code in one class named SnnsCLib.

   Changes in header files mainly include:
   * removed all static keywords
   * moved initializations of variables to the constructor of SnnsCLib

   Changes in cpp code files mainly include:
   * changed file ending from .c to .cpp
   * removed all SNNS internal includes and only include SnnsCLib   
   * static variables within functions were turned into member variables of SnnsCLib
   * function declarations were changed to method declarations, i.e. "SnnsCLib::.."
     was added
   * calls to the function table are now "C++-style", using the "this"-pointer

   License of SnnsCLib:
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

************************************************************************************/


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/learn_f.ph,v $
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

  CHANGED BY     : Sven Doering, Michael Vogt (Martin Reczko), Guenter Mamier
  RCS VERSION    : $Revision: 2.19 $
  LAST CHANGE    : $Date: 1998/04/08 09:26:46 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _LEARN_F_DEFINED_
#define  _LEARN_F_DEFINED_

/* begin global definition section */
krui_err LEARN_HEBB (int start_pattern, int end_pattern,
                     float *parameterInArray, int NoOfInParams, 
                     float **parameterOutArray, int *NoOfOutParams);

krui_err LEARN_RM_delta (int start_pattern, int end_pattern,
			 float parameterInArray[], int NoOfInParams,
			 float * *parameterOutArray, int *NoOfOutParams	);

krui_err  LEARN_ART1(int start_pattern, int end_pattern,
		     float parameterInArray[], int NoOfInParams,
		     float * *parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_ART2(int start_pattern, int end_pattern, 
		     float parameterInArray[], int NoOfInParams,
		     float * *parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_ARTMAP(int start_pattern, int end_pattern,
		       float parameterInArray[], int NoOfInParams,
		       float * *parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_kohonen(int start_pattern, int end_pattern,
			float parameterInArray[], int NoOfInParams,
			float * *parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_backprop(int start_pattern, int end_pattern, 
			 float *parameterInArray, int NoOfInParams, 
			 float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_backpropBatch(int start_pattern, int end_pattern, 
			      float *parameterInArray, int NoOfInParams, 
			      float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_backpropMomentum(int start_pattern, int end_pattern, 
				 float *parameterInArray, int NoOfInParams, 
				 float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_backpropWeigthDecay(int start_pattern, int end_pattern, 
				    float *parameterInArray, int NoOfInParams, 
				    float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_backpropChunk(int start_pattern, int end_pattern, 
			      float *parameterInArray, int NoOfInParams,
			      float **parameterOutArray, 
			      int *NoOfOutParams);

krui_err  LEARN_backpropJogChunk(int start_pattern, int end_pattern, 
				 float *parameterInArray, int NoOfInParams,
				 float **parameterOutArray, 
				 int *NoOfOutParams);

krui_err  LEARN_backpropClassJogChunk(int start_pattern, int end_pattern, 
				 float *parameterInArray, int NoOfInParams,
				 float **parameterOutArray, 
				 int *NoOfOutParams);

krui_err LEARN_quickprop(int start_pattern, int end_pattern, 
			 float *parameterInArray, int NoOfInParams, 
			 float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_CPN(int start_pattern, int end_pattern, 
		    float *parameterInArray, int NoOfInParams, 
		    float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_perc(int start_pattern, int end_pattern, 
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams);

krui_err  RbfLearnClean(void);

krui_err  RbfLearnForward(int pattern_no, int sub_pat_no);

float RbfLearnAdjustDelta(float para_center, float para_bias, 
			  float para_weight, float para_pain, 
			  float para_momentum, float para_delta_max, 
			  int learn_mask);

void RbfLearnAdjustWeights(float para_center, float para_bias, 
			   float para_weight, float para_momentum);

krui_err RbfTopoCheck(void);

krui_err  LEARN_RBF(int start_pattern, int end_pattern, 
		    float *parameterInArray, int NoOfInParams, 
		    float **parameterOutArray, int *NoOfOutParams);

krui_err LEARN_RBF_DDA(int start_pattern, int end_pattern, 
		   float  *parameterInArray,  int  NoOfInParams, 
		   float **parameterOutArray, int *NoOfOutParams);

krui_err LEARN_rprop(int start_pattern, int end_pattern, 
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams);

krui_err LEARN_RpropMAP(int start_pattern, int end_pattern, 
                     float *parameterInArray, int NoOfInParams, 
                     float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_BPTT(int start_pattern, int end_pattern, 
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_BBPTT(int start_pattern, int end_pattern, 
		      float *parameterInArray, int NoOfInParams, 
		      float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_QPTT(int start_pattern, int end_pattern, 
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_JE_Backprop(int start_pattern, int end_pattern,
			    float *parameterInArray, int NoOfInParams,
			    float **parameterOutArray, int *NoOfOutParams) ;

krui_err LEARN_JE_BackpropMomentum(int start_pattern, int end_pattern,
				   float *parameterInArray, int NoOfInParams,
				   float **parameterOutArray, 
				   int *NoOfOutParams);

krui_err  LEARN_JE_Quickprop(int start_pattern, int end_pattern,
			     float *parameterInArray, int NoOfInParams,
			     float **parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_JE_Rprop(int start_pattern, int end_pattern,
			 float *parameterInArray, int NoOfInParams,
			 float **parameterOutArray, int *NoOfOutParams) ;

krui_err  spanning_tree(void);

void propagateNetForward(int pattern_no, int sub_pat_no);

float propagateNetBackwardBatch(int pattern_no, int sub_pat_no,float delta_max);
float propagateClassNetBackwardBatch(int pattern_no, int sub_pat_no,float delta_max);

float propagateNetBackwardBatch(int pattern_no, int sub_pat_no,
				       float delta_max,
				       Patterns alternative_out_pat, 
				       int alternative_size)
{
return propagateNetBackwardBatch( pattern_no,  sub_pat_no, delta_max);
};


krui_err clearDeltas(void);
krui_err clearAllDeltas(void);

krui_err  TEST_backprop(int start_pattern, int end_pattern, 
			 float *parameterInArray, int NoOfInParams, 
			 float **parameterOutArray, int *NoOfOutParams);

krui_err  TEST_backpropMomentum(int start_pattern, int end_pattern, 
			 float *parameterInArray, int NoOfInParams, 
			 float **parameterOutArray, int *NoOfOutParams);

krui_err TEST_rprop(int start_pattern, int end_pattern, 
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams);

krui_err TEST_MAP(int start_pattern, int end_pattern, 
                     float *parameterInArray, int NoOfInParams, 
                     float **parameterOutArray, int *NoOfOutParams);

krui_err  TEST_JE_Backprop(int start_pattern, int end_pattern,
			    float *parameterInArray, int NoOfInParams,
			    float **parameterOutArray, int *NoOfOutParams) ;

krui_err  TEST_JE_BackpropMomentum(int start_pattern, int end_pattern,
			    float *parameterInArray, int NoOfInParams,
			    float **parameterOutArray, int *NoOfOutParams) ;

krui_err  TEST_JE_Rprop(int start_pattern, int end_pattern,
			 float *parameterInArray, int NoOfInParams,
			 float **parameterOutArray, int *NoOfOutParams) ;

krui_err  TEST_BPTT(int start_pattern, int end_pattern, 
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams);




krui_err LEARN_backpropWeightDecay (int start_pattern, 
					   int end_pattern, 
					   float *parameterInArray, 
					   int NoOfInParams, 
					   float **parameterOutArray,
					   int *NoOfOutParams); 

/* end global definition section */

/* begin private definition section */


	float testNetBackward2(int pattern_no, int sub_pat_no, 
				    float learn_parameter, float delta_max);

 void test_update_je_context_units(int pattern_no, int sub_pat_no) ; 

 float testNetBackwardRprop(int pattern_no, int sub_pat_no);

#define  NET_ERROR( param )      param[ 0 ]  /* returns the net error   */
#define  LEARN_PARAM1( param )   param[ 0 ]  /* contains 1st learning param  */
#define  LEARN_PARAM2( param )   param[ 1 ]  /* contains 2nd learning param  */
#define  LEARN_PARAM3( param )   param[ 2 ]  /* contains 3rd learning param  */
#define  LEARN_PARAM4( param )   param[ 3 ]  /* contains 4th learning param  */
#define  LEARN_PARAM5( param )   param[ 4 ]  /* contains 5th learning param  */

#define  SIGMOID_PRIME_OFFSET    0.1         /* for modified sigmoid function */
#define SQR(x)  (x) * (x)

 int  NoOfLearnedPatterns;

 float Hebb_error(int NoOfTimes);

 void RM_propagate(int pattern_no, int sub_pat_no, float prop_step);

 void RM_learn(float learn_parameter);

 krui_err put_ART1_in_pattern(int pattern_no, int sub_pat_no, 
				    TopoPtrArray topo_inp_ptr);

 krui_err adjust_ART1_weights(double beta, TopoPtrArray  comp_ptr,
				    TopoPtrArray delay_ptr,
				    struct Unit   *winner_ptr);

 krui_err put_ART2_in_pattern(int pattern_no, int sub_pat_no,
				    TopoPtrArray topo_inp_ptr);

 krui_err adjust_ART2_weights(double param_d, TopoPtrArray  p_ptr,
				    struct Unit *winner_ptr);

 krui_err put_ARTMAP_in_pattern(int pattern_no, int sub_pat_no,
				      TopoPtrArray topo_inpa_ptr,
				      TopoPtrArray topo_inpb_ptr);

 krui_err adjust_ARTMAP_weights(double beta_a, double beta_b,
				      TopoPtrArray compa_ptr,
				      TopoPtrArray  compb_ptr,
				      TopoPtrArray  dela_ptr,
				      TopoPtrArray  delb_ptr,
				      TopoPtrArray  map_ptr,
				      struct Unit *winner_ptr_a,
				      struct Unit *winner_ptr_b);

	float propagateNetBackward2(int pattern_no, int sub_pat_no, 
				    float learn_parameter, float delta_max);

 krui_err updateWeights(float eta);
 krui_err updateNormalizedWeights(float eta);

 krui_err initializeBackpropMomentum(void);

	float Backprop_momentum_FSE(int pattern_no, int sub_pat_no, 
				    float learn_parameter, float mu, 
				    float FSE_term, float delta_max);

 float Backprop_weightdecay (int pattern_no, int sub_pat_no, 
				   float learn_parameter, 
				   float wd_gamma, float min_weight,
				   float delta_max);

 krui_err initializeQuickprop(void);

 float  propagateNetBackwardQuickprop(int pattern_no, int sub_pat_no,
					    float delta_max);

 void MODI_quickprop(float learn_parameter, float max_factor, 
			   float decay);

 krui_err  initializeCPN(void);

 void normalize_weight(struct Unit *winner_ptr, float sum);

 void normalize_inputvector(float sum);

 float propagateNet_CPN(int pattern_no, int sub_pat_no,
			      float alpha, float beta, float threshold);

	void  propagateNetForward_perc(int pattern_no, int sub_pat_no);

	float propagateNetBackward_perc(int pattern_no, int sub_pat_no,
					float learn_parameter, float delta_max,
					float *perc_error);

 int computeDevite(float *devit, float *sum_error,
                         float target, float output, int errorType );

 float computeAlpha(void);

 void MODI_rprop(float maxeps, float weight_decay);

 krui_err initializeRprop(float update_val);

 void propagateNetForwardMAP(int pattern_no, int sub_pat_no, 
				   int errorType);

 float propagateNetBackwardRprop(int pattern_no, int sub_pat_no);

 float propagateNetBackwardMAP(int pattern_no, int sub_pat_no, 
				     int errorType);

 float testNetBackwardMAP(int pattern_no, int sub_pat_no, int errorType);

 krui_err BPTT_clear_deltaw(void);

 krui_err initializeBPTT(void);

 void BPTT_propagateNetForward(int pattern_no, int sub_pat_no, int nhist);

 float initOldDeltas(int pattern_no, int sub_pat_no );

 float oneStepBackprop(int backstep, int pattern_no, int sub_pat_no, 
			     int nhist);

 void BPTTadapt(float step_size, float bptt_momentum);

 float BPTT_propagateNetBackward(int pattern_no, int sub_pat_no, 
				       int nhist);

 float propagateNet_kohonen(int pattern_no, int sub_pat_no, float height,
				  float width, int sizehor);

 krui_err initializeKohonenLearning(void);

 void update_je_context_units(int pattern_no, int sub_pat_no,
				    float use_real_value_percent) ; 

 void reset_je_context_units(void) ;

 krui_err check_je_network(void) ;

/* end private definition section */

#endif 

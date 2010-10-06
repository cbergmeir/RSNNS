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

/* end global definition section */

/* begin private definition section */


static	float testNetBackward2(int pattern_no, int sub_pat_no, 
				    float learn_parameter, float delta_max);

static void test_update_je_context_units(int pattern_no, int sub_pat_no) ; 

static float testNetBackwardRprop(int pattern_no, int sub_pat_no);

#define  NET_ERROR( param )      param[ 0 ]  /* returns the net error   */
#define  LEARN_PARAM1( param )   param[ 0 ]  /* contains 1st learning param  */
#define  LEARN_PARAM2( param )   param[ 1 ]  /* contains 2nd learning param  */
#define  LEARN_PARAM3( param )   param[ 2 ]  /* contains 3rd learning param  */
#define  LEARN_PARAM4( param )   param[ 3 ]  /* contains 4th learning param  */
#define  LEARN_PARAM5( param )   param[ 4 ]  /* contains 5th learning param  */

#define  SIGMOID_PRIME_OFFSET    0.1         /* for modified sigmoid function */
#define SQR(x)  (x) * (x)

static int  NoOfLearnedPatterns;

static float Hebb_error(int NoOfTimes);

static void RM_propagate(int pattern_no, int sub_pat_no, float prop_step);

static void RM_learn(float learn_parameter);

static krui_err put_ART1_in_pattern(int pattern_no, int sub_pat_no, 
				    TopoPtrArray topo_inp_ptr);

static krui_err adjust_ART1_weights(double beta, TopoPtrArray  comp_ptr,
				    TopoPtrArray delay_ptr,
				    struct Unit   *winner_ptr);

static krui_err put_ART2_in_pattern(int pattern_no, int sub_pat_no,
				    TopoPtrArray topo_inp_ptr);

static krui_err adjust_ART2_weights(double param_d, TopoPtrArray  p_ptr,
				    struct Unit *winner_ptr);

static krui_err put_ARTMAP_in_pattern(int pattern_no, int sub_pat_no,
				      TopoPtrArray topo_inpa_ptr,
				      TopoPtrArray topo_inpb_ptr);

static krui_err adjust_ARTMAP_weights(double beta_a, double beta_b,
				      TopoPtrArray compa_ptr,
				      TopoPtrArray  compb_ptr,
				      TopoPtrArray  dela_ptr,
				      TopoPtrArray  delb_ptr,
				      TopoPtrArray  map_ptr,
				      struct Unit *winner_ptr_a,
				      struct Unit *winner_ptr_b);

static	float propagateNetBackward2(int pattern_no, int sub_pat_no, 
				    float learn_parameter, float delta_max);

static krui_err updateWeights(float eta);
static krui_err updateNormalizedWeights(float eta);

static krui_err initializeBackpropMomentum(void);

static	float Backprop_momentum_FSE(int pattern_no, int sub_pat_no, 
				    float learn_parameter, float mu, 
				    float FSE_term, float delta_max);

static float Backprop_weightdecay (int pattern_no, int sub_pat_no, 
				   float learn_parameter, 
				   float wd_gamma, float min_weight,
				   float delta_max);

static krui_err initializeQuickprop(void);

static float  propagateNetBackwardQuickprop(int pattern_no, int sub_pat_no,
					    float delta_max);

static void MODI_quickprop(float learn_parameter, float max_factor, 
			   float decay);

static krui_err  initializeCPN(void);

static void normalize_weight(struct Unit *winner_ptr, float sum);

static void normalize_inputvector(float sum);

static float propagateNet_CPN(int pattern_no, int sub_pat_no,
			      float alpha, float beta, float threshold);

static	void  propagateNetForward_perc(int pattern_no, int sub_pat_no);

static	float propagateNetBackward_perc(int pattern_no, int sub_pat_no,
					float learn_parameter, float delta_max,
					float *perc_error);

static int computeDevite(float *devit, float *sum_error,
                         float target, float output, int errorType );

static float computeAlpha(void);

static void MODI_rprop(float maxeps, float weight_decay);

static krui_err initializeRprop(float update_val);

static void propagateNetForwardMAP(int pattern_no, int sub_pat_no, 
				   int errorType);

static float propagateNetBackwardRprop(int pattern_no, int sub_pat_no);

static float propagateNetBackwardMAP(int pattern_no, int sub_pat_no, 
				     int errorType);

static float testNetBackwardMAP(int pattern_no, int sub_pat_no, int errorType);

static krui_err BPTT_clear_deltaw(void);

static krui_err initializeBPTT(void);

static void BPTT_propagateNetForward(int pattern_no, int sub_pat_no, int nhist);

static float initOldDeltas(int pattern_no, int sub_pat_no );

static float oneStepBackprop(int backstep, int pattern_no, int sub_pat_no, 
			     int nhist);

static void BPTTadapt(float step_size, float bptt_momentum);

static float BPTT_propagateNetBackward(int pattern_no, int sub_pat_no, 
				       int nhist);

static float propagateNet_kohonen(int pattern_no, int sub_pat_no, float height,
				  float width, int sizehor);

static krui_err initializeKohonenLearning(void);

static void update_je_context_units(int pattern_no, int sub_pat_no,
				    float use_real_value_percent) ; 

static void reset_je_context_units(void) ;

static krui_err check_je_network(void) ;

/* end private definition section */

#endif 

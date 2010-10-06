/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/scaled_conj_grad.ph,v $
  SHORTNAME      : scaled_conj_grad.ph
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Learning Functions
  NOTES          : with following learning functions:
	           - scaled conjugated gradient

  AUTHOR         : Bruno Orsier
  DATE           : 01.10.96

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.4 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:39 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _SCALED_CONJ_GRAD_DEFINED_
#define _SCALED_CONJ_GRAD_DEFINED_

/* begin global definition section */
krui_err LEARN_SCG(int start_pattern, int end_pattern, 
		   float *parameterInArray, int NoOfInParams, 
		   float **parameterOutArray, int *NoOfOutParams) ;

krui_err TEST_SCG(int start_pattern, int end_pattern,
		  float *parameterInArray, int NoOfInParams,
		  float **parameterOutArray, int *NoOfOutParams);
/* end global definition section */

/* begin private definition section */
static float product_of_xt_by_y(float * x, float * y, int tab_size) ;
static float square_of_norm(float * x, int tab_size) ;
static krui_err compute_gradient(int start_pattern, int end_pattern, 
				 float delta_max, float * error) ;

static int        scg_space_size ;
static FlintType* *scg_gradient ;

#define SCG_FIRST_SIGMA     1E-4	/* should be in ]0 ; 1E-4] */
#define SCG_FIRST_LAMBDA    1E-6        /* should be in ]0 ; 1E-6] */

#define  LEARN_PARAM1( param )   param[ 0 ]  /* contains 1st learning param */
#define  LEARN_PARAM2( param )   param[ 1 ]  /* contains 2nd learning param */
#define  LEARN_PARAM3( param )   param[ 2 ]  /* contains 3rd learning param */
#define  LEARN_PARAM4( param )   param[ 3 ]  /* contains 4th learning param */


#define SCG_TOLERANCE 1E-8		/* for single precision */
				/* should be 1E-16 for double precision */
/* end private definition section */

#endif

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_td.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Time delay learning and propagation functions
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 20.02.93

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:09 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _KR_TD_DEFINED_
#define  _KR_TD_DEFINED_

/* begin global definition section */

void      propagateTDNetForward(int pattern_no, int sub_pat_no);

krui_err  LEARN_TDbackprop(int start_pattern, int end_pattern, 
			   float parameterInArray[], int NoOfInParams,
			   float * *parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_TDBP_McClelland(int start_pattern, int end_pattern, 
				float parameterInArray[], int NoOfInParams,
				float * *parameterOutArray, int *NoOfOutParams);
 
/* end global definition section */


/* begin private definition section */

#define  NET_ERROR( param )    param[ 0 ] /* returns the net error   */
#define  LEARN_PARAM1( param ) param[ 0 ] /* contains the 1st learning param.*/
#define  LEARN_PARAM2( param ) param[ 1 ] /* contains the 2nd learning param.*/
#define  LEARN_PARAM3( param ) param[ 2 ] /* contains the 3rd learning param.*/
#define  LEARN_PARAM4( param ) param[ 3 ] /* contains the 4th learning param.*/
#define  LEARN_PARAM5( param ) param[ 4 ] /* contains the 5th learning param.*/

static krui_err initializeTDBackprop(void);

static float propagateTDNetBackward(int pattern_no, int sub_pat_no, 
				    float learn_parameter, float delta_max );

static float propagateTDNetBackMcClelland(int pattern_no, int sub_pat_no,
					   float learn_parameter, 
					   float delta_max );

static float testTDNetBackward(int pattern_no, int sub_pat_no, 
				    float learn_parameter, float delta_max );

/* end private definition section */

#endif 

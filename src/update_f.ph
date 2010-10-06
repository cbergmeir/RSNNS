/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/update_f.ph,v $
  SHORTNAME      : update_f
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Network Update Functions
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 18.03.91

  CHANGED BY     : Sven Doering, Michael Vogt (Martin Reczko)
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:58 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _UPDATE_F_DEFINED_
#define  _UPDATE_F_DEFINED_

/* begin global definition section */

krui_err  UPDATE_syncPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_syncPropagateHop(float *parameterArray, int NoOfParams);
krui_err  UPDATE_FixAct_Hop(float *parameterArray, int NoOfParams);
krui_err  UPDATE_serialPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_randomOrderPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_randomPermutPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_topologicalPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_CPNPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_TimeDelayPropagate(float parameterArray[], int NoOfParams );
krui_err  UPDATE_ART1_syncPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_ART1_Propagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_ART2_syncPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_ART2_Propagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_ARTMAP_syncPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_ARTMAP_Propagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_CC_Propagate(float parameterArray[],  int NoOfParams);
krui_err  UPDATE_BPTT(float *parameterArray, int NoOfParams);
krui_err  UPDATE_DLVQ_Propagate(float parameterArray[], int NoOfParams);
krui_err  UPDATE_BAM(float *parameterArray, int NoOfParams);
krui_err  UPDATE_JE_Propagate (float *parameterArray, int NoOfParams) ;
krui_err  UPDATE_JE_Special   (float *parameterArray, int NoOfParams) ;
krui_err  UPDATE_RM_Propagate(float *parameterArray, int NoOfParams);
krui_err  ENZO_propagate(float *parameterArray, int NoOfParams);

/* end global definition section */

/* begin private definition section */

#define  UPDATE_PARAM1( param )   param[ 0 ]  /*    contains the 1st initialisation parameter  */
#define  UPDATE_PARAM2( param )   param[ 1 ]  /*    contains the 2nd initialisation parameter  */
#define  UPDATE_PARAM3( param )   param[ 2 ]  /*    contains the 3rd initialisation parameter  */
#define  UPDATE_PARAM4( param )   param[ 3 ]  /*    contains the 4th initialisation parameter  */
static void normalize_inputvector(float sum);
static krui_err ENZO_PROPAGATE_error_back(void);
static krui_err ENZO_PROPAGATE_ff(void);

/* end private definition section */

#endif 

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_learn.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Functions of CC
  NOTES          :

  AUTHOR         : Michael Schmalzl, modified by Christian Wehrfritz (PCC)
  DATE           : 5.2.93

  CHANGED BY     : Michael Schmalzl, Juergen Gatter
  RCS VERSION    : $Revision: 2.18 $
  LAST CHANGE    : $Date: 1998/04/20 11:54:44 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <memory.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#include "random.h"
#include "kr_typ.h"      /*  Kernel Types and Constants  */
#include "kr_const.h"    /*  Constant Declarators for SNNS-Kernel  */
#include "kr_def.h"      /*  Default Values  */
#include "kernel.h"      /*  kernel function prototypes  */
#include "kr_mac.h"      /*  Kernel Macros   */


#include "kr_ui.h"
#include "cc_type.h"
#include "cc_mac.h"
#include "cc_learn.ph"
#include "cc_glob.h"
#include "kr_newpattern.h"
#include "cc_modify.h"
#include "cc_prune.h"
#include "cc_display.h"
#include "tacoma_learn.h"

extern FlintType ACT_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_DERIV_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_2_DERIV_Custom_Python(struct Unit * unit_ptr);


/*****************************************************************************
  FUNCTION : cc_initVariables

  PURPOSE  : read some Parameters and global dates.

  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
krui_err cc_initVariables(float* ParameterInArray,
			  int StartPattern,int EndPattern)
{
    int i;

    cc_LayerCorrectnessTest(ParameterInArray,StartPattern,EndPattern);

    srand48((long)time(NULL)); 
 

    /* now read the parameters from the window to global variables */

    cc_printOnOff = (int)ParameterInArray[8]; 
    cc_backfittingOnOff = (int)ParameterInArray[18];
    cc_MaxSpecialUnitNo = (int)ParameterInArray[12];
    cc_modification = (int)ParameterInArray[21];
    for (i=0;i<5;i++){
	cc_Parameter[i]=ParameterInArray[22+i];
    } 
    cc_fastmode=(int)ParameterInArray[27];
    cc_fse=((LEARNING_FUNCTION==BACKPROP)? PARAM3 : 0.1);

    KernelErrorCode=cc_InitModificationVariables();
    ERROR_CHECK;
    KernelErrorCode=cc_TestWhetherParametersAreValid();
    ERROR_CHECK;
    cc_end=FALSE;
    cc_cascade=1; 

    KernelErrorCode = cc_GetTrainFunctions(LEARNING_FUNCTION);
    ERROR_CHECK;

    if (NoOfHiddenUnits==0)
	KernelErrorCode = cc_calculateNetParameters();
    ERROR_CHECK;
    KernelErrorCode = cc_generateLayerList();
    ERROR_CHECK;
    if(strcmp(krui_getUpdateFunc(),"CC_Order")){
	return(KRERR_CC_ERROR10);
    }

    if(strcmp(krui_getInitialisationFunc(),"CC_Weights")){
	return(KRERR_CC_ERROR11);
    }

    return KRERR_NO_ERROR;
}



/*****************************************************************************
  FUNCTION : cc_calculateCorrelation

  PURPOSE  : Calculates the correlation of the pool of candidate units and 
             returns the high score.
  NOTES    :

  UPDATE   : 30.3.96 <Juergen Gatter>
******************************************************************************/
float cc_calculateCorrelation(int StartPattern, int EndPattern, int counter)
{

    int s,o,n;
    double highScore, unchangedhighScore, scoreBuffer;
    float bestSpecialUnitScore = -0.1 ,
	bestSpecialUnitUnchangedScore=0.0;
    struct Unit *SpecialUnitPtr = NULL,*OutputUnitPtr;
    int start, end;

    cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
    ERROR_CHECK;

    if(cc_printOnOff)  printf("Cycle %d ",counter);

    FOR_ALL_SPECIAL_UNITS(SpecialUnitPtr,s) {
	unchangedhighScore = 0.0;
	FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,o) {
	    scoreBuffer = (CorBetweenSpecialActAndOutError[s][o] - 
			   (MeanOutputUnitError[o] * SpecialUnitSumAct[s] )) / 
		          SumSqError;
	    unchangedhighScore += fabs(scoreBuffer);
	    SIGN_OF_THE_CORRELATION[s][o] = SIGN(scoreBuffer);
	}
    
	highScore = cc_modifyHighScore(SpecialUnitPtr,s,unchangedhighScore);

	cc_actualizeGroupHighscores(highScore,s,SpecialUnitPtr);

	if(highScore> bestSpecialUnitScore){
	    bestSpecialUnitScore = highScore;
	    bestSpecialUnitUnchangedScore = unchangedhighScore;
	    bestSpecialUnitPtr = SpecialUnitPtr;
	}
	if(cc_printOnOff)  printf("S[%d]: %.4f ",s,unchangedhighScore);
    }
    if(cc_printOnOff) {
	printf("Best : %d:%.4f \n",GET_UNIT_NO(bestSpecialUnitPtr),
	       bestSpecialUnitScore);
    } 
    return(bestSpecialUnitUnchangedScore);
}


/*****************************************************************************
  FUNCTION : cc_GetTrainFunctions

  PURPOSE  : Assigns the trainings functions.

  NOTES    :

  UPDATE   : 10.10.95 <Juergen Gatter>
******************************************************************************/
krui_err cc_GetTrainFunctions(int learnFunc)
{
    cc_learningFunction = learnFunc;
    cc_propagateOutputUnitsBackward  = cc_propagateOutput;
    cc_propagateSpecialUnitsBackward = cc_propagateSpecial;
    switch(learnFunc) { 
    case BACKPROP: 
	cc_SpecialUnitUpdate = 
	    cc_OutputUnitUpdate = BackPropOfflinePart;
	break; 
    case BACKPROP_ONLINE:
	cc_SpecialUnitUpdate =
	    cc_OutputUnitUpdate = OnlineBackPropOfflinePart;
	cc_propagateOutputUnitsBackward  = cc_propagateOutputOnlineCase;
	cc_propagateSpecialUnitsBackward = cc_propagateSpecialOnlineCase;
	break;            
    case QUICKPROP: 
	cc_SpecialUnitUpdate = 
	    cc_OutputUnitUpdate = QuickPropOfflinePart;

	break; 
    case RPROP: 
	cc_SpecialUnitUpdate = 
	    cc_OutputUnitUpdate = RPropOfflinePart;

	break;
    default: CC_ERROR(KRERR_CC_ERROR3);
    }
    return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : cc_trainSpecialUnits

  PURPOSE  : Trains the special units.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
void cc_trainSpecialUnits(int maxNoOfCovarianceUpdateCycles, 
			    float minCovarianceChange,
                            int specialPatience,int StartPattern, 
			    int EndPattern,float param1, float param2,
                            float param3,int MaxSpecialUnitNo)
{
    struct Unit* UnitPtr;
    struct Link* LinkPtr;
    int s;
    int counter,start,end,n;
    float oldHighScore=0.0,newHighScore;
    float Error;

    cc_printHeadline("Training of the candidates",LENGTH_HEADLINE);
    cc_calculateOutputUnitError(StartPattern,EndPattern);
    if (SumSqError==0.0){
	Error = cc_getErr(StartPattern,EndPattern); /* calc SumSqError */
	if (SumSqError==0.0)
	    return;
    }
    for (counter=0;counter<maxNoOfCovarianceUpdateCycles;counter++){
	cc_calculateSpecialUnitActivation(StartPattern,EndPattern);
        newHighScore = cc_calculateCorrelation(StartPattern,EndPattern,counter);
        KernelErrorCode = cc_getPatternParameter(StartPattern, EndPattern,
						 &start, &end, &n);
        ERROR_CHECK_WRC;
	(*cc_propagateSpecialUnitsBackward)(start, end, n, counter,
					    param1, param2, param3);
        ERROR_CHECK_WRC;

        FOR_ALL_SPECIAL_UNITS(UnitPtr,s){
	    UnitPtr->bias += 
		(*cc_SpecialUnitUpdate)(UnitPtr->bias,
					&BIAS_PREVIOUS_SLOPE(UnitPtr),
					&BIAS_CURRENT_SLOPE(UnitPtr),
					&BIAS_LAST_WEIGHT_CHANGE(UnitPtr),
					param1,param2,param3);
	    FOR_ALL_LINKS(UnitPtr,LinkPtr){
		LinkPtr->weight +=
		    (*cc_SpecialUnitUpdate)(LinkPtr->weight,
					    &LN_PREVIOUS_SLOPE(LinkPtr),
					    &LN_CURRENT_SLOPE(LinkPtr),
					    &LN_LAST_WEIGHT_CHANGE(LinkPtr),
					    param1, param2, param3);
	    }
	}

	cc_initActivationArrays(); 

        if ((counter % specialPatience)==0){
	    if(fabs(newHighScore-oldHighScore) < 
	       minCovarianceChange*oldHighScore)
		return;
	    else
		oldHighScore = newHighScore;
	}
    }
}



/*****************************************************************************
  FUNCTION : cc_trainOutputUnits

  PURPOSE  : Trains the output units.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
void cc_trainOutputUnits(int maxNoOfErrorUpdateCycles, int backfittPatience,
			    float minErrorChange,
                            int outPatience,int StartPattern,int EndPattern, 
			    float param1, float param2, float param3, 
			    float **ParameterOutArray,int *NoOfOutParams)
{

    int p,pat,sub,counter=0;
    float oldNetError=FLOAT_MAX;
    int start,end,n;
    struct Unit* UnitPtr;
    struct Link* LinkPtr;
    int s,dummy;
    static float OutParameter[1];

    cc_printHeadline("Training of the output units",LENGTH_HEADLINE);
    *NoOfOutParams = 1;
    *ParameterOutArray = OutParameter;
    cc_initOutputUnits();
    cc_actualNetSaved=FALSE;

   /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(StartPattern,EndPattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
        return;

    for (counter=0;counter< maxNoOfErrorUpdateCycles;counter++){
        cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
	for(p=start; p<=end;p++){
           cc_getActivationsForActualPattern(p,start,&pat,&sub);
           PROPAGATE_THROUGH_OUTPUT_LAYER(UnitPtr,dummy,p);
	   (*cc_propagateOutputUnitsBackward)(pat,sub,param1,param2,param3);
	}
        cc_actualNetSaved=TRUE;
        FOR_ALL_OUTPUT_UNITS(UnitPtr,s){
           UnitPtr->bias += 
               (*cc_OutputUnitUpdate)(UnitPtr->bias,
				      &BIAS_PREVIOUS_SLOPE(UnitPtr),
				      &BIAS_CURRENT_SLOPE(UnitPtr),
				      &BIAS_LAST_WEIGHT_CHANGE(UnitPtr),
				      param1,param2,param3);
           FOR_ALL_LINKS(UnitPtr,LinkPtr){
               LinkPtr->weight +=
                  (*cc_OutputUnitUpdate)(LinkPtr->weight,
					 &LN_PREVIOUS_SLOPE(LinkPtr),
					 &LN_CURRENT_SLOPE(LinkPtr),
					 &LN_LAST_WEIGHT_CHANGE(LinkPtr),
					 param1,param2,param3);
           }
	}
         NET_ERROR(OutParameter)=cc_getErr(StartPattern,EndPattern);

	 /* otherways, it's not the actual error */
	 if(cc_printOnOff) printf("Epoch: %d NetError: %f \n",counter+1, 
				  NET_ERROR(OutParameter));
	 if ((counter % outPatience)==0){
	    if(fabs(NET_ERROR(OutParameter)-oldNetError) < 
	       minErrorChange * oldNetError)
		return;
	    else
		oldNetError=NET_ERROR(OutParameter);
	 }
    }

}



/*****************************************************************************
  FUNCTION : cc_calculateOutputUnitError

  PURPOSE  : Calculates the error of all output units and stores the result
             in the array OutputUnitError
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
void cc_calculateOutputUnitError(int StartPattern,int EndPattern)
{

    register struct Unit *UnitPtr;
    register Patterns  out_pat;
    register TopoPtrArray     topo_ptr;
    register int o,p;
    int start, end,pat,sub,n;
  
    FOR_ALL_OUTPUT_UNITS(UnitPtr,o) 
       OUTPUT_UNIT_SUM_ERROR[o] = 0.0;

    cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
    ERROR_CHECK_WRC;

    for(p=start; p<=end;p++){
	topo_ptr = topo_ptr_array;
        cc_getActivationsForActualPattern(p,start,&pat,&sub);
	out_pat = kr_getSubPatData(pat,sub,OUTPUT,NULL);
        ERROR_CHECK_WRC;
        FOR_ALL_OUTPUT_UNITS(UnitPtr,o) {
            CALCULATE_ACTIVATION_AND_OUTPUT(UnitPtr,
					    ((UnitPtr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(UnitPtr->python_act_func,
						UnitPtr) :
			(UnitPtr->act_func) (UnitPtr)) ,p);
            OUTPUT_UNIT_SUM_ERROR[o] += 
		(OutputUnitError[p][o] =  
		      (UnitPtr->Out.output-(*out_pat++))*
		      (((UnitPtr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(UnitPtr->python_act_deriv_func,
						UnitPtr) :
			(UnitPtr->act_deriv_func) (UnitPtr)) +cc_fse)); 
        }
    }

    FOR_ALL_OUTPUT_UNITS(UnitPtr,o)
       MeanOutputUnitError[o] = (OUTPUT_UNIT_SUM_ERROR[o] / n);

    cc_actualNetSaved=TRUE;
}



/*****************************************************************************
  FUNCTION :  cc_calculateSpecialUnitActivation

  PURPOSE  :  Calculates the covariance  between the output units and the 
              special units and stores the result the array 
	      CorBetweenSpecialActAndOutError.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
void cc_calculateSpecialUnitActivation(int StartPattern, int EndPattern)
{ 

    register struct Unit  *specialUnitPtr,*outputUnitPtr;
    register int o,s,p;
    int pat,sub;
    int start,end,n;

    cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);  
    ERROR_CHECK_WRC;

    for(p=start; p<=end;p++){
        cc_getActivationsForActualPattern(p,start,&pat,&sub);
	FOR_ALL_SPECIAL_UNITS(specialUnitPtr,s) {
            CALCULATE_ACTIVATION_AND_OUTPUT(specialUnitPtr,
				((specialUnitPtr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(specialUnitPtr->python_act_deriv_func,
						specialUnitPtr) :
			(specialUnitPtr->act_deriv_func) (specialUnitPtr)) ,p);
	    SpecialUnitSumAct[s] += 
		SpecialUnitAct[p][s] = specialUnitPtr->Out.output;
            FOR_ALL_OUTPUT_UNITS(outputUnitPtr,o) {
	        CorBetweenSpecialActAndOutError[s][o] += 
		    SpecialUnitAct[p][s] * OutputUnitError[p][o];
	    }
	}
    } 
    cc_actualNetSaved=TRUE;
}



/*****************************************************************************
  FUNCTION : cc_propagateOutput

  PURPOSE  : Propagates a pattern backward through the net.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
float cc_propagateOutput(int PatternNo, int sub_pat_no, float param1, 
			 float param2, float param3)
{

    struct Link   *LinkPtr;
    struct Site   *site_ptr;
    struct Unit   *OutputUnitPtr;
    Patterns      out_pat;
    float         error,devit;
    int           dummy;

    out_pat = kr_getSubPatData(PatternNo,sub_pat_no,OUTPUT,NULL);
    ERROR_CHECK;

    FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,dummy){
	devit =  OutputUnitPtr->Out.output - *(out_pat++);
	error = devit * 
	    (((OutputUnitPtr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(OutputUnitPtr->python_act_deriv_func,
						OutputUnitPtr) :
			(OutputUnitPtr->act_deriv_func) (OutputUnitPtr))  + cc_fse);
        BIAS_CURRENT_SLOPE(OutputUnitPtr) += error;

      	if (UNIT_HAS_DIRECT_INPUTS(OutputUnitPtr)) {  
	    FOR_ALL_LINKS(OutputUnitPtr,LinkPtr) {
		LN_CURRENT_SLOPE(LinkPtr) +=  error * LinkPtr->to->Out.output;
	    }
	}else{
	    FOR_ALL_SITES_AND_LINKS(OutputUnitPtr,site_ptr,LinkPtr) {
		LN_CURRENT_SLOPE(LinkPtr) +=  error * LinkPtr->to->Out.output;
	    }
	}
    }
    return(0); /* not used anymore, since result isn't valid when used */ 
}



/*****************************************************************************
  FUNCTION : cc_propagateOutputOnlineCase

  PURPOSE  : Propagates a pattern backward through the net.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
float cc_propagateOutputOnlineCase(int PatternNo, int sub_pat_no,
				   float eta,float mu,float dummy1)
{

    struct Link   *LinkPtr;
    struct Site   *site_ptr;
    struct Unit   *OutputUnitPtr;
    Patterns      out_pat;
    float         error,sum_error,devit;
    int           dummy;
    float         lastChange;

    sum_error = 0.0; 
    out_pat = kr_getSubPatData(PatternNo,sub_pat_no,OUTPUT,NULL);
    ERROR_CHECK;
    FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,dummy){
	devit =  OutputUnitPtr->Out.output - *(out_pat++);

	sum_error += devit * devit;
	error = devit * 
	        (((OutputUnitPtr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(OutputUnitPtr->python_act_deriv_func,
						OutputUnitPtr) :
			(OutputUnitPtr->act_deriv_func) (OutputUnitPtr))  + dummy1);
        lastChange = BIAS_LAST_WEIGHT_CHANGE(OutputUnitPtr);
  	OutputUnitPtr->bias -= 
	    ((BIAS_LAST_WEIGHT_CHANGE(OutputUnitPtr) = 
	      error * eta  + lastChange * mu));

	if (UNIT_HAS_DIRECT_INPUTS(OutputUnitPtr)) { 
	    FOR_ALL_LINKS(OutputUnitPtr,LinkPtr) {
                lastChange =  LN_LAST_WEIGHT_CHANGE(LinkPtr);
		LinkPtr->weight -= 
		    ((LN_LAST_WEIGHT_CHANGE(LinkPtr) = 
		      error*LinkPtr->to->Out.output*eta+lastChange*mu));
	    }
	}else{
	    FOR_ALL_SITES_AND_LINKS(OutputUnitPtr,site_ptr,LinkPtr) {
                lastChange =  LN_LAST_WEIGHT_CHANGE(LinkPtr);
		LinkPtr->weight -= 
		    ((LN_LAST_WEIGHT_CHANGE(LinkPtr) = 
		      error*LinkPtr->to->Out.output*eta+lastChange*mu));
	    }
	}
    }
    return(sum_error); 
}


/*****************************************************************************
  FUNCTION : cc_propagateSpecial

  PURPOSE  : Propagation in Case of Special Units, no BackProp
  NOTES    :

  UPDATE   : 30.3.96 <Juergen Gatter>
******************************************************************************/
krui_err cc_propagateSpecial(int start,int end,int n,int counter,
			     float param1, float param2,float param3)
{

    float change=0.0,actPrime;
    int s,o,p,pat,sub;
    struct Unit *SpecialUnitPtr,*OutputUnitPtr;
    struct Link *LinkPtr;

    for(p=start; p<=end;p++){
        cc_getActivationsForActualPattern(p,start,&pat,&sub);

	FOR_ALL_SPECIAL_UNITS(SpecialUnitPtr,s) {
	    change = 0.0;
	    SpecialUnitPtr->act = SpecialUnitAct[p][s];
	    actPrime = ((SpecialUnitPtr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(SpecialUnitPtr->python_act_deriv_func,
						SpecialUnitPtr) :
			(SpecialUnitPtr->act_deriv_func) (SpecialUnitPtr)) ;
	    FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,o) 
		if(UNITS_IN_SAME_GROUP(s,o)){
		    change -= SIGN_OF_THE_CORRELATION[s][o] *
			(OutputUnitError[p][o]-MeanOutputUnitError[o]);
		}  
            change *= actPrime/SumSqError;
            BIAS_CURRENT_SLOPE(SpecialUnitPtr) += change;

	    FOR_ALL_LINKS(SpecialUnitPtr,LinkPtr)
		LN_CURRENT_SLOPE(LinkPtr) += change * LinkPtr->to->Out.output;
	}
    }
    cc_actualNetSaved=TRUE;
    return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : cc_propagateSpecialOnlineCase

  PURPOSE  : Propagation in Case of Special Units for BackProp
  NOTES    :

  UPDATE   : 30.3.96 <Juergen Gatter>
******************************************************************************/
krui_err cc_propagateSpecialOnlineCase(int start,int end,int n,int counter,
				       float eta, float mu, float dummy)
{

    float change=0.0,actPrime;
    int s,o,p,pat,sub;
    struct Unit *SpecialUnitPtr,*OutputUnitPtr;
    struct Link *LinkPtr;
    float lastChange;

    for(p=start; p<=end;p++){
        cc_getActivationsForActualPattern(p,start,&pat,&sub);

	FOR_ALL_SPECIAL_UNITS(SpecialUnitPtr,s) {
	    change = 0.0;
	    SpecialUnitPtr->act = SpecialUnitAct[p][s];
	    actPrime = ((SpecialUnitPtr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(SpecialUnitPtr->python_act_deriv_func,
						SpecialUnitPtr) :
			(SpecialUnitPtr->act_deriv_func) (SpecialUnitPtr)) ;
	    FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,o) 
               if(UNITS_IN_SAME_GROUP(s,o)){
		change += SIGN_OF_THE_CORRELATION[s][o] *
		   (OutputUnitError[p][o]-MeanOutputUnitError[o]);
	       }  
            change *= actPrime/SumSqError;
            lastChange = BIAS_LAST_WEIGHT_CHANGE(SpecialUnitPtr);
	    SpecialUnitPtr->bias += 
		(BIAS_LAST_WEIGHT_CHANGE(SpecialUnitPtr) = 
		 (change * eta + lastChange * mu));     
	    FOR_ALL_LINKS(SpecialUnitPtr,LinkPtr) {
		lastChange = LN_LAST_WEIGHT_CHANGE(LinkPtr);
		LinkPtr->weight += 
		    (LN_LAST_WEIGHT_CHANGE(LinkPtr) = 
		     change*LinkPtr->to->Out.output*eta+lastChange*mu);
	    }
	}
    }
    cc_actualNetSaved=TRUE;
    return(KRERR_NO_ERROR);
}




/*****************************************************************************
  FUNCTION : LEARN_CC

  PURPOSE  : The main routine of CC
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err LEARN_CC(int StartPattern, int EndPattern, 
		  float *ParameterInArray, int NoOfInParams, 
		  float **ParameterOutArray, int *NoOfOutParams)
{

  NetLearnAlgorithm = CC;

  return(LEARN_CasCor(StartPattern,EndPattern,ParameterInArray,NoOfInParams,
		      ParameterOutArray,NoOfOutParams));
}


 


/*****************************************************************************
  FUNCTION : LEARN_CasCor

  PURPOSE  : The main learn routine of CC
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err LEARN_CasCor(int StartPattern, int EndPattern,
		      float *ParameterInArray, int NoOfInParams,
		      float **ParameterOutArray, int *NoOfOutParams)
    
{

    static int   GroupNo;
    static float LeTe;
    int TempErrorCode; /* Used by the macro ERROR_CHECK_WITH_MEM_DEALLOC */
 
    if(cc_allButtonIsPressed == 1) { 
	KernelErrorCode = cc_initVariables(ParameterInArray, StartPattern,
					   EndPattern);
	ERROR_CHECK;
    }

    if(cc_end){
	cc_deleteAllSpecialUnits();
	return(cc_freeStorage(StartPattern,EndPattern,0));
    }

    cc_allocateStorage(StartPattern,EndPattern,MAX_SPECIAL_UNIT_NUMBER);
    ERROR_CHECK_WITH_MEMORY_DEALLOCATION;
 
    /* it's easier to delete and reconstruct the Units, than to change them */
    KernelErrorCode = cc_deleteAllSpecialUnits();
    ERROR_CHECK_WITH_MEMORY_DEALLOCATION; 
  
    KernelErrorCode = cc_generateSpecialUnits(SPECIAL_FUNCTION_TYPE);
    ERROR_CHECK_WITH_MEMORY_DEALLOCATION;

    KernelErrorCode = cc_initSpecialUnitLinks();
    ERROR_CHECK_WITH_MEMORY_DEALLOCATION;
  
    if(cc_storageFree){   CC_ERROR(KRERR_CC_ERROR2);    }

    /* If this is the first run, train the output connections */
    if (NoOfHiddenUnits<1) {
	if((OUT_PATIEN != 0) && (MAX_NO_ERROR_UPDATE_CYCLES != 0)) {
	    cc_trainOutputUnits(MAX_NO_ERROR_UPDATE_CYCLES, BACKFITT_PATIENCE, 
				MINIMAL_ERROR_CHANGE, OUT_PATIEN,
				StartPattern, EndPattern, PARAM1, PARAM2,
				PARAM3, ParameterOutArray, NoOfOutParams);
	}
    }

    /* compute the selection criterion before the new hidden is installed */
    if (CC_PRUNE_ONOFF) {
	LeTe=cc_getPruningError(PRUNE_FUNC,StartPattern,EndPattern,1);
    }

    /* train the candidates */
    if(cc_test(StartPattern,EndPattern,MAX_PIXEL_ERR) ==  CONTINUE_LEARNING) {
	if((SPEC_PATIENCE != 0) && (MAX_NO_OF_COVAR_UPDATE_CYCLES != 0)) {
	    cc_trainSpecialUnits(MAX_NO_OF_COVAR_UPDATE_CYCLES,
				 MIN_COVAR_CHANGE,SPEC_PATIENCE,
				 StartPattern,EndPattern,
				 PARAM4,PARAM5,PARAM6,MAX_SPECIAL_UNIT_NUMBER);
	}
    }else {			/* STOP_LEARNING  */
	cc_end = TRUE;
	return(cc_freeStorage(StartPattern,EndPattern,0));
    }

    cc_actualNetSaved=FALSE;

    /* install new hidden unit(s) */
    for (GroupNo=0;GroupNo<cc_calculateNoOfNewUnits();GroupNo++){
	if((SPEC_PATIENCE != 0) && (MAX_NO_OF_COVAR_UPDATE_CYCLES != 0)) {
	    KernelErrorCode = cc_generateHiddenUnit(GroupNo);
	    ERROR_CHECK_WITH_MEMORY_DEALLOCATION;
	}
    }
  
    /* retrain the output connections */
    if((OUT_PATIEN != 0) && (MAX_NO_ERROR_UPDATE_CYCLES != 0)) {
	cc_trainOutputUnits(MAX_NO_ERROR_UPDATE_CYCLES, BACKFITT_PATIENCE,
			    MINIMAL_ERROR_CHANGE, OUT_PATIEN, StartPattern,
			    EndPattern, PARAM1, PARAM2, PARAM3,
			    ParameterOutArray,NoOfOutParams); 
    }


    /* prune the new hidden unit and retrain the output connections again */
    if (CC_PRUNE_ONOFF) {
	cc_pruneNet(StartPattern, EndPattern, PRUNE_FUNC);
	if((OUT_PATIEN != 0) && (MAX_NO_ERROR_UPDATE_CYCLES != 0)) {
	    cc_trainOutputUnits(MAX_NO_ERROR_UPDATE_CYCLES, BACKFITT_PATIENCE,
				MINIMAL_ERROR_CHANGE,
				OUT_PATIEN,StartPattern,EndPattern,PARAM1,
				PARAM2,PARAM3,ParameterOutArray,NoOfOutParams); 
	}
	if (cc_getPruningError(PRUNE_FUNC,StartPattern,EndPattern,2)>LeTe) {
	    cc_remove_Unit(); 
	}       
    }
    cc_deleteAllSpecialUnits();
    cc_allButtonIsPressed = 0;
    return(cc_freeStorage(StartPattern,EndPattern,0));
}


/*****************************************************************************
  FUNCTION : void cc_generateLayerList

  PURPOSE  : generates list with data of the layers
  NOTES    :

  UPDATE   : 20.11.95 <Juergen Gatter>
******************************************************************************/
krui_err cc_generateLayerList(void)
{

    int i;
    struct Unit* UnitPtr;

    FREE_IF_USED(ListOfLayers);  /* we allocate a new list */
    CALLOC_ERRORCHECK(ListOfLayers,MINIMAL_LAYERLIST_SIZE+NoOfLayers,
		      Layer_Type);
    ListOfLayers[0].NoOfUnitsInLayer = NoOfInputUnits;
    ListOfLayers[0].xPosFirstRow = cc_hiddenXminPos;

    SizeOfLayerlist = MINIMAL_LAYERLIST_SIZE+NoOfLayers;

    for (i=1;i<SizeOfLayerlist;i++){
	ListOfLayers[i].xPosFirstRow = MAX_POS;
	ListOfLayers[i].NoOfUnitsInLayer = 0;
    }
    /* calculating data for the layers */
    FOR_ALL_UNITS(UnitPtr){
	if (IS_HIDDEN_UNIT(UnitPtr)){
	    if(CC_LAYER_NO(UnitPtr) >=SizeOfLayerlist)
		printf("\n\n\nInternal error with layers\n\n\n");
	    else{
		ListOfLayers[CC_LAYER_NO(UnitPtr)].NoOfUnitsInLayer++;
		ListOfLayers[CC_LAYER_NO(UnitPtr)].xPosFirstRow =
		    MIN(ListOfLayers[CC_LAYER_NO(UnitPtr)].xPosFirstRow,
			GET_UNIT_XPOS(UnitPtr));
	    }
	}
    } 
    return(KRERR_NO_ERROR);
}




/*****************************************************************************
  FUNCTION : void cc_actualizeLayerList

  PURPOSE  : changes the Size of the LayerList, iff needed.
  NOTES    : the List have to be generated with cc_generateLayerList.

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
krui_err cc_actualizeLayerlist(struct Unit* UnitPtr, int LayerNo)
{

    int i;
    struct CC_LAYER* Newlist;

    if (LayerNo > NoOfLayers){
	NoOfLayers++;
	if (NoOfLayers >= SizeOfLayerlist-1){ /* then double size */

	    CALLOC_ERRORCHECK(Newlist,SizeOfLayerlist * 2,struct CC_LAYER);

	    for(i=0;i<SizeOfLayerlist;i++)
		Newlist[i]=ListOfLayers[i]; 

	    for(i=SizeOfLayerlist;i<2*SizeOfLayerlist;i++)
		Newlist[i].NoOfUnitsInLayer=0;

	    SizeOfLayerlist *= 2;
	    free(ListOfLayers);
	    ListOfLayers=Newlist;    
	}
	ListOfLayers[NoOfLayers].NoOfUnitsInLayer=1;
	ListOfLayers[NoOfLayers].xPosFirstRow=
	    FIRST_ROW_NEXT_LAYER(NoOfLayers-1);
    }else     
	ListOfLayers[LayerNo].NoOfUnitsInLayer++;

    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : krui_err cc_generateHiddenUnit

  PURPOSE  : Generates a new hidden unit 
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err cc_generateHiddenUnit(int GroupNo)
{

    int LayerOfNewUnit;
 
    int CurrentUnit,dummy,NewHiddenUnit;
    struct Unit *HiddenUnitPtr,*OutputUnitPtr;

    bestSpecialUnitPtr=cc_getModifiedBestSpecialUnitPtr(GroupNo);

    LayerOfNewUnit = CC_LAYER_NO(bestSpecialUnitPtr);
    KernelErrorCode=cc_actualizeLayerlist(bestSpecialUnitPtr,LayerOfNewUnit);
    ERROR_CHECK;
    LastInsertedHiddenUnit=
        CurrentUnit = 
	    KernelErrorCode = 
	        kr_copyUnit(ONLY_INPUTS,GET_UNIT_NO(bestSpecialUnitPtr));
    if(KernelErrorCode < 0)  return(KernelErrorCode); 

    KernelErrorCode = KRERR_NO_ERROR;
    KernelErrorCode = kr_unitSetTType(CurrentUnit,HIDDEN); 
    ERROR_CHECK;

    HiddenUnitPtr = kr_getUnitPtr(CurrentUnit); 
    ERROR_CHECK;

    CC_SET_LAYER_NO(HiddenUnitPtr,LayerOfNewUnit);

    KernelErrorCode = cc_setHiddenUnit(HiddenUnitPtr,LayerOfNewUnit);
    ERROR_CHECK;

    KernelErrorCode = krui_setCurrentUnit(CurrentUnit); 
    ERROR_CHECK;

    NewHiddenUnit = CurrentUnit;

    /* generate links between output unit and new hidden unit */
    FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,dummy){
	CurrentUnit = GET_UNIT_NO(OutputUnitPtr);
	KernelErrorCode = krui_setCurrentUnit(CurrentUnit); 
	ERROR_CHECK;

	KernelErrorCode = krui_createLink(NewHiddenUnit,0.0);
	ERROR_CHECK; 
    }       /* and change its Layer if necessary */
    KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
    ERROR_CHECK;    

    KernelErrorCode = cc_setPointers();
    ERROR_CHECK;

    NetModified = FALSE;

    return(KRERR_NO_ERROR);
    
}

 

/*****************************************************************************
  FUNCTION : cc_test

  PURPOSE  : Tests whether to continue learning or not.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
int cc_test(int StartPattern, int EndPattern, float maxPixelError)
{
    int p,sub,pat,o;
    int start, end,n,dummy;
    Patterns out_pat;
    struct Unit *unitPtr;

    /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(StartPattern,EndPattern);
    ERROR_CHECK;

    cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);

    for(p=start; p<=end;p++){
     	cc_getActivationsForActualPattern(p,start,&pat,&sub);

	PROPAGATE_THROUGH_OUTPUT_LAYER(unitPtr,dummy,p);

	out_pat = kr_getSubPatData(pat,sub,OUTPUT,NULL);

	FOR_ALL_OUTPUT_UNITS(unitPtr,o){
	    if((fabs(*(out_pat++) - unitPtr->Out.output))>maxPixelError){
		return(CONTINUE_LEARNING);
	    } 
	}
    }
    cc_actualNetSaved=TRUE;
    return(STOP_LEARNING);
}



/*****************************************************************************
  FUNCTION : cc_generateSpecialUnits   

  PURPOSE  : Generates the special units
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err cc_generateSpecialUnits(int type)
{

    int i,selector,calcLayerNo;
    struct Unit *UnitPtr;
    struct Unit *SpecUnitPtr;
    int CurrentUnit;

    cc_MakeMiscCalculationsForModifications();

    for(i=0;i<cc_MaxSpecialUnitNo;i++) {
        selector = ((type==RANDOM) ?  i % (NO_OF_ACT_FUNCS - 1) : type);

	KernelErrorCode = 
	    kr_unitSetTType(CurrentUnit=kr_makeDefaultUnit(),SPECIAL); 
	ERROR_CHECK;
	KernelErrorCode = krui_setUnitActFunc(CurrentUnit,
					      cc_actFuncArray[selector]);
	ERROR_CHECK;  

	SpecUnitPtr = kr_getUnitPtr(CurrentUnit); 
	ERROR_CHECK;
  
	KernelErrorCode = krui_setCurrentUnit(CurrentUnit); 
	ERROR_CHECK;

        KernelErrorCode = cc_calculateConnections(i);
        ERROR_CHECK;

        calcLayerNo=0;
	FOR_ALL_UNITS(UnitPtr){
	    if((IS_INPUT_UNIT(UnitPtr) ||  IS_HIDDEN_UNIT(UnitPtr)) && 
	       cc_UnitsHaveToBeConnected(i,UnitPtr) &&
	       UNIT_IN_USE(UnitPtr)) {
		if (CC_LAYER_NO(UnitPtr) > calcLayerNo) 
		    calcLayerNo = CC_LAYER_NO(UnitPtr);
		KernelErrorCode = 
		    krui_createLink(GET_UNIT_NO(UnitPtr),
				    cc_generateRandomNo(CC_MAX_VALUE)); 
		ERROR_CHECK;
	    }
	}
        CC_SET_LAYER_NO(SpecUnitPtr,calcLayerNo+1);
    }

   
    KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
    ERROR_CHECK;    

    KernelErrorCode = cc_setPointers();
    ERROR_CHECK;

    cc_setSpecialUnits(cc_outputXMax+2);
    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : TEST_CasCor

  PURPOSE  : main routine for the test of Cascade Correlation nets
  RETURNS  : kernel error code
  NOTES    : Output Parameters:  1 : error of the network (sum of all cycles)


  UPDATE   : 17.01.96 <Juergen Gatter>
******************************************************************************/
krui_err TEST_CasCor(int start_pattern, int end_pattern,
		     float *parameterInArray, int NoOfInParams,
		     float **parameterOutArray, int *NoOfOutParams)
{
    int          saved_cc_fastmode;
    static float OutParameter[1];

    KernelErrorCode = KRERR_NO_ERROR;	/* reset return code  */

    if (NoOfInParams < 1) {	/* Not enough input parameters	 */
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }
    *NoOfOutParams = 1;		/* One return value is available (the
				   learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */

    saved_cc_fastmode = cc_fastmode; 
    cc_fastmode = FALSE;       
    NET_ERROR(OutParameter) = cc_getErr(start_pattern, end_pattern);
  
    cc_fastmode = saved_cc_fastmode;
    cc_actualNetSaved = FALSE;
    return (KernelErrorCode);
}





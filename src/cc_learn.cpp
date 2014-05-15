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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <memory.h>

#include <string.h>

#include "SnnsCLib.h"

/*****************************************************************************
  FUNCTION : cc_initVariables

  PURPOSE  : read some Parameters and global dates.

  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
krui_err SnnsCLib::cc_initVariables(float* ParameterInArray,
			  int StartPattern,int EndPattern)
{
    int i;

    cc_LayerCorrectnessTest(ParameterInArray,StartPattern,EndPattern);

    //SnnsCLib: random number generator is now initialized in the constructor
    //u_srand48((long)time(NULL)); 
 

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
float SnnsCLib::cc_calculateCorrelation(int StartPattern, int EndPattern, int counter)
{

    int s,o,n;
    double highScore, unchangedhighScore, scoreBuffer;
    float bestSpecialUnitScore = -0.1 ,
	bestSpecialUnitUnchangedScore=0.0;
    struct Unit *SpecialUnitPtr = NULL,*OutputUnitPtr;
    int start, end;

    cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
    ERROR_CHECK;

    //if(cc_printOnOff)  printf("Cycle %d ",counter);

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
	//if(cc_printOnOff)  printf("S[%d]: %.4f ",s,unchangedhighScore);
    }
    //if(cc_printOnOff) {
	//printf("Best : %d:%.4f \n",GET_UNIT_NO(bestSpecialUnitPtr),
	  //     bestSpecialUnitScore);
    //} 
    return(bestSpecialUnitUnchangedScore);
}


/*****************************************************************************
  FUNCTION : cc_GetTrainFunctions

  PURPOSE  : Assigns the trainings functions.

  NOTES    :

  UPDATE   : 10.10.95 <Juergen Gatter>
******************************************************************************/
krui_err SnnsCLib::cc_GetTrainFunctions(int learnFunc)
{
    cc_learningFunction = learnFunc;
    cc_propagateOutputUnitsBackward  = &SnnsCLib::cc_propagateOutput;
    cc_propagateSpecialUnitsBackward = &SnnsCLib::cc_propagateSpecial;
    switch(learnFunc) { 
    case BACKPROP: 
	cc_SpecialUnitUpdate = 
	    cc_OutputUnitUpdate = &SnnsCLib::BackPropOfflinePart;
	break; 
    case BACKPROP_ONLINE:
	cc_SpecialUnitUpdate =
	    cc_OutputUnitUpdate = &SnnsCLib::OnlineBackPropOfflinePart;
	cc_propagateOutputUnitsBackward  = &SnnsCLib::cc_propagateOutputOnlineCase;
	cc_propagateSpecialUnitsBackward = &SnnsCLib::cc_propagateSpecialOnlineCase;
	break;            
    case QUICKPROP: 
	cc_SpecialUnitUpdate = 
	    cc_OutputUnitUpdate = &SnnsCLib::QuickPropOfflinePart;

	break; 
    case RPROP: 
	cc_SpecialUnitUpdate = 
	    cc_OutputUnitUpdate = &SnnsCLib::RPropOfflinePart;

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
void SnnsCLib::cc_trainSpecialUnits(int maxNoOfCovarianceUpdateCycles, 
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
    //float Error;

    cc_printHeadline(const_cast<char*>("Training of the candidates"),LENGTH_HEADLINE);
    cc_calculateOutputUnitError(StartPattern,EndPattern);
    if (SumSqError==0.0){
	//Error = 
        cc_getErr(StartPattern,EndPattern); /* calc SumSqError */
	if (SumSqError==0.0)
	    return;
    }
    for (counter=0;counter<maxNoOfCovarianceUpdateCycles;counter++){
	cc_calculateSpecialUnitActivation(StartPattern,EndPattern);
        newHighScore = cc_calculateCorrelation(StartPattern,EndPattern,counter);
        KernelErrorCode = cc_getPatternParameter(StartPattern, EndPattern,
						 &start, &end, &n);
        ERROR_CHECK_WRC;
	(this->*cc_propagateSpecialUnitsBackward)(start, end, n, counter,
					    param1, param2, param3);
        ERROR_CHECK_WRC;

        FOR_ALL_SPECIAL_UNITS(UnitPtr,s){
	    UnitPtr->bias += 
		(this->*cc_SpecialUnitUpdate)(UnitPtr->bias,
					&BIAS_PREVIOUS_SLOPE(UnitPtr),
					&BIAS_CURRENT_SLOPE(UnitPtr),
					&BIAS_LAST_WEIGHT_CHANGE(UnitPtr),
					param1,param2,param3);
	    FOR_ALL_LINKS(UnitPtr,LinkPtr){
		LinkPtr->weight +=
		    (this->*cc_SpecialUnitUpdate)(LinkPtr->weight,
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
void SnnsCLib::cc_trainOutputUnits(int maxNoOfErrorUpdateCycles, int backfittPatience,
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
    //static float OutParameter[1];

    cc_printHeadline(const_cast<char*>("Training of the output units"),LENGTH_HEADLINE);
    *NoOfOutParams = 1;
    *ParameterOutArray = m_cc_trainOutputUnits_OutParameter;
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
	   (this->*cc_propagateOutputUnitsBackward)(pat,sub,param1,param2,param3);
	}
        cc_actualNetSaved=TRUE;
        FOR_ALL_OUTPUT_UNITS(UnitPtr,s){
           UnitPtr->bias += 
               (this->*cc_OutputUnitUpdate)(UnitPtr->bias,
				      &BIAS_PREVIOUS_SLOPE(UnitPtr),
				      &BIAS_CURRENT_SLOPE(UnitPtr),
				      &BIAS_LAST_WEIGHT_CHANGE(UnitPtr),
				      param1,param2,param3);
           FOR_ALL_LINKS(UnitPtr,LinkPtr){
               LinkPtr->weight +=
                  (this->*cc_OutputUnitUpdate)(LinkPtr->weight,
					 &LN_PREVIOUS_SLOPE(LinkPtr),
					 &LN_CURRENT_SLOPE(LinkPtr),
					 &LN_LAST_WEIGHT_CHANGE(LinkPtr),
					 param1,param2,param3);
           }
	}
         NET_ERROR(m_cc_trainOutputUnits_OutParameter)=cc_getErr(StartPattern,EndPattern);

	 /* otherways, it's not the actual error */
	 //if(cc_printOnOff) printf("Epoch: %d NetError: %f \n",counter+1, 
	//			  NET_ERROR(m_cc_trainOutputUnits_OutParameter));
	 if ((counter % outPatience)==0){
	    if(fabs(NET_ERROR(m_cc_trainOutputUnits_OutParameter)-oldNetError) < 
	       minErrorChange * oldNetError)
		return;
	    else
		oldNetError=NET_ERROR(m_cc_trainOutputUnits_OutParameter);
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
void SnnsCLib::cc_calculateOutputUnitError(int StartPattern,int EndPattern)
{

    register struct Unit *UnitPtr;
    register Patterns  out_pat;
    //register TopoPtrArray     topo_ptr;
    register int o,p;
    int start, end,pat,sub,n;
  
    FOR_ALL_OUTPUT_UNITS(UnitPtr,o) 
       OUTPUT_UNIT_SUM_ERROR[o] = 0.0;

    cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
    ERROR_CHECK_WRC;

    for(p=start; p<=end;p++){
	//topo_ptr = topo_ptr_array;
        cc_getActivationsForActualPattern(p,start,&pat,&sub);
	out_pat = kr_getSubPatData(pat,sub,OUTPUT,NULL);
        ERROR_CHECK_WRC;
        FOR_ALL_OUTPUT_UNITS(UnitPtr,o) {
            CALCULATE_ACTIVATION_AND_OUTPUT(UnitPtr,
					    (this->*UnitPtr->act_func)(UnitPtr),p);
            OUTPUT_UNIT_SUM_ERROR[o] += 
		(OutputUnitError[p][o] =  
		      (UnitPtr->Out.output-(*out_pat++))*
		      ((this->*UnitPtr->act_deriv_func)(UnitPtr)+cc_fse)); 
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
void SnnsCLib::cc_calculateSpecialUnitActivation(int StartPattern, int EndPattern)
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
				(this->*specialUnitPtr->act_func) (specialUnitPtr),p);
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
float SnnsCLib::cc_propagateOutput(int PatternNo, int sub_pat_no, float param1, 
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
	    ((this->*OutputUnitPtr->act_deriv_func)(OutputUnitPtr) + cc_fse);
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
float SnnsCLib::cc_propagateOutputOnlineCase(int PatternNo, int sub_pat_no,
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
	        ((this->*OutputUnitPtr->act_deriv_func)(OutputUnitPtr) + dummy1);
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
krui_err SnnsCLib::cc_propagateSpecial(int start,int end,int n,int counter,
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
	    actPrime = (this->*SpecialUnitPtr->act_deriv_func)(SpecialUnitPtr);
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
krui_err SnnsCLib::cc_propagateSpecialOnlineCase(int start,int end,int n,int counter,
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
	    actPrime = (this->*SpecialUnitPtr->act_deriv_func)(SpecialUnitPtr);
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
krui_err SnnsCLib::LEARN_CC(int StartPattern, int EndPattern, 
		  float *ParameterInArray, int NoOfInParams, 
		  float **ParameterOutArray, int *NoOfOutParams)
{

  NetLearnAlgorithm = SNNSCLIB_CC;

  return(LEARN_CasCor(StartPattern,EndPattern,ParameterInArray,NoOfInParams,
		      ParameterOutArray,NoOfOutParams));
}


 


/*****************************************************************************
  FUNCTION : LEARN_CasCor

  PURPOSE  : The main learn routine of CC
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err SnnsCLib::LEARN_CasCor(int StartPattern, int EndPattern,
		      float *ParameterInArray, int NoOfInParams,
		      float **ParameterOutArray, int *NoOfOutParams)
    
{

    //static int   GroupNo;
    //static float LeTe;
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
	m_cc_learn_LeTe=cc_getPruningError(PRUNE_FUNC,StartPattern,EndPattern,1);
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
    for (m_cc_learn_GroupNo=0;m_cc_learn_GroupNo<cc_calculateNoOfNewUnits();m_cc_learn_GroupNo++){
	if((SPEC_PATIENCE != 0) && (MAX_NO_OF_COVAR_UPDATE_CYCLES != 0)) {
	    KernelErrorCode = cc_generateHiddenUnit(m_cc_learn_GroupNo);
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
        ERROR_CHECK_WITH_MEMORY_DEALLOCATION;
	if((OUT_PATIEN != 0) && (MAX_NO_ERROR_UPDATE_CYCLES != 0)) {
	    cc_trainOutputUnits(MAX_NO_ERROR_UPDATE_CYCLES, BACKFITT_PATIENCE,
				MINIMAL_ERROR_CHANGE,
				OUT_PATIEN,StartPattern,EndPattern,PARAM1,
				PARAM2,PARAM3,ParameterOutArray,NoOfOutParams); 
	}
	if (cc_getPruningError(PRUNE_FUNC,StartPattern,EndPattern,2)>m_cc_learn_LeTe) {
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
krui_err SnnsCLib::cc_generateLayerList(void)
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
	    //if(CC_LAYER_NO(UnitPtr) >=SizeOfLayerlist)
		//printf("\n\n\nInternal error with layers\n\n\n");
	    //else{
		ListOfLayers[CC_LAYER_NO(UnitPtr)].NoOfUnitsInLayer++;
		ListOfLayers[CC_LAYER_NO(UnitPtr)].xPosFirstRow =
		    MIN(ListOfLayers[CC_LAYER_NO(UnitPtr)].xPosFirstRow,
			GET_UNIT_XPOS(UnitPtr));
	    //}
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
krui_err SnnsCLib::cc_actualizeLayerlist(struct Unit* UnitPtr, int LayerNo)
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
krui_err SnnsCLib::cc_generateHiddenUnit(int GroupNo)
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
int SnnsCLib::cc_test(int StartPattern, int EndPattern, float maxPixelError)
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
krui_err SnnsCLib::cc_generateSpecialUnits(int type)
{

    int i,selector,calcLayerNo;
    struct Unit *UnitPtr;
    struct Unit *SpecUnitPtr;
    int CurrentUnit;

    const char *cc_actFuncArray[]     ={"Act_Logistic","Act_LogSym","Act_TanH",
                              "Act_CC_Thresh","Act_Sinus","Act_Exponential",
			      "Act_Random"};

    cc_MakeMiscCalculationsForModifications();

    for(i=0;i<cc_MaxSpecialUnitNo;i++) {
        selector = ((type==RANDOM) ?  i % (NO_OF_ACT_FUNCS - 1) : type);

	KernelErrorCode = 
	    kr_unitSetTType(CurrentUnit=kr_makeDefaultUnit(),SPECIAL); 
	ERROR_CHECK;
	KernelErrorCode = krui_setUnitActFunc(CurrentUnit,
					      const_cast<char*>(cc_actFuncArray[selector]));
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
krui_err SnnsCLib::TEST_CasCor(int start_pattern, int end_pattern,
		     float *parameterInArray, int NoOfInParams,
		     float **parameterOutArray, int *NoOfOutParams)
{
    int          saved_cc_fastmode;
    //static float OutParameter[1];

    KernelErrorCode = KRERR_NO_ERROR;	/* reset return code  */

    if (NoOfInParams < 1) {	/* Not enough input parameters	 */
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }
    *NoOfOutParams = 1;		/* One return value is available (the
				   learning error)  */
    *parameterOutArray = m_TEST_CasCor_OutParameter;	/* set the output parameter reference */

    saved_cc_fastmode = cc_fastmode; 
    cc_fastmode = FALSE;       
    NET_ERROR(m_TEST_CasCor_OutParameter) = cc_getErr(start_pattern, end_pattern);
  
    cc_fastmode = saved_cc_fastmode;
    cc_actualNetSaved = FALSE;
    return (KernelErrorCode);
}





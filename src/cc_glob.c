/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_glob.c,v $
  SHORTNAME      : cc_glob
  SNNS VERSION   : 4.2

  PURPOSE        : Common functions for all CC algorithms 
  NOTES          : This file was put together from the earlier files cc_rcc 
                   and cc_rcc_topo

  AUTHOR         : Michael Schmalzl
  DATE           : 5.2.93

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/13 16:23:51 $

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
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include "kr_typ.h"	
#include "kr_const.h"	 
#include "kr_def.h"	 
#include "random.h"
#include "kr_mac.h"
#include "kernel.h"
#include "kr_ui.h"
#include "kr_newpattern.h"
#include "cc_mac.h"	
#include "cc_type.h"
#include "cc_modify.h"
#include "cc_display.h"

#include "cc_glob.ph"   

extern FlintType ACT_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_DERIV_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_2_DERIV_Custom_Python(struct Unit * unit_ptr);


/*****************************************************************************
  FUNCTION : cc_printHeadline
  PURPOSE  : prints Headline iff cc_printOnOff is ON
  NOTES    :

  UPDATE   : 30.3.96 <Juergen Gatter>
******************************************************************************/
void cc_printHeadline(char* s,int Length)
{
    int LengthText,Length1,Length2;
    int i;

    LengthText=strlen(s)+2;
    if ((LengthText>Length)||(!cc_printOnOff))
	return;
    Length2 = ((Length-LengthText) / 2);
    Length1 = Length-Length2-LengthText;
    printf("\n");
    for (i=0;i<Length1;i++)
	printf("-");
    printf(" %s ",s);
    for (i=0;i<Length2;i++)
	printf("-");
    printf("\n\n");
}



/*****************************************************************************
  FUNCTION : cc_getErr
  PURPOSE  : get sum of squared errors (sse) = (o_actual - y_desired)^2
  NOTES    :

  UPDATE   : 19.01.96
******************************************************************************/
float cc_getErr (int StartPattern, int EndPattern)
{
    int p=0, sub, start, end, n,  pat, dummy;
    float sse=0, devit,error;
    register Patterns out_pat;
    register struct Unit *OutputUnitPtr;
    int Correct;
    int WhichWin,CorrWin;
    float MaxAct;

    KernelErrorCode = kr_initSubPatternOrder(StartPattern,EndPattern);
    ERROR_CHECK;
    cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
    ERROR_CHECK;
    SumSqError = 0.0;

    for(p=start; p<=end;p++){
	Correct=TRUE;
	MaxAct=0.0;
	cc_getActivationsForActualPattern(p,start,&pat,&sub);
	PROPAGATE_THROUGH_OUTPUT_LAYER(OutputUnitPtr,dummy,p);

	out_pat = kr_getSubPatData(pat,sub,OUTPUT,NULL);

	FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,dummy){
	    if (*out_pat > 0.5) CorrWin = dummy;
	    devit =  OutputUnitPtr->Out.output - *(out_pat++);
	    if  (OutputUnitPtr->Out.output > MaxAct)
	    {
		MaxAct=OutputUnitPtr->Out.output;
		WhichWin=dummy;
	    }
	    if (abs(devit) > 0.2) Correct=FALSE;
	    sse += devit*devit;
	    error = devit * 
		(((OutputUnitPtr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(OutputUnitPtr->python_act_deriv_func,
						OutputUnitPtr) :
			(OutputUnitPtr->act_deriv_func) (OutputUnitPtr))  + cc_fse);
	    SumSqError += error*error;
	}
    }
    cc_actualNetSaved=TRUE;
    return sse;
}

/*****************************************************************************
  FUNCTION : cc_LayerCorrectnessTest

  PURPOSE  : Tests if the layer data is valid

  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
void cc_LayerCorrectnessTest(float* ParameterInArray,
			     int StartPattern, int EndPattern)
{
    int LayerDataCorrect=TRUE;
    struct Unit *UnitPtr,*UnitPtr2;
    struct Link *LinkPtr;

    FOR_ALL_UNITS(UnitPtr){
	if ((CC_LAYER_NO(UnitPtr)==0)&&(IS_OUTPUT_UNIT(UnitPtr)))
	    LayerDataCorrect=FALSE;
    }

    if (!LayerDataCorrect) {
	cc_calculateNetParameters();
	FOR_ALL_UNITS(UnitPtr){
	    CC_SET_LAYER_NO(UnitPtr,0);
	} 

	NoOfLayers=0;
	/*cc_lastFirstOutputRow=100000;*/

	FOR_ALL_UNITS(UnitPtr){
	    FOR_ALL_LINKS(UnitPtr,LinkPtr){
		UnitPtr2=LinkPtr->to;
		if ((CC_LAYER_NO(UnitPtr2)+1) > CC_LAYER_NO(UnitPtr))
		    CC_SET_LAYER_NO(UnitPtr,CC_LAYER_NO(UnitPtr2)+1);
	    }
	    if(CC_LAYER_NO(UnitPtr)>NoOfLayers)
                NoOfLayers=CC_LAYER_NO(UnitPtr);
	    /*if ((IS_OUTPUT_UNIT(unitPtr))&&
	      (GET_UNIT_XPOS(unitPtr) < cc_lastFirstOutputRow))
	      cc_lastFirstOutputRow =  GET_UNIT_XPOS(unitPtr);*/
	} /* This algorithm works correct, iff u2 is in a layer greater than u1
	     --> NO(U2) > NO(U1) (except output units)*/
 
    }
 
    if (NoOfHiddenUnits<=0) {
	NoOfLayers=0;
	LastInsertedHiddenUnit=0;
    }

    SumSqError=0.0;  /* Recalc SumSqEror later */
}

/*****************************************************************************
  FUNCTION : cc_freeStorage

  PURPOSE  : Frees all the storage allocated by CC.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err cc_freeStorage(int StartPattern,int EndPattern, int flag)
{
    struct Unit *unitPtr;
    struct Link *linkPtr;
    int start, end, n;

    cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
    ERROR_CHECK;

    cc_storageFree = 1;

    if(flag==1){
	(void) cc_deleteAllSpecialAndAllHiddenUnits();
	cc_end = 0;
	FOR_ALL_UNITS(unitPtr){
	    if(UNIT_IN_USE(unitPtr) && IS_OUTPUT_UNIT(unitPtr)){
		unitPtr->value_a =  unitPtr->value_b = unitPtr->value_c = 0;
		FOR_ALL_LINKS(unitPtr,linkPtr){
		    linkPtr->value_a =  linkPtr->value_b = linkPtr->value_c = 0;
		}
	    }
	}
	return(KRERR_NO_ERROR);
    }else{
	FREE_2DIMENSIONAL_ARRAY_WITH_PRINT(OutputUnitError,n,p);
	FREE_2DIMENSIONAL_ARRAY(CorBetweenSpecialActAndOutError,
				cc_MaxSpecialUnitNo,s);
	FREE_2DIMENSIONAL_ARRAY(SpecialUnitAct,n,p);
	FREE_2DIMENSIONAL_ARRAY(ActOfUnit,n,p);
	FREE_IF_USED(MeanOutputUnitError);
	FREE_IF_USED(SpecialUnitSumAct);

	cc_actualNetSaved=FALSE;

	cc_deallocateMemory(); /* deallocate Memory needed by modifications */

	return(KRERR_NO_ERROR);
    }

}

/*****************************************************************************
  FUNCTION : cc_allocateStorage

  PURPOSE  : Allocates all the storage needed by CC
  NOTES    :

  UPDATE   : 19.01.96
******************************************************************************/
krui_err cc_allocateStorage(int StartPattern, int  EndPattern, 
			    int NoOfSpecialUnits)
{
    int p;
    int start, end, n;

    OldNoOfSpecialUnitStorage = NoOfSpecialUnits;
    cc_storageFree = 0;

    cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
    ERROR_CHECK;

    CALLOC_ERRORCHECK(MeanOutputUnitError,NoOfOutputUnits,float);
    CALLOC_ERRORCHECK(SpecialUnitSumAct,NoOfSpecialUnits,float);
    CALLOC_2DIMENSIONAL_ARRAY(OutputUnitError,n,NoOfOutputUnits,float,p);
    CALLOC_2DIMENSIONAL_ARRAY(SpecialUnitAct,n,cc_MaxSpecialUnitNo,float,p);
    CALLOC_2DIMENSIONAL_ARRAY(CorBetweenSpecialActAndOutError,NoOfSpecialUnits,
			      NoOfOutputUnits,float,p);
    if(cc_fastmode){
	CALLOC_2DIMENSIONAL_ARRAY(ActOfUnit,n,
				  (NoOfInputUnits+NoOfHiddenUnits+NO_OF_GROUPS),
				  float,p);
	/* no of groups is set to a value bigger than 1, iff CCS-modification
	   is set */
    }

    return(cc_allocateMemoryForModifications());
}


/*****************************************************************************
  FUNCTION : cc_deleteAllSpecialAndAllHiddenUnits

  PURPOSE  : Deletes all special units and all hidden units.
  NOTES    :

  UPDATE   : 19.01.96
******************************************************************************/
krui_err cc_deleteAllSpecialAndAllHiddenUnits(void)
{
    struct Unit *UnitPtr;

    if(NoOfUnits != 0) {
	FOR_ALL_UNITS(UnitPtr){
	    if((IS_HIDDEN_UNIT(UnitPtr) || IS_SPECIAL_UNIT(UnitPtr)) && 
	       UNIT_IN_USE(UnitPtr)) {
		KernelErrorCode = kr_removeUnit(UnitPtr); 
		ERROR_CHECK;
	    }
	}
	kr_forceUnitGC();
	NoOfHiddenUnits = 0;
	NetModified = 1;
    }
    return(KRERR_NO_ERROR);
} 

 

/*****************************************************************************
  FUNCTION : cc_initActivationArrays

  PURPOSE  : 
  NOTES    :

  UPDATE   : 19.01.96
******************************************************************************/
void cc_initActivationArrays(void)
{
    int s,o;
    struct Unit *outputUnitPtr,*specialUnitPtr;

    FOR_ALL_SPECIAL_UNITS(specialUnitPtr,s) {
	SpecialUnitSumAct[s] = 0.0;
    }

    FOR_ALL_SPECIAL_UNITS(specialUnitPtr,s) {
	FOR_ALL_OUTPUT_UNITS(outputUnitPtr,o) {
	    CorBetweenSpecialActAndOutError[s][o] = 0.0;
	}
    } 
}


/*****************************************************************************
  FUNCTION : cc_generateRandomNo

  PURPOSE  : Generates a random number in the interval [-maxValue,+maxValue]
  NOTES    :

  UPDATE   : 19.01.96
******************************************************************************/
FlintType cc_generateRandomNo(float maxValue)
{
    return (FlintType)(drand48()*2.0*maxValue-maxValue);  
}


  
/*****************************************************************************
  FUNCTION : cc_initOutputUnits

  PURPOSE  : Initializes the links of the output units.
  NOTES    :

  UPDATE   : 19.01.96
******************************************************************************/
void cc_initOutputUnits(void)
{
    struct Unit *outputUnitPtr;
    struct Link *linkPtr;
    int o;

    FOR_ALL_OUTPUT_UNITS(outputUnitPtr,o){
	outputUnitPtr->value_a = 
	    outputUnitPtr->value_b = outputUnitPtr->value_c = 0;
	FOR_ALL_LINKS(outputUnitPtr,linkPtr){
	    linkPtr->value_a =  linkPtr->value_b = linkPtr->value_c = 0;
	}
    }
}


/*****************************************************************************
  FUNCTION : cc_getPatternParameter

  PURPOSE  : Get the Pattern-Parameters needed
  NOTES    :

  UPDATE   : 30.11.95 <Juergen Gatter>
******************************************************************************/
krui_err cc_getPatternParameter(int StartPattern,int EndPattern,
				int* start, int* end,int* n)
{
    KernelErrorCode = kr_initSubPatternOrder(StartPattern,EndPattern);
    ERROR_CHECK;
    *start = kr_AbsPosOfFirstSubPat(StartPattern);
    *end   = kr_AbsPosOfFirstSubPat(EndPattern);
    *end  += kr_NoOfSubPatPairs(EndPattern) - 1;
    *n = *end - *start + 1;
    return (KernelErrorCode);
}


/*****************************************************************************
  FUNCTION : cc_getActivationsForActualPattern

  PURPOSE  : Gets or calculates the activations for the input and
             hidden units.
  NOTES    :

  UPDATE   : 19.01.96
******************************************************************************/
void cc_getActivationsForActualPattern(int SubPatternNo,int First,int* pat,
                                       int* sub)
{
    struct Unit   *UnitPtr;
    Patterns  in_pat;
    int dummy,relPatternNo;
    int Index1,Index2;

    relPatternNo=SubPatternNo-First;

    kr_getSubPatternByNo(pat,sub,SubPatternNo);
    in_pat = kr_getSubPatData(*pat,*sub,INPUT,NULL);
    if((cc_fastmode)&&(cc_actualNetSaved)){
	FOR_ALL_INPUT_UNITS(UnitPtr,Index1){
	    UnitPtr->Out.output = ActOfUnit[relPatternNo][Index1];
	}
     	FOR_ALL_HIDDEN_UNITS(UnitPtr,Index2){
	    UnitPtr->Out.output = UnitPtr->act = 
		ActOfUnit[relPatternNo][Index1+Index2];
	}
    }else{
	PROPAGATE_THROUGH_INPUT_LAYER(UnitPtr,dummy,in_pat);
	PROPAGATE_THROUGH_HIDDEN_LAYER(UnitPtr,dummy,SubPatternNo);
	if(cc_fastmode){  /* then save activations */
	    FOR_ALL_INPUT_UNITS(UnitPtr,Index1){
		ActOfUnit[relPatternNo][Index1] = UnitPtr->Out.output;
	    }
	    FOR_ALL_HIDDEN_UNITS(UnitPtr,Index2){
		ActOfUnit[relPatternNo][Index1+Index2] = UnitPtr->Out.output;
	    }
	}
    }
}


/*****************************************************************************
  FUNCTION : cc_setPointers

  PURPOSE  : Calculates the beginning of the input, hidden, output and special
             units in the topo_ptr_array. 
  NOTES    :

  UPDATE   : 19.01.96
******************************************************************************/
krui_err cc_setPointers(void)
{
    FirstInputUnitPtr   = (struct Unit **)(&topo_ptr_array[1]);
    if(*(FirstInputUnitPtr-1)!=NULL) CC_ERROR(KRERR_CC_ERROR1);
    FirstHiddenUnitPtr  = FirstInputUnitPtr  + NoOfInputUnits  + 1;
    if(*(FirstHiddenUnitPtr-1)!=NULL) CC_ERROR(KRERR_CC_ERROR1);
    FirstOutputUnitPtr  = FirstHiddenUnitPtr + NoOfHiddenUnits + 1;
    if(*(FirstOutputUnitPtr-1)!=NULL) CC_ERROR(KRERR_CC_ERROR1);
    FirstSpecialUnitPtr = FirstOutputUnitPtr + NoOfOutputUnits + 1;    
    if(*(FirstSpecialUnitPtr-1)!=NULL) CC_ERROR(KRERR_CC_ERROR1);
    return(KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : cc_initSpecialUnitLinks

  PURPOSE  : Sets all the links of the special units to zero.
  NOTES    :

  UPDATE   : 19.01.96
******************************************************************************/
krui_err cc_initSpecialUnitLinks(void)
{
    int s;
    struct Unit *SpecialUnitPtr;
    struct Link *LinkPtr;

    FOR_ALL_SPECIAL_UNITS(SpecialUnitPtr,s) {
	SpecialUnitPtr->bias = 0.0;
	BIAS_CURRENT_SLOPE(SpecialUnitPtr) = 0.0; 
	BIAS_PREVIOUS_SLOPE(SpecialUnitPtr) = 0.0; 
	BIAS_LAST_WEIGHT_CHANGE(SpecialUnitPtr) = 0.0;
	FOR_ALL_LINKS(SpecialUnitPtr,LinkPtr) {
	    LinkPtr->weight = cc_generateRandomNo(CC_MAX_VALUE);
	    LN_CURRENT_SLOPE(LinkPtr) = 0.0;
	    LN_PREVIOUS_SLOPE(LinkPtr) = 0.0;
	    LN_LAST_WEIGHT_CHANGE(LinkPtr) = 0.0;
	}
    }
    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : cc_deleteAllSpecialUnits

  PURPOSE  : Deletes all special units.
  NOTES    :

  UPDATE   : 19.01.96
******************************************************************************/
krui_err cc_deleteAllSpecialUnits(void)
{
    struct Unit *UnitPtr;

    if(NoOfUnits != 0) {
	FOR_ALL_UNITS(UnitPtr){
	    if(IS_SPECIAL_UNIT(UnitPtr) && UNIT_IN_USE(UnitPtr)){
		KernelErrorCode = kr_removeUnit(UnitPtr); 
		ERROR_CHECK;
	    }
	}
	kr_forceUnitGC();
	NetModified = 1;
    }
    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : QuickPropOfflinePart

  PURPOSE  : Update the weights ( or bias ) in QuickProp-Style
  NOTES    :

  UPDATE   : extracted 19.1.96 Juergen Gatter
******************************************************************************/
float QuickPropOfflinePart(float oldValue, float* previousSlope,
			   float* currentSlope, float* LastChange,
			   float epsilon, float mu, float decay)
{
    float current,change;

    current = *currentSlope + decay * oldValue;
    if(*previousSlope == 0.0){
	change = -epsilon*current;
    }else{
	if(current*(SGN(*previousSlope)) >= (mu/(mu+1))*fabs(*previousSlope)){
	    change = mu * (*LastChange);
	}else{
	    change = *LastChange * current / (*previousSlope-current);
	}
	if (SGN(*previousSlope)==SGN(current)){
	    change -= epsilon * current;
	}
    }
    *previousSlope =  current;
    *currentSlope  =  0.0;
    return (*LastChange = change);
}


/*****************************************************************************
  FUNCTION : RPropOfflinePart

  PURPOSE  : Update the weights ( or bias ) in QuickProp-Style
  NOTES    : 

  UPDATE   : extracted 19.1.96 Juergen Gatter
******************************************************************************/
float RPropOfflinePart(float oldValue,float* previousSlope, float* currentSlope,
		       float* LastChange, float epsilonMinus, 
		       float epsilonPlus,float dummy)
{
    float change,lastChange;

    change = 0;
    lastChange = (*LastChange == 0.0) ? 1.0 : *LastChange;
    if (*currentSlope != 0.0){ 
	if (*previousSlope == 0.0){
	    change = fabs(lastChange) * SIGN(*currentSlope);
	}else if (*previousSlope > 0.0){
	    change = 
		((*currentSlope>0.0)? epsilonPlus : -epsilonMinus) * lastChange;
	}else{
	    change = 
		((*currentSlope<0.0)? epsilonPlus : -epsilonMinus) * lastChange;
	}
	if (fabs(change) < 0.00001) change = 0.00001 * SIGN(change);
	if (fabs(change) > 10.0   ) change = 10.0    * SIGN(change);
    }
    *previousSlope = *currentSlope;
    *currentSlope  =  0.0;
    *LastChange = change;
    return (-change);
}

/*****************************************************************************
  FUNCTION : BackPropOfflinePart

  PURPOSE  : Update the weights ( or bias ) in BackProp
  NOTES    : This one is used by rcc.

  UPDATE   : extracted 19.1.96 Juergen Gatter
******************************************************************************/
float BackPropOfflinePart(float oldValue, float* previousSlope,
			  float* currentSlope, float* LastChange,
			  float eta, float mu, float dummy)
{
    float change;

    *LastChange = change = -(*currentSlope * eta + *LastChange * mu);

    *previousSlope = *currentSlope;
    *currentSlope = 0.0;
    return(change);

}
          
/*****************************************************************************
  FUNCTION : OnlineBackPropPropOfflinePart

  PURPOSE  : Update the weights ( or bias ) in BackProp (online)
  NOTES    : 

  UPDATE   : extracted 19.1.96 Juergen Gatter
******************************************************************************/
float OnlineBackPropOfflinePart(float oldValue, float* previousSlope,
				float* currentSlope, float* LastChange,
				float eta, float mu, float dummy)
{  
    return(0.0);
}
          


/*****************************************************************************
  FUNCTION : cc_setCycletestFlag(struct Unit* UnitPtr)

  PURPOSE  : Sets CycletestFlag to 1
  NOTES    :

  UPDATE   : 22.11.95 (Juergen Gatter)
******************************************************************************/
void cc_setCycletestFlag(struct Unit* UnitPtr)
{
  if (UnitPtr->lln >= 0)
     UnitPtr->lln = (-1)-UnitPtr->lln;
}

/*****************************************************************************
  FUNCTION : cc_testCycletestFlag(struct Unit* UnitPtr)

  PURPOSE  : test whether CycletestFlag is 1
  NOTES    :

  UPDATE   : 22.11.95 (Juergen Gatter)
******************************************************************************/
static bool cc_testCycletestFlag(struct Unit* UnitPtr)
{
  return (UnitPtr->lln >= 0);
}

/*****************************************************************************
  FUNCTION : cc_clearAllCycletestFlags(void)

  PURPOSE  : resets the CycletestFlag to 0.
  NOTES    : 

  UPDATE   : 22.11.95 (Juergen Gatter)
******************************************************************************/
static void cc_clearAllCycletestFlags(void)
{
  struct Unit* UnitPtr;

  FOR_ALL_UNITS(UnitPtr) {
    if (UnitPtr->lln < 0)
       UnitPtr->lln = (-1) - UnitPtr->lln;
  }
}


/*****************************************************************************
  FUNCTION : cc_clearFlags

  PURPOSE  : Clears all CC flags.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void cc_clearFlags(void)
{
    struct Unit *unitPtr;

    cc_clearAllCycletestFlags();

    FOR_ALL_UNITS(unitPtr){
	if(UNIT_IN_USE(unitPtr)){
	    unitPtr->flags &= ~UFLAG_REFRESH;
	    LINKS_LEAVING(unitPtr) = 0;
	    LINKS_ARRIVEING(unitPtr) = 0;
	    INPUT_LINKS(unitPtr) = 0;
	}
    }
} 


/*****************************************************************************
  FUNCTION : DepthFirst4

  PURPOSE  : Depth search routine for topological sorting.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void  DepthFirst4(struct Unit *unit_ptr, int depth )
{

    struct Site   *site_ptr;
    struct Link   *link_ptr;

    if (unit_ptr->flags & UFLAG_REFRESH){
	/*  the 'touch' flag is set: don't continue search  */
	topo_msg.src_error_unit = unit_ptr - unit_array; /* store unit number */

	if IS_OUTPUT_UNIT( unit_ptr ){
	    /*  this output unit has a output connection to another unit  */
	    if (topo_msg.error_code == KRERR_NO_ERROR)
		topo_msg.error_code = KRERR_O_UNITS_CONNECT;
	}else if (cc_testCycletestFlag(unit_ptr)){
	    /*  logical layer no. isn't set => Cycle found  */
	    topo_msg.no_of_cycles++;
	    if (topo_msg.error_code == KRERR_NO_ERROR)
		topo_msg.error_code = KRERR_CYCLES;
	}

	return;
    }else
	/*  set the 'touch' flag  */
	unit_ptr->flags |= UFLAG_REFRESH;

    switch (unit_ptr->flags & UFLAG_INPUT_PAT){
    
    case  UFLAG_DLINKS:   /*  unit has direct links  */
	FOR_ALL_LINKS(unit_ptr,link_ptr){
	    DepthFirst4( link_ptr->to, depth + 1 );  /*  increase depth  */
	    if(IS_INPUT_UNIT(link_ptr->to)){
		INPUT_LINKS(unit_ptr)++;
	    }
	    if((IS_HIDDEN_UNIT(link_ptr->to)) && (IS_HIDDEN_UNIT(unit_ptr))){
		LINKS_LEAVING(link_ptr->to)++;
		LINKS_ARRIVEING(unit_ptr)++;
	    }
	}
	break;
    case  UFLAG_SITES:  /*  unit has sites  */
	FOR_ALL_SITES_AND_LINKS(unit_ptr,site_ptr, link_ptr) {
	    DepthFirst4( link_ptr->to, depth + 1 );  /*  increase depth  */
	    if(IS_INPUT_UNIT(link_ptr->to)){
		INPUT_LINKS(unit_ptr)++;
	    }
	    if((IS_HIDDEN_UNIT(link_ptr->to)) && (IS_HIDDEN_UNIT(unit_ptr))){
		LINKS_LEAVING(link_ptr->to)++;
		LINKS_ARRIVEING(unit_ptr)++;
	    }
	}
	break;
    }

    /*  remember the depth (for cycle detection and statistics)  */
    cc_setCycletestFlag(unit_ptr);

    /*  store only hidden units  */
    if IS_HIDDEN_UNIT( unit_ptr )
	*global_topo_ptr++ = unit_ptr;  /*  store sorted unit pointer  */
}



/*****************************************************************************
  FUNCTION : DepthFirst5

  PURPOSE  : Depth search routine for topological sorting.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void  DepthFirst5(struct Unit *unit_ptr, int depth )
{
    struct Site   *site_ptr;
    struct Link   *link_ptr;

    if (unit_ptr->flags & UFLAG_REFRESH){
	/* the 'touch' flag is set: don't continue search  */
	topo_msg.src_error_unit = unit_ptr - unit_array; /* store unit number */

	if IS_OUTPUT_UNIT( unit_ptr ){
	    /*  this output unit has a output connection to another unit  */
	    if (topo_msg.error_code == KRERR_NO_ERROR)
		topo_msg.error_code = KRERR_O_UNITS_CONNECT;
	}else if (cc_testCycletestFlag(unit_ptr)){
	    /*  logical layer no. isn't set => Cycle found  */
	    topo_msg.no_of_cycles++;
	    if (topo_msg.error_code == KRERR_NO_ERROR)
		topo_msg.error_code = KRERR_CYCLES;
	}

	return;
    }else
	/*  set the 'touch' flag  */
	unit_ptr->flags |= UFLAG_REFRESH;

    switch (unit_ptr->flags & UFLAG_INPUT_PAT){
    
    case  UFLAG_DLINKS:   /*  unit has direct links  */
	FOR_ALL_LINKS(unit_ptr,link_ptr) {
	    if((IS_HIDDEN_UNIT(unit_ptr)) && (link_ptr->to == unit_ptr)) {
		/* RCC code deleted */
	    }else{ 
		DepthFirst5( link_ptr->to, depth + 1 );  /*  increase depth  */
		if(IS_INPUT_UNIT(link_ptr->to)){
		    INPUT_LINKS(unit_ptr)++;
		}
		if((IS_HIDDEN_UNIT(link_ptr->to))&&(IS_HIDDEN_UNIT(unit_ptr))){
		    LINKS_LEAVING(link_ptr->to)++;
		    LINKS_ARRIVEING(unit_ptr)++;
		}
	    }
	}
	break;
    case  UFLAG_SITES:  /*  unit has sites  */
	FOR_ALL_SITES_AND_LINKS(unit_ptr,site_ptr, link_ptr) {
	    if((IS_HIDDEN_UNIT(unit_ptr)) && (link_ptr->to == unit_ptr)) {
		/* RCC code deleted */
	    }else{ 
		DepthFirst5( link_ptr->to, depth + 1 );  /*  increase depth  */
		if(IS_INPUT_UNIT(link_ptr->to)){
		    INPUT_LINKS(unit_ptr)++;
		}
		if((IS_HIDDEN_UNIT(link_ptr->to))&&(IS_HIDDEN_UNIT(unit_ptr))){
		    LINKS_LEAVING(link_ptr->to)++;
		    LINKS_ARRIVEING(unit_ptr)++;
		}
	    }
	}
	break;
    }

    cc_setCycletestFlag(unit_ptr);

    /*  store only hidden units  */
    if IS_HIDDEN_UNIT( unit_ptr )
	 *global_topo_ptr++ = unit_ptr;  /*  store sorted unit pointer  */
}


/*****************************************************************************
  FUNCTION : cc_topoSort

  PURPOSE  : Calls the Main routine for topological sorting for CC
             and clears all cycletestFlags.

  UPDATE   : 5.2.93
******************************************************************************/
krui_err cc_topoSort(int topoSortID)
{
    int Error;

    Error = cc_topoSortMain(topoSortID);
    cc_clearAllCycletestFlags();
    return (Error);
}



/*****************************************************************************
  FUNCTION : cc_topoSortMain

  PURPOSE  : Main routine for topological sorting for CC.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err cc_topoSortMain(int topoSortId)
{
    register struct Unit   *unit_ptr;
    int   io_units,h,counter=0;
  
    KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */
    if(topoSortId == TOPOLOGICAL_CC) {
	cc_clearFlags();    /*  reset units 'touch' flags  */
    }

    global_topo_ptr = topo_ptr_array;  /*  initialize global pointer */

    /*  limit left side of the topological array with NULL pointer  */
    *global_topo_ptr++ = NULL;

    /*  put all input units in the topologic array  */
    io_units = 0;
    FOR_ALL_UNITS( unit_ptr ) 
	if (IS_INPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr )){
	    if UNIT_HAS_INPUTS( unit_ptr ){
		/*  this input unit has a connection to another unit  */
		topo_msg.dest_error_unit = unit_ptr - unit_array;  
		
		KernelErrorCode = KRERR_I_UNITS_CONNECT;  
		return( KernelErrorCode );
	    }

	    io_units++;       /*  there is a input unit  */
	    *global_topo_ptr++ = unit_ptr;  /*  save input unit  */
	}
  
    if ((NoOfInputUnits = io_units) == 0){
	/*  no input units */
	KernelErrorCode = KRERR_NO_INPUT_UNITS;
	return( KernelErrorCode );
    }

    /*  limit input units in the topological array with NULL pointer  */
    *global_topo_ptr++ = NULL;

    /*  begin depth search at the first output unit  */
    io_units = 0;
    FOR_ALL_UNITS( unit_ptr )
	if (IS_OUTPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr )){
	    io_units++;       /*  there is a output unit  */
	    if(topoSortId == TOPOLOGICAL_CC){
		DepthFirst4( unit_ptr, 1 );
	    }
	    else { /* topoSortId == TOPOLOGICAL_BCC */ 
		DepthFirst4(unit_ptr,1);
	    }      
	    if (topo_msg.error_code != KRERR_NO_ERROR){
		/*  stop if an error occured  */
		KernelErrorCode = topo_msg.error_code;
		return( KernelErrorCode );
	    }
	}


    if ((NoOfOutputUnits = io_units) == 0){
	/*  no output units */
	KernelErrorCode = KRERR_NO_OUTPUT_UNITS;
	return( KernelErrorCode );
    }

    /*  limit hidden units in the topological array with NULL pointer  */
    *global_topo_ptr++ = NULL;

    /*  put all output units in the topological array  */
    FOR_ALL_UNITS( unit_ptr ) 
	if (IS_OUTPUT_UNIT(unit_ptr ) && UNIT_IN_USE( unit_ptr ))
	    *global_topo_ptr++ = unit_ptr;  /*  save output unit  */

    /*  limit right side of the topologic array with NULL pointer  */
    *global_topo_ptr++ = NULL;
  
    FOR_ALL_UNITS( unit_ptr ) {
	if (IS_SPECIAL_UNIT(unit_ptr) && UNIT_IN_USE( unit_ptr )) {
	    *global_topo_ptr++ = unit_ptr;  /*  save output unit  */
	    unit_ptr->flags |= UFLAG_REFRESH;
	}
    }
    /*  limit right side of the topologic array with NULL pointer  */
    *global_topo_ptr++ = NULL;

    /*  calc. no. of sorted units  */
    no_of_topo_units = (global_topo_ptr - topo_ptr_array) - 5;

    /*  search for dead units i.e. units without inputs  */
    FOR_ALL_UNITS( unit_ptr )
	if (!(unit_ptr->flags & UFLAG_REFRESH) && UNIT_IN_USE( unit_ptr )){
	    topo_msg.no_of_dead_units++;
	    if (topo_msg.src_error_unit == 0)
		topo_msg.src_error_unit = unit_ptr - unit_array;  
	}

    if (topo_msg.no_of_dead_units != 0)
	/* KernelErrorCode = KRERR_DEAD_UNITS;
	 * allowed for compression */
	if(KernelErrorCode == KRERR_NO_ERROR){
	    FirstHiddenUnitPtr = (struct Unit **)(&topo_ptr_array[1]) + 
		                 NoOfInputUnits + 1;
	    FOR_ALL_HIDDEN_UNITS(unit_ptr,h){
		switch(topoSortId){
		case TOPOLOGICAL_CC : 
		    break;
		case TOPOLOGICAL_BCC : 
		    if((LINKS_LEAVING(unit_ptr)+LINKS_ARRIVEING(unit_ptr)+1) !=
		       NoOfHiddenUnits) {
			KernelErrorCode = KRERR_CC_ERROR6;
			return(KernelErrorCode);
		    }
		    if(LINKS_ARRIVEING(unit_ptr) != counter++) {
			KernelErrorCode = KRERR_CC_ERROR6;
			return(KernelErrorCode);
		    }
		    if(counter == NoOfHiddenUnits) {
			counter = 0;
		    }
		    break;
		}  
	    }
	}
    return( KernelErrorCode );
}







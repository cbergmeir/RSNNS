/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/tacoma_learn.c,v $
  SHORTNAME      : tacoma_learn.c
  SNNS VERSION   : 4.2

  PURPOSE        : Learn-Functions of Tacoma
  NOTES          : For informations about the algorithms see one of
                   the following papers:

                   J.M. Lange, H.M. Voigt, D. Wolf: 
                   "Growing Artificial Neural Networks Based on
                    Correlation Measures, Task Decomposition and Local 
                    Attention Neurons."
                   IEEE '94, pp. 1355-1358.

                   J.M. Lange, H.M. Voigt, D. Wolf: 
                   "Task Decomposition and Correlations in Growing
                     Artificial Neural Networks."
                   ICANN '94, pp. 735-738.

                   J. Gatter:
                   "Lernverfahren neuronaler Netze mit automatischer
                    Bestimmung der Netzwerktopologie"
                   Diplomarbeit Nr. 1337, university of Stuttgart


  AUTHOR         : Juergen Gatter
  DATE           : October 1995 - March 1996

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.10 $
  LAST CHANGE    : 

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
#include "cc_glob.h"
#include "cc_display.h"
#include "kr_newpattern.h"
#include "cc_learn.ph"
#include "tacoma_learn.ph"



/*****************************************************************************
  FUNCTION : tac_testCorrectnessOfAddParameters

  PURPOSE  : 
      tests correctness of additional parameters. Correct values are :
      [0] TAC_KOHONEN             [0    .. inf)
      [1] TAC_XI_RI_ETA           [0.0  .. inf)
      [2] TAC_THRESHOLD           (-inf .. 1.0)
      [3] TAC_LAMBDA              (-inf .. inf)
      [4] TAC_BETA                (0.0  .. 1.0)

  NOTES    :

      if parameter setting is incorrect KRERR_CC_INVALID_ADD_PARAMETERS
      will be returned tests correctness of additional parameters

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
krui_err tac_testCorrectnessOfAddParameters(void)
{
   if ((TAC_KOHONEN < 0) || (TAC_XI_RI_ETA < 0.0 ) || (TAC_THRESHOLD >= 1.0) ||
       (TAC_BETA<=0.0) || (TAC_BETA>=1.0))
        return (KRERR_CC_INVALID_ADD_PARAMETERS);

   return(KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : tac_initVariables

  PURPOSE  :
      Initializes global variables, read the Parameters, sorts the units, 
      assigns the learning routines, generates the Layerlist, checks 
      the parameters and calculates the initial display-Parameters.
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/


krui_err tac_initVariables(float* ParameterInArray,
				int StartPattern,int EndPattern)
{
   int i;

   cc_LayerCorrectnessTest(ParameterInArray,StartPattern,EndPattern);

   srand48((long)time(NULL)); 
	      /* inits the random-generator */
   cc_printOnOff = (int)ParameterInArray[8]; 
   cc_backfittingOnOff = (int)ParameterInArray[18];
   cc_MaxSpecialUnitNo = (int)ParameterInArray[12];
   cc_modification = (int)ParameterInArray[21];
   for (i=0;i<5;i++){
     cc_Parameter[i]=ParameterInArray[22+i];
   }
   cc_fastmode=(int)ParameterInArray[27];
   cc_end=0;
   KernelErrorCode=tac_testCorrectnessOfAddParameters();
   ERROR_CHECK;

   /* take the outputpart and overwrite the special part */

   cc_propagateSpecialUnitsBackward = tac_propagateSpecial;
   cc_propagateOutputUnitsBackward  = cc_propagateOutput;
 
   switch(LEARNING_FUNCTION) { 
     case BACKPROP: 
       cc_SpecialUnitUpdate = 
               cc_OutputUnitUpdate = BackPropOfflinePart;
     break; 
     case BACKPROP_ONLINE:
       cc_SpecialUnitUpdate =
               cc_OutputUnitUpdate = OnlineBackPropOfflinePart;
       cc_propagateOutputUnitsBackward  = cc_propagateOutputOnlineCase;
       cc_propagateSpecialUnitsBackward = tac_propagateSpecialOnlineCase;
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
   KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
   ERROR_CHECK;    /* Sort the Units */
   cc_setPointers(); 
   if (NoOfHiddenUnits<=0) 
   {    
     KernelErrorCode = 
       cc_calculateNetParameters();
     ERROR_CHECK;
   }

   KernelErrorCode = cc_generateLayerList();
   ERROR_CHECK;    /* build the array with the dates of the Layers */ 
 
   return KRERR_NO_ERROR;
}



/*****************************************************************************
  FUNCTION : tac_allocateStorage

  PURPOSE  :  
      allocates the first part of the storage used by TACOMA. Tacoma
      uses much more Arrays than for example Cascade-Correlation.
      The Second Part of the Arrays will be allocated later, when
      the number of Units to be inserted, is calculated
  NOTES    : 
      the second part of the needed storage will be allocated
      when then no of new units is calculated 

  UPDATE   : 30.03.96
******************************************************************************/

 krui_err tac_allocateStorage(int StartPattern, int  EndPattern)
{
  int start,end,n;
  int p,i;


  cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
  ERROR_CHECK;

  CALLOC_2DIMENSIONAL_ARRAY(SpecialUnitAct,n,cc_MaxSpecialUnitNo,float,p);
  CALLOC_2DIMENSIONAL_ARRAY(OutputUnitError,n,NoOfOutputUnits,float,p);
  CALLOC_2DIMENSIONAL_ARRAY
     (CorBetweenSpecialActAndOutError,cc_MaxSpecialUnitNo,NoOfOutputUnits,float,p);

  CALLOC_ERRORCHECK(SpecialUnitSumAct,cc_MaxSpecialUnitNo,float);
  MeanYi=SpecialUnitSumAct; 
  CALLOC_ERRORCHECK(MeanOutputUnitError,NoOfOutputUnits,float);
  CALLOC_ERRORCHECK(PatternSumError,n,float);
  CALLOC_ERRORCHECK(SpecialUnitData,cc_MaxSpecialUnitNo,TAC_SPECIAL_UNIT_TYPE);
  CALLOC_2ND_ARRAY(SpecialUnitData,cc_MaxSpecialUnitNo,Ri,
		   NoOfInputUnits,float,i);
  CALLOC_2ND_ARRAY(SpecialUnitData,cc_MaxSpecialUnitNo,Xi,
		   NoOfInputUnits,float,i);
  CALLOC_2ND_ARRAY(SpecialUnitData,cc_MaxSpecialUnitNo,LinkError,
	 NoOfInputUnits+NoOfHiddenUnits+cc_MaxSpecialUnitNo,TAC_LINK_ERROR_TYPE,i);
  if(cc_fastmode){
    CALLOC_2DIMENSIONAL_ARRAY
     (ActOfUnit,n,NoOfInputUnits+NoOfHiddenUnits+cc_MaxSpecialUnitNo,float,p);
  }
  return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : tac_freeStorage

  PURPOSE  :
      Deallocates all the storage needed by TACOMA
  NOTES    : 
      Between two Learning-Cycles no add. memory should be held by TACOMA

  UPDATE   : 30.03.96 <Juergen Gatter>
 *****************************************************************************/

 krui_err tac_freeStorage(int StartPattern, int  EndPattern)
{
  int start,end,n;

  cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
  ERROR_CHECK;

  FREE_IF_USED(PatternSumError);

  FREE_2ND_ARRAY(SpecialUnitData,cc_MaxSpecialUnitNo,LinkError,i);
  FREE_2ND_ARRAY(SpecialUnitData,cc_MaxSpecialUnitNo,Xi,i);
  FREE_2ND_ARRAY(SpecialUnitData,cc_MaxSpecialUnitNo,Ri,i);

  FREE_2DIMENSIONAL_ARRAY(Rij,NoOfInstalledUnits,i);
  FREE_2DIMENSIONAL_ARRAY(Nij,NoOfInstalledUnits,i);

  FREE_IF_USED(SpecialUnitData);

  FREE_2DIMENSIONAL_ARRAY(PrimesOfSpecialUnits,NoOfInstalledUnits,i);

  return(cc_freeStorage(StartPattern,EndPattern,0));
}

/*****************************************************************************
  FUNCTION : tac_calculateOutputUnitError

  PURPOSE  : 
      Calculates the error of all output units and stores the result
      in the array OutputUnitError. Additionaly the Arrays 
      MeanOutputUnitError and PatternSumError are calculated here. 
      And the WholeSummedError, too.
      PSE und WSE are using fabs(act-teach), whereas the others
      are using act-teach.
      The routine is similar to the one in cc_learn.c.
  NOTES    : OUTPUT_UNIT_SUM_ERROR is only used temporarly

  UPDATE   : 30.3.96
******************************************************************************/
 krui_err tac_calculateOutputUnitError(int StartPattern,int EndPattern)
{
    register struct Unit *UnitPtr;
    register Patterns  out_pat;
    register int o,p;
    int start, end,pat,sub,n;
    float devit;
  
    cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
    ERROR_CHECK;     

    for(p=start; p<=end;p++){
	PatternSumError[p] = 0.0;
	cc_getActivationsForActualPattern(p,start,&pat,&sub);
	      /* propagate through in and hidden-Layers */
	out_pat = kr_getSubPatData(pat,sub,OUTPUT,NULL);
	ERROR_CHECK;/* get Pattern-Data for Output-Units */
	FOR_ALL_OUTPUT_UNITS(UnitPtr,o) {
	    CALCULATE_ACTIVATION_AND_OUTPUT(UnitPtr,
		      (*UnitPtr->act_func)(UnitPtr),p);
	      /* Propagate through Output-Layer */
	    devit =  UnitPtr->Out.output - *(out_pat++);
	      /* difference between actual and Should be */
	    OUTPUT_UNIT_SUM_ERROR[o] += 
	        (OutputUnitError[p][o] =  devit) ;
	    PatternSumError[p] += fabs(OutputUnitError[p][o]);
	      /* And calculate the summed Error for this Pattern */
	}
    }
    WholeSummedError=0.0;
    for(p=start;p<=end;p++)
        WholeSummedError += PatternSumError[p];

    FOR_ALL_OUTPUT_UNITS(UnitPtr,o){
	MeanOutputUnitError[o] = (OUTPUT_UNIT_SUM_ERROR[o] / n);
    }
    cc_actualNetSaved=TRUE;
  return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION :  tac_connect(int NewUnitNo,struct Unit* OldUnit,int StartPattern,int EndPattern)

  PURPOSE  :  returns true, iff s and OldUnit have to be connected.
	      If the oldUnit is a input unit then it's clear,
              if it is a hidden Unit,
	      then the two window functions have to have a siginificant
	      overlap. 
  NOTES    :  NewUnitNo is not GET_UNIT_NO(NewUnit). It's the internal number
              as used in the kohonen-mapping.
  
  UPDATE   :  30.3.96
******************************************************************************/
 bool tac_connect(int s,struct Unit* OldUnit,
		   int StartPattern,int EndPattern,float* Correlation)
{
  int start,end,n,p,pat,sub,k;
  Patterns in_pat;
  float SumFirst,SumSecond,First,Second,devit,devit2,SumZaehler,Zaehler;
  struct Link* LinkPtr;
  int UnitNo;
  
  if (krui_getUnitActFuncName(GET_UNIT_NO(OldUnit)) != "ACT_TACOMA") return (TRUE);
  /* in/output or with CC calculated units */
  /* Now start with connection routing */
  cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
  if(KernelErrorCode!=KRERR_NO_ERROR) {
      return 0;
  }

  SumFirst = SumSecond = SumZaehler = 0.0;

  for(p=start;p<=end;p++){
     kr_getSubPatternByNo(&pat,&sub,p);
     in_pat =  kr_getSubPatData(pat,sub,INPUT,NULL);
     First = Second = Zaehler = 0.0; 
     FOR_ALL_LINKS(OldUnit,LinkPtr) {
	if (IS_INPUT_UNIT(LinkPtr->to)){
           UnitNo=GET_UNIT_NO(LinkPtr->to);
   	   k = (NoOfInputUnits - UnitNo);  /* the patterns are stored backw. */
	   devit  = (in_pat[k] - SpecialUnitData[s].Xi[k]) / SpecialUnitData[s].Ri[k];
	   devit2 = (in_pat[k] - XI_OF_LINK(LinkPtr)) / RI_OF_LINK(LinkPtr);
	   First  += (devit*devit);
	   Second += (devit2*devit2);
	}
     }
     SumFirst   += (TAC_EXP(-First) *TAC_EXP(-First));
     SumSecond  += (TAC_EXP(-Second)*TAC_EXP(-Second));
     SumZaehler += (TAC_EXP(-First) *TAC_EXP(-Second));
  }     

  *Correlation = ((SumSecond>0.0) ? SumZaehler/sqrt(SumFirst*SumSecond) : 0.0 );

  return(*Correlation>TAC_LAMBDA);
}
/*****************************************************************************
  FUNCTION : tac_initWindowFuncParameter(struct Unit* UnitPtr)

  PURPOSE  : Link values of xi and ri are copied from the internal array
  NOTES    : UnitNo is the internal number of the unit

  UPDATE   : 30.03.96
******************************************************************************/
 void tac_initWindowFuncParameter(struct Unit* UnitPtr,int UnitNo)
{
  struct Link * LinkPtr;
  int InpUnitNo;

   FOR_ALL_LINKS(UnitPtr,LinkPtr){
      if(IS_INPUT_UNIT(LinkPtr->to)){
	 InpUnitNo = GET_UNIT_NO(LinkPtr->to)-1;
            XI_OF_LINK(LinkPtr) = SpecialUnitData[UnitNo].Xi[InpUnitNo];
            RI_OF_LINK(LinkPtr) = SpecialUnitData[UnitNo].Ri[InpUnitNo];
      }
      else{
	 XI_OF_LINK(LinkPtr) = RI_OF_LINK(LinkPtr) = 0.0;
      }
   }
}


/*****************************************************************************
  FUNCTION : tac_generateNewUnit   

  PURPOSE  : 
      Builds a new special unit. The new unit is declared as a special
      unit to make the old routines of cc_learn/cc_rcc useable.
      The routine does the following things :  
         Set type to SPECIAL
         Set activition function to ACT_TACOMA
         Actualizes the Layerlist and the LayerNo of the Unit
         Sets the display (like a hidden unit in CC)
         Generates the links between the input/ older hidden units
              and the new one.
         Sets Xi and Ri (see above)
  NOTES    :

  UPDATE   : 30.03.96
******************************************************************************/
krui_err tac_generateNewUnit(int UnitNo,int LayerNo,int StartPattern,int EndPattern)
{
   struct Unit* NewUnitPtr;
   struct Unit* UnitPtr;
   struct Link* NewLink;
   int CurrentUnit;
   float Correlation;

   KernelErrorCode = 
      kr_unitSetTType(CurrentUnit=kr_makeDefaultUnit(),SPECIAL); 
   ERROR_CHECK;
   KernelErrorCode = 
      krui_setUnitActFunc(CurrentUnit,"Act_TACOMA");
   ERROR_CHECK;  
   NewUnitPtr = kr_getUnitPtr(CurrentUnit); 
   ERROR_CHECK;
   KernelErrorCode = krui_setCurrentUnit(CurrentUnit); 
   ERROR_CHECK;
   KernelErrorCode =
     cc_actualizeLayerlist(NewUnitPtr,LayerNo);
   CC_SET_LAYER_NO(NewUnitPtr,NoOfLayers);
   cc_setHiddenUnit(NewUnitPtr,NoOfLayers);

   FOR_ALL_UNITS(UnitPtr){
      if((IS_INPUT_UNIT(UnitPtr) ||  IS_HIDDEN_UNIT(UnitPtr)) &&
	 UNIT_IN_USE(UnitPtr) &&
	 (CC_LAYER_NO(UnitPtr)<NoOfLayers)
	 &&(tac_connect(UnitNo,UnitPtr,StartPattern,EndPattern,&Correlation))) 
         {
            ERROR_CHECK;
            NewLink=kr_createLinkWithAdditionalParameters(GET_UNIT_NO(UnitPtr),
	      cc_generateRandomNo(TAC_MAX_VALUE), 0.0, 0.0, 0.0); 
            ERROR_CHECK;
            if(!IS_INPUT_UNIT(UnitPtr)){
               PRINTF(
               "Link from unit %2d to unit %2d added. Correlation between the units is %5.3f\n",
               GET_UNIT_NO(UnitPtr),
               GET_UNIT_NO(NewUnitPtr),Correlation);
            }
         }
      ERROR_CHECK;
   }

   tac_initWindowFuncParameter(NewUnitPtr,UnitNo);
   
   return(KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : tac_initXiAndRis

  PURPOSE  : 
      Initialises the Array Xi with values 
      (sum(x_i)/n_p) + random*(max(x_i)-min(x_i). -0.1 < Random < 0.1
      Ri with 0.0
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/

krui_err tac_initXiAndRis(StartPattern,EndPattern)
{
  int p,i;
  int start,end,n,pat,sub;
  float* MaxXI,*MinXI,*SumXI;
  float Xi;
  Patterns in_pat;

  cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);

  CALLOC_ERRORCHECK(MaxXI,NoOfInputUnits,float);
  CALLOC_ERRORCHECK(MinXI,NoOfInputUnits,float);
  CALLOC_ERRORCHECK(SumXI,NoOfInputUnits,float);
 
  for(i=0;i<NoOfInputUnits;i++)
  {
     MaxXI[i]=-100000.0; MinXI[i]=10000.0; SumXI[i]=0.0;
  }

  cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
  for(p=start;p<=end;p++){
     kr_getSubPatternByNo(&pat,&sub,p);
     in_pat =  kr_getSubPatData(pat,sub,INPUT,NULL);
     for(i=0;i<NoOfInputUnits;i++){
	  if(*in_pat > MaxXI[i]) MaxXI[i]=*in_pat;
	  if(*in_pat < MinXI[i]) MinXI[i]=*in_pat;
          SumXI[i] += *in_pat;
          in_pat++;
     }
  }
  for(p=0;p<cc_MaxSpecialUnitNo;p++){
     for(i=0;i<NoOfInputUnits;i++)
     { 
        Xi=SumXI[i]/n +  cc_generateRandomNo((MaxXI[i]-MinXI[i]) * 0.1);
	SpecialUnitData[p].Xi[i]=Xi;
	SpecialUnitData[p].Ri[i]=0.0;
     }
  }
  FREE_IF_USED(MaxXI);
  FREE_IF_USED(MinXI);
  FREE_IF_USED(SumXI);

  return(KRERR_NO_ERROR); 
}        

/*****************************************************************************
  FUNCTION : tac_NextSpecialUnit

  PURPOSE  : searches the unit with minimal distance to the given
             pattern in input space.
             Returns internal number of this unit.
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
int  tac_NextSpecialUnit(int p,Patterns in_pat_First)
{
  int s,i,NextSpecialUnitInInputSpace;
  float devit,SummedSquaredDistance,BestSSD;
  Patterns in_pat;

  BestSSD = 1e20;
  for(s=0;s<cc_MaxSpecialUnitNo;s++){
      SummedSquaredDistance = 0.0;
      in_pat = in_pat_First;
      for(i=0;i<NoOfInputUnits;i++){
	  devit = *in_pat++ - SpecialUnitData[s].Xi[i];
	  SummedSquaredDistance+= (devit*devit);
      }
      if (SummedSquaredDistance < BestSSD)
      {
	  BestSSD=SummedSquaredDistance;
	  NextSpecialUnitInInputSpace=s;
      }
   }
  return(NextSpecialUnitInInputSpace);
}

/*****************************************************************************
  FUNCTION : void tac_changeXi(int UnitNo,int p,int d,Patterns in_pat)

  PURPOSE  : 
      changes the Xis of the virt. SpecialUnit, which was calculated
      before as the next in Input-Space. Here is TAC_ALPHA a 
      regulation term, which is decreased in time.
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
 void tac_changeXi(int UnitNo,int p,int d,int MaxD,Patterns in_pat)
{
  int i;
  float Xi_alt;
  float alpha;
  float Res_Error;

  alpha =  TAC_ALPHA((float)d,(float)MaxD);
  for (i=0;i<NoOfInputUnits;i++){
     Xi_alt=SpecialUnitData[UnitNo].Xi[i];
     Res_Error = (PatternSumError[p] / WholeSummedError);
     SpecialUnitData[UnitNo].Xi[i] = 
 	  Xi_alt + alpha * Res_Error  * (*in_pat++ - Xi_alt); 
  }
}

/*****************************************************************************
  FUNCTION : void tac_printRanks

  PURPOSE  : Prints the data after the kohonen-mapping
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/

void tac_printRanks(float MaxSummedError)
{
  int UnitNo,NewUnitCnt=0;

  if (cc_printOnOff)
  {
    cc_printHeadline("Installing new units",LENGTH_HEADLINE);

    for(UnitNo=0;UnitNo<cc_MaxSpecialUnitNo;UnitNo++){
       printf (
         "Unit %2d has %4d inputpattern in its voronoi region, Summed and normed error is %4f. ",
          UnitNo,SpecialUnitData[UnitNo].NoOfUnitsInRegion,
          SpecialUnitData[UnitNo].SummedErrorInRegion/MaxSummedError);
       if ((SpecialUnitData[UnitNo].SummedErrorInRegion/MaxSummedError) > TAC_THRESHOLD){
          printf(" Installed as hidden unit %d",++NewUnitCnt + NO_OF_NET_UNITS);
       }
       printf("\n");
    }
    printf("\nInstalled %d units on layer %d\n",NewUnitCnt,NoOfLayers+1);
  }
}

/*****************************************************************************
  FUNCTION : void tac_calculateRanksAndRadius(int start,int end)

  PURPOSE  :
      When we made a certain amount of changes to the X_i, we hope,
      that the X's are now located at the maxima of the remaining
      error. Now we count the no of Patterns, which are in our
      region, the correlated error and the mean distances of the
      patterns to the X. These results are used to determine, which
      of the units have to be taken and which initial radius they
      should get. (for formulas see one of the papers or the DA)

  NOTES    : SUMMED_DISTANCES is only used internal.

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
int tac_calculateRanksAndRadius(int start,int end)
{
  int p,pat,sub;
  int UnitNo,dim;
  Patterns in_pat;
  float MaxSummedError=0.0000001;
  float MeanDistance;

      /* init the values */
  for(UnitNo=0;UnitNo<cc_MaxSpecialUnitNo;UnitNo++){
     SpecialUnitData[UnitNo].NoOfUnitsInRegion=0;
     SpecialUnitData[UnitNo].SummedErrorInRegion=0.0;
     for (dim=0;dim<NoOfInputUnits;dim++)
	SpecialUnitData[UnitNo].SUMMED_DISTANCES[dim]=0.0;
  }
      /* determine units and error in voronoi regions */
  for(p=start;p<=end;p++){
     kr_getSubPatternByNo(&pat,&sub,p);
     in_pat = kr_getSubPatData(pat,sub,INPUT,NULL);
     UnitNo=tac_NextSpecialUnit(p,in_pat);
     SpecialUnitData[UnitNo].NoOfUnitsInRegion++;
     SpecialUnitData[UnitNo].SummedErrorInRegion += PatternSumError[p];
      /* now add the distance to the sum of the unit. The values
         are weighten with the Errorvalue */
     for(dim=0;dim<NoOfInputUnits;dim++){
	SpecialUnitData[UnitNo].SUMMED_DISTANCES[dim] +=
	   fabs((*in_pat++) - SpecialUnitData[UnitNo].Xi[dim])
           * PatternSumError[p];
     }
  }

  for(UnitNo=0;UnitNo<cc_MaxSpecialUnitNo;UnitNo++){
     if (SpecialUnitData[UnitNo].SummedErrorInRegion > MaxSummedError)
	MaxSummedError=SpecialUnitData[UnitNo].SummedErrorInRegion;
  }        /* determine max (SummedErrorInRegion) */


  for(UnitNo=0;UnitNo<cc_MaxSpecialUnitNo;UnitNo++){
    if(SpecialUnitData[UnitNo].SummedErrorInRegion > 0.0){
       for(dim=0;dim<NoOfInputUnits;dim++){
	  MeanDistance =  (SpecialUnitData[UnitNo].SUMMED_DISTANCES[dim] / 
		 SpecialUnitData[UnitNo].SummedErrorInRegion);
	  SpecialUnitData[UnitNo].Ri[dim] = 
	     sqrt((-MeanDistance*MeanDistance) / (2*log(TAC_BETA)));
       }
    }   
  }

  tac_printRanks(MaxSummedError);  

  return MaxSummedError;
}


/*****************************************************************************
  FUNCTION : tac_MappingOfTheNewUnits

  PURPOSE  : 
      Realizes the mapping of the new units onto the input space.
      Unlike in CC the Special
      Units are now virtual, they where never generated. Only the two
      Arrays Xi and Ri are generated and calculated   NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
 int tac_MappingOfTheNewUnits(int StartPattern,int EndPattern)
{
  int start,end,n,p,d;
  int pat,sub;
  Patterns in_pat;
  int UnitNo;
  int Modulo;

  Modulo = TAC_KOHONEN /20;  /* for the printing */

  cc_printHeadline("Kohonen-Training",LENGTH_HEADLINE);

  KernelErrorCode=tac_initXiAndRis(StartPattern,EndPattern);
  ERROR_CHECK;

  cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
  ERROR_CHECK;

  for (d=0;d<TAC_KOHONEN;d++){
     if ((d % Modulo)==0) PRINTF("Pass %5d\n",d);
     for(p=start;p<=end;p++){
	kr_getSubPatternByNo(&pat,&sub,p);
	in_pat = kr_getSubPatData(pat,sub,INPUT,NULL);
	UnitNo=tac_NextSpecialUnit(p,in_pat);
	tac_changeXi(UnitNo,p,d,TAC_KOHONEN,in_pat);
     }
  }

  return tac_calculateRanksAndRadius(start,end);
}

/*****************************************************************************
  FUNCTION : tac_calculateCorrelation

  PURPOSE  : 
      Calculates the Correlation as given in CC. The Correlation is a 
      measure of how good a new special unit correlates with the given
      Output-Error. The unit weights (not the r_i and x_i) are trained
      to maximize this Correlation.
      Prints the values.
  NOTES    : 
      Sideeffect : SpecialUnitSumAct is divided by the No
      of Patterns, so it now contains the mean activation.
      SUSA is accesable via MeanYi, now.

  UPDATE   : 30.03.96
******************************************************************************/

 krui_err tac_calculateCorrelation(int StartPattern, int EndPattern,
			      int counter)
{
 int s,o,n;
 float scoreBuffer;
 struct Unit *SpecialUnitPtr,*OutputUnitPtr;
 int start, end;

 cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
 ERROR_CHECK;

 PRINTF("Cycle %3d ",counter);
 FOR_ALL_SPECIAL_UNITS(SpecialUnitPtr,s) {
   SpecialUnitData[s].ErrorCorrelation = 0.0;
   FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,o) {
     scoreBuffer =  (CorBetweenSpecialActAndOutError[s][o] - 
		    (MeanOutputUnitError[o] * SpecialUnitSumAct[s] ));
     SpecialUnitData[s].ErrorCorrelation += fabs(scoreBuffer);
     SIGN_OF_THE_CORRELATION[s][o] = SIGN(scoreBuffer);
   }
   SpecialUnitData[s].ErrorCorrelation /= SumSqError;
   PRINTF("s[%2d]=%4f   ",s,SpecialUnitData[s].ErrorCorrelation);
   MeanYi[s]= SpecialUnitSumAct[s] / n;
 }
 return KRERR_NO_ERROR;
}


/*****************************************************************************
  FUNCTION : tac_calculateRijAndSumRij

  PURPOSE  : 
      the Arrays Nij and Rij were calculated. Both Arrays were needed
      by the AntiCorrelation F. (Formulas see papers or DA)
      The routine gives back the divident of F.
  NOTES    : 

  UPDATE   : 30.03.96
******************************************************************************/

 float tac_calculateRijAndSumRij(float** Rij,float* MeanYi,
				int start,int end,int n)
{
  double** Di;   /* temporal array to hold  (Yi-MeanYi)^2  */
  int s,p,i,j;
  double FNenner,Sum,Zaehler;

  CALLOC_2DIMENSIONAL_ARRAY(Di,NoOfInstalledUnits,n,double,i);

  for(s=0;s<NoOfInstalledUnits;s++){
     for(p=start;p<=end;p++){
	 Di[s][p] = SpecialUnitAct[p][s] - MeanYi[s];
	 Di[s][p] *= Di[s][p];   /* set Di= (Yi-MeanYi)^2 */
     }
  }

  FNenner=0.0;

  for(i=0;i<NoOfInstalledUnits;i++){
     for(j=i+1;j<NoOfInstalledUnits;j++){
	Sum = Zaehler = 0.0;
	for(p=0;p<n;p++){
	   Sum += Di[i][p] * Di[j][p];
	   Zaehler += SpecialUnitAct[p][i]*SpecialUnitAct[p][j];
	}
	if (Sum>0) {
	   Nij[i][j] = sqrt(Sum);
	   Rij[i][j] = (Zaehler - MeanYi[i]*MeanYi[j]*n) / Nij[i][j];
	}
	else {
	   Rij[i][j] = Nij[i][j] = 0.00001; /* no rij==0 allowed */
	}
       FNenner += fabs(Rij[i][j]);
     }
  }

  FREE_2DIMENSIONAL_ARRAY(Di,NoOfInstalledUnits,i);

  return(FNenner);
}


/*****************************************************************************
  FUNCTION : tac_calculateAntiCorrelation

  PURPOSE  :  Calculates the AntiCorrelation F. F is a measure of :
      1. How good is the Correlation between the units and the 
         ouput error ? This is achieved by using the calculated and 
         summarized Correlations S. 
      2. Do the new units all have different outputs ? The Correlation
         Rij between 2 different units is calculated, and the sum is
         placed in the Nenner, to guarantee minimal correlation.
      F will be used to determine, if the learning procedure is stagnant
      or further learning could be usefull.
      F will be trained by a gradient ascent procedure. 

  NOTES    :

  UPDATE   : 30.03.96
******************************************************************************/

float tac_calculateAntiCorrelation(int StartPattern, int EndPattern,bool First)
{
  int start,end,n,i;
  
  cc_getPatternParameter(StartPattern,EndPattern,&start,&end,&n);
  ERROR_CHECK;
  
  AC_Nenner = 
     TAC_ETA + tac_calculateRijAndSumRij(Rij,MeanYi,start,end,n);
    
  AC_Zaehler=0.0;

  for(i=0;i<NoOfInstalledUnits;i++)
     AC_Zaehler+= SpecialUnitData[i].ErrorCorrelation;

  return(AC_Zaehler/AC_Nenner);
} 

/*****************************************************************************
  FUNCTION : tac_initSpecialUnitLinks

  PURPOSE  : 
      initializes the weights, radii, xi and the variables used by the 
      learning routine of all special units.
      The weights and the bias are set to a random value. 
      Further are the arrays allocated, which are needed for the
      thraining of the new hidden units.

  NOTES    :

  UPDATE   : 30.03.96
******************************************************************************/

krui_err tac_initSpecialUnitLinks(void)
{
 int i,s,LinkCnt;
 struct Unit *SpecialUnitPtr;
 struct Link *LinkPtr;

 FOR_ALL_SPECIAL_UNITS(SpecialUnitPtr,s) {
   SpecialUnitPtr->bias = cc_generateRandomNo(TAC_MAX_VALUE);
   BIAS_CURRENT_SLOPE(SpecialUnitPtr)      = 0.0; 
   BIAS_PREVIOUS_SLOPE(SpecialUnitPtr)     = 0.0; 
   BIAS_LAST_WEIGHT_CHANGE(SpecialUnitPtr) = 0.0;
   LinkCnt = 0;
   FOR_ALL_LINKS(SpecialUnitPtr,LinkPtr) {
     SpecialUnitData[s].LinkError[LinkCnt  ].LnCurrentSlope     = 0.0;
     SpecialUnitData[s].LinkError[LinkCnt  ].LnPreviousSlope    = 0.0;
     SpecialUnitData[s].LinkError[LinkCnt++].LnLastWeightChange = 0.0;
     LinkPtr->weight = cc_generateRandomNo(TAC_MAX_VALUE);
   }
 }
 CALLOC_2DIMENSIONAL_ARRAY
    (Rij,NoOfInstalledUnits,NoOfInstalledUnits,float,i);
 CALLOC_2DIMENSIONAL_ARRAY
    (Nij,NoOfInstalledUnits,NoOfInstalledUnits,float,i);
 CALLOC_2DIMENSIONAL_ARRAY
   (PrimesOfSpecialUnits,NoOfInstalledUnits,NoOfInputUnits,TAC_PRIME_TYPE,i);

 return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : tac_trainSpecialUnits

  PURPOSE  : 
      Calls the training procedure for the training of the Special units.
      It calls the routines for the calculation of the correlations, the
      anticorrelation, and the online/offline parts of the training. 
      See cc_trainSpecialUnits,too.
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
 krui_err tac_trainSpecialUnits(int maxNoOfCovarianceUpdateCycles, 
			    float minCovarianceChange,
     	                    int specialPatience,int StartPattern, 
			    int EndPattern,float param1, float param2,
			    float param3,int MaxSpecialUnitNo,int LayerNo)
{
    int counter,start,end,n;
    float oldHighScore=0.0;

    cc_printHeadline("Training of the candidates",LENGTH_HEADLINE);

    for (counter=0;counter<maxNoOfCovarianceUpdateCycles;counter++){
	KernelErrorCode=cc_getPatternParameter
			  (StartPattern,EndPattern,&start,&end,&n);
	ERROR_CHECK;

	cc_calculateSpecialUnitActivation(StartPattern,EndPattern);
        ERROR_CHECK;

	KernelErrorCode=
           tac_calculateCorrelation(StartPattern,EndPattern,counter);
        ERROR_CHECK;

	AntiCorr=tac_calculateAntiCorrelation
            (StartPattern,EndPattern,counter==0);
        ERROR_CHECK;

        PRINTF("AntiCorr is %0.5f\n",AntiCorr);

	(*cc_propagateSpecialUnitsBackward)
			     (start,end,n,counter,param1,param2,param3);
	ERROR_CHECK;

        tac_updateSpecial(param1,param2,param3);

	cc_initActivationArrays(); 

	if ((counter % specialPatience)==0){
	   if(fabs(AntiCorr-oldHighScore) <
	      minCovarianceChange * oldHighScore)
	      return(KRERR_NO_ERROR);
           oldHighScore = AntiCorr;
	}
        
    } 
  PRINTF("\n");
  return(KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : tac_protocollNet

  PURPOSE  : Write protocoll data to 'protokoll.dat', if it's not done by now
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/

void tac_protocollNet(void)
{
    /* This function should be removed */
}


/*****************************************************************************
  FUNCTION : tac_protocollNet

  PURPOSE  : Write protocoll data of learning process to protokoll.dat.
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/


void tac_protocollLearning(int LayerNo,int StartPattern,int EndPattern)
{
    /* This function should be removed */
}

/*****************************************************************************
  FUNCTION : tac_installNewUnits

  PURPOSE  : 
      Determine how much and which hiddden units have to be generated.
      Do this and initialize the links.
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/

krui_err tac_installNewUnits(int LayerNo,float MaxError,int StartPattern,int EndPattern)
{
  int i;

  NoOfInstalledUnits=0;

  if (LayerNo>1) cc_printHeadline("Connection routing",LENGTH_HEADLINE);

  for (i=0;i<cc_MaxSpecialUnitNo;i++){
     if ((SpecialUnitData[i].SummedErrorInRegion/MaxError)
          >TAC_THRESHOLD){
	NoOfInstalledUnits++;
	KernelErrorCode = 
	   tac_generateNewUnit(i,LayerNo,StartPattern,EndPattern);
	ERROR_CHECK;
	
        KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
	ERROR_CHECK;
        KernelErrorCode = cc_setPointers();
        ERROR_CHECK;
     }
  }
  KernelErrorCode = tac_initSpecialUnitLinks();
  ERROR_CHECK;

  return(KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : LEARN_TACOMA

  PURPOSE  : The main learn routine of Tacoma
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/

krui_err LEARN_Tacoma(int StartPattern, int EndPattern,
		      float *ParameterInArray, int NoOfInParams,
		      float **ParameterOutArray, int *NoOfOutParams)
{
   int TempErrorCode; /* Used by the macro ERROR_CHECK_WITH_MEM_DEALLOC */
   int LayerNo; 
   struct Unit * UnitPtr,*OutputUnitPtr;
   struct Link* LinkPtr;
   int dummy;
   int CurrentUnit;    
   float MaxError;

   NetLearnAlgorithm = TACOMA;

   cc_deleteAllSpecialUnits(); /* no need for special units here.
                                  the could be some, if learning func was switched
                                  from CC to Tacoma */

   if (cc_allButtonIsPressed) { /* something might have changed */
      KernelErrorCode =       
	 tac_initVariables(ParameterInArray,StartPattern,EndPattern);
      ERROR_CHECK;
   }

   if (cc_end) return(KRERR_NO_ERROR);
                                /* We're ready last time */
   KernelErrorCode = tac_allocateStorage(StartPattern,EndPattern);
   TAC_ERROR_CHECK_WITH_MEMORY_DEALLOCATION; 

   if(cc_test(StartPattern,EndPattern,MAX_PIXEL_ERR) !=  CONTINUE_LEARNING) {
      cc_end = 1;               /* we're ready now */
      tac_protocollNet();       
      return(tac_freeStorage(StartPattern,EndPattern));
   }

   if (NoOfHiddenUnits<1) {
      if((OUT_PATIEN != 0) && (MAX_NO_ERROR_UPDATE_CYCLES != 0)) {
	 cc_trainOutputUnits(MAX_NO_ERROR_UPDATE_CYCLES, BACKFITT_PATIENCE, 
			     MINIMAL_ERROR_CHANGE,
			     OUT_PATIEN,StartPattern,EndPattern,PARAM1,
			     PARAM2,PARAM3,ParameterOutArray,
			     NoOfOutParams);
      }  
      cc_getErr(StartPattern, EndPattern);
   }

  KernelErrorCode = tac_calculateOutputUnitError(StartPattern,EndPattern);
  TAC_ERROR_CHECK_WITH_MEMORY_DEALLOCATION;
                                /* determine errors */
  if (SumSqError==0.0)
     MaxError=cc_getErr(StartPattern,EndPattern);
                                /* calc SumSqError, use MaxError as dummy */

  MaxError = tac_MappingOfTheNewUnits(StartPattern,EndPattern);
  TAC_ERROR_CHECK_WITH_MEMORY_DEALLOCATION;
                                /* kohonen-feature maps */
  LayerNo=NoOfLayers+1; 

  KernelErrorCode = 
      tac_installNewUnits(LayerNo,MaxError,StartPattern,EndPattern);
  TAC_ERROR_CHECK_WITH_MEMORY_DEALLOCATION;
                                /* generate units and install them */

  if((SPEC_PATIENCE != 0) && (MAX_NO_OF_COVAR_UPDATE_CYCLES != 0)) {
      tac_trainSpecialUnits(MAX_NO_OF_COVAR_UPDATE_CYCLES,
			    MIN_COVAR_CHANGE,SPEC_PATIENCE,
			    StartPattern,EndPattern,
			    PARAM4,PARAM5,PARAM6,
                            MAX_SPECIAL_UNIT_NUMBER,LayerNo);
  }                            /* train the candidates */

  FOR_ALL_UNITS(UnitPtr){
     if (IS_SPECIAL_UNIT(UnitPtr)){
	KernelErrorCode = kr_unitSetTType(GET_UNIT_NO(UnitPtr),HIDDEN); 
        TAC_ERROR_CHECK_WITH_MEMORY_DEALLOCATION; 
	/* generate links between output unit and new hidden unit */
	FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,dummy){
	   CurrentUnit = GET_UNIT_NO(OutputUnitPtr);
	   KernelErrorCode = krui_setCurrentUnit(CurrentUnit); 
	   ERROR_CHECK;
	   LinkPtr = krui_createLinkWithAdditionalParameters
               (GET_UNIT_NO(UnitPtr), 0.0, 0.0, 0.0, 0.0 );
	   ERROR_CHECK; 
	}   
     }    
  }

  KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
  TAC_ERROR_CHECK_WITH_MEMORY_DEALLOCATION;
  KernelErrorCode = cc_setPointers();
  TAC_ERROR_CHECK_WITH_MEMORY_DEALLOCATION;

  if((OUT_PATIEN != 0) && (MAX_NO_ERROR_UPDATE_CYCLES!= 0)) {
    cc_trainOutputUnits    (MAX_NO_ERROR_UPDATE_CYCLES, BACKFITT_PATIENCE,
			   MINIMAL_ERROR_CHANGE,
			   OUT_PATIEN,StartPattern,EndPattern,PARAM1,
			   PARAM2,PARAM3,ParameterOutArray,NoOfOutParams); 
  }

  tac_protocollLearning(LayerNo,StartPattern,EndPattern);

  cc_allButtonIsPressed = NetModified = FALSE;

  KernelErrorCode=tac_freeStorage(StartPattern,EndPattern);
  return(KernelErrorCode);
}



/*****************************************************************************
  FUNCTION : tac_propagateXiRi

  PURPOSE  : online part of learning of the xi and ri for the offline
             learning algorithms like Quickprop and Rprop.
  NOTES    : This routine will never used by default. Only if
             TAC_XIRI_ONLINE is set to FALSE, then the training of the
             xis and ris will be done with the same learning function 
             and parametersm as the training of the weights.
             By default TAC_XIRI_ONLINE is true because learning with
             for example Quickprop is to good. Then xi and ri will
             be changed too much, and the calculated links to other
             hidden units will make no sense. And the overhead of
             calculating the refernce vecors, too.

  UPDATE   : 30.03.96
******************************************************************************/
void tac_propagateXiRi(struct Unit* SpecialUnitPtr,
          int p,int s,float SummedDeltaS,double nMinus1Divn,
          float eta,float mu, float dummy)

{
  struct Unit* SecondSpecUnitPtr;
  struct Link* LinkPtr;
  int s2;
  int First,Sec,i;
  float sum;
  float Diff,Prime_Xi,Prime_Ri,DeltaF;

    sum=0.0; 
  FOR_ALL_SPECIAL_UNITS(SecondSpecUnitPtr,s2){
     if(s2!=s){
	  if(s<s2) { First=s; Sec=s2; }
	      else { First=s2; Sec=s; }
	  sum += SIGN(Rij[First][Sec]) *
		 (SpecialUnitAct[p][s2]*Nij[First][Sec]-Rij[First][Sec]*
		 (SpecialUnitAct[p][s]-MeanYi[s])*
                 (SpecialUnitAct[p][s2]-MeanYi[s2])*
                 (SpecialUnitAct[p][s2]-MeanYi[s2]))
		 / (Nij[First][Sec]*Nij[First][Sec]);
     }
  }
  DeltaF = (SummedDeltaS*AC_Nenner-nMinus1Divn*AC_Zaehler*sum) /
	      (AC_Nenner*AC_Nenner);
  
  i=0;
  FOR_ALL_LINKS(SpecialUnitPtr,LinkPtr){
     if (IS_INPUT_UNIT(LinkPtr->to)){
	  Diff = 
		(LinkPtr->to->Out.output-XI_OF_LINK(LinkPtr));
	  Prime_Xi =   (DeltaF*2*Diff*SpecialUnitPtr->Out.output) / 
		(RI_OF_LINK(LinkPtr)*RI_OF_LINK(LinkPtr));
	  Prime_Ri=(Prime_Xi*Diff)/RI_OF_LINK(LinkPtr);
	  PrimesOfSpecialUnits[s][i].xi_CurrentSlope -= Prime_Xi;
	  PrimesOfSpecialUnits[s][i].ri_CurrentSlope -= Prime_Ri;
	  i++;
     }
  }
}
/*****************************************************************************
  FUNCTION : tac_propagateXiRiOnlineCase

  PURPOSE  : 
      online part of learning of the xi and ri for the offline
      learning algorithms like Quickprop and Rprop.
      This routine will never used by default. Only if
      TAC_XIRI_ONLINE is set to FALSE, then the training of the
      xis and ris will be done with the same learning function 
      and parametersm as the training of the weights.
      By default TAC_XIRI_ONLINE is true because learning with
      for example Quickprop is to good. Then xi and ri will
      be changed too much, and the calculated links to other
      hidden units will make no sense. And the overhead of
      calculating the refernce vecors, too.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
void tac_propagateXiRiOnlineCase(struct Unit* SpecialUnitPtr,
          int p,int s,float SummedDeltaS,double nMinus1Divn ,float eta,float mu, float dummy)

{
  struct Unit* SecondSpecUnitPtr;
  struct Link* LinkPtr;
  int s2;
  int First,Sec,i;
  float sum;
  float Diff,Prime_Xi,Prime_Ri,DeltaF;

  sum=0.0;
    FOR_ALL_SPECIAL_UNITS(SecondSpecUnitPtr,s2){
       if(s2!=s){
	  if(s<s2) { First=s; Sec=s2; }
	      else { First=s2; Sec=s; }
	  sum += SIGN(Rij[First][Sec]) *
		 (SpecialUnitAct[p][s2]*Nij[First][Sec]-Rij[First][Sec]*
		 (SpecialUnitAct[p][s]-MeanYi[s])*
                 (SpecialUnitAct[p][s2]-MeanYi[s2])*(SpecialUnitAct[p][s2]-MeanYi[s2]))
		 / (Nij[First][Sec]*Nij[First][Sec]);
       }
    }
    DeltaF = (SummedDeltaS*AC_Nenner-nMinus1Divn*AC_Zaehler*sum) / (AC_Nenner*AC_Nenner);
    
    i=0;
    FOR_ALL_LINKS(SpecialUnitPtr,LinkPtr){
       if (IS_INPUT_UNIT(LinkPtr->to)){
	  Diff = 
		(LinkPtr->to->Out.output-XI_OF_LINK(LinkPtr));
	  Prime_Xi =   (DeltaF*2*Diff*SpecialUnitPtr->Out.output) / 
		(RI_OF_LINK(LinkPtr)*RI_OF_LINK(LinkPtr));
	  Prime_Ri=(Prime_Xi*Diff)/RI_OF_LINK(LinkPtr);
          XI_OF_LINK(LinkPtr) += Prime_Xi*eta;
          RI_OF_LINK(LinkPtr) += Prime_Ri*eta;    
	  i++;
       }
    }
}

/*****************************************************************************
  FUNCTION : tac_propagateSpecialOnlineCase  

  PURPOSE  : online Part of training (weights, bias), this one 
             for backprop online 
  NOTES    : Calls tac_propagateXiRiOnline

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
 krui_err tac_propagateSpecialOnlineCase (int start,int end,int n,
		   int counter,float eta, float mu,float dummy)
{
    float change=0.0,actPrime;
    int s,o,p,pat,sub,i;
    struct Unit *SpecialUnitPtr,*OutputUnitPtr;
    struct Link *LinkPtr;
    int LinkCnt;
    double nMinus1Divn = (n-1) / n;
    double SummedDeltaS;
    float  lastChange;

    for(s=0;s<NoOfInstalledUnits;s++){
       for(i=0;i<NoOfInputUnits;i++){
	  PrimesOfSpecialUnits[s][i].xi_CurrentSlope=0.0;
	  PrimesOfSpecialUnits[s][i].ri_CurrentSlope=0.0;
       }
    }

    for(p=start; p<=end;p++){
	cc_getActivationsForActualPattern(p,start,&pat,&sub);
	FOR_ALL_SPECIAL_UNITS(SpecialUnitPtr,s) {


	    SummedDeltaS = 0.0;
	    SpecialUnitPtr->Out.output =
                  SpecialUnitPtr->act = 
                        SpecialUnitAct[p][s];
	    actPrime = (*SpecialUnitPtr->act_deriv_func)(SpecialUnitPtr);
	    FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,o){
		SummedDeltaS += SIGN_OF_THE_CORRELATION[s][o] *
		   (OutputUnitError[p][o]-MeanOutputUnitError[o]);
	    }  
            SummedDeltaS /= SumSqError;
	    change = SummedDeltaS*actPrime;
	    BIAS_CURRENT_SLOPE(SpecialUnitPtr) += change;     
            lastChange = BIAS_LAST_WEIGHT_CHANGE(SpecialUnitPtr);
	              SpecialUnitPtr->bias -= 
               (BIAS_LAST_WEIGHT_CHANGE(SpecialUnitPtr) =
                   eta*change + lastChange*mu);

	    LinkCnt = 0;
	    FOR_ALL_LINKS(SpecialUnitPtr,LinkPtr) {
                lastChange =
                  SpecialUnitData[s].LinkError[LinkCnt].LnLastWeightChange;
 	        LinkPtr->weight -= 
                   (SpecialUnitData[s].LinkError[LinkCnt].LnLastWeightChange =
                   eta*LinkPtr->to->Out.output*change + lastChange*mu);
                LinkCnt++;
	    }

            if (TAC_XIRI_ONLINE) 
              tac_propagateXiRiOnlineCase
                 (SpecialUnitPtr,p,s,SummedDeltaS,nMinus1Divn,
                  TAC_XI_RI_ETA,0.0,0.0);
            else
              tac_propagateXiRiOnlineCase
                 (SpecialUnitPtr,p,s,SummedDeltaS,nMinus1Divn,eta,mu,dummy);

	}
    }
    cc_actualNetSaved=TRUE;
    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : tac_propagateSpecial

  PURPOSE  : online Part of training (weights, bias), this one 
             for Quickprop, RProp, Backprop offline
  NOTES    :

  UPDATE   : 30.4.96 <Juergen Gatter>
******************************************************************************/
krui_err tac_propagateSpecial (int start,int end,int n,
		   int counter,float dummy1, float dummy2,float dummy3)
{
    float change=0.0,actPrime;
    int s,o,p,pat,sub,i,LinkCnt;
    struct Unit *SpecialUnitPtr,*OutputUnitPtr;
    struct Link *LinkPtr;
    double nMinus1Divn = (n-1) / n;
    double SummedDeltaS;
 
    for(s=0;s<NoOfInstalledUnits;s++){
       for(i=0;i<NoOfInputUnits;i++){
	  PrimesOfSpecialUnits[s][i].xi_CurrentSlope=0.0;
	  PrimesOfSpecialUnits[s][i].ri_CurrentSlope=0.0;
       }
    }

    for(p=start; p<=end;p++){

	cc_getActivationsForActualPattern(p,start,&pat,&sub);
	FOR_ALL_SPECIAL_UNITS(SpecialUnitPtr,s) {
	    SummedDeltaS = 0.0;
	    SpecialUnitPtr->Out.output = SpecialUnitPtr->act = SpecialUnitAct[p][s];
	    actPrime = (*SpecialUnitPtr->act_deriv_func)(SpecialUnitPtr);
	    FOR_ALL_OUTPUT_UNITS(OutputUnitPtr,o){
		SummedDeltaS += SIGN_OF_THE_CORRELATION[s][o] *
		   (OutputUnitError[p][o]-MeanOutputUnitError[o]);
	    }  
            SummedDeltaS /= SumSqError;
	    change = SummedDeltaS*actPrime;
	    BIAS_CURRENT_SLOPE(SpecialUnitPtr) += change;     
	    LinkCnt = 0;
	    FOR_ALL_LINKS(SpecialUnitPtr,LinkPtr) {

		SpecialUnitData[s].LinkError[LinkCnt++].LnCurrentSlope += 
		  change * LinkPtr->to->Out.output;
	    }

            if (TAC_XIRI_ONLINE) 
              tac_propagateXiRiOnlineCase
                 (SpecialUnitPtr,p,s,SummedDeltaS,nMinus1Divn,TAC_XI_RI_ETA,0.0,0.0);
            else
 
            tac_propagateXiRi
               (SpecialUnitPtr,p,s,SummedDeltaS,nMinus1Divn, dummy1,dummy2,dummy3);

	}
    }
    cc_actualNetSaved=TRUE;
    return(KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION :  tac_updateSpecial

  PURPOSE  : 
      this routine updates the bias of the Special-Units and
      the weights, the radii and the xi of links to a special unit. 
      The routine calls the procedure, which makes the update-step
      according to the actual valid learning function (QuickProp,RProp
      or BackProp, according to cc_SpecialUnitUpdate)
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
   void tac_updateSpecial(float param1,float param2,float param3)
{
    struct Unit *SpecialUnitPtr;
    struct Link *LinkPtr;
    int s,LinkCnt,InpCnt;

    FOR_ALL_SPECIAL_UNITS(SpecialUnitPtr,s) {
      	SpecialUnitPtr->bias +=
           (*cc_SpecialUnitUpdate)
              (SpecialUnitPtr->bias,&BIAS_PREVIOUS_SLOPE(SpecialUnitPtr),
               &BIAS_CURRENT_SLOPE(SpecialUnitPtr),
               &BIAS_LAST_WEIGHT_CHANGE(SpecialUnitPtr),
               param1,param2,param3);  		          
	LinkCnt = InpCnt = 0;
	FOR_ALL_LINKS(SpecialUnitPtr,LinkPtr) {
 	         /* training the weights ... */
	              LinkPtr->weight +=
                 (*cc_SpecialUnitUpdate) (LinkPtr->weight,
                     &SpecialUnitData[s].LinkError[LinkCnt].LnPreviousSlope,
		     &SpecialUnitData[s].LinkError[LinkCnt].LnCurrentSlope,
		     &SpecialUnitData[s].LinkError[LinkCnt].LnLastWeightChange,
                     param1,param2,param3);
            LinkCnt++;
	    if ((IS_INPUT_UNIT(LinkPtr->to)&&(!TAC_XIRI_ONLINE))){ 
		/* and now the Xi */
	      	                    XI_OF_LINK(LinkPtr) +=
		      (*cc_SpecialUnitUpdate) (XI_OF_LINK(LinkPtr),
                          &PrimesOfSpecialUnits[s][InpCnt].xi_PreviousSlope,
			  &PrimesOfSpecialUnits[s][InpCnt].xi_CurrentSlope,
			  &PrimesOfSpecialUnits[s][InpCnt].xi_LastChange,
                          param1,param2,param3);
		/* and now the radii */
	      		                RI_OF_LINK(LinkPtr) +=
		      (*cc_SpecialUnitUpdate) (RI_OF_LINK(LinkPtr),
                           &PrimesOfSpecialUnits[s][InpCnt].ri_PreviousSlope,
			   &PrimesOfSpecialUnits[s][InpCnt].ri_CurrentSlope,
			   &PrimesOfSpecialUnits[s][InpCnt].ri_LastChange,
                           param1,param2,param3);
		InpCnt++;
            }
        }
    } 
}






/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_modify.c,v $
  SHORTNAME      : cc_modify.c
  SNNS VERSION   : 4.2

  PURPOSE        : Functions of CC for the Modifications
  NOTES          :
                   For more information about the implemented modifications see:
                   
                   J. Gatter:
                   "Lernverfahren neuronaler Netze mit automatischer
                    Bestimmung der Netzwerktopologie"
                   Diplomarbeit Nr. 1337, university of Stuttgart

  AUTHOR         : Juergen Gatter
  DATE           : 30.11.95

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.11 $
  LAST CHANGE    : $Date: 1998/04/20 11:54:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>  
#include <memory.h>
#include <math.h>
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
#include "kr_newpattern.h"
#include "cc_modify.ph"
#include "cc_glob.h"

/*****************************************************************************
  FUNCTION : cc_InitModificationVariables

  PURPOSE  : Initialise the Variables needed by the modification-routines.

  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
krui_err  cc_InitModificationVariables(void)
{
  if ((NoOfLayers==0)||(cc_modification!=CC_STAT))
    ccm_CurrentLayerHeight=ccm_MissingUnitsInLayer=0;
  return (KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : cc_TestWhetherParametersAreValid

  PURPOSE  : tests correctness of the additional parameters.

  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/


krui_err cc_TestWhetherParametersAreValid(void)
{
  switch(cc_modification){
      case(CC_GCC) :
           if (CCS_NO_OF_GROUPS <=0){
          	 printf("0 groups isn't valid. Changed to 1 group.\n");
              cc_Parameter[0]=1.0;
           }
           if (CCS_NO_OF_GROUPS > NoOfOutputUnits){
              printf("I'm not able to make more groups than OutputUnits.\n");
              printf("Changed to %d groups.\n",NoOfOutputUnits);
              cc_Parameter[0]= (float)NoOfOutputUnits;
           }
           if (CCS_NO_OF_GROUPS > cc_MaxSpecialUnitNo){
              printf("I'm not able to make more groups than candidates.\n");
              printf("Changed to %d groups.\n",cc_MaxSpecialUnitNo);
              cc_Parameter[0]= (float)cc_MaxSpecialUnitNo;
           }
         break;
      case(CC_SDCC) :
           if (CCB_LAMBDA < 0.0) return (KRERR_CC_INVALID_ADD_PARAMETERS);
         break;
      case(CC_LFCC) :
           if (CCR_NO_OF_LINKS < 1) return (KRERR_CC_INVALID_ADD_PARAMETERS);
         break;
      case(CC_RLCC):
           if (CCO_FAKTOR <= 0.0) return (KRERR_CC_INVALID_ADD_PARAMETERS);
         break;
      case(CC_ECC):
           if (CCC_M <= 0.0) return (KRERR_CC_INVALID_ADD_PARAMETERS);
         break;
      case(CC_STAT) :
         break;       /* everything's possible, since 0 or negative LayerHeights
                         are automatic converted to 1 */
   }
   return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : cc_allocateMemoryForModifications

  PURPOSE  : allocates the needed memory

  NOTES    : dealloc by cc_deallocateMemory 
             

  UPDATE   : 30.03.96
******************************************************************************/
krui_err cc_allocateMemoryForModifications (void)
{
    switch(cc_modification){
      case(CC_LFCC) :        
        CALLOC_ERRORCHECK(ccr_ListOfChosenUnits,CCR_NO_OF_LINKS,int);
	break;
      case(CC_GCC) :
        CALLOC_ERRORCHECK(ccs_GroupList,NoOfOutputUnits,int);
        CALLOC_ERRORCHECK(ccs_GroupData,CCS_NO_OF_GROUPS,struct CC_GROUP);
	break;
      default:
	break;
    }
    return (KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : cc_deallocateMemory

  PURPOSE  : deallocates used memory.

  NOTES    : macro tests if pointer isn't used. Then it have to be NULL.

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
void cc_deallocateMemory(void)
{
  FREE_IF_USED(ccs_GroupList);
  FREE_IF_USED(ccs_GroupData);
  FREE_IF_USED(ccr_ListOfChosenUnits);
}

/*****************************************************************************
  FUNCTION : cc_modifyHighScore

  PURPOSE  : changes the HighScore according the the actual modification.

  NOTES    : 
             

  UPDATE   : 30.03.96
******************************************************************************/
double cc_modifyHighScore(struct Unit* SpecialUnitPtr, int s, double old)
{   
   switch(cc_modification){
      case(CC_SDCC) : 
         return  ((s >= (cc_MaxSpecialUnitNo/2)) ? CCB_LAMBDA*old : old);
      case(CC_RLCC) :
         return (pow(CCO_FAKTOR,NoOfLayers-CC_LAYER_NO(SpecialUnitPtr))*old);
      default :
         return (old);
   }
 }

/*****************************************************************************
  FUNCTION : cc_actualizeGroupHighscores

  PURPOSE  : changes the HighScores if more then one candidate should be 
             built in.

  NOTES    : 
             

  UPDATE   : 30.03.96
******************************************************************************/
void cc_actualizeGroupHighscores(double Score,int s,struct Unit* UnitPtr)
{
   if(cc_modification==CC_GCC){
      if(Score > ccs_GroupData[s % CCS_NO_OF_GROUPS].Score)
         ccs_GroupData[s % CCS_NO_OF_GROUPS].Score = Score;
         ccs_GroupData[s % CCS_NO_OF_GROUPS].BestUnitPtr = UnitPtr;
   }
}

/*****************************************************************************
  FUNCTION : cc_calculateNoOfNewUnits

  PURPOSE  : How many new Units will we have in one cycle ?
             

  NOTES    : 
             

  UPDATE   : 30.03.96
******************************************************************************/
int cc_calculateNoOfNewUnits(void)
{
  switch(cc_modification){
     case (CC_GCC) : return (CCS_NO_OF_GROUPS);
     default              : return 1;
   }
}



/*****************************************************************************
  FUNCTION : cc_getModifiedBestSpecialUnitPtr()

  PURPOSE  : change the bestSpecialUnitPtr if needed.
             
  NOTES    :

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/
 struct Unit*  cc_getModifiedBestSpecialUnitPtr(int GroupNo)
{
  switch(cc_modification){
     case(CC_GCC) :
         return(ccs_GroupData[GroupNo].BestUnitPtr);
     default : 
         return(bestSpecialUnitPtr);
   }
}


/*****************************************************************************
  FUNCTION : cc_UnitsHaveToBeConnected

  PURPOSE  : Test whether the Units have to be connected, according
             the the actual modification.
  NOTES    : SpecialUnitNo is in [0..cc_MaxSpecialUnitNo-1]. UnitNo
             is the number of the other unit.

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/

bool cc_UnitsHaveToBeConnected(int SpecialUnitNo,struct Unit *UnitPtr)
{
  int UnitNo,relUnitNo;
  int i;

  UnitNo=GET_UNIT_NO(UnitPtr);

  relUnitNo=((IS_HIDDEN_UNIT(UnitPtr)) ? REL_HIDDEN_NO(UnitNo) : UnitNo);

  switch (cc_modification){  /* no break required */
    case(CC_SDCC)      : return((IS_INPUT_UNIT(UnitPtr))||
                           (SpecialUnitNo >= cc_MaxSpecialUnitNo/2)||
                           (CC_LAYER_NO(UnitPtr)<NoOfLayers));
    case(CC_ECC) :
                       return ((IS_INPUT_UNIT(UnitPtr)) ||
                           ((float)relUnitNo/(float)(1+NoOfHiddenUnits)) <= CCC_M);
    case(CC_STAT) :
                       return ((IS_INPUT_UNIT(UnitPtr)) ||
                           (relUnitNo <= NoOfHiddenUnits + 1 -
                           (ccm_CurrentLayerHeight-ccm_MissingUnitsInLayer)));
    case(CC_LFCC) :
                       for(i=0;i<CCR_NO_OF_LINKS;i++){
                           if (UnitNo==ccr_ListOfChosenUnits[i]){
                                 return (TRUE);
                           }
                       }
                       return (FALSE); /* Nicht in Liste vorhanden */        
    case(CC_RLCC)      :
                       return(CC_LAYER_NO(UnitPtr) < cco_ActualLayer);
    default          : return (TRUE);  /* no modification */
  }
}

/*****************************************************************************
  FUNCTION : cc_calculateConnections

  PURPOSE  : calculates the connections, if necessary
  NOTES    : 

  UPDATE   : 30.03.96 <Juergen Gatter>
******************************************************************************/

krui_err cc_calculateConnections(int SpecUnitNo)
{
  struct Unit * UnitPtr;
  int *ListOfAllUnits;
  int  cnt=0,i,j,Choose; 
  int FoundUnits=0;
 
  switch(cc_modification){
     case (CC_LFCC) :
        CALLOC_ERRORCHECK(ListOfAllUnits,NoOfInputUnits+NoOfHiddenUnits,int);
        FOR_ALL_UNITS(UnitPtr){
           if (IS_INPUT_UNIT(UnitPtr)||
              IS_HIDDEN_UNIT(UnitPtr)){
                ListOfAllUnits[FoundUnits++] = GET_UNIT_NO(UnitPtr);
           }     /* Copz numbers of every unit to LOAL */
        }

        if (FoundUnits<=CCR_NO_OF_LINKS){
           for(i=0;i<FoundUnits;i++){
              ccr_ListOfChosenUnits[i]=ListOfAllUnits[i];
           }     /* get input from all units */
        }
        else{
           for(i=0;i<CCR_NO_OF_LINKS;i++){
              Choose = (int)(drand48()*(float)(FoundUnits-i));    
                /* choose one of the remaining units */
              for (j=0,cnt=0;
                  ((cnt<Choose)||(ListOfAllUnits[j] == FIELD_EMPTY));
                   j++){
                 if (ListOfAllUnits[j] != FIELD_EMPTY) cnt++;
		 /* Count `til choosen number. There have to be 
                    CCR_NO_OF_LINKS entries remaining.*/
      	      }
              ccr_ListOfChosenUnits[i] = ListOfAllUnits[j];
              ListOfAllUnits[j] = FIELD_EMPTY;
              UnitPtr=kr_getUnitPtr(ccr_ListOfChosenUnits[i]);
                /* get Unit number and free place. */
           }
        }  
        free(ListOfAllUnits);
        return(KRERR_NO_ERROR);
     case(CC_RLCC) :
        if (cc_MaxSpecialUnitNo==1)
           cco_ActualLayer = NoOfLayers+1;
        else
          cco_ActualLayer = 1+(int)((SpecUnitNo*(NoOfLayers+0.9999)) / (cc_MaxSpecialUnitNo-1));
        return(KRERR_NO_ERROR);
     default : return(KRERR_NO_ERROR);
   }
}


/*****************************************************************************
  FUNCTION : cc_MakeMiscCalculationsForModifications

  PURPOSE  : <CCM>    calculate Height of layer iff necessary.
             <CCS>    generates the list of the output-Units of a group.
  NOTES    : 

  UPDATE   : 17.11.95 <Juergen Gatter>
******************************************************************************/

void cc_MakeMiscCalculationsForModifications(void)
{
   float fTemp;
   int   Height,i,j,k,chose;

   switch(cc_modification){
      case(CC_STAT) :
         if (ccm_MissingUnitsInLayer == 0){
            if(NoOfLayers==0){
               fTemp = CCM_HEIGHT;
               Height= (int)fTemp;
	    }else{
               fTemp=
                  exp((-NoOfLayers*CCM_DAEMPFUNG))*CCM_HEIGHT;
                         /* calculate funtion */
               Height = (int)(fTemp+
                     (drand48()*2.0*CCM_DIFF_HEIGHT-CCM_DIFF_HEIGHT));
                         /* add or substract radom didderence */
            }     
            PRINTF("HeightOfLayer %d set to %d.\n",NoOfLayers,((Height>0) ? Height : 1));
        
            ccm_CurrentLayerHeight = ccm_MissingUnitsInLayer = ((Height>0) ? Height : 1);
                        /* Height is correct, if it's bigger than 0 */
	 }
         ccm_MissingUnitsInLayer--;
         return;
      case(CC_GCC):
         for(i=0;i<NoOfOutputUnits;i++)
            ccs_GroupList[i] = FIELD_EMPTY;  /* all places are free */
         for(i=0;i<CCS_NO_OF_GROUPS;i++){
            chose=(int)(drand48()*(NoOfOutputUnits-i));                             
            /* I want the OutputUnit No chose, which is left, in my group */
            for(j=0;(k<chose)||(ccs_GroupList[j]!=FIELD_EMPTY);j++){
            /* Count from the start, and take the chosen Unit which is left.
               j can't get greater or equal NoOfOutputUnits, because there
               must be (NoOfOutputUnits-i) empty places in ccs_groupList. */
                if (ccs_GroupList[j] == FIELD_EMPTY) k++;
            }
            ccs_GroupList[j] = i; /* it gets the no of the group. */
	 }
         /* Now it's guaranteed that it's exactly one output-unit in
            every group. The rest of the Output-Units will distributed
            on the groups. */
         for (j=0;j<NoOfOutputUnits;j++){
            if (ccs_GroupList[j] == FIELD_EMPTY)
                ccs_GroupList[j] = (int)(drand48()*CCS_NO_OF_GROUPS);
	 }
         return;
      default :  
         return;
   }
}

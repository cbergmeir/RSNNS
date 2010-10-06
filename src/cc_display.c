/****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_display.c,v $
  SHORTNAME      : cc_display.c
  SNNS VERSION   : 4.2

  PURPOSE        : Display Functions of Cascade-Correlation
  NOTES          :

  AUTHOR         : Michael Schmalzl /  Juergen Gatter
  DATE           : 26.11.95

  CHANGED BY     : Michael Vogt, Guenter Mamier, Christian Wehrfritz
  RCS VERSION    : $Revision: 1.10 $
  LAST CHANGE    : $Date: 1998/03/13 16:23:49 $

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

#include "kr_typ.h"	
#include "kr_const.h"	 
#include "kr_def.h"	 
#include "kr_mac.h"
#include "kernel.h"
#include "kr_ui.h"
#include "kr_newpattern.h"
#include "cc_mac.h"
#include "cc_type.h"
#include "cc_glob.h"

#include "cc_display.ph"



/*****************************************************************************
  FUNCTION : cc_calculateNetParameters

  PURPOSE  : Calculates the position of the current input, hidden and output
             layer.
  NOTES    :

  UPDATE   : 13.11.95 <changed by Juergen Gatter>
******************************************************************************/

krui_err cc_calculateNetParameters()
{         
 struct Unit *unitPtr;
 int x,y;
 int xPosOfLastInsertedHiddenUnit,yPosOfLastInsertedHiddenUnit;

 int inputXMin=MAX_POS, inputXMax=0,
     hiddenXMin=MAX_POS,hiddenXMax=0,
     outputXMin=MAX_POS,outputXMax=0;
 int inputYMin=MAX_POS,inputYMax=0,
     hiddenYMin=MAX_POS,hiddenYMax=0,
     outputYMin=MAX_POS,outputYMax=0;
 int xInputOffset=0,xHiddenOffset=0,xOutputOffset=0;
 int yInputOffset=0,yHiddenOffset=0,yOutputOffset=0;

 NoOfInputUnits  = 0;
 NoOfHiddenUnits = 0;
 NoOfOutputUnits = 0;

 FOR_ALL_UNITS(unitPtr){  /* calculate the dimensions of the actual net */
   x = GET_UNIT_XPOS(unitPtr);
   y = GET_UNIT_YPOS(unitPtr);

   if(IS_INPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)){
     NoOfInputUnits++;
     inputYMax = MAX(y,inputYMax); inputYMin = MIN(y,inputYMin);
     inputXMax = MAX(x,inputXMax); inputXMin = MIN(x,inputXMin);
   }
   else if(IS_HIDDEN_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)){
     NoOfHiddenUnits++;
     hiddenYMax = MAX(y,hiddenYMax); hiddenYMin = MIN(y,hiddenYMin);
     hiddenXMax = MAX(x,hiddenXMax); hiddenXMin = MIN(x,hiddenXMin);
   }
   else if(IS_OUTPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)){
     NoOfOutputUnits++;
     outputYMax = MAX(y,outputYMax); outputYMin = MIN(y,outputYMin);
     outputXMax = MAX(x,outputXMax); outputXMin = MIN(x,outputXMin);
   }
 }

 if(NoOfHiddenUnits==0){ /* init Positions for empty network */
   hiddenXMax = hiddenXMin = X_MIN_POS + (inputXMax-inputXMin) + 3;
   hiddenYMax = hiddenYMin = Y_MIN_POS;
 }

 xPosOfLastInsertedHiddenUnit =
       X_MIN_POS + (inputXMax - inputXMin) + (hiddenXMax - hiddenXMin) + 3;

 yPosOfLastInsertedHiddenUnit = Y_MIN_POS-1;

 cc_outputXMax =
       X_MIN_POS + (inputXMax-inputXMin) + (hiddenXMax-hiddenXMin) + 
                   (outputXMax-outputXMin) + 6;

 xInputOffset = X_MIN_POS - inputXMin;
 yInputOffset = Y_MIN_POS - inputYMin;
 xHiddenOffset = X_MIN_POS + (inputXMax-inputXMin) - hiddenXMin + 3;
 yHiddenOffset = Y_MIN_POS - hiddenYMin;
 xOutputOffset = X_MIN_POS + (inputXMax-inputXMin) + (hiddenXMax-hiddenXMin) - outputXMin + 4;
 yOutputOffset = Y_MIN_POS - outputYMin;   

 cc_lastFirstOutputRow=MAX_POS;

 FOR_ALL_UNITS(unitPtr){
   if(IS_INPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr) ) {
     SET_UNIT_XPOS(unitPtr,GET_UNIT_XPOS(unitPtr)+xInputOffset);
     SET_UNIT_YPOS(unitPtr,GET_UNIT_YPOS(unitPtr)+yInputOffset);
   }
   if(IS_HIDDEN_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
     SET_UNIT_XPOS(unitPtr,GET_UNIT_XPOS(unitPtr)+xHiddenOffset);
     SET_UNIT_YPOS(unitPtr,GET_UNIT_YPOS(unitPtr)+yHiddenOffset);
   }
   if(IS_OUTPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr) ) {
     SET_UNIT_XPOS(unitPtr,GET_UNIT_XPOS(unitPtr)+xOutputOffset);
     SET_UNIT_YPOS(unitPtr,GET_UNIT_YPOS(unitPtr)+yOutputOffset);
     if(cc_lastFirstOutputRow > GET_UNIT_XPOS(unitPtr))
        cc_lastFirstOutputRow =  GET_UNIT_XPOS(unitPtr);

   }
 }

 if(NoOfHiddenUnits!=0) {
   FOR_ALL_UNITS(unitPtr){
     if(IS_HIDDEN_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
       x = GET_UNIT_XPOS(unitPtr);
       y = GET_UNIT_YPOS(unitPtr);
       if(x == xPosOfLastInsertedHiddenUnit){
         if(y >= yPosOfLastInsertedHiddenUnit){
           yPosOfLastInsertedHiddenUnit = y;
         }
       }
     }
   }
 }

 FOR_ALL_UNITS(unitPtr){
   if(IS_INPUT_UNIT(unitPtr))
     CC_SET_LAYER_NO(unitPtr,0);
   if(IS_OUTPUT_UNIT(unitPtr))
     CC_SET_LAYER_NO(unitPtr,NoOfLayers+2);
 }

 cc_outputXMax=/* *outXMax; */ X_MIN_POS+4;

 /* cc_display_mode = Y_MAX_MODE1;
 cc_LayerDistance = 2;
 */
 cc_hiddenXminPos = hiddenXMin;

 return(KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : cc_setSpecialUnits

  PURPOSE  : Positions the SpecialUnits
  NOTES    :

  UPDATE   : 25.11.95 <Juergen Gatter>
******************************************************************************/


void cc_setSpecialUnits(int X_Max)
{
  int Multiplikator,cnt;
  struct Unit* UnitPtr;

  if((cc_MaxSpecialUnitNo==0)||(NetLearnAlgorithm==TACOMA)) return;
  Multiplikator = MAX(1,(X_Max-X_MIN_POS)/cc_MaxSpecialUnitNo);

  FOR_ALL_SPECIAL_UNITS(UnitPtr,cnt){
     SET_UNIT_XPOS(UnitPtr,X_MIN_POS+Multiplikator*cnt);
     SET_UNIT_YPOS(UnitPtr,Y_MIN_POS-2);
  }
}

/*****************************************************************************
  FUNCTION : cc_setHiddenUnit

  PURPOSE  : Positions the hidden layer.
  NOTES    : originally, only the new hidden unit was placed.
             But with the modifications, it's easier to re-place
             all hidden/output-units new.

  UPDATE   : 30.03.96
******************************************************************************/
krui_err cc_setHiddenUnit(struct Unit *NotUsedUnitPtr, int LayerOfNewUnit)

{

  struct Unit* UnitPtr;
  int i,LayerNo;
  int* UnitCount;
  int diff;

  CALLOC_ERRORCHECK(UnitCount,NoOfLayers+2,int);

  UnitCount[0]=0;
  for(i=1;i<=NoOfLayers;i++){
      ListOfLayers[i].xPosFirstRow = FIRST_ROW_NEXT_LAYER(i-1);
      UnitCount[i]=0;
  }

  diff = FIRST_ROW_NEXT_LAYER(NoOfLayers)-cc_lastFirstOutputRow;

  cc_outputXMax = cc_lastFirstOutputRow = FIRST_ROW_NEXT_LAYER(NoOfLayers);
 
  if ((LayerOfNewUnit==0)||((ListOfLayers[LayerOfNewUnit].NoOfUnitsInLayer % cc_display_mode)==1))
  {               
    cc_redisplay();                       /* look, if we must change the mode */
    cc_setSpecialUnits(cc_outputXMax+2);  /* re-place special units */ 
  }

  FOR_ALL_UNITS(UnitPtr){
      if((IS_HIDDEN_UNIT(UnitPtr))||
	 ((NetLearnAlgorithm==TACOMA)&&(IS_SPECIAL_UNIT(UnitPtr)))) {
	  LayerNo=CC_LAYER_NO(UnitPtr);
	  SET_UNIT_XPOS(UnitPtr,ListOfLayers[LayerNo].xPosFirstRow +
			UnitCount[LayerNo] / cc_display_mode);
	  SET_UNIT_YPOS(UnitPtr,Y_MIN_POS+UnitCount[LayerNo] % cc_display_mode);
	  UnitCount[LayerNo]++;
      }else if(IS_OUTPUT_UNIT(UnitPtr)){
/*	  SET_UNIT_XPOS(UnitPtr,FIRST_ROW_NEXT_LAYER(NoOfLayers));*/
	  SET_UNIT_XPOS(UnitPtr,GET_UNIT_XPOS(UnitPtr)+diff);
      }
  }
  FREE_IF_USED(UnitCount); 
  return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : cc_updatePosOfSpecialUnits

  PURPOSE  : Updates the position of the special units.
  NOTES    :

  UPDATE   : 30.03.96
******************************************************************************/

void cc_updatePosOfSpecialUnits(void)
{
 int x,outputXMax=0;
 struct Unit *unitPtr;
 
 if(cc_cascade) {
   FOR_ALL_UNITS(unitPtr) {
     if(IS_OUTPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)){
       x = GET_UNIT_XPOS(unitPtr);
       if((x >= outputXMax) || (outputXMax==0)){
         outputXMax = x;
       }
     }
   }
   cc_setSpecialUnits(outputXMax+2);
   cc_cascade = 0;
   cc_redisplay();
 }
}



/*****************************************************************************
  FUNCTION : void cc_redisplay

  PURPOSE  : changes, iff necessary, the display-mode.
  NOTES    :

  UPDATE   : 20.11.95 <Juergen Gatter>
******************************************************************************/

void cc_redisplay(void)
{
  int MaxUnitsInLayer = 0;
  int i;

  for(i=1;i<=NoOfLayers;i++){
      ListOfLayers[i].xPosFirstRow = FIRST_ROW_NEXT_LAYER(i-1);
  }                 /* eval the pos of the outputlayer */

  if (FIRST_ROW_NEXT_LAYER(NoOfLayers) < X_MAX_DISPLAYABLE) return; 
  /* ah, we must change something */

  for(i=0;i<=NoOfLayers;i++)
     MaxUnitsInLayer = MAX(ListOfLayers[i].NoOfUnitsInLayer,MaxUnitsInLayer);
  
  if (MaxUnitsInLayer <= cc_display_mode){
           /* then a change of cc_display_mode wouldn't help */
     cc_LayerDistance = MAX(1,cc_LayerDistance-1);
     if (cc_LayerDistance==1)
        cc_display_mode = Y_MAX_MODE5;
  }        /* if LayerDistance==1 there could be arbritrary large Layers. */
  else{    /* I change the display mode */
     switch(cc_display_mode){
        case(Y_MAX_MODE1) : cc_display_mode = Y_MAX_MODE2; break;
        case(Y_MAX_MODE2) : cc_display_mode = Y_MAX_MODE3; break;
        case(Y_MAX_MODE3) : cc_display_mode = Y_MAX_MODE4; break;
        case(Y_MAX_MODE4) : cc_display_mode = Y_MAX_MODE5; break;
        case(Y_MAX_MODE5) : return;
     } 
  }
  
 if(cc_display_mode != Y_MAX_MODE5) cc_redisplay();
  return;
}


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_mac.h,v $
  SHORTNAME      : mac.h
  SNNS VERSION   : 4.2

  PURPOSE        : macros for cascade correlation and TACOMA
  NOTES          :

  AUTHOR         : Michael Schmalzl
  DATE           : 24.2.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.12 $
  LAST CHANGE    : $Date: 1998/02/25 15:25:59 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _CC_MAC_DEFINED_
#define _CC_MAC_DEFINED_

/* constants for all algorithms */

#define NO_INC_ALGORITHM 0
#define CC 1
#define TACOMA 3

#define OFF 0
#define ON  1


#define FIELD_EMPTY -1

#define NO_OF_ACT_FUNCS 7

#define ASYM_SIGMOID 0
#define SYM_SIGMOID  1
#define TANH         2
#define THRESHOLD    3
#define SINUS        4
#define EXPONENTIAL  5
#define RANDOM       6  /* RANDOM has to be last Act_function !!! */

#define SIN_FAKTOR   0.1    
#define THRESHOLD_DERIV 1   /* not used anymore */

#define BACKPROP         0
#define BACKPROP_ONLINE  1
#define QUICKPROP        2
#define RPROP            3

#define SBC          0
#define AIC          1
#define CMSEP        2

#define CC_NO_MOD    0
#define CC_SDCC      1
#define CC_LFCC      2
#define CC_RLCC      3
#define CC_ECC       4
#define CC_GCC       5
#define CC_STAT      6

#define CONTINUE_LEARNING 1
#define STOP_LEARNING 0

#define CC_MAX_VALUE  0.1 
#define FLOAT_MAX 1E+37



     /* these are the standard parameter settings */

/* output parameter definitions */
#define  OUT_PATIENCE                  50 
#define  MAX_NO_OF_ERROR_UPDATE_CYCLES 200
#define  MIN_ERROR_CHANGE              0.01

/* special parameter definitions */
#define  SPECIAL_PATIENCE                   25 
#define  MAX_NO_OF_COVARIANCE_UPDATE_CYCLES 200
#define  MIN_COVARIANCE_CHANGE              0.04
#define  MAX_SPECIAL_UNIT_NO                8
#define  SPECIAL_FUNC_TYPE                  SYM_SIGMOID

/* global parameter definitions */
#define  MAX_PIXEL_ERROR 0.2
#define  LEARNING_FUNC   QUICKPROP
#define  MODIFICATION    CC_NO_MOD
#define  ON_OFF   OFF
#define  CACHING_ONOFF ON


/* Constants for the  display-functions */

#define X_MIN_POS 2
#define Y_MIN_POS 3
#define MAX_POS   1000000
#define MIN_HIDDEN_LAYER_HEIGHT 5

#define Y_MAX_MODE1                      5
#define Y_MAX_MODE2                      8
#define Y_MAX_MODE3                     12
#define Y_MAX_MODE4                     20
#define Y_MAX_MODE5                     MAX_POS
#define X_MAX_DISPLAYABLE               30
#define DEFAULT_DISTANCE_BETWEEN_LAYERS  2
#define DISTANCE_TO_INOUT_LAYERS         3

/* Output-constants */    

#define LENGTH_HEADLINE                 80



/* Size of one block of the layerlist */

#define MINIMAL_LAYERLIST_SIZE 100

/* where the slopes are stored */

#define LN_CURRENT_SLOPE(LinkPtr)       LinkPtr->value_a
#define LN_PREVIOUS_SLOPE(LinkPtr)      LinkPtr->value_b
#define LN_LAST_WEIGHT_CHANGE(LinkPtr)  LinkPtr->value_c

#define BIAS_CURRENT_SLOPE(UnitPtr)      UnitPtr->value_a
#define BIAS_PREVIOUS_SLOPE(UnitPtr)     UnitPtr->value_b
#define BIAS_LAST_WEIGHT_CHANGE(UnitPtr) UnitPtr->value_c


/* Macros for all learn algorithms */

#define  NET_ERROR( param )      param[ 0 ]  /*    returns the net error   */

#define GET_UNIT_NO(UnitPtr) \
   ((int) ((UnitPtr) - unit_array))  

#define GET_UNIT_XPOS(UnitPtr) \
   (*UnitPtr).unit_pos.x

#define GET_UNIT_YPOS(UnitPtr) \
   (*UnitPtr).unit_pos.y

#define SET_UNIT_XPOS(UnitPtr,XPos) \
   (*UnitPtr).unit_pos.x = XPos 

#define SET_UNIT_YPOS(UnitPtr,YPos) \
   (*UnitPtr).unit_pos.y = YPos

/* error-macros */

#define ERROR_CHECK \
   if(KernelErrorCode!=KRERR_NO_ERROR) { \
     return(KernelErrorCode); \
   }  

#define ERROR_CHECK_WITH_MEMORY_DEALLOCATION \
   if((TempErrorCode=KernelErrorCode)!=KRERR_NO_ERROR) { \
     cc_freeStorage(StartPattern,EndPattern,0); \
     return(TempErrorCode); \
   } 

#define TAC_ERROR_CHECK_WITH_MEMORY_DEALLOCATION \
   if((TempErrorCode=KernelErrorCode)!=KRERR_NO_ERROR) { \
     tac_freeStorage(StartPattern,EndPattern); \
     return(TempErrorCode); \
   } 

#define UPS_ERROR_CHECK_WITH_MEMORY_DEALLOCATION \
   if((TempErrorCode=KernelErrorCode)!=KRERR_NO_ERROR) { \
     ups_freeStorage(StartPattern,EndPattern); \
     return(TempErrorCode); \
   } 

#define ERROR_CHECK_WRC \
   if(KernelErrorCode!=KRERR_NO_ERROR) { \
     return; \
   } 

#define CC_ERROR(ErrorCode) \
   return(ErrorCode)
 
/* memory allocation macros */

#define FREE_IF_USED(i) \
   if((i) != NULL){free(i); i=NULL; }

#define FREE_2DIMENSIONAL_ARRAY(Anker,NoOfRows,i) \
   if (Anker!=NULL){ \
      FREE_IF_USED(Anker[0]); \
      FREE_IF_USED(Anker); \
      Anker = NULL; \
   }

#define FREE_2DIMENSIONAL_ARRAY_WITH_PRINT(Anker,NoOfRows,i) \
   if (Anker!=NULL){ \
     FREE_IF_USED(Anker[0]);  \
      Anker = NULL; \
   }

#define FREE_2ND_ARRAY(Anker,NoOfRows,UnderType,i) \
   if (Anker!=NULL){ \
      FREE_IF_USED(Anker[0].UnderType); \
   }

#define CALLOC_ERRORCHECK(Anker,Anzahl,Datentyp) \
 if( (Anker = (Datentyp *) calloc(Anzahl,sizeof(Datentyp))) == NULL){ \
      CC_ERROR(KernelErrorCode=KRERR_CC_ERROR3); \
 }

#define CALLOC_2DIMENSIONAL_ARRAY(Anker,NoOfRows,NoOfCols,DatenTyp,i) \
  { \
     CALLOC_ERRORCHECK(Anker,NoOfRows,DatenTyp*); \
     CALLOC_ERRORCHECK(Anker[0],(NoOfRows)*(NoOfCols),DatenTyp); \
     for(i=1;i<NoOfRows;i++) \
     { \
        Anker[i]=Anker[i-1]+NoOfCols; \
     } \
  } \

#define CALLOC_2ND_ARRAY(Anker,NoOfRows,UnderType,NoOfCols,DataType,i) \
  { \
     CALLOC_ERRORCHECK(Anker[0].UnderType,(NoOfRows)*(NoOfCols),DataType); \
     for(i=1;i<(NoOfRows);i++) \
     { \
        Anker[i].UnderType=Anker[i-1].UnderType+(NoOfCols); \
     } \
  }
  


/* propagation macros */
extern FlintType OUT_Custom_Python(FlintType act);

#define CALCULATE_ACTIVATION_AND_OUTPUT(UnitPtr,value,p) \
 { \
    UnitPtr->Out.output = ((UnitPtr->out_func == OUT_IDENTITY) ? \
                          (UnitPtr->act = value) : \
			  ((UnitPtr->out_func == OUT_Custom_Python) ? \
			   kr_PythonOutFunction(UnitPtr->python_out_func, \
			    UnitPtr->act = value) : \
                          (*UnitPtr->out_func) (UnitPtr->act = value))); \
 }

#define CALCULATE_INPUTUNIT_ACTIVATION_AND_OUTPUT(UnitPtr,value) \
    UnitPtr->Out.output = ((UnitPtr->out_func == OUT_IDENTITY) ? \
                          (UnitPtr->act = value) : \
			  ((UnitPtr->out_func == OUT_Custom_Python) ? \
			   kr_PythonOutFunction(UnitPtr->python_out_func, \
			     UnitPtr->act = value) : \
                          (*UnitPtr->out_func) (UnitPtr->act = value))); \

#define PROPAGATE_THROUGH_INPUT_LAYER(inputUnitPtr,dummy,pattern) \
    FOR_ALL_INPUT_UNITS(inputUnitPtr,dummy){ \
        CALCULATE_INPUTUNIT_ACTIVATION_AND_OUTPUT(inputUnitPtr,*pattern++); \
    }

#define PROPAGATE_THROUGH_HIDDEN_LAYER(hiddenUnitPtr,dummy,pattern) \
    FOR_ALL_HIDDEN_UNITS(hiddenUnitPtr,dummy) { \
        CALCULATE_ACTIVATION_AND_OUTPUT(hiddenUnitPtr, \
                  ((hiddenUnitPtr->act_func == ACT_Custom_Python) ? \
			kr_PythonActFunction(hiddenUnitPtr->python_act_func, \
						hiddenUnitPtr) : \
			(hiddenUnitPtr->act_func) (hiddenUnitPtr)) ,pattern); \
    }

#define PROPAGATE_THROUGH_OUTPUT_LAYER(outputUnitPtr,dummy,pattern) \
    FOR_ALL_OUTPUT_UNITS(outputUnitPtr,dummy) { \
        CALCULATE_ACTIVATION_AND_OUTPUT(outputUnitPtr, \
                  ((outputUnitPtr->act_func == ACT_Custom_Python) ? \
			kr_PythonActFunction(outputUnitPtr->python_act_func, \
						outputUnitPtr) : \
			(outputUnitPtr->act_func) (outputUnitPtr)) ,pattern); \
    }
#define PROPAGATE_THROUGH_SPECIAL_LAYER(specialUnitPtr,dummy,pattern) \
	FOR_ALL_SPECIAL_UNITS(specialUnitPtr,dummy) { \
            CALCULATE_ACTIVATION_AND_OUTPUT(specialUnitPtr, \
                      ((specialUnitPtr->act_func == ACT_Custom_Python) ? \
			kr_PythonActFunction(specialUnitPtr->python_act_func, \
						specialUnitPtr) : \
			(specialUnitPtr->act_func) (specialUnitPtr)) ,pattern); \
    }

/* modification macros */

#define UNITS_IN_SAME_GROUP(spec,outpt) \
    ((cc_modification!=CC_GCC)|| \
    ((spec%CCS_NO_OF_GROUPS)==ccs_GroupList[outpt]))

#define NO_OF_GROUPS ((cc_modification!=CC_GCC) ? 1 : CCS_NO_OF_GROUPS)


/* misc macros */

#define IF_PTR_IS_NOT_NULL(ptr) if(ptr!=NULL)

#define FOR_ALL_INPUT_UNITS(UnitPtr,i) \
   for(UnitPtr= *FirstInputUnitPtr,i=0;UnitPtr!=NULL;UnitPtr=FirstInputUnitPtr[++i])

#define FOR_ALL_HIDDEN_UNITS(UnitPtr,h) \
   for(UnitPtr= *FirstHiddenUnitPtr,h=0;UnitPtr!=NULL;UnitPtr=FirstHiddenUnitPtr[++h])

#define FOR_ALL_OUTPUT_UNITS(UnitPtr,o) \
   for(UnitPtr= *FirstOutputUnitPtr,o=0;UnitPtr!=NULL;UnitPtr=FirstOutputUnitPtr[++o])

#define FOR_ALL_SPECIAL_UNITS(UnitPtr,s) \
   for(UnitPtr= *FirstSpecialUnitPtr,s=0;UnitPtr!=NULL;UnitPtr=FirstSpecialUnitPtr[++s])
          
#define FOR_ALL_PATTERNS(StartPattern,EndPattern,p) \
   for(p=StartPattern;p<=EndPattern;p++)

#define SIGN(n) (((n) > 0) ? (1) : (-1))

#define GET_RECURRENT_LINK(unit_ptr,link_ptr) \
   (link_ptr) = (struct Link *) (unit_ptr)->sites 

#define FOR_ALL_NOT_RECURRENT_LINKS( unit_ptr, link_ptr ) \
for ((link_ptr) = ((struct Link *) (unit_ptr)->sites)->next; (link_ptr) != NULL; \
     (link_ptr) = (link_ptr)->next)

#define MAX(a,b) (((a)>(b))?(a):(b))

#define MIN(a,b) (((a)<(b))?(a):(b))

#define NO_OF_NET_UNITS \
   (NoOfInputUnits+NoOfOutputUnits+NoOfHiddenUnits)

#define NO_OF_INOUT_UNITS \
   (NoOfInputUnits+NoOfOutputUnits)

#define REL_HIDDEN_NO(a) \
   ((a)-NoOfInputUnits-NoOfOutputUnits)


#define DISTANCE_BETWEEN_LAYERS(a) \
   (((a)==0)||((a)==NoOfLayers)) ? DISTANCE_TO_INOUT_LAYERS : cc_LayerDistance


#define CC_LAYER_NO(Ptr) ((Ptr->lln >= 0) ? Ptr->lln : ((-1) - Ptr->lln))

#define CC_SET_LAYER_NO(Ptr,LayerNo) \
   if (Ptr->lln >= 0) \
      Ptr->lln = LayerNo; \
   else \
      Ptr->lln = (-1) - LayerNo;

#define FIRST_ROW_NEXT_LAYER(i) \
   ((i>0) ? \
   (ListOfLayers[i].xPosFirstRow + \
   (((ListOfLayers[i].NoOfUnitsInLayer)-1) / cc_display_mode) + \
   (DISTANCE_BETWEEN_LAYERS(i))) : \
   ListOfLayers[i].xPosFirstRow)

#define PRINTF  if (cc_printOnOff) printf 

#define SIGN_OF_THE_CORRELATION CorBetweenSpecialActAndOutError

#define OUTPUT_UNIT_SUM_ERROR   MeanOutputUnitError
/* Aus Performance-Gruenden wird Variable 2-mal benutzt. 
*/
/* */

/* Parameters for Cascade-Correlation */


#define PARAM1                          ParameterInArray[0]
#define PARAM2                          ParameterInArray[1]
#define PARAM3                          ParameterInArray[2]
#define PARAM4                          ParameterInArray[3]
#define PARAM5                          ParameterInArray[4]
#define PARAM6                          0.0001
#define MAX_PIXEL_ERR                   ParameterInArray[6]
#define LEARNING_FUNCTION               (int)ParameterInArray[7]
#define CC_PRINT_ONOFF                  (int)ParameterInArray[8]
#define MIN_COVAR_CHANGE                ParameterInArray[9]
#define SPEC_PATIENCE                   ParameterInArray[10]
#define MAX_NO_OF_COVAR_UPDATE_CYCLES   ParameterInArray[11] 
#define MAX_SPECIAL_UNIT_NUMBER         ParameterInArray[12] 
#define SPECIAL_FUNCTION_TYPE           (int)ParameterInArray[13] 
#define MINIMAL_ERROR_CHANGE            ParameterInArray[14]
#define OUT_PATIEN                      ParameterInArray[15] 
#define MAX_NO_ERROR_UPDATE_CYCLES      ParameterInArray[16]
#define CC_PRUNE_ONOFF                  (int)ParameterInArray[17]
#define CC_BACKFITTING_ONOFF            (int)ParameterInArray[18]
#define BACKFITT_PATIENCE               ParameterInArray[19]
#define PRUNE_FUNC                      (int)ParameterInArray[20]
#define MODIFICATION_NO                 (int)ParameterInArray[21]
#define CC_PARAMETER1                   ParameterInArray[22]
#define CC_PARAMETER2                   ParameterInArray[23]
#define CC_PARAMETER3                   ParameterInArray[24]
#define CC_PARAMETER4                   ParameterInArray[25]
#define CC_PARAMETER5                   ParameterInArray[26]
#define CC_FASTMODE                     ParameterInArray[27]

/* Parameters for the modifications of CC */


#define CCM_HEIGHT                      cc_Parameter[0]
#define CCM_DIFF_HEIGHT                 cc_Parameter[1]
#define CCM_DAEMPFUNG                   cc_Parameter[2]

#define CCR_NO_OF_LINKS                 (int)cc_Parameter[0]

#define CCC_M                           cc_Parameter[0]

#define CCO_FAKTOR                      cc_Parameter[0]

#define CCS_NO_OF_GROUPS                (int)cc_Parameter[0]

#define CCB_LAMBDA                      cc_Parameter[0]

#define SGN(x) (((x)==0.0)?0.0:(((x)<0.0)?-1.0:1.0))


/* And now .... the TACOMA macros... */


#define TAC_ALPHA(d,max) (0.1 * ((max-d)/(max)))


#define TAC_KOHONEN  ((int)cc_Parameter[0])
   /* how much runs to determine the best special units ? */
#define TAC_XI_RI_ETA  (cc_Parameter[1])
     /* step-width learning of xi and ri */
#define TAC_THRESHOLD cc_Parameter[2]
   /* All special units with g over this threshold were built in */
#define TAC_LAMBDA    cc_Parameter[3]
   /* A connection ist built in, iff the correlation is better than lambda */
#define TAC_BETA      cc_Parameter[4]
   /* To determine the initial radius of a link.*/
#define TAC_ETA       0.7


#define TAC_XIRI_ONLINE TRUE


#define TAC_EXP(x) \
	( (x>88.72) ? MAXFLOAT : ((x<-88.0) ? 0.0 : exp(x)) )

#define XI_OF_LINK(LinkPtr) \
	(LinkPtr->value_b)

#define RI_OF_LINK(LinkPtr) \
	(LinkPtr->value_a)
 
/* if you change  XI_OF_LINK and/or RI_OF_LINK, then take a look at the loading
   routines at krio_readConnectionDefs (kr_io.c) */


#define SUMMED_DISTANCES Ri

#define TAC_MAX_VALUE 1.0

/* Macros for Upstart */


#define UPS_MAX_VALUE 1.0

#define UPS_BEST_WEIGHT(LinkPtr) LinkPtr->value_c

#define UPS_MODIFIKATION_ON  (cc_Parameter[0] > 0.0)

#define UPS_ETA (cc_Parameter[1])

#define UPS_NEW_MODE (cc_Parameter[1]>0.0)

#define UPS_TEST (cc_Parameter[2]>0.0)

#define UPS_NOT_CALCULATED 0

#define UPS_GREAT_VALUE 10000

#define UPS_DAUGHTER_LINK(LinkPtr) LinkPtr->value_a

#endif

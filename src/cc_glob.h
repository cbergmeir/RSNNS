/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_glob.h,v $
  SHORTNAME      : cc_glob.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          : This file was put together from the earlier files cc_rcc 
                   and cc_rcc_topo

  AUTHOR         : Michael Schmalzl
  DATE           : 5.2.92

  CHANGED BY     : Michael Schmalzl
  RCS VERSION    : $Revision: 2.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:25:52 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _CC_GLOB_DEFINED_
#define  _CC_GLOB_DEFINED_

/* begin global definition section */
extern void      cc_printHeadline(char* s,int Length);
extern float     cc_getErr (int StartPattern, int EndPattern);
extern void      cc_LayerCorrectnessTest(float* ParameterInArray, 
					 int StartPattern,
					 int EndPattern);
extern krui_err  cc_freeStorage(int StartPattern, int EndPattern, int flag);
extern krui_err  cc_deleteAllSpecialAndAllHiddenUnits(void);
extern krui_err  cc_allocateStorage(int StartPattern, int EndPattern, 
				    int NoOfSpecialUnits);
extern void      cc_initActivationArrays(void);
extern FlintType cc_generateRandomNo(float maxValue);
extern void      cc_getActivationsForActualPattern(int SubPatterNo,int First,
					      int* pat,int* sub);
extern void      cc_initOutputUnits(void);
extern krui_err  cc_getPatternParameter(int StartPattern, int Endpattern,
					int* start, int* end, int* n);
extern void      cc_initInputUnitsWithPattern(int PatternNo);
extern krui_err  cc_setPointers(void);
extern krui_err  cc_initSpecialUnitLinks(void);
extern krui_err  cc_deleteAllSpecialUnits(void); 

extern krui_err  cc_topoSort(int topoSortId);
extern void      cc_setCycletestFlag(struct Unit* UnitPtr);

extern struct CC_DATA cc_data; 
extern char *cc_onOffArray[];  
extern char *cc_actFuncArray[]; 
extern char *cc_actFuncArray2[]; 
extern char *cc_learningFuncArray[];
extern char *cc_ModificationArray[];
extern char *cc_pruningFuncArray[];
extern int cc_end;
extern int cc_storageFree;
extern int cc_allButtonIsPressed;
extern int cc_cascadeFlag;
/* pcc */
extern int LastInsertedHiddenUnit;
extern int cc_backfittingOnOff;
extern int cc_MaxSpecialUnitNo;
extern int cc_modification;
extern struct Unit  **FirstInputUnitPtr,**FirstHiddenUnitPtr,
                    **FirstOutputUnitPtr,**FirstSpecialUnitPtr,
                    *bestSpecialUnitPtr;

extern float **OutputUnitError;
extern float **SpecialUnitAct;
extern float **CorBetweenSpecialActAndOutError;
extern float *MeanOutputUnitError;
extern float *SpecialUnitSumAct;

extern float SumSqError;
extern int cc_printOnOff;
extern int *reset;
extern int cc_cascade;
extern int FirstUnitOnLastLayer;

extern float cc_Parameter[5];
extern int NoOfLayers;         /* Number of hidden layers */
extern struct CC_LAYER* ListOfLayers;  /* holds data for each layer */
extern int SizeOfLayerlist;   
extern float** ActOfUnit;
extern float cc_fse;
extern int cc_learningFunction;


/* now the variables needed by display */

extern int cc_display_mode;
extern int cc_LayerDistance;
extern int cc_outputXMax;

extern int cc_fastmode;
extern int cc_actualNetSaved;

extern krui_err (*cc_propagateSpecialUnitsBackward)
                             (int start,int end,int n,int counter,
                              float param1,float param2,float param3);
extern float (*cc_propagateOutputUnitsBackward)
                             (int PatternNo, int sub_pat_no,
                              float param1,float param2,float param3);
extern float (*cc_SpecialUnitUpdate)( float oldValue, float* previousSlope,
				      float* currentSlope, float* LastChange,
				      float param1,float param2,float param3);
extern float (*cc_OutputUnitUpdate)(float oldValue, float* previousSlope,
				    float* currentSlope, float* LastChange,
				    float param1,float param2,float param3);
extern float QuickPropOfflinePart(float oldValue, float* previousSlope, 
				  float* currentSlope, float* LastChange,
				  float epsilon,float mu,float decay);
extern float BackPropOfflinePart(float oldValue, float* previousSlope,
				 float* currentSlope,float* LastChange,
				 float dummy1,float dummy2,float dummy3);
extern float OfflineBackPropOfflinePart(float oldValue, float* previousSlope,
					float* currentSlope,float* LastChange,
					float eta,float mu,float dummy);
extern float RPropOfflinePart(float oldValue, float* previousSlope,
			      float* currentSlope, float* LastChange,
			      float epsilonMinus,float epsilonPlus,float dummy);
extern float OnlineBackPropOfflinePart(float oldValue, float* previousSlope,
				       float* currentSlope, float* LastChange,
				       float eta,float mu,float dummy);

extern int NetLearnAlgorithm;

/* end global definition section */

#endif /* _CC_GLOB_DEFINED_ */

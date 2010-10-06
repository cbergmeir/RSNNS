/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_glob.ph,v $
  SHORTNAME      : cc_glob.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          : This file was put together from the earlier files cc_rcc 
                   and cc_rcc_topo

  AUTHOR         : Michael Schmalzl
  DATE           : 5.2.92

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.5 $
  LAST CHANGE    : $Date: 1998/02/25 15:25:53 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _CC_GLOB_DEFINED_
#define  _CC_GLOB_DEFINED_

/* begin global definition section */
void      cc_printHeadline(char* s,int Length);
float     cc_getErr (int StartPattern, int EndPattern);
void      cc_LayerCorrectnessTest(float* ParameterInArray,
				  int StartPattern, int EndPattern);
krui_err  cc_freeStorage(int StartPattern, int EndPattern, int flag);
krui_err  cc_deleteAllSpecialAndAllHiddenUnits(void);
krui_err  cc_allocateStorage(int StartPattern, int EndPattern, 
			     int NoOfSpecialUnits);
FlintType cc_generateRandomNo(float maxValue);
void      cc_initActivationArrays(void);
void      cc_getActivationsForActualPattern(int SubPatterNo,int First,int* pat,
					    int* sub);
void      cc_initOutputUnits(void);
krui_err  cc_getPatternParameter(int StartPattern, int Endpattern,
				 int* start, int* end,int* n);
void      cc_initInputUnitsWithPattern(int PatternNo);
krui_err  cc_setPointers(void);
krui_err  cc_initSpecialUnitLinks(void);
krui_err  cc_deleteAllSpecialUnits(void);   

krui_err  cc_topoSort(int topoSortId);
void      cc_setCycletestFlag(struct Unit* UnitPtr);

float QuickPropOfflinePart(float oldValue, float* previousSlope,
			   float* currentSlope,float* LastChange,
			   float epsilon, float mu, float decay);
float BackPropOfflinePart(float oldValue, float* previousSlope,
			  float* currentSlope, float* LastChange,
			  float dummy1, float dummy2, float dummy3);
float OfflineBackPropOfflinePart(float oldValue, float* previousSlope,
				 float* currentSlope, float* LastChange,
				 float epsilon, float mu, float dummy);
float RPropOfflinePart(float oldValue, float* previousSlope, 
		       float* currentSlope, float* LastChange,
		       float epsilonMinus, float epsilonPlus, float dummy);
float OnlineBackPropOfflinePart(float oldValue, float* previousSlope,
				float* currentSlope, float* LastChange,
				float eta, float mu, float dummy);

struct CC_DATA  cc_data ={
  { MAX_PIXEL_ERROR,
    LEARNING_FUNC, MODIFICATION,
    ON_OFF, 0, 0, 150, SBC,
    { 0.0, 0.0, 0.0, 0.0, 0.0 },
    OFF},
  { MIN_COVARIANCE_CHANGE, 
    SPECIAL_PATIENCE,
    MAX_NO_OF_COVARIANCE_UPDATE_CYCLES,
    MAX_SPECIAL_UNIT_NO, 
    SPECIAL_FUNC_TYPE },
  { MIN_ERROR_CHANGE,
    OUT_PATIENCE,
    MAX_NO_OF_ERROR_UPDATE_CYCLES}
};

char *cc_actFuncArray[]     ={"Act_Logistic","Act_LogSym","Act_TanH",
                              "Act_CC_Thresh","Act_Sinus","Act_Exponential",
			      "Act_Random"};
char *cc_actFuncArray2[]     ={"Logistic"," LogSym ",
			      "  TanH  ","dummy"," Sinus  ",
			      "  Gauss "," Random "};
char *cc_learningFuncArray[]={" Batch-BP  "," Backprop  ",
			      " Quickprop ","   Rprop   "};
char *cc_ModificationArray[]={"   none ","   SDCC ","   LFCC ","   RLCC ",
			      "   ECC  ","   GCC  ","  Static"};
char *cc_pruningFuncArray[] ={" SBC "," AIC ","CMSEP"};

int cc_end                = 0;
int cc_storageFree        = 1;
int cc_allButtonIsPressed = 1;


struct Unit  **FirstInputUnitPtr,**FirstHiddenUnitPtr,
             **FirstOutputUnitPtr,**FirstSpecialUnitPtr,
              *bestSpecialUnitPtr;

float **OutputUnitError=NULL;
float **SpecialUnitAct=NULL;
float **CorBetweenSpecialActAndOutError=NULL;
float *MeanOutputUnitError=NULL;
float *SpecialUnitSumAct=NULL;
int   LastInsertedHiddenUnit;

float SumSqError;

int cc_printOnOff;
int cc_backfittingOnOff;
int cc_MaxSpecialUnitNo;
int cc_modification = -1;
int cc_fastmode;
float cc_Parameter[5];


int *reset=0;
int cc_cascade=0;
int NoOfLayers = 0;             /* number hidden layers */
Layer_Type* ListOfLayers=NULL;  /* data of layers */
int SizeOfLayerlist;  


int cc_outputXMax;

int     cc_actualNetSaved = FALSE;
float** ActOfUnit = NULL;
int     NetLearnAlgorithm = NO_INC_ALGORITHM;

float cc_fse = 0.1;
int cc_learningFunction;

int prot_fd = -1;
FILE* prot_stream;
char DumpText[1024];
bool cc_NetErgProtocolled;
int CountSpecTrains,CountOutTrains,CountLinks;

void Prot();
void prot(char* s);
void Prot_Parameters(float* Params,int NetIsCC);
int filedesc = -1;

/* the following functions are dummy declarations without implemented body!!
   They are realized by directing their pointers to other functions */
krui_err (*cc_propagateSpecialUnitsBackward)(int start, int end, int n,
					     int counter, float param1,
					     float param2, float param3);
float (*cc_propagateOutputUnitsBackward)(int PatternNo, int sub_pat_no,
					 float param1, float param2,
					 float param3);
float (*cc_SpecialUnitUpdate)(float oldValue, float* previousSlope,
			      float* currentSlope, float* LastChange,
			      float param1, float param2, float param3);
float (*cc_OutputUnitUpdate)(float oldValue, float* previousSlope,
			     float* currentSlope, float* LastChange,
			     float param1, float param2, float param3);

/* end global definition section */



/* begin privat definition section */

static int OldNoOfSpecialUnitStorage = 0;

#define LINKS_LEAVING(unitPtr)   unitPtr->value_a
#define LINKS_ARRIVEING(unitPtr) unitPtr->value_b
#define INPUT_LINKS(unitPtr)     unitPtr->value_c

static void  cc_clearFlags(void);
static void  DepthFirst4(struct Unit *unitPtr, int depth);
static void  DepthFirst5(struct Unit *unitPtr, int depth);
krui_err     cc_topoSortMain(int topoSortID);

static TopoPtrArray	global_topo_ptr;

/* end  privat definition section */

#endif /* _CC_GLOB_DEFINED_ */








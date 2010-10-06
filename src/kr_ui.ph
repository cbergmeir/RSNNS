/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_ui.ph,v $
  SHORTNAME      : kr_ui.ph
  SNNS VERSION   : 4.2
 
  PURPOSE        : SNNS-Kernel User Interface Function Prototypes
  NOTES          :
 
  AUTHOR         : Niels Mache
  DATE           : 27.02.90
 
  CHANGED BY     : Michael Vogt, Guenter Mamier, Christine Bagdi
  RCS VERSION    : $Revision: 2.25 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:17 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#ifndef _KR_UI_DEFINED_
#define  _KR_UI_DEFINED_

/* begin global definition section */


#include "krui_typ.h"	/*  Interface function prototypes  */

/*
#ifdef __MSDOS__

#ifndef  int
#define  int long
#endif

#endif
*/

void (*krui_errorHandler)(int)=NULL;
int krui_error_code;
bool rpckernel=FALSE;

int  krui_getNoOfUnits(void);
int  krui_getFirstUnit(void);
int  krui_getNextUnit(void);
int  krui_getCurrentUnit(void);
krui_err  krui_setCurrentUnit(int unit_no);
char  *krui_getUnitName(int UnitNo);
krui_err  krui_setUnitName(int unit_no, char *unit_name);
int  krui_searchUnitName(char *unit_name);
int  krui_searchNextUnitName(void);
char  *krui_getUnitOutFuncName(int UnitNo);
krui_err  krui_setUnitOutFunc(int unit_no, char *unitOutFuncName);
krui_err  krui_setUnitOutPyFunc(int unit_no, PyObject *outfunc);
char  *krui_getUnitActFuncName(int UnitNo);
krui_err  krui_setUnitActFunc(int unit_no, char *unitActFuncName);
char  *krui_getUnitFTypeName(int UnitNo);
FlintType  krui_getUnitActivation(int UnitNo);
krui_err  krui_setUnitActivation(int UnitNo, FlintTypeParam unit_activation);
FlintType  krui_getUnitInitialActivation(int UnitNo);
void  krui_setUnitInitialActivation(int UnitNo, FlintTypeParam unit_i_activation);
FlintType  krui_getUnitOutput(int UnitNo);
krui_err  krui_setUnitOutput(int unit_no, FlintTypeParam unit_output);
FlintType  krui_getUnitBias(int UnitNo);
void  krui_setUnitBias(int UnitNo, FlintTypeParam unit_bias);
FlintType  krui_getUnitValueA(int UnitNo);
void  krui_setUnitValueA(int UnitNo, FlintTypeParam unit_valueA);
int  krui_getUnitSubnetNo(int UnitNo);
void  krui_setUnitSubnetNo(int UnitNo, int subnet_no);
unsigned short	krui_getUnitLayerNo(int UnitNo);
void    krui_setUnitLayerNo(int UnitNo, int layer_no);
void    krui_getUnitPosition(int UnitNo, struct PosType *position);
void    krui_setUnitPosition(int UnitNo, struct PosType *position);
int   krui_getUnitNoAtPosition(struct PosType *position, int subnet_no);
int  krui_getUnitNoNearPosition(struct PosType *position, int subnet_no, int range, int gridWidth);
int  krui_getUnitTType(int unit_no);
krui_err  krui_setUnitTType(int unit_no, int UnitTType);
krui_err  krui_freezeUnit(int unit_no);
krui_err  krui_unfreezeUnit(int unit_no);
bool  krui_isUnitFrozen(int unit_no);
int  krui_getUnitInputType(int unit_no);
int  krui_createDefaultUnit(void);
int  krui_createUnit(char *unit_name, char *out_func_name, char *act_func_name, FlintTypeParam i_act, FlintTypeParam bias);
krui_err  krui_deleteUnit(int unit_no);
krui_err  krui_deleteUnitList(int no_of_units, int unit_list[]);
int  krui_createFTypeUnit(char *Ftype_symbol);
krui_err  krui_setUnitFType(int unit_no, char *Ftype_symbol);
int   krui_copyUnit(int unit_no, int copy_mode);
bool  krui_setFirstFTypeEntry(void);
bool  krui_setNextFTypeEntry(void);
bool  krui_setFTypeEntry(char *Ftype_symbol);
char  *krui_getFTypeName(void);
krui_err  krui_setFTypeName(char *Ftype_symbol);
char  *krui_getFTypeActFuncName(void);
krui_err  krui_setFTypeActFunc(char *act_func_name);
krui_err   krui_setFTypeOutFunc(char *out_func_name);
bool  krui_setFirstFTypeSite(void);
bool  krui_setNextFTypeSite(void);
char  *krui_getFTypeSiteName(void);
krui_err  krui_setFTypeSiteName(char *FType_site_name);
krui_err  krui_createFTypeEntry(char *Ftype_symbol, char *act_func_name, char *out_func_name, int no_of_sites, char **array_of_site_names);
krui_err    krui_deleteFTypeEntry(char *Ftype_symbol);
int  krui_getNoOfFunctions(void);
void  krui_getFuncInfo(int func_no, char **func_name, int *func_type);
bool  krui_isFunction(char *func_name, int func_type);
bool  krui_getFuncParamInfo(char *func_name, int func_type, int *no_of_input_params, int *no_of_output_params);
bool   krui_getFirstSiteTableEntry(char **site_name, char **site_func);
bool  krui_getNextSiteTableEntry(char **site_name, char **site_func);
char  *krui_getSiteTableFuncName(char *site_name);
krui_err  krui_createSiteTableEntry(char *site_name, char *site_func);
krui_err  krui_changeSiteTableEntry(char *old_site_name, char *new_site_name, char *new_site_func);
krui_err  krui_deleteSiteTableEntry(char *site_name);
bool  krui_setFirstSite(void);
bool  krui_setNextSite(void);
krui_err  krui_setSite(char *site_name);
FlintType   krui_getSiteValue(void);
char  *krui_getSiteName(void);
krui_err  krui_setSiteName(char *site_name);
char  *krui_getSiteFuncName(void);
krui_err  krui_addSite(char *site_name);
bool  krui_deleteSite(void);
int  krui_getFirstPredUnit(FlintType *strength);
int  krui_getNextPredUnit(FlintType *strength);
int  krui_getFirstPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c);
int  krui_getNextPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c);
int  krui_getCurrentPredUnit(FlintType *strength);
int  krui_getFirstSuccUnit(int source_unit_no, FlintType *weight);
int  krui_getNextSuccUnit(FlintType *weight);
bool  krui_areConnected(int source_unit_no, int target_unit_no);
bool  krui_areConnectedWeight(int source_unit_no, int target_unit_no, FlintType *weight);
bool  krui_isConnected(int source_unit_no);
FlintType  krui_getLinkWeight(void);
void  krui_setLinkWeight(FlintTypeParam weight);
krui_err  krui_createLink(int source_unit_no, FlintTypeParam weight);
struct Link*  krui_createLinkWithAdditionalParameters
      (int source_unit_no, FlintTypeParam weight,float val_a,float val_b,float val_c);
krui_err  krui_deleteLink(void);
krui_err  krui_deleteAllInputLinks(void);
krui_err  krui_deleteAllOutputLinks(void);

void  krui_jogWeights(FlintTypeParam minus, FlintTypeParam plus);
krui_err  krui_jogCorrWeights(FlintTypeParam minus, FlintTypeParam plus,
				  FlintTypeParam mincorr);
krui_err   krui_updateSingleUnit(int unit_no);
char  *krui_getUpdateFunc(void);
krui_err   krui_setUpdateFunc(char *update_func);
krui_err krui_setRemapFunc(char *name, float *params);
krui_err  krui_updateNet(float *parameterInArray, int NoOfInParams);
char  *krui_getInitialisationFunc(void);
krui_err   krui_setInitialisationFunc(char *initialisation_func);
krui_err  krui_initializeNet(float *parameterInArray, int NoOfInParams);

char  *krui_getLearnFunc(void);
krui_err   krui_setLearnFunc(char *learning_func);
krui_err krui_trainNetwork(NetLearnParameters *parameters);
krui_err krui_getNetworkErrorArray(double **learnErrors,int **atEpoch,int *noOfErrors);
krui_err krui_stopTraining(void);
krui_err   krui_learnAllPatterns(float *parameterInArray, int NoOfInParams, float **parameterOutArray, int *NoOfOutParams);
krui_err   krui_testAllPatterns(float *parameterInArray, int NoOfInParams, float **parameterOutArray, int *NoOfOutParams); /* joe */
krui_err   krui_learnSinglePattern(int pattern_no, float *parameterInArray, int NoOfInParams, float **parameterOutArray, int *NoOfOutParams);
krui_err   krui_testSinglePattern(int pattern_no, float *parameterInArray, int NoOfInParams, float **parameterOutArray, int *NoOfOutParams); /* joe */
int krui_checkPruning ();
krui_err   krui_learnAllPatternsFF(float *parameterInArray, int NoOfInParams, float **parameterOutArray, int *NoOfOutParams);
krui_err   krui_learnSinglePatternFF(int pattern_no, float *parameterInArray, int NoOfInParams, float **parameterOutArray, int *NoOfOutParams);

char *krui_getPrunFunc (void);
krui_err krui_setPrunFunc (char *pruning_func);

extern char *krui_getFFLearnFunc (void);
extern krui_err krui_setFFLearnFunc (char *FF_learning_func);

krui_err  krui_setClassDistribution(unsigned int *classDist);
krui_err  krui_setClassInfo(char *name);
krui_err  krui_useClassDistribution(bool use_it);
krui_err  krui_showPattern(int mode);
krui_err  krui_allocNewPatternSet(int *set_no);
krui_err  krui_newPattern(void);
krui_err  krui_setPatternNo(int pattern_no);
krui_err  krui_getPatternNo(void);
krui_err  krui_deletePattern(void);
krui_err  krui_modifyPattern(void);
void  krui_deleteAllPatterns(void);
krui_err  krui_shufflePatterns(bool on_or_off);
krui_err  krui_shuffleSubPatterns(bool on_or_off);
int  krui_getNoOfPatterns(void);
int  krui_getTotalNoOfSubPatterns(void);

krui_err  krui_loadNewPatterns(char *filename, int *number);
krui_err  krui_setCurrPatSet(int number);
krui_err  krui_saveNewPatterns(char *filename, int number);
krui_err  krui_deletePatSet(int number);
krui_err  krui_GetPatInfo(pattern_set_info *set_info, 
			  pattern_descriptor *pat_info);
krui_err  krui_DefShowSubPat(int *insize, int *outsize, 
			     int *inpos, int *outpos);
krui_err  krui_DefTrainSubPat(int *insize, int *outsize,
			      int *instep, int *outstep, int *max_n_pos);
krui_err  krui_AlignSubPat(int *inpos, int *outpos, int *no);
krui_err  krui_GetShapeOfSubPattern(int *insize, int *outsize, 
				    int *inpos, int *outpos, int n_pos);

krui_err  krui_saveNet(char *filename, char *netname);
krui_err  krui_loadNet(char *filename, char **netname);
krui_err  krui_saveResultParam(char *filename, bool create, 
			       int startpattern, int endpattern, 
			       bool includeinput, bool includeoutput,
			       float *Update_param_array, int NoOfUpdateParam);
krui_err  krui_allocateUnits(int no_of_units);
void  krui_deleteNet(void);
bool  krui_getFirstSymbolTableEntry(char **symbol_name, int *symbol_type);
bool  krui_getNextSymbolTableEntry(char **symbol_name, int *symbol_type);
bool  krui_symbolSearch(char *symbol, int symbol_type);
char  *krui_getVersion(void);
void  krui_getNetInfo(int *no_of_sites, int *no_of_links, int *no_of_STable_entries, int *no_of_FTable_entries);
void  krui_getMemoryManagerInfo(int *unit_bytes, int *site_bytes, int *link_bytes, int *NTable_bytes, int *STable_bytes, int *FTable_bytes);
void  krui_getUnitDefaults(FlintType *act, FlintType *bias, int *st, int *subnet_no, int *layer_no, char **act_func, char **out_func);
krui_err  krui_setUnitDefaults(FlintTypeParam act, FlintTypeParam bias, int st, int subnet_no, int layer_no, char *act_func, char *out_func);
void  krui_resetNet(void);
void  krui_setSeedNo(long int seed);
int  krui_getNoOfInputUnits(void);
int  krui_getNoOfOutputUnits(void);
int  krui_getNoOfTTypeUnits(int UnitTType);
int  krui_getNoOfSpecialInputUnits(void);
int  krui_getNoOfSpecialOutputUnits(void);



krui_err  krui_xyTransTable(int op, int *x, int *y, int z);
krui_err  krui_getUnitCenters(int unit_no, int center_no, struct PositionVector **unit_center);
krui_err  krui_setUnitCenters(int unit_no, int center_no, struct PositionVector *unit_center);



char  *krui_error(int error_code);

/* for statistics
 */
float krui_getVariance (void);
int krui_countLinks(void);

/* for the network-analyzer tool
*/
float krui_NA_Error(int currentPattern, int error_unit, int error, bool ave);

#ifdef MASPAR_KERNEL

krui_err  krui_MasPar( mode );
krui_err  krui_getMasParStatus(void);
krui_err  krui_MasParBenchmark( func_type, cycles, result );

#endif

krui_err  krui_setSpecialNetworkType(int net_type);
int  krui_getSpecialNetworkType(void);
void krui_kohonen_SetExtraParameter(int x);
void krui_spanning_tree(void);
void krui_cc_updatePosOfSpecialUnits(void);
krui_err krui_cc_deleteAllSpecialUnits(void);

int krui_getErrorCode(void);
krui_err krui_setErrorHandler(void(* error_Handler )(int));
void krui_execHandler(int error_code);


/*
#ifdef __MSDOS__

#ifndef  KR_TYPES
#undef  int
#endif

#endif
*/


/* end global definition section */

/* begin private definition section */

static char  *krui_topo_err_msg(void);

/*#################################################

GROUP: Local Var's

#################################################*/

static int  UICurrentNameSearchUnitNo = 0;
static struct Site  *UICurrentFtypeSite = NULL;
static struct FtypeUnitStruct  *UICurrentFtypeEntry = NULL;
static char  *UICurrentNameSearchUnitSymbolPtr = NULL;

static bool dotraining; /* If True => train Network */
static int noOfStoredErrors=0;
static double storedLearnErrors[NO_OF_STORED_ERRORS+1];
static int storedAtEpoch[NO_OF_STORED_ERRORS+1];

/*#################################################

GROUP: Macros

#################################################*/

/*  Check operation for compatibility with MasPar parallel 
    network implementation
*/
#ifdef MASPAR_KERNEL

#define  MASPAR_FF1_VALIDATE_OP \
if (specialNetworkType == NET_TYPE_FF1)\
  { KernelErrorCode = KRERR_MODE_FF1_INVALID_OP;\
    return( KRERR_MODE_FF1_INVALID_OP ); }\

#endif




/* end private definition section */


#endif 

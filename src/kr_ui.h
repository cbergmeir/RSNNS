/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_ui.h,v $
  SHORTNAME      : kr_ui.h
  SNNS VERSION   : 4.2
 
  PURPOSE        : SNNS-Kernel User Interface Function Prototypes
  NOTES          :
 
  AUTHOR         : Niels Mache
  DATE           : 27.02.90
 
  CHANGED BY     : Michael Vogt, Guenter Mamier, Christine Bagdi
  RCS VERSION    : $Revision: 2.23 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:16 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#ifndef _KR_UI_DEFINED_
#define  _KR_UI_DEFINED_



#include "krui_typ.h"	/*  Interface function prototypes  */

/*
#ifdef __MSDOS__

#ifndef  int
#define  int long
#endif

#endif
*/
/* pcc */
extern int       krui_countLinks(void);

extern void    (*krui_errorHandler)(int);
extern int       krui_error_code;

extern int       krui_getNoOfUnits(void);
extern int       krui_getFirstUnit(void);
extern int       krui_getNextUnit(void);
extern int       krui_getCurrentUnit(void);
extern krui_err  krui_setCurrentUnit(int unit_no);
extern char     *krui_getUnitName(int UnitNo);
extern krui_err  krui_setUnitName(int unit_no, char *unit_name);
extern int       krui_searchUnitName(char *unit_name);
extern int       krui_searchNextUnitName(void);
extern char     *krui_getUnitOutFuncName(int UnitNo);
extern krui_err  krui_setUnitOutFunc(int unit_no, char *unitOutFuncName);
extern char     *krui_getUnitActFuncName(int UnitNo);
extern krui_err  krui_setUnitActFunc(int unit_no, char *unitActFuncName);
extern char     *krui_getUnitFTypeName(int UnitNo);
extern FlintType krui_getUnitActivation(int UnitNo);
extern krui_err  krui_setUnitActivation(int UnitNo, 
					FlintTypeParam unit_activation);
extern FlintType krui_getUnitInitialActivation(int UnitNo);
extern void      krui_setUnitInitialActivation(int UnitNo, 
					   FlintTypeParam unit_i_activation);
extern FlintType krui_getUnitOutput(int UnitNo);
extern krui_err  krui_setUnitOutput(int unit_no, FlintTypeParam unit_output);
extern FlintType krui_getUnitBias(int UnitNo);
extern void      krui_setUnitBias(int UnitNo, FlintTypeParam unit_bias);
extern FlintType krui_getUnitValueA(int UnitNo);
extern void      krui_setUnitValueA(int UnitNo, FlintTypeParam unit_valueA);
extern int       krui_getUnitSubnetNo(int UnitNo);
extern void      krui_setUnitSubnetNo(int UnitNo, int subnet_no);
extern unsigned short	krui_getUnitLayerNo(int UnitNo);
extern void      krui_setUnitLayerNo(int UnitNo, int layer_no);
extern void      krui_getUnitPosition(int UnitNo, struct PosType *position);
extern void      krui_setUnitPosition(int UnitNo, struct PosType *position);
extern int       krui_getUnitNoAtPosition(struct PosType *position, 
					  int subnet_no);
extern int       krui_getUnitNoNearPosition(struct PosType *position, 
					    int subnet_no,
					    int range, int gridWidth);
extern int       krui_getUnitTType(int unit_no);
extern krui_err  krui_setUnitTType(int unit_no, int UnitTType);
extern krui_err  krui_freezeUnit(int unit_no);
extern krui_err  krui_unfreezeUnit(int unit_no);
extern bool      krui_isUnitFrozen(int unit_no);
extern int       krui_getUnitInputType(int unit_no);
extern int       krui_createDefaultUnit(void);
extern int       krui_createUnit(char *unit_name, char *out_func_name, 
				 char *act_func_name, FlintTypeParam i_act, 
				 FlintTypeParam bias);
extern krui_err  krui_deleteUnit(int unit_no);
extern krui_err  krui_deleteUnitList(int no_of_units, int unit_list[]);
extern int       krui_createFTypeUnit(char *Ftype_symbol);
extern krui_err  krui_setUnitFType(int unit_no, char *Ftype_symbol);
extern int       krui_copyUnit(int unit_no, int copy_mode);
extern bool      krui_setFirstFTypeEntry(void);
extern bool      krui_setNextFTypeEntry(void);
extern bool      krui_setFTypeEntry(char *Ftype_symbol);
extern char     *krui_getFTypeName(void);
extern krui_err  krui_setFTypeName(char *Ftype_symbol);
extern char     *krui_getFTypeActFuncName(void);
extern krui_err  krui_setFTypeActFunc(char *act_func_name);
extern krui_err  krui_setFTypeOutFunc(char *out_func_name);
extern bool      krui_setFirstFTypeSite(void);
extern bool      krui_setNextFTypeSite(void);
extern char     *krui_getFTypeSiteName(void);
extern krui_err  krui_setFTypeSiteName(char *FType_site_name);
extern krui_err  krui_createFTypeEntry(char *Ftype_symbol, char *act_func_name,
				       char *out_func_name, int no_of_sites, 
				       char **array_of_site_names);
extern krui_err  krui_deleteFTypeEntry(char *Ftype_symbol);
extern int       krui_getNoOfFunctions(void);
extern void      krui_getFuncInfo(int func_no, char **func_name,int *func_type);
extern bool      krui_isFunction(char *func_name, int func_type);
extern bool      krui_getFuncParamInfo(char *func_name, int func_type, 
				   int *no_of_input_params, 
				   int *no_of_output_params);
extern bool      krui_getFirstSiteTableEntry(char **site_name,char **site_func);
extern bool      krui_getNextSiteTableEntry(char **site_name, char **site_func);
extern char     *krui_getSiteTableFuncName(char *site_name);
extern krui_err  krui_createSiteTableEntry(char *site_name, char *site_func);
extern krui_err  krui_changeSiteTableEntry(char *old_site_name, 
					   char *new_site_name, 
					   char *new_site_func);
extern krui_err  krui_deleteSiteTableEntry(char *site_name);
extern bool      krui_setFirstSite(void);
extern bool      krui_setNextSite(void);
extern krui_err  krui_setSite(char *site_name);
extern FlintType krui_getSiteValue(void);
extern char     *krui_getSiteName(void);
extern krui_err  krui_setSiteName(char *site_name);
extern char     *krui_getSiteFuncName(void);
extern krui_err  krui_addSite(char *site_name);
extern bool      krui_deleteSite(void);
extern int       krui_getFirstPredUnit(FlintType *strength);
extern int       krui_getFirstPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c);
extern int       krui_getNextPredUnit(FlintType *strength);
extern int       krui_getNextPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c);
extern int       krui_getCurrentPredUnit(FlintType *strength);
extern int       krui_getFirstSuccUnit(int source_unit_no, FlintType *weight);
extern int       krui_getNextSuccUnit(FlintType *weight);
extern bool      krui_areConnected(int source_unit_no, int target_unit_no);
extern bool      krui_areConnectedWeight(int source_unit_no,int target_unit_no, 
					 FlintType *weight);
extern bool      krui_isConnected(int source_unit_no);
extern FlintType krui_getLinkWeight(void);
extern void      krui_setLinkWeight(FlintTypeParam weight);
extern krui_err  krui_createLink(int source_unit_no, FlintTypeParam weight);
extern struct Link*  krui_createLinkWithAdditionalParameters
      (int source_unit_no, FlintTypeParam weight,float val_a,float val_b,float val_c);
extern krui_err  krui_deleteLink(void);
extern krui_err  krui_deleteAllInputLinks(void);
extern krui_err  krui_deleteAllOutputLinks(void);

extern void      krui_jogWeights(FlintTypeParam minus, FlintTypeParam plus);
extern krui_err  krui_jogCorrWeights(FlintTypeParam minus, FlintTypeParam plus,
				     FlintTypeParam mincorr);
extern krui_err  krui_updateSingleUnit(int unit_no);
extern char     *krui_getUpdateFunc(void);
extern krui_err  krui_setUpdateFunc(char *update_func);
extern krui_err  krui_setRemapFunc(char *name, float *params);
extern krui_err  krui_updateNet(float *parameterInArray, int NoOfInParams);
extern char     *krui_getInitialisationFunc(void);
extern krui_err  krui_setInitialisationFunc(char *initialisation_func);
extern krui_err  krui_initializeNet(float *parameterInArray, int NoOfInParams);

extern char     *krui_getLearnFunc(void);
extern krui_err  krui_setLearnFunc(char *learning_func);
extern krui_err  krui_trainNetwork(NetLearnParameters *parameters);
extern krui_err  krui_getNetworkErrorArray(double **learnErrors,int **atEpoch,
					   int *noOfErrors);
extern krui_err  krui_stopTraining(void);
extern krui_err  krui_learnAllPatterns(float *parameterInArray,int NoOfInParams,
				       float **parameterOutArray, 
				       int *NoOfOutParams);
extern krui_err  krui_testAllPatterns(float *parameterInArray, int NoOfInParams,
				      float **parameterOutArray, 
				      int *NoOfOutParams);
extern krui_err  krui_learnSinglePattern(int pattern_no,float *parameterInArray,
					 int NoOfInParams, 
					 float **parameterOutArray, 
					 int *NoOfOutParams);
extern krui_err  krui_testSinglePattern(int pattern_no, float *parameterInArray,
				        int NoOfInParams, 
				        float **parameterOutArray, 
				        int *NoOfOutParams);
extern int       krui_checkPruning (void);
extern krui_err  krui_learnAllPatternsFF(float *parameterInArray, 
					 int NoOfInParams, 
					 float **parameterOutArray, 
					 int *NoOfOutParams);
extern krui_err  krui_learnSinglePatternFF(int pattern_no, 
					   float *parameterInArray, 
					   int NoOfInParams, 
					   float **parameterOutArray, 
					   int *NoOfOutParams);
extern krui_err  krui_setClassDistribution(unsigned int *classDist);
extern krui_err  krui_setClassInfo(char *name);
extern krui_err  krui_useClassDistribution(bool use_it);
extern char     *krui_getPrunFunc (void);
extern char     *krui_getFFLearnFunc (void);
extern krui_err  krui_setFFLearnFunc (char *FF_learning_func);
extern krui_err  krui_setPrunFunc (char *pruning_func);
extern krui_err  krui_showPattern(int mode);
extern krui_err  krui_allocNewPatternSet(int *set_no);
extern krui_err  krui_newPattern(void);
extern krui_err  krui_setPatternNo(int pattern_no);
extern krui_err  krui_getPatternNo(void);
extern krui_err  krui_deletePattern(void);
extern krui_err  krui_modifyPattern(void);
extern void      krui_deleteAllPatterns(void);
extern krui_err  krui_shufflePatterns(bool on_or_off);
extern krui_err  krui_shuffleSubPatterns(bool on_or_off);
extern int       krui_getNoOfPatterns(void);
extern int       krui_getTotalNoOfSubPatterns(void);
extern krui_err  krui_loadNewPatterns(char *filename, int *number);
extern krui_err  krui_setCurrPatSet(int number);
extern krui_err  krui_saveNewPatterns(char *filename, int number);
extern krui_err  krui_deletePatSet(int number);
extern krui_err  krui_GetPatInfo(pattern_set_info *set_info, 
				 pattern_descriptor *pat_info);
extern krui_err  krui_DefShowSubPat(int *insize, int *outsize, 
				    int *inpos, int *outpos);
extern krui_err  krui_DefTrainSubPat(int *insize, int *outsize,
				     int *instep, int *outstep, 
				     int *max_n_pos);
extern krui_err  krui_AlignSubPat(int *inpos, int *outpos, int *no);
extern krui_err  krui_GetShapeOfSubPattern(int *insize, int *outsize, 
					   int *inpos, int *outpos, int n_pos);
extern krui_err  krui_saveNet(char *filename, char *netname);
extern krui_err  krui_loadNet(char *filename, char **netname);
extern krui_err  krui_saveResultParam(char *filename, bool create, 
				      int startpattern, int endpattern, 
				      bool includeinput, bool includeoutput,
				      float *Update_param_array,
				      int NoOfUpdateParam);
extern krui_err  krui_allocateUnits(int no_of_units);
extern void      krui_deleteNet(void);
extern bool      krui_getFirstSymbolTableEntry(char **symbol_name, 
					       int *symbol_type);
extern bool      krui_getNextSymbolTableEntry(char **symbol_name, 
					      int *symbol_type);
extern bool      krui_symbolSearch(char *symbol, int symbol_type);
extern char     *krui_getVersion(void);
extern void      krui_getNetInfo(int *no_of_sites, int *no_of_links, 
				 int *no_of_STable_entries, 
				 int *no_of_FTable_entries);
extern void      krui_getMemoryManagerInfo(int *unit_bytes, int *site_bytes, 
					   int *link_bytes, int *NTable_bytes, 
					   int *STable_bytes,int *FTable_bytes);
extern void      krui_getUnitDefaults(FlintType *act, FlintType *bias, int *st, 
				      int *subnet_no, int *layer_no, 
				      char **act_func, char **out_func);
extern krui_err  krui_setUnitDefaults(FlintTypeParam act, FlintTypeParam bias, 
				      int st, int subnet_no, int layer_no, 
				      char *act_func, char *out_func);
extern void      krui_resetNet(void);
extern void      krui_setSeedNo(long int seed);
extern int       krui_getNoOfInputUnits(void);
extern int       krui_getNoOfOutputUnits(void);
extern int       krui_getNoOfTTypeUnits(int UnitTType);
extern int       krui_getNoOfSpecialInputUnits(void);
extern int       krui_getNoOfSpecialOutputUnits(void);
extern krui_err  krui_xyTransTable(int op, int *x, int *y, int z);
extern krui_err  krui_getUnitCenters(int unit_no, int center_no, 
				     struct PositionVector **unit_center);
extern krui_err  krui_setUnitCenters(int unit_no, int center_no, 
				     struct PositionVector *unit_center);
extern char     *krui_error(int error_code);
extern float     krui_NA_Error(int currentPattern, int error_unit, int error, 
			       bool ave);

extern float krui_getVariance (void);
extern int krui_countLinks(void);

#ifdef MASPAR_KERNEL

extern krui_err  krui_MasPar( mode );
extern krui_err  krui_getMasParStatus(void);
extern krui_err  krui_MasParBenchmark( func_type, cycles, result );

#endif

extern krui_err  krui_setSpecialNetworkType(int net_type);
extern int       krui_getSpecialNetworkType(void);
extern int       krui_initInversion(void);
extern void      krui_inv_forwardPass(struct UnitList *inputs);
extern double    krui_inv_backwardPass(float learn, float delta_max, 
				       int *err_units, float ratio,
				       struct UnitList *inputs,
				       struct UnitList *outputs);
extern void      krui_kohonen_SetExtraParameter(int x);
extern void      krui_spanning_tree(void);
extern void      krui_cc_updatePosOfSpecialUnits(void);
extern krui_err  krui_cc_deleteAllSpecialUnits(void);
extern int       krui_getErrorCode(void);
extern krui_err  krui_setErrorHandler(void(* error_Handler )(int));
extern void      krui_execHandler(int error_code);


/*
#ifdef __MSDOS__

#ifndef  KR_TYPES
#undef  int
#endif

#endif
*/





#endif 


/**
 * This file is derived from the original SNNS Kernel User Interface Protorype
 * file $SNNS_DIR/kernel/sources/kr_ui.h
 */

#ifndef SNNS_KR_UI_HEADER
#define SNNS_KR_UI_HEADER

#include "RSNNS_glob_typ.h"

extern "C" void          (*krui_errorHandler)(int);
extern "C" int             krui_error_code;

/**
 * Unit functions.
 */
extern "C" int             krui_getNoOfUnits(void);
extern "C" int             krui_getFirstUnit(void);
extern "C" int             krui_getNextUnit(void);
extern "C" int             krui_getCurrentUnit(void);
extern "C" krui_err        krui_setCurrentUnit(int unit_no);
extern "C" char           *krui_getUnitName(int UnitNo);
extern "C" krui_err        krui_setUnitName(int unit_no, char *unit_name);
extern "C" int             krui_searchUnitName(char *unit_name);
extern "C" int             krui_searchNextUnitName(void);
extern "C" char           *krui_getUnitOutFuncName(int UnitNo);
extern "C" krui_err        krui_setUnitOutFunc(int unit_no, char *unitOutFuncName);
extern "C" char           *krui_getUnitActFuncName(int UnitNo);
extern "C" krui_err        krui_setUnitActFunc(int unit_no, char *unitActFuncName);
extern "C" char           *krui_getUnitFTypeName(int UnitNo);
extern "C" FlintType       krui_getUnitActivation(int UnitNo);
extern "C" krui_err        krui_setUnitActivation(int UnitNo,
                                              FlintTypeParam unit_activation);
extern "C" FlintType       krui_getUnitInitialActivation(int UnitNo);
extern "C" void            krui_setUnitInitialActivation(int UnitNo,
                                                 FlintTypeParam unit_i_activation);
extern "C" FlintType       krui_getUnitOutput(int UnitNo);
extern "C" krui_err        krui_setUnitOutput(int unit_no, FlintTypeParam unit_output);
extern "C" FlintType       krui_getUnitBias(int UnitNo);
extern "C" void            krui_setUnitBias(int UnitNo, FlintTypeParam unit_bias);
extern "C" FlintType       krui_getUnitValueA(int UnitNo);
extern "C" void            krui_setUnitValueA(int UnitNo, FlintTypeParam unit_valueA);
extern "C" int             krui_getUnitSubnetNo(int UnitNo);
extern "C" void            krui_setUnitSubnetNo(int UnitNo, int subnet_no);
extern "C" unsigned short  krui_getUnitLayerNo(int UnitNo);
extern "C" void            krui_setUnitLayerNo(int UnitNo, int layer_no);
extern "C" void            krui_getUnitPosition(int UnitNo, struct PosType *position);
extern "C" void            krui_setUnitPosition(int UnitNo, struct PosType *position);
extern "C" int             krui_getUnitNoAtPosition(struct PosType *position,
                                                int subnet_no);
extern "C" int             krui_getUnitNoNearPosition(struct PosType *position,
                                                  int subnet_no,
                                                  int range, int gridWidth);
extern "C" int             krui_getUnitTType(int unit_no);
extern "C" krui_err        krui_setUnitTType(int unit_no, int UnitTType);
extern "C" krui_err        krui_freezeUnit(int unit_no);
extern "C" krui_err        krui_unfreezeUnit(int unit_no);
extern "C" bool            krui_isUnitFrozen(int unit_no);
extern "C" int             krui_getUnitInputType(int unit_no);
extern "C" int             krui_createDefaultUnit(void);
extern "C" int             krui_createUnit(char *unit_name, char *out_func_name,
                                 char *act_func_name, FlintTypeParam i_act,
                                 FlintTypeParam bias);
extern "C" krui_err        krui_deleteUnit(int unit_no);
extern "C" krui_err        krui_deleteUnitList(int no_of_units, int unit_list[]);
extern "C" int             krui_createFTypeUnit(char *Ftype_symbol);
extern "C" krui_err        krui_setUnitFType(int unit_no, char *Ftype_symbol);
extern "C" int             krui_copyUnit(int unit_no, int copy_mode);
extern "C" bool            krui_setFirstFTypeEntry(void);
extern "C" bool            krui_setNextFTypeEntry(void);
extern "C" bool            krui_setFTypeEntry(char *Ftype_symbol);
extern "C" char           *krui_getFTypeName(void);
extern "C" krui_err        krui_setFTypeName(char *Ftype_symbol);
extern "C" char           *krui_getFTypeActFuncName(void);
extern "C" krui_err        krui_setFTypeActFunc(char *act_func_name);
extern "C" krui_err        krui_setFTypeOutFunc(char *out_func_name);
extern "C" bool            krui_setFirstFTypeSite(void);
extern "C" bool            krui_setNextFTypeSite(void);
extern "C" char           *krui_getFTypeSiteName(void);
extern "C" krui_err        krui_setFTypeSiteName(char *FType_site_name);
extern "C" krui_err        krui_createFTypeEntry(char *Ftype_symbol, char *act_func_name,
                                             char *out_func_name, int no_of_sites,
                                             char **array_of_site_names);
extern "C" krui_err        krui_deleteFTypeEntry(char *Ftype_symbol);
extern "C" int             krui_getNoOfFunctions(void);
extern "C" void            krui_getFuncInfo(int func_no, char **func_name,int *func_type);
extern "C" bool            krui_isFunction(char *func_name, int func_type);
extern "C" bool            krui_getFuncParamInfo(char *func_name, int func_type,
                                         int *no_of_input_params,
                                         int *no_of_output_params);
extern "C" bool            krui_getFirstSiteTableEntry(char **site_name,char **site_func);
extern "C" bool            krui_getNextSiteTableEntry(char **site_name, char **site_func);
extern "C" char           *krui_getSiteTableFuncName(char *site_name);
extern "C" krui_err        krui_createSiteTableEntry(char *site_name, char *site_func);
extern "C" krui_err        krui_changeSiteTableEntry(char *old_site_name,
                                                 char *new_site_name,
                                                 char *new_site_func);
extern "C" krui_err        krui_deleteSiteTableEntry(char *site_name);
extern "C" bool            krui_setFirstSite(void);
extern "C" bool            krui_setNextSite(void);
extern "C" krui_err        krui_setSite(char *site_name);
extern "C" FlintType       krui_getSiteValue(void);
extern "C" char           *krui_getSiteName(void);
extern "C" krui_err        krui_setSiteName(char *site_name);
extern "C" char           *krui_getSiteFuncName(void);
extern "C" krui_err        krui_addSite(char *site_name);
extern "C" bool            krui_deleteSite(void);
extern "C" int             krui_getFirstPredUnit(FlintType *strength);
extern "C" int             krui_getFirstPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c);
extern "C" int             krui_getNextPredUnit(FlintType *strength);
extern "C" int             krui_getNextPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c);
extern "C" int             krui_getCurrentPredUnit(FlintType *strength);

/**
 * Link functions.
 */
extern "C" int             krui_getFirstSuccUnit(int source_unit_no, FlintType *weight);
extern "C" int             krui_getNextSuccUnit(FlintType *weight);
extern "C" bool            krui_areConnected(int source_unit_no, int target_unit_no);
extern "C" bool            krui_areConnectedWeight(int source_unit_no,int target_unit_no,
                                               FlintType *weight);
extern "C" bool            krui_isConnected(int source_unit_no);
extern "C" FlintType       krui_getLinkWeight(void);
extern "C" void            krui_setLinkWeight(FlintTypeParam weight);
extern "C" krui_err        krui_createLink(int source_unit_no, FlintTypeParam weight);
extern "C" struct Link*    krui_createLinkWithAdditionalParameters(
                              int source_unit_no, FlintTypeParam weight,
                              float val_a,float val_b,float val_c);
extern "C" krui_err        krui_deleteLink(void);
extern "C" krui_err        krui_deleteAllInputLinks(void);
extern "C" krui_err        krui_deleteAllOutputLinks(void);

extern "C" void            krui_jogWeights(FlintTypeParam minus, FlintTypeParam plus);
extern "C" krui_err        krui_jogCorrWeights(FlintTypeParam minus, FlintTypeParam plus,
                                           FlintTypeParam mincorr);
extern "C" krui_err        krui_updateSingleUnit(int unit_no);
extern "C" char           *krui_getUpdateFunc(void);
extern "C" krui_err        krui_setUpdateFunc(char *update_func);
extern "C" krui_err        krui_setRemapFunc(char *name, float *params);
extern "C" krui_err        krui_updateNet(float *parameterInArray, int NoOfInParams);
extern "C" char           *krui_getInitialisationFunc(void);
extern "C" krui_err        krui_setInitialisationFunc(char *initialisation_func);

/**
 * Network initialization functions.
 */
extern "C" krui_err        krui_initializeNet(float *parameterInArray, int NoOfInParams);

/**
 * Learning and pruning functions.
 */
extern "C" char           *krui_getLearnFunc(void);
extern "C" krui_err        krui_setLearnFunc(char *learning_func);
extern "C" krui_err        krui_trainNetwork(NetLearnParameters *parameters);
extern "C" krui_err        krui_getNetworkErrorArray(double **learnErrors,int **atEpoch,
                                                 int *noOfErrors);
extern "C" krui_err        krui_stopTraining(void);
extern "C" krui_err        krui_learnAllPatterns(float *parameterInArray,int NoOfInParams,
                                             float **parameterOutArray,
                                             int *NoOfOutParams);
extern "C" krui_err        krui_testAllPatterns(float *parameterInArray, int NoOfInParams,
                                            float **parameterOutArray,
                                            int *NoOfOutParams);
extern "C" krui_err        krui_learnSinglePattern(int pattern_no,float *parameterInArray,
                                               int NoOfInParams,
                                               float **parameterOutArray,
                                               int *NoOfOutParams);
extern "C" krui_err        krui_testSinglePattern(int pattern_no, float *parameterInArray,
                                              int NoOfInParams,
                                              float **parameterOutArray,
                                              int *NoOfOutParams);
extern "C" int             krui_checkPruning (void);
extern "C" krui_err        krui_learnAllPatternsFF(float *parameterInArray,
                                               int NoOfInParams,
                                               float **parameterOutArray,
                                               int *NoOfOutParams);
extern "C" krui_err        krui_learnSinglePatternFF(int pattern_no,
                                                 float *parameterInArray,
                                                 int NoOfInParams,
                                                 float **parameterOutArray,
                                                 int *NoOfOutParams);
extern "C" krui_err        krui_setClassDistribution(unsigned int *classDist);
extern "C" krui_err        krui_setClassInfo(char *name);
extern "C" krui_err        krui_useClassDistribution(bool use_it);
extern "C" char           *krui_getPrunFunc (void);
extern "C" char           *krui_getFFLearnFunc (void);
extern "C" krui_err        krui_setFFLearnFunc (char *FF_learning_func);
extern "C" krui_err        krui_setPrunFunc (char *pruning_func);
extern "C" krui_err        krui_showPattern(int mode);
extern "C" krui_err        krui_allocNewPatternSet(int *set_no);
//extern "C" krui_err        kr_np_ReallocatePatternSet(int pat_set, int new_number);
extern "C" krui_err        krui_newPattern(void);
extern "C" krui_err        krui_setPatternNo(int pattern_no);
extern "C" krui_err        krui_getPatternNo(void);
extern "C" krui_err        krui_deletePattern(void);
extern "C" krui_err        krui_modifyPattern(void);
extern "C" void            krui_deleteAllPatterns(void);
extern "C" krui_err        krui_shufflePatterns(bool on_or_off);
extern "C" krui_err        krui_shuffleSubPatterns(bool on_or_off);
extern "C" int             krui_getNoOfPatterns(void);
extern "C" int             krui_getTotalNoOfSubPatterns(void);
extern "C" krui_err        krui_loadNewPatterns(char *filename, int *number);
extern "C" krui_err        krui_setCurrPatSet(int number);
extern "C" krui_err        krui_saveNewPatterns(char *filename, int number);
extern "C" krui_err        krui_deletePatSet(int number);
extern "C" krui_err        krui_GetPatInfo(pattern_set_info *set_info,
                                       pattern_descriptor *pat_info);
extern "C" krui_err        krui_DefShowSubPat(int *insize, int *outsize,
                                          int *inpos, int *outpos);
extern "C" krui_err        krui_DefTrainSubPat(int *insize, int *outsize,
                                           int *instep, int *outstep,
                                           int *max_n_pos);
extern "C" krui_err        krui_AlignSubPat(int *inpos, int *outpos, int *no);
extern "C" krui_err        krui_GetShapeOfSubPattern(int *insize, int *outsize,
                                                 int *inpos, int *outpos, int n_pos);

/**
 * File I/O functions.
 */
extern "C" krui_err        krui_saveNet(char *filename, char *netname);
extern "C" krui_err        krui_loadNet(char *filename, char **netname);
extern "C" krui_err        krui_saveResultParam(char *filename, bool create,
                                            int startpattern, int endpattern,
                                            bool includeinput, bool includeoutput,
                                            float *Update_param_array,
                                            int NoOfUpdateParam);
extern "C" krui_err        krui_allocateUnits(int no_of_units);
extern "C" void            krui_deleteNet(void);
extern "C" bool            krui_getFirstSymbolTableEntry(char **symbol_name,
                                                     int *symbol_type);
extern "C" bool            krui_getNextSymbolTableEntry(char **symbol_name,
                                                    int *symbol_type);
extern "C" bool            krui_symbolSearch(char *symbol, int symbol_type);
extern "C" char           *krui_getVersion(void);
extern "C" void            krui_getNetInfo(int *no_of_sites, int *no_of_links,
                                       int *no_of_STable_entries,
                                       int *no_of_FTable_entries);
extern "C" void            krui_getMemoryManagerInfo(int *unit_bytes,
                              int *site_bytes, int *link_bytes,
                              int *NTable_bytes, int *STable_bytes,
                              int *FTable_bytes);
extern "C" void            krui_getUnitDefaults(FlintType *act, FlintType *bias, int *st,
                              int *subnet_no, int *layer_no, char **act_func,
                              char **out_func);
extern "C" krui_err        krui_setUnitDefaults(FlintTypeParam act,
                              FlintTypeParam bias, int st, int subnet_no,
                              int layer_no, char *act_func, char *out_func);

/**
 * Miscelaneous other interface finctions.
 */
extern "C" void            krui_resetNet(void);
extern "C" void            krui_setSeedNo(long int seed);
extern "C" int             krui_getNoOfInputUnits(void);
extern "C" int             krui_getNoOfOutputUnits(void);
extern "C" int             krui_getNoOfTTypeUnits(int UnitTType);
extern "C" int             krui_getNoOfSpecialInputUnits(void);
extern "C" int             krui_getNoOfSpecialOutputUnits(void);
extern "C" krui_err        krui_xyTransTable(int op, int *x, int *y, int z);
extern "C" krui_err        krui_getUnitCenters(int unit_no, int center_no,
                              struct PositionVector **unit_center);
extern "C" krui_err        krui_setUnitCenters(int unit_no, int center_no,
                              struct PositionVector *unit_center);
extern "C" char           *krui_error(int error_code);
extern "C" float           krui_NA_Error(int currentPattern, int error_unit,
                                         int error, bool ave);

extern "C" float           krui_getVariance (void);
extern "C" int             krui_countLinks(void);

#ifdef MASPAR_KERNEL

extern "C" krui_err        krui_MasPar( mode );
extern "C" krui_err        krui_getMasParStatus(void);
extern "C" krui_err        krui_MasParBenchmark( func_type, cycles, result );

#endif

extern "C" krui_err        krui_setSpecialNetworkType(int net_type);
extern "C" int             krui_getSpecialNetworkType(void);
extern "C" int             krui_initInversion(void);
extern "C" void            krui_inv_forwardPass(struct UnitList *inputs);
extern "C" double          krui_inv_backwardPass(float learn, float delta_max,
                                             int *err_units, float ratio,
                                             struct UnitList *inputs,
                                             struct UnitList *outputs);
extern "C" void            krui_kohonen_SetExtraParameter(int x);
extern "C" void            krui_spanning_tree(void);
extern "C" void            krui_cc_updatePosOfSpecialUnits(void);
extern "C" krui_err        krui_cc_deleteAllSpecialUnits(void);
extern "C" int             krui_getErrorCode(void);
extern "C" krui_err        krui_setErrorHandler(void(* error_Handler )(int));
extern "C" void            krui_execHandler(int error_code);



extern "C" krui_err bn_jordan_createNet  (int IUnits, int HUnits, int OUnits,
                                      int ICols , int HCols , int OCols ) ;
extern "C" krui_err bn_elman_createNet        (int layer[], int columns[], 
                                      int no_of_layers, bool out_context) ;

extern "C" float* kr_getSubPatData(int pat_no, int sub_no, int io_type, int *size);



#endif

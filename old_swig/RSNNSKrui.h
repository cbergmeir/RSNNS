#ifndef SNNSKRUI_HEADER
#define SNNSKRUI_HEADER

#include "RSNNS_kr_ui.h"

#include <Rcpp.h>

  RcppExport SEXP RSNNSKrui_getNoOfUnits(void);
  RcppExport SEXP RSNNSKrui_getFirstUnit(void);
  RcppExport SEXP RSNNSKrui_getNextUnit(void);
  RcppExport SEXP RSNNSKrui_getCurrentUnit(void);
  RcppExport SEXP RSNNSKrui_setCurrentUnit( SEXP unit_no);
  char           *getUnitName( SEXP UnitNo);
  RcppExport SEXP RSNNSKrui_setUnitName( SEXP unit_no, char *unit_name);
  int             searchUnitName(char *unit_name);
  int             searchNextUnitName(void);
  char           *getUnitOutFuncName( SEXP UnitNo);
  RcppExport SEXP RSNNSKrui_setUnitOutFunc( SEXP unit_no, char *unitOutFuncName);
  char           *getUnitActFuncName( SEXP UnitNo);
  RcppExport SEXP RSNNSKrui_setUnitActFunc( SEXP unit_no, char *unitActFuncName);
  char           *getUnitFTypeName( SEXP UnitNo);
  FlintType       getUnitActivation( SEXP UnitNo);
  RcppExport SEXP RSNNSKrui_setUnitActivation( SEXP UnitNo,
                                         FlintTypeParam unit_activation);
  FlintType       getUnitInitialActivation( SEXP UnitNo);
  void            setUnitInitialActivation( SEXP UnitNo,
                                            FlintTypeParam unit_i_activation);
  FlintType       getUnitOutput( SEXP UnitNo);
  RcppExport SEXP RSNNSKrui_setUnitOutput( SEXP unit_no, FlintTypeParam unit_output);
  FlintType       getUnitBias( SEXP UnitNo);
  void            setUnitBias( SEXP UnitNo, FlintTypeParam unit_bias);
  FlintType       getUnitValueA( SEXP UnitNo);
  void            setUnitValueA( SEXP UnitNo, FlintTypeParam unit_valueA);
  int             getUnitSubnetNo( SEXP UnitNo);
  void            setUnitSubnetNo( SEXP UnitNo, int subnet_no);
  unsigned short  getUnitLayerNo( SEXP UnitNo);
  void            setUnitLayerNo( SEXP UnitNo, int layer_no);
  void            getUnitPosition( SEXP UnitNo, struct PosType *position);
  void            setUnitPosition( SEXP UnitNo, struct PosType *position);
  int             getUnitNoAtPosition(struct PosType *position,
                                           int subnet_no);
  int             getUnitNoNearPosition(struct PosType *position,
                                             int subnet_no,
                                             int range, int gridWidth);
  int             getUnitTType( SEXP unit_no);
  RcppExport SEXP RSNNSKrui_setUnitTType( SEXP unit_no, int UnitTType);
  RcppExport SEXP RSNNSKrui_freezeUnit( SEXP unit_no);
  RcppExport SEXP RSNNSKrui_unfreezeUnit( SEXP unit_no);
  bool            isUnitFrozen( SEXP unit_no);
  int             getUnitInputType( SEXP unit_no);
  int             createDefaultUnit(void);
  int             createUnit(char *unit_name, char *out_func_name,
                            char *act_func_name, FlintTypeParam i_act,
                            FlintTypeParam bias);
  RcppExport SEXP RSNNSKrui_deleteUnit( SEXP unit_no);
  RcppExport SEXP RSNNSKrui_deleteUnitList( SEXP no_of_units, int unit_list[]);
  int             createFTypeUnit(char *Ftype_symbol);
  RcppExport SEXP RSNNSKrui_setUnitFType( SEXP unit_no, char *Ftype_symbol);
  int             copyUnit( SEXP unit_no, int copy_mode);
  bool            setFirstFTypeEntry(void);
  bool            setNextFTypeEntry(void);
  bool            setFTypeEntry(char *Ftype_symbol);
  char           *getFTypeName(void);
  RcppExport SEXP RSNNSKrui_setFTypeName(char *Ftype_symbol);
  char           *getFTypeActFuncName(void);
  RcppExport SEXP RSNNSKrui_setFTypeActFunc(char *act_func_name);
  RcppExport SEXP RSNNSKrui_setFTypeOutFunc(char *out_func_name);
  bool            setFirstFTypeSite(void);
  bool            setNextFTypeSite(void);
  char           *getFTypeSiteName(void);
  RcppExport SEXP RSNNSKrui_setFTypeSiteName(char *FType_site_name);
  RcppExport SEXP RSNNSKrui_createFTypeEntry(char *Ftype_symbol, char *act_func_name,
                                        char *out_func_name, int no_of_sites,
                                        char **array_of_site_names);
  RcppExport SEXP RSNNSKrui_deleteFTypeEntry(char *Ftype_symbol);
  int             getNoOfFunctions(void);
  void            getFuncInfo( SEXP func_no, char **func_name,int *func_type);
  bool            isFunction(char *func_name, int func_type);
  bool            getFuncParamInfo(char *func_name, int func_type,
                                    int *no_of_input_params,
                                    int *no_of_output_params);
  bool            getFirstSiteTableEntry(char **site_name,char **site_func);
  bool            getNextSiteTableEntry(char **site_name, char **site_func);
  char           *getSiteTableFuncName(char *site_name);
  RcppExport SEXP RSNNSKrui_createSiteTableEntry(char *site_name, char *site_func);
  RcppExport SEXP RSNNSKrui_changeSiteTableEntry(char *old_site_name,
                                            char *new_site_name,
                                            char *new_site_func);
  RcppExport SEXP RSNNSKrui_deleteSiteTableEntry(char *site_name);
  bool            setFirstSite(void);
  bool            setNextSite(void);
  RcppExport SEXP RSNNSKrui_setSite(char *site_name);
  FlintType       getSiteValue(void);
  char           *getSiteName(void);
  RcppExport SEXP RSNNSKrui_setSiteName(char *site_name);
  char           *getSiteFuncName(void);
  RcppExport SEXP RSNNSKrui_addSite(char *site_name);
  bool            deleteSite(void);
  int             getFirstPredUnit(FlintType *strength);
  int             getFirstPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c);
  int             getNextPredUnit(FlintType *strength);
  int             getNextPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c);
  int             getCurrentPredUnit(FlintType *strength);

  int             getFirstSuccUnit( SEXP source_unit_no, FlintType *weight);
  int             getNextSuccUnit(FlintType *weight);
  bool            areConnected( SEXP source_unit_no, int target_unit_no);
  bool            areConnectedWeight( SEXP source_unit_no,int target_unit_no,
                                          FlintType *weight);
  bool            isConnected( SEXP source_unit_no);
  FlintType       getLinkWeight(void);
  void            setLinkWeight(FlintTypeParam weight);
  RcppExport SEXP RSNNSKrui_createLink( SEXP source_unit_no, FlintTypeParam weight);
  struct Link*    createLinkWithAdditionalParameters(
                         int source_unit_no, FlintTypeParam weight,
                         float val_a,float val_b,float val_c);
  RcppExport SEXP RSNNSKrui_deleteLink(void);
  RcppExport SEXP RSNNSKrui_deleteAllInputLinks(void);
  RcppExport SEXP RSNNSKrui_deleteAllOutputLinks(void);

  void            jogWeights(FlintTypeParam minus, FlintTypeParam plus);
  RcppExport SEXP RSNNSKrui_jogCorrWeights(FlintTypeParam minus, FlintTypeParam plus,
                                      FlintTypeParam mincorr);
  RcppExport SEXP RSNNSKrui_updateSingleUnit( SEXP unit_no);
  char           *getUpdateFunc(void);
  RcppExport SEXP RSNNSKrui_setUpdateFunc(char *update_func);
  RcppExport SEXP RSNNSKrui_setRemapFunc(char *name, float *params);
  RcppExport SEXP RSNNSKrui_updateNet(float *parameterInArray, int NoOfInParams);
  char           *getInitialisationFunc(void);
  RcppExport SEXP RSNNSKrui_setInitialisationFunc(char *initialisation_func);
  RcppExport SEXP RSNNSKrui_initializeNet(float *parameterInArray, int NoOfInParams);
  RcppExport SEXP RSNNSKrui_getLearnFunc(void);
  RcppExport SEXP RSNNSKrui_setLearnFunc(SEXP learning_func);
  RcppExport SEXP RSNNSKrui_trainNetwork(NetLearnParameters *parameters);
  RcppExport SEXP RSNNSKrui_getNetworkErrorArray(double **learnErrors,int **atEpoch,
                                            int *noOfErrors);
  RcppExport SEXP RSNNSKrui_stopTraining(void);
  RcppExport SEXP RSNNSKrui_learnAllPatterns(float *parameterInArray,int NoOfInParams,
                                        float **parameterOutArray,
                                        int *NoOfOutParams);
  RcppExport SEXP RSNNSKrui_testAllPatterns(float *parameterInArray, int NoOfInParams,
                                       float **parameterOutArray,
                                       int *NoOfOutParams);
  RcppExport SEXP RSNNSKrui_learnSinglePattern( SEXP pattern_no,float *parameterInArray,
                                          int NoOfInParams,
                                          float **parameterOutArray,
                                          int *NoOfOutParams);
  RcppExport SEXP RSNNSKrui_testSinglePattern( SEXP pattern_no, float *parameterInArray,
                                         int NoOfInParams,
                                         float **parameterOutArray,
                                         int *NoOfOutParams);
  int             checkPruning (void);
  RcppExport SEXP RSNNSKrui_learnAllPatternsFF(float *parameterInArray,
                                          int NoOfInParams,
                                          float **parameterOutArray,
                                          int *NoOfOutParams);
  RcppExport SEXP RSNNSKrui_learnSinglePatternFF( SEXP pattern_no,
                                            float *parameterInArray,
                                            int NoOfInParams,
                                            float **parameterOutArray,
                                            int *NoOfOutParams);
  RcppExport SEXP RSNNSKrui_setClassDistribution(unsigned int *classDist);
  RcppExport SEXP RSNNSKrui_setClassInfo(char *name);
  RcppExport SEXP RSNNSKrui_useClassDistribution(bool use_it);
  char           *getPrunFunc (void);
  char           *getFFLearnFunc (void);
  RcppExport SEXP RSNNSKrui_setFFLearnFunc (char *FF_learning_func);
  RcppExport SEXP RSNNSKrui_setPrunFunc (char *pruning_func);
  RcppExport SEXP RSNNSKrui_showPattern( SEXP mode);
  RcppExport SEXP RSNNSKrui_allocNewPatternSet( int *set_no);
  RcppExport SEXP RSNNSKrui_newPattern(void);
  RcppExport SEXP RSNNSKrui_setPatternNo( SEXP pattern_no);
  RcppExport SEXP RSNNSKrui_getPatternNo(void);
  RcppExport SEXP RSNNSKrui_deletePattern(void);
  RcppExport SEXP RSNNSKrui_modifyPattern(void);
  void            deleteAllPatterns(void);
  RcppExport SEXP RSNNSKrui_shufflePatterns(bool on_or_off);
  RcppExport SEXP RSNNSKrui_shuffleSubPatterns(bool on_or_off);
  int             getNoOfPatterns(void);
  int             getTotalNoOfSubPatterns(void);
  RcppExport SEXP RSNNSKrui_loadNewPatterns(char *filename, int *number);
  RcppExport SEXP RSNNSKrui_setCurrPatSet( SEXP number);
  RcppExport SEXP RSNNSKrui_saveNewPatterns(char *filename, int number);
  RcppExport SEXP RSNNSKrui_deletePatSet( SEXP number);
  RcppExport SEXP RSNNSKrui_GetPatInfo(pattern_set_info *set_info,
                                  pattern_descriptor *pat_info);
  RcppExport SEXP RSNNSKrui_DefShowSubPat( int *insize, int *outsize,
                                     int *inpos, int *outpos);
  RcppExport SEXP RSNNSKrui_DefTrainSubPat( int *insize, int *outsize,
                                      int *instep, int *outstep,
                                      int *max_n_pos);
  RcppExport SEXP RSNNSKrui_AlignSubPat( int *inpos, int *outpos, int *no);
  RcppExport SEXP RSNNSKrui_GetShapeOfSubPattern( int *insize, int *outsize,
                                            int *inpos, int *outpos, int n_pos);
  RcppExport SEXP RSNNSKrui_saveNet(char *filename, char *netname);
  RcppExport SEXP RSNNSKrui_loadNet(char *filename, char **netname);
  RcppExport SEXP RSNNSKrui_saveResultParam(char *filename, bool create,
                                       int startpattern, int endpattern,
                                       bool includeinput, bool includeoutput,
                                       float *Update_param_array,
                                       int NoOfUpdateParam);
  RcppExport SEXP RSNNSKrui_allocateUnits( SEXP no_of_units);
  void            deleteNet(void);
  bool            getFirstSymbolTableEntry(char **symbol_name,
                                                int *symbol_type);
  bool            getNextSymbolTableEntry(char **symbol_name,
                                               int *symbol_type);
  bool            symbolSearch(char *symbol, int symbol_type);
  char           *getVersion(void);
  void            getNetInfo( int *no_of_sites, int *no_of_links,
                                  int *no_of_STable_entries,
                                  int *no_of_FTable_entries);
  void            getMemoryManagerInfo( int *unit_bytes,
                         int *site_bytes, int *link_bytes,
                         int *NTable_bytes, int *STable_bytes,
                         int *FTable_bytes);
  void            getUnitDefaults(FlintType *act, FlintType *bias, int *st,
                         int *subnet_no, int *layer_no, char **act_func,
                         char **out_func);
  RcppExport SEXP RSNNSKrui_setUnitDefaults(FlintTypeParam act,
                         FlintTypeParam bias, int st, int subnet_no,
                         int layer_no, char *act_func, char *out_func);
  void            resetNet(void);
  void            setSeedNo(long int seed);
  int             getNoOfInputUnits(void);
  int             getNoOfOutputUnits(void);
  int             getNoOfTTypeUnits( SEXP UnitTType);
  int             getNoOfSpecialInputUnits(void);
  int             getNoOfSpecialOutputUnits(void);
  RcppExport SEXP RSNNSKrui_xyTransTable( SEXP op, int *x, int *y, int z);
  RcppExport SEXP RSNNSKrui_getUnitCenters( SEXP unit_no, int center_no,
                         struct PositionVector **unit_center);
  RcppExport SEXP RSNNSKrui_setUnitCenters( SEXP unit_no, int center_no,
                         struct PositionVector *unit_center);
  //char           *error( SEXP error_code);
  float           NA_Error( SEXP currentPattern, int error_unit,
                                    int error, bool ave);

  float           getVariance (void);
  int             countLinks(void);

#ifdef MASPAR_KERNEL

  RcppExport SEXP RSNNSKrui_MasPar( mode );
  RcppExport SEXP RSNNSKrui_getMasParStatus(void);
  RcppExport SEXP RSNNSKrui_MasParBenchmark( func_type, cycles, result );

#endif

  RcppExport SEXP RSNNSKrui_setSpecialNetworkType( SEXP net_type);
  int             getSpecialNetworkType(void);
  int             initInversion(void);
  void            inv_forwardPass(struct UnitList *inputs);
  double          inv_backwardPass(float learn, float delta_max,
                                        int *err_units, float ratio,
                                        struct UnitList *inputs,
                                        struct UnitList *outputs);
  void            kohonen_SetExtraParameter( SEXP x);
  void            spanning_tree(void);
  void            cc_updatePosOfSpecialUnits(void);
  RcppExport SEXP RSNNSKrui_cc_deleteAllSpecialUnits(void);
  int             getErrorCode(void);
  RcppExport SEXP RSNNSKrui_setErrorHandler(void(* error_Handler )( int ));
  void            execHandler( SEXP error_code);

#endif

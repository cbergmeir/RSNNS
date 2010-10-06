#ifndef SNNSKRUI_HEADER
#define SNNSKRUI_HEADER

#include "RSNNS_kr_ui.h"

#include <Rcpp.h>

  RcppExport SEXP SnnsKrui_getNoOfUnits(void);
  RcppExport SEXP SnnsKrui_getFirstUnit(void);
  RcppExport SEXP SnnsKrui_getNextUnit(void);
  RcppExport SEXP SnnsKrui_getCurrentUnit(void);
  RcppExport SEXP SnnsKrui_setCurrentUnit( SEXP unit_no);
  RcppExport SEXP SnnsKrui_getUnitName( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_setUnitName( SEXP unit_no, SEXP unit_name);
  RcppExport SEXP SnnsKrui_searchUnitName(SEXP unit_name);
  RcppExport SEXP SnnsKrui_searchNextUnitName(void);
  RcppExport SEXP SnnsKrui_getUnitOutFuncName( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_setUnitOutFunc( SEXP unit_no, SEXP unitOutFuncName);
  RcppExport SEXP SnnsKrui_getUnitActFuncName( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_setUnitActFunc( SEXP unit_no, SEXP unitActFuncName);
  RcppExport SEXP SnnsKrui_getUnitFTypeName( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_getUnitActivation( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_setUnitActivation( SEXP UnitNo,
                                         SEXP unit_activation);
  RcppExport SEXP SnnsKrui_getUnitInitialActivation( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_setUnitInitialActivation( SEXP UnitNo,
                                            SEXP unit_i_activation);
  RcppExport SEXP SnnsKrui_getUnitOutput( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_setUnitOutput( SEXP unit_no, SEXP unit_output);
  RcppExport SEXP SnnsKrui_getUnitBias( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_setUnitBias( SEXP UnitNo, SEXP unit_bias);
  RcppExport SEXP SnnsKrui_getUnitValueA( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_setUnitValueA( SEXP UnitNo, SEXP unit_valueA);
  RcppExport SEXP SnnsKrui_getUnitSubnetNo( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_setUnitSubnetNo( SEXP UnitNo, SEXP subnet_no);
  RcppExport SEXP SnnsKrui_getUnitLayerNo( SEXP UnitNo);
  RcppExport SEXP SnnsKrui_setUnitLayerNo( SEXP UnitNo, SEXP layer_no);
  RcppExport SEXP SnnsKrui_getUnitPosition( SEXP UnitNo );

  RcppExport SEXP SnnsKrui_setUnitPosition( SEXP UnitNo, SEXP x, SEXP y, SEXP z);

  RcppExport SEXP SnnsKrui_getUnitNoAtPosition(SEXP x, SEXP y, SEXP z,
                                           SEXP subnet_no);
  RcppExport SEXP SnnsKrui_getUnitNoNearPosition(SEXP x, SEXP y, SEXP z,
                                             SEXP subnet_no,
                                             SEXP range, SEXP gridWidth);
  RcppExport SEXP SnnsKrui_getUnitTType( SEXP unit_no);
  RcppExport SEXP SnnsKrui_setUnitTType( SEXP unit_no, SEXP UnitTType);
  RcppExport SEXP SnnsKrui_freezeUnit( SEXP unit_no);
  RcppExport SEXP SnnsKrui_unfreezeUnit( SEXP unit_no);
  RcppExport SEXP SnnsKrui_isUnitFrozen( SEXP unit_no);
  RcppExport SEXP SnnsKrui_getUnitInputType( SEXP unit_no);
  RcppExport SEXP SnnsKrui_createDefaultUnit(void);
  RcppExport SEXP SnnsKrui_createUnit(SEXP unit_name, SEXP out_func_name,
                            SEXP act_func_name, SEXP i_act,
                            SEXP bias);
  RcppExport SEXP SnnsKrui_deleteUnit( SEXP unit_no);
  RcppExport SEXP SnnsKrui_deleteUnitList( SEXP no_of_units, int unit_list[]);
  RcppExport SEXP SnnsKrui_createFTypeUnit(SEXP Ftype_symbol);
  RcppExport SEXP SnnsKrui_setUnitFType( SEXP unit_no, SEXP Ftype_symbol);
  RcppExport SEXP SnnsKrui_copyUnit( SEXP unit_no, SEXP copy_mode);
  RcppExport SEXP SnnsKrui_setFirstFTypeEntry(void);
  RcppExport SEXP SnnsKrui_setNextFTypeEntry(void);
  RcppExport SEXP SnnsKrui_setFTypeEntry(SEXP Ftype_symbol);
  RcppExport SEXP SnnsKrui_getFTypeName(void);
  RcppExport SEXP SnnsKrui_setFTypeName(SEXP Ftype_symbol);
  RcppExport SEXP SnnsKrui_getFTypeActFuncName(void);
  RcppExport SEXP SnnsKrui_setFTypeActFunc(SEXP act_func_name);
  RcppExport SEXP SnnsKrui_setFTypeOutFunc(SEXP out_func_name);
  RcppExport SEXP SnnsKrui_setFirstFTypeSite(void);
  RcppExport SEXP SnnsKrui_setNextFTypeSite(void);
  RcppExport SEXP SnnsKrui_getFTypeSiteName(void);
  RcppExport SEXP SnnsKrui_setFTypeSiteName(SEXP FType_site_name);
  RcppExport SEXP SnnsKrui_createFTypeEntry(SEXP Ftype_symbol, SEXP act_func_name,
                                        SEXP out_func_name, SEXP no_of_sites,
                                        char **array_of_site_names);
  RcppExport SEXP SnnsKrui_deleteFTypeEntry(SEXP Ftype_symbol);
  RcppExport SEXP SnnsKrui_getNoOfFunctions(void);
  RcppExport SEXP SnnsKrui_getFuncInfo( SEXP func_no );
  RcppExport SEXP SnnsKrui_isFunction(SEXP func_name, SEXP func_type);
  RcppExport SEXP SnnsKrui_getFuncParamInfo(SEXP func_name, SEXP func_type);
  RcppExport SEXP SnnsKrui_getFirstSiteTableEntry();
  RcppExport SEXP SnnsKrui_getNextSiteTableEntry();
  RcppExport SEXP SnnsKrui_getSiteTableFuncName(SEXP site_name);
  RcppExport SEXP SnnsKrui_createSiteTableEntry(SEXP site_name, SEXP site_func);
  RcppExport SEXP SnnsKrui_changeSiteTableEntry(SEXP old_site_name,
                                            SEXP new_site_name,
                                            SEXP new_site_func);
  RcppExport SEXP SnnsKrui_deleteSiteTableEntry(SEXP site_name);
  RcppExport SEXP SnnsKrui_setFirstSite(void);
  RcppExport SEXP SnnsKrui_setNextSite(void);
  RcppExport SEXP SnnsKrui_setSite(SEXP site_name);
  RcppExport SEXP SnnsKrui_getSiteValue(void);
  RcppExport SEXP SnnsKrui_getSiteName(void);
  RcppExport SEXP SnnsKrui_setSiteName(SEXP site_name);
  RcppExport SEXP SnnsKrui_getSiteFuncName(void);
  RcppExport SEXP SnnsKrui_addSite(SEXP site_name);
  RcppExport SEXP SnnsKrui_deleteSite(void);
  RcppExport SEXP SnnsKrui_getFirstPredUnit(float *strength);
  RcppExport SEXP SnnsKrui_getFirstPredUnitAndData(float *strength,float *val_a,float *val_b, float *val_c);
  RcppExport SEXP SnnsKrui_getNextPredUnit(float *strength);
  RcppExport SEXP SnnsKrui_getNextPredUnitAndData(float *strength,float *val_a,float *val_b, float *val_c);
  RcppExport SEXP SnnsKrui_getCurrentPredUnit(float *strength);

  RcppExport SEXP SnnsKrui_getFirstSuccUnit( SEXP source_unit_no, float *weight);
  RcppExport SEXP SnnsKrui_getNextSuccUnit(float *weight);
  RcppExport SEXP SnnsKrui_areConnected( SEXP source_unit_no, SEXP target_unit_no);
  RcppExport SEXP SnnsKrui_areConnectedWeight( SEXP source_unit_no,SEXP target_unit_no,
                                          float *weight);
  RcppExport SEXP SnnsKrui_isConnected( SEXP source_unit_no);
  RcppExport SEXP SnnsKrui_getLinkWeight(void);
  RcppExport SEXP SnnsKrui_setLinkWeight(SEXP weight);
  RcppExport SEXP SnnsKrui_createLink( SEXP source_unit_no, SEXP weight);
  struct Link*    SnnsKrui_createLinkWithAdditionalParameters(
                         SEXP source_unit_no, SEXP weight,
                         SEXP val_a,SEXP val_b,SEXP val_c);
  RcppExport SEXP SnnsKrui_deleteLink(void);
  RcppExport SEXP SnnsKrui_deleteAllInputLinks(void);
  RcppExport SEXP SnnsKrui_deleteAllOutputLinks(void);

  RcppExport SEXP SnnsKrui_jogWeights(SEXP minus, SEXP plus);
  RcppExport SEXP SnnsKrui_jogCorrWeights(SEXP minus, SEXP plus,
                                      SEXP mincorr);
  RcppExport SEXP SnnsKrui_updateSingleUnit( SEXP unit_no);
  RcppExport SEXP SnnsKrui_getUpdateFunc(void);
  RcppExport SEXP SnnsKrui_setUpdateFunc(SEXP update_func);
  RcppExport SEXP SnnsKrui_setRemapFunc(SEXP name, float *params);
  RcppExport SEXP SnnsKrui_updateNet(SEXP parameterInArray);
  RcppExport SEXP SnnsKrui_getInitialisationFunc(void);
  RcppExport SEXP SnnsKrui_setInitialisationFunc(SEXP initialisation_func);
  RcppExport SEXP SnnsKrui_initializeNet( SEXP parameterInArray);
  RcppExport SEXP SnnsKrui_getLearnFunc(void);
  RcppExport SEXP SnnsKrui_setLearnFunc(SEXP learning_func);
  RcppExport SEXP SnnsKrui_trainNetwork(NetLearnParameters *parameters);
  RcppExport SEXP SnnsKrui_getNetworkErrorArray(double **learnErrors,int **atEpoch,
                                            int *noOfErrors);
  RcppExport SEXP SnnsKrui_stopTraining(void);
  RcppExport SEXP SnnsKrui_learnAllPatterns(SEXP parameterInArray);
  RcppExport SEXP SnnsKrui_testAllPatterns(SEXP parameterInArray);
  RcppExport SEXP SnnsKrui_learnSinglePattern( SEXP pattern_no, SEXP parameterInArray );
  RcppExport SEXP SnnsKrui_testSinglePattern( SEXP pattern_no, SEXP parameterInArray );
  RcppExport SEXP SnnsKrui_checkPruning (void);
  RcppExport SEXP SnnsKrui_learnAllPatternsFF(SEXP parameterInArray);
  RcppExport SEXP SnnsKrui_learnSinglePatternFF( SEXP pattern_no, SEXP parameterInArray );
  RcppExport SEXP SnnsKrui_setClassDistribution(unsigned int *classDist);
  RcppExport SEXP SnnsKrui_setClassInfo(SEXP name);
  RcppExport SEXP SnnsKrui_useClassDistribution(SEXP use_it);
  RcppExport SEXP SnnsKrui_getPrunFunc (void);
  RcppExport SEXP SnnsKrui_getFFLearnFunc (void);
  RcppExport SEXP SnnsKrui_setFFLearnFunc (SEXP FF_learning_func);
  RcppExport SEXP SnnsKrui_setPrunFunc (SEXP pruning_func);
  RcppExport SEXP SnnsKrui_showPattern( SEXP mode);
  RcppExport SEXP SnnsKrui_allocNewPatternSet();
  RcppExport SEXP SnnsKrui_newPattern(void);
  RcppExport SEXP SnnsKrui_setPatternNo( SEXP pattern_no);
  RcppExport SEXP SnnsKrui_getPatternNo(void);
  RcppExport SEXP SnnsKrui_deletePattern(void);
  RcppExport SEXP SnnsKrui_modifyPattern(void);
  RcppExport SEXP SnnsKrui_deleteAllPatterns(void);
  RcppExport SEXP SnnsKrui_shufflePatterns(SEXP on_or_off);
  RcppExport SEXP SnnsKrui_shuffleSubPatterns(SEXP on_or_off);
  RcppExport SEXP SnnsKrui_getNoOfPatterns(void);
  RcppExport SEXP SnnsKrui_getTotalNoOfSubPatterns(void);
  RcppExport SEXP SnnsKrui_loadNewPatterns(SEXP filename);
  RcppExport SEXP SnnsKrui_setCurrPatSet( SEXP number);
  RcppExport SEXP SnnsKrui_saveNewPatterns(SEXP filename, SEXP number);
  RcppExport SEXP SnnsKrui_deletePatSet( SEXP number);
  /*RcppExport SEXP SnnsKrui_GetPatInfo(pattern_set_info *set_info,
                                  pattern_descriptor *pat_info);*/

  RcppExport SEXP SnnsKrui_GetPatInfo(void);

  RcppExport SEXP SnnsKrui_DefShowSubPat();
  RcppExport SEXP SnnsKrui_DefTrainSubPat();
  RcppExport SEXP SnnsKrui_AlignSubPat( int *inpos, int *outpos, int *no);
  RcppExport SEXP SnnsKrui_GetShapeOfSubPattern( int *insize, int *outsize,
                                            int *inpos, int *outpos, SEXP n_pos);
  RcppExport SEXP SnnsKrui_saveNet(SEXP filename, SEXP netname);
  RcppExport SEXP SnnsKrui_loadNet(SEXP filename);
  RcppExport SEXP SnnsKrui_saveResultParam(SEXP filename, SEXP create,
                                       SEXP startpattern, SEXP endpattern,
                                       SEXP includeinput, SEXP includeoutput,
                                       float *Update_param_array,
                                       SEXP NoOfUpdateParam);
  RcppExport SEXP SnnsKrui_allocateUnits( SEXP no_of_units);
  RcppExport SEXP SnnsKrui_deleteNet(void);
  RcppExport SEXP SnnsKrui_getFirstSymbolTableEntry();
  RcppExport SEXP SnnsKrui_getNextSymbolTableEntry();
  RcppExport SEXP SnnsKrui_symbolSearch(SEXP symbol, SEXP symbol_type);
  RcppExport SEXP SnnsKrui_getVersion(void);
  RcppExport SEXP SnnsKrui_getNetInfo();
  RcppExport SEXP SnnsKrui_getMemoryManagerInfo();
  RcppExport SEXP SnnsKrui_getUnitDefaults();
  RcppExport SEXP SnnsKrui_setUnitDefaults(SEXP act,
                         SEXP bias, SEXP st, SEXP subnet_no,
                         SEXP layer_no, SEXP act_func, SEXP out_func);
  RcppExport SEXP SnnsKrui_resetNet(void);
  RcppExport SEXP SnnsKrui_setSeedNo(SEXP seed);
  RcppExport SEXP SnnsKrui_getNoOfInputUnits(void);
  RcppExport SEXP SnnsKrui_getNoOfOutputUnits(void);
  RcppExport SEXP SnnsKrui_getNoOfTTypeUnits( SEXP UnitTType);
  RcppExport SEXP SnnsKrui_getNoOfSpecialInputUnits(void);
  RcppExport SEXP SnnsKrui_getNoOfSpecialOutputUnits(void);
  RcppExport SEXP SnnsKrui_xyTransTable( SEXP op, int *x, int *y, SEXP z);
  RcppExport SEXP SnnsKrui_getUnitCenters( SEXP unit_no, SEXP center_no,
                         struct PositionVector **unit_center);
  RcppExport SEXP SnnsKrui_setUnitCenters( SEXP unit_no, SEXP center_no,
                         struct PositionVector *unit_center);
  //RcppExport SEXP SnnsKrui_error( SEXP error_code);
  RcppExport SEXP SnnsKrui_NA_Error( SEXP currentPattern, SEXP error_unit,
                                    SEXP error, SEXP ave);

  RcppExport SEXP SnnsKrui_getVariance (void);
  RcppExport SEXP SnnsKrui_countLinks(void);

#ifdef MASPAR_KERNEL

  RcppExport SEXP SnnsKrui_MasPar( mode );
  RcppExport SEXP SnnsKrui_getMasParStatus(void);
  RcppExport SEXP SnnsKrui_MasParBenchmark( func_type, cycles, result );

#endif

  RcppExport SEXP SnnsKrui_setSpecialNetworkType( SEXP net_type);
  RcppExport SEXP SnnsKrui_getSpecialNetworkType(void);
  RcppExport SEXP SnnsKrui_initInversion(void);
  RcppExport SEXP SnnsKrui_inv_forwardPass(struct UnitList *inputs);
  RcppExport SEXP SnnsKrui_inv_backwardPass(SEXP learn, SEXP delta_max,
                                        int *err_units, SEXP ratio,
                                        struct UnitList *inputs,
                                        struct UnitList *outputs);
  RcppExport SEXP SnnsKrui_kohonen_SetExtraParameter( SEXP x);
  RcppExport SEXP SnnsKrui_spanning_tree(void);
  RcppExport SEXP SnnsKrui_cc_updatePosOfSpecialUnits(void);
  RcppExport SEXP SnnsKrui_cc_deleteAllSpecialUnits(void);
  RcppExport SEXP SnnsKrui_getErrorCode(void);
  RcppExport SEXP SnnsKrui_setErrorHandler(void(* error_Handler )( int ));
  RcppExport SEXP SnnsKrui_execHandler( SEXP error_code);


//----------------------------------------------------
// Bignet
//
//----------------------------------------------------
RcppExport SEXP SnnsBn_jordan_createNet( SEXP IUnits, SEXP HUnits, SEXP OUnits, SEXP ICols, SEXP HCols, SEXP OCols);
RcppExport SEXP SnnsBn_elman_createNet( SEXP layer, SEXP columns, SEXP out_context);

RcppExport SEXP SnnsKr_getSubPatData(SEXP pat_no, SEXP sub_no, SEXP io_type);

#endif

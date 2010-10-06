#include "RSNNSKrui.h"

#include <R.h>
#include <R_ext/Print.h>



//using namespace std;

RcppExport SEXP RSNNSKrui_getNoOfUnits(void) {
  int ret = krui_getNoOfUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_getFirstUnit(void) {
  int ret = krui_getFirstUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_getNextUnit(void) {
  int ret = krui_getNextUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_getCurrentUnit(void) {
  int ret = krui_getCurrentUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_setCurrentUnit( SEXP unit_no) {

  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_setCurrentUnit(p1);
  return Rcpp::wrap(err);
}

char *getUnitName( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  return krui_getUnitName(p1);
}

RcppExport SEXP RSNNSKrui_setUnitName( SEXP unit_no, char *unit_name) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_setUnitName(p1, unit_name);
  return Rcpp::wrap(err);
}

RcppExport SEXP searchUnitName(char *unit_name) {
  int ret = krui_searchUnitName(unit_name);
  return Rcpp::wrap(ret);
}

RcppExport SEXP searchNextUnitName(void) {
  int ret = krui_searchNextUnitName();
  return Rcpp::wrap(ret);
}

char *getUnitOutFuncName( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  return krui_getUnitOutFuncName(p1);
}

RcppExport SEXP RSNNSKrui_setUnitOutFunc( SEXP unit_no, char *unitOutFuncName) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_setUnitOutFunc(p1, unitOutFuncName);
  return Rcpp::wrap(err);
}

char *getUnitActFuncName( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  return krui_getUnitActFuncName(p1);
}

RcppExport SEXP RSNNSKrui_setUnitActFunc( SEXP unit_no, char *unitActFuncName) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_setUnitActFunc(p1, unitActFuncName);
  return Rcpp::wrap(err);
}

char *getUnitFTypeName( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  return krui_getUnitFTypeName(p1);
}

FlintType getUnitActivation( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  return krui_getUnitActivation(p1);
}

RcppExport SEXP RSNNSKrui_setUnitActivation( SEXP UnitNo,
                                     FlintTypeParam unit_activation) {
  int p1 = Rcpp::as<int>(UnitNo);
  int err = krui_setUnitActivation(p1, unit_activation);
  return Rcpp::wrap(err);
}

FlintType getUnitInitialActivation( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  return krui_getUnitInitialActivation(p1);
}

void setUnitInitialActivation( SEXP UnitNo,
                                               FlintTypeParam unit_i_activation) {
  int p1 = Rcpp::as<int>(UnitNo);
  krui_setUnitInitialActivation(p1, unit_i_activation);
}

FlintType getUnitOutput( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  return krui_getUnitOutput(p1);
}

RcppExport SEXP RSNNSKrui_setUnitOutput( SEXP unit_no, FlintTypeParam unit_output) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_setUnitOutput(p1, unit_output);
  return Rcpp::wrap(err);
}

FlintType getUnitBias( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  return krui_getUnitBias(p1);
}

void setUnitBias( SEXP UnitNo, FlintTypeParam unit_bias) {
  int p1 = Rcpp::as<int>(UnitNo);
  krui_setUnitBias(p1, unit_bias);
}

FlintType getUnitValueA( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  return krui_getUnitValueA(p1);
}

void setUnitValueA( SEXP UnitNo, FlintTypeParam unit_valueA) {
  int p1 = Rcpp::as<int>(UnitNo);
  krui_setUnitValueA(p1, unit_valueA);
}

RcppExport SEXP getUnitSubnetNo( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  int ret = krui_getUnitSubnetNo(p1);
  return Rcpp::wrap(ret);
}

void setUnitSubnetNo( SEXP UnitNo, SEXP subnet_no) {
  int p1 = Rcpp::as<int>(UnitNo);
  int p2 = Rcpp::as<int>(subnet_no);
  krui_setUnitSubnetNo(p1, p2);
}

unsigned short getUnitLayerNo( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  return krui_getUnitLayerNo(p1);
}

void setUnitLayerNo( SEXP UnitNo, SEXP layer_no) {
  int p1 = Rcpp::as<int>(UnitNo);
  int p2 = Rcpp::as<int>(layer_no);
  krui_setUnitLayerNo(p1, p2);
}

void getUnitPosition( SEXP UnitNo, struct PosType *position) {
  int p1 = Rcpp::as<int>(UnitNo);
  krui_getUnitPosition(p1, position);
}

void setUnitPosition( SEXP UnitNo, struct PosType *position) {
  int p1 = Rcpp::as<int>(UnitNo);
  krui_setUnitPosition(p1, position);
}

RcppExport SEXP getUnitNoAtPosition(struct PosType *position,
                                              SEXP subnet_no) {

  int p2 = Rcpp::as<int>(subnet_no);

  int ret = krui_getUnitNoAtPosition(position, p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP getUnitNoNearPosition(struct PosType *position,
                                                SEXP subnet_no,
                                                SEXP range, SEXP gridWidth) {

  int p2 = Rcpp::as<int>(subnet_no);
  int p3 = Rcpp::as<int>(range);
  int p4 = Rcpp::as<int>(gridWidth);

  int ret = krui_getUnitNoNearPosition(position, p2, p3, p4);
  return Rcpp::wrap(ret);
}

RcppExport SEXP getUnitTType( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int ret = krui_getUnitTType(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_setUnitTType( SEXP unit_no, SEXP UnitTType) {

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(UnitTType);

  int err = krui_setUnitTType(p1, p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_freezeUnit( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_freezeUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_unfreezeUnit( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_unfreezeUnit(p1);
  return Rcpp::wrap(err);
}

bool isUnitFrozen( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  return krui_isUnitFrozen(p1);
}

RcppExport SEXP getUnitInputType( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int ret = krui_getUnitInputType(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP createDefaultUnit(void) {
  int ret = krui_createDefaultUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP createUnit(char *unit_name, char *out_func_name,
                               char *act_func_name, FlintTypeParam i_act,
                               FlintTypeParam bias) {
  int ret = krui_createUnit(unit_name, out_func_name, act_func_name, i_act, bias);
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_deleteUnit( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_deleteUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_deleteUnitList( SEXP no_of_units, int unit_list[]) {

  int p1 = Rcpp::as<int>(no_of_units);

  int err = krui_deleteUnitList(p1, unit_list);
  return Rcpp::wrap(err);
}

RcppExport SEXP createFTypeUnit(char *Ftype_symbol) {
  int ret = krui_createFTypeUnit(Ftype_symbol);
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_setUnitFType( SEXP unit_no, char *Ftype_symbol) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_setUnitFType(p1, Ftype_symbol);
  return Rcpp::wrap(err);
}

RcppExport SEXP copyUnit( SEXP unit_no, SEXP copy_mode) {

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(copy_mode);

  int ret = krui_copyUnit(p1, p2);
  return Rcpp::wrap(ret);
}

bool setFirstFTypeEntry(void) {
  return krui_setFirstFTypeEntry();
}

bool setNextFTypeEntry(void) {
  return krui_setNextFTypeEntry();
}

bool setFTypeEntry(char *Ftype_symbol) {
  return krui_setFTypeEntry(Ftype_symbol);
}

char *getFTypeName(void) {
  return krui_getFTypeName();
}

RcppExport SEXP RSNNSKrui_setFTypeName(char *Ftype_symbol) {
  int err = krui_setFTypeName(Ftype_symbol);
  return Rcpp::wrap(err);
}

char *getFTypeActFuncName(void) {
  return krui_getFTypeActFuncName();
}

RcppExport SEXP RSNNSKrui_setFTypeActFunc(char *act_func_name) {
  int err = krui_setFTypeActFunc(act_func_name);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_setFTypeOutFunc(char *out_func_name) {
  int err = krui_setFTypeOutFunc(out_func_name);
  return Rcpp::wrap(err);
}

bool setFirstFTypeSite(void) {
  return krui_setFirstFTypeSite();
}

bool setNextFTypeSite(void) {
  return krui_setNextFTypeSite();
}

char *getFTypeSiteName(void) {
  return krui_getFTypeSiteName();
}

RcppExport SEXP RSNNSKrui_setFTypeSiteName(char *FType_site_name) {
  int err = krui_setFTypeSiteName(FType_site_name);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_createFTypeEntry(char *Ftype_symbol, char *act_func_name,
                                           char *out_func_name, SEXP no_of_sites,
                                           char **array_of_site_names) {

  int p4 = Rcpp::as<int>(no_of_sites);

  int err = krui_createFTypeEntry(Ftype_symbol, act_func_name,
                                           out_func_name, p4,
                                           array_of_site_names);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_deleteFTypeEntry(char *Ftype_symbol) {
  int err = krui_deleteFTypeEntry(Ftype_symbol);
  return Rcpp::wrap(err);
}

RcppExport SEXP getNoOfFunctions(void) {
  int ret = krui_getNoOfFunctions();
  return Rcpp::wrap(ret);
}

void getFuncInfo( SEXP func_no, char **func_name,int *func_type) {
  int p1 = Rcpp::as<int>(func_no);
  krui_getFuncInfo(p1, func_name, func_type);
}

bool isFunction(char *func_name, SEXP func_type) {

  int p2 = Rcpp::as<int>(func_type);

  return krui_isFunction(func_name, p2);
}

bool getFuncParamInfo(char *func_name, SEXP func_type,
                                       int *no_of_input_params,
                                       int *no_of_output_params) {

  int p2 = Rcpp::as<int>(func_type);

  return krui_getFuncParamInfo(func_name, p2,
                                       no_of_input_params,
                                       no_of_output_params);
}

bool getFirstSiteTableEntry(char **site_name,char **site_func) {
  return krui_getFirstSiteTableEntry(site_name, site_func);
}

bool getNextSiteTableEntry(char **site_name, char **site_func) {
  return krui_getNextSiteTableEntry(site_name, site_func);
}

char *getSiteTableFuncName(char *site_name) {
  return krui_getSiteTableFuncName(site_name);
}

RcppExport SEXP RSNNSKrui_createSiteTableEntry(char *site_name, char *site_func) {
  int err = krui_createSiteTableEntry(site_name, site_func);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_changeSiteTableEntry(char *old_site_name,
                                               char *new_site_name,
                                               char *new_site_func) {
  int err = krui_changeSiteTableEntry(old_site_name,
                                               new_site_name,
                                               new_site_func);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_deleteSiteTableEntry(char *site_name) {
  int err = krui_deleteSiteTableEntry(site_name);
  return Rcpp::wrap(err);
}

bool setFirstSite(void) {
  return krui_setFirstSite();
}

bool setNextSite(void) {
  return krui_setNextSite();
}

RcppExport SEXP RSNNSKrui_setSite(char *site_name) {
  int err = krui_setSite(site_name);
  return Rcpp::wrap(err);
}

FlintType getSiteValue(void) {
  return krui_getSiteValue();
}

char *getSiteName(void) {
  return krui_getSiteName();
}

RcppExport SEXP RSNNSKrui_setSiteName(char *site_name) {
  int err = krui_setSiteName(site_name);
  return Rcpp::wrap(err);
}

char *getSiteFuncName(void) {
  return krui_getSiteFuncName();
}

RcppExport SEXP RSNNSKrui_addSite(char *site_name) {
  int err = krui_addSite(site_name);
  return Rcpp::wrap(err);
}

bool deleteSite(void) {
  return krui_deleteSite();
}

RcppExport SEXP getFirstPredUnit(FlintType *strength) {
  int ret = krui_getFirstPredUnit(strength);
  return Rcpp::wrap(ret);
}

RcppExport SEXP getFirstPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c) {
  int ret = krui_getFirstPredUnitAndData(strength, val_a, val_b, val_c);
  return Rcpp::wrap(ret);
}

RcppExport SEXP getNextPredUnit(FlintType *strength) {
  int ret = krui_getNextPredUnit(strength);
  return Rcpp::wrap(ret);
}

RcppExport SEXP getNextPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c) {
  int ret = krui_getNextPredUnitAndData(strength, val_a, val_b, val_c);
  return Rcpp::wrap(ret);
}

RcppExport SEXP getCurrentPredUnit(FlintType *strength) {
  int ret = krui_getCurrentPredUnit(strength);
  return Rcpp::wrap(ret);
}


RcppExport SEXP getFirstSuccUnit( SEXP source_unit_no, FlintType *weight) {
  int p1 = Rcpp::as<int>(source_unit_no);
  int ret = krui_getFirstSuccUnit(p1, weight);
  return Rcpp::wrap(ret);
}

RcppExport SEXP getNextSuccUnit(FlintType *weight) {
  int ret = krui_getNextSuccUnit(weight);
  return Rcpp::wrap(ret);
}

bool areConnected( SEXP source_unit_no, SEXP target_unit_no) {

  int p1 = Rcpp::as<int>(source_unit_no);
  int p2 = Rcpp::as<int>(target_unit_no);

  return krui_areConnected(p1, p2);
}

bool areConnectedWeight( SEXP source_unit_no, SEXP target_unit_no,
                                             FlintType *weight) {

  int p1 = Rcpp::as<int>(source_unit_no);
  int p2 = Rcpp::as<int>(target_unit_no);

  return krui_areConnectedWeight(p1, p2,
                                             weight);
}

bool isConnected( SEXP source_unit_no) {
  int p1 = Rcpp::as<int>(source_unit_no);
  return krui_isConnected(p1);
}

FlintType getLinkWeight(void) {
  return krui_getLinkWeight();
}

void setLinkWeight(FlintTypeParam weight) {
  krui_setLinkWeight(weight);
}

RcppExport SEXP RSNNSKrui_createLink( SEXP source_unit_no, FlintTypeParam weight) {
  int p1 = Rcpp::as<int>(source_unit_no);
  int err = krui_createLink(p1, weight);
  return Rcpp::wrap(err);
}

struct Link* createLinkWithAdditionalParameters(
                            int source_unit_no, FlintTypeParam weight,
                            float val_a,float val_b,float val_c) {
  return krui_createLinkWithAdditionalParameters(
                            source_unit_no, weight,
                            val_a, val_b, val_c);
}

RcppExport SEXP RSNNSKrui_deleteLink(void) {
  int err = krui_deleteLink();
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_deleteAllInputLinks(void) {
  int err = krui_deleteAllInputLinks();
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_deleteAllOutputLinks(void) {
  int err = krui_deleteAllOutputLinks();
  return Rcpp::wrap(err);
}


void jogWeights(FlintTypeParam minus, FlintTypeParam plus) {
  krui_jogWeights(minus, plus);
}

RcppExport SEXP RSNNSKrui_jogCorrWeights(FlintTypeParam minus, FlintTypeParam plus,
                                         FlintTypeParam mincorr) {
  int err = krui_jogCorrWeights(minus, plus,
                                         mincorr);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_updateSingleUnit( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_updateSingleUnit(p1);
  return Rcpp::wrap(err);
}

char *getUpdateFunc(void) {
  return krui_getUpdateFunc();
}

RcppExport SEXP RSNNSKrui_setUpdateFunc(char *update_func) {
  int err = krui_setUpdateFunc(update_func);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_setRemapFunc(char *name, float *params) {
  int err = krui_setRemapFunc(name, params);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_updateNet(float *parameterInArray, SEXP NoOfInParams) {

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = krui_updateNet(parameterInArray, p2);
  return Rcpp::wrap(err);
}

char *getInitialisationFunc(void) {
  return krui_getInitialisationFunc();
}

RcppExport SEXP RSNNSKrui_setInitialisationFunc(char *initialisation_func) {
  int err = krui_setInitialisationFunc(initialisation_func);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_initializeNet(float *parameterInArray, SEXP NoOfInParams) {

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = krui_initializeNet(parameterInArray, p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_getLearnFunc(void) {
  const char* s = krui_getLearnFunc();
  return Rcpp::wrap(s);
}

RcppExport SEXP RSNNSKrui_setLearnFunc(SEXP learning_func) {

  std::string lf = Rcpp::as<std::string>( learning_func );

  int err = krui_setLearnFunc(const_cast<char*>(lf.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_trainNetwork(NetLearnParameters *parameters) {
  int err = krui_trainNetwork(parameters);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_getNetworkErrorArray(double **learnErrors,int **atEpoch,
                                               int *noOfErrors) {
  int err = krui_getNetworkErrorArray(learnErrors, atEpoch,
                                               noOfErrors);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_stopTraining(void) {
  int err = krui_stopTraining();
  return Rcpp::wrap(err);
}


RcppExport SEXP RSNNSKrui_learnAllPatterns(float *parameterInArray,SEXP NoOfInParams,
                                           float **parameterOutArray,
                                           int *NoOfOutParams) {

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = krui_learnAllPatterns(parameterInArray, p2,
                                           parameterOutArray,
                                           NoOfOutParams);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_testAllPatterns(float *parameterInArray, SEXP NoOfInParams,
                                          float **parameterOutArray,
                                          int *NoOfOutParams) {

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = krui_testAllPatterns(parameterInArray, p2,
                                          parameterOutArray,
                                          NoOfOutParams);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_learnSinglePattern( SEXP pattern_no,float *parameterInArray,
                                             SEXP NoOfInParams,
                                             float **parameterOutArray,
                                             int *NoOfOutParams) {

  int p1 = Rcpp::as<int>(pattern_no);
  int p3 = Rcpp::as<int>(NoOfInParams);

  int err = krui_learnSinglePattern(p1, parameterInArray,
                                             p3,
                                             parameterOutArray,
                                             NoOfOutParams);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_testSinglePattern( SEXP pattern_no, float *parameterInArray,
                                            SEXP NoOfInParams,
                                            float **parameterOutArray,
                                            int *NoOfOutParams) {
  int p1 = Rcpp::as<int>(pattern_no);
  int p3 = Rcpp::as<int>(NoOfInParams);

  int err = krui_testSinglePattern(p1, parameterInArray,
                                            p3,
                                            parameterOutArray,
                                            NoOfOutParams);
  return Rcpp::wrap(err);
}

RcppExport SEXP checkPruning (void) {
  int ret = krui_checkPruning ();
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_learnAllPatternsFF(float *parameterInArray,
                                             SEXP NoOfInParams,
                                             float **parameterOutArray,
                                             int *NoOfOutParams) {

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = krui_learnAllPatternsFF(parameterInArray,
                                             p2,
                                             parameterOutArray,
                                             NoOfOutParams);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_learnSinglePatternFF( SEXP pattern_no,
                                               float *parameterInArray,
                                               SEXP NoOfInParams,
                                               float **parameterOutArray,
                                               int *NoOfOutParams) {
  int p1 = Rcpp::as<int>(pattern_no);
  int p3 = Rcpp::as<int>(NoOfInParams);

  int err = krui_learnSinglePatternFF(p1,
                                               parameterInArray,
                                               p3,
                                               parameterOutArray,
                                               NoOfOutParams);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_setClassDistribution(unsigned int *classDist) {
  int err = krui_setClassDistribution(classDist);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_setClassInfo(char *name) {
  int err = krui_setClassInfo(name);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_useClassDistribution(bool use_it) {
  int err = krui_useClassDistribution(use_it);
  return Rcpp::wrap(err);
}

char *getPrunFunc (void) {
  return krui_getPrunFunc ();
}

char *getFFLearnFunc (void) {
  return krui_getFFLearnFunc ();
}

RcppExport SEXP RSNNSKrui_setFFLearnFunc (char *FF_learning_func) {
  int err = krui_setFFLearnFunc (FF_learning_func);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_setPrunFunc (char *pruning_func) {
  int err = krui_setPrunFunc (pruning_func);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_showPattern( SEXP mode) {
  int p1 = Rcpp::as<int>(mode);
  int err = krui_showPattern(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_allocNewPatternSet( int *set_no) {
  int err = krui_allocNewPatternSet(set_no);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_newPattern(void) {
  int err = krui_newPattern();
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_setPatternNo( SEXP pattern_no) {
  int p1 = Rcpp::as<int>(pattern_no);
  int err = krui_setPatternNo(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_getPatternNo(void) {
  int err = krui_getPatternNo();
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_deletePattern(void) {
  int err = krui_deletePattern();
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_modifyPattern(void) {
  int err = krui_modifyPattern();
  return Rcpp::wrap(err);
}

void deleteAllPatterns(void) {
  krui_deleteAllPatterns();
}

RcppExport SEXP RSNNSKrui_shufflePatterns(bool on_or_off) {
  int err = krui_shufflePatterns(on_or_off);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_shuffleSubPatterns(bool on_or_off) {
  int err = krui_shuffleSubPatterns(on_or_off);
  return Rcpp::wrap(err);
}

RcppExport SEXP getNoOfPatterns(void) {
  int ret = krui_getNoOfPatterns();
  return Rcpp::wrap(ret);
}

RcppExport SEXP getTotalNoOfSubPatterns(void) {
  int ret = krui_getTotalNoOfSubPatterns();
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_loadNewPatterns(char *filename, int *number) {
  int err = krui_loadNewPatterns(filename, number);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_setCurrPatSet( SEXP number) {
  int p1 = Rcpp::as<int>(number);
  int err = krui_setCurrPatSet(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_saveNewPatterns(char *filename, SEXP number) {

  int p2 = Rcpp::as<int>(number);

  int err = krui_saveNewPatterns(filename, p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_deletePatSet( SEXP number) {
  int p1 = Rcpp::as<int>(number);
  int err = krui_deletePatSet(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_GetPatInfo(pattern_set_info *set_info,
                                     pattern_descriptor *pat_info) {
  int err = krui_GetPatInfo(set_info,
                                   pat_info);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_DefShowSubPat( int *insize, int *outsize,
                                        int *inpos, int *outpos) {
  int err = krui_DefShowSubPat(insize, outsize,
                                       inpos, outpos);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_DefTrainSubPat( int *insize, int *outsize,
                                         int *instep, int *outstep,
                                         int *max_n_pos) {
  int err = krui_DefTrainSubPat(insize, outsize, instep, outstep, max_n_pos);

  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_AlignSubPat( int *inpos, int *outpos, int *no) {
  int err = krui_AlignSubPat(inpos, outpos, no);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_GetShapeOfSubPattern( int *insize, int *outsize,
                                               int *inpos, int *outpos, SEXP n_pos) {

  int p5 = Rcpp::as<int>(n_pos);

  int err = krui_GetShapeOfSubPattern(insize, outsize,
                                           inpos, outpos, p5);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_saveNet(char *filename, char *netname) {
  int err = krui_saveNet(filename, netname);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_loadNet(char *filename, char **netname) {
  int err = krui_loadNet(filename, netname);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_saveResultParam(char *filename, bool create,
                                          SEXP startpattern, SEXP endpattern,
                                          bool includeinput, bool includeoutput,
                                          float *Update_param_array,
                                          SEXP NoOfUpdateParam) {

  int p3 = Rcpp::as<int>(startpattern);
  int p4 = Rcpp::as<int>(endpattern);
  int p8 = Rcpp::as<int>(NoOfUpdateParam);

  int err = krui_saveResultParam(filename, create,
                                          p3, p4,
                                          includeinput, includeoutput,
                                          Update_param_array,
                                          p8);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_allocateUnits( SEXP no_of_units) {
  int p1 = Rcpp::as<int>(no_of_units);
  int err = krui_allocateUnits(p1);
  return Rcpp::wrap(err);
}

void deleteNet(void) {
  krui_deleteNet();
}

bool getFirstSymbolTableEntry(char **symbol_name,
                                                   int *symbol_type) {
  return krui_getFirstSymbolTableEntry(symbol_name,
                                                   symbol_type);
}

bool getNextSymbolTableEntry(char **symbol_name,
                                                  int *symbol_type) {
  return krui_getNextSymbolTableEntry(symbol_name,
                                                  symbol_type);
}

bool symbolSearch(char *symbol, SEXP symbol_type) {

  int p2 = Rcpp::as<int>(symbol_type);

  return krui_symbolSearch(symbol, p2);
}

char *getVersion(void) {
  return krui_getVersion();
}

void getNetInfo( int *no_of_sites, int *no_of_links,
                                     int *no_of_STable_entries,
                                     int *no_of_FTable_entries) {
  krui_getNetInfo(no_of_sites, no_of_links,
                                     no_of_STable_entries,
                                     no_of_FTable_entries);
}

void getMemoryManagerInfo( int *unit_bytes,
                            int *site_bytes, int *link_bytes,
                            int *NTable_bytes, int *STable_bytes,
                            int *FTable_bytes) {
  krui_getMemoryManagerInfo(unit_bytes,
                            site_bytes, link_bytes,
                            NTable_bytes, STable_bytes,
                            FTable_bytes);
}

void getUnitDefaults(FlintType *act, FlintType *bias, int *st,
                            int *subnet_no, int *layer_no, char **act_func,
                            char **out_func) {
  krui_getUnitDefaults(act, bias, st,
                            subnet_no, layer_no, act_func,
                            out_func);
}

RcppExport SEXP RSNNSKrui_setUnitDefaults(FlintTypeParam act,
                            FlintTypeParam bias, SEXP st, SEXP subnet_no,
                            SEXP layer_no, char *act_func, char *out_func) {

  int p3 = Rcpp::as<int>(st);
  int p4 = Rcpp::as<int>(subnet_no);
  int p5 = Rcpp::as<int>(layer_no);

  int err = krui_setUnitDefaults(act,
                            bias, p3, p4, p5, act_func, out_func);
  return Rcpp::wrap(err);
}

void resetNet(void) {
  krui_resetNet();
}

void setSeedNo(long int seed) {
  krui_setSeedNo(seed);
}

RcppExport SEXP getNoOfInputUnits(void) {
  int ret = krui_getNoOfInputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP getNoOfOutputUnits(void) {
  int ret = krui_getNoOfOutputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP getNoOfTTypeUnits( SEXP UnitTType) {
  int p1 = Rcpp::as<int>(UnitTType);
  int ret = krui_getNoOfTTypeUnits(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP getNoOfSpecialInputUnits(void) {
  int ret = krui_getNoOfSpecialInputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP getNoOfSpecialOutputUnits(void) {
  int ret = krui_getNoOfSpecialOutputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_xyTransTable( SEXP op, int *x, int *y, SEXP z) {

  int p1 = Rcpp::as<int>(op);
  int p4 = Rcpp::as<int>(z);

  int err = krui_xyTransTable(p1, x, y, p4);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_getUnitCenters( SEXP unit_no, SEXP center_no,
                            struct PositionVector **unit_center) {

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(center_no);

  int err = krui_getUnitCenters(p1, p2, unit_center);
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_setUnitCenters( SEXP unit_no, SEXP center_no,
                            struct PositionVector *unit_center) {

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(center_no);

  int err = krui_setUnitCenters(p1, p2, unit_center);
  return Rcpp::wrap(err);
}

//char *error( SEXP error_code) {
//  int p1 = Rcpp::as<int>(error_code);
//  return krui_error(p1);
//}

float NA_Error( SEXP currentPattern, SEXP error_unit,
                                       SEXP error, bool ave) {
  int p1 = Rcpp::as<int>(currentPattern);
  int p2 = Rcpp::as<int>(error_unit);
  int p3 = Rcpp::as<int>(error);

  return krui_NA_Error(p1, p2, p3, ave);
}

float getVariance (void) {
  return krui_getVariance ();
}

RcppExport SEXP countLinks(void) {
  int ret = krui_countLinks();
  return Rcpp::wrap(ret);
}


#ifdef MASPAR_KERNEL

RcppExport SEXP RSNNSKrui_MasPar( mode ) {
  int err = krui_MasPar( mode );
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_getMasParStatus(void) {
  int err = krui_getMasParStatus();
  return Rcpp::wrap(err);
}

RcppExport SEXP RSNNSKrui_MasParBenchmark( func_type, cycles, result ) {
  int err = krui_MasParBenchmark(func_type, cycles, result );
  return Rcpp::wrap(err);
}


#endif

RcppExport SEXP RSNNSKrui_setSpecialNetworkType( SEXP net_type) {
  int p1 = Rcpp::as<int>(net_type);
  int err = krui_setSpecialNetworkType(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP getSpecialNetworkType(void) {
  int ret = krui_getSpecialNetworkType();
  return Rcpp::wrap(ret);
}

RcppExport SEXP initInversion(void) {
  int ret = krui_initInversion();
  return Rcpp::wrap(ret);
}

void inv_forwardPass(struct UnitList *inputs) {
  krui_inv_forwardPass(inputs);
}

double inv_backwardPass(float learn, float delta_max,
                                           int *err_units, float ratio,
                                           struct UnitList *inputs,
                                           struct UnitList *outputs) {
  return krui_inv_backwardPass(learn, delta_max,
                                           err_units, ratio,
                                           inputs,
                                           outputs);
}

void kohonen_SetExtraParameter( SEXP x) {
  int p1 = Rcpp::as<int>(x);
  krui_kohonen_SetExtraParameter(p1);
}

void spanning_tree(void) {
  krui_spanning_tree();
}

void cc_updatePosOfSpecialUnits(void) {
  krui_cc_updatePosOfSpecialUnits();
}

RcppExport SEXP RSNNSKrui_cc_deleteAllSpecialUnits(void) {
  int err = krui_cc_deleteAllSpecialUnits();
  return Rcpp::wrap(err);
}

RcppExport SEXP getErrorCode(void) {
  int ret = krui_getErrorCode();
  return Rcpp::wrap(ret);
}

RcppExport SEXP RSNNSKrui_setErrorHandler(void(* error_Handler )(int)) {
  int err = krui_setErrorHandler(error_Handler);
  return Rcpp::wrap(err);
}

void execHandler( SEXP error_code) {
  int p1 = Rcpp::as<int>(error_code);
  krui_execHandler(p1);
}


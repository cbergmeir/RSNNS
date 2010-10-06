#include "RSNNSKrui.h"

#include <R_ext/Print.h>



//using namespace std;

RcppExport SEXP SnnsKrui_getNoOfUnits(void) {
  int ret = krui_getNoOfUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getFirstUnit(void) {
  int ret = krui_getFirstUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getNextUnit(void) {
  int ret = krui_getNextUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getCurrentUnit(void) {
  int ret = krui_getCurrentUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setCurrentUnit( SEXP unit_no) {

  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_setCurrentUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getUnitName( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  const char* ret = krui_getUnitName(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitName( SEXP unit_no, SEXP unit_name) {

  int p1 = Rcpp::as<int>(unit_no);
  std::string p2 = Rcpp::as<std::string>( unit_name );  

  int err = krui_setUnitName(p1, const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_searchUnitName(SEXP unit_name) {

  std::string p1 = Rcpp::as<std::string>( unit_name );

  int ret = krui_searchUnitName(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_searchNextUnitName(void) {
  int ret = krui_searchNextUnitName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getUnitOutFuncName( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  const char* ret = krui_getUnitOutFuncName(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitOutFunc( SEXP unit_no, SEXP unitOutFuncName) {

  int p1 = Rcpp::as<int>(unit_no);
  std::string p2 = Rcpp::as<std::string>( unitOutFuncName );  

  int err = krui_setUnitOutFunc(p1, const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getUnitActFuncName( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  const char* ret = krui_getUnitActFuncName(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitActFunc( SEXP unit_no, SEXP unitActFuncName) {

  int p1 = Rcpp::as<int>(unit_no);
  std::string p2 = Rcpp::as<std::string>( unitActFuncName );  

  int err = krui_setUnitActFunc(p1, const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getUnitFTypeName( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  const char* ret = krui_getUnitFTypeName(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getUnitActivation( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  float ret = krui_getUnitActivation(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitActivation( SEXP UnitNo,
                                     SEXP unit_activation) {
  int p1 = Rcpp::as<int>(UnitNo);
  float p2 = Rcpp::as<float>(unit_activation);

  int err = krui_setUnitActivation(p1, p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getUnitInitialActivation( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  float ret = krui_getUnitInitialActivation(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitInitialActivation( SEXP UnitNo,
                                               SEXP unit_i_activation) {
  int p1 = Rcpp::as<int>(UnitNo);
  float p2 = Rcpp::as<float>(unit_i_activation);
  krui_setUnitInitialActivation(p1, p2);
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_getUnitOutput( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  float ret = krui_getUnitOutput(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitOutput( SEXP unit_no, SEXP unit_output) {

  int p1 = Rcpp::as<int>(unit_no);
  float p2 = Rcpp::as<float>(unit_output);

  int err = krui_setUnitOutput(p1, p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getUnitBias( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  float ret = krui_getUnitBias(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitBias( SEXP UnitNo, SEXP unit_bias) {

  int p1 = Rcpp::as<int>(UnitNo);
  float p2 = Rcpp::as<float>(unit_bias);

  krui_setUnitBias(p1, p2);
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_getUnitValueA( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  float ret = krui_getUnitValueA(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitValueA( SEXP UnitNo, SEXP unit_valueA) {

  int p1 = Rcpp::as<int>(UnitNo);
  float p2 = Rcpp::as<float>(unit_valueA);

  krui_setUnitValueA(p1, p2);
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_getUnitSubnetNo( SEXP UnitNo) {
  int p1 = Rcpp::as<int>(UnitNo);
  int ret = krui_getUnitSubnetNo(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitSubnetNo( SEXP UnitNo, SEXP subnet_no) {
  int p1 = Rcpp::as<int>(UnitNo);
  int p2 = Rcpp::as<int>(subnet_no);
  krui_setUnitSubnetNo(p1, p2);
  return R_NilValue;
}

 
RcppExport SEXP SnnsKrui_getUnitLayerNo( SEXP UnitNo) {

  int p1 = Rcpp::as<int>(UnitNo);

  unsigned short ret = krui_getUnitLayerNo(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitLayerNo( SEXP UnitNo, SEXP layer_no) {
  int p1 = Rcpp::as<int>(UnitNo);
  int p2 = Rcpp::as<int>(layer_no);
  krui_setUnitLayerNo(p1, p2);
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_getUnitPosition( SEXP UnitNo ) {
  int p1 = Rcpp::as<int>(UnitNo);

  PosType position;

  krui_getUnitPosition(p1, &position);

  return Rcpp::List::create( 
    	Rcpp::Named( "x" ) = position.x, 
    	Rcpp::Named( "y" ) = position.y,
    	Rcpp::Named( "z" ) = position.z
    	) ;
}



RcppExport SEXP SnnsKrui_setUnitPosition( SEXP UnitNo, SEXP x, SEXP y, SEXP z) {

  int p1 = Rcpp::as<int>(UnitNo);

  PosType position;
  position.x = Rcpp::as<short>(x);
  position.y = Rcpp::as<short>(y);
  position.z = Rcpp::as<short>(z);

  krui_setUnitPosition(p1, &position);
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_getUnitNoAtPosition(SEXP x, SEXP y, SEXP z,
                                              SEXP subnet_no) {

  PosType position;
  position.x = Rcpp::as<short>(x);
  position.y = Rcpp::as<short>(y);
  position.z = Rcpp::as<short>(z);

  int p2 = Rcpp::as<int>(subnet_no);

  int ret = krui_getUnitNoAtPosition(&position, p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getUnitNoNearPosition(SEXP x, SEXP y, SEXP z,
                                                SEXP subnet_no,
                                                SEXP range, SEXP gridWidth) {

  PosType position;
  position.x = Rcpp::as<short>(x);
  position.y = Rcpp::as<short>(y);
  position.z = Rcpp::as<short>(z);

  int p2 = Rcpp::as<int>(subnet_no);
  int p3 = Rcpp::as<int>(range);
  int p4 = Rcpp::as<int>(gridWidth);

  int ret = krui_getUnitNoNearPosition(&position, p2, p3, p4);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getUnitTType( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int ret = krui_getUnitTType(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitTType( SEXP unit_no, SEXP UnitTType) {

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(UnitTType);

  int err = krui_setUnitTType(p1, p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_freezeUnit( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_freezeUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_unfreezeUnit( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_unfreezeUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_isUnitFrozen( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  bool ret = krui_isUnitFrozen(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getUnitInputType( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int ret = krui_getUnitInputType(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_createDefaultUnit(void) {
  int ret = krui_createDefaultUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_createUnit(SEXP unit_name, SEXP out_func_name,
                               SEXP act_func_name, SEXP i_act,
                               SEXP bias) {

  std::string p1 = Rcpp::as<std::string>( unit_name );  
  std::string p2 = Rcpp::as<std::string>( out_func_name );  
  std::string p3 = Rcpp::as<std::string>( act_func_name );  
  float p4 = Rcpp::as<float>(i_act);
  float p5 = Rcpp::as<float>(bias);

  int ret = krui_createUnit(const_cast<char*>(p1.c_str()), const_cast<char*>(p2.c_str()), const_cast<char*>(p3.c_str()), p4, p5);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_deleteUnit( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_deleteUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_deleteUnitList( SEXP no_of_units, int unit_list[]) {

  int p1 = Rcpp::as<int>(no_of_units);

  int err = krui_deleteUnitList(p1, unit_list);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_createFTypeUnit(SEXP Ftype_symbol) {

  std::string p1 = Rcpp::as<std::string>( Ftype_symbol );  

  int ret = krui_createFTypeUnit(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUnitFType( SEXP unit_no, SEXP Ftype_symbol) {
  int p1 = Rcpp::as<int>(unit_no);
  std::string p2 = Rcpp::as<std::string>( Ftype_symbol );  

  int err = krui_setUnitFType(p1, const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_copyUnit( SEXP unit_no, SEXP copy_mode) {

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(copy_mode);

  int ret = krui_copyUnit(p1, p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setFirstFTypeEntry(void) {
  bool ret = krui_setFirstFTypeEntry();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setNextFTypeEntry(void) {
  bool ret = krui_setNextFTypeEntry();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setFTypeEntry(SEXP Ftype_symbol) {

  std::string p1 = Rcpp::as<std::string>( Ftype_symbol );  

  bool ret = krui_setFTypeEntry(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getFTypeName(void) {
  const char* ret = krui_getFTypeName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setFTypeName(SEXP Ftype_symbol) {

  std::string p1 = Rcpp::as<std::string>( Ftype_symbol );  

  int err = krui_setFTypeName(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getFTypeActFuncName(void) {
  const char* ret = krui_getFTypeActFuncName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setFTypeActFunc(SEXP act_func_name) {

  std::string p1 = Rcpp::as<std::string>( act_func_name );  

  int err = krui_setFTypeActFunc(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_setFTypeOutFunc(SEXP out_func_name) {

  std::string p1 = Rcpp::as<std::string>( out_func_name );  

  int err = krui_setFTypeOutFunc(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_setFirstFTypeSite(void) {
  bool ret = krui_setFirstFTypeSite();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setNextFTypeSite(void) {
  bool ret = krui_setNextFTypeSite();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getFTypeSiteName(void) {
  const char* ret = krui_getFTypeSiteName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setFTypeSiteName(SEXP FType_site_name) {

  std::string p1 = Rcpp::as<std::string>( FType_site_name );  

  int err = krui_setFTypeSiteName(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_createFTypeEntry(SEXP Ftype_symbol, SEXP act_func_name,
                                           SEXP out_func_name, SEXP no_of_sites,
                                           char **array_of_site_names) {

  std::string p1 = Rcpp::as<std::string>( Ftype_symbol );  
  std::string p2 = Rcpp::as<std::string>( act_func_name );  
  std::string p3 = Rcpp::as<std::string>( out_func_name );  
  int p4 = Rcpp::as<int>(no_of_sites);

  int err = krui_createFTypeEntry(const_cast<char*>(p1.c_str()), const_cast<char*>(p2.c_str()),
                                           const_cast<char*>(p3.c_str()), p4,
                                           array_of_site_names);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_deleteFTypeEntry(SEXP Ftype_symbol) {

  std::string p1 = Rcpp::as<std::string>( Ftype_symbol );  

  int err = krui_deleteFTypeEntry(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getNoOfFunctions(void) {
  int ret = krui_getNoOfFunctions();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getFuncInfo( SEXP func_no ) {
  int p1 = Rcpp::as<int>(func_no);

  char* func_name;
  int func_type;

  krui_getFuncInfo(p1, &func_name, &func_type);

  return Rcpp::List::create( 
    	Rcpp::Named( "func_name" ) = (const char*) func_name, 
    	Rcpp::Named( "func_type" ) = func_type
    	);
}

RcppExport SEXP SnnsKrui_isFunction(SEXP func_name, SEXP func_type) {

  std::string p1 = Rcpp::as<std::string>( func_name );  
  int p2 = Rcpp::as<int>(func_type);
  
  bool ret = krui_isFunction(const_cast<char*>(p1.c_str()), p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getFuncParamInfo(SEXP func_name, SEXP func_type) {

  std::string p1 = Rcpp::as<std::string>( func_name );  
  int p2 = Rcpp::as<int>(func_type);

  int no_of_input_params=0;
  int no_of_output_params=0;

  bool ret = krui_getFuncParamInfo(const_cast<char*>(p1.c_str()), p2,
                                       &no_of_input_params,
                                       &no_of_output_params);

  return Rcpp::List::create( 
    	Rcpp::Named( "ret" ) = ret, 
    	Rcpp::Named( "no_of_input_params" ) = no_of_input_params,
    	Rcpp::Named( "no_of_output_params" ) = no_of_output_params
    	) ;
}

RcppExport SEXP SnnsKrui_getFirstSiteTableEntry() {

  char* site_name;
  char* site_func;

  bool ret = krui_getFirstSiteTableEntry(&site_name, &site_func);

  return Rcpp::List::create( 
    	Rcpp::Named( "ret" ) = ret, 
    	Rcpp::Named( "site_name" ) = (const char*) site_name,
    	Rcpp::Named( "site_func" ) = (const char*) site_func
    	) ;
}

RcppExport SEXP SnnsKrui_getNextSiteTableEntry() {

  char* site_name;
  char* site_func;

  bool ret = krui_getNextSiteTableEntry(&site_name, &site_func);

  return Rcpp::List::create( 
    	Rcpp::Named( "ret" ) = ret, 
    	Rcpp::Named( "site_name" ) = (const char*) site_name,
    	Rcpp::Named( "site_func" ) = (const char*) site_func
    	) ;
}

RcppExport SEXP SnnsKrui_getSiteTableFuncName(SEXP site_name) {

  std::string p1 = Rcpp::as<std::string>( site_name );  

  const char* ret = krui_getSiteTableFuncName(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_createSiteTableEntry(SEXP site_name, SEXP site_func) {

  std::string p1 = Rcpp::as<std::string>( site_name );  
  std::string p2 = Rcpp::as<std::string>( site_func );  

  int err = krui_createSiteTableEntry(const_cast<char*>(p1.c_str()), const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_changeSiteTableEntry(SEXP old_site_name,
                                               SEXP new_site_name,
                                               SEXP new_site_func) {

  std::string p1 = Rcpp::as<std::string>( old_site_name );  
  std::string p2 = Rcpp::as<std::string>( new_site_name );  
  std::string p3 = Rcpp::as<std::string>( new_site_func );  

  int err = krui_changeSiteTableEntry(const_cast<char*>(p1.c_str()),
                                               const_cast<char*>(p2.c_str()),
                                               const_cast<char*>(p3.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_deleteSiteTableEntry(SEXP site_name) {

  std::string p1 = Rcpp::as<std::string>( site_name );  

  int err = krui_deleteSiteTableEntry(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_setFirstSite(void) {
  bool ret = krui_setFirstSite();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setNextSite(void) {
  bool ret = krui_setNextSite();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setSite(SEXP site_name) {

  std::string p1 = Rcpp::as<std::string>( site_name );  
 
  int err = krui_setSite(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getSiteValue(void) {
  float ret = krui_getSiteValue();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getSiteName(void) {
  const char* ret = krui_getSiteName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setSiteName(SEXP site_name) {

  std::string p1 = Rcpp::as<std::string>( site_name );  

  int err = krui_setSiteName(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getSiteFuncName(void) {
  const char* ret = krui_getSiteFuncName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_addSite(SEXP site_name) {

  std::string p1 = Rcpp::as<std::string>( site_name );  

  int err = krui_addSite(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_deleteSite(void) {
  bool ret = krui_deleteSite();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getFirstPredUnit(FlintType *strength) {
  int ret = krui_getFirstPredUnit(strength);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getFirstPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c) {
  int ret = krui_getFirstPredUnitAndData(strength, val_a, val_b, val_c);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getNextPredUnit(FlintType *strength) {
  int ret = krui_getNextPredUnit(strength);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getNextPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c) {
  int ret = krui_getNextPredUnitAndData(strength, val_a, val_b, val_c);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getCurrentPredUnit(FlintType *strength) {
  int ret = krui_getCurrentPredUnit(strength);
  return Rcpp::wrap(ret);
}


RcppExport SEXP SnnsKrui_getFirstSuccUnit( SEXP source_unit_no, FlintType *weight) {
  int p1 = Rcpp::as<int>(source_unit_no);
  int ret = krui_getFirstSuccUnit(p1, weight);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getNextSuccUnit(FlintType *weight) {
  int ret = krui_getNextSuccUnit(weight);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_areConnected( SEXP source_unit_no, SEXP target_unit_no) {

  int p1 = Rcpp::as<int>(source_unit_no);
  int p2 = Rcpp::as<int>(target_unit_no);

  bool ret = krui_areConnected(p1, p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_areConnectedWeight( SEXP source_unit_no, SEXP target_unit_no,
                                             FlintType *weight) {

  int p1 = Rcpp::as<int>(source_unit_no);
  int p2 = Rcpp::as<int>(target_unit_no);

  bool ret = krui_areConnectedWeight(p1, p2, weight);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_isConnected( SEXP source_unit_no) {
  int p1 = Rcpp::as<int>(source_unit_no);
  bool ret = krui_isConnected(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getLinkWeight(void) {
  float ret = krui_getLinkWeight();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setLinkWeight(SEXP weight) {

  float p1 = Rcpp::as<float>(weight);

  krui_setLinkWeight(p1);
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_createLink( SEXP source_unit_no, SEXP weight) {

  int p1 = Rcpp::as<int>(source_unit_no);
  float p2 = Rcpp::as<float>(weight);

  int err = krui_createLink(p1, p2);
  return Rcpp::wrap(err);
}

struct Link* SnnsKrui_createLinkWithAdditionalParameters(
                            SEXP source_unit_no, SEXP weight,
                            SEXP val_a,SEXP val_b,SEXP val_c) {

  int p1 = Rcpp::as<int>(source_unit_no);
  float p2 = Rcpp::as<float>(weight);
  float p3 = Rcpp::as<float>(val_a);
  float p4 = Rcpp::as<float>(val_b);
  float p5 = Rcpp::as<float>(val_c);

  return krui_createLinkWithAdditionalParameters( p1, p2, p3, p4, p5);
}

RcppExport SEXP SnnsKrui_deleteLink(void) {
  int err = krui_deleteLink();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_deleteAllInputLinks(void) {
  int err = krui_deleteAllInputLinks();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_deleteAllOutputLinks(void) {
  int err = krui_deleteAllOutputLinks();
  return Rcpp::wrap(err);
}


RcppExport SEXP SnnsKrui_jogWeights(SEXP minus, SEXP plus) {

  float p1 = Rcpp::as<float>(minus);
  float p2 = Rcpp::as<float>(plus);

  krui_jogWeights(p1, p2);
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_jogCorrWeights(SEXP minus, SEXP plus,
                                         SEXP mincorr) {

  float p1 = Rcpp::as<float>(minus);
  float p2 = Rcpp::as<float>(plus);
  float p3 = Rcpp::as<float>(mincorr);

  int err = krui_jogCorrWeights(p1, p2, p3);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_updateSingleUnit( SEXP unit_no) {
  int p1 = Rcpp::as<int>(unit_no);
  int err = krui_updateSingleUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getUpdateFunc(void) {
  const char* ret = krui_getUpdateFunc();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setUpdateFunc(SEXP update_func) {

  std::string p1 = Rcpp::as<std::string>( update_func );  

  int err = krui_setUpdateFunc(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_setRemapFunc(SEXP name, float *params) {

  std::string p1 = Rcpp::as<std::string>( name );  

  int err = krui_setRemapFunc(const_cast<char*>(p1.c_str()), params);
  return Rcpp::wrap(err);
}

/*RcppExport SEXP SnnsKrui_updateNet(float *parameterInArray, SEXP NoOfInParams) {

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = krui_updateNet(parameterInArray, p2);
  return Rcpp::wrap(err);
}*/

RcppExport SEXP SnnsKrui_updateNet(SEXP parameterInArray) {

  Rcpp::NumericVector params(parameterInArray);

  //TODO: this should be variable length..
  float p1[100];

  int n = params.size();
  for (int i=0; i<n; i++) {
    p1[i] = static_cast<float>(params(i));
  }

  int err = krui_updateNet(p1, n);
  return Rcpp::wrap(err);
}


RcppExport SEXP SnnsKrui_getInitialisationFunc(void) {
  const char* ret = krui_getInitialisationFunc();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setInitialisationFunc(SEXP initialisation_func) {

  std::string p1 = Rcpp::as<std::string>( initialisation_func );  

  int err = krui_setInitialisationFunc(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_initializeNet(SEXP parameterInArray) {

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p1[n+1];

  for (int i=0; i<n; i++) {
    p1[i] = static_cast<float>(params(i));
  }

  int err = krui_initializeNet(p1, n);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getLearnFunc(void) {
  const char* ret = krui_getLearnFunc();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setLearnFunc(SEXP learning_func) {

  std::string p1 = Rcpp::as<std::string>( learning_func );

  int err = krui_setLearnFunc(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_trainNetwork(NetLearnParameters *parameters) {
  int err = krui_trainNetwork(parameters);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getNetworkErrorArray(double **learnErrors,int **atEpoch,
                                               int *noOfErrors) {
  int err = krui_getNetworkErrorArray(learnErrors, atEpoch,
                                               noOfErrors);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_stopTraining(void) {
  int err = krui_stopTraining();
  return Rcpp::wrap(err);
}


RcppExport SEXP SnnsKrui_learnAllPatterns(SEXP parameterInArray) {

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p1[n+1];

  for (int i=0; i<n; i++) {
    p1[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = krui_learnAllPatterns(p1, n, &parameterOutArray, &NoOfOutParams);

  Rcpp::NumericVector out(NoOfOutParams);
  for (int i=0; i<NoOfOutParams; i++) {
    out[i] = parameterOutArray[i];
  }

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "parameterOutArray" ) = out
    	) ;
}

RcppExport SEXP SnnsKrui_testAllPatterns(SEXP parameterInArray) {

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p1[n+1];

  for (int i=0; i<n; i++) {
    p1[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = krui_testAllPatterns(p1, n, &parameterOutArray, &NoOfOutParams);

  Rcpp::NumericVector out(NoOfOutParams);
  for (int i=0; i<NoOfOutParams; i++) {
    out[i] = parameterOutArray[i];
  }

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "parameterOutArray" ) = out
    	) ;
}

/*RcppExport SEXP SnnsKrui_testAllPatterns(float *parameterInArray, SEXP NoOfInParams,
                                          float **parameterOutArray,
                                          int *NoOfOutParams) {

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = krui_testAllPatterns(parameterInArray, p2,
                                          parameterOutArray,
                                          NoOfOutParams);
  return Rcpp::wrap(err);
}*/

RcppExport SEXP SnnsKrui_learnSinglePattern(SEXP pattern_no, SEXP parameterInArray) {

  int p1 = Rcpp::as<int>(pattern_no);

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p2[n+1];

  for (int i=0; i<n; i++) {
    p2[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = krui_learnSinglePattern(p1, p2, n, &parameterOutArray, &NoOfOutParams);

  Rcpp::NumericVector out(NoOfOutParams);
  for (int i=0; i<NoOfOutParams; i++) {
    out[i] = parameterOutArray[i];
  }

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "parameterOutArray" ) = out
    	) ;
}

/*RcppExport SEXP SnnsKrui_learnSinglePattern( SEXP pattern_no,float *parameterInArray,
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
}*/

RcppExport SEXP SnnsKrui_testSinglePattern(SEXP pattern_no, SEXP parameterInArray) {

  int p1 = Rcpp::as<int>(pattern_no);

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p2[n+1];

  for (int i=0; i<n; i++) {
    p2[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = krui_testSinglePattern(p1, p2, n, &parameterOutArray, &NoOfOutParams);

  Rcpp::NumericVector out(NoOfOutParams);
  for (int i=0; i<NoOfOutParams; i++) {
    out[i] = parameterOutArray[i];
  }

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "parameterOutArray" ) = out
    	) ;
}
/*
RcppExport SEXP SnnsKrui_testSinglePattern( SEXP pattern_no, float *parameterInArray,
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
*/

RcppExport SEXP SnnsKrui_checkPruning (void) {
  int ret = krui_checkPruning ();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_learnAllPatternsFF(SEXP parameterInArray) {

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p1[n+1];

  for (int i=0; i<n; i++) {
    p1[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = krui_learnAllPatternsFF(p1, n, &parameterOutArray, &NoOfOutParams);

  Rcpp::NumericVector out(NoOfOutParams);
  for (int i=0; i<NoOfOutParams; i++) {
    out[i] = parameterOutArray[i];
  }

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "parameterOutArray" ) = out
    	) ;
}

/*RcppExport SEXP SnnsKrui_learnAllPatternsFF(float *parameterInArray,
                                             SEXP NoOfInParams,
                                             float **parameterOutArray,
                                             int *NoOfOutParams) {

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = krui_learnAllPatternsFF(parameterInArray,
                                             p2,
                                             parameterOutArray,
                                             NoOfOutParams);
  return Rcpp::wrap(err);
}*/

RcppExport SEXP SnnsKrui_learnSinglePatternFF(SEXP pattern_no, SEXP parameterInArray) {

  int p1 = Rcpp::as<int>(pattern_no);

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p2[n+1];

  for (int i=0; i<n; i++) {
    p2[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = krui_learnSinglePatternFF(p1, p2, n, &parameterOutArray, &NoOfOutParams);

  Rcpp::NumericVector out(NoOfOutParams);
  for (int i=0; i<NoOfOutParams; i++) {
    out[i] = parameterOutArray[i];
  }

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "parameterOutArray" ) = out
    	) ;
}

/*
RcppExport SEXP SnnsKrui_learnSinglePatternFF( SEXP pattern_no,
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
*/

RcppExport SEXP SnnsKrui_setClassDistribution(unsigned int *classDist) {
  int err = krui_setClassDistribution(classDist);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_setClassInfo(SEXP name) {

  std::string p1 = Rcpp::as<std::string>( name );  

  int err = krui_setClassInfo(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_useClassDistribution(SEXP use_it) {

  int p1 = Rcpp::as<bool>(use_it);

  int err = krui_useClassDistribution(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getPrunFunc (void) {
  const char* ret = krui_getPrunFunc ();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getFFLearnFunc (void) {
  const char* ret = krui_getFFLearnFunc ();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setFFLearnFunc (SEXP FF_learning_func) {

  std::string p1 = Rcpp::as<std::string>( FF_learning_func );  

  int err = krui_setFFLearnFunc (const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_setPrunFunc (SEXP pruning_func) {

  std::string p1 = Rcpp::as<std::string>( pruning_func );  

  int err = krui_setPrunFunc (const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_showPattern( SEXP mode) {
  int p1 = Rcpp::as<int>(mode);
  int err = krui_showPattern(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_allocNewPatternSet() {

  int set_no;
  int err = krui_allocNewPatternSet(&set_no);

  //return Rcpp::wrap(err);
  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "set_no" ) = set_no
    	) ;
}

RcppExport SEXP SnnsKrui_newPattern(void) {
  int err = krui_newPattern();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_setPatternNo( SEXP pattern_no) {
  int p1 = Rcpp::as<int>(pattern_no);
  int err = krui_setPatternNo(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getPatternNo(void) {
  int err = krui_getPatternNo();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_deletePattern(void) {
  int err = krui_deletePattern();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_modifyPattern(void) {
  int err = krui_modifyPattern();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_deleteAllPatterns(void) {
  krui_deleteAllPatterns();
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_shufflePatterns(SEXP on_or_off) {

  int p1 = Rcpp::as<bool>(on_or_off);

  int err = krui_shufflePatterns(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_shuffleSubPatterns(SEXP on_or_off) {

  int p1 = Rcpp::as<bool>(on_or_off);

  int err = krui_shuffleSubPatterns(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getNoOfPatterns(void) {
  int ret = krui_getNoOfPatterns();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getTotalNoOfSubPatterns(void) {
  int ret = krui_getTotalNoOfSubPatterns();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_loadNewPatterns(SEXP filename) {

  std::string p1 = Rcpp::as<std::string>( filename );  

  int number=0;

  int err = krui_loadNewPatterns(const_cast<char*>(p1.c_str()), &number);

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "number" ) = number
    	) ;

}

RcppExport SEXP SnnsKrui_setCurrPatSet( SEXP number) {
  int p1 = Rcpp::as<int>(number);
  int err = krui_setCurrPatSet(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_saveNewPatterns(SEXP filename, SEXP number) {

  std::string p1 = Rcpp::as<std::string>( filename );  
  int p2 = Rcpp::as<int>(number);

  int err = krui_saveNewPatterns(const_cast<char*>(p1.c_str()), p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_deletePatSet( SEXP number) {
  int p1 = Rcpp::as<int>(number);
  int err = krui_deletePatSet(p1);
  return Rcpp::wrap(err);
}

/*RcppExport SEXP SnnsKrui_GetPatInfo(pattern_set_info *set_info,
                                     pattern_descriptor *pat_info) {
  int err = krui_GetPatInfo(set_info,
                                   pat_info);
  return Rcpp::wrap(err);
}*/


//typedef struct
//{
//    int input_dim;             /* number of variable input dimensions        */
//    int input_dim_sizes[MAX_NO_OF_VAR_I_DIM];  /* actual sizes of the input  */
//                                               /* dimensions                 */
//    int input_fixsize;         /* size of the fixed part of the in pattern   */
//                               /* or 0 if no input pattern present           */
//    int output_dim;            /* number of variable output dimensions       */
//    int output_dim_sizes[MAX_NO_OF_VAR_O_DIM]; /* actual sizes of the output */
//                                               /* dimensions                 */
//    int output_fixsize;        /* size of the fixed part of the out pattern  */
//                               /* or 0 if no output pattern present          */
//    int my_class;              /* class index of this pattern, if classes    */
//                               /* available, -1 otherwise                    */
//} pattern_descriptor;

RcppExport SEXP SnnsKrui_GetPatInfo(void) {

  pattern_set_info set_info;
  pattern_descriptor pat_info;

  int err = krui_GetPatInfo(&set_info, &pat_info);

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "input_fixsize" ) = pat_info.input_fixsize,
    	Rcpp::Named( "output_fixsize" ) = pat_info.output_fixsize
//    	Rcpp::Named( "input_dim" ) = pat_info.input_dim,
//    	Rcpp::Named( "output_dim" ) = pat_info.output_dim,
//    	Rcpp::Named( "input_dim_sizes" ) = pat_info.input_dim_sizes[0], 
//    	Rcpp::Named( "output_dim_sizes" ) = pat_info.output_dim_sizes[0]
    	) ;

//  int err=0;
//  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_DefShowSubPat() {

  int insize=0;
  int outsize=0;
  int inpos=0;
  int outpos=0;
  int max_n_pos=0;

  int err = krui_DefShowSubPat(&insize, &outsize, &inpos, &outpos);

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "insize" ) = insize,
    	Rcpp::Named( "outsize" ) = outsize,
    	Rcpp::Named( "inpos" ) = inpos, 
    	Rcpp::Named( "outpos" ) = outpos
    	) ;
}


RcppExport SEXP SnnsKrui_DefTrainSubPat() {

  int insize=0;
  int outsize=0;
  int instep=0;
  int outstep=0;
  int max_n_pos=0;

  int err = krui_DefTrainSubPat(&insize, &outsize, &instep, &outstep, &max_n_pos);

  //return Rcpp::wrap(err);
  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "insize" ) = insize,
    	Rcpp::Named( "outsize" ) = outsize,
    	Rcpp::Named( "instep" ) = instep, 
    	Rcpp::Named( "outstep" ) = outstep,
    	Rcpp::Named( "max_n_pos" ) = max_n_pos
    	) ;
}

RcppExport SEXP SnnsKrui_AlignSubPat( int *inpos, int *outpos, int *no) {
  int err = krui_AlignSubPat(inpos, outpos, no);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_GetShapeOfSubPattern( int *insize, int *outsize,
                                               int *inpos, int *outpos, SEXP n_pos) {

  int p5 = Rcpp::as<int>(n_pos);

  int err = krui_GetShapeOfSubPattern(insize, outsize,
                                           inpos, outpos, p5);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_saveNet(SEXP filename, SEXP netname) {

  std::string p1 = Rcpp::as<std::string>( filename );  
  std::string p2 = Rcpp::as<std::string>( netname );  

  int err = krui_saveNet(const_cast<char*>(p1.c_str()), const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_loadNet(SEXP filename) {

  std::string p1 = Rcpp::as<std::string>( filename );  
  char* netname;

  int err = krui_loadNet(const_cast<char*>(p1.c_str()), &netname);

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "netname" ) = (const char*) netname
    	) ;

}

RcppExport SEXP SnnsKrui_saveResultParam(SEXP filename, SEXP create,
                                          SEXP startpattern, SEXP endpattern,
                                          SEXP includeinput, SEXP includeoutput,
                                          float *Update_param_array,
                                          SEXP NoOfUpdateParam) {

  std::string p1 = Rcpp::as<std::string>( filename );  

  int p2 = Rcpp::as<bool>(create);

  int p3 = Rcpp::as<int>(startpattern);
  int p4 = Rcpp::as<int>(endpattern);

  int p5 = Rcpp::as<bool>(includeinput);
  int p6 = Rcpp::as<bool>(includeoutput);

  int p8 = Rcpp::as<int>(NoOfUpdateParam);

  int err = krui_saveResultParam(const_cast<char*>(p1.c_str()), p2, p3, p4, p5, p6, Update_param_array, p8);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_allocateUnits( SEXP no_of_units) {
  int p1 = Rcpp::as<int>(no_of_units);
  int err = krui_allocateUnits(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_deleteNet(void) {
  krui_deleteNet();
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_getFirstSymbolTableEntry() {

  char* symbol_name;
  int symbol_type;

  bool ret = krui_getFirstSymbolTableEntry(&symbol_name, &symbol_type);

  return Rcpp::List::create( 
    	Rcpp::Named( "ret" ) = ret, 
    	Rcpp::Named( "symbol_name" ) = (const char*) symbol_name,
    	Rcpp::Named( "symbol_type" ) = symbol_type
    	) ;
}

RcppExport SEXP SnnsKrui_getNextSymbolTableEntry() {

  char* symbol_name;
  int symbol_type;

  bool ret = krui_getNextSymbolTableEntry(&symbol_name, &symbol_type);

  return Rcpp::List::create( 
    	Rcpp::Named( "ret" ) = ret, 
    	Rcpp::Named( "symbol_name" ) = (const char*) symbol_name,
    	Rcpp::Named( "symbol_type" ) = symbol_type
    	) ;
}

RcppExport SEXP SnnsKrui_symbolSearch(SEXP symbol, SEXP symbol_type) {

  std::string p1 = Rcpp::as<std::string>( symbol );  
  int p2 = Rcpp::as<int>(symbol_type);

  bool ret = krui_symbolSearch(const_cast<char*>(p1.c_str()), p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getVersion(void) {
  const char* ret = krui_getVersion();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getNetInfo() {

  int no_of_sites=0;
  int no_of_links=0;
  int no_of_STable_entries=0;
  int no_of_FTable_entries=0;

  krui_getNetInfo(&no_of_sites, &no_of_links, &no_of_STable_entries, &no_of_FTable_entries);

  return Rcpp::List::create( 
    	Rcpp::Named( "no_of_sites" ) = no_of_sites, 
    	Rcpp::Named( "no_of_links" ) = no_of_links,
    	Rcpp::Named( "no_of_STable_entries" ) = no_of_STable_entries,
    	Rcpp::Named( "no_of_FTable_entries" ) = no_of_FTable_entries
    	) ;
}

/*RcppExport SEXP SnnsKrui_getNetInfo( int *no_of_sites, int *no_of_links,
                                     int *no_of_STable_entries,
                                     int *no_of_FTable_entries) {
  krui_getNetInfo(no_of_sites, no_of_links,
                                     no_of_STable_entries,
                                     no_of_FTable_entries);
  return R_NilValue;
}*/

RcppExport SEXP SnnsKrui_getMemoryManagerInfo() {

  int unit_bytes=0;
  int site_bytes=0;
  int link_bytes=0;
  int NTable_bytes=0;
  int STable_bytes=0;
  int FTable_bytes=0;

  krui_getMemoryManagerInfo(&unit_bytes,
                            &site_bytes, &link_bytes,
                            &NTable_bytes, &STable_bytes,
                            &FTable_bytes);

  return Rcpp::List::create( 
    	Rcpp::Named( "unit_bytes" ) = unit_bytes, 
    	Rcpp::Named( "site_bytes" ) = site_bytes,
    	Rcpp::Named( "link_bytes" ) = link_bytes,
    	Rcpp::Named( "NTable_bytes" ) = NTable_bytes,
    	Rcpp::Named( "STable_bytes" ) = STable_bytes,
    	Rcpp::Named( "FTable_bytes" ) = FTable_bytes
    	) ;
}

/*RcppExport SEXP SnnsKrui_getMemoryManagerInfo( int *unit_bytes,
                            int *site_bytes, int *link_bytes,
                            int *NTable_bytes, int *STable_bytes,
                            int *FTable_bytes) {
  krui_getMemoryManagerInfo(unit_bytes,
                            site_bytes, link_bytes,
                            NTable_bytes, STable_bytes,
                            FTable_bytes);
  return R_NilValue;
}*/

RcppExport SEXP SnnsKrui_getUnitDefaults() {

  float act, bias;
  int st, subnet_no, layer_no;
  char *act_func, *out_func;

  krui_getUnitDefaults(&act, &bias, &st,
                            &subnet_no, &layer_no, &act_func,
                            &out_func);

  return Rcpp::List::create( 
    	Rcpp::Named( "act" ) = act, 
    	Rcpp::Named( "bias" ) = bias,
    	Rcpp::Named( "st" ) = st,
    	Rcpp::Named( "subnet_no" ) = subnet_no, 
    	Rcpp::Named( "layer_no" ) = layer_no,
    	Rcpp::Named( "act_func" ) = (const char*) act_func,
    	Rcpp::Named( "out_func" ) = (const char*) out_func
    	) ;

//  return R_NilValue;
}

RcppExport SEXP SnnsKrui_setUnitDefaults(SEXP act,
                            SEXP bias, SEXP st, SEXP subnet_no,
                            SEXP layer_no, SEXP act_func, SEXP out_func) {

  float p1 = Rcpp::as<float>(act);
  float p2 = Rcpp::as<float>(bias);
  int p3 = Rcpp::as<int>(st);
  int p4 = Rcpp::as<int>(subnet_no);
  int p5 = Rcpp::as<int>(layer_no);
  std::string p6 = Rcpp::as<std::string>( act_func );
  std::string p7 = Rcpp::as<std::string>( out_func );  

  int err = krui_setUnitDefaults(p1, p2, p3, p4, p5, const_cast<char*>(p6.c_str()), const_cast<char*>(p7.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_resetNet(void) {
  krui_resetNet();
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_setSeedNo(SEXP seed) {

  long int p1 = Rcpp::as<long int>(seed);

  krui_setSeedNo(p1);
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_getNoOfInputUnits(void) {
  int ret = krui_getNoOfInputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getNoOfOutputUnits(void) {
  int ret = krui_getNoOfOutputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getNoOfTTypeUnits( SEXP UnitTType) {
  int p1 = Rcpp::as<int>(UnitTType);
  int ret = krui_getNoOfTTypeUnits(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getNoOfSpecialInputUnits(void) {
  int ret = krui_getNoOfSpecialInputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getNoOfSpecialOutputUnits(void) {
  int ret = krui_getNoOfSpecialOutputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_xyTransTable( SEXP op, int *x, int *y, SEXP z) {

  int p1 = Rcpp::as<int>(op);
  int p4 = Rcpp::as<int>(z);

  int err = krui_xyTransTable(p1, x, y, p4);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getUnitCenters( SEXP unit_no, SEXP center_no,
                            struct PositionVector **unit_center) {

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(center_no);

  int err = krui_getUnitCenters(p1, p2, unit_center);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_setUnitCenters( SEXP unit_no, SEXP center_no,
                            struct PositionVector *unit_center) {

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(center_no);

  int err = krui_setUnitCenters(p1, p2, unit_center);
  return Rcpp::wrap(err);
}

//RcppExport SEXP SnnsKrui_error( SEXP error_code) {
//  int p1 = Rcpp::as<int>(error_code);
//  const char* ret = krui_error(p1);
//  return Rcpp::wrap(ret);
//}

RcppExport SEXP SnnsKrui_NA_Error( SEXP currentPattern, SEXP error_unit,
                                       SEXP error, SEXP ave) {
  int p1 = Rcpp::as<int>(currentPattern);
  int p2 = Rcpp::as<int>(error_unit);
  int p3 = Rcpp::as<int>(error);
  int p4 = Rcpp::as<bool>(ave);

  float ret = krui_NA_Error(p1, p2, p3, p4);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_getVariance (void) {
  float ret = krui_getVariance ();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_countLinks(void) {
  int ret = krui_countLinks();
  return Rcpp::wrap(ret);
}


#ifdef MASPAR_KERNEL

RcppExport SEXP SnnsKrui_MasPar( mode ) {
  int err = krui_MasPar( mode );
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getMasParStatus(void) {
  int err = krui_getMasParStatus();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_MasParBenchmark( func_type, cycles, result ) {
  int err = krui_MasParBenchmark(func_type, cycles, result );
  return Rcpp::wrap(err);
}


#endif

RcppExport SEXP SnnsKrui_setSpecialNetworkType( SEXP net_type) {
  int p1 = Rcpp::as<int>(net_type);
  int err = krui_setSpecialNetworkType(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getSpecialNetworkType(void) {
  int ret = krui_getSpecialNetworkType();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_initInversion(void) {
  int ret = krui_initInversion();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_inv_forwardPass(struct UnitList *inputs) {
  krui_inv_forwardPass(inputs);
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_inv_backwardPass(SEXP learn, SEXP delta_max,
                                           int *err_units, SEXP ratio,
                                           struct UnitList *inputs,
                                           struct UnitList *outputs) {

  float p1 = Rcpp::as<float>(learn);
  float p2 = Rcpp::as<float>(delta_max);

  float p4 = Rcpp::as<float>(ratio);

  double ret = krui_inv_backwardPass(p1, p2,
                                           err_units, p4,
                                           inputs,
                                           outputs);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_kohonen_SetExtraParameter( SEXP x) {
  int p1 = Rcpp::as<int>(x);
  krui_kohonen_SetExtraParameter(p1);
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_spanning_tree(void) {
  krui_spanning_tree();
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_cc_updatePosOfSpecialUnits(void) {
  krui_cc_updatePosOfSpecialUnits();
  return R_NilValue;
}

RcppExport SEXP SnnsKrui_cc_deleteAllSpecialUnits(void) {
  int err = krui_cc_deleteAllSpecialUnits();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_getErrorCode(void) {
  int ret = krui_getErrorCode();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsKrui_setErrorHandler(void(* error_Handler )(int)) {
  int err = krui_setErrorHandler(error_Handler);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsKrui_execHandler( SEXP error_code) {
  int p1 = Rcpp::as<int>(error_code);
  krui_execHandler(p1);
  return R_NilValue;
}



//----------------------------------------------------
// Bignet
//
//----------------------------------------------------

RcppExport SEXP SnnsBn_jordan_createNet( SEXP IUnits, SEXP HUnits, SEXP OUnits, SEXP ICols, SEXP HCols, SEXP OCols) {

//(int IUnits, int HUnits, int OUnits, int ICols , int HCols , int OCols )

  int p1 = Rcpp::as<int>(IUnits);
  int p2 = Rcpp::as<int>(HUnits);
  int p3 = Rcpp::as<int>(OUnits);
  int p4 = Rcpp::as<int>(ICols);
  int p5 = Rcpp::as<int>(HCols);
  int p6 = Rcpp::as<int>(OCols);

  int err = bn_jordan_createNet(p1, p2, p3, p4, p5, p6);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsBn_elman_createNet( SEXP layer, SEXP columns, SEXP out_context) {

  Rcpp::NumericVector paramsLayer(layer);
  Rcpp::NumericVector paramsColumns(columns);

  int n = paramsLayer.size();

  if(n != paramsColumns.size()) return Rcpp::wrap(-1);

  int p1[n+1];
  int p2[n+1];

  for (int i=0; i<n; i++) {
    p1[i] = static_cast<int>(paramsLayer(i));
  }

  for (int i=0; i<n; i++) {
    p2[i] = static_cast<int>(paramsColumns(i));
  }

//bn_elman_createNet        (int layer[], int columns[], int no_of_layers, bool out_context) ;

  bool p4 = Rcpp::as<bool>(out_context);

  int err = bn_elman_createNet(p1, p2, n, p4);
  return Rcpp::wrap(err);
}


RcppExport SEXP SnnsKr_getSubPatData(SEXP pat_no, SEXP sub_no, SEXP io_type)  {

  int p1 = Rcpp::as<int>(pat_no);
  int p2 = Rcpp::as<int>(sub_no);
  int p3 = Rcpp::as<int>(io_type);

  int size=0;
  float* patternData = kr_getSubPatData(p1, p2, p3, &size);

  Rcpp::NumericVector pattern(size);

  for (int i=0; i<size; i++) {
    pattern[i] = patternData[i];
  }

  return pattern;
}

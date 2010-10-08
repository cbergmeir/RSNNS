
#include <Rcpp.h>

#include <R_ext/Print.h>

//using namespace Rcpp ;

#include "SnnsCLib.h"

RcppExport SEXP SnnsCLib__new(){
 return Rcpp::XPtr<SnnsCLib>( new SnnsCLib, true ) ;
}

RcppExport SEXP SnnsCLib__getNoOfUnits(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);
  int ret = snnsCLib->krui_getNoOfUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getFirstUnit(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);
  int ret = snnsCLib->krui_getFirstUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getNextUnit(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);
  int ret = snnsCLib->krui_getNextUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getCurrentUnit(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);
  int ret = snnsCLib->krui_getCurrentUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setCurrentUnit(SEXP xp, SEXP unit_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int err = snnsCLib->krui_setCurrentUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getUnitName(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  const char* ret = snnsCLib->krui_getUnitName(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitName(SEXP xp, SEXP unit_no, SEXP unit_name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  std::string p2 = Rcpp::as<std::string>( unit_name );  

  int err = snnsCLib->krui_setUnitName(p1, const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__searchUnitName(SEXP xp, SEXP unit_name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( unit_name );

  int ret = snnsCLib->krui_searchUnitName(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__searchNextUnitName(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_searchNextUnitName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getUnitOutFuncName(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  const char* ret = snnsCLib->krui_getUnitOutFuncName(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitOutFunc(SEXP xp, SEXP unit_no, SEXP unitOutFuncName) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  std::string p2 = Rcpp::as<std::string>( unitOutFuncName );  

  int err = snnsCLib->krui_setUnitOutFunc(p1, const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getUnitActFuncName(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  const char* ret = snnsCLib->krui_getUnitActFuncName(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitActFunc(SEXP xp, SEXP unit_no, SEXP unitActFuncName) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  std::string p2 = Rcpp::as<std::string>( unitActFuncName );  

  int err = snnsCLib->krui_setUnitActFunc(p1, const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getUnitFTypeName(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  const char* ret = snnsCLib->krui_getUnitFTypeName(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getUnitActivation(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  float ret = snnsCLib->krui_getUnitActivation(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitActivation(SEXP xp, SEXP UnitNo,
                                     SEXP unit_activation) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  float p2 = Rcpp::as<float>(unit_activation);

  int err = snnsCLib->krui_setUnitActivation(p1, p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getUnitInitialActivation(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  float ret = snnsCLib->krui_getUnitInitialActivation(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitInitialActivation(SEXP xp, SEXP UnitNo,
                                               SEXP unit_i_activation) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  float p2 = Rcpp::as<float>(unit_i_activation);
  snnsCLib->krui_setUnitInitialActivation(p1, p2);
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__getUnitOutput(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  float ret = snnsCLib->krui_getUnitOutput(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitOutput(SEXP xp, SEXP unit_no, SEXP unit_output) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  float p2 = Rcpp::as<float>(unit_output);

  int err = snnsCLib->krui_setUnitOutput(p1, p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getUnitBias(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  float ret = snnsCLib->krui_getUnitBias(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitBias(SEXP xp, SEXP UnitNo, SEXP unit_bias) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  float p2 = Rcpp::as<float>(unit_bias);

  snnsCLib->krui_setUnitBias(p1, p2);
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__getUnitValueA(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  float ret = snnsCLib->krui_getUnitValueA(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitValueA(SEXP xp, SEXP UnitNo, SEXP unit_valueA) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  float p2 = Rcpp::as<float>(unit_valueA);

  snnsCLib->krui_setUnitValueA(p1, p2);
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__getUnitSubnetNo(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  int ret = snnsCLib->krui_getUnitSubnetNo(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitSubnetNo(SEXP xp, SEXP UnitNo, SEXP subnet_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  int p2 = Rcpp::as<int>(subnet_no);
  snnsCLib->krui_setUnitSubnetNo(p1, p2);
  return R_NilValue;
}

 
RcppExport SEXP SnnsCLib__getUnitLayerNo(SEXP xp, SEXP UnitNo) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);

  unsigned short ret = snnsCLib->krui_getUnitLayerNo(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitLayerNo(SEXP xp, SEXP UnitNo, SEXP layer_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);
  int p2 = Rcpp::as<int>(layer_no);
  snnsCLib->krui_setUnitLayerNo(p1, p2);
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__getUnitPosition(SEXP xp, SEXP UnitNo ) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);

  PosType position;

  snnsCLib->krui_getUnitPosition(p1, &position);

  return Rcpp::List::create( 
    	Rcpp::Named( "x" ) = position.x, 
    	Rcpp::Named( "y" ) = position.y,
    	Rcpp::Named( "z" ) = position.z
    	) ;
}



RcppExport SEXP SnnsCLib__setUnitPosition(SEXP xp, SEXP UnitNo, SEXP x, SEXP y, SEXP z) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitNo);

  PosType position;
  position.x = Rcpp::as<short>(x);
  position.y = Rcpp::as<short>(y);
  position.z = Rcpp::as<short>(z);

  snnsCLib->krui_setUnitPosition(p1, &position);
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__getUnitNoAtPosition(SEXP xp, SEXP x, SEXP y, SEXP z,
                                              SEXP subnet_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  PosType position;
  position.x = Rcpp::as<short>(x);
  position.y = Rcpp::as<short>(y);
  position.z = Rcpp::as<short>(z);

  int p2 = Rcpp::as<int>(subnet_no);

  int ret = snnsCLib->krui_getUnitNoAtPosition(&position, p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getUnitNoNearPosition(SEXP xp, SEXP x, SEXP y, SEXP z,
                                                SEXP subnet_no,
                                                SEXP range, SEXP gridWidth) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  PosType position;
  position.x = Rcpp::as<short>(x);
  position.y = Rcpp::as<short>(y);
  position.z = Rcpp::as<short>(z);

  int p2 = Rcpp::as<int>(subnet_no);
  int p3 = Rcpp::as<int>(range);
  int p4 = Rcpp::as<int>(gridWidth);

  int ret = snnsCLib->krui_getUnitNoNearPosition(&position, p2, p3, p4);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getUnitTType(SEXP xp, SEXP unit_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int ret = snnsCLib->krui_getUnitTType(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitTType(SEXP xp, SEXP unit_no, SEXP UnitTType) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(UnitTType);

  int err = snnsCLib->krui_setUnitTType(p1, p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__freezeUnit(SEXP xp, SEXP unit_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int err = snnsCLib->krui_freezeUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__unfreezeUnit(SEXP xp, SEXP unit_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int err = snnsCLib->krui_unfreezeUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__isUnitFrozen(SEXP xp, SEXP unit_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  bool ret = snnsCLib->krui_isUnitFrozen(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getUnitInputType(SEXP xp, SEXP unit_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int ret = snnsCLib->krui_getUnitInputType(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__createDefaultUnit(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_createDefaultUnit();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__createUnit(SEXP xp, SEXP unit_name, SEXP out_func_name,
                               SEXP act_func_name, SEXP i_act,
                               SEXP bias) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( unit_name );  
  std::string p2 = Rcpp::as<std::string>( out_func_name );  
  std::string p3 = Rcpp::as<std::string>( act_func_name );  
  float p4 = Rcpp::as<float>(i_act);
  float p5 = Rcpp::as<float>(bias);

  int ret = snnsCLib->krui_createUnit(const_cast<char*>(p1.c_str()), const_cast<char*>(p2.c_str()), const_cast<char*>(p3.c_str()), p4, p5);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__deleteUnit(SEXP xp, SEXP unit_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int err = snnsCLib->krui_deleteUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__deleteUnitList(SEXP xp, SEXP no_of_units, int unit_list[]) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(no_of_units);

  int err = snnsCLib->krui_deleteUnitList(p1, unit_list);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__createFTypeUnit(SEXP xp, SEXP Ftype_symbol) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( Ftype_symbol );  

  int ret = snnsCLib->krui_createFTypeUnit(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUnitFType(SEXP xp, SEXP unit_no, SEXP Ftype_symbol) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  std::string p2 = Rcpp::as<std::string>( Ftype_symbol );  

  int err = snnsCLib->krui_setUnitFType(p1, const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__copyUnit(SEXP xp, SEXP unit_no, SEXP copy_mode) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(copy_mode);

  int ret = snnsCLib->krui_copyUnit(p1, p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setFirstFTypeEntry(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  bool ret = snnsCLib->krui_setFirstFTypeEntry();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setNextFTypeEntry(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  bool ret = snnsCLib->krui_setNextFTypeEntry();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setFTypeEntry(SEXP xp, SEXP Ftype_symbol) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( Ftype_symbol );  

  bool ret = snnsCLib->krui_setFTypeEntry(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getFTypeName(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getFTypeName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setFTypeName(SEXP xp, SEXP Ftype_symbol) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( Ftype_symbol );  

  int err = snnsCLib->krui_setFTypeName(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getFTypeActFuncName(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getFTypeActFuncName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setFTypeActFunc(SEXP xp, SEXP act_func_name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( act_func_name );  

  int err = snnsCLib->krui_setFTypeActFunc(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__setFTypeOutFunc(SEXP xp, SEXP out_func_name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( out_func_name );  

  int err = snnsCLib->krui_setFTypeOutFunc(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__setFirstFTypeSite(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  bool ret = snnsCLib->krui_setFirstFTypeSite();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setNextFTypeSite(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  bool ret = snnsCLib->krui_setNextFTypeSite();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getFTypeSiteName(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getFTypeSiteName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setFTypeSiteName(SEXP xp, SEXP FType_site_name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( FType_site_name );  

  int err = snnsCLib->krui_setFTypeSiteName(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__createFTypeEntry(SEXP xp, SEXP Ftype_symbol, SEXP act_func_name,
                                           SEXP out_func_name, SEXP no_of_sites,
                                           char **array_of_site_names) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( Ftype_symbol );  
  std::string p2 = Rcpp::as<std::string>( act_func_name );  
  std::string p3 = Rcpp::as<std::string>( out_func_name );  
  int p4 = Rcpp::as<int>(no_of_sites);

  int err = snnsCLib->krui_createFTypeEntry(const_cast<char*>(p1.c_str()), const_cast<char*>(p2.c_str()),
                                           const_cast<char*>(p3.c_str()), p4,
                                           array_of_site_names);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__deleteFTypeEntry(SEXP xp, SEXP Ftype_symbol) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( Ftype_symbol );  

  int err = snnsCLib->krui_deleteFTypeEntry(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getNoOfFunctions(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getNoOfFunctions();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getFuncInfo(SEXP xp, SEXP func_no ) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(func_no);

  char* func_name;
  int func_type;

  snnsCLib->krui_getFuncInfo(p1, &func_name, &func_type);

  return Rcpp::List::create( 
    	Rcpp::Named( "func_name" ) = (const char*) func_name, 
    	Rcpp::Named( "func_type" ) = func_type
    	);
}

RcppExport SEXP SnnsCLib__isFunction(SEXP xp, SEXP func_name, SEXP func_type) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( func_name );  
  int p2 = Rcpp::as<int>(func_type);
  
  bool ret = snnsCLib->krui_isFunction(const_cast<char*>(p1.c_str()), p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getFuncParamInfo(SEXP xp, SEXP func_name, SEXP func_type) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( func_name );  
  int p2 = Rcpp::as<int>(func_type);

  int no_of_input_params=0;
  int no_of_output_params=0;

  bool ret = snnsCLib->krui_getFuncParamInfo(const_cast<char*>(p1.c_str()), p2,
                                       &no_of_input_params,
                                       &no_of_output_params);

  return Rcpp::List::create( 
    	Rcpp::Named( "ret" ) = ret, 
    	Rcpp::Named( "no_of_input_params" ) = no_of_input_params,
    	Rcpp::Named( "no_of_output_params" ) = no_of_output_params
    	) ;
}

RcppExport SEXP SnnsCLib__getFirstSiteTableEntry(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  char* site_name;
  char* site_func;

  bool ret = snnsCLib->krui_getFirstSiteTableEntry(&site_name, &site_func);

  return Rcpp::List::create( 
    	Rcpp::Named( "ret" ) = ret, 
    	Rcpp::Named( "site_name" ) = (const char*) site_name,
    	Rcpp::Named( "site_func" ) = (const char*) site_func
    	) ;
}

RcppExport SEXP SnnsCLib__getNextSiteTableEntry(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  char* site_name;
  char* site_func;

  bool ret = snnsCLib->krui_getNextSiteTableEntry(&site_name, &site_func);

  return Rcpp::List::create( 
    	Rcpp::Named( "ret" ) = ret, 
    	Rcpp::Named( "site_name" ) = (const char*) site_name,
    	Rcpp::Named( "site_func" ) = (const char*) site_func
    	) ;
}

RcppExport SEXP SnnsCLib__getSiteTableFuncName(SEXP xp, SEXP site_name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( site_name );  

  const char* ret = snnsCLib->krui_getSiteTableFuncName(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__createSiteTableEntry(SEXP xp, SEXP site_name, SEXP site_func) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( site_name );  
  std::string p2 = Rcpp::as<std::string>( site_func );  

  int err = snnsCLib->krui_createSiteTableEntry(const_cast<char*>(p1.c_str()), const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__changeSiteTableEntry(SEXP xp, SEXP old_site_name,
                                               SEXP new_site_name,
                                               SEXP new_site_func) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( old_site_name );  
  std::string p2 = Rcpp::as<std::string>( new_site_name );  
  std::string p3 = Rcpp::as<std::string>( new_site_func );  

  int err = snnsCLib->krui_changeSiteTableEntry(const_cast<char*>(p1.c_str()),
                                               const_cast<char*>(p2.c_str()),
                                               const_cast<char*>(p3.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__deleteSiteTableEntry(SEXP xp, SEXP site_name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( site_name );  

  int err = snnsCLib->krui_deleteSiteTableEntry(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__setFirstSite(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  bool ret = snnsCLib->krui_setFirstSite();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setNextSite(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  bool ret = snnsCLib->krui_setNextSite();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setSite(SEXP xp, SEXP site_name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( site_name );  
 
  int err = snnsCLib->krui_setSite(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getSiteValue(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  float ret = snnsCLib->krui_getSiteValue();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getSiteName(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getSiteName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setSiteName(SEXP xp, SEXP site_name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( site_name );  

  int err = snnsCLib->krui_setSiteName(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getSiteFuncName(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getSiteFuncName();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__addSite(SEXP xp, SEXP site_name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( site_name );  

  int err = snnsCLib->krui_addSite(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__deleteSite(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  bool ret = snnsCLib->krui_deleteSite();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getFirstPredUnit(SEXP xp, FlintType *strength) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getFirstPredUnit(strength);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getFirstPredUnitAndData(SEXP xp, FlintType *strength,float *val_a,float *val_b, float *val_c) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getFirstPredUnitAndData(strength, val_a, val_b, val_c);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getNextPredUnit(SEXP xp, FlintType *strength) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getNextPredUnit(strength);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getNextPredUnitAndData(SEXP xp, FlintType *strength,float *val_a,float *val_b, float *val_c) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getNextPredUnitAndData(strength, val_a, val_b, val_c);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getCurrentPredUnit(SEXP xp, FlintType *strength) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getCurrentPredUnit(strength);
  return Rcpp::wrap(ret);
}


RcppExport SEXP SnnsCLib__getFirstSuccUnit(SEXP xp, SEXP source_unit_no, FlintType *weight) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(source_unit_no);
  int ret = snnsCLib->krui_getFirstSuccUnit(p1, weight);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getNextSuccUnit(SEXP xp, FlintType *weight) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getNextSuccUnit(weight);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__areConnected(SEXP xp, SEXP source_unit_no, SEXP target_unit_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(source_unit_no);
  int p2 = Rcpp::as<int>(target_unit_no);

  bool ret = snnsCLib->krui_areConnected(p1, p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__areConnectedWeight(SEXP xp, SEXP source_unit_no, SEXP target_unit_no,
                                             FlintType *weight) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(source_unit_no);
  int p2 = Rcpp::as<int>(target_unit_no);

  bool ret = snnsCLib->krui_areConnectedWeight(p1, p2, weight);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__isConnected(SEXP xp, SEXP source_unit_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(source_unit_no);
  bool ret = snnsCLib->krui_isConnected(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getLinkWeight(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  float ret = snnsCLib->krui_getLinkWeight();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setLinkWeight(SEXP xp, SEXP weight) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  float p1 = Rcpp::as<float>(weight);

  snnsCLib->krui_setLinkWeight(p1);
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__createLink(SEXP xp, SEXP source_unit_no, SEXP weight) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(source_unit_no);
  float p2 = Rcpp::as<float>(weight);

  int err = snnsCLib->krui_createLink(p1, p2);
  return Rcpp::wrap(err);
}

struct Link* SnnsCLib__createLinkWithAdditionalParameters(SEXP xp,
                            SEXP source_unit_no, SEXP weight,
                            SEXP val_a,SEXP val_b,SEXP val_c) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(source_unit_no);
  float p2 = Rcpp::as<float>(weight);
  float p3 = Rcpp::as<float>(val_a);
  float p4 = Rcpp::as<float>(val_b);
  float p5 = Rcpp::as<float>(val_c);

  return snnsCLib->krui_createLinkWithAdditionalParameters( p1, p2, p3, p4, p5);
}

RcppExport SEXP SnnsCLib__deleteLink(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_deleteLink();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__deleteAllInputLinks(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_deleteAllInputLinks();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__deleteAllOutputLinks(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_deleteAllOutputLinks();
  return Rcpp::wrap(err);
}


RcppExport SEXP SnnsCLib__jogWeights(SEXP xp, SEXP minus, SEXP plus) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  float p1 = Rcpp::as<float>(minus);
  float p2 = Rcpp::as<float>(plus);

  snnsCLib->krui_jogWeights(p1, p2);
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__jogCorrWeights(SEXP xp, SEXP minus, SEXP plus,
                                         SEXP mincorr) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  float p1 = Rcpp::as<float>(minus);
  float p2 = Rcpp::as<float>(plus);
  float p3 = Rcpp::as<float>(mincorr);

  int err = snnsCLib->krui_jogCorrWeights(p1, p2, p3);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__updateSingleUnit(SEXP xp, SEXP unit_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int err = snnsCLib->krui_updateSingleUnit(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getUpdateFunc(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getUpdateFunc();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setUpdateFunc(SEXP xp, SEXP update_func) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( update_func );  

  int err = snnsCLib->krui_setUpdateFunc(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__setRemapFunc(SEXP xp, SEXP name, float *params) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( name );  

  int err = snnsCLib->krui_setRemapFunc(const_cast<char*>(p1.c_str()), params);
  return Rcpp::wrap(err);
}

/*RcppExport SEXP SnnsCLib__updateNet(SEXP xp, float *parameterInArray, SEXP NoOfInParams) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = snnsCLib->krui_updateNet(parameterInArray, p2);
  return Rcpp::wrap(err);
}*/

RcppExport SEXP SnnsCLib__updateNet(SEXP xp, SEXP parameterInArray) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  Rcpp::NumericVector params(parameterInArray);

  //TODO: this should be variable length..
  float p1[100];

  int n = params.size();
  for (int i=0; i<n; i++) {
    p1[i] = static_cast<float>(params(i));
  }

  int err = snnsCLib->krui_updateNet(p1, n);
  return Rcpp::wrap(err);
}


RcppExport SEXP SnnsCLib__getInitialisationFunc(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getInitialisationFunc();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setInitialisationFunc(SEXP xp, SEXP initialisation_func) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( initialisation_func );  

  int err = snnsCLib->krui_setInitialisationFunc(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__initializeNet(SEXP xp, SEXP parameterInArray) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p1[n+1];

  for (int i=0; i<n; i++) {
    p1[i] = static_cast<float>(params(i));
  }

  int err = snnsCLib->krui_initializeNet(p1, n);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getLearnFunc(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getLearnFunc();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setLearnFunc(SEXP xp, SEXP learning_func) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( learning_func );

  int err = snnsCLib->krui_setLearnFunc(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__trainNetwork(SEXP xp, NetLearnParameters *parameters) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_trainNetwork(parameters);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getNetworkErrorArray(SEXP xp, double **learnErrors,int **atEpoch,
                                               int *noOfErrors) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_getNetworkErrorArray(learnErrors, atEpoch,
                                               noOfErrors);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__stopTraining(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_stopTraining();
  return Rcpp::wrap(err);
}


RcppExport SEXP SnnsCLib__learnAllPatterns(SEXP xp, SEXP parameterInArray) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p1[n+1];

  for (int i=0; i<n; i++) {
    p1[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = snnsCLib->krui_learnAllPatterns(p1, n, &parameterOutArray, &NoOfOutParams);

  Rcpp::NumericVector out(NoOfOutParams);
  for (int i=0; i<NoOfOutParams; i++) {
    out[i] = parameterOutArray[i];
  }

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "parameterOutArray" ) = out
    	) ;
}

RcppExport SEXP SnnsCLib__testAllPatterns(SEXP xp, SEXP parameterInArray) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p1[n+1];

  for (int i=0; i<n; i++) {
    p1[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = snnsCLib->krui_testAllPatterns(p1, n, &parameterOutArray, &NoOfOutParams);

  Rcpp::NumericVector out(NoOfOutParams);
  for (int i=0; i<NoOfOutParams; i++) {
    out[i] = parameterOutArray[i];
  }

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "parameterOutArray" ) = out
    	) ;
}

/*RcppExport SEXP SnnsCLib__testAllPatterns(SEXP xp, float *parameterInArray, SEXP NoOfInParams,
                                          float **parameterOutArray,
                                          int *NoOfOutParams) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = snnsCLib->krui_testAllPatterns(parameterInArray, p2,
                                          parameterOutArray,
                                          NoOfOutParams);
  return Rcpp::wrap(err);
}*/

RcppExport SEXP SnnsCLib__learnSinglePattern(SEXP xp, SEXP pattern_no, SEXP parameterInArray) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(pattern_no);

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p2[n+1];

  for (int i=0; i<n; i++) {
    p2[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = snnsCLib->krui_learnSinglePattern(p1, p2, n, &parameterOutArray, &NoOfOutParams);

  Rcpp::NumericVector out(NoOfOutParams);
  for (int i=0; i<NoOfOutParams; i++) {
    out[i] = parameterOutArray[i];
  }

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "parameterOutArray" ) = out
    	) ;
}

/*RcppExport SEXP SnnsCLib__learnSinglePattern(SEXP xp, SEXP pattern_no,float *parameterInArray,
                                             SEXP NoOfInParams,
                                             float **parameterOutArray,
                                             int *NoOfOutParams) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(pattern_no);
  int p3 = Rcpp::as<int>(NoOfInParams);

  int err = snnsCLib->krui_learnSinglePattern(p1, parameterInArray,
                                             p3,
                                             parameterOutArray,
                                             NoOfOutParams);
  return Rcpp::wrap(err);
}*/

RcppExport SEXP SnnsCLib__testSinglePattern(SEXP xp, SEXP pattern_no, SEXP parameterInArray) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(pattern_no);

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p2[n+1];

  for (int i=0; i<n; i++) {
    p2[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = snnsCLib->krui_testSinglePattern(p1, p2, n, &parameterOutArray, &NoOfOutParams);

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
RcppExport SEXP SnnsCLib__testSinglePattern(SEXP xp, SEXP pattern_no, float *parameterInArray,
                                            SEXP NoOfInParams,
                                            float **parameterOutArray,
                                            int *NoOfOutParams) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(pattern_no);
  int p3 = Rcpp::as<int>(NoOfInParams);

  int err = snnsCLib->krui_testSinglePattern(p1, parameterInArray,
                                            p3,
                                            parameterOutArray,
                                            NoOfOutParams);
  return Rcpp::wrap(err);
}
*/

RcppExport SEXP SnnsCLib__checkPruning (SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_checkPruning ();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__learnAllPatternsFF(SEXP xp, SEXP parameterInArray) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p1[n+1];

  for (int i=0; i<n; i++) {
    p1[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = snnsCLib->krui_learnAllPatternsFF(p1, n, &parameterOutArray, &NoOfOutParams);

  Rcpp::NumericVector out(NoOfOutParams);
  for (int i=0; i<NoOfOutParams; i++) {
    out[i] = parameterOutArray[i];
  }

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "parameterOutArray" ) = out
    	) ;
}

/*RcppExport SEXP SnnsCLib__learnAllPatternsFF(SEXP xp, float *parameterInArray,
                                             SEXP NoOfInParams,
                                             float **parameterOutArray,
                                             int *NoOfOutParams) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p2 = Rcpp::as<int>(NoOfInParams);

  int err = snnsCLib->krui_learnAllPatternsFF(parameterInArray,
                                             p2,
                                             parameterOutArray,
                                             NoOfOutParams);
  return Rcpp::wrap(err);
}*/

RcppExport SEXP SnnsCLib__learnSinglePatternFF(SEXP xp, SEXP pattern_no, SEXP parameterInArray) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(pattern_no);

  Rcpp::NumericVector params(parameterInArray);

  int n = params.size();
  float p2[n+1];

  for (int i=0; i<n; i++) {
    p2[i] = static_cast<float>(params(i));
  }

  float *parameterOutArray;
  int NoOfOutParams;

  int err = snnsCLib->krui_learnSinglePatternFF(p1, p2, n, &parameterOutArray, &NoOfOutParams);

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
RcppExport SEXP SnnsCLib__learnSinglePatternFF(SEXP xp, SEXP pattern_no,
                                               float *parameterInArray,
                                               SEXP NoOfInParams,
                                               float **parameterOutArray,
                                               int *NoOfOutParams) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(pattern_no);
  int p3 = Rcpp::as<int>(NoOfInParams);

  int err = snnsCLib->krui_learnSinglePatternFF(p1,
                                               parameterInArray,
                                               p3,
                                               parameterOutArray,
                                               NoOfOutParams);
  return Rcpp::wrap(err);
}
*/

RcppExport SEXP SnnsCLib__setClassDistribution(SEXP xp, unsigned int *classDist) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_setClassDistribution(classDist);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__setClassInfo(SEXP xp, SEXP name) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( name );  

  int err = snnsCLib->krui_setClassInfo(const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__useClassDistribution(SEXP xp, SEXP use_it) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<bool>(use_it);

  int err = snnsCLib->krui_useClassDistribution(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getPrunFunc (SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getPrunFunc ();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getFFLearnFunc (SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getFFLearnFunc ();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__setFFLearnFunc (SEXP xp, SEXP FF_learning_func) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( FF_learning_func );  

  int err = snnsCLib->krui_setFFLearnFunc (const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__setPrunFunc (SEXP xp, SEXP pruning_func) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( pruning_func );  

  int err = snnsCLib->krui_setPrunFunc (const_cast<char*>(p1.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__showPattern(SEXP xp, SEXP mode) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(mode);
  int err = snnsCLib->krui_showPattern(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__allocNewPatternSet(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int set_no;
  int err = snnsCLib->krui_allocNewPatternSet(&set_no);

  //return Rcpp::wrap(err);
  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "set_no" ) = set_no
    	) ;
}

RcppExport SEXP SnnsCLib__newPattern(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_newPattern();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__setPatternNo(SEXP xp, SEXP pattern_no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(pattern_no);
  int err = snnsCLib->krui_setPatternNo(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getPatternNo(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_getPatternNo();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__deletePattern(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_deletePattern();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__modifyPattern(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_modifyPattern();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__deleteAllPatterns(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  snnsCLib->krui_deleteAllPatterns();
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__shufflePatterns(SEXP xp, SEXP on_or_off) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<bool>(on_or_off);

  int err = snnsCLib->krui_shufflePatterns(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__shuffleSubPatterns(SEXP xp, SEXP on_or_off) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<bool>(on_or_off);

  int err = snnsCLib->krui_shuffleSubPatterns(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getNoOfPatterns(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getNoOfPatterns();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getTotalNoOfSubPatterns(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getTotalNoOfSubPatterns();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__loadNewPatterns(SEXP xp, SEXP filename) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( filename );  

  int number=0;

  int err = snnsCLib->krui_loadNewPatterns(const_cast<char*>(p1.c_str()), &number);

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "number" ) = number
    	) ;

}

RcppExport SEXP SnnsCLib__setCurrPatSet(SEXP xp, SEXP number) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(number);
  int err = snnsCLib->krui_setCurrPatSet(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__saveNewPatterns(SEXP xp, SEXP filename, SEXP number) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( filename );  
  int p2 = Rcpp::as<int>(number);

  int err = snnsCLib->krui_saveNewPatterns(const_cast<char*>(p1.c_str()), p2);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__deletePatSet(SEXP xp, SEXP number) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(number);
  int err = snnsCLib->krui_deletePatSet(p1);
  return Rcpp::wrap(err);
}

/*RcppExport SEXP SnnsCLib__GetPatInfo(SEXP xp, pattern_set_info *set_info,
                                     pattern_descriptor *pat_info) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_GetPatInfo(set_info,
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

RcppExport SEXP SnnsCLib__GetPatInfo(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  pattern_set_info set_info;
  pattern_descriptor pat_info;

  int err = snnsCLib->krui_GetPatInfo(&set_info, &pat_info);

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

RcppExport SEXP SnnsCLib__DefShowSubPat(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int insize=0;
  int outsize=0;
  int inpos=0;
  int outpos=0;
  int max_n_pos=0;

  int err = snnsCLib->krui_DefShowSubPat(&insize, &outsize, &inpos, &outpos);

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "insize" ) = insize,
    	Rcpp::Named( "outsize" ) = outsize,
    	Rcpp::Named( "inpos" ) = inpos, 
    	Rcpp::Named( "outpos" ) = outpos
    	) ;
}


RcppExport SEXP SnnsCLib__DefTrainSubPat(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int insize=0;
  int outsize=0;
  int instep=0;
  int outstep=0;
  int max_n_pos=0;

  int err = snnsCLib->krui_DefTrainSubPat(&insize, &outsize, &instep, &outstep, &max_n_pos);

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

RcppExport SEXP SnnsCLib__AlignSubPat(SEXP xp, int *inpos, int *outpos, int *no) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_AlignSubPat(inpos, outpos, no);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__GetShapeOfSubPattern(SEXP xp, int *insize, int *outsize,
                                               int *inpos, int *outpos, SEXP n_pos) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p5 = Rcpp::as<int>(n_pos);

  int err = snnsCLib->krui_GetShapeOfSubPattern(insize, outsize,
                                           inpos, outpos, p5);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__saveNet(SEXP xp, SEXP filename, SEXP netname) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( filename );  
  std::string p2 = Rcpp::as<std::string>( netname );  

  int err = snnsCLib->krui_saveNet(const_cast<char*>(p1.c_str()), const_cast<char*>(p2.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__loadNet(SEXP xp, SEXP filename) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( filename );  
  char* netname;

  int err = snnsCLib->krui_loadNet(const_cast<char*>(p1.c_str()), &netname);

  return Rcpp::List::create( 
    	Rcpp::Named( "err" ) = err, 
    	Rcpp::Named( "netname" ) = (const char*) netname
    	) ;

}

RcppExport SEXP SnnsCLib__saveResultParam(SEXP xp, SEXP filename, SEXP create,
                                          SEXP startpattern, SEXP endpattern,
                                          SEXP includeinput, SEXP includeoutput,
                                          float *Update_param_array,
                                          SEXP NoOfUpdateParam) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( filename );  

  int p2 = Rcpp::as<bool>(create);

  int p3 = Rcpp::as<int>(startpattern);
  int p4 = Rcpp::as<int>(endpattern);

  int p5 = Rcpp::as<bool>(includeinput);
  int p6 = Rcpp::as<bool>(includeoutput);

  int p8 = Rcpp::as<int>(NoOfUpdateParam);

  int err = snnsCLib->krui_saveResultParam(const_cast<char*>(p1.c_str()), p2, p3, p4, p5, p6, Update_param_array, p8);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__allocateUnits(SEXP xp, SEXP no_of_units) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(no_of_units);
  int err = snnsCLib->krui_allocateUnits(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__deleteNet(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  snnsCLib->krui_deleteNet();
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__getFirstSymbolTableEntry(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  char* symbol_name;
  int symbol_type;

  bool ret = snnsCLib->krui_getFirstSymbolTableEntry(&symbol_name, &symbol_type);

  return Rcpp::List::create( 
    	Rcpp::Named( "ret" ) = ret, 
    	Rcpp::Named( "symbol_name" ) = (const char*) symbol_name,
    	Rcpp::Named( "symbol_type" ) = symbol_type
    	) ;
}

RcppExport SEXP SnnsCLib__getNextSymbolTableEntry(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  char* symbol_name;
  int symbol_type;

  bool ret = snnsCLib->krui_getNextSymbolTableEntry(&symbol_name, &symbol_type);

  return Rcpp::List::create( 
    	Rcpp::Named( "ret" ) = ret, 
    	Rcpp::Named( "symbol_name" ) = (const char*) symbol_name,
    	Rcpp::Named( "symbol_type" ) = symbol_type
    	) ;
}

RcppExport SEXP SnnsCLib__symbolSearch(SEXP xp, SEXP symbol, SEXP symbol_type) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  std::string p1 = Rcpp::as<std::string>( symbol );  
  int p2 = Rcpp::as<int>(symbol_type);

  bool ret = snnsCLib->krui_symbolSearch(const_cast<char*>(p1.c_str()), p2);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getVersion(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  const char* ret = snnsCLib->krui_getVersion();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getNetInfo(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int no_of_sites=0;
  int no_of_links=0;
  int no_of_STable_entries=0;
  int no_of_FTable_entries=0;

  snnsCLib->krui_getNetInfo(&no_of_sites, &no_of_links, &no_of_STable_entries, &no_of_FTable_entries);

  return Rcpp::List::create( 
    	Rcpp::Named( "no_of_sites" ) = no_of_sites, 
    	Rcpp::Named( "no_of_links" ) = no_of_links,
    	Rcpp::Named( "no_of_STable_entries" ) = no_of_STable_entries,
    	Rcpp::Named( "no_of_FTable_entries" ) = no_of_FTable_entries
    	) ;
}

/*RcppExport SEXP SnnsCLib__getNetInfo(SEXP xp, int *no_of_sites, int *no_of_links,
                                     int *no_of_STable_entries,
                                     int *no_of_FTable_entries) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  snnsCLib->krui_getNetInfo(no_of_sites, no_of_links,
                                     no_of_STable_entries,
                                     no_of_FTable_entries);
  return R_NilValue;
}*/

RcppExport SEXP SnnsCLib__getMemoryManagerInfo(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int unit_bytes=0;
  int site_bytes=0;
  int link_bytes=0;
  int NTable_bytes=0;
  int STable_bytes=0;
  int FTable_bytes=0;

  snnsCLib->krui_getMemoryManagerInfo(&unit_bytes,
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

/*RcppExport SEXP SnnsCLib__getMemoryManagerInfo(SEXP xp, int *unit_bytes,
                            int *site_bytes, int *link_bytes,
                            int *NTable_bytes, int *STable_bytes,
                            int *FTable_bytes) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  snnsCLib->krui_getMemoryManagerInfo(unit_bytes,
                            site_bytes, link_bytes,
                            NTable_bytes, STable_bytes,
                            FTable_bytes);
  return R_NilValue;
}*/

RcppExport SEXP SnnsCLib__getUnitDefaults(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  float act, bias;
  int st, subnet_no, layer_no;
  char *act_func, *out_func;

  snnsCLib->krui_getUnitDefaults(&act, &bias, &st,
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

RcppExport SEXP SnnsCLib__setUnitDefaults(SEXP xp, SEXP act,
                            SEXP bias, SEXP st, SEXP subnet_no,
                            SEXP layer_no, SEXP act_func, SEXP out_func) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  float p1 = Rcpp::as<float>(act);
  float p2 = Rcpp::as<float>(bias);
  int p3 = Rcpp::as<int>(st);
  int p4 = Rcpp::as<int>(subnet_no);
  int p5 = Rcpp::as<int>(layer_no);
  std::string p6 = Rcpp::as<std::string>( act_func );
  std::string p7 = Rcpp::as<std::string>( out_func );  

  int err = snnsCLib->krui_setUnitDefaults(p1, p2, p3, p4, p5, const_cast<char*>(p6.c_str()), const_cast<char*>(p7.c_str()));
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__resetNet(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  snnsCLib->krui_resetNet();
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__setSeedNo(SEXP xp, SEXP seed) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  long int p1 = Rcpp::as<long int>(seed);

  snnsCLib->krui_setSeedNo(p1);
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__getNoOfInputUnits(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getNoOfInputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getNoOfOutputUnits(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getNoOfOutputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getNoOfTTypeUnits(SEXP xp, SEXP UnitTType) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(UnitTType);
  int ret = snnsCLib->krui_getNoOfTTypeUnits(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getNoOfSpecialInputUnits(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getNoOfSpecialInputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getNoOfSpecialOutputUnits(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getNoOfSpecialOutputUnits();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__xyTransTable(SEXP xp, SEXP op, int *x, int *y, SEXP z) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(op);
  int p4 = Rcpp::as<int>(z);

  int err = snnsCLib->krui_xyTransTable(p1, x, y, p4);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getUnitCenters(SEXP xp, SEXP unit_no, SEXP center_no,
                            struct PositionVector **unit_center) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(center_no);

  int err = snnsCLib->krui_getUnitCenters(p1, p2, unit_center);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__setUnitCenters(SEXP xp, SEXP unit_no, SEXP center_no,
                            struct PositionVector *unit_center) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(unit_no);
  int p2 = Rcpp::as<int>(center_no);

  int err = snnsCLib->krui_setUnitCenters(p1, p2, unit_center);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__error(SEXP xp, SEXP error_code) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(error_code);
  const char* ret = snnsCLib->krui_error(p1);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__NA_Error(SEXP xp, SEXP currentPattern, SEXP error_unit,
                                       SEXP error, SEXP ave) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(currentPattern);
  int p2 = Rcpp::as<int>(error_unit);
  int p3 = Rcpp::as<int>(error);
  int p4 = Rcpp::as<bool>(ave);

  float ret = snnsCLib->krui_NA_Error(p1, p2, p3, p4);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__getVariance (SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  float ret = snnsCLib->krui_getVariance ();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__countLinks(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_countLinks();
  return Rcpp::wrap(ret);
}


#ifdef MASPAR_KERNEL

RcppExport SEXP SnnsCLib__MasPar(SEXP xp, mode ) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_MasPar( mode );
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getMasParStatus(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_getMasParStatus();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__MasParBenchmark(SEXP xp, func_type, cycles, result ) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_MasParBenchmark(func_type, cycles, result );
  return Rcpp::wrap(err);
}


#endif

RcppExport SEXP SnnsCLib__setSpecialNetworkType(SEXP xp, SEXP net_type) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(net_type);
  int err = snnsCLib->krui_setSpecialNetworkType(p1);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getSpecialNetworkType(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getSpecialNetworkType();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__initInversion(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_initInversion();
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__inv_forwardPass(SEXP xp, struct UnitList *inputs) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  snnsCLib->krui_inv_forwardPass(inputs);
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__inv_backwardPass(SEXP xp, SEXP learn, SEXP delta_max,
                                           int *err_units, SEXP ratio,
                                           struct UnitList *inputs,
                                           struct UnitList *outputs) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  float p1 = Rcpp::as<float>(learn);
  float p2 = Rcpp::as<float>(delta_max);

  float p4 = Rcpp::as<float>(ratio);

  double ret = snnsCLib->krui_inv_backwardPass(p1, p2,
                                           err_units, p4,
                                           inputs,
                                           outputs);
  return Rcpp::wrap(ret);
}

RcppExport SEXP SnnsCLib__kohonen_SetExtraParameter(SEXP xp, SEXP x) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(x);
  snnsCLib->krui_kohonen_SetExtraParameter(p1);
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__spanning_tree(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  snnsCLib->krui_spanning_tree();
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__cc_updatePosOfSpecialUnits(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  snnsCLib->krui_cc_updatePosOfSpecialUnits();
  return R_NilValue;
}

RcppExport SEXP SnnsCLib__cc_deleteAllSpecialUnits(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_cc_deleteAllSpecialUnits();
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__getErrorCode(SEXP xp) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int ret = snnsCLib->krui_getErrorCode();
  return Rcpp::wrap(ret);
}

/*
RcppExport SEXP SnnsCLib__setErrorHandler(void(* error_Handler )(int)) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int err = snnsCLib->krui_setErrorHandler(error_Handler);
  return Rcpp::wrap(err);
}
*/

RcppExport SEXP SnnsCLib__execHandler(SEXP xp, SEXP error_code) {
 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(error_code);
  snnsCLib->krui_execHandler(p1);
  return R_NilValue;
}



//----------------------------------------------------
// Bignet
//
//----------------------------------------------------

RcppExport SEXP SnnsCLib__jordan_createNet(SEXP xp, SEXP IUnits, SEXP HUnits, SEXP OUnits, SEXP ICols, SEXP HCols, SEXP OCols) {

//(int IUnits, int HUnits, int OUnits, int ICols , int HCols , int OCols )

 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(IUnits);
  int p2 = Rcpp::as<int>(HUnits);
  int p3 = Rcpp::as<int>(OUnits);
  int p4 = Rcpp::as<int>(ICols);
  int p5 = Rcpp::as<int>(HCols);
  int p6 = Rcpp::as<int>(OCols);

  int err = snnsCLib->bn_jordan_createNet(p1, p2, p3, p4, p5, p6);
  return Rcpp::wrap(err);
}

RcppExport SEXP SnnsCLib__elman_createNet(SEXP xp, SEXP layer, SEXP columns, SEXP out_context) {

 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

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

  int err = snnsCLib->bn_elman_createNet(p1, p2, n, p4);
  return Rcpp::wrap(err);
}


RcppExport SEXP SnnsCLib__art1_createNet (SEXP xp, SEXP IUnits, SEXP IRow, SEXP CUnits, SEXP CRos) {

//krui_err bn_art1_createNet (int IUnits, int IRow, int CUnits, int CRos);

 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(IUnits);
  int p2 = Rcpp::as<int>(IRow);
  int p3 = Rcpp::as<int>(CUnits);
  int p4 = Rcpp::as<int>(CRos);

  int err = snnsCLib->bn_art1_createNet(p1, p2, p3, p4);
  return Rcpp::wrap(err);
}


RcppExport SEXP SnnsCLib__getSubPatData(SEXP xp, SEXP pat_no, SEXP sub_no, SEXP io_type)  {

 Rcpp::XPtr<SnnsCLib> snnsCLib(xp);

  int p1 = Rcpp::as<int>(pat_no);
  int p2 = Rcpp::as<int>(sub_no);
  int p3 = Rcpp::as<int>(io_type);

  int size=0;
  float* patternData = snnsCLib->kr_getSubPatData(p1, p2, p3, &size);

  Rcpp::NumericVector pattern(size);

  for (int i=0; i<size; i++) {
    pattern[i] = patternData[i];
  }

  return pattern;
}


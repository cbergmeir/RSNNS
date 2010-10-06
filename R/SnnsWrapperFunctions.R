
#int             getNoOfUnits(void);
SnnsKrui_getNoOfUnits <- function() {
  return(.Call("SnnsKrui_getNoOfUnits", package="RSNNS"))  
}

#int             getFirstUnit(void);
SnnsKrui_getFirstUnit <- function() {
  return(.Call("SnnsKrui_getFirstUnit", package="RSNNS"))  
}

#int             getNextUnit(void);
SnnsKrui_getNextUnit <- function() {
  return(.Call("SnnsKrui_getNextUnit", package="RSNNS"))  
}

#int             getCurrentUnit(void);
SnnsKrui_getCurrentUnit <- function() {
  return(.Call("SnnsKrui_getCurrentUnit", package="RSNNS"))  
}

#krui_err        setCurrentUnit(int unit_no);
SnnsKrui_setCurrentUnit <- function(unit_no) {
  err <- .Call("SnnsKrui_setCurrentUnit", unit_no, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setCurrentUnit", err)
}

#char           *getUnitName(int UnitNo);
SnnsKrui_getUnitName <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitName", UnitNo, package="RSNNS"))  
}

#krui_err        setUnitName(int unit_no, char *unit_name);
SnnsKrui_setUnitName <- function(unit_no, unit_name) {
  err <- .Call("SnnsKrui_setUnitName", unit_no, unit_name, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setUnitName", err)
}

#int             searchUnitName(char *unit_name);
SnnsKrui_searchUnitName <- function(unit_name) {
  return(.Call("SnnsKrui_searchUnitName", unit_name, package="RSNNS"))  
}

#int             searchNextUnitName(void);
SnnsKrui_searchNextUnitName <- function() {
  return(.Call("SnnsKrui_searchNextUnitName", package="RSNNS"))  
}

#char           *getUnitOutFuncName(int UnitNo);
SnnsKrui_getUnitOutFuncName <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitOutFuncName", UnitNo, package="RSNNS"))  
}

#krui_err        setUnitOutFunc(int unit_no, char *unitOutFuncName);
SnnsKrui_setUnitOutFunc <- function(unit_no, unitOutFuncName) {
  err <- .Call("SnnsKrui_setUnitOutFunc", unit_no, unitOutFuncName, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setUnitOutFunc", err)
}

#char           *getUnitActFuncName(int UnitNo);
SnnsKrui_getUnitActFuncName <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitActFuncName", UnitNo, package="RSNNS"))  
}

#krui_err        setUnitActFunc(int unit_no, char *unitActFuncName);
SnnsKrui_setUnitActFunc <- function(unit_no, unitActFuncName) {
  err <- .Call("SnnsKrui_setUnitActFunc", unit_no, unitActFuncName, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setUnitActFunc", err)
}

#char           *getUnitFTypeName(int UnitNo);
SnnsKrui_getUnitFTypeName <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitFTypeName", UnitNo, package="RSNNS"))  
}

#FlintType       getUnitActivation(int UnitNo);
SnnsKrui_getUnitActivation <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitActivation", UnitNo, package="RSNNS"))  
}

#krui_err        setUnitActivation(int UnitNo, FlintTypeParam unit_activation);
SnnsKrui_setUnitActivation <- function(UnitNo, unit_activation) {
  err <- .Call("SnnsKrui_setUnitActivation", UnitNo, unit_activation, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setUnitActivation", err)
}

#FlintType       getUnitInitialActivation(int UnitNo);
SnnsKrui_getUnitInitialActivation <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitInitialActivation", UnitNo, package="RSNNS"))  
}

#void            setUnitInitialActivation(int UnitNo, FlintTypeParam unit_i_activation);
SnnsKrui_setUnitInitialActivation <- function(UnitNo, unit_i_activation) {
  return(.Call("SnnsKrui_setUnitInitialActivation", UnitNo, unit_i_activation, package="RSNNS"))  
}

#FlintType       getUnitOutput(int UnitNo);
SnnsKrui_getUnitOutput <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitOutput", UnitNo, package="RSNNS"))  
}

#krui_err        setUnitOutput(int unit_no, FlintTypeParam unit_output);
SnnsKrui_setUnitOutput <- function(unit_no, unit_output) {
  err <- .Call("SnnsKrui_setUnitOutput", unit_no, unit_output, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setUnitOutput", err)
}

#FlintType       getUnitBias(int UnitNo);
SnnsKrui_getUnitBias <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitBias", UnitNo, package="RSNNS"))  
}

#void            setUnitBias(int UnitNo, FlintTypeParam unit_bias);
SnnsKrui_setUnitBias <- function(UnitNo, unit_bias) {
  return(.Call("SnnsKrui_setUnitBias", UnitNo, unit_bias, package="RSNNS"))  
}

#FlintType       getUnitValueA(int UnitNo);
SnnsKrui_getUnitValueA <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitValueA", UnitNo, package="RSNNS"))  
}

#void            setUnitValueA(int UnitNo, FlintTypeParam unit_valueA);
SnnsKrui_setUnitValueA <- function(UnitNo, unit_valueA) {
  return(.Call("SnnsKrui_setUnitValueA", UnitNo, unit_valueA, package="RSNNS"))  
}

#int             getUnitSubnetNo(int UnitNo);
SnnsKrui_getUnitSubnetNo <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitSubnetNo", UnitNo, package="RSNNS"))  
}

#void            setUnitSubnetNo(int UnitNo, int subnet_no);
SnnsKrui_setUnitSubnetNo <- function(UnitNo, subnet_no) {
  return(.Call("SnnsKrui_setUnitSubnetNo", UnitNo, subnet_no, package="RSNNS"))
}

#unsigned short  getUnitLayerNo(int UnitNo);
SnnsKrui_getUnitLayerNo <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitLayerNo", UnitNo, package="RSNNS"))  
}

#void            setUnitLayerNo(int UnitNo, int layer_no);
SnnsKrui_setUnitLayerNo <- function(UnitNo, layer_no) {
  return(.Call("SnnsKrui_setUnitLayerNo", UnitNo, layer_no, package="RSNNS"))
}

#void            getUnitPosition(int UnitNo, struct PosType *position);
SnnsKrui_getUnitPosition <- function(UnitNo) {
  return(.Call("SnnsKrui_getUnitPosition", UnitNo, package="RSNNS"))  
}

#void            setUnitPosition(int UnitNo, struct PosType *position);
SnnsKrui_setUnitPosition <- function(UnitNo, x, y, z) {
  return(.Call("SnnsKrui_setUnitPosition", UnitNo, x, y, z, package="RSNNS"))
}

#int             getUnitNoAtPosition(struct PosType *position, int subnet_no);
SnnsKrui_getUnitNoAtPosition <- function(x, y, z, subnet_no) {
  return(.Call("SnnsKrui_getUnitNoAtPosition", x, y, z, subnet_no, package="RSNNS"))  
}

#int             getUnitNoNearPosition(struct PosType *position, int subnet_no, int range, int gridWidth);
SnnsKrui_getUnitNoNearPosition <- function(x, y, z, subnet_no, range, gridWidth) {
  return(.Call("SnnsKrui_getUnitNoNearPosition", x, y, z, subnet_no, range, gridWidth, package="RSNNS"))  
}

#int             getUnitTType(int unit_no);
SnnsKrui_getUnitTType <- function(unit_no) {
  return(.Call("SnnsKrui_getUnitTType", unit_no, package="RSNNS"))  
}

#krui_err        setUnitTType(int unit_no, int UnitTType);
SnnsKrui_setUnitTType <- function(unit_no, UnitTType) {
  err <- .Call("SnnsKrui_setUnitTType", unit_no, UnitTType, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setUnitTType", err)
}

#krui_err        freezeUnit(int unit_no);
SnnsKrui_freezeUnit <- function(unit_no) {
  err <- .Call("SnnsKrui_freezeUnit", unit_no, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("freezeUnit", err)
}

#krui_err        unfreezeUnit(int unit_no);
SnnsKrui_unfreezeUnit <- function(unit_no) {
  err <- .Call("SnnsKrui_unfreezeUnit", unit_no, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("unfreezeUnit", err)
}

#bool            isUnitFrozen(int unit_no);
SnnsKrui_isUnitFrozen <- function(unit_no) {
  return(.Call("SnnsKrui_isUnitFrozen", unit_no, package="RSNNS"))  
}

#int             getUnitInputType(int unit_no);
SnnsKrui_getUnitInputType <- function(unit_no) {
  return(.Call("SnnsKrui_getUnitInputType", unit_no, package="RSNNS"))  
}

#int             createDefaultUnit(void);
SnnsKrui_createDefaultUnit <- function() {
  return(.Call("SnnsKrui_createDefaultUnit", package="RSNNS"))  
}

#int             createUnit(char *unit_name, char *out_func_name, char *act_func_name, FlintTypeParam i_act,
#    FlintTypeParam bias);
SnnsKrui_createUnit <- function(unit_name, out_func_name, act_func_name, i_act, bias) {
  return(.Call("SnnsKrui_createUnit", unit_name, out_func_name, act_func_name, i_act, bias, package="RSNNS"))  
}

#krui_err        deleteUnit(int unit_no);
SnnsKrui_deleteUnit <- function(unit_no) {
  err <- .Call("SnnsKrui_deleteUnit", unit_no, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("deleteUnit", err)
}

#krui_err        deleteUnitList(int no_of_units, int unit_list[]);
#int             createFTypeUnit(char *Ftype_symbol);
SnnsKrui_createFTypeUnit <- function(Ftype_symbol) {
  return(.Call("SnnsKrui_createFTypeUnit", Ftype_symbol, package="RSNNS"))  
}

#krui_err        setUnitFType(int unit_no, char *Ftype_symbol);
SnnsKrui_setUnitFType <- function(unit_no, Ftype_symbol) {
  err <- .Call("SnnsKrui_setUnitFType", unit_no, Ftype_symbol, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setUnitFType", err)
}

#int             copyUnit(int unit_no, int copy_mode);
SnnsKrui_copyUnit <- function(unit_no, copy_mode) {
  return(.Call("SnnsKrui_copyUnit", unit_no, copy_mode, package="RSNNS"))  
}

#bool            setFirstFTypeEntry(void);
SnnsKrui_setFirstFTypeEntry <- function() {
  return(.Call("SnnsKrui_setFirstFTypeEntry", package="RSNNS"))  
}

#bool            setNextFTypeEntry(void);
SnnsKrui_setNextFTypeEntry <- function() {
  return(.Call("SnnsKrui_setNextFTypeEntry", package="RSNNS"))  
}

#bool            setFTypeEntry(char *Ftype_symbol);
SnnsKrui_setFTypeEntry <- function(Ftype_symbol) {
  return(.Call("SnnsKrui_setFTypeEntry", Ftype_symbol, package="RSNNS"))  
}

#char           *getFTypeName(void);
SnnsKrui_getFTypeName <- function() {
  return(.Call("SnnsKrui_getFTypeName", package="RSNNS"))  
}

#krui_err        setFTypeName(char *Ftype_symbol);
SnnsKrui_setFTypeName <- function(Ftype_symbol) {
  err <- .Call("SnnsKrui_setFTypeName", Ftype_symbol, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setFTypeName", err)
}

#char           *getFTypeActFuncName(void);
SnnsKrui_getFTypeActFuncName <- function() {
  return(.Call("SnnsKrui_getFTypeActFuncName", package="RSNNS"))  
}

#krui_err        setFTypeActFunc(char *act_func_name);
SnnsKrui_setFTypeActFunc <- function(act_func_name) {
  err <- .Call("SnnsKrui_setFTypeActFunc", act_func_name, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setFTypeActFunc", err)
}

#krui_err        setFTypeOutFunc(char *out_func_name);
SnnsKrui_setFTypeOutFunc <- function(out_func_name) {
  err <- .Call("SnnsKrui_setFTypeOutFunc", out_func_name, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setFTypeOutFunc", err)
}

#bool            setFirstFTypeSite(void);
SnnsKrui_setFirstFTypeSite <- function() {
  return(.Call("SnnsKrui_setFirstFTypeSite", package="RSNNS"))  
}

#bool            setNextFTypeSite(void);
SnnsKrui_setNextFTypeSite <- function() {
  return(.Call("SnnsKrui_setNextFTypeSite", package="RSNNS"))  
}

#char           *getFTypeSiteName(void);
SnnsKrui_getFTypeSiteName <- function() {
  return(.Call("SnnsKrui_getFTypeSiteName", package="RSNNS"))  
}

#krui_err        setFTypeSiteName(char *FType_site_name);
SnnsKrui_setFTypeSiteName <- function(FType_site_name) {
  err <- .Call("SnnsKrui_setFTypeSiteName", FType_site_name, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setFTypeSiteName", err)
}

#krui_err        createFTypeEntry(char *Ftype_symbol, char *act_func_name,
#    char *out_func_name, int no_of_sites,
#    char **array_of_site_names);
#krui_err        deleteFTypeEntry(char *Ftype_symbol);
SnnsKrui_deleteFTypeEntry <- function(Ftype_symbol) {
  err <- .Call("SnnsKrui_deleteFTypeEntry", Ftype_symbol, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("deleteFTypeEntry", err)
}

#int             getNoOfFunctions(void);
SnnsKrui_getNoOfFunctions <- function() {
  return(.Call("SnnsKrui_getNoOfFunctions", package="RSNNS"))  
}

#void            getFuncInfo(int func_no, char **func_name,int *func_type);
SnnsKrui_getFuncInfo <- function(func_no) {
  return(.Call("SnnsKrui_getFuncInfo", func_no, package="RSNNS"))  
}

#bool            isFunction(char *func_name, int func_type);
SnnsKrui_isFunction <- function(func_name, func_type) {
  return(.Call("SnnsKrui_isFunction", func_name, func_type, package="RSNNS"))  
}

#bool            getFuncParamInfo(char *func_name, int func_type, int *no_of_input_params, int *no_of_output_params);
SnnsKrui_getFuncParamInfo <- function(func_name, func_type) {
  res <- .Call("SnnsKrui_getFuncParamInfo", func_name, func_type, package="RSNNS")
  if(!res$ret) SnnsR_showWarningFromSnnsError("getFuncParamInfo", res$ret)
  return(res)
}

#bool            getFirstSiteTableEntry(char **site_name,char **site_func);
SnnsKrui_getFirstSiteTableEntry <- function() {
  res <- .Call("SnnsKrui_getFirstSiteTableEntry", package="RSNNS")
  if(!res$ret) SnnsR_showWarningFromSnnsError("getFirstSiteTableEntry", res$ret)
  return(res)
}

#bool            getNextSiteTableEntry(char **site_name, char **site_func);
SnnsKrui_getNextSiteTableEntry <- function() {
  res <- .Call("SnnsKrui_getNextSiteTableEntry", package="RSNNS")
  if(!res$ret) SnnsR_showWarningFromSnnsError("getNextSiteTableEntry", res$ret)
  return(res)
}

#char           *getSiteTableFuncName(char *site_name);
SnnsKrui_getSiteTableFuncName <- function(site_name) {
  return(.Call("SnnsKrui_getSiteTableFuncName", site_name, package="RSNNS"))  
}

#krui_err        createSiteTableEntry(char *site_name, char *site_func);
SnnsKrui_createSiteTableEntry <- function(site_name, site_func) {
  err <- .Call("SnnsKrui_createSiteTableEntry", site_name, site_func, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("createSiteTableEntry", err)
}

#krui_err        changeSiteTableEntry(char *old_site_name, char *new_site_name, char *new_site_func);
SnnsKrui_changeSiteTableEntry <- function(old_site_name, new_site_name, new_site_func) {
  err <- .Call("SnnsKrui_changeSiteTableEntry", old_site_name, new_site_name, new_site_func, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("changeSiteTableEntry", err)
}

#krui_err        deleteSiteTableEntry(char *site_name);
SnnsKrui_deleteSiteTableEntry <- function(site_name) {
  err <- .Call("SnnsKrui_deleteSiteTableEntry", site_name, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("deleteSiteTableEntry", err)
}

#bool            setFirstSite(void);
SnnsKrui_setFirstSite <- function() {
  return(.Call("SnnsKrui_setFirstSite", package="RSNNS"))  
}

#bool            setNextSite(void);
SnnsKrui_setNextSite <- function() {
  return(.Call("SnnsKrui_setNextSite", package="RSNNS"))  
}

#krui_err        setSite(char *site_name);
SnnsKrui_setSite <- function(site_name) {
  err <- .Call("SnnsKrui_setSite", site_name, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setSite", err)
}

#FlintType       getSiteValue(void);
SnnsKrui_getSiteValue <- function() {
  return(.Call("SnnsKrui_getSiteValue", package="RSNNS"))  
}

#char           *getSiteName(void);
SnnsKrui_getSiteName <- function() {
  return(.Call("SnnsKrui_getSiteName", package="RSNNS"))  
}

#krui_err        setSiteName(char *site_name);
SnnsKrui_setSiteName <- function(site_name) {
  err <- .Call("SnnsKrui_setSiteName", site_name, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setSiteName", err)
}

#char           *getSiteFuncName(void);
SnnsKrui_getSiteFuncName <- function() {
  return(.Call("SnnsKrui_getSiteFuncName", package="RSNNS"))  
}

#krui_err        addSite(char *site_name);
SnnsKrui_addSite <- function(site_name) {
  err <- .Call("SnnsKrui_addSite", site_name, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("addSite", err)
}

#bool            deleteSite(void);
SnnsKrui_deleteSite <- function() {
  return(.Call("SnnsKrui_deleteSite", package="RSNNS"))  
}

#int             getFirstPredUnit(FlintType *strength);
#int             getFirstPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c);
#int             getNextPredUnit(FlintType *strength);
#int             getNextPredUnitAndData(FlintType *strength,float *val_a,float *val_b, float *val_c);
#int             getCurrentPredUnit(FlintType *strength);
#
#int             getFirstSuccUnit(int source_unit_no, FlintType *weight);
#int             getNextSuccUnit(FlintType *weight);
#bool            areConnected(int source_unit_no, int target_unit_no);
SnnsKrui_areConnected <- function(source_unit_no, target_unit_no) {
  return(.Call("SnnsKrui_areConnected", source_unit_no, target_unit_no, package="RSNNS"))  
}

#bool            areConnectedWeight(int source_unit_no,int target_unit_no,
#    FlintType *weight);
#bool            isConnected(int source_unit_no);
SnnsKrui_isConnected <- function(source_unit_no) {
  return(.Call("SnnsKrui_isConnected", source_unit_no, package="RSNNS"))  
}

#FlintType       getLinkWeight(void);
SnnsKrui_getLinkWeight <- function() {
  return(.Call("SnnsKrui_getLinkWeight", package="RSNNS"))  
}

#void            setLinkWeight(FlintTypeParam weight);
SnnsKrui_setLinkWeight <- function(weight) {
  return(.Call("SnnsKrui_setLinkWeight", weight, package="RSNNS"))  
}

#krui_err        createLink(int source_unit_no, FlintTypeParam weight);
SnnsKrui_createLink <- function(source_unit_no, weight) {
  err <- .Call("SnnsKrui_createLink", source_unit_no, weight, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("createLink", err)
}

#struct Link*    createLinkWithAdditionalParameters(
#    int source_unit_no, FlintTypeParam weight,
#    float val_a,float val_b,float val_c);
#krui_err        deleteLink(void);
SnnsKrui_deleteLink <- function() {
  err <- .Call("SnnsKrui_deleteLink", package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("deleteLink", err)
}

#krui_err        deleteAllInputLinks(void);
SnnsKrui_deleteAllInputLinks <- function() {
  err <- .Call("SnnsKrui_deleteAllInputLinks", package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("deleteAllInputLinks", err)
}

#krui_err        deleteAllOutputLinks(void);
SnnsKrui_deleteAllOutputLinks <- function() {
  err <- .Call("SnnsKrui_deleteAllOutputLinks", package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("deleteAllOutputLinks", err)
}


#void            jogWeights(FlintTypeParam minus, FlintTypeParam plus);
SnnsKrui_jogWeights <- function(minus, plus) {
  return(.Call("SnnsKrui_jogWeights", minus, plus, package="RSNNS"))  
}

#krui_err        jogCorrWeights(FlintTypeParam minus, FlintTypeParam plus, FlintTypeParam mincorr);
SnnsKrui_jogCorrWeights <- function(minus, plus, mincorr) {
  err <- .Call("SnnsKrui_jogCorrWeights", minus, plus, mincorr, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("jogCorrWeights", err)
}

#krui_err        updateSingleUnit(int unit_no);
SnnsKrui_updateSingleUnit <- function(unit_no) {
  err <- .Call("SnnsKrui_updateSingleUnit", unit_no, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("updateSingleUnit", err)
}

#char           *getUpdateFunc(void);
SnnsKrui_getUpdateFunc <- function() {
  return(.Call("SnnsKrui_getUpdateFunc", package="RSNNS"))  
}

#krui_err        setUpdateFunc(char *update_func);
SnnsKrui_setUpdateFunc <- function(update_func) {
  err <- .Call("SnnsKrui_setUpdateFunc", update_func, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setUpdateFunc", err)
}

#krui_err        setRemapFunc(char *name, float *params);
#krui_err        updateNet(float *parameterInArray, int NoOfInParams);
SnnsKrui_updateNet <- function(parameterInArray) {
  err <- .Call("SnnsKrui_updateNet", parameterInArray, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("updateNet", err)
}

#char           *getInitialisationFunc(void);
SnnsKrui_getInitialisationFunc <- function() {
  return(.Call("SnnsKrui_getInitialisationFunc", package="RSNNS"))  
}

#krui_err        setInitialisationFunc(char *initialisation_func);
SnnsKrui_setInitialisationFunc <- function(initialisation_func) {
  err <- .Call("SnnsKrui_setInitialisationFunc", initialisation_func, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setInitialisationFunc", err)
}

#krui_err        initializeNet(float *parameterInArray, int NoOfInParams);
SnnsKrui_initializeNet <- function(parameterInArray) {
  err <- .Call("SnnsKrui_initializeNet", parameterInArray, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("initializeNet", err)
}

#char           *getLearnFunc(void);
SnnsKrui_getLearnFunc <- function() {
  return(.Call("SnnsKrui_getLearnFunc", package="RSNNS"))  
}

#krui_err        setLearnFunc(char *learning_func);
SnnsKrui_setLearnFunc <- function(learning_func) {
  err <- .Call("SnnsKrui_setLearnFunc", learning_func, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setLearnFunc", err)
}

#krui_err        trainNetwork(NetLearnParameters *parameters);
#krui_err        getNetworkErrorArray(double **learnErrors,int **atEpoch,
#    int *noOfErrors);
#krui_err        stopTraining(void);
SnnsKrui_stopTraining <- function() {
  err <- .Call("SnnsKrui_stopTraining", package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("stopTraining", err)
}

#krui_err        learnAllPatterns(float *parameterInArray,int NoOfInParams,
#    float **parameterOutArray,
#    int *NoOfOutParams);
SnnsKrui_learnAllPatterns <- function(parameterInArray) {
  res <- .Call("SnnsKrui_learnAllPatterns", parameterInArray, package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("learnAllPatterns", res$err)
  return(res)
}

#krui_err        testAllPatterns(float *parameterInArray, int NoOfInParams,
#    float **parameterOutArray,
#    int *NoOfOutParams);
SnnsKrui_testAllPatterns <- function(parameterInArray) {
  res <- .Call("SnnsKrui_testAllPatterns", parameterInArray, package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("testAllPatterns", res$err)
  return(res)
}

#krui_err        learnSinglePattern(int pattern_no,float *parameterInArray,
#    int NoOfInParams,
#    float **parameterOutArray,
#    int *NoOfOutParams);
SnnsKrui_learnSinglePattern <- function(pattern_no, parameterInArray) {
  res <- .Call("SnnsKrui_learnSinglePattern", pattern_no, parameterInArray, package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("learnSinglePattern", res$err)
  return(res)
}

#krui_err        testSinglePattern(int pattern_no, float *parameterInArray,
#    int NoOfInParams,
#    float **parameterOutArray,
#    int *NoOfOutParams);
SnnsKrui_testSinglePattern <- function(pattern_no, parameterInArray) {
  res <- .Call("SnnsKrui_testSinglePattern", pattern_no, parameterInArray, package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("testSinglePattern", res$err)
  return(res)
}

#int             checkPruning (void);
SnnsKrui_checkPruning <- function() {
  return(.Call("SnnsKrui_checkPruning", package="RSNNS"))  
}

#krui_err        learnAllPatternsFF(float *parameterInArray,
#    int NoOfInParams,
#    float **parameterOutArray,
#    int *NoOfOutParams);
SnnsKrui_learnAllPatternsFF <- function(parameterInArray) {
  res <- .Call("SnnsKrui_learnAllPatternsFF", parameterInArray, package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("learnAllPatternsFF", res$err)
  return(res)
}

#krui_err        learnSinglePatternFF(int pattern_no,
#    float *parameterInArray,
#    int NoOfInParams,
#    float **parameterOutArray,
#    int *NoOfOutParams);
SnnsKrui_learnSinglePatternFF <- function(pattern_no, parameterInArray) {
  res <- .Call("SnnsKrui_learnSinglePatternFF", pattern_no, parameterInArray, package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("learnSinglePatternFF", res$err)
  return(res)
}

#krui_err        setClassDistribution(unsigned int *classDist);
#krui_err        setClassInfo(char *name);
SnnsKrui_setClassInfo <- function(name) {
  err <- .Call("SnnsKrui_setClassInfo", name, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setClassInfo", err)
}

#krui_err        useClassDistribution(bool use_it);
SnnsKrui_useClassDistribution <- function(use_it) {
  err <- .Call("SnnsKrui_useClassDistribution", use_it, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("useClassDistribution", err)
}

#char           *getPrunFunc (void);
SnnsKrui_getPrunFunc <- function() {
  return(.Call("SnnsKrui_getPrunFunc", package="RSNNS"))  
}

#char           *getFFLearnFunc (void);
SnnsKrui_getFFLearnFunc <- function() {
  return(.Call("SnnsKrui_getFFLearnFunc", package="RSNNS"))  
}

#krui_err        setFFLearnFunc (char *FF_learning_func);
SnnsKrui_setFFLearnFunc <- function(FF_learning_func) {
  err <- .Call("SnnsKrui_setFFLearnFunc", FF_learning_func, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setFFLearnFunc", err)
}

#krui_err        setPrunFunc (char *pruning_func);
SnnsKrui_setPrunFunc <- function(pruning_func) {
  err <- .Call("SnnsKrui_setPrunFunc", pruning_func, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setPrunFunc", err)
}

#krui_err        showPattern(int mode);
SnnsKrui_showPattern <- function(mode) {
  err <- .Call("SnnsKrui_showPattern", mode, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("showPattern", err)
}

#krui_err        allocNewPatternSet(int *set_no);
SnnsKrui_allocNewPatternSet <- function() {
  res <- .Call("SnnsKrui_allocNewPatternSet", package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("allocNewPatternSet", res$err)
  return(res$set_no)
}

#krui_err        newPattern(void);
SnnsKrui_newPattern <- function() {
  err <- .Call("SnnsKrui_newPattern", package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("newPattern", err)
}

#krui_err        setPatternNo(int pattern_no);
SnnsKrui_setPatternNo <- function(pattern_no) {
  err <- .Call("SnnsKrui_setPatternNo", pattern_no, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setPatternNo", err)
}

##TODO: how is this function working?!
#krui_err        getPatternNo(void);
SnnsKrui_getPatternNo <- function() {
  err <- .Call("SnnsKrui_getPatternNo", package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("getPatternNo", err)
}

#krui_err        deletePattern(void);
SnnsKrui_deletePattern <- function() {
  err <- .Call("SnnsKrui_deletePattern", package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("deletePattern", err)
}

#krui_err        modifyPattern(void);
SnnsKrui_modifyPattern <- function() {
  err <- .Call("SnnsKrui_modifyPattern", package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("modifyPattern", err)
}

#void            deleteAllPatterns(void);
SnnsKrui_deleteAllPatterns <- function() {
  return(.Call("SnnsKrui_deleteAllPatterns", package="RSNNS"))
}

#krui_err        shufflePatterns(bool on_or_off);
SnnsKrui_shufflePatterns <- function(on_or_off) {
  err <- .Call("SnnsKrui_shufflePatterns", on_or_off, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("shufflePatterns", err)
}

#krui_err        shuffleSubPatterns(bool on_or_off);
SnnsKrui_shuffleSubPatterns <- function(on_or_off) {
  err <- .Call("SnnsKrui_shuffleSubPatterns", on_or_off, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("shuffleSubPatterns", err)
}

#int             getNoOfPatterns(void);
SnnsKrui_getNoOfPatterns <- function() {
  return(.Call("SnnsKrui_getNoOfPatterns", package="RSNNS"))
}

#int             getTotalNoOfSubPatterns(void);
SnnsKrui_getTotalNoOfSubPatterns <- function() {
  return(.Call("SnnsKrui_getTotalNoOfSubPatterns", package="RSNNS"))
}

#krui_err        loadNewPatterns(char *filename, int *number);
SnnsKrui_loadNewPatterns <- function(filename) {
  res <- .Call("SnnsKrui_loadNewPatterns", filename, package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("loadNewPatterns", res$err)
  return(res)
}

#krui_err        setCurrPatSet(int number);
SnnsKrui_setCurrPatSet <- function(number) {
  err <- .Call("SnnsKrui_setCurrPatSet", number, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setCurrPatSet", err)
}

#krui_err        saveNewPatterns(char *filename, int number);
SnnsKrui_saveNewPatterns <- function(filename, number) {
  err <- .Call("SnnsKrui_saveNewPatterns", filename, number, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("saveNewPatterns", err)
}

#krui_err        deletePatSet(int number);
SnnsKrui_deletePatSet <- function(number) {
  err <- .Call("SnnsKrui_deletePatSet", number, package="RSNNS")
  #if(err != 0) SnnsR_showWarningFromSnnsError("deletePatSet", err)
  return(err)
}

#krui_err        GetPatInfo(pattern_set_info *set_info,
#    pattern_descriptor *pat_info);
#krui_err        DefShowSubPat(int *insize, int *outsize, int *inpos, int *outpos);
SnnsKrui_DefShowSubPat <- function() {
  res <- .Call("SnnsKrui_DefShowSubPat", package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("DefShowSubPat", res$err)
  return(res)
}

#krui_err        DefTrainSubPat(int *insize, int *outsize, int *instep, int *outstep, int *max_n_pos);
SnnsKrui_DefTrainSubPat <- function() {
  res <- .Call("SnnsKrui_DefTrainSubPat", package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("DefTrainSubPat", res$err)
  return(res)
}

#krui_err        AlignSubPat(int *inpos, int *outpos, int *no);
#krui_err        GetShapeOfSubPattern(int *insize, int *outsize,
#    int *inpos, int *outpos, int n_pos);
#krui_err        saveNet(char *filename, char *netname);
SnnsKrui_saveNet <- function(filename, netname) {
  err <- .Call("SnnsKrui_saveNet", filename, netname, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("saveNet", err)
}

#krui_err        loadNet(char *filename, char **netname);
SnnsKrui_loadNet <- function() {
  res <- .Call("SnnsKrui_loadNet", package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("loadNet", res$err)
  return(res)
}

#krui_err        saveResultParam(char *filename, bool create,
#    int startpattern, int endpattern,
#    bool includeinput, bool includeoutput,
#    float *Update_param_array,
#    int NoOfUpdateParam);
#krui_err        allocateUnits(int no_of_units);
SnnsKrui_allocateUnits <- function(no_of_units) {
  err <- .Call("SnnsKrui_allocateUnits", no_of_units, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("allocateUnits", err)
}

#void            deleteNet(void);
SnnsKrui_deleteNet <- function() {
  return(.Call("SnnsKrui_deleteNet", package="RSNNS"))
}

#bool            getFirstSymbolTableEntry(char **symbol_name, int *symbol_type);
SnnsKrui_getFirstSymbolTableEntry <- function() {
  res <- .Call("SnnsKrui_getFirstSymbolTableEntry", package="RSNNS")
  if(!res$ret) SnnsR_showWarningFromSnnsError("getFirstSymbolTableEntry", res$ret)
  return(res)
}

#bool            getNextSymbolTableEntry(char **symbol_name, int *symbol_type);
SnnsKrui_getNextSymbolTableEntry <- function() {
  res <- .Call("SnnsKrui_getNextSymbolTableEntry", package="RSNNS")
  if(!res$ret) SnnsR_showWarningFromSnnsError("getNextSymbolTableEntry", res$ret)
  return(res)
}


#bool            symbolSearch(char *symbol, int symbol_type);
SnnsKrui_symbolSearch <- function(symbol, symbol_type) {
  return(.Call("SnnsKrui_symbolSearch", symbol, symbol_type, package="RSNNS"))  
}

#char           *getVersion(void);
SnnsKrui_getVersion <- function() {
  return(.Call("SnnsKrui_getVersion", package="RSNNS"))
}

#void            getNetInfo(int *no_of_sites, int *no_of_links,
#    int *no_of_STable_entries,
#    int *no_of_FTable_entries);
SnnsKrui_getNetInfo <- function() {
  return(.Call("SnnsKrui_getNetInfo", package="RSNNS"))
}

#void            getMemoryManagerInfo(int *unit_bytes,
#    int *site_bytes, int *link_bytes,
#    int *NTable_bytes, int *STable_bytes,
#    int *FTable_bytes);
SnnsKrui_getMemoryManagerInfo <- function() {
  return(.Call("SnnsKrui_getMemoryManagerInfo", package="RSNNS"))
}

#void            getUnitDefaults(FlintType *act, FlintType *bias, int *st,
#    int *subnet_no, int *layer_no, char **act_func,
#    char **out_func);
SnnsKrui_getUnitDefaults <- function() {
  return(.Call("SnnsKrui_getUnitDefaults", package="RSNNS"))
}

#krui_err        setUnitDefaults(FlintTypeParam act,
#    FlintTypeParam bias, int st, int subnet_no,
#    int layer_no, char *act_func, char *out_func);
SnnsKrui_setUnitDefaults <- function(act, bias, st, subnet_no, layer_no, act_func, out_func) {
  err <- .Call("SnnsKrui_setUnitDefaults", act, bias, st, subnet_no, layer_no, 
                                               act_func, out_func, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setUnitDefaults", err)
}

#void            resetNet(void);
SnnsKrui_resetNet <- function() {
  return(.Call("SnnsKrui_resetNet", package="RSNNS"))
}

#void            setSeedNo(long int seed);
SnnsKrui_setSeedNo <- function(seed) {
  return(.Call("SnnsKrui_setSeedNo", seed, package="RSNNS"))
}

#int             getNoOfInputUnits(void);
SnnsKrui_getNoOfInputUnits <- function() {
  return(.Call("SnnsKrui_getNoOfInputUnits", package="RSNNS"))
}

#int             getNoOfOutputUnits(void);
SnnsKrui_getNoOfOutputUnits <- function() {
  return(.Call("SnnsKrui_getNoOfOutputUnits", package="RSNNS"))
}

#int             getNoOfTTypeUnits(int UnitTType);
SnnsKrui_getNoOfTTypeUnits <- function(UnitTType) {
  return(.Call("SnnsKrui_getNoOfTTypeUnits", UnitTType, package="RSNNS"))
}

#int             getNoOfSpecialInputUnits(void);
SnnsKrui_getNoOfSpecialInputUnits <- function() {
  return(.Call("SnnsKrui_getNoOfSpecialInputUnits", package="RSNNS"))
}

#int             getNoOfSpecialOutputUnits(void);
SnnsKrui_getNoOfSpecialOutputUnits <- function() {
  return(.Call("SnnsKrui_getNoOfSpecialOutputUnits", package="RSNNS"))
}

#krui_err        xyTransTable(int op, int *x, int *y, int z);
#krui_err        getUnitCenters(int unit_no, int center_no,
#    struct PositionVector **unit_center);
#krui_err        setUnitCenters(int unit_no, int center_no,
#    struct PositionVector *unit_center);
#//char           *error(int error_code);
#float           NA_Error(int currentPattern, int error_unit, int error, bool ave);
SnnsKrui_NA_Error <- function(currentPattern, error_unit, error, ave) {
  return(.Call("SnnsKrui_NA_Error", currentPattern, error_unit, error, ave, package="RSNNS"))
}
#
#float           getVariance (void);
SnnsKrui_getVariance <- function() {
  return(.Call("SnnsKrui_getVariance", package="RSNNS"))
}

#int             countLinks(void);
SnnsKrui_countLinks <- function() {
  return(.Call("SnnsKrui_countLinks", package="RSNNS"))
}

#
##ifdef MASPAR_KERNEL
#
#krui_err        MasPar( mode );
#krui_err        getMasParStatus(void);
#krui_err        MasParBenchmark( func_type, cycles, result );
#
##endif
#
#krui_err        setSpecialNetworkType(int net_type);
SnnsKrui_setSpecialNetworkType <- function(net_type) {
  err <- .Call("SnnsKrui_setSpecialNetworkType", net_type, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("setSpecialNetworkType", err)
}

#int             getSpecialNetworkType(void);
SnnsKrui_getSpecialNetworkType <- function() {
  return(.Call("SnnsKrui_getSpecialNetworkType", package="RSNNS"))
}

#int             initInversion(void);
SnnsKrui_initInversion <- function() {
  return(.Call("SnnsKrui_initInversion", package="RSNNS"))
}

#void            inv_forwardPass(struct UnitList *inputs);
#double          inv_backwardPass(float learn, float delta_max,
#    int *err_units, float ratio,
#    struct UnitList *inputs,
#    struct UnitList *outputs);
#void            kohonen_SetExtraParameter(int x);
SnnsKrui_kohonen_SetExtraParameter <- function(x) {
  return(.Call("SnnsKrui_kohonen_SetExtraParameter", x, package="RSNNS"))
}

#void            spanning_tree(void);
SnnsKrui_spanning_tree <- function() {
  return(.Call("SnnsKrui_spanning_tree", package="RSNNS"))
}

#void            cc_updatePosOfSpecialUnits(void);
SnnsKrui_cc_updatePosOfSpecialUnits <- function() {
  return(.Call("SnnsKrui_cc_updatePosOfSpecialUnits", package="RSNNS"))
}

#krui_err        cc_deleteAllSpecialUnits(void);
SnnsKrui_cc_deleteAllSpecialUnits <- function() {
  err <- .Call("SnnsKrui_cc_deleteAllSpecialUnits", package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("cc_deleteAllSpecialUnits", err)
}

#int             getErrorCode(void);
SnnsKrui_getErrorCode <- function() {
  return(.Call("SnnsKrui_getErrorCode", package="RSNNS"))
}

#krui_err        setErrorHandler(void(* error_Handler )(int));
#void            execHandler(int error_code);
SnnsKrui_execHandler <- function(error_code) {
  return(.Call("SnnsKrui_execHandler", error_code, package="RSNNS"))
}

#----------------------------------------------
SnnsKrui_GetPatInfo <- function() {
  res <- .Call("SnnsKrui_GetPatInfo", package="RSNNS")
  if(res$err != 0) SnnsR_showWarningFromSnnsError("GetPatInfo", res$err)
  return(res)
}

#----------------------------------------------

#SnnsBn_jordan_createNet( SEXP IUnits, SEXP HUnits, SEXP OUnits, SEXP ICols, SEXP HCols, SEXP OCols)
SnnsBn_jordan_createNet <- function(IUnits, HUnits, OUnits, ICols, HCols, OCols) {
  err <- .Call("SnnsBn_jordan_createNet", IUnits, HUnits, OUnits, ICols, HCols, OCols, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("jordan_createNet", err)
}


SnnsBn_elman_createNet <- function(layer, columns, out_context) {
  err <- .Call("SnnsBn_elman_createNet", layer, columns, out_context, package="RSNNS")
  if(err != 0) SnnsR_showWarningFromSnnsError("elman_createNet", err)
}

SnnsKr_getSubPatData <- function(pat_no, sub_no, io_type) {
  return(.Call("SnnsKr_getSubPatData", pat_no, sub_no, io_type, package="RSNNS"))
}
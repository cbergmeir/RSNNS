SnnsR_getDefine <- function(defList, defValue)  {
  defRow <- which(defList[,2] == toString(defValue))
  return(defList[defRow,1])
}

SnnsR_resolveDefine <- function(defList, def)  {
  defRow <- which(defList[,1] == toString(def))
  return(as.numeric(defList[defRow,2]))  
}

#examples
#  getDefine(topological_unit_types,3)
#  resolveDefine(topological_unit_types,"UNIT_HIDDEN")
#  getDefine(error_codes,50)

SnnsR_showWarningFromSnnsError <- function(func, err) {
  warning(paste("An error occured in ", func,": ", SnnsR_getDefine(SnnsR_errorCodes, err),sep=""))
}

SnnsR_getAllUnitsTType <- function(ttype) {
  
  res <- NULL
  
  resolvedTType <- SnnsR_resolveDefine(SnnsR_topologicalUnitTypes, ttype)
  
  nUnits <- SnnsKrui_getNoOfUnits()
  
  for(i in 1:nUnits)  {
    if(i==1)  unit <- SnnsKrui_getFirstUnit()
    else unit <- SnnsKrui_getNextUnit()
    
    #print(unit)
    type <- SnnsKrui_getUnitTType(unit)
    if(type == resolvedTType) res <- c(res, unit)
    #print(type)
  }
  
  return(res)
}

SnnsR_getAllOutputUnits <- function() {
  return(SnnsR_getAllUnitsTType("UNIT_OUTPUT"))  
}

SnnsR_getAllInputUnits <- function() {
  return(SnnsR_getAllUnitsTType("UNIT_INPUT"))  
}

SnnsR_getAllHiddenUnits <- function() {
  return(SnnsR_getAllUnitsTType("UNIT_HIDDEN"))  
}

SnnsR_resetRSNNS <- function()  {
  
  err <- 0
  while (err == 0) {
    err <- SnnsKrui_deletePatSet(0)
  };
  
  #SnnsKrui_deleteAllPatterns()
  
  SnnsKrui_deleteNet()
  
  
}


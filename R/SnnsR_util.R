
SnnsR__getAllUnitsTType <- function(snnsObject, ttype) {
  
  res <- NULL
  
  resolvedTType <- SnnsDefines_resolveDefine(SnnsDefines_topologicalUnitTypes, ttype)
  
  nUnits <- snnsObject$getNoOfUnits()
  
  for(i in 1:nUnits)  {
    if(i==1)  unit <- snnsObject$getFirstUnit()
    else unit <- snnsObject$getNextUnit()
    
    #print(unit)
    type <- snnsObject$getUnitTType(unit)
    if(type == resolvedTType) res <- c(res, unit)
    #print(type)
  }
  
  return(res)
}

SnnsR__getAllOutputUnits <- function(snnsObject) {
  return(snnsObject$getAllUnitsTType("UNIT_OUTPUT"))  
}

SnnsR__getAllInputUnits <- function(snnsObject) {
  return(snnsObject$getAllUnitsTType("UNIT_INPUT"))  
}

SnnsR__getAllHiddenUnits <- function(snnsObject) {
  return(snnsObject$getAllUnitsTType("UNIT_HIDDEN"))  
}

SnnsR__resetRSNNS <- function(snnsObject)  {
  
  err <- 0
  while (err == 0) {
    err <- snnsObject$deletePatSet(0)
  };
  
  #SnnsKrui_deleteAllPatterns()
  
  snnsObject$deleteNet()
  
  
}


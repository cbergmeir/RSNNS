# @include SnnsRObjectFactory.R
#NULL

#' Create a pattern set
#' @param snnsObject \code{\linkS4class{SnnsR}} object
#' @param inputs The input values
#' @param targets The target values
#' @export
#' @rdname SnnsR-methods
SnnsR__createPatSet <- function(snnsObject, inputs, targets) {

  iUnits <- snnsObject$getAllInputUnits()
  oUnits <- snnsObject$getAllOutputUnits()
  
  x <- as.matrix(inputs)
  nInputs <- ncol(x)
  if (nInputs != length(iUnits)) 
    stop(paste("number of input data columns ", nInputs ," does not match number of input neurons ", length(iUnits) ,sep=""))
  
  if(!missing(targets)){
    y <- as.matrix(targets)
    nOutputs <- ncol(y)
    if (nOutputs != length(oUnits)) 
      stop(paste("number of output data columns ", nOutputs ," does not match number of output neurons ", length(oUnits) ,sep=""))
    
    if(nrow(x) != nrow(y)) 
      stop(paste("input value rows ",nrow(x)," not same as output value rows ",nrow(y),sep=""))
  }
  
  patSet <- snnsObject$allocNewPatternSet()


  
  for(i in 1:nrow(x)) {
    for(j in 1:nInputs)  {
      snnsObject$setUnitActivation(iUnits[j], x[i,j]);
    }
    
    if(!missing(targets) && length(targets) != 0) {  
      for(j in 1:nOutputs)  {
        snnsObject$setUnitActivation(oUnits[j], y[i,j]);
      }
    }
    snnsObject$newPattern();
  }

  snnsObject$setCurrPatSet(patSet$set_no)
  
  return(patSet)
}

#' Predict values with a trained net.
#' 
#' Generic function for prediction with a trained net.
#' 
#' @param snnsObject \code{\linkS4class{SnnsR}} object
#' @export
#' @rdname SnnsR-methods
SnnsR__genericPredictCurrPatSet <- function(snnsObject, units, updateFuncParams=c(0.0))  {
  
  #units <- snnsObject$getAllOutputUnits()
  
  noOfPatterns <- snnsObject$getNoOfPatterns()
  
  predictions <- matrix(nrow= noOfPatterns, ncol=length(units))
  
  for(currentPattern in 1:noOfPatterns)  {
    
    snnsObject$setPatternNo(currentPattern)
    
    snnsObject$showPattern(SnnsDefines_resolveDefine(SnnsDefines_patternUpdateModes,"OUTPUT_NOTHING"))
    
    snnsObject$updateNet(updateFuncParams)
    
    for(i in 1:length(units)) {
      predictions[currentPattern,i] <- snnsObject$getUnitOutput(units[i])
    }
    
  }
  
  return(predictions)
} 

SnnsR__whereAreResults <- function(snnsObject, netType="reg_class") {
  
  units <- NULL
  #netType <- "art1"
  
  if(netType == "art1") {  
    # in the ART1 network, the units that represent the output patterns are named cmp1, cmp2, ...
    units <- snnsObject$getUnitsByName("cmp")
    
  } else if(netType == "art2") {
    
    unitsX <- snnsObject$getUnitsByName("x")
    unitsQ <- snnsObject$getUnitsByName("q")
    units <- c(unitsX, unitsQ) 
    
  } else if(netType=="assoz") {
    
    units <- snnsObject$getAllHiddenUnits()
    
  } else if(netType=="som") {
    
    units <- snnsObject$getAllHiddenUnits()
    
  } else { #if(netType=="reg_class") {
    
    units <- snnsObject$getAllOutputUnits()
    
  } #else if(netType=="assoz") {  }
  
  units
}

#' Predict values with a trained net.
#' 
#' This function has to be used embedded in a step of loading and afterwards 
#' removing the patterns into the snns. As the snns only supports 2 pattern sets
#' in parallel, this has to be done with caution..
#' 
#' @param snnsObject \code{\linkS4class{SnnsR}} object
#' @export
#' @rdname SnnsR-methods
SnnsR__predictCurrPatSet <- function(snnsObject, netType="reg_class", updateFuncParams=c(0.0))  {
  
  units <- snnsObject$whereAreResults(netType)
  predictions <- snnsObject$genericPredictCurrPatSet(units, updateFuncParams)
  predictions
}

##' Predict values with a trained net.
##' 
##' This function has to be used embedded in a step of loading and afterwards 
##' removing the patterns into the snns. As the snns only supports 2 pattern sets
##' in parallel, this has to be done with caution..
##' 
##' @param snnsObject \code{\linkS4class{SnnsR}} object
##' @export
##' @rdname SnnsR-methods
#SnnsR__predictValuesCurrPatSet <- function(snnsObject)  {
#  
#  units <- snnsObject$getAllOutputUnits()
#  predictions <- snnsObject$genericPredictCurrPatSet(units)
#  predictions
#} 


##' Get the som output for every pattern in the current pattern set.
##' 
##' @param snnsObject \code{\linkS4class{SnnsR}} object
##' @export
##' @rdname SnnsR-methods
#SnnsR__somPredictCurrPatSet <- function(snnsObject)  {
#  
#  units <- snnsObject$getAllHiddenUnits()
#  predictions <- snnsObject$genericPredictCurrPatSet(units, updateFuncParams=c(0.0, 0.0, 1.0))
#  predictions
#
#} 

#' Get the som output for every pattern in the current pattern set.
#' 
#' @param snnsObject \code{\linkS4class{SnnsR}} object
#' @export
#' @rdname SnnsR-methods
SnnsR__somPredictComponentMaps <- function(snnsObject)  {
  
  snnsObject$setTTypeUnitsActFunc("UNIT_HIDDEN", "Act_Component")
  
  nInputs <- snnsObject$getNoOfInputUnits()
  units <- snnsObject$getAllHiddenUnits()
  
  predictions <- matrix(nrow= nInputs, ncol=length(units))
  
  
  for(input in 1:nInputs)  {
  
    #parameter has to be reversed to get same order as in SNNS gui.. TODO: why?
    snnsObject$kohonen_SetExtraParameter((nInputs+1) - input)
    snnsObject$updateNet(c(0.0, 0.0, 1.0))
    
    for(i in 1:length(units)) {
      predictions[input,i] <- snnsObject$getUnitOutput(units[i])
      #predictions[input,i] <- snnsObject$getUnitValueA(units[i])
    }
    
  }
  
  snnsObject$setTTypeUnitsActFunc("UNIT_HIDDEN", "Act_Euclid")
  
  return(predictions)
} 

#' Get the som winners for every pattern in the current pattern set.
#' 
#' @param snnsObject \code{\linkS4class{SnnsR}} object
#' @export
#' @rdname SnnsR-methods
SnnsR__somPredictCurrPatSetWinners <- function(snnsObject)  {
  
  units <- snnsObject$getAllHiddenUnits()
  
  noOfPatterns <- snnsObject$getNoOfPatterns()
  
  cat("noOfPatterns: ", noOfPatterns, "\n", sep="")
  
  #winners <- NULL
  winners <- vector()
  
  for(currentPattern in 1:noOfPatterns)  {
   
    predictions <- vector()
    
    snnsObject$setPatternNo(currentPattern)
    snnsObject$showPattern(SnnsDefines_resolveDefine(SnnsDefines_patternUpdateModes,"OUTPUT_NOTHING"))
    snnsObject$updateNet(c(0.0, 0.0, 1.0))
    
    for(i in 1:length(units)) {
      predictions[i] <- snnsObject$getUnitOutput(units[i])
      #predictions[currentPattern,i] <- snnsObject$getUnitValueA(units[i])
    }
    #x <- matrix(predictions, nrow=X)

    #the winner is the one with minimal distance
    #winners <- rbind(winners, which(x == min(x, na.rm=TRUE), arr.ind = TRUE))
    winners <- c(winners, which(predictions == min(predictions, na.rm=TRUE)))
  }
  
#  som <- matrix(0, nrow=X, ncol=Y)
  
#  for(i in 1:nrow(winners)) {
#    som[winners[i,1], winners[i,2]] <- som[winners[i,1], winners[i,2]] + 1 
#  }

  map <- seq(0, 0, length=length(units))

  for(i in 1:length(winners)) {
    map[winners[i]] <- map[winners[i]] + 1 
  }

  map
  
} 

#' Get the som winners for every pattern in the current pattern set.
#' 
#' @param snnsObject \code{\linkS4class{SnnsR}} object
#' @export
#' @rdname SnnsR-methods
SnnsR__somPredictCurrPatSetWinnersSpanTree <- function(snnsObject)  {
  
  units <- snnsObject$getAllHiddenUnits()
  
  noOfPatterns <- snnsObject$getNoOfPatterns()
  
  snnsObject$spanning_tree()
      
  predictions <- vector() #matrix(nrow= noOfPatterns, ncol=length(units))
  
  #for(currentPattern in 1:noOfPatterns)  {
    
  #  snnsObject$setPatternNo(currentPattern)
  #  snnsObject$showPattern(SnnsDefines_resolveDefine(SnnsDefines_patternUpdateModes,"OUTPUT_NOTHING"))
  #  snnsObject$updateNet(0)
    
    for(i in 1:length(units)) {
      #predictions[currentPattern,i] <- snnsObject$getUnitOutput(units[i])
      predictions[i] <- snnsObject$getUnitValueA(units[i])
    }
    
  #}
  
  return(predictions)
} 



##' Get the art1 output for every pattern in the current pattern set.
##' 
##' @param snnsObject \code{\linkS4class{SnnsR}} object
##' @export
##' @rdname SnnsR-methods
#SnnsR__art1Predict <- function(snnsObject, updateFuncParams=c(0.0))  {
#  
#  units <- snnsObject$getUnitsByName("cmp")
#
#  predictions <- snnsObject$genericPredictCurrPatSet(units, updateFuncParams=updateFuncParams)
#  predictions
#  
#} 


##' Get the art2 output for every pattern in the current pattern set.
##' 
##' @param snnsObject \code{\linkS4class{SnnsR}} object
##' @export
##' @rdname SnnsR-methods
#SnnsR__art2Predict <- function(snnsObject, updateFuncParams=c(0.0))  {
#  
#  unitsX <- snnsObject$getUnitsByName("x")
#  unitsQ <- snnsObject$getUnitsByName("q")
#  units <- c(unitsX, unitsQ)
#  
#  #unitsR1 <- snnsObject$getUnitsByName("r1")
#  #unitsR2 <- snnsObject$getUnitsByName("r2")
#  #unitsR3 <- snnsObject$getUnitsByName("r3")  
#  #units <- c(unitsR1, unitsR2, unitsR3)  
#
#  #units <- snnsObject$getUnitsByName("rec")
#  
#  predictions <- snnsObject$genericPredictCurrPatSet(units, updateFuncParams=updateFuncParams)
#  predictions
#  
#} 
#
#SnnsR__assozPredict <- function(snnsObject, updateFuncParams=c(50.0))  {
#  
#  units <- snnsObject$getAllHiddenUnits()
#  predictions <- snnsObject$genericPredictCurrPatSet(units, updateFuncParams=updateFuncParams)
#  predictions
#} 

 

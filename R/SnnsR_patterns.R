# @include SnnsRObjectFactory.R
#NULL

#' Create a pattern
#' @param snnsObject \code{\linkS4class{SnnsR}} object
#' @param inputValues The input values
#' @param targetValues The target values
#' @export
#' @rdname SnnsR-methods
SnnsR__createPatterns <- function(snnsObject, inputValues, targetValues) {

  #setMethod("createPatterns", 
   #   signature = signature(object = "SnnsR", inputValues = "numeric", targetValues = "numeric"),
    #  function(object, inputValues, targetValues) {
        
  #snnsObject <- object
  
  inputs <- snnsObject$getAllInputUnits()
  outputs <- snnsObject$getAllOutputUnits()
  
  x <- as.matrix(inputValues)
  nInputs <- ncol(x)
  if (nInputs != length(inputs)) 
    stop(paste("number of input data columns ", nInputs ," does not match number of input neurons ", length(inputs) ,sep=""))
  
  if(!missing(targetValues)){
    y <- as.matrix(targetValues)
    nOutputs <- ncol(y)
    if (nOutputs != length(outputs)) 
      stop(paste("number of output data columns ", nOutputs ," does not match number of output neurons ", length(outputs) ,sep=""))
    
    if(nrow(x) != nrow(y)) 
      stop(paste("input value rows ",nrow(x)," not same as output value rows ",nrow(y),sep=""))
  }
  
  #snnsObject$deleteAllPatterns()
  
  patset <- snnsObject$allocNewPatternSet()
  print(snnsObject$getNoOfPatterns())
  #print(patset)
  #snnsObject$setCurrPatSet(patset)

  
  for(i in 1:nrow(x)) {
    for(j in 1:nInputs)  {
      snnsObject$setUnitActivation(inputs[j], x[i,j]);
    }
    
    if(!missing(targetValues) && length(targetValues) != 0) {  
      for(j in 1:nOutputs)  {
        snnsObject$setUnitActivation(outputs[j], y[i,j]);
      }
    }
    snnsObject$newPattern();
  }

  #print("-----------------")
  #print(nrow(x))
  #print(snnsObject$getNoOfPatterns())
  
  return(patset)
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
SnnsR__predictValuesCurrPatSet <- function(snnsObject)  {
  
  outputs <- snnsObject$getAllOutputUnits()
  
  noOfPatterns <- snnsObject$getNoOfPatterns()
  
  predictions <- matrix(nrow= noOfPatterns, ncol=length(outputs))

  for(currentPattern in 1:noOfPatterns)  {
    
    snnsObject$setPatternNo(currentPattern)
    
    snnsObject$showPattern(SnnsDefines_resolveDefine(SnnsDefines_patternUpdateModes,"OUTPUT_NOTHING"))
    
    snnsObject$updateNet(0)
    
    for(i in 1:length(outputs)) {
      predictions[currentPattern,i] <- snnsObject$getUnitOutput(outputs[i])
    }
    
  }
  
  return(predictions)
} 


#' Get the som output for every pattern in the current pattern set.
#' 
#' @param snnsObject \code{\linkS4class{SnnsR}} object
#' @export
#' @rdname SnnsR-methods
SnnsR__somPredictCurrPatSet <- function(snnsObject)  {
  
  units <- snnsObject$getAllHiddenUnits()
  
  noOfPatterns <- snnsObject$getNoOfPatterns()
  
  predictions <- matrix(nrow= noOfPatterns, ncol=length(units))
  
  for(currentPattern in 1:noOfPatterns)  {
    
    snnsObject$setPatternNo(currentPattern)
    
    snnsObject$showPattern(SnnsDefines_resolveDefine(SnnsDefines_patternUpdateModes,"OUTPUT_NOTHING"))
    
    snnsObject$updateNet(0)
    
    for(i in 1:length(units)) {
      predictions[currentPattern,i] <- snnsObject$getUnitOutput(units[i])
    }
    
  }
  
  return(predictions)
} 

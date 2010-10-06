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
  
  for(i in 1:nrow(x)) {
    for(j in 1:nInputs)  {
      snnsObject$setUnitActivation(inputs[j], x[i,j]);
    }
    
    if(!missing(targetValues)) {  
      for(j in 1:nOutputs)  {
        snnsObject$setUnitActivation(outputs[j], y[i,j]);
      }
    }
    snnsObject$newPattern();
  }
  
  return(patset)
}

#' Predict values with a trained net
#' @param snnsObject \code{\linkS4class{SnnsR}} object
#' @param inputValues The new input values
#' @export
#' @rdname SnnsR-methods
SnnsR__predictValues <- function(snnsObject, inputValues)  {
  
  outputs <- snnsObject$getAllOutputUnits()
  
  predictions <- matrix(nrow= nrow(inputValues), ncol=length(outputs))
  #currentPattern <- 84
  for(currentPattern in 1:nrow(inputValues))  {
    
    snnsObject$setPatternNo(currentPattern)
    
    #OUTPUT_NOTHING  1
    snnsObject$showPattern(SnnsDefines_resolveDefine(SnnsDefines_patternUpdateModes,"OUTPUT_NOTHING"))
    
    snnsObject$updateNet(0)
    
    for(i in 1:length(outputs)) {
      predictions[currentPattern,i] <- snnsObject$getUnitOutput(outputs[i])
    }
    
#    if (length(which(o != irisTargets[currentPattern,])) != 0)  {
#      print("not recognized correctly:")
#      print(currentPattern)
#    }
#    print(o)
#    print(irisTargets[currentPattern,])
  }
  
  return(predictions)
} 
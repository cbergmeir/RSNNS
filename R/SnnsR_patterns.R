#---------------------------------------
#Function createPatterns
#---------------------------------------
SnnsR_createPatterns <- function(inputValues, targetValues) {
  
  inputs <- SnnsR_getAllInputUnits()
  outputs <- SnnsR_getAllOutputUnits()
  
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
  
  #SnnsKrui_deleteAllPatterns()
  
  patset <- SnnsKrui_allocNewPatternSet()
  
  for(i in 1:nrow(x)) {
    for(j in 1:nInputs)  {
      SnnsKrui_setUnitActivation(inputs[j], x[i,j]);
    }
    
    if(!missing(targetValues)) {  
      for(j in 1:nOutputs)  {
        SnnsKrui_setUnitActivation(outputs[j], y[i,j]);
      }
    }
    SnnsKrui_newPattern();
  }
  
  return(patset)
}

#---------------------------------------
#Function predictValues
#---------------------------------------
SnnsR_predictValues <- function(inputValues)  {
  
  outputs <- SnnsR_getAllOutputUnits()
  
  predictions <- matrix(nrow= nrow(inputValues), ncol=length(outputs))
  #currentPattern <- 84
  for(currentPattern in 1:nrow(inputValues))  {
    
    SnnsKrui_setPatternNo(currentPattern)
    
    #OUTPUT_NOTHING  1
    SnnsKrui_showPattern(SnnsR_resolveDefine(SnnsR_patternUpdateModes,"OUTPUT_NOTHING"))
    
    SnnsKrui_updateNet(0)
    
    for(i in 1:length(outputs)) {
      predictions[currentPattern,i] <- SnnsKrui_getUnitOutput(outputs[i])
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
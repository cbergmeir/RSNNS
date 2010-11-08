# @include SnnsRObjectFactory.R
#NULL

#' Create a pattern set.
#' 
#' @param inputs the input values
#' @param targets the target values
#' @return a list with elements \code{$err} and \code{$set_no}, which identifies the patern set within the SnnsR object 
#' @rdname SnnsRObject$createPatSet
#' @usage \S4method{createPatSet}{SnnsR}(inputs, targets)
#' @aliases createPatSet,SnnsR-method SnnsRObject$createPatSet
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
#' @param units the units that define the output
#' @param updateFuncParams the parameters for the update function (the function has to be already set)
#' @rdname SnnsRObject$genericPredictCurrPatSet
#' @usage \S4method{genericPredictCurrPatSet}{SnnsR}(units, updateFuncParams=c(0.0))
#' @aliases genericPredictCurrPatSet,SnnsR-method SnnsRObject$genericPredictCurrPatSet
SnnsR__genericPredictCurrPatSet <- function(snnsObject, units, updateFuncParams=c(0.0))  {
  
  noOfPatterns <- snnsObject$getNoOfPatterns()
  
  predictions <- matrix(nrow= noOfPatterns, ncol=length(units))
  
  snnsObject$DefTrainSubPat()  
  
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


#' Get a list of output units of a net.
#' 
#' Depending on the network architecture, output is present in hidden units, in output units, etc.
#' In some network types, the output units have a certain name prefix in SNNS. This function finds
#' the output units according to certain network types. Possible types (= values for outputMethod)
#' are: "art1", "art2", "assoz", "som", "output". If the given \code{outputMethod} is unknown, 
#' the function defaults to "output".  
#' 
#' @param outputMethod a string defining the output method of the net. 
#' @rdname SnnsRObject$whereAreResults
#' @usage \S4method{whereAreResults}{SnnsR}(outputMethod="output")
#' @aliases whereAreResults,SnnsR-method SnnsRObject$whereAreResults
SnnsR__whereAreResults <- function(snnsObject, outputMethod="output") {
  
  units <- NULL
  #outputMethod <- "art1"
  
  if(outputMethod == "art1") {  
    # in the ART1 network, the units that represent the output patterns are named cmp1, cmp2, ...
    units <- snnsObject$getUnitsByName("cmp")
    
  } else if(outputMethod == "art2") {
    
    unitsX <- snnsObject$getUnitsByName("x")
    unitsQ <- snnsObject$getUnitsByName("q")
    units <- c(unitsX, unitsQ) 
    
  } else if(outputMethod=="assoz") {
    
    units <- snnsObject$getAllHiddenUnits()
    
  } else if(outputMethod=="som") {
    
    units <- snnsObject$getAllHiddenUnits()
    
  } else { #if(outputMethod=="reg_class") {
    
    units <- snnsObject$getAllOutputUnits()
    
  } #else if(outputMethod=="assoz") {  }
  
  units
}


#' Predict values with a trained net.
#' 
#' This function has to be used embedded in a step of loading and afterwards 
#' removing the patterns into the SnnsRObject. As SNNS only supports 2 pattern sets
#' in parallel, removing unneeded pattern sets is quite important.
#' 
#' @param outputMethod is passed to \link{SnnsRObject$whereAreResults}
#' @param updateFuncParams parameters passed to the networks update function
#' @rdname SnnsRObject$predictCurrPatSet
#' @usage \S4method{predictCurrPatSet}{SnnsR}(outputMethod="reg_class", updateFuncParams=c(0.0))
#' @aliases predictCurrPatSet,SnnsR-method SnnsRObject$predictCurrPatSet
SnnsR__predictCurrPatSet <- function(snnsObject, outputMethod="reg_class", updateFuncParams=c(0.0))  {
  
  units <- snnsObject$whereAreResults(outputMethod)
  predictions <- snnsObject$genericPredictCurrPatSet(units, updateFuncParams)
  predictions
}


#' Get the som component maps.
#' 
#' @rdname SnnsRObject$somPredictComponentMaps
#' @usage \S4method{somPredictComponentMaps}{SnnsR}()
#' @aliases somPredictComponentMaps,SnnsR-method SnnsRObject$somPredictComponentMaps
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



#' Get for every unit the amount it has won, for every pattern in the current pattern set.
#' 
#' @rdname SnnsRObject$somPredictCurrPatSetWinners
#' @usage \S4method{somPredictCurrPatSetWinners}{SnnsR}()
#' @aliases somPredictCurrPatSetWinners,SnnsR-method SnnsRObject$somPredictCurrPatSetWinners
SnnsR__somPredictCurrPatSetWinners <- function(snnsObject, updateFuncParams=c(0.0, 0.0, 1.0), saveWinnersPerPattern=TRUE, targets=NULL)  {
  
  units <- snnsObject$getAllHiddenUnits()
  noOfPatterns <- snnsObject$getNoOfPatterns()
  winners <- snnsObject$somPredictCurrPatSetWinnersC(units, noOfPatterns, updateFuncParams)
  
  map <- seq(0, 0, length=length(units))
  
  for(i in 1:length(winners)) {
    map[winners[i]] <- map[winners[i]] + 1 
  }

  if(!is.null(targets)) {

    classes <- unique(targets)
    numClasses <- 1:length(classes)
    names(numClasses) <- classes
    labeledUnits <- matrix(0, nrow=length(units), ncol=length(classes))
    colnames(labeledUnits) <- classes
    
    for(i in 1:length(winners)) {
      currUnit <- winners[i]
      labeledUnits[currUnit, numClasses[targets[i]]] <- labeledUnits[currUnit, numClasses[targets[i]]] + 1
    }
  } else {
    labeledUnits <- NULL
  }
  
  if(!saveWinnersPerPattern) winners <- NULL
  
  return(list(nWinnersPerUnit=map, winnersPerPattern=winners, labeledUnits=labeledUnits))
 
#  Function is now reimplemented in C++ to be faster..
  
#  units <- snnsObject$getAllHiddenUnits()
#  noOfPatterns <- snnsObject$getNoOfPatterns()
#  
#  winners <- vector()
#  
#  for(currentPattern in 1:noOfPatterns)  {
#   
#    predictions <- vector()
#    
#    snnsObject$setPatternNo(currentPattern)
#    snnsObject$showPattern(SnnsDefines_resolveDefine(SnnsDefines_patternUpdateModes,"OUTPUT_NOTHING"))
#    snnsObject$updateNet(updateFuncParams)
#    
#    for(i in 1:length(units)) {
#      predictions[i] <- snnsObject$getUnitOutput(units[i])
#    }
#    
#    winners <- c(winners, which(predictions == min(predictions, na.rm=TRUE)))
#  }
#
#  map <- seq(0, 0, length=length(units))
#
#  for(i in 1:length(winners)) {
#    map[winners[i]] <- map[winners[i]] + 1 
#  }
#
#  map
  
} 


#' Get the spanning tree of the som, calculated directly by SNNS.
#' 
#' @rdname SnnsRObject$somPredictCurrPatSetWinnersSpanTree
#' @usage \S4method{somPredictCurrPatSetWinnersSpanTree}{SnnsR}()
#' @aliases somPredictCurrPatSetWinnersSpanTree,SnnsR-method SnnsRObject$somPredictCurrPatSetWinnersSpanTree
SnnsR__somPredictCurrPatSetWinnersSpanTree <- function(snnsObject)  {
  
  units <- snnsObject$getAllHiddenUnits()
  
  noOfPatterns <- snnsObject$getNoOfPatterns()
  
  snnsObject$spanning_tree()
      
  predictions <- vector() 
    
    for(i in 1:length(units)) {
      predictions[i] <- snnsObject$getUnitValueA(units[i])
    }
  
  return(predictions)
} 


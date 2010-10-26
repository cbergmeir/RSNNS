#' Create and train an rbf with the DDA algorithm.
#'
#' As the output is winner-takes-all, this type of net can only be used
#' for classification. The output of the iterative error is meaningless
#'
#' @export
#' @author Christoph
rbfDDA <- function(x, ...) UseMethod("rbfDDA")

#' Create and train an rbf with the DDA algorithm.
#'
#' As the output is winner-takes-all, this type of net can only be used
#' for classification. The output of the iterative error is meaningless
#' 
#' @export
#' @author Christoph
rbfDDA.default <- function(x, y, maxit=100, 
    initFunc="Randomize_Weights", initFuncParams=c(-0.3, 0.3), 
    learnFunc="RBF-DDA", learnFuncParams=c(0.4, 0.2, 5), 
    updateFunc="Topological_Order", updateFuncParams=c(0.0),
    shufflePatterns=TRUE, linOut=FALSE) {
  
  #TODO: does linOut make sense here?  
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("rbfDDA", "reg_class"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)
  
  snns$archParams <- NULL#list(size=size)
  
  snns$snnsObject$setUnitDefaults(c(0,0,1,0,1,"Act_Logistic","Out_Identity"))
  snns$snnsObject$createNet(c(nInputs,nOutputs), fullyConnectedFeedForward = FALSE)
  
  if(linOut) {
    outputActFunc <- "Act_Identity"
  } else {
    outputActFunc <- "Act_Logistic"
  }
  
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_INPUT", "Act_Identity")
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_OUTPUT", outputActFunc)
  
  
  snns <- train.rsnns(snns, inputsTrain=x, targetsTrain=y)
  
  snns
  
#  #------------------------------------------
#  x <- as.matrix(x)
#  y <- as.matrix(y)
#  
#  checkInput(x,y)
#  
#  snnsObject <- SnnsRObjectFactory()
#  
#  nInputs <- dim(x)[2L]
#  nOutputs <- dim(y)[2L]
#  
#  #snnsObject$setUpdateFunc("Topological_Order")
#  
#  snnsObject$setUnitDefaults(c(0,0,1,0,1,"Act_Logistic","Out_Identity"))
#  snnsObject$createNet(c(nInputs,nOutputs), fullyConnectedFeedForward = FALSE)
#  
#  if(linOut) {
#    outputActFunc <- "Act_Identity"
#  } else {
#    outputActFunc <- "Act_Logistic"
#  }
#  
#  snnsObject$setTTypeUnitsActFunc("UNIT_INPUT", "Act_Identity")
#  snnsObject$setTTypeUnitsActFunc("UNIT_OUTPUT", outputActFunc)
#  
#  #result <- snnsObject$train(x, y, learnFunc="RBF-DDA", learnFuncParams=c(0.4, 0.2, 5), maxit=maxit, shufflePatterns=TRUE, inputsTest=inputsTest, targetsTest=targetsTest)
#
#  result <- snnsObject$train(x, y, initFunc=initFunc, initFuncParams=initFuncParams, 
#      learnFunc=learnFunc, learnFuncParams=learnFuncParams, updateFunc=updateFunc, 
#      updateFuncParams=updateFuncParams, outputMethod="output", maxit=maxit, 
#      shufflePatterns=shufflePatterns, computeError=FALSE)
#  
#  snns <- reg_classObjectFactory(nInputs, nOutputs, snnsObject, "rbfDDA", result)
#    
#  snns  
}


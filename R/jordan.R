#' Create and train a jordan network.
#'
#' @export
#' @author Christoph
jordan <- function(x, ...) UseMethod("jordan")

#' Create and train a jordan network.
#'
#' @export
#' @author Christoph
jordan.default <- function(x, y, size=c(5), maxit=100, 
    initFunc="JE_Weights", initFuncParams=c(1.0,  -1.0,  0.3,  1.0,  0.5), 
    learnFunc="JE_BP", learnFuncParams=c(0.2), 
    updateFunc="JE_Order", updateFuncParams=c(0.0),    
    shufflePatterns=TRUE, linOut=TRUE, inputsTest=NULL, targetsTest=NULL) {
  
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("jordan", "reg_class"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=TRUE)
  
  snns$archParams <- list(size=size)
  
  snns$snnsObject$setUnitDefaults(1,0,1,0,1,"Act_Logistic","Out_Identity")
  snns$snnsObject$jordan_createNet(nInputs, size, nOutputs, 1, 1, 1)
  
  
  if(linOut) {
    outputActFunc <- "Act_Identity"
  } else {
    outputActFunc <- "Act_Logistic"
  }
  
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_INPUT", "Act_Identity")
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_OUTPUT", outputActFunc)
  
  
  snns <- train.rsnns(snns, inputsTrain=x, targetsTrain=y, inputsTest=inputsTest, targetsTest=targetsTest)
  
  snns  
  
#  #-------------------------------------------
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
#  snnsObject$setUnitDefaults(1,0,1,0,1,"Act_Logistic","Out_Identity")
#  snnsObject$jordan_createNet(nInputs, size, nOutputs, 1, 1, 1)
#  
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
#  result <- snnsObject$train(x, y, initFunc=initFunc, initFuncParams=initFuncParams, 
#      learnFunc=learnFunc, learnFuncParams=learnFuncParams, updateFunc=updateFunc, 
#      updateFuncParams=updateFuncParams, outputMethod="output", maxit=maxit, 
#      shufflePatterns=shufflePatterns, computeError=TRUE,
#      inputsTest=inputsTest, targetsTest=targetsTest)
#  
#  snns <- reg_classObjectFactory(nInputs, nOutputs, snnsObject, "jordan", result)
#  
#  snns  
}

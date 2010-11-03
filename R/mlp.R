#' Create and train an mlp.
#'
#' @export
#' @author Christoph
mlp <- function(x, ...) UseMethod("mlp")

#' Create and train an mlp.
#'
#' @export
#' @S3method mlp default
#' @author Christoph
mlp.default <- function(x, y, size=c(5), maxit=100, 
    initFunc="Randomize_Weights", initFuncParams=c(-0.3, 0.3), 
    learnFunc="Quickprop", learnFuncParams=c(0.2), 
    updateFunc="Topological_Order", updateFuncParams=c(0.0),
    hiddenActFunc="Act_Logistic",
    shufflePatterns=TRUE, linOut=FALSE, inputsTest=NULL, targetsTest=NULL) {
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)

  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("mlp", "reg_class"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=TRUE)
  
  snns$archParams <- list(size=size)
  
  snns$snnsObject$setUnitDefaults(0,0,1,0,1,"Act_Logistic","Out_Identity")
  snns$snnsObject$createNet(unitsPerLayer=c(nInputs, size, nOutputs), fullyConnectedFeedForward=TRUE)
  
  if(linOut) {
    outputActFunc <- "Act_Identity"
  } else {
    outputActFunc <- "Act_Logistic"
  }
  
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_INPUT", "Act_Identity")
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_HIDDEN", hiddenActFunc)
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_OUTPUT", outputActFunc)
  
  
  snns <- train.rsnns(snns, inputsTrain=x, targetsTrain=y, inputsTest=inputsTest, targetsTest=targetsTest)

}


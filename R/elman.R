#' Create and train an elman network.
#'
#' @export
#' @author Christoph
elman <- function(x, ...) UseMethod("elman")

#' Create and train an elman network.
#'
#' @export
#' @author Christoph
elman.default <- function(x, y, size=c(5), maxit=100, 
    initFunc="JE_Weights", initFuncParams=c(1.0,  -1.0,  0.3,  1.0,  0.5), 
    learnFunc="JE_BP", learnFuncParams=c(0.2), 
    updateFunc="JE_Order", updateFuncParams=c(0.0),    
    shufflePatterns=TRUE, linOut=TRUE, outContext=FALSE, inputsTest=NULL, targetsTest=NULL) {
  

  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("elman", "reg_class"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=TRUE)
  
  snns$archParams <- list(size=size)
  
  snns$snnsObject$setUnitDefaults(1,0,1,0,1,"Act_Logistic","Out_Identity")
  snns$snnsObject$elman_createNet(c(nInputs, size, nOutputs), seq(1,1,length=(length(size)+2)), outContext)
  
  if(linOut) {
    outputActFunc <- "Act_Identity"
  } else {
    outputActFunc <- "Act_Logistic"
  }
  
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_INPUT", "Act_Identity")
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_OUTPUT", outputActFunc)
  
  
  snns <- train.rsnns(snns, inputsTrain=x, targetsTrain=y, inputsTest=inputsTest, targetsTest=targetsTest)

  snns  
}

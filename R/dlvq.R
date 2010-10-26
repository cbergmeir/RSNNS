#' Create and train an dlvq network.
#'
#' @export
#' @author Christoph
dlvq <- function(x, ...) UseMethod("dlvq")

#' Create and train an dlvq network.
#'
#' @export
#' @author Christoph
dlvq.default <- function(x, y, maxit=100, 
    initFunc="Randomize_Weights", initFuncParams=c(1.0, -1.0), 
    learnFunc="Dynamic_LVQ", learnFuncParams=c(0.03, 0.03, 10.0), 
    updateFunc="Dynamic_LVQ", updateFuncParams=c(0.0),    
    shufflePatterns=TRUE) {
  
  
  x <- as.matrix(x)
  
  nInputs <- dim(x)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("dlvq", "reg_class"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)
  
  snns$archParams <- NULL #list(dimX=dimX, dimY=dimY)
  
  snns$snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Identity','Out_Identity')
  snns$snnsObject$createNet(c(nInputs, 1), fullyConnectedFeedForward = FALSE)
  
  snns <- train.rsnns(snns, inputsTrain=x, targetsTrain=y)
  
  #snns$fitted.values <- matrixToActMapList(snns$fitted.values, nrow=dimX)
  
  snns
}


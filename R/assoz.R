#' Create and train an associative memory.
#'
#' @export
assoz <- function(x, ...) UseMethod("assoz")

#' Create and train an associative memory.
#' 
#' @param x a matrix with training inputs for the network
#' @param dimX
#' @param dimY
#' @param maxit maximum of iterations to learn
#' @param initFunc the initialization function to use
#' @param initFuncParams the parameters for the initialization function
#' @param learnFunc the learning function to use
#' @param learnFuncParams the parameters for the learning function
#' @param updateFunc the update function to use
#' @param updateFuncParams the parameters for the update function
#' @param shufflePatterns should the patterns be shuffled?
#' @param ... additional function parameters (currently not used)
#' @export
#' @S3method assoz default
#' @method assoz default
#' @rdname assoz
assoz.default <- function(x, dimX, dimY, maxit=100, 
    initFunc="RM_Random_Weights", initFuncParams=c(1.0, -1.0), 
    learnFunc="RM_delta", learnFuncParams=c(0.01, 100, 0.0, 0.0, 0.0), 
    updateFunc="Auto_Synchronous", updateFuncParams=c(50.0),    
    shufflePatterns=TRUE, ...) {
  
  
  x <- as.matrix(x)
  
  nInputs <- dim(x)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("assoz", "association"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)
  
  snns$archParams <- list(dimX=dimX, dimY=dimY)
  
  snns$snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Identity','Out_Identity')
  snns$snnsObject$assoz_createNet(dimX, dimY)
  
  snns <- train(snns, inputsTrain=x)
  
  snns$fitted.values <- matrixToActMapList(snns$fitted.values, nrow=dimX)
  
  snns
}


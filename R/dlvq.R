#' Create and train a dlvq network.
#'
#' @export
dlvq <- function(x, ...) UseMethod("dlvq")

#' Create and train a dlvq network.
#' 
#' @param x a matrix with training inputs for the network
#' @param y the corresponding targets values
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
#' @S3method dlvq default
#' @method dlvq default
#' @rdname dlvq
dlvq.default <- function(x, y, maxit=100, 
    initFunc="Randomize_Weights", initFuncParams=c(1.0, -1.0), 
    learnFunc="Dynamic_LVQ", learnFuncParams=c(0.03, 0.03, 10.0), 
    updateFunc="Dynamic_LVQ", updateFuncParams=c(0.0),    
    shufflePatterns=TRUE, ...) {
  
  
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
  
  snns <- train(snns, inputsTrain=x, targetsTrain=y)
  
  #snns$fitted.values <- matrixToActMapList(snns$fitted.values, nrow=dimX)
  
  snns
}


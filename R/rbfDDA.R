#' Create and train an rbf with the DDA algorithm.
#'
#' As the output is winner-takes-all, this type of net can only be used
#' for classification.
#'
#' @export
rbfDDA <- function(x, ...) UseMethod("rbfDDA")

#' Create and train an rbf with the DDA algorithm.
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
#' @param linOut sets the activation function of the output units to linear or logistic
#' @param ... additional function parameters (currently not used)
#' @export
#' @S3method rbfDDA default
#' @method rbfDDA default
#' @rdname rbfDDA
rbfDDA.default <- function(x, y, maxit=100, 
    initFunc="Randomize_Weights", initFuncParams=c(-0.3, 0.3), 
    learnFunc="RBF-DDA", learnFuncParams=c(0.4, 0.2, 5), 
    updateFunc="Topological_Order", updateFuncParams=c(0.0),
    shufflePatterns=TRUE, linOut=FALSE, ...) {
  
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
  
  
  snns <- train(snns, inputsTrain=x, targetsTrain=y)
  
  snns

}


#' Create and train an art1 network.
#'
#' @export
art1 <- function(x, ...) UseMethod("art1")


#' Create and train an art1 network.
#' 
#' @param x a matrix with training inputs for the network
#' @param dimX 
#' @param dimY
#' @param nClusters
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
#' @S3method art1 default
#' @method art1 default
#' @rdname art1
art1.default <- function(x, dimX, dimY, nClusters=nrow(x), maxit=100, 
    initFunc="ART1_Weights", initFuncParams=c(1.0, 1.0), 
    learnFunc="ART1", learnFuncParams=c(0.9, 0.0, 0.0), 
    updateFunc="ART1_Stable", updateFuncParams=c(0.0),    
    shufflePatterns=TRUE, ...) {
  
  x <- as.matrix(x)

  nInputs <- dim(x)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("art1", "association"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)

  snns$archParams <- list(nClusters=nClusters, dimX=dimX, dimY=dimY)
  
  #snns$snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')
  snns$snnsObject$art1_createNet(dimX*dimY,dimX,nClusters,dimX)
 
  snns <- train(snns, inputsTrain=x)
  
  snns$fitted.values <- matrixToActMapList(snns$fitted.values, nrow=dimX)
  
  snns
}


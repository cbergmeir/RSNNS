#' Create and train an art1 network.
#'
#' @export
#' @author Christoph
art1 <- function(x, ...) UseMethod("art1")

#' Create and train an art1 network.
#'
#' @export
#' @author Christoph
art1.default <- function(x, dimX, dimY, nClusters=nrow(x), maxit=100, 
    initFunc="ART1_Weights", initFuncParams=c(1.0, 1.0), 
    learnFunc="ART1", learnFuncParams=c(0.9, 0.0, 0.0), 
    updateFunc="ART1_Stable", updateFuncParams=c(0.0),    
    shufflePatterns=TRUE) {
  
  x <- as.matrix(x)

  nInputs <- dim(x)[2L]
  
  snnsObject <- SnnsRObjectFactory()
 
  snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')
  snnsObject$art1_createNet(dimX*dimY,dimX,nClusters,dimX)
  
  trainResult <- snnsObject$train(inputsTrain=x, targetsTrain=NULL, initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams, outputMethod="art1", maxit=maxit, 
      shufflePatterns=shufflePatterns, computeError=FALSE)
 
  snns <- NULL
  snns$nInputs <- nInputs
  snns$nClusters <- nClusters
  snns$dimX <- dimX
  snns$dimY <- dimY
  snns$predictions <- matrixToActMapList(trainResult$fitValues, nrow=dimX)
  snns$snnsObject <- snnsObject
  
  class(snns) <- c("art1", "association")
  snns
}


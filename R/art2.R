#' Create and train an art2 network.
#'
#' @export
#' @author Christoph
art2 <- function(x, ...) UseMethod("art2")

#' Create and train an art2 network.
#'
#' @export
#' @author Christoph
art2.default <- function(x, f2Units=5, maxit=100, 
    initFunc="ART2_Weights", initFuncParams=c(0.9, 2.0), 
    learnFunc="ART2", learnFuncParams=c(0.98, 10.0, 10.0, 0.1, 0.0), 
    updateFunc="ART2_Stable", updateFuncParams=c(0.98, 10.0, 10.0, 0.1, 0.0),    
    shufflePatterns=TRUE) {
  
  
  x <- as.matrix(x)
  
  nInputs <- dim(x)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("art2", "association"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)
  
  snns$archParams <- list(f2Units=f2Units)
  
  #snns$snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')
  snns$snnsObject$art2_createNet(nInputs,nInputs,f2Units,f2Units)

  snns <- train.rsnns(snns, inputsTrain=x)
  
  snns
}


#' Create and train an mlp.
#'
#' @export
#' @author Christoph
mlp <- function(x, ...) UseMethod("mlp")

#' Create and train an mlp.
#'
#' @export
#' @author Christoph
mlp.default <- function(x, y, size=c(5), decay=0.2, maxit=100, type="regression") {
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  linOut <- FALSE
  if(type=="regression") linOut <- TRUE
  
  snnsObject <- SnnsRObjectFactory()
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snnsObject$createFullyConnectedFeedForwardNet(unitDefaults = c(0,0,1,0,1,"Act_Logistic","Out_Identity"), linOut,
                                                updateFunc="Topological_Order", 
                                                unitsPerLayer=c(nInputs, size, nOutputs))
  
  #snnsObject$initializeNet(-1)
  snnsObject$initializeNet(c(-0.3, 0.3), "Randomize_Weights")

  error <- snnsObject$train(x, y, learnFunc="Quickprop", learnFuncParams=c(decay, 0, 0, 0), maxit=maxit, shufflePatterns=TRUE)
  
  snns <- NULL
  snns$nInputs <- nInputs
  snns$nOutputs <- nOutputs
  snns$type <- type
  
  snns$generalErrorIterations <- as.matrix(error)
  fit <- snnsObject$predictValues(x)
  
  snns$fitted.values <- fit
  snns$snnsObject <- snnsObject
  
  class(snns) <- c("mlp", "rsnns")
  snns  
}


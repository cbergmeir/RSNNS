#' Create and train an rbf with the DDA algorithm.
#'
#' As the output is winner-takes-all, this type of net can only be used
#' for classification. The output of the iterative error is meaningless
#'
#' @export
#' @author Christoph
rbfDDA <- function(x, ...) UseMethod("rbfDDA")

#' Create and train an rbf with the DDA algorithm.
#'
#' As the output is winner-takes-all, this type of net can only be used
#' for classification. The output of the iterative error is meaningless
#' 
#' @export
#' @author Christoph
rbfDDA.default <- function(x, y, size=c(5), decay=0.2, maxit=100, type="regression", inputsTest=NULL, targetsTest=NULL) {
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  linOut <- FALSE
  if(type=="regression") linOut <- TRUE
  
  snnsObject <- SnnsRObjectFactory()
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snnsObject$setUpdateFunc("Topological_Order")
  snnsObject$setUnitDefaults(c(0,0,1,0,1,"Act_Logistic","Out_Identity"))
  snnsObject$createNet(c(nInputs,nOutputs), linOut = FALSE, fullyConnectedFeedForward = FALSE)
  result <- snnsObject$train(x, y, learnFunc="RBF-DDA", learnFuncParams=c(0.4, 0.2, 5), maxit=maxit, shufflePatterns=TRUE, inputsTest=inputsTest, targetsTest=targetsTest)

  snns <- rsnnsObjectFactory(nInputs, nOutputs, type, snnsObject, "rbfDDA", result)
    
  snns  
}


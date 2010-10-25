#' Create and train an rbf (not working so far).
#'
#' @export
#' @author Christoph
rbf <- function(x, ...) UseMethod("rbfDDA")

#' Create and train an rbf (not working so far).
#' 
#' @export
#' @author Christoph
rbf.default <- function(x, y, size=c(5), decay=0.2, maxit=100, type="regression", inputsTest=NULL, targetsTest=NULL) {
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  linOut <- FALSE
  if(type=="regression") linOut <- TRUE
  
  snnsObject <- SnnsRObjectFactory()
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
 
  #In order to get it working, the learnFuncParams have to be changes (currently, these are the ones of rbf-dda),
  #Act_Funcs of input and output neurons have to be changed,
  #and maybe some other things. See http://www.csc.kth.se/~orre/snns-manual/UserManual/node191.html
  
  snnsObject$setUpdateFunc('Topological_Order')
  snnsObject$setUnitDefaults(0,0,1,0,1,'Act_RBF_Gaussian','Out_Identity')
  snnsObject$createNet(c(nInputs,size,nOutputs), linOut = FALSE, fullyConnectedFeedForward = TRUE)
  snnsObject$initializeNet(c(1.0,  -1.0,  0.3,  1.0,  0.5) , "RBF_Weights")
  result <- snnsObject$train(x, y, learnFunc="RadialBasisLearning", learnFuncParams=c(0.4, 0.2, 5), maxit=maxit, shufflePatterns=TRUE, inputsTest=inputsTest, targetsTest=targetsTest)

  snns <- reg_classObjectFactory(nInputs, nOutputs, type, snnsObject, "rbfDDA", result)
    
  snns  
}


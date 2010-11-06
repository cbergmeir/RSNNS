#' Create and train an rbf (not working so far).
#'
#' @export
rbf <- function(x, ...) UseMethod("rbf")

#' Create and train an rbf (not working so far).
#' 
#' @param x a matrix with training inputs for the network
#' @param y the corresponding targets values
#' @param size number of units in the hidden layer(s)
#' @param maxit maximum of iterations to learn
#' @param initFunc the initialization function to use
#' @param initFuncParams the parameters for the initialization function
#' @param learnFunc the learning function to use
#' @param learnFuncParams the parameters for the learning function
#' @param updateFunc the update function to use
#' @param updateFuncParams the parameters for the update function
#' @param shufflePatterns should the patterns be shuffled?
#' @param linOut sets the activation function of the output units to linear or logistic
#' @param inputsTest a matrix with inputs to test the network
#' @param targetsTest the corresponding targets for the test input
#' @param ... additional function parameters (currently not used)
#' @export
#' @S3method rbf default
#' @method rbf default
#' @rdname rbf
rbf.default <- function(x, y, size=c(5), maxit=100, 
    initFunc="RBF_Weights", initFuncParams=c(0.0,  1.0,  0.0,  0.02,  0.0), 
    learnFunc="RadialBasisLearning", learnFuncParams=c(0.01, 0, 0.01, 0.1, 0.8), 
    updateFunc="Topological_Order", updateFuncParams=c(0.0),
    shufflePatterns=TRUE, linOut=FALSE,
    inputsTest=NULL, targetsTest=NULL, ...) {
  
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("rbf", "reg_class"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=TRUE)
  
  snns$archParams <- list(size=size)
  
  #In order to get it working, maybe the learnFuncParams have to be changed,
  #Act_Funcs of input and output neurons have to be changed,
  #and maybe some other things. See http://www.csc.kth.se/~orre/snns-manual/UserManual/node191.html
  
  snns$snnsObject$setUnitDefaults(0,0,1,0,1,'Act_Logistic','Out_Identity')
  snns$snnsObject$createNet(c(nInputs,size,nOutputs), fullyConnectedFeedForward = TRUE)
  
  if(linOut) {
    outputActFunc <- "Act_IdentityPlusBias"
  } else {
    outputActFunc <- "Act_Logistic"
  }
  
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_INPUT", "Act_Identity")
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_HIDDEN", "Act_RBF_Gaussian")
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_OUTPUT", outputActFunc)
  
  
  snns <- train(snns, inputsTrain=x, targetsTrain=y, inputsTest=inputsTest, targetsTest=targetsTest)
  
  snns
}


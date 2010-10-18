#' Create and train an elman network.
#'
#' @export
#' @author Christoph
elman <- function(x, ...) UseMethod("elman")

#' Create and train an elman network.
#'
#' @export
#' @author Christoph
elman.default <- function(x, y, size=c(5), decay=0.2, maxit=100, type="regression", inputsTest=NULL, targetsTest=NULL) {
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  snnsObject <- SnnsRObjectFactory()
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snnsObject$setLearnFunc('JE_BP')
  
  snnsObject$createElmanNet(unitDefaults = c(1,0,1,0,1,"Act_Logistic","Out_Identity"), 
                            updateFunc="JE_Order", arch=c(nInputs, size, nOutputs), 
                            columns=seq(1,1,length=(length(size)+2)), outContext=FALSE)
    
  snnsObject$initializeNet(c(1.0,  -1.0,  0.3,  1.0,  0.5) )
  
  result <- snnsObject$train(x, y, learnFunc="JE_BP", learnFuncParams=c(decay, 0, 0, 0), maxit=maxit, 
                              shufflePatterns=TRUE, inputsTest=inputsTest, targetsTest=targetsTest)

  snns <- rsnnsObjectFactory(nInputs, nOutputs, type, snnsObject, "elman", result)
  
  snns  
}

#' Create and train a jordan network.
#'
#' @export
#' @author Christoph
jordan <- function(x, ...) UseMethod("jordan")

#' Create and train a jordan network.
#'
#' @export
#' @author Christoph
jordan.default <- function(x, y, size=c(5), decay=0.2, maxit=100, type="regression", inputsTest=NULL, targetsTest=NULL) {
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  snnsObject <- SnnsRObjectFactory()
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snnsObject$setLearnFunc('JE_BP')
  
  snnsObject$createJordanNet(unitDefaults = c(1,0,1,0,1,"Act_Logistic","Out_Identity"), 
      updateFunc="JE_Order", arch=c(nInputs, size, nOutputs), 
      columns=seq(1,1,length=(length(size)+2)))
  
  snnsObject$initializeNet(c(1.0,  -1.0,  0.3,  1.0,  0.5) )
  
  result <- snnsObject$train(x, y, learnFunc="JE_BP", learnFuncParams=c(decay, 0, 0, 0), maxit=maxit, shufflePatterns=TRUE, inputsTest=inputsTest, targetsTest=targetsTest)
  
  snns <- rsnnsObjectFactory(nInputs, nOutputs, type, snnsObject, "jordan", result)
  
  snns  
}

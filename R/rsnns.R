#' Generic print function for rsnns objects.
#'
#' Print out some characteristics of any network object
#' 
#' @param object the rsnns object
#' @export
#' @author Christoph
print.rsnns <- function(object, ...)
{
  if(!inherits(object, "rsnns")) stop("not a legitimate rsnns model")
  
  cat("Class: ", paste(class(object), sep="", collapse="->"), "\n", sep="")
  cat("Number of inputs:",object$nInputs, "\n",sep=" ")
  cat("Number of outputs:",object$nOutputs, "\n",sep=" ")
  cat("Maximal iterations:",object$maxit, "\n",sep=" ")
  cat("Initialization function:",object$initFunc, "\n",sep=" ")
  cat("Initialization function parameters:",object$initFuncParams, "\n",sep=" ")
  cat("Learning function:",object$learnFunc, "\n",sep=" ")  
  cat("Learning function parameters:",object$learnFuncParams, "\n",sep=" ")
  cat("Update function:",object$updateFunc, "\n",sep="")
  cat("Update function parameters:",object$updateFuncParams, "\n",sep=" ")  
  cat("Patterns are shuffled internally:",object$shufflePatterns, "\n",sep=" ")
  cat("Compute error in every iteration:",object$computeIterativeError, "\n",sep=" ")  
  cat("Architecture Parameters:\n",sep="")
  print(object$archParams)
  cat("All members of model:\n",sep="") 
  print(names(object))
}

#' Generic summary function for reg_class objects.
#'
#' Print out a summary any network object the funciton calls the SNNS saveNet function to
#' save the net to a temporary file, then reads this file in, displays its contents and
#' deletes the file 
#' 
#' @param object the reg_class object
#' @export
#' @author Christoph
summary.rsnns <- function(object)
{
  if(!inherits(object, "rsnns")) stop("not a legitimate rsnns model")
  filename <- tempfile(pattern = "rsnns")
  object$snnsObject$saveNet(filename, " ")
  file <- file(filename, "r")
  s <- readLines(file)
  close(file)
  unlink(filename)
  s
}


rsnnsObjectFactory <- function(subclass, nInputs, maxit, 
    initFunc, initFuncParams, 
    learnFunc, learnFuncParams, 
    updateFunc, updateFuncParams, 
    shufflePatterns=TRUE, computeIterativeError=TRUE) {
  
  snns <- NULL
  
  snns$nInputs <- nInputs
  snns$maxit <- maxit  
  
  snns$initFunc <- initFunc
  snns$initFuncParams <- initFuncParams 
  snns$learnFunc <- learnFunc
  snns$learnFuncParams <- learnFuncParams  
  snns$updateFunc <- updateFunc
  snns$updateFuncParams <- updateFuncParams    
  snns$shufflePatterns <- shufflePatterns
  snns$computeIterativeError <- computeIterativeError
  
  snns$snnsObject <- SnnsRObjectFactory()
  
  class(snns) <- c(subclass, "rsnns")
 
  #snns$IterativeFitError <- result$IterativeFitError
  #snns$IterativeTestError <- result$IterativeTestError
  
  #snns$fitted.values <- result$fitValues
  #snns$fittedTestValues <- result$testValues
    
  snns
}

train.rsnns <- function(object, inputsTrain, targetsTrain=NULL, inputsTest=NULL, targetsTest=NULL) {
  
  if(!inherits(object, "rsnns")) stop("not a legitimate rsnns model")
  
  trainResult <- object$snnsObject$train(inputsTrain=inputsTrain, targetsTrain=targetsTrain, 
      initFunc=object$initFunc, initFuncParams=object$initFuncParams, 
      learnFunc=object$learnFunc, learnFuncParams=object$learnFuncParams, updateFunc=object$updateFunc, 
      updateFuncParams=object$updateFuncParams, outputMethod=class(object)[1], maxit=object$maxit, 
      shufflePatterns=object$shufflePatterns, computeError=object$computeIterativeError, 
      inputsTest=inputsTest, targetsTest=targetsTest)
  
  object$IterativeFitError <- trainResult$IterativeFitError
  object$IterativeTestError <- trainResult$IterativeTestError
  
  object$fitted.values <- trainResult$fitValues
  object$fittedTestValues <- trainResult$testValues
  
  object$nOutputs <- ncol(trainResult$fitValues)
  object
}

#' Generic predict function.
#' 
#' Predict values using the given network. This is only meaningful with regression and classification
#' for a clustering network, another function has to be implemented 
#'
#' @param object the reg_class object
#' @param newdata the new input data which is used for prediction
#' @param type is the new data regression or classification data?
#' @export
#' @author Christoph
predict.rsnns <- function(object, newdata)
{
  if(!inherits(object, "rsnns")) stop("not a legitimate reg_class model")
  #type <- match.arg(type)
  if(missing(newdata)) z <- fitted(object)
  else {
    if(is.null(dim(newdata)))
      dim(newdata) <- c(1L, length(newdata)) # a row vector
    x <- as.matrix(newdata)     # to cope with dataframes
    if(any(is.na(x))) stop("missing values in 'x'")
    
    keep <- 1L:nrow(x)
    rn <- rownames(x)
    
    ntr <- nrow(x)
    nout <- object$nOutputs
    
    z <- matrix(NA, nrow(newdata), nout, dimnames = list(rn, dimnames(object$fitted.values)[[2L]]))
    
    #predict values.. 
    patSet <- object$snnsObject$createPatSet(newdata) 
    predictions <- object$snnsObject$predictCurrPatSet(class(object)[1], updateFuncParams=object$updateFuncParams)
    object$snnsObject$deletePatSet(patSet$set_no)
    z[keep,] <- predictions
  }
  z
  #predictions
}
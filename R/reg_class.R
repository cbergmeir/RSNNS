

## Generic summary function for reg_class objects.
##
## Print out a summary any network object the funciton calls the SNNS saveNet function to
## save the net to a temporary file, then reads this file in, displays its contents and
## deletes the file 
## 
## @param object the reg_class object
## @export
## @author Christoph
#summary.reg_class <- function(object)
#{
#  if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
#  filename <- tempfile(pattern = "reg_class")
#  object$snnsObject$saveNet(filename, " ")
#  file <- file(filename, "r")
#  s <- readLines(file)
#  close(file)
#  unlink(filename)
#  s
#}

## Generic predict function.
## 
## Predict values using the given network. This is only meaningful with regression and classification
## for a clustering network, another function has to be implemented 
##
## @param object the reg_class object
## @param newdata the new input data which is used for prediction
## @param type is the new data regression or classification data?
## @export
## @author Christoph
#predict.reg_class <- function(object, newdata, type=c("regression","classification"), ...)
#{
#  if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
#  type <- match.arg(type)
#  if(missing(newdata)) z <- fitted(object)
#  else {
#    if(is.null(dim(newdata)))
#      dim(newdata) <- c(1L, length(newdata)) # a row vector
#    x <- as.matrix(newdata)     # to cope with dataframes
#    if(any(is.na(x))) stop("missing values in 'x'")
#    
#    keep <- 1L:nrow(x)
#    rn <- rownames(x)
#    
#    ntr <- nrow(x)
#    nout <- object$nOutputs
#    
#    z <- matrix(NA, nrow(newdata), nout,
#        dimnames = list(rn, dimnames(object$fitted.values)[[2L]]))
#    
#    #predict values.. 
#    patSet <- object$snnsObject$createPatSet(newdata) 
#    predictions <- object$snnsObject$predictCurrPatSet("output")
#    object$snnsObject$deletePatSet(patSet$set_no)
#    z[keep,] <- predictions
#  }
#  z
#  #predictions
#}

#' Plot the iterative training and test error during training of the net.
#'
#' @export
#' @author Christoph
plotIterativeError <- function(object, ...)
{
  if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
  
  if(object$computeIterativeError) {
    plot(object$IterativeFitError, ylab="Weighted SSE", xlab="Iteration", type="l", ...)
    
    if(!is.null(object$IterativeTestError)) {
      
      testSetRatio <- nrow(as.matrix(object$fitted.values)) / nrow(as.matrix(object$fittedTestValues)) 
      
      lines(object$IterativeTestError * testSetRatio, col="red")    
    }    
  } else {
    print("Iterative error was not computed during training")
  }
  
}

#reg_classSetResult <- function(object, nOutputs, result) {
#  
#  if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
#  
##  rsnnsObjectFactory <- function(subclass, nInputs, nOutputs, maxit, 
##      initFunc, initFuncParams, 
##      learnFunc, learnFuncParams, 
##      updateFunc, updateFuncParams, 
##      shufflePatterns=TRUE, computeIterativeError=TRUE)
##    
##  snns <- rsnnsObjectFactory(subclass=c("art1", "association"), nInputs=nInputs, maxit=maxit, 
##      initFunc=initFunc, initFuncParams=initFuncParams, 
##      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
##      updateFunc=updateFunc, 
##      updateFuncParams=updateFuncParams,
##      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)
##  
##  
##  snns <- NULL
##  snns$nInputs <- nInputs
#  object$nOutputs <- nOutputs
#
#  
#  
##  snns$snnsObject <- snnsObject
#  
##  class(snns) <- c(subclass, "reg_class", "rsnns")
#  
#  #if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
#  
#  object$IterativeFitError <- result$IterativeFitError
#  object$IterativeTestError <- result$IterativeTestError
#  
#  object$fitted.values <- result$fitValues #rbind(result$fitValues, result$testValues)
#  object$fittedTestValues <- result$testValues
#  #snns$testSetRatio <- testSetRatio
#  
#  object
#}
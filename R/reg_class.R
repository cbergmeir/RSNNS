#' Generic print function for reg_class objects.
#'
#' Print out some characteristics of any network object
#' 
#' @param object the reg_class object
#' @export
#' @author Christoph
print.reg_class <- function(object, ...)
{
  if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
  
  cat("nInputs: ",object$nInputs, "\n",sep="")
  cat("nOutputs: ",object$nOutputs, "\n", sep="")
  cat("Network type: ",object$type, "\n", sep="")
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
summary.reg_class <- function(object)
{
  if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
  filename <- tempfile(pattern = "reg_class")
  object$snnsObject$saveNet(filename, " ")
  file <- file(filename, "r")
  s <- readLines(file)
  close(file)
  unlink(filename)
  s
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
predict.reg_class <- function(object, newdata, type=c("regression","classification"), ...)
{
  if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
  type <- match.arg(type)
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
    
    z <- matrix(NA, nrow(newdata), nout,
        dimnames = list(rn, dimnames(object$fitted.values)[[2L]]))
    
    #predict values.. 
    patSet <- object$snnsObject$createPatSet(newdata) 
    predictions <- object$snnsObject$predictCurrPatSet("reg_class")
    object$snnsObject$deletePatSet(patSet$set_no)
    z[keep,] <- predictions
  }
  z
  #predictions
}

#' Plot the iterative training and test error during training of the net.
#'
#' @export
#' @author Christoph
plotIterativeError <- function(object, ...)
{
  if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
  
  plot(object$IterativeFitError, ylab="Weighted SSE", xlab="Iteration", type="l", ...)
  
  if(!is.null(object$IterativeTestError)) {
    
    testSetRatio <- nrow(as.matrix(object$fitted.values)) / nrow(as.matrix(object$fittedTestValues)) 
    
    lines(object$IterativeTestError * testSetRatio, col="red")    
  }
  
}

reg_classObjectFactory <- function(nInputs, nOutputs, type, snnsObject, subclass, result) {
  
  snns <- NULL
  snns$nInputs <- nInputs
  snns$nOutputs <- nOutputs
  snns$type <- type
  
  
  snns$snnsObject <- snnsObject
  
  class(snns) <- c(subclass, "reg_class")
  
  #if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
  
  snns$IterativeFitError <- result$IterativeFitError
  snns$IterativeTestError <- result$IterativeTestError
  
  snns$fitted.values <- result$fitValues #rbind(result$fitValues, result$testValues)
  snns$fittedTestValues <- result$testValues
  #snns$testSetRatio <- testSetRatio
  
  snns
}
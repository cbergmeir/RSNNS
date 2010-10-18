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
  
  cat("nInputs: ",object$nInputs, "\n",sep="")
  cat("nOutputs: ",object$nOutputs, "\n", sep="")
  cat("Network type: ",object$type, "\n", sep="")
}

#' Generic summary function for rsnns objects.
#'
#' Print out a summary any network object the funciton calls the SNNS saveNet function to
#' save the net to a temporary file, then reads this file in, displays its contents and
#' deletes the file 
#' 
#' @param object the rsnns object
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

#' Generic predict function.
#' 
#' Predict values using the given network. This is only meaningful with regression and classification
#' for a clustering network, another function has to be implemented 
#'
#' @param object the rsnns object
#' @param newdata the new input data which is used for prediction
#' @param type is the new data regression or classification data?
#' @export
#' @author Christoph
predict.rsnns <- function(object, newdata, type=c("regression","classification"), ...)
{
  if(!inherits(object, "rsnns")) stop("not a legitimate rsnns model")
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
    patset <- object$snnsObject$createPatterns(newdata) 
    object$snnsObject$setCurrPatSet(patset$set_no)
    predictions <- object$snnsObject$predictValuesCurrPatSet()
    object$snnsObject$deletePatSet(patset$set_no)
    z[keep,] <- predictions
  }
  z
  #predictions
}

#' Plot the iterative training and test error during training of the net.
#'
#' @export
#' @author Christoph
plotIterativeError <- function(object)
{
  if(!inherits(object, "rsnns")) stop("not a legitimate rsnns model")
  
  if(object$testSetRatio != 0) {
    plot(object$IterativeFitError, type="l")
    lines(object$IterativeTestError / object$testSetRatio, col="red")    
  } else {
    plot(object$IterativeFitError, ylab="Weighted SSE", xlab="Iteration", type="l")
  }
  
}

#############################################################################
#
#   This file is part of the R package "RSNNS".
#
#   Author: Christoph Bergmeir
#   Supervisor: José M. Benítez
#   Copyright (c) DiCITS Lab, Sci2s group, DECSAI, University of Granada.
#
#   This library is free software; you can redistribute it and/or
#   modify it under the terms of the GNU Library General Public
#   License as published by the Free Software Foundation; either
#   version 2 of the License, or (at your option) any later version.
# 
#   This library is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   Library General Public License for more details.
# 
#   You should have received a copy of the GNU Library General Public License
#   along with this library; see the file COPYING.LIB.  If not, write to
#   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
#   Boston, MA 02110-1301, USA.
#
#############################################################################


#' Generic print function for rsnns objects.
#'
#' Print out some characteristics of an rsnns object.
#' 
#' @param x the rsnns object
#' @param ... additional function parameters (currently not used)
#' @export
#' @S3method print rsnns
#' @method print rsnns
# @rdname rsnns
print.rsnns <- function(x, ...)
{
  if(!inherits(x, "rsnns")) stop("not a legitimate rsnns model")
  
  cat("Class: ", paste(class(x), sep="", collapse="->"), "\n", sep="")
  cat("Number of inputs:",x$nInputs, "\n",sep=" ")
  cat("Number of outputs:",x$nOutputs, "\n",sep=" ")
  cat("Maximal iterations:",x$maxit, "\n",sep=" ")
  cat("Initialization function:",x$initFunc, "\n",sep=" ")
  cat("Initialization function parameters:",x$initFuncParams, "\n",sep=" ")
  cat("Learning function:",x$learnFunc, "\n",sep=" ")  
  cat("Learning function parameters:",x$learnFuncParams, "\n",sep=" ")
  cat("Update function:",x$updateFunc, "\n",sep="")
  cat("Update function parameters:",x$updateFuncParams, "\n",sep=" ")  
  cat("Patterns are shuffled internally:",x$shufflePatterns, "\n",sep=" ")
  cat("Compute error in every iteration:",x$computeIterativeError, "\n",sep=" ")  
  cat("Architecture Parameters:\n",sep="")
  print(x$archParams)
  cat("All members of model:\n",sep="") 
  print(names(x))
}

#' Generic summary function for rsnns objects.
#'
#' Print out a summary any network object the funciton calls the SNNS saveNet function to
#' save the net to a temporary file, then reads this file in, displays its contents and
#' deletes the file 
#' 
#' @param object the rsnns object
#' @param ... additional function parameters (currently not used)
#' @export
#' @S3method summary rsnns
#' @method summary rsnns
# @rdname rsnns
summary.rsnns <- function(object, ...)
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

#' Object factory for generating objects of type rsnns.
#'
#' The object factory initializes member variables of object 
#' with the values given as parameters and generates an object of type \link{SnnsR}
#' Later, during training with \link{train}, this information is used to train the network.
#' TODO
#' Create and train a multi layer perceptron. 
#' Most of the parameters are directly passed to \link{rsnnsObjectFactory} or \link{train}.
#' The method follows the typical procedure for rsnns objects: 
#' \itemize{
#' \item generate the rsnns object (with \link{rsnnsObjectFactory})
#' \item generate the network according to the architecture given
#' \item train the network (with \link{train})
#' }
#'
#' 
#'
#' @param subclass the subclass of rsnns to generate (vector of strings)
#' @param nInputs the number of inputs the network will have
#' @param maxit maximum of iterations to learn
#' @param initFunc the initialization function to use
#' @param initFuncParams the parameters for the initialization function
#' @param learnFunc the learning function to use
#' @param learnFuncParams the parameters for the learning function
#' @param updateFunc the update function to use
#' @param updateFuncParams the parameters for the update function
#' @param shufflePatterns should the patterns be shuffled?
#' @param computeIterativeError should the error be computed in every iteration? 
#' @export
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
    
  snns
}

#' Generic train function.
#'
#' @param object the object to which to apply train
#' @param ... additional function parameters
#' @export
train <- function(object, ...) UseMethod("train")

#' Generic train function for rsnns objects.
#'
#' The function calls \link{SnnsRObject$train} and saves the result in the
#' current rsnns object
#' 
#' @param object the rsnns object
#' @param inputsTrain training input
#' @param targetsTrain training targets
#' @param inputsTest test input
#' @param targetsTest test targets
#' @param ... additional function parameters (currently not used)
#' @export
#' @S3method train rsnns
#' @method train rsnns
#' @rdname train
train.rsnns <- function(object, inputsTrain, targetsTrain=NULL, inputsTest=NULL, targetsTest=NULL, ...) {
  
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

#' Generic predict function for rsnns object.
#' 
#' Predict values using the given network. This is only meaningful with regression and classification
#' for a clustering network, another function has to be implemented 
#'
#' @param object the rsnns object
#' @param newdata the new input data which is used for prediction
#' @param ... additional function parameters (currently not used)
#' @S3method predict rsnns
#' @method predict rsnns
# @rdname rsnns
#' @export
predict.rsnns <- function(object, newdata, ...)
{
  if(!inherits(object, "rsnns")) stop("not a legitimate rsnns model")
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
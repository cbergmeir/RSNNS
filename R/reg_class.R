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


#' Function to split data into training and test set.
#'
#' Split the input and target values to a training and a test set. Test set is taken from the end of the
#' data, if shuffling is wanted, it has to be done as preprocessing to this function.
#' 
#' @param x inputs
#' @param y targets
#' @param ratio ratio of training and test sets (default: 15\% of the data is used for testing)
#' @return a named list with the following elements:
#' \item{inputsTrain}{a matrix containing the training inputs}
#' \item{targetsTrain}{a matrix containing the training targets}
#' \item{inputsTest}{a matrix containing the test inputs}
#' \item{targetsTest}{a matrix containing the test targets}
#' @export
#' @examples
#' data(iris)
#' #shuffle the vector
#' iris <- iris[sample(1:nrow(iris),length(1:nrow(iris))),1:ncol(iris)]
#' irisValues <- normalizeData(iris[,1:4], "norm")
#' irisTargets <- decodeClassLabels(iris[,5])
#' splitForTrainingAndTest(irisValues, irisTargets, ratio=0.15)
splitForTrainingAndTest <- function(x, y, ratio=0.15) {
  
  x <- as.matrix(x)
  nInputs <- nrow(x)
  
  y <- as.matrix(y)
  
  trainIndices <- 1 : (nInputs * (1-ratio))
  testIndices <- (1:nInputs)[-trainIndices]
  
  inputsTrain <- x[trainIndices,]
  targetsTrain <- y[trainIndices,]
  
  inputsTest <- x[testIndices,]
  targetsTest <- y[testIndices,]
  
  return(list(inputsTrain=inputsTrain, targetsTrain=targetsTrain, inputsTest=inputsTest, targetsTest=targetsTest))
  
}

## Check the input of a reg_class object for eventual problems.
checkInput <- function(x,y) {
  
  ok <- TRUE
  
  if(any(is.na(x))) {
    stop("missing values in 'x'")
    ok <- FALSE
  }
  if(any(is.na(y))) {
    stop("missing values in 'y'")
    ok <- FALSE
  }
  if(dim(x)[1L] != dim(y)[1L]) {
    stop("nrows of 'x' and 'y' must match")
    ok <- FALSE    
  }
  
  ok
}

#' Plot the iterative training and test error during training of the net.
#'
#' Plots (if present) the class members IterativeFitError (as black line) and 
#' IterativeTestError (as red line).
#' 
#' @param object the object to which to apply plotIterativeError
#' @param ... additional function parameters
#' @export
plotIterativeError <- function(object, ...) UseMethod("plotIterativeError")

#' Plot the iterative training and test error of the net of this rsnns object.
#' 
#' @param object a reg_class object
#' @param ... parameters passed to \code{plot}
#' @export
#' @S3method plotIterativeError reg_class
#' @method plotIterativeError reg_class
#' @rdname plotIterativeError
plotIterativeError.reg_class <- function(object, ...)
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


#' Decode class labels from a numerical or levels vector to a binary matrix.
#'
#' Convert the input vector to a binary matrix, which has the value \code{valTrue} (e.g. 1) exactly in
#' the column given by the value in the input vector, and the value \code{valFalse} (e.g. 0) in the other 
#' columns. The number of columns of the resulting matrix depends on the number of unique 
#' labels found in the vector. E.g. the input c(1, 3, 2, 3) will result in an output matrix with rows: 
#' 100 001 010 001
#' 
#' @param x class label vector
#' @param valTrue see description above 
#' @param valFalse see description above
#' @return a binary matrix. See description above
#' @export
#' @examples
#' decodeClassLabels(c(1,3,2,3))
#' 
#' data(iris)
#' decodeClassLabels(iris[,5])
decodeClassLabels <- function(x, valTrue=1, valFalse=0) {
  
  #y <- gl(2, 4, 8)
  #levels(y) <- c("low", "high")
  #if(length(levels(y))!=0) {}

  classes <- unique(x)
  numClasses <- 1:length(classes)
  names(numClasses) <- classes

  targets <- matrix(nrow=length(x), ncol=length(classes))
  for(i in 1:length(x))
    for(j in numClasses)  {
      if(j == numClasses[x[i]]) {
        targets[i,j] <- valTrue  
      }
      else {
        targets[i,j] <- valFalse
      }
    }
  
  colnames(targets) <- classes
  targets
}

#' Applies \code{analyzeClassification} row-wise to a matrix.
#' 
#' @param x inputs
#' @param method same as in analyzeClassification
#' @param l idem
#' @param h idem
#' @return a numeric vector, each number represents a different class. A zero means,
#' that no class was assigned to the pattern. 
#' @export
#' @seealso \code{\link{analyzeClassification}}
#' @examples 
#' data(iris)
#' labels <- decodeClassLabels(iris[,5])
#' encodeClassLabels(labels)
encodeClassLabels <- function(x, method="WTA", l=0.0, h=0.0) {
  apply(x, 1, function(y) analyzeClassification(y, method, l, h))
}

#' Converts a vector (of class labels) to a numeric vector.
#' 
#' @param x inputs
#' @return the vector converted to a numeric vector
#' @export
#' @examples 
#' data(iris)
#' toNumericClassLabels(iris[,5])
toNumericClassLabels <- function(x) {
  if(is.numeric(x)) return(x)
  else return(as.numeric(x))
}

#' Converts continuous neuronal outputs to class labels.
#' 
#' Three methods are implemented: 402040, WTA and ForcedWTA.
#' 402040 and WTA are implemented as in the SNNS Manual 4.2 (pp 269)
#' 
#' @param y inputs
#' @param method winner-takes-all or 402040
#' @param l lower bound, e.g. in 402040: l=0.4
#' @param h upper bound, e.g. in 402040: h=0.6
#' @return the position of the winning unit, or zero, if no classification was done.
# @references 
# Zell, A. et al. SNNS Stuttgart Neural Network Simulator User Manual, Version 4.2
# \url{http://www.ra.cs.uni-tuebingen.de/SNNS/}
#' @export
#' @seealso \code{\link{encodeClassLabels}}
analyzeClassification <- function(y, method="WTA", l=0.0, h=0.0) {
  
  classes <- length(y)
  resClass <- 0
  
  if(method=="402040") {
    candClass <- which(y >= h)
    
    if(length(candClass) == 1) {
      if(max(y[-candClass]) <= l) {
        resClass <- candClass
      }
    }
    
  } else if(method=="WTA") {
    candClass <- which(y == max(y))
    
    if(length(candClass) == 1) {
      if(y[candClass] > h) {
        if(max(y[-candClass]) < (max(y) - l)) {
          resClass <- candClass
        }        
      }
    }
  } #else if(method=="ForcedWTA") {
    #resClass <- which(y==max(y))[1]
  #}
  
  resClass  
}

#' Computes a confusion matrix.
#' 
#' If the class labels are not already encoded, they are encoded with
#' default values. The confusion matrix shows how many times a pattern
#' with the real class x was classified as class y. A perfect method
#' should result in a diagonal matrix. All values not on the diagonal
#' are errors of the method.
#' 
#' @param targets the known, correct target values
#' @param predictions the corresponding predictions of a method for the targets
#' @return the confusion matrix
#' @export
confusionMatrix <- function(targets, predictions) {
  
  if(is.matrix(targets)) {
    if(ncol(targets)!=1) tr <- encodeClassLabels(targets)
  } else {
    tr <- toNumericClassLabels(targets)  
  }
  
  enc <- FALSE
  if(is.matrix(predictions)) {
    if(ncol(predictions)!=1) {
      pr <- encodeClassLabels(predictions)
      enc <- TRUE
    }
  } 
  
  if(!enc) pr <- toNumericClassLabels(predictions)
  
  cm <- table(targets=tr, predictions=pr)
  #rowSums(cm)
  #colSums(cm)
  cm
}

#' Plot a ROC curve.
#'
#' Code is taken from R news Volume 4/1, June 2004.
#' 
#' @param T predictions
#' @param D targets
#' @references 
#' R news Volume 4/1, June 2004
#' @export
#' @author ...
plotROC <-function(T,D){
  cutpoints<-c(-Inf, sort(unique(T)), Inf)
  sens<-sapply(cutpoints,
      function(c) sum(D[T>c])/sum(D))
  spec<-sapply(cutpoints,
      function(c) sum((1-D)[T<=c]/sum(1-D)))
  plot(1-spec, sens, type="l")
}


#' Plot a regression error plot.
#' 
#' The plot shows target values on the x-axis and fitted/predicted values on the y-axis. 
#' The optimal fit would yield a line through zero with gradient one.
#' This optimal line is shown in black color. A linear fit to the actual data
#'  is shown in red color.
#' 
#' @param targets the target values
#' @param fits the values predicted/fitted by the model
#' @param ... parameters passed to \code{plot}
#' @export
plotRegressionError <- function(targets, fits, ...)
{
  #if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
  
  plot(targets, fits, xlim=c(0,1), ylim=c(0,1), ...)
  
  linMod <- lm(fits ~ targets)
  abline(linMod, col="red")
  lines(c(0,1), c(0,1))
  
}

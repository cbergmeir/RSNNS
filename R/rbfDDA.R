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


#' Create and train a rbf network with the DDA algorithm.
#'
#' As the output is winner-takes-all, this type of net can only be used
#' for classification.
#'
#' @references 
#' Berthold, M. R. & Diamond, J. (1995), Boosting the Performance of RBF Networks with Dynamic Decay Adjustment, in 'Advances in Neural Information Processing Systems', MIT Press, , pp. 521--528.
#' 
#' Hudak, M. (1993), 'RCE classifiers: theory and practice', Cybernetics and Systems 23(5), 483--515.
#' @export
rbfDDA <- function(x, ...) UseMethod("rbfDDA")

#' Create and train an rbf with the DDA algorithm.
#'
#' @param x a matrix with training inputs for the network
#' @param y the corresponding targets values
#' @param maxit maximum of iterations to learn
#' @param initFunc the initialization function to use
#' @param initFuncParams the parameters for the initialization function
#' @param learnFunc the learning function to use
#' @param learnFuncParams the parameters for the learning function
#' @param updateFunc the update function to use
#' @param updateFuncParams the parameters for the update function
#' @param shufflePatterns should the patterns be shuffled?
#' @param linOut sets the activation function of the output units to linear or logistic
#' @param ... additional function parameters (currently not used)
#' @return an \code{\link{rsnns}} object.
#' @export
#' @S3method rbfDDA default
#' @method rbfDDA default
#' @rdname rbfDDA
#' @examples 
#' \dontrun{demo(iris)}
#' \dontrun{demo(rbfDDA_spiralsSnnsR)}
rbfDDA.default <- function(x, y, maxit=1, 
    initFunc="Randomize_Weights", initFuncParams=c(-0.3, 0.3), 
    learnFunc="RBF-DDA", learnFuncParams=c(0.4, 0.2, 5), 
    updateFunc="Topological_Order", updateFuncParams=c(0.0),
    shufflePatterns=TRUE, linOut=FALSE, ...) {
  
  #TODO: does linOut make sense here?  
  #as in the beginning no hidden units are present, the init function is not needed
  #is one iteration always sufficient?
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("rbfDDA", "reg_class"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)
  
  snns$archParams <- NULL#list(size=size)
  
  snns$snnsObject$setUnitDefaults(c(0,0,1,0,1,"Act_Logistic","Out_Identity"))
  snns$snnsObject$createNet(c(nInputs,nOutputs), fullyConnectedFeedForward = FALSE)
  
  if(linOut) {
    outputActFunc <- "Act_Identity"
  } else {
    outputActFunc <- "Act_Logistic"
  }
  
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_INPUT", "Act_Identity")
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_OUTPUT", outputActFunc)
  
  
  snns <- train(snns, inputsTrain=x, targetsTrain=y)
  
  snns

}


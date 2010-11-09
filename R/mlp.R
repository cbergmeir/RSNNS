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


#' Create and train a multi-layer perceptron (mlp).
#'
#' Create and train a multi layer perceptron. 
#' Most of the parameters are directly passed to \link{rsnnsObjectFactory} or \link{train}.
#' The method follows the typical procedure for rsnns objects: 
#' \itemize{
#' \item generate the rsnns object (with \link{rsnnsObjectFactory})
#' \item generate the network according to the architecture given
#' \item train the network (with \link{train})
#' }
#'
#' @export
mlp <- function(x, ...) UseMethod("mlp")

#' Create and train an mlp.
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
#' @param hiddenActFunc the activation function of all hidden units
#' @param shufflePatterns should the patterns be shuffled?
#' @param linOut sets the activation function of the output units to linear or logistic
#' @param inputsTest a matrix with inputs to test the network
#' @param targetsTest the corresponding targets for the test input
#' @param ... additional function parameters (currently not used)
#' @export
#' @S3method mlp default
#' @method mlp default
#' @rdname mlp
mlp.default <- function(x, y, size=c(5), maxit=100, 
    initFunc="Randomize_Weights", initFuncParams=c(-0.3, 0.3), 
    learnFunc="Quickprop", learnFuncParams=c(0.2), 
    updateFunc="Topological_Order", updateFuncParams=c(0.0),
    hiddenActFunc="Act_Logistic",
    shufflePatterns=TRUE, linOut=FALSE, inputsTest=NULL, targetsTest=NULL, ...) {
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)

  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("mlp", "reg_class"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=TRUE)
  
  snns$archParams <- list(size=size)
  
  snns$snnsObject$setUnitDefaults(0,0,1,0,1,"Act_Logistic","Out_Identity")
  snns$snnsObject$createNet(unitsPerLayer=c(nInputs, size, nOutputs), fullyConnectedFeedForward=TRUE)
  
  if(linOut) {
    outputActFunc <- "Act_Identity"
  } else {
    outputActFunc <- "Act_Logistic"
  }
  
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_INPUT", "Act_Identity")
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_HIDDEN", hiddenActFunc)
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_OUTPUT", outputActFunc)
  
  
  snns <- train(snns, inputsTrain=x, targetsTrain=y, inputsTest=inputsTest, targetsTest=targetsTest)

}


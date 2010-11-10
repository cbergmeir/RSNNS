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

#' Create and train a radial basis function (rbf) network.
#'
#' Radial basis functions are feed-forward networks with one hidden layer.
#' They save information about each pattern locally in the net (not globally as mlps)
#' Their use is similar to that of \code{\link{mlp}}. However, their initialization
#' can be difficult and require prior knowledge. Before their use, you probably want
#' to read pp 172-183 of the SNNS User Manual 4.2. The initialization is performed in
#' the current implementation by a call to RBF_Weights_Kohonen with all parameters
#' set to zero and a successive call to the given \code{initFunc} (usually RBF_Weights).
#' If this initialization doesn't fit your needs, you should use the RSNNS low-level interface
#' to implement your own one. Have a look then at the demos/examples. 
#' Also, depending on whether linear or logistic output is chosen, the initialization parameters
#' have to be different (normally c(0,1,...) for linear and c(-4,4,...) for logistic).
#' 
#' @export
rbf <- function(x, ...) UseMethod("rbf")

#' Create and train a radial basis function (rbf) network.
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
#' @examples 
#' \dontrun{demo(rbf_irisSnnsR)}
#' \dontrun{demo(rbf_sin)}
#' \dontrun{demo(rbf_sinSnnsR)}
rbf.default <- function(x, y, size=c(5), maxit=100, 
    initFunc="RBF_Weights", initFuncParams=c(0.0,  1.0,  0.0,  0.02,  0.04), 
    learnFunc="RadialBasisLearning", learnFuncParams=c(1e-5, 0, 1e-5, 0.1, 0.8), 
    updateFunc="Topological_Order", updateFuncParams=c(0.0),
    shufflePatterns=TRUE, linOut=TRUE,
    inputsTest=NULL, targetsTest=NULL, ...) {
  
  if(!is.null(inputsTest)) {
    warning("Supplying test patterns here is not supported for RBFs (due to problems with the testAllPatterns function of the SNNS kernel). Use predict() instead.")
  }
  
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


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


#' Create and train a jordan network.
#'
#' Jordan networks are recurrent networks 
#' and very similar to \code{\link{elman}} networks.
#' 
#' @export
jordan <- function(x, ...) UseMethod("jordan")

#' Create and train a jordan network. 
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
#' @return an \code{\link{rsnns}} object.
#' @export
#' @S3method jordan default
#' @method jordan default
#' @rdname jordan
#' @seealso \code{\link{elman}}
#' @examples 
#' \dontrun{demo(iris)}
#' \dontrun{demo(laser)}
#' \dontrun{demo(eight_elman)}
#' \dontrun{demo(eight_elmanSnnsR)}
jordan.default <- function(x, y, size=c(5), maxit=100, 
    initFunc="JE_Weights", initFuncParams=c(1.0,  -1.0,  0.3,  1.0,  0.5), 
    learnFunc="JE_BP", learnFuncParams=c(0.2), 
    updateFunc="JE_Order", updateFuncParams=c(0.0),    
    shufflePatterns=TRUE, linOut=TRUE, inputsTest=NULL, targetsTest=NULL, ...) {
  
  
  x <- as.matrix(x)
  y <- as.matrix(y)
  
  checkInput(x,y)
  
  nInputs <- dim(x)[2L]
  nOutputs <- dim(y)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("jordan", "reg_class"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=TRUE)
  
  snns$archParams <- list(size=size)
  
  snns$snnsObject$setUnitDefaults(1,0,1,0,1,"Act_Logistic","Out_Identity")
  snns$snnsObject$jordan_createNet(nInputs, size, nOutputs, 1, 1, 1)
  
  
  if(linOut) {
    outputActFunc <- "Act_Identity"
  } else {
    outputActFunc <- "Act_Logistic"
  }
  
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_INPUT", "Act_Identity")
  snns$snnsObject$setTTypeUnitsActFunc("UNIT_OUTPUT", outputActFunc)
  
  
  snns <- train(snns, inputsTrain=x, targetsTrain=y, inputsTest=inputsTest, targetsTest=targetsTest)
  
  snns  
}

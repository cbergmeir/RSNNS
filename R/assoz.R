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


#' Create and train an (auto-)associative memory.
#'
#' Similar to the \code{\link{art1}} and \code{\link{art2}} network types.
#' The implementation assumes two-dimensional input and output (cf. \code{\link{art1}}).
#' 
#' @export
assoz <- function(x, ...) UseMethod("assoz")

#' Create and train an (auto-)associative memory.
#' 
#' @param x a matrix with training inputs for the network
#' @param dimX x dimension of inputs and outputs
#' @param dimY y dimension of inputs and outputs
#' @param maxit maximum of iterations to learn
#' @param initFunc the initialization function to use
#' @param initFuncParams the parameters for the initialization function
#' @param learnFunc the learning function to use
#' @param learnFuncParams the parameters for the learning function
#' @param updateFunc the update function to use
#' @param updateFuncParams the parameters for the update function
#' @param shufflePatterns should the patterns be shuffled?
#' @param ... additional function parameters (currently not used)
#' @return an rsnns object. The \code{fitted.values} member contains the 
#' activation patterns for all inputs
#' @export
#' @S3method assoz default
#' @method assoz default
#' @rdname assoz
#' @seealso \code{\link{art1}}, \code{\link{art2}}
#' @examples 
#' \dontrun{demo(assoz_letters)}
#' \dontrun{demo(assoz_lettersSnnsR)}
assoz.default <- function(x, dimX, dimY, maxit=100, 
    initFunc="RM_Random_Weights", initFuncParams=c(1.0, -1.0), 
    learnFunc="RM_delta", learnFuncParams=c(0.01, 100, 0.0, 0.0, 0.0), 
    updateFunc="Auto_Synchronous", updateFuncParams=c(50.0),    
    shufflePatterns=TRUE, ...) {
  
  
  x <- as.matrix(x)
  
  nInputs <- dim(x)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("assoz", "association"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)
  
  snns$archParams <- list(dimX=dimX, dimY=dimY)
  
  snns$snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Identity','Out_Identity')
  snns$snnsObject$assoz_createNet(dimX, dimY)
  
  snns <- train(snns, inputsTrain=x)
  
  snns$fitted.values <- matrixToActMapList(snns$fitted.values, nrow=dimX)
  
  snns
}


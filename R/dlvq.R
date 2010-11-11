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


#' Create and train a dlvq network.
#'
#' @export
dlvq <- function(x, ...) UseMethod("dlvq")

#' Create and train a dlvq network.
#' 
#' @param x a matrix with training inputs for the network
#' @param y the corresponding target values
#' @param initFunc the initialization function to use
#' @param initFuncParams the parameters for the initialization function
#' @param learnFunc the learning function to use
#' @param learnFuncParams the parameters for the learning function
#' @param updateFunc the update function to use
#' @param updateFuncParams the parameters for the update function
#' @param shufflePatterns should the patterns be shuffled?
#' @param ... additional function parameters (currently not used)
#' @return an \code{\link{rsnns}} object. The \code{fitted.values} member contains the 
#' activation patterns for all inputs.
#' @export
#' @S3method dlvq default
#' @method dlvq default
#' @rdname dlvq
#' @examples 
#' \dontrun{demo(dlvq_ziff)}
#' \dontrun{demo(dlvq_ziffSnnsR)}
dlvq.default <- function(x, y, 
    initFunc="DLVQ_Weights", initFuncParams=c(1.0, -1.0), 
    learnFunc="Dynamic_LVQ", learnFuncParams=c(0.03, 0.03, 10.0), 
    updateFunc="Dynamic_LVQ", updateFuncParams=c(0.0),    
    shufflePatterns=TRUE, ...) {
  
  
  x <- as.matrix(x)
  
  nInputs <- dim(x)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("dlvq", "reg_class"), nInputs=nInputs, maxit=1, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)
  
  snns$archParams <- NULL #list(dimX=dimX, dimY=dimY)
  
  snns$snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Identity','Out_Identity')
  snns$snnsObject$createNet(c(nInputs, 1), fullyConnectedFeedForward = FALSE)
  
  snns <- train(snns, inputsTrain=x, targetsTrain=y)
  
  #snns$fitted.values <- matrixToActMapList(snns$fitted.values, nrow=dimX)
  
  snns
}


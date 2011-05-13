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


#' Create and train an art1 network.
#'
#' Adaptive resonance theory (art) networks are association networks. I.e. they 
#' perform clustering by finding a prototype to the given input. So, input and output
#' are the same type of data. Art1 is for binary inputs only, if you have real-valued input, use 
#' \code{\link{art2}} instead. In its current implementation, the network
#' has two-dimensional input (and output). The matrix \code{x} contains all 
#' (one dimensional) input patterns. Internally, every one of these patterns
#' is converted to a two-dimensional pattern using parameters \code{dimX} and \code{dimY}.
#' The parameter \code{nClusters} controls the amount of clusters that are assumed to
#' be present in the input patterns. A detailed description of the theory is available from the SNNS decumentation. 
#'
#' @export
art1 <- function(x, ...) UseMethod("art1")


#' Create and train an art1 network.
#' 
#' @param x a matrix with training inputs for the network
#' @param dimX x dimension of inputs and outputs
#' @param dimY y dimension of inputs and outputs
#' @param nClusters controls the number of clusters assumed to be present
#' @param maxit maximum of iterations to learn
#' @param initFunc the initialization function to use
#' @param initFuncParams the parameters for the initialization function
#' @param learnFunc the learning function to use
#' @param learnFuncParams the parameters for the learning function
#' @param updateFunc the update function to use
#' @param updateFuncParams the parameters for the update function
#' @param shufflePatterns should the patterns be shuffled?
#' @param ... additional function parameters (currently not used)
#' @return an \code{\link{rsnns}} object. The \code{fitted.values} member of the object contains a 
#' list of two-dimensional activation patterns.
#' @export
#' @S3method art1 default
#' @method art1 default
#' @seealso \code{\link{art2}}
#' @rdname art1
#' @examples 
#' \dontrun{demo(art1_letters)}
#' \dontrun{demo(art1_lettersSnnsR)}
art1.default <- function(x, dimX, dimY, nClusters=nrow(x), maxit=100, 
    initFunc="ART1_Weights", initFuncParams=c(1.0, 1.0), 
    learnFunc="ART1", learnFuncParams=c(0.9, 0.0, 0.0), 
    updateFunc="ART1_Stable", updateFuncParams=c(0.0),    
    shufflePatterns=TRUE, ...) {
  
  x <- as.matrix(x)

  nInputs <- dim(x)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("art1", "association"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)

  snns$archParams <- list(nClusters=nClusters, dimX=dimX, dimY=dimY)
  
  #snns$snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')
  snns$snnsObject$art1_createNet(dimX*dimY,dimX,nClusters,dimX)
 
  snns <- train(snns, inputsTrain=x)
  
  snns$fitted.values <- matrixToActMapList(snns$fitted.values, nrow=dimX)
  
  snns
}


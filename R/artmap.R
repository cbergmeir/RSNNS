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


#' Create and train an artmap network.
#'
#' An artmap performs supervised learning. It consists of two coupled art networks.
#' In theory, these could be art1, art2, or other art networks. However, in SNNS artmap is
#' implemented for art1 only. So, this function is to be used with binary input. 
#'
#' @references
#' Carpenter, G. A.; Grossberg, S. & Reynolds, J. H. (1991), 'ARTMAP: Supervised real-time learning and classification of nonstationary data by a self-organizing neural network', Neural Networks 4(5), 565--588.
#' 
#' Herrmann, K.-U. (1992), 'ART -- Adaptive Resonance Theory -- Architekturen, Implementierung und Anwendung', Master's thesis, IPVR, University of Stuttgart. 
# accepts binary input only
# 
# consists of two 
# artAdaptive resonance theory (art) networks are association networks. I.e. they 
# perform clustering by finding a prototype to the given input. So, input and output
# are the same type of data. Art1 is for binary inputs only, if you have real-valued input, use 
# \code{\link{art2}} instead. In its current implementation, the network
# has two-dimensional input (and output). The matrix \code{x} contains all 
# (one dimensional) input patterns. Internally, every one of these patterns
# is converted to a two-dimensional pattern using parameters \code{dimX} and \code{dimY}.
# The parameter \code{nClusters} controls the amount of clusters that are assumed to
# be present in the input patterns. A detailed description of the theory is available from the SNNS decumentation. 
#
#' @export
artmap <- function(x, ...) UseMethod("artmap")


#' Create and train an artmap network.
#' 
#' @param x a matrix with training inputs for the network
#' @param nInputsTrain the number of columns of the matrix that are training input
#' @param nInputsTargets the number of columns that are teacher signals
#' @param nUnitsRecLayerTrain number of units in the recognition layer of the training-data ART network
#' @param nUnitsRecLayerTargets number of units in the recognition layer of the target-data ART network
#' @param maxit maximum of iterations to perform
#' @param nRowInputsTrain number of rows the training input units are to be organized in (only for visualization purposes of the net in the original SNNS software)
#' @param nRowInputsTargets same, but for the teacher signal input units
#' @param nRowUnitsRecLayerTrain same, but for the recognition layer of the training-data ART network
#' @param nRowUnitsRecLayerTargets same, but for the recognition layer of the teacher-data ART network
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
#' @S3method artmap default
#' @method artmap default
#' @seealso \code{\link{art1}}, \code{\link{art2}}
#' @rdname artmap
#' @examples 
#' \dontrun{demo(artmap_letters)}
#' \dontrun{demo(artmap_lettersSnnsR)}
artmap.default <- function(x, nInputsTrain, nInputsTargets, nUnitsRecLayerTrain, nUnitsRecLayerTargets, maxit=1, 
    nRowInputsTrain=1, nRowInputsTargets=1, nRowUnitsRecLayerTrain=1, nRowUnitsRecLayerTargets=1,
    initFunc="ARTMAP_Weights", initFuncParams=c(1.0, 1.0, 1.0, 1.0, 0.0), 
    learnFunc="ARTMAP", learnFuncParams=c(0.8, 1.0, 1.0, 0, 0), 
    updateFunc="ARTMAP_Stable", updateFuncParams=c(0.8, 1.0, 1.0, 0, 0),    
    shufflePatterns=TRUE, ...) {
  
  x <- as.matrix(x)

  nInputs <- nInputsTrain + nInputsTargets
  
  if(nInputs != dim(x)[2L]) {
    warning("nInputsTrain + nInputsTargets has to be the same as the number of columns of the input matrix. Not training the ARTMAP..")
    return()
  }

  snns <- rsnnsObjectFactory(subclass=c("artmap", "association"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)

  snns$archParams <- list(nInputsTrain=nInputsTrain, nInputsTargets=nInputsTargets, 
                          nUnitsRecLayerTrain=nUnitsRecLayerTrain, nUnitsRecLayerTargets=nUnitsRecLayerTargets, 
      nRowInputsTrain=nRowInputsTrain, nRowInputsTargets=nRowInputsTargets, 
      nRowUnitsRecLayerTrain=nRowUnitsRecLayerTrain, nRowUnitsRecLayerTargets=nRowUnitsRecLayerTargets)
    
  snns$snnsObject$artmap_createNet(nInputsTrain,nRowInputsTrain,nUnitsRecLayerTrain,nRowUnitsRecLayerTrain,
                              nInputsTargets,nRowInputsTargets,nUnitsRecLayerTargets,nRowUnitsRecLayerTargets)
  
  snns <- train(snns, inputsTrain=x)
  
  snns
}


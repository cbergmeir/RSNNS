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


#' Create and train an art2 network.
#' 
#' Art2 is very similar to art1, but for real-valued input. See \code{\link{art1}}
#' for more information. A difference is, that the art2 implementation doesn't 
#' assume two-dimpensional input and output. 
#'
#' @export
art2 <- function(x, ...) UseMethod("art2")

#' Create and train an art2 network.
#' 
#' @param x a matrix with training inputs for the network
#' @param f2Units controls the number of clusters assumed to be present
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
#' @S3method art2 default
#' @method art2 default
#' @seealso \code{\link{art2}}
#' @rdname art2
#' @examples 
#' \dontrun{demo(art2_tetra)}
#' \dontrun{demo(art2_tetraSnnsR)}
art2.default <- function(x, f2Units=5, maxit=100, 
    initFunc="ART2_Weights", initFuncParams=c(0.9, 2.0), 
    learnFunc="ART2", learnFuncParams=c(0.98, 10.0, 10.0, 0.1, 0.0), 
    updateFunc="ART2_Stable", updateFuncParams=c(0.98, 10.0, 10.0, 0.1, 0.0),    
    shufflePatterns=TRUE, ...) {
  
  
  x <- as.matrix(x)
  
  nInputs <- dim(x)[2L]
  
  snns <- rsnnsObjectFactory(subclass=c("art2", "association"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)
  
  snns$archParams <- list(f2Units=f2Units)
  
  #snns$snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')
  snns$snnsObject$art2_createNet(nInputs,nInputs,f2Units,f2Units)

  snns <- train(snns, inputsTrain=x)
  
  snns
}


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


#' Create and train a self organizing map (som).
#'
#' @export
som <- function(x, ...) UseMethod("som")

#' Create and train a self organizing map (som).
#'
#' @param x a matrix with training inputs for the network
#' @param mapX
#' @param mapY
#' @param maxit maximum of iterations to learn
#' @param initFuncParams the parameters for the initialization function
#' @param learnFuncParams the parameters for the learning function
#' @param updateFuncParams the parameters for the update function
#' @param shufflePatterns should the patterns be shuffled?
#' @param calculateMap
#' @param calculateActMaps
#' @param calculateSpanningTree
#' @param targets
#' @param ... additional function parameters (currently not used)
#' @export
#' @S3method som default
#' @method som default
#' @rdname som
som.default <- function(x, mapX=16, mapY=16, maxit=100, 
    initFuncParams = c(1.0,  -1.0), 
    learnFuncParams=c(0.5, mapX/2, 0.8, 0.8, mapX), 
    updateFuncParams = c(0.0, 0.0, 1.0),  
    shufflePatterns = TRUE,
    calculateMap=TRUE, calculateActMaps=FALSE, calculateSpanningTree=FALSE, saveWinnersPerPattern=FALSE, targets=NULL, ...) {

  #for the som, no other init/learn/update functions are feasible, so they are not present as
  #parameters..
  initFunc = "Kohonen_Weights_v3.2"
  learnFunc = "Kohonen"
  updateFunc = "Kohonen_Order"
  
  x <- as.matrix(x)

  nInputs <- dim(x)[2L]
  nOutputs <- mapX*mapY
  
  snns <- rsnnsObjectFactory(subclass=c("som", "clustering"), nInputs=nInputs, maxit=maxit, 
      initFunc=initFunc, initFuncParams=initFuncParams, 
      learnFunc=learnFunc, learnFuncParams=learnFuncParams, 
      updateFunc=updateFunc, 
      updateFuncParams=updateFuncParams,
      shufflePatterns=shufflePatterns, computeIterativeError=FALSE)
  
  snnsObject <- snns$snnsObject
  
  snnsObject$setLearnFunc(snns$learnFunc)
  snnsObject$setUpdateFunc(snns$updateFunc)
  snnsObject$setInitialisationFunc(snns$initFunc)
  
  snnsObject$setUnitDefaults(0,0,3,0,1,'Act_Logistic','Out_Identity')
  snnsObject$kohonen_createNet(mapX,mapY,nInputs,mapX*mapY)
    
  snnsObject$setTTypeUnitsActFunc("UNIT_INPUT", "Act_Identity")
  snnsObject$setTTypeUnitsActFunc("UNIT_HIDDEN", "Act_Euclid")
  
  patSet <- snnsObject$createPatSet(x)
  
  snnsObject$initializeNet(snns$initFuncParams)
  snnsObject$shufflePatterns(snns$shufflePatterns)
  snnsObject$DefTrainSubPat()
  
#parameters <- c(0.855, 9.0, 0.9, 0.9, 16.0)
#parameters <- c(0.855, mapX/2, 0.9, 0.9, mapX)
  #parameters <- c(0.5, mapX/2, 0.8, 0.8, mapX)
  #maxit <- 1000
    
  for(i in 1:maxit) {
    res <- snnsObject$learnAllPatterns(snns$learnFuncParams)
  }
  
  snns$archParams <- list(mapX=mapX, mapY=mapY)
  
  snns$nOutputs <- nOutputs
  
  if(calculateMap) {

    mapVec <- snnsObject$somPredictCurrPatSetWinners(updateFuncParams, 
                                  saveWinnersPerPattern=saveWinnersPerPattern, targets=targets)
    snns$map <- vectorToActMap(mapVec$nWinnersPerUnit, nrow=mapX)
    snns$winnersPerPattern <- mapVec$winnersPerPattern
    
    if(!is.null(targets)) {
      
      snns$labeledUnits <- mapVec$labeledUnits
      labeledMap <- encodeClassLabels(mapVec$labeledUnits, method="WTA", l=0,h=0)
      snns$labeledMap <- vectorToActMap(labeledMap, mapX)      
        
    } else {
      snns$labeledUnits <- NULL
      snns$labeledMap <- NULL          
    }
    
#    if(!is.null(targets)) {
#      labels <- as.numeric(targets)
#      labeledSpanningTree <- spanningTree
#      for(i in 1:nrow(spanningTree))
#        for(j in 1:ncol(spanningTree))
#        {
#          if(spanningTree[i,j] != 0)  {
#            labeledSpanningTree[i,j] <- labels[spanningTree[i,j]]      
#          } 
#        }
#      snns$labeledSpanningTree <- labeledSpanningTree
#    } else {
#      snns$labeledSpanningTree <- NULL
#    }
    
  } else {
    snns$map <- NULL
    snns$winnersPerPattern <- NULL
    snns$labeledUnits <- NULL
    snns$labeledMap <- NULL
  }

  if(calculateActMaps) {
    
    actMat <- snnsObject$predictCurrPatSet("som", updateFuncParams)
    snns$actMaps <- matrixToActMapList(actMat, nrow=mapX)
    
  } else {
    snns$actMaps <- NULL
  }
  
  if(calculateSpanningTree) {

    spanningTree <- snnsObject$somPredictCurrPatSetWinnersSpanTree()
    snns$spanningTree <- vectorToActMap(spanningTree, nrow=mapX)
    
  } else {
    snns$spanningTree <- NULL
  }

  #calculate component maps
  compMat <- snnsObject$somPredictComponentMaps()  
  snns$componentMaps <- matrixToActMapList(compMat, nrow=mapX)
  
  snnsObject$deletePatSet(patSet$set_no)
  
  snns
}


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


# @include SnnsWrapperFunctions.R
#NULL

#' Create a layered network.
#'
#' @param unitsPerLayer a vector of integers that represents the number of units in each layer, including input and output layer
#' @param fullyConnectedFeedForward if \code{TRUE}, the network is fully connected as a feed-forward network. If \code{FALSE}, 
#' no connections are created
#' @rdname SnnsRObject$createNet
#' @usage \S4method{createNet}{SnnsR}(unitsPerLayer, fullyConnectedFeedForward = TRUE)
#' @aliases createNet,SnnsR-method createNet$getAllUnitsTType
#' @examples
#' \dontrun{SnnsRObject$createNet(c(2,2), FALSE)}
#' \dontrun{SnnsRObject$createNet(c(8,5,5,2), TRUE)}
SnnsR__createNet <- function(snnsObject, unitsPerLayer, fullyConnectedFeedForward = TRUE) {
  
  if(length(unitsPerLayer) < 2) stop("At least 2 layers have to be specified")
  
  layers <- list()
  currLayer <- 1
  
  nInputs <- unitsPerLayer[currLayer]
  layers[[currLayer]] <- vector()
  
  for(i in 1:nInputs) {
    
    num <- snnsObject$createDefaultUnit()
    layers[[currLayer]][i] <- num
    
    snnsObject$setUnitName(num,paste("Input_",i,sep=""))
    
    snnsObject$setUnitTType(num, resolveSnnsRDefine("topologicalUnitTypes","UNIT_INPUT"))
    
    snnsObject$setUnitPosition(num, i, 0, 0)
    
  }
  
  currLayer <- currLayer + 1
  
  
  for(k in seq(length=(length(unitsPerLayer)-2)))  {
    
    nHidden <- unitsPerLayer[currLayer]
    layers[[currLayer]] <- vector()
    
    for(i in 1:nHidden) {
      
      num <- snnsObject$createDefaultUnit()
      layers[[currLayer]][i] <- num
      
      snnsObject$setUnitName(num,paste("Hidden_",currLayer,"_",i,sep=""))
     
      snnsObject$setUnitTType(num, resolveSnnsRDefine("topologicalUnitTypes","UNIT_HIDDEN"))
      
      snnsObject$setUnitPosition(num, i, (currLayer-1)*2, 0)
      
      if(fullyConnectedFeedForward)  {
        
        snnsObject$setCurrentUnit(num)
        
        for(j in layers[[currLayer-1]])  {
          snnsObject$createLink(j,0);
        }   
      }
    }
    
    currLayer <- currLayer + 1
  }
  
  nOutputs <- unitsPerLayer[currLayer]
  layers[[currLayer]] <- vector()
  
  for(i in 1:nOutputs) {
    
    num <- snnsObject$createDefaultUnit()
    layers[[currLayer]][i] <- num
    
    snnsObject$setUnitName(num,paste("Output_",i,sep=""))
    
    snnsObject$setUnitTType(num, resolveSnnsRDefine("topologicalUnitTypes","UNIT_OUTPUT"))
    
    snnsObject$setUnitPosition(num, i, (currLayer-1)*2, 0)
    
    if(fullyConnectedFeedForward)  {

      snnsObject$setCurrentUnit(num)
      
      for(j in layers[[currLayer-1]])  {
        snnsObject$createLink(j,0);
      }
    }
  }
} 


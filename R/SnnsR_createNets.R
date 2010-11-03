# @include SnnsWrapperFunctions.R
#NULL

#' Create a network.
#'
#' @param unitsPerLayer a vector of integers that represents the number of units in each layer, including input and output layer
#' @param fullyConnectedFeedForward if TRUE, the network is fully connected as a feed-forward network. If FALSE, no connections are made
#' @rdname SnnsRObject$createNet
#' @usage \S4method{createNet}{SnnsR}(unitsPerLayer, fullyConnectedFeedForward = TRUE)
#' @aliases createNet,SnnsR-method createNet$getAllUnitsTType
#' @author Christoph
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
    
    snnsObject$setUnitTType(num, SnnsDefines_resolveDefine(SnnsDefines_topologicalUnitTypes,"UNIT_INPUT"))
    
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
     
      snnsObject$setUnitTType(num,SnnsDefines_resolveDefine(SnnsDefines_topologicalUnitTypes,"UNIT_HIDDEN"))
      
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
    
    snnsObject$setUnitTType(num,SnnsDefines_resolveDefine(SnnsDefines_topologicalUnitTypes,"UNIT_OUTPUT"))
    
    snnsObject$setUnitPosition(num, i, (currLayer-1)*2, 0)
    
    if(fullyConnectedFeedForward)  {

      snnsObject$setCurrentUnit(num)
      
      for(j in layers[[currLayer-1]])  {
        snnsObject$createLink(j,0);
      }
    }
  }
} 


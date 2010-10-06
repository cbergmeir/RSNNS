#' @include SnnsWrapperFunctions.R
NULL

#' Create a fully connected feed-forward network.
#'
#' The function is a convenience funcion to use \code{\link{SnnsR_createNet}}.
#'
#' @param unitsPerLayer a vector of integers that represents the number of units in each layer, including input and output layer
#' @export
#' @seealso \code{\link{SnnsR_createNet}}
#' @author Christoph
#' @examples
#' slp <- SnnsR_createFullyConnectedFeedForwardNet(c(8,5,8))
SnnsR_createFullyConnectedFeedForwardNet <- function(unitsPerLayer) {
  return(SnnsR_createNet(unitsPerLayer, TRUE))
}



#' Create a network.
#'
#' @param unitsPerLayer a vector of integers that represents the number of units in each layer, including input and output layer
#' @param fullyConnectedFeedForward if TRUE, the network is fully connected as a feed-forward network. If FALSE, no connections are made
#' @export
#' @seealso \code{\link{SnnsR_createFullyConnectedFeedForwardNet}}
#' @author Christoph
#' @examples
#' rbf_dda <- SnnsR_createNet(c(2,2), FALSE)
#' mlp <- SnnsR_createNet(c(8,5,5,2), TRUE)
SnnsR_createNet <- function(unitsPerLayer, fullyConnectedFeedForward = TRUE) {
  
  if(length(unitsPerLayer) < 2) error("At least 2 layers have to be specified")
  
  layers <- list()
  currLayer <- 1
  
  nInputs <- unitsPerLayer[currLayer]
  layers[[currLayer]] <- vector()
  
  for(i in 1:nInputs) {
    
    num <- SnnsKrui_createDefaultUnit()
    layers[[currLayer]][i] <- num
    
    SnnsKrui_setUnitName(num,paste("Input_",i,sep=""))
    SnnsKrui_setUnitTType(num, SnnsR_resolveDefine(SnnsR_topologicalUnitTypes,"UNIT_INPUT"))
    
    SnnsKrui_setUnitPosition(num, i, 0, 0)
    
  }
  
  currLayer <- currLayer + 1
  
  
  for(k in seq(length=(length(unitsPerLayer)-2)))  {
    
    nHidden <- unitsPerLayer[currLayer]
    layers[[currLayer]] <- vector()
    
    for(i in 1:nHidden) {
      
      num <- SnnsKrui_createDefaultUnit()
      layers[[currLayer]][i] <- num
      
      SnnsKrui_setUnitName(num,paste("Hidden_",currLayer,"_",i,sep=""))
      
      SnnsKrui_setUnitTType(num,SnnsR_resolveDefine(SnnsR_topologicalUnitTypes,"UNIT_HIDDEN"))
      
      SnnsKrui_setUnitPosition(num, i, (currLayer-1)*2, 0)
      
      if(fullyConnectedFeedForward)  {
        
        SnnsKrui_setCurrentUnit(num)
        
        for(j in layers[[currLayer-1]])  {
          SnnsKrui_createLink(j,0);
        }   
      }
    }
    
    currLayer <- currLayer + 1
  }
  
  nOutputs <- unitsPerLayer[currLayer]
  layers[[currLayer]] <- vector()
  
  #outputs <- vector()
  for(i in 1:nOutputs) {
    
    num <- SnnsKrui_createDefaultUnit()
    layers[[currLayer]][i] <- num
    
    SnnsKrui_setUnitName(num,paste("Output_",i,sep=""))
    
    #SnnsKrui_setUnitActFunc(num, 'Act_Identity');
    #SnnsKrui_setUnitOutFunc(num, 'Out_Threshold05');
    
#  SnnsKrui_setUnitTType(num,OUTPUT)
    SnnsKrui_setUnitTType(num,SnnsR_resolveDefine(SnnsR_topologicalUnitTypes,"UNIT_OUTPUT"))
    
    SnnsKrui_setUnitPosition(num, i, (currLayer-1)*2, 0)
    
    if(fullyConnectedFeedForward)  {

      SnnsKrui_setCurrentUnit(num)
      
      for(j in layers[[currLayer-1]])  {
        SnnsKrui_createLink(j,0);
      }
    }
  }
  
  #return(list(inputs=inputs, hidden=hidden, outputs=outputs))
  return(0)
} 
# @include SnnsWrapperFunctions.R
#NULL

#' Create a fully connected feed-forward network.
#'
#' The function is a convenience funcion to use \code{\link{SnnsR__createNet}}.
#'
#' @param unitsPerLayer a vector of integers that represents the number of units in each layer, including input and output layer
#' @export
#' @seealso \code{\link{SnnsR__createNet}}
#' @author Christoph
#' @examples
#' \dontrun{slp <- SnnsR__createFullyConnectedFeedForwardNet(c(8,5,8))}
SnnsR__createFullyConnectedFeedForwardNet <- function(snnsObject, unitDefaults = c(1,0,1,0,1,"Act_Logistic","Out_Identity"), updateFunc="Topological_Order", unitsPerLayer, linOut=FALSE) {
  
  snnsObject$setUpdateFunc(updateFunc)
  snnsObject$setUnitDefaults(unitDefaults)
  
  return(snnsObject$createNet(unitsPerLayer, linOut=linOut, TRUE))
}



#' Create a network.
#'
#' @param unitsPerLayer a vector of integers that represents the number of units in each layer, including input and output layer
#' @param fullyConnectedFeedForward if TRUE, the network is fully connected as a feed-forward network. If FALSE, no connections are made
#' @export
#' @seealso \code{\link{SnnsR__createFullyConnectedFeedForwardNet}}
#' @author Christoph
#' @examples
#' \dontrun{rbf_dda <- SnnsR__createNet(c(2,2), FALSE)}
#' \dontrun{mlp <- SnnsR__createNet(c(8,5,5,2), TRUE)}
SnnsR__createNet <- function(snnsObject, unitsPerLayer, linOut = FALSE, fullyConnectedFeedForward = TRUE) {
  
  if(length(unitsPerLayer) < 2) error("At least 2 layers have to be specified")
  
  layers <- list()
  currLayer <- 1
  
  nInputs <- unitsPerLayer[currLayer]
  layers[[currLayer]] <- vector()
  
  for(i in 1:nInputs) {
    
    num <- snnsObject$createDefaultUnit()
    layers[[currLayer]][i] <- num
    
    snnsObject$setUnitName(num,paste("Input_",i,sep=""))
    
    if(linOut) {
      snnsObject$setUnitActFunc(num, 'Act_Identity');
    }
    
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

      #if(linOut) {
      #  snnsObject$setUnitActFunc(num, 'Act_Identity');
      #}
      
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
  
  #outputs <- vector()
  for(i in 1:nOutputs) {
    
    num <- snnsObject$createDefaultUnit()
    layers[[currLayer]][i] <- num
    
    snnsObject$setUnitName(num,paste("Output_",i,sep=""))
    
    if(linOut) {
      snnsObject$setUnitActFunc(num, 'Act_Identity');
    }
    
    #snnsObject$setUnitActFunc(num, 'Act_Identity');
    #snnsObject$setUnitOutFunc(num, 'Out_Threshold05');
    
#  snnsObject$setUnitTType(num,OUTPUT)
    snnsObject$setUnitTType(num,SnnsDefines_resolveDefine(SnnsDefines_topologicalUnitTypes,"UNIT_OUTPUT"))
    
    snnsObject$setUnitPosition(num, i, (currLayer-1)*2, 0)
    
    if(fullyConnectedFeedForward)  {

      snnsObject$setCurrentUnit(num)
      
      for(j in layers[[currLayer-1]])  {
        snnsObject$createLink(j,0);
      }
    }
  }
  
  #return(list(inputs=inputs, hidden=hidden, outputs=outputs))
  return(0)
} 

#' Create an elman network.
#'
#' @param snnsObject the object the function is applied to 
#' @param unitDefaults the default values with wich all units will be created
#' @param updateFunc the update function of the net
#' @param arch the architecture of the network
#' @param columns ...
#' @param outContext ...
#' @export
#' @seealso \code{\link{SnnsR__createFullyConnectedFeedForwardNet}}
#' @author Christoph
# @examples
SnnsR__createElmanNet <- function(snnsObject, unitDefaults = c(1,0,1,0,1,"Act_Logistic","Out_Identity"), updateFunc="JE_Order", arch=c(2,8,2), columns=c(1,1,1), outContext=FALSE) {

  snnsObject$setUpdateFunc(updateFunc)
  snnsObject$setUnitDefaults(unitDefaults)
  snnsObject$elman_createNet(arch, columns, outContext)

}


#' Create a jordan network.
#'
#' @param snnsObject the object the function is applied to 
#' @param unitDefaults the default values with wich all units will be created
#' @param updateFunc the update function of the net
#' @param arch the architecture of the network
#' @param columns ...
#' @export
#' @seealso \code{\link{SnnsR__createFullyConnectedFeedForwardNet}}
#' @author Christoph
# @examples
SnnsR__createJordanNet <- function(snnsObject, unitDefaults = c(1,0,1,0,1,"Act_Logistic","Out_Identity"), updateFunc="JE_Order", arch=c(2,8,2), columns=c(1,2,1)) {

  snnsObject$setUpdateFunc(updateFunc)
  snnsObject$setUnitDefaults(unitDefaults)
  
  if(length(arch) != 3 || length(columns) != 3) 
    warning("createJordanNet: arch and columns should have length 3, as a jordan net always has 3 layers..")
  
  snnsObject$jordan_createNet(arch[1],arch[2],arch[3],columns[1],columns[2],columns[3])
  
}



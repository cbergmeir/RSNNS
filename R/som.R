#' Create and train an mlp.
#'
#' @export
#' @author Christoph
som <- function(x, ...) UseMethod("som")

#' Create and train an som.
#'
#' @export
#' @author Christoph
som.default <- function(x, mapX=16, mapY=16, maxit=100, 
    initFuncParams = c(1.0,  -1.0), 
    learnFuncParams=c(0.5, mapX/2, 0.8, 0.8, mapX), 
    updateFuncParams = c(0.0, 0.0, 1.0),  
    shufflePatterns = TRUE,
    calculateMap=TRUE, calculateActMaps=FALSE, calculateSpanningTree=FALSE, targets=NULL) {

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
  
  #snnsObject <- SnnsRObjectFactory()
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
  
  #snns <- NULL
  #snns$nInputs <- nInputs
  
  snns$archParams <- list(mapX=mapX, mapY=mapY)
  
  snns$nOutputs <- nOutputs
  #snns$mapX <- mapX
  #snns$mapY <- mapY
  
  if(calculateMap) {

    mapVec <- snnsObject$somPredictCurrPatSetWinners()
    print(mapVec)
    snns$map <- vectorToActMap(mapVec, nrow=mapX)
    
  } else {
    snns$map <- NULL
  }

  if(calculateActMaps) {
    
    actMat <- snnsObject$predictCurrPatSet("som", updateFuncParams)
    snns$actMaps <- matrixToActMapList(actMat, nrow=mapX)
    
  } else {
    snns$actMaps <- NULL
  }
  
  if(calculateSpanningTree) {
    spanningTree <- snnsObject$somPredictCurrPatSetWinnersSpanTree()
    spanningTree <- vectorToActMap(spanningTree, nrow=mapX)

    if(!is.null(targets)) {
      labels <- as.numeric(targets)
      labelledSpanningTree <- spanningTree
      for(i in 1:nrow(spanningTree))
        for(j in 1:ncol(spanningTree))
        {
          if(spanningTree[i,j] != 0)  {
            labelledSpanningTree[i,j] <- labels[spanningTree[i,j]]      
          } 
        }
      snns$labelledSpanningTree <- labelledSpanningTree
    } else {
      snns$labelledSpanningTree <- NULL
    }
    
    snns$spanningTree <- spanningTree  

  } else {
    snns$spanningTree <- NULL
  }

  #calculate component maps
  compMat <- snnsObject$somPredictComponentMaps()  
  snns$componentMaps <- matrixToActMapList(compMat, nrow=mapX)
  
  snnsObject$deletePatSet(patSet$set_no)
  
  #compMaps <- apply(compMaps, 1, function(x) { return(list(matrix(x, nrow=mapX)))})
  #compMaps <- lapply(compMaps, function(x) {x[[1]]})
  #snns$componentMaps <- compMaps
  
  #snns$type <- "som"
    
  #snns$snnsObject <- snnsObject
  #class(snns) <- c("som", "clustering", "rsnns")
  snns
}


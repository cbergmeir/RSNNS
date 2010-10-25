#' Train a network and test it in every training iteration.
#'
#' @param snnsObject the object the function is applied to 
#' @param inputs a matrix with inputs for the network
#' @param targets the corresponding targets
#' @param learnFunc the learning function to use
#' @param learnFuncParams the parameters for the learning function
#' @param maxit maximum of iterations to learn
#' @param shufflePatterns should the patterns be shuffled?
#' @export
# @seealso \code{\link{SnnsR__createFullyConnectedFeedForwardNet}}
#' @author Christoph
# @examples
SnnsR__train <- function(snnsObject, inputsTrain, targetsTrain=NULL, 
    initFunc="Randomize_Weights", initFuncParams=c(1.0, -1.0), 
    learnFunc="Quickprop", learnFuncParams=c(0.2, 0, 0, 0),
    updateFunc="Topological_Order", updateFuncParams=c(0.0), outputMethod="reg_class", 
    maxit=100, shufflePatterns=TRUE, computeError=TRUE, inputsTest=NULL, targetsTest=NULL) {

  testing <- TRUE
  if(is.null(inputsTest)) testing <- FALSE
  
  result <- list()
  
  
  #use the maximal possible amount of parameters, otherwise, unused parameters
  #might yield unforeseeable effects in the C code..
  expandedLearnFuncParams <- c(0,0,0,0,0)
  for(i in 1:length(learnFuncParams)) expandedLearnFuncParams[i] <- learnFuncParams[i]
  
  expandedInitFuncParams <- c(0,0,0,0,0)
  for(i in 1:length(initFuncParams)) expandedInitFuncParams[i] <- initFuncParams[i]
  
  expandedUpdateFuncParams <- c(0,0,0,0,0)
  for(i in 1:length(updateFuncParams)) expandedUpdateFuncParams[i] <- updateFuncParams[i]
  
  
  snnsObject$setLearnFunc(learnFunc)
  snnsObject$setUpdateFunc(updateFunc)
  
  patSetTrain <- NULL
  
  if(is.null(targetsTrain)) {
    patSetTrain <- snnsObject$createPatSet(inputsTrain)
  } else {
    patSetTrain <- snnsObject$createPatSet(inputsTrain, targetsTrain)
  }
    
  
  if(computeError)
    errorTrain <- vector()
  
  if(testing) {
    patSetTest <- snnsObject$createPatSet(inputsTest, targetsTest)
    errorTest <- vector()    
  }
  
  #print(patSetTrain)
  #print(patSetTest)

  snnsObject$initializeNet(expandedInitFuncParams, initFunc)
  
  snnsObject$shufflePatterns(shufflePatterns)
  
  snnsObject$setCurrPatSet(patSetTrain$set_no)
  snnsObject$DefTrainSubPat()  

  for(i in 1:maxit) {
  
    res <- snnsObject$learnAllPatterns(expandedLearnFuncParams)
    #if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
    if(computeError)
      errorTrain[i] <- res[[2]]
    
    if(testing)  {
      snnsObject$setCurrPatSet(patSetTest$set_no)
      snnsObject$DefTrainSubPat()
      
      res <- snnsObject$testAllPatterns(expandedLearnFuncParams)
      if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
      errorTest[i] <- res[[2]]
      
      snnsObject$setCurrPatSet(patSetTrain$set_no)
      snnsObject$DefTrainSubPat()      
    }
    
  }
  
  if(computeError)
    result$IterativeFitError <- errorTrain
 
  snnsObject$setCurrPatSet(patSetTrain$set_no)
  result$fitValues <- snnsObject$predictCurrPatSet(outputMethod, expandedUpdateFuncParams)

  if(testing) {
    
    result$IterativeTestError <- errorTest
    
    snnsObject$setCurrPatSet(patSetTest$set_no)
    result$testValues <- snnsObject$predictCurrPatSet(outputMethod, expandedUpdateFuncParams)
    snnsObject$deletePatSet(patSetTest$set_no)
    
  } else {
    result$IterativeTestError <- NULL
    result$testValues <- NULL
  }

  #snns auto-reorganizes the pattern set numbers, so the first generated pattern set
  #has to be deleted at last
  snnsObject$deletePatSet(patSetTrain$set_no)
  
  return(result)
}
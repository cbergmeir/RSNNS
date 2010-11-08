#' Train a network and test it in every training iteration.
#'
#' @param inputsTrain a matrix with inputs for the network
#' @param targetsTrain the corresponding targets
#' @param initFunc the initialization function to use
#' @param initFuncParams the parameters for the initialization function
#' @param learnFunc the learning function to use
#' @param learnFuncParams the parameters for the learning function
#' @param updateFunc the update function to use
#' @param updateFuncParams the parameters for the update function
#' @param outputMethod the output method of the net
#' @param maxit maximum of iterations to learn
#' @param shufflePatterns should the patterns be shuffled?
#' @param computeError should the error be computed in every iteration?
#' @param inputsTest a matrix with inputs to test the network
#' @param targetsTest the corresponding targets for the test input
#' @rdname SnnsRObject$train
#' @usage \S4method{train}{SnnsR}(inputsTrain, targetsTrain=NULL, 
#'     initFunc="Randomize_Weights", initFuncParams=c(1.0, -1.0), 
#'     learnFunc="Quickprop", learnFuncParams=c(0.2, 0, 0, 0),
#'     updateFunc="Topological_Order", updateFuncParams=c(0.0), outputMethod="reg_class", 
#'     maxit=100, shufflePatterns=TRUE, computeError=TRUE, inputsTest=NULL, targetsTest=NULL)
#' @aliases train,SnnsR-method SnnsRObject$train
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
  
  snnsObject$shufflePatterns(shufflePatterns)
  snnsObject$setCurrPatSet(patSetTrain$set_no)
  snnsObject$DefTrainSubPat()  

  if(learnFunc == "RadialBasisLearning") {
    #cat("RBF: First Initialization step with Kohonen\n")
    snnsObject$initializeNet(c(0,0,0,0,0), "RBF_Weights_Kohonen")
  }
    
  
  snnsObject$initializeNet(expandedInitFuncParams, initFunc)
  
  for(i in 1:maxit) {
  
    res <- snnsObject$learnAllPatterns(expandedLearnFuncParams)
    #if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
    if(computeError)
      errorTrain[i] <- res[[2]]
    
    if(testing)  {
      snnsObject$setCurrPatSet(patSetTest$set_no)
      snnsObject$DefTrainSubPat()
      
      #TODO: Why doesn't testAllPatterns work with RadialBasisLearning? 
      #And learning even with all parameters = 0 alters the results.. 
      #if(learnFunc == "RadialBasisLearning")
      #  res <- snnsObject$learnAllPatterns(c(0,0,0,0.1,0.8))
      #else
        res <- snnsObject$testAllPatterns(expandedLearnFuncParams)
      #if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
      errorTest[i] <- res[[2]]
      
      #print(res)
      
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
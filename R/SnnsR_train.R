#' Train a network. 
#'
#' Convenience function that calls \code{\link{SnnsR__trainWithoutTest}}, or \code{\link{SnnsR__trainAndTest}}, respectively.
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
SnnsR__train <- function(snnsObject, inputs, targets, learnFunc="Quickprop", learnFuncParams=c(0.2, 0, 0, 0), maxit=1000, shufflePatterns=TRUE, testSetRatio=0.0) {
    
  if(testSetRatio == 0.0) {
    result <- list()
    result$IterativeFitError <- snnsObject$trainWithoutTest(inputs, targets, learnFunc, learnFuncParams, maxit, shufflePatterns)
    result$IterativeTestError <- NULL
    result$fitValues <- snnsObject$predictValuesCurrPatSet()
    result$testValues <- NULL
  } else {
    result <- snnsObject$trainAndTest(inputs, targets, learnFunc, learnFuncParams, maxit, shufflePatterns, testSetRatio)
  }

  result
}


#' Train a network.
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
SnnsR__trainWithoutTest <- function(snnsObject, inputs, targets, learnFunc="Quickprop", learnFuncParams=c(0.2, 0, 0, 0), maxit=1000, shufflePatterns=TRUE) {
  
  snnsObject$setLearnFunc(learnFunc)
  
  patset <- snnsObject$createPatterns(inputs, targets)
  snnsObject$setCurrPatSet(patset$set_no)
  
  snnsObject$shufflePatterns(shufflePatterns)
  snnsObject$DefTrainSubPat()
  
  #use the maximal possible amount of parameters, otherwise, unused parameters
  #might yield unforeseeable effects in the C code..
  expandedLearnFuncParams <- c(0,0,0,0,0)
  for(i in 1:length(learnFuncParams)) expandedLearnFuncParams[i] <- learnFuncParams[i]
  
  error <- vector()
  for(i in 1:maxit) {
    res <- snnsObject$learnAllPatterns(expandedLearnFuncParams)
    if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
    error[i] <- res[[2]]
  }
  
  snnsObject$deletePatSet(patset$set_no)
  
  error
}

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
SnnsR__trainAndTest <- function(snnsObject, inputs, targets, learnFunc="Quickprop", learnFuncParams=c(0.2, 0, 0, 0), maxit=1000, shufflePatterns=TRUE, testSetRatio=0.0) {

  result <- list()
  
  x <- as.matrix(inputs)
  nInputs <- nrow(x)
  
  y <- as.matrix(targets)
  #nTargets <- nrow(y)
    
  trainIndices <- 1 : (nInputs * (1-testSetRatio))
  testIndices <- (1:nInputs)[-trainIndices]

  #print(trainIndices)
  #print(testIndices)
  
  #print(paste("trainIndices: ", trainIndices, "testIndices: ", testIndices,sep=""))
  
  inputsTrain <- x[trainIndices,]
  targetsTrain <- y[trainIndices,]

  #print(inputsTrain)
  #print(targetsTrain)
  
  inputsTest <- x[testIndices,]
  targetsTest <- y[testIndices,]
  
  snnsObject$setLearnFunc(learnFunc)
  
  patsetTrain <- snnsObject$createPatterns(inputsTrain, targetsTrain)
  patsetTest <- snnsObject$createPatterns(inputsTest, targetsTest)

  #print(patsetTrain)
  #print(patsetTest)
    
  snnsObject$shufflePatterns(shufflePatterns)
  
  #snnsObject$DefTrainSubPat()
  
  #res <- snnsObject$setCurrPatSet(patsetTest$set_no)
  #snnsObject$DefTrainSubPat()
  #res <- snnsObject$setCurrPatSet(patsetTrain$set_no)
  #snnsObject$DefTrainSubPat()
  
  #print("before loop")
  
  #use the maximal possible amount of parameters, otherwise, unused parameters
  #might yield unforeseeable effects in the C code..
  expandedLearnFuncParams <- c(0,0,0,0,0)
  for(i in 1:length(learnFuncParams)) expandedLearnFuncParams[i] <- learnFuncParams[i]

  errorTrain <- vector()
  errorTest <- vector()
  for(i in 1:maxit) {
  
    #print("h1")
    snnsObject$setCurrPatSet(patsetTrain$set_no)
    snnsObject$DefTrainSubPat()
    #print(res)
    #print("h2")    
    res <- snnsObject$learnAllPatterns(expandedLearnFuncParams)
    #print("h3")    
    if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
    errorTrain[i] <- res[[2]]
    
    #print(res)
    
    snnsObject$setCurrPatSet(patsetTest$set_no)
    snnsObject$DefTrainSubPat()
    
    res <- snnsObject$testAllPatterns(expandedLearnFuncParams)
    if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
    errorTest[i] <- res[[2]]
    
    #print(res)
  }
  
  result$IterativeFitError <- errorTrain
  result$IterativeTestError <- errorTest
  
  snnsObject$setCurrPatSet(patsetTrain$set_no)
  result$fitValues <- snnsObject$predictValuesCurrPatSet()
  snnsObject$setCurrPatSet(patsetTest$set_no)
  result$testValues <- snnsObject$predictValuesCurrPatSet()
  
  #print("after loop")
  snnsObject$deletePatSet(patsetTest$set_no)
  snnsObject$deletePatSet(patsetTrain$set_no)

  return(result)
}
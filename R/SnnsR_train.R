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
#' @return a list containing:
#' \item{fitValues}{the fitted values, i.e. outputs of the training inputs}
#' \item{IterativeFitError}{The SSE in every iteration/epoch on the training set}
#' \item{testValues}{the predicted values, i.e. outputs of the test inputs}
#' \item{IterativeTestError}{The SSE in every iteration/epoch on the test set}
#' @rdname SnnsRObject$train
#' @usage \S4method{train}{SnnsR}(inputsTrain, targetsTrain=NULL, 
#'     initFunc="Randomize_Weights", initFuncParams=c(1.0, -1.0), 
#'     learnFunc="Quickprop", learnFuncParams=c(0.2, 0, 0, 0),
#'     updateFunc="Topological_Order", updateFuncParams=c(0.0), 
#'     outputMethod="reg_class", maxit=100, shufflePatterns=TRUE, 
#'     computeError=TRUE, inputsTest=NULL, targetsTest=NULL)
#' @aliases train,SnnsR-method SnnsRObject$train
SnnsR__train <- function(snnsObject, inputsTrain, targetsTrain=NULL, 
    initFunc="Randomize_Weights", initFuncParams=c(1.0, -1.0), 
    learnFunc="Quickprop", learnFuncParams=c(0.2, 0, 0, 0),
    updateFunc="Topological_Order", updateFuncParams=c(0.0), outputMethod="reg_class", 
    maxit=100, shufflePatterns=TRUE, computeError=TRUE, inputsTest=NULL, targetsTest=NULL, 
    serializeTrainedObject=TRUE) {

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
  
  if(serializeTrainedObject)
    snnsObject$serialize()
  
  result
}
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
SnnsR__train <- function(snnsObject, inputs, targets, learnFunc="Quickprop", learnFuncParams=c(0.2, 0, 0, 0), maxit=1000, shufflePatterns=TRUE) {
  
  snnsObject$setLearnFunc(learnFunc)
  
  patset <- snnsObject$createPatterns(inputs, targets)
  snnsObject$setCurrPatSet(patset$set_no)
  
  snnsObject$shufflePatterns(shufflePatterns)
  snnsObject$DefTrainSubPat()
  
  error <- vector()
  for(i in 1:maxit) {
    res <- snnsObject$learnAllPatterns(learnFuncParams)
    if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
    error[i] <- res[[2]]
  }
  
  error
}
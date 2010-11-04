##Find out if a string has a certain suffix
endsWith <- function(myString, mySubString) {
  
  l1 <- nchar(myString)
  l2 <- nchar(mySubString)
  if(l1 < l2) return(FALSE)
  s <- substr(myString, l1-l2+1, l1)
  if(s == mySubString) return(TRUE)
  return(FALSE)
  
}

##Find out if a string has a certain prefix
beginsWith <- function(myString, mySubString) {
  
  l1 <- nchar(myString)
  l2 <- nchar(mySubString)
  if(l1 < l2) return(FALSE)
  s <- substr(myString, 1, l2)
  if(s == mySubString) return(TRUE)
  return(FALSE)
  
}

##Rotate a square matrix 90 degrees clockwise.
rot90 <- function(a) {
  n <- dim(a)[1]
  t(a[n:1, ])
}

#' Organize network activation as 2d map.
#'
#' The input to this function is a vector containing in each row an activation
#' pattern/output of a neural network. This function reorganizes the vector to
#' a matrix. Normally, only the number of rows \code{nrow} will be used.
#' 
#' @param v the vector containing the activation pattern
#' @param nrow number of rows the resulting matrices will have
#' @param ncol number of columns the resulting matrices will have
#' @export
#' @seealso \link{matrixToActMapList} \link{plotActMap}
vectorToActMap <- function(v, nrow=0, ncol=0) {
  
  if(nrow==0) nrow <- ncol(v) / ncol  
  return(matrix(v, nrow=nrow, byrow=TRUE))
  
}
  
#' Organize a matrix with network activations as 2d maps.
#'
#' The input to this function is a matrix containing in each row an activation
#' pattern/output of a neural network. This function uses \link{vectorToActMap} to 
#' reorganizes the matrix to a list of matrices, whereby each row of the input matrix 
#' is converted to a matrix in the output list.
#' 
#' @param m the matrix containing one activation pattern in every row
#' @param nrow number of rows the resulting matrices will have
#' @param ncol number of columns the resulting matrices will have
#' @export
#' @seealso \link{vectorToActMap} \link{plotActMap}
matrixToActMapList <- function(m, nrow=0, ncol=0) {
  
  actMapList <- apply(m, 1, function(x) { return(list(vectorToActMap(x,nrow,ncol)))})
  actMapList <- lapply(actMapList, function(x) {x[[1]]})
  actMapList
} 

#' Plot an activation map as a heatmap.
#'
#' @param x the input data matrix 
#' @param ... parameters passed to \code{image}
#' @export
#' @seealso \link{vectorToActMap} \link{matrixToActMapList}
plotActMap <- function(x, ...) {
  image(rot90(x),...)
}

#' Get the function table of available SNNS functions.
#'
#' @return a data.frame with columns:
#' \item{name}{name of the function}
#' \item{type}{the type of the function (learning, init, update,...)}
#' \item{#inParams}{the number of input parameters of the function}
#' \item{#outParams}{the number of output parameters of the function}
#' @export
getSNNSFunctionTable <- function() {

  snnsObject <- SnnsRObjectFactory()
  
  noFunc <- snnsObject$getNoOfFunctions()
  allFuncs <- data.frame()
  
  for(i in 1:noFunc) {
    fi <- snnsObject$getFuncInfo(i)
    fiInfo <- snnsObject$getFuncParamInfo(fi[[1]], fi[[2]])
    allFuncs <- rbind(allFuncs, cbind(fi$func_name, fi$func_type, fiInfo$no_of_input_params, fiInfo$no_of_input_params))
  }
  
  names(allFuncs) <- c("name", "type", "#inParams", "#outParams")
  
  rm(snnsObject)
  
  allFuncs
}

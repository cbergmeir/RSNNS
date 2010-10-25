#' Check the input of a reg_class class for eventual problems.
#'
#' @author Christoph
checkInput <- function(x,y) {
  
  ok <- TRUE
  
  if(any(is.na(x))) {
    stop("missing values in 'x'")
    ok <- FALSE
  }
  if(any(is.na(y))) {
    stop("missing values in 'y'")
    ok <- FALSE
  }
  if(dim(x)[1L] != dim(y)[1L]) {
    stop("nrows of 'x' and 'y' must match")
    ok <- FALSE    
  }
  
  ok
}

#' Decode class labels from a numerical or levels vector to a binary matrix.
#'
#' @export
#' @author Christoph
decodeClassLabels <- function(x) {

  #decodeClassLabels(iris[,5])
  
  #y <- gl(2, 4, 8)
  #levels(y) <- c("low", "high")
  #y
  
  #if(length(levels(x))!=0) {
  #  
  #}
  
  classes <- unique(x)
  
  targets <- matrix(nrow=length(x), ncol=length(classes))
  for(i in 1:length(x))
    for(j in 1:length(classes))  {
      targets[i,j] <- as.numeric(j == as.numeric(x[i]))
    }
  targets
}

encodeClassLabels <- function(x) {
  apply(x, 1, function(y) which(y==max(y)))
}

toNumericClassLabels <- function(x) {
  if(is.numeric(x)) return(x)
  else return(as.numeric(x))
}

#examples
#y <- decodeClassLabels(iris[,5])
#y
#encodeClassLabels(y)
#toNumericClassLabels(iris[,5])

confusionMatrix <- function(targets, predictions) {
   
  if(is.matrix(targets)) {
    if(ncol(targets)!=1) tr <- encodeClassLabels(targets)
  } else {
    tr <- toNumericClassLabels(targets)  
  }
  
  if(is.matrix(predictions)) {
    if(ncol(predictions)!=1) pr <- encodeClassLabels(predictions)
  } else {
    pr <- toNumericClassLabels(predictions)
  }
  
  cm <- table(targets=tr, predictions=pr)
  #rowSums(cm)
  #colSums(cm)
  cm
}

#' Plot a ROC curve.
#'
#' Code is taken from R news Volume 4/1, June 2004
#' 
#' @export
#' @author ...

plotROC <-function(T,D){
  cutpoints<-c(-Inf, sort(unique(T)), Inf)
  sens<-sapply(cutpoints,
      function(c) sum(D[T>c])/sum(D))
  spec<-sapply(cutpoints,
      function(c) sum((1-D)[T<=c]/sum(1-D)))
  plot(1-spec, sens, type="l")
}


#' Regression plot.
#'
#' @export
#' @author Christoph
plotRegressionError <- function(targets, fits, ...)
{
  #if(!inherits(object, "reg_class")) stop("not a legitimate reg_class model")
  
  plot(targets, fits, xlim=c(0,1), ylim=c(0,1), ...)
  
  linMod <- lm(fits ~ targets)
  abline(linMod, col="red")
  lines(c(0,1), c(0,1))
  
}

splitForTrainingAndTest <- function(x, y, ratio=0.15) {

  x <- as.matrix(x)
  nInputs <- nrow(x)
  
  y <- as.matrix(y)
  
  trainIndices <- 1 : (nInputs * (1-ratio))
  testIndices <- (1:nInputs)[-trainIndices]
  
  inputsTrain <- x[trainIndices,]
  targetsTrain <- y[trainIndices,]
  
  inputsTest <- x[testIndices,]
  targetsTest <- y[testIndices,]
  
  return(list(inputsTrain=inputsTrain, targetsTrain=targetsTrain, inputsTest=inputsTest, targetsTest=targetsTest))
  
}

endsWith <- function(myString, mySubString) {
  
  l1 <- nchar(myString)
  l2 <- nchar(mySubString)
  if(l1 < l2) return(FALSE)
  s <- substr(myString, l1-l2+1, l1)
  if(s == mySubString) return(TRUE)
  return(FALSE)
  
}

beginsWith <- function(myString, mySubString) {
  
  l1 <- nchar(myString)
  l2 <- nchar(mySubString)
  if(l1 < l2) return(FALSE)
  s <- substr(myString, 1, l2)
  if(s == mySubString) return(TRUE)
  return(FALSE)
  
}

vectorToActMap <- function(v, nrow=0, ncol=0) {
  
  if(nrow==0) nrow <- ncol(m) / ncol  
  return(matrix(v, nrow=nrow, byrow=TRUE))
  
}
  
matrixToActMapList <- function(m, nrow=0, ncol=0) {
  
  #if(nrow==0) nrow <- ncol(m) / ncol
  
  actMapList <- apply(m, 1, function(x) { return(list(vectorToActMap(x,nrow,ncol)))})
  #actMapList <- apply(m, 1, function(x) { return(list(matrix(x, nrow=nrow)))})
  actMapList <- lapply(actMapList, function(x) {x[[1]]})

  actMapList
} 

##Rotate a square matrix 90 degrees clockwise.
rot90 <- function(a) {
  n <- dim(a)[1]
#  stopifnot(n==dim(a)[2])
  t(a[n:1, ])
}

plotActMap <- function(x, ...) {
  image(rot90(x),...)
}
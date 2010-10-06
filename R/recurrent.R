
recurrent <- function(x, ...) UseMethod("recurrent")

recurrent.default <- function(x, y, size=c(5), decay=0.2, maxit=100, linout=FALSE) {
  
  snns <- NULL
  x <- as.matrix(x)
  y <- as.matrix(y)
  if(any(is.na(x))) stop("missing values in 'x'")
  if(any(is.na(y))) stop("missing values in 'y'")
  if(dim(x)[1L] != dim(y)[1L]) stop("nrows of 'x' and 'y' must match")
  
  #layers <- c(dim(x)[2L], size, dim(y)[2L])
  snns$n <- c(dim(x)[2L], size, dim(y)[2L])
  snns$nunits <- as.integer(1L + sum(snns$n))
  snns$nconn <- rep(0, snns$nunits+1L)
  snns$conn <- numeric(0L)
  
  snnsObject <- SnnsRObjectFactory()
 
  #--------------------------------------------------------------
  snnsObject$setLearnFunc('JE_BP')
  snnsObject$setUpdateFunc('JE_Order')
  snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')

  #recurrent <-  
  #snnsObject$elman_createNet(c(2,8,2),c(1,1,1),FALSE)
  snnsObject$elman_createNet(snns$n,seq(1,1,length=length(snns$n)),FALSE)
  
  #snnsObject$jordan_createNet(2,8,2,1,2,1)
    
  patset <- snnsObject$createPatterns(x, y)
  snnsObject$setCurrPatSet(patset$set_no)
  
  snnsObject$initializeNet(c(1.0,  -1.0,  0.3,  1.0,  0.5) )
  #snnsObject$initializeNet(-1)
  snnsObject$shufflePatterns(TRUE)
  snnsObject$DefTrainSubPat()
  
  parameters <- c(decay, 0, 0)
  #--------------------------------------------------------------
  
  error <- vector()
  for(i in 1:maxit) {
    res <- snnsObject$learnAllPatterns(parameters)
    if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
    error[i] <- res[[2]]
  }
  
  #snns$recurrent <- recurrent
  snns$generalErrorIterations <- as.matrix(error)
  fit <- snnsObject$predictValues(x)
  
  snns$fitted.values <- fit
  snns$snnsObject <- snnsObject
  
  class(snns) <- "recurrent"
  snns  
}

print.recurrent <- function(x, ...)
{
  if(!inherits(x, "recurrent")) stop("not a legitimate recurrent model")
  cat("a ",x$n," network", sep="")
  #cat("a ",x$n[1L],"-",x$n[2L],"-",x$n[3L]," network", sep="")
}

predict.recurrent <- function(object, newdata, type=c("raw","class"), ...)
{
  if(!inherits(object, "recurrent")) stop("object not of class \"recurrent\"")
  type <- match.arg(type)
  if(missing(newdata)) z <- fitted(object)
  else {
    if(is.null(dim(newdata)))
      dim(newdata) <- c(1L, length(newdata)) # a row vector
    x <- as.matrix(newdata)     # to cope with dataframes
    if(any(is.na(x))) stop("missing values in 'x'")
    
    keep <- 1L:nrow(x)
    rn <- rownames(x)
    
    ntr <- nrow(x)
    nout <- object$n[length(object$n)]
    
    #nout <- object$snnsObject@layers[length(object$snnsObject@layers)]
    
    z <- matrix(NA, nrow(newdata), nout,
        dimnames = list(rn, dimnames(object$fitted.values)[[2L]]))
    
    #predict values.. 
    patset <- object$snnsObject$createPatterns(newdata) 
    object$snnsObject$setCurrPatSet(patset$set_no)
    predictions <- object$snnsObject$predictValues(newdata)
    z[keep,] <- predictions
  }
  z
}

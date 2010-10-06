
mlp <- function(x, ...) UseMethod("mlp")

mlp.default <- function(x, y, size=c(5), decay=0.2, maxit=100, linout=FALSE) {
  
  snns <- NULL
  x <- as.matrix(x)
  y <- as.matrix(y)
  if(any(is.na(x))) stop("missing values in 'x'")
  if(any(is.na(y))) stop("missing values in 'y'")
  if(dim(x)[1L] != dim(y)[1L]) stop("nrows of 'x' and 'y' must match")
  
  snns$n <- c(dim(x)[2L], size, dim(y)[2L])
  snns$nunits <- as.integer(1L + sum(snns$n))
  snns$nconn <- rep(0, snns$nunits+1L)
  snns$conn <- numeric(0L)
  
  SnnsKrui_setLearnFunc('Quickprop')
  SnnsKrui_setUpdateFunc('Topological_Order')
  SnnsKrui_setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')
  
  mlp <- SnnsR_createFullyConnectedFeedForwardNet(snns$n)
  patset <- SnnsR_createPatterns(x, y)
  SnnsKrui_setCurrPatSet(patset)
  
  SnnsKrui_initializeNet(-1)
  SnnsKrui_shufflePatterns(TRUE)
  SnnsKrui_DefTrainSubPat()
  
  error <- vector()
  for(i in 1:maxit) {
    res <- SnnsKrui_learnAllPatterns(c(decay, 0, 0, 0))
    if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
    error[i] <- res[[2]]
  }
  
  snns$mlp <- mlp
  snns$generalErrorIterations <- error
  fit <- SnnsR_predictValues(x)
  
  snns$fitted.values <- fit
  
  class(snns) <- "mlp"
  snns  
}

print.mlp <- function(x, ...)
{
  if(!inherits(x, "mlp")) stop("not a legitimate mlp model")
  cat("a ",x$n," network", sep="")
  #cat("a ",x$n[1L],"-",x$n[2L],"-",x$n[3L]," network", sep="")
}

predict.mlp <- function(object, newdata, type=c("raw","class"), ...)
{
  if(!inherits(object, "mlp")) stop("object not of class \"mlp\"")
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
    
    z <- matrix(NA, nrow(newdata), nout,
        dimnames = list(rn, dimnames(object$fitted.values)[[2L]]))
    
    #predict values.. 
    patset <- SnnsR_createPatterns(newdata) 
    SnnsKrui_setCurrPatSet(patset)
    predictions <- SnnsR_predictValues(newdata)
    z[keep,] <- predictions
  }
  z
}


#-------------------------------------
# Util functions for string processing

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

#--------------------------
# Util functions for 2d output


##Rotate a square matrix 90 degrees clockwise.
rot90 <- function(a) {
  n <- dim(a)[1]
#  stopifnot(n==dim(a)[2])
  t(a[n:1, ])
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


plotActMap <- function(x, ...) {
  image(rot90(x),...)
}
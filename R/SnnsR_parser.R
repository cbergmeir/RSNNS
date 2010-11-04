#' Extract the current pattern set to a matrix.
#' 
#' Extracts all patterns of the current pattern set and
#' returns them as a matrix. Columns are named with the prefix "in" or "out", respectively.
#'  
#' @return a matrix containing the patterns of the currently loaded patern set.
#' @rdname SnnsRObject$extractPatterns
#' @usage \S4method{extractPatterns}{SnnsR}()
#' @aliases extractPatterns,SnnsR-method SnnsRObject$extractPatterns
SnnsR__extractPatterns <- function(snnsObject)  {
  
  noPatterns <- snnsObject$getNoOfPatterns()
  
  inputs <- NULL
  outputs <- NULL
  
  for(i in 1:noPatterns)  {
    #INPUT: 1
    input <- snnsObject$getSubPatData(i-1, 0, 1)
    #OUTPUT: 2
    output <- snnsObject$getSubPatData(i-1, 0, 2)
    
    inputs <- rbind(inputs, input)
    outputs <- rbind(outputs, output)
  }
  
  colnames(inputs) <- paste("in", 1:ncol(inputs), sep="") 
  
  table <- inputs
  
  if(ncol(outputs) != 0)  {
    colnames(outputs) <- paste("out", 1:ncol(outputs), sep="")
    table <- cbind(table, outputs)
  } 
    
  
  #inputs
  #outputs
  
  rownames(table) <- paste("pattern", 1:nrow(table), sep="")
  
  return(table)
}
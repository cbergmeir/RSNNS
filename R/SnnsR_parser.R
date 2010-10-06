SnnsR_extractPatterns <- function()  {
  
  noPatterns <- SnnsKrui_getNoOfPatterns()
  
  inputs <- NULL
  outputs <- NULL
  
  for(i in 1:noPatterns)  {
    #INPUT: 1
    input <- SnnsKr_getSubPatData(i-1, 0, 1)
    #OUTPUT: 2
    output <- SnnsKr_getSubPatData(i-1, 0, 2)
    
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
readResFile <- function(filename)  {
  
  allData <- scan(filename, what="list", multi.line=TRUE)
  
  table <- NULL
  mode <- "omitHeader"
  lineName <- NULL
  line <- NULL
  for(i in 1:length(allData)) {
    
    if(mode == "omitHeader")
    {
      firstSymbol <- substr(allData[i],1,1)
      if(!is.na(firstSymbol) && firstSymbol == "#")  {
        lineName <- allData[i]
        mode <- "parseData"
      }
    } else if(mode == "parseData")  {
      firstSymbol <- substr(allData[i],1,1)
      
      if(!is.na(firstSymbol) && firstSymbol == "#")  {
        table <- rbind(table, line)
        rownames(table)[nrow(table)] <- lineName
        
        lineName <- allData[i]
        line <- NULL
      } else  {
        line <- c(line, as.numeric(allData[i]))
      }    
    }
  }
  table <- rbind(table, line)
  rownames(table)[nrow(table)] <- lineName
  
  #table
  prediction <- table[,ncol(table)]
  return(prediction)
  
}


savePatFile <- function(inputs, targets, filename)  {

  snnsObject <- SnnsRObjectFactory()

  snnsObject$createFullyConnectedFeedForwardNet(unitsPerLayer=c(ncol(as.matrix(inputs)),ncol(as.matrix(targets))))
  
  patSet <- snnsObject$createPatSet(inputs, targets)
  snnsObject$saveNewPatterns(filename, patSet$set_no)
  
  rm(snnsObject)
  
}

readPatFile <- function(filename)  {
    
  snnsObject <- SnnsRObjectFactory()
  
  snnsObject$loadNewPatterns(filename)
  
  patterns <- snnsObject$extractPatterns()
  
  rm(snnsObject)
  
  return(patterns)
  
}

#' Function to get the columns that are inputs
#'
#' This function extracts all columns from a matrix whose column names begin with "in".
#' The example data of this package follows this naming convention. 
#' 
#' @param patterns matrix or data.frame containing the patterns 
#' @export
#' @author Christoph
inputColumns <- function(patterns)  {
  
  res <- which(substr(colnames(patterns),1,2) == "in")
  return(res)

}

#' Function to get the columns that are targets
#'
#' This function extracts all columns from a matrix whose column names begin with "out".
#' The example data of this package follows this naming convention. 
#' 
#' @param patterns matrix or data.frame containing the patterns 
#' @export
#' @author Christoph
outputColumns <- function(patterns)  {
  
  res <- which(substr(colnames(patterns),1,3) == "out")
  return(res)

}


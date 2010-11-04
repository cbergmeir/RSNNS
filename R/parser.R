#' Rudimentary parser for .res files.
#'
#' This function contains a rudimentary parser for the SNNS .res files. It
#' is completely implemented in R and doesn't make use of SNNS.
#' 
#' @param filename the name of the .res file 
#' @return a matrix containing the predicted values that were found in the .res file
#' @export
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

#' Save data to a .pat file.
#'
#' This function generates an instance of SNNS (i.e. an \link{SnnsR-class} object), loads the given
#' data there as a pattern set and then uses the functionality of SNNS to save the data as a .pat file. 
#' 
#' @param inputs a matrix with input values 
#' @param targets a matrix with target values
#' @param filename the name of the .pat file 
#' @export
savePatFile <- function(inputs, targets, filename)  {

  snnsObject <- SnnsRObjectFactory()

  snnsObject$createFullyConnectedFeedForwardNet(unitsPerLayer=c(ncol(as.matrix(inputs)),ncol(as.matrix(targets))))
  
  patSet <- snnsObject$createPatSet(inputs, targets)
  snnsObject$saveNewPatterns(filename, patSet$set_no)
  
  rm(snnsObject)
  
}

#' Load data from a .pat file.
#'
#' This function generates an instance of SNNS (i.e. an \link{SnnsR-class} object), loads the given
#' .pat file there as a pattern set and then extracts the patterns to a matrix, using \link{SnnsRObject$extractPatterns}. 
#' 
#' @param filename the name of the .pat file
#' @return a matrix containing the data loaded from the .pat file. 
#' @export
readPatFile <- function(filename)  {
    
  snnsObject <- SnnsRObjectFactory()
  
  snnsObject$loadNewPatterns(filename)
  
  patterns <- snnsObject$extractPatterns()
  
  rm(snnsObject)
  
  return(patterns)
  
}

#' Function to get the columns that are inputs.
#'
#' This function extracts all columns from a matrix whose column names begin with "in".
#' The example data of this package follows this naming convention. 
#' 
#' @param patterns matrix or data.frame containing the patterns 
#' @export
inputColumns <- function(patterns)  {
  
  res <- which(substr(colnames(patterns),1,2) == "in")
  return(res)

}

#' Function to get the columns that are targets.
#'
#' This function extracts all columns from a matrix whose column names begin with "out".
#' The example data of this package follows this naming convention. 
#' 
#' @param patterns matrix or data.frame containing the patterns 
#' @export
outputColumns <- function(patterns)  {
  
  res <- which(substr(colnames(patterns),1,3) == "out")
  return(res)

}


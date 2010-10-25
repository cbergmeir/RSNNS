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

inputColumns <- function(table)  {
  
  res <- which(substr(colnames(table),1,2) == "in")
  return(res)

}

outputColumns <- function(table)  {
  
  res <- which(substr(colnames(table),1,3) == "out")
  return(res)

}


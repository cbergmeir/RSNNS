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

savePatFile <- function(inputValues, targetValues, filename)  {

 SnnsR_resetRSNNS()

  SnnsR_createFullyConnectedFeedForwardNet(c(ncol(inputValues),1,ncol(targetValues)))
  patset <- SnnsR_createPatterns(inputValues, targetValues)
  SnnsKrui_saveNewPatterns(filename, patset)
  
 SnnsR_resetRSNNS()
  
}

readPatFile <- function(filename)  {
    
 SnnsR_resetRSNNS()
  
  SnnsKrui_loadNewPatterns(filename)
  
  patterns <- SnnsR_extractPatterns()
  
 SnnsR_resetRSNNS()
  
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

#readPatFile <- function(filename)  {
#  
#  #examplesPath <- ("/home/bergmeir/Download/SNNSv4.3/examples")
#  #filename <- paste(examplesPath,"/eight_016.pat",sep="")
#  
#  allData <- scan(filename, what="list", multi.line=TRUE)
#  
#  inTable <- NULL
#  outTable <- NULL
#  
#  mode <- "omitHeader"
#  lineName <- NULL
#  line <- NULL
#
#  i <- 0
#  while (i < length(allData)) {
#    
#    i <- i + 1
#    
#    if(mode == "omitHeader")
#    {
#      firstSymbol <- substr(allData[i],1,1)
#      if(!is.na(firstSymbol) && firstSymbol == "#")  {
#        lineName <- paste(allData[i+1], allData[i+2], allData[i+3], sep=" ")
#        i <- i + 3
#        mode <- "parseData"
#      }
#    } else if(mode == "parseData")  {
#      firstSymbol <- substr(allData[i],1,1)
#
#      if(!is.na(firstSymbol) && firstSymbol == "#")  {
#        
#        if(allData[i+1] == "Output") {
#          inTable <- rbind(inTable, line)  
#          rownames(inTable)[nrow(inTable)] <- lineName
#        } else  {
#          outTable <- rbind(outTable, line)
#          rownames(outTable)[nrow(outTable)] <- lineName
#        }
#
#        lineName <- paste(allData[i+1], allData[i+2], allData[i+3], sep=" ")
#        i <- i + 3
#        line <- NULL
#      } else  {
#        line <- c(line, as.numeric(allData[i]))
#      }  
#      
#      
#    }
#  } 
#  outTable <- rbind(outTable, line)
#  rownames(outTable)[nrow(outTable)] <- lineName
#  
#  colnames(inTable) <- paste("in", 1:ncol(inTable), sep="") 
#  colnames(outTable) <- paste("out", 1:ncol(outTable), sep="")
#  
#  #inTable
#  #outTable
#  
#  table <- cbind(inTable, outTable)
#  rownames(table) <- paste("pattern", 1:nrow(table), sep="")
#  #table
#  return(table)
#
#}
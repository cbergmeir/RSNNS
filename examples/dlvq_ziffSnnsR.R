library(RSNNS)

basePath <- ("/home/bergmeir")
#examplesPath <- ("/home/bergmeir/Download/SNNSv4.3/examples")
#load("/home/bergmeir/ts-code/Rpackages/RSNNS/data/snnsData.RData")

data(snnsData)

dataset <- snnsData$dlvq_ziff_100.pat

inputs <- dataset[,inputColumns(dataset)]
outputs <- dataset[,outputColumns(dataset)]

snnsObject <- SnnsRObjectFactory()

snnsObject$setLearnFunc('Dynamic_LVQ')
snnsObject$setUpdateFunc('Dynamic_LVQ')
snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')

snnsObject$createNet(c(ncol(inputs), 1), fullyConnectedFeedForward = FALSE)

patset <- snnsObject$createPatterns(inputs, outputs)
snnsObject$setCurrPatSet(patset$set_no)

#snnsObject$error(-47)
#snnsObject$getFuncParamInfo("ART1_Weights", 6);

snnsObject$initializeNet(c(1.0, -1.0))
#snnsObject$initializeNet(c(1.0, 1.0))
snnsObject$shufflePatterns(TRUE)
snnsObject$DefTrainSubPat()

#snnsObject$saveNet(paste(basePath,"/art1_test_untrained.net",sep=""),"art1_test_untrained")

parameters <- c(0.03, 0.03, 10.0, 0.0, 0.0)
maxit <- 100

#error <- vector()
for(i in 1:maxit) {
  res <- snnsObject$learnAllPatterns(parameters)
 # error[i] <- res[[2]]
}

#error

snnsObject$saveNet(paste(basePath,"/dlvq_test.net",sep=""),"dlvq_test")
#snnsObject$saveNewPatterns(paste(basePath,"/art1_test.pat",sep=""), patset$set_no);

#mapX <- 7
predictions <- snnsObject$predictValuesCurrPatSet()

mean(predictions - outputs)

#outputMaps <- apply(outputMaps, 1, function(x) { return(list(matrix(x, nrow=mapX, byrow=TRUE)))})
#outputMaps <- lapply(outputMaps, function(x) {x[[1]]})
#par(mfrow=c(3,3))

#outputMaps <- matrixToActMapList(outputs, nrow=7)

#for (i in 1:9) image(rot90(outputMaps[[i]]))

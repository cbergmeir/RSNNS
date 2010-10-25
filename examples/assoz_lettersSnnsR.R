library(RSNNS)

basePath <- ("/home/bergmeir")

data(snnsData)

patterns <- snnsData$art1_letters.pat

snnsObject <- SnnsRObjectFactory()

#snnsObject$setInitialisationFunc('RM_Random_Weights')
#snnsObject$setLearnFunc('RM_delta')
#snnsObject$setUpdateFunc('ART1_Synchronous')
snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Identity','Out_Identity')

snnsObject$assoz_createNet(5,7)

#snnsObject$setTTypeUnitsActFunc("UNIT_HIDDEN", "Act_RM")

patset <- snnsObject$createPatterns(patterns)
snnsObject$setCurrPatSet(patset$set_no)

#snnsObject$getFuncParamInfo("RM_delta", 4);

snnsObject$initializeNet(c(1.0, -1.0))
#snnsObject$initializeNet(c(1.0, 1.0))
snnsObject$shufflePatterns(TRUE)
snnsObject$DefTrainSubPat()

#snnsObject$saveNet(paste(basePath,"/art1_test_untrained.net",sep=""),"art1_test_untrained")
snnsObject$saveNet(paste(basePath,"/assoz_test.net",sep=""),"assoz_test")
parameters <- c(0.01, 100, 0.0, 0.0, 0.0)
maxit <- 100

for(i in 1:maxit) {
  res <- snnsObject$learnAllPatterns(parameters)
}


#snnsObject$saveNewPatterns(paste(basePath,"/art1_test.pat",sep=""), patset$set_no);

#mapX <- 7
outputs <- snnsObject$assozPredict()
#outputMaps <- apply(outputMaps, 1, function(x) { return(list(matrix(x, nrow=mapX, byrow=TRUE)))})
#outputMaps <- lapply(outputMaps, function(x) {x[[1]]})
par(mfrow=c(3,3))

outputMaps <- matrixToActMapList(outputs, nrow=7)

for (i in 1:9) image(rot90(outputMaps[[i]]))

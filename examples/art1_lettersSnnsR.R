library(RSNNS)

basePath <- ("/home/bergmeir")
#examplesPath <- ("/home/bergmeir/Download/SNNSv4.3/examples")
#load("/home/bergmeir/ts-code/Rpackages/RSNNS/data/snnsData.RData")

data(snnsData)

dataset <- snnsData$art1_letters.pat

inputs <- dataset[,inputColumns(dataset)]
outputs <- dataset[,outputColumns(dataset)]

snnsObject <- SnnsRObjectFactory()

snnsObject$setInitialisationFunc('ART1_Weights')
snnsObject$setLearnFunc('ART1')
snnsObject$setUpdateFunc('ART1_Synchronous')
snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')

snnsObject$art1_createNet(35,7,25,7)

patset <- snnsObject$createPatterns(inputs, outputs)
snnsObject$setCurrPatSet(patset$set_no)

#snnsObject$error(-47)
#snnsObject$getFuncParamInfo("ART1_Weights", 6);

snnsObject$initializeNet(c(1.0, 1.0))
#snnsObject$initializeNet(c(1.0, 1.0))
snnsObject$shufflePatterns(TRUE)
snnsObject$DefTrainSubPat()

snnsObject$saveNet(paste(basePath,"/art1_test_untrained.net",sep=""),"art1_test_untrained")

parameters <- c(0.9, 0, 0)
maxit <- 1000

error <- vector()
for(i in 1:maxit) {
  res <- snnsObject$learnAllPatterns(parameters)
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
  error[i] <- res[[2]]
}

#error[1:500]
#plot(error, type="l")
#lines(error, type="l")

snnsObject$saveNet(paste(basePath,"/art1_test.net",sep=""),"art1_test")
snnsObject$saveNewPatterns(paste(basePath,"/art1_test.pat",sep=""), patset$set_no);


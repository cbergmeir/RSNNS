library(RSNNS)

#examplesPath <- ("/home/bergmeir/Download/SNNSv4.3/examples")
#load("/home/bergmeir/ts-code/Rpackages/RSNNS/data/snnsData.RData")

data(snnsData)

inputs <- snnsData$eight_016.pat[,inputColumns(snnsData$eight_016.pat)]
outputs <- snnsData$eight_016.pat[,outputColumns(snnsData$eight_016.pat)]

snnsObject <- SnnsRObjectFactory()

snnsObject$setLearnFunc('JE_BP')
snnsObject$setUpdateFunc('JE_Order')
snnsObject$setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')

snnsObject$elman_createNet(c(2,8,2),c(1,1,1),FALSE)

#IUnits, HUnits, OUnits, ICols, HCols, OCols
#snnsObject$jordan_createNet(2,8,2,1,2,1)

patset <- snnsObject$createPatterns(inputs, outputs)
#snnsObject$setCurrPatSet(patset)
snnsObject$setCurrPatSet(patset$set_no)

snnsObject$initializeNet(c(1.0,  -1.0,  0.3,  1.0,  0.5) )
snnsObject$shufflePatterns(TRUE)
snnsObject$DefTrainSubPat()

parameters <- c(0.2, 0, 0)
maxit <- 1000

error <- vector()
for(i in 1:maxit) {
  res <- snnsObject$learnAllPatterns(parameters)
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
  error[i] <- res[[2]]
}

error[1:500]
plot(error, type="l")

#lines(error, type="l")

#basePath <- ("/home/bergmeir")
#snnsObject$saveNet(paste(basePath,"/elman_test.net",sep=""),"elman_test")
#snnsObject$saveNewPatterns(paste(basePath,"/elman_test.pat",sep=""), patset);


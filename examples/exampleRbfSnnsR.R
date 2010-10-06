library(RSNNS)

#examplesPath <- ("/home/bergmeir/Download/SNNSv4.3/examples")
#load("/home/bergmeir/ts-code/Rpackages/RSNNS/data/snnsData.RData")

data(snnsData)

inputs <- snnsData$spirals.pat[,inputColumns(snnsData$spirals.pat)]
outputs <- snnsData$spirals.pat[,outputColumns(snnsData$spirals.pat)]

snnsObject <- snnsObjectFactory()

snnsObject$setLearnFunc('RBF-DDA')
snnsObject$setUpdateFunc('Topological_Order')
snnsObject$setUnitDefaults(0,0,1,0,1,'Act_Logistic','Out_Identity')

#snnsObject$setLearnFunc('RadialBasisLearning')
#snnsObject$setUpdateFunc('Topological_Order')
#snnsObject$setUnitDefaults(0,0,1,0,1,'Act_RBF_Gaussian','Out_Identity')
#snnsObject$setInitialisationFunc('RBF_Weights')
#snnsObject$createNet(c(2,45,2), TRUE)

snnsObject$createNet(c(2,2), FALSE)



patset <- snnsObject$createPatterns(inputs, outputs)
snnsObject$setCurrPatSet(patset$set_no)

#snnsObject$initializeNet(c(1.0,  -1.0,  0.3,  1.0,  0.5) )
#snnsObject$initializeNet(0)
snnsObject$shufflePatterns(TRUE)
snnsObject$DefTrainSubPat()

parameters <- c(0.4, 0.2, 5)
maxit <- 1000

error <- vector()
for(i in 1:maxit) {
  res <- snnsObject$learnAllPatterns(parameters)
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
  error[i] <- res[[2]]
}

error[1:500]
plot(error, type="l")

basePath <- ("/home/bergmeir")
snnsObject$saveNet(paste(basePath,"/rbf_testSnnsCLib.net",sep=""),"rbf_test")
#snnsObject$saveNewPatterns(paste(basePath,"/rbf_testSnnsCLib.pat",sep=""), patset);











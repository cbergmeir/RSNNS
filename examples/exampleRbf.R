library(RSNNS)

#examplesPath <- ("/home/bergmeir/Download/SNNSv4.3/examples")
#load("/home/bergmeir/ts-code/Rpackages/RSNNS/data/snnsData.RData")

data(snnsData)

inputs <- snnsData$spirals.pat[,inputColumns(snnsData$spirals.pat)]
outputs <- snnsData$spirals.pat[,outputColumns(snnsData$spirals.pat)]

SnnsKrui_setLearnFunc('RBF-DDA')
SnnsKrui_setUpdateFunc('Topological_Order')
SnnsKrui_setUnitDefaults(0,0,1,0,1,'Act_Logistic','Out_Identity')

#SnnsKrui_setLearnFunc('RadialBasisLearning')
#SnnsKrui_setUpdateFunc('Topological_Order')
#SnnsKrui_setUnitDefaults(0,0,1,0,1,'Act_RBF_Gaussian','Out_Identity')
#SnnsKrui_setInitialisationFunc('RBF_Weights')
#SnnsR_createNet(c(2,45,2), TRUE)

SnnsR_createNet(c(2,2), FALSE)



patset <- SnnsR_createPatterns(inputs, outputs)
SnnsKrui_setCurrPatSet(patset)

#SnnsKrui_initializeNet(c(1.0,  -1.0,  0.3,  1.0,  0.5) )
#SnnsKrui_initializeNet(0)
SnnsKrui_shufflePatterns(TRUE)
SnnsKrui_DefTrainSubPat()

parameters <- c(0.4, 0.2, 5)
maxit <- 1000

error <- vector()
for(i in 1:maxit) {
  res <- SnnsKrui_learnAllPatterns(parameters)
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
  error[i] <- res[[2]]
}

error[1:500]
plot(error, type="l")

basePath <- ("/home/bergmeir")
SnnsKrui_saveNet(paste(basePath,"/rbf_test.net",sep=""),"rbf_test")
#SnnsKrui_saveNewPatterns(paste(basePath,"/elman_test.pat",sep=""), patset);











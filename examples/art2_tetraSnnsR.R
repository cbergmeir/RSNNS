library(RSNNS)

#basePath <- ("/home/bergmeir")
#examplesPath <- ("/home/bergmeir/Download/SNNSv4.3/examples")
#load("/home/bergmeir/ts-code/Rpackages/RSNNS/data/snnsData.RData")

data(snnsData)
dataset <- snnsData$art2_tetra.pat

inputs <- dataset

snnsObject <- SnnsRObjectFactory()

snnsObject$setUnitDefaults(0,0,3,0,1,'Act_Logistic','Out_Identity')

snnsObject$art2_createNet(3,3,5,5)

snnsObject$setInitialisationFunc('ART2_Weights')
snnsObject$setLearnFunc('ART2')
snnsObject$setUpdateFunc('ART2_Stable')

patset <- snnsObject$createPatterns(inputs)
snnsObject$setCurrPatSet(patset$set_no)

#snnsObject$error(-47)
#snnsObject$getFuncParamInfo("ART1_Weights", 6);

snnsObject$initializeNet(c(0.9, 2.0, 0, 0, 0))
#snnsObject$initializeNet(c(1.0, 1.0))
snnsObject$shufflePatterns(TRUE)
snnsObject$DefTrainSubPat()

snnsObject$saveNet("/home/bergmeir/art2_test_untrained.net","art2_test_untrained")

parameters <- c(0.98, 10.0, 10.0, 0.1, 0.0)
maxit <- 100

for(i in 1:maxit) {
  res <- snnsObject$learnAllPatterns(parameters)
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
}

snnsObject$saveNet("/home/bergmeir/art2_test.net","art2_test")
#snnsObject$saveNet(paste(basePath,"/art1_test.net",sep=""),"art1_test")
#snnsObject$saveNewPatterns(paste(basePath,"/art1_test.pat",sep=""), patset$set_no);


testdata <- snnsData$art2_tetra_med.pat
testPatset <- snnsObject$createPatterns(testdata)
snnsObject$setCurrPatSet(testPatset$set_no)

outputs <- snnsObject$art2Predict(parameters)
outputs

#inputs

#par(mfrow=c(2,2))
#plotRegressionError(inputs[,1], outputs[,1])
#plotRegressionError(inputs[,2], outputs[,2])
#plotRegressionError(inputs[,3], outputs[,3])

#par(mfrow=c(2,2))
#plotRegressionError(inputs[,1], outputs[,4])
#plotRegressionError(inputs[,2], outputs[,5])
#plotRegressionError(inputs[,3], outputs[,6])

library(scatterplot3d)
scatterplot3d(outputs[,1:3])

#warnings()
#par(mfrow=c(3,3))
#outputMaps <- matrixToActMapList(outputs, nrow=2)
#for (i in 1:9) image(rot90(outputMaps[[i]]))

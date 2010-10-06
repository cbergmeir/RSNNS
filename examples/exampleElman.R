library(RSNNS)

#examplesPath <- ("/home/bergmeir/Download/SNNSv4.3/examples")
#load("/home/bergmeir/ts-code/Rpackages/RSNNS/data/snnsData.RData")

data(snnsData)

inputs <- snnsData$eight_016.pat[,inputColumns(snnsData$eight_016.pat)]
outputs <- snnsData$eight_016.pat[,outputColumns(snnsData$eight_016.pat)]

SnnsKrui_setLearnFunc('JE_BP')
SnnsKrui_setUpdateFunc('JE_Order')
SnnsKrui_setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')

#SnnsBn_elman_createNet(c(2,8,2),c(1,1,1),FALSE)

#IUnits, HUnits, OUnits, ICols, HCols, OCols
SnnsBn_jordan_createNet(2,8,2,1,2,1)

patset <- SnnsR_createPatterns(inputs, outputs)
SnnsKrui_setCurrPatSet(patset)

SnnsKrui_initializeNet(c(1.0,  -1.0,  0.3,  1.0,  0.5) )
SnnsKrui_shufflePatterns(TRUE)
SnnsKrui_DefTrainSubPat()

parameters <- c(0.2, 0, 0)
maxit <- 1000

error <- vector()
for(i in 1:maxit) {
  res <- SnnsKrui_learnAllPatterns(parameters)
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
  error[i] <- res[[2]]
}

error[1:500]
plot(error, type="l")

#basePath <- ("/home/bergmeir")
#SnnsKrui_saveNet(paste(basePath,"/elman_test.net",sep=""),"elman_test")
#SnnsKrui_saveNewPatterns(paste(basePath,"/elman_test.pat",sep=""), patset);











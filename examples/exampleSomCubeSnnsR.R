library(RSNNS)

data(snnsData)

inputs <- snnsData$som_cube.pat

snnsObject <- SnnsRObjectFactory()

snnsObject$setLearnFunc('Kohonen')
snnsObject$setUpdateFunc('Kohonen_Order')
snnsObject$setInitialisationFunc('KOHONEN_Weights')
snnsObject$setUnitDefaults(0,0,3,0,1,'Act_Logistic','Out_Identity')

snnsObject$kohonen_createNet(16,16,3,256)


snnsObject$setTTypeUnitsActFunc("UNIT_HIDDEN", "Act_Euclid")

patset <- snnsObject$createPatterns(inputs)#, outputs)
snnsObject$setCurrPatSet(patset$set_no)

snnsObject$initializeNet(c(1.0,  -1.0))
snnsObject$shufflePatterns(TRUE)
snnsObject$DefTrainSubPat()

parameters <- c(0.855, 9.0, 0.9, 0.9, 16.0)
maxit <- 1000

for(i in 1:maxit) {
  res <- snnsObject$learnAllPatterns(parameters)
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
}

results <- snnsObject$somPredictCurrPatSet()

maps <- apply(results, 1, function(x) { return(list(matrix(x, nrow=16)))})

#par(mfrow=c(3,3))
#lapply(maps, function(x){
#      persp(1:16, 1:16, x[[1]], theta = 30, phi = 30, expand = 0.5, col = "lightblue")
#    })


par(mfrow=c(1,2))
image(maps[[1]][[1]])
persp(1:16, 1:16, maps[[1]][[1]], theta = 30, phi = 30, expand = 0.5, col = "lightblue")

basePath <- ("/home/bergmeir")
#snnsObject$saveNet(paste(basePath,"/kohonen_test.net",sep=""),"kohonen_test")
#snnsObject$saveNewPatterns(paste(basePath,"/elman_test.pat",sep=""), patset$set_no);


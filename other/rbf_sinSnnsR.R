library(RSNNS)

basePath <- ("D:/Doctorado/")

#x <- seq(0,1,0.01)
#y <- sin(10*x)

x <- seq(0,10,0.1)
y <- sin(x)

#model <- rbf(x, y, size=80, maxit=1, initFunc="RBF_Weights", initFuncParams=c(-4,  4,  0.0,  0.02,  0.01),
#             learnFuncParams=c(0.01, 0, 0.01, 0.1, 0.9), linOut=FALSE)
#model <- rbf(x, y, size=80, maxit=50, initFunc="RBF_Weights_Kohonen",
#    initFuncParams=c(50,  0.4,  0), learnFuncParams=c(0.01, 0, 0.01, 0.1, 0.8), linOut=FALSE)

#data(snnsData)
#inputs <- snnsData$spirals.pat[,inputColumns(snnsData$spirals.pat)]
#outputs <- snnsData$spirals.pat[,outputColumns(snnsData$spirals.pat)]

inputs <- x
outputs <- y

#rbf.default <- function(x, y, size=c(5), maxit=100, 
#    initFunc="RBF_Weights", initFuncParams=, 
#    learnFunc="RadialBasisLearning", learnFuncParams=, 
#    updateFunc="Topological_Order", updateFuncParams=c(0.0),
#    shufflePatterns=TRUE, linOut=FALSE,

snnsObject <- SnnsRObjectFactory()

snnsObject$setLearnFunc('RadialBasisLearning')
snnsObject$setUpdateFunc('Topological_Order')
snnsObject$setUnitDefaults(0,0,1,0,1,'Act_Logistic','Out_Identity')

snnsObject$setLearnFunc('RadialBasisLearning')
snnsObject$setUpdateFunc('Topological_Order')
snnsObject$setUnitDefaults(0,0,1,0,1,'Act_RBF_Gaussian','Out_Identity')
#snnsObject$setInitialisationFunc('RBF_Weights')
snnsObject$createNet(c(1,40,1), TRUE)

snnsObject$setTTypeUnitsActFunc("UNIT_OUTPUT", "Act_IdentityPlusBias")

patset <- snnsObject$createPatSet(inputs, outputs)
snnsObject$setCurrPatSet(patset$set_no)

snnsObject$shufflePatterns(TRUE)
snnsObject$DefTrainSubPat()

snnsObject$initializeNet(c(0,0,0), "RBF_Weights_Kohonen")
#snnsObject$initializeNet(c(-4, 4, 0, 0.435, 0.05), "RBF_Weights")
snnsObject$initializeNet(c(0, 1, 0, 0.02, 0.01), "RBF_Weights")
snnsObject$learnAllPatterns(c(0,0,0,0.1,0.9))

#snnsObject$error(-130)

snnsObject$saveNet(paste(basePath,"rbf_sinSnnsR_untrained.net",sep=""),"rbf_sinSnnsR_untrained.net")

parameters <- c(0, 0, 0, 0.1, 0.9)
#parameters <- c(0.01, 0, 0.01, 0.1, 0.8)
maxit <- 100

#error <- vector()
#for(i in 1:maxit) {
#  res <- snnsObject$learnAllPatterns(parameters)
#  error[i] <- res[[2]]
#}

#error
#plot(error, type="l")

predictions <- snnsObject$predictCurrPatSet("output", c(0))
predictions

plot(x,y)
lines(x, predictions, col="green")
#predictions

#snnsObject$saveNet(paste(basePath,"rbfDDA_spiralsSnnsR.net",sep=""),"rbfDDA_spiralsSnnsR")
#snnsObject$saveNewPatterns(paste(basePath,"rbfDDA_spiralsSnnsR.pat",sep=""), patset$set_no);


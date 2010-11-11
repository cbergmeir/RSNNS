library(RSNNS)

seed <- 2
set.seed(seed)
setSnnsRSeedValue(seed)

data(iris)

#shuffle the vector
iris <- iris[sample(1:nrow(iris),length(1:nrow(iris))),1:ncol(iris)]

#normalize data
irisValues <- normalizeData(iris[,1:4], "norm")
#irisValues <- normalizeData(iris[,1:4], "0_1")

irisTargets <- decodeClassLabels(iris[,5])
#irisTargets <- decodeClassLabels(iris[,5], valTrue=0.9, valFalse=0.1)

iris <- splitForTrainingAndTest(irisValues, irisTargets, ratio=0.15)

model <- mlp(iris$inputsTrain, iris$targetsTrain, size=5, learnFuncParams=c(0.1), 
    maxit=50, inputsTest=iris$inputsTest, targetsTest=iris$targetsTest)

#model <- rbfDDA(iris$inputsTrain, iris$targetsTrain)

#model <- elman(iris$inputsTrain, iris$targetsTrain, size=5, learnFuncParams=c(0.1), maxit=100, inputsTest=iris$inputsTest, targetsTest=iris$targetsTest)

#model <- rbf(iris$inputsTrain, iris$targetsTrain, size=40, maxit=200, initFuncParams=c(-4, 4,  0.0,  0.2,  0.04), 
#             learnFuncParams=c(1e-3, 0, 1e-3, 0.1, 0.8), linOut=FALSE)

#model <- rbf(iris$inputsTrain, iris$targetsTrain, size=40, maxit=600, initFuncParams=c(0, 1,  0.0,  0.2,  0.04), 
#             learnFuncParams=c(1e-5, 0, 1e-5, 0.1, 0.8), linOut=TRUE)
 
##experimental..:
##model <- rbf(iris$inputsTrain, iris$targetsTrain, size=20, maxit=50, initFunc="RBF_Weights_Kohonen",
##    initFuncParams=c(50,  0.4,  0), learnFuncParams=c(0.01, 0, 0.01, 0.1, 0.8))

#summary(model)

par(mfrow=c(2,2))

plotIterativeError(model)

predictions <- predict(model,iris$inputsTest)

plotRegressionError(predictions[,2], iris$targetsTest[,2])

confusionMatrix(iris$targetsTrain,fitted.values(model))
confusionMatrix(iris$targetsTest,predictions)

plotROC(fitted.values(model)[,2], iris$targetsTrain[,2])
plotROC(predictions[,2], iris$targetsTest[,2])

#confusion matrix with 402040-method
confusionMatrix(iris$targetsTrain, encodeClassLabels(fitted.values(model),method="402040", l=0.4, h=0.6))

model


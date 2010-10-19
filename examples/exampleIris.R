library(RSNNS)

data(iris)

#shuffle the vector
iris <- iris[sample(1:nrow(iris),length(1:nrow(iris))),1:ncol(iris)]

#center data
irisValues <- matrix(nrow=nrow(iris), ncol=4)
for(i in 1:4) {
  irisValues[,i] <- iris[,i] - mean(iris[,i])  
}

irisTargets <- decodeClassLabels(iris[,5])

iris <- splitForTrainingAndTest(irisValues, irisTargets, ratio=0.15)


mySlp <- mlp(iris$inputsTrain, iris$targetsTrain, size=5, decay=0.1, type="classification", maxit=200, iris$inputsTest, iris$targetsTest)
#mySlp <- rbfDDA(iris$inputsTrain, iris$targetsTrain, size=5, decay=0.1, type="classification", maxit=1000)#, iris$inputsTest, iris$targetsTest)
#mySlp <- elman(irisTrainValues, irisTrainTargets, size=5, decay=0.1, maxit=1000)


mySlp
summary(mySlp)
par(mfrow=c(2,2))

plotIterativeError(mySlp)

predictions <- predict(mySlp,iris$inputsTest)

#plotRegressionError(predictions[,2], iris$targetsTest[,2])

confusionMatrix(iris$targetsTrain,fitted.values(mySlp))
confusionMatrix(iris$targetsTest,predictions)

plotROC(fitted.values(mySlp)[,2], iris$targetsTrain[,2])
plotROC(predictions[,2], iris$targetsTest[,2])


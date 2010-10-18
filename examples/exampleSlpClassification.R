library(RWeka)
library(RSNNS)

iris <- read.arff(system.file("arff", "iris.arff", package = "RWeka"))

#shuffle the vector
iris <- iris[sample(1:nrow(iris),length(1:nrow(iris))),1:ncol(iris)]

#center data
irisValues <- matrix(nrow=nrow(iris), ncol=4)
for(i in 1:4) {
  irisValues[,i] <- iris[,i] - mean(iris[,i])  
}

irisTargets <- decodeClassLabels(iris[,5])

iris <- splitForTrainingAndTest(irisValues, irisTargets, ratio=0.15)

#irisTrainValues <- irisValues[1:120,] 
#irisTrainTargets <- irisTargets[1:120,] 

#irisTestValues <- irisValues[121:nrow(iris),]    
#irisTestTargets <- irisTargets[121:nrow(iris),]    

mySlp <- mlp(iris$inputsTrain, iris$targetsTrain, size=5, decay=0.1, type="classification", maxit=200, iris$inputsTest, iris$targetsTest)
#mySlp <- elman(irisTrainValues, irisTrainTargets, size=5, decay=0.1, maxit=1000)

#mySlp$snnsObject$getNoOfPatterns()

#mySlp
#summary(mySlp)
par(mfrow=c(2,2))

plotIterativeError(mySlp)

#plot(mySlp$generalErrorIterations, type="l")
#mySlp$generalErrorIterations



predictions <- predict(mySlp,iris$inputsTest)
#labels <- toNumericClassLabels(iris$targetsTest)
#labels <- encodeClassLabels(iris$targetsTest)
#apply(predictions, 1, function(x) {x[labels]})

#plotRegressionError(predictions[,2], iris$targetsTest[,2])

#rocpred <- vector()
#for(i in 1:nrow(predictions))  {
#  rocpred[i] <- predictions[i,labels[i]] - sum(predictions[i,-labels[i]]) 
#}
#apply(predictions, 1, function(x) {max(x) - sum(x[-which(x==max(x))])})

#regression plot
#plot(predictions[,2], as.numeric(labels==2))

confusionMatrix(iris$targetsTrain,fitted.values(mySlp))
confusionMatrix(iris$targetsTest,predictions)

plotROC(predictions[,2], iris$targetsTest[,2])
plotROC(fitted.values(mySlp)[,2], iris$targetsTrain[,2])

#drawROC(predictions[,2], as.numeric(labels==2))

#library(ROCR)
#pred <- ROCR::prediction(predictions[,2], as.numeric(labels==2))
#
#perf <- performance(pred,"tpr","fpr")
#plot(perf)
### precision/recall curve (x-axis: recall, y-axis: precision)
#perf1 <- performance(pred, "prec", "rec")
#plot(perf1)
### sensitivity/specificity curve (x-axis: specificity,
### y-axis: sensitivity)
#perf1 <- performance(pred, "sens", "spec")
#plot(perf1)

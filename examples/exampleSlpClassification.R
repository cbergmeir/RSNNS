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

irisTrainValues <- irisValues[1:120,] 
irisTrainTargets <- irisTargets[1:120,] 

irisTestValues <- irisValues[121:nrow(iris),]    
irisTestTargets <- irisTargets[121:nrow(iris),]    

mySlp <- mlp(irisTrainValues, irisTrainTargets, size=5, decay=0.1, type="classification", maxit=200)
#mySlp <- elman(irisTrainValues, irisTrainTargets, size=5, decay=0.1, maxit=1000)

#mySlp
#summary(mySlp)
par(mfrow=c(2,2))

plot(mySlp$generalErrorIterations, type="l")
#mySlp$generalErrorIterations

predictions <- predict(mySlp,irisTestValues)
labels <- toNumericClassLabels(iris[121:nrow(iris),5])

#apply(predictions, 1, function(x) {x[labels]})

#rocpred <- vector()
#for(i in 1:nrow(predictions))  {
#  rocpred[i] <- predictions[i,labels[i]] - sum(predictions[i,-labels[i]]) 
#}
#apply(predictions, 1, function(x) {max(x) - sum(x[-which(x==max(x))])})

#regression plot
#plot(predictions[,2], as.numeric(labels==2))

confusionMatrix(iris[1:120,5],fitted.values(mySlp))
confusionMatrix(iris[121:nrow(iris),5],predictions)

library(ROCR)
pred <- ROCR::prediction(predictions[,2], as.numeric(labels==2))

perf <- performance(pred,"tpr","fpr")
plot(perf)
## precision/recall curve (x-axis: recall, y-axis: precision)
perf1 <- performance(pred, "prec", "rec")
plot(perf1)
## sensitivity/specificity curve (x-axis: specificity,
## y-axis: sensitivity)
perf1 <- performance(pred, "sens", "spec")
plot(perf1)

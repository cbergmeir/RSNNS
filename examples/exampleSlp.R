library(RWeka)
iris <- read.arff(system.file("arff", "iris.arff", package = "RWeka"))
#iris[,5] <- as.numeric(iris[,5])

#shuffle the vector
iris <- iris[sample(1:nrow(iris),length(1:nrow(iris))),1:ncol(iris)]

#center data
irisValues <- matrix(nrow=nrow(iris), ncol=4)
for(i in 1:4) {
  irisValues[,i] <- iris[,i] - mean(iris[,i])  
}

#decode class labels
irisTargets <- matrix(nrow=nrow(iris), ncol=3)
for(i in 1:nrow(iris))
  for(j in 1:3)  {
    irisTargets[i,j] <- as.numeric(j == as.numeric(iris[i,5]))    
  }

irisTrainValues <- irisValues[1:120,] 
irisTrainTargets <- irisTargets[1:120,] 

irisTestValues <- irisValues[121:nrow(iris),]    
irisTestTargets <- irisTargets[121:nrow(iris),]    

library(RSNNS)

mySlp <- mlp(irisTrainValues, irisTrainTargets, size=5, decay=0.1, maxit=1000)

print(mySlp)

plot(mySlp$generalErrorIterations[1:500])#, type="l")

mySlp$generalErrorIterations[1:500]

fits <- round(fitted.values(mySlp))

notCorrect <- 0
for(i in 1:nrow(fits))  {
  if (length(which(fits[i,] != irisTrainTargets[i,])) != 0)  {
    notCorrect <- notCorrect + 1
  }
}
print(paste("Errors in fit: ",notCorrect, " of ", nrow(fits), "\n",sep=""))

predictions <- round(predict(mySlp,irisTestValues))
#round(predictions)

notCorrect <- 0
for(i in 1:nrow(predictions))  {
  if (length(which(predictions[i,] != irisTestTargets[i,])) != 0)  {
    notCorrect <- notCorrect + 1
  }
}
print(paste("Errors in test set: ",notCorrect, " of ", nrow(predictions), "\n",sep=""))

SnnsKrui_saveNet("/home/bergmeir/iris.net","iris")

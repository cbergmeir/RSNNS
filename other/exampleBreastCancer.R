library(RSNNS)
library(mlbench)

data(BreastCancer)
data <- BreastCancer

#BreastCancer[618,]

#shuffle the vector
BreastCancer <- BreastCancer[sample(1:nrow(BreastCancer),length(1:nrow(BreastCancer))),1:ncol(BreastCancer)]

#center data
#dataValues <- matrix(nrow=nrow(data), ncol=4)
#for(i in 2:10) {
#  dataValues[,i] <- data[,i] - mean(data[,i])  
#}

dataValues <- BreastCancer[,2:10]

#remove na's
colContainsNa <- apply(dataValues, 1, function(x) length(which(is.na(x))))
dataValues <- dataValues[-which(colContainsNa != 0),]

#normalize to 0-1
dataValues <- normalizeData(data.matrix(dataValues), "norm")
#dataValues <- normalizeData(data.matrix(dataValues), "0_1")

dataTargets <- decodeClassLabels(BreastCancer[,11])
data <- splitForTrainingAndTest(dataValues, dataTargets, ratio=0.25)

#learnFuncParams=c(0.4, 0.2, 5)

#model <- mlp(data$inputsTrain, data$targetsTrain, size=c(3), decay=0.05, type="classification", maxit=200, data$inputsTest, data$targetsTest)
model <- rbfDDA(data$inputsTrain, data$targetsTrain, maxit=1, learnFuncParams=c(0.02, 0.01, 20))
#model <- elman(data$inputsTrain, data$targetsTrain, size=5, decay=0.01, type="classification", maxit=200, data$inputsTest, data$targetsTest)
#model <- jordan(data$inputsTrain, data$targetsTrain, size=5, decay=0.01, type="classification", maxit=200, data$inputsTest, data$targetsTest)


model
#summary(model)
par(mfrow=c(2,2))

plotIterativeError(model)

predictions <- predict(model,data$inputsTest)

plotRegressionError(predictions[,2], data$targetsTest[,2])

confusionMatrix(data$targetsTrain,fitted.values(model))
confusionMatrix(data$targetsTest,predictions)

plotROC(fitted.values(model)[,2], data$targetsTrain[,2])
plotROC(predictions[,2], data$targetsTest[,2])

#table(targets=encodeClassLabels(data$targetsTrain), predictions=encodeClassLabels(fitted.values(model)))


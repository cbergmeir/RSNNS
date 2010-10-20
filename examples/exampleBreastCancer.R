library(RSNNS)
library(mlbench)
#library(som)

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
#dataValues <- normalize(data.matrix(dataValues))



#remove na's
colContainsNa <- apply(dataValues, 1, function(x) length(which(is.na(x))))
dataValues <- dataValues[-which(colContainsNa != 0),]

#normalize to 0-1
dataValues <- apply(data.matrix(dataValues),2, function(x) {(x - min(x)) / (max(x) - min(x))})

dataTargets <- decodeClassLabels(BreastCancer[,11])
data <- splitForTrainingAndTest(dataValues, dataTargets, ratio=0.25)


model <- mlp(data$inputsTrain, data$targetsTrain, size=c(3), decay=0.05, type="classification", maxit=200, data$inputsTest, data$targetsTest)
#model <- rbfDDA(data$inputsTrain, data$targetsTrain, size=5, decay=0.1, type="classification", maxit=1000)#, data$inputsTest, data$targetsTest)
#model <- elman(data$inputsTrain, data$targetsTrain, size=5, decay=0.01, type="classification", maxit=200, data$inputsTest, data$targetsTest)
#model <- jordan(data$inputsTrain, data$targetsTrain, size=5, decay=0.01, type="classification", maxit=200, data$inputsTest, data$targetsTest)


model
summary(model)
par(mfrow=c(2,2))

plotIterativeError(model)

predictions <- predict(model,data$inputsTest)

plotRegressionError(predictions[,2], data$targetsTest[,2])

confusionMatrix(data$targetsTrain,fitted.values(model))
confusionMatrix(data$targetsTest,predictions)

plotROC(fitted.values(model)[,2], data$targetsTrain[,2])
plotROC(predictions[,2], data$targetsTest[,2])

#table(targets=encodeClassLabels(data$targetsTrain), predictions=encodeClassLabels(fitted.values(model)))

#foo <- som(matrix(rnorm(1000), 250), 3, 5)
#plot(foo, ylim=c(-1, 1))

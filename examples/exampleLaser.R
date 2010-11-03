library(RSNNS)

data(snnsData)

inputs <- snnsData$laser_1000.pat[,inputColumns(snnsData$laser_1000.pat)]
outputs <- snnsData$laser_1000.pat[,outputColumns(snnsData$laser_1000.pat)]

par(mfrow=c(3,2))

patterns <- splitForTrainingAndTest(inputs, outputs, ratio=0.15)

#as.matrix(patterns$inputsTest)

##results with linOut=TRUE are worse than with linOut=FALSE..

#model <- jordan(patterns$inputsTrain, patterns$targetsTrain, size=c(8), learnFuncParams=c(0.1), maxit=100,
#                inputsTest=patterns$inputsTest, targetsTest=patterns$targetsTest, linOut=FALSE)

model <- elman(patterns$inputsTrain, patterns$targetsTrain, size=c(8,8), learnFuncParams=c(0.1), maxit=500,
    inputsTest=patterns$inputsTest, targetsTest=patterns$targetsTest, linOut=FALSE)

#model <- mlp(patterns$inputsTrain, patterns$targetsTrain, initFuncParams=c(-0.3,0.3),size=c(8), learnFuncParams=c(0.05), maxit=500,
#                inputsTest=patterns$inputsTest, targetsTest=patterns$targetsTest, linOut=TRUE)

summary(model)

#names(model)
#model$IterativeFitError
#model$fitted.values
#model$fittedTestValues

plotIterativeError(model)
#plot(model$IterativeFitError*testSetRatio, type="l")
#lines(model$IterativeTestError, col="red")

plotRegressionError(patterns$targetsTrain, model$fitted.values, main="Regression Plot Fit")
plotRegressionError(patterns$targetsTest, model$fittedTestValues, main="Regression Plot Test")

hist(model$fitted.values - patterns$targetsTrain, col="lightblue", main="Error Histogram Fit")

#plot(outputs, model$fitted.values, xlim=c(0,1), ylim=c(0,1))
#linMod <- lm(model$fitted.values ~ outputs)
#abline(linMod, col="red")
#lines(c(0,1), c(0,1))

model$IterativeFitError[length(model$IterativeFitError)]

#summary(model)

plot(inputs, type="l")

plot(inputs[1:100], type="l")
lines(outputs[1:100], col="red")
lines(model$fitted.values[1:100], col="green")
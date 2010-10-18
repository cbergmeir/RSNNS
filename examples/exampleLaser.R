library(RSNNS)

data(snnsData)

inputs <- snnsData$laser_1000.pat[,inputColumns(snnsData$laser_1000.pat)]
outputs <- snnsData$laser_1000.pat[,outputColumns(snnsData$laser_1000.pat)]

par(mfrow=c(2,2))

testSetRatio <- 0.15
myNet <- elman(inputs, outputs, size=c(8,8), decay=0.1, maxit=500, testSetRatio=testSetRatio)
#myNet <- mlp(inputs, outputs, size=c(5), decay=0.5, maxit=2000, type="classification")

plotIterativeError(myNet)
#plot(myNet$IterativeFitError*testSetRatio, type="l")
#lines(myNet$IterativeTestError, col="red")

plotRegressionError(outputs, myNet$fitted.values)

#plot(outputs, myNet$fitted.values, xlim=c(0,1), ylim=c(0,1))
#linMod <- lm(myNet$fitted.values ~ outputs)
#abline(linMod, col="red")
#lines(c(0,1), c(0,1))

myNet$IterativeFitError[length(myNet$IterativeFitError)]

summary(myNet)

plot(inputs, type="l")

plot(inputs[1:100], type="l")
lines(outputs[1:100], col="red")
lines(myNet$fitted.values[1:100], col="green")
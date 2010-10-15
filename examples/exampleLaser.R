library(RSNNS)

data(snnsData)

inputs <- snnsData$laser_1000.pat[,inputColumns(snnsData$laser_1000.pat)]
outputs <- snnsData$laser_1000.pat[,outputColumns(snnsData$laser_1000.pat)]

par(mfrow=c(2,2))

myNet <- elman(inputs, outputs, size=c(8,8), decay=0.1, maxit=2000)
#myNet <- mlp(inputs, outputs, size=c(5), decay=0.5, maxit=2000, type="classification")

plot(myNet$generalErrorIterations, type="l")
plot(outputs, myNet$fitted.values, xlim=c(0,1), ylim=c(0,1))

linMod <- lm(myNet$fitted.values ~ outputs)
abline(linMod)
lines(c(0,1), c(0,1), col="red")

myNet$generalErrorIterations[length(myNet$generalErrorIterations)]

summary(myNet)
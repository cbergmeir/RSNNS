library(RSNNS)

data(snnsData)

inputs <- snnsData$eight_016.pat[,inputColumns(snnsData$eight_016.pat)]
outputs <- snnsData$eight_016.pat[,outputColumns(snnsData$eight_016.pat)]

par(mfrow=c(1,2))

myElmanNet <- elman(inputs, outputs, size=8, decay=0.1, maxit=1000)
print(myElmanNet)
#summary(myElmanNet)
plot(myElmanNet$generalErrorIterations, type="l")
#myElmanNet$generalErrorIterations[1:500]
#myElmanNet$fitted.values


myJordanNet <- jordan(inputs, outputs, size=8, decay=0.1, maxit=1000)
print(myJordanNet)
#summary(myJordanNet)
plot(myJordanNet$generalErrorIterations, type="l")

#mySlp <- mlp(inputs, outputs, size=8, decay=0.1, maxit=1000)
#lines(mySlp$generalErrorIterations[1:500], type="l")

#savePatFile(inputs,outputs, "/home/bergmeir/elmanTEST.pat")

#inputs
#outputs
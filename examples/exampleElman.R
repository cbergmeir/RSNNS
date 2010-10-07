library(RSNNS)

data(snnsData)

inputs <- snnsData$eight_016.pat[,inputColumns(snnsData$eight_016.pat)]
outputs <- snnsData$eight_016.pat[,outputColumns(snnsData$eight_016.pat)]

myElmanNet <- recurrent(inputs, outputs, size=8, decay=0.1, maxit=1000)

print(myElmanNet)

plot(myElmanNet$generalErrorIterations[1:500], type="l")
myElmanNet$generalErrorIterations[1:500]
myElmanNet$fitted.values

#mySlp <- mlp(inputs, outputs, size=8, decay=0.1, maxit=1000)
#lines(mySlp$generalErrorIterations[1:500], type="l")

#savePatFile(inputs,outputs, "/home/bergmeir/elmanTEST.pat")
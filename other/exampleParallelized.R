library(tseriesChaos)
library(RSNNS)
library(multicore)
path <- "/home/bergmeir/datasets/Proyecto_Isaac/"
#resultsPath <- "/home/bergmeir/datasets/TS-v1/"

#files <- c("TS_v1.data", "TS_v2.data", "totalVIA1.data", "totalVIA2.data")
files <- c("TS_v1.data", "TS_v2.data")

allTS <- list()

for(file in files)  {
  series <- as.list(read.table(paste(path,file,sep="")))
  names(series) <- paste(names(series), "_", file ,sep="")
  allTS <- append(allTS,series)
}

#diff
allTSDiff <- lapply(allTS, diff)

#norm to 0-1
allMax <- max(unlist(allTSDiff))
#allTSDiff <- lapply(allTSDiff, function(x) {x / (2*allMax) + 0.5})
allTSDiff <- lapply(allTSDiff, function(x) {(x - min(x)) / (max(x) - min(x))})
#allMin <- min(unlist(allTSDiff))
#allTSDiff <- lapply(allTSDiff, function(x) {(x + abs(allMin)) / 10})

#embedd
embeddedTS <- lapply(allTSDiff, embedd, m=3, d=1)

trainingValues <- lapply(embeddedTS, function(x) x[,1:(ncol(x)-1)])
trainingTargets <- lapply(embeddedTS, function(x) x[,ncol(x)])




models <- mclapply(embeddedTS, function(x) {
      
      trainingValues <- x[,1:(ncol(x)-1)]
      trainingTargets <- x[,ncol(x)]
      
      #myNet <- elman(trainingValues, trainingTargets, size=c(8,8), decay=0.1, maxit=2000)      
      myNet <- mlp(trainingValues, trainingTargets, size=c(8), decay=0.1, maxit=2000, type="regression")
      myNet
    })

par(mfrow=c(2,2))
plot(models[[1]]$generalErrorIterations, type="l")
#plot(models[[9]]$generalErrorIterations, type="l")

#par(mfrow=c(6,6))
#for (model in models) {
#  plot(model$generalErrorIterations, type="l")  
#}

unlist(lapply(models, function(x) x$generalErrorIterations[length(x$generalErrorIterations)]))

#regression plot

#models[[1]]
#summary(models[[1]])
plot(trainingTargets[[1]], models[[1]]$fitted.values)#, xlim=c(-0.5,0.5), ylim=c(-0.5,0.5))


#myNet <- elman(trainingValues[[5]], trainingTargets[[5]], size=c(8,8), decay=0.1, maxit=2000)
myNet <- mlp(trainingValues[[5]], trainingTargets[[5]], size=c(8), decay=0.5, maxit=100000, type="classification")
plot(myNet$generalErrorIterations, type="l")
plot(trainingTargets[[5]], myNet$fitted.values, xlim=c(0,1), ylim=c(0,1))

linMod <- lm(myNet$fitted.values ~ trainingTargets[[5]])
abline(linMod)
lines(c(0,1), c(0,1), col="red")

myNet$generalErrorIterations[length(myNet$generalErrorIterations)]

summary(myNet)

#myNet$snnsObject$saveNet("/home/bergmeir/ts_mlp_test.net", "ts_mlp_test")
#myNet$snnsObject$saveNet("/home/bergmeir/ts_elman_test.net", "ts_elman_test")
#savePatFile(trainingValues[[5]], trainingTargets[[5]], "/home/bergmeir/ts_elman_test.pat")
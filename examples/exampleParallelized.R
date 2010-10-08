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

allTSDiff <- lapply(allTS, diff)


embeddedTS <- lapply(allTSDiff, embedd, m=3, d=1)

trainingValues <- lapply(embeddedTS, function(x) x[,1:(ncol(x)-1)])
trainingTargets <- lapply(embeddedTS, function(x) x[,ncol(x)])




models <- mclapply(embeddedTS, function(x) {
      
      trainingValues <- x[,1:(ncol(x)-1)]
      trainingTargets <- x[,ncol(x)]
      
      #myNet <- recurrent(trainingValues, trainingTargets, size=c(8,8), decay=0.1, maxit=2000)      
      myNet <- mlp(trainingValues, trainingTargets, size=c(5), decay=0.1, maxit=2000)
      myNet
    })

par(mfrow=c(6,6))
plot(models[[1]]$generalErrorIterations, type="l")

#plot(models[[9]]$generalErrorIterations, type="l")

for (model in models) {
  plot(model$generalErrorIterations, type="l")  
}

unlist(lapply(models, function(x) x$generalErrorIterations[length(x$generalErrorIterations)]))
library(RSNNS)
library(som)

#data(snnsData)
#inputs <- snnsData$som_cube.pat

data(iris)
##normalization
##inputs <- apply(iris[,1:4],2, function(x) {(x-mean(x))/sd(x)})
inputs <- apply(iris[,1:4],2, function(x) {(x - min(x)) / (max(x) - min(x))})


#data(yeast)
#dataValues <- yeast
#inputs <- apply(data.matrix(dataValues), 2, function(x) {(x - min(x)) / (max(x) - min(x))})

#colContainsNa <- apply(dataValues, 1, function(x) length(which(is.na(x))))
#dataValues <- dataValues[-which(colContainsNa != 0),]
#inputs <- normalize(data.matrix(dataValues))
#foo <- som(inputs, 3, 5)
#plot(foo, ylim=c(-1, 1), sdbar=0)





#mapX <- 16
#mapY <- 16
#mapX <- 8
#mapY <- 8

mapX <- 32
mapY <- 32
#mapX <- 64
#mapY <- 64

#mapX <- 100
#mapY <- 100

snnsObject <- SnnsRObjectFactory()

snnsObject$setLearnFunc('Kohonen')
snnsObject$setUpdateFunc('Kohonen_Order')
#snnsObject$setInitialisationFunc('KOHONEN_Weights')
snnsObject$setInitialisationFunc('Kohonen_Weights_v3.2')

snnsObject$setUnitDefaults(0,0,3,0,1,'Act_Logistic','Out_Identity')

snnsObject$kohonen_createNet(mapX,mapY,ncol(inputs),mapX*mapY)


snnsObject$setTTypeUnitsActFunc("UNIT_HIDDEN", "Act_Euclid")

patset <- snnsObject$createPatterns(inputs)#, outputs)
snnsObject$setCurrPatSet(patset$set_no)

snnsObject$initializeNet(c(1.0,  -1.0))
snnsObject$shufflePatterns(TRUE)
snnsObject$DefTrainSubPat()

#parameters <- c(0.855, 9.0, 0.9, 0.9, 16.0)
#parameters <- c(0.855, mapX/2, 0.9, 0.9, mapX)
parameters <- c(0.5, mapX/2, 0.8, 0.8, mapX)
maxit <- 1000

for(i in 1:maxit) {
  res <- snnsObject$learnAllPatterns(parameters)
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
}

results <- snnsObject$somPredictCurrPatSet()
maps <- apply(results, 1, function(x) { return(list(matrix(x, nrow=mapX)))})
maps <- lapply(maps, function(x) {x[[1]]})

par(mfrow=c(2,3))
#image(maps[[1]][[1]])
#persp(1:mapX, 1:mapY, maps[[1]][[1]], theta = 30, phi = 30, expand = 0.5, col = "lightblue")
#
##par(mfrow=c(3,3))
##lapply(maps, function(x){
##      persp(1:16, 1:16, x[[1]], theta = 30, phi = 30, expand = 0.5, col = "lightblue")
##    })

winners <- snnsObject$somPredictCurrPatSetWinners(mapX, mapY)
image(winners, col=rev(heat.colors(12)))
persp(1:mapX, 1:mapY, log(winners+1), theta = 30, phi = 30, expand = 0.5, col = "lightblue")

image(log(winners+1), col=rev(heat.colors(12)))
image(log(winners+1), col=topo.colors(12))



spanningTree <- snnsObject$somPredictCurrPatSetWinnersSpanTree()
#image(matrix(as.numeric(spanningTree > 0), nrow=mapX))

som <- matrix(spanningTree, nrow=mapX)
labels <- as.numeric(iris[,5])
newsom <- som
for(i in 1:nrow(som))
  for(j in 1:ncol(som))
  {
    if(som[i,j] != 0)  {
      newsom[i,j] <- labels[som[i,j]]      
      #print(labels[som[i,j]])
    } 
  }

image(newsom)

#foo <- som(normalize(iris[,1:4]), 3, 5)
#foo <- som(normalize(iris[,1:4]), 16, 16)
#plot(foo, ylim=c(-1, 1), sdbar=0)
#summary(foo)

winners
som

basePath <- ("/home/bergmeir")
#snnsObject$saveNet(paste(basePath,"/kohonen_test.net",sep=""),"kohonen_test")
#snnsObject$saveNewPatterns(paste(basePath,"/elman_test.pat",sep=""), patset$set_no);

length(which(winners != 0))
nrow(iris)




#library(hexbin)
#
#x <- rnorm(10000)
#y <- rnorm(10000)
#
#bin <- hexbin(x,y)
#erodebin <- erode(smooth.hexbin(bin))
#
#hboxplot(erodebin, density = c(32,7), border = c(2,4))

library(RSNNS)

basePath <- ("./")

#data(snnsData)
#inputs <- snnsData$som_cube.pat

data(iris)
inputs <- normalizeData(iris[,1:4], "norm")

mapX <- 10
mapY <- 10
#mapX <- 16
#mapY <- 16
#mapX <- 32
#mapY <- 32
#mapX <- 64
#mapY <- 64
#mapX <- 100
#mapY <- 100

snnsObject <- SnnsRObjectFactory()

snnsObject$setLearnFunc('Kohonen')
snnsObject$setUpdateFunc('Kohonen_Order')
snnsObject$setInitialisationFunc('Kohonen_Weights_v3.2')

snnsObject$setUnitDefaults(0,0,3,0,1,'Act_Logistic','Out_Identity')

snnsObject$kohonen_createNet(mapX,mapY,ncol(inputs),mapX*mapY)


snnsObject$setTTypeUnitsActFunc("UNIT_HIDDEN", "Act_Euclid")

patset <- snnsObject$createPatSet(inputs)
snnsObject$setCurrPatSet(patset$set_no)

snnsObject$initializeNet(c(1.0,  -1.0))
snnsObject$shufflePatterns(TRUE)
snnsObject$DefTrainSubPat()

snnsObject$saveNet(paste(basePath,"som_cubeSnnsR_untrained.net",sep=""),"som_cubeSnnsR_untrained")

#parameters <- c(0.855, 9.0, 0.9, 0.9, 16.0)
#parameters <- c(0.855, mapX/2, 0.9, 0.9, mapX)
parameters <- c(0.5, mapX/2, 0.8, 0.8, mapX)
maxit <- 1000

for(i in 1:maxit) {
  res <- snnsObject$learnAllPatterns(parameters)
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
}

actMaps <- snnsObject$predictCurrPatSet("som", c(0.0, 0.0, 1.0))
actMaps <- matrixToActMapList(actMaps, mapX)

compMaps <- snnsObject$somPredictComponentMaps()
compMaps <- matrixToActMapList(compMaps, mapX)
compMaps

par(mfrow=c(3,3))
for (i in 1:3) plotActMap(compMaps[[i]], col=topo.colors(12))

labels <- as.numeric(iris[,5])

patsetWinners <- snnsObject$somPredictCurrPatSetWinners(c(0.0, 0.0, 1.0), targets=labels)
winners <- vectorToActMap(patsetWinners$map, mapX)
plotActMap(winners, col=rev(heat.colors(12)))
persp(1:mapX, 1:mapY, log(winners+1), theta = 30, phi = 30, expand = 0.5, col = "lightblue")

plotActMap(log(winners+1), col=rev(heat.colors(12)))
plotActMap(log(winners+1), col=topo.colors(12))

spTree <- snnsObject$somPredictCurrPatSetWinnersSpanTree()
spTree <- vectorToActMap(spTree, mapX)


labeledSpTree <- spTree
for(i in 1:nrow(spTree))
  for(j in 1:ncol(spTree))
  {
    if(spTree[i,j] != 0)  {
      labeledSpTree[i,j] <- labels[spTree[i,j]]      
    } 
  }

plotActMap(labeledSpTree)

winners
spTree

print("is the overlap in the map high?:")
length(which(winners != 0))
nrow(iris)

#get the labeled map from the labeled units
labeledMap <- encodeClassLabels(patsetWinners$labeledUnits, method="WTA", l=0,h=0)
labeledMap <- vectorToActMap(labeledMap, mapX)
plotActMap(labeledMap)

snnsObject$saveNet(paste(basePath,"som_cubeSnnsR.net",sep=""),"som_cubeSnnsR")
snnsObject$saveNewPatterns(paste(basePath,"som_cubeSnnsR.pat",sep=""), patset$set_no);



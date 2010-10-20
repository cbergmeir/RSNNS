library(RSNNS)

#data(snnsData)
#inputs <- snnsData$som_cube.pat

data(iris)
##normalization
##inputs <- apply(iris[,1:4],2, function(x) {(x-mean(x))/sd(x)})
inputs <- apply(iris[,1:4],2, function(x) {(x - min(x)) / (max(x) - min(x))})

#library(som)
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
#model <- som(inputs, mapX=mapX, mapY=mapY, maxit=100, parameters=c(0.5, mapX/2, 0.8, 0.8, mapX), targets=NULL)

model <- som(inputs, maxit=500,  calculateActMaps=TRUE, calculateSpanningTree=TRUE, targets=iris[,5])

par(mfrow=c(2,2))

image(model$map, col=rev(heat.colors(12)))
image(log(model$map+1), col=rev(heat.colors(12)))
persp(1:model$mapX, 1:model$mapY, log(model$map+1), theta = 30, phi = 30, expand = 0.5, col = "lightblue")

image(model$labelledSpanningTree)

model$map
model$spanningTree
model$labelledSpanningTree








#
#
#
#
#
#
##image(maps[[1]][[1]])
##persp(1:mapX, 1:mapY, maps[[1]][[1]], theta = 30, phi = 30, expand = 0.5, col = "lightblue")
##
###par(mfrow=c(3,3))
###lapply(maps, function(x){
###      persp(1:16, 1:16, x[[1]], theta = 30, phi = 30, expand = 0.5, col = "lightblue")
###    })
#
#winners <- snnsObject$somPredictCurrPatSetWinners(mapX, mapY)
#image(winners, col=rev(heat.colors(12)))
#persp(1:mapX, 1:mapY, log(winners+1), theta = 30, phi = 30, expand = 0.5, col = "lightblue")
#
#image(log(winners+1), col=rev(heat.colors(12)))
#image(log(winners+1), col=topo.colors(12))
#
#
#
#spanningTree <- snnsObject$somPredictCurrPatSetWinnersSpanTree()
##image(matrix(as.numeric(spanningTree > 0), nrow=mapX))
#
#som <- matrix(spanningTree, nrow=mapX)
#labels <- as.numeric(iris[,5])
#newsom <- som
#for(i in 1:nrow(som))
#  for(j in 1:ncol(som))
#  {
#    if(som[i,j] != 0)  {
#      newsom[i,j] <- labels[som[i,j]]      
#      #print(labels[som[i,j]])
#    } 
#  }
#
#image(newsom)
#
##foo <- som(normalize(iris[,1:4]), 3, 5)
##foo <- som(normalize(iris[,1:4]), 16, 16)
##plot(foo, ylim=c(-1, 1), sdbar=0)
##summary(foo)
#
#winners
#som
#
#basePath <- ("/home/bergmeir")
##snnsObject$saveNet(paste(basePath,"/kohonen_test.net",sep=""),"kohonen_test")
##snnsObject$saveNewPatterns(paste(basePath,"/elman_test.pat",sep=""), patset$set_no);
#
#length(which(winners != 0))
#nrow(iris)
#
#
#
#
##library(hexbin)
##
##x <- rnorm(10000)
##y <- rnorm(10000)
##
##bin <- hexbin(x,y)
##erodebin <- erode(smooth.hexbin(bin))
##
##hboxplot(erodebin, density = c(32,7), border = c(2,4))

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

model <- som(inputs, mapX=16, mapY=16, maxit=500,  calculateActMaps=TRUE, calculateSpanningTree=TRUE, targets=iris[,5])

par(mfrow=c(3,3))

for(i in 1:ncol(inputs)) image(model$componentMaps[[i]], col=topo.colors(12))

image(model$map, col=rev(heat.colors(12)))
image(log(model$map+1), col=rev(heat.colors(12)))
persp(1:model$mapX, 1:model$mapY, log(model$map+1), theta = 30, phi = 30, expand = 0.5, col = "lightblue")

image(model$labelledSpanningTree)

model$componentMaps

model$map
model$spanningTree
model$labelledSpanningTree


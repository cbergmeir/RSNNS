basePath <- ("/home/bergmeir")
##basePath <- ("D:/Doctorado")
#
#dyn.load(paste(basePath,"/ts-code/Rpackages/RSNNS.Rcheck/RSNNS/libs/RSNNS", .Platform$dynlib.ext, sep=""))
#source(paste(basePath,"/ts-code/Rpackages/RSNNS/R/RSNNS.R",sep=""))
#cacheMetaData(1)

library(RSNNS)

#setClass( "SnnsCLib", representation( pointer = "externalptr" ) )
#
#SnnsCLib_method <- function(name){
#  paste( "SnnsCLib", name, sep = "__" )
#}
#
#setMethod( "$", "SnnsCLib", function(x, name ){
#      function(...) .Call( SnnsCLib_method(name) , x@pointer , ... )
#    } )
#
#
#mySnnsObject <- new( "SnnsCLib")
#mySnnsObject@pointer <- .Call("SnnsCLib__new", package="RSNNS")

mySnnsObject <- snnsObjectFactory()

mySnnsObject$setLearnFunc('Quickprop')
mySnnsObject$setUpdateFunc('Topological_Order')
mySnnsObject$setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')

print("Creating input layer")

inputs <- vector()
for(i in 1:9) {
  #pos <- PosType()
  #pos$x <- i
  #pos$y <- 0
  #pos$z <- 0
  
  num <- mySnnsObject$createDefaultUnit()
  inputs[i] <- num
  
  mySnnsObject$setUnitName(num,paste("Input_",i,sep=""))
  #mySnnsObject$setUnitPosition(num, pos)
  mySnnsObject$setUnitPosition(num, i, 0, 0)
}

print("Creating hidden layer")

hidden <- vector()
for(i in 1:4) {
  #pos <- PosType()
  #pos$x <- i+3
  #pos$y <- 2
  #pos$z <- 0
  
  num <- mySnnsObject$createDefaultUnit()
  hidden[i] <- num
  
  mySnnsObject$setUnitName(num,paste("Hidden_",i,sep=""))
  
#  mySnnsObject$setUnitTType(num,HIDDEN)
  mySnnsObject$setUnitTType(num,3)
  
  #mySnnsObject$setUnitPosition(num, pos)
  mySnnsObject$setUnitPosition(num, i+3, 2, 0)
  mySnnsObject$setCurrentUnit(num)
  for(j in inputs)  {
    mySnnsObject$createLink(j,0);
  };  
}

print("Creating output layer")

outputs <- vector()
for(i in 1:9) {
  #pos <- PosType()
  #pos$x <- i
  #pos$y <- 4
  #pos$z <- 0
  
  num <- mySnnsObject$createDefaultUnit()
  outputs[i] <- num
  
  mySnnsObject$setUnitName(num,paste("Output_",i,sep=""))
  
#  mySnnsObject$setUnitTType(num,OUTPUT)
  mySnnsObject$setUnitTType(num,2)
  
  #mySnnsObject$setUnitPosition(num, pos)
  mySnnsObject$setUnitPosition(num, i, 4, 0)
  mySnnsObject$setCurrentUnit(num)
  for(j in hidden)  {
    mySnnsObject$createLink(j,0);
  };  
}

print("Creating patterns")

mySnnsObject$deleteAllPatterns()

#in the wrapping code, "free" has to be removed..
patset <- mySnnsObject$allocNewPatternSet()

for(unum in 1:9)  {
  for(curnum in 1:9)  {
    mySnnsObject$setUnitActivation(inputs[curnum], as.numeric(curnum == unum));
    mySnnsObject$setUnitActivation(outputs[curnum], as.numeric(curnum == unum));
  }
  mySnnsObject$newPattern();
}

mySnnsObject$initializeNet(-1)
mySnnsObject$shufflePatterns(TRUE)

#int insize, outsize, instep, outstep, max_n_pos;
#res = mySnnsObject$DefTrainSubPat(&insize, &outsize, &instep, &outstep, &max_n_pos);

mySnnsObject$DefTrainSubPat()
#mySnnsObject$DefTrainSubPat(1,2,3,4,5)

#mySnnsObject$saveNet(paste(basePath,"/pyencoder_Rtest2000.net",sep=""),"pyencoder")

mySnnsObject$saveNet(paste(basePath,"/snnsCLibencoder.net",sep=""),"snnsCLibencoder")



##Training
#
#myBackpropLearnAllPatterns(0.2)
#
error <- vector()
for(i in 1:5000) {
  res <- mySnnsObject$learnAllPatterns(c(0.2, 0, 0, 0))
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
  error[i] <- res[[2]]
}

##i <- 0
##while(i < 1000) {
#  #error[i] <- myLearnAllPatterns()
#  res <- mySnnsObject$learnAllPatterns(0.2, 0);
#  #if(res != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
##  i <- i + 1;
##}
#


error[1:500]
#plot(error, type="l")
lines(error)


#
##mySnnsObject$saveNewPatterns(paste(basePath,"/pyencoder_Rtest.pat",sep=""),patset)
##mySnnsObject$saveNet(paste(basePath,"/pyencoder_Rtest.net",sep=""),"pyencoder")

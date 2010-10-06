basePath <- ("/home/bergmeir")
##basePath <- ("D:/Doctorado")
#
#dyn.load(paste(basePath,"/ts-code/Rpackages/RSNNS.Rcheck/RSNNS/libs/RSNNS", .Platform$dynlib.ext, sep=""))
#source(paste(basePath,"/ts-code/Rpackages/RSNNS/R/RSNNS.R",sep=""))
#cacheMetaData(1)

library(RSNNS)

SnnsKrui_setLearnFunc('Quickprop')
SnnsKrui_setUpdateFunc('Topological_Order')
SnnsKrui_setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')

print("Creating input layer")

inputs <- vector()
for(i in 1:9) {
  #pos <- PosType()
  #pos$x <- i
  #pos$y <- 0
  #pos$z <- 0
  
  num <- SnnsKrui_createDefaultUnit()
  inputs[i] <- num
  
  SnnsKrui_setUnitName(num,paste("Input_",i,sep=""))
  #SnnsKrui_setUnitPosition(num, pos)
  SnnsKrui_setUnitPosition(num, i, 0, 0)
}

print("Creating hidden layer")

hidden <- vector()
for(i in 1:4) {
  #pos <- PosType()
  #pos$x <- i+3
  #pos$y <- 2
  #pos$z <- 0
  
  num <- SnnsKrui_createDefaultUnit()
  hidden[i] <- num
  
  SnnsKrui_setUnitName(num,paste("Hidden_",i,sep=""))
  
#  SnnsKrui_setUnitTType(num,HIDDEN)
  SnnsKrui_setUnitTType(num,3)
  
  #SnnsKrui_setUnitPosition(num, pos)
  SnnsKrui_setUnitPosition(num, i+3, 2, 0)
  SnnsKrui_setCurrentUnit(num)
  for(j in inputs)  {
    SnnsKrui_createLink(j,0);
  };  
}

print("Creating output layer")

outputs <- vector()
for(i in 1:9) {
  #pos <- PosType()
  #pos$x <- i
  #pos$y <- 4
  #pos$z <- 0
  
  num <- SnnsKrui_createDefaultUnit()
  outputs[i] <- num
  
  SnnsKrui_setUnitName(num,paste("Output_",i,sep=""))
  
#  SnnsKrui_setUnitTType(num,OUTPUT)
  SnnsKrui_setUnitTType(num,2)
  
  #SnnsKrui_setUnitPosition(num, pos)
  SnnsKrui_setUnitPosition(num, i, 4, 0)
  SnnsKrui_setCurrentUnit(num)
  for(j in hidden)  {
    SnnsKrui_createLink(j,0);
  };  
}

#SnnsKrui_saveNet(paste(basePath,"/pyencoder_Rtest3000.net",sep=""),"pyencoder")

print("Creating patterns")

SnnsKrui_deleteAllPatterns()

#in the wrapping code, "free" has to be removed..
patset <- SnnsKrui_allocNewPatternSet()

for(unum in 1:9)  {
  for(curnum in 1:9)  {
    SnnsKrui_setUnitActivation(inputs[curnum], as.numeric(curnum == unum));
    SnnsKrui_setUnitActivation(outputs[curnum], as.numeric(curnum == unum));
  }
  SnnsKrui_newPattern();
}

SnnsKrui_initializeNet(-1)
SnnsKrui_shufflePatterns(TRUE)

#int insize, outsize, instep, outstep, max_n_pos;
#res = SnnsKrui_DefTrainSubPat(&insize, &outsize, &instep, &outstep, &max_n_pos);

SnnsKrui_DefTrainSubPat()
#SnnsKrui_DefTrainSubPat(1,2,3,4,5)

#SnnsKrui_saveNet(paste(basePath,"/pyencoder_Rtest2000.net",sep=""),"pyencoder")





##Training
#
#myBackpropLearnAllPatterns(0.2)
#
error <- vector()
for(i in 1:5000) {
  res <- SnnsKrui_learnAllPatterns(c(0.2, 0, 0, 0))
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
  error[i] <- res[[2]]
}

##i <- 0
##while(i < 1000) {
#  #error[i] <- myLearnAllPatterns()
#  res <- SnnsKrui_learnAllPatterns(0.2, 0);
#  #if(res != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
##  i <- i + 1;
##}
#


error[1:500]
plot(error, type="l")



#
##SnnsKrui_saveNewPatterns(paste(basePath,"/pyencoder_Rtest.pat",sep=""),patset)
##SnnsKrui_saveNet(paste(basePath,"/pyencoder_Rtest.net",sep=""),"pyencoder")

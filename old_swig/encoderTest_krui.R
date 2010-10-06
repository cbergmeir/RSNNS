#basePath <- ("/home/bergmeir")
##basePath <- ("D:/Doctorado")
#
#dyn.load(paste(basePath,"/ts-code/Rpackages/RSNNS.Rcheck/RSNNS/libs/RSNNS", .Platform$dynlib.ext, sep=""))
#source(paste(basePath,"/ts-code/Rpackages/RSNNS/R/RSNNS.R",sep=""))
#cacheMetaData(1)

library(RSNNS)

krui_setLearnFunc('Quickprop')
krui_setUpdateFunc('Topological_Order')
krui_setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')

print("Creating input layer")

inputs <- vector()
for(i in 1:9) {
  pos <- PosType()
  pos$x <- i
  pos$y <- 0
  pos$z <- 0
  
  num <- krui_createDefaultUnit()
  inputs[i] <- num
  
  krui_setUnitName(num,paste("Input_",i,sep=""))
  krui_setUnitPosition(num, pos)
}

print("Creating hidden layer")

hidden <- vector()
for(i in 1:4) {
  pos <- PosType()
  pos$x <- i+3
  pos$y <- 2
  pos$z <- 0
  
  num <- krui_createDefaultUnit()
  hidden[i] <- num
  
  krui_setUnitName(num,paste("Hidden_",i,sep=""))
  
#  krui_setUnitTType(num,HIDDEN)
  krui_setUnitTType(num,3)
  
  krui_setUnitPosition(num, pos)
  krui_setCurrentUnit(num)
  for(j in inputs)  {
    krui_createLink(j,0);
  };  
}

print("Creating output layer")

outputs <- vector()
for(i in 1:9) {
  pos <- PosType()
  pos$x <- i
  pos$y <- 4
  pos$z <- 0
  
  num <- krui_createDefaultUnit()
  outputs[i] <- num
  
  krui_setUnitName(num,paste("Output_",i,sep=""))
  
#  krui_setUnitTType(num,OUTPUT)
  krui_setUnitTType(num,2)
  
  krui_setUnitPosition(num, pos)
  krui_setCurrentUnit(num)
  for(j in hidden)  {
    krui_createLink(j,0);
  };  
}

print("Creating patterns")

krui_deleteAllPatterns()

#in the wrapping code, "free" has to be removed..
res <- krui_allocNewPatternSet()
patset <- res[[2]]

for(unum in 1:9)  {
  for(curnum in 1:9)  {
    krui_setUnitActivation(inputs[curnum], as.numeric(curnum == unum));
    krui_setUnitActivation(outputs[curnum], as.numeric(curnum == unum));
  }
  krui_newPattern();
}

krui_initializeNet(-1,1)
krui_shufflePatterns(1)

#int insize, outsize, instep, outstep, max_n_pos;
#res = krui_DefTrainSubPat(&insize, &outsize, &instep, &outstep, &max_n_pos);

krui_DefTrainSubPat()
#krui_DefTrainSubPat(1,2,3,4,5)


#Training

myBackpropLearnAllPatterns(0.2)

error <- vector()
for(i in 1:5000) {
  res <- myBackpropLearnAllPatterns(0.2)
  if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
  error[i] <- res[[2]]
}

#i <- 0
#while(i < 1000) {
  #error[i] <- myLearnAllPatterns()
  #res <- krui_learnAllPatterns(0.2, 0);
  #if(res != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
#  i <- i + 1;
#}

error[1:500]
plot(error, type="l")

#krui_saveNewPatterns(paste(basePath,"/pyencoder_Rtest.pat",sep=""),patset)
#krui_saveNet(paste(basePath,"/pyencoder_Rtest.net",sep=""),"pyencoder")

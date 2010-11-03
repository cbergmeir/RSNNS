library(RSNNS)

getSNNSFunctionTable()

#library(ROCR)
#pred <- ROCR::prediction(predictions[,2], as.numeric(labels==2))
#
#perf <- performance(pred,"tpr","fpr")
#plot(perf)
### precision/recall curve (x-axis: recall, y-axis: precision)
#perf1 <- performance(pred, "prec", "rec")
#plot(perf1)
### sensitivity/specificity curve (x-axis: specificity,
### y-axis: sensitivity)
#perf1 <- performance(pred, "sens", "spec")
#plot(perf1)


#library(datasets)
#lapply(ls("package:datasets"), function(x) { 
#    obj <- get(x)
#    if(is.matrix(obj)) return(c(x, nrow(obj), ncol(obj)))
#    else return(c(x, length(obj), 1))        
#    })

res <- 5

is.list(res)
is.null(res$err)



snnsObject <- SnnsRObjectFactory()
snnsObject$setCurrentUnit(500)

snnsObject$error(-2)
    
setClass( "SnnsCLib", representation( pointer = "externalptr" ) )

SnnsCLib_method <- function(name){
  paste( "SnnsCLib", name, sep = "__" )
}

SnnsR_method <- function(name){
  paste( "SnnsR", name, sep = "__" )
}

setMethod( "$", "SnnsCLib", function(x, name ){
      function(...) {
        #print(x)
        
        myFunc <- mget(SnnsR_method(name), mode="any", 
            envir = as.environment(-1), 
            ifnotfound = list(FALSE), inherits=TRUE)

        if(is.function(myFunc[[1]])) return(myFunc[[1]](...))
        else return(.Call( SnnsCLib_method(name) , x@pointer , ... )) 
      }     
    } )

snnsObjectFactory <- function(){
  mySnnsObject <- new( "SnnsCLib")
  mySnnsObject@pointer <- .Call("SnnsCLib__new", package="RSNNS")
  return(mySnnsObject)
}

snns <- snnsObjectFactory()

snns

snns$getVersion()
snns$createFullyConnectedFeedForwardNet()


SnnsR__createFullyConnectedFeedForwardNet <- function(x, unitsPerLayer) {
  print(x)
  return("test")
  
}

exists("SnnsR__createFullyConnectedFeedForwardNet")
ls(RSNNS)

name <- "getVersion"
name <- "createFullyConnectedFeedForwardNet"















library(mvbutils)

doc2Rd("savePatFile")
fixr( savePatFile, new.doc=T)

library(RSNNS)

data(snnsData)

snnsData$sprach_test.pat

#resetRSNNS()

examplesPath <- ("/home/bergmeir/Download/SNNSv4.3/examples")
basePath <- ("/home/bergmeir")

savePatFile(irisTrainValues, irisTrainTargets, paste(basePath,"/iris.pat",sep=""))
#SnnsKrui_loadNewPatterns(paste(examplesPath,"/encoder.pat",sep=""))
#INPUT: 1
#OUTPUT: 2
SnnsKr_getSubPatData(0, 0, 2)

eight_160 <- readPatFile(paste(examplesPath,"/eight_160.pat",sep=""))
save(eight_160, file = paste(basePath,"/eight_160.Rdata",sep=""))

nInputs <- 2
nOutputs <- 2

SnnsKrui_setUpdateFunc('Topological_Order')
SnnsKrui_setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')

createFullyConnectedFeedForwardNet(c(nInputs,1,nOutputs))

inputs <- getAllInputUnits()
outputs <- getAllOutputUnits()

#RcppExport SEXP SnnsKrui_setPatternNo( SEXP pattern_no);
#RcppExport SEXP SnnsKrui_getPatternNo(void);

nPatterns <- SnnsKrui_getNoOfPatterns()

#SnnsKrui_setPatternNo(5)
#SnnsKrui_GetPatInfo()

inPatterns <- matrix(nrow= nPatterns, ncol=nInputs)
outPatterns <- matrix(nrow= nPatterns, ncol=nOutputs)


for(currentPattern in 1:nPatterns)  {
  SnnsKrui_setPatternNo(currentPattern)
  #print(SnnsKrui_GetPatInfo())  
  
  SnnsKrui_updateNet(0)
  
  SnnsKrui_showPattern(resolveDefine(pattern_update_modes,"OUTPUT_OUT"))
  
  SnnsKrui_testSinglePattern( currentPattern, 0 );
  
  for(j in 1:nInputs) {
    inPatterns[currentPattern,j] <- SnnsKrui_getUnitOutput(inputs[j])
  }
  
  for(j in 1:nOutputs) {
    outPatterns[currentPattern,j] <- SnnsKrui_getUnitValueA(outputs[j])
  }
}

patterns <- cbind(inputs=inPatterns, outputs=outPatterns)

patterns

inputs
outputs

nPatterns

#IUnits, HUnits, OUnits, ICols, HCols, OCols
#SnnsBn_jordan_createNet(2,8,2,1,2,1)
#SnnsKrui_saveNet(paste(basePath,"/jordan_test.net",sep=""),"elman_test")

#layer, columns, out_context
#SnnsBn_elman_createNet(c(6,10,10,10,6),c(1,1,1,1,1),FALSE)
#SnnsKrui_saveNet(paste(basePath,"/elman_test.net",sep=""),"elman_test")



#SnnsKrui_saveNet(paste(basePath,"/ffnet_test.net",sep=""),"ffnet_test")

#getAllInputUnits()

#
#createUnitLayer <- function(no_of_units, io_type, act_func_name, out_func_name, unit_name)  {
#
#  #if (no_of_units < 1) return (resolveDefine(error_codes,"KRERR_NO_UNITS")) ;
#
#  units <- vector()
#  
#  for (i in 1:no_of_units)  {
#  
#    name <- paste(unit_name,i,sep="")
#  
#    unit_no = SnnsKrui_createUnit (name, out_func_name, act_func_name, 0, 0) 
#    if (unit_no < 0) return (unit_no)
#  
#    units[i] <- unit_no
#  
#    error_code = SnnsKrui_setUnitTType(unit_no, io_type)
#
#    #if(error_code < 0) return (error_code)
#  }
#
##return (first_unit)
#return (units)  
#}


rm(list = ls(all = TRUE))
dyn.unload(paste("/home/bergmeir/ts-code/Rpackages/RSNNS.Rcheck/RSNNS/libs/RSNNS", .Platform$dynlib.ext, sep=""))
dyn.load(paste("/home/bergmeir/ts-code/Rpackages/RSNNS.Rcheck/RSNNS/libs/RSNNS", .Platform$dynlib.ext, sep=""))
source("/home/bergmeir/ts-code/Rpackages/RSNNS/R/RSNNS.R")
cacheMetaData(1)

dyn.unload(paste("D:/Doctorado/ts-code/Rpackages/RSNNS.Rcheck/RSNNS/libs/RSNNS", .Platform$dynlib.ext, sep=""))
dyn.load(paste("D:/Doctorado/ts-code/Rpackages/RSNNS.Rcheck/RSNNS/libs/RSNNS", .Platform$dynlib.ext, sep=""))
source("D:/Doctorado/ts-code/Rpackages/RSNNS/R/RSNNS.R")

#p <- 0
#t <- testTrainNeuralNetwork(10,20,30)
#t
#p

t <- testTrainNeuralNetworkWrapper()
t

x <- Circle(10)

x <- PosType()

x$x <- 1
x$y <- 0
x$z <- 0

x

Circle

getFuncInfoWrapper2(10)

x <- encoderExample()

getFuncInfoWrapper2(10)
x <- lapply(1:10, getFuncInfoWrapper2)
as.data.frame(x)

newunit <- krui_createDefaultUnit()
print("New unit:")
newunit

print("Act func name:")
krui_getUnitActFuncName(newunit)

krui_getVersion()
krui_getNoOfFunctions()

krui_isFunction("Act_Logistic", 2);

#p1 <- ""
#p2 <- 0
#params <- krui_getFuncInfo(100)

krui_getLearnFunc()
krui_getUpdateFunc()

krui_getUnitOutFuncName(200)

getClassDef("SWIGArray")



p

p <- SWIGArray(10) #PosType(0,0,0)

#-----------------------
rm(list = ls(all = TRUE))
dyn.unload(paste("/home/bergmeir/ts-code/Rpackages/RSNNS.Rcheck/RSNNS/libs/RSNNS", .Platform$dynlib.ext, sep=""))


#krui_getLearnFunc()
#krui_getUpdateFunc()

krui_getLearnFunc()
krui_getUpdateFunc()
krui_getUnitDefaults()



#x <- 0
#y <- 0
#z <- 0
#w <- 0

#krui_getNetInfo(x,y,z,w)

dyn.unload(paste("/home/bergmeir/ts-code/Rpackages/RSNNS.Rcheck/RSNNS/libs/RSNNS", .Platform$dynlib.ext, sep=""))

#krui_setLearnFunc("Std_Backpropagation")
#krui_setUpdateFunc("Topological_Order")

#num <- 100
#krui_getFuncInfo()
#krui_getNoOfFunctions()
#krui_loadNet("encoder.net", "encoder")



#dyn.load(paste("/home/bergmeir/ts-code/wrapSNNS/example", .Platform$dynlib.ext, sep=""))
#source("/home/bergmeir/ts-code/wrapSNNS/example.R")
#cacheMetaData(1)
#
#My_variable()
#
#fact(5)

#swig -r example.i
#PKG_LIBS="example.c" R CMD SHLIB example.c example_wrap.c -fPIC

#PKG_LIBS="snnswrap.c" R CMD SHLIB libfunc.so libkernel.so snnswrap.c -fPIC
#PKG_LIBS="snnskernel_wrap.c" R CMD SHLIB snnskernel_wrap.c -L. -llibfunc.a -llibkernel.a -fPIC
#PKG_LIBS="/home/bergmeir/ts-code/packageTest/RwrapSNNS/src/libfunc.a /home/bergmeir/ts-code/packageTest/RwrapSNNS/src/libkernel.a snnskernel_wrap.cpp" R CMD SHLIB snnskernel_wrap.cpp -fPIC 
#-L/home/bergmeir/ts-code/packageTest/RwrapSNNS/src -lfunc -lkernel 
#PKG_CFLAGS="-L/home/bergmeir/ts-code/packageTest/RwrapSNNS/src -lfunc -lkernel -fPIC" PKG_LIBS="snnskernel_wrap.c" R CMD SHLIB snnskernel_wrap.c
#PKG_LIBS="snnskernel2_wrap.c" R CMD SHLIB snnskernel2_wrap.c -L. -llibfunc.a -llibkernel.a -fPIC


# swig -r snnskernel2.i
# change in snnskernel2_wrap.c all true to TRUE and all false to FALSE
# PKG_LIBS="snnskernel2_wrap.c" R CMD SHLIB snnskernel2_wrap.c -I. -L. -lfunc -lkernel -fPIC

#dyn.load(paste("/home/bergmeir/Desktop/SNNSv4.3/kernel/sources/snnskernel2_wrap", .Platform$dynlib.ext, sep=""))
#dyn.unload(paste("/home/bergmeir/Desktop/SNNSv4.3/kernel/sources/snnskernel2_wrap", .Platform$dynlib.ext, sep=""))

#source("/home/bergmeir/Desktop/SNNSv4.3/kernel/sources/snnskernel2.R")
#cacheMetaData(1)
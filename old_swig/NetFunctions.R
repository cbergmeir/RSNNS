#basePath <- ("/home/bergmeir")
basePath <- ("D:/Doctorado")

dyn.load(paste(basePath,"/ts-code/Rpackages/RSNNS.Rcheck/RSNNS/libs/RSNNS", .Platform$dynlib.ext, sep=""))
source(paste(basePath,"/ts-code/Rpackages/RSNNS/R/RSNNS.R",sep=""))
cacheMetaData(1)

krui_setLearnFunc('Quickprop')
krui_setUpdateFunc('Topological_Order')
krui_setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')
#krui_setUnitDefaults(1,0,1,0,1,'Act_Identity','Out_Identity')

#krui_getFuncParamInfo('Topological_Order',5)
#noOfFunctions <- krui_getNoOfFunctions()
#funcInfo <- lapply(1:noOfFunctions, RSNNS_getFuncInfo)
#unlist(funcInfo)
#RSNNS_getFuncInfo(162)

#krui_getUnitDefaults(1,1,1,1,1,1,1)
#RSNNS_getUnitDefaults();


#---------------------------------------
#Function createSingleLayerPerceptron
#---------------------------------------
createSingleLayerPerceptron <- function(nInputs, nHidden, nOutputs) {
 
  inputs <- vector()
  for(i in 1:nInputs) {
    pos <- PosType()
    pos$x <- i
    pos$y <- 0
    pos$z <- 0
    
    num <- krui_createDefaultUnit()
    inputs[i] <- num
    
    krui_setUnitName(num,paste("Input_",i,sep=""))
    krui_setUnitPosition(num, pos)
    
  }
  
  hidden <- vector()
  for(i in 1:nHidden) {
    pos <- PosType()
    pos$x <- i
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
  
  outputs <- vector()
  for(i in 1:nOutputs) {
    pos <- PosType()
    pos$x <- i
    pos$y <- 4
    pos$z <- 0
    
    num <- krui_createDefaultUnit()
    outputs[i] <- num
    
    krui_setUnitName(num,paste("Output_",i,sep=""))
    
    #krui_setUnitActFunc(num, 'Act_Identity');
    #krui_setUnitOutFunc(num, 'Out_Threshold05');
    
#  krui_setUnitTType(num,OUTPUT)
    krui_setUnitTType(num,2)
    
    krui_setUnitPosition(num, pos)
    krui_setCurrentUnit(num)
    for(j in hidden)  {
      krui_createLink(j,0);
    };  
  }
  
  return(list(inputs=inputs, hidden=hidden, outputs=outputs))
} 

#---------------------------------------
#Function createPatterns
#---------------------------------------
createPatterns <- function(inputValues, targetValues, slp=slp) {

  x <- as.matrix(inputValues)
  nInputs <- ncol(x)
  if (nInputs != length(slp$inputs)) 
    stop(paste("number of input data columns ", nInputs ," does not match number of input neurons ", length(slp$inputs) ,sep=""))

  if(!missing(targetValues)){
    y <- as.matrix(targetValues)
    nOutputs <- ncol(y)
    if (nOutputs != length(slp$outputs)) 
      stop(paste("number of output data columns ", nOutputs ," does not match number of output neurons ", length(slp$outputs) ,sep=""))

    if(nrow(x) != nrow(y)) 
      stop(paste("input value rows ",nrow(x)," not same as output value rows ",nrow(y),sep=""))
  }
  
  #krui_deleteAllPatterns()
  
  #in the wrapping code, "free" has to be removed..
  res <- krui_allocNewPatternSet()
  patset <- res[[2]]
  
  for(i in 1:nrow(x)) {
    for(j in 1:nInputs)  {
      krui_setUnitActivation(slp$inputs[j], x[i,j]);
    }
	
    if(!missing(targetValues)) {	
      for(j in 1:nOutputs)  {
        krui_setUnitActivation(slp$outputs[j], y[i,j]);
      }
    }
    krui_newPattern();
  }

  return(patset)
}

#---------------------------------------
#Function predictValues
#---------------------------------------
predictValues <- function(inputValues, slp)  {
  
  predictions <- matrix(nrow= nrow(inputValues), ncol=length(slp$outputs))
  #currentPattern <- 84
  for(currentPattern in 1:nrow(inputValues))  {
    
    krui_setPatternNo(currentPattern)
    
    #OUTPUT_NOTHING  1
    krui_showPattern(1)
    
    krui_updateNet(0,0)
    
    for(i in 1:length(slp$outputs)) {
      predictions[currentPattern,i] <- krui_getUnitOutput(slp$outputs[i])
    }
    
#    if (length(which(o != irisTargets[currentPattern,])) != 0)  {
#      print("not recognized correctly:")
#      print(currentPattern)
#    }
#    print(o)
#    print(irisTargets[currentPattern,])
  }
  
  return(predictions)
} 

rsnns <- function(x, ...) UseMethod("rsnns")

rsnns.default <- function(x, y, size=5, decay=0.2, maxit=100, linout=FALSE) {

    snns <- NULL
    x <- as.matrix(x)
    y <- as.matrix(y)
    if(any(is.na(x))) stop("missing values in 'x'")
    if(any(is.na(y))) stop("missing values in 'y'")
    if(dim(x)[1L] != dim(y)[1L]) stop("nrows of 'x' and 'y' must match")
	
    snns$n <- c(dim(x)[2L], size, dim(y)[2L])
    snns$nunits <- as.integer(1L + sum(snns$n))
    snns$nconn <- rep(0, snns$nunits+1L)
    snns$conn <- numeric(0L)
	
	krui_setLearnFunc('Quickprop')
    krui_setUpdateFunc('Topological_Order')
    krui_setUnitDefaults(1,0,1,0,1,'Act_Logistic','Out_Identity')

	slp <- createSingleLayerPerceptron(snns$n[1L],snns$n[2L],snns$n[3L])
    patset <- createPatterns(x, y, slp)
	krui_setCurrPatSet(patset)
	
    krui_initializeNet(-1,1)
    krui_shufflePatterns(1)
    krui_DefTrainSubPat()

    error <- vector()
    for(i in 1:maxit) {
      res <- myBackpropLearnAllPatterns(decay)
      if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
      error[i] <- res[[2]]
    }
	
	snns$slp <- slp
	snns$generalErrorIterations <- error
	fit <- predictValues(x, slp)
    
	snns$fitted.values <- fit
	
	class(snns) <- "rsnns"
    snns	
}

print.rsnns <- function(x, ...)
{
    if(!inherits(x, "rsnns")) stop("not a legitimate snns model")
    cat("a ",x$n[1L],"-",x$n[2L],"-",x$n[3L]," network", sep="")
}

predict.rsnns <- function(object, newdata, type=c("raw","class"), ...)
{
    if(!inherits(object, "rsnns")) stop("object not of class \"rsnns\"")
    type <- match.arg(type)
    if(missing(newdata)) z <- fitted(object)
	else {
      if(is.null(dim(newdata)))
        dim(newdata) <- c(1L, length(newdata)) # a row vector
      x <- as.matrix(newdata)     # to cope with dataframes
      if(any(is.na(x))) stop("missing values in 'x'")
   
      keep <- 1L:nrow(x)
      rn <- rownames(x)
	
  	  ntr <- nrow(x)
      nout <- object$n[3L]
		
      z <- matrix(NA, nrow(newdata), nout,
                    dimnames = list(rn, dimnames(object$fitted.values)[[2L]]))
			
      #predict values..	
      patset <- createPatterns(newdata, slp=object$slp)	
	  krui_setCurrPatSet(patset)
	  predictions <- predictValues(newdata, object$slp)
	  z[keep,] <- predictions
	}
	z
}

#function(x, y, weights, size, Wts, mask=rep(TRUE, length(wts)),
#	 linout=FALSE, entropy=FALSE, softmax=FALSE, censored=FALSE, skip=FALSE,
#	 rang=0.7, decay=0, maxit=100, Hess=FALSE, trace=TRUE,
#         MaxNWts=1000, abstol=1.0e-4, reltol=1.0e-8, ...)



library(RWeka)
iris <- read.arff(system.file("arff", "iris.arff", package = "RWeka"))
#iris[,5] <- as.numeric(iris[,5])

#shuffle the vector
iris <- iris[sample(1:nrow(iris),length(1:nrow(iris))),1:ncol(iris)]

#center data
irisValues <- matrix(nrow=nrow(iris), ncol=4)
for(i in 1:4) {
  irisValues[,i] <- iris[,i] - mean(iris[,i])  
}

#decode class labels
irisTargets <- matrix(nrow=nrow(iris), ncol=3)
for(i in 1:nrow(iris))
  for(j in 1:3)  {
    irisTargets[i,j] <- as.numeric(j == as.numeric(iris[i,5]))    
  }

irisTrainValues <- irisValues[1:120,] 
irisTrainTargets <- irisTargets[1:120,] 

irisTestValues <- irisValues[121:nrow(iris),]    
irisTestTargets <- irisTargets[121:nrow(iris),]    

mySnnsNet <- rsnns(irisTrainValues, irisTrainTargets, size=5, decay=0.1, maxit=5000)

plot(mySnnsNet$generalErrorIterations[1:500])#, type="l")

mySnnsNet$generalErrorIterations[1:500]

fits <- round(fitted.values(mySnnsNet))

notCorrect <- 0
for(i in 1:nrow(fits))  {
    if (length(which(fits[i,] != irisTrainTargets[i,])) != 0)  {
	  notCorrect <- notCorrect + 1
    }
}
print(paste("Errors in fit: ",notCorrect, " of ", nrow(fits), "\n",sep=""))

predictions <- round(predict(mySnnsNet,irisTestValues))
#round(predictions)

notCorrect <- 0
for(i in 1:nrow(predictions))  {
    if (length(which(predictions[i,] != irisTestTargets[i,])) != 0)  {
	  notCorrect <- notCorrect + 1
    }
}
print(paste("Errors in test set: ",notCorrect, " of ", nrow(predictions), "\n",sep=""))


#irisTargets
#iris

# #----------------------------------------------------
# slp <- createSingleLayerPerceptron(4,3,3)
# slp

# trainPatternSet <- createPatterns(irisTrainValues, irisTrainTargets, slp)
# testPatternSet <- createPatterns(irisTestValues, irisTestTargets, slp)

# krui_setCurrPatSet(trainPatternSet)

# #Training

# #myBackpropLearnAllPatterns(0.2)
# #RSNNS_testSinglePattern(1, 0.2)

# krui_initializeNet(-1,1)
# krui_shufflePatterns(1)

# #int insize, outsize, instep, outstep, max_n_pos;
# #res = krui_DefTrainSubPat(&insize, &outsize, &instep, &outstep, &max_n_pos);

# krui_DefTrainSubPat()
# #krui_DefTrainSubPat(1,2,3,4,5)

# error <- vector()
# for(i in 1:5000) {
  # res <- myBackpropLearnAllPatterns(0.2)
  # if(res[[1]] != 0) print(paste("An error occured at iteration ", i, " : ", res, sep=""))
  # error[i] <- res[[2]]
# }

# error[1:100]
# error[4900:5000]

# plot(error[1:100], type="l")

# krui_setCurrPatSet(trainPatternSet);
# fit <- predictValues(irisTrainValues, slp)

# krui_setCurrPatSet(testPatternSet);
# predictions <- predictValues(irisTestValues, slp)

# round(fit)
# round(predictions)

# #krui_saveNewPatterns(paste(basePath,"/pyencoder_iris_Rtest.pat",sep=""),trainPatternSet)
# #krui_saveNet(paste(basePath,"/pyencoder_iris_Rtest.net",sep=""),"pyencoder")

# #----------------------------------------------------





		 
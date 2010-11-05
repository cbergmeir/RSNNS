library(RSNNS)

basePath <- ("/home/bergmeir/ts-code/Rpackages/RSNNS/man/")

for (objectName in ls("package:RSNNS")) {
  if(is.function(get(objectName)))  {
    prompt(objectName, paste(basePath, objectName,".Rd",sep=""), force.function=TRUE)  
  } else {
    prompt(objectName, paste(basePath, objectName,".Rd",sep=""))    
  }
  
}

setwd(basePath)
command <- "sed  -i -e 's/%%  ~~function to do ... ~~/function to do/g' *.Rd"
try(system(command, intern=TRUE))

command <- "sed  -i -e 's/%%   ~~ data name\\/kind ... ~~/data name\\/kind/g' *.Rd"
try(system(command, intern=TRUE))

    
#ls("package:RSNNS")
#lsf.str("package:RSNNS")

#get(ls("package:RSNNS")[50])
#is.function(get(ls("package:RSNNS")[50]))

library(RSNNS)

mySnnsObject <- SnnsRObjectFactory()

ver <- mySnnsObject$getVersion()
ver

mySnnsObject$getInitialisationFunc()
    
noFunc <- mySnnsObject$getNoOfFunctions()
noFunc

allFuncs <- NULL

for(i in 1:noFunc) allFuncs <- rbind(allFuncs, mySnnsObject$getFuncInfo(i))

allFuncs

newunit <- mySnnsObject$createDefaultUnit()
print("New unit:")
newunit

print("Act func name:")
mySnnsObject$getUnitActFuncName(newunit)


mySnnsObject$isFunction("Act_Logistic", 2);

mySnnsObject$getFuncParamInfo("Act_Logistic", 2);

mySnnsObject$getFuncParamInfo("Act_Component", 2);

mySnnsObject$getFuncParamInfo("Std_Backpropagation", 4);
mySnnsObject$getFuncParamInfo("Quickprop", 4);

mySnnsObject$getFuncParamInfo("JE_BP", 4);
mySnnsObject$getFuncParamInfo("JE_BP_Momentum", 4);

mySnnsObject$getFuncParamInfo("BackpropJogChunk", 4);


#TODO: these functions crash...
#mySnnsObject$getFirstSiteTableEntry()
#mySnnsObject$getNextSiteTableEntry()
#
#mySnnsObject$getFirstSymbolTableEntry()
#mySnnsObject$getNextSymbolTableEntry()

mySnnsObject$getNetInfo()
mySnnsObject$getMemoryManagerInfo()

mySnnsObject$getLearnFunc()
mySnnsObject$getUpdateFunc()
mySnnsObject$getUnitDefaults()

#mySnnsObject$getUnitOutFuncName(200)



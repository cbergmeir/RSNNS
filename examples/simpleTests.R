library(RSNNS)

ver <- SnnsKrui_getVersion()
ver

noFunc <- SnnsKrui_getNoOfFunctions()
noFunc

allFuncs <- NULL

for(i in 1:noFunc) allFuncs <- rbind(allFuncs, SnnsKrui_getFuncInfo(i))

allFuncs

newunit <- SnnsKrui_createDefaultUnit()
print("New unit:")
newunit

print("Act func name:")
SnnsKrui_getUnitActFuncName(newunit)


SnnsKrui_isFunction("Act_Logistic", 2);

SnnsKrui_getFuncParamInfo("Act_Logistic", 2);

SnnsKrui_getFuncParamInfo("Std_Backpropagation", 4);
SnnsKrui_getFuncParamInfo("Quickprop", 4);

SnnsKrui_getFuncParamInfo("JE_BP", 4);
SnnsKrui_getFuncParamInfo("JE_BP_Momentum", 4);

SnnsKrui_getFuncParamInfo("BackpropJogChunk", 4);


#TODO: these functions crash...
#SnnsKrui_getFirstSiteTableEntry()
#SnnsKrui_getNextSiteTableEntry()
#
#SnnsKrui_getFirstSymbolTableEntry()
#SnnsKrui_getNextSymbolTableEntry()

SnnsKrui_getNetInfo()
SnnsKrui_getMemoryManagerInfo()

SnnsKrui_getLearnFunc()
SnnsKrui_getUpdateFunc()
SnnsKrui_getUnitDefaults()

#SnnsKrui_getUnitOutFuncName(200)



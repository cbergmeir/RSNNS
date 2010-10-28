library(RSNNS)

snnsObject <- SnnsRObjectFactory()

ver <- snnsObject$getVersion()
ver

snnsObject$getInitialisationFunc()
    
noFunc <- snnsObject$getNoOfFunctions()
noFunc

allFuncs <- data.frame()

for(i in 1:noFunc) {

  fi <- snnsObject$getFuncInfo(i)
  fiInfo <- snnsObject$getFuncParamInfo(fi[[1]], fi[[2]])
  allFuncs <- rbind(allFuncs, cbind(fi$func_name, fi$func_type, fiInfo$no_of_input_params, fiInfo$no_of_input_params))

}

names(allFuncs) <- c("name", "type", "#inParams", "#outParams")
allFuncs  

newunit <- snnsObject$createDefaultUnit()
print("New unit:")
newunit

print("Act func name:")
snnsObject$getUnitActFuncName(newunit)


snnsObject$isFunction("Act_Logistic", 2);

snnsObject$getFuncParamInfo("Act_Logistic", 2);

snnsObject$getFuncParamInfo("Act_Component", 2);

snnsObject$getFuncParamInfo("Std_Backpropagation", 4);
snnsObject$getFuncParamInfo("Quickprop", 4);

snnsObject$getFuncParamInfo("JE_BP", 4);
snnsObject$getFuncParamInfo("JE_BP_Momentum", 4);

snnsObject$getFuncParamInfo("BackpropJogChunk", 4);


#TODO: these functions crash...
#snnsObject$getFirstSiteTableEntry()
#snnsObject$getNextSiteTableEntry()
#
#snnsObject$getFirstSymbolTableEntry()
#snnsObject$getNextSymbolTableEntry()

snnsObject$getNetInfo()
snnsObject$getMemoryManagerInfo()

snnsObject$getLearnFunc()
snnsObject$getUpdateFunc()
snnsObject$getUnitDefaults()

#snnsObject$getUnitOutFuncName(200)



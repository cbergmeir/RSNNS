#' Initialize the network.
#'
#' This function masks the SNNS kernel function of the same name
#'  to allow for both giving the initialization function
#' directly in the call or to use the one that is currently set. 
#' 
#' @param parameterInArray the parameters of the initialization function
#' @param initFunc the name of the initialization function
#' @rdname SnnsRObject$initializeNet
#' @usage \S4method{initializeNet}{SnnsR}(parameterInArray, initFunc)
#' @aliases initializeNet,SnnsR-method SnnsRObject$initializeNet
SnnsR__initializeNet <- function(snnsObject, parameterInArray, initFunc) {
  
  if(!missing(initFunc)) {
    .Call("SnnsCLib__setInitialisationFunc", snnsObject@snnsCLibPointer, initFunc, package="RSNNS")
  }
  err <- .Call("SnnsCLib__initializeNet", snnsObject@snnsCLibPointer, parameterInArray, package="RSNNS")
  err
}

#' Set the unit defaults.
#'
#' This function maskes the SNNS kernel function of the same name 
#' to allow both for giving the parameters directly or as a vector.
#' If the second parameter, \code{bias}, is missing, it is assumed 
#' that the first parameter should be interpreted as a vector containing
#' all parameters.
#' 
#' @param act same as SNNS kernel function
#' @param bias idem
#' @param st idem
#' @param subnet_no idem
#' @param layer_no idem
#' @param act_func idem
#' @param out_func idem
#' @rdname SnnsRObject$setUnitDefaults
#' @usage \S4method{setUnitDefaults}{SnnsR}(act, bias, st, subnet_no, layer_no, act_func, out_func)
#' @aliases setUnitDefaults,SnnsR-method SnnsRObject$setUnitDefaults
SnnsR__setUnitDefaults <- function(snnsObject, act, bias, st, subnet_no, layer_no, act_func, out_func) {
  
  #act <- c(1,0,1,0,1,"Act_Logistic","Out_Identity")
  
  if(missing(bias) && length(act)==7){

    bias <- as.numeric(act[2])
    st <- as.numeric(act[3])
    subnet_no <- as.numeric(act[4])
    layer_no <- as.numeric(act[5])
    act_func <- act[6]
    out_func <- act[7]
    act <- as.numeric(act[1])    
  }
    
  err <- .Call("SnnsCLib__setUnitDefaults", snnsObject@snnsCLibPointer, act, bias, st, subnet_no, layer_no, 
      act_func, out_func, package="RSNNS")
  err
}
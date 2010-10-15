#' Initialize the network.
#'
#' this function is a masked from the snns kernel to allow for both giving the initialization function
#' directly in the call or to use the one that is currently set. 
#' 
#' @export
#' @author Christoph
SnnsR__initializeNet <- function(snnsObject, parameterInArray, initFunc) {
  
  if(!missing(initFunc)) {
    .Call("SnnsCLib__setInitialisationFunc", snnsObject@snnsCLibPointer, initFunc, package="RSNNS")
  }
  err <- .Call("SnnsCLib__initializeNet", snnsObject@snnsCLibPointer, parameterInArray, package="RSNNS")
  #if(err != 0) SnnsR_showWarningFromSnnsError("initializeNet", err)
}

#' Set the unit defaults.
#'
#' This version is masked from the snns kernel to allow both for giving the parameters directly or in a list.
#' 
#' @export
#' @author Christoph
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
  #if(err != 0) SnnsR_showWarningFromSnnsError("setUnitDefaults", err)
  err
}
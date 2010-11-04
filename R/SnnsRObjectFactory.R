#' The main class, contains a C++ object of type SnnsCLib, i.e. an instance of SNNS
#' 
#' @slot snnsCLibPointer a pointer to the corresponding C++ class
setClass( "SnnsR", representation( snnsCLibPointer = "externalptr" ) )

#' Enable calling of C++ functions as methods of SnnsR objects.
#'
#' This function makes methods of SnnsR__ and SnnsCLib__ accessible via $.
#' If no SnnsR__ method with the name is present, then the according SnnsCLib__ 
#' method is called
#'
# @export
#' @rdname SnnsRObjectMethodCaller
#' @param x object of class \link{SnnsR-class}
#' @param name function to call
#' @usage \S4method{$}{SnnsR}(x, name) 
#' @aliases $,SnnsR-method
setMethod( "$", "SnnsR", function(x, name ){
      function(...) {
        #print(x)
        
        myFunc <- mget(paste( "SnnsR", name, sep = "__" ), mode="any", 
            envir = as.environment(-1), 
            ifnotfound = list(FALSE), inherits=TRUE)
        
        #very usefull for debugging..everytime an SnnsR or SnnsCLib funtion is called, its name is printed
        #print(name)
        
        if(is.function(myFunc[[1]])) return(myFunc[[1]](x, ... ))
        else {
          res <- .Call( paste( "SnnsCLib", name, sep = "__" ) , x@snnsCLibPointer , ... )
          
          if(is.list(res))
            if(!is.null(res$err)) {
              err <- res$err
              if(err != 0) {
                msg <- .Call( "SnnsCLib__error", x@snnsCLibPointer , err )
                warning(paste("SNNS error message in ", name, " : ", msg, sep=""))
              }
            }
          return(res)
        }
      }     
    } )


#' Object factory to create a new object of type SnnsR.
#'
#' @export
#' @author Christoph
SnnsRObjectFactory <- function(){

  snnsObject <- new( "SnnsR")
  snnsObject@snnsCLibPointer <- .Call("SnnsCLib__new", package="RSNNS")
  
  return(snnsObject)
}




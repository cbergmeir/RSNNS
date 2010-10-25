#' The main class, contains an instance of the SnnsCLib, i.e. a neural net
# @slot layers ...
# @slot generalErrorIterations ...
# @slot fitted.values ...
#' @slot snnsCLibPointer a pointer to the corresponding c++ class
setClass( "SnnsR", representation( snnsCLibPointer = "externalptr" ) )

#layers = "matrix", 
#generalErrorIterations = "matrix", 
#fitted.values = "matrix",

#' Enable calling of C functions as methods of SnnsR objects.
#'
#' This function makes methods of SnnsR__ and SnnsCLib__ accessible via $
#' if no SnnsR__ method with the name is present, then the according SnnsCLib__ 
#' method is called
#'
# @export
#' @author Christoph
setMethod( "$", "SnnsR", function(x, name ){
      function(...) {
        #print(x)
        
        myFunc <- mget(paste( "SnnsR", name, sep = "__" ), mode="any", 
            envir = as.environment(-1), 
            ifnotfound = list(FALSE), inherits=TRUE)
        
        #very usefull for debugging..everytime an SnnsR or SnnsCLib funtion is called, its name is printed
        print(name)
        
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

  #print("object factory start\n")  
  #mySnnsObject <- new( "SnnsR", pointer=.Call("SnnsCLib__new", package="RSNNS"))

  mySnnsObject <- new( "SnnsR")
  mySnnsObject@snnsCLibPointer <- .Call("SnnsCLib__new", package="RSNNS")
  
  #mySnnsObject@layers <- as.matrix(0)
  #mySnnsObject@generalErrorIterations <- as.matrix(0)
  #mySnnsObject@fitted.values <- as.matrix(0)
  
  #print("object factory ok\n")
  
  return(mySnnsObject)
}




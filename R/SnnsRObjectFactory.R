#############################################################################
#
#   This file is part of the R package "RSNNS".
#
#   Author: Christoph Bergmeir
#   Supervisor: José M. Benítez
#   Copyright (c) DiCITS Lab, Sci2s group, DECSAI, University of Granada.
#
#   This library is free software; you can redistribute it and/or
#   modify it under the terms of the GNU Library General Public
#   License as published by the Free Software Foundation; either
#   version 2 of the License, or (at your option) any later version.
# 
#   This library is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   Library General Public License for more details.
# 
#   You should have received a copy of the GNU Library General Public License
#   along with this library; see the file COPYING.LIB.  If not, write to
#   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
#   Boston, MA 02110-1301, USA.
#
#############################################################################


#' The main class of the package. Each instance contains a pointer to 
#' a C++ object of type SnnsCLib, i.e. an instance of the SNNS kernel.
#' 
#' @slot snnsCLibPointer A pointer to the corresponding C++ class
setClass( "SnnsR", representation( snnsCLibPointer = "externalptr" ) )

#' Enable calling of C++ functions as methods of \code{SnnsR-class} objects.
#'
#' This function makes methods of SnnsR__ and SnnsCLib__ accessible via "$".
#' If no SnnsR__ method is present, then the according SnnsCLib__ 
#' method is called. This enables a very flexible method handling.
#' To mask a method from SnnsCLib, e.g. to do some parameter checking or postprocessing,
#' only a method with the same name, but beginning with SnnsR__ has to be present in R. 
#' See e.g. \code{\link{SnnsRObject$initializeNet}} for such an implementation. 
#' Error handling is also done within the method caller. If the result of a function is a list with a member \code{err}, 
#' then \code{SnnsCLib__error} is called to use the SNNS kernel function to get the corresponding error message code
#' and an R warning is thrown containing this message.
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
        
        if(is.function(myFunc[[1]])) {
          res <- myFunc[[1]](x, ... )
        }
        else {
          res <- .Call( paste( "SnnsCLib", name, sep = "__" ) , x@snnsCLibPointer , ... )
        }
        
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
    } )


#' Object factory to create a new object of type \code{SnnsR-class}.
#'
#' @export
SnnsRObjectFactory <- function(){

  snnsObject <- new( "SnnsR")
  snnsObject@snnsCLibPointer <- .Call("SnnsCLib__new", package="RSNNS")
  
  return(snnsObject)
}




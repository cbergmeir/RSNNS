#' Wrapping for the Stuttgart Neural Network Simulator (SNNS)
#'
#' \tabular{ll}{
#' Package: \tab RSNNS\cr
#' Type: \tab Package\cr
#' Version: \tab 0.1\cr
#' Date: \tab 2010-07-30\cr
#' License: \tab GPL (>= 2)\cr
#' LazyLoad: \tab yes\cr
#' }
#'
#' The Stuttgart Neural Network Simulator (SNNS) is a library containing many 
#' standard implemetations of neural networks. The package makes these
#' available through a convenient interface for seamless integration within R 
#'
#' \code{\link{mlp}} is a function for training a multilayer perceptron...
#' 
#' @name RSNNS-package
#' @aliases RSNNS
#' @docType package
#' @title Wrapping for the Stuttgart Neural Network Simulator (SNNS)
#' @author Christoph Bergmeir \email{c.bergmeir@@decsai.ugr.es}
#' @references
#' \url{http://dicits.ugr.es}, \url{http://sci2s.ugr.es}
#' @keywords package
#' @seealso \code{\link{mlp}}
#' @useDynLib RSNNS .registration=TRUE
# @exportPattern "^[[:alpha:]]+"
#' @examples
#' \dontrun{mlp(...)}
NULL


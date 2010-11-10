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


#' Getting started with the RSNNS package
#'
#' \tabular{ll}{
#' Package: \tab RSNNS\cr
#' Type: \tab Package\cr
#' Version: \tab 0.3\cr
#' Date: \tab 2010-11-10\cr
#' License: \tab LGPL (>= 2)\cr
#' LazyLoad: \tab yes\cr
#' }
#'
#' The Stuttgart Neural Network Simulator (SNNS) is a library containing many 
#' standard implementations of neural networks. This package wraps the SNNS 
#' functionality to make it available from within R.
#' 
#' The package has a hierarchical architecture with three levels:
#' \itemize{
#' \item RSNNS high-level api (rsnns)
#' \item RSNNS low-level api (SnnsR)
#' \item The api of our C++ port of SNNS (SnnsCLib)
#' }
#' 
#' Many demos for using both low-level and high-level api of the package are available. To get a list of them, type:
#' 
#' \code{library(RSNNS)}
#' 
#' \code{demo()}
#'  
#' It is a good idea to start with the demos of the high-level api (which is much more convenient to use). E.g. to access
#' the iris classification demo type:
#' 
#' \code{demo(iris)}
#'  
#' or for the laser regression demo type:
#' 
#' \code{demo(laser)} 
#' 
#' As the high-level api is already quite powerful and flexible, you'll most probably normally end up using one of the functions:
#' \code{\link{mlp}}, \code{\link{dlvq}}, \code{\link{rbf}}, \code{\link{rbfDDA}}, \code{\link{elman}}, 
#' \code{\link{jordan}}, \code{\link{som}}, \code{\link{art1}}, \code{\link{art2}}, or \code{\link{assoz}},
#' with some pre- and postprocessing.
#' 
#' Demos ending with "SnnsR" show use of the low-level api. If you want to do special things with neural networks that are currently not implemented
#' in the high-level api, you can see in this demos how to do it. Many demos are present both as high-level and low-level versions. 
#' 
#' Also have a look at the original SNNS and the SNNS User Manual 4.2, especially pp 67-87 for explications on all the parameters
#' of the learning functions, and pp 145-215 for detailed (theoretical) explications of the methods and advice on their use.
#' 
#' @name RSNNS-package
#' @aliases RSNNS
#' @docType package
#' @title Getting started with the RSNNS package
#' @author Christoph Bergmeir \email{c.bergmeir@@decsai.ugr.es}
#' @references 
#' Zell, A. et al. SNNS Stuttgart Neural Network Simulator User Manual, Version 4.2
#' \url{http://www.ra.cs.uni-tuebingen.de/SNNS/}
# @references
# \url{http://dicits.ugr.es}, \url{http://sci2s.ugr.es}
#' @keywords package neural networks SNNS
#' @seealso \code{\link{mlp}}, \code{\link{dlvq}}, \code{\link{rbf}}, \code{\link{rbfDDA}}, \code{\link{elman}}, 
#' \code{\link{jordan}}, \code{\link{som}}, \code{\link{art1}}, \code{\link{art2}}, \code{\link{assoz}}
#' @useDynLib RSNNS .registration=TRUE
# @exportPattern "^[[:alpha:]]+"
# @examples
# \dontrun{mlp(...)}
NULL


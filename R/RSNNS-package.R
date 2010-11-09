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


#' Wrapping for the Stuttgart Neural Network Simulator (SNNS)
#'
#' \tabular{ll}{
#' Package: \tab RSNNS\cr
#' Type: \tab Package\cr
#' Version: \tab 0.3\cr
#' Date: \tab 2010-11-05\cr
#' License: \tab LGPL (>= 2)\cr
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


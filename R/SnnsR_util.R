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


#' Reset the \code{SnnsR-class} object.
#' 
#' Delete all pattern sets and delete the current net in the \code{\link{SnnsR-class}} object.
#'  
#' @rdname SnnsRObject$resetRSNNS
#' @usage \S4method{resetRSNNS}{SnnsR}()
#' @aliases resetRSNNS,SnnsR-method SnnsRObject$resetRSNNS
SnnsR__resetRSNNS <- function(snnsObject)  {
  
  res <- list()
  res$err <- 0
  while (res$err == 0) {
    res <- snnsObject$deletePatSet(0)
  };
  
  snnsObject$deleteNet()
  
}

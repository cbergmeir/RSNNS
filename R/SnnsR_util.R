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


#' Reset the SnnsRObject.
#' 
#' Delete all pattern sets and delete the current net in the SnnsRObject.
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

#' Get all units in the net of a certain ttype.
#' 
#' Possible ttypes defined by SNNS are, among others:
#' "UNIT_OUTPUT", "UNIT_INPUT", and "UNIT_HIDDEN". For these ttypes, special functions exist.
#' 
#' @param ttype a string containing the ttype.
#' @return a vector with integer numbers identifying the units.
#' @rdname SnnsRObject$getAllUnitsTType
#' @usage \S4method{getAllUnitsTType}{SnnsR}(ttype)
#' @aliases getAllUnitsTType,SnnsR-method SnnsRObject$getAllUnitsTType
#' @seealso \link{SnnsRObject$getAllOutputUnits}, \link{SnnsRObject$getAllInputUnits}, \link{SnnsRObject$getAllHiddenUnits}
SnnsR__getAllUnitsTType <- function(snnsObject, ttype) {
  
  res <- NULL
  
  resolvedTType <- resolveSnnsRDefine("topologicalUnitTypes", ttype)
  
  nUnits <- snnsObject$getNoOfUnits()
  
  for(i in 1:nUnits)  {
    if(i==1)  unit <- snnsObject$getFirstUnit()
    else unit <- snnsObject$getNextUnit()
    
    #print(unit)
    type <- snnsObject$getUnitTType(unit)
    if(type == resolvedTType) res <- c(res, unit)
    #print(type)
  }
  
  return(res)
}

#' Get all output units of the net.
#' 
#' @return a vector with integer numbers identifying the units.
#' @rdname SnnsRObject$getAllOutputUnits
#' @usage \S4method{getAllOutputUnits}{SnnsR}()
#' @aliases getAllOutputUnits,SnnsR-method SnnsRObject$getAllOutputUnits
#' @seealso \link{SnnsRObject$getAllUnitsTType}
SnnsR__getAllOutputUnits <- function(snnsObject) {
  return(snnsObject$getAllUnitsTType("UNIT_OUTPUT"))  
}

#' Get all input units of the net.
#' 
#' @return a vector with integer numbers identifying the units.
#' @rdname SnnsRObject$getAllInputUnits
#' @usage \S4method{getAllInputUnits}{SnnsR}()
#' @aliases getAllInputUnits,SnnsR-method SnnsRObject$getAllInputUnits
#' @seealso \link{SnnsRObject$getAllUnitsTType}
SnnsR__getAllInputUnits <- function(snnsObject) {
  return(snnsObject$getAllUnitsTType("UNIT_INPUT"))  
}

#' Get all hidden units of the net.
#' 
#' @return a vector with integer numbers identifying the units.
#' @rdname SnnsRObject$getAllHiddenUnits
#' @usage \S4method{getAllHiddenUnits}{SnnsR}()
#' @aliases getAllHiddenUnits,SnnsR-method SnnsRObject$getAllHiddenUnits
#' @seealso \link{SnnsRObject$getAllUnitsTType}
SnnsR__getAllHiddenUnits <- function(snnsObject) {
  return(snnsObject$getAllUnitsTType("UNIT_HIDDEN"))  
}

#' Set the activation function for all units of a certain ttype.
#' 
#' The function uses the function \link{getAllUnitsTType,SnnsR-method} to find all units of a certain
#' ttype, and sets the activation function of all these units to the given activation function
#'  
#' @param ttype a string containing the ttype.
#' @param act_func the name of the activation function to set.
#' @rdname SnnsRObject$setTTypeUnitsActFunc
#' @usage \S4method{setTTypeUnitsActFunc}{SnnsR}(ttype, act_func)
#' @aliases setTTypeUnitsActFunc,SnnsR-method SnnsRObject$setTTypeUnitsActFunc
#' @seealso \link{SnnsRObject$getAllUnitsTType}
#' @examples
#' \dontrun{SnnsRObject$setTTypeUnitsActFunc("UNIT_HIDDEN", "Act_Logistic")}
SnnsR__setTTypeUnitsActFunc <- function(snnsObject, ttype, act_func) {
  
  units <- snnsObject$getAllUnitsTType(ttype)
  
  for(unit in units) {
    snnsObject$setUnitActFunc(unit, act_func)
  }

}

#' Find all units whose name begins with a given prefix.
#' 
#' @param prefix a prefix that the names of the units to find have.
#' @return a vector with integer numbers identifying the units.
#' @rdname SnnsRObject$getUnitsByName
#' @usage \S4method{getUnitsByName}{SnnsR}(prefix)
#' @aliases getUnitsByName,SnnsR-method SnnsRObject$getUnitsByName
SnnsR__getUnitsByName <- function(snnsObject, prefix) {
  
  res <- NULL
  
  nUnits <- snnsObject$getNoOfUnits()
  
  for(i in 1:nUnits)  {
    if(i==1)  unit <- snnsObject$getFirstUnit()
    else unit <- snnsObject$getNextUnit()
    
    name <- snnsObject$getUnitName(unit)
    if(beginsWith(name,prefix)) res <- c(res, unit)
  }
  
  return(res)
}

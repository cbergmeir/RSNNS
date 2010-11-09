/************************************************************************************

   This file is part of SnnsCLib, a fork of the kernel and parts of the gui of 
   the Stuttgart Neural Network Simulator (SNNS), version 4.3.

   The file's original version is part of SNNS 4.3. It's source code can be found at

   http://www.ra.cs.uni-tuebingen.de/SNNS/

   SNNS 4.3 is under the license LGPL v2. We note that source code files of SNNS 4.3 
   state as version "4.2". Base of this fork is SNNS 4.3 with a reverse-applied 
   python patch (see http://developer.berlios.de/projects/snns-dev/).

   SnnsCLib was developed in 2010 by Christoph Bergmeir under supervision of 
   José M. Benítez, both affiliated to DiCITS Lab, Sci2s group, DECSAI, 
   University of Granada

   Changes done to the original code were performed with the objective to
   port it from C to C++ and to encapsulate all code in one class named SnnsCLib.

   Changes in header files mainly include:
   * removed all static keywords
   * moved initializations of variables to the constructor of SnnsCLib

   Changes in cpp code files mainly include:
   * changed file ending from .c to .cpp
   * removed all SNNS internal includes and only include SnnsCLib   
   * static variables within functions were turned into member variables of SnnsCLib
   * function declarations were changed to method declarations, i.e. "SnnsCLib::.."
     was added
   * calls to the function table are now "C++-style", using the "this"-pointer

   License of SnnsCLib:
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

************************************************************************************/


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_type.h,v $
  SHORTNAME      : type.h
  SNNS VERSION   : 4.2

  PURPOSE        : type definitions for cascade correlation
  NOTES          :

  AUTHOR         : Michael Schmalzl
  DATE           : 24.2.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:05 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _CC_TYPE_DEFINED_
#define _CC_TYPE_DEFINED_

typedef struct CC_DATA {
  struct {
    float pixelError;
    int learningFunc;
    int modification;
    int onOff;
    int pruneOnOff;
    int backfittOnOff;
    int backfittPatience;
    int pruneFunc;
    float addParameters[5]; 
    int fastmode;
  } GLOBAL;
  struct {
    double covarianceChange;
    int    candidatePatience;
    int    maxNoOfUpdateCycles;
    int    maxNoOfCandUnits;
    int    actFunc;
  } CAND;
  struct {
    double errorChange;
    int    outputPatience;
    int    maxNoOfUpdateCycles;
  } OUT;
} CASCADE_TYPE;

typedef struct CC_LAYER {
  int xPosFirstRow;
  int NoOfUnitsInLayer;
} Layer_Type;

typedef struct CC_GROUP {
  struct Unit* BestUnitPtr;
  int Score;
} CC_Group_Type;

typedef struct TAC_LINK_ERROR {
  float LnCurrentSlope;
  float LnPreviousSlope;
  float LnLastWeightChange;
} TAC_LINK_ERROR_TYPE;

typedef struct TAC_SPECIAL_UNIT {
  int   NoOfUnitsInRegion;
  float SummedErrorInRegion;
  float ErrorCorrelation;
  float Window;
  float* Xi;
  float* Ri;
  TAC_LINK_ERROR_TYPE* LinkError;
} TAC_SPECIAL_UNIT_TYPE;


typedef struct TAC_PRIME{
  float xi_CurrentSlope;
  float ri_CurrentSlope;
  float xi_PreviousSlope;
  float ri_PreviousSlope;
  float xi_LastChange;
  float ri_LastChange;
} TAC_PRIME_TYPE;

#endif

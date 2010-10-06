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

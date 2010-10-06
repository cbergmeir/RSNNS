/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/dlvq_learn.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Michael Schmalzl 
  DATE           : 5.2.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:07 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

/* begin global definition section */

extern krui_err getNoOfClasses(int startPattern, int endPattern);
extern krui_err LEARN_DLVQ(int startPattern, int endPattern, float *ParameterInArray,
                   int NoOfInParams, float **ParameterOutArray, int *NoOfOutParams);
extern void normPatterns(int startPattern, int endPattern);
extern void allocInitialUnitArray(void);
extern void initInitialUnitArray(int startPattern, int endPattern);
extern void allocArrays(void);
extern krui_err dlvq_setPointers(void);
extern void generateMissingClassHiddenUnits(int *generatedNewUnit);

extern int newPatternsLoaded;
extern int dlvq_numberOfLearnCycles;
/* end global definition section */

/* begin privat definition section */
/* end privat definition section */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_inversion.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 29.02.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _KR_INVERSION_DEFINED_
#define  _KR_INVERSION_DEFINED_



/* required functions that are declared here */

extern int    kr_initInversion(void);
extern void   kr_inv_forwardPass(struct UnitList *inputs);
extern double kr_inv_backwardPass(float learn, float delta_max, int *err_units,
				  float ratio, struct UnitList *inputs,
				  struct UnitList *outputs);

#endif 

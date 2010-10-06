/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_display.h,v $
  SHORTNAME      : cc_display.h
  SNNS VERSION   : 4.2

  PURPOSE        : Display Functions of Cascade-Correlation
  NOTES          :

  AUTHOR         : Michael Schmalzl /  Juergen Gatter
  DATE           : 26.11.95

  CHANGED BY     : Michael Vogt, Guenter Mamier, Christian Wehrfritz
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:25:48 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _CC_DISPLAY_DEFINED_
#define  _CC_DISPLAY_DEFINED_

/* begin global definition section */
extern krui_err cc_calculateNetParameters(void);
extern void cc_setSpecialUnits(int X_Max);
extern krui_err cc_setHiddenUnit(struct Unit *UnitPtr,int LayerOfNewUnit);
extern void cc_updatePosOfSpecialUnits(void);
extern void cc_redisplay(void);

extern int cc_hiddenXminPos;

/* end   global definition section */

/* begin privat definition section */
/* end privat definition section */

#endif /* _CC_DISPLAY_DEFINED_ */

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_display.ph,v $
  SHORTNAME      : cc_display.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Michael Schmalzl, Christian Wehrfritz (PCC)
  DATE           : 5.2.1993

  CHANGED BY     : Michael Schmalzl
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:25:50 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _CC_DISPLAY_DEFINED_
#define  _CC_DISPLAY_DEFINED_

/* begin global definition section */
krui_err cc_calculateNetParameters(void);
void cc_setSpecialUnits(int X_Max);
krui_err cc_setHiddenUnit(struct Unit *UnitPtr,int LayerOfNewUnit);
void cc_updatePosOfSpecialUnits(void);
void cc_redisplay(void);

int cc_hiddenXminPos;

/* end   global definition section */

/* begin privat definition section */
int cc_display_mode=Y_MAX_MODE1;
int cc_LayerDistance=DEFAULT_DISTANCE_BETWEEN_LAYERS;
int cc_lastFirstOutputRow;


/* end privat definition section */

#endif

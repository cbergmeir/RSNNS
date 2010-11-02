/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_art2.ph,v $
  SHORTNAME      : bn_art2
  SNNS VERSION   : 4.2

  PURPOSE        : 
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 15.1.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_ART2_DEFINED_
#define  _BN_ART2_DEFINED_

/* begin global definition section */

//void bn_createART2 (void);

/* end global definition section */


//static int      bn_art2_open = 0;
//static Widget   baseWidget;
//static Widget   art2UnitWidget[2], art2RowWidget[2];



krui_err bn_art2_make_layer (
                            int    units,
                            int    rows,
                            int    cols,
                            int    Xbegin,
                            int    Ybegin,
                            int    Xsep,
                            int    Ysep,
                            char  *name_prefix,
                            int    unit_ttype,
                            char  *actfuncname,
                            char  *outfuncname,
                            int    NoOfSites,
                            char **SiteNames,
                            char **SiteFuncNames);                        
krui_err bn_art2_createNet (int f1Units, int f1Rows, int f2Units, int f2Rows);
//static void bn_art2_donePROC (void);
//static void bn_art2_createPROC (void);

/* end private definition section */

#endif

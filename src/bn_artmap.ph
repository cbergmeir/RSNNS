/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_artmap.ph,v $
  SHORTNAME      : bn_artmap
  SNNS VERSION   : 4.2

  PURPOSE        : 
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 15.1.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:35 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_ARTMAP_DEFINED_
#define  _BN_ARTMAP_DEFINED_

/* begin global definition section */

//void bn_createARTMAP (void);

/* end global definition section */


/* begin private definition section */








//static int      bn_artmap_open = 0;
//static Widget   baseWidget;
//static Widget   artmapUnitWidget[4], artmapRowWidget[4];



krui_err bn_artmap_make_layer (
                            int    units,
                            int    rows,
                            int    cols,
                            int    Xbegin,
                            int    Ybegin,
                            char  *name_prefix,
                            int    unit_ttype,
                            char  *actfuncname,
                            char  *outfuncname,
                            int    NoOfSites,
                            char **SiteNames,
                            char **SiteFuncNames);                        
krui_err bn_artmap_createNet (int f1aUnits, int f1aRows, int f2aUnits, int f2aRows, 
                                     int f1bUnits, int f1bRows, int f2bUnits, int f2bRows);
//static void bn_artmap_donePROC (void);
//static void bn_artmap_createPROC (void);

/* end private definition section */

#endif

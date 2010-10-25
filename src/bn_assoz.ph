/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_assoz.ph,v $
  SHORTNAME      : bn_assoz
  SNNS VERSION   : 4.2

  PURPOSE        : Bignet Auto Assoziative Function Prototypes, private
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : February 11 1994

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:38 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_ASSOZ_DEFINED_
#define  _BN_ASSOZ_DEFINED_

/* begin global definition section */

//void bn_create_assoz (void);

/* end global definition section */

/* begin private definition section */

//static int assoz_widget_open = 0;
//static Widget baseWidget;
//static Widget Xsize, Ysize;
//static int X,Y,IUnits,HUnits;


/* Learning function name */
#define ASSOZ_LEARN_FUNC_NAME     "RM_delta"

/* Update function name */
#define ASSOZ_UPDATE_FUNC_NAME    "Auto_Synchronous"

/* Init function name */
#define ASSOZ_INIT_FUNC_NAME      "RM_Random_Weights"

/* bn_bignet */
//static void done_PROC (void);
//static void create_net_PROC (void);

/* bn_create_net */
//void bn_assoz_errChk(krui_err);
krui_err bn_assoz_createNet(int X, int Y);

/* end  private definition section */
#endif

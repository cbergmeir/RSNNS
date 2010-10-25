/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_kohonen.ph,v $
  SHORTNAME      : bn_kohonen
  SNNS VERSION   : 4.2

  PURPOSE        : Bignet Kohonen Function Prototypes, private
  NOTES          :

  AUTHOR         : Marc Seemann, Marcus Ritt
  DATE           : July 13 1993

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:50 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_KOHONEN_DEFINED_
#define  _BN_KOHONEN_DEFINED_

/* begin global definition section */

//void bn_createKOHONEN (void);

/* end global definition section */


/* begin private definition section */
/*
static int kohonen_widget_open = 0;
static Widget baseWidget;
static Widget Comp, Xsize, Ysize;
static int X,Y,IUnits,HUnits,OUnits;
*/

/* Learning function name */
#define KOHONEN_LEARN_FUNC_NAME     "Kohonen"

/* Update function name */
#define KOHONEN_UPDATE_FUNC_NAME    "Kohonen_Order"

/* Init function name */
#define KOHONEN_INIT_FUNC_NAME      "Kohonen_Weights_v3.2"

/* bn_bignet */
//static void done_PROC (void);
//static void create_net_PROC (void);

/* bn_create_net */
//void errChk(krui_err);
krui_err bn_kohonen_createNet(int X, int Y, int IUnits, int HUnits);

/* end  private definition section */
#endif








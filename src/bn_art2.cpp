/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_art2.c,v $
  SHORTNAME      : bn_art2
  SNNS VERSION   : 4.2

  PURPOSE        : Creates the ART2-window and ART2 networks. 
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 15.1.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#include "SnnsCLib.h"

/*
#include <config.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_mainP.h"
#include "ui_xWidgets.h"
#include "ui_fileP.h"
#include "bn_basics.h"
#include "ui_control.h"
#include "ui_confirmer.h"

#include "bn_art2.ph"
*/


/*****************************************************************************
  FUNCTION : bn_art2_make_layer

  PURPOSE  : Generation of a complete layer
  NOTES    : 

  UPDATE   : 20.1.1993
******************************************************************************/

krui_err SnnsCLib::bn_art2_make_layer (
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
                            char **SiteFuncNames)
                         

{
   krui_err             ret_code = KRERR_NO_ERROR;

   int                  row;
   int                  col;
   int                  count;
   int                  last_col;
   int                  unit_no;
   struct PosType       unit_pos;
   char                 *site_name;
   int                  site_no;
   char                 name[10];


   /* Create Units */
   for (row = 1, count = 0; row <= rows; row++) {

      if ((units % rows) > 0) {
         last_col = POS_SIGN((units-count) % (rows-row+1));
      } else {
         last_col = 1;
      } /*if*/

      for (col = 1; col <= (cols-1+last_col); col++) {

         count++;

         unit_no = krui_createDefaultUnit();
         if (unit_no < 0)  CHECK_RETURN (unit_no);

         ret_code = krui_setUnitTType (unit_no, unit_ttype);
         CHECK_RETURN( ret_code );

         ret_code = krui_setUnitActFunc(unit_no, actfuncname);
         CHECK_RETURN (ret_code);

         ret_code = krui_setUnitOutFunc(unit_no, outfuncname);
         CHECK_RETURN (ret_code);

         if (units > 1) {
            sprintf (name,"%s%d",name_prefix,count);
         } else {
            sprintf (name,"%s",name_prefix);
         } /*if*/

         ret_code = krui_setUnitName (unit_no, name);
         CHECK_RETURN (ret_code);

         unit_pos.x = Xbegin + Xsep*(col-1);
         unit_pos.y = Ybegin + Ysep*(row-1);
         krui_setUnitPosition( unit_no, &unit_pos );

         ret_code = krui_setCurrentUnit (unit_no);
         CHECK_RETURN (ret_code);

         /* Add Sites */
         site_no = 1;
         while (site_no <= NoOfSites) {
             site_name= *(SiteNames+(site_no-1));

             if (site_name != NULL) {
                ret_code = krui_addSite (site_name);
                CHECK_RETURN (ret_code);
             } /*if*/

             site_no++;
         } /*while*/

      } /*for*/

   } /*for*/

   return (ret_code);
} 




/*****************************************************************************
  FUNCTION : bn_art2_createNet

  PURPOSE  : Generation of the ART2 network using SNNS kernel functions.
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/

krui_err SnnsCLib::bn_art2_createNet (int f1Units, int f1Rows, int f2Units, int f2Rows)

{
   krui_err          ret_code   = KRERR_NO_ERROR;

   int               i, j;


   /********* create units ******************/

   /* create Input Layer  */
   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, INP_X, INP_Y,
                          1, 1, "inp", INPUT, ACTF_INP, OUTF_INP, 0,
                          NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create W-Units */
   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, W_X, W_Y,
                          F1_SEP_X, F1_SEP_Y, "w",
                          HIDDEN, ACTF_W, OUTF_W, 0, NULL, NULL);
   CHECK_RETURN (ret_code);



   /* create X-Units */

   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, X_X, X_Y,
                          F1_SEP_X, F1_SEP_Y, "x", HIDDEN, ACTF_X,
                          OUTF_X, 0, NULL, NULL);
   CHECK_RETURN (ret_code);


   /* create U-Units */

   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, U_X, U_Y,
                          F1_SEP_X, F1_SEP_Y, "u", HIDDEN, ACTF_U,
                          OUTF_U, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create V-Units */
   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, V_X, V_Y,
                          F1_SEP_X, F1_SEP_Y, "v",
                          HIDDEN, ACTF_V, OUTF_V, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create P-Units */
   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, P_X, P_Y,
                          F1_SEP_X, F1_SEP_Y, "p",
                          HIDDEN, ACTF_P, OUTF_P, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Q-Units */

   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, Q_X, Q_Y,
                          F1_SEP_X, F1_SEP_Y, "q", HIDDEN, ACTF_Q,
                          OUTF_Q, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create R-Units */

   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, R_X, R_Y,
                          F1_SEP_X, F1_SEP_Y, "r", HIDDEN, ACTF_R,
                          OUTF_R, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Recognition Layer */
   ret_code = bn_art2_make_layer (f2Units, f2Rows, F2_COLS, REC_X, REC_Y, 1, 1,
                          "rec", SPECIAL, ACTF_REC, OUTF_REC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   ret_code = bn_art2_make_layer (f2Units, f2Rows, F2_COLS, RST_X, RST_Y, 1, 1,
                          "rst", HIDDEN, ACTF_RST, OUTF_RST, 0, NULL, NULL);
   CHECK_RETURN (ret_code);




   /********* Create Links **************************************/


   /* TO w units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (W_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM inp units */
      ret_code = krui_createLink (INP_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

      /* FROM u units */
      ret_code = krui_createLink (U_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO x units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (X_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM w units */
      ret_code = krui_createLink (W_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO u units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (U_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM v units */
      ret_code = krui_createLink (V_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO v units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (V_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM x units */
      ret_code = krui_createLink (X_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

      /* FROM q unit */
      ret_code = krui_createLink (Q_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO p units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (P_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM rec units */
      for (j=1; j<=f2Units; j++) {
         ret_code = krui_createLink (REC_UNIT(j),0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

      /* FROM u unit */
      ret_code = krui_createLink (U_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO q units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (Q_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM p unit */
      ret_code = krui_createLink (P_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO r units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (R_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM p unit */
      ret_code = krui_createLink (P_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM inp unit */
      ret_code = krui_createLink (INP_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO rec units */
   for (i=1; i<=f2Units; i++) {

      ret_code = krui_setCurrentUnit (REC_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM p units */
      for (j=1; j<=f1Units; j++) {
         ret_code = krui_createLink (P_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

      /* FROM rst unit */
      ret_code = krui_createLink (RST_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO rst units */
   for (i=1; i<=f2Units; i++) {

      ret_code = krui_setCurrentUnit (RST_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM rec units */
      ret_code = krui_createLink (REC_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/

   printf (" Done.\n");


   /*  set the update function  */
   ret_code = krui_setUpdateFunc (UPDATE_FUNC_NAME);
   CHECK_RETURN( ret_code );

   /* set the learning function */
   ret_code = krui_setLearnFunc (LEARN_FUNC_NAME);

   //ui_NumberOfLearnParamsChanged();
   //ui_NumberOfUpdateParamsChanged();
   return (ret_code);
}






/*****************************************************************************
  FUNCTION : bn_art2_donePROC

  PURPOSE  : Callback function for DONE button in art2
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/
/*
static void bn_art2_donePROC (void)

{

   XtDestroyWidget (baseWidget);
   bn_art2_open = 0;

}
*/


/*****************************************************************************
  FUNCTION : bn_art2_createPROC

  PURPOSE  : Callback function for CREATE NET button in art2
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/
/*
static void bn_art2_createPROC (void)

{
  int units[2], rows[2];
  int NoOfLayers = 2;

  if (bn_basics_check_existingNetwork()) {

     bn_basics_getValues (NoOfLayers, units, rows, art2UnitWidget, art2RowWidget);

     if (bn_basics_checkValues(NoOfLayers, units, rows)) {
        bn_art2_createNet (units[0], rows[0], units[1], rows[1]);
        bn_basics_refresh ();
        ui_confirmOk ("Network created!");
     }

  }

  return;

}
*/


/*****************************************************************************
  FUNCTION : bn_createART2

  PURPOSE  : create ART2 panel
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/
/*
void bn_createART2(void)

{
  bn_basics_createART (ART2_MODEL, &baseWidget, &bn_art2_open,
                       art2UnitWidget, art2RowWidget,
                       (XtCallbackProc) bn_art2_createPROC,
                       (XtCallbackProc) bn_art2_donePROC);


}
*/

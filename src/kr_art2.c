/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_art2.c,v $
  SHORTNAME      : kr_art2
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Functions for ART2 networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:36 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

/*#################################################

GROUP: include files

#################################################*/




#include <stdlib.h>

#ifndef NULL /* if NULL pointer is not defined include stdio.h */
#include <stdio.h>
#endif

#include <string.h>

#include <math.h>


#include "kr_const.h"
#include "kr_mac.h"
#include "kr_def.h"
#include "kr_typ.h"
#include "kr_funcs.h"
#include "kernel.h"
#include "glob_typ.h"
#include "kr_art.h"     /*  Function prototypes for ART networks */
#include "krart_df.h"   /*  Definitions for ART networks */
#include "kr_art2.ph"






/*#################################################

GROUP: declaration of module functions

#################################################*/




/*#################################################

GROUP: IMPLEMENTATION SECTION of
       ART 2 kernel functions visible to other
       modules
       by Kai-Uwe Herrmann

#################################################*/






/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
krui_err kra2_sort (void)
{

   TopoPtrArray        topo_ptr = topo_ptr_array;
   int                 ret_code = KRERR_NO_ERROR;

   int                 no_of_w_units = 0;
   int                 no_of_x_units = 0;
   int                 no_of_u_units = 0;
   int                 no_of_v_units = 0;
   int                 no_of_p_units = 0;
   int                 no_of_q_units = 0;
   int                 no_of_r_units = 0;
   int                 no_of_rst_units = 0;

   /* initialize ART2 sorting
   */
   krart_init_sorting ();


   /* get no of input units and no of recognition units
   */
   NoOfInputUnits         = krart_get_NoOfInputUnits ();
   Art2_NoOfRecUnits      = kra2_get_NoOfRecUnits ();

   if (NoOfInputUnits == 0) {
      ret_code = KRERR_NO_INPUT_UNITS;
      return (ret_code);
   } /*if*/

   if (Art2_NoOfRecUnits == 0) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("recognition");
   } /*if*/


   /* insert a NULL pointer to topo ptr array for left limitation
   */
   *topo_ptr++ = NULL;


   /* determine unit types and insert them into topo ptr array
   */

   topo_layer[0] = topo_ptr;


   /* determine input units
   */
   topo_layer[0] = topo_ptr;
   ret_code = kra2_get_InpUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   *topo_ptr++ = NULL;

   /* determine w units
   */
   topo_layer[1] = topo_ptr;
   ret_code = kra2_get_WUnits (&topo_ptr, &no_of_w_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_w_units != NoOfInputUnits) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("w");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine x units
   */
   topo_layer[2] = topo_ptr;
   ret_code = kra2_get_XUnits (&topo_ptr, &no_of_x_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_x_units != NoOfInputUnits) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("x");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine u units
   */
   topo_layer[3] = topo_ptr;
   ret_code = kra2_get_UUnits (&topo_ptr, &no_of_u_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_u_units != NoOfInputUnits) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("u");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine v units
   */
   topo_layer[4] = topo_ptr;
   ret_code = kra2_get_VUnits (&topo_ptr, &no_of_v_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_v_units != NoOfInputUnits) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("v");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine p units
   */
   topo_layer[5] = topo_ptr;
   ret_code = kra2_get_PUnits (&topo_ptr, &no_of_p_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_p_units != NoOfInputUnits) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("p");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine q units
   */
   topo_layer[6] = topo_ptr;
   ret_code = kra2_get_QUnits (&topo_ptr, &no_of_q_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_q_units != NoOfInputUnits) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("q");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine r units
   */
   topo_layer[7] = topo_ptr;
   ret_code = kra2_get_RUnits (&topo_ptr, &no_of_r_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_r_units != NoOfInputUnits) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("r");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine rec units
   */
   topo_layer[8] = topo_ptr;
   ret_code = kra2_get_RecUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine rst units
   */
   topo_layer[9] = topo_ptr;
   ret_code = kra2_get_RstUnits (&topo_ptr, &no_of_rst_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_rst_units != Art2_NoOfRecUnits) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("reset");
   } /*if*/

   *topo_ptr++ = NULL;


   /* check if the logical type of really all units is determined
   */
   if (krart_check_undeterminedUnits ()) {
      ret_code = topo_msg.error_code;
      return (ret_code);
   } /*if*/


   /* Now check the topo ptr array
   */
   ret_code = kra2_TopoPtrArray ();

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check link structure
   */
   topo_ptr = topo_ptr_array + 1;

   /* Check links of input units
   */
   ret_code = kra2_LinksToInpUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of w units
   */
   ret_code = kra2_LinksToWUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of x units
   */
   ret_code = kra2_LinksToXUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of u units
   */
   ret_code = kra2_LinksToUUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of v units
   */
   ret_code = kra2_LinksToVUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of p units
   */
   ret_code = kra2_LinksToPUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of q units
   */
   ret_code = kra2_LinksToQUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of r units
   */
   ret_code = kra2_LinksToRUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of rec units
   */
   ret_code = kra2_LinksToRecUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of rst units
   */
   ret_code = kra2_LinksToRstUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


/*--------------------------------------------------------------------*/
#if 0
/* this was the preliminary version of the sorting routine for testing*/
                          *topo_ptr++ = NULL;
                          topo_layer[ART2_INP_LAY-1] = topo_ptr;

                          Art2_NoOfRecUnits = 0;
                          FOR_ALL_UNITS (unit_ptr) {
                             if (IS_SPECIAL_UNIT (unit_ptr)) {
                                Art2_NoOfRecUnits++;
                             } /*if*/
                          } /*FOR_ALL_UNITS*/


                          unit_ptr = unit_array + 1;

                          for (i=1; i<= NoOfInputUnits; i++, unit_ptr++) {
                             *topo_ptr++ = unit_ptr;
                             unit_ptr->lln = ART2_INP_LAY;
                          } /*for*/

                          *topo_ptr++ = NULL;
                          topo_layer[ART2_W_LAY-1] = topo_ptr;

                          for (i=1; i<=NoOfInputUnits; i++, unit_ptr++) {
                             *topo_ptr++ = unit_ptr;
                             unit_ptr->lln = ART2_W_LAY;
                          } /*for*/

                          *topo_ptr++ = NULL;
                          topo_layer[ART2_X_LAY-1] = topo_ptr;

                          for (i=1; i<=NoOfInputUnits; i++, unit_ptr++) {
                             *topo_ptr++ = unit_ptr;
                             unit_ptr->lln = ART2_X_LAY;
                          } /*for*/

                          *topo_ptr++ = NULL;
                          topo_layer[ART2_U_LAY-1] = topo_ptr;

                          for (i=1; i<=NoOfInputUnits; i++, unit_ptr++) {
                             *topo_ptr++ = unit_ptr;
                             unit_ptr->lln = ART2_U_LAY;
                          } /*for*/

                          *topo_ptr++ = NULL;
                          topo_layer[ART2_V_LAY-1] = topo_ptr;

                          for (i=1; i<=NoOfInputUnits; i++, unit_ptr++) {
                             *topo_ptr++ = unit_ptr;
                             unit_ptr->lln = ART2_V_LAY;
                          } /*for*/

                          *topo_ptr++ = NULL;
                          topo_layer[ART2_P_LAY-1] = topo_ptr;

                          for (i=1; i<=NoOfInputUnits; i++, unit_ptr++) {
                             *topo_ptr++ = unit_ptr;
                             unit_ptr->lln = ART2_P_LAY;
                          } /*for*/

                          *topo_ptr++ = NULL;
                          topo_layer[ART2_Q_LAY-1] = topo_ptr;

                          for (i=1; i<=NoOfInputUnits; i++, unit_ptr++) {
                             *topo_ptr++ = unit_ptr;
                             unit_ptr->lln = ART2_Q_LAY;
                          } /*for*/


                          *topo_ptr++ = NULL;
                          topo_layer[ART2_R_LAY-1] = topo_ptr;

                          for (i=1; i<=NoOfInputUnits; i++, unit_ptr++) {
                             *topo_ptr++ = unit_ptr;
                             unit_ptr->lln = ART2_R_LAY;
                          } /*for*/


                          *topo_ptr++ = NULL;
                          topo_layer[ART2_REC_LAY-1] = topo_ptr;

                          for (i=1; i<=Art2_NoOfRecUnits; i++, unit_ptr++) {
                             *topo_ptr++ = unit_ptr;
                             unit_ptr->lln = ART2_REC_LAY;
                          } /*for*/

                          *topo_ptr++ = NULL;
                          topo_layer[ART2_RST_LAY-1] = topo_ptr;

                          for (i=1; i<=Art2_NoOfRecUnits; i++, unit_ptr++) {
                             *topo_ptr++ = unit_ptr;
                             unit_ptr->lln = ART2_RST_LAY;
                          } /*for*/

                          *topo_ptr++ = NULL;
                          *topo_ptr   = NULL;
#endif /*#if 0*/
/*-----------------------------------------------------------------*/

   return (ret_code);

} /* kra2_sort () */
/*___________________________________________________________________________*/


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
krui_err kra2_init_propagate (void)
{
   int                    ret_code = KRERR_NO_ERROR;

   ret_code = kra2_init_i_act ();

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   ret_code = kra2_init_fix_weights ();

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   ret_code = krart_reset_activations ();

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   NoOfDelaySteps = 0;

   return (ret_code);

} /* kra2_init_propagate () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
krui_err  kra2_set_params (FlintType rho, FlintType a, FlintType b, FlintType c, FlintType d, FlintType theta)
{
   Param_rho   = rho;
   Param_a     = a;
   Param_b     = b;
   Param_c     = c;
   Param_d     = d;
   Param_theta = theta;
   return (KRERR_NO_ERROR);
} /* kra2_set_theta () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
FlintType  kra2_get_rho (void)
{
   return (Param_rho);
} /* kra2_get_Param_rho () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
FlintType  kra2_get_a (void)
{
   return (Param_a);
} /* kra2_get_Param_a () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
FlintType  kra2_get_b (void)
{
   return (Param_b);
} /* kra2_get_Param_b () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
FlintType  kra2_get_c (void)
{
   return (Param_c);
} /* kra2_get_Param_c () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
FlintType  kra2_get_d (void)
{
   return (Param_d);
} /* kra2_get_Param_d () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
FlintType  kra2_get_theta (void)
{
   return (Param_theta);
} /* kra2_get_Param_theta () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void  kra2_checkReset (void)
{
   FlintType NormR;

   NormR = ART2_PARAM_e + kra2_L2_Norm (ART2_R_LAY);

   if ((kra2_f1_stable()) && (kra2_topdn_phase()) && (kra2_get_rho() / NormR > 1)) {
      GlobalReset  = TRUE;
   } else {
      GlobalReset  = FALSE;
   } /*if*/

} /* kra2_checkReset () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
bool  kra2_Reset (void)
{
   return (GlobalReset);
} /* kra2_Reset () */
/*___________________________________________________________________________*/





/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void  kra2_init_pattern (void)
{
  TopDownPhase = FALSE;
  f1_stable    = FALSE;
  GlobalReset  = FALSE;
} /* kra2_init_phase () */
/*___________________________________________________________________________*/





/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
bool  kra2_topdn_phase (void)
{
  return (TopDownPhase);
} /* kra2_topdn_phase () */
/*___________________________________________________________________________*/







/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void   kra2_compute_norms (void)
{
   NormInp = kra2_compute_l2_norm (ART2_INP_LAY);
   NormW   = kra2_compute_l2_norm (ART2_W_LAY);
   NormU   = kra2_compute_l2_norm (ART2_U_LAY);
   NormV   = kra2_compute_l2_norm (ART2_V_LAY);
   NormP   = kra2_compute_l2_norm (ART2_P_LAY);
   NormR   = kra2_compute_l2_norm (ART2_R_LAY);
} /* kra2_compute_norms () */
/*___________________________________________________________________________*/






/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
FlintType  kra2_L2_Norm (int Layer)
{
   switch (Layer) {
   case ART2_INP_LAY:
      return (NormInp);
   case ART2_W_LAY:
      return (NormW);
   case ART2_U_LAY:
      return (NormU);
   case ART2_V_LAY:
      return (NormV);
   case ART2_P_LAY:
      return (NormP);
   case ART2_R_LAY:
      return (NormR);
   default :
      return (0.0);
   } /* switch */
} /* kra2_L2_Norm () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
bool  kra2_classified (void)
{

   /* if the F1-Layer is actually stable, then it is necessary to continue
      propagation for MIN_NO_OF_DELAY_STEPS cycles. Then the information
      has been forwarded from recognition layer to r-units and it may be
      decided about a reset.

      if there occurs a reset during the delay propagation steps the no. of
      executed delay propagation steps is reset to 0 and FALSE is returned.

      if the F1-Layer becomes unstable during the delay propagation steps, the
      no. of executed delay propagation steps is reset to 0, too and FALSE is
      returned.
   */

   if (kra2_topdn_phase() && kra2_f1_stable()) {
      if (NoOfDelaySteps >= MIN_NO_OF_DELAY_STEPS) {
         if ( ! kra2_Reset()) {
            return (TRUE);
         } else {
            NoOfDelaySteps = 0;
            return (FALSE);
         } /*if*/
      } else {
         NoOfDelaySteps++;
         return (FALSE);
      } /*if*/
   } else {
      NoOfDelaySteps = 0;
      return (FALSE);
   } /*if*/

} /* kra2_classified () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
bool  kra2_not_classifiable (void)
{
   TopoPtrArray topo_ptr         = topo_layer[ART2_RST_LAY-1];


   /* if there is one local reset unit that is inactive, then the
      pattern may still be classifiable
   */
   while (*topo_ptr != NULL) {
      if ((*topo_ptr)->Out.output < 0.1) {
         return (FALSE);
      } /*if*/
      topo_ptr++;
   } /*while*/


   /* all local reset units are active -> pattern not classifiable */
   return (TRUE);

} /* kra2_classified () */
/*___________________________________________________________________________*/





/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void  kra2_save_for_stability_check (void)
{
   register TopoPtrArray    topo_ptr ;

   topo_ptr = topo_layer[ART2_W_LAY-1];

   while (*topo_ptr != NULL) {
      (*topo_ptr)->value_a = (*topo_ptr)->act;
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_X_LAY-1];

   while (*topo_ptr != NULL) {
      (*topo_ptr)->value_a = (*topo_ptr)->act;
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_U_LAY-1];

   while (*topo_ptr != NULL) {
      (*topo_ptr)->value_a = (*topo_ptr)->act;
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_V_LAY-1];

   while (*topo_ptr != NULL) {
      (*topo_ptr)->value_a = (*topo_ptr)->act;
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_P_LAY-1];

   while (*topo_ptr != NULL) {
      (*topo_ptr)->value_a = (*topo_ptr)->act;
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_Q_LAY-1];

   while (*topo_ptr != NULL) {
      (*topo_ptr)->value_a = (*topo_ptr)->act;
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_R_LAY-1];

   while (*topo_ptr != NULL) {
      (*topo_ptr)->value_a = (*topo_ptr)->act;
      topo_ptr++;
   } /*while*/

} /* kra2_save_for_stability_check () */
/*___________________________________________________________________________*/





/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void  kra2_check_f1_stability (void)
{
   register TopoPtrArray    topo_ptr;

   if (f1_stable && ( ! kra2_topdn_phase())) {
      TopDownPhase = TRUE;
   } /*if*/

   if (kra2_Reset() && kra2_topdn_phase()) {
      TopDownPhase = FALSE;
      f1_stable    = FALSE;
   } /*if*/


   topo_ptr = topo_layer[ART2_W_LAY-1];

   while (*topo_ptr != NULL) {
      if ( fabs((*topo_ptr)->value_a-(*topo_ptr)->act) > F1_STABILITY_PARAM ) {
         f1_stable = FALSE;
         return;
      } /*if*/
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_X_LAY-1];

   while (*topo_ptr != NULL) {
      if ( fabs((*topo_ptr)->value_a-(*topo_ptr)->act) > F1_STABILITY_PARAM ) {
         f1_stable = FALSE;
         return;
      } /*if*/
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_U_LAY-1];

   while (*topo_ptr != NULL) {
      if ( fabs((*topo_ptr)->value_a-(*topo_ptr)->act) > F1_STABILITY_PARAM ) {
         f1_stable = FALSE;
         return;
      } /*if*/
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_V_LAY-1];

   while (*topo_ptr != NULL) {
      if ( fabs((*topo_ptr)->value_a-(*topo_ptr)->act) > F1_STABILITY_PARAM ) {
         f1_stable = FALSE;
         return;
      } /*if*/
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_P_LAY-1];

   while (*topo_ptr != NULL) {
      if ( fabs((*topo_ptr)->value_a-(*topo_ptr)->act) > F1_STABILITY_PARAM ) {
         f1_stable = FALSE;
         return;
      } /*if*/
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_Q_LAY-1];

   while (*topo_ptr != NULL) {
      if ( fabs((*topo_ptr)->value_a-(*topo_ptr)->act) > F1_STABILITY_PARAM ) {
         f1_stable = FALSE;
         return;
      } /*if*/
      topo_ptr++;
   } /*while*/

   topo_ptr = topo_layer[ART2_R_LAY-1];

   while (*topo_ptr != NULL) {
      if ( fabs((*topo_ptr)->value_a-(*topo_ptr)->act) > F1_STABILITY_PARAM ) {
         f1_stable = FALSE;
         return;
      } /*if*/
      topo_ptr++;
   } /*while*/

   f1_stable = TRUE;

   return;

} /* kra2_check_f1_stability () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
bool  kra2_f1_stable (void)
{
   return (f1_stable);
} /* kra2_f1_stable () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
int  kra2_getClassNo (void)
{
    TopoPtrArray   topo_ptr = topo_layer[ART2_REC_LAY-1];
    int            i;

    /* if ART2 sorting wasn't performed then return negative value
       to indicate mistake
    */
    if (topo_ptr == NULL) {
       return (-1);
    } /*if*/

    /* look for winning unit */
    for (i = 1;
         (i <= Art2_NoOfRecUnits) || ((*topo_ptr)->Out.output == kra2_get_d());
         i++, topo_ptr++
        );

    if ((i > Art2_NoOfRecUnits) && ((*topo_ptr)->Out.output < kra2_get_d())) {
       return (-1);
    } else {
       return (topo_ptr - topo_layer[ART2_REC_LAY-1] + 1);
    } /*if*/

} /* kra2_getClassNo () */
/*___________________________________________________________________________*/




/*#################################################

GROUP: IMPLEMENTATION SECTION of
       ART 2 kernel functions, local to this module
       by Kai-Uwe Herrmann

#################################################*/





/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static void   kra2_set_fix_weight (struct Unit *src_unit, struct Unit *trgt_unit, FlintType *weight)
{

   if ((src_unit == NULL) || (trgt_unit == NULL)) {
      return;
   } /*if*/

   switch (src_unit->lln) {

   case ART2_INP_LAY:

      switch (trgt_unit->lln) {
      case ART2_W_LAY:
         *weight = ART2_LINK_INP_W;
         break;
      case ART2_R_LAY:
         *weight = ART2_LINK_INP_R;
         break;
      } /*switch*/

      break;

   case ART2_W_LAY:

      switch (trgt_unit->lln) {
      case ART2_X_LAY:
         *weight = ART2_LINK_W_X;
         break;
      } /*switch*/
      break;

   case ART2_X_LAY:

      *weight = ART2_LINK_X_V;
      break;

   case ART2_U_LAY:

      switch (trgt_unit->lln) {
      case ART2_W_LAY:
         *weight = ART2_LINK_U_W(kra2_get_a());
         break;
      case ART2_P_LAY:
         *weight = ART2_LINK_U_P;
         break;
      default :
         break;
      } /* switch */
      break;

   case ART2_V_LAY:

      switch (trgt_unit->lln) {
      case ART2_U_LAY:
         *weight = ART2_LINK_V_U;
         break;
      default :
         break;
      } /* switch */
      break;

   case ART2_P_LAY:

      switch (trgt_unit->lln) {
      case ART2_Q_LAY:
         *weight = ART2_LINK_P_Q;
         break;
      case ART2_R_LAY:
         *weight = ART2_LINK_P_R(kra2_get_c());
         break;
      default :
         break;
      } /* switch */
      break;

   case ART2_Q_LAY:

      *weight = ART2_LINK_Q_V(kra2_get_b());
      break;

   case ART2_R_LAY:

      /* doesn't have outgoing links because Norm of R is computed
         within the function kra2_L2_Norm and not in a unit
      */
      break;

   case ART2_REC_LAY:

      if (trgt_unit->lln == ART2_RST_LAY) {
         *weight = ART2_LINK_REC_RST;
      } /*if*/
      break;

   case ART2_RST_LAY:

      switch (trgt_unit->lln) {
      case ART2_RST_LAY:
         *weight = ART2_LINK_RST_RST;
         break;
      case ART2_REC_LAY:
         *weight = ART2_LINK_RST_REC(kra2_get_d());
         break;
      default :
         break;
      } /* switch */
      break;

   } /* switch */


} /* kra2_set_fix_weights () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static FlintType kra2_compute_l2_norm (int Layer)
{
   register TopoPtrArray    topo_ptr = topo_layer[Layer-1];
   register FlintType       sum = 0.0;
   register FlintType       val;

   while (*topo_ptr != NULL) {
      val = (*topo_ptr)->Out.output;
      sum += val * val;
      topo_ptr++;
   } /*while*/
   return ( sqrt((double) sum) );
}
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static int kra2_get_NoOfRecUnits (void)
{
   register struct Unit  *unit_ptr;
   int                   count           = 0;


   FOR_ALL_UNITS (unit_ptr) {

      if (IS_SPECIAL_UNIT(unit_ptr)) {
         count++;
      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (count);


} /* kra2_get_NoOfRecUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_get_InpUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit  *unit_ptr;

   krui_err              ret_code = KRERR_NO_ERROR;


   FOR_ALL_UNITS (unit_ptr) {

      if (IS_INPUT_UNIT (unit_ptr)) {

         if (!(CHECK_ACT_FUNC(unit_ptr, ART2_ACTF_INP))) {
            TOPO_MSG_ACT_FUNC (unit_ptr);
         } /*if*/

         if (!(CHECK_OUT_FUNC(unit_ptr, ART2_OUTFUNC))) {
            TOPO_MSG_OUT_FUNC (unit_ptr);
         } /*if*/

         unit_ptr->lln = ART2_INP_LAY;
         **topo_ptr = unit_ptr;
         unit_ptr->flags |= UFLAG_REFRESH;
         (*topo_ptr)++;
      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_get_InpUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kra2_get_WUnits (TopoPtrArray *topo_ptr, int *no_of_w_units)
{
   register struct Unit   *unit_ptr;
   register struct Unit   *unit_ptr2;
   register struct Link   *link_ptr;
   register struct Link   *link_ptr2;

   bool                   has_link_to_inp    = FALSE;
   bool                   has_outgoing_links = FALSE;

   krui_err               ret_code = KRERR_NO_ERROR;


   /* look for w units */

   FOR_ALL_UNITS (unit_ptr) {

      if (unit_ptr->lln == 0) {

         /* check if unit has direct inputs and link to inp unit */

         if (UNIT_HAS_SITES (unit_ptr)) {
            TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
         } else {

            has_link_to_inp = FALSE;
            has_outgoing_links = FALSE;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {
               if (link_ptr->to->lln == ART2_INP_LAY) {
                  has_link_to_inp = TRUE;
                  break;
               } /*if*/
            } /*FOR_ALL_UNITS*/

            /* check if unit_ptr->  has outgoing links */

            FOR_ALL_UNITS (unit_ptr2) {
               if (UNIT_HAS_SITES (unit_ptr2)) {
                  TOPO_MSG_UNEXPECTED_SITES (unit_ptr2);
               } else {
                  FOR_ALL_LINKS (unit_ptr2, link_ptr2) {
                     if (link_ptr2->to == unit_ptr) {
                        has_outgoing_links = TRUE;
                        break;
                     } /*if*/
                  } /*FOR_ALL_LINKS*/
               } /*if*/

               if (has_outgoing_links) {
                  break;
               } /*if*/
            } /*FOR_ALL_UNITS*/

            /* if unit has link to input unit and has outgoing links it
               is a w unit
            */
            if (has_link_to_inp && has_outgoing_links) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ART2_ACTF_W))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ART2_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               if (! (UNIT_REFRESHED (unit_ptr))) {
                  unit_ptr->lln = ART2_W_LAY;
                  (*no_of_w_units)++;
                  **topo_ptr = unit_ptr;
                  unit_ptr->flags |= UFLAG_REFRESH;
                  (*topo_ptr)++;
               } /*if*/

            }  /*if*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_get_WUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_get_XUnits (TopoPtrArray *topo_ptr, int *no_of_x_units)
{
   krui_err               ret_code   = KRERR_NO_ERROR;

   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;

   bool                   has_link_to_w;

   FOR_ALL_UNITS (unit_ptr) {

     if (UNIT_HAS_SITES (unit_ptr)) {
        TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
     } else {

        has_link_to_w = FALSE;

        FOR_ALL_LINKS (unit_ptr, link_ptr) {
           if (link_ptr->to->lln == ART2_W_LAY) {
              has_link_to_w = TRUE;
              break;
           } /*if*/
        } /*FOR_ALL_LINKS*/
     } /*if*/

     if (has_link_to_w) {

        if (!(CHECK_ACT_FUNC (unit_ptr, ART2_ACTF_X))) {
           TOPO_MSG_ACT_FUNC (unit_ptr);
        } /*if*/

        if (! (UNIT_REFRESHED (unit_ptr))) {
           unit_ptr->lln = ART2_X_LAY;
           (*no_of_x_units)++;
           **topo_ptr = unit_ptr;
           unit_ptr->flags |= UFLAG_REFRESH;
           (*topo_ptr)++;
        } /*if*/

     } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_get_XUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_get_UUnits (TopoPtrArray *topo_ptr, int *no_of_u_units)
{
   register struct Unit    *unit_ptr1;
   register struct Unit    *unit_ptr2 = NULL;
   register struct Link    *link_ptr;

   bool                    is_u_unit;

   krui_err                ret_code   = KRERR_NO_ERROR;

   FOR_ALL_UNITS (unit_ptr1) {

      if (unit_ptr1->lln == ART2_W_LAY) {

         is_u_unit = FALSE;

         FOR_ALL_LINKS (unit_ptr1, link_ptr) {

            unit_ptr2=link_ptr->to;
            if (unit_ptr2->lln != ART2_INP_LAY) {
               is_u_unit = TRUE;
               break;
            } /*if*/

         } /*FOR_ALL_LINKS*/


         if (is_u_unit) {

            if (!(CHECK_ACT_FUNC (unit_ptr2, ART2_ACTF_U))) {
               TOPO_MSG_ACT_FUNC (unit_ptr2);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr2, ART2_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_ptr2);
            } /*if*/

            if (! (UNIT_REFRESHED (unit_ptr2))) {
               unit_ptr2->lln = ART2_U_LAY;
               (*no_of_u_units)++;
               **topo_ptr = unit_ptr2;
               unit_ptr2->flags |= UFLAG_REFRESH;
               (*topo_ptr)++;
            } /*if*/

         }  /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_get_UUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_get_VUnits (TopoPtrArray *topo_ptr, int *no_of_v_units)
{
   register struct Unit  *unit_ptr;
   register struct Link  *link_ptr;
   bool                  has_link_to_x;

   krui_err               ret_code   = KRERR_NO_ERROR;

   FOR_ALL_UNITS (unit_ptr) {

      if (unit_ptr->lln == 0) {

         if (UNIT_HAS_SITES (unit_ptr)) {
            TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
         } else {
            has_link_to_x = FALSE;
            FOR_ALL_LINKS (unit_ptr, link_ptr) {
               if (link_ptr->to->lln == ART2_X_LAY) {
                  has_link_to_x = TRUE;
                  break;
               } /*if*/
            } /*FOR_ALL_LINKS*/
         } /*if*/

         if (has_link_to_x) {

           if (!(CHECK_ACT_FUNC (unit_ptr, ART2_ACTF_V))) {
              TOPO_MSG_ACT_FUNC (unit_ptr);
           } /*if*/

           if (!(CHECK_OUT_FUNC (unit_ptr, ART2_OUTFUNC))) {
              TOPO_MSG_OUT_FUNC (unit_ptr);
           } /*if*/

           if (! (UNIT_REFRESHED (unit_ptr))) {
              unit_ptr->lln = ART2_V_LAY;
              (*no_of_v_units)++;
              **topo_ptr = unit_ptr;
              unit_ptr->flags |= UFLAG_REFRESH;
              (*topo_ptr)++;
           } /*if*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_get_VUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_get_PUnits (TopoPtrArray *topo_ptr, int *no_of_p_units)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;

   bool                   is_p_unit;

   krui_err               ret_code   = KRERR_NO_ERROR;

   FOR_ALL_UNITS (unit_ptr) {

      if (unit_ptr->lln == 0) {

         if (UNIT_HAS_SITES (unit_ptr)) {
            TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
         } else {

            is_p_unit = FALSE;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((unit_ptr->lln != ART2_W_LAY) &&
                   (link_ptr->to->lln == ART2_U_LAY)
                  )
               {
                  is_p_unit = TRUE;
                  break;
               } /*if*/

            } /*FOR_ALL_UNITS*/

            if (is_p_unit) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ART2_ACTF_P))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ART2_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               if (! (UNIT_REFRESHED (unit_ptr))) {
                  unit_ptr->lln = ART2_P_LAY;
                  (*no_of_p_units)++;
                  **topo_ptr = unit_ptr;
                  unit_ptr->flags |= UFLAG_REFRESH;
                  (*topo_ptr)++;
               } /*if*/

            } /*if*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_get_PUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kra2_get_QUnits (TopoPtrArray *topo_ptr, int *no_of_q_units)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;

   bool                   has_link_to_p;
   bool                   has_link_to_other;

   krui_err               ret_code   = KRERR_NO_ERROR;

   FOR_ALL_UNITS (unit_ptr) {

      if (unit_ptr->lln == 0) {

         if (UNIT_HAS_SITES (unit_ptr)) {
            TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
         } else {

            has_link_to_p     = FALSE;
            has_link_to_other = FALSE;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               switch (link_ptr->to->lln) {
               case ART2_P_LAY:
                  has_link_to_p = TRUE;
                  break;
               default:
                  has_link_to_other = TRUE;
                  break;
               } /*switch*/

            } /*FOR_ALL_LINKS*/

            if ((has_link_to_p) && (! has_link_to_other)) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ART2_ACTF_Q))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (! (UNIT_REFRESHED (unit_ptr))) {
                  unit_ptr->lln = ART2_Q_LAY;
                  (*no_of_q_units)++;
                  **topo_ptr = unit_ptr;
                  unit_ptr->flags |= UFLAG_REFRESH;
                  (*topo_ptr)++;
               } /*if*/

            } /*if*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_get_QUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_get_RUnits (TopoPtrArray *topo_ptr, int *no_of_r_units)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;

   bool                   has_link_to_p;
   bool                   has_link_to_inp;

   krui_err               ret_code   = KRERR_NO_ERROR;

   FOR_ALL_UNITS (unit_ptr) {

      if (unit_ptr->lln == 0) {

         if (UNIT_HAS_SITES (unit_ptr)) {
            TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
         } else {

            has_link_to_inp = FALSE;
            has_link_to_p   = FALSE;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               switch (link_ptr->to->lln) {
               case ART2_INP_LAY:
                  has_link_to_inp = TRUE;
                  break;
               case ART2_P_LAY:
                  has_link_to_p = TRUE;
                  break;
               } /*switch*/

            } /*FOR_ALL_LINKS*/

            if (has_link_to_inp && has_link_to_p) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ART2_ACTF_R))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ART2_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               if (! (UNIT_REFRESHED (unit_ptr))) {
                  unit_ptr->lln = ART2_R_LAY;
                  (*no_of_r_units)++;
                  **topo_ptr = unit_ptr;
                  unit_ptr->flags |= UFLAG_REFRESH;
                  (*topo_ptr)++;
               } /*if*/

            } /*if*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_get_RUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_get_RecUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit  *unit_ptr;

   krui_err               ret_code   = KRERR_NO_ERROR;

   FOR_ALL_UNITS (unit_ptr) {

      if ((unit_ptr->lln == 0) && (IS_SPECIAL_UNIT (unit_ptr))) {

         if (!(CHECK_ACT_FUNC (unit_ptr, ART2_ACTF_REC))) {
            TOPO_MSG_ACT_FUNC (unit_ptr);
         } /*if*/

         if (!(CHECK_OUT_FUNC (unit_ptr, ART2_OUTFUNC))) {
            TOPO_MSG_OUT_FUNC (unit_ptr);
         } /*if*/

         if (! (UNIT_REFRESHED (unit_ptr))) {
            unit_ptr->lln = ART2_REC_LAY;
            **topo_ptr = unit_ptr;
            unit_ptr->flags |= UFLAG_REFRESH;
            (*topo_ptr)++;
         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_get_RecUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_get_RstUnits (TopoPtrArray *topo_ptr, int *no_of_rst_units)
{
   register struct Unit   *unit_ptr;

   krui_err               ret_code   = KRERR_NO_ERROR;

   FOR_ALL_UNITS (unit_ptr) {

      if (unit_ptr->lln == 0)  {

         if (!(CHECK_ACT_FUNC (unit_ptr, ART2_ACTF_RST))) {
            TOPO_MSG_ACT_FUNC (unit_ptr);
         } /*if*/

         if (!(CHECK_OUT_FUNC (unit_ptr, ART2_OUTFUNC))) {
            TOPO_MSG_OUT_FUNC (unit_ptr);
         } /*if*/

         if (! (UNIT_REFRESHED (unit_ptr))) {
            unit_ptr->lln = ART2_RST_LAY;
            (*no_of_rst_units)++;
            **topo_ptr = unit_ptr;
            unit_ptr->flags |= UFLAG_REFRESH;
            (*topo_ptr)++;
         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_get_RstUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kra2_TopoPtrArray (void)
{
   TopoPtrArray    topo_inp_w_sep,
                   topo_w_x_sep,
                   topo_x_u_sep,
                   topo_u_v_sep,
                   topo_v_p_sep,
                   topo_p_q_sep,
                   topo_q_r_sep,
                   topo_r_rec_sep,
                   topo_rec_rst_sep,
                   topo_ptr_array_end;

   krui_err        ret_code = KRERR_NO_ERROR;

   topo_inp_w_sep   = topo_ptr_array + NoOfInputUnits + 1;
   topo_w_x_sep     = topo_inp_w_sep + NoOfInputUnits + 1;
   topo_x_u_sep     = topo_w_x_sep   + NoOfInputUnits + 1;
   topo_u_v_sep     = topo_x_u_sep   + NoOfInputUnits + 1;
   topo_v_p_sep     = topo_u_v_sep   + NoOfInputUnits + 1;
   topo_p_q_sep     = topo_v_p_sep   + NoOfInputUnits + 1;
   topo_q_r_sep     = topo_p_q_sep   + NoOfInputUnits + 1;
   topo_r_rec_sep   = topo_q_r_sep   + NoOfInputUnits + 1;
   topo_rec_rst_sep = topo_r_rec_sep + Art2_NoOfRecUnits + 1;
   topo_ptr_array_end = topo_rec_rst_sep + Art2_NoOfRecUnits + 1;

   if ((*topo_inp_w_sep   != NULL) ||
       (*topo_w_x_sep     != NULL) ||
       (*topo_x_u_sep     != NULL) ||
       (*topo_u_v_sep     != NULL) ||
       (*topo_v_p_sep     != NULL) ||
       (*topo_p_q_sep     != NULL) ||
       (*topo_q_r_sep     != NULL) ||
       (*topo_r_rec_sep   != NULL) ||
       (*topo_rec_rst_sep != NULL) ||
       (*topo_ptr_array_end  != NULL)
      )
   {
      ART2_RETURN_NET_ERROR (ret_code);
   } /*if*/

   return (ret_code);

} /* kra2_TopoPtrArray () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kra2_LinksToInpUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;

   krui_err               ret_code = KRERR_NO_ERROR;

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      if (UNIT_HAS_INPUTS (unit_ptr)) {
         topo_msg.error_code = KRERR_I_UNITS_CONNECT;
         topo_msg.dest_error_unit = unit_ptr-unit_array;
         topo_msg.src_error_unit = 0;
         ret_code = topo_msg.error_code;
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra2_LinksToInpUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_LinksToWUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;
   int                    count_inp, count_u;
   krui_err               ret_code = KRERR_NO_ERROR;

   krart_deleteTouchFlags();

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      count_inp = 0;
      count_u   = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {
         switch (link_ptr->to->lln) {
         case ART2_INP_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_inp > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_inp++;
            break;
         case ART2_U_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_u > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_u++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         } /*switch*/
      } /*FOR_ALL_LINKS*/

      if ((count_inp != 1) || (count_u != 1)) {
         TOPO_MSG_LINK_MISSING (unit_ptr);
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra2_LinksToWUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_LinksToXUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;
   int                    count_w;
   krui_err               ret_code = KRERR_NO_ERROR;

   krart_deleteTouchFlags();

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      count_w   = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {
         switch (link_ptr->to->lln) {
         case ART2_W_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_w > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_w++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/
      } /*FOR_ALL_LINKS*/

      if (count_w != 1) {
         TOPO_MSG_LINK_MISSING (unit_ptr);
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra2_LinksToXUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_LinksToUUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;
   int                    count_v;
   krui_err               ret_code = KRERR_NO_ERROR;

   krart_deleteTouchFlags();

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      count_v   = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {
         switch (link_ptr->to->lln) {
         case ART2_V_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_v > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_v++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/
      } /*FOR_ALL_LINKS*/

      if (count_v != 1) {
         TOPO_MSG_LINK_MISSING (unit_ptr);
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra2_LinksToUUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_LinksToVUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;
   int                    count_x, count_q;
   krui_err               ret_code = KRERR_NO_ERROR;

   krart_deleteTouchFlags();

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      count_x   = 0;
      count_q   = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {
         switch (link_ptr->to->lln) {
         case ART2_X_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_x > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_x++;
            break;
         case ART2_Q_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_q > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_q++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/
      } /*FOR_ALL_LINKS*/

      if ((count_x != 1) || (count_q != 1)) {
         TOPO_MSG_LINK_MISSING (unit_ptr);
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra2_LinksToVUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_LinksToPUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;
   int                    count_u, count_rec;
   krui_err               ret_code = KRERR_NO_ERROR;

   krart_deleteTouchFlags();

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      count_u   = 0;
      count_rec = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {
         switch (link_ptr->to->lln) {
         case ART2_U_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_u > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_u++;
            break;
         case ART2_REC_LAY:
            count_rec++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/
      } /*FOR_ALL_LINKS*/

      if ((count_u != 1) || (count_rec != Art2_NoOfRecUnits)) {
         TOPO_MSG_LINK_MISSING (unit_ptr);
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra2_LinksToPUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_LinksToQUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;
   int                    count_p;
   krui_err               ret_code = KRERR_NO_ERROR;

   krart_deleteTouchFlags();

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      count_p   = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {
         switch (link_ptr->to->lln) {
         case ART2_P_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_p > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_p++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/
      } /*FOR_ALL_LINKS*/

      if (count_p != 1) {
         TOPO_MSG_LINK_MISSING (unit_ptr);
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra2_LinksToQUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_LinksToRUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;
   int                    count_inp, count_p;
   krui_err               ret_code = KRERR_NO_ERROR;

   krart_deleteTouchFlags();

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      count_inp = 0;
      count_p   = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {
         switch (link_ptr->to->lln) {
         case ART2_INP_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_inp > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_inp++;
            break;
         case ART2_P_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_p > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_p++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/
      } /*FOR_ALL_LINKS*/

      if ((count_inp != 1) || (count_p != 1)) {
         TOPO_MSG_LINK_MISSING (unit_ptr);
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra2_LinksToRUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_LinksToRecUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;
   int                    count_p, count_rst;
   krui_err               ret_code = KRERR_NO_ERROR;

   krart_deleteTouchFlags();

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      count_p   = 0;
      count_rst = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {
         switch (link_ptr->to->lln) {
         case ART2_P_LAY:
            count_p++;
            break;
         case ART2_RST_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_rst > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_rst++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/
      } /*FOR_ALL_LINKS*/

      if ((count_p != NoOfInputUnits) || (count_rst != 1)) {
         TOPO_MSG_LINK_MISSING (unit_ptr);
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra2_LinksToRecUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_LinksToRstUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr;
   int                    count_rec;
   krui_err               ret_code = KRERR_NO_ERROR;

   krart_deleteTouchFlags();

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      count_rec   = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {
         switch (link_ptr->to->lln) {
         case ART2_REC_LAY:
            if ((UNIT_REFRESHED (link_ptr->to)) || (count_rec > 0)) {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            link_ptr->to->flags |= UFLAG_REFRESH;
            count_rec++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/
      } /*FOR_ALL_LINKS*/

      if (count_rec != 1) {
         TOPO_MSG_LINK_MISSING (unit_ptr);
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra2_LinksToRstUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_init_i_act (void)
{
   int                    ret_code = KRERR_NO_ERROR;

   register struct Unit   *unit_ptr;


   /* set initial activation values to 0.0 and
      copy actual activation values of input units into value_a field to
      remember it
   */


   FOR_ALL_UNITS (unit_ptr) {

         unit_ptr->i_act = 0.0;

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra2_init_i_act () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err kra2_init_fix_weights (void)
{
   int                    ret_code  = KRERR_NO_ERROR;

   register struct Unit   *unit_ptr;
   register struct Site   *site_ptr;
   register struct Link   *link_ptr;




   FOR_ALL_UNITS (unit_ptr) {

      if (UNIT_HAS_INPUTS (unit_ptr)) {

         if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               kra2_set_fix_weight (link_ptr->to, unit_ptr, &(link_ptr->weight));

            } /*FOR_ALL_LINKS*/

         } else {

            FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

               kra2_set_fix_weight (link_ptr->to, unit_ptr, &(link_ptr->weight));

            } /*FOR_ALL_SITES_AND_LINKS*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/


   return (ret_code);


} /* kra2_init_fix_weights () */
/*___________________________________________________________________________*/



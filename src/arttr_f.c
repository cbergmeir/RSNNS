/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/arttr_f.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel special transfer functions for ART-networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/13 16:23:48 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <math.h>
#include <string.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#include "kr_typ.h"	    /* Kernel types and constants  */
#include "kr_def.h"	    /* Default values	*/
#include "kr_const.h"   /* constant values of kernel */
#include "func_mac.h"   /* Transfer function macros  */
#include "glob_typ.h"

#include "kr_art.h"     /* global definitions especially for ART networks */
#include "kr_art1.h"
#include "kr_art2.h"
#include "kr_amap.h"
#include "krart_df.h"

#include "arttr_f.ph"

/*#################################################

GROUP: Unit Output Functions

#################################################*/


/*######### for ART2 model ##########*/

/* This is one of the possible noise function f which works
   between q and v, and x and v units in the F1-Layer in ART2-networks.

              |-
              |  0, if 0 <= x < Theta
       f(x) = |
              | x, if x >= Theta
              |-
  It is piecewise linear (PLin)
*/
FlintType  OUT_ART2_Noise_PLin (register FlintType activation)
{
   if (activation < kra2_get_theta()) {
      return (0.0);
   } else {
      return (activation);
   } /*if*/
} /* OUT_ART2_Noise_PLin () */


/* This is one of the possible noise function f which works
   between q and v, and x and v units in the F1-Layer in ART2-networks.

              |-
              |  (2*Theta*x*x) / (x*x + Theta*Theta), if 0 <= x < Theta
       f(x) = |
              |  x                                  , if x >= Theta
              |-

   It is continously differentiable (ContDiff)
*/
FlintType  OUT_ART2_Noise_ContDiff (FlintType activation)
{
   register FlintType theta;

   theta = kra2_get_theta();

   if ((0 <= activation) && (activation < theta)) {
      return ( (2*theta*activation*activation) /
               (activation*activation+theta*theta)
             );
   } else {
      if (activation >= theta) {
         return (activation);
      } else {
         return (0.0);
      } /*if*/
   } /*if*/
} /* OUT_ART2_Noise_ContDiff () */





/*#################################################

GROUP: Unit Activation Functions

#################################################*/


/*######### for ART1 model ##########*/


FlintType  ACT_ART1_NC  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType     sum = 0.0;

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   if (((int) (sum+0.5)) >= Art1_NoOfRecUnits) {
      return (1.0);
   } else {
      return (0.0);
   } /*if*/
} /* ACT_ART1_NC () */


/*######### for ART2 model ##########*/


FlintType ACT_ART2_Linear (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType     sum = 0.0;

   if (kra2_Reset()) {
      return (unit_ptr->i_act);
   } /*if*/

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   return (sum);
} /* ACT_ART2_Linear () */



FlintType ACT_ART2_NormP  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType              NormP;
   register FlintType     sum = 0.0;

   if (kra2_Reset()) {
      return (unit_ptr->i_act);
   } /*if*/

   NormP = kra2_L2_Norm (ART2_P_LAY);

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   return ( sum / (ART2_PARAM_e + NormP) );
} /* ACT_ART2_NormP() */



FlintType ACT_ART2_NormV  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType              NormV;
   register FlintType     sum = 0.0;

   if (kra2_Reset()) {
      return (unit_ptr->i_act);
   } /*if*/

   NormV = kra2_L2_Norm (ART2_V_LAY);

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   return ( sum / (ART2_PARAM_e + NormV) );
} /* ACT_ART2_NormV() */



FlintType ACT_ART2_NormW  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType              NormW;
   register FlintType     sum  = 0.0;

   if (kra2_Reset()) {
      return (unit_ptr->i_act);
   } /*if*/

   NormW = kra2_L2_Norm (ART2_W_LAY);

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   return ( sum / (ART2_PARAM_e + NormW) );
} /* ACT_ART2_NormW() */



FlintType ACT_ART2_NormIP  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType              NormP;
   FlintType              NormInp;
   register FlintType     sum = 0.0;

   if (kra2_Reset()) {
      return (unit_ptr->i_act);
   } /*if*/

   NormP   = kra2_L2_Norm (ART2_P_LAY);
   NormInp = kra2_L2_Norm (ART2_INP_LAY);

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   return ( sum / (ART2_PARAM_e + kra2_get_c() * NormP + NormInp) );
} /* ACT_ART2_NormIP() */




FlintType ACT_ART2_Rec  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType     sum  = 0.0;


   /* Top Down Phase */
   if (kra2_topdn_phase()) {
      if (kra2_Reset()) {
         return (-1.0);
      } else {
         return (unit_ptr->act);
      } /*if*/
   } /*if*/


   /* Bottom Up Phase */
   if ( ! kra2_f1_stable() ) {
      return (-1.0);
   } /*if*/

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   return (sum);
} /* ACT_ART2_Rec() */




FlintType ACT_ART2_Rst  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType     sum  = 0.0;

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   if (((sum >= unit_ptr->bias - 0.0001) &&
       (kra2_Reset())) || (unit_ptr->act >= 0.9))
   {
      return (1.0);
   } else {
      return (0.0);
   } /*if*/
} /* ACT_ART2_Rst () */










/*######### for ARTMAP model ##########*/


FlintType  ACT_ARTMAP_NCa  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType     sum = 0.0;

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   if (((int) (sum+0.5)) >= ArtMap_NoOfRecUnits_a) {
      return (1.0);
   } else {
      return (0.0);
   } /*if*/
} /* ACT_ARTMAP_NCa () */


FlintType  ACT_ARTMAP_NCb  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType     sum = 0.0;

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   if (((int) (sum+0.5)) >= ArtMap_NoOfRecUnits_b) {
      return (1.0);
   } else {
      return (0.0);
   } /*if*/
} /* ACT_ARTMAP_NCb () */



/* This is an important function for ARTMAP networks
   it calculates the net input to the drho unit (sum)
   If sum is greater than 0 then the activation of
   drho becomes  sum-1+epsilon where epsilon << 1
*/
FlintType  ACT_ARTMAP_DRho (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType    sum = 0.0;
   float                 epsilon = 0.0001;


   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   /* sum equals     (qu - rho_a + rg + cl_b) */
   if (sum - 2 >= 0) {
      return (sum - 2 + epsilon);
   } else {
      return (0.0);
   } /*if*/

} /* ACT_ARTMAP_DRho () */




/*#################################################

GROUP: Site Functions

#################################################*/

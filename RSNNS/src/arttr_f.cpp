/************************************************************************************

   This file is part of SnnsCLib, a fork of the kernel and parts of the gui of 
   the Stuttgart Neural Network Simulator (SNNS), version 4.3.

   The file's original version is part of SNNS 4.3. It's source code can be found at

   http://www.ra.cs.uni-tuebingen.de/SNNS/

   SNNS 4.3 is under the license LGPL v2. We note that source code files of SNNS 4.3 
   state as version "4.2". Base of this fork is SNNS 4.3 with a reverse-applied 
   python patch (see http://developer.berlios.de/projects/snns-dev/).

   SnnsCLib was developed in 2010 by Christoph Bergmeir under supervision of 
   José M. Benítez, both affiliated to DiCITS Lab, Sci2s group, DECSAI, 
   University of Granada

   Changes done to the original code were performed with the objective to
   port it from C to C++ and to encapsulate all code in one class named SnnsCLib.

   Changes in header files mainly include:
   * removed all static keywords
   * moved initializations of variables to the constructor of SnnsCLib

   Changes in cpp code files mainly include:
   * changed file ending from .c to .cpp
   * removed all SNNS internal includes and only include SnnsCLib   
   * static variables within functions were turned into member variables of SnnsCLib
   * function declarations were changed to method declarations, i.e. "SnnsCLib::.."
     was added
   * calls to the function table are now "C++-style", using the "this"-pointer

   License of SnnsCLib:
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

************************************************************************************/


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
#include <math.h>
#include <string.h>

#include "SnnsCLib.h"

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
FlintType  SnnsCLib::OUT_ART2_Noise_PLin (FlintType activation)
{
   if (activation < kra2_get_theta()) {
      return (0.0f);
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
FlintType  SnnsCLib::OUT_ART2_Noise_ContDiff (FlintType activation)
{
   FlintType theta;

   theta = kra2_get_theta();

   if ((0 <= activation) && (activation < theta)) {
      return ( (2*theta*activation*activation) /
               (activation*activation+theta*theta)
             );
   } else {
      if (activation >= theta) {
         return (activation);
      } else {
         return (0.0f);
      } /*if*/
   } /*if*/
} /* OUT_ART2_Noise_ContDiff () */





/*#################################################

GROUP: Unit Activation Functions

#################################################*/


/*######### for ART1 model ##########*/


FlintType  SnnsCLib::ACT_ART1_NC  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType     sum = 0.0f;

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
      return (1.0f);
   } else {
      return (0.0f);
   } /*if*/
} /* ACT_ART1_NC () */


/*######### for ART2 model ##########*/


FlintType SnnsCLib::ACT_ART2_Linear (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType     sum = 0.0f;

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



FlintType SnnsCLib::ACT_ART2_NormP  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType              NormP;
   FlintType     sum = 0.0f;

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



FlintType SnnsCLib::ACT_ART2_NormV  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType              NormV;
   FlintType     sum = 0.0f;

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



FlintType SnnsCLib::ACT_ART2_NormW  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType              NormW;
   FlintType     sum  = 0.0f;

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



FlintType SnnsCLib::ACT_ART2_NormIP  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType              NormP;
   FlintType              NormInp;
   FlintType     sum = 0.0f;

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




FlintType SnnsCLib::ACT_ART2_Rec  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType     sum  = 0.0f;


   /* Top Down Phase */
   if (kra2_topdn_phase()) {
      if (kra2_Reset()) {
         return (-1.0f);
      } else {
         return (unit_ptr->act);
      } /*if*/
   } /*if*/


   /* Bottom Up Phase */
   if ( ! kra2_f1_stable() ) {
      return (-1.0f);
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




FlintType SnnsCLib::ACT_ART2_Rst  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType     sum  = 0.0f;

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
      return (1.0f);
   } else {
      return (0.0f);
   } /*if*/
} /* ACT_ART2_Rst () */










/*######### for ARTMAP model ##########*/


FlintType  SnnsCLib::ACT_ARTMAP_NCa  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType     sum = 0.0f;

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
      return (1.0f);
   } else {
      return (0.0f);
   } /*if*/
} /* ACT_ARTMAP_NCa () */


FlintType  SnnsCLib::ACT_ARTMAP_NCb  (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType     sum = 0.0f;

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
      return (1.0f);
   } else {
      return (0.0f);
   } /*if*/
} /* ACT_ARTMAP_NCb () */



/* This is an important function for ARTMAP networks
   it calculates the net input to the drho unit (sum)
   If sum is greater than 0 then the activation of
   drho becomes  sum-1+epsilon where epsilon << 1
*/
FlintType  SnnsCLib::ACT_ARTMAP_DRho (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   FlintType    sum = 0.0f;
   float                 epsilon = 0.0001f;


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
      return (0.0f);
   } /*if*/

} /* ACT_ARTMAP_DRho () */




/*#################################################

GROUP: Site Functions

#################################################*/

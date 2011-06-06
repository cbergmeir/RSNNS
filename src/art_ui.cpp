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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/art_ui.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel-User-Interface for ART Networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:18 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#include "SnnsCLib.h"



/* funcname: artui_GetClassifiedStatus ()
   Purpose : Returns the classification status of the actual network
   in-Par  : none
   out-Par : status :   Possible results are:   ART_NO_CLASSIFICATION
                                                ART_CLASSIFIED
                                                ART_NOT_CLASSIFIABLE
                                                ART_DONT_KNOW
   ret_val : krui_err:  Returns an error if no Units defined
*/
krui_err SnnsCLib::artui_getClassifiedStatus (art_cl_status *status)
{
   krui_err        ret_code           = KRERR_NO_ERROR;

   *status = ART_NO_CLASSIFICATION;

   if (krui_getNoOfUnits() == 0) {
      ret_code = KRERR_NO_UNITS;
      return (ret_code);
   } /*if*/

   /* Check if network has been initialized or update- or learning function
      has been applied to it. If not, we can not know, which type of ART
      network this is, so we return, that there has been no classification.
   */
   if (NetModified) {
      return (ret_code);
   } /*if*/

   switch (TopoSortID) {
   case ART1_TOPO_TYPE:
      if (ART1_CLASSIFIED) {
         *status = ART_CLASSIFIED;
      } else {
         if (ART1_NOT_CLASSIFIABLE) {
            *status = ART_NOT_CLASSIFIABLE;
         } /*if*/
      } /*if*/
      break;
   case ART2_TOPO_TYPE:
      if (ART2_CLASSIFIED) {
         *status = ART_CLASSIFIED;
      } else {
         if (ART2_NOT_CLASSIFIABLE) {
           *status = ART_NOT_CLASSIFIABLE;
         } /*if*/
      } /*if*/
      break;  //bergmeir 20110606
   case ARTMAP_TOPO_TYPE:
      if (ARTMAP_CLASSIFIED) {
         if (kram_AllMapUnitsActive()) {
            *status = ART_DONT_KNOW;
         } else {
            *status = ART_CLASSIFIED;
         } /*if*/
      } else {
         if (ARTMAP_NOT_CLASSIFIABLE) {
            *status = ART_NOT_CLASSIFIABLE;
         } /*if*/
      } /*if*/
      break;  //bergmeir 20110606
   default:
      break;
   } /* switch */

   return (ret_code);
} /* artui_getClassifiedStatus () */




/* funcname: artui_getClassNo ()
   Purpose : Returns the index of the F2-winner-unit
   in-par  : none
   out-par : class_no :  If unit_no is negative, then no winning unit exists.
   ret-val : krui_err :  Returns an error if no Units defined.
*/
krui_err SnnsCLib::artui_getClassNo (int *class_no)
{
   krui_err       ret_code        = KRERR_NO_ERROR;

   *class_no = -1;

   if (krui_getNoOfUnits () == 0) {
      ret_code = KRERR_NO_UNITS;
      return (ret_code);
   } /*if*/

   /* Check if network has been initialized or update- or learning function
      has been applied to it. If not, we can not know, which type of ART
      network this is, so we return, that there has been no classification.
      If there is a adequate TopoSortID, then we get the no. of the class,
      if there has been a classification.
   */

   if (NetModified) {
      return (ret_code);
   } /*if*/

   switch (TopoSortID) {
   case ART1_TOPO_TYPE:
      if (ART1_CLASSIFIED) {
         *class_no = kra1_getClassNo ();
      } /*if*/
      break;
   case ART2_TOPO_TYPE:
      if (ART2_CLASSIFIED) {
         *class_no = kra2_getClassNo ();
      } /*if*/
      break;  //bergmeir 20110606
   case ARTMAP_TOPO_TYPE:
      if (ARTMAP_CLASSIFIED && (!kram_AllMapUnitsActive())) {
         *class_no = kram_getClassNo ();
      } /*if*/
      break;  //bergmeir 20110606
   default:
      break;
   } /* switch */

   return (ret_code);
} /* artui_getClassNo () */



/* funcname: artui_getN ()
   Purpose : Returns the number of F1-units in an ART1 or ART2 network
   in-par  : none
   out-par : N : number of F1-Units (-1 is returned if not topologically sorted)
   ret-val : krui_err:  Returns an error if no Units defined.
*/
krui_err SnnsCLib::artui_getN (int *N)
{
   krui_err         ret_code = KRERR_NO_ERROR;

   *N = -1;

   if (krui_getNoOfUnits() == 0) {
      ret_code = KRERR_NO_UNITS;
      return (ret_code);
   } /*if*/

   if (NetModified) {
      return (ret_code);
   } /*if*/

   if ((TopoSortID == ART1_TOPO_TYPE) || (TopoSortID == ART2_TOPO_TYPE)) {
      *N = NoOfInputUnits;
   } /*if*/

   return (ret_code);
} /* artui_getN () */


/* funcname: artui_getM ()
   Purpose : Returns the number of F2-units in an ART1 or ART2 network
   in-par  : none
   out-par : M : number of F2-Units (-1 is returned if not topologically sorted)
   ret-val : krui_err:  Returns an error if no Units defined.
*/
krui_err SnnsCLib::artui_getM (int *M)
{
   krui_err         ret_code = KRERR_NO_ERROR;

   *M = -1;

   if (krui_getNoOfUnits() == 0) {
      ret_code = KRERR_NO_UNITS;
      return (ret_code);
   } /*if*/

   if (NetModified) {
      return (ret_code);
   } /*if*/

   switch (TopoSortID) {
   case ART1_TOPO_TYPE:
      *M = Art1_NoOfRecUnits;
      break;
   case ART2_TOPO_TYPE:
      *M = Art2_NoOfRecUnits;
      break;
   } /*switch*/

   return (ret_code);
} /* artui_getM () */



/* funcname: artui_getNa ()
   Purpose : Returns the number of F1a-units in an ARTMAP network
   in-par  : none
   out-par : Na: number of F1a-Units (-1 is returned if not topologically sorted)
   ret-val : krui_err:  Returns an error if no Units defined.
*/
krui_err SnnsCLib::artui_getNa (int *Na)
{
   krui_err         ret_code = KRERR_NO_ERROR;

   *Na = -1;

   if (krui_getNoOfUnits() == 0) {
      ret_code = KRERR_NO_UNITS;
      return (ret_code);
   } /*if*/

   if (NetModified) {
      return (ret_code);
   } /*if*/

   if (TopoSortID == ARTMAP_TOPO_TYPE) {
      *Na = ArtMap_NoOfInpUnits_a;
   } /*if*/

   return (ret_code);
} /* artui_getNa () */




/* funcname: artui_getNb ()
   Purpose : Returns the number of F1b-units in an ARTMAP network
   in-par  : none
   out-par : Nb: number of F1b-Units (-1 is returned if not topologically sorted)
   ret-val : krui_err:  Returns an error if no Units defined.
*/
krui_err SnnsCLib::artui_getNb (int *Nb)
{
   krui_err         ret_code = KRERR_NO_ERROR;

   *Nb = -1;

   if (krui_getNoOfUnits() == 0) {
      ret_code = KRERR_NO_UNITS;
      return (ret_code);
   } /*if*/

   if (NetModified) {
      return (ret_code);
   } /*if*/

   if (TopoSortID == ARTMAP_TOPO_TYPE) {
      *Nb = ArtMap_NoOfInpUnits_b;
   } /*if*/

   return (ret_code);
} /* artui_getNb () */




/* funcname: artui_getMa ()
   Purpose : Returns the number of F2a-units in an ARTMAP network
   in-par  : none
   out-par : Ma: number of F2a-Units (-1 is returned if not topologically sorted)
   ret-val : krui_err:  Returns an error if no Units defined.
*/
krui_err SnnsCLib::artui_getMa (int *Ma)
{
   krui_err         ret_code = KRERR_NO_ERROR;

   *Ma = -1;

   if (krui_getNoOfUnits() == 0) {
      ret_code = KRERR_NO_UNITS;
      return (ret_code);
   } /*if*/

   if (NetModified) {
      return (ret_code);
   } /*if*/

   if (TopoSortID == ARTMAP_TOPO_TYPE) {
      *Ma = ArtMap_NoOfRecUnits_a;
   } /*if*/

   return (ret_code);
} /* artui_getMa () */



/* funcname: artui_getMb ()
   Purpose : Returns the number of F2b-units in an ARTMAP network
   in-par  : none
   out-par : Mb: number of F2b-Units (-1 is returned if not topologically sorted)
   ret-val : krui_err:  Returns an error if no Units defined.
*/
krui_err SnnsCLib::artui_getMb (int *Mb)
{
   krui_err         ret_code = KRERR_NO_ERROR;

   *Mb = -1;

   if (krui_getNoOfUnits() == 0) {
      ret_code = KRERR_NO_UNITS;
      return (ret_code);
   } /*if*/

   if (NetModified) {
      return (ret_code);
   } /*if*/

   if (TopoSortID == ARTMAP_TOPO_TYPE) {
      *Mb = ArtMap_NoOfRecUnits_b;
   } /*if*/

   return (ret_code);
} /* artui_getMb () */

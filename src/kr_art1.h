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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_art1.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Function Prototypes for ART1-Networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann 
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:35 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _KR_ART1_DEFINED_
#define  _KR_ART1_DEFINED_

/* begin global definition section */

 int             Art1_NoOfRecUnits;

 struct Unit     *Art1_cl_unit; /* Pointer to ART1 cl-unit */
 struct Unit     *Art1_nc_unit; /* Pointer to ART1 nc-unit */



/***************************************************************************/
/* kra1_sort ()

   Set logical layer numbers and logical unit numbers in an ART1 network.
   Also this function checks, whether the network is an ART1 network or not.
   Returns an error code, when actual network is no ART1 architecture.
*/
 krui_err  kra1_sort (

                            void

                           );

/***************************************************************************/
/* kra1_init_i_act ()

   Sets the initial activation values of the units
*/
 krui_err  kra1_init_i_act (

                                  double rho

                                 );


/***************************************************************************/
/* kra1_getClassNo ()

   Returns the number of the actually activated class J, 1 <= J <= M
*/
 int  kra1_getClassNo (

                             void

                            );



/* end global definition section */

/* begin private definition section */

TopoPtrArray      Art1_del_layer;   /* Points to first pointer
                                                     within delay-layer
                                                  */


/**************** functions, local to this module ******************************/

int                kra1_get_NoOfRecUnits (

                                                 void

                                                );

krui_err           kra1_get_InpUnits (

                                             TopoPtrArray *topo_ptr

                                            );

krui_err           kra1_get_CmpUnits (

                                             TopoPtrArray *topo_ptr,
                                             int *no_of_cmp_units

                                            );

krui_err           kra1_get_RecUnits (

                                             TopoPtrArray *topo_ptr

                                            );

krui_err           kra1_get_DelUnits (

                                             TopoPtrArray *topo_ptr,
                                             int *no_of_del_units

                                            );

krui_err           kra1_get_RstUnits (

                                             TopoPtrArray *topo_ptr,
                                             int *no_of_rst_units

                                            );

krui_err           kra1_get_SpecUnits (

                                              TopoPtrArray *topo_ptr,
                                              int *no_of_spec_units

                                             );

krui_err           kra1_TopoPtrArray (

                                             void

                                            );

krui_err           kra1_Sites (

                                      void

                                     );

krui_err           kra1_LinksToInpUnits (

                                                TopoPtrArray *topo_inp_ptr

                                               );

krui_err           kra1_LinksToCmpUnits (

                                                TopoPtrArray *topo_cmp_ptr

                                               );

krui_err           kra1_LinksToRecUnits (

                                                TopoPtrArray *topo_rec_ptr

                                               );

krui_err           kra1_LinksToDelUnits (

                                                TopoPtrArray *topo_del_ptr

                                               );

krui_err           kra1_LinksToRstUnits (

                                                TopoPtrArray *topo_rst_ptr

                                               );

krui_err           kra1_LinksToSpecUnits (

                                                 TopoPtrArray *topo_spec_ptr

                                                );

void               kra1_set_fix_weight (

                                               struct Unit *src_unit,
                                               struct Unit *trgt_unit,
                                               FlintType   *weight

                                              );

krui_err           kra1_init_fix_weights (

                                                 void

                                                );


/* end private definition section */

 
#endif

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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_amap.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Function Prototypes for ARTMAP-Networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:29 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _KR_AMAP_DEFINED_
#define  _KR_AMAP_DEFINED_

/* begin global definition section */



 int             ArtMap_NoOfInpUnits_a;
 int             ArtMap_NoOfInpUnits_b;
 int             ArtMap_NoOfRecUnits_a;
 int             ArtMap_NoOfRecUnits_b;

 struct Unit    *ArtMap_cl_unit;  /* Pointer to cl-unit */
 struct Unit    *ArtMap_nc_unit;  /* Pointer to nc-unit */




/***************************************************************************/
/* kram_init_i_act ()

   Sets the initial activation values of the units
*/
 krui_err kram_init_i_act (

                                 double rho_a,
                                 double rho_b,
                                 double rho

                                );


/***************************************************************************/
/* kram_sort ()

   Check for ARTMAP topology and create topo ptr array
*/
 krui_err  kram_sort (

                            void

                           );




/***************************************************************************/
/* kram_getClassNo ()

   Returns the number of the actually activated class K, 1 <= K <= Mb
*/
 int  kram_getClassNo (

                             void

                            );



/***************************************************************************/
/* kram_AllMapUnitsActive ()

   Returns TRUE, if all MAP-Field-Units are active, else, FALSE
*/
 bool  kram_AllMapUnitsActive (

                                     void

                                    );


/* end global definition section */

/* begin private definition section */

TopoPtrArray      ArtMap_map_layer;   /* Points to first pointer
                                                       within map-layer
                                                    */




krui_err  kram_get_InpUnits_a (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );

krui_err  kram_get_CmpUnits_a (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );

krui_err  kram_get_RecUnits_a (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


krui_err  kram_get_DelUnits_a (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


krui_err  kram_get_RstUnits_a (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


krui_err  kram_get_SpecUnits_a (

                                       TopoPtrArray      *topo_ptr,
                                       int               *no_of_units

                                      );


krui_err  kram_get_InpUnits_b (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );

krui_err  kram_get_CmpUnits_b (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );

krui_err  kram_get_RecUnits_b (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


krui_err  kram_get_DelUnits_b (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


krui_err  kram_get_RstUnits_b (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


krui_err  kram_get_SpecUnits_b (

                                       TopoPtrArray      *topo_ptr,
                                       int               *no_of_units

                                      );



krui_err  kram_get_MapUnits (

                                    TopoPtrArray      *topo_ptr,
                                    int               *no_of_units

                                   );


krui_err  kram_get_SpecUnits (

                                     TopoPtrArray      *topo_ptr,
                                     int               *no_of_units

                                    );


krui_err  kram_TopoPtrArray (

                                    void

                                   );


krui_err  kram_Sites (

                             void

                            );


krui_err  kram_LinksToInpUnits_a (

                                         TopoPtrArray *topo_ptr

                                        );


krui_err  kram_LinksToCmpUnits_a (

                                         TopoPtrArray *topo_ptr

                                        );


krui_err  kram_LinksToRecUnits_a (

                                         TopoPtrArray *topo_ptr

                                        );


krui_err  kram_LinksToDelUnits_a (

                                         TopoPtrArray *topo_ptr

                                        );


krui_err  kram_LinksToRstUnits_a (

                                         TopoPtrArray *topo_ptr

                                        );


krui_err  kram_LinksToSpecUnits_a (

                                          TopoPtrArray *topo_ptr

                                         );


krui_err  kram_LinksToInpUnits_b (

                                         TopoPtrArray *topo_ptr

                                        );


krui_err  kram_LinksToCmpUnits_b (

                                         TopoPtrArray *topo_ptr

                                        );


krui_err  kram_LinksToRecUnits_b (

                                         TopoPtrArray *topo_ptr

                                        );


krui_err  kram_LinksToDelUnits_b (

                                         TopoPtrArray *topo_ptr

                                        );


krui_err  kram_LinksToRstUnits_b (

                                         TopoPtrArray *topo_ptr

                                        );


krui_err  kram_LinksToSpecUnits_b (

                                          TopoPtrArray *topo_ptr

                                         );


krui_err  kram_LinksToMapUnits (

                                       TopoPtrArray *topo_ptr

                                      );


krui_err  kram_LinksToSpecUnits (

                                        TopoPtrArray *topo_ptr

                                       );


krui_err  kram_init_fix_weights (

                                        void

                                       );


void  kram_set_fix_weight (

                                  struct Unit *src_unit,
                                  struct Unit *trgt_unit,
                                  FlintType   *weight

                                 );




/* end private definition section */

#endif 

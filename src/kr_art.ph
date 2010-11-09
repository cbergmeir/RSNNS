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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_art.ph,v $
  SHORTNAME      : kr_art.c
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Function Prototypes for ART-Networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _KR_ART_DEFINED_
#define  _KR_ART_DEFINED_

/* begin global definition section */

/***************************************************************************/
/* krart_prop_synch ()

   synchronous propagation (1 cycle) i.e. each unit puts its output onto
   its outgoing links and calculates its new activation.
*/
 void  krart_prop_synch (void);

/***************************************************************************/
/* krart_get_winner ()

   get the winner of the last propagation cycle. returns a pointer to
   the winning recognition unit.
*/
 struct Unit  *krart_get_winner (TopoPtrArray wta_layer,FlintType winner_output);

/***************************************************************************/
/* krart_reset_activations

   reset activation values of all non input units to their i_act value.
*/
 krui_err   krart_reset_activations (void);


/***************************************************************************/
/* krart_deleteTouchFlags

   delete touch-flags of all units in unit-array
*/
 void               krart_deleteTouchFlags (void);

/***************************************************************************/
/* krart_init_sorting

   initialize unit-array in order to prepare it for sorting routine
*/
 void               krart_init_sorting (void);

/***************************************************************************/
/* krart_get_NoOfInputUnits

   returns no of input units of the network
*/
 int                krart_get_NoOfInputUnits (void);

/***************************************************************************/
/* krart_check_undeterminedUnits

   checks network for undetermined units. if there are any, returns true
   else false.
*/
 bool               krart_check_undeterminedUnits (void);

/***************************************************************************/
/* krart_save_inp_pat()

   copies activation values of input units to value_a field of unit-struct.
*/
 void krart_save_inp_pat (TopoPtrArray topo_inp_ptr);




/***************************************************************************/
/* krart_inp_pat_changed ()

   returns TRUE, if one or more activation values of input units
   differ from value_a i.e. their value changed.
*/
 bool krart_inp_pat_changed (TopoPtrArray topo_inp_ptr);


/* end global definition section */

/* begin private definition section */

/* end private definition section */

#endif 





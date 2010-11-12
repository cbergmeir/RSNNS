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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/update_f.ph,v $
  SHORTNAME      : update_f
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Network Update Functions
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 18.03.91

  CHANGED BY     : Sven Doering, Michael Vogt (Martin Reczko)
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:58 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _UPDATE_F_DEFINED_
#define  _UPDATE_F_DEFINED_

/* begin global definition section */

krui_err  UPDATE_syncPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_syncPropagateHop(float *parameterArray, int NoOfParams);
krui_err  UPDATE_FixAct_Hop(float *parameterArray, int NoOfParams);
krui_err  UPDATE_serialPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_randomOrderPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_randomPermutPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_topologicalPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_CPNPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_TimeDelayPropagate(float parameterArray[], int NoOfParams );
krui_err  UPDATE_ART1_syncPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_ART1_Propagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_ART2_syncPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_ART2_Propagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_ARTMAP_syncPropagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_ARTMAP_Propagate(float *parameterArray, int NoOfParams);
krui_err  UPDATE_CC_Propagate(float parameterArray[],  int NoOfParams);
krui_err  UPDATE_BPTT(float *parameterArray, int NoOfParams);
krui_err  UPDATE_DLVQ_Propagate(float parameterArray[], int NoOfParams);
krui_err  UPDATE_BAM(float *parameterArray, int NoOfParams);
krui_err  UPDATE_JE_Propagate (float *parameterArray, int NoOfParams) ;
krui_err  UPDATE_JE_Special   (float *parameterArray, int NoOfParams) ;
krui_err  UPDATE_RM_Propagate(float *parameterArray, int NoOfParams);
krui_err  ENZO_propagate(float *parameterArray, int NoOfParams);

krui_err  UPDATE_KohonenPropagate(float *parameterArray, int NoOfParams);

/* end global definition section */

/* begin private definition section */

#define  UPDATE_PARAM1( param )   param[ 0 ]  /*    contains the 1st initialisation parameter  */
#define  UPDATE_PARAM2( param )   param[ 1 ]  /*    contains the 2nd initialisation parameter  */
#define  UPDATE_PARAM3( param )   param[ 2 ]  /*    contains the 3rd initialisation parameter  */
#define  UPDATE_PARAM4( param )   param[ 3 ]  /*    contains the 4th initialisation parameter  */
 void update_f_normalize_inputvector(float sum);
 krui_err ENZO_PROPAGATE_error_back(void);
 krui_err ENZO_PROPAGATE_ff(void);

/* end private definition section */

#endif 

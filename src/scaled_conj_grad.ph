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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/scaled_conj_grad.ph,v $
  SHORTNAME      : scaled_conj_grad.ph
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Learning Functions
  NOTES          : with following learning functions:
	           - scaled conjugated gradient

  AUTHOR         : Bruno Orsier
  DATE           : 01.10.96

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.4 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:39 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _SCALED_CONJ_GRAD_DEFINED_
#define _SCALED_CONJ_GRAD_DEFINED_

/* begin global definition section */
krui_err LEARN_SCG(int start_pattern, int end_pattern, 
		   float *parameterInArray, int NoOfInParams, 
		   float **parameterOutArray, int *NoOfOutParams) ;

krui_err TEST_SCG(int start_pattern, int end_pattern,
		  float *parameterInArray, int NoOfInParams,
		  float **parameterOutArray, int *NoOfOutParams);
/* end global definition section */

/* begin private definition section */
 float product_of_xt_by_y(float * x, float * y, int tab_size) ;
 float square_of_norm(float * x, int tab_size) ;
 krui_err compute_gradient(int start_pattern, int end_pattern, 
				 float delta_max, float * error) ;

 int        scg_space_size ;
 FlintType* *scg_gradient ;

#define SCG_FIRST_SIGMA     1E-4	/* should be in ]0 ; 1E-4] */
#define SCG_FIRST_LAMBDA    1E-6        /* should be in ]0 ; 1E-6] */

#define  LEARN_PARAM1( param )   param[ 0 ]  /* contains 1st learning param */
#define  LEARN_PARAM2( param )   param[ 1 ]  /* contains 2nd learning param */
#define  LEARN_PARAM3( param )   param[ 2 ]  /* contains 3rd learning param */
#define  LEARN_PARAM4( param )   param[ 3 ]  /* contains 4th learning param */


#define SCG_TOLERANCE 1E-8		/* for single precision */
				/* should be 1E-16 for double precision */
/* end private definition section */

#endif

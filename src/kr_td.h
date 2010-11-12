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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_td.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Time delay learning and propagation functions
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 20.02.93

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:09 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _KR_TD_DEFINED_
#define  _KR_TD_DEFINED_

/* begin global definition section */

void      propagateTDNetForward(int pattern_no, int sub_pat_no);

krui_err  LEARN_TDbackprop(int start_pattern, int end_pattern, 
			   float parameterInArray[], int NoOfInParams,
			   float * *parameterOutArray, int *NoOfOutParams);

krui_err  LEARN_TDBP_McClelland(int start_pattern, int end_pattern, 
				float parameterInArray[], int NoOfInParams,
				float * *parameterOutArray, int *NoOfOutParams);
 
krui_err TEST_TDbackprop(int start_pattern, int end_pattern, 
				 float parameterInArray[], int NoOfInParams,
				 float * *parameterOutArray, 
				 int *NoOfOutParams );

/* end global definition section */


/* begin private definition section */

#define  NET_ERROR( param )    param[ 0 ] /* returns the net error   */
#define  LEARN_PARAM1( param ) param[ 0 ] /* contains the 1st learning param.*/
#define  LEARN_PARAM2( param ) param[ 1 ] /* contains the 2nd learning param.*/
#define  LEARN_PARAM3( param ) param[ 2 ] /* contains the 3rd learning param.*/
#define  LEARN_PARAM4( param ) param[ 3 ] /* contains the 4th learning param.*/
#define  LEARN_PARAM5( param ) param[ 4 ] /* contains the 5th learning param.*/

 krui_err initializeTDBackprop(void);

 float propagateTDNetBackward(int pattern_no, int sub_pat_no, 
				    float learn_parameter, float delta_max );

 float propagateTDNetBackMcClelland(int pattern_no, int sub_pat_no,
					   float learn_parameter, 
					   float delta_max );

 float testTDNetBackward(int pattern_no, int sub_pat_no, 
				    float learn_parameter, float delta_max );

/* end private definition section */

#endif 

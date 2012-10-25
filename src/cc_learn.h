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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_learn.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Michael Schmalzl, Christian Wehrfritz (PCC)
  DATE           : 5.2.1993

  CHANGED BY     : Michael Schmalzl
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:25:58 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _CC_LEARN_DEFINED_
#define  _CC_LEARN_DEFINED_

/* begin global definition section */
krui_err LEARN_CC(int StartPattern, int EndPattern,
                      float *ParameterInArray, int NoOfInParams,
                      float **ParameterOutArray, int *NoOfOutParams);
krui_err LEARN_CasCor(int StartPattern, int EndPattern,
                      float *ParameterInArray, int NoOfInParams,
                      float **ParameterOutArray, int *NoOfOutParams);

krui_err TEST_CasCor(int start_pattern, int end_pattern,
			float *parameterInArray, int NoOfInParams,
			float **parameterOutArray, int *NoOfOutParams);

/* end   global definition section */

/* begin privat definition section */
float cc_calculateCorrelation(int StartPattern,int EndPattern,int counter);
 krui_err cc_GetTrainFunctions(int learnFunc);

 void cc_calculateSpecialUnitActivation(int startPattern,int endPattern);
 krui_err cc_generateSpecialUnits(int type);
 int  cc_test(int StartPattern,int EndPattern,float maxPixelError); 

 void  cc_calculateOutputUnitError(int StartPattern,int EndPattern);
 void  cc_trainSpecialUnits(int maxNoOfCovarianceUpdateCycles, 
			    float minCovarianceChange,
                            int specialPatience,int StartPattern, 
			    int EndPattern,
                            float param1, float param2, float param3, 
			    int MaxSpecialUnitNo);
void  cc_trainOutputUnits(int maxNoOfErrorUpdateCycles,int backfittPatience,
			     float minErrorChange, 
                             int outPatience, int StartPattern,int EndPattern,
                             float param1, float param2,float param3,
                             float **ParameterOutArray,int *NoOfOutParams);

krui_err cc_propagateSpecial(int start,int end,int n,
					 int counter,float param1, float param2, 
                                          float param3);
 float cc_propagateOutput(int PatternNo, int sub_pat_no,float param1,float param2,float param3);
krui_err cc_propagateSpecialOnlineCase(int start,int end,int n,
					 int counter,float param1, float param2, 
                                          float param3);
 float cc_propagateOutputOnlineCase(int PatternNo, int sub_pat_no,float param1,float param2,float param3);

 krui_err cc_generateHiddenUnit(int GroupNo);

 krui_err cc_initVariables(float* ParameterInArray,
                                int StartPattern,int EndPattern);
 krui_err cc_generateLayerList(void);

 krui_err cc_actualizeLayerlist(struct Unit* UnitPtr,int LayerNo);



/* end privat definition section */


#endif /* _CC_LEARN_DEFINED_ */

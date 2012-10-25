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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_glob.ph,v $
  SHORTNAME      : cc_glob.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          : This file was put together from the earlier files cc_rcc 
                   and cc_rcc_topo

  AUTHOR         : Michael Schmalzl
  DATE           : 5.2.92

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.5 $
  LAST CHANGE    : $Date: 1998/02/25 15:25:53 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _CC_GLOB_DEFINED_
#define  _CC_GLOB_DEFINED_

/* begin global definition section */
void      cc_printHeadline(char* s,int Length);
float     cc_getErr (int StartPattern, int EndPattern);
void      cc_LayerCorrectnessTest(float* ParameterInArray,
				  int StartPattern, int EndPattern);
krui_err  cc_freeStorage(int StartPattern, int EndPattern, int flag);
krui_err  cc_deleteAllSpecialAndAllHiddenUnits(void);
krui_err  cc_allocateStorage(int StartPattern, int EndPattern, 
			     int NoOfSpecialUnits);
FlintType cc_generateRandomNo(float maxValue);
void      cc_initActivationArrays(void);
void      cc_getActivationsForActualPattern(int SubPatterNo,int First,int* pat,
					    int* sub);
void      cc_initOutputUnits(void);
krui_err  cc_getPatternParameter(int StartPattern, int Endpattern,
				 int* start, int* end,int* n);
void      cc_initInputUnitsWithPattern(int PatternNo);
krui_err  cc_setPointers(void);
krui_err  cc_initSpecialUnitLinks(void);
krui_err  cc_deleteAllSpecialUnits(void);   

krui_err  cc_topoSort(int topoSortId);
void      cc_setCycletestFlag(struct Unit* UnitPtr);

float QuickPropOfflinePart(float oldValue, float* previousSlope,
			   float* currentSlope,float* LastChange,
			   float epsilon, float mu, float decay);
float BackPropOfflinePart(float oldValue, float* previousSlope,
			  float* currentSlope, float* LastChange,
			  float dummy1, float dummy2, float dummy3);
float OfflineBackPropOfflinePart(float oldValue, float* previousSlope,
				 float* currentSlope, float* LastChange,
				 float epsilon, float mu, float dummy);
float RPropOfflinePart(float oldValue, float* previousSlope, 
		       float* currentSlope, float* LastChange,
		       float epsilonMinus, float epsilonPlus, float dummy);
float OnlineBackPropOfflinePart(float oldValue, float* previousSlope,
				float* currentSlope, float* LastChange,
				float eta, float mu, float dummy);

static const char* err_message[];
static const char* ext_messages[];
static const int NoOfMessages;

struct CC_DATA  cc_data;


int cc_end;
int cc_storageFree;
int cc_allButtonIsPressed;


struct Unit  **FirstInputUnitPtr,**FirstHiddenUnitPtr,
             **FirstOutputUnitPtr,**FirstSpecialUnitPtr,
              *bestSpecialUnitPtr;

float **OutputUnitError;
float **SpecialUnitAct;
float **CorBetweenSpecialActAndOutError;
float *MeanOutputUnitError;
float *SpecialUnitSumAct;
int   LastInsertedHiddenUnit;

float SumSqError;

int cc_printOnOff;
int cc_backfittingOnOff;
int cc_MaxSpecialUnitNo;
int cc_modification;
int cc_fastmode;
float cc_Parameter[5];


int *reset;
int cc_cascade;
int NoOfLayers;             /* number hidden layers */
Layer_Type* ListOfLayers;  /* data of layers */
int SizeOfLayerlist;  


int cc_outputXMax;

int     cc_actualNetSaved;
float** ActOfUnit;
int     NetLearnAlgorithm;

float cc_fse;
int cc_learningFunction;

int prot_fd;
FILE* prot_stream;
char DumpText[1024];
bool cc_NetErgProtocolled;
int CountSpecTrains,CountOutTrains,CountLinks;

void Prot();
void prot(char* s);
void Prot_Parameters(float* Params,int NetIsCC);
int filedesc;


/* the following functions are dummy declarations without implemented body!!
   They are realized by directing their pointers to other functions */

krui_err (SnnsCLib::*cc_propagateSpecialUnitsBackward)(int start, int end, int n,
					     int counter, float param1,
					     float param2, float param3);
float (SnnsCLib::*cc_propagateOutputUnitsBackward)(int PatternNo, int sub_pat_no,
					 float param1, float param2,
					 float param3);
float (SnnsCLib::*cc_SpecialUnitUpdate)(float oldValue, float* previousSlope,
			      float* currentSlope, float* LastChange,
			      float param1, float param2, float param3);
float (SnnsCLib::*cc_OutputUnitUpdate)(float oldValue, float* previousSlope,
			     float* currentSlope, float* LastChange,
			     float param1, float param2, float param3);

/* end global definition section */

/* begin privat definition section */

int OldNoOfSpecialUnitStorage;

#define LINKS_LEAVING(unitPtr)   unitPtr->value_a
#define LINKS_ARRIVEING(unitPtr) unitPtr->value_b
#define INPUT_LINKS(unitPtr)     unitPtr->value_c

void  cc_clearFlags(void);
void  DepthFirst4(struct Unit *unitPtr, int depth);
void  DepthFirst5(struct Unit *unitPtr, int depth);
krui_err     cc_topoSortMain(int topoSortID);

TopoPtrArray	global_topo_ptr;

/* end  privat definition section */

bool cc_testCycletestFlag(struct Unit* UnitPtr);
void cc_clearAllCycletestFlags(void);

#endif /* _CC_GLOB_DEFINED_ */








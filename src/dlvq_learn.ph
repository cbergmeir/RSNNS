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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/dlvq_learn.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Michael Schmalzl 
  DATE           : 5.2.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:08 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

/* begin global definition section */

krui_err getNoOfClasses(int startPattern, int endPattern);

krui_err LEARN_DLVQ(int startPattern, int endPattern, float *ParameterInArray,
		    int NoOfInParams, float **ParameterOutArray, 
		    int *NoOfOutParams);

void normPatterns(int startPattern, int endPattern);

void allocInitialUnitArray(void);

void initInitialUnitArray(int startPattern, int endPattern);

void allocArrays(void);

krui_err dlvq_setPointers(void);

void generateMissingClassHiddenUnits(int *generatedNewUnit);

int newPatternsLoaded;
int dlvq_numberOfLearnCycles;

/* end global definition section */

/* begin privat definition section */

struct MIX_UP  **mixupArray;
struct MIX_UP  *initialUnitArray;
int *lastInsertedUnitArray;

int noOfClasses;
int oldNoOfClasses;
int wrongClassCounter;
int continueLearning;

void printMixupArray(int cycle);

void allocMixupArray(void);

void allocLastInsertedUnitArray(void);

void normReferenceVec(struct Unit *hiddenUnitPtr);

void moveVec(struct Unit *correctReferenceVec, float learnParam1,
                     struct Unit *wrongReferenceVec,  float learnParam2);

void writeVectorToMixupArray(int correctClass, int wrongClass, 
				    int patternNo, int sub_pat_no);

void initFirstUnit(struct Unit *hiddenUnitPtr, int dlvq_learn_class);

krui_err insertFirstUnit(struct Unit **hiddenUnitPtr);

krui_err insertNewUnits(void);

void initMixupArray(void);

void dlvq_trainNet(int noOfTrainingCycles, int startPattern, 
			  int endPattern, float learnParam1, 
			  float learnParam2);

void generateTmpTopoPtrArray(void);

void calculateUnitXYPos(void);

void initLastInsertedUnitArray(void);

void sortHiddenUnitsByClasses(int left, int right);

/* begin privat definition section */

krui_err deleteAllLinksOfTheOutputUnit(void);
void freeTmpTopoPtrArray(void);

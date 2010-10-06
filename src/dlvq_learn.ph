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

public:

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

private:

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

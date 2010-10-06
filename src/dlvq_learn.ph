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

int newPatternsLoaded = 0;
int dlvq_numberOfLearnCycles=0;

/* end global definition section */

/* begin privat definition section */

static struct MIX_UP  **mixupArray;
static struct MIX_UP  *initialUnitArray;
static int *lastInsertedUnitArray;

static int noOfClasses=0;
static int oldNoOfClasses=0;
static int wrongClassCounter=0;
static int continueLearning=0;

static void printMixupArray(int cycle);

static void allocMixupArray(void);

static void allocLastInsertedUnitArray(void);

static void normReferenceVec(struct Unit *hiddenUnitPtr);

static void moveVec(struct Unit *correctReferenceVec, float learnParam1,
                     struct Unit *wrongReferenceVec,  float learnParam2);

static void writeVectorToMixupArray(int correctClass, int wrongClass, 
				    int patternNo, int sub_pat_no);

static void initFirstUnit(struct Unit *hiddenUnitPtr, int class);

static krui_err insertFirstUnit(struct Unit **hiddenUnitPtr);

static krui_err insertNewUnits(void);

static void initMixupArray(void);

static void dlvq_trainNet(int noOfTrainingCycles, int startPattern, 
			  int endPattern, float learnParam1, 
			  float learnParam2);

static void generateTmpTopoPtrArray(void);

static void calculateUnitXYPos(void);

static void initLastInsertedUnitArray(void);

static void sortHiddenUnitsByClasses(int left, int right);

/* begin privat definition section */

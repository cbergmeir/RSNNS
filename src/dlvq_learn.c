/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/dlvq_learn.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Functions of DLVQ
  NOTES          :

  AUTHOR         : Michael Schmalzl 
  DATE           : 5.2.93

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.12 $
  LAST CHANGE    : $Date: 1998/03/13 16:23:54 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <memory.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#include "kr_typ.h"      /*  Kernel Types and Constants  */
#include "kr_const.h"    /*  Constant Declarators for SNNS-Kernel  */
#include "kr_def.h"      /*  Default Values  */
#include "kernel.h"      /*  kernel function prototypes  */
#include "kr_ui.h"
#include "kr_mac.h"      /*  Kernel Macros   */

#include "kr_ui.h"
#include "cc_mac.h"
#include "kr_newpattern.h"
#include "cc_glob.h"
#include "dlvq_type.h"
#include "dlvq_learn.ph"


/*****************************************************************************
  FUNCTION : sortHiddenUnitsByClasses

  PURPOSE  : Sorts the hidden units by its class.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void sortHiddenUnitsByClasses(int left, int right)
{
    int i,last;
    struct Unit *temp;
 
    if(left >= right ){
	return;
    }
    temp = FirstHiddenUnitPtr[left]; 
    FirstHiddenUnitPtr[left] = FirstHiddenUnitPtr[(left+right)/2]; 
    FirstHiddenUnitPtr[(left+right)/2] = temp;  
    last = left; 
    for(i=left+1;i<=right;i++){
	if(FirstHiddenUnitPtr[i]->bias < FirstHiddenUnitPtr[left]->bias) {
	    temp = FirstHiddenUnitPtr[++last]; 
	    FirstHiddenUnitPtr[last]=FirstHiddenUnitPtr[i]; 
	    FirstHiddenUnitPtr[i]=temp;
	}
    }
    temp = FirstHiddenUnitPtr[left]; 
    FirstHiddenUnitPtr[left]=FirstHiddenUnitPtr[last]; 
    FirstHiddenUnitPtr[last] = temp;
    sortHiddenUnitsByClasses(left,last);
    sortHiddenUnitsByClasses(last+1,right);
}



/*****************************************************************************
  FUNCTION : getNoOfClasses

  PURPOSE  : Looks how many different classes exist and checks whether the 
             no. of the first class is zero.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err getNoOfClasses(int startPattern, int endPattern)
{
    int p,patternClass=0,maxPatternClass=0,minPatternClass=0;
    int *tmpArray,counter,class;
    int start, end;
    int pat, sub_pat;
    Patterns out_pat;

    KernelErrorCode = kr_initSubPatternOrder(startPattern, endPattern);
    if (KernelErrorCode != KRERR_NO_ERROR)
	return KernelErrorCode;
    start = kr_AbsPosOfFirstSubPat(startPattern);
    end   = kr_AbsPosOfFirstSubPat(endPattern);
    end  += kr_NoOfSubPatPairs(endPattern) - 1;
    for(p=start; p<=end;p++){

	kr_getSubPatternByNo(&pat, &sub_pat, p);
	out_pat = kr_getSubPatData(pat, sub_pat, OUTPUT, NULL);

	patternClass = *out_pat;
	if(patternClass < minPatternClass){
	    minPatternClass = patternClass;
	} 
	if(patternClass > maxPatternClass){
	    maxPatternClass = patternClass;
	} 
    }

    if(minPatternClass != 0){
	return(DLVQ_ERROR2); /* There exists a class smaller than 0 */
    } 

    counter = 0;
    p = start;
    tmpArray =  (int *)calloc(maxPatternClass+1,sizeof(int));

    while((counter != (maxPatternClass+1)) && (p <= end)){
	kr_getSubPatternByNo(&pat, &sub_pat, p++);
	out_pat = kr_getSubPatData(pat, sub_pat, OUTPUT, NULL);
	class = *out_pat;
	if(tmpArray[class] == 0){
	    counter++;
	    tmpArray[class] = 1;
	}
    }
    free(tmpArray);
    
    if(counter != (maxPatternClass+1)){
	return(DLVQ_ERROR1); /*  There are empty classes */
    }
    noOfClasses = maxPatternClass+1;
    return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : allocMixupArray

  PURPOSE  : Allocate the storage of the "mixupArray".
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void allocMixupArray(void)
{
    int i,j;

    /* free storage */
    for(i=0;i<oldNoOfClasses;i++) {
	for(j=0;j<oldNoOfClasses;j++) {
	    free(mixupArray[i][j].link);
	}
    }

    for(i=0;i<oldNoOfClasses;i++) {
	free(mixupArray[i]);
    }
 
    if(mixupArray != NULL) {
	free(mixupArray);
	mixupArray = NULL;
    }
 
    /* alloc storage */
    mixupArray = (struct MIX_UP **)calloc(noOfClasses,sizeof(struct MIX_UP *));

    for(i=0;i<noOfClasses;i++) {
	mixupArray[i] = 
	    (struct MIX_UP *)calloc(noOfClasses,sizeof(struct MIX_UP));   
    }

    for(i=0;i<noOfClasses;i++) {
	for(j=0;j<noOfClasses;j++) {
	    mixupArray[i][j].link = 
  	        (double *)calloc(NoOfInputUnits,sizeof(double));
	}
    }
}



/*****************************************************************************
  FUNCTION : allocLastInsertedUnitArray

  PURPOSE  : Allocate the storage of the array "lastInsertedUnitArray".
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void allocLastInsertedUnitArray(void)
{

    /* free storage */
    if(lastInsertedUnitArray != NULL){
	free(lastInsertedUnitArray);
	lastInsertedUnitArray = NULL;
    }

    /* alloc storage */
    lastInsertedUnitArray = (int *)calloc(noOfClasses,sizeof(int));
}



/*****************************************************************************
  FUNCTION : allocInitialUnitArray

  PURPOSE  : Allocate the storage of the array "initialUnitArray"
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
void allocInitialUnitArray(void)
{
    int i;

    /* free storage */
    for(i=0;i<oldNoOfClasses;i++) {
	free(initialUnitArray[i].link);
    }

    if(initialUnitArray != NULL) {
	free(initialUnitArray);
	initialUnitArray = NULL;
    }
 
    /* alloc storage */
    initialUnitArray = 
	(struct MIX_UP *)calloc(noOfClasses,sizeof(struct MIX_UP));

    for(i=0;i<noOfClasses;i++) {
	initialUnitArray[i].link = 
	    (double *)calloc(NoOfInputUnits,sizeof(double));
    }
}



/*****************************************************************************
  FUNCTION : normPatterns

  PURPOSE  : Norm all patterns.
  NOTES    : This function is of no permanent effect since the manipulation 
  of  the input pattern takes place on a copy of the original data.
  Patterns must be normalized before use of DLVQ !!!!!
  Therefore the body of the function is commented out. The original code is 
  left in place to demonstrate the necessary operation. (M. Vogt)

  UPDATE   : 5.2.93
******************************************************************************/
void normPatterns(int startPattern, int endPattern)
{

/* see NOTES of funtion header */

/*
    double sum,sqrtSum;
    Patterns in_pat,in_patStorage;
    int p,i;
    int start, end;
    int pat, sub;
    int count;

    KernelErrorCode = kr_initSubPatternOrder(startPattern,endPattern);
    start = kr_AbsPosOfFirstSubPat(startPattern);
    end   = kr_AbsPosOfFirstSubPat(endPattern);
    end  += kr_NoOfSubPatPairs(endPattern) - 1;
    for(p=start; p<=end;p++){

	kr_getSubPatternByNo(&pat,&sub,p);
	in_pat = kr_getSubPatData(pat,sub,INPUT,&count);
	sum = 0.0;  
	in_patStorage = in_pat;  

	for(i=0;i<count;i++){   
	    sum += (*in_pat) * (*in_pat);
	    in_pat++;
	}
	sqrtSum = sqrt(sum);
	in_pat = in_patStorage;

	for(i=0;i<count;i++){   
	    *in_pat = *in_pat / sqrtSum;
	    in_pat++;
	}
    }
*/
}



/*****************************************************************************
  FUNCTION : normReferenceVec

  PURPOSE  :
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void normReferenceVec(struct Unit *hiddenUnitPtr)
{
    double sum,sqrtSum;
    struct Link *linkPtr;

    sum = 0.0;
    FOR_ALL_LINKS(hiddenUnitPtr,linkPtr) {
	sum += linkPtr->weight * linkPtr->weight;
    }

    sqrtSum = sqrt(sum);

    FOR_ALL_LINKS(hiddenUnitPtr,linkPtr){
	if(sqrtSum > 0)
	    linkPtr->weight = linkPtr->weight / sqrtSum;
	else
	    /* Special case */
	    linkPtr->weight = 0.0;
    }
}



/*****************************************************************************
  FUNCTION : moveVec

  PURPOSE  : Train the vectors.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void moveVec(struct Unit *correctReferenceVec, float learnParam1,
                    struct Unit *wrongReferenceVec,   float learnParam2)
{
    struct Link *linkPtr;

    FOR_ALL_LINKS(correctReferenceVec,linkPtr){
	linkPtr->weight += learnParam1 * (linkPtr->to->act - linkPtr->weight);
    }
    normReferenceVec(correctReferenceVec);

    FOR_ALL_LINKS(wrongReferenceVec,linkPtr){
	linkPtr->weight -= learnParam2 * (linkPtr->to->act - linkPtr->weight);
    }
    normReferenceVec(wrongReferenceVec);
}



/*****************************************************************************
  FUNCTION : 

  PURPOSE  :
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void writeVectorToMixupArray(int correctClass, int wrongClass, 
				    int patternNo, int sub_pat_no)
{
    Patterns in_pat;
    double *link=NULL;
    int i,count;
 
    /* calculate startaddress for input pattern array  */
    in_pat = kr_getSubPatData(patternNo,sub_pat_no,INPUT,&count);
    link = mixupArray[correctClass][wrongClass].link; 

    for(i=0;i<count;i++){
	link[i] = (double)in_pat[i];
    }
}



/*****************************************************************************
  FUNCTION : initInitialUnitArray

  PURPOSE  : Init the array "initialUnitArray".
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
void initInitialUnitArray(int startPattern, int endPattern)
{
    int p,i,j;
    Patterns in_pat;
    Patterns out_pat;
    int start, end;
    int pat, sub;
    int count;

    KernelErrorCode = kr_initSubPatternOrder(startPattern,endPattern);
    start = kr_AbsPosOfFirstSubPat(startPattern);
    end   = kr_AbsPosOfFirstSubPat(endPattern);
    end  += kr_NoOfSubPatPairs(endPattern) - 1;
    for(p=start; p<=end;p++){
	kr_getSubPatternByNo(&pat,&sub,p);
	in_pat = kr_getSubPatData(pat,sub,INPUT,&count);
	out_pat = kr_getSubPatData(pat,sub,OUTPUT,NULL);
	for(i=0;i<count;i++){
	    (initialUnitArray[(int) *out_pat].link)[i] = *in_pat++;
	    initialUnitArray[(int) *out_pat].counter++;
	}
    }
    for(i=0;i<noOfClasses;i++){
	for(j=0;j<count;j++){
	    (initialUnitArray[i].link)[j] /= initialUnitArray[i].counter;
	}
    }
}



/*****************************************************************************
  FUNCTION : initFirstUnit

  PURPOSE  : Init the links of a class unit. Remember: At the beginning of
             the learning there exist no hidden units. The first step of 
             the algorithm inserts for every class a hidden unit. This
             functions inits the links of this first units. The array
             "initialUnitArray" contains for every class the average
             activation over all patterns.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void initFirstUnit(struct Unit *hiddenUnitPtr, int class)
{
    int i;
    struct Link *linkPtr;
    struct Unit *inputUnitPtr;

    i = 0;
    FOR_ALL_UNITS(inputUnitPtr){
	if(IS_INPUT_UNIT(inputUnitPtr) && UNIT_IN_USE(inputUnitPtr)){
	    inputUnitPtr->act = (initialUnitArray[class].link)[i++];
	}
    }

    hiddenUnitPtr->bias = class;
    FOR_ALL_LINKS(hiddenUnitPtr,linkPtr) {
	linkPtr->weight = linkPtr->to->act;
    }
    normReferenceVec(hiddenUnitPtr);
}



/*****************************************************************************
  FUNCTION : deleteAllLinksOfTheOutputUnit

  PURPOSE  : 
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static krui_err deleteAllLinksOfTheOutputUnit(void)
{
    KernelErrorCode = krui_setCurrentUnit(GET_UNIT_NO(*FirstOutputUnitPtr));
    ERROR_CHECK;

    krui_deleteAllInputLinks();
    krui_deleteAllOutputLinks();

    return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : calculateUnitXYPos

  PURPOSE  : Calculates the position of the current input, hidden and output
             layer.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void calculateUnitXYPos(void)
{
    int xPos,yPos,maxXPos=0,maxYPos,minXPos=0,minYPos=0,i,
        xOffset,yOffset,bias,h;
    struct Unit *inputUnitPtr=NULL,*hiddenUnitPtr;

    inputUnitPtr = *FirstInputUnitPtr;
    maxXPos = minXPos = GET_UNIT_XPOS(inputUnitPtr);
    maxYPos = minYPos = GET_UNIT_YPOS(inputUnitPtr); 

    /* find maxXPos ... of the input layer */
    FOR_ALL_INPUT_UNITS(inputUnitPtr,i) {
	xPos = GET_UNIT_XPOS(inputUnitPtr);
	yPos = GET_UNIT_YPOS(inputUnitPtr);
	if(xPos > maxXPos) {
	    maxXPos = xPos;
	}
	if(xPos < minXPos){
	    minXPos = xPos;
	}
	if(yPos > maxYPos) {
	    maxYPos = yPos;
	}
	if(yPos < minYPos){
	    minYPos = yPos;
	}
    }

    xOffset = minXPos - 2;
    yOffset = minYPos - 2;

    FOR_ALL_INPUT_UNITS(inputUnitPtr,i){
	SET_UNIT_XPOS(inputUnitPtr,GET_UNIT_XPOS(inputUnitPtr) - xOffset);
	SET_UNIT_YPOS(inputUnitPtr,GET_UNIT_YPOS(inputUnitPtr) - yOffset);
    }

    bias = (*FirstHiddenUnitPtr)->bias; 
    xPos = (maxXPos - xOffset + 3);
    yPos = 1;
 
    FOR_ALL_HIDDEN_UNITS(hiddenUnitPtr,h){
	if(bias == hiddenUnitPtr->bias){
	    yPos++;
	}else{
	    xPos++;
	    bias = hiddenUnitPtr->bias;
	    yPos = 2;
	}
	SET_UNIT_XPOS(hiddenUnitPtr,xPos);
	SET_UNIT_YPOS(hiddenUnitPtr,yPos);
    }   

    SET_UNIT_XPOS(*FirstOutputUnitPtr,xPos+3);
    SET_UNIT_YPOS(*FirstOutputUnitPtr,2);
}



/*****************************************************************************
  FUNCTION : insertNewUnits

  PURPOSE  : Inserts new hidden units to the net. The weights of the links
             are stored in the "mixupArray". 
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static krui_err insertNewUnits(void)
{
    int i,j,k,maxCount,maxJ=0,newUnit;
    double sum,sqrtSum,weight;
    struct Unit *newUnitPtr,*inputUnitPtr;
    struct Link *linkPtr;

    for(i=0;i<noOfClasses;i++) {
	maxCount = 0;
	for(j=0;j<noOfClasses;j++){
	    if(mixupArray[i][j].counter > maxCount) {
		maxCount = mixupArray[i][j].counter;
		maxJ = j;
	    }     
	}
	if(maxCount != 0){

	    /* Generate new unit */
	    newUnit = 
		KernelErrorCode = 
		    krui_copyUnit(lastInsertedUnitArray[i],INPUTS_AND_OUTPUTS);
	    if(KernelErrorCode < 0){
		ERROR_CHECK;
	    }
	    KernelErrorCode = KRERR_NO_ERROR; 
	    newUnitPtr = kr_getUnitPtr(lastInsertedUnitArray[i] = newUnit);
	    newUnitPtr->unit_pos.y += 1;     
   
	    /*init new unit */
	    sum = 0.0;
	    k = 0;
	    FOR_ALL_UNITS(inputUnitPtr){
		if(IS_INPUT_UNIT(inputUnitPtr) && UNIT_IN_USE(inputUnitPtr)) {
		    weight = 
			inputUnitPtr->act = 
			    ((mixupArray[i][maxJ].link)[k++] /
			     mixupArray[i][maxJ].counter);     
		    sum += weight * weight;
		}
	    }
	    sqrtSum = sqrt(sum);

	    FOR_ALL_LINKS(newUnitPtr,linkPtr) {
		linkPtr->weight = linkPtr->to->act / sqrtSum;
	    }
	}
    }

    initMixupArray();
 
    return(KRERR_NO_ERROR);
} 



/*****************************************************************************
  FUNCTION : initMixupArray

  PURPOSE  : Init the array "mixupArray".
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void initMixupArray(void)
{
    int i,j,sizeOfInputVector;
    
    sizeOfInputVector = sizeof(double) * NoOfInputUnits;

    for(i=0;i<noOfClasses;i++) {
	for(j=0;j<noOfClasses;j++){
	    if(mixupArray[i][j].counter != 0){
		(void)memset(mixupArray[i][j].link,0,sizeOfInputVector);
		mixupArray[i][j].counter = 0;
	    }
	}
    }
}



/*****************************************************************************
  FUNCTION :  printMixupArray

  PURPOSE  :
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void printMixupArray(int cycle)
{
    int i,j;

    printf("Cycle %d \n",cycle+1);
    printf("Total number of incorrectly classified items: %d \n",
	   wrongClassCounter);
    printf("Array of classified items:\n\n");

    for(i=0;i<noOfClasses;i++) {
	for(j=0;j<noOfClasses;j++){
	    printf("%d ",mixupArray[i][j].counter);
	}
	printf("\n");
    }
    printf("\n\n");
}



/*****************************************************************************
  FUNCTION : dlvq_trainNet

  PURPOSE  :
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void dlvq_trainNet(int noOfTrainingCycles, int startPattern,
			  int endPattern, float learnParam1, float learnParam2)
{
    struct Unit *correctMaxActivatedUnitPtr=NULL,
    *wrongMaxActivatedUnitPtr=NULL,
    *inputUnitPtr,
    *hiddenUnitPtr;
    double correctMaxAct,wrongMaxAct,act;
    Patterns in_pat;
    Patterns out_pat;
    int i,h,p,j,noOfTrainingCyclesMinus1=noOfTrainingCycles-1;
    struct Link *linkPtr;
    int start, end;
    int pat, sub;
 
    for(j=0;j<noOfTrainingCycles;j++){

	KernelErrorCode = kr_initSubPatternOrder(startPattern,endPattern);
	start = kr_AbsPosOfFirstSubPat(startPattern);
	end   = kr_AbsPosOfFirstSubPat(endPattern);
	end  += kr_NoOfSubPatPairs(endPattern) - 1;
	for(p=start; p<=end;p++){

	    kr_getSubPatternByNo(&pat,&sub,p);
	    in_pat = kr_getSubPatData(pat,sub,INPUT,NULL);
	    out_pat = kr_getSubPatData(pat,sub,OUTPUT,NULL);

	    FOR_ALL_INPUT_UNITS(inputUnitPtr,i){
		inputUnitPtr->Out.output = inputUnitPtr->act = *in_pat++;
	    }
  
	    correctMaxAct = -1e9;
	    wrongMaxAct   = -1e9;
	    correctMaxActivatedUnitPtr = NULL;
	    wrongMaxActivatedUnitPtr = NULL;
	    
	    FOR_ALL_HIDDEN_UNITS(hiddenUnitPtr,h) {
		act = 0.0;
		FOR_ALL_LINKS(hiddenUnitPtr,linkPtr) {
		    act += linkPtr->weight * linkPtr->to->act;
		}
		hiddenUnitPtr->act = act;

		if((((int)hiddenUnitPtr->bias) == ((int) *out_pat)) && 
		   (act >= correctMaxAct)){
		    correctMaxAct = act;
		    correctMaxActivatedUnitPtr = hiddenUnitPtr;
		}
		else if((((int)hiddenUnitPtr->bias)!=((int) *out_pat)) && 
			(act >= wrongMaxAct)){
		    wrongMaxAct = act;
		    wrongMaxActivatedUnitPtr = hiddenUnitPtr;
		}
	    }
	    if(correctMaxAct <= wrongMaxAct){
		wrongClassCounter++;
		mixupArray[(int) *out_pat]
		          [(int)wrongMaxActivatedUnitPtr->bias].counter++;
		if(j == noOfTrainingCyclesMinus1){
		    writeVectorToMixupArray(
				       (int)correctMaxActivatedUnitPtr->bias,
				       (int)wrongMaxActivatedUnitPtr->bias,
				       pat,sub);
		}else{ 
		    moveVec(correctMaxActivatedUnitPtr,learnParam1,
			    wrongMaxActivatedUnitPtr,learnParam2);
		}
	    }
	} /* end: noOfTrainingCycles */
	printMixupArray(j);  

	if(wrongClassCounter == 0){
	    continueLearning = 0;
	    return;
	}

	if(j!=noOfTrainingCyclesMinus1){ 
	    initMixupArray();  
	    wrongClassCounter = 0;   
	}
    }
}



/*****************************************************************************
  FUNCTION : initLastInsertedUnitArray

  PURPOSE  : Init the array lastInsertedUnitArray. This array contains the
             position of the last inserted unit. There is an entry for
             every class.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void initLastInsertedUnitArray(void)
{
    int h,bias;
    struct Unit *hiddenUnitPtr=NULL,*lastUnitPtr=NULL;

    bias = (*FirstHiddenUnitPtr)->bias;
    FOR_ALL_HIDDEN_UNITS(hiddenUnitPtr,h){
	if(((int)(hiddenUnitPtr->bias)) == ((int)bias)) {
	    lastUnitPtr = hiddenUnitPtr;
	}else{
	    lastInsertedUnitArray[(int)lastUnitPtr->bias] =
		GET_UNIT_NO(lastUnitPtr);
	    bias = hiddenUnitPtr->bias;
	    lastUnitPtr = hiddenUnitPtr;
	}
    }
    lastInsertedUnitArray[(int)lastUnitPtr->bias]=GET_UNIT_NO(lastUnitPtr);

}  



/*****************************************************************************
  FUNCTION : dlvq_setPointers

  PURPOSE  : Calculates the beginning of the input, hidden, output
             units in the topo_ptr_array.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err dlvq_setPointers(void)
{
    FirstInputUnitPtr   = (struct Unit **)(&topo_ptr_array[1]);
    if(*(FirstInputUnitPtr-1)!=NULL) CC_ERROR(KRERR_CC_ERROR1);
    FirstHiddenUnitPtr  = FirstInputUnitPtr  + NoOfInputUnits  + 1;
    if(*(FirstHiddenUnitPtr-1)!=NULL) CC_ERROR(KRERR_CC_ERROR1);
    FirstOutputUnitPtr  = FirstHiddenUnitPtr + NoOfHiddenUnits + 1;
    if(*(FirstOutputUnitPtr-1)!=NULL) CC_ERROR(KRERR_CC_ERROR1);
    return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : generateMissingClassHiddenUnits

  PURPOSE  : If all hidden units of a class are removed, this functions 
             generates a new first hidden unit. 
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
void generateMissingClassHiddenUnits(int *generatedNewUnit)
{
    struct Unit *hiddenUnitPtr;
    int i,h;

    *generatedNewUnit = 0;

    for(i=0;i<noOfClasses;i++){
	lastInsertedUnitArray[i] = 0;
    }
    FOR_ALL_HIDDEN_UNITS(hiddenUnitPtr,h){
	lastInsertedUnitArray[(int)hiddenUnitPtr->bias] = 1;
    }
    for(i=0;i<noOfClasses;i++){
	if(lastInsertedUnitArray[i] == 0) {
	    (void)insertFirstUnit(&hiddenUnitPtr);
	    initFirstUnit(hiddenUnitPtr,i);
	    *generatedNewUnit = 1;
	}else{
	    lastInsertedUnitArray[i] = 0;
	} 
    }
}



/*****************************************************************************
  FUNCTION : insertFirstUnit

  PURPOSE  : Inserts the first hidden units to the net. For every class one
             unit.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static krui_err insertFirstUnit(struct Unit **hiddenUnitPtr)
{
    int hiddenUnit=0;
    struct Unit *inputUnitPtr;

    hiddenUnit = KernelErrorCode = krui_createDefaultUnit(); 
    if(KernelErrorCode < 0) {
	ERROR_CHECK;
    }

    KernelErrorCode = krui_setUnitTType(hiddenUnit,HIDDEN);  
    ERROR_CHECK;

    KernelErrorCode = krui_setUnitActFunc(hiddenUnit,"Act_Identity");
    ERROR_CHECK;

    *hiddenUnitPtr = kr_getUnitPtr(hiddenUnit);
    KernelErrorCode = krui_setCurrentUnit(hiddenUnit);
    ERROR_CHECK;

    /* generate links between hidden unit and input units */
    FOR_ALL_UNITS(inputUnitPtr) {
	if(IS_INPUT_UNIT(inputUnitPtr) && UNIT_IN_USE(inputUnitPtr)){
	    KernelErrorCode = krui_createLink(GET_UNIT_NO(inputUnitPtr),0.0);
	    ERROR_CHECK;
	}
    }

    /* generate a link between the output unit and the hidden unit */
    KernelErrorCode = krui_setCurrentUnit(GET_UNIT_NO(*FirstOutputUnitPtr));
    ERROR_CHECK;
    KernelErrorCode = krui_createLink(hiddenUnit,1.0); 
    ERROR_CHECK;
    return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : allocArrays

  PURPOSE  :
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
void allocArrays(void)
{
    allocMixupArray();
    allocLastInsertedUnitArray();
}



/*****************************************************************************
  FUNCTION : generateTmpTopoPtrArray

  PURPOSE  : Generates a topo_ptr_array for local use.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void generateTmpTopoPtrArray(void)
{
    struct Unit *unitPtr;
    TopoPtrArray tmp_array;

    if(topo_ptr_array != NULL) {
	free(topo_ptr_array);
    }

    tmp_array = 
	topo_ptr_array = 
	    (struct Unit **)calloc(NoOfInputUnits+5,sizeof(struct Unit *));

    *tmp_array = NULL;
    tmp_array++;
    FOR_ALL_UNITS(unitPtr){
	if(IS_INPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)){
	    *tmp_array = unitPtr;
	    tmp_array++;
	}
    }
    *tmp_array = NULL;
    tmp_array++;
    *tmp_array = NULL;
    tmp_array++;
    FOR_ALL_UNITS(unitPtr){
	if(IS_OUTPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)){
	    *tmp_array = unitPtr;
	    tmp_array++;
	}
    }
    *tmp_array = NULL;
}



/*****************************************************************************
  FUNCTION : freeTmpTopoPtrArray

  PURPOSE  : Frees the storage of the temporary "topo_ptr_array".
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
static void freeTmpTopoPtrArray(void)
{
    free(topo_ptr_array);
    topo_ptr_array = NULL;
}



/*****************************************************************************
  FUNCTION : LEARN_DLVQ

  PURPOSE  : The main learn routine of DLVQ
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err LEARN_DLVQ(int startPattern, int endPattern, float *ParameterInArray,
		    int NoOfInParams, float **ParameterOutArray, 
		    int *NoOfOutParams)
{
    static int cycleCounter=0,noOfTrainingCycles=0;
    struct Unit  *unitPtr,*hiddenUnitPtr;
    int i,d1,d2,d3,noOfLinks,generatedNewUnit;
    static float learnParam1=0.03,learnParam2=0.03;
    static float OutParameter[1];

    *NoOfOutParams = 1;
    *ParameterOutArray = OutParameter;

    if(cc_allButtonIsPressed) {
	continueLearning = 1;
	cycleCounter = 0;
	wrongClassCounter = 0;
	NoOfInputUnits = NoOfHiddenUnits = NoOfOutputUnits = 0;
	FOR_ALL_UNITS(unitPtr) {
	    if(IS_INPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
		NoOfInputUnits++;
	    }
	    if(IS_HIDDEN_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
		NoOfHiddenUnits++;
	    }
	    if(IS_OUTPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
		NoOfOutputUnits++;
	    }
	}
	if(NoOfOutputUnits != 1){
	    return(DLVQ_ERROR3); /* Wrong no. of output units */
	}

	learnParam1 = ParameterInArray[0];
	learnParam2 = ParameterInArray[1];
	noOfTrainingCycles = (int)ParameterInArray[2];
	dlvq_numberOfLearnCycles = (int)ParameterInArray[3];
    }
  

    if(newPatternsLoaded){
	newPatternsLoaded = 0;
	KernelErrorCode = getNoOfClasses(startPattern,endPattern);
	ERROR_CHECK;
	normPatterns(startPattern,endPattern);
	allocInitialUnitArray();
	initInitialUnitArray(startPattern,endPattern);
    }

    if(cc_allButtonIsPressed && (NoOfHiddenUnits == 0)){ /* First time */
	allocArrays();
	generateTmpTopoPtrArray();
	KernelErrorCode = dlvq_setPointers();
	ERROR_CHECK;
	KernelErrorCode = deleteAllLinksOfTheOutputUnit();
	ERROR_CHECK;
	for(i=0;i<noOfClasses;i++) {
	    KernelErrorCode = insertFirstUnit(&hiddenUnitPtr);
	    ERROR_CHECK;
	    initFirstUnit(hiddenUnitPtr,i);
	}
	freeTmpTopoPtrArray();
	KernelErrorCode = kr_topoSort(TOPOLOGICAL_FF);
	ERROR_CHECK;     
	KernelErrorCode = dlvq_setPointers();
	ERROR_CHECK; 

	sortHiddenUnitsByClasses(0,noOfClasses-1);
	calculateUnitXYPos();    
	initLastInsertedUnitArray();

	NetModified = FALSE;
	LearnFuncHasChanged = FALSE;
    }else if(cc_allButtonIsPressed && (NoOfHiddenUnits != 0)) { 

	/* Continue learning */
	if(NetModified || LearnFuncHasChanged){
	    allocArrays();
	    KernelErrorCode = kr_topoSort(TOPOLOGICAL_FF);
	    ERROR_CHECK;    
	    KernelErrorCode = dlvq_setPointers();
	    ERROR_CHECK; 
	    krui_getNetInfo(&d1,&noOfLinks,&d2,&d3);
	    if(noOfLinks != NoOfInputUnits*NoOfHiddenUnits + NoOfHiddenUnits){
		return(DLVQ_ERROR4); /* the network has a wrong topology */
	    }

	    generateMissingClassHiddenUnits(&generatedNewUnit);
	    if(generatedNewUnit) {
		KernelErrorCode = kr_topoSort(TOPOLOGICAL_FF);
		ERROR_CHECK;
		KernelErrorCode = dlvq_setPointers();
		ERROR_CHECK; 
	    }
	    NetModified = FALSE;
	    LearnFuncHasChanged = FALSE;
	}
      
	/* even if the net is not modified you have to do the following
	   steps for security. 
	   */  
	sortHiddenUnitsByClasses(0,NoOfHiddenUnits-1);
	calculateUnitXYPos();         
	initLastInsertedUnitArray();
    }  

    if(cc_allButtonIsPressed){
	oldNoOfClasses = noOfClasses;
	cc_allButtonIsPressed = 0;
    }

    if(continueLearning){
	initMixupArray();
	dlvq_trainNet(noOfTrainingCycles,startPattern,endPattern,
		      learnParam1,learnParam2);
	NET_ERROR(OutParameter) = wrongClassCounter;
	if((cycleCounter<(dlvq_numberOfLearnCycles-1)) &&
	   (wrongClassCounter != 0)){ 
	    /*do not insert new hidden units by the last path through the net*/
	    wrongClassCounter = 0;
	    cycleCounter++; 
	    KernelErrorCode = insertNewUnits();
	    ERROR_CHECK;
	    KernelErrorCode = kr_topoSort(TOPOLOGICAL_FF);
	    ERROR_CHECK;
	    KernelErrorCode = dlvq_setPointers();
	    ERROR_CHECK; 
	}
    } 
    return(KRERR_NO_ERROR);
}



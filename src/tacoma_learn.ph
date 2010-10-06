/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/tacoma_learn.ph,v $
  SHORTNAME      : tacoma_learn.ph
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          : For informations about the algorithms see one of
                   the following papers:

                   J.M. Lange, H.M. Voigt, D. Wolf: 
                   "Growing Artificial Neural Networks Based on
                    Correlation Measures, Task Decomposition and Local 
                    Attention Neurons."
                   IEEE '94, pp. 1355-1358.

                   J.M. Lange, H.M. Voigt, D. Wolf: 
                   "Task Decomposition and Correlations in Growing
                     Artificial Neural Networks."
                   ICANN '94, pp. 735-738.

                   J. Gatter:
                   "Lernverfahren neuronaler Netze mit automatischer
                    Bestimmung der Netzwerktopologie"
                   Diplomarbeit Nr. 1337, university of Stuttgart

  AUTHOR         : Juergen Gatter
  DATE           : 30.3.1996

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : 

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _TACOMA_LEARN_DEFINED_
#define  _TACOMA_LEARN_DEFINED_

/* begin global definition section */
krui_err LEARN_Tacoma(int StartPattern, int EndPattern,
                      float *ParameterInArray, int NoOfInParams,
                      float **ParameterOutArray, int *NoOfOutParams);

/* end   global definition section */

/* begin privat definition section */

krui_err tac_testCorrectnessOfAddParameters(void);
   /* tests correctness of additional parameters. Correct values are :
      [0] TAC_KOHONEN             [0    .. inf)
      [1] TAC_XI_RI_ETA           [0.0  .. inf)
      [2] TAC_THRESHOLD           (-inf .. 1.0)
      [3] TAC_LAMBDA              (-inf .. inf)
      [4] TAC_BETA                (0.0  .. 1.0)
      if parameter setting is incorrect KRERR_CC_INVALID_ADD_PARAMETERS
      will be returned */


krui_err tac_initVariables(float* ParameterInArray,
                                int StartPattern,int EndPattern);
   /* Initializes global variables, read the Parameters, sorts the units, 
      assigns the learning routines, generates the Layerlist, checks 
      the parameters and calculates the initial display-Parameters. */

krui_err tac_allocateStorage(int StartPattern, int  EndPattern);

   /* allocates the first part of the storage used by TACOMA. Tacoma
      uses much more Arrays than for example Cascade-Correlation.
      The Second Part of the Arrays will be allocated later, when
      the number of Units to be inserted, is calculated */

krui_err tac_freeStorage(int StartPattern, int  EndPattern);

   /* frees all allocated Memory. Between two Learning-Cycles no
      add. memory should be held by TACOMA */

krui_err tac_calculateOutputUnitError(int StartPattern,int EndPattern);

   /* Calculates the error of all output units and stores the result
      in the array OutputUnitError. Additionaly the Arrays 
      MeanOutputUnitError and PatternSumError are calculated here. 
      And the WholeSummedError, too.
      PSE und WSE are using fabs(act-teach), whereas the others
      are using act-teach.
      The routine is similar to the one in cc_learn.c. */

bool tac_connect(int s,struct Unit* OldUnit,
	         int StartPattern,int EndPattern,float* Correlation);

   /* returns true, iff Special Unit s and Input/Hidden-Unit OldUnit
      have to be connected. If OldUnit is a Input-Unit than it's
      quite clear to connect them, but if it is another hidden unit
      then the two window functions have to have a significant 
      overlap. For further Details see the original paper or my DA */

void tac_initWindowFuncParameter(struct Unit* UnitPtr,int UnitNo);

   /* if a new Unit is in construction, the WindowFuncParameters
      Xi and Ri are taken from the arrays calculated above */

krui_err tac_generateNewUnit(int UnitNo,int LayerNo,
			   int StartPattern,int EndPattern);
   
   /* Builds a new special unit. The new unit is declared as a special
      unit to make the old routines of cc_learn/cc_rcc useable.
      The routine does the following things :  
         Set type to SPECIAL
         Set activition function to ACT_TACOMA
         Actualizes the Layerlist and the LayerNo of the Unit
         Sets the display (like a hidden unit in CC)
         Generates the links between the input/ older hidden units
              and the new one.
         Sets Xi and Ri (see above) */

krui_err tac_initXiAndRis(int StartPattern,int EndPattern);
                         
   /* Initialises the Arrays Xi and Ri with values 
      (sum(x_i)/n_p) + random*(max(x_i)-min(x_i). -0.1 < Random < 0.1 */

int  tac_NextSpecialUnit(int p,Patterns in_pat_First);

   /* returns the X in Input-Space, which has the minimum
      distance the the given Pattern. As distance-norm is
      sqrt( sum_i ( diff_i ^ 2 ) ) used . */

void tac_changeXi(int UnitNo,int p,int d,int maxD,
                                Patterns in_pat);

   /* changes the Xis of the virt. SpecialUnit, which was calculated
      before as the next in Input-Space. Here is TAC_ALPHA a 
      regulation term, which is decreased in time. */

void tac_printRanks(float MaxSummedError);

   /* Prints the data after the kohonen-mapping */

int tac_calculateRanksAndRadius(int start,int end);

   /* When we made a certain amount of changes to the X, we hope,
      that the X's are now located at the maxima of the remaining
      error. Now we count the now of Patterns, which are in our
      region, the correlated error and the mean distances of the
      patterns to the X. These results are used to determine, which
      of the units have to be taken and which initial radius they
      should get. (for formulas see paper or DA, please) */

int tac_MappingOfTheNewUnits(int StartPattern,int EndPattern);

   /* Realizes the Mapping of the new units. Unlike in CC the Special
      Units are now virtual, they where never generated. Only the two
      Arrays Xi and Ri are generated and calculated */

krui_err tac_calculateCorrelation(int StartPattern, int EndPattern,
			      int counter);

   /* Calculates the Correlation as given in CC. The Correlation is a 
      measure of how good a new special unit correlates with the given
      Output-Error. The unit weights (not the r_i and x_i) are trained
      to maximize this Correlation. As a sideeffect, this routine 
      divides the array SpecialUnitSumAct through the number of
      patterns. SUSA is then accesible via MeanYi. 
      Prints the values */

 float tac_calculateRijAndSumRij(float** Rij,float* MeanYi,
				int start,int end,int n);

   /* the Arrays Nij and Rij were calculated. Both Arrays were needed
      by the AntiCorrelation F. (More see papers or DA)
      The routine gives back the divident of F. */

float tac_calculateAntiCorrelation(int StartPattern, int EndPattern,bool First);

   /* Calculates the AntiCorrelation F. F is a measure of :
      1. How good is the Correlation between the units and the 
         ouput error ? This is achieved by using the calculated and 
         summarized Correlations S. 
      2. Do the new units all have different outputs ? The Correlation
         Rij between 2 different units is calculated, and the sum is
         placed in the Nenner, to guarantee minimal correlation.
      F will be used to determine, if the learning procedure is stagnant
      or further learning could be usefull.
      F will be trained by a gradient ascent procedure. */

krui_err tac_initSpecialUnitLinks(void);

   /* initializes the weights, radii, xi and the variables used by the 
      learning routine of all special units.
      The weights and the bias are set to a random value. 
      Further are the arrays allocated, which are needed for the
      thraining of the new hidden units.*/

krui_err tac_trainSpecialUnits(int maxNoOfCovarianceUpdateCycles, 
			    float minCovarianceChange,
			    int specialPatience,int StartPattern, 
			    int EndPattern,float param1, float param2,
			    float param3,int MaxSpecialUnitNo,int LayerNo);
    
   /* Calls the training procedure for the training of the Special units.
      It calls the routines for the calculation of the correlations, the
      anticorrelation, and the online/offline parts of the training. 
      See cc_trainSpecialUnits,too. */

void tac_protocollNet(void);

   /* protocolls net to the file 'protokoll.dat', if it's the first call. */

void tac_protocollLearning(int LayerNo,int StartPattern,int EndPattern);

   /* Write protocoll data of learning process to protokoll.dat. */

krui_err tac_installNewUnits(int LayerNo,float MaxError,int StartPattern,int EndPattern);
   /* Determine how much and which hiddden units have to be generated.
      Do this and initialize the links. */

void tac_propagateXiRiOnlineCase(struct Unit* SpecialUnitPtr,
          int p,int s,float SummedDeltaS,double nMinus1Divn ,
          float eta,float mu, float dummy);

   /* online part of learning of the xi and ri for the online
      Backprop. This one is used by default, as TAC_XIRI_ONLINE 
      isn't changed. The parameters mu and dummy aren't used in
      the function as implemented. 
      For more information see tac_propagateXiRi, too */
 
void tac_propagateXiRi(struct Unit* SpecialUnitPtr,int p,int s,
          float SummedDeltaS, double nMinus1Divn, float eta, float mu, 
          float dummy);
   /* online part of learning of the xi and ri for the offline
      learning algorithms like Quickprop and Rprop.
      This routine will never used by default. Only if
      TAC_XIRI_ONLINE is set to FALSE, then the training of the
      xis and ris will be done with the same learning function 
      and parametersm as the training of the weights.
      By default TAC_XIRI_ONLINE is true because learning with
      for example Quickprop is to good. Then xi and ri will
      be changed too much, and the calculated links to other
      hidden units will make no sense. And the overhead of
      calculating the refernce vecors, too. */

krui_err tac_propagateSpecial(int start,int end,int n,
                  int counter,float dummy1, float dummy2,float dummy);

   /* Calculates and summarizes the slopes needed by the gradient 
      procedures. Calls the routines for the training of the xi and
      ris. */

krui_err tac_propagateSpecialOnlineCase (int start,int end,int n,
		   int counter,float eta, float mu,float dummy3);
 
   /* Does the online-learning, iff the learning procedure is 
      Backprop online. Calls tac_propagateXiRiOnline */



void tac_updateSpecial(float param1,float param2,float param3);

   /* this routine updates the bias of the Special-Units and
      the weights, the radii and the xi of links to a special unit. 
      The routine calls the procedure, which makes the update-step
      according to the actual valid learning function (QuickProp,RProp
      or BackProp, according to cc_SpecialUnitUpdate) */
      
float*  PatternSumError=NULL;
 
   /* Array [n_p] of the summed absolute errors of the output-units
      for a given pattern */

float   WholeSummedError;

   /* Sum of the array above */

float** Rij;

   /* Two-dim. array [n_i][n_i], whereas n_i is the number of 
      new units. See papers for further explanation */

float** Nij;  

   /* see Rij */

float*  MeanYi;  /* points to SpecialUnitSumAct */

   /* Stores the mean activations of the outputs */

TAC_SPECIAL_UNIT_TYPE* SpecialUnitData=NULL;

   /* Stores additional data of actually used hidden units */

TAC_PRIME_TYPE** PrimesOfSpecialUnits;

   /* Stores the last changes and primes of the vectors resp. the 
      radii. This is only needed if the xi and ri are trained
      with the offline versions */

int NoOfInstalledUnits;

   /* Counts the actually installed units */

float AntiCorr;

   /* F as in the formulas */

float AC_Zaehler,AC_Nenner;

  /* the divisor and the divident of F */

/* end privat definition section */

#endif /* _TACOMA_LEARN_DEFINED_ */

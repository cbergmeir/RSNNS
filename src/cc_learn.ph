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
extern krui_err LEARN_CC(int StartPattern, int EndPattern,
                      float *ParameterInArray, int NoOfInParams,
                      float **ParameterOutArray, int *NoOfOutParams);
extern krui_err LEARN_CasCor(int StartPattern, int EndPattern,
                      float *ParameterInArray, int NoOfInParams,
                      float **ParameterOutArray, int *NoOfOutParams);
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

/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/tacoma_learn.h,v $
  SHORTNAME      : 
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
  DATE           : 30.03.96

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : 

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _TACOMA_LEARN_DEFINED_
#define  _TACOMA_LEARN_DEFINED_

/* begin global definition section */
extern krui_err LEARN_Tacoma(int StartPattern, int EndPattern,
                      float *ParameterInArray, int NoOfInParams,
                      float **ParameterOutArray, int *NoOfOutParams);

/* end   global definition section */

/* begin privat definition section */
/* end privat definition section */

#endif /* _TACOMA_LEARN_DEFINED_ */

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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/prun_f.ph,v $
  SHORTNAME      : prun_f.ph
  SNNS VERSION   : 4.2

  PURPOSE        : private header file for pruning algorithms
  NOTES          :

  AUTHOR         : Tobias Schreiner
  DATE           : 13.3.94

  CHANGED BY     : Jochen Biedermann
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _PRUN_F_DEFINED_
#define  _PRUN_F_DEFINED_

/* begin global section */

#define PR_ALL_PATTERNS -1

#define DEFAULT_TRAIN_CYCLES 1000
#define DEFAULT_RETRAIN_CYCLES 100
#define DEFAULT_ERROR_INCREASE 10.0
#define DEFAULT_ACCEPTED_ERROR 5.0
#define DEFAULT_MIN_ERROR 1.0
#define DEFAULT_OBS_INIT_PARAMETER 0.000001

int pr_trainCycles,
    pr_retrainCycles,
    pr_recreate,
    pr_refresh,
    pr_inputPruning,
    pr_hiddenPruning;

float pr_maxErrorInc,
      pr_acceptedError,
      pr_minError,
      pr_obs_initParameter;


void pr_checkDeadUnits (void);
krui_err pr_calcMeanDeviation (int pattern, float *sum_error);

krui_err PRUNE_Mag (int pattern);

krui_err PRUNE_OBD (int pattern);

krui_err PRUNE_OBS (int pattern);

krui_err PRUNE_Skeletonization (int pattern);

krui_err PRUNE_Noncontributing (int pattern);

void pr_obs_setInitParameter (float param);

krui_err pr_callPrunFunc (int pattern);

void pr_setInputPruning (int value);

void pr_setHiddenPruning (int value);

/* end global section */

/* begin private section */

#define SMALL_WEIGHT 0.0001

#define PR_CONST        1
#define PR_SAME         2
#define PR_REVERSE      3

#define UFLAG_PRUNEFLAG 0x8000


typedef krui_err (SnnsCLib::*PrunFuncPtr) (...);

 struct Unit *pr_candidateTargetUnit, *pr_candidateUnit;
 struct Link *pr_candidateLink;
 float pr_candidateSaliency, pr_candidateStddev;
 int pr_candidateSourceUnitNo, pr_candidatePass, pr_Pass;

/* variables only used by OBS */
 int pr_candidateLinkNo, pr_noOfLinks;
 RbfFloatMatrix pr_inverseHessian, pr_derivVector, pr_helpHX, pr_helpXH;

/* functions */

 void pr_updateNoOfLinks (struct Unit *unit_ptr, struct Link *link_ptr);

 void pr_mag_processLink (struct Unit *unit_ptr, struct Link *link_ptr);

 void pr_obd_processLink (struct Unit *unit_ptr, struct Link *link_ptr,
				float delta, float delta2);
 void pr_obd_checkLink (struct Unit *unit_ptr, struct Link *link_ptr);

 void pr_obs_countLinks ();
 void pr_obs_calculateDerivative (struct Link *link_ptr,
					float help, int weight_no);
 void pr_obs_calculateDerivVector (struct Unit *output_unit);
 krui_err pr_obs_updateInverseHessian (int no_of_patterns);
 krui_err pr_obs_calculateInverseHessian (int pattern);
 void pr_obs_processLink (struct Unit *unit_ptr,
				struct Link *link_ptr,
				int weight_no);
 void pr_obs_updateLink (struct Link *link_ptr,
			       float update_const,
			       int weight_no);

 void pr_skel_processLink (struct Unit *unit_ptr, struct Link *link_ptr);
 void pr_skel_check_saliency (struct Unit *unit_ptr);

 krui_err pr_nc_calc_stddev (int pattern, struct Unit *this_unit_ptr);
 void     pr_nc_check_stddev (struct Unit *unit_ptr, struct Unit *unit_ptr2);
 krui_err pr_nc_process_succ_unit (struct Unit *unit_ptr, struct Link *link_ptr);
 void     pr_nc_mark_all_pred (struct Unit *root_unit_ptr);
 krui_err pr_nc_remove_unit (void);
#if 0
 void     pr_nc_output (void);
#endif
/* end private section */

void pr_nc_clear_marks();

#endif /* _PRUN_F_DEFINED_ */

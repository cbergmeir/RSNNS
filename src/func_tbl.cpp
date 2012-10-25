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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/func_tbl.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Function table
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 25.06.90

  CHANGED BY     : Sven Doering, Michael Vogt, Ralf Huebner, Thomas Gern
  RCS VERSION    : $Revision: 2.51 $
  LAST CHANGE    : $Date: 1998/09/03 09:52:20 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#include "SnnsCLib.h"

/*  The function table contains 5 entries:
     - the XGUI-Identifier of this function (a pointer to a string constant),
       note that the identifier have to match [A-Za-z]^[|, ]* (i.e. the first
       character of the symbol must be a letter and all other characters must
       not match [|, ])

     - an integer constant that determine the class of the function.
       8 different classes are possible:
       OUT_FUNC, ACT_FUNC, ACT_DERIV_FUNC, SITE_FUNC, UPDATE_FUNC, LEARN_FUNC,
       INIT_FUNC, PRUNING_FUNC and TEST_FUNC

     - the number of input parameters for this function (only needed for Update,
       Init, Learning and Test functions)

     - the number of output parameters returned by this function (only needed
       for Update, Learning and Test functions)

     - a pointer to the (user defined) function.
*/

/*  *********************************************************************  */

/*  Here is the function table
*/
const struct FuncTable SnnsCLib::kernel_func_table[]	=
  {

/*  Output functions  */
/*  In 'kr_const.h' is defined that the NULL-pointer presents the
    identity function. So the "Out_Identity" function is represented
    by the NULL-pointer instead of the function pointer.
*/
/*  This is the DEFAULT FUNCTION  */
{"Out_Identity",     OUT_FUNC | DEFAULT_FUNC, 0, 0, (FunctionPtr) OUT_IDENTITY},

{"Out_Threshold05",  OUT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::OUT_Threshold05},
{"Out_Clip_01",      OUT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::OUT_Clip_01},
{"Out_Clip_11",      OUT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::OUT_Clip_11},
{"Out_ART2_Noise_PLin", OUT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::OUT_ART2_Noise_PLin},
{"Out_ART2_Noise_ContDiff", OUT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::OUT_ART2_Noise_ContDiff},

/*  Activation functions  */
/*  This is the DEFAULT FUNCTION  */
{"Act_Logistic",     ACT_FUNC | DEFAULT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_Logistic},
{"Act_Elliott",      ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_Elliott},
{"Act_BSB",          ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_BSBFunction},
{"Act_TanH",         ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_TanHFunction},
{"Act_TanH_Xdiv2",   ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_TanHFunction_Xdiv2},
{"Act_Perceptron",   ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_Perceptron},
{"Act_Signum",       ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_Signum},
{"Act_Signum0",      ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_Signum0},
{"Act_Softmax",     ACT_FUNC ,0, 0, (FunctionPtr) &SnnsCLib::ACT_Softmax},
{"Act_StepFunc",     ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_StepFunction},
{"Act_HystStep",     ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_HystStepFunction},
{"Act_BAM",          ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_BAMFunction},
{"Logistic_notInhibit" , ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_LogisticI},
{"Act_MinOutPlusWeight", ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_MinOutPlusWeight},
{"Act_Identity",     ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_Linear},
{"Act_IdentityPlusBias", ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_Linear_bias},
{"Act_LogisticTbl",  ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_LogisticTbl},
{"Act_RBF_Gaussian", ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_RBF_Gaussian},
{"Act_RBF_MultiQuadratic", ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_RBF_Multiquadratic},
{"Act_RBF_ThinPlateSpline", ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_RBF_Thinplatespline},
{"Act_less_than_0",  ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_less_than_0},
{"Act_at_most_0",    ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_at_most_0},
{"Act_at_least_2",   ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_at_least_2},
{"Act_at_least_1",   ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_at_least_1},
{"Act_exactly_1",    ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_exactly_1},
{"Act_Product",      ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_Product},

{"Act_ART1_NC",      ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ART1_NC},
{"Act_ART2_Identity",ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ART2_Linear},
{"Act_ART2_NormP",   ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ART2_NormP},
{"Act_ART2_NormV",   ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ART2_NormV},
{"Act_ART2_NormW",   ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ART2_NormW},
{"Act_ART2_NormIP",  ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ART2_NormIP},
{"Act_ART2_Rec",     ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ART2_Rec},
{"Act_ART2_Rst",     ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ART2_Rst},
{"Act_ARTMAP_NCa",   ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ARTMAP_NCa},
{"Act_ARTMAP_NCb",   ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ARTMAP_NCb},
{"Act_ARTMAP_DRho",  ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_ARTMAP_DRho},
{"Act_LogSym",       ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_LogisticSym},
{"Act_CC_Thresh",    ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_CC_Threshold},
{"Act_Sinus",        ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_SIN},
{"Act_Exponential",  ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_EXPONENT},
{"Act_TD_Logistic",  ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_TD_Logistic},
{"Act_TD_Elliott",   ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_TD_Elliott},
{"Act_Euclid",       ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_Euclid},
{"Act_Component",    ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_Component},
{"Act_RM",	     ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_RM}, 
{"Act_TACOMA",       ACT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_TACOMA},


/*  Derivation functions of the activation functions
    NOTE: The derivative function of the activation function MUST have
	  the same name as the belonging activation function.
*/
{"Act_Logistic",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Logistic},
{"Act_Elliott",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Elliott},
{"Act_BSB",          ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_BSBFunction},
{"Act_TanH",         ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_TanHFunction},
{"Act_TanH_Xdiv2",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_TanHFunction_Xdiv2},
{"Act_Perceptron",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_Signum",       ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_Signum0",      ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_Softmax",     ACT_DERIV_FUNC ,0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_StepFunc",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_HystStep",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_BAM",          ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Logistic_notInhibit",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_MinOutPlusWeight", ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_Identity",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Identity},
{"Act_IdentityPlusBias", ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Identity},
{"Act_LogisticTbl",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Logistic},
{"Act_RBF_Gaussian", ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_RBF_Gaussian},
{"Act_RBF_MultiQuadratic", ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_RBF_Multiquadratic},
{"Act_RBF_ThinPlateSpline", ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_RBF_Thinplatespline},
{"Act_less_than_0",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_at_most_0",    ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_at_least_1",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_at_least_2",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_exactly_1",    ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_Product",      ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ART1_NC",      ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ART2_Identity",ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ART2_NormP",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ART2_NormV",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ART2_NormW",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ART2_NormIP",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ART2_Rec",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ART2_Rst",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ARTMAP_NCa",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ARTMAP_NCb",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_ARTMAP_DRho",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_LogSym",       ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_LogisticSym},
{"Act_CC_Thresh",    ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_CC_Threshold},
{"Act_Sinus",        ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_SIN},
{"Act_Exponential",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_EXPONENT},
{"Act_TD_Logistic",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_TD_Logistic},
{"Act_TD_Elliott",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_TD_Elliott},
{"Act_Euclid",       ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_Component",    ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy},
{"Act_RM",           ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_Dummy}, 
{"Act_TACOMA",       ACT_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_DERIV_TACOMA},

/*  Second derivation functions of the activation functions
    NOTE: The second derivative function of the activation function
    MUST have the same name as the belonging activation function.
*/
{"Act_Logistic",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Logistic},
{"Act_Elliott",             ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Elliott},
{"Act_BSB",                 ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_BSBFunction},
{"Act_TanH",                ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_TanHFunction},
{"Act_TanHPlusBias",        ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_TanHFunction},
{"Act_TanH_Xdiv2",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_TanHFunction_Xdiv2},
{"Act_Perceptron",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_Signum",              ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_Signum0",             ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_Softmax",             ACT_2_DERIV_FUNC ,0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_StepFunc",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_HystStep",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_BAM",                 ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Logistic_notInhibit",     ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_MinOutPlusWeight",    ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_Identity",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Identity},
{"Act_IdentityPlusBias",    ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Identity},
{"Act_LogisticTbl",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Logistic},
{"Act_RBF_Gaussian",        ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_RBF_MultiQuadratic",  ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_RBF_ThinPlateSpline", ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_less_than_0",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_at_most_0",           ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_at_least_1",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_at_least_2",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_exactly_1",           ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_Product",             ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ART1_NC",             ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ART2_Identity",       ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ART2_NormP",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ART2_NormV",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ART2_NormW",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ART2_NormIP",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ART2_Rec",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ART2_Rst",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ARTMAP_NCa",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ARTMAP_NCb",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_ARTMAP_DRho",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_LogSym",              ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_TanHFunction},
{"Act_TD_Logistic",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_TD_Elliott",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_Euclid",              ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_Component",           ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_RM",                  ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy}, 
{"Act_TACOMA",              ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_CC_Thresh",           ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_Sinus",               ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},
{"Act_Exponential",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::ACT_2_DERIV_Dummy},


/*  Site functions  */
{"Site_WeightedSum", SITE_FUNC | DEFAULT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::SITE_WeightedSum},
{"Site_Pi",          SITE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::SITE_ProductA},
{"Site_Produkt",     SITE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::SITE_Product},
{"Site_Max",         SITE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::SITE_Max},
{"Site_Min",         SITE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::SITE_Min},
{"Site_at_least_2",  SITE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::SITE_at_least_2},
{"Site_at_least_1",  SITE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::SITE_at_least_1},
{"Site_at_most_0",   SITE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::SITE_at_most_0},
{"Site_Reciprocal",  SITE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::SITE_Reciprocal_WeightedSum},

/*  Update functions  */
/*  This is the DEFAULT FUNCTION  */
{"Topological_Order", UPDATE_FUNC | DEFAULT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_topologicalPropagate},
{"ART1_Stable",       UPDATE_FUNC, 1, 0, (FunctionPtr) &SnnsCLib::UPDATE_ART1_Propagate},
{"ART1_Synchronous",  UPDATE_FUNC, 1, 0, (FunctionPtr) &SnnsCLib::UPDATE_ART1_syncPropagate},
{"ART2_Stable",       UPDATE_FUNC, 5, 0, (FunctionPtr) &SnnsCLib::UPDATE_ART2_Propagate},
{"ART2_Synchronous",  UPDATE_FUNC, 5, 0, (FunctionPtr) &SnnsCLib::UPDATE_ART2_syncPropagate},
{"ARTMAP_Stable",     UPDATE_FUNC, 3, 0, (FunctionPtr) &SnnsCLib::UPDATE_ARTMAP_Propagate},
{"ARTMAP_Synchronous",UPDATE_FUNC, 3, 0, (FunctionPtr) &SnnsCLib::UPDATE_ARTMAP_syncPropagate},
{"Auto_Synchronous",    UPDATE_FUNC, 1, 0, (FunctionPtr) &SnnsCLib::UPDATE_RM_Propagate},
{"BAM_Order",         UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_BAM},
{"BPTT_Order",        UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_BPTT},
{"CC_Order",          UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_CC_Propagate},
{"CounterPropagation",UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_CPNPropagate},
{"Dynamic_LVQ",       UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_DLVQ_Propagate},
{"Hopfield_Fixed_Act",  UPDATE_FUNC, 1, 0, (FunctionPtr) &SnnsCLib::UPDATE_FixAct_Hop},
{"Hopfield_Synchronous",UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_syncPropagateHop},
{"JE_Order"  ,        UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_JE_Propagate},
{"JE_Special",        UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_JE_Special},
{"Kohonen_Order",     UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_KohonenPropagate},
{"Random_Order",      UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_randomOrderPropagate},
{"Random_Permutation",UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_randomPermutPropagate},
{"Serial_Order",      UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_serialPropagate},
{"Synchonous_Order",  UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_syncPropagate},
{"TimeDelay_Order",   UPDATE_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::UPDATE_TimeDelayPropagate},
{"ENZO_prop",         UPDATE_FUNC, 1, 0, (FunctionPtr) &SnnsCLib::ENZO_propagate},

/*  Leaning Functions  */
/*  This is the DEFAULT FUNCTION  */
{"Std_Backpropagation", LEARN_FUNC | DEFAULT_FUNC | FF_LEARN_FUNC, 2, 1, (FunctionPtr) &SnnsCLib::LEARN_backprop},

{"ART1",                LEARN_FUNC, 1, 0, (FunctionPtr) &SnnsCLib::LEARN_ART1},
{"ART2",                LEARN_FUNC, 5, 0, (FunctionPtr) &SnnsCLib::LEARN_ART2},
{"ARTMAP",              LEARN_FUNC, 3, 0, (FunctionPtr) &SnnsCLib::LEARN_ARTMAP},
{"BackPercolation",     LEARN_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::LEARN_perc},
{"BackpropBatch",       LEARN_FUNC | FF_LEARN_FUNC, 2, 1, (FunctionPtr) &SnnsCLib::LEARN_backpropBatch},
{"BackpropChunk",       LEARN_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::LEARN_backpropChunk},
{"BackpropClassJogChunk", LEARN_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::LEARN_backpropClassJogChunk},
{"BackpropJogChunk",    LEARN_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::LEARN_backpropJogChunk},
{"BackpropMomentum",    LEARN_FUNC | FF_LEARN_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::LEARN_backpropMomentum},
{"BackpropWeightDecay", LEARN_FUNC | FF_LEARN_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::LEARN_backpropWeightDecay},
{"BPTT",                LEARN_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::LEARN_BPTT},
{"BBPTT",               LEARN_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::LEARN_BBPTT},
{"CC",                  LEARN_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::LEARN_CC},
{"Counterpropagation",  LEARN_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::LEARN_CPN},
{"Dynamic_LVQ",         LEARN_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::LEARN_DLVQ}, /* No Of params must be 5 !!! Otherwise batchman will not work for this function */
{"Hebbian",             LEARN_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::LEARN_HEBB},
{"JE_BP",               LEARN_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::LEARN_JE_Backprop},
{"JE_BP_Momentum",      LEARN_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::LEARN_JE_BackpropMomentum},
{"JE_Quickprop",        LEARN_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::LEARN_JE_Quickprop},
{"JE_Rprop",            LEARN_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::LEARN_JE_Rprop},
{"Kohonen",             LEARN_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::LEARN_kohonen},
{"Monte-Carlo",         LEARN_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::LEARN_MonteCarlo},
{"PruningFeedForward",  LEARN_FUNC, 5, 1, (FunctionPtr) NULL},
{"QPTT",                LEARN_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::LEARN_QPTT},
{"Quickprop",           LEARN_FUNC | FF_LEARN_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::LEARN_quickprop},
{"RadialBasisLearning", LEARN_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::LEARN_RBF},
{"RBF-DDA",             LEARN_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::LEARN_RBF_DDA},
{"RM_delta",            LEARN_FUNC, 2, 1, (FunctionPtr) &SnnsCLib::LEARN_RM_delta},
{"Rprop",               LEARN_FUNC | FF_LEARN_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::LEARN_rprop},
{"RpropMAP",            LEARN_FUNC | FF_LEARN_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::LEARN_RpropMAP},
{"SCG",                 LEARN_FUNC | FF_LEARN_FUNC, 4, 2, (FunctionPtr) &SnnsCLib::LEARN_SCG},
{"Sim_Ann_SS", LEARN_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::LEARN_SimulatedAnnealingSS},
{"Sim_Ann_WTA", LEARN_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::LEARN_SimulatedAnnealingWTA},
{"Sim_Ann_WWTA", LEARN_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::LEARN_SimulatedAnnealingWWTA},
{"TACOMA",              LEARN_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::LEARN_Tacoma},
{"TimeDelayBackprop",   LEARN_FUNC, 2, 1, (FunctionPtr) &SnnsCLib::LEARN_TDbackprop},

/*  Testing Functions  */
/*  This is the DEFAULT FUNCTION  */
{"Std_Backpropagation", TEST_FUNC | DEFAULT_FUNC, 2, 1, (FunctionPtr) &SnnsCLib::TEST_backprop},
/* {"ART1",                TEST_FUNC, 1, 0, (FunctionPtr) &SnnsCLib::TEST_ART1}, */
 {"BackpropBatch",       TEST_FUNC, 2, 1, (FunctionPtr) &SnnsCLib::TEST_backprop},
/* {"ART2",                TEST_FUNC, 5, 0, (FunctionPtr) &SnnsCLib::TEST_ART2}, */
{"BackpropChunk",       TEST_FUNC, 2, 1, (FunctionPtr) &SnnsCLib::TEST_backprop},
{"BackpropClassJogChunk",TEST_FUNC, 2, 1, (FunctionPtr) &SnnsCLib::TEST_backprop},
{"BackpropMomentum",    TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_backpropMomentum},
/* {"ARTMAP",              TEST_FUNC, 3, 0, (FunctionPtr) &SnnsCLib::TEST_ARTMAP}, */
{"BackpropWeightDecay", TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_backpropMomentum},
/* {"BackPercolation",     TEST_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::TEST_perc}, 
{"PruningFeedForward",  TEST_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::NULL},
{"Hebbian",             TEST_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::TEST_HEBB}, */
{"TimeDelayBackprop",   TEST_FUNC, 2, 1, (FunctionPtr) &SnnsCLib::TEST_TDbackprop},
/* {"RM_delta",            TEST_FUNC, 2, 1, (FunctionPtr) &SnnsCLib::TEST_RM_delta},*/
{"Quickprop",           TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_backpropMomentum},
/* {"Counterpropagation",  TEST_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::TEST_CPN},*/
{"Rprop",               TEST_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::TEST_rprop},
{"RpropMAP",          TEST_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::TEST_MAP},
/* {"RadialBasislearning", TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_RBF},
{"Dynamic_LVQ",         TEST_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::TEST_DLVQ},*/
{"BPTT",                TEST_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::TEST_BPTT},
{"CC",                  TEST_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::TEST_CasCor},
{"TACOMA",              TEST_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::TEST_CasCor},
{"BBPTT",               TEST_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::TEST_BPTT},
{"QPTT",                TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_BPTT},
/*{"Kohonen",             TEST_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::TEST_kohonen},*/
{"JE_BP",               TEST_FUNC, 3, 1, (FunctionPtr) &SnnsCLib::TEST_JE_Backprop},
{"JE_BP_Momentum",      TEST_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::TEST_JE_BackpropMomentum},
{"JE_Quickprop",        TEST_FUNC, 5, 1, (FunctionPtr) &SnnsCLib::TEST_JE_BackpropMomentum},
{"JE_Rprop",            TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_JE_Rprop}, 
{"Monte-Carlo",         TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_MonteCarlo}, 
{"SCG",                 TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_SCG},
{"Sim_Ann_SS", TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_SimulatedAnnealingSS},
{"Sim_Ann_WTA", TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_SimulatedAnnealingWTA},
{"Sim_Ann_WWTA", TEST_FUNC, 4, 1, (FunctionPtr) &SnnsCLib::TEST_SimulatedAnnealingWWTA},

/*  Initialisation Functions  */
/*  This is the DEFAULT FUNCTION  */
{"Randomize_Weights",   INIT_FUNC | DEFAULT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_randomizeWeights},

{"ART1_Weights",        INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_Weights_ART1},
{"ART2_Weights",        INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_Weights_ART2},
{"ARTMAP_Weights",      INIT_FUNC, 4, 0, (FunctionPtr) &SnnsCLib::INIT_Weights_ARTMAP},
{"CC_Weights",          INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_CC_Weights},
{"ClippHebb",           INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_ClippHebb},
{"CPN_Weights_v3.2",    INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_Weights_CPNv32},
{"CPN_Weights_v3.3",    INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_Weights_CPNv33},
{"CPN_Rand_Pat",        INIT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::INIT_Weights_CPN_Rand_Pat},
{"DLVQ_Weights",        INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_CC_Weights},
{"Hebb",                INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_Hebb}, 
{"Hebb_Fixed_Act",      INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_HOP_FixAct},
{"JE_Weights",          INIT_FUNC, 5, 0, (FunctionPtr) &SnnsCLib::INIT_JE_Weights},
{"Kohonen_Rand_Pat",    INIT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::INIT_SOM_Rand_Pat},
{"Kohonen_Weights_v3.2",INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_SOM_Weights_v32},
{"Kohonen_Const",       INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_SOM_Weights_const},
{"PseudoInv",           INIT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::INIT_PseudoInv},
{"Random_Weights_Perc", INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_randomizeWeights_perc},
{"RBF_Weights",         INIT_FUNC, 5, 0, (FunctionPtr) &SnnsCLib::INIT_RBF_Weights},
{"RBF_Weights_Kohonen", INIT_FUNC, 3, 0, (FunctionPtr) &SnnsCLib::INIT_RBF_Weights_kohonen},
{"RBF_Weights_Redo",    INIT_FUNC, 3, 0, (FunctionPtr) &SnnsCLib::INIT_RBF_Weights_redo},
{"RM_Random_Weights",   INIT_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::INIT_RM_randomizeWeights},
{"ENZO_noinit",         INIT_FUNC,  0, 0, (FunctionPtr) &SnnsCLib::ENZO_noinit},

/* Pruning Functions */
/*  This is the DEFAULT FUNCTION  */
{"MagPruning", PRUNING_FUNC | DEFAULT_FUNC, 1, 1, (FunctionPtr) &SnnsCLib::PRUNE_Mag},

{"OptimalBrainDamage", PRUNING_FUNC, 1, 1, (FunctionPtr) &SnnsCLib::PRUNE_OBD},
{"OptimalBrainSurgeon", PRUNING_FUNC, 1, 1, (FunctionPtr) &SnnsCLib::PRUNE_OBS},
{"Skeletonization", PRUNING_FUNC, 1, 1, (FunctionPtr) &SnnsCLib::PRUNE_Skeletonization},
{"Noncontributing_Units", PRUNING_FUNC, 1, 1, (FunctionPtr) &SnnsCLib::PRUNE_Noncontributing},

/* Remap Functions */
/*  This is the DEFAULT FUNCTION  */
{"None", REMAP_FUNC | DEFAULT_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::REMAP_none},
{"Binary", REMAP_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::REMAP_binary},
{"Inverse", REMAP_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::REMAP_invers},
{"Clip", REMAP_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::REMAP_clip},
{"LinearScale", REMAP_FUNC, 2, 0, (FunctionPtr) &SnnsCLib::REMAP_linearscale},
{"Norm", REMAP_FUNC, 0, 0, (FunctionPtr) &SnnsCLib::REMAP_norm},
{"Threshold", REMAP_FUNC, 4, 0, (FunctionPtr) &SnnsCLib::REMAP_threshold}
  };


/*  Define the function table size
*/
//const int  NoOfKernelFuncs = (sizeof (kernel_func_table) / sizeof (struct FuncTable));
const int  SnnsCLib::NoOfKernelFuncs = (sizeof (kernel_func_table) / sizeof (struct FuncTable));
/*  **********************************************************************  */




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
#include <config.h>
#include <stdio.h>

#include "kr_typ.h"	    /*	Kernel Types and Constants  */
#include "kr_const.h"       /*  Constant Declarators for SNNS-Kernel  */
#include "learn_f.h"
#include "init_f.h"
#include "trans_f.h"
#include "remap_f.h"
#include "update_f.h"
#include "arttr_f.h"
#include "tbl_func.h"
#include "kr_td.h"
#include "cc_learn.h"
#include "tacoma_learn.h"
#include "cc_type.h"
#include "dlvq_learn.h"
#include "prun_f.h"
#include "stochastic_learn_f.h" 
#include "scaled_conj_grad.h"

#include "func_tbl.ph"

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
struct FuncTable  kernel_func_table[]	=
  {

/*  Output functions  */
/*  In 'kr_const.h' is defined that the NULL-pointer presents the
    identity function. So the "Out_Identity" function is represented
    by the NULL-pointer instead of the function pointer.
*/
/*  This is the DEFAULT FUNCTION  */
{"Out_Identity",     OUT_FUNC | DEFAULT_FUNC, 0, 0, (FunctionPtr) OUT_IDENTITY},

{"Out_Threshold05",  OUT_FUNC, 0, 0, (FunctionPtr) OUT_Threshold05},
{"Out_Clip_01",      OUT_FUNC, 0, 0, (FunctionPtr) OUT_Clip_01},
{"Out_Clip_11",      OUT_FUNC, 0, 0, (FunctionPtr) OUT_Clip_11},
{"Out_ART2_Noise_PLin", OUT_FUNC, 0, 0, (FunctionPtr) OUT_ART2_Noise_PLin},
{"Out_ART2_Noise_ContDiff", OUT_FUNC, 0, 0, (FunctionPtr) OUT_ART2_Noise_ContDiff},
{"Out_Custom_Python",OUT_FUNC, 0, 0, (FunctionPtr) OUT_Custom_Python},

/*  Activation functions  */
/*  This is the DEFAULT FUNCTION  */
{"Act_Logistic",     ACT_FUNC | DEFAULT_FUNC, 0, 0, (FunctionPtr) ACT_Logistic},
{"Act_Elliott",      ACT_FUNC, 0, 0, (FunctionPtr) ACT_Elliott},
{"Act_BSB",          ACT_FUNC, 0, 0, (FunctionPtr) ACT_BSBFunction},
{"Act_TanH",         ACT_FUNC, 0, 0, (FunctionPtr) ACT_TanHFunction},
{"Act_TanH_Xdiv2",   ACT_FUNC, 0, 0, (FunctionPtr) ACT_TanHFunction_Xdiv2},
{"Act_Perceptron",   ACT_FUNC, 0, 0, (FunctionPtr) ACT_Perceptron},
{"Act_Signum",       ACT_FUNC, 0, 0, (FunctionPtr) ACT_Signum},
{"Act_Signum0",      ACT_FUNC, 0, 0, (FunctionPtr) ACT_Signum0},
{"Act_Softmax",     ACT_FUNC ,0, 0, (FunctionPtr) ACT_Softmax},
{"Act_StepFunc",     ACT_FUNC, 0, 0, (FunctionPtr) ACT_StepFunction},
{"Act_HystStep",     ACT_FUNC, 0, 0, (FunctionPtr) ACT_HystStepFunction},
{"Act_BAM",          ACT_FUNC, 0, 0, (FunctionPtr) ACT_BAMFunction},
{"Logistic_notInhibit" , ACT_FUNC, 0, 0, (FunctionPtr) ACT_LogisticI},
{"Act_MinOutPlusWeight", ACT_FUNC, 0, 0, (FunctionPtr) ACT_MinOutPlusWeight},
{"Act_Identity",     ACT_FUNC, 0, 0, (FunctionPtr) ACT_Linear},
{"Act_IdentityPlusBias", ACT_FUNC, 0, 0, (FunctionPtr) ACT_Linear_bias},
{"Act_LogisticTbl",  ACT_FUNC, 0, 0, (FunctionPtr) ACT_LogisticTbl},
{"Act_RBF_Gaussian", ACT_FUNC, 0, 0, (FunctionPtr) ACT_RBF_Gaussian},
{"Act_RBF_MultiQuadratic", ACT_FUNC, 0, 0, (FunctionPtr) ACT_RBF_Multiquadratic},
{"Act_RBF_ThinPlateSpline", ACT_FUNC, 0, 0, (FunctionPtr) ACT_RBF_Thinplatespline},
{"Act_less_than_0",  ACT_FUNC, 0, 0, (FunctionPtr) ACT_less_than_0},
{"Act_at_most_0",    ACT_FUNC, 0, 0, (FunctionPtr) ACT_at_most_0},
{"Act_at_least_2",   ACT_FUNC, 0, 0, (FunctionPtr) ACT_at_least_2},
{"Act_at_least_1",   ACT_FUNC, 0, 0, (FunctionPtr) ACT_at_least_1},
{"Act_exactly_1",    ACT_FUNC, 0, 0, (FunctionPtr) ACT_exactly_1},
{"Act_Product",      ACT_FUNC, 0, 0, (FunctionPtr) ACT_Product},

{"Act_ART1_NC",      ACT_FUNC, 0, 0, (FunctionPtr) ACT_ART1_NC},
{"Act_ART2_Identity",ACT_FUNC, 0, 0, (FunctionPtr) ACT_ART2_Linear},
{"Act_ART2_NormP",   ACT_FUNC, 0, 0, (FunctionPtr) ACT_ART2_NormP},
{"Act_ART2_NormV",   ACT_FUNC, 0, 0, (FunctionPtr) ACT_ART2_NormV},
{"Act_ART2_NormW",   ACT_FUNC, 0, 0, (FunctionPtr) ACT_ART2_NormW},
{"Act_ART2_NormIP",  ACT_FUNC, 0, 0, (FunctionPtr) ACT_ART2_NormIP},
{"Act_ART2_Rec",     ACT_FUNC, 0, 0, (FunctionPtr) ACT_ART2_Rec},
{"Act_ART2_Rst",     ACT_FUNC, 0, 0, (FunctionPtr) ACT_ART2_Rst},
{"Act_ARTMAP_NCa",   ACT_FUNC, 0, 0, (FunctionPtr) ACT_ARTMAP_NCa},
{"Act_ARTMAP_NCb",   ACT_FUNC, 0, 0, (FunctionPtr) ACT_ARTMAP_NCb},
{"Act_ARTMAP_DRho",  ACT_FUNC, 0, 0, (FunctionPtr) ACT_ARTMAP_DRho},
{"Act_LogSym",       ACT_FUNC, 0, 0, (FunctionPtr) ACT_LogisticSym},
{"Act_CC_Thresh",    ACT_FUNC, 0, 0, (FunctionPtr) ACT_CC_Threshold},
{"Act_Sinus",        ACT_FUNC, 0, 0, (FunctionPtr) ACT_SIN},
{"Act_Exponential",  ACT_FUNC, 0, 0, (FunctionPtr) ACT_EXPONENT},
{"Act_TD_Logistic",  ACT_FUNC, 0, 0, (FunctionPtr) ACT_TD_Logistic},
{"Act_TD_Elliott",   ACT_FUNC, 0, 0, (FunctionPtr) ACT_TD_Elliott},
{"Act_Euclid",       ACT_FUNC, 0, 0, (FunctionPtr) ACT_Euclid},
{"Act_Component",    ACT_FUNC, 0, 0, (FunctionPtr) ACT_Component},
{"Act_RM",	     ACT_FUNC, 0, 0, (FunctionPtr) ACT_RM}, 
{"Act_TACOMA",       ACT_FUNC, 0, 0, (FunctionPtr) ACT_TACOMA},
{"Act_Custom_Python",       ACT_FUNC, 0, 0, (FunctionPtr) ACT_Custom_Python},


/*  Derivation functions of the activation functions
    NOTE: The derivative function of the activation function MUST have
	  the same name as the belonging activation function.
*/
{"Act_Logistic",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Logistic},
{"Act_Elliott",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Elliott},
{"Act_BSB",          ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_BSBFunction},
{"Act_TanH",         ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_TanHFunction},
{"Act_TanH_Xdiv2",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_TanHFunction_Xdiv2},
{"Act_Perceptron",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_Signum",       ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_Signum0",      ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_Softmax",     ACT_DERIV_FUNC ,0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_StepFunc",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_HystStep",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_BAM",          ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Logistic_notInhibit",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_MinOutPlusWeight", ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_Identity",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Identity},
{"Act_IdentityPlusBias", ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Identity},
{"Act_LogisticTbl",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Logistic},
{"Act_RBF_Gaussian", ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_RBF_Gaussian},
{"Act_RBF_MultiQuadratic", ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_RBF_Multiquadratic},
{"Act_RBF_ThinPlateSpline", ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_RBF_Thinplatespline},
{"Act_less_than_0",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_at_most_0",    ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_at_least_1",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_at_least_2",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_exactly_1",    ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_Product",      ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ART1_NC",      ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ART2_Identity",ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ART2_NormP",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ART2_NormV",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ART2_NormW",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ART2_NormIP",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ART2_Rec",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ART2_Rst",     ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ARTMAP_NCa",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ARTMAP_NCb",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_ARTMAP_DRho",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_LogSym",       ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_LogisticSym},
{"Act_CC_Thresh",    ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_CC_Threshold},
{"Act_Sinus",        ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_SIN},
{"Act_Exponential",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_EXPONENT},
{"Act_TD_Logistic",  ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_TD_Logistic},
{"Act_TD_Elliott",   ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_TD_Elliott},
{"Act_Euclid",       ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_Component",    ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy},
{"Act_RM",           ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Dummy}, 
{"Act_TACOMA",       ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_TACOMA},
{"Act_Custom_Python",       ACT_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_DERIV_Custom_Python},

/*  Second derivation functions of the activation functions
    NOTE: The second derivative function of the activation function
    MUST have the same name as the belonging activation function.
*/
{"Act_Logistic",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Logistic},
{"Act_Elliott",             ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Elliott},
{"Act_BSB",                 ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_BSBFunction},
{"Act_TanH",                ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_TanHFunction},
{"Act_TanHPlusBias",        ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_TanHFunction},
{"Act_TanH_Xdiv2",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_TanHFunction_Xdiv2},
{"Act_Perceptron",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_Signum",              ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_Signum0",             ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_Softmax",             ACT_2_DERIV_FUNC ,0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_StepFunc",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_HystStep",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_BAM",                 ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Logistic_notInhibit",     ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_MinOutPlusWeight",    ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_Identity",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Identity},
{"Act_IdentityPlusBias",    ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Identity},
{"Act_LogisticTbl",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Logistic},
{"Act_RBF_Gaussian",        ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_RBF_MultiQuadratic",  ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_RBF_ThinPlateSpline", ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_less_than_0",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_at_most_0",           ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_at_least_1",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_at_least_2",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_exactly_1",           ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_Product",             ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ART1_NC",             ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ART2_Identity",       ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ART2_NormP",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ART2_NormV",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ART2_NormW",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ART2_NormIP",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ART2_Rec",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ART2_Rst",            ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ARTMAP_NCa",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ARTMAP_NCb",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_ARTMAP_DRho",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_LogSym",              ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_TanHFunction},
{"Act_TD_Logistic",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_TD_Elliott",          ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_Euclid",              ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_Component",           ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_RM",                  ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy}, 
{"Act_TACOMA",              ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_CC_Thresh",           ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_Sinus",               ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_Exponential",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},
{"Act_Custom_Python",         ACT_2_DERIV_FUNC, 0, 0, (FunctionPtr) ACT_2_DERIV_Dummy},


/*  Site functions  */
{"Site_WeightedSum", SITE_FUNC | DEFAULT_FUNC, 0, 0, (FunctionPtr) SITE_WeightedSum},
{"Site_Pi",          SITE_FUNC, 0, 0, (FunctionPtr) SITE_ProductA},
{"Site_Produkt",     SITE_FUNC, 0, 0, (FunctionPtr) SITE_Product},
{"Site_Max",         SITE_FUNC, 0, 0, (FunctionPtr) SITE_Max},
{"Site_Min",         SITE_FUNC, 0, 0, (FunctionPtr) SITE_Min},
{"Site_at_least_2",  SITE_FUNC, 0, 0, (FunctionPtr) SITE_at_least_2},
{"Site_at_least_1",  SITE_FUNC, 0, 0, (FunctionPtr) SITE_at_least_1},
{"Site_at_most_0",   SITE_FUNC, 0, 0, (FunctionPtr) SITE_at_most_0},
{"Site_Reciprocal",  SITE_FUNC, 0, 0, (FunctionPtr) SITE_Reciprocal_WeightedSum},

/*  Update functions  */
/*  This is the DEFAULT FUNCTION  */
{"Topological_Order", UPDATE_FUNC | DEFAULT_FUNC, 0, 0, (FunctionPtr) UPDATE_topologicalPropagate},
{"ART1_Stable",       UPDATE_FUNC, 1, 0, (FunctionPtr) UPDATE_ART1_Propagate},
{"ART1_Synchronous",  UPDATE_FUNC, 1, 0, (FunctionPtr) UPDATE_ART1_syncPropagate},
{"ART2_Stable",       UPDATE_FUNC, 5, 0, (FunctionPtr) UPDATE_ART2_Propagate},
{"ART2_Synchronous",  UPDATE_FUNC, 5, 0, (FunctionPtr) UPDATE_ART2_syncPropagate},
{"ARTMAP_Stable",     UPDATE_FUNC, 3, 0, (FunctionPtr) UPDATE_ARTMAP_Propagate},
{"ARTMAP_Synchronous",UPDATE_FUNC, 3, 0, (FunctionPtr) UPDATE_ARTMAP_syncPropagate},
{"Auto_Synchronous",    UPDATE_FUNC, 1, 0, (FunctionPtr) UPDATE_RM_Propagate},
{"BAM_Order",         UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_BAM},
{"BPTT_Order",        UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_BPTT},
{"CC_Order",          UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_CC_Propagate},
{"CounterPropagation",UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_CPNPropagate},
{"Dynamic_LVQ",       UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_DLVQ_Propagate},
{"Hopfield_Fixed_Act",  UPDATE_FUNC, 1, 0, (FunctionPtr) UPDATE_FixAct_Hop},
{"Hopfield_Synchronous",UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_syncPropagateHop},
{"JE_Order"  ,        UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_JE_Propagate},
{"JE_Special",        UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_JE_Special},
{"Kohonen_Order",     UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_KohonenPropagate},
{"Random_Order",      UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_randomOrderPropagate},
{"Random_Permutation",UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_randomPermutPropagate},
{"Serial_Order",      UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_serialPropagate},
{"Synchonous_Order",  UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_syncPropagate},
{"TimeDelay_Order",   UPDATE_FUNC, 0, 0, (FunctionPtr) UPDATE_TimeDelayPropagate},
{"ENZO_prop",         UPDATE_FUNC, 1, 0, (FunctionPtr) ENZO_propagate},

/*  Leaning Functions  */
/*  This is the DEFAULT FUNCTION  */
{"Std_Backpropagation", LEARN_FUNC | DEFAULT_FUNC | FF_LEARN_FUNC, 2, 1, (FunctionPtr) LEARN_backprop},

{"ART1",                LEARN_FUNC, 1, 0, (FunctionPtr) LEARN_ART1},
{"ART2",                LEARN_FUNC, 5, 0, (FunctionPtr) LEARN_ART2},
{"ARTMAP",              LEARN_FUNC, 3, 0, (FunctionPtr) LEARN_ARTMAP},
{"BackPercolation",     LEARN_FUNC, 3, 1, (FunctionPtr) LEARN_perc},
{"BackpropBatch",       LEARN_FUNC | FF_LEARN_FUNC, 2, 1, (FunctionPtr) LEARN_backpropBatch},
{"BackpropChunk",       LEARN_FUNC, 3, 1, (FunctionPtr) LEARN_backpropChunk},
{"BackpropClassJogChunk", LEARN_FUNC, 5, 1, (FunctionPtr) LEARN_backpropClassJogChunk},
{"BackpropJogChunk",    LEARN_FUNC, 5, 1, (FunctionPtr) LEARN_backpropJogChunk},
{"BackpropMomentum",    LEARN_FUNC | FF_LEARN_FUNC, 4, 1, (FunctionPtr) LEARN_backpropMomentum},
{"BackpropWeightDecay", LEARN_FUNC | FF_LEARN_FUNC, 4, 1, (FunctionPtr) LEARN_backpropWeightDecay},
{"BPTT",                LEARN_FUNC, 3, 1, (FunctionPtr) LEARN_BPTT},
{"BBPTT",               LEARN_FUNC, 3, 1, (FunctionPtr) LEARN_BBPTT},
{"CC",                  LEARN_FUNC, 5, 1, (FunctionPtr) LEARN_CC},
{"Counterpropagation",  LEARN_FUNC, 3, 1, (FunctionPtr) LEARN_CPN},
{"Dynamic_LVQ",         LEARN_FUNC, 5, 1, (FunctionPtr) LEARN_DLVQ}, /* No Of params must be 5 !!! Otherwise batchman will not work for this function */
{"Hebbian",             LEARN_FUNC, 3, 1, (FunctionPtr) LEARN_HEBB},
{"JE_BP",               LEARN_FUNC, 3, 1, (FunctionPtr) LEARN_JE_Backprop},
{"JE_BP_Momentum",      LEARN_FUNC, 5, 1, (FunctionPtr) LEARN_JE_BackpropMomentum},
{"JE_Quickprop",        LEARN_FUNC, 5, 1, (FunctionPtr) LEARN_JE_Quickprop},
{"JE_Rprop",            LEARN_FUNC, 4, 1, (FunctionPtr) LEARN_JE_Rprop},
{"Kohonen",             LEARN_FUNC, 5, 1, (FunctionPtr) LEARN_kohonen},
{"Monte-Carlo",         LEARN_FUNC, 4, 1, (FunctionPtr) LEARN_MonteCarlo},
{"PruningFeedForward",  LEARN_FUNC, 5, 1, (FunctionPtr) NULL},
{"QPTT",                LEARN_FUNC, 4, 1, (FunctionPtr) LEARN_QPTT},
{"Quickprop",           LEARN_FUNC | FF_LEARN_FUNC, 4, 1, (FunctionPtr) LEARN_quickprop},
{"RadialBasisLearning", LEARN_FUNC, 5, 1, (FunctionPtr) LEARN_RBF},
{"RBF-DDA",             LEARN_FUNC, 3, 1, (FunctionPtr) LEARN_RBF_DDA},
{"RM_delta",            LEARN_FUNC, 2, 1, (FunctionPtr) LEARN_RM_delta},
{"Rprop",               LEARN_FUNC | FF_LEARN_FUNC, 3, 1, (FunctionPtr) LEARN_rprop},
{"RpropMAP",            LEARN_FUNC | FF_LEARN_FUNC, 5, 1, (FunctionPtr) LEARN_RpropMAP},
{"SCG",                 LEARN_FUNC | FF_LEARN_FUNC, 4, 2, (FunctionPtr) LEARN_SCG},
{"Sim_Ann_SS", LEARN_FUNC, 4, 1, (FunctionPtr) LEARN_SimulatedAnnealingSS},
{"Sim_Ann_WTA", LEARN_FUNC, 4, 1, (FunctionPtr) LEARN_SimulatedAnnealingWTA},
{"Sim_Ann_WWTA", LEARN_FUNC, 4, 1, (FunctionPtr) LEARN_SimulatedAnnealingWWTA},
{"TACOMA",              LEARN_FUNC, 5, 1, (FunctionPtr) LEARN_Tacoma},
{"TimeDelayBackprop",   LEARN_FUNC, 2, 1, (FunctionPtr) LEARN_TDbackprop},

/*  Testing Functions  */
/*  This is the DEFAULT FUNCTION  */
{"Std_Backpropagation", TEST_FUNC | DEFAULT_FUNC, 2, 1, (FunctionPtr) TEST_backprop},
/* {"ART1",                TEST_FUNC, 1, 0, (FunctionPtr) TEST_ART1}, */
 {"BackpropBatch",       TEST_FUNC, 2, 1, (FunctionPtr) TEST_backprop},
/* {"ART2",                TEST_FUNC, 5, 0, (FunctionPtr) TEST_ART2}, */
{"BackpropChunk",       TEST_FUNC, 2, 1, (FunctionPtr) TEST_backprop},
{"BackpropClassJogChunk",TEST_FUNC, 2, 1, (FunctionPtr) TEST_backprop},
{"BackpropMomentum",    TEST_FUNC, 4, 1, (FunctionPtr) TEST_backpropMomentum},
/* {"ARTMAP",              TEST_FUNC, 3, 0, (FunctionPtr) TEST_ARTMAP}, */
{"BackpropWeightDecay", TEST_FUNC, 4, 1, (FunctionPtr) TEST_backpropMomentum},
/* {"BackPercolation",     TEST_FUNC, 3, 1, (FunctionPtr) TEST_perc}, 
{"PruningFeedForward",  TEST_FUNC, 5, 1, (FunctionPtr) NULL},
{"Hebbian",             TEST_FUNC, 3, 1, (FunctionPtr) TEST_HEBB}, */
{"TimeDelayBackprop",   TEST_FUNC, 2, 1, (FunctionPtr) TEST_TDbackprop},
/* {"RM_delta",            TEST_FUNC, 2, 1, (FunctionPtr) TEST_RM_delta},*/
{"Quickprop",           TEST_FUNC, 4, 1, (FunctionPtr) TEST_backpropMomentum},
/* {"Counterpropagation",  TEST_FUNC, 3, 1, (FunctionPtr) TEST_CPN},*/
{"Rprop",               TEST_FUNC, 3, 1, (FunctionPtr) TEST_rprop},
{"RpropMAP",          TEST_FUNC, 5, 1, (FunctionPtr) TEST_MAP},
/* {"RadialBasislearning", TEST_FUNC, 4, 1, (FunctionPtr) TEST_RBF},
{"Dynamic_LVQ",         TEST_FUNC, 5, 1, (FunctionPtr) TEST_DLVQ},*/
{"BPTT",                TEST_FUNC, 3, 1, (FunctionPtr) TEST_BPTT},
{"CC",                  TEST_FUNC, 5, 1, (FunctionPtr) TEST_CasCor},
{"TACOMA",              TEST_FUNC, 5, 1, (FunctionPtr) TEST_CasCor},
{"BBPTT",               TEST_FUNC, 3, 1, (FunctionPtr) TEST_BPTT},
{"QPTT",                TEST_FUNC, 4, 1, (FunctionPtr) TEST_BPTT},
/*{"Kohonen",             TEST_FUNC, 5, 1, (FunctionPtr) TEST_kohonen},*/
{"JE_BP",               TEST_FUNC, 3, 1, (FunctionPtr) TEST_JE_Backprop},
{"JE_BP_Momentum",      TEST_FUNC, 5, 1, (FunctionPtr) TEST_JE_BackpropMomentum},
{"JE_Quickprop",        TEST_FUNC, 5, 1, (FunctionPtr) TEST_JE_BackpropMomentum},
{"JE_Rprop",            TEST_FUNC, 4, 1, (FunctionPtr) TEST_JE_Rprop}, 
{"Monte-Carlo",         TEST_FUNC, 4, 1, (FunctionPtr) TEST_MonteCarlo}, 
{"SCG",                 TEST_FUNC, 4, 1, (FunctionPtr) TEST_SCG},
{"Sim_Ann_SS", TEST_FUNC, 4, 1, (FunctionPtr) TEST_SimulatedAnnealingSS},
{"Sim_Ann_WTA", TEST_FUNC, 4, 1, (FunctionPtr) TEST_SimulatedAnnealingWTA},
{"Sim_Ann_WWTA", TEST_FUNC, 4, 1, (FunctionPtr) TEST_SimulatedAnnealingWWTA},

/*  Initialisation Functions  */
/*  This is the DEFAULT FUNCTION  */
{"Randomize_Weights",   INIT_FUNC | DEFAULT_FUNC, 2, 0, (FunctionPtr) INIT_randomizeWeights},

{"ART1_Weights",        INIT_FUNC, 2, 0, (FunctionPtr) INIT_Weights_ART1},
{"ART2_Weights",        INIT_FUNC, 2, 0, (FunctionPtr) INIT_Weights_ART2},
{"ARTMAP_Weights",      INIT_FUNC, 4, 0, (FunctionPtr) INIT_Weights_ARTMAP},
{"CC_Weights",          INIT_FUNC, 2, 0, (FunctionPtr) INIT_CC_Weights},
{"ClippHebb",           INIT_FUNC, 2, 0, (FunctionPtr) INIT_ClippHebb},
{"CPN_Weights_v3.2",    INIT_FUNC, 2, 0, (FunctionPtr) INIT_Weights_CPNv32},
{"CPN_Weights_v3.3",    INIT_FUNC, 2, 0, (FunctionPtr) INIT_Weights_CPNv33},
{"CPN_Rand_Pat",        INIT_FUNC, 0, 0, (FunctionPtr) INIT_Weights_CPN_Rand_Pat},
{"DLVQ_Weights",        INIT_FUNC, 2, 0, (FunctionPtr) INIT_CC_Weights},
{"Hebb",                INIT_FUNC, 2, 0, (FunctionPtr) INIT_Hebb}, 
{"Hebb_Fixed_Act",      INIT_FUNC, 2, 0, (FunctionPtr) INIT_HOP_FixAct},
{"JE_Weights",          INIT_FUNC, 5, 0, (FunctionPtr) INIT_JE_Weights},
{"Kohonen_Rand_Pat",    INIT_FUNC, 0, 0, (FunctionPtr) INIT_SOM_Rand_Pat},
{"Kohonen_Weights_v3.2",INIT_FUNC, 2, 0, (FunctionPtr) INIT_SOM_Weights_v32},
{"Kohonen_Const",       INIT_FUNC, 2, 0, (FunctionPtr) INIT_SOM_Weights_const},
{"PseudoInv",           INIT_FUNC, 0, 0, (FunctionPtr) INIT_PseudoInv},
{"Random_Weights_Perc", INIT_FUNC, 2, 0, (FunctionPtr) INIT_randomizeWeights_perc},
{"RBF_Weights",         INIT_FUNC, 5, 0, (FunctionPtr) INIT_RBF_Weights},
{"RBF_Weights_Kohonen", INIT_FUNC, 3, 0, (FunctionPtr) INIT_RBF_Weights_kohonen},
{"RBF_Weights_Redo",    INIT_FUNC, 3, 0, (FunctionPtr) INIT_RBF_Weights_redo},
{"RM_Random_Weights",   INIT_FUNC, 2, 0, (FunctionPtr) INIT_RM_randomizeWeights},
{"ENZO_noinit",         INIT_FUNC,  0, 0, (FunctionPtr) ENZO_noinit},

/* Pruning Functions */
/*  This is the DEFAULT FUNCTION  */
{"MagPruning", PRUNING_FUNC | DEFAULT_FUNC, 1, 1, (FunctionPtr) PRUNE_Mag},

{"OptimalBrainDamage", PRUNING_FUNC, 1, 1, (FunctionPtr) PRUNE_OBD},
{"OptimalBrainSurgeon", PRUNING_FUNC, 1, 1, (FunctionPtr) PRUNE_OBS},
{"Skeletonization", PRUNING_FUNC, 1, 1, (FunctionPtr) PRUNE_Skeletonization},
{"Noncontributing_Units", PRUNING_FUNC, 1, 1, (FunctionPtr) PRUNE_Noncontributing},

/* Remap Functions */
/*  This is the DEFAULT FUNCTION  */
{"None", REMAP_FUNC | DEFAULT_FUNC, 0, 0, (FunctionPtr) REMAP_none},
{"Binary", REMAP_FUNC, 0, 0, (FunctionPtr) REMAP_binary},
{"Inverse", REMAP_FUNC, 0, 0, (FunctionPtr) REMAP_invers},
{"Clip", REMAP_FUNC, 2, 0, (FunctionPtr) REMAP_clip},
{"LinearScale", REMAP_FUNC, 2, 0, (FunctionPtr) REMAP_linearscale},
{"Norm", REMAP_FUNC, 0, 0, (FunctionPtr) REMAP_norm},
{"Threshold", REMAP_FUNC, 4, 0, (FunctionPtr) REMAP_threshold}
  };


/*  Define the function table size
*/
const int  NoOfKernelFuncs = (sizeof (kernel_func_table) / sizeof (struct FuncTable));
/*  **********************************************************************  */




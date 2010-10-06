/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/trans_f.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel transfer functions
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 25.06.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.12 $
  LAST CHANGE    : $Date: 1998/03/10 15:12:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _TRANS_F_DEFINED_
#define  _TRANS_F_DEFINED_


extern FlintType  OUTP_Identity(register FlintType activation);
extern FlintType  OUT_Clip_01(register FlintType activation);
extern FlintType  OUT_Clip_11(register FlintType activation);
extern FlintType  OUT_Threshold05(register FlintType activation);
extern FlintType   ACT_Linear(struct Unit *unit_ptr);
extern FlintType   ACT_BSBFunction(struct Unit *unit_ptr);
extern FlintType   ACT_MinOutPlusWeight(struct Unit *unit_ptr);
extern FlintType   ACT_TanHFunction(struct Unit *unit_ptr);
extern FlintType   ACT_TanHFunction_Xdiv2(struct Unit *unit_ptr);
extern FlintType   ACT_Logistic(struct Unit *unit_ptr);
extern FlintType   ACT_Elliott(struct Unit *unit_ptr);
extern FlintType   ACT_Perceptron(struct Unit *unit_ptr);
extern FlintType   ACT_Signum(struct Unit *unit_ptr);
extern FlintType   ACT_Softmax(struct Unit *unit_ptr);
extern FlintType   ACT_CC_Threshold(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_CC_Threshold(struct Unit *unit_ptr);
extern FlintType   ACT_EXPONENT(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_EXPONENT(struct Unit *unit_ptr);
extern FlintType   ACT_SIN(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_SIN(struct Unit *unit_ptr);
extern FlintType   ACT_Signum0(struct Unit *unit_ptr);
extern FlintType   ACT_StepFunction(struct Unit *unit_ptr);
extern FlintType   ACT_HystStepFunction(struct Unit *unit_ptr);
extern FlintType   ACT_BAMFunction(struct Unit *unit_ptr);
extern FlintType   ACT_RM (struct Unit *unit_ptr);
extern FlintType   ACT_LogisticI(struct Unit *unit_ptr);
extern FlintType RbfUnitGetNormsqr(struct Unit *unit_ptr);
extern FlintType   ACT_RBF_Gaussian(struct Unit *unit_ptr);
extern FlintType ACT_RBF_Multiquadratic(struct Unit *unit_ptr);
extern FlintType ACT_RBF_Thinplatespline(struct Unit *unit_ptr);
extern FlintType   ACT_Linear_bias(struct Unit *unit_ptr);
extern FlintType  ACT_at_least_2 (struct Unit *unit_ptr);
extern FlintType  ACT_less_than_0 (struct Unit *unit_ptr);
extern FlintType  ACT_at_least_1 (struct Unit *unit_ptr);
extern FlintType  ACT_at_most_0 (struct Unit *unit_ptr);
extern FlintType  ACT_Product (struct Unit *unit_ptr);
extern FlintType  ACT_exactly_1 (struct Unit *unit_ptr);
extern FlintType   ACT_TD_Logistic(struct Unit *unit_ptr);
extern FlintType   ACT_TD_Elliott(struct Unit *unit_ptr);
extern FlintType   ACT_Euclid(struct Unit *unit_ptr);
extern FlintType   ACT_Component(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_Logistic(struct Unit *unit_ptr);
extern FlintType   ACT_2_DERIV_Logistic(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_Elliott(struct Unit *unit_ptr);
extern FlintType   ACT_2_DERIV_Elliott(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_TD_Logistic(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_TD_Elliott(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_Identity(struct Unit *unit_ptr);
extern FlintType   ACT_2_DERIV_Identity(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_BSBFunction(struct Unit *unit_ptr);
extern FlintType   ACT_2_DERIV_BSBFunction(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_TanHFunction(struct Unit *unit_ptr);
extern FlintType   ACT_2_DERIV_TanHFunction(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_TanHFunction_Xdiv2(struct Unit *unit_ptr);
extern FlintType   ACT_2_DERIV_TanHFunction_Xdiv2(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_Dummy(struct Unit *unit_ptr);
extern FlintType   ACT_2_DERIV_Dummy(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_RBF_Gaussian(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_RBF_Multiquadratic(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_RBF_Thinplatespline(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_TACOMA(struct Unit *unit_ptr);
extern FlintType  SITE_WeightedSum(struct Site *site_ptr);
extern FlintType  SITE_Product(struct Site *site_ptr);
extern FlintType  SITE_ProductA(struct Site *site_ptr);
extern FlintType  SITE_Max(struct Site *site_ptr);
extern FlintType  SITE_Min(struct Site *site_ptr);
extern FlintType  SITE_at_least_2 (struct Site *site_ptr);
extern FlintType  SITE_at_least_1 (struct Site *site_ptr);
extern FlintType  SITE_at_most_0 (struct Site *site_ptr);
extern FlintType  SITE_Reciprocal_WeightedSum (struct Site *site_ptr);
extern FlintType   ACT_LogisticSym(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_LogisticSym(struct Unit *unit_ptr);
extern FlintType   ACT_DERIV_tanh(struct Unit *unit_ptr);
extern FlintType   ACT_2_DERIV_tanh(struct Unit *unit_ptr);
extern FlintType   ACT_TACOMA(struct Unit *unit_ptr);
extern void kohonen_SetExtraParameter(int x);




#endif 

/* 82 lines generated by deleteprivatedefinitions.awk */

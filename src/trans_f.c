/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/trans_f.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel transfer functions
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 25.06.90

  CHANGED BY     : Sven Doering, Ralf Huebner, Marc Seemann (Uni Tuebingen)
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/03/13 16:24:05 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <math.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif
#include <string.h>

#include "kr_typ.h"	    /*	Kernel types and constants  */
#include "kr_const.h"       /*  Constant Declarators for SNNS-Kernel  */
#include "func_mac.h"	    /*	Transfer function macros  */
#include "glob_typ.h"
#include "kr_mac.h"
#include "cc_mac.h" 

#include "trans_f.ph"

#ifdef  __BORLANDC__
#pragma option -w-
#endif

/*#################################################

GROUP: Aritmetic Functions

#################################################*/

/*  exp function that prevents from over- and underflow, that means
    exp_s ist a "save" exp function.
*/
static float exp_s( float arg )
{
  if (arg > 88.72) return( MAXFLOAT );
  else if (arg < -88.0) return( 0.0 );
  return( exp( arg ) );
}


/*#################################################

GROUP: Unit Output Functions

#################################################*/

/*  Linear Output Function
    This function isn't used now, because the identity output function is
    the NULL pointer.
*/
FlintType  OUTP_Identity(register FlintType activation)
{
  return( activation );
}


/*  Clipping [0,1] function
*/
FlintType  OUT_Clip_01(register FlintType activation)
{
  if (activation < 0.0)  return( (FlintType) 0.0 );
  if (activation > 1.0)  return( (FlintType) 1.0 );
  return( activation );
}


/*  Clipping [-1,1] function
*/
FlintType  OUT_Clip_11(register FlintType activation)
{
  if (activation < -1.0)  return( (FlintType) -1.0 );
  if (activation > 1.0)  return( (FlintType) 1.0 );
  return( activation );
}

/*  Threshold 0.5 Output Function
*/
FlintType  OUT_Threshold05(register FlintType activation)
{
  if (activation < 0.5)  return( (FlintType) 0.0 );
  return( (FlintType) 1.0 );
}

FlintType OUT_Custom_Python(register FlintType activation)
{
  fputs("Dummy for custom python output functions - should never be called\n",stderr);
  return 0;
}
  
/*#################################################

GROUP: Unit Activation Functions

#################################################*/


/*  Linear Activation Function
*/
FlintType   ACT_Linear(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  return( sum );
}

/*  Brain-State-in-a-Box Function
*/
FlintType   ACT_BSBFunction(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  return( sum * GET_UNIT_BIAS( unit_ptr ));
}

/*  Minimum Function (Unit's output and weight)
*/
FlintType   ACT_MinOutPlusWeight(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  min1, min2;


  min1 = 0.0;

  if (GET_FIRST_UNIT_LINK( unit_ptr ))  {
    min1 = GET_OUTPUT + GET_WEIGHT;
    while (GET_NEXT_LINK)
      if ((min2 = GET_OUTPUT + GET_WEIGHT) < min1)
	min1 = min2;
  }
  else
    if (GET_FIRST_SITE( unit_ptr ))  {
      min1 = GET_SITE_VALUE;
      while (GET_NEXT_SITE)
       if ((min2 = GET_SITE_VALUE) < min1)
	    min1 = min2;
    }

  return( min1 );
}


/*  Hyperbolic Tangent Function with Bias
*/
FlintType   ACT_TanHFunction(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  return( tanh( sum + GET_UNIT_BIAS( unit_ptr )));
}


/*  Hyperbolic Tangent Function of (unit_ptr/2)
*/
FlintType   ACT_TanHFunction_Xdiv2( unit_ptr )
UNIT_PTR    unit_ptr;
{
  ACT_FUNC_DEFS
  register FlintType  sum;
   float expon;
   float wert;

  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);
  
  wert =  sum + GET_UNIT_BIAS( unit_ptr );
  if( wert > 9 )  wert = 9;
  if( wert <  -9 )  wert = -9;

  expon = exp_s(wert);
  return( (expon - 1) / (expon + 1));   

}




/*  Sigmoid Function
*/
FlintType   ACT_Logistic(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  return( (FlintType) (1.0 / (1.0 + exp_s( -sum - GET_UNIT_BIAS( unit_ptr )))) );
}


/*  Elliott Function
*/
FlintType   ACT_Elliott(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  sum += GET_UNIT_BIAS(unit_ptr);
  if (sum <= 0.0)
      return (FlintType) sum/(1.0 - sum);
  else
      return (FlintType) sum/(1.0 + sum);
}


/*  Perceptron Function
*/
FlintType   ACT_Perceptron(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  if (sum >= GET_UNIT_BIAS(unit_ptr))
    return( (FlintType) 1.0 );

  return( (FlintType) 0.0 );
}

/*  Signum Function
*/
FlintType   ACT_Signum(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  if (sum > 0.0)
    return( (FlintType) 1.0 );

  return( (FlintType) -1.0 );
}

/*  Softmax Function
*/
FlintType   ACT_Softmax(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  return( exp_s( -sum - GET_UNIT_BIAS( unit_ptr )) );
}

FlintType ACT_EXPONENT(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;

  sum =  GET_UNIT_BIAS(unit_ptr);
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);
  return exp_s(-0.5*sum*sum);
}

FlintType ACT_DERIV_EXPONENT(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  GET_UNIT_BIAS(unit_ptr);
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);
  return (-sum*exp_s(-0.5*sum*sum));
}

FlintType ACT_SIN(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  GET_UNIT_BIAS(unit_ptr);
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  return(sin(SIN_FAKTOR*sum));
}

FlintType ACT_DERIV_SIN(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  GET_UNIT_BIAS(unit_ptr);
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);
  return SIN_FAKTOR*cos(SIN_FAKTOR*sum);
}

 
FlintType   ACT_CC_Threshold(struct Unit *unit_ptr)
     /* only used, because I need a pseudo-derivation */
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  if (sum >=  - GET_UNIT_BIAS(unit_ptr)) /* remember it's negative */
    return( (FlintType) 1.0 );

  return( (FlintType) 0.0 );
}

FlintType   ACT_DERIV_CC_Threshold(struct Unit *unit_ptr)
{
  /*  return ACT_DERIV_Logistic(unit_ptr);*/
  return (THRESHOLD_DERIV);
}

/*  Signum0 Function
*/
FlintType   ACT_Signum0(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  if (sum > 0.0)  return( (FlintType) 1.0 );
  if (sum < 0.0)  return( (FlintType) -1.0 );
  return( (FlintType) 0.0 );
}


/*  Step Function
*/
FlintType   ACT_StepFunction(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  if (sum > 0.0)  return( (FlintType) 1.0 );
  return( (FlintType) 0.0 );
}


/*  Hysteresis Step Function
*/
FlintType   ACT_HystStepFunction(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;
           FlintType  Schwellwert = 0.1;

  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  if  (sum - (unit_ptr->bias) > Schwellwert)   return( (FlintType) 1.0 );
  if  (sum - (unit_ptr->bias) < -Schwellwert)  return( (FlintType) 0.0 );

  return( unit_ptr->act );
}

/*  Bi-Directional Associative Memory
*/
FlintType   ACT_BAMFunction(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  if (sum > 0.0)  return( (FlintType) 1.0 );
  if (sum < 0.0)  return( (FlintType) -1.0 );
  return( unit_ptr->Out.output );
}

/* Rummelhart-McClelland's activation function for the delta rule
*/
FlintType ACT_RM (struct Unit *unit_ptr)
{
    ACT_FUNC_DEFS
    register FlintType RM_act, sum;
    FlintType Eparam=.15, Dparam=.15;

    sum = 0.0;

    if (GET_FIRST_UNIT_LINK (unit_ptr))
	do
	    sum += GET_WEIGHTED_OUTPUT;
	while (GET_NEXT_LINK);
    else
	if (GET_FIRST_SITE (unit_ptr))
	    do
		sum += GET_SITE_VALUE;
	    while (GET_NEXT_SITE);

    if (sum > 0)
	RM_act = (unit_ptr->act + (Eparam * sum * (1 - unit_ptr->act)) 
		  - (Dparam * unit_ptr->act));
    else
	RM_act = (unit_ptr->act + (Eparam * sum * (unit_ptr->act + 1)) 
		  - (Dparam * unit_ptr->act));
    
    return (RM_act);
}  

/* Tacoma-Activation-Function.
   This Function needs additional parameters. They are stored
   in value_a (radius) and value_b (coord.) */



FlintType ACT_TACOMA(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType sum, coordAct,WeightSum;

  sum =  0.0;
  WeightSum = GET_UNIT_BIAS( unit_ptr );

  if (GET_FIRST_UNIT_LINK(unit_ptr))
     do{
        if ((GET_TACOMA_RADIUS > 0.0)&&(GET_TACOMA_RADIUS>0)){
           coordAct = ((GET_OUTPUT-GET_TACOMA_COORD) / GET_TACOMA_RADIUS);
           sum += coordAct*coordAct;
        }
        WeightSum += GET_WEIGHTED_OUTPUT;
     }while (GET_NEXT_LINK);
/*     {
        printf("Summe ist %.3f, exp ist : %.3f, normal was %.4f Erg ist %.3f\n",
      sum,exp_s(-sum),(1/(1+exp_s(-WeightSum))-0.5),
       (exp_s(-sum) * (1/(1+exp_s(-WeightSum))-0.5)));
     }*/
  return(exp_s(-sum) * (1/(1+exp_s(-WeightSum))-0.5));
}




/*  demonstation function: this function act like the Logistic function,
    but the site with the name "Inhibit" will be skipped.
*/
FlintType   ACT_LogisticI(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum = 0.0;
  if (GET_FIRST_SITE( unit_ptr ))
    /*	Do not calculate the 'Inhibit' site */
    do
      if (strcmp( "Inhibit", GET_SITE_NAME ))
        sum += GET_SITE_VALUE;
    while (GET_NEXT_SITE);
  else
    if (GET_FIRST_UNIT_LINK( unit_ptr ))
      do
        sum += GET_WEIGHTED_OUTPUT;
      while (GET_NEXT_LINK);

  return( (FlintType) (1.0 / (1.0 + exp_s( -sum - GET_UNIT_BIAS( unit_ptr )))) );
}

/* help function for all Radial Basis Activation, Derivation and Learn
 * functions. Computes the square of the L2-Norm of (T - X), where T is the
 * vector of all weights from links leading to <unit_ptr> and X is the
 * vector of output units the links are connected from.
 * Store calculated value into value_a field of the current unit.
 * ALL FUTURE RBF ACTIVATION FUNCTIONS HAVE TO CALL THIS FUNCTION !!!!!!!!!!
 */

FlintType RbfUnitGetNormsqr(struct Unit *unit_ptr)
{
        ACT_FUNC_DEFS
        register FlintType      norm_2 = 0.0;   /* |X - T|^2            */
        register FlintType      diff;           /* difference           */


        if (!GET_FIRST_UNIT_LINK(unit_ptr))
        {
                fprintf(stderr,"No input links!\n");
                return norm_2;
        }

        do
        {
                diff = GET_OUTPUT - GET_WEIGHT;
                norm_2 += diff * diff;
        }
        while (GET_NEXT_LINK);

      return unit_ptr -> value_a = norm_2;
}

/*
 * Gaussian RBF Activation function: h(L2, s) = exp(-s*L2^2)
 * where L2 is the L2 Norm (see RbfUnitGetNormsqr), and s is the bias 
 * of <unit_ptr>.
 */

FlintType   ACT_RBF_Gaussian(struct Unit *unit_ptr)
{
        register FlintType      norm_2;

        norm_2 = RbfUnitGetNormsqr(unit_ptr);
        return (FlintType) exp_s(- GET_UNIT_BIAS(unit_ptr)*norm_2);
}

/*
 * Multiquadratic Activation function: h(L2, s) = sqrt(s^2 + L2^2)
 */

FlintType ACT_RBF_Multiquadratic(struct Unit *unit_ptr)
{
      register FlintType      norm_2;

      norm_2 = RbfUnitGetNormsqr(unit_ptr);
      return (FlintType) sqrt(norm_2 + GET_UNIT_BIAS(unit_ptr));
}

/*
 * Thin plate splines Activation function: h(L2, s) = (L2*s)^2*ln(L2*s)
 */

FlintType ACT_RBF_Thinplatespline(struct Unit *unit_ptr)
{
      register FlintType      norm_2;
      register FlintType      bias;

      norm_2 = RbfUnitGetNormsqr(unit_ptr);
      bias = GET_UNIT_BIAS(unit_ptr);

      if (norm_2 == (FlintType) 0.0)
          return (FlintType) 0.0;
      else
          return (FlintType) bias*bias*norm_2*(0.5*log(norm_2) + log(bias));
}

/*  Linear Activation Function + BIAS
*/
FlintType   ACT_Linear_bias(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
      sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);

  return( sum + GET_UNIT_BIAS(unit_ptr));
}



/* NOTE: This function is nothing but a threshold function,
   which checks, whether the netinput is greater or equal 2, and if so
   returns 1.0, else 0.0 .
*/
FlintType  ACT_at_least_2 (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType    sum = 0.0;

   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/


   if (sum >= 2.0) {
      return ( (FlintType) 1.0);
   } else {
      return ( (FlintType) 0.0);
   } /*if*/
} /* ACT_at_least_2 */



FlintType  ACT_less_than_0 (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType    sum = 0.0;


   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   if (sum >= 0.0) {
      return ( (FlintType) 0.0);
   } else {
      return ( (FlintType) 1.0);
   } /*if*/

} /* ACT_less_than_0 */





FlintType  ACT_at_least_1 (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType   sum = 0.0;


   if (GET_FIRST_SITE (unit_ptr)) {
      do {
         sum += GET_SITE_VALUE;
      } while (GET_NEXT_SITE);
   } else {
      if (GET_FIRST_UNIT_LINK (unit_ptr)) {
         do {
            sum += GET_WEIGHTED_OUTPUT;
         } while (GET_NEXT_LINK);
      } /*if*/
   } /*if*/


   if (sum >= 1.0) {
      return ( (FlintType) 1.0);
   } else {
      return ( (FlintType) 0.0);
   } /*if*/

} /* ACT_at_least_1 */



FlintType  ACT_at_most_0 (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType    sum = 0.0;


   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   if (sum > 0.0) {
      return ( (FlintType) 0.0);
   } else {
      return ( (FlintType) 1.0);
   } /*if*/

} /* ACT_at_most_0 */



FlintType  ACT_Product (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType    prod = 1.0;


   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         prod *= GET_WEIGHTED_OUTPUT;
         if (prod == 0.0) {
            break;
         } /*if*/
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            prod *= GET_SITE_VALUE;
            if (prod == 0.0) {
               break;
            } /*if*/
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   return (prod);

} /* ACT_Product () */


FlintType  ACT_exactly_1 (struct Unit *unit_ptr)
{
   ACT_FUNC_DEFS
   register FlintType    sum = 0.0;


   if (GET_FIRST_UNIT_LINK (unit_ptr)) {
      do {
         sum += GET_WEIGHTED_OUTPUT;
      } while (GET_NEXT_LINK);
   } else {
      if (GET_FIRST_SITE (unit_ptr)) {
         do {
            sum += GET_SITE_VALUE;
         } while (GET_NEXT_SITE);
      } /*if*/
   } /*if*/

   if ((sum > 0.8) && (sum < 1.2)) {
      return (1.0);
   } else {
      return (0.0);
   } /*if*/

} /* ACT_exactly_1 */

/*****************************************************************************
  FUNCTION : ACT_TD_Logistic

  PURPOSE  : logistic activation function for use in time delay networks
  RETURNS  : activation
  NOTES    : the TD section of the unit must be initialized correct
             the units must be sorted TOPOLOGIC_LOGICAL

  UPDATE   : 19.2.93 M. Vogt
******************************************************************************/

FlintType   ACT_TD_Logistic(struct Unit *unit_ptr)
{
/* the common macros are not used */

  register FlintType  sum;
  register UNIT_PTR ref_unit;
  register int source_offset;
  register struct Link *link;

  if (unit_ptr -> TD.td_connect_typ == 0)
      return ACT_Logistic(unit_ptr);

  ref_unit = *(unit_ptr -> TD.my_topo_ptr + unit_ptr -> TD.target_offset);
  source_offset = unit_ptr -> TD.source_offset;
  sum =  0.0;
  if ((ref_unit -> flags) & UFLAG_DLINKS)
  {
      link = (struct Link *) ref_unit->sites;
      while (link != (struct Link *) NULL)
      {
          sum += (*(link->to->TD.my_topo_ptr + source_offset))->Out.output 
                 * link->weight;
          link = link->next;
      }
  }
  else
  {
      fprintf(stderr, 
              "Warning: Illegal link structure used in time delay layer\n");
  }
  return( (FlintType) (1.0 / (1.0 + exp_s( -sum - ref_unit->bias))) );
}

/*****************************************************************************
  FUNCTION : ACT_TD_Elliott

  PURPOSE  : elliott activation function for use in time delay networks
  RETURNS  : activation
  NOTES    : the TD section of the unit must be initialized correct
             the units must be sorted TOPOLOGIC_LOGICAL

  UPDATE   : 5.3.93 M. Vogt
******************************************************************************/

FlintType   ACT_TD_Elliott(struct Unit *unit_ptr)
{
/* the common macros are not used */

  register FlintType  sum;
  register UNIT_PTR ref_unit;
  register int source_offset;
  register struct Link *link;

  if (unit_ptr -> TD.td_connect_typ == 0)
      return ACT_Elliott(unit_ptr);

  ref_unit = *(unit_ptr -> TD.my_topo_ptr + unit_ptr -> TD.target_offset);
  source_offset = unit_ptr -> TD.source_offset;
  sum =  0.0;
  if ((ref_unit -> flags) & UFLAG_DLINKS)
  {
      link = (struct Link *) ref_unit->sites;
      while (link != (struct Link *) NULL)
      {
          sum += (*(link->to->TD.my_topo_ptr + source_offset))->Out.output 
                 * link->weight;
          link = link->next;
      }
  }
  else
  {
      fprintf(stderr, 
              "Warning: Illegal link structure used in time delay layer\n");
  }

  sum += ref_unit->bias;
  if (sum <= 0.0)
      return (FlintType) sum/(1.0 - sum);
  else
      return (FlintType) sum/(1.0 + sum);
}



/* This function is called by a xgui function for kohonen networks */
void kohonen_SetExtraParameter(int x)
  /* no. of layer chosen in remote panel */
{
  ExtraParameter=x;
}


/* Activate specific layer of the net chosen in the remote panel, to set
the layer call:  kohonen_SetExtraParameter( Layer ) 
*/
FlintType   ACT_Component( UNIT_PTR unit_ptr )

{
  ACT_FUNC_DEFS
  register FlintType  sum;
  int i=1,n;

  n = ExtraParameter;

  sum =  0.0;
  if (GET_FIRST_SITE( unit_ptr ))
    sum = GET_SITE_VALUE;
  else
    if (GET_FIRST_UNIT_LINK( unit_ptr ))
      do
        sum=GET_WEIGHT;
      while((i++<n)&&GET_NEXT_LINK);

  return( sum );
}

/*  Calculate the simple euclidic distance
    between in-vector and weight-vector for kohonen networks
*/
FlintType   ACT_Euclid( UNIT_PTR unit_ptr )

{
  ACT_FUNC_DEFS
  register FlintType  dist;

  dist=  0.0;
  if (GET_FIRST_SITE( unit_ptr ))
    do
      dist += GET_SITE_VALUE;
    while (GET_NEXT_SITE);
  else
    if (GET_FIRST_UNIT_LINK( unit_ptr ))
      do
        dist += GET_EUCLID_COMP;
      while (GET_NEXT_LINK);

  return(sqrt(dist));
}

FlintType ACT_Custom_Python(struct Unit *unit_ptr)
{
  fputs("Dummy for custom python activation functions - should never be called\n",stderr);
  return 0;
}



/*######################################################################

GROUP: First and Second Derivation Functions of the Activation Functions

Second Derivation Funtions are not available for TD and RBF activation
functions.

######################################################################*/

FlintType ACT_DERIV_Custom_Python(struct Unit *unit_ptr)
{
  fputs("Dummy for custom python activation functions - should never be called\n",stderr);
  return 0;
}

FlintType ACT_2_DERIV_Custom_Python(struct Unit *unit_ptr)
{
  fputs("Dummy for custom python activation functions - should never be called\n",stderr);
  return 0;
}

/*  Sigmoid Derivation Function
*/
FlintType   ACT_DERIV_Logistic(struct Unit *unit_ptr)
{
    return( GET_UNIT_ACT( unit_ptr ) * (1.0 - GET_UNIT_ACT( unit_ptr )) );
}


FlintType ACT_2_DERIV_Logistic (struct Unit *unit_ptr)
{

    return (GET_UNIT_ACT (unit_ptr) * (1.0 - GET_UNIT_ACT (unit_ptr)) 
	    * (2 * GET_UNIT_ACT (unit_ptr) - 1.0));

}


/*  Elliott Derivation Function
*/
FlintType   ACT_DERIV_Elliott(struct Unit *unit_ptr)
{
    register FlintType act;
    if ((act = GET_UNIT_ACT(unit_ptr)) <= 0.0)
	act = 1.0 + act;
    else
	act = 1.0 - act;

    return (act*act);
}


FlintType ACT_2_DERIV_Elliott (struct Unit *unit_ptr)
{

    register FlintType act;
    
    if ((act = GET_UNIT_ACT(unit_ptr)) <= 0.0)
	act = 1.0 + act;
    else
	act = 1.0 - act;

    if (act <= 0.0)
	return (2 * act * act);
    else
	return (-2 * act * act);

}


/*  Sigmoid Derivation Function for TD Networks
*/

FlintType   ACT_DERIV_TD_Logistic(struct Unit *unit_ptr)
{
  return( GET_UNIT_ACT( unit_ptr ) * (1.0 - GET_UNIT_ACT( unit_ptr )) );
}


/*  Elliott Derivation Function for TD Networks
*/

FlintType   ACT_DERIV_TD_Elliott(struct Unit *unit_ptr)
{
    register FlintType act;
    if ((act = GET_UNIT_ACT(unit_ptr)) <= 0.0)
	act = 1.0 + act;
    else
	act = 1.0 - act;

    return (act*act);
}


/*  Identity Derivation Function
*/
FlintType   ACT_DERIV_Identity(struct Unit *unit_ptr)
{
  return( (FlintType) 1.0 );
}


FlintType ACT_2_DERIV_Identity (struct Unit *unit_ptr)
{

    return ((FlintType) 0.0);

}

/*  Brain-State-in-a-Box Derivation Function
*/
FlintType   ACT_DERIV_BSBFunction(struct Unit *unit_ptr)
{
  return( GET_UNIT_BIAS( unit_ptr ));
}


FlintType ACT_2_DERIV_BSBFunction (struct Unit *unit_ptr)
{

    return ((FlintType) 0.0);

}


/* TanH Derivation Function
*/
FlintType   ACT_DERIV_TanHFunction(struct Unit *unit_ptr)
{
  return(1.0-GET_UNIT_ACT( unit_ptr ) * (GET_UNIT_ACT( unit_ptr )) );
}


FlintType ACT_2_DERIV_TanHFunction (struct Unit *unit_ptr)
{

    return (2 * GET_UNIT_ACT (unit_ptr) * 
	    (1.0 - GET_UNIT_ACT (unit_ptr) * 
	           GET_UNIT_ACT (unit_ptr)));

}


/* TanH Derivation Function
*/
FlintType   ACT_DERIV_TanHFunction_Xdiv2(struct Unit *unit_ptr)
{
  return(1.0-(GET_UNIT_ACT( unit_ptr ) * (GET_UNIT_ACT( unit_ptr )))/2 );
}


FlintType ACT_2_DERIV_TanHFunction_Xdiv2 (struct Unit *unit_ptr)
{

    return (ACT_2_DERIV_TanHFunction (unit_ptr) / 2);

}


/*  Dummy function for the derivation functions. Returns always the value 1.0.
    This function is used for activation functions that can't have a derivation
    function.

    NOTE: All activation functions have to provide a derivation function.
*/
FlintType   ACT_DERIV_Dummy(struct Unit *unit_ptr)
{
  return( (FlintType) 1.0 );
}


/* Dummy function for second derivation, always returns 0.0. */

FlintType ACT_2_DERIV_Dummy (struct Unit *unit_ptr)
{

    return ((FlintType) 0.0);

}


/* Gaussian Radial Basis Derivation functionS
 * depending on Aux: 0 derivated to T
 *                   1 derivated to s (BIAS)
 *                 2 derivated to T if value_a holds norm ^ 2;
 *                 3 derivated to s if value_a holds norm ^ 2;
 *                 others: const 1;
 */

FlintType   ACT_DERIV_RBF_Gaussian(struct Unit *unit_ptr)
{
      register FlintType      rc;             /* return value         */
      register FlintType      norm_2;         /* norm ^ 2             */

      switch (unit_ptr -> Aux.int_no)
      {
          case 0:
              /* derivated to norm_2:                                 */
              norm_2 = RbfUnitGetNormsqr(unit_ptr);
              rc =  (FlintType) -GET_UNIT_BIAS(unit_ptr)
                      * exp_s(- GET_UNIT_BIAS(unit_ptr)*norm_2);
              break;
          case 1:
              /* derivated to BIAS:                                   */
              norm_2 = RbfUnitGetNormsqr(unit_ptr);
              rc = (FlintType) -norm_2 
                      * exp_s(- GET_UNIT_BIAS(unit_ptr)*norm_2);
              break;
          case 2:
              /* derivated to norm_2: (norm ^ 2 = value_a)            */
              rc =  (FlintType) -GET_UNIT_BIAS(unit_ptr)
                      * exp_s(- GET_UNIT_BIAS(unit_ptr)*unit_ptr -> value_a);
              break;
          case 3:
              /* derivated to BIAS: (norm ^ 2 = value_a)              */
              rc = (FlintType) -unit_ptr -> value_a 
                      * exp_s(- GET_UNIT_BIAS(unit_ptr)*unit_ptr -> value_a);
              break;
          default:
              rc = (FlintType) 1.0;
      }

  return rc;
}

/* Multiquadratic Radial Basis Derivation functionS
 * depending on Aux: 0 derivated to T
 *                   1 derivated to s (BIAS)
 *                 2 derivated to T if value_a holds norm ^ 2;
 *                 3 derivated to s if value_a holds norm ^ 2;
 *                 others: const 1;
 */

FlintType   ACT_DERIV_RBF_Multiquadratic(struct Unit *unit_ptr)
{
      register FlintType      rc;             /* return value         */
      register FlintType      norm_2;         /* norm ^ 2             */
      register FlintType      bias;           /* s                    */

      bias = (FlintType) GET_UNIT_BIAS(unit_ptr);
      switch (unit_ptr -> Aux.int_no)
      {
          case 0:
          case 1:
              /* derivated to BIAS:                                   */
              /* derivated to norm_2:                                 */
              norm_2 = RbfUnitGetNormsqr(unit_ptr);
              rc =  (FlintType) 1.0/(2.0 * sqrt(bias + norm_2));
              break;
          case 2:
          case 3:
              /* derivated to BIAS: (norm ^ 2 = value_a)              */
              /* derivated to norm_2: (norm ^ 2 = value_a)            */
              rc =  (FlintType) 1.0/(2.0 * sqrt(bias + unit_ptr -> value_a));
              break;
          default:
              rc = (FlintType) 1.0;
      }

  return rc;
}

/* Thin Plate Spline Radial Basis Derivation functionS
 * depending on Aux: 0 derivated to T
 *                   1 derivated to s (BIAS)
 *                 2 derivated to T if value_a holds norm ^ 2;
 *                 3 derivated to s if value_a holds norm ^ 2;
 *                 others: const 1;
 */

FlintType   ACT_DERIV_RBF_Thinplatespline(struct Unit *unit_ptr)
{
      register FlintType      rc;             /* return value         */
      register FlintType      norm_2;         /* norm ^ 2             */
      register FlintType      bias;           /* s                    */

      bias = (FlintType) GET_UNIT_BIAS(unit_ptr);
      switch (unit_ptr -> Aux.int_no)
      {
          case 0:
              /* derivated to norm_2:                                 */
              norm_2 = RbfUnitGetNormsqr(unit_ptr);
              if (norm_2 == (FlintType) 0.0)
                  rc = (FlintType) 0.0;
              else
                  rc =  (FlintType) bias * bias *
                      (log(norm_2) + 2.0*log(bias) + 1.0) / 2.0;
              break;
          case 1:
              /* derivated to BIAS:                                   */
              norm_2 = RbfUnitGetNormsqr(unit_ptr);
              if (norm_2 == (FlintType) 0.0)
                  rc = (FlintType) 0.0;
              else
                  rc = (FlintType) bias * norm_2 *
                      (log(norm_2) + 2.0*log(bias) + 1.0);
              break;
          case 2:
              /* derivated to norm_2: (norm ^ 2 = value_a)            */
              if (unit_ptr -> value_a == (FlintType) 0.0)
                  rc = (FlintType) 0.0;
              else
                  rc =  (FlintType) bias * bias *
                      (log(unit_ptr -> value_a) + 2.0*log(bias) + 1.0) / 2.0;
              break;
          case 3:
              /* derivated to BIAS: (norm ^ 2 = value_a)              */
              if (unit_ptr -> value_a == (FlintType) 0.0)
                  rc = (FlintType) 0.0;
              else
                  rc = (FlintType) bias * unit_ptr -> value_a *
                      (log(unit_ptr -> value_a) + 2.0*log(bias) + 1.0);
              break;
          default:
              rc = (FlintType) 1.0;
      }

  return rc;
}




/* Tacoma-Deriv-Activation-Function.
   This Function needs additional parameters. They are stored
   in value_a (radius) and value_b (coord.) */

FlintType ACT_DERIV_TACOMA(struct Unit *unit_ptr)
{

/*  returns ( (        1           1  ) 2        1   )    _
           ( (   -------------  - ---  )      - ---   ) h(x)
            ( (    1+e^(-Net)      2  )          4   )              

    or in latex-code :
    ((\frac{1}{1+e^{-Net}}-\frac{1}{2})^2 - frac{1}{4} )*h(\vec{x}) */


  ACT_FUNC_DEFS
  float sum,coordAct,WeightSum;
  float bruch;

  sum = 0.0;

  WeightSum=GET_UNIT_BIAS(unit_ptr);

  if (GET_FIRST_UNIT_LINK(unit_ptr))
     do{
        if (GET_TACOMA_RADIUS > 0.0){
           coordAct = ((GET_OUTPUT-GET_TACOMA_COORD) / GET_TACOMA_RADIUS);
           sum += coordAct*coordAct;
        }
        WeightSum+=GET_WEIGHTED_OUTPUT;
     }while (GET_NEXT_LINK);
  bruch=1/(1+exp_s(-WeightSum))-0.5;
  return (bruch*bruch-0.25)*(exp_s(-sum));
}

  



/*#################################################

GROUP: Site functions

#################################################*/

/*  Linear Site Function
*/
FlintType  SITE_WeightedSum(struct Site *site_ptr)
{
  SITE_FUNC_DEFS
  register FlintType  sum;


  sum = 0.0;
  if (GET_FIRST_SITE_LINK( site_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);

  return( sum );
}


/*  Product of all predecessor outputs and input link weights
*/
FlintType  SITE_Product(struct Site *site_ptr)
{
  SITE_FUNC_DEFS
  register FlintType  prod;


  if (GET_FIRST_SITE_LINK( site_ptr ))  {
    prod = 1.0;
    do
      prod *= GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);

    return( prod );
  }
  else
    return( (FlintType) 0.0 );
}


/*  Like SITE_Product() but no weighting of the unit's output
*/
FlintType  SITE_ProductA(struct Site *site_ptr)
{
  SITE_FUNC_DEFS
  register FlintType  prod;


  if (GET_FIRST_SITE_LINK( site_ptr ))  {
    prod = 1.0;
    do
      prod *= GET_OUTPUT;
    while (GET_NEXT_LINK);

/*  Future Application (in SNNS-Kernel V2.1 the sites don't have weights).
    So the return value is only the product.
*/
    return( GET_SITE_WEIGHT * prod );
  }
  else
    return( (FlintType) 0.0 );
}



/*  Get the highest weighted output
*/
FlintType  SITE_Max(struct Site *site_ptr)
{
  SITE_FUNC_DEFS
  register FlintType  max, out;


  if (GET_FIRST_SITE_LINK( site_ptr ))  {
    max = GET_WEIGHTED_OUTPUT;

    while (GET_NEXT_LINK)  {
      out = GET_WEIGHTED_OUTPUT;
      if (max < out)  max = out;
    }

    return( max );
  }
  else
    return( (FlintType) 0.0 );
}


/*  Get the lowest weighted output
*/
FlintType  SITE_Min(struct Site *site_ptr)
{
  SITE_FUNC_DEFS
  register FlintType  min, out;


  if (GET_FIRST_SITE_LINK( site_ptr ))  {
    min = GET_WEIGHTED_OUTPUT;

    while (GET_NEXT_LINK)  {
      out = GET_WEIGHTED_OUTPUT;
      if (min > out)  min = out;
    }

    return( min );
  }
  else
    return( (FlintType) 0.0 );

}


FlintType  SITE_at_least_2 (struct Site *site_ptr)
{
  SITE_FUNC_DEFS
  register FlintType  sum = 0.0;


  sum = 0.0;
  if (GET_FIRST_SITE_LINK (site_ptr))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);

  if (sum >= 2.0) {
     return ( (FlintType) 1.0);
  } else {
     return ( (FlintType) 0.0);
  } /*if*/

} /* SITE_at_least_2 */


FlintType  SITE_at_least_1 (struct Site *site_ptr)
{
  SITE_FUNC_DEFS
  register FlintType  sum = 0.0;


  sum = 0.0;
  if (GET_FIRST_SITE_LINK (site_ptr))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);

  if (sum >= 1.0) {
     return ( (FlintType) 1.0);
  } else {
     return ( (FlintType) 0.0);
  } /*if*/

} /* SITE_at_least_1 */


FlintType  SITE_at_most_0 (struct Site *site_ptr)
{
  SITE_FUNC_DEFS
  register FlintType  sum = 0.0;


  sum = 0.0;
  if (GET_FIRST_SITE_LINK (site_ptr))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);

  if (sum <= 0.0) {
     return ( (FlintType) 1.0);
  } else {
     return ( (FlintType) 0.0);
  } /*if*/

} /* SITE_at_most_0 */



/* IMPORTANT:
   This function doesn't check for overflows. So make sure
   that sum is greater than 0.0 when using this Site function.
*/
FlintType  SITE_Reciprocal_WeightedSum (struct Site *site_ptr)
{
  SITE_FUNC_DEFS
  register FlintType  sum = 0.0;


  sum = 0.0;
  if (GET_FIRST_SITE_LINK( site_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);

  if (sum == 0.0) {
     return (0.0);
  } else {
     return((FlintType) (1/sum));
  } /*if*/
} /* SITE_Reciprocal_WeightedSum */



FlintType   ACT_LogisticSym(struct Unit *unit_ptr)
{
  ACT_FUNC_DEFS
  register FlintType  sum;


  sum =  0.0;
  if (GET_FIRST_UNIT_LINK( unit_ptr ))
    do
      sum += GET_WEIGHTED_OUTPUT;
    while (GET_NEXT_LINK);
  else
    if (GET_FIRST_SITE( unit_ptr ))
      do
	sum += GET_SITE_VALUE;
      while (GET_NEXT_SITE);
  return( (FlintType) (1.0 / (1.0 + exp_s( -sum - GET_UNIT_BIAS( unit_ptr ))))-0.5);
}

FlintType   ACT_DERIV_LogisticSym(struct Unit *unit_ptr)
{
  return( 0.25 - GET_UNIT_ACT( unit_ptr ) * GET_UNIT_ACT( unit_ptr ));
}

FlintType   ACT_DERIV_tanh(struct Unit *unit_ptr)
{
  return( 2 * (1.0 - GET_UNIT_ACT( unit_ptr )) * GET_UNIT_ACT( unit_ptr ));
}


FlintType ACT_2_DERIV_tanh (struct Unit *unit_ptr)
{

    return (2 * ACT_2_DERIV_Logistic (unit_ptr));

}


#ifdef  __BORLANDC__
#pragma option -w+.
#endif

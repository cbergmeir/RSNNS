/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/tbl_func.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel: Transfer functions using table lookup and
	           linear approximation method
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 25.07.91

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:58 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <math.h>

#include "kr_typ.h"	    /*	Kernel types and constants  */
#include "kr_const.h"       /*  Constant Declarators for SNNS-Kernel  */
#include "func_mac.h"	    /*	Transfer function macros  */
#include "tbl_func.ph"


/*#################################################

GROUP: Unit activation functions using table lookup
       and linear approximation method

#################################################*/


/*  Sigmoid function
    using table lookup and linear approximation method
*/
FlintType   ACT_LogisticTbl(struct Unit *unit_ptr)
{
#include "sigmoid.tbl"	/*  m,b value tables for linear approximation  */

  ACT_FUNC_DEFS
  register FlintType  sum, x;
  register int	index;


  sum =  0.0;
  if (GET_FIRST_SITE( unit_ptr ))
    do
      sum += GET_SITE_VALUE;
    while (GET_NEXT_SITE);
  else
    if (GET_FIRST_UNIT_LINK( unit_ptr ))
      do
        sum += GET_WEIGHTED_OUTPUT;
      while (GET_NEXT_LINK);

/*  ***************************************************  */


  x = sum + GET_UNIT_BIAS( unit_ptr );
  index = (int) (x * SCALE_FACTOR) + INDEX_OFFSET;

  if (index < 0)
    {  /*  x is less then MIN_APPROX_X:
	   approx. func value to MINUS_INFINITE_FUNC_VALUE  */
    if (index <= MIN_INDEX)
      { /*  printf( "x below -infinite : %g\n", x );  */
	return( MINUS_INFINITE_FUNC_VALUE );
      }
    /*	printf( "x below MIN_APPROX_X : %g\n", x );  */
    return( m[0] * x + b[0] );
    }

  if (index > NO_OF_APPROX)
    {  /*  x is greater then MAX_APPROX_X:
	   approx. func value to PLUS_INFINITE_FUNC_VALUE  */
    if (index >= MAX_INDEX)
      { /*  printf( "x above +infinite : %g\n", x );  */
	return( PLUS_INFINITE_FUNC_VALUE );
      }
    /*	printf( "x above MAX_APPROX_X : %g\n", x );  */
    return( m[ NO_OF_APPROX ] * x + b[ NO_OF_APPROX ] );
    }

  return( m[ index ] * x + b[ index ] );
}

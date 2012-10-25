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

#include <stdio.h>
#include <math.h>

#include "SnnsCLib.h"


/*#################################################

GROUP: Unit activation functions using table lookup
       and linear approximation method

#################################################*/


/*  Sigmoid function
    using table lookup and linear approximation method
*/
FlintType   SnnsCLib::ACT_LogisticTbl(struct Unit *unit_ptr)
{
#include "sigmoid_tbl.h"	/*  m,b value tables for linear approximation  */

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

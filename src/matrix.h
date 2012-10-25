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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/matrix.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel standard matrix operations
  NOTES          :

  AUTHOR         : Michael Vogt
  DATE           : 10.02.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:29 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef	_RBF_MATRIX_DEFINED_
#define _RBF_MATRIX_DEFINED_

/* begin global definition section */

/************************************************************************/
/* type definitions:							*/
/************************************************************************/

typedef struct
{
	int	rows;		/* number of rows			*/
	int	columns;	/* number of columns			*/
	float	*field;		/* matrix organized as list		*/
	float	**r_pt;		/* array of references to rows		*/
} RbfFloatMatrix;

/************************************************************************/
/* allocate matrix m with r rows and c columns				*/
/* returns 0 if impossible, 1 otherwise					*/
/************************************************************************/

int	RbfAllocMatrix(int r, int c, RbfFloatMatrix *m);

/************************************************************************/
/* deallocate matrix m							*/
/************************************************************************/

void	RbfFreeMatrix(RbfFloatMatrix *m);

/************************************************************************/
/* set all elements of matrix m to value c				*/
/************************************************************************/

void	RbfClearMatrix(RbfFloatMatrix *m, double c);

/************************************************************************/
/* returns the square norm of  matrix m            		     	*/
/************************************************************************/

float	RbfSquareOfNorm(RbfFloatMatrix *m);

/************************************************************************/
/* sets m to the idempotent matrix           				*/
/************************************************************************/

void	RbfIdempotentMatrix(RbfFloatMatrix *m);

/************************************************************************/
/* multiplies matrix m with a constant factor  				*/
/************************************************************************/

void	RbfMulScalarMatrix(RbfFloatMatrix *m, float a);


/************************************************************************/
/* set m1 to m2 (m1 = m2). m1 must be allocated with the same		*/
/* dimensions as m2							*/
/************************************************************************/

void	RbfSetMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2);

/************************************************************************/
/* set m1 to m2 transposed (m1 = m2T)					*/
/* number of rows of m1 must be equal to number of columns of m2.	*/
/* number of columns of m1 must be equal to number of rows of m2.	*/
/************************************************************************/

void	RbfTranspMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2);

/************************************************************************/
/* set m to inverse of m (m = m^-1).					*/
/* returns 0 if impossible, 1 otherwise, negative if kernel error	*/
/************************************************************************/

int	RbfInvMatrix(RbfFloatMatrix *m);

/************************************************************************/
/* function RbfMulTranspMatrix:						*/
/* set m1 to m2*m2T. number of rows of m2 must be equal to number of    */
/* rows and columns of m1.                                              */
/************************************************************************/

void	RbfMulTranspMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2);

/************************************************************************/
/* set m1 to m2*m3. number of columns of m2 must be equal to number of  */
/* rows of m3. m1 must be allocated with r = number of rows of m2 and   */
/* c = number of columns of m3.						*/
/************************************************************************/

void	RbfMulMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2, RbfFloatMatrix *m3); 

/************************************************************************/
/* set m1 to m2+m3. number of columns of m1, m2 and m3 must be equal.	*/
/* number of rows of m1, m2 and m3 must be equal.			*/
/************************************************************************/

void	RbfAddMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2, RbfFloatMatrix *m3); 

/************************************************************************/
/* print out matrix m to stream (file) s				*/
/************************************************************************/

void	RbfPrintMatrix(RbfFloatMatrix *m, FILE *s);

/************************************************************************/
/* print message to stderr			                        */
/************************************************************************/

void    ErrMess(char *message);

/************************************************************************/
/* macro definitions							*/
/************************************************************************/

/************************************************************************/
/* set element at row r and column c in matrix referenced by m to the   */
/* value of v. r goes from 0 to rows -1, c goes from 0 to columns -1    */
/************************************************************************/

#define	RbfMatrixSetValue(m, r, c, v)	((m)->r_pt)[r][c] = v

/************************************************************************/
/* get value of element at row r and column c in matrix referenced by m.*/
/* r goes from 0 to rows -1, c goes from 0 to columns -1		*/
/************************************************************************/

#define RbfMatrixGetValue(m, r, c)	(((m) -> r_pt)[r][c])

/* end global definition section */

/* begin private definition section */

#define RBF_MATRIX_LUDCOMP	1

/* end private definition section */

int RbfLUDcmp(RbfFloatMatrix*, int*);
void RbfLUBksb(RbfFloatMatrix*, int*, float*);


#endif





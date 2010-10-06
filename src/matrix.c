/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/matrix.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel standard matrix operations
  NOTES          :

  AUTHOR         : Michael Vogt
  DATE           : 10.02.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/03/16 13:33:58 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

/************************************************************************/
/* included headers:							*/
/************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.ph"
#include "glob_typ.h"


/************************************************************************/
/* functions:								*/
/************************************************************************/

/************************************************************************/
/* function RbfAllocMatrix:						*/
/* allocate matrix m with r rows and c columns				*/
/* returns 0 if impossible, 1 otherwise					*/
/************************************************************************/

int	RbfAllocMatrix(int r, int c, RbfFloatMatrix *m)
{
	int	rc;			/* return code			*/
	int	i;			/* index variable		*/

	m -> field = (float *) malloc(r * c * sizeof(float));
	m -> r_pt = (float **) malloc(r * sizeof(float *));
	if (m -> field == (float *) NULL ||
	    m -> r_pt == (float **) NULL)
	{
		/* malloc was impossible, return 0 (reports error)	*/
		rc = 0;
	}
	else
	{
		/* malloc successfull, initialize matrix:		*/
		rc = 1;
		m -> rows = r;
		m -> columns = c;
		for (i = 0; i<r; i++)
			(m -> r_pt)[i] = &(m -> field)[i * c];
	}

	return rc;			/* report status		*/
}

/************************************************************************/
/* function RbfFreeMatrix:						*/
/* deallocate matrix m							*/
/************************************************************************/

void	RbfFreeMatrix(RbfFloatMatrix *m)
{
	free(m -> field);
	free(m -> r_pt);
	m -> rows = 0;
	m -> columns = 0;
}

/************************************************************************/
/* function RbfClearMatrix:						*/
/* set all elements of matrix m to value c				*/
/************************************************************************/

void RbfClearMatrix(RbfFloatMatrix *m, double c)
{
	int	count;
	float	*ptoelement;

	ptoelement = m -> field;
	count = (m -> rows)*(m -> columns);

	while(count--)
	{
		*ptoelement++ = c;
	}
}

/************************************************************************/
/* function RbfSquareOfNorm:						*/
/*                                      				*/
/************************************************************************/

float RbfSquareOfNorm(RbfFloatMatrix *m)
{
  int	i, j;
  float norm = 0.0;

  for (i = m->rows -1 ; i>=0; i--)
    {
      for (j = m->columns -1 ; j>=0; j--)
	norm += RbfMatrixGetValue(m, i ,j )*RbfMatrixGetValue(m, i, j);
    };
  return norm ;
}

/************************************************************************/
/* function RbfIdempotentMatrix:	                                */
/* CAUTION: m must be a square matrix      		      		*/
/* set all elements of matrix m to value 0, the diagonal to 1  		*/
/************************************************************************/

void RbfIdempotentMatrix(RbfFloatMatrix *m)
{
register int     i,j;

for (i = m->rows -1 ; i>=0; i--)
  {
    for (j = m->columns -1 ; j>=0; j--)
      RbfMatrixSetValue(m,  i, j, 0);
    RbfMatrixSetValue(m, i, i, 1);
  };

}

/************************************************************************/
/* function RbfMulScalarMatrix:	                                */
/* set all elements of matrix m to their value multiplied with a      	*/
/************************************************************************/

void RbfMulScalarMatrix(RbfFloatMatrix *m, float a)
{
register int     i,j;

for (i = m->rows -1 ; i>=0; i--)
  {
    for (j = m->columns -1 ; j>=0; j--)
      RbfMatrixSetValue(m, i, j, a * RbfMatrixGetValue(m, i, j) );
  };

}

/************************************************************************/
/* function RbfSetMatrix:						*/
/* set m1 to m2 (m1 = m2). m1 must be allocated with the same		*/
/* dimensions as m2							*/
/************************************************************************/

void RbfSetMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2)
{
	int	count;
	float	*ptofrom;
	float	*ptoto;

#ifdef	DEBUG_MODE
	if (m1 -> rows != m2 -> rows ||
	    m1 -> columns != m2 -> columns)
	{
		ErrMess("RbfSetMatrix: incompatible matrixes.\n");
	}
#endif

	count = (m2 -> rows)*(m2 -> columns);
	ptofrom = m2 -> field;
	ptoto = m1 -> field;

	while(count--)
	{
		*ptoto++ = *ptofrom++;
	}
}

/************************************************************************/
/* function RbfTranspMatrix:						*/
/* set m1 to m2 transposed (m1 = m2T)					*/
/* number of rows of m1 must be equal to number of columns of m2.	*/
/* number of columns of m1 must be equal to number of rows of m2.	*/
/************************************************************************/

void	RbfTranspMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2)
{
	
	int		r;
	int		c;

#ifdef	DEBUG_MODE
	if (m1 -> rows != m2 -> columns ||
	    m1 -> columns != m2 -> rows)
		ErrMess("RbfTranspMatrix: incompatible matrixes.\n"); 
#endif

	/* make m1 to become m2 transposed				*/

	for (r = 0; r < m2 -> rows; r++)
	    for (c = 0; c < m2 -> columns; c++)
		RbfMatrixSetValue(m1, c, r, RbfMatrixGetValue(m2, r, c));
}

#ifdef RBF_MATRIX_LUDCOMP

/************************************************************************/
/* function RbfLUDcmp:							*/
/* builds the LU Decomposition of matrix m and stores it into m 	*/
/* the permutation of rows is stored in vector <index> to use with	*/
/* the function RbfLUBksb for solving linear equations.			*/
/* (Algorithm taken from 'Numerical Recipts')				*/
/* returns 1 if succesfull, 0 if singular matrix, negative if kernel	*/
/* error								*/
/************************************************************************/

static int RbfLUDcmp(RbfFloatMatrix *m, int *indx)
{
	register float		sum;
	register float		dum;
	register float		big;
	register int		i, j, k, imax;
	register float		temp;
	register float		*vv;

	if ((vv = (float *) malloc(m -> rows * sizeof(float))) == NULL)
	{
	    ErrMess("RbfLUDcmp: impossible to allocate helpmatrix.\n");
	    return KRERR_INSUFFICIENT_MEM;
	}

	for (i = 0; i < m -> rows; i++)
	{
	    big = 0.0;
	    for (j = 0; j < m -> rows; j++)
	    {
		if ((temp = fabs(RbfMatrixGetValue(m, i, j))) > big)
		    big = temp;
	    }
	    if (big == 0.0)
	    {
		free(vv);
		return 0;
	    }
	    vv[i] = 1.0/big;
	}
	for (j = 0; j < m -> rows; j++)
	{
	    for (i = 0; i < j; i++)
	    {
		sum = RbfMatrixGetValue(m, i, j);
		for (k = 0; k < i; k++)
		    sum -= RbfMatrixGetValue(m, i, k) *
			   RbfMatrixGetValue(m, k, j);
		RbfMatrixSetValue(m, i, j, sum);
	    }
	    big = 0.0;
	    for (i = j; i < m -> rows; i++)
	    {
		sum = RbfMatrixGetValue(m, i, j);
		for (k = 0; k < j; k++)
		    sum -= RbfMatrixGetValue(m, i, k) *
			   RbfMatrixGetValue(m, k, j);
		RbfMatrixSetValue(m, i, j, sum);
		if ((dum = vv[i] * fabs(sum)) >= big)
		{
		    big = dum;
		    imax = i;
		}
	    }
	    if (j != imax)
	    {
		for (k = 0; k < m -> rows; k++)
		{
		    dum = RbfMatrixGetValue(m, imax, k);
		    RbfMatrixSetValue(m, imax, k, 
			RbfMatrixGetValue(m, j, k));
		    RbfMatrixSetValue(m, j, k, dum);
		}
		dum = vv[imax];
		vv[imax] = vv[j];
		vv[j] = dum;
	    }
	    indx[j] = imax;
	    if (RbfMatrixGetValue(m, j, j) == 0.0)
	    {
		fprintf(stderr,"RbfLUDcmp: seems to be a singular matrix\n");
		free(vv);
		return 0;
	    }
	    if (j != (m -> rows - 1))
	    {
		dum = 1.0/RbfMatrixGetValue(m, j, j);
		for (i = j+1; i < m -> rows; i++)
		    RbfMatrixSetValue(m, i, j,
			RbfMatrixGetValue(m, i, j) * dum);
	    }
	}
	free(vv);
	return 1;
}

/************************************************************************/
/* function RbfLUBksb							*/
/* solves the linear equation LU*x = b, where LU is placed in m using   */
/* the algorithm RbfLUDcmp. b is replaced by the solution x.		*/
/************************************************************************/

static void RbfLUBksb(RbfFloatMatrix *m, int *indx, float *b)
{
	register float	sum;
	register int	i, ii=0, ip, j;

	for (i = 0; i < m -> rows; i++)
	{
	    ip = indx[i];
	    sum = b[ip];
	    b[ip] = b[i];
	    if (ii)
	    {
		for (j = ii-1; j < i; j++)
		    sum -= RbfMatrixGetValue(m, i, j) * b[j];
	    }
	    else if (sum != 0.0)
		ii = i+1;
	    b[i] = sum;
	}
	for (i = (m -> rows - 1); i >= 0; i--)
	{
	    sum = b[i];
	    for (j = i+1; j < m -> rows; j++)
		sum -= RbfMatrixGetValue(m, i, j) * b[j];
	    b[i] = sum / RbfMatrixGetValue(m, i, i);
	}
	
}

/************************************************************************/
/* function RbfInvMatrix:						*/
/* set m to inverse of m (m = m^-1).					*/
/* returns 0 if impossible, 1 otherwise, negative if kernel error	*/
/* This function makes use of the Gauss-Jordan algorithm, which devides */
/* the matrix m into two triangular matrixes m = l*r. m needs to be not */
/* singulary! (Algorithm taken from "Numerical Recipts")		*/
/************************************************************************/

int	RbfInvMatrix(RbfFloatMatrix *m)
{
	register 	int	i, j;
	register	int	*indx;
	register	float	*b;
	register	int	tmp_err;
	RbfFloatMatrix	help;

#ifdef	DEBUG_MODE
	if (m -> rows != m -> columns)
	    ErrMess("RbfInvMatrix: matrix not of form N x N.\n");
#endif

	if (!RbfAllocMatrix(m -> rows, m -> rows, &help) ||
	    (indx = (int *) malloc(m -> rows * sizeof(int))) == NULL ||
	    (b = (float *) malloc(m -> rows * sizeof(float))) == NULL)
	{
	    ErrMess("RbfInvMatrix: impossible to allocate helpmatrix.\n");
	    return KRERR_INSUFFICIENT_MEM;
	}

	RbfSetMatrix(&help, m);
	if ((tmp_err = RbfLUDcmp(&help, indx)) != 1)
	{
	    free(b);
	    free(indx);
	    RbfFreeMatrix(&help);
	    return tmp_err;
	}

	for (j = 0; j < m -> rows; j++)
	{
	    for (i = 0; i < m -> rows; i++)
		b[i] = 0.0;
	    b[j] = 1.0;
	    RbfLUBksb(&help, indx, b);
	    for (i = 0; i < m -> rows; i++)
		RbfMatrixSetValue(m, i, j, b[i]);
	}

	free(b);
	free(indx);
	RbfFreeMatrix(&help);

	return 1;
}

#else

/************************************************************************/
/* function RbfInvMatrix:						*/
/* set m to inverse of m (m = m^-1).					*/
/* returns 0 if impossible, 1 otherwise, negative if kernel error	*/
/* This function makes use of the Gauss-Jordan algorithm, which devides */
/* the matrix m into two triangular matrixes m = l*r. m needs to be not */
/* singulary! (Algorithm taken from "Stoer: Numerische Mathematik 1")	*/
/************************************************************************/

int	RbfInvMatrix(m)
RbfFloatMatrix	*m;
{
	RbfFloatMatrix	help;
	int		i, j, r, k, n;
	float		max, hr, hi;

#ifdef	DEBUG_MODE
	if (m -> rows != m -> columns)
	    ErrMess("RbfInvMatrix: matrix not of form N x N.\n");
#endif

	n = m -> rows;			/* n = dimension of matrix	*/

	/* allocate helpmatrix: 0. row: field p[N] of algorithm		*/
	/*			1. row: field hv[N] of algorithm	*/

	if (!RbfAllocMatrix(2, n, &help))
	{
	    ErrMess("RbfInvMatrix: impossible to allocate helpmatrix.\n");
	    return KRERR_INSUFFICIENT_MEM;
	}

	/* initialize permutation field:				*/
 
	for (j = 0; j < n; j++)
	    RbfMatrixSetValue(&help, 0, j, (float) j);

	/* invert matrix:						*/
	/* notation in comments: m[r,c] means element at row r column c */

	for (j = 0; j < n; j++)
	{
	    /* looking for pivot:					*/
	    /* find row r, where r >= j and m[r,j] is maximal		*/

	    max = fabs(RbfMatrixGetValue(m, j, j));
	    r = j;
	    for (i = j+1; i < n; i++)
	    {
		hi = fabs(RbfMatrixGetValue(m, j, i));
		if (hi > max)
		{
		    max = hi;
		    r = i;
		}
	    }

	    /* test if matrix is singulary:				*/
	    /* if true, then return directly and report errorcode	*/

	    if (max == 0.0)
	    {
		RbfFreeMatrix(&help);
		return 0;
	    }

	    /* if r != j (r > j) then switch row r with row j and mark	*/
	    /* this in helpmatrix:					*/

	    if (r > j)
	    {
		for (k = 0; k < n; k++)
		{
		    hr = RbfMatrixGetValue(m, j, k);
		    RbfMatrixSetValue(m, j, k, RbfMatrixGetValue(m, r, k));
		    RbfMatrixSetValue(m, r, k, hr);
		}
		hi = RbfMatrixGetValue(&help, 0, j);
		RbfMatrixSetValue(&help, 0, j, RbfMatrixGetValue(&help, 0, r));
		RbfMatrixSetValue(&help, 0, r, hi);
	    }

	    /* do the transformation:					*/

	    hr = 1.0 / RbfMatrixGetValue(m, j, j);
	    for (i = 0; i < n; i++)
	    {
		RbfMatrixSetValue(m, i, j, hr * RbfMatrixGetValue(m, i, j));
	    }
	    RbfMatrixSetValue(m, j, j, hr);
	    for (k = 0; k < n; k++)
		if (k != j)
		{
		    for (i = 0; i < n; i++)
			if (i != j)
			{
				RbfMatrixSetValue(m, i, k,
				    RbfMatrixGetValue(m, i, k) -
				    RbfMatrixGetValue(m, i, j) *
				    RbfMatrixGetValue(m, j, k));
			}
		    RbfMatrixSetValue(m, j, k, 
			-hr * RbfMatrixGetValue(m, j, k));
		}
	}

	/* now switch back the columns:					*/

	for (i = 0; i < n; i++)
	{
	    for (k = 0; k < n; k++)
		RbfMatrixSetValue(&help, 1, 
		    (int) RbfMatrixGetValue(&help, 0, k),
		    RbfMatrixGetValue(m, i, k));
	    for (k = 0; k < n; k++)
		RbfMatrixSetValue(m, i, k,
		    RbfMatrixGetValue(&help, 1, k));
	}

	/* report success:						*/

	RbfFreeMatrix(&help);
	return 1;
}

#endif

/************************************************************************/
/* function RbfMulTranspMatrix:						*/
/* set m1 to m2*m2T. number of rows of m2 must be equal to number of    */
/* rows and columns of m1.                                              */
/************************************************************************/

void	RbfMulTranspMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2)
{
    int	dest_c;
    int	dest_r;
    int	count;
    register float scalar_product;

#ifdef	DEBUG_MODE
	if (m2 -> rows != m1 -> rows ||
	    m2 -> rows != m1 -> columns)
	{
		ErrMess("RbfMulTranspMatrix: incompatible matrixes.\n");
	}
#endif

    /* notice that m2*m2T is a symmetric matrix!                        */

    for (dest_r = 0; dest_r < m1 -> rows; dest_r++)
    {
	for (dest_c = dest_r; dest_c < m1 -> rows; dest_c++)
	{
	    scalar_product = 0.0;
	    for (count = 0; count < m2 -> columns; count++)
	    {
		scalar_product += RbfMatrixGetValue(m2, dest_r, count) *
			          RbfMatrixGetValue(m2, dest_c, count);
	    }
	    RbfMatrixSetValue(m1, dest_r, dest_c, scalar_product);
	    if (dest_r != dest_c)
		RbfMatrixSetValue(m1, dest_c, dest_r, scalar_product);
	}
    }
}

/************************************************************************/
/* function RbfMulMatrix:						*/
/* set m1 to m2*m3. number of columns of m2 must be equal to number of  */
/* rows of m3. m1 must be allocated with r = number of rows of m2 and   */
/* c = number of columns of m3.						*/
/************************************************************************/

void	RbfMulMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2, RbfFloatMatrix *m3)
{
	int	dest_c;
	int	dest_r;
	int	count;

#ifdef	DEBUG_MODE
	if (m1 -> rows != m2 -> rows ||
	    m1 -> columns != m3 -> columns ||
	    m2 -> columns != m3 -> rows)
	{
		ErrMess("RbfMulMatrix: incompatible matrixes.\n");
	}
#endif

	/* This seems to be a strange way to multiply two matrices but  */
	/* it prevents the swapper from trashing:                       */

	RbfClearMatrix(m1, 0.0);

	for (dest_r = 0; dest_r < m1 -> rows; dest_r++)
	{
	    for (count = 0; count < m2 -> columns; count++)
	    {
		for (dest_c = 0; dest_c < m1 -> columns; dest_c++)
		    RbfMatrixSetValue(m1, dest_r, dest_c,
			    RbfMatrixGetValue(m2, dest_r, count) *
			    RbfMatrixGetValue(m3, count, dest_c) +
			    RbfMatrixGetValue(m1, dest_r, dest_c));
	    }
	}
}

/************************************************************************/
/* set m1 to m2+m3. number of columns of m1, m2 and m3 must be equal.	*/
/* number of rows of m1, m2 and m3 must be equal.			*/
/************************************************************************/

void	RbfAddMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2, RbfFloatMatrix *m3)
{
	int	dest_c;
	int	dest_r;

#ifdef	DEBUG_MODE
	if (!(m1 -> rows == m2 -> rows &&
	      m2 -> rows == m3 -> rows) ||
	    !(m1 -> columns == m2 -> columns &&
	      m2 -> columns == m3 -> columns)
	   )
	{
		ErrMess("RbfAddMatrix: incompatible matrixes.\n");
	}
#endif

	for (dest_r = 0; dest_r < m1 -> rows; dest_r++)
	    for (dest_c = 0; dest_c < m1 -> columns; dest_c++)
		RbfMatrixSetValue(m1, dest_r, dest_c,
		    RbfMatrixGetValue(m2, dest_r, dest_c) +
		    RbfMatrixGetValue(m3, dest_r, dest_c));
}

/************************************************************************/
/* function RbfPrintMatrix:						*/
/* print out matrix m to stream (file) s				*/
/************************************************************************/

void	RbfPrintMatrix(RbfFloatMatrix *m, FILE *s)
{
	int	r;
	int	c;

	for (r = 0; r < m -> rows; r++)
	{
		for (c = 0; c < m -> columns; c++)
			fprintf(s, "%.4e ",RbfMatrixGetValue(m, r, c));
		fprintf(s,"\n");
	}
}

/************************************************************************/
/* function ErrMess:                                                    */
/* print message to stderr			                        */
/************************************************************************/

void    ErrMess(char *message)
{
        fprintf(stderr, "%s", message);
}

/************************************************************************/
/* end of file								*/
/************************************************************************/

















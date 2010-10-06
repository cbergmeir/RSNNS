/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/matrix.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel standard matrix operations
  NOTES          :

  AUTHOR         : Michael Vogt
  DATE           : 10.02.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:28 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef	_RBF_MATRIX_DEFINED_
#define _RBF_MATRIX_DEFINED_


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

extern int	RbfAllocMatrix(int r, int c, RbfFloatMatrix *m);

/************************************************************************/
/* deallocate matrix m							*/
/************************************************************************/

extern void	RbfFreeMatrix(RbfFloatMatrix *m);

/************************************************************************/
/* set all elements of matrix m to value c				*/
/************************************************************************/

extern void	RbfClearMatrix(RbfFloatMatrix *m, double c);

/************************************************************************/
/* returns the square norm of  matrix m            		     	*/
/************************************************************************/

extern float	RbfSquareOfNorm(RbfFloatMatrix *m);

/************************************************************************/
/* sets m to the idempotent matrix           				*/
/************************************************************************/

extern void	RbfIdempotentMatrix(RbfFloatMatrix *m);

/************************************************************************/
/* multiplies matrix m with a constant factor  				*/
/************************************************************************/

extern void	RbfMulScalarMatrix(RbfFloatMatrix *m, float a);

/************************************************************************/
/* set m1 to m2 (m1 = m2). m1 must be allocated with the same		*/
/* dimensions as m2							*/
/************************************************************************/

extern void	RbfSetMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2);

/************************************************************************/
/* set m1 to m2 transposed (m1 = m2T)					*/
/* number of rows of m1 must be equal to number of columns of m2.	*/
/* number of columns of m1 must be equal to number of rows of m2.	*/
/************************************************************************/

extern void	RbfTranspMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2);

/************************************************************************/
/* set m to inverse of m (m = m^-1).					*/
/* returns 0 if impossible, 1 otherwise, negative if kernel error	*/
/************************************************************************/

extern int	RbfInvMatrix(RbfFloatMatrix *m);

/************************************************************************/
/* function RbfMulTranspMatrix:						*/
/* set m1 to m2*m2T. number of rows of m2 must be equal to number of    */
/* rows and columns of m1.                                              */
/************************************************************************/

extern void	RbfMulTranspMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2);

/************************************************************************/
/* set m1 to m2*m3. number of columns of m2 must be equal to number of  */
/* rows of m3. m1 must be allocated with r = number of rows of m2 and   */
/* c = number of columns of m3.						*/
/************************************************************************/

extern void	RbfMulMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2, RbfFloatMatrix *m3); 

/************************************************************************/
/* set m1 to m2+m3. number of columns of m1, m2 and m3 must be equal.	*/
/* number of rows of m1, m2 and m3 must be equal.			*/
/************************************************************************/

extern void	RbfAddMatrix(RbfFloatMatrix *m1, RbfFloatMatrix *m2, RbfFloatMatrix *m3); 

/************************************************************************/
/* print out matrix m to stream (file) s				*/
/************************************************************************/

extern void	RbfPrintMatrix(RbfFloatMatrix *m, FILE *s);

/************************************************************************/
/* print message to stderr			                        */
/************************************************************************/

extern void    ErrMess(char *message);

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



#endif







/* 141 lines generated by deleteprivatedefinitions.awk */

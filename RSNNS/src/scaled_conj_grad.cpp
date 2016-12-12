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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/scaled_conj_grad.c,v $
  SHORTNAME      : scaled_conj_grad.c
  SNNS VERSION   : 4.2
  
  PURPOSE        : SNNS-Kernel Learning Functions
  NOTES          : with following learning functions:
                   - SCG
  
  AUTHOR         : Bruno ORSIER
  DATE           : 09.95
  VERSION        : 1.01

  CHANGES :1.01 -> 1.02 
		 - bug fix: in case the first computed gradient is null, 
		 norm_of_p_2 = |p|^2 is equal to zero and then a division by 
		 zero occurs in the computation of sigma.  The 4th learning 
		 parameter, tolerance, is now also used to check whether |p| 
		 is almost equal to zero. If the answer is yes, SCG stops.  
		 SCG also now stops if the norm of the gradient is smaller 
		 than tolerance.
			
		 1.0 -> 1.01    
                 new termination criterion, based on the book 'Numerical
                 Recipes'. Takes into account the floating-point precision of
		 the machine. (new #define TOLERANCE in .ph)
		 
		 new learning parameters with defaults
  
		 Validation-function by Thomas Gern
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SnnsCLib.h"

#ifdef SCG_DEBUG
#define TRACE(s)         printf s
#else
#define TRACE(s)
#endif


/*****************************************************************************
  FUNCTION : LEARN_SCG

  PURPOSE  : main function for SCG learning algorithm
  RETURNS  : kernel error code
  NOTES    : 

  UPDATE   : 
******************************************************************************/
krui_err SnnsCLib::LEARN_SCG(int start_pattern, int end_pattern, float *parameterInArray,
		   int NoOfInParams, float **parameterOutArray,
		   int *NoOfOutParams)
{
    //static float    LEARN_SCG_OutParameter[1]; /* LEARN_SCG_OutParameter[0] stores the
    //				      * learning error. */
    //static int      LEARN_SCG_k, LEARN_SCG_restart_scg = FALSE,
    //            LEARN_SCG_stop_scg = FALSE,
    //          LEARN_SCG_success;
    //static int      LEARN_SCG_count_under_tol ; /* scg stops after 3 consecutive under
    //			  	        tolerance cases. This is the counter. */
    //static float    LEARN_SCG_delta, LEARN_SCG_norm_of_p_2, LEARN_SCG_lambda, LEARN_SCG_lambda_bar, LEARN_SCG_current_error, 
    //	            LEARN_SCG_old_error, LEARN_SCG_norm_of_rk ;
    //static FlintType* *LEARN_SCG_weights ;
    //static FlintType  *LEARN_SCG_old_gradient, *LEARN_SCG_p, *LEARN_SCG_r, *LEARN_SCG_old_weights, *LEARN_SCG_step ;

    register FlagWord flags;
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;

    int   i, ret_code ;
    int   start_scg,  make_allocations, under_tolerance ;
    float sigma,  mu, alpha, grand_delta, beta ;
    float sigma_1, lambda_1, tolerance ;    /* learning parameters */
    
    
    if (NoOfInParams < 4)	
	return (KRERR_PARAMETERS); /* Not enough input parameters  */

    /* DEFAULTS: */

    if (( sigma_1 = LEARN_PARAM1( parameterInArray )) == 0.0) 
	  sigma_1 = SCG_FIRST_SIGMA ;

    if (( lambda_1 = LEARN_PARAM2( parameterInArray )) == 0.0) 
	lambda_1 = SCG_FIRST_LAMBDA ;

				/* param 3 is DELTA_max */
    if (( tolerance = LEARN_PARAM4( parameterInArray )) == 0.0) 
	  tolerance = SCG_TOLERANCE ; 


    *NoOfOutParams = 1;		/* One return value is available (the
				 * learning error)  */
    *parameterOutArray = LEARN_SCG_OutParameter; /* set the output parameter reference  */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    start_scg = NetModified || NetInitialize || LearnFuncHasChanged ;
    make_allocations = NetModified || LearnFuncHasChanged ;
    
    if (NetModified) {		/* Net has been modified */
      /* count the no. of I/O units and check the patterns  */
      ret_code = kr_IOCheck();
      if (ret_code < KRERR_NO_ERROR)
	return (ret_code);

      /* sort units by topology and by topologic type  */
      ret_code = kr_topoSort(TOPOLOGICAL_FF);
      if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	return (ret_code);
      NetModified = FALSE;
    }

    if (make_allocations) {
       int ts = 0, malloc_size ;
       
      /* store pointers to weigths and biases into a table */
      /* 1) determines the size of the table */
      scg_space_size = 0 ;
      FOR_ALL_UNITS(unit_ptr)
	if (!IS_INPUT_UNIT(unit_ptr) && !IS_SPECIAL_UNIT(unit_ptr)) {
	flags = unit_ptr->flags;
	scg_space_size++ ; /* bias */
	if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {
	  /* unit is in use  */
	  if (flags & UFLAG_SITES) { /* unit has sites  */
	    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
	      scg_space_size++;
	  } else {	/* unit has no sites   */
	    if (flags & UFLAG_DLINKS) {
	      /* unit has direct links         */
	      FOR_ALL_LINKS(unit_ptr, link_ptr)
		scg_space_size++;
	    }
	  }
	}
      }
      TRACE(("SCG - weights+biases space size : %d\n",scg_space_size));

      /* allocate space for all vectors of size scg_space_size */
      malloc_size = scg_space_size*(sizeof(FlintType *)) ;
      
      if (LEARN_SCG_weights != NULL) free(LEARN_SCG_weights) ;
      LEARN_SCG_weights = (FlintType **)malloc(malloc_size) ;
      if (scg_gradient != NULL) free(scg_gradient) ;
      scg_gradient = (FlintType **)malloc(malloc_size) ;
      if (LEARN_SCG_step != NULL) free(LEARN_SCG_step) ;
      LEARN_SCG_step = (FlintType *) malloc(scg_space_size*sizeof(FlintType)) ;
      if (LEARN_SCG_p != NULL) free(LEARN_SCG_p) ;
      LEARN_SCG_p = (FlintType *) malloc(scg_space_size*sizeof(FlintType)) ;
      if (LEARN_SCG_r != NULL) free(LEARN_SCG_r) ;
      LEARN_SCG_r = (FlintType *) malloc(scg_space_size*sizeof(FlintType)) ;
      if (LEARN_SCG_old_weights != NULL) free(LEARN_SCG_old_weights) ;
      LEARN_SCG_old_weights = (FlintType *) malloc(scg_space_size*sizeof(FlintType)) ;
      if (LEARN_SCG_old_gradient != NULL) free(LEARN_SCG_old_gradient) ;
      LEARN_SCG_old_gradient = (FlintType *) malloc(scg_space_size*sizeof(FlintType)) ;

      if ((LEARN_SCG_step == NULL)||(scg_gradient == NULL)||(LEARN_SCG_weights == NULL)
	||(LEARN_SCG_p==NULL)||(LEARN_SCG_r==NULL) ||(LEARN_SCG_old_weights==NULL) ||(LEARN_SCG_old_gradient ==NULL))
	{
	  //printf("SCG : malloc problem\n") ;
	  return(KRERR_CRITICAL_MALLOC);
      }
      else TRACE(("SCG - malloc's done\n"));

      /* 2) store the adresses */

      FOR_ALL_UNITS(unit_ptr)
	if (!IS_INPUT_UNIT(unit_ptr) && !IS_SPECIAL_UNIT(unit_ptr)) {
	    flags = unit_ptr->flags;
	    scg_gradient[ts] = &unit_ptr->value_a ;
	    LEARN_SCG_weights[ts++] = &unit_ptr->bias;
	    if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {
		/* unit is in use  */
		if (flags & UFLAG_SITES) { /* unit has sites  */
		    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {
			scg_gradient[ts] = &link_ptr->value_a ;
			LEARN_SCG_weights[ts++] = &link_ptr->weight;
		    }
		} else {	/* unit has no sites   */
		    if (flags & UFLAG_DLINKS) {
			/* unit has direct links         */
			FOR_ALL_LINKS(unit_ptr, link_ptr) {
			    scg_gradient[ts] = &link_ptr->value_a ;
			    LEARN_SCG_weights[ts++] = &link_ptr->weight;
			}
		    }
		}
	    }
	}
      
    }				/* make_allocations */
    

    
    if (start_scg || LEARN_SCG_restart_scg) {
      /* now we choose initial values for SCG parameters */
      LEARN_SCG_lambda = lambda_1 ;
      LEARN_SCG_lambda_bar = 0.0 ;
      LEARN_SCG_success = TRUE ;
      LEARN_SCG_k = 1 ;
      LEARN_SCG_restart_scg = FALSE ;
      LEARN_SCG_count_under_tol = 0 ;
      TRACE(("START || RESTART\n"));
    } 

    TRACE(("\nSCG K=%d\n",LEARN_SCG_k));
    if (start_scg) {
      TRACE(("SCG - starting\n"));
      /* initialize  p and r */

      /* we have starting weight vector (initialized through xgui or snnsbat)
	 and we need the current gradient : we do as in LEARN_backpropBatch */
      ret_code = compute_gradient(start_pattern,end_pattern,
				   LEARN_PARAM3(parameterInArray),
				  &LEARN_SCG_current_error) ;
      if(ret_code != KRERR_NO_ERROR) return (ret_code);
      
      /* now we have the gradient in vector 'gradient' and copy it to p and r */

      for (i=0 ; i < scg_space_size ; i++) {
	LEARN_SCG_p[i] = - *scg_gradient[i] ;
	LEARN_SCG_r[i] = LEARN_SCG_p[i] ;
      }
      LEARN_SCG_norm_of_rk = sqrt(square_of_norm(LEARN_SCG_r,scg_space_size)) ;
      start_scg = FALSE ;
      LEARN_SCG_stop_scg = FALSE ;
    }				/* end of start scg */

    if (LEARN_SCG_stop_scg) {
      LEARN_SCG_OutParameter[0] = LEARN_SCG_current_error ;
      return (ret_code);
    }
    
    /* main part of SCG */
    
    if (LEARN_SCG_success) {/* calculate second order information */
      TRACE(("success\n")) ;
      LEARN_SCG_norm_of_p_2 = square_of_norm(LEARN_SCG_p,scg_space_size) ;
      TRACE((" norm_of_p_2=%e ", LEARN_SCG_norm_of_p_2)) ;
      if (LEARN_SCG_norm_of_p_2 <= tolerance*tolerance) {
	LEARN_SCG_stop_scg = 1 ;
	LEARN_SCG_OutParameter[0] = LEARN_SCG_current_error ;
	LEARN_SCG_k++;
	return(ret_code) ;
      }
      sigma = sigma_1/sqrt(LEARN_SCG_norm_of_p_2) ;
      /* in order to compute the new step we need a need gradient. */
      for (i=0 ; i < scg_space_size ; i++) {
	LEARN_SCG_old_gradient[i] = *scg_gradient[i] ;
	LEARN_SCG_old_weights[i] = *LEARN_SCG_weights[i];
      }
      LEARN_SCG_old_error = LEARN_SCG_current_error ;
      TRACE(("old_error=%e ", LEARN_SCG_old_error)) ;
      /* now we move to the new point in weight space */
      for (i=0 ; i < scg_space_size ; i++)
	*LEARN_SCG_weights[i] += sigma*LEARN_SCG_p[i] ;
      /* and compute the new gradient */
      ret_code = compute_gradient(start_pattern,end_pattern,
				   LEARN_PARAM3(parameterInArray),
				  &LEARN_SCG_current_error) ;
      if(ret_code != KRERR_NO_ERROR) return (ret_code);
      TRACE(("current_error = %e",LEARN_SCG_current_error)) ;
      /* now we have the new gradient and we continue the step computation */
      for (i=0 ; i < scg_space_size ; i++)
	LEARN_SCG_step[i] = (*scg_gradient[i]-LEARN_SCG_old_gradient[i])/sigma ;

      LEARN_SCG_delta = product_of_xt_by_y(LEARN_SCG_p,LEARN_SCG_step,scg_space_size) ;
      TRACE(("\n delta=%e\n",LEARN_SCG_delta)) ;
      TRACE(("end of success true\n"));
    }				/* end of if (success) */

    /* scale delta */
    LEARN_SCG_delta += (LEARN_SCG_lambda - LEARN_SCG_lambda_bar) * LEARN_SCG_norm_of_p_2 ;
    TRACE(("delta scaled = %e\n", LEARN_SCG_delta));
    
    if (LEARN_SCG_delta <=0) {		/* make the Hessian positive definite */
      LEARN_SCG_lambda_bar = 2.0 * (LEARN_SCG_lambda - LEARN_SCG_delta/LEARN_SCG_norm_of_p_2) ;
      LEARN_SCG_delta = -LEARN_SCG_delta + LEARN_SCG_lambda*LEARN_SCG_norm_of_p_2 ;
      LEARN_SCG_lambda = LEARN_SCG_lambda_bar ;
      TRACE(("hessian: l_bar=%e delta=%e lambda=%e\n",LEARN_SCG_lambda_bar,LEARN_SCG_delta,LEARN_SCG_lambda));
      
    }

    /* calculate step size */
    mu = product_of_xt_by_y(LEARN_SCG_p,LEARN_SCG_r,scg_space_size) ;
    alpha = mu/LEARN_SCG_delta ;

    TRACE(("mu=%e alpha=%e\n",mu,alpha));
    
    /* calculate the comparison parameter */

    /* we must compute a new gradient but this time we do not want to */
    /* keep the previous values  (they were useful only for */
    /* approximating the Hessian )*/
    
    for (i=0 ; i < scg_space_size ; i++) *LEARN_SCG_weights[i]=LEARN_SCG_old_weights[i]+alpha*LEARN_SCG_p[i];
    ret_code = compute_gradient(start_pattern,end_pattern,
				LEARN_PARAM3(parameterInArray),
				&LEARN_SCG_current_error) ;
    if(ret_code != KRERR_NO_ERROR) return (ret_code);
    
    TRACE(("current error=%e\n",LEARN_SCG_current_error)) ;
    
    grand_delta = 2.0*LEARN_SCG_delta*(LEARN_SCG_old_error-LEARN_SCG_current_error)/(mu*mu) ;
    TRACE(("grand delta=%e\n",grand_delta));
    if (grand_delta >= 0) {	/* a successful reduction in error */
				/* can be made */
      float r_sum = 0.0 ;	/* product of r(k+1) by r(k) */
      float tmp ;


      TRACE(("ERROR REDUCTION of %e %% --- ",
	     (LEARN_SCG_old_error-LEARN_SCG_current_error)/LEARN_SCG_old_error*100.0));

      under_tolerance =
	2.0 * fabs(LEARN_SCG_old_error-LEARN_SCG_current_error)
	<=
	tolerance * (fabs(LEARN_SCG_old_error)+fabs(LEARN_SCG_current_error)+1E-10) ;
      TRACE(("under tolerance = %d\n",under_tolerance));
	
      /* we already are at w(k+1) in weight space, so we don't move */
      
      /* we compute |r(k)| before changing r to r(k+1) */
      LEARN_SCG_norm_of_rk = sqrt(square_of_norm(LEARN_SCG_r,scg_space_size)) ;

      /* now, r <- r(k+1) */
      for (i=0 ; i < scg_space_size ; i++) {
	tmp = -1.0 * *scg_gradient[i] ;
	r_sum +=  tmp * LEARN_SCG_r[i] ;
	LEARN_SCG_r[i] = tmp ;
      }

      LEARN_SCG_lambda_bar = 0 ;
      LEARN_SCG_success = TRUE ;
      
      if (LEARN_SCG_k >= scg_space_size) {
	LEARN_SCG_restart_scg = TRUE ;
	for (i=0 ; i < scg_space_size ; i++) LEARN_SCG_p[i] = LEARN_SCG_r[i] ;
      }
      else {			/* compute new conjugate direction */
	beta = (square_of_norm(LEARN_SCG_r,scg_space_size) - r_sum)/mu ;
	TRACE(("beta=%e\n",beta));
	for (i=0 ; i < scg_space_size ; i++) LEARN_SCG_p[i] = LEARN_SCG_r[i]+ beta*LEARN_SCG_p[i] ;

	LEARN_SCG_restart_scg = FALSE ;
      }

      if (grand_delta >=0.75) LEARN_SCG_lambda = LEARN_SCG_lambda/4.0 ;
    }
    
    else {
      /* no reduction can be made */
      TRACE(("NO REDUCTION\n"));
      under_tolerance = 0 ;
      /* we must go back to w(k), since w(k)+alpha*p(k) is not better */
      for (i=0 ; i < scg_space_size ; i++) *LEARN_SCG_weights[i] = LEARN_SCG_old_weights[i] ;
      LEARN_SCG_current_error = LEARN_SCG_old_error ;
      
      LEARN_SCG_lambda_bar = LEARN_SCG_lambda ;
      LEARN_SCG_success = FALSE ;
    }

    if (grand_delta < 0.25)
      LEARN_SCG_lambda = LEARN_SCG_lambda+LEARN_SCG_delta*(1-grand_delta)/LEARN_SCG_norm_of_p_2 ;

				/* let's try to prevent floating-point
				   exceptions. Lambda may become too big even
				   with the under_tolerance criterion, in case
				   of a several consecutive 'NO REDUCTION'. */
    if (LEARN_SCG_lambda > MAXFLOAT) LEARN_SCG_lambda = MAXFLOAT ;
    
    TRACE(("lambda after resizing=%e\n",LEARN_SCG_lambda)) ;

    TRACE(("|r(k)| = %e\n",LEARN_SCG_norm_of_rk)) ;

    /* scg stops after 3 consecutive under tolerance steps */
    if (under_tolerance)     LEARN_SCG_count_under_tol++ ;
    else LEARN_SCG_count_under_tol = 0 ;

    LEARN_SCG_stop_scg = (LEARN_SCG_count_under_tol > 2) || (LEARN_SCG_norm_of_rk <= tolerance) ;
    TRACE(("STOP SCG : count_under_tol=%d, stop_scg=%d\n", 
	   LEARN_SCG_count_under_tol, LEARN_SCG_stop_scg)) ;

    if (LEARN_SCG_stop_scg) LEARN_SCG_count_under_tol = 0 ;	/* just in case the user wants to try
					   with a smaller tolerance */

    LEARN_SCG_k++;
       
    LEARN_SCG_OutParameter[0] = LEARN_SCG_current_error ;
    
    return (ret_code);
}



/*****************************************************************************
  FUNCTION : product_of_xt_by_y

  PURPOSE  : 
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
 float SnnsCLib::product_of_xt_by_y(float * x, float * y, int tab_size)
{
  int indice ;
  float sum = 0 ;

  for (indice = 0 ; indice < tab_size ; indice++)
    sum += x[indice]*y[indice] ;

  return(sum) ;
}


/*****************************************************************************
  FUNCTION : square_of_norm

  PURPOSE  : 
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
 float SnnsCLib::square_of_norm(float * x, int tab_size)
{
  return(product_of_xt_by_y(x,x,tab_size));
}


/*****************************************************************************
  FUNCTION : compute_gradient

  PURPOSE  : computes the SCG gradient
  RETURNS  : kernel error code
  NOTES    : 

  UPDATE   : 
******************************************************************************/
 krui_err SnnsCLib::compute_gradient(int start_pattern, int end_pattern, 
				 float delta_max,  float * error)
{
    int pattern_no, sub_pat_no, i ;
  
    clearDeltas();

    /* compute the necessary sub patterns */
  
    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    *error = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){
    
	propagateNetForward(pattern_no,sub_pat_no);  /* Forward propagation */
	
	*error += propagateNetBackwardBatch(pattern_no,sub_pat_no, delta_max, NULL, 0);
	/* -0.5 * gradient's coordinates are stored in value_a's */
    }

    for (i=0 ; i< scg_space_size ; i++) 
	*scg_gradient[i] = - 2.0 * *scg_gradient[i];

    return (KernelErrorCode);
}



/*****************************************************************************
  FUNCTION : TEST_SCG

  PURPOSE  : tests the network using TEST_backprop.
  RETURNS  : kernel error code
  NOTES    : saves the value of NetModified for initialising LEARN_SCG

  UPDATE   : 18.10.95 by Thomas Gern
******************************************************************************/
krui_err SnnsCLib::TEST_SCG(int start_pattern, int end_pattern,
		  float *parameterInArray, int NoOfInParams,
		  float **parameterOutArray, int *NoOfOutParams)
{
    bool OldNetModified;
    krui_err ret_val;

    OldNetModified = NetModified;
    ret_val = TEST_backprop(start_pattern, end_pattern, 
			    parameterInArray, NoOfInParams,
			    parameterOutArray, NoOfOutParams);
    NetModified = OldNetModified;
    return ret_val;
}


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
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#include "kr_typ.h"		/* Kernel Types and Constants  */
#include "kr_const.h"		/* Constant Declarators for SNNS-Kernel */
#include "kernel.h"		/* kernel function prototypes  */
#include "kr_mac.h"		/* Kernel Macros   */
#include "glob_typ.h"
#include "learn_f.h"
#include "scaled_conj_grad.ph"
#include "kr_newpattern.h"


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
krui_err LEARN_SCG(int start_pattern, int end_pattern, float *parameterInArray,
		   int NoOfInParams, float **parameterOutArray,
		   int *NoOfOutParams)
{
    static float    OutParameter[1]; /* OutParameter[0] stores the
				      * learning error. */
    static int      k, restart_scg = FALSE,
	            stop_scg = FALSE,
	            success;
    static int      count_under_tol ; /* scg stops after 3 consecutive under
			  	        tolerance cases. This is the counter. */
    static float    delta, norm_of_p_2, lambda, lambda_bar, current_error, 
	            old_error, norm_of_rk ;
    static FlintType* *weights ;
    static FlintType  *old_gradient, *p, *r, *old_weights, *step ;

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
    *parameterOutArray = OutParameter; /* set the output parameter reference  */
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
      
      if (weights != NULL) free(weights) ;
      weights = (FlintType **)malloc(malloc_size) ;
      if (scg_gradient != NULL) free(scg_gradient) ;
      scg_gradient = (FlintType **)malloc(malloc_size) ;
      if (step != NULL) free(step) ;
      step = (FlintType *) malloc(scg_space_size*sizeof(FlintType)) ;
      if (p != NULL) free(p) ;
      p = (FlintType *) malloc(scg_space_size*sizeof(FlintType)) ;
      if (r != NULL) free(r) ;
      r = (FlintType *) malloc(scg_space_size*sizeof(FlintType)) ;
      if (old_weights != NULL) free(old_weights) ;
      old_weights = (FlintType *) malloc(scg_space_size*sizeof(FlintType)) ;
      if (old_gradient != NULL) free(old_gradient) ;
      old_gradient = (FlintType *) malloc(scg_space_size*sizeof(FlintType)) ;

      if ((step == NULL)||(scg_gradient == NULL)||(weights == NULL)
	||(p==NULL)||(r==NULL) ||(old_weights==NULL) ||(old_gradient ==NULL))
	{
	  printf("SCG : malloc problem\n") ;
	  return(KRERR_CRITICAL_MALLOC);
      }
      else TRACE(("SCG - malloc's done\n"));

      /* 2) store the adresses */

      FOR_ALL_UNITS(unit_ptr)
	if (!IS_INPUT_UNIT(unit_ptr) && !IS_SPECIAL_UNIT(unit_ptr)) {
	    flags = unit_ptr->flags;
	    scg_gradient[ts] = &unit_ptr->value_a ;
	    weights[ts++] = &unit_ptr->bias;
	    if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {
		/* unit is in use  */
		if (flags & UFLAG_SITES) { /* unit has sites  */
		    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {
			scg_gradient[ts] = &link_ptr->value_a ;
			weights[ts++] = &link_ptr->weight;
		    }
		} else {	/* unit has no sites   */
		    if (flags & UFLAG_DLINKS) {
			/* unit has direct links         */
			FOR_ALL_LINKS(unit_ptr, link_ptr) {
			    scg_gradient[ts] = &link_ptr->value_a ;
			    weights[ts++] = &link_ptr->weight;
			}
		    }
		}
	    }
	}
      
    }				/* make_allocations */
    

    
    if (start_scg || restart_scg) {
      /* now we choose initial values for SCG parameters */
      lambda = lambda_1 ;
      lambda_bar = 0.0 ;
      success = TRUE ;
      k = 1 ;
      restart_scg = FALSE ;
      count_under_tol = 0 ;
      TRACE(("START || RESTART\n"));
    } 

    TRACE(("\nSCG K=%d\n",k));
    if (start_scg) {
      TRACE(("SCG - starting\n"));
      /* initialize  p and r */

      /* we have starting weight vector (initialized through xgui or snnsbat)
	 and we need the current gradient : we do as in LEARN_backpropBatch */
      ret_code = compute_gradient(start_pattern,end_pattern,
				   LEARN_PARAM3(parameterInArray),
				  &current_error) ;
      if(ret_code != KRERR_NO_ERROR) return (ret_code);
      
      /* now we have the gradient in vector 'gradient' and copy it to p and r */

      for (i=0 ; i < scg_space_size ; i++) {
	p[i] = - *scg_gradient[i] ;
	r[i] = p[i] ;
      }
      norm_of_rk = sqrt(square_of_norm(r,scg_space_size)) ;
      start_scg = FALSE ;
      stop_scg = FALSE ;
    }				/* end of start scg */

    if (stop_scg) {
      OutParameter[0] = current_error ;
      return (ret_code);
    }
    
    /* main part of SCG */
    
    if (success) {/* calculate second order information */
      TRACE(("success\n")) ;
      norm_of_p_2 = square_of_norm(p,scg_space_size) ;
      TRACE((" norm_of_p_2=%e ", norm_of_p_2)) ;
      if (norm_of_p_2 <= tolerance*tolerance) {
	stop_scg = 1 ;
	OutParameter[0] = current_error ;
	k++;
	return(ret_code) ;
      }
      sigma = sigma_1/sqrt(norm_of_p_2) ;
      /* in order to compute the new step we need a need gradient. */
      for (i=0 ; i < scg_space_size ; i++) {
	old_gradient[i] = *scg_gradient[i] ;
	old_weights[i] = *weights[i];
      }
      old_error = current_error ;
      TRACE(("old_error=%e ", old_error)) ;
      /* now we move to the new point in weight space */
      for (i=0 ; i < scg_space_size ; i++)
	*weights[i] += sigma*p[i] ;
      /* and compute the new gradient */
      ret_code = compute_gradient(start_pattern,end_pattern,
				   LEARN_PARAM3(parameterInArray),
				  &current_error) ;
      if(ret_code != KRERR_NO_ERROR) return (ret_code);
      TRACE(("current_error = %e",current_error)) ;
      /* now we have the new gradient and we continue the step computation */
      for (i=0 ; i < scg_space_size ; i++)
	step[i] = (*scg_gradient[i]-old_gradient[i])/sigma ;

      delta = product_of_xt_by_y(p,step,scg_space_size) ;
      TRACE(("\n delta=%e\n",delta)) ;
      TRACE(("end of success true\n"));
    }				/* end of if (success) */

    /* scale delta */
    delta += (lambda - lambda_bar) * norm_of_p_2 ;
    TRACE(("delta scaled = %e\n", delta));
    
    if (delta <=0) {		/* make the Hessian positive definite */
      lambda_bar = 2.0 * (lambda - delta/norm_of_p_2) ;
      delta = -delta + lambda*norm_of_p_2 ;
      lambda = lambda_bar ;
      TRACE(("hessian: l_bar=%e delta=%e lambda=%e\n",lambda_bar,delta,lambda));
      
    }

    /* calculate step size */
    mu = product_of_xt_by_y(p,r,scg_space_size) ;
    alpha = mu/delta ;

    TRACE(("mu=%e alpha=%e\n",mu,alpha));
    
    /* calculate the comparison parameter */

    /* we must compute a new gradient but this time we do not want to */
    /* keep the previous values  (they were useful only for */
    /* approximating the Hessian )*/
    
    for (i=0 ; i < scg_space_size ; i++) *weights[i]=old_weights[i]+alpha*p[i];
    ret_code = compute_gradient(start_pattern,end_pattern,
				LEARN_PARAM3(parameterInArray),
				&current_error) ;
    if(ret_code != KRERR_NO_ERROR) return (ret_code);
    
    TRACE(("current error=%e\n",current_error)) ;
    
    grand_delta = 2.0*delta*(old_error-current_error)/(mu*mu) ;
    TRACE(("grand delta=%e\n",grand_delta));
    if (grand_delta >= 0) {	/* a successful reduction in error */
				/* can be made */
      float r_sum = 0.0 ;	/* product of r(k+1) by r(k) */
      float tmp ;


      TRACE(("ERROR REDUCTION of %e %% --- ",
	     (old_error-current_error)/old_error*100.0));

      under_tolerance =
	2.0 * fabs(old_error-current_error)
	<=
	tolerance * (fabs(old_error)+fabs(current_error)+1E-10) ;
      TRACE(("under tolerance = %d\n",under_tolerance));
	
      /* we already are at w(k+1) in weight space, so we don't move */
      
      /* we compute |r(k)| before changing r to r(k+1) */
      norm_of_rk = sqrt(square_of_norm(r,scg_space_size)) ;

      /* now, r <- r(k+1) */
      for (i=0 ; i < scg_space_size ; i++) {
	tmp = -1.0 * *scg_gradient[i] ;
	r_sum +=  tmp * r[i] ;
	r[i] = tmp ;
      }

      lambda_bar = 0 ;
      success = TRUE ;
      
      if (k >= scg_space_size) {
	restart_scg = TRUE ;
	for (i=0 ; i < scg_space_size ; i++) p[i] = r[i] ;
      }
      else {			/* compute new conjugate direction */
	beta = (square_of_norm(r,scg_space_size) - r_sum)/mu ;
	TRACE(("beta=%e\n",beta));
	for (i=0 ; i < scg_space_size ; i++) p[i] = r[i]+ beta*p[i] ;

	restart_scg = FALSE ;
      }

      if (grand_delta >=0.75) lambda = lambda/4.0 ;
    }
    
    else {
      /* no reduction can be made */
      TRACE(("NO REDUCTION\n"));
      under_tolerance = 0 ;
      /* we must go back to w(k), since w(k)+alpha*p(k) is not better */
      for (i=0 ; i < scg_space_size ; i++) *weights[i] = old_weights[i] ;
      current_error = old_error ;
      
      lambda_bar = lambda ;
      success = FALSE ;
    }

    if (grand_delta < 0.25)
      lambda = lambda+delta*(1-grand_delta)/norm_of_p_2 ;

				/* let's try to prevent floating-point
				   exceptions. Lambda may become too big even
				   with the under_tolerance criterion, in case
				   of a several consecutive 'NO REDUCTION'. */
    if (lambda > MAXFLOAT) lambda = MAXFLOAT ;
    
    TRACE(("lambda after resizing=%e\n",lambda)) ;

    TRACE(("|r(k)| = %e\n",norm_of_rk)) ;

    /* scg stops after 3 consecutive under tolerance steps */
    if (under_tolerance)     count_under_tol++ ;
    else count_under_tol = 0 ;

    stop_scg = (count_under_tol > 2) || (norm_of_rk <= tolerance) ;
    TRACE(("STOP SCG : count_under_tol=%d, stop_scg=%d\n", 
	   count_under_tol, stop_scg)) ;

    if (stop_scg) count_under_tol = 0 ;	/* just in case the user wants to try
					   with a smaller tolerance */

    k++;
       
    OutParameter[0] = current_error ;
    
    return (ret_code);
}



/*****************************************************************************
  FUNCTION : product_of_xt_by_y

  PURPOSE  : 
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
static float product_of_xt_by_y(float * x, float * y, int tab_size)
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
static float square_of_norm(float * x, int tab_size)
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
static krui_err compute_gradient(int start_pattern, int end_pattern, 
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
	
	*error += propagateNetBackwardBatch(pattern_no,sub_pat_no, delta_max,
					    NULL, 0);
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
krui_err TEST_SCG(int start_pattern, int end_pattern,
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


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/learn_f.c,v $
  SHORTNAME      : learn_f
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Learning Functions
  NOTES          : with following learning functions:
	           - Backpropagation
	           - Backpropagation with momentum term
                   - Quickprop
	           - Counterpropagation
	           - BackPercolation
                   - Backpropagation through time
                   - Batch backpropagation through time
                   - Quickprop through time
		   - Kohonen (by Univ. of Tuebingen)

  AUTHOR         : Niels Mache
  DATE           : 01.10.90

  CHANGED BY     : Sven Doering, Michael Vogt, Martin Reczko ,Guenter Mamier
  RCS VERSION    : $Revision: 2.46 $
  LAST CHANGE    : $Date: 1998/05/20 09:35:23 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif
#include <ctype.h>

#include <limits.h>
#define MAXINT INT_MAX

#include "kr_typ.h"		/* Kernel Types and Constants  */
#include "kr_const.h"		/* Constant Declarators for SNNS-Kernel  */
#include "kr_def.h"		/* Default Values  */

#ifndef rand
#include "random.h"	 /*  Randomize Library Function Prototypes  */
#endif

#include "kernel.h"		/* kernel function prototypes  */
#include "kr_mac.h"		/* Kernel Macros   */
#include "glob_typ.h"
#include "kr_ui.h"
#include "kr_art.h"
#include "kr_art1.h"
#include "kr_art2.h"
#include "kr_amap.h"
#include "krart_df.h"
#include "kr_newpattern.h"
#include "kr_JordElm.h"
#include "prun_f.h"

#ifdef PARAGON_KERNEL
#include PARAGON_INCLUDE
#include "kr_ipdef.h"
#endif


#include "learn_f.ph"

extern FlintType OUT_Custom_Python(register FlintType activation);
extern FlintType ACT_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_DERIV_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_2_DERIV_Custom_Python(struct Unit * unit_ptr);


/*****************************************************************************
 *****************************************************************************

  GROUP        : backpropagation learning algorithm

  AUTHOR       : Niels Mache
             
******************************************************************************
******************************************************************************/


/*****************************************************************************
  FUNCTION : propagateNetForward

  PURPOSE  : forward pass for most of the learning algorithms
  RETURNS  : 
  NOTES    : topological forward propagation

  UPDATE   : 05.11.1993
******************************************************************************/

void propagateNetForward(int pattern_no, int sub_pat_no)
{
    register struct Unit *unit_ptr;
    register Patterns in_pat;
    register TopoPtrArray topo_ptr;


    /* calculate startaddress for input pattern array  */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);
    if(in_pat == NULL){
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return;
    }

    topo_ptr = topo_ptr_array;

    /* copy pattern into input unit's activation and calculate output of the
       input units */
    while ((unit_ptr = *++topo_ptr) != NULL) {	
	
	/* topo_ptr points to a (topological sorted) unit
	   stucture (input unit first)  */
	if (unit_ptr->out_func == OUT_IDENTITY)
	    /* identity output function: there is no need to call the output
	       function  */
	    unit_ptr->Out.output = unit_ptr->act = *in_pat++;
	else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->Out.output
		 = kr_PythonOutFunction(unit_ptr->python_out_func,
		                          unit_ptr->act = *in_pat++);
	} else
	    /* no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output 
		= (*unit_ptr->out_func) (unit_ptr->act = *in_pat++);
    }

    /* popagate hidden units  */
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* topo_ptr points to a
						   (topological sorted) unit
						   stucture */
	/* clear error values  */
	unit_ptr->Aux.flint_no = 0.0;

	/* calculate the activation value of the unit: call the activation
	   function if needed  */
	unit_ptr->act = ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(*unit_ptr->act_func) (unit_ptr)) ;

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /* identity output function: there is no need to call the output
	       function  */
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->Out.output
		 = kr_PythonOutFunction(unit_ptr->python_out_func,
		                          unit_ptr->act);
	} else
	    /* no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    /* popagate output units  */
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* topo_ptr points to a
						   (topological sorted) unit
						   stucture */
	/* clear error values  */
	unit_ptr->Aux.flint_no = 0.0;

	/* calculate the activation value of the unit: call the activation
	   function if needed  */
	if(unit_ptr->act_func == ACT_Custom_Python) {
		unit_ptr->act = 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr);
	} else unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /* identity output function: there is no need to call the output
	       function  */
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->Out.output 
		 = kr_PythonOutFunction(unit_ptr->python_out_func,
		                          unit_ptr->act);
	} else
	    /* no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }
}



/*****************************************************************************
  FUNCTION : propagateNetBackward2

  PURPOSE  : backward pass of the backprop learning algorithm
  RETURNS  : network error
  NOTES    : network must be topologically sorted

  UPDATE   : 07.02.1994 by Sven Doering 
******************************************************************************/
static float propagateNetBackward2(int pattern_no, int sub_pat_no, 
				   float learn_parameter, float delta_max)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register float  error, sum_error, eta, devit, learn_error;
    register TopoPtrArray topo_ptr;
    int size;

    sum_error = 0.0;		/* reset network error  */
    eta = learn_parameter;	/* store learn_parameter in CPU register  */

    /* calculate address of the output pattern (with number pattern_no + 1)  */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    if(out_pat == NULL){
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return(-1);
    }
    out_pat += size;


    /* add 3 to no_of_topo_units because the topologic array contains 4 NULL
       pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	devit = *(--out_pat) - unit_ptr->Out.output; /* calc. devitation */
	if ((float) fabs(devit) <= delta_max)
	    continue;

	sum_error += devit * devit; /* sum up the error of the network  */

	/* calc. error for output units	 */
	error = devit * ((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ?
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr));
	/* calc. the error for adjusting weights and bias of the pred.
	   units  */
	if (IS_SPECIAL_UNIT(unit_ptr))
	    learn_error = 0.0;
	else
	    learn_error = eta * error;
	/* adjust bias value  */
	unit_ptr->bias += learn_error;

	if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {
	    /* the unit has direkt links  */
	    FOR_ALL_LINKS(unit_ptr, link_ptr) { /* adjust links and
						   calc. errors of the
						   predecessor units  */
		    link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		    link_ptr->weight += learn_error * link_ptr->to->Out.output;
		}
	    } else {		/* the unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {	
		    /* adjust links and calc. errors of the predecessor units */
		    link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		    link_ptr->weight += learn_error * link_ptr->to->Out.output;
		}
	    }
    }

    /* calculate hidden units only  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	/* calc. the error of the (hidden) unit  */
	error = ((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) * 
	    unit_ptr->Aux.flint_no;
	/* calc. the error for adjusting weights and bias of the pred.
	   units  */
	if (IS_SPECIAL_UNIT(unit_ptr)) 
	    learn_error =0.0;
	else
	    learn_error = eta * error;
	/* adjust bias value  */
	unit_ptr->bias += learn_error;

	if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {
	    /* the unit has direkt links	 */
	    FOR_ALL_LINKS(unit_ptr, link_ptr) {	/* adjust links and
						   calc. sum of errors
						   of the pred. units */
		    if IS_HIDDEN_UNIT
			(link_ptr->to)
			/* this link points to a hidden unit: sum up the
			   error's from previos units  */
			    link_ptr->to->Aux.flint_no += 
				link_ptr->weight * error;
		    link_ptr->weight
			+= learn_error * link_ptr->to->Out.output;
		}
	    } else {		/* the unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {	
		    /* adjust links and calc sum of errors of the pred. units */
		    if IS_HIDDEN_UNIT
			(link_ptr->to)
			/* this link points to a hidden unit: sum up the
			   error's from previos units  */
			    link_ptr->to->Aux.flint_no += 
				link_ptr->weight * error;
		    link_ptr->weight
			+= learn_error * link_ptr->to->Out.output;
		}
	    }
    }

    return (sum_error);		/* return the error of the network */
}



/*****************************************************************************
  FUNCTION : LEARN_backprop

  PURPOSE  : main routine for the backpropagation algorithm
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : delta max

             Output Parameters:  1 : error of the network (sum of all cycles)


  UPDATE   : 05.11.1993
******************************************************************************/
krui_err LEARN_backprop(int start_pattern, int end_pattern,
			float *parameterInArray, int NoOfInParams,
			float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error */
    int             pattern_no, sub_pat_no, no_of_layers;


    KernelErrorCode = KRERR_NO_ERROR;	/* reset return code  */

    /* ####  have to be changed (must be 2)  #### */
    if (NoOfInParams < 1) {	/* Not enough input parameters	 */
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }
    *NoOfOutParams = 1;		/* One return value is available (the
				   learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {
	/* Net has been modified or topologic array isn't initialized */
	/* check the topology of the network  */
	no_of_layers = kr_topoCheck();
	if (KernelErrorCode != KRERR_NO_ERROR)
	    /* an error has occured	 */
	    return (KernelErrorCode);

	if (no_of_layers < 2) {	/* the network has less then 2 layers  */
	    KernelErrorCode = KRERR_FEW_LAYERS;
	    return (KernelErrorCode);
	}
	/* count the no. of I/O units and check the patterns  */
	if (kr_IOCheck() != KRERR_NO_ERROR)
	    return (KernelErrorCode);

	/* sort units by topology and by topologic type  */
	(void) kr_topoSort(TOPOLOGICAL_FF);
	if ((KernelErrorCode != KRERR_NO_ERROR) &&
	    (KernelErrorCode != KRERR_DEAD_UNITS))
	    return (KernelErrorCode);

	NetModified = FALSE;
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	propagateNetForward(pattern_no,sub_pat_no);   /* Forward propagation */

	/* Backward propagation  */
	/* 1st parameter is the learning parameter 2nd parameter is the max.
	   devitation between output pattern and the output of the output
	   unit (delta max) */
	NET_ERROR(OutParameter) +=
	    propagateNetBackward2(pattern_no,sub_pat_no,
				  LEARN_PARAM1(parameterInArray),
				  LEARN_PARAM2(parameterInArray));
    }

    return (KernelErrorCode);
}


/*****************************************************************************
  FUNCTION : TEST_backprop             joe

  PURPOSE  : main routine for the test of MLPs
  RETURNS  : kernel error code
  NOTES    : Output Parameters:  1 : error of the network (sum of all cycles)


  UPDATE   : 17.01.95
******************************************************************************/
krui_err TEST_backprop(int start_pattern, int end_pattern,
			float *parameterInArray, int NoOfInParams,
			float **parameterOutArray, int *NoOfOutParams)
{

    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error */
    int             pattern_no, sub_pat_no, no_of_layers;


    KernelErrorCode = KRERR_NO_ERROR;	/* reset return code  */

    if (NoOfInParams < 1) {	/* Not enough input parameters	 */
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }
    *NoOfOutParams = 1;		/* One return value is available (the
				   learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {
	/* Net has been modified or topologic array isn't initialized */
	/* check the topology of the network  */
	no_of_layers = kr_topoCheck();
	if (KernelErrorCode != KRERR_NO_ERROR)
	    /* an error has occured	 */
	    return (KernelErrorCode);

	if (no_of_layers < 2) {	/* the network has less then 2 layers  */
	    KernelErrorCode = KRERR_FEW_LAYERS;
	    return (KernelErrorCode);
	}
	/* count the no. of I/O units and check the patterns  */
	if (kr_IOCheck() != KRERR_NO_ERROR)
	    return (KernelErrorCode);

	/* sort units by topology and by topologic type  */
	(void) kr_topoSort(TOPOLOGICAL_FF);
	if ((KernelErrorCode != KRERR_NO_ERROR) &&
	    (KernelErrorCode != KRERR_DEAD_UNITS))
	    return (KernelErrorCode);

	NetModified = FALSE;
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */
    
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	propagateNetForward(pattern_no,sub_pat_no);   /* Forward propagation */

	NET_ERROR(OutParameter) +=
	    testNetBackward2(pattern_no,sub_pat_no,
				  LEARN_PARAM1(parameterInArray),
				  LEARN_PARAM2(parameterInArray));

    }

    return (KernelErrorCode);
}

/*****************************************************************************
  FUNCTION : testNetBackward2                        joe

  PURPOSE  : calculates network error for MLPs
  RETURNS  : network error
  NOTES    : network must be topologically sorted 

  UPDATE   : 19.02.95
******************************************************************************/
static float testNetBackward2(int pattern_no, int sub_pat_no, 
				   float learn_parameter, float delta_max)
{
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register float sum_error, devit;
    register TopoPtrArray topo_ptr;
    int size;

    sum_error = 0.0;		/* reset network error  */

    /* calculate address of the output pattern (with number pattern_no + 1)  */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    if(out_pat == NULL){
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return(-1);
    }
    out_pat += size;


    /* add 3 to no_of_topo_units because the topologic array contains 4 NULL
       pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	devit = *(--out_pat) - unit_ptr->Out.output; /* calc. devitation */
	if ((float) fabs(devit) <= delta_max)
	    continue;

	sum_error += devit * devit; /* sum up the error of the network  */

    }


    return (sum_error);		/* return the error of the network */
}



/*****************************************************************************
  FUNCTION : propagateNetBackwardBatch

  PURPOSE  : backward pass in batch mode for the backprop learning algorithm
  RETURNS  : network error
  NOTES    :

  UPDATE   : 05.11.1993 by Guenter Mamier
******************************************************************************/
float propagateNetBackwardBatch(int pattern_no, int sub_pat_no, float delta_max)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register float  error, sum_error, devit;
    register TopoPtrArray topo_ptr;
    int size;

    sum_error = 0.0;		/* reset network error  */

    /* calculate address of the output pattern (with number pattern_no+1)*/
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    if(out_pat == NULL){
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return(-1);
    }

    out_pat += size;

    /* add 3 to no_of_topo_units because the topologic array contains 4 NULL
       pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    /* no test for special units takes place because the final weight */
    /* change is performed  by updateWeights */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	devit = *(--out_pat) - unit_ptr->Out.output; /* calc. devitation */
	if ((float) fabs(devit) <= delta_max)
	    continue;

	sum_error += devit * devit; /* sum up the error of the network  */

	/* calc. error for output units	 */
	error = devit * ((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)); 
	/* calc. the error for adjusting weights and bias of the pred.
	   units  */
	/* adjust bias value  */
	unit_ptr->value_a += error;

	if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {
	    /* the unit has direkt links  */
	    FOR_ALL_LINKS(unit_ptr, link_ptr) {	/* adjust links and
						   calc. sum of errors
						   of pred. units  */
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		link_ptr->value_a += error * link_ptr->to->Out.output;
	    }
	} else {
	    /* the unit has sites  */
	    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {	
		/* adjust links and calc. sum of errors of pred. units */
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		link_ptr->value_a += error * link_ptr->to->Out.output;
	    }
	}
    }

    /* calculate hidden units only  */
    /* no test for special units takes place because the final weight */
    /* change is performed  by updateWeights */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	/* calc. the error of the (hidden) unit  */
	error = ((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) * 
	    unit_ptr->Aux.flint_no;
	/* calc. the error for adjusting weights and bias of the pred.
	   units  */
	/* adjust bias value  */
	unit_ptr->value_a += error;

	if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {
	    /* the unit has direkt links	 */
	    FOR_ALL_LINKS(unit_ptr, link_ptr) {	
		/* adjust links and calc sum of errors of the pred. units */
		if IS_HIDDEN_UNIT
		    (link_ptr->to)
			/* this link points to a hidden unit: sum up the
			   error's from previos units  */
			link_ptr->to->Aux.flint_no += 
			    link_ptr->weight * error;
		link_ptr->value_a += error * link_ptr->to->Out.output;
	    }
	} else {
	    /* the unit has sites  */
	    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {	
		/* adjust links and calc sum of errors of the pred. units */
		if IS_HIDDEN_UNIT
		    (link_ptr->to)
			/* this link points to a hidden unit: sum up the
			   error's from previos units  */
			link_ptr->to->Aux.flint_no
			    += link_ptr->weight * error;
		link_ptr->value_a += error * link_ptr->to->Out.output;
	    }
	}
    }

    return (sum_error);		/* return the error of the network */
}

/*****************************************************************************
  FUNCTION : propagateClassNetBackwardBatch

  PURPOSE  : backward pass in batch mode for the backprop learning algorithm
             only updates weights if units usr_flag info matches current
	     pattern class (if classes are given)
  RETURNS  : network error
  NOTES    :

  UPDATE   : 31.03.98 Michael Vogt
******************************************************************************/
float propagateClassNetBackwardBatch(int pattern_no, int sub_pat_no, float delta_max)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register float  error, sum_error, devit;
    register TopoPtrArray topo_ptr;
    int size;
    int pattern_class;
    unsigned long int class_flag;
    const int maxclasses = 8 * sizeof(unsigned long int);
    int adjust_this;
    sum_error = 0.0;		/* reset network error  */

    /* calculate address of the output pattern (with number pattern_no+1)*/
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    if(out_pat == NULL){
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return(-1);
    }
    pattern_class = kr_getSubPatClass(pattern_no,sub_pat_no);
    if (pattern_class >= maxclasses)
    {
	fprintf(stderr, 
		"propagateClassNetBackwardBatch: pattern class index %d out of range\n"
		"pattern is trained as usual\n", pattern_class);
	pattern_class = -1;
    }
    if (pattern_class >= 0)
	class_flag = ((unsigned long int) 1) << pattern_class;

#ifdef DEBUG
    printf("maxclasses: %d, pattern_class: %d, class_flag: %lx\n",
	  maxclasses, pattern_class, class_flag);
#endif

    out_pat += size;

    /* add 3 to no_of_topo_units because the topologic array contains 4 NULL
       pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    /* no test for special units takes place because the final weight */
    /* change is performed  by updateWeights */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	devit = *(--out_pat) - unit_ptr->Out.output; /* calc. devitation */
	if ((float) fabs(devit) <= delta_max)
	    continue;

	adjust_this= (pattern_class == -1 || unit_ptr->usr_flags & class_flag);
#ifdef DEBUG
	printf("%s%s ", 
	       adjust_this ? "+" : "-", 
	       unit_ptr->unit_name ? unit_ptr->unit_name : "");
#endif
	sum_error += devit * devit; /* sum up the error of the network  */

	/* calc. error for output units	 */
	error = devit * ((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) ;
	/* calc. the error for adjusting weights and bias of the pred.
	   units  */
	if (adjust_this)
	{
	    /* adjust bias value  */
	    unit_ptr->value_a += error;
	    unit_ptr->value_b += 1.0;

	    if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {
		/* the unit has direkt links  */
		FOR_ALL_LINKS(unit_ptr, link_ptr) {	/* adjust links and
							   calc. sum of errors
							   of pred. units  */
		    link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		    link_ptr->value_a += error * link_ptr->to->Out.output;
		    link_ptr->value_b += 1.0;
		}
	    } else {
		/* the unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {	
		    /* adjust links and calc. sum of errors of pred. units */
		    link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		    link_ptr->value_a += error * link_ptr->to->Out.output;
		    link_ptr->value_b += 1.0;
		}
	    }
	}
    }

    /* calculate hidden units only  */
    /* no test for special units takes place because the final weight */
    /* change is performed  by updateWeights */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	adjust_this= (pattern_class == -1 || unit_ptr->usr_flags & class_flag);
#ifdef DEBUG
	printf("%s%s ", 
	       adjust_this ? "+" : "-", 
	       unit_ptr->unit_name ? unit_ptr->unit_name : "");
#endif
	/* calc. the error of the (hidden) unit  */
	error = ((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr))  
		* unit_ptr->Aux.flint_no;
	/* calc. the error for adjusting weights and bias of the pred. units */
	if (adjust_this)
	{
	    /* adjust bias value  */
	    unit_ptr->value_a += error;
	    unit_ptr->value_b += 1.0;

	    if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {
		/* the unit has direkt links	 */
		FOR_ALL_LINKS(unit_ptr, link_ptr) {	
		    /* adjust links and calc sum of errors of the pred. units */
		    if (IS_HIDDEN_UNIT(link_ptr->to))
			/* this link points to a hidden unit: sum up the
			   error's from previos units  */
			link_ptr->to->Aux.flint_no += link_ptr->weight * error;

		    link_ptr->value_a += error * link_ptr->to->Out.output;
		    link_ptr->value_b += 1.0;
		}
	    } else {
		/* the unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {	
		    /* adjust links and calc sum of errors of the pred. units */
		    if (IS_HIDDEN_UNIT(link_ptr->to))
			/* this link points to a hidden unit: sum up the
			   error's from previos units  */
			link_ptr->to->Aux.flint_no += link_ptr->weight * error;

		    link_ptr->value_a += error * link_ptr->to->Out.output;
		    link_ptr->value_b += 1.0;
		}
	    }
	}
    }

#ifdef DEBUG
    printf("\n");
#endif
    return (sum_error);		/* return the error of the network */
}


/*****************************************************************************
  FUNCTION : clearDeltas

  PURPOSE  : clears delta values for a new run of backprop batch
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 05.11.1993 by Guenter Mamier
******************************************************************************/
krui_err clearDeltas(void)
{
    register FlagWord flags;
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register FlintType fastnull = 0.0;

    FOR_ALL_UNITS(unit_ptr) {
	flags = unit_ptr->flags;

	if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {	/* unit is in use  */
	    unit_ptr->value_a = fastnull;

	    if (flags & UFLAG_SITES) {	/* unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
		    link_ptr->value_a = fastnull;
	    } else {		/* unit has no sites   */
		if (flags & UFLAG_DLINKS) { /* unit has direct links   */
		    FOR_ALL_LINKS(unit_ptr, link_ptr)
			link_ptr->value_a = fastnull;
		}
	    }
	}
    }

    return (KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : clearAllDeltas

  PURPOSE  : clears all delta values for a new run of special batch schemes
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 31.03.98 by Michael Vogt
******************************************************************************/
krui_err clearAllDeltas(void)
{
    register FlagWord flags;
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register FlintType fastnull = 0.0;

    FOR_ALL_UNITS(unit_ptr) {
	flags = unit_ptr->flags;

	if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {	/* unit is in use  */
	    unit_ptr->value_a = fastnull;
	    unit_ptr->value_b = fastnull;
	    unit_ptr->value_c = fastnull;

	    if (flags & UFLAG_SITES) {	/* unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
		    {
			link_ptr->value_a = fastnull;
			link_ptr->value_b = fastnull;
			link_ptr->value_c = fastnull;
		    }
	    } else if (flags & UFLAG_DLINKS) {
		/* unit has direct links   */
		FOR_ALL_LINKS(unit_ptr, link_ptr)
		    {
			link_ptr->value_a = fastnull;
			link_ptr->value_b = fastnull;
			link_ptr->value_c = fastnull;
		    }
	    }
	}
    }

    return (KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : updateWeights

  PURPOSE  : Update the weights after all patterns have been presented by 
             backpropBatch 
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 05.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err updateWeights(float eta)
{
    register FlagWord flags;
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;

    FOR_ALL_UNITS(unit_ptr) {
	if (!IS_SPECIAL_UNIT(unit_ptr)) {
	    flags = unit_ptr->flags;

	    if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {
		/* unit is in use  */
		unit_ptr->bias += unit_ptr->value_a * eta;

		if (flags & UFLAG_SITES) {
		    /* unit has sites  */
		    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
			link_ptr->weight += link_ptr->value_a * eta;
		} else {
		    /* unit has no sites   */
		    if (flags & UFLAG_DLINKS) {	/* unit has direct links    */
			FOR_ALL_LINKS(unit_ptr, link_ptr)
			    link_ptr->weight += link_ptr->value_a * eta;
		    }
		}
	    }
	}
    }
    return (KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : updateNormalizedWeights

  PURPOSE  : Update the weights after all patterns have been presented by 
             a spezialized batch learning scheme, which places the number
	     of weight changes in the value_b fields.
  RETURNS  : kernel error code
  NOTES    : This function should not be called with a normalized learning rate

  UPDATE   : 31.03.98 Michael Vogt
******************************************************************************/
static krui_err updateNormalizedWeights(float eta)
{
    register FlagWord flags;
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register FlintType fastnull = 0.0;

    FOR_ALL_UNITS(unit_ptr) {
	if (!IS_SPECIAL_UNIT(unit_ptr)) {
	    flags = unit_ptr->flags;

	    if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {
		/* unit is in use */
		if (unit_ptr->value_b > fastnull)
		    unit_ptr->bias += 
			unit_ptr->value_a * eta/unit_ptr->value_b;
		if (flags & UFLAG_SITES) {
		    /* unit has sites */
		    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
			{
			    if (link_ptr->value_b > fastnull)
				link_ptr->weight += 
				    link_ptr->value_a * eta/link_ptr->value_b;
			}
		} else if (flags & UFLAG_DLINKS) {
		    /* unit has direct links */
		    FOR_ALL_LINKS(unit_ptr, link_ptr)
			{
			    if (link_ptr->value_b > fastnull)
				link_ptr->weight += 
				    link_ptr->value_a * eta/link_ptr->value_b;
			}
		}
	    }
	}
    }
    return (KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : LEARN_backpropBatch

  PURPOSE  : main routine for the batch version of the backpropagation 
             algorithm
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : delta max

             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 05.11.1993 by Guenter Mamier
******************************************************************************/
krui_err  LEARN_backpropBatch(int start_pattern, int end_pattern, 
			      float *parameterInArray, int NoOfInParams, 
			      float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             pattern_no, sub_pat_no, no_of_layers;
    int             pattern_count;

    KernelErrorCode = KRERR_NO_ERROR;	/* reset return code  */

    /* ####  have to be changed (must be 2)  #### */
    if (NoOfInParams < 1) {	/* Not enough input parameters	 */
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }
    *NoOfOutParams = 1;	  /* One return value is available ( learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {	
	/* Net has been modified or topologic array isn't initialized */
	/* check the topology of the network  */
	no_of_layers = kr_topoCheck();
	if (KernelErrorCode != KRERR_NO_ERROR)
	    /* an error has occured	 */
	    return (KernelErrorCode);

	if (no_of_layers < 2) {	/* the network has less then 2 layers  */
	    KernelErrorCode = KRERR_FEW_LAYERS;
	    return (KernelErrorCode);
	}
	/* count the no. of I/O units and check the patterns  */
	if (kr_IOCheck() != KRERR_NO_ERROR)
	    return (KernelErrorCode);

	/* sort units by topology and by topologic type  */
	(void) kr_topoSort(TOPOLOGICAL_FF);
	if ((KernelErrorCode != KRERR_NO_ERROR) && 
	    (KernelErrorCode != KRERR_DEAD_UNITS))
	    return (KernelErrorCode);

	NetModified = FALSE;
    }
    clearDeltas();

    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */
    pattern_count = 0;
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){
	pattern_count++;
	propagateNetForward(pattern_no,sub_pat_no);  /* Forward propagation */

	/* Backward propagation  */
	/* 1st parameter is the learning parameter 2nd parameter is the max.
	   devitation between output pattern and the output of the output
	   unit (delta max) */
	NET_ERROR(OutParameter) +=
	    propagateNetBackwardBatch(pattern_no,sub_pat_no,
				      LEARN_PARAM2(parameterInArray));
    }

    if (pattern_count > 0)
	updateWeights(LEARN_PARAM1(parameterInArray)/pattern_count);


    return (KernelErrorCode);
}


/*****************************************************************************
 *****************************************************************************

  GROUP        : backpropagation learning algorithm with momentum term

  AUTHOR       : Niels Mache
             
******************************************************************************
******************************************************************************/


/*****************************************************************************
  FUNCTION : initializeBackpropMomentum

  PURPOSE  : backprop-momentum initialisation
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 05.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err initializeBackpropMomentum(void)
{
    register FlagWord flags;
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;


    FOR_ALL_UNITS(unit_ptr) {
	flags = unit_ptr->flags;

	if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {	/* unit is in use  */
	    unit_ptr->value_a = (FlintType) 0;

	    if (flags & UFLAG_SITES) {	/* unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
		    link_ptr->value_b = (FlintType) 0;
	    } else {		/* unit has no sites   */
		if (flags & UFLAG_DLINKS) {	/* unit has direct links   */
		    FOR_ALL_LINKS(unit_ptr, link_ptr)
			link_ptr->value_b = (FlintType) 0;
		}
	    }
	}
    }

    return (KRERR_NO_ERROR);
}




/*****************************************************************************
  FUNCTION : Backprop_momentum_FSE
  PURPOSE  : Backward error propagation (topological) of backpropagation 
             learnig function with momentum term and flat spot elimination

  RETURNS  : network error
  NOTES    :

  UPDATE   : 07.02.1994 by Sven Doering
******************************************************************************/
static float Backprop_momentum_FSE(int pattern_no, int sub_pat_no, 
				   float learn_parameter, 
				   float mu, float FSE_term, float delta_max)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register float  error, sum_error, eta, devit, learn_error, mu_help;
    register TopoPtrArray topo_ptr;
    int size;


    sum_error = 0.0;		/* reset network error  */
    eta = learn_parameter;	/* store learn_parameter in CPU register  */

    /* calculate address of the output pattern (with number pattern_no + 1)  */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    if(out_pat == NULL){
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return(-1);
    }
    out_pat += size;


    /* add 3 to no_of_topo_units because the topologic array contains 4 NULL
       pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	devit = *(--out_pat) - unit_ptr->Out.output;	/* calc. devitation */
	if ((float) fabs(devit) <= delta_max)
	    continue;

	sum_error += devit * devit;	/* sum up the error of the network  */
	/* calc. error for output units	 */
	error = devit * (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr))  + FSE_term);

	/* calc. the error for adjusting weights and bias of the predecessor
	   units  */
	mu_help = mu;
	learn_error = eta * error;
	if(IS_SPECIAL_UNIT( unit_ptr )){
	    learn_error = 0.0;
	    mu = 0.0;
	}
	unit_ptr->value_a = learn_error + mu * unit_ptr->value_a;
	/* adjust bias value  */
	unit_ptr->bias += unit_ptr->value_a;

	if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {	/* the unit has direkt links  */
	    FOR_ALL_LINKS(unit_ptr, link_ptr) {	/* adjust link weights and
						   calc. sum of errors of the
						   predecessor units  */
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		link_ptr->value_b = learn_error * link_ptr->to->Out.output + 
		                    mu * link_ptr->value_b;
		link_ptr->weight += link_ptr->value_b;
	    }
	} else {		/* the unit has sites  */
	    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {	
		/* adjust links and calc. sum of errors of the pred. units */
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		link_ptr->value_b = learn_error * link_ptr->to->Out.output + 
		                    mu * link_ptr->value_b;
		link_ptr->weight += link_ptr->value_b;
	    }
	}
	mu = mu_help;
    }


    /* calculate hidden units only  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	/* calc. the error of the (hidden) unit  */
	error = unit_ptr->Aux.flint_no * 
	        (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr))  + FSE_term);

	/* calc. the error for adjusting weights and bias of the pred. units */
	mu_help = mu;
	learn_error = eta * error;
	if(IS_SPECIAL_UNIT( unit_ptr )){
	    learn_error = 0.0;
	    mu = 0.0;
	}
	unit_ptr->value_a = learn_error + mu * unit_ptr->value_a;
	/* adjust bias value  */
	unit_ptr->bias += unit_ptr->value_a;

	if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {	/* the unit has direkt links */
	    FOR_ALL_LINKS(unit_ptr, link_ptr) {	/* adjust link weights and
						   calc. sum of errors of the
						   predecessor units  */
		if IS_HIDDEN_UNIT
		    (link_ptr->to)
		    /* this link points to a hidden unit: sum up the error's
		       from previos units  */
			link_ptr->to->Aux.flint_no += link_ptr->weight * error;

		link_ptr->value_b = learn_error * link_ptr->to->Out.output + 
		                    mu * link_ptr->value_b;
		link_ptr->weight += link_ptr->value_b;
	    }
	} else {		/* the unit has sites  */
	    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {	
		/* adjust links and calc. sum of errors of the pred. units */
		if IS_HIDDEN_UNIT
		    (link_ptr->to)
		    /* this link points to a hidden unit: sum up the error's
		       from previos units  */
			link_ptr->to->Aux.flint_no += link_ptr->weight * error;

		link_ptr->value_b = learn_error * link_ptr->to->Out.output + 
		                    mu * link_ptr->value_b;
		link_ptr->weight += link_ptr->value_b;
	    }
	}
	mu = mu_help;
    }

    return (sum_error);		/* return the error of the network */
}


/*****************************************************************************
  FUNCTION : LEARN_backpropMomentum

  PURPOSE  : main routine for backpropagation with momentum
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : momentum term
				 3 : flat-spot-elimination value
				 4 : delta max

             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 05.11.1993 by Guenter Mamier
******************************************************************************/
krui_err LEARN_backpropMomentum(int start_pattern, int end_pattern, 
				float *parameterInArray, int NoOfInParams, 
				float **parameterOutArray, int *NoOfOutParams)
{
    static float OutParameter[1];  /*OutParameter[0] stores the learning error*/
    int          ret_code, pattern_no, sub_pat_no;


    if (NoOfInParams < 1)	/* ####  have to be changed (must be 2)  #### */
	return (KRERR_PARAMETERS);	/* Not enough input parameters  */

    *NoOfOutParams = 1;		/* One return value is available (the
				   learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {	
	/* Net has been modified or topologic array isn't initialized */
	/* check the topology of the network  */
	ret_code = kr_topoCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);	/* an error has occured  */
	if (ret_code < 2)
	    return (KRERR_FEW_LAYERS);	/* the network has less then 2 layers */

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
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified or
						   initialized, initialize
						   backprop now  */
	ret_code = initializeBackpropMomentum();
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	propagateNetForward(pattern_no,sub_pat_no);	/* Forward propagation */

	/* Backward propagation  */
	/* 1st parameter is the learning parameter 2nd parameter is the
	   momentum term 3rd parameter is the flat-spot-elimination value 4th
	   parameter is the max. devitation between output pattern and the
	   output of the output unit (delta max) */
	NET_ERROR(OutParameter) +=
	    Backprop_momentum_FSE(pattern_no,sub_pat_no,
				  LEARN_PARAM1(parameterInArray),
				  LEARN_PARAM2(parameterInArray),
				  LEARN_PARAM3(parameterInArray),
				  LEARN_PARAM4(parameterInArray));
    }

    return (ret_code);
}

/*****************************************************************************
  FUNCTION : TEST_backpropMomentum              joe

  PURPOSE  : main routine for testing backpropagation with momentum
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : momentum term
				 3 : flat-spot-elimination value
				 4 : delta max

             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 20.01.95 by Joachim Danz
******************************************************************************/
krui_err TEST_backpropMomentum(int start_pattern, int end_pattern, 
				float *parameterInArray, int NoOfInParams, 
				float **parameterOutArray, int *NoOfOutParams)
{
    static float OutParameter[1];  /*OutParameter[0] stores the learning error*/
    int          ret_code, pattern_no, sub_pat_no;


    if (NoOfInParams < 1)	/* ####  have to be changed (must be 2)  #### */
	return (KRERR_PARAMETERS);	/* Not enough input parameters  */

    *NoOfOutParams = 1;		/* One return value is available (the
				   learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {	
	/* Net has been modified or topologic array isn't initialized */
	/* check the topology of the network  */
	ret_code = kr_topoCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);	/* an error has occured  */
	if (ret_code < 2)
	    return (KRERR_FEW_LAYERS);	/* the network has less then 2 layers */

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


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	propagateNetForward(pattern_no,sub_pat_no);	/* Forward propagation */

	/* Backward propagation  */
	/* 1st parameter is the learning parameter 2nd parameter is the
	   momentum term 3rd parameter is the flat-spot-elimination value 4th
	   parameter is the max. devitation between output pattern and the
	   output of the output unit (delta max) */
	NET_ERROR(OutParameter) +=
	    testNetBackward2(pattern_no,sub_pat_no,
				  LEARN_PARAM1(parameterInArray),
				   LEARN_PARAM4(parameterInArray));
    }

    return (ret_code);
}



/*****************************************************************************
 *****************************************************************************

  GROUP        : backpropagation learning algorithm with weight decay

  AUTHOR       : Tobias Schreiner
             
******************************************************************************
******************************************************************************/


/*****************************************************************************
  FUNCTION : Backprop_weightdecay
  PURPOSE  : Backward error propagation (topological) of backpropagation 
             learnig function with weigth decay

  RETURNS  : network error
  NOTES    :

  UPDATE   :
******************************************************************************/
static float Backprop_weightdecay (int pattern_no, int sub_pat_no, 
				   float learn_parameter, 
				   float wd_gamma, float min_weight,
				   float delta_max)

{

    register Patterns out_pat;
    register float error, sum_error, eta, devit, learn_error;
    register TopoPtrArray topo_ptr;
    int size;

    /* reset network error  */
    sum_error = 0.0;

    /* store learn_parameter in CPU register  */
    eta = learn_parameter;

    /* calculate address of the output pattern (with number pattern_no + 1)  */
/*    out_pat = out_patterns + (pattern_no + 1) * NoOfOutputPatterns;*/

    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    if(out_pat == NULL)
    {
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return(-1);
    }
    out_pat += size;

    /* add 3 to no_of_topo_units because the topologic array contains */
    /* 4 NULL pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    while ((unitPtr = *--topo_ptr) != NULL)
    {
	/* update unitNo */
	unitNo = unitPtr - unit_array;

	/* calculate devitation */
	devit = *(--out_pat) - unitPtr->Out.output;
	if ((float) fabs (devit) <= delta_max)
	    continue;

	/* sum up the error of the network  */
	sum_error += devit * devit;

	/* calculate error for output units	 */
	error = devit * ((unitPtr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unitPtr->python_act_deriv_func,
						unitPtr) :
			(unitPtr->act_deriv_func) (unitPtr)) ;

	/* calculate the error for adjusting weights and bias of the */
	/* predecessor units  */
	if (IS_SPECIAL_UNIT(unitPtr))
	    learn_error = 0.0;
	else
	    learn_error = eta * error;

	/* adjust bias value  */
	unitPtr->bias += learn_error - wd_gamma * unitPtr->bias;

	/* initialize prevLinkPtr */
	prevLinkPtr = NULL;

	if UNIT_HAS_DIRECT_INPUTS (unitPtr)
	    /* the unit has direkt links  */
	    for (linkPtr = (struct Link *) unitPtr->sites;
		 linkPtr != NULL;
		 linkPtr = linkPtr ? linkPtr->next : linkPtr)
	    {
		/* calculate errors of the predecessor units  */
		linkPtr->to->Aux.flint_no += linkPtr->weight * error;

		/* adjust the link weights */
		linkPtr->weight += 
		    learn_error * linkPtr->to->Out.output
			- wd_gamma * linkPtr->weight;

		/* prune link if smaller than minimum */
		if (fabs (linkPtr->weight) < min_weight)
		    kr_deleteLink ();

		/* adjust prevLinkPtr */
		prevLinkPtr = linkPtr;
	    }
	else 
	    /* the unit has sites  */
	    for (sitePtr = unitPtr->sites;
		 sitePtr != NULL;
		 sitePtr = sitePtr ? sitePtr->next : sitePtr)
		for (linkPtr = sitePtr->links;
		     linkPtr != NULL;
		     linkPtr = linkPtr ? linkPtr->next : linkPtr)
		{	
		    /* calculate errors of the predecessor units  */
		    linkPtr->to->Aux.flint_no += linkPtr->weight * error;
		    
		    /* adjust the link weights */
		    linkPtr->weight += 
			learn_error * linkPtr->to->Out.output
			    - wd_gamma * linkPtr->weight;
		    
		    /* prune link if smaller than minimum */
		    if (fabs (linkPtr->weight) < min_weight)
			kr_deleteLink ();
		    
		    /* adjust prevLinkPtr */
		    prevLinkPtr = linkPtr;
		}
    }

    /* calculate hidden units only  */
    while ((unitPtr = *--topo_ptr) != NULL)
    {
	/* update unitNo */
	unitNo = unitPtr - unit_array;

	/* calculate the error of the (hidden) unit  */
	error = ((unitPtr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unitPtr->python_act_deriv_func,
						unitPtr) :
			(unitPtr->act_deriv_func) (unitPtr))  * 
	    unitPtr->Aux.flint_no;

	/* calculate the error for adjusting weights and bias of the */
	/* predecessor units  */
	if (IS_SPECIAL_UNIT(unitPtr)) 
	    learn_error = 0.0;
	else
	    learn_error = eta * error;

	/* adjust bias value  */
	unitPtr->bias += learn_error - wd_gamma * unitPtr->bias;

	/* initialize prevLinkPtr */
	prevLinkPtr = NULL;

	if UNIT_HAS_DIRECT_INPUTS (unitPtr)
	    /* the unit has direkt links  */
	    for (linkPtr = (struct Link *) unitPtr->sites;
		 linkPtr != NULL;
		 linkPtr = linkPtr ? linkPtr->next : linkPtr)
	    {
		/* calculate errors of the predecessor units  */
		if IS_HIDDEN_UNIT (linkPtr->to)
		    /* this link points to a hidden unit: sum up the */
		    /* error's from previos units  */
		    linkPtr->to->Aux.flint_no += 
			linkPtr->weight * error;

		/* adjust link weights */
		linkPtr->weight +=
		    learn_error * linkPtr->to->Out.output
			- wd_gamma * linkPtr->weight;

		/* prune link if smaller than minimum */
		if (fabs (linkPtr->weight) < min_weight)
		    kr_deleteLink ();

		/* adjust prevLinkPtr */
		prevLinkPtr = linkPtr;
	    }
	else
	    /* the unit has sites  */
	    for (sitePtr = unitPtr->sites;
		 sitePtr != NULL;
		 sitePtr = sitePtr ? sitePtr->next : sitePtr)
		for (linkPtr = sitePtr->links;
		     linkPtr != NULL;
		     linkPtr = linkPtr ? linkPtr->next : linkPtr)
		{	
		    /* calculate errors of the predecessor units  */
		    if IS_HIDDEN_UNIT (linkPtr->to)
			/* this link points to a hidden unit: sum up */
			/* the error's from previos units  */
			linkPtr->to->Aux.flint_no += 
			    linkPtr->weight * error;

		    /* adjust links */
		    linkPtr->weight +=
			learn_error * linkPtr->to->Out.output
			    - wd_gamma * linkPtr->weight;
		    
		    /* prune link if smaller than minimum */
		    if (fabs (linkPtr->weight) < min_weight)
			kr_deleteLink ();
		    
		    /* adjust prevLinkPtr */
		    prevLinkPtr = linkPtr;
		}
    }

    return (sum_error);		/* return the error of the network */
}


/*****************************************************************************
  FUNCTION : LEARN_backpropWeightDecay

  PURPOSE  : main routine for backpropagation with weight decay
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : parameter for weight decay
				 3 : minimum weight (smaller weights
				     will be pruned) 
				 4 : delta max

             Output Parameters:  1 : error of the network (sum of all cycles)

	     special flags are reset!

  UPDATE   :
******************************************************************************/
krui_err LEARN_backpropWeightDecay (int start_pattern, int end_pattern, 
		float *parameterInArray, int NoOfInParams, 
		float **parameterOutArray, int *NoOfOutParams)

{

    static float OutParameter [1];  
    /* OutParameter [0] stores the learning error */ 
    int pattern_no, sub_pat_no, no_of_layers;

    /* reset return code  */
    KernelErrorCode = KRERR_NO_ERROR;

    /* assure four input parameters */
    if (NoOfInParams < 4)
    {
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }

    /* learning error is the only output parameter */
    *NoOfOutParams = 1;
    *parameterOutArray = OutParameter;

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) 
	/* Net has been modified or topologic array isn't initialized */
    {	
	/* check the topology of the network  */
	no_of_layers = kr_topoCheck ();
	if (KernelErrorCode != KRERR_NO_ERROR)
	    return (KernelErrorCode);	/* an error has occured  */
	if (no_of_layers < 2)
	    /* the network has less then 2 layers */
	{
	    KernelErrorCode = KRERR_FEW_LAYERS;
	    return (KernelErrorCode);
	}

	/* count the no. of I/O units and check the patterns  */
	if (kr_IOCheck () != KRERR_NO_ERROR)
	    return (KernelErrorCode);

	/* sort units by topology and by topologic type  */
	kr_topoSort (TOPOLOGICAL_FF);
	if ((KernelErrorCode != KRERR_NO_ERROR) && 
	    (KernelErrorCode != KRERR_DEAD_UNITS))
	    return (KernelErrorCode);

	NetModified = FALSE;
    }

    /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(start_pattern, end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    /* reset network error value  */
    NET_ERROR (OutParameter) = 0.0;

    while (kr_getSubPatternByOrder (&pattern_no, &sub_pat_no))
    {
	/* Forward propagation */
	propagateNetForward(pattern_no,sub_pat_no);

	/* Backward propagation with parameters according to header */
	NET_ERROR(OutParameter) +=
	    Backprop_weightdecay (pattern_no, sub_pat_no,
				  LEARN_PARAM1 (parameterInArray),
				  LEARN_PARAM2 (parameterInArray),
				  LEARN_PARAM3 (parameterInArray),
				  LEARN_PARAM4 (parameterInArray));
    }

    pr_checkDeadUnits();

    return (KernelErrorCode);
}


/*****************************************************************************
  FUNCTION : LEARN_backpropChunk

  PURPOSE  : main routine for the chunk-update version of the backpropagation 
             algorithm
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : delta max
                                 3 : chunk size

             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 
******************************************************************************/
krui_err  LEARN_backpropChunk(int start_pattern, int end_pattern, 
			      float *parameterInArray, int NoOfInParams, 
			      float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             pattern_no, sub_pat_no, no_of_layers;
    int             i = 0;
    

    KernelErrorCode = KRERR_NO_ERROR;	/* reset return code  */

    if (NoOfInParams < 3) {	/* Not enough input parameters	 */
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }

    *NoOfOutParams = 1;	  /* One return value is available ( learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */
    OutParameter[0] = 0.0;

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {	
	/* Net has been modified or topologic array isn't initialized */
	/* check the topology of the network  */
	no_of_layers = kr_topoCheck();
	if (KernelErrorCode != KRERR_NO_ERROR)
	    /* an error has occured	 */
	    return (KernelErrorCode);

	if (no_of_layers < 2) {	/* the network has less then 2 layers  */
	    KernelErrorCode = KRERR_FEW_LAYERS;
	    return (KernelErrorCode);
	}
	/* count the no. of I/O units and check the patterns  */
	if (kr_IOCheck() != KRERR_NO_ERROR)
	    return (KernelErrorCode);

	/* sort units by topology and by topologic type  */
	(void) kr_topoSort(TOPOLOGICAL_FF);
	if ((KernelErrorCode != KRERR_NO_ERROR) && 
	    (KernelErrorCode != KRERR_DEAD_UNITS))
	    return (KernelErrorCode);

	NetModified = FALSE;
    }

    NET_ERROR(OutParameter) = 0.0; /* init error variable */

    /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    /* prepare for beginning of one chunk */
    clearDeltas();

    i = 0;
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

        propagateNetForward(pattern_no,sub_pat_no);  
        NET_ERROR(OutParameter) +=
            propagateNetBackwardBatch(pattern_no,sub_pat_no,
                                      LEARN_PARAM2(parameterInArray));
	
	if( ++i >= (int)(LEARN_PARAM3(parameterInArray))){
	    updateWeights(LEARN_PARAM1(parameterInArray)/i);
	    clearDeltas();
	    i = 0;
	}
    }

    /* update the network weights */
    if (i>0)
    {
	updateWeights(LEARN_PARAM1(parameterInArray)/i);
    }

    return (KernelErrorCode);
    
}

/*****************************************************************************
  FUNCTION : LEARN_backpropJogChunk

  PURPOSE  : main routine for the chunk-update version of the backpropagation 
             algorithm with embedded weights jogging
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : delta max
                                 3 : chunk size
                                 4 : lower jog value
                                 5 : upper jog value

             Output Parameters:  1 : error of the network (sum of all cycles)

	     To be used within batchman scripts that apply automatically 
	     decrease of absolute jog values.

  UPDATE   : 
******************************************************************************/
krui_err  LEARN_backpropJogChunk(int start_pattern, int end_pattern, 
				 float *parameterInArray, int NoOfInParams, 
				 float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             pattern_no, sub_pat_no, no_of_layers;
    int             i = 0;
    

    KernelErrorCode = KRERR_NO_ERROR;	/* reset return code  */

    if (NoOfInParams < 3) {	/* Not enough input parameters	 */
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }

    *NoOfOutParams = 1;	  /* One return value is available ( learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */
    OutParameter[0] = 0.0;

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {	
	/* Net has been modified or topologic array isn't initialized */
	/* check the topology of the network  */
	no_of_layers = kr_topoCheck();
	if (KernelErrorCode != KRERR_NO_ERROR)
	    /* an error has occured	 */
	    return (KernelErrorCode);

	if (no_of_layers < 2) {	/* the network has less then 2 layers  */
	    KernelErrorCode = KRERR_FEW_LAYERS;
	    return (KernelErrorCode);
	}
	/* count the no. of I/O units and check the patterns  */
	if (kr_IOCheck() != KRERR_NO_ERROR)
	    return (KernelErrorCode);

	/* sort units by topology and by topologic type  */
	(void) kr_topoSort(TOPOLOGICAL_FF);
	if ((KernelErrorCode != KRERR_NO_ERROR) && 
	    (KernelErrorCode != KRERR_DEAD_UNITS))
	    return (KernelErrorCode);

	NetModified = FALSE;
    }

    NET_ERROR(OutParameter) = 0.0; /* init error variable */

    /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    /* prepare for beginning of one chunk */
    clearDeltas();

    i = 0;
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	if (i==0 && 
	    (LEARN_PARAM4(parameterInArray) != 0.0 || 
	     LEARN_PARAM5(parameterInArray) != 0.0))
	    kr_jogWeights(LEARN_PARAM4(parameterInArray), 
			  LEARN_PARAM5(parameterInArray));

        propagateNetForward(pattern_no,sub_pat_no);  
        NET_ERROR(OutParameter) +=
            propagateNetBackwardBatch(pattern_no,sub_pat_no,
                                      LEARN_PARAM2(parameterInArray));
	
	if( ++i == (int)(LEARN_PARAM3(parameterInArray))){
	    updateWeights(LEARN_PARAM1(parameterInArray)/i);
	    clearDeltas();
	    i = 0;
	}
    }

    /* update the network weights */
    if (i>0)
    {
	updateWeights(LEARN_PARAM1(parameterInArray)/i);
    }

    return (KernelErrorCode);
    
}

/*****************************************************************************
  FUNCTION : LEARN_backpropClassJogChunk

  PURPOSE  : main routine for the chunk-update version of the backpropagation 
             algorithm with embedded weights jogging
	     only neurons with matching class information are trained:

	     with x in {0, 1, 2, 3, ...}
	     if unit name = "class+x[+x]*"
	         only train, if current pattern class index is one of the given
                 x values
	     else if unit name = "class-x[-x]*"
	         only train, if current pattern class index is different from
                 all of the given x values
	     else (for all other unit names and if no class information at all)
                 train as usual

  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : delta max
                                 3 : chunk size
                                 4 : lower jog value
                                 5 : upper jog value

             Output Parameters:  1 : error of the network (sum of all cycles)

	     To be used within batchman scripts that apply automatically 
	     decrease of absolute jog values.

  UPDATE   : 
******************************************************************************/
krui_err  LEARN_backpropClassJogChunk(int start_pattern, int end_pattern, 
				 float *parameterInArray, int NoOfInParams, 
				 float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             pattern_no, sub_pat_no, no_of_layers;
    int             i = 0;
    unsigned long int class_flags;
    unsigned long int class_add_flags;
    char            delimiter;
    char            *class_p;
    char            class_str[9];
    int             class_num;
    const int       maxclasses = 8 * sizeof(unsigned long int);
    struct Unit     *unit_ptr;

    KernelErrorCode = KRERR_NO_ERROR;	/* reset return code  */

    if (NoOfInParams < 3) {	/* Not enough input parameters	 */
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }

    *NoOfOutParams = 1;	  /* One return value is available ( learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */
    OutParameter[0] = 0.0;

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {	
	/* Net has been modified or topologic array isn't initialized */
	/* check the topology of the network  */
	no_of_layers = kr_topoCheck();
	if (KernelErrorCode != KRERR_NO_ERROR)
	    /* an error has occured	 */
	    return (KernelErrorCode);

	if (no_of_layers < 2) {	/* the network has less then 2 layers  */
	    KernelErrorCode = KRERR_FEW_LAYERS;
	    return (KernelErrorCode);
	}
	/* count the no. of I/O units and check the patterns  */
	if (kr_IOCheck() != KRERR_NO_ERROR)
	    return (KernelErrorCode);

	/* sort units by topology and by topologic type  */
	(void) kr_topoSort(TOPOLOGICAL_FF);
	if ((KernelErrorCode != KRERR_NO_ERROR) && 
	    (KernelErrorCode != KRERR_DEAD_UNITS))
	    return (KernelErrorCode);

	/* set the usr_flags field of all units according to the class
	   information given in the unit name:
	   A bit set at position 2^n means to train this unit for class n */
	FOR_ALL_UNITS(unit_ptr) {
	    if (unit_ptr->unit_name &&
		(strncmp(unit_ptr->unit_name, "class+", 6) == 0 ||
		 strncmp(unit_ptr->unit_name, "class-", 6) == 0)
		)
	    {
		delimiter = unit_ptr->unit_name[5];
		if (delimiter == '+')
		    class_flags = ((unsigned long int) 0); /* 00000000 */
		else /* delimiter == '-' */
		    class_flags = ~((unsigned long int) 0); /* 11111111 */
		
		class_p = &(unit_ptr->unit_name[5]);
		while (*class_p != '\0')
		{
		    class_p++;
		    class_str[0] = '\0';
		    strncpy(class_str, class_p, 8);
		    i = 0;
		    while (i < 8 && isdigit(class_str[i]))
			i++;
		    if (i<8)
			class_str[i] = '\0';
		    class_num = atoi(class_str);
		    if (class_num >= maxclasses)
		    {
			fprintf(stderr, 
				"LEARN_backpropClassJogChunk: "
				"units class information not handled\n"
				"%d is >= %d (maxclasses)\n",
				class_num, maxclasses);
			class_add_flags = ((unsigned long int) 0);
		    }
		    else
			class_add_flags = 
			    ((unsigned long int) 1) << class_num;
		    if (delimiter == '+')
			class_flags |= class_add_flags;
		    else
			class_flags ^= class_add_flags;
		    while (*class_p != '\0' && *class_p != delimiter)
			class_p++;
		}
		unit_ptr->usr_flags = class_flags;
	    }
	    else
		unit_ptr->usr_flags = ~((unsigned long int) 0); /* 11111111 */
#ifdef DEBUG
	    printf("%s: %lx  ", 
		  unit_ptr->unit_name ? unit_ptr->unit_name : "",
		  unit_ptr->usr_flags);
#endif
	}
#ifdef DEBUG
	printf("\n");
#endif
	NetModified = FALSE;
    }

    NET_ERROR(OutParameter) = 0.0; /* init error variable */

    /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    /* prepare for beginning of one chunk */
    clearAllDeltas();

    i = 0;
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	if (i==0 && 
	    (LEARN_PARAM4(parameterInArray) != 0.0 || 
	     LEARN_PARAM5(parameterInArray) != 0.0))
	{
#ifdef HAVE_LRAND48
	    unsigned short lastseed[3];
	    unsigned short *seedptr;

	    /* read out current seed information and also use it */
	    seedptr = seed48(lastseed);
	    lastseed[0] = seedptr[0];
	    lastseed[1] = seedptr[1];
	    lastseed[2] = seedptr[2];
	    seed48(lastseed);
#endif
	    kr_jogWeights(LEARN_PARAM4(parameterInArray), 
			  LEARN_PARAM5(parameterInArray));
#ifdef HAVE_LRAND48
	    /* reset to previous random seed */
	    seed48(lastseed);
#endif
	}

        propagateNetForward(pattern_no,sub_pat_no);  
        NET_ERROR(OutParameter) +=
            propagateClassNetBackwardBatch(pattern_no,sub_pat_no,
					   LEARN_PARAM2(parameterInArray));
	
	if( ++i >= (int)(LEARN_PARAM3(parameterInArray))){
	    updateNormalizedWeights(LEARN_PARAM1(parameterInArray));
	    clearAllDeltas();
	    i = 0;
	}
    }

    /* update the network weights */
    if (i>0)
    {
	updateNormalizedWeights(LEARN_PARAM1(parameterInArray));
    }

    return (KernelErrorCode);
    
}


/*****************************************************************************
 *****************************************************************************

  GROUP        : quickpropagation learning function

  AUTHOR       : Peter Zimmerer
             
******************************************************************************
******************************************************************************/

/*****************************************************************************
  FUNCTION : initializeQuickprop

  PURPOSE  : initializes the quickprop learning
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 05.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err initializeQuickprop(void)
{
    register unsigned short flags;
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;


    FOR_ALL_UNITS(unit_ptr) {
	flags = unit_ptr->flags;

	if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {	/* unit is in use  */
	    unit_ptr->value_a = unit_ptr->value_b =
		unit_ptr->value_c = (FlintType) 0;

	    if (flags & UFLAG_SITES) {	/* unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
		    link_ptr->value_a = link_ptr->value_b = 
			                link_ptr->value_c = (FlintType) 0;
	    } else {		/* unit has no sites   */
		if (flags & UFLAG_DLINKS) {	/* unit has direct links    */
		    FOR_ALL_LINKS(unit_ptr, link_ptr)
			link_ptr->value_a = link_ptr->value_b = 
			                    link_ptr->value_c = (FlintType) 0;
		}
	    }
	}
    }

    return (KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : propagateNetBackwardQuickprop

  PURPOSE  : quickprop backward error propagation
  RETURNS  : network error
  NOTES    : quickprop backward error propagation
             (topological) for quickprop with SIGMOID_PRIME_OFFSET
	     batch-modus: without adaption of links and bias

  UPDATE   : 05.11.1993 by Guenter Mamier
******************************************************************************/
static float propagateNetBackwardQuickprop(int pattern_no, int sub_pat_no, 
					   float delta_max)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register float  error,	/* error  */
    sum_error,			/* sum of the error  */
    devit;			/* deviation  */
    TopoPtrArray    topo_ptr;
    int size;


    sum_error = 0.0;		/* reset network error  */

    /* calculate address of the output pattern (with number pattern_no + 1)  */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;

    /* add 3 to no_of_topo_units because the topologic array contains 4 NULL
       pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	devit = *(--out_pat) - unit_ptr->Out.output;
	/* = o * (1.0 - o) in [0.0,0.25], */
	/* for asymmetric logistic function */

	if ((float) fabs(devit) <= delta_max)
	    continue;

	sum_error += devit * devit; /* sum up the error of the network  */

	/* calc. error for output units	 */
	error = devit * (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr))  +
			 SIGMOID_PRIME_OFFSET);

	unit_ptr->value_c += -error; /* calculate the bias slopes  */
	/* learn bias like a weight  */
	if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {
	    /* the unit has direct links  */
	    FOR_ALL_LINKS(unit_ptr, link_ptr) {	/* calculate the slopes */
		link_ptr->value_c += -error * link_ptr->to->Out.output;
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
	    }
	} else {
	    /* the unit has sites  */
	    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {	
		/* calculate the value_cs  */
		link_ptr->value_c += -error * link_ptr->to->Out.output;
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
	    }
	}
    }


    /* calculate hidden units only  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	error = (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr))  +
		 SIGMOID_PRIME_OFFSET) * unit_ptr->Aux.flint_no;

	unit_ptr->value_c += -error; /* calculate the bias slopes  */
	/* learn bias like a weight  */
	if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {
	    /* the unit has direct links  */
	    FOR_ALL_LINKS(unit_ptr, link_ptr) { /* calculate the slopes  */
		if IS_HIDDEN_UNIT
		    (link_ptr->to)
			/* this link points to a hidden unit: sum up the
			   error's from previos units  */
			link_ptr->to->Aux.flint_no += 
			    link_ptr->weight * error;

		link_ptr->value_c += -error * link_ptr->to->Out.output;
	    }
	} else {
	    /* the unit has sites  */
	    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {	
		/* calculate the slopes  */
		if IS_HIDDEN_UNIT
		    (link_ptr->to)
			/* this link points to a hidden unit: sum up the
			   error's from previos units  */
			link_ptr->to->Aux.flint_no += 
			    link_ptr->weight * error;

		link_ptr->value_c += -error * link_ptr->to->Out.output;
	    }
	}
    }
    return (sum_error);		/* return the error of the network  */
}


/*****************************************************************************
  FUNCTION : MODI_quickprop

  PURPOSE  : modifies the network at the end of each epoch
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static void  MODI_quickprop(float learn_parameter, float max_factor, 
			    float decay)
 /* learning parameter */
 /* maximal grow factor of weights */
 /* decay factor */

{
    double          deltaw;	/* actual weight (bias) change */
    float           shfac;	/* shrink factor */
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    TopoPtrArray    topo_ptr;
    bool            hidden_units;


    /* maximal grow factor of weights is max_factor  */
    shfac = max_factor / (1.0 + max_factor);

     topo_ptr = topo_ptr_array + (NoOfInputUnits + 1);
    hidden_units = TRUE;

    /* calculate hidden and output units only  */
    do {
	if ((unit_ptr = *++topo_ptr) == NULL) {
	    if (!hidden_units)
		break;		/* end of topologic pointer array reached  */
	    unit_ptr = *++topo_ptr;	/* skip NULL pointer  */
	    hidden_units = FALSE;
	}
	if (IS_SPECIAL_UNIT(unit_ptr)) {
	    unit_ptr->value_a = 
	    unit_ptr->value_b = 
	    unit_ptr->value_c = 0.0;
	    if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {/* unit has direct links */
		FOR_ALL_LINKS(unit_ptr, link_ptr) {
		    link_ptr->value_a = 
		    link_ptr->value_b = 
		    link_ptr->value_c = 0.0;
		}
	    } else {		/* the unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {
		    link_ptr->value_a = 
		    link_ptr->value_b = 
		    link_ptr->value_c = 0.0;
		}
	    }
	} else {
	    deltaw = 0.0;	/* adjust bias like a weight  */
	    if (unit_ptr->value_a > 0.0) { /* previous step was positive  */
		if (unit_ptr->value_c < 0.0)
		    /* same direction,i.e. slope, value_b have same sign  */
		    deltaw += learn_parameter * (-unit_ptr->value_c);

		if (unit_ptr->value_c <= shfac * unit_ptr->value_b)
		    /* maximal positive step  */
		    deltaw += max_factor * unit_ptr->value_a;
		else
		    /* littler positive step squared approximation  */
		    deltaw += unit_ptr->value_c / 
			      (unit_ptr->value_b - unit_ptr->value_c)
			      * unit_ptr->value_a;
	    } else if (unit_ptr->value_a < 0.0) {	/* previous step was
							   negative  */
		if (unit_ptr->value_c > 0.0)
		    /* same direction,i.e. slope, prevslope have same sign  */
		    deltaw += learn_parameter * (-unit_ptr->value_c);

		if (unit_ptr->value_c >= shfac * unit_ptr->value_b)
		    /* maximal negative step  */
		    deltaw += max_factor * unit_ptr->value_a;
		else
		    /* littler negative step squared approximation */
		    deltaw += unit_ptr->value_c / 
			      (unit_ptr->value_b - unit_ptr->value_c)
			      * unit_ptr->value_a;
	    } else
		/* previous step was 0.0  */
		/* start of learning process with BP  */
		deltaw += learn_parameter * (-unit_ptr->value_c);

	    unit_ptr->bias += deltaw;	/* new bias */
	    unit_ptr->value_a = deltaw;	/* bias change */
	    unit_ptr->value_b = unit_ptr->value_c;	/* previous slope */
	    unit_ptr->value_c = decay * unit_ptr->bias;	/* set new slope  */

	    /* adjust links */
	    if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {/* unit has direct links */
		FOR_ALL_LINKS(unit_ptr, link_ptr) {
		    deltaw = 0.0;

		    if (link_ptr->value_a > 0.0) { /* prev step was positive */
			if (link_ptr->value_c < 0.0)
			    /* same direction,i.e. slope, prevslope have same
			       sign  */
			    deltaw += learn_parameter * (-link_ptr->value_c);

			if (link_ptr->value_c <= shfac * link_ptr->value_b)
			    /* maximal positive step  */
			    deltaw += max_factor * link_ptr->value_a;
			else
			    deltaw += link_ptr->value_c / 
				      (link_ptr->value_b - link_ptr->value_c)
				      * link_ptr->value_a;
		    } else if (link_ptr->value_a < 0.0) {	
			/* previous step was negative */
			if (link_ptr->value_c > 0.0)
			    /* same direction,i.e. slope, prevslope have same
			       sign */
			    deltaw += learn_parameter * (-link_ptr->value_c);

			if (link_ptr->value_c >= shfac * link_ptr->value_b)
			    /* maximal negative step  */
			    deltaw += max_factor * link_ptr->value_a;
			else
			    deltaw += link_ptr->value_c / 
				      (link_ptr->value_b - link_ptr->value_c)
				      * link_ptr->value_a;
		    } else	/* previous step was 0.0  */
			/* start of learning process with BP  */
			deltaw += learn_parameter * (-link_ptr->value_c);

		    link_ptr->weight += deltaw;	/* new weight */
		    link_ptr->value_a = deltaw;	/* weight change */
		    link_ptr->value_b = link_ptr->value_c;  /* previous slope */
		    /* set new slope  */
		    link_ptr->value_c = decay * link_ptr->weight;
		}		/* for links  */
	    } else {		/* the unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr) {
		    deltaw = 0.0;
		    if (link_ptr->value_a > 0.0) {	/* previous step was
							   positive  */
			if (link_ptr->value_c < 0.0)
			    /* same direction,i.e. slope, prevslope have same
			       sign  */
			    deltaw += learn_parameter * (-link_ptr->value_c);

			if (link_ptr->value_c <= shfac * link_ptr->value_b)
			    /* maximal positive step  */
			    deltaw += max_factor * link_ptr->value_a;
			else
			    /* littler positive step squared approximation  */
			    deltaw += link_ptr->value_c / 
				      (link_ptr->value_b - link_ptr->value_c)
				       * link_ptr->value_a;
		    } else if (link_ptr->value_a < 0.0) {	
			/* previous step was negative  */
			if (link_ptr->value_c > 0.0)
			    /* same direction,i.e. slope, prevslope have same
			       sign  */
			    deltaw += learn_parameter * (-link_ptr->value_c);

			if (link_ptr->value_c >= shfac * link_ptr->value_b)
			    /* maximal negative step  */
			    deltaw += max_factor * link_ptr->value_a;
			else
			    deltaw += link_ptr->value_c / 
				      (link_ptr->value_b - link_ptr->value_c)
				      * link_ptr->value_a;
		    } else	/* previous step was 0.0  */
			/* start of learning process with BP  */
			deltaw += learn_parameter * (-link_ptr->value_c);

		    link_ptr->weight += deltaw;	/* new weight */
		    link_ptr->value_a = deltaw;	/* weight change */
		    link_ptr->value_b = link_ptr->value_c; /* previous slope */
		    /* set new slope */
		    link_ptr->value_c = decay * link_ptr->weight;
		}
	    }
	}
    }				/* for units  */
    while (TRUE);

}


/*****************************************************************************
  FUNCTION : LEARN_quickprop

  PURPOSE  : Quickprop learning function
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : max factor (of the net after every epoch)
                                 3 : decay
                                 4 : delta max

             Output Parameters:  1 : error of the network (sum of all cycles)


  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err LEARN_quickprop(int start_pattern, int end_pattern, 
			 float *parameterInArray, int NoOfInParams, 
			 float **parameterOutArray, int *NoOfOutParams)
{
    static float OutParameter[1]; /* OutParameter[0] stores the learning error*/
    int          pattern_no, sub_pat_no, ret_code;


    if (NoOfInParams < 1)	/* ###  have to be changed  (must be 3)  #### */
	return (KRERR_PARAMETERS);	/* not enough input parameters  */

    *NoOfOutParams = 1;		/* one return value is available (the
				   learning error) */

    *parameterOutArray = OutParameter;	/* set output parameter reference  */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {	
	/* Net has been modified or topologic array isn't initialized */
	/* check the topology of the network  */
	ret_code = kr_topoCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);	/* an error has occured  */
	if (ret_code < 2)
	    return (KRERR_FEW_LAYERS);	/* the network has less then 2 layers */

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
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified or
						   initialized, initialize
						   backprop now  */
	ret_code = initializeQuickprop();
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	propagateNetForward(pattern_no,sub_pat_no);   /* Forward propagation */

	/* backward propagation and summation of gradient  */
	NET_ERROR(OutParameter) +=
	    propagateNetBackwardQuickprop(pattern_no,sub_pat_no,
					  LEARN_PARAM4(parameterInArray));
    }

    /* modificate links and bias  */
    MODI_quickprop(LEARN_PARAM1(parameterInArray),
		   LEARN_PARAM2(parameterInArray),
		   LEARN_PARAM3(parameterInArray));

    return (ret_code);
}



/*****************************************************************************
 *****************************************************************************

  GROUP        : Counterpropagation learning function

******************************************************************************
******************************************************************************/

/*****************************************************************************
  FUNCTION : initializeCPN

  PURPOSE  : Counterpropagation initialisation
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err initializeCPN(void)
{
    register struct Unit *unit_ptr;


    NoOfLearnedPatterns = 0;

    /* set unit's bias to zero  */
    FOR_ALL_UNITS(unit_ptr)
	if ((unit_ptr->flags & UFLAG_IN_USE) == UFLAG_IN_USE)
	/* unit is in use  */
	unit_ptr->bias = (FlintType) 0;

    return (KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : normalize_weight

  PURPOSE  : Counterpropagation initialisation
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static void normalize_weight(struct Unit * winner_ptr, float sum)
{
    register struct Site *site_ptr;
    register struct Link *link_ptr;
    register float  amount;


    amount = 1.0 / sqrt(sum);

    /* not necessary to see whether this is a special unit */

    if (winner_ptr->flags & UFLAG_SITES)
	/* the unit has sites */
	FOR_ALL_SITES_AND_LINKS(winner_ptr, site_ptr, link_ptr)
	    link_ptr->weight = link_ptr->weight * amount;
    else
	/* the unit has direct links */
	FOR_ALL_LINKS(winner_ptr, link_ptr)
	    link_ptr->weight = link_ptr->weight * amount;
}


/*****************************************************************************
  FUNCTION : normalize_inputvector 

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static void normalize_inputvector(float sum)
{
    register struct Unit *unit_ptr;
    register float  amount;


    amount = 1.0 / sqrt(sum);

    FOR_ALL_UNITS(unit_ptr)
	if (IS_INPUT_UNIT(unit_ptr) && UNIT_IN_USE(unit_ptr))
	/* this is a input unit */
	unit_ptr->Out.output = unit_ptr->Out.output * amount;
}



/*****************************************************************************
  FUNCTION : propagateNet_CPN

  PURPOSE  : forward pass of counterprop
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static float propagateNet_CPN(int pattern_no, int sub_pat_no, float alpha,
			      float beta, float threshold)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    register struct Unit *winner_ptr;
    register Patterns in_pat, out_pat;
    float           maximum, sum_error, devit, learn_error, sum;
    float           unit_ptr_net;
    float           noOfPatterns_mul_NoHiddenUnits;
    register TopoPtrArray topo_ptr;

    /* calculate the activation and the output values         */
    /* of the input units (Input Layer)                       */

    noOfPatterns_mul_NoHiddenUnits = (float) NoOfLearnedPatterns *
	                             (float) NoOfHiddenUnits;

    sum = 0.0;

    /* calculate startaddress for input pattern array  */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);


    topo_ptr = topo_ptr_array;

    /* copy pattern into input unit's activation and calculate output of the
       input units */
    while ((unit_ptr = *++topo_ptr) != NULL) {	
	
	/* topo_ptr points to the unit stuctures (sorted by: input-, hidden- 
	   and output-units, separated with NULL pointers) */
	sum += *in_pat * *in_pat;

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /* identity output function: there is no need to call the output
	       function  */
	    unit_ptr->Out.output = unit_ptr->act = *in_pat++;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act = *in_pat++);
	else
	    /* no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = 
		(*unit_ptr->out_func) (unit_ptr->act = *in_pat++);
    }

    if (sum != 0.0)
	/* normalize the inputvector */
	normalize_inputvector(sum);


    /* propagate Kohonen Layer   */

    /* calculate the activation and the output values         */
    /* of the hidden units (Kohonen Layer)                    */


    winner_ptr = NULL;
    maximum = -1.0e30;		/* contains the maximum of the activations */

    /* popagate hidden units  */
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* topo_ptr points to a
						   (topological sorted) unit
						   stucture */
	unit_ptr_net = 0.0;
	if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {	/* the unit has direct links */
	    FOR_ALL_LINKS(unit_ptr, link_ptr)
		unit_ptr_net += (link_ptr->weight * link_ptr->to->Out.output);
	} else {		/* the unit has sites	 */
	    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
		unit_ptr_net += (link_ptr->weight * link_ptr->to->Out.output);
	}

	if (unit_ptr->bias >= noOfPatterns_mul_NoHiddenUnits)
	    unit_ptr_net -= threshold;

	if (maximum < unit_ptr_net) {	/* determine winner unit  */
	    winner_ptr = unit_ptr;
	    maximum = unit_ptr_net;
	}
	/* reset output and activation of hidden units  */
	unit_ptr->Out.output = unit_ptr->act = (FlintType) 0;
    }


    /* the competitive winner is chosen                */

    winner_ptr->Out.output = winner_ptr->act = (FlintType) 1;
    winner_ptr->bias++;



    /* Training the Kohonen Layer                   
       
       Only the weights of links that go to the winning unit are adjusted, 
       the others remain the same. The incoming weights to the competitive 
       unit are adapted as follows:
         
       weight(new) = weight(old) + eta * (output - weight(old))  
         
       where eta is the learning constant (0<eta<=1.0)             
       and output is the output of the input unit                  
    */ 


    if (!IS_SPECIAL_UNIT(winner_ptr)) {
	sum = 0.0;
	if (winner_ptr->flags & UFLAG_DLINKS) {	/* the winner unit has direct
						   links  */
	    FOR_ALL_LINKS(winner_ptr, link_ptr) {
		devit = link_ptr->to->Out.output - link_ptr->weight;
		learn_error = alpha * devit;
		link_ptr->weight += learn_error;
		/* this is needed for the normalization of the weight_vector */
		sum += link_ptr->weight * link_ptr->weight;
	    }
	} else {		/* the winner unit has sites  */
	    FOR_ALL_SITES_AND_LINKS(winner_ptr, site_ptr, link_ptr) {
		devit = link_ptr->to->Out.output - link_ptr->weight;
		learn_error = alpha * devit;
		link_ptr->weight += learn_error;
		/* this is needed for the normalization of the weight_vector */
		sum += link_ptr->weight * link_ptr->weight;
	    }
	}
	if (sum != 0.0)
	    normalize_weight(winner_ptr, sum);
    }


    /* propagate Grossberg Layer                      */
    /* Training the Grossberg Layer                   */
    /* Adaptation of the Grossberg Layer weights is done by the    */
    /* Widrow-Hoff rule:                                           */

    /* weight(new) = weight(old) + beta * (target output - output) */

    /* for all weights connected with the winning unit of the      */
    /* Kohonen Layers                                              */


    /* calculate address of the output pattern */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,NULL);


    sum_error = 0.0;

    /* popagate output units  */
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* topo_ptr points to a
						   (topological sorted) unit
						   stucture */

	/* calculate the activation and the output values         */
	/* of the output units (Grossberg Layer)                  */

	/* the activation function is the identity function (weighted sum)
	   and identity output function */
	unit_ptr->Out.output = unit_ptr->act = 
		((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(*unit_ptr->act_func) (unit_ptr)) ;

	devit = *out_pat++ - unit_ptr->Out.output;    /* calculate devitation */
	sum_error += devit * devit;
	learn_error = beta * devit;

	if (!IS_SPECIAL_UNIT(unit_ptr)) {
	    if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)){ /* unit has direct links */
		FOR_ALL_LINKS(unit_ptr, link_ptr)
		    if (link_ptr->to == winner_ptr) {
			/* link to the winning unit of the Kohonen Layer */
			link_ptr->weight += learn_error;
			break;
		    }
	    } else {		/* the unit has sites */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
		    if (link_ptr->to == winner_ptr) {
			/* link to the winning unit of the Kohonen Layer */
			link_ptr->weight += learn_error;
			break;
		    }
	    }
	}
    }

    return (sum_error);
}



/*****************************************************************************
  FUNCTION : LEARN_CPN

  PURPOSE  : main function for counterpropagtion
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err  LEARN_CPN(int start_pattern, int end_pattern, 
		    float *parameterInArray, int NoOfInParams, 
		    float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             ret_code, pattern_no, sub_pat_no;


    if (NoOfInParams < 1)	/* have to be changed (must be 3) */
	return (KRERR_PARAMETERS);	/* Not enough input parameters  */

    *NoOfOutParams = 1;		/* one return value is available (the
				   learning error) */
    *parameterOutArray = OutParameter;	/* set output parameter reference  */
    ret_code = KRERR_NO_ERROR;	/* clear return code  */


    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)) {	
	/* Net has been modified  or topologic array isn't initialized */
	/* check the topology of the network  */
	ret_code = kr_topoCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);	/* an error has occured  */
	if (ret_code != 3)
	    return (KRERR_FEW_LAYERS);	/* the network has less then 2 layers */

	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);

	/* sort units by topology and by topologic type  */
	ret_code = kr_topoSort(TOPOLOGIC_TYPE);
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return (ret_code);

	NetModified = FALSE;
    }
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified or
						   initialized, initialize
						   backprop now  */
	ret_code = initializeCPN();
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	NoOfLearnedPatterns++;
	NET_ERROR(OutParameter) 
	    += propagateNet_CPN(pattern_no,sub_pat_no,
				LEARN_PARAM1(parameterInArray),
				LEARN_PARAM2(parameterInArray),
				LEARN_PARAM3(parameterInArray));
    }

    return (ret_code);
}




/*****************************************************************************
 *****************************************************************************

  GROUP        : Back-Percolation Learning Function

  AUTHOR       : Artemis Hatzigeorgiou  Algorithm by Mark Jurik
             
******************************************************************************
******************************************************************************/

/*****************************************************************************
  FUNCTION : propagateNetForward_perc

  PURPOSE  : topological forward propagation
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static void propagateNetForward_perc(int pattern_no, int sub_pat_no)
{
    register struct Unit *unit_ptr;
    register Patterns in_pat;
    register TopoPtrArray topo_ptr;

    /* calculate startaddress for input pattern array  */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);

    topo_ptr = topo_ptr_array;

    /* copy pattern into input unit's activation and calculate output of the
       input units */
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* topo_ptr points to a
						   (topological sorted) unit
						   stucture (input units
						   first)  */
	if (unit_ptr->out_func == OUT_IDENTITY)
	    /* identity output function: there is no need to call the output
	       function  */
	    unit_ptr->Out.output = unit_ptr->act = *in_pat++;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act = *in_pat++);
	else
	    /* no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = 
		(*unit_ptr->out_func) (unit_ptr->act = *in_pat++);
    }

    /* popagate hidden units  */
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* topo_ptr points to a
						   (topological sorted) unit
						   stucture */
	/* clear values  */
	unit_ptr->Aux.flint_no = 0.0;
	unit_ptr->value_a = 0.0;
	unit_ptr->value_b = 0.000001;

	/* calculate the activation value of the unit: call the activation
	   function if needed  */
	unit_ptr->act = ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(*unit_ptr->act_func) (unit_ptr)) ;

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /* identity output function: there is no need to call the output
	       function  */
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else
	    /* no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    /* popagate output units  */
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* topo_ptr points to a
						   (topological sorted) unit
						   stucture */
	/* clear values  */
	unit_ptr->Aux.flint_no = 0.0;
	unit_ptr->value_a = 0.0;
	unit_ptr->value_b = 0.000001;

	/* calculate the activation value of the unit: call the activation
	   function if needed  */
	unit_ptr->act = ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(*unit_ptr->act_func) (unit_ptr)) ;

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /* identity output function: there is no need to call the output
	       function  */
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else
	    /* no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }
}


/*****************************************************************************
  FUNCTION : propagateNetBackward_perc

  PURPOSE  :topological backward propagation
  RETURNS  : network error
  NOTES    :

  UPDATE   : 07.02.1994 by Sven Doering
******************************************************************************/
static float propagateNetBackward_perc(int pattern_no, int sub_pat_no,
				       float learn_parameter,
				       float delta_max, float *perc_error)
{
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register float  error, sum_error, eta, devit;
    register TopoPtrArray topo_ptr;
    register float  norm, delta_sig_normaliser, message_weight;
    register float  act_err, normalised_error, scaled_error, 
                    delta_weight_normaliser;
    register float  der = 0.0;
    register float  tmp;	
    register int    is_special;
    int size;

    sum_error = 0.0;		/* reset network error  */
    eta = learn_parameter;	/* store learn_parameter in CPU register  */

    /* calculate address of the output pattern (with number pattern_no + 1)  */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;

    /* add 3 to no_of_topo_units because the topologic array contains 4 NULL
       pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	devit = *(--out_pat) - unit_ptr->Out.output;	/* calc. devitation */

	if (fabs(devit) > delta_max) {	/* calc. error for output units     */
	    *perc_error += fabs(devit);
	    error = -2.0 * devit * 
	    	((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) ;
	    act_err = devit * eta;
	    sum_error += devit * devit;	/* sum up the error of the network  */
	} else {		/* set error of output units to zero	 */
	    error = 0.0;
	    act_err = 0.000001 * eta;
	    continue;
	}

	/* calc. the error for adjusting weights and bias of the predecessor
	   units  */

	norm = 0.0;
	delta_sig_normaliser = 0.000001;
	FOR_ALL_LINKS(unit_ptr, link_ptr) {	/* adjust link weights and
						   calc. sum of errors of the
						   predecessor units  */
	    if (IS_HIDDEN_UNIT(link_ptr->to))
		norm += fabs(link_ptr->weight);
	    delta_sig_normaliser += SQR(link_ptr->to->Out.output);
	}
	delta_weight_normaliser = delta_sig_normaliser + 1;
	norm += delta_sig_normaliser;
	is_special = IS_SPECIAL_UNIT(unit_ptr);
	normalised_error = act_err / norm;
	scaled_error = act_err / delta_weight_normaliser;
	FOR_ALL_LINKS(unit_ptr, link_ptr) {
	    tmp = link_ptr->weight * error;
	    link_ptr->to->Aux.flint_no += tmp;

	    message_weight = tmp * tmp;
	    if (!is_special) {
		link_ptr->to->value_a += link_ptr->weight * 
		    normalised_error * message_weight;
		link_ptr->to->value_b += message_weight;
		link_ptr->weight += link_ptr->to->Out.output * scaled_error;
	    }
	}


	/* adjust bias value  */
	if (!is_special)
	    unit_ptr->bias += scaled_error;
    }

    /* calculate hidden units only  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	der = ((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) ;
	error = der * unit_ptr->Aux.flint_no;
	act_err = (unit_ptr->value_a / unit_ptr->value_b) * der;

	/* calc. the error for adjusting weights and bias of the predecessor
	   units  */

	norm = 0.0;
	delta_sig_normaliser = 0.000001;
	FOR_ALL_LINKS(unit_ptr, link_ptr) {
	    if (IS_HIDDEN_UNIT(link_ptr->to))
		norm += fabs(link_ptr->weight);

	    delta_sig_normaliser += SQR(link_ptr->to->Out.output);
	}
	delta_weight_normaliser = delta_sig_normaliser + 1;
	norm += delta_sig_normaliser;
	is_special = IS_SPECIAL_UNIT(unit_ptr);
	normalised_error = act_err / norm;
	scaled_error = act_err / delta_weight_normaliser;
	FOR_ALL_LINKS(unit_ptr, link_ptr) {
	    tmp = link_ptr->weight * error;
	    link_ptr->to->Aux.flint_no += tmp;

	    message_weight = tmp * tmp;

	    if (!is_special) {
		link_ptr->to->value_a += link_ptr->weight * 
		    normalised_error * message_weight;
		link_ptr->to->value_b += message_weight;
		link_ptr->weight += link_ptr->to->Out.output * scaled_error;
	    }
	}


	/* adjust bias value  */
	if (!is_special)
	    unit_ptr->bias += scaled_error;
    }

    return (sum_error);		/* return the error of the network */
}



/*****************************************************************************
  FUNCTION : LEARN_perc

  PURPOSE  : main function for backpercolation
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : delta max

             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err LEARN_perc(int start_pattern, int end_pattern, 
	            float *parameterInArray, int NoOfInParams, 
	            float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             ret_code, pattern_no, sub_pat_no;
    float           p_error, l_error;
    register struct Unit *unit_ptr;

    if (NoOfInParams < 1)	        /* have to be changed (must be 2)  */
	return (KRERR_PARAMETERS);	/* Not enough input parameters  */

    *NoOfOutParams = 1;		        /* One return value is available (the
				           learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {	
	/* Net has been modified or topologic array isn't initialized */
	/* check the topology of the network  */
	FOR_ALL_UNITS(unit_ptr)
	    if UNIT_HAS_SITES
	    (unit_ptr)
		return (KRERR_SITES_NO_SUPPORT);

	ret_code = kr_topoCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);	/* an error has occured  */
	if (ret_code < 2)
	    return (KRERR_FEW_LAYERS);	/* the network has less then 2 layers */

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
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified or
						   initialized, initialize
						   backprop now  */
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);
	parameterInArray[4] = 1.0;
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */
    p_error = 0.0;

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	propagateNetForward_perc(pattern_no,sub_pat_no);  /* Forward pass */

	/* Backward propagation  */
	/* 1st parameter is the learning parameter 2nd parameter is the max.
	   devitation between output pattern and the output of the output
	   unit (delta max) */

	NET_ERROR(OutParameter) +=
	    propagateNetBackward_perc(pattern_no,sub_pat_no,
				      LEARN_PARAM1(parameterInArray),
				      LEARN_PARAM3(parameterInArray), &p_error);
    }

    p_error = p_error / (kr_TotalNoOfSubPatPairs()* NoOfOutputUnits);

    if (p_error < LEARN_PARAM2(parameterInArray)) {
	p_error = (parameterInArray[4] + p_error) / 2;
	l_error = exp((parameterInArray[4] - p_error) / 
		      (parameterInArray[4] + p_error));
	if (l_error <= 0.5)
	    l_error = 0.5;
	else if (l_error >= 1.05)
	    l_error = 1.05;
	parameterInArray[0] = parameterInArray[0] * l_error;
    }
    parameterInArray[4] = p_error;

    return (ret_code);
}



/*****************************************************************************
 *****************************************************************************

  GROUP  : Radial Basis Functions Learning

  AUTHOR : Michael Vogt
  Notes  : Use of special entries in links and units with RBFs:

           for Units in hidden layer:
	   Unit value_a: |X - L|^2  == norm^2 == square of euclidean distance 
	                  between all links and all input units to this unit.
	   Unit value_b: delta_BIAS == sum of all deltas to BIAS during learning
	   Unit value_c: Backpropagated weighted sum of errors in output layer

	   for Units in output layer:
	   Unit value_a: error (y_learn - y_net) during learning current pattern
	   Unit value_b: delta_BIAS == sum of all deltas to BIAS during learning

	   for links between input and hidden layer:
	   Link value_b: delta for this link during learning (link treated as 
	                 vector)
	   Link value_a: Momentum term for this link (last change)

	   for links between hidden and output layer:
	   Link value_b: delta for weight of this link during learning.
	   Link value_a: Momentum term for this link (last change)
 
	   for links between input and output layer:
	   Link value_b: delta for weight of this link during learning.
	   Link value_a: Momentum term for this link (last change)
             
******************************************************************************
******************************************************************************/


/*****************************************************************************
  FUNCTION : RbfLearnClean

  PURPOSE  : Clean all deltas, so that learning can start.
  RETURNS  : kernel error code
  NOTES    : Called every time LEARN_RBF is called to be sure that there is 
             no stuff inside the value_b fields of links and units

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err RbfLearnClean(void)
{
    register struct Unit *unit_ptr;
    register struct Link *link_ptr;

    FOR_ALL_UNITS(unit_ptr) {
	unit_ptr->value_b = 0.0;
	FOR_ALL_LINKS(unit_ptr, link_ptr) {
	    link_ptr->value_b = 0.0;
	}
    }

    return KRERR_NO_ERROR;
}



/*****************************************************************************
  FUNCTION : RbfLearnForward

  PURPOSE  : Forward propagation of current pattern. Calculation of different
             value_a fields. value_c of hidden units is set to 0.0
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err  RbfLearnForward(int pattern_no, int sub_pat_no)
{
    register struct Unit *unit_ptr;
    register Patterns current_in_pattern;
    register Patterns current_out_pattern;
    register TopoPtrArray topo_ptr;

    /* calculate index of current input pattern in Pattern array:	 */
    current_in_pattern = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);

    /* activate input units with current patterns and calculate	 */
    /* their output value:						 */

    topo_ptr = topo_ptr_array;
    while ((unit_ptr = *(++topo_ptr)) != NULL) {
	/* go through all input units, set activation and calculate */
	/* output:							 */

	unit_ptr->act = *current_in_pattern++;
	if(unit_ptr->out_func == OUT_IDENTITY)
		unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else	
	unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    /* activate hidden units, by calling the activation function	 */
    /* (has to be a RBF activation function which places norm ^ 2	 */
    /* into value_a of the unit: see trans_f.c: RbfUnitGetNormsqr). */
    /* The output function is supposed to be OUT_IDENTITY !		 */
    /* (so the output function is never called !)			 */

    while ((unit_ptr = *(++topo_ptr)) != NULL) {
	unit_ptr->act = unit_ptr->Out.output =
	    ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(*unit_ptr->act_func) (unit_ptr)) ;

	unit_ptr->value_c = 0.0;
    }

    /* activate output units. Again, the output function is supposed */
    /* to be OUT_IDENTITY. The calculated output is compared to the */
    /* current pattern, the error (difference) is calculated and    */
    /* stored in value_a of the current output unit.		 */

    current_out_pattern = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,NULL);

    while ((unit_ptr = *(++topo_ptr)) != NULL) {
	unit_ptr->act = unit_ptr->Out.output =
	    ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(unit_ptr->act_func) (unit_ptr)) ;
	unit_ptr->value_a = *current_out_pattern++ - unit_ptr->act;
    }

    return KRERR_NO_ERROR;
}


#define	RBF_LEARN_CENTER	0x1
#define RBF_LEARN_BIAS		0x2
#define RBF_LEARN_WEIGHT	0x4
#define RBF_LEARN_PAIN		0x8

/*****************************************************************************
  FUNCTION : RbfLearnAdjustDelta

  PURPOSE  : Adjusting of all deltas (value_b fields) by using the current 
             input pattern (activation of input units) and the stored error 
	     of the output units (value_a). value_c of hidden units is used 
	     too!
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
float RbfLearnAdjustDelta(float para_center, float para_bias, 
			  float para_weight, float para_pain, 
			  float para_momentum, float para_delta_max, 
			  int learn_mask)
{
    register struct Unit *curr_unit;	/* current unit		 */
    register struct Link *curr_link;	/* current link		 */
    register struct Unit *source_unit;	/* input unit to link	 */
    register TopoPtrArray topo_ptr;
    register float  center_delta;	/* delta of centers	 */
    register float  w_error;	        /* weighted error of 	 */
                                        /* output unit		 */
    register float  w2_error;           /* w_error for special u.*/
    register float  learn_error;

    /* start with last unit in output layer:			 */
    topo_ptr = topo_ptr_array + no_of_topo_units + 3;

    learn_error = 0.0;

    /* work on the output layer and all links leading to it:	 */

    while ((curr_unit = *(--topo_ptr)) != NULL) {
	/* go on with next unit if |error| <= delta_max		 */
	if ((float) fabs(curr_unit->value_a) <= para_delta_max)
	    continue;

	/* error, weighted by the deviation of the activation:	 */
	w2_error = w_error = (curr_unit->value_a) *
	    ((curr_unit->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(curr_unit->python_act_deriv_func,
						curr_unit) :
			(curr_unit->act_deriv_func) (curr_unit)) ;

	/* sum up the learning error:				 */
	learn_error += (curr_unit->value_a) * (curr_unit->value_a);

	if (learn_mask & RBF_LEARN_WEIGHT) {
	    /* sum up all deltas for change of bias:		 */

#ifdef RBF_INCR_LEARNING
	    if (IS_SPECIAL_UNIT(curr_unit)
		w_error = 0.0;
	    curr_unit->bias += para_weight * w_error;
#else
	    curr_unit->value_b += w_error;
#endif
	}
	if (learn_mask) {
	    FOR_ALL_LINKS(curr_unit, curr_link) {
		source_unit = curr_link->to;

		/* sum up deltas for change of link weight:	 */

#ifdef RBF_INCR_LEARNING
		curr_link->weight += para_weight * w_error *
		    source_unit->Out.output;
#else
		curr_link->value_b += w_error * source_unit->Out.output;
#endif

		/* if comming from hidden unit: sum up delta for change */
		/* of bias of hidden unit:			        */
		if (IS_HIDDEN_UNIT(source_unit))
		    source_unit->value_c += w2_error * curr_link->weight;
	    }
	}
    }

    /* work on the hidden layer and all links leading to it:	 */

    if (learn_mask & (RBF_LEARN_CENTER | RBF_LEARN_BIAS)) {
	while ((curr_unit = *(--topo_ptr)) != NULL) {
	    /* now calculate delta for weights of links (centers of the */
	    /* RBF function)						 */
	    curr_unit->Aux.int_no = 2;	/* derivated to norm ^2 */
	    center_delta = curr_unit->value_c *
		((curr_unit->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(curr_unit->python_act_deriv_func,
						curr_unit) :
			(curr_unit->act_deriv_func) (curr_unit)) ;

	    if (learn_mask & RBF_LEARN_CENTER) {
#ifdef RBF_INCR_LEARNING
		if (IS_SPECIAL_UNIT(curr_unit))
		    center_delta = 0.0;
#endif
		FOR_ALL_LINKS(curr_unit, curr_link) {

#ifdef RBF_INCR_LEARNING
		    curr_link->weight += para_center * center_delta *
			((curr_link->to->Out.output) - (curr_link->weight));
#else
		    curr_link->value_b += center_delta *
			((curr_link->to->Out.output) - (curr_link->weight));
#endif
		}
	    }
	    /* calculate delta for bias (parameter of RBF function):	 */
	    curr_unit->Aux.int_no = 3;	/* derivation to bias!  */

#ifdef RBF_INCR_LEARNING
	    if (!IS_SPECIAL_UNIT(curr_unit))
		curr_unit->bias += para_bias * curr_unit->value_c *
		    ((curr_unit->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(curr_unit->python_act_deriv_func,
						curr_unit) :
			(*curr_unit->act_deriv_func) (curr_unit)) ;
#else
	    curr_unit->value_b += curr_unit->value_c *
		((curr_unit->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(curr_unit->python_act_deriv_func,
						curr_unit) :
			(curr_unit->act_deriv_func) (curr_unit)) ;
#endif
	}
    }
    return learn_error;
}



/*****************************************************************************
  FUNCTION : RbfLearnAdjustWeights

  PURPOSE  : Adjusting of all learnable parameters, depending on collected 
             deltas and on actual learning parameters.
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
void RbfLearnAdjustWeights(float para_center, float para_bias, 
			   float para_weight, float para_momentum)
{
    register struct Unit *curr_unit;	/* current unit		 */
    register struct Link *curr_link;	/* current link		 */
    register TopoPtrArray topo_ptr;

#ifdef RBF_DELTA_PROT
    static int      step = 0;	/* current learning step */
    char            filename[20];	/* Name of prot file	 */
    FILE           *protfile;	/* filepointer		 */

#endif

#ifdef RBF_DELTA_PROT
    step++;
    sprintf(filename, "rbf_%04d.prot", step);
    protfile = fopen(filename, "w");
    if (protfile == NULL)
	fprintf(stderr, "RbfLearnAdjustWeights: Can't open protfile\n");
#endif

    /* start with last unit in output layer:			 */
    topo_ptr = topo_ptr_array + no_of_topo_units + 3;

#ifdef RBF_DELTA_PROT
    fprintf(protfile, "%s\t\t\n", "h -> o");
#endif

    while ((curr_unit = *(--topo_ptr)) != NULL) {
	if (!IS_SPECIAL_UNIT(curr_unit)) {
	    /* adjust bias of output unit:                       */
	    curr_unit->bias += para_weight * (curr_unit->value_b);

#ifdef RBF_DELTA_PROT
	    fprintf(protfile, "%13s:\t\n", curr_unit->unit_name);
#endif

	    /* adjust weights of links leading to this unit:	 */
	    FOR_ALL_LINKS(curr_unit, curr_link) {

#ifdef RBF_DELTA_PROT
		fprintf(protfile, "%-10.2e\t\n",
			para_weight * (curr_link->value_b));
#endif

		curr_link->weight +=
		    (curr_link->value_a = para_weight * (curr_link->value_b)
		     + para_momentum * curr_link->value_a);
	    }
	}
    }

    /* now adjust weights of hidden layer:			 */

#ifdef RBF_DELTA_PROT
    fprintf(protfile, "%s\t\t\n", "i -> h");
#endif

    while ((curr_unit = *(--topo_ptr)) != NULL) {
	if (!IS_SPECIAL_UNIT(curr_unit)) {
	    /* adjust bias of hidden unit (parameter of RBF function):	 */
	    curr_unit->bias += para_bias * (curr_unit->value_b);
	    if (curr_unit->bias <= 0.0)
		fprintf(stderr, "Hidden unit bias %f !\n", curr_unit->bias);

#ifdef RBF_DELTA_PROT
	    fprintf(protfile, "%13s:\t\n", curr_unit->unit_name);
#endif

	    /* adjust weights of links (centers of RBF functions):	 */
	    FOR_ALL_LINKS(curr_unit, curr_link) {

#ifdef RBF_DELTA_PROT
		fprintf(protfile, "%-10.2e\t\n",
			para_center * (curr_link->value_b));
#endif

		curr_link->weight +=
		    (curr_link->value_a = para_center * (curr_link->value_b)
		     + para_momentum * curr_link->value_a);
	    }
	}
    }

#ifdef RBF_DELTA_PROT
    fclose(protfile);
#endif
}



/*****************************************************************************
  FUNCTION : RbfTopoCheck

  PURPOSE  : Topological Check for Radial Basis Functions.
             Also the number of output units is compared to the patterns.
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err RbfTopoCheck(void)
{
    krui_err        ret_code;	/* error return code		 */

    /* Net has been modified or topologic array isn't		 */
    /* initialized. check the topology of the network.		 */
    ret_code = kr_topoCheck();
    if (ret_code < KRERR_NO_ERROR)
	return (ret_code);	/* an error has occured */
    if (ret_code < 2)
	return (KRERR_NET_DEPTH);	/* the network has less */
    /* then 2 layers	 */

    /* count the no. of I/O units and check the patterns	 */
    ret_code = kr_IOCheck();
    if (ret_code < KRERR_NO_ERROR)
	return (ret_code);

    /* sort units by topology and by topologic type		 */
    ret_code = kr_topoSort(TOPOLOGICAL_FF);

    return ret_code;
}


/*****************************************************************************
  FUNCTION : LEARN_RBF

  PURPOSE  : Learning function for RBF (GRBF) called from kernel.
  RETURNS  : kernel error code
  NOTES    : Use of Learning Parameters:
             LEARN_PARAM1: learning parameter for adjusting centers (links 
	                   between input and hidden layer, treated as vectors)
             LEARN_PARAM2: learning parameter for adjusting RBF-parameter 
	                   (BIAS of units in hidden layer)
             LEARN_PARAM3: learning parameter for adjusting weights (all links
	                   to output layer + bias of output units)
	     LEARN_PARAM4: maximum difference between output value and teaching
                           input which is treated as error 0.0 (delta_max)
             LEARN_PARAM5: factor for momentum term

  UPDATE   : 06.11.1993 by Guenter Mamier 
******************************************************************************/
krui_err LEARN_RBF(int start_pattern, int end_pattern, 
		   float *parameterInArray, int NoOfInParams, 
		   float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores	 */
                                        /* the learning error   	 */
    int             ret_code, pattern_no, sub_pat_no, learn_mask;
    float           para_bias, para_center, para_weight, para_pain, 
                    para_momentum,para_delta_max;

    register struct Unit *unit_ptr;
    register struct Link *link_ptr;

#ifdef RBF_LEARN_PROT
    static int      schritt = 1;
    int             fehler_zaehler = 0;
    float           temp_fehler;
    FILE           *protfile;

#endif

    if (NoOfUnits == 0)
	return (KRERR_NO_UNITS);/* No Units defined		 */
    if (NoOfInParams < 1)	/* has to be changed (must be 4) */
	return (KRERR_PARAMETERS);	/* Not enough input parameters  */

    *NoOfOutParams = 1;		/* One return value is available */
                                /* (the learning error)		 */
    *parameterOutArray = OutParameter;	/* set the reference to */
    /* the output parameter */

    ret_code = KRERR_NO_ERROR;	/* default return code		 */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)) {
	ret_code = RbfTopoCheck();

	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return (ret_code);

	NetModified = FALSE;
    }
    if (NetInitialize || LearnFuncHasChanged) {
	fprintf(stderr, "Initialization RBF_Weights should be called!\n");
	/* initialize fields for momentum term */
	FOR_ALL_UNITS(unit_ptr) {
	    FOR_ALL_LINKS(unit_ptr, link_ptr) {
		link_ptr->value_a = 0.0;
	    }
	}
    }
    NET_ERROR(OutParameter) = 0.0;
    para_center = -LEARN_PARAM1(parameterInArray);
    para_bias = LEARN_PARAM2(parameterInArray);
    para_weight = LEARN_PARAM3(parameterInArray);
    para_momentum = LEARN_PARAM5(parameterInArray);
    para_delta_max = LEARN_PARAM4(parameterInArray);
    para_pain = 0.0;		/* not used now	*/

    /* set learn mask in condition of the learning parameters:	 */
    learn_mask = 0;
    if (para_center != 0.0)
	learn_mask |= RBF_LEARN_CENTER;
    if (para_bias != 0.0)
	learn_mask |= RBF_LEARN_BIAS;
    if (para_weight != 0.0)
	learn_mask |= RBF_LEARN_WEIGHT;
    if (para_pain != 0.0)
	learn_mask |= RBF_LEARN_PAIN;

#ifndef RBF_INCR_LEARNING
    ret_code = RbfLearnClean();
    if (ret_code != KRERR_NO_ERROR)
	return ret_code;
#endif


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	RbfLearnForward(pattern_no,sub_pat_no);	/* forward propagation	 */

	/* backward propagation 					 */

#ifdef RBF_LEARN_PROT
	temp_fehler = RbfLearnAdjustDelta(para_center,
			   para_bias, para_weight, para_pain, para_momentum,
					  para_delta_max, learn_mask);
	NET_ERROR(OutParameter) += temp_fehler;
	if (temp_fehler > 0.0)
	    fehler_zaehler++;
#else
	NET_ERROR(OutParameter) += RbfLearnAdjustDelta(para_center,
			   para_bias, para_weight, para_pain, para_momentum,
						para_delta_max, learn_mask);
#endif
    }

#ifndef RBF_INCR_LEARNING
    RbfLearnAdjustWeights(para_center, para_bias, para_weight,
			  para_momentum);
#endif

#ifdef RBF_LEARN_PROT
    protfile = fopen("rbf_learn_prot_file", "a");
    if (schritt == 1) {
	fprintf(protfile, "# Neues Lernprotokoll: \n");
    }
    fprintf(protfile, "%d %f %d\n", schritt, NET_ERROR(OutParameter),
	    fehler_zaehler);
    fclose(protfile);
    schritt++;
#endif

    return (ret_code);
}

/*****************************************************************************
 *****************************************************************************

  GROUP  (Radial Basis Functions) Dynamic Decay Adjustment (DDA) Learning
  
  AUTHORS Michael R. Berthold (Michael.Berthold@informatik.uni-karlsruhe.de) and 
          Markus J. Weihrauch (Markus.Weihrauch@informatik.uni-karlsruhe.de)
  NOTES Use of special entries in links and units with RBFs
        for units in hidden layer:
          unit.value_a == |X - L|^2  = norm^2 = summed squares of euclidian
            distances between all links and input units of this unit.
        for units in output layer:
          unit.value_a == <desired activation> - <actual activation>
	Use of the following global variables
	  KernelErrorCode, NoOfInputUnits, NoOfHiddenUnits, NoOfOutputUnits,
	  NetModified, TopoSortID, topo_ptr_array
	Uses the following global MACROS
	  MAXINT, KRERR_...
	Output units *must* have "Act_Identity" and RBF (==hidden) units *must*
	  have "Act_RBF_Gaussian" as activation function. 
	All output functions must be "Out_Identity".

******************************************************************************
******************************************************************************/

/* The RBFs act. fct. is exp(-distance^2/sigma^2). Sigma == 1/sqrt(bias) */
#define RBF_MIN_SIGMA 1e-10 
#define RBF_MAX_SIGMA 1e10 

/* This is the maximal number of units displayed in the graphical display */
#define DEF_MAX_UNITS_DISPLAYED 20

#define RBF_GET_UNIT_NO( unit_ptr ) ( (unit_ptr) - unit_array ) 

#define RBF_ERROR_CHECK( x ) \
  KernelErrorCode = ( x ); \
  if ( KernelErrorCode < KRERR_NO_ERROR && \
       KernelErrorCode != KRERR_DEAD_UNITS ) return KernelErrorCode; 

/* Factor for the calculation of the inverse RBF activation function */
#define RBF_INV ( -1.0 * log(theta_neg) )

#define RBF_SQR( x ) ( (x) * (x) )
#define RBF_MIN( x, y ) ( (x) < (y) ? (x) : (y) )
#define RBF_MAX( x, y ) ( (x) > (y) ? (x) : (y) )

/*****************************************************************************
  FUNCTION : LEARN_RBF_DDA

  PURPOSE  : Learning function for RBFs called from kernel.
  RETURNS  : An kernel error code and OutParameter[0] == Mean Square Error 
             (MSE is not very meaningful in classification tasks)
  NOTES    : LEARN_PARAM1: 
               theta_pos, minimum activation of correct RBF (default 0.4)
             LEARN_PARAM2: 
	       theta_neg, maximum activation of competing RBFs (default 0.2)
	     LEARN_PARAM3: 
	       #units per line/column in display (default 20)

  UPDATE   : 8 May 1995 by mb
******************************************************************************/

krui_err LEARN_RBF_DDA(int start_pattern, int end_pattern, 
		       float  *parameterInArray,  int  NoOfInParams, 
		       float **parameterOutArray, int *NoOfOutParams)
{
  static float OutParameter[1];
  float        theta_pos           =       LEARN_PARAM1(parameterInArray),
               theta_neg           =       LEARN_PARAM2(parameterInArray);
  int          max_units_displayed = (int )LEARN_PARAM3(parameterInArray),
               pattern_no, sub_pat_no,
               xmin_in,  ymin_in, xmin_out, ymin_out,
               xmax_in,  ymax_in, xmax_out, ymax_out;
  struct Unit  *input_unit_ptr, *hidden_unit_ptr, *output_unit_ptr; 
  struct Link  *link_ptr;
  TopoPtrArray topo_ptr;
  Patterns     out_pat_ptr;


  /* Treat parameters */

  if  ( theta_pos == 0.0 ) theta_pos = 0.4;

  if  ( theta_neg == 0.0 ) theta_neg = 0.2;

  if ( theta_pos <= 0.0 || theta_pos > 1.0 ) return DDA_PARAM_ONE; 

  if ( theta_neg <= 0.0 || theta_neg > 1.0 ) return DDA_PARAM_TWO; 

  if ( max_units_displayed == 0 ) max_units_displayed = DEF_MAX_UNITS_DISPLAYED;

  if ( max_units_displayed <= 0 ) return DDA_PARAM_THREE; 


  /*  If net has been modified or topologic array isn't initialized:
      Check the topology of the network  */    

  if ( NetModified || (TopoSortID != TOPOLOGICAL_FF) )
    {
      int no_of_layers = kr_topoCheck(); 
      RBF_ERROR_CHECK( no_of_layers );

      if ( no_of_layers < 1 ) return KRERR_FEW_LAYERS;
      if ( no_of_layers > 3 ) return KRERR_MUCH_LAYERS; 

      /*  count the no. of I/O units and check the patterns  */
      RBF_ERROR_CHECK( kr_IOCheck () );

      /*  sort units by topology and by topologic type  */
      RBF_ERROR_CHECK( kr_topoSort( TOPOLOGICAL_FF ) );

      NetModified = FALSE;
    } 


  /* Check the topology and find min+max positions */

  /* NULL before first  unit */
  topo_ptr = topo_ptr_array;
  xmin_in = ymin_in =  MAXINT;
  xmax_in = ymax_in = -MAXINT;

  while ( (input_unit_ptr = * ++topo_ptr ) != NULL)
    {
      if ( !IS_INPUT_UNIT(input_unit_ptr) ) return KRERR_TOPOLOGY; 
      
      if ( UNIT_HAS_INPUTS(input_unit_ptr) || 
	  UNIT_HAS_SITES(input_unit_ptr) )
	{ 
	  krui_setCurrentUnit( RBF_GET_UNIT_NO(input_unit_ptr) );
	  return KRERR_UNEXPECTED_SITES; 
	}

#if 0
      if ( strcmp( krui_getUnitActFuncName( RBF_GET_UNIT_NO(input_unit_ptr) ),
		   "Act_Identity" ) )
	return DDA_INPUT_ACT_FUNC;

      if ( input_unit_ptr->out_func != OUT_IDENTITY )
	{ 
	  krui_setCurrentUnit( RBF_GET_UNIT_NO(input_unit_ptr) );
	  return KRERR_OUT_FUNC; 
	}
#endif

      input_unit_ptr->bias = 0.0;

      xmin_in = RBF_MIN( xmin_in, input_unit_ptr->unit_pos.x );
      ymin_in = RBF_MIN( ymin_in, input_unit_ptr->unit_pos.y );
      xmax_in = RBF_MAX( xmax_in, input_unit_ptr->unit_pos.x );
      ymax_in = RBF_MAX( ymax_in, input_unit_ptr->unit_pos.y );
    } 

  /* topo_ptr points now to the NULL before first hidden (RBF) unit */

  while ( (hidden_unit_ptr = * ++topo_ptr ) != NULL)
    {
      int no_input_units = 0;

      if ( UNIT_HAS_SITES(hidden_unit_ptr) )
	{ 
	  krui_setCurrentUnit( RBF_GET_UNIT_NO(hidden_unit_ptr) );
	  return KRERR_UNEXPECTED_SITES; 
	}
      
      if ( hidden_unit_ptr->out_func != OUT_IDENTITY )
	{ 
	  krui_setCurrentUnit( RBF_GET_UNIT_NO(hidden_unit_ptr) );
	  return KRERR_OUT_FUNC; 
	}
      
      if ( strcmp( krui_getUnitActFuncName( RBF_GET_UNIT_NO(hidden_unit_ptr) ),
		   "Act_RBF_Gaussian" ) )
	return DDA_HIDDEN_ACT_FUNC;

      FOR_ALL_LINKS( hidden_unit_ptr, link_ptr ) 
	{
	  if ( !IS_INPUT_UNIT(link_ptr->to) ) return KRERR_TOPOLOGY; 
	  
	  no_input_units++;
	}

      if ( no_input_units != NoOfInputUnits ||
	  !IS_HIDDEN_UNIT(hidden_unit_ptr) ||
	  !UNIT_HAS_DIRECT_INPUTS(hidden_unit_ptr) || 
	  hidden_unit_ptr->sites == NULL ) return KRERR_TOPOLOGY; 
    } 

  /* topo_ptr points now to the NULL before first output unit */

  xmin_out = ymin_out =  MAXINT;
  xmax_out = ymax_out = -MAXINT;

  while ( (output_unit_ptr = * ++topo_ptr ) != NULL)
    {
      if ( UNIT_HAS_SITES(output_unit_ptr) )
	{ 
	  krui_setCurrentUnit( RBF_GET_UNIT_NO(output_unit_ptr) );
	  return KRERR_UNEXPECTED_SITES; 
	}

#if 0
      if ( output_unit_ptr->out_func != OUT_IDENTITY )
	{
	  krui_setCurrentUnit( RBF_GET_UNIT_NO(output_unit_ptr) );
	  return KRERR_OUT_FUNC; 
	}

      if ( strcmp( krui_getUnitActFuncName( RBF_GET_UNIT_NO(output_unit_ptr) ),
		   "Act_Identity" ) )
	return DDA_OUTPUT_ACT_FUNC;
#endif
      if ( strcmp( krui_getUnitActFuncName( RBF_GET_UNIT_NO(output_unit_ptr) ),
		   "Act_Identity" ) )
        {
	  RBF_ERROR_CHECK( krui_setUnitActFunc ( RBF_GET_UNIT_NO(output_unit_ptr), "Act_Identity" ) );
        }

      if ( output_unit_ptr->out_func != OUT_IDENTITY )
        output_unit_ptr->out_func = OUT_IDENTITY; 

      output_unit_ptr->bias = 0.0;

      xmin_out = RBF_MIN( xmin_out, output_unit_ptr->unit_pos.x );
      ymin_out = RBF_MIN( ymin_out, output_unit_ptr->unit_pos.y );
      xmax_out = RBF_MAX( xmax_out, output_unit_ptr->unit_pos.x );
      ymax_out = RBF_MAX( ymax_out, output_unit_ptr->unit_pos.y );

      /* Set the weights of all links from RBFs to output to zero */
      FOR_ALL_LINKS( output_unit_ptr, link_ptr ) 
	{
	  if ( start_pattern != end_pattern ) link_ptr->weight = 0.0;

	  if ( !IS_HIDDEN_UNIT(link_ptr->to) ) return DDA_SHORTCUTS; 
	}

      /* If it's an empty net no connections are allowed */
      if ( !IS_OUTPUT_UNIT(output_unit_ptr) ||
	  (NoOfHiddenUnits == 0 && output_unit_ptr->sites != NULL ))
	return KRERR_TOPOLOGY; 
    } 


  /* The big MAIN Loop:
     Loop through patterns from pattern no. start_pattern to end_pattern */

  /* compute the necessary sub patterns */
  RBF_ERROR_CHECK( kr_initSubPatternOrder ( start_pattern, end_pattern ) );
 
  while( kr_getSubPatternByOrder ( &pattern_no, &sub_pat_no ) )
    {
      int      correct_output_unit_no;
      struct   Link *max_to_out_link_ptr;
      struct   Unit *max_rbf_ptr, *correct_output_unit_ptr;

      /* forward propagation */
      RbfLearnForward ( pattern_no, sub_pat_no );

      /* Find correct output neuron */

      out_pat_ptr = kr_getSubPatData ( pattern_no, sub_pat_no, OUTPUT, NULL );

      topo_ptr = topo_ptr_array + 2 + NoOfInputUnits  + NoOfHiddenUnits;
      correct_output_unit_ptr = NULL;

      while( (output_unit_ptr = * ++topo_ptr) != NULL )
	if ( *out_pat_ptr++ > 0.0 )
	  if ( correct_output_unit_ptr == NULL )
	    correct_output_unit_ptr = output_unit_ptr;
	  else
	    return DDA_DESIRED_CLASS; 
      
      /* Is there a desired class ? */
      max_rbf_ptr = NULL;
      
      if ( correct_output_unit_ptr != NULL )
	{
	  correct_output_unit_no = RBF_GET_UNIT_NO ( correct_output_unit_ptr );
	  
	  /* Find nearest RBF (having highest activation) of correct class */
	  
	  FOR_ALL_LINKS( correct_output_unit_ptr, link_ptr)
	    if ( link_ptr->to->act >= theta_pos )
	      if ( max_rbf_ptr != NULL )
		{
		  if ( link_ptr->to->act > max_rbf_ptr->act ) 
		    max_rbf_ptr = link_ptr->to; 
		}
	      else 
		max_rbf_ptr = link_ptr->to; 
	}
      
      /* Shrink competing RBFs */
      
      topo_ptr = topo_ptr_array + 2 + NoOfInputUnits + NoOfHiddenUnits;
      
      while ( ( output_unit_ptr = * ++topo_ptr ) != NULL )
	/* Only competing classes */
	if ( output_unit_ptr != correct_output_unit_ptr )
	  FOR_ALL_LINKS( output_unit_ptr, link_ptr )
	    /* Only competing RBFs with a too big activation */
	    if ( link_ptr->to->act > theta_neg )
	      { 
		/* Shrink! */
		if ( link_ptr->to->bias < 1.0/RBF_SQR( RBF_MIN_SIGMA ) )
		  {
		    if ( link_ptr->to->value_a/RBF_INV > RBF_SQR( RBF_MIN_SIGMA ) )
		      link_ptr->to->bias = RBF_INV/link_ptr->to->value_a;
		    else 
		      link_ptr->to->bias = 1.0/RBF_SQR( RBF_MIN_SIGMA );
		  }
#ifdef RBF_DEBUG
		else
		  fprintf(stderr,"\nRBF-DDA WARNING: Sigma too small (competing)!\n");
#endif
	      }
      /* End Shrink competing RBFs */

      /* If there is a desired class: is the actual input pattern already covered 
	 by an RBF of the correct class? */
      
      if ( correct_output_unit_ptr != NULL )
	if ( max_rbf_ptr != NULL) 
	  {
	    /* increase weight of link from nearest RBF 
	       to correct output neuron */
	    
	    /* Find link from RBF with max. act. to the correct output unit */
	    max_to_out_link_ptr = 
	      (struct Link *) correct_output_unit_ptr->sites;
	    
	    FOR_ALL_LINKS ( correct_output_unit_ptr, link_ptr )
	      if ( link_ptr->to == max_rbf_ptr ) 
		max_to_out_link_ptr = link_ptr;
	    
	    /* Increase the weight of this link */
	    max_to_out_link_ptr->weight += 1.0;
	  } /* end if covered or no desired class */
      
	else 
	  /* Not covered:
	     No RBF near enough or no RBF at all yet ... commit new RBF */
	  {
	    struct Unit *new_rbf_ptr;
	    int new_rbf_no = kr_makeDefaultUnit();
	    
	    NetModified = TRUE;
	    RBF_ERROR_CHECK( new_rbf_no );
	    RBF_ERROR_CHECK( kr_unitSetTType ( new_rbf_no, HIDDEN ) );
	    RBF_ERROR_CHECK( krui_setUnitActFunc ( new_rbf_no, "Act_RBF_Gaussian" ) );
	    new_rbf_ptr = kr_getUnitPtr ( new_rbf_no );
	    RBF_ERROR_CHECK( KernelErrorCode );
	    new_rbf_ptr->i_act      = 0.0;
	    new_rbf_ptr->bias       = 1.0/RBF_SQR( RBF_MAX_SIGMA );
	    new_rbf_ptr->out_func   = OUT_IDENTITY; 
	    
	    /* Set  weight of links from inputs to new RBF (=center of RBF) */
	    
	    RBF_ERROR_CHECK( krui_setCurrentUnit ( new_rbf_no ) );
	    
	    /* Scan input units and create links */
	    topo_ptr = topo_ptr_array;
	    
	    while ( (input_unit_ptr = * ++topo_ptr ) != NULL)
	      RBF_ERROR_CHECK
		( krui_createLink ( RBF_GET_UNIT_NO ( input_unit_ptr ), 
				   input_unit_ptr->act ) );  
	    
	    /* Make link from correct output unit to the RBF having max. act. */
	    
	    RBF_ERROR_CHECK( kr_setCurrUnit ( correct_output_unit_no ) );
	    RBF_ERROR_CHECK( krui_createLink ( new_rbf_no, 1.0 ) );
	    
	    /*  sort units by topology and by topologic type  */
	    RBF_ERROR_CHECK( kr_topoSort ( TOPOLOGICAL_FF ) );
	    
	    /* Shrink new RBF */
	    
	    /* Scan all competing RBFs */
	    
	    new_rbf_ptr = kr_getUnitPtr ( new_rbf_no );
	    topo_ptr = topo_ptr_array + 2 + NoOfInputUnits + NoOfHiddenUnits;
	    
	    while ( (output_unit_ptr = * ++topo_ptr ) != NULL)
	      /* Only competing RBFs! */
	      if ( output_unit_ptr != correct_output_unit_ptr )
		FOR_ALL_LINKS( output_unit_ptr, link_ptr )
		  {
		    float sqr_distance = 0.0;
		    struct Link *link_ptr1, *link_ptr2;
		    
		    /* Compute Euclidian distance of the center of the new RBF 
		       to the center of a competing RBFs */
		    
		    link_ptr1 = (struct Link *) new_rbf_ptr->sites;
		    link_ptr2 = (struct Link *) link_ptr->to->sites;
		    
		    while ( link_ptr1 != NULL)
		      {
			float diff;
			
			/* Find corresponding link so that link_ptr1 and 
			   link_ptr2 start at the same unit. */
			
			if ( link_ptr1->to != link_ptr2->to )
			  {
			    link_ptr1 = (struct Link *) new_rbf_ptr->sites;		      
			    while ( link_ptr1->to != link_ptr2->to )
			      if ( link_ptr1 != NULL )
				link_ptr1 = link_ptr1->next;
			      else
				return KRERR_NP_DOES_NOT_FIT; 
			  }
			
			/* Compute distance of the two centers */
			
			diff = ( link_ptr1->weight - link_ptr2->weight );
			sqr_distance += diff*diff;
			if ( link_ptr1->to != link_ptr2->to ) 
			  return DDA_CONN_POINTER;
			
			link_ptr1 = link_ptr1->next;
			link_ptr2 = link_ptr2->next;
		      }
		    
		    /* Activation greater than theta_neg? Sigma too big ? */
		    
		    if ( RBF_INV/new_rbf_ptr->bias > sqr_distance )
		      /* Shrink! */
		      if ( new_rbf_ptr->bias < 1.0/RBF_SQR( RBF_MIN_SIGMA ) )
	  	        {
			  if ( sqr_distance/RBF_INV > RBF_SQR( RBF_MIN_SIGMA ) )
			    new_rbf_ptr->bias = RBF_INV / sqr_distance;
			  else 
			    new_rbf_ptr->bias = 1.0/RBF_SQR( RBF_MIN_SIGMA );
		        }
#ifdef RBF_DEBUG
		      else
		        fprintf( stderr, "\nRBF-DDA WARNING: Sigma too small (new)!\n" );
#endif
		  }
	    
	  } /* end else not covered/new rbf */
      
    } /* end while kr_getSubPatternByOrder (Main Loop) */
  
  
  /* Set positions of units on display */
  {
    int i = 0;
    
    /* Direction of data flow: From left to right */
    if ( xmax_in < xmin_out )
      {
	topo_ptr = topo_ptr_array + 1 + NoOfInputUnits;

	while ( (hidden_unit_ptr = * ++topo_ptr ) != NULL)
	  {
	    hidden_unit_ptr->unit_pos.x = xmax_in + 4 + i / max_units_displayed;
	    hidden_unit_ptr->unit_pos.y = RBF_MIN ( ymin_in, ymin_out) + 
	      i % max_units_displayed;
	    i++;
	  } 

	while ( (output_unit_ptr = * ++topo_ptr ) != NULL)
	  output_unit_ptr->unit_pos.x += 
	    xmax_in + 8 + (NoOfHiddenUnits - 1) / max_units_displayed - xmin_out;
      }

    /* right to left */
    else if ( xmin_in > xmax_out &&  !( ymax_in < ymin_out ) )
      {
	topo_ptr = topo_ptr_array;

	while ( (input_unit_ptr = * ++topo_ptr ) != NULL)
	  input_unit_ptr->unit_pos.x +=  
	    xmax_out + 8 + (NoOfHiddenUnits - 1) / max_units_displayed - xmin_in ;

	while ( (hidden_unit_ptr = * ++topo_ptr ) != NULL)
	  {
	    hidden_unit_ptr->unit_pos.x = xmax_out + 4 + 
	      ( NoOfHiddenUnits - 1 - i ) / max_units_displayed;
	    hidden_unit_ptr->unit_pos.y =  RBF_MIN ( ymin_in, ymin_out) + 
	      i % max_units_displayed;
	    i++;
	  } 
      }

    /* downwards to upwards */
    else if ( ymin_in > ymax_out )
      {
	topo_ptr = topo_ptr_array;

	while ( (input_unit_ptr = * ++topo_ptr ) != NULL)
	  input_unit_ptr->unit_pos.y += 
	    ymax_out + 8 + (NoOfHiddenUnits - 1) / max_units_displayed - ymin_in;

	while ( (hidden_unit_ptr = * ++topo_ptr ) != NULL)
	  {
	    hidden_unit_ptr->unit_pos.x =  RBF_MIN ( xmin_in, xmin_out) + 
	      i % max_units_displayed;
	    hidden_unit_ptr->unit_pos.y = ymax_out + 4 + 
	      ( NoOfHiddenUnits - 1 - i ) / max_units_displayed;
	    i++;
	  } 
      }

    /* Default case: upwards to downwards */
    else
      {
	topo_ptr = topo_ptr_array + 1 + NoOfInputUnits;

	while ( (hidden_unit_ptr = * ++topo_ptr ) != NULL)
	  {
	    hidden_unit_ptr->unit_pos.x =  RBF_MIN ( xmin_in, xmin_out) + 
	      i % max_units_displayed;
	    hidden_unit_ptr->unit_pos.y = ymax_in + 4 + i / max_units_displayed;
	    i++;
	  } 

	while ( (output_unit_ptr = * ++topo_ptr ) != NULL)
	  output_unit_ptr->unit_pos.y += 
	    ymax_in + 8 + (NoOfHiddenUnits - 1) / max_units_displayed - ymin_out;
      }

  } /* end Set positions of units ... */


  /* Compute Error == # misclassified patterns */

  *NoOfOutParams = 1; /*  One return value: OutParameter[0] == learning error */
  *parameterOutArray = OutParameter;  /*  set the output parameter reference  */
    
  /* reset network error value  */
  OutParameter[0] = 0.0;  
    
  /* compute the necessary sub patterns */
  RBF_ERROR_CHECK( kr_initSubPatternOrder ( start_pattern, end_pattern ) );
    
  while( kr_getSubPatternByOrder ( &pattern_no, &sub_pat_no ) )
    {
      /* forward propagation */
      RbfLearnForward ( pattern_no, sub_pat_no );

      topo_ptr = topo_ptr_array + 2 + NoOfInputUnits  + NoOfHiddenUnits;

      while( (output_unit_ptr = * ++topo_ptr) != NULL )
	OutParameter[0] +=  RBF_SQR( output_unit_ptr->value_a );
    }
  
  return ( KRERR_NO_ERROR );
}


/*****************************************************************************
 *****************************************************************************


 GROUP        : RPROP learning function, V1.1

  AUTHOR       : Martin Riedmiller, ILKD, University of Karlsruhe
  Notes        : RPROP parameters are the initial update value (default 0.1) 
                 and the maximal update value (default 50.0). The defaults 
		 are assumed if the parameters are set to 0.0. It may be 
		 helpfull to limit the second paream to 0.01.

		V1.1 supports weight decay (third parameter)

		V1.0 in SNNSv3.0 according to descritption in IEEE ICNN '93
		V1.1 in SNNSv3.3 according to technical report (Riedmiller 1994)
	       		  -no backtracking in case of jump over minimum
		        new features:
        	          -weight decay included
	       		  -'weights-fixed' removed

*******************************************************************************
******************************************************************************/

#define RPROP_ETAPLUS 1.2
#define RPROP_ETAMINUS 0.5
#define RPROP_MINEPS 1e-6
#define RPROP_MAXEPS 2.0
#define RPROP_DEFAULT_UPDATE_VALUE 0.001
#define SUM_SQUARE_ERROR          0
#define CROSS_ENTROPY_ERROR       1
#define MULTIPLE_CROSS_ERROR      2


/*****************************************************************************
  FUNCTION : initializeRprop

  PURPOSE  : Rprop initialisation:
  RETURNS  : kernel error code
  NOTES    : ->value_c : Sum (dEdw)
             ->value_b : dw(t-1)
             ->value_a : update_value

  UPDATE   : 09.05.1994 by Guenter Mamier
******************************************************************************/
static krui_err initializeRprop(float update_val)
{
    register unsigned short flags;
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;

  FOR_ALL_UNITS( unit_ptr ){
	flags = unit_ptr->flags;

      if ( (flags & UFLAG_IN_USE) == UFLAG_IN_USE){ /*  unit is in use  */
	    unit_ptr->value_b = unit_ptr->value_c = (FlintType) 0;
	  unit_ptr->value_a = (FlintType)update_val;

	  if (flags & UFLAG_SITES){ /*  unit has sites  */
	      FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
		    link_ptr->value_b = link_ptr->value_c = (FlintType) 0;
		  link_ptr->value_a = (FlintType)update_val;
		}

	  } else{		/*  unit has no sites   */
		if (flags & UFLAG_DLINKS) {	/* unit has direct links   */
		  FOR_ALL_LINKS( unit_ptr, link_ptr ){
			link_ptr->value_b = link_ptr->value_c = (FlintType) 0;
		      link_ptr->value_a = (FlintType)update_val;
		    }
		}
	    }
	}
    }

  return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : computeDevite

  PURPOSE  : one function to compute the error
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
static int computeDevite(float *devit, float *sum_error,
                         float target, float output, int errorType )
{
    int ret = 0;
    float sum1,sum2,sum3,sum4;


    switch(errorType){
    
    case MULTIPLE_CROSS_ERROR:
	(*devit) = output-target;
	if (output > 0.0)
	    sum1 = target * log (output);
	else
	    sum1 = 0.0;
	if (target > 0.0)
	    sum2 = target * log (target);
	else
	    sum2 = 0.0;
	*sum_error -= (sum1 - sum2);
	break;

    case CROSS_ENTROPY_ERROR:
	(*devit) = target-output;
	if (output > 0.0)
	    sum1 = target * log (output);
	else
	    sum1 = 0.0;
	if (target > 0.0)
	    sum2 = target * log (target);
	else
	    sum2 = 0.0;
	if ( (1 - output) > 0.0)
	    sum3 = (1 - target) * log (1 - output);
	else
	    sum3 = 0.0;
	if ((1 - target) > 0.0)
	    sum4 = ( 1- target) * log ( 1 - target);
	else
	    sum4 = 0.0;
    
	*sum_error -= (sum1 - sum2 + sum3 - sum4);    
	break;
    
    case SUM_SQUARE_ERROR:
    default:
	(*devit) = target-output;
	*sum_error += (*devit) * (*devit);
	break;
    }
    return ret;
}



/*****************************************************************************
  FUNCTION : computeAlpha

  PURPOSE  : one function to compute the alpha value
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
static float computeAlpha(void)
{
    int i=0,s,t;
    float Alpha = 0.0, sum = 0.0,weightVal;

    for( s = krui_getFirstUnit(); s != 0; s = krui_getNextUnit() ) {
	if (krui_getUnitTType( s ) != INPUT){
	    weightVal = krui_getUnitBias(s);
	    weightVal = krui_getUnitBias(s);
	    sum += weightVal * weightVal;     /* add biases */
	    i++;
	}
      
	for( t=krui_getFirstSuccUnit(s,&weightVal); t != 0; 
	     t = krui_getNextSuccUnit(&weightVal) ){
	    i++;
	    sum += weightVal * weightVal;     /* add weights */
        }
          
	/* getNextUnit will get the succ of the current Unit */
	krui_setCurrentUnit( s ); 
    }
    if (sum > 0.0)
	Alpha = i / sum;
    return Alpha;
}



/*****************************************************************************
  FUNCTION : propagateNetForwardRprop

  PURPOSE  : forward pass for Rprop with different error functions
  RETURNS  : 
  NOTES    : topological forward propagation

  UPDATE   : 
******************************************************************************/
static void propagateNetForwardMAP(int pattern_no, int sub_pat_no, 
				   int errorType)
{
    register struct Unit *unit_ptr;
    register Patterns in_pat;
    register TopoPtrArray topo_ptr;
    float sum_act = 0.0;

    /* calculate startaddress for input pattern array  */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);
    if(in_pat == NULL){
        KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
        return;
    }

    topo_ptr = topo_ptr_array;

    /* copy pattern into input unit's activation and calculate output of the
       input units */
    while ((unit_ptr = *++topo_ptr) != NULL) {  
        
        /* topo_ptr points to a (topological sorted) unit
           stucture (input unit first)  */
        if (unit_ptr->out_func == OUT_IDENTITY)
            /* identity output function: there is no need to call the output
               function  */
            unit_ptr->Out.output = unit_ptr->act = *in_pat++;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act = *in_pat++);
        else
            /* no identity output function: calculate unit's output also  */
            unit_ptr->Out.output 
                = (*unit_ptr->out_func) (unit_ptr->act = *in_pat++);
    }

    /* popagate hidden units  */
    while ((unit_ptr = *++topo_ptr) != NULL) {  /* topo_ptr points to a
                                                   (topological sorted) unit
                                                   stucture */
        /* clear error values  */
        unit_ptr->Aux.flint_no = 0.0;

        /* calculate the activation value of the unit: call the activation
           function if needed  */
        unit_ptr->act = ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(unit_ptr->act_func) (unit_ptr)) ;

        if (unit_ptr->out_func == OUT_IDENTITY)
            /* identity output function: there is no need to call the output
               function  */
            unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
        else
            /* no identity output function: calculate unit's output also  */
            unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    /* popagate output units  */
    while ((unit_ptr = *++topo_ptr) != NULL) {  /* topo_ptr points to a
                                                   (topological sorted) unit
                                                   stucture */
        /* clear error values  */
        unit_ptr->Aux.flint_no = 0.0;

        /* calculate the activation value of the unit: call the activation
           function if needed  */
        unit_ptr->act = ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(unit_ptr->act_func) (unit_ptr)) ;
        sum_act += unit_ptr->act;
        if (unit_ptr->out_func == OUT_IDENTITY)
            /* identity output function: there is no need to call the output
               function  */
            unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
        else
            /* no identity output function: calculate unit's output also  */
            unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }
    if (errorType == MULTIPLE_CROSS_ERROR){
      /* normalize activations of output units  */
      while ((unit_ptr = *--topo_ptr) != NULL) {        /* topo_ptr points to a
                                                   (topological sorted) unit
                                                   stucture */
        /* normalize activation */
        if (sum_act > 0.0)
          unit_ptr->act /= sum_act; /* / (sum_act + unit_ptr->act);*/
        
        if (unit_ptr->out_func == OUT_IDENTITY)
          /* identity output function: there is no need to call the output
             function  */
          unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
        else
          /* no identity output function: calculate unit's output also  */
          unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
      }
    }
}


/*****************************************************************************
  FUNCTION : propagateNetBackwardRprop

  PURPOSE  : Pure Backpropagation of gradient without weight-update
  RETURNS  : network error
  NOTES    : sum(dE/dw) -> value_c.

  UPDATE   : 09.05.1994 by Guenter Mamier
******************************************************************************/
static float propagateNetBackwardRprop(int pattern_no, int sub_pat_no)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register float  error,	/* error  */
                    sum_error,	/* sum of the error  */
                    devit;	/* deviation  */
    TopoPtrArray    topo_ptr;
    int size;


    sum_error = 0.0;		/* reset network error  */

    /* calculate address of the output pattern (with number pattern_no + 1)  */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;

    /*  add 3 to no_of_topo_units because the topologic array contains
	4 NULL pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    while ((unit_ptr = *--topo_ptr) != NULL){
	devit = *(--out_pat) - unit_ptr->Out.output;
	/*= o * (1.0 - o) in [0.0,0.25],*/
	/*for asymmetric logistic function*/

	sum_error += devit * devit;	/* sum up the error of the network  */

	/* calc. error for output units	 */
	error = devit * (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) );

	unit_ptr->value_c += - error /* *1 */; /* calculate the bias slopes */
	/* learn bias like a weight  */
	if (UNIT_HAS_DIRECT_INPUTS( unit_ptr )){
	    /*  the unit has direct links  */
	    FOR_ALL_LINKS( unit_ptr, link_ptr ){
		/*	calculate the slopes  */
		link_ptr->value_c += - error * link_ptr->to->Out.output;
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
	    }
	}else{	
	    /*  the unit has sites  */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
		/* calculate the value_cs  */
		link_ptr->value_c += - error * link_ptr->to->Out.output;
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
	    }
	}
    }


    /* calculate hidden units only  */
    while ((unit_ptr = *--topo_ptr) != NULL){
	error = (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) ) * unit_ptr->Aux.flint_no;

	unit_ptr->value_c += - error /* * 1 */; /* calculate the bias slopes  */
	/* learn bias like a weight  */
	if (UNIT_HAS_DIRECT_INPUTS( unit_ptr )){ /* the unit has direct links */
	    FOR_ALL_LINKS( unit_ptr, link_ptr ){ /* calculate the slopes  */
		if (link_ptr->to->flags & UFLAG_TTYP_HIDD)
		    /*  this link points to a hidden unit: 
			sum up the error's from previos units  */
		    link_ptr->to->Aux.flint_no += link_ptr->weight * error;

		link_ptr->value_c += - error * link_ptr->to->Out.output;
	    }
	}else {		/*  the unit has sites  */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
		/* calculate the slopes  */
		if (link_ptr->to->flags & UFLAG_TTYP_HIDD)
		    /*  this link points to a hidden unit: 
			sum up the error's from previos units  */
		    link_ptr->to->Aux.flint_no += link_ptr->weight * error;

		link_ptr->value_c += - error * link_ptr->to->Out.output;
	    }
	}
    }
    return( sum_error );	/*  return the error of the network  */
}


/*****************************************************************************
  FUNCTION : propagateNetBackwardMAP

  PURPOSE  : Backward phase for RPROP with weight decay
  RETURNS  : network error
  NOTES    : sum(dE/dw) -> value_c.

  UPDATE   : 09.05.1994 by Guenter Mamier
******************************************************************************/
static float propagateNetBackwardMAP(int pattern_no, int sub_pat_no, 
				     int errorType)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    float  error,       /* error  */
                    sum_error,  /* sum of the error  */
                    devit;      /* deviation  */
    TopoPtrArray    topo_ptr;
    int size;


    sum_error = 0.0;            /* reset network error  */

    /* calculate address of the output pattern (with number pattern_no + 1)  */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;

    /*  add 3 to no_of_topo_units because the topologic array contains
        4 NULL pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    while ((unit_ptr = *--topo_ptr) != NULL){
        computeDevite(&devit,&sum_error,*(--out_pat), unit_ptr->Out.output, 
		      errorType);

        /* calc. error for output units  */
        error = devit;
        if (errorType == SUM_SQUARE_ERROR)
          error *= (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) );

        
        unit_ptr->value_c += - error /* *1 */; /* calculate the bias slopes */
        /* learn bias like a weight  */
        if (UNIT_HAS_DIRECT_INPUTS( unit_ptr )){
            /*  the unit has direct links  */
            FOR_ALL_LINKS( unit_ptr, link_ptr ){
                /*      calculate the slopes  */
                link_ptr->value_c += - error * link_ptr->to->Out.output;
                link_ptr->to->Aux.flint_no += link_ptr->weight * error;
            }
        }else{  
            /*  the unit has sites  */
            FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
                /* calculate the value_cs  */
                link_ptr->value_c += - error * link_ptr->to->Out.output;
                link_ptr->to->Aux.flint_no += link_ptr->weight * error;
            }
        }
    }


    /* calculate hidden units only  */
    while ((unit_ptr = *--topo_ptr) != NULL){
        error = (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) ) * unit_ptr->Aux.flint_no;

        unit_ptr->value_c += - error /* * 1 */; /* calculate the bias slopes  */
        /* learn bias like a weight  */
        if (UNIT_HAS_DIRECT_INPUTS( unit_ptr )){ /* the unit has direct links */
            FOR_ALL_LINKS( unit_ptr, link_ptr ){ /* calculate the slopes  */
                if (link_ptr->to->flags & UFLAG_TTYP_HIDD)
                    /*  this link points to a hidden unit: 
                        sum up the error's from previos units  */
                    link_ptr->to->Aux.flint_no += link_ptr->weight * error;

                link_ptr->value_c += - error * link_ptr->to->Out.output;
            }
        }else {         /*  the unit has sites  */
            FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
                /* calculate the slopes  */
                if (link_ptr->to->flags & UFLAG_TTYP_HIDD)
                    /*  this link points to a hidden unit: 
                        sum up the error's from previos units  */
                    link_ptr->to->Aux.flint_no += link_ptr->weight * error;

                link_ptr->value_c += - error * link_ptr->to->Out.output;
            }
        }
    }
    return( sum_error );        /*  return the error of the network  */
}



/*****************************************************************************
  FUNCTION : MODI_rprop

  PURPOSE  : modifies network after each epoch
  RETURNS  : 
  NOTES    :

  UPDATE   : 09.05.1994 by Guenter Mamier
******************************************************************************/
static void MODI_rprop(float maxeps, float weight_decay)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    TopoPtrArray    topo_ptr;
    bool            hidden_units;
    float           direction;


    topo_ptr = topo_ptr_array + (NoOfInputUnits + 1);
    hidden_units = TRUE;

    /* calculate hidden and output units only  */
    do {
	if ((unit_ptr = *++topo_ptr) == NULL) {
	  if (!hidden_units) break; /* end of topologic pointer array reached */
	    unit_ptr = *++topo_ptr;	/* skip NULL pointer  */
	    hidden_units = FALSE;
	}

      unit_ptr->value_c += weight_decay * unit_ptr->bias;
	direction = unit_ptr->value_b * unit_ptr->value_c;
      if (direction < 0.0){	/*  same direction : dw * dEdw < 0  */
	  unit_ptr->value_a *= RPROP_ETAPLUS;	/* adapt update_value*/
	  if (unit_ptr->value_a > maxeps) unit_ptr->value_a = maxeps; 
	    if (unit_ptr->value_c < 0.0)
		unit_ptr->value_b = unit_ptr->value_a;
	    else
	      unit_ptr->value_b = - (unit_ptr->value_a);
      }else
	  if (direction > 0.0){	/*  direction changed  */
	      unit_ptr->value_b = 0; /* reset for restarting adaptation 
					in next step */
	      unit_ptr->value_a *= RPROP_ETAMINUS; /* adapt update_value*/
	    if (unit_ptr->value_a < RPROP_MINEPS)
		unit_ptr->value_a = RPROP_MINEPS;
	} else {
	    /* start of RPROP learning process */
	    if (unit_ptr->value_c < 0.0)
		unit_ptr->value_b = unit_ptr->value_a;
	    else if (unit_ptr->value_c > 0.0)
		  unit_ptr->value_b = - (unit_ptr->value_a);

	    /* else no action if  derivative was zero */

	}

      if(!IS_SPECIAL_UNIT(unit_ptr))
	  unit_ptr->bias += unit_ptr->value_b; /* compute new bias*/
      unit_ptr->value_c = 0.0;	/* reset */

      /*adjust links*/
      if (UNIT_HAS_DIRECT_INPUTS( unit_ptr )) { /*  the unit has direct links */
	  FOR_ALL_LINKS( unit_ptr, link_ptr ){
	      link_ptr->value_c += weight_decay * link_ptr->weight;	
		direction = link_ptr->value_b * link_ptr->value_c;
		if (direction < 0.0) {	/* same direction : dw * dEdw < 0  */
		  link_ptr->value_a *= RPROP_ETAPLUS;	/* adapt update_value*/
		  if (link_ptr->value_a > maxeps) link_ptr->value_a = maxeps; 
		    if (link_ptr->value_c < 0.0)
			link_ptr->value_b = link_ptr->value_a;
		    else
		      link_ptr->value_b = - (link_ptr->value_a);
	      } else
		  if (direction > 0.0) { /*  direction changed  */
		      link_ptr->value_b = 0; /* reset for restarting adaptation
						in next step */
		      link_ptr->value_a *= RPROP_ETAMINUS;/*adapt update_value*/
		      if( link_ptr->value_a < RPROP_MINEPS)
			link_ptr->value_a = RPROP_MINEPS;
		} else {
		    /* start of RPROP learning process  */
		    if (link_ptr->value_c < 0.0)
			link_ptr->value_b = link_ptr->value_a;
		    else if (link_ptr->value_c > 0.0)
			  link_ptr->value_b = - (link_ptr->value_a);

		    /* else no action if  derivative was zero */

		}
	      if(!IS_SPECIAL_UNIT(unit_ptr))
		  link_ptr->weight += link_ptr->value_b;/* compute new weight*/
		link_ptr->value_c = 0.0;	        /* reset */

	    }
	} else {		/* the unit has sites  */
	  FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
	      link_ptr->value_c += weight_decay * link_ptr->weight;
	      direction = link_ptr->value_b * link_ptr->value_c;
	      if (direction < 0.0){ /*  same direction : dw * dEdw <! 0  */
		    link_ptr->value_a *= RPROP_ETAPLUS;	/* adapt update_value */
		  if (link_ptr->value_a > maxeps) link_ptr->value_a = maxeps; 
		    if (link_ptr->value_c < 0.0)
			link_ptr->value_b = link_ptr->value_a;
		    else
		      link_ptr->value_b = - (link_ptr->value_a);
	      } else
		  if (direction > 0.0) { /*  direction changed  */
		    link_ptr->value_b = 0;	/* reset for restarting
						   adaptation in next step */
		      link_ptr->value_a *= RPROP_ETAMINUS;/*adapt update_value*/
		      if(link_ptr->value_a < RPROP_MINEPS)
			link_ptr->value_a = RPROP_MINEPS;
		  }else {
		    /* start of RPROP learning process */
		    if (link_ptr->value_c < 0.0)
			link_ptr->value_b = link_ptr->value_a;
		    else if (link_ptr->value_c > 0.0)
			  link_ptr->value_b = - (link_ptr->value_a);

		    /* else no action if  derivative was zero */

		}

	      if(!IS_SPECIAL_UNIT(unit_ptr))
		  link_ptr->weight += link_ptr->value_b; /*compute new weight*/
		link_ptr->value_c = 0.0;	/* reset */
	    }
	}
    }				/* for units  */
  while( TRUE );

}


/*****************************************************************************
  FUNCTION : LEARN_rprop

  PURPOSE  : RPROP learning function
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : initial update value
                                 2 : maxeps;
                                 3 : exponent weight decay

             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 09.05.1994 by Guenter Mamier
******************************************************************************/
krui_err LEARN_rprop(int start_pattern, int end_pattern, 
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams)
{
    static float OutParameter[1]; /*OutParameter[0] stores the*/
    /*learning error*/
    int    pattern_no, sub_pat_no, ret_code;
    float  maxeps, update_value, wd;


    if (NoOfUnits == 0)
	return( KRERR_NO_UNITS ); /*  No Units defined  */

    if (NoOfInParams < 3)
	return( KRERR_PARAMETERS ); /*  not enough input parameters  */

    /* DEFAULTS: */

    if (( update_value = LEARN_PARAM1( parameterInArray )) == 0.0) 
	update_value = RPROP_DEFAULT_UPDATE_VALUE;
    if ((maxeps = LEARN_PARAM2( parameterInArray )) == 0.0) 
	maxeps = RPROP_MAXEPS;
    if (update_value > maxeps) update_value = maxeps;

    wd = LEARN_PARAM3( parameterInArray );
    if (wd != 0.0)
	wd = (float) pow(10.0, (double)(- wd));




    *NoOfOutParams = 1;		/* one return value */

    *parameterOutArray = OutParameter;	/* set output parameter reference  */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)){
	/* Net has been modified  or topologic array isn't initialized */
	/* check the topology of the network  */
	ret_code = kr_topoCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return( ret_code ); /*  an error has occured  */
	if (ret_code < 2)
	    return( KRERR_NET_DEPTH ); /*the network has less than 2 layers  */

	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)  return( ret_code );

	/* sort units by topology and by topologic type  */
	ret_code = kr_topoSort( TOPOLOGICAL_FF );
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return( ret_code );

    }

    if (NetModified || NetInitialize || LearnFuncHasChanged){
	/*  Net has been modified or initialized, initialize RPROP */
	ret_code = initializeRprop(update_value);
	if (ret_code != KRERR_NO_ERROR)  return( ret_code );
    }
    NetModified = FALSE;

    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){
	propagateNetForward(pattern_no,sub_pat_no);   /* forward propagation  */

	/* backward propagation and summation of gradient  */
	NET_ERROR(OutParameter) 
	    += propagateNetBackwardRprop(pattern_no,sub_pat_no);
    }

    /*  modify links and bias  */
    MODI_rprop(maxeps, wd); 
    return( ret_code );
}

/*****************************************************************************
  FUNCTION : LEARN_RpropMAP

  PURPOSE  : RPROP learning function with adaptive weight decay
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : maxeps

             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 
******************************************************************************/
krui_err LEARN_RpropMAP(int start_pattern, int end_pattern, 
                     float *parameterInArray, int NoOfInParams, 
                     float **parameterOutArray, int *NoOfOutParams)
{
    static float OutParameter[1]; /*OutParameter[0] stores the*/
    /*learning error*/
    static int counter=0;
    int    pattern_no, sub_pat_no, ret_code,errorType=0,updateEpoch=20;
    float  maxeps, update_value;
    float  alpha, beta, lambda; 
    
    
    if (NoOfUnits == 0)
        return( KRERR_NO_UNITS ); /*  No Units defined  */

    if (NoOfInParams < 1)
        return( KRERR_PARAMETERS ); /*  not enough input parameters  */

    /* DEFAULTS: */

    if ((maxeps = LEARN_PARAM1( parameterInArray )) == 0.0) 
        maxeps = RPROP_MAXEPS;
    update_value = RPROP_DEFAULT_UPDATE_VALUE;
    if (update_value > maxeps) update_value = maxeps;

    
    if (( update_value = LEARN_PARAM1( parameterInArray )) == 0.0) 
        update_value = RPROP_DEFAULT_UPDATE_VALUE;
    if ((maxeps = LEARN_PARAM2( parameterInArray )) == 0.0) 
        maxeps = RPROP_MAXEPS;
    if (update_value > maxeps) update_value = maxeps;
    if (!(( lambda = LEARN_PARAM3( parameterInArray )) == 0.0))
        lambda = (float) pow(10,(double)(- lambda));

    updateEpoch = (int) LEARN_PARAM4( parameterInArray );
    errorType = (int) LEARN_PARAM5( parameterInArray );

    *NoOfOutParams = 1;         /* one return value */

    *parameterOutArray = OutParameter;  /* set output parameter reference  */
    ret_code = KRERR_NO_ERROR;  /* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)){
        /* Net has been modified  or topologic array isn't initialized */
        /* check the topology of the network  */
        ret_code = kr_topoCheck();
        if (ret_code < KRERR_NO_ERROR)
            return( ret_code ); /*  an error has occured  */
        if (ret_code < 2)
            return( KRERR_NET_DEPTH ); /*the network has less than 2 layers  */

        /* count the no. of I/O units and check the patterns  */
        ret_code = kr_IOCheck();
        if (ret_code < KRERR_NO_ERROR)  return( ret_code );

        /* sort units by topology and by topologic type  */
        ret_code = kr_topoSort( TOPOLOGICAL_FF );
        if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
            return( ret_code );
        counter = 0;
    }

    if (NetModified || NetInitialize || LearnFuncHasChanged){
        /*  Net has been modified or initialized, initialize RPROP */
        ret_code = initializeRprop(update_value);
        if (ret_code != KRERR_NO_ERROR)  return( ret_code );
        counter = 0;
    }
    NetModified = FALSE;

    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
        return (KernelErrorCode);

    NET_ERROR(OutParameter) = 0.0;      /* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){
        propagateNetForwardMAP(pattern_no,sub_pat_no,errorType);  

        /* backward propagation and summation of gradient  */
        NET_ERROR(OutParameter) 
            += propagateNetBackwardMAP(pattern_no,sub_pat_no, errorType);
    }
    counter++;
    if (updateEpoch && (counter % updateEpoch == 0) ){
      /* compute bayes hyperparameter */
      alpha = computeAlpha();
      beta  = krui_getNoOfPatterns() / NET_ERROR(OutParameter);
      if (beta ==  0.0)
         lambda = 0.0; /* should never happen */
        else
          lambda = alpha / beta;

      fprintf(stderr,"Epoch %d, beta:  %.4f alpha: %.4f lambda: %.4f \n",
              counter, beta, alpha, lambda);
    }
    /*  modify links and bias  */
    MODI_rprop(maxeps, lambda); 
    return( ret_code );
}



/*****************************************************************************
  FUNCTION : TEST_rprop

  PURPOSE  : RPROP testing function
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : initial update value
                                 2 : maxeps;
                                 3 : exponent weight decay

             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 03.03.95 Joachim Danz
******************************************************************************/
krui_err TEST_rprop(int start_pattern, int end_pattern, 
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams)
{
    static float OutParameter[1]; /*OutParameter[0] stores the*/
    /*learning error*/
    int    pattern_no, sub_pat_no, ret_code;

    if (NoOfUnits == 0)
	return( KRERR_NO_UNITS ); /*  No Units defined  */

    if (NoOfInParams < 3)
	return( KRERR_PARAMETERS ); /*  not enough input parameters  */



    *NoOfOutParams = 1;		/* one return value */

    *parameterOutArray = OutParameter;	/* set output parameter reference  */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)){
	/* Net has been modified  or topologic array isn't initialized */
	/* check the topology of the network  */
	ret_code = kr_topoCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return( ret_code ); /*  an error has occured  */
	if (ret_code < 2)
	    return( KRERR_NET_DEPTH ); /*the network has less than 2 layers  */

	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)  return( ret_code );

	/* sort units by topology and by topologic type  */
	ret_code = kr_topoSort( TOPOLOGICAL_FF );
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return( ret_code );

	NetModified = FALSE;
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){
	propagateNetForward(pattern_no,sub_pat_no);   /* forward propagation  */

	/* backward propagation and summation of gradient  */
	NET_ERROR(OutParameter) 
	    += testNetBackwardRprop(pattern_no,sub_pat_no);
    }

    return( ret_code );
}


/*****************************************************************************
  FUNCTION : testNetBackwardRprop

  PURPOSE  : Calculation of Error
  RETURNS  : network error
  NOTES    : sum(dE/dw) -> value_c.

  UPDATE   : 03.03.1995 by Joachim Danz
******************************************************************************/
static float testNetBackwardRprop(int pattern_no, int sub_pat_no)
{
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register float  error,	/* error  */
                    sum_error,	/* sum of the error  */
                    devit;	/* deviation  */
    TopoPtrArray    topo_ptr;
    int size;


    sum_error = 0.0;		/* reset network error  */

    /* calculate address of the output pattern (with number pattern_no + 1)  */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;

    /*  add 3 to no_of_topo_units because the topologic array contains
	4 NULL pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    while ((unit_ptr = *--topo_ptr) != NULL){
	devit = *(--out_pat) - unit_ptr->Out.output;
	/*= o * (1.0 - o) in [0.0,0.25],*/
	/*for asymmetric logistic function*/

	sum_error += devit * devit;	/* sum up the error of the network  */

	/* calc. error for output units	 */
	error = devit * (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) );

    }

    return( sum_error );	/*  return the error of the network  */
}


/*****************************************************************************
  FUNCTION : testNetBackwardMAP

  PURPOSE  : Calculation of Error
  RETURNS  : network error
  NOTES    : sum(dE/dw) -> value_c.

  UPDATE   : 03.03.1997 by Thomas Ragg
******************************************************************************/
static float testNetBackwardMAP(int pattern_no, int sub_pat_no, int errorType)
{
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    float           sum_error,  /* sum of the error  */
                    devit;      /* deviation  */
    TopoPtrArray    topo_ptr;
    int size;


    sum_error = 0.0;            /* reset network error  */

    /* calculate address of the output pattern (with number pattern_no + 1)  */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;

    /*  add 3 to no_of_topo_units because the topologic array contains
        4 NULL pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* calculate output units only  */
    while ((unit_ptr = *--topo_ptr) != NULL){
	computeDevite(&devit, &sum_error, *(--out_pat), unit_ptr->Out.output,
		      errorType);
    }
    return( sum_error );        /*  return the error of the network  */
}




/*****************************************************************************
  FUNCTION : TEST_MAP

  PURPOSE  : RPROP_MAP testing function
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : initial update value
                                 2 : maxeps;
                                 3 : exponent weight decay
                                 4 : #epochs update hyperparameter
                                 5 : error function type

             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 03.10.97 Thomas Ragg
******************************************************************************/
krui_err TEST_MAP(int start_pattern, int end_pattern, 
                     float *parameterInArray, int NoOfInParams, 
                     float **parameterOutArray, int *NoOfOutParams)
{
    static float OutParameter[1]; /*OutParameter[0] stores the*/
    /*learning error*/
    int    pattern_no, sub_pat_no, ret_code;
    int errorType;

    if (NoOfUnits == 0)
        return( KRERR_NO_UNITS ); /*  No Units defined  */

    if (NoOfInParams < 3)
        return( KRERR_PARAMETERS ); /*  not enough input parameters  */

    errorType = (int) LEARN_PARAM5( parameterInArray );
    
    *NoOfOutParams = 1;         /* one return value */

    *parameterOutArray = OutParameter;  /* set output parameter reference  */
    ret_code = KRERR_NO_ERROR;  /* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)){
        /* Net has been modified  or topologic array isn't initialized */
        /* check the topology of the network  */
        ret_code = kr_topoCheck();
        if (ret_code < KRERR_NO_ERROR)
            return( ret_code ); /*  an error has occured  */
        if (ret_code < 2)
            return( KRERR_NET_DEPTH ); /*the network has less than 2 layers  */

        /* count the no. of I/O units and check the patterns  */
        ret_code = kr_IOCheck();
        if (ret_code < KRERR_NO_ERROR)  return( ret_code );

        /* sort units by topology and by topologic type  */
        ret_code = kr_topoSort( TOPOLOGICAL_FF );
        if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
            return( ret_code );

        NetModified = FALSE;
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
        return (KernelErrorCode);

    NET_ERROR(OutParameter) = 0.0;      /* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){
        propagateNetForwardMAP(pattern_no,sub_pat_no,errorType);

        /* backward propagation and summation of gradient  */
        NET_ERROR(OutParameter) 
          += testNetBackwardMAP(pattern_no,sub_pat_no,errorType);
    }

    return( ret_code );
}




/*****************************************************************************
 *****************************************************************************

  GROUP        : ART 1 learning function

  AUTHOR       : Kai-Uwe Herrmann
             
******************************************************************************
******************************************************************************/



/*****************************************************************************
  FUNCTION : LEARN_ART1

  PURPOSE  : ART 1 learning function.
  RETURNS  : kernel error code
  NOTES    : 1 input-parameter  :  1. vigilance parameter RHO

             output-parameters  :  numbers of classified patterns,
                                   separator -1,
                                   numbers of not classifiable patterns

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err  LEARN_ART1(int start_pattern, int end_pattern,
		     float parameterInArray[], int NoOfInParams,
		     float **parameterOutArray, int *NoOfOutParams)
{
    krui_err        ret_code = KRERR_NO_ERROR;
    int             pattern_no, sub_pat_no;	/* Contains actual */
						/* pattern number */
    int             start, end;
    int             i,n;
    struct Unit    *winner_ptr;	/* recognition unit which is the winner of
				   w.t.a  */
    TopoPtrArray    topo_layer[6];	/* topo_layer[0] : *first input unit
					   topo_layer[1] : *first comp. unit
					   topo_layer[2] : *first rec.  unit
					   topo_layer[3] : *first delay unit
					   topo_layer[4] : *first local reset
					   unit topo_layer[5] : *first
					   special unit (classified_unit) */
    TopoPtrArray    topo_ptr;
    FlintType       beta;
    float           rho;


    /* Check number of incoming parameters */

    if (NoOfInParams < 1) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }				/* if */
    /* rho is the vigilance parameter   */
    rho = parameterInArray[0];


    /* Check interval for vigilance parameter and constant value L */

    if ((rho < 0.0) || (rho > 1.0)) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }				/* if */
    /* Check if network has been modified or learning func has been changed */
    if (NetModified || LearnFuncHasChanged || (TopoSortID != ART1_TOPO_TYPE)) {
	(void) kr_topoSort(ART1_TOPO_TYPE);
	ret_code = KernelErrorCode;
	if (ret_code != KRERR_NO_ERROR) {
	    NetModified = TRUE;
	    return (ret_code);
	}			/* if */
	NetModified = FALSE;
	LearnFuncHasChanged = FALSE;
    }				/* if */
    /* set initial activation values */
    ret_code = kra1_init_i_act(rho);

    if (ret_code != KRERR_NO_ERROR) {
	return (ret_code);
    }				/* if */
    /* beta is another learning parameter of the network which is determined
       when initializing the network. It is there written to the bias field
       of the structure of each unit. Now we will read this value. */
    beta = (unit_array + 1)->bias;

    if (beta <= 0.0) {
	topo_msg.error_code = KRERR_PARAM_BETA;
	topo_msg.src_error_unit = 0;
	topo_msg.dest_error_unit = 1;
	return (topo_msg.error_code);
    }				/* if */
    /* # of output parameters is 0  */
    *NoOfOutParams = 0;
    *parameterOutArray = NULL;


    /* get pointers to first elements of each layer in topo_ptr_array */

    topo_ptr = topo_ptr_array + 1;

    for (i = 0; i <= 5; i++) {
	topo_layer[i] = topo_ptr;
	do {
	} while (*topo_ptr++ != NULL);

    }				/* for */


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    /* Search phase */
    start = kr_AbsPosOfFirstSubPat(start_pattern);
    end   = kr_AbsPosOfFirstSubPat(end_pattern);
    end  += kr_NoOfSubPatPairs(end_pattern) - 1;

    for(n=start; n<=end; n++){

	kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n);
    
	/* initialize the unit activations of the whole net */

	ret_code = krart_reset_activations();
	if (ret_code != KRERR_NO_ERROR) {
	    return (ret_code);
	}			/* if */
	/* put pattern into input units */
	ret_code = put_ART1_in_pattern(pattern_no, sub_pat_no, topo_layer[0]);
	if (ret_code != KRERR_NO_ERROR) {
	    return (ret_code);
	}			/* if */
	/* repeat synchronous propagation and look for winner until pattern
	   is classified or network tells us, that pattern is not
	   classifiable */
	do {
	    /* 1 propagation step (all units push their information onto
	       their output and calculate their new activation. */

	    krart_prop_synch();

	    /* look for the recognition unit with the highest activation
	       returns a NULL pointer if all recognition units have
	       activation 0.0 */
	    winner_ptr = krart_get_winner(topo_layer[2], 1.0);

	} while (!(ART1_CLASSIFIED) && !(ART1_NOT_CLASSIFIABLE));


	/* training phase */

	if (ART1_CLASSIFIED) {

	    /* Train network i.e. adjust weights between comparison layer and
	       winner_unit and vice versa */

	    ret_code = adjust_ART1_weights(beta, topo_layer[1],
					   topo_layer[3], winner_ptr);

	    if (ret_code != KRERR_NO_ERROR) {
		return (ret_code);
	    }/* if */
	}/* if */
    }/* for */


    return (ret_code);

}/* LEARN_ART1 */




/*****************************************************************************
  FUNCTION : put_ART1_in_pattern

  PURPOSE  : pushes a new pattern into the input units of the network
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err put_ART1_in_pattern(int pattern_no, int sub_pat_no, 
				    TopoPtrArray topo_inp_ptr)
{
    int             ret_code = KRERR_NO_ERROR;
    register Patterns in_pat;
    struct Unit    *unit_ptr;
    TopoPtrArray    topo_ptr = topo_inp_ptr;

    /* calculate startadress of actual pattern   */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);

    while ((unit_ptr = *topo_ptr++) != NULL) {
	if (unit_ptr->out_func == OUT_IDENTITY) {
	    unit_ptr->act = unit_ptr->Out.output = *in_pat++;
	} else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->act = *in_pat++;
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	} else {
	    unit_ptr->act = *in_pat++;
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	}/* if */
    }/* while */

    return (ret_code);

}/* put_ART1_in_pattern */



/*****************************************************************************
  FUNCTION : adjust_ART1_weights

  PURPOSE  : training function for ART1 networks
  RETURNS  : kernel error code
  NOTES    : Parameters:  
             beta         constant value beta > 0.0

             comp_ptr     points to pointer to first comparison unit

             delay_ptr    points to pointer to first unit in the delay layer. 
	                  The t(j,i) links are not between recognition layer 
			  and comparison layer but between the respective 
			  delay unit of the recogniton unit and the comparison 
			  layer. So first we have to look for the corresponding
			  delay unit of the winning unit before training these 
			  weights.

             winner_ptr   points to winning unit of the recognition layer.

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err adjust_ART1_weights(double beta, TopoPtrArray comp_ptr,
				    TopoPtrArray delay_ptr,
				    struct Unit * winner_ptr)
{
    krui_err        ret_code = KRERR_NO_ERROR;
    TopoPtrArray    topo_ptr = NULL;
    struct Unit    *unit_ptr_comp = NULL, *unit_ptr_delay = NULL;
    struct Link    *link_ptr = NULL;
    bool            found_delay_unit = FALSE;
    FlintType       sum_ck = 0.0;


    /* get corresponding unit of the winning unit in the delay layer */

    topo_ptr = delay_ptr;

    while ((!found_delay_unit) && (*topo_ptr != NULL)) {

	unit_ptr_delay = *topo_ptr++;

	if (((struct Link *) unit_ptr_delay->sites)->to == winner_ptr) {
	    found_delay_unit = TRUE;
	}/* if */
    }/* while */

    if (!found_delay_unit) {

	/* There was no delay unit found corresponding to the winning
	   recognition unit */

	ret_code = KRERR_TOPOLOGY;
	return (ret_code);

    }/* if */
    /* Adjust weights between winning unit (delay-layer) and comparison layer
       (t(j,i) link values)
    
    t(j,i) = c(i)   where j is the number of the winning neuron in the delay
       layer and i ist the number of a comparison unit. */
    topo_ptr = comp_ptr;

    while ((unit_ptr_comp = *topo_ptr++) != NULL) {

	sum_ck += unit_ptr_comp->act;	/* sum up activatons of comparison
					   layer. sum_ck is needed for b(i,j) */

	FOR_ALL_LINKS(unit_ptr_comp, link_ptr) {

	    if (link_ptr->to == unit_ptr_delay) {
		link_ptr->weight = ART1_ADJUST_LINK_DEL_CMP(unit_ptr_comp);
	    }/* if */
	}/* FOR_ALL_LINKS */

    }/* while */


    /* Adjust weights between comparison layer and winning unit (recognition
       layer) (b(i,j) link values)
    
       b(i,j) = c(i) / (beta + sum(k)(c(k)))
    
       where j is the number of the winning neuron in the recognition layer, i
       ist the number of a comparison unit and k runs over all comparison
       units. (sum(k)(c(k))) = sum_ck. 
    */


    FOR_ALL_LINKS(winner_ptr, link_ptr) {

	if (link_ptr->to->lln == ART1_CMP_LAY) {
	    link_ptr->weight = (FlintType)ART1_ADJUST_LINK_CMP_REC(link_ptr->to,
								   beta,sum_ck);
	}/* if */
    }/* FOR_ALL_LINKS */


    return (ret_code);
}/* adjust_ART1_weights () */


/*****************************************************************************
 *****************************************************************************

  GROUP        : ART2 learning function

  AUTHOR       : Kai-Uwe Herrmann

******************************************************************************
******************************************************************************/


/*****************************************************************************
  FUNCTION : LEARN_ART2

  PURPOSE  : ART2 learning function.
  RETURNS  : kernel error code
  NOTES    : Parameters: 
                    6 input-parameter  :  1. vigilance parameter RHO
                                          2. Parameter a
                                          3. Parameter b
                                          4. Parameter c
                                          5. Parameter e
                                          6. Parameter THETA

                    output-parameters  :  none


  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err LEARN_ART2(int start_pattern, int end_pattern,
		    float parameterInArray[], int NoOfInParams,
		    float **parameterOutArray, int *NoOfOutParams)
{
    krui_err        ret_code = KRERR_NO_ERROR;
    int             pattern_no, sub_pat_no; /* Contains actual pattern number */
    int             i,n;
    int             start, end;
    struct Unit    *winner_ptr;	/* recognition unit which is the winner of
				   w.t.a */
    TopoPtrArray    topo_layer[12];	/* topo_layer[0] : *first input unit
					   topo_layer[1] : *first w unit
					   topo_layer[2] : *first x unit
					   topo_layer[3] : *first u unit
					   topo_layer[4] : *first v unit
					   topo_layer[5] : *first p unit
					   topo_layer[6] : *first q unit
					   topo_layer[7] : *first r unit
					   topo_layer[8] : *first rec.  unit
					   topo_layer[10] : *first local
					   reset unit */
    TopoPtrArray    topo_ptr;
    FlintType       rho, param_a, param_b, param_c, param_d, theta;


    /* Check number of incoming parameters */

    if (NoOfInParams < 5) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }/* if */
    rho = parameterInArray[0];
    param_a = parameterInArray[1];
    param_b = parameterInArray[2];
    param_c = parameterInArray[3];
    theta = parameterInArray[4];


    /* Check if network has been modified or learning func has been changed */

    if (NetModified || LearnFuncHasChanged || (TopoSortID != ART2_TOPO_TYPE)) {
	(void) kr_topoSort(ART2_TOPO_TYPE);
	ret_code = KernelErrorCode;
	if (ret_code != KRERR_NO_ERROR) {
	    NetModified = TRUE;
	    return (ret_code);
	}/* if */
	NetModified = FALSE;
	LearnFuncHasChanged = FALSE;
    }/* if */
    /* Read out value of parameter d from bias field of any unit. The value
       has been written into the bias field by the init-function */
    param_d = (*(topo_ptr_array + 1))->bias;


    /* Check values of the parameters */

    if ((rho < 0.0) || (rho > 1.0) ||
	(param_a <= 0.0) || (param_b <= 0.0) ||
	((param_c * param_d) / (1 - param_d) > 1.0) ||
	(theta < 0.0) || (theta > 1.0)
	) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }/* if */
    ret_code = kra2_set_params(rho, param_a, param_b, param_c, param_d, theta);

    if (ret_code != KRERR_NO_ERROR) {
	return (ret_code);
    }/* if */
    ret_code = kra2_init_propagate();

    if (ret_code != KRERR_NO_ERROR) {
	return (ret_code);
    }/* if */
    /* get pointers to first elements of each layer in topo_ptr_array */
    topo_ptr = topo_ptr_array + 1;

    for (i = 0; i <= 9; i++) {
	topo_layer[i] = topo_ptr;
	do {
	} while (*topo_ptr++ != NULL);

    }/* for */


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    /* Search phase */
    start = kr_AbsPosOfFirstSubPat(start_pattern);
    end   = kr_AbsPosOfFirstSubPat(end_pattern);
    end  += kr_NoOfSubPatPairs(end_pattern) - 1;

    for(n=start; n<=end; n++){

	kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n);

	/* initialize the unit activations of the whole net */

	ret_code = krart_reset_activations();
	if (ret_code != KRERR_NO_ERROR) {
	    return (ret_code);
	}/* if */
	/* put pattern into input units */
	ret_code = put_ART2_in_pattern(pattern_no, sub_pat_no,
				       topo_layer[ART2_INP_LAY-1]);
	if (ret_code != KRERR_NO_ERROR) {
	    return (ret_code);
	}/* if */

	/* initialize of ART2 Simulator for new pattern */
	kra2_init_pattern();

	/* repeat synchronous propagation and look for winner until pattern
	   is classified or network tells us, that pattern is not
	   classifiable */

	do {

	    /* compute vector norms */
	    kra2_compute_norms();

	    /* save old activation values of f1-units */
	    kra2_save_for_stability_check();

	    /* 1 propagation step (all units push their information onto
	       their output and calculate their new activation. */
	    krart_prop_synch();

	    /* look for the recognition unit with the highest activation
	       returns a NULL pointer if all recognition units have
	       activation 0.0 */
	    winner_ptr = krart_get_winner(topo_layer[ART2_REC_LAY-1], param_d);

	    /* Check if F1-Layer is stable */
	    kra2_check_f1_stability();

	    /* Check Reset */
	    kra2_checkReset();

	} while (!(ART2_CLASSIFIED) && !(ART2_NOT_CLASSIFIABLE));


	/* training phase */

	if (ART2_CLASSIFIED) {

	    /* Train network i.e. adjust weights between comparison layer and
	       winner_unit and vice versa */
	    ret_code = adjust_ART2_weights(param_d, topo_layer[ART2_P_LAY - 1],
					   winner_ptr);

	    if (ret_code != KRERR_NO_ERROR) {
		return (ret_code);
	    }/* if */
	}/* if */
    }/* for */

    return (ret_code);

}/* LEARN_ART2 */



/*****************************************************************************
  FUNCTION : krui_err put_ART2_in_pattern

  PURPOSE  : pushes a new pattern into the input units of the network
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err put_ART2_in_pattern(int pattern_no, int sub_pat_no,
				    TopoPtrArray topo_inp_ptr)
{
    int               ret_code = KRERR_NO_ERROR;
    register Patterns in_pat;
    struct Unit       *unit_ptr;
    TopoPtrArray      topo_ptr = topo_inp_ptr;

    /* calculate startadress of actual pattern */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);

    while ((unit_ptr = *topo_ptr++) != NULL) {

	if (unit_ptr->out_func == OUT_IDENTITY) {
	    unit_ptr->act = unit_ptr->Out.output = *in_pat++;
	} else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->act = *in_pat++;
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	} else {
	    unit_ptr->act = *in_pat++;
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	}/* if */

    }/* while */

    return (ret_code);

}/* put_ART2_in_pattern */



/*****************************************************************************
  FUNCTION : adjust_ART2_weights

  PURPOSE  : training function for ART2 networks
  RETURNS  : kernel error code
  NOTES    : Parameters:  
             param_d     constant value 0 < param_d < 1

             p_ptr       points to pointer to first comparison unit

             delay_ptr   points to pointer to first unit in the delay layer. 
	                 The z(j,i) links are not between recognition layer 
			 and comparison layer but between the respective delay 
			 unit of the recogniton unit and the comparison layer.
			 So first we have to look for the corresponding delay 
			 unit of the winning unit before training these weights.

             winner_ptr  points to winning unit of the recognition layer.

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err adjust_ART2_weights(double param_d, TopoPtrArray p_ptr,
				    struct Unit * winner_ptr)
{
    krui_err        ret_code = KRERR_NO_ERROR;
    TopoPtrArray    topo_ptr = NULL;
    struct Unit    *unit_ptr_p = NULL;
    struct Link    *link_ptr = NULL, *link_ptr_u = NULL;

    /* Adjust weights between winning unit and p layer (z(J,i) link values)
    
       (d/dt) z(J,i) = z(J,i) + d * (1-d) * [ u(i)/(1-d) - z(J,i) ]
    
       for (d/dt) -> 0:        z(J,i) = u(i)/(1-d) 
    */

    topo_ptr = p_ptr;

    while ((unit_ptr_p = *topo_ptr++) != NULL) {
	FOR_ALL_LINKS(unit_ptr_p, link_ptr) {
	    if (link_ptr->to == winner_ptr) {

		/* lookin' for corresponding u unit */
		FOR_ALL_LINKS(unit_ptr_p, link_ptr_u) {
		    if (link_ptr_u->to->lln == ART2_U_LAY) {
			link_ptr->weight =
			    ART2_ADJUST_LINK_REC_P(link_ptr_u->to, param_d);
			break;
		    }/* if */
		}/* FOR_ALL_LINKS */

	    }/* if */
	}/* FOR_ALL_LINKS */
    }/* while */


    /* Adjust weights between p layer and winning unit (recognition layer)
       (z(i,j) link values)
    
       (d/dt) z(i,J) = d * (1-d) * [ u(i)/(1-d) - z(i,J) ]
    
       where J is the number of the winning neuron in the recognition layer, i
       ist the number of a p unit
    
       for (d/dt) -> 0:   z(i,J) = u(i)/(1-d)
    
    */


    FOR_ALL_LINKS(winner_ptr, link_ptr) {
	if (link_ptr->to->lln == ART2_P_LAY) {

	    /* lookin' for corresponding u unit */
	    FOR_ALL_LINKS(link_ptr->to, link_ptr_u) {
		if (link_ptr_u->to->lln == ART2_U_LAY) {
		    link_ptr->weight =
			ART2_ADJUST_LINK_P_REC(link_ptr_u->to, param_d);

		    break;
		}/* if */
	    }/* FOR_ALL_LINKS */
	}/* if */
    }/* FOR_ALL_LINKS */


    return (ret_code);

}/* adjust_ART2_weights () */



/*****************************************************************************
 *****************************************************************************

  GROUP        : ARTMAP learning function

  AUTHOR       : Kai-Uwe Herrmann

******************************************************************************
******************************************************************************/



/*****************************************************************************
  FUNCTION : LEARN_ARTMAP

  PURPOSE  : ARTMAP learning function.
  RETURNS  : kernel error code
  NOTES    : Parameters:      
                    3 input-parameter  :  1. vigilance parameter RHOa
                                          2. vigilance parameter RHOb
                                          3. vigilance parameter RHO

                    output-parameters  :  none

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err LEARN_ARTMAP(int start_pattern, int end_pattern,
		      float parameterInArray[], int NoOfInParams,
		      float **parameterOutArray, int *NoOfOutParams)
{
    krui_err        ret_code = KRERR_NO_ERROR;
    int             pattern_no, sub_pat_no; /* Contains actual pattern number */
    int             i,n;
    struct Unit    *winner_ptr_a;	/* recognition unit which is the
					   winner of w.t.a ARTa */
    struct Unit    *winner_ptr_b;	/* recognition unit which is the
					   winner of w.t.a ARTb */
    struct Unit    *unit_ptr;
    TopoPtrArray    topo_layer[14];	/* topo_layer[0] : *first input unit
					   ARTa topo_layer[1] : *first comp.
					   unit ARTa topo_layer[2] : *first
					   rec.  unit ARTa topo_layer[3] :
					   *first delay unit ARTa
					   topo_layer[4] : *first local reset
					   unit ARTa topo_layer[5] : *first
					   special unit ARTa
					   (classified_unit) topo_layer[6] :
					   *first input unit ARTb
					   topo_layer[7] : *first comp. unit
					   ARTb topo_layer[8] : *first rec.
					   unit ARTb topo_layer[9] : *first
					   delay unit ARTb topo_layer[10]:
					   *first local reset unit ARTb
					   topo_layer[11]: *first special
					   unit ARTb (classified_unit)
					   topo_layer[12]: *first map unit
					   topo_layer[13]: *first special map
					   unit */

    TopoPtrArray    topo_ptr;
    FlintType       beta_a;
    FlintType       beta_b;
    float           rho_a;
    float           rho_b;
    float           rho;
    int             start, end;


    /* Check number of incoming parameters */

    if (NoOfInParams < 3) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }/* if */
    /* rho is the vigilance parameter */
    rho_a = parameterInArray[0];
    rho_b = parameterInArray[1];
    rho = parameterInArray[2];


    /* Check interval in which vigilance parameter and constant value L have
       to be */

    if ((rho_a < 0.0) || (rho_a > 1.0) || (rho_b < 0.0) ||
	(rho_b > 1.0) || (rho < 0.0) || (rho > 1.0)
	) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }/* if */
    /* Check if network has been modified or learning func has been changed */
    if (NetModified || LearnFuncHasChanged || (TopoSortID != ARTMAP_TOPO_TYPE)){
	(void) kr_topoSort(ARTMAP_TOPO_TYPE);
	ret_code = KernelErrorCode;
	if (ret_code != KRERR_NO_ERROR) {
	    NetModified = TRUE;
	    return (ret_code);
	}/* if */
	NetModified = FALSE;
	LearnFuncHasChanged = FALSE;
    }/* if */

    /* set initial activation values */
    ret_code = kram_init_i_act(rho_a, rho_b, rho);

    if (ret_code != KRERR_NO_ERROR) {
	return (ret_code);
    }/* if */
    /* beta_a, beta_b are other learning parameters of the network which are
       determined when initializing the network. They are there written to
       the bias field of the structure of each unit of the corresponding ART
       1 network. Now we will read these values. */

    /* find an ARTa unit and get ARTa beta value */
    for (unit_ptr=unit_array+1; unit_ptr->lln != ARTMAP_INPa_LAY; unit_ptr++);
    beta_a = unit_ptr->bias;

    /* find an ARTb unit and get ARTb beta value */
    for (unit_ptr=unit_array+1; unit_ptr->lln != ARTMAP_INPb_LAY; unit_ptr++);
    beta_b = unit_ptr->bias;

    if ((beta_a <= 0.0) || (beta_b <= 0.0)) {
	topo_msg.error_code = KRERR_PARAM_BETA;
	topo_msg.src_error_unit = 0;
	topo_msg.dest_error_unit = 1;
	return (topo_msg.error_code);
    }/* if */

    /* # of output parameters is 0 */
    *NoOfOutParams = 0;
    *parameterOutArray = NULL;


    /* get pointers to first elements of each layer in topo_ptr_array */

    topo_ptr = topo_ptr_array + 1;

    for (i = 0; i <= 13; i++) {
	topo_layer[i] = topo_ptr;
	do {
	} while (*topo_ptr++ != NULL);

    }/* for */


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    /* Search phase */
    start = kr_AbsPosOfFirstSubPat(start_pattern);
    end   = kr_AbsPosOfFirstSubPat(end_pattern);
    end  += kr_NoOfSubPatPairs(end_pattern) - 1;

    for(n=start; n<=end; n++){

	kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n);

	/* initialize the unit activations of the whole net */
	ret_code = krart_reset_activations();
	if (ret_code != KRERR_NO_ERROR) {
	    return (ret_code);
	}/* if */

	/* put pattern into input units */
	ret_code = 
	    put_ARTMAP_in_pattern(pattern_no, sub_pat_no, topo_layer[0], 
				  topo_layer[6]);
	if (ret_code != KRERR_NO_ERROR) {
	    return (ret_code);
	}/* if */
	/* repeat synchronous propagation and look for winner until pattern
	   is classified or network tells us, that pattern is not
	   classifiable */
	do {

	    /* 1 propagation step (all units push their information onto
	       their output and calculate their new activation. */

	    krart_prop_synch();

	    /* look for the recognition unit with the highest activation
	       returns a NULL pointer if all recognition units have
	       activation 0.0 */
	    winner_ptr_a = krart_get_winner(topo_layer[2], 1.0);
	    winner_ptr_b = krart_get_winner(topo_layer[8], 1.0);

	} while (!(ARTMAP_CLASSIFIED) && !(ARTMAP_NOT_CLASSIFIABLE));


	/* training phase */

	if (ARTMAP_CLASSIFIED) {

	    /* Train network i.e. adjust weights between comparison layer and
	       winner_unit and vice versa of both, ARTa and ARTb. Further
	       adjust weights between ARTa delay and map field layer. */

	    ret_code = adjust_ARTMAP_weights(beta_a, beta_b,
					     topo_layer[1], topo_layer[7],
					     topo_layer[3], topo_layer[9],
					     topo_layer[12],
					     winner_ptr_a, winner_ptr_b);

	    if (ret_code != KRERR_NO_ERROR) {
		return (ret_code);
	    }/* if */
	} else {

	    /* we're doing nothing */

	}/* if */

    }/* for */

    return (ret_code);

}/* LEARN_ARTMAP */



/*****************************************************************************
  FUNCTION : put_ARTMAP_in_pattern

  PURPOSE  : pushes a new pattern into the input units of the network
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err put_ARTMAP_in_pattern(int pattern_no, int sub_pat_no,
				      TopoPtrArray topo_inpa_ptr,
				      TopoPtrArray topo_inpb_ptr)
{
    int               ret_code = KRERR_NO_ERROR;
    register Patterns in_pat;
    struct Unit       *unit_ptr;
    TopoPtrArray      topo_ptr_a = topo_inpa_ptr;
    TopoPtrArray      topo_ptr_b = topo_inpb_ptr;


    /* calculate startadress of actual pattern */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);

    while ((unit_ptr = *topo_ptr_a++) != NULL) {
	if (unit_ptr->out_func == OUT_IDENTITY) {
	    unit_ptr->act = unit_ptr->Out.output = *in_pat++;
	} else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->act = *in_pat++;
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	} else {
	    unit_ptr->act = *in_pat++;
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	}/* if */
    }/* while */

    while ((unit_ptr = *topo_ptr_b++) != NULL) {
	if (unit_ptr->out_func == OUT_IDENTITY) {
	    unit_ptr->act = unit_ptr->Out.output = *in_pat++;
	} else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->act = *in_pat++;
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	} else {
	    unit_ptr->act = *in_pat++;
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	}/* if */
    }/* while */


    return (ret_code);

}/* put_ARTMAP_in_pattern */




/*****************************************************************************
  FUNCTION : adjust_ARTMAP_weights

  PURPOSE  : training function for ARTMAP networks
  RETURNS  : kernel error code
  NOTES    : Parameters:  
             beta_a         constant value beta of ARTa > 0.0
             beta_b         constant value beta of ARTb > 0.0

             compa_ptr      points to pointer to 1st comparison unit of ARTa
             compb_ptr      points to pointer to 1st comparison unit of ARTb

             dela_ptr       points to pointer to first unit in the delay layer.
	                    The t(j,i) links are not between recognition layer
			    and comparison layer but between the respective 
			    delay unit of the recogniton unit and the 
			    comparison layer. So first we have to look for the 
			    corresponding delay unit of the winning unit before
			    training these weights.
             delb_ptr       points to pointer to first unit in the delay layer.
	                    The t(j,i) links are not between recognition layer 
			    and comparison layer but between the respective 
			    delay unit of the recogniton unit and the 
			    comparison layer. So first we have to look for the 
			    corresponding delay unit of the winning unit before
			    training these weights.

             map_ptr        points to pointer to first unit in the map layer

             winner_ptr_a   points to winning unit of the recognition layer of 
	                    ARTa.
             winner_ptr_b   points to winning unit of the recognition layer of 
	                    ARTb.


  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err adjust_ARTMAP_weights(double beta_a, double beta_b,
				      TopoPtrArray compa_ptr,
				      TopoPtrArray compb_ptr,
				      TopoPtrArray dela_ptr,
				      TopoPtrArray delb_ptr,
				      TopoPtrArray map_ptr,
				      struct Unit * winner_ptr_a,
				      struct Unit * winner_ptr_b)
{
    krui_err        ret_code = KRERR_NO_ERROR;
    TopoPtrArray    topo_ptr = NULL;
    struct Unit    *unit_ptr_compa = NULL, *unit_ptr_compb = NULL, 
                   *unit_ptr_dela = NULL, *unit_ptr_delb = NULL, 
                   *unit_ptr_map = NULL;

    struct Link    *link_ptr = NULL;

    bool            found_dela_unit = FALSE;
    bool            found_delb_unit = FALSE;

    FlintType       sum_ck = 0.0;


    if ((winner_ptr_a == NULL) || (winner_ptr_b == NULL)) {
	/* We are using ARTMAP in a non-learning mode, wo we are not allowed
	   to adjust weights now. Weights may just be adjusted, if we have an
	   input in ARTa and ARTb each of which brings out a winner in the
	   respective F2-Layer */
	return (ret_code);
    }/* if */
    /* get corresponding unit of the winning unit of ARTa in the delay layer */
    topo_ptr = dela_ptr;

    while ((!found_dela_unit) && (*topo_ptr != NULL)) {
	unit_ptr_dela = *topo_ptr++;
	FOR_ALL_LINKS(unit_ptr_dela, link_ptr) {
	    if (link_ptr->to == winner_ptr_a) {
		found_dela_unit = TRUE;
	    }/* if */
	}/* FOR_ALL_LINKS */
    }/* while */

    /* get corresponding unit of the winning unit of ARTb in the delay layer */

    topo_ptr = delb_ptr;
    while ((!found_delb_unit) && (*topo_ptr != NULL)) {
	unit_ptr_delb = *topo_ptr++;
	FOR_ALL_LINKS(unit_ptr_delb, link_ptr) {
	    if (link_ptr->to == winner_ptr_b) {
		found_delb_unit = TRUE;
		break;
	    }/* if */
	}/* FOR_ALL_LINKS */
    }/* while */

    if ((!found_dela_unit) || (!found_delb_unit)) {

	/* There was no delay unit found corresponding to the winning
	   recognition unit in ARTa or ARTb */

	ret_code = KRERR_TOPOLOGY;
	return (ret_code);

    }/* if */
    /********* ADJUST WEIGHTS *********/
    /* Adjust weights between winning unit (delay-layer) and comparison layer
       (t(j,i) link values) -> ARTa
    
       t(j,i) = c(i)   where j is the number of the winning neuron in the delay
       layer and i ist the number of a comparison unit. 
    */

    topo_ptr = compa_ptr;
    while ((unit_ptr_compa = *topo_ptr++) != NULL) {
	sum_ck += unit_ptr_compa->act;	/* sum up activatons of comparison
					   layer. sum_ck is needed for b(i,j) */
	FOR_ALL_LINKS(unit_ptr_compa, link_ptr) {
	    if (link_ptr->to == unit_ptr_dela) {
		link_ptr->weight = ART1_ADJUST_LINK_DEL_CMP(unit_ptr_compa);
	    }/* if */
	}/* FOR_ALL_LINKS */
    }/* while */

    /* Adjust weights between comparison layer and winning unit (recognition
       layer) -> ARTa
    
       b(i,j) = c(i) / (beta + sum(k)(c(k)))
    
       where j is the number of the winning neuron in the recognition layer, i
       ist the number of a comparison unit and k runs over all comparison
       units. (sum(k)(c(k))) = sum_ck. 
    */

    FOR_ALL_LINKS(winner_ptr_a, link_ptr) {
	if (link_ptr->to->lln == ARTMAP_CMPa_LAY) {
	    link_ptr->weight = (FlintType)ART1_ADJUST_LINK_CMP_REC(link_ptr->to,
								   beta_a,
								   sum_ck);
	}/* if */
    }/* FOR_ALL_LINKS */


    /* Adjust weights between winning unit (delay-layer) and comparison layer
       (t(j,i) link values) -> ARTb
    
       t(j,i) = c(i)   where j is the number of the winning neuron in the delay
       layer and i ist the number of a comparison unit. 
    */

    topo_ptr = compb_ptr;
    sum_ck = 0.0;
    while ((unit_ptr_compb = *topo_ptr++) != NULL) {
	sum_ck += unit_ptr_compb->act;	/* sum up activatons of comparison
					   layer. sum_ck is needed for b(i,j) */
	FOR_ALL_LINKS(unit_ptr_compb, link_ptr) {
	    if (link_ptr->to == unit_ptr_delb) {
		link_ptr->weight = ART1_ADJUST_LINK_DEL_CMP(unit_ptr_compb);
	    }/* if */
	}/* FOR_ALL_LINKS */
    }/* while */

    /* Adjust weights between comparison layer and winning unit (recognition
       layer) (b(i,j) link values)
    
       b(i,j) = c(i) / (beta + sum(k)(c(k)))
    
       where j is the number of the winning neuron in the recognition layer, i
       ist the number of a comparison unit and k runs over all comparison
       units. (sum(k)(c(k))) = sum_ck. 
    */

    FOR_ALL_LINKS(winner_ptr_b, link_ptr) {
	if (link_ptr->to->lln == ARTMAP_CMPb_LAY) {
	    link_ptr->weight = (FlintType)ART1_ADJUST_LINK_CMP_REC(link_ptr->to,
								   beta_b,
								   sum_ck);
	}/* if */
    }/* FOR_ALL_LINKS */


    /* Adjust weights between delay units of ARTa and map units
    
       w(i,j) = map(j) where j is the number of a neuron in the map layer i is
       the number of the winning neuron in the dela layer 
    */

    topo_ptr = map_ptr;
    while ((unit_ptr_map = *topo_ptr++) != NULL) {
	FOR_ALL_LINKS(unit_ptr_map, link_ptr) {
	    if (link_ptr->to == unit_ptr_dela) {
		/* Same as adjustment between delay and comparison layer */
		link_ptr->weight = ART1_ADJUST_LINK_DEL_CMP(unit_ptr_map);
	    }/* if */
	}/* FOR_ALL_LINKS */
    }/* while */

    return (ret_code);

}/* adjust_ARTMAP_weights () */



/*****************************************************************************
 *****************************************************************************

  GROUP        : backpropagation through time learning functions

  AUTHOR       : Martin Reczko
  NOTES        : Implemented are Truncated backpropagation through time with 
                 online-update (BPTT), Truncated backpropagation through time 
                 with batch-update (BBPTT) and truncated quickprop through 
		 time (QPTT) learning functions

******************************************************************************
******************************************************************************/


/*****************************************************************************
  FUNCTION : BPTT_clear_deltaw

  PURPOSE  : BPTT weight change reset
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err BPTT_clear_deltaw(void)
{
    register struct Unit *unit_ptr;
    struct Link    *link_ptr;


    FOR_ALL_UNITS(unit_ptr) {

	/* reset old weight changes (_a), old gradients (_b) and gradient
	   accumulators (_c) */
	unit_ptr->value_a = 0.0;
	unit_ptr->value_b = 0.0;
	unit_ptr->value_c = 0.0;
	FOR_ALL_LINKS(unit_ptr, link_ptr) {
	    link_ptr->value_a = 0.0;
	    link_ptr->value_b = 0.0;
	    link_ptr->value_c = 0.0;
	}
    }
    return (KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : initializeBPTT 

  PURPOSE  : BPTT network activity reset 
  RETURNS  : kernel error code
  NOTES    : BPTT data structures: unit:
             unit_ptr->olddelta : delta values, after finished calculation 
	                          for 1 time step
             unit_ptr->newdelta : accumulators for new delta values

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static krui_err initializeBPTT(void)
{
    register struct Unit *unit_ptr;
    int             i;

    FOR_ALL_UNITS(unit_ptr) {
	/* clear netact-copies */
	for (i = 0; i < MAX_BPTT_BACKSTEP; i++)
	    unit_ptr->actbuf[i] = 0.0;
    }
    return (KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : BPTT_propagateNetForward

  PURPOSE  : topological forward propagation (backprop thru time)
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static void BPTT_propagateNetForward(int pattern_no, int sub_pat_no, int nhist)
{
    register struct Unit *unit_ptr;
    register Patterns in_pat;
    register TopoPtrArray topo_ptr;
    TopoPtrArray    first_hidden_ptr;
    int             i, done_hidden;
    int             all_zero_input = 1;	/* flag to reset net-copies */

    /* calculate startaddress for input pattern array  */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);

    topo_ptr = topo_ptr_array;

    /* ACTIVATE INPUT LAYER */
    /* copy pattern into input unit's activation and calculate output of the
       input units */
    /* topo_ptr points to a (topological sorted) unit stucture (input units
       first)  */
    while ((unit_ptr = *++topo_ptr) != NULL){

	/* apply input pattern */
	if (unit_ptr->out_func == OUT_IDENTITY)
	    /* there is no need to call the output function  */
	    unit_ptr->Out.output = unit_ptr->act = *in_pat++;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act = *in_pat++);
	else
	    /* no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = 
		(*unit_ptr->out_func) (unit_ptr->act = *in_pat++);
	if (fabs(unit_ptr->act) > 0.000001)
	    all_zero_input = 0;	/* no reset-input */

	/* BPTT: shift the actbuf for this input buffer one step back in time */
	for (i = nhist; i > 0; i--) {
	    unit_ptr->actbuf[i] = unit_ptr->actbuf[i - 1];
	}

	/* the new input pattern moves into the second time-layer with index 1,
	   since activations for this pattern are calculated in time-layer 0 */
	unit_ptr->actbuf[1] = unit_ptr->act;

    }

    /* An all-zero input pattern resets all network activities */
    if (all_zero_input) {
	initializeBPTT();    /* reset all netact-copies at start of sequences */
    }

    /* INPUT LAYER DONE */

    /* store first hidden unit pointer */
    first_hidden_ptr = topo_ptr;

    /* shift all actbufs for non-input units one step back in time, make most
       recent activity visible in unit_ptr->Out.output for subsequent calls
       to act_func */
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* hidden layer */
	for (i = nhist; i > 0; i--)
	    unit_ptr->actbuf[i] = unit_ptr->actbuf[i - 1];
	unit_ptr->Out.output = unit_ptr->actbuf[1];
    }

    while ((unit_ptr = *++topo_ptr) != NULL) {	/* output layer */
	for (i = nhist; i > 0; i--)
	    unit_ptr->actbuf[i] = unit_ptr->actbuf[i - 1];
	unit_ptr->Out.output = unit_ptr->actbuf[1];
    }


    /* calculate new activities for hidden and output units */
    /* point to first hidden unit */
    topo_ptr = first_hidden_ptr;
    done_hidden = 0;
    while (((unit_ptr = *++topo_ptr) != NULL) || (done_hidden == 0))
	if (unit_ptr == NULL) {
	    done_hidden = 1;
	} else {
	    /* calc actbuf[0] using actbuf[1], don't update Out.output while
	       updating units, wait until all units are processed  */
	    unit_ptr->act = ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(unit_ptr->act_func) (unit_ptr)) ;
	    unit_ptr->actbuf[0] = unit_ptr->act;
	}

    /* set Out.output */
    topo_ptr = first_hidden_ptr;
    done_hidden = 0;
    while (((unit_ptr = *++topo_ptr) != NULL) || (done_hidden == 0))
	if (unit_ptr == NULL) {
	    done_hidden = 1;
	} else {
	    if (unit_ptr->out_func == OUT_IDENTITY)
		/* identity output function: there is no need to call the
		   output function  */
		unit_ptr->Out.output = unit_ptr->act;
	    else if(unit_ptr->out_func == OUT_Custom_Python)
	    	unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	    else
		/* no identity output function: calculate unit's output also  */
		unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	}
}


/*****************************************************************************
  FUNCTION : initOldDeltas

  PURPOSE  :
  RETURNS  : 
  NOTES    : BPTT starts at the first time-layer (actbuf[0]).
             The deltas for this layer are calculated for the output units by
	     comparison with the target values. All other deltas for hidden 
	     units are zero. The deltas are propagated to the second time-layer
	     (actbuf[1]) into oldelta

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static float initOldDeltas(int pattern_no, int sub_pat_no)
{
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register float  error, sum_error, devit, delta, tmp;
    register TopoPtrArray topo_ptr;
    TopoPtrArray    first_hidden_ptr;
    int             all_correct = 1;	/* flag, wether all bits in the
					   pattern are correct */
    int size;

    /* Initdelta, Step 1: clear all olddeltas (accumulate delta in olddelta) */

    topo_ptr = topo_ptr_array;
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* input units */
	unit_ptr->olddelta = 0.0;
    }

    /* store first hidden unit pointer */
    first_hidden_ptr = topo_ptr;
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* hidden units */
	unit_ptr->olddelta = 0.0;
    }

    while ((unit_ptr = *++topo_ptr) != NULL) {	/* output units */
	unit_ptr->olddelta = 0.0;
    }

    sum_error = 0.0;		/* reset network error  */

    /* calculate address of the output pattern (with number pattern_no + 1)  */
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;


    /* last output unit: add 3 to no_of_topo_units because the topologic
       array contains 4 NULL pointers  */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    /* LOOP FOR ALL OUTPUT UNITS */
    /* calculate olddelta for output units  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	tmp = unit_ptr->Out.output;
	devit = *(--out_pat);

	/* count correct bits using threshold of 0.5 */
	if (devit > 0.5) {
	    if (tmp > 0.5)
		NoOfLearnedPatterns++;
	    else
		all_correct = 0;
	} else {
	    if (tmp <= 0.5)
		NoOfLearnedPatterns++;
	    else
		all_correct = 0;
	}

	devit = devit - tmp;	/* calc. devitation (target_j - output_j) */
	error = devit * devit;
	sum_error += error;

	/* BPTT uses sum_j ( o_j - t_j )^2 as error function => -2.0 * ... */
	delta = -2.0 * devit * (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) );

	/* Initdelta, Step 2: upstream propagation of gradients for backprop */
	FOR_ALL_LINKS(unit_ptr, link_ptr) {
	    tmp = delta * link_ptr->weight;
	    link_ptr->to->olddelta += tmp;	/* accumulate delta */
	    /* accumulate weight gradient */
	    link_ptr->value_c += link_ptr->to->actbuf[1] * delta;	
	}

	/* accumulate bias gradient */
	unit_ptr->value_c += delta;
    }/* output units done */

    /* Initdelta, Step 3:  clear newdelta */
    topo_ptr = topo_ptr_array;
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* input units */
	unit_ptr->newdelta = 0.0;
    }

    while ((unit_ptr = *++topo_ptr) != NULL) {	/* hidden units */
	unit_ptr->newdelta = 0.0;
    }

    while ((unit_ptr = *++topo_ptr) != NULL) {	/* output units */
	unit_ptr->newdelta = 0.0;
    }

    return (sum_error);
}


/*****************************************************************************
  FUNCTION : oneStepBackprop

  PURPOSE  : calc weight changes between consecutive time steps 
  RETURNS  : network error
  NOTES    : heart of BPTT

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static float oneStepBackprop(int backstep, int pattern_no, int sub_pat_no, 
			     int nhist)
{
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    double          delta, sum_error;
    register TopoPtrArray topo_ptr;
    int             done_hidden, nextlayer;
    float           tmp;

    /* CHECK FOR START OF BACKPROP AT THE LAST TIME LAYER */
    if (backstep == 0) {
	sum_error = initOldDeltas(pattern_no,sub_pat_no);
	return (sum_error);	/* start case */
    } else			/* at least for time layer 0, old deltas are
				   known */
	sum_error = 0.0;

    /* index of next layer (used frequently!) */
    nextlayer = backstep + 1;

    /* point to seperator after last input unit */
    topo_ptr = topo_ptr_array;	/* + (NoOfInputUnits + 1); */
    while ((unit_ptr = *++topo_ptr) != NULL);
    done_hidden = 0;

    /* DO BACKPROP FOR ALL NON-INPUT-UNITS */
    while (((unit_ptr = *++topo_ptr) != NULL) || (done_hidden == 0))
	if (unit_ptr == NULL) {	/* skip NULL seperator between hidden and
				   output units */
	    done_hidden = 1;
	} else {		/* delta =  f'(net[backstep]) * olddelta */
	    /* copy actbuf[backstep] to act to enable call to act_deriv_func
	       (overhead: better definition of activation functions required) */
	    unit_ptr->act = unit_ptr->actbuf[backstep];
	    delta = (((unit_ptr->act_deriv_func == ACT_DERIV_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_deriv_func,
						unit_ptr) :
			(unit_ptr->act_deriv_func) (unit_ptr)) ) * unit_ptr->olddelta;

	    /* propagate gradients upstream */
	    FOR_ALL_LINKS(unit_ptr, link_ptr) {
		tmp = delta * link_ptr->weight;
		link_ptr->to->newdelta += tmp;	/* accumulate delta */
		/* accumulate weight gradient */
		link_ptr->value_c += link_ptr->to->actbuf[nextlayer] * delta;
	    }

	    /* accumulate bias gradient */
	    unit_ptr->value_c += delta;
	}

    /* copy newdeltas to olddeltas, clear newdeltas */
    topo_ptr = topo_ptr_array;
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* input units */
	unit_ptr->olddelta = unit_ptr->newdelta;
	unit_ptr->newdelta = 0.0;
    }

    while ((unit_ptr = *++topo_ptr) != NULL) {	/* hidden units */
	unit_ptr->olddelta = unit_ptr->newdelta;
	unit_ptr->newdelta = 0.0;
    }

    while ((unit_ptr = *++topo_ptr) != NULL) {	/* output units */
	unit_ptr->olddelta = unit_ptr->newdelta;
	unit_ptr->newdelta = 0.0;
    }

    return (sum_error);
}



/*****************************************************************************
  FUNCTION : BPTTadapt 

  PURPOSE  : adapt all weights after BPTT using steepest descent with momentum
  RETURNS  : 
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static void BPTTadapt(float step_size, float bptt_momentum)
{
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register TopoPtrArray topo_ptr;
    int             done_hidden = 0;
    float           delta;

    /* point to seperator after last input unit */
    topo_ptr = topo_ptr_array + (NoOfInputUnits + 1);

    /* for each non-input unit: add weight changes to old weights */
    while (((unit_ptr = *++topo_ptr) != NULL) || (done_hidden == 0)) {
	if (unit_ptr == NULL) {
	    done_hidden = 1;
	} else {
	    delta = step_size * (-unit_ptr->value_c) + 
		bptt_momentum * unit_ptr->value_a;
	    if (!IS_SPECIAL_UNIT(unit_ptr))
		unit_ptr->bias += delta;
	    unit_ptr->value_a = delta;
	    unit_ptr->value_c = 0.0;
	    /* set act to last activity, since it was scrambled by bptt */
	    unit_ptr->act = unit_ptr->Out.output;
	    FOR_ALL_LINKS(unit_ptr, link_ptr) {
		delta = step_size * (-link_ptr->value_c) + 
		    bptt_momentum * link_ptr->value_a;
		link_ptr->value_a = delta;
		link_ptr->value_c = 0.0;
	    }
	    if (!IS_SPECIAL_UNIT(unit_ptr))
		FOR_ALL_LINKS(unit_ptr, link_ptr) {
		    link_ptr->weight += link_ptr->value_a;
		}
	}
    }
}



/*****************************************************************************
  FUNCTION : BPTT_propagateNetBackward

  PURPOSE  : BPTT-main: accumulate weight changes backward thru time
  RETURNS  : network error
  NOTES    :

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
static float BPTT_propagateNetBackward(int pattern_no, int sub_pat_no,int nhist)
{
    float           error = 0.0;
    float           dummy;
    int             backstep;

    /* go nhist steps back thru time */
    for (backstep = 0; backstep < nhist; backstep++)
	if (backstep == 0) {
	    /* start at output, pattern-error is calculated first */
	    error = oneStepBackprop(backstep, pattern_no, sub_pat_no, nhist);
	} else {
	    dummy = oneStepBackprop(backstep, pattern_no, sub_pat_no, nhist);
	}
    return (error);
}



/*****************************************************************************
  FUNCTION : LEARN_BPTT

  PURPOSE  : Backpropagation through time learning function
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : step_size
                                 2 : momentum
				 3 : nhist
             Output Parameters:  1 : error of the network (sum of all cycles)


  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err LEARN_BPTT(int start_pattern, int end_pattern, 
		    float *parameterInArray, int NoOfInParams, 
		    float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             ret_code, pattern_no, sub_pat_no, patterns;
    int             nhist;	/* number of steps back in time */
    register struct Unit *unit_ptr;

    if (NoOfUnits == 0)
	return (KRERR_NO_UNITS);        /* No Units defined	 */
    if (NoOfInParams < 1)	        /* has to be ... snns habit ? */
	return (KRERR_PARAMETERS);	/* Not enough input parameters  */

    *NoOfOutParams = 1;		/* One return value is available (the
				   learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)) {
	/* Net has been modified or topologic array isn't initialized */
	/* any connected topology allowed */
	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);

	/* sort units by ''topologic type'', criterion is visibility
	   (input,hidden,output), not topology */
	ret_code = kr_topoSort(TOPOLOGIC_TYPE);

	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return (ret_code);
	/* sites are not supported, check absence */
	FOR_ALL_UNITS(unit_ptr)
	    if UNIT_HAS_SITES
	    (unit_ptr)
		return (KRERR_SITES_NO_SUPPORT);
	NetModified = FALSE;
    }
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified or
						   initialized, clear weight
						   changes */
	ret_code = BPTT_clear_deltaw();
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);
    }
    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    NoOfLearnedPatterns = 0;	/* correct bits using threshold of 0.5 */
    nhist = LEARN_PARAM3(parameterInArray);
    if (nhist > MAX_BPTT_BACKSTEP)
	return (KRERR_NET_DEPTH);	/* actbuf and learning functions
					   support only MAX_BPTT_BACKSTEP net
					   copies */

    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    patterns = 0;
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	/* FORWARD-BPTT */
	/* 1st parameter is the pattern number 2nd parameter is the number of
	   steps back in time */
	BPTT_propagateNetForward(pattern_no,sub_pat_no,nhist); /*Forward pass */

	/* Backward propagation  */
	NET_ERROR(OutParameter) 
	    += BPTT_propagateNetBackward(pattern_no, sub_pat_no, nhist);

	/* online version: adapt net after each pattern has been
	   backpropagated through time and weight changes have accumulated
	   through time */
	BPTTadapt(LEARN_PARAM1(parameterInArray), 
		  LEARN_PARAM2(parameterInArray));

	patterns++;
    }
    return (ret_code);
}


/*****************************************************************************
  FUNCTION : TEST_BPTT

  PURPOSE  : Backpropagation through time validation function
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : step_size
                                 2 : momentum
				 3 : nhist
             Output Parameters:  1 : error of the network (sum of all cycles)

 AUTHOR    : 02.06.1995, Martin Reczko
******************************************************************************/
krui_err TEST_BPTT(int start_pattern, int end_pattern, 
		    float *parameterInArray, int NoOfInParams, 
		    float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             ret_code, pattern_no, sub_pat_no, patterns;
    int             nhist;	/* number of steps back in time */
    register struct Unit *unit_ptr;

    if (NoOfUnits == 0)
	return (KRERR_NO_UNITS);        /* No Units defined	 */
    if (NoOfInParams < 1)	        /* has to be ... snns habit ? */
	return (KRERR_PARAMETERS);	/* Not enough input parameters  */

    *NoOfOutParams = 1;		/* One return value is available (the
				   learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)) {
	/* Net has been modified or topologic array isn't initialized */
	/* any connected topology allowed */
	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);

	/* sort units by ''topologic type'', criterion is visibility
	   (input,hidden,output), not topology */
	ret_code = kr_topoSort(TOPOLOGIC_TYPE);

	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return (ret_code);
	/* sites are not supported, check absence */
	FOR_ALL_UNITS(unit_ptr)
	    if UNIT_HAS_SITES
	    (unit_ptr)
		return (KRERR_SITES_NO_SUPPORT);
	NetModified = FALSE;
    }
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified or
						   initialized, clear weight
						   changes */
	ret_code = BPTT_clear_deltaw();
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);
    }
    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    NoOfLearnedPatterns = 0;	/* correct bits using threshold of 0.5 */
    nhist = 1;

    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern); 
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    patterns = 0;

    while (kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)) {

	/* FORWARD-BPTT */
	/* 1st parameter is the pattern number 2nd parameter is the number of
	   steps back in time */
	BPTT_propagateNetForward(pattern_no,sub_pat_no,nhist); /*Forward pass*/
    
	/* Backward propagation  */
	NET_ERROR(OutParameter) 
	    += BPTT_propagateNetBackward(pattern_no, sub_pat_no, nhist);
  
    }
printf("%d bits correct\n",    NoOfLearnedPatterns);fflush(stdout); 
    return (ret_code);
}


/*****************************************************************************
  FUNCTION : LEARN_BBPTT

  PURPOSE  : Batch backpropagation through time learning function (BBPTT)
  RETURNS  : 
  NOTES    : Input Parameters:   1 : step_size
                                 2 : momentum
				 3 : nhist
             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err LEARN_BBPTT(int start_pattern, int end_pattern, 
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             ret_code, pattern_no, sub_pat_no, patterns;
    int             nhist;	/* number of steps back in time */
    register struct Unit *unit_ptr;

    if (NoOfUnits == 0)
	return (KRERR_NO_UNITS);        /* No Units defined	 */
    if (NoOfInParams < 1)     	        /* has to be ... snns habit ? */
	return (KRERR_PARAMETERS);	/* Not enough input parameters  */

    *NoOfOutParams = 1;		/* One return value is available (the
				   learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)) {
	/* Net has been modified or topologic array isn't initialized */
	/* any connected topology allowed */
	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);

	/* sort units by ''topologic type'', criterion is visibility
	   (input,hidden,output), not topology */
	ret_code = kr_topoSort(TOPOLOGIC_TYPE);

	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return (ret_code);
	/* sites are not supported, check absence */
	FOR_ALL_UNITS(unit_ptr)
	    if UNIT_HAS_SITES
	    (unit_ptr)
		return (KRERR_SITES_NO_SUPPORT);
	NetModified = FALSE;
    }
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified or
						   initialized, clear weight
						   changes */
	ret_code = BPTT_clear_deltaw();
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);
    }
    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    NoOfLearnedPatterns = 0;	/* correct bits using threshold of 0.5 */
    nhist = LEARN_PARAM3(parameterInArray);
    if (nhist > MAX_BPTT_BACKSTEP)
	return (KRERR_NET_DEPTH);	/* actbuf and learning functions
					   support only MAX_BPTT_BACKSTEP net
					   copies */

    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    patterns = 0;
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	/* FORWARD-BPTT */
	/* 1st parameter is the pattern number 2nd parameter is the number of
	   steps back in time */
	BPTT_propagateNetForward(pattern_no,sub_pat_no,nhist); /*Forward pass */

	/* Backward propagation  */
	NET_ERROR(OutParameter) 
	    += BPTT_propagateNetBackward(pattern_no, sub_pat_no, nhist);
	patterns++;
    }

    /* batch version */
    BPTTadapt(LEARN_PARAM1(parameterInArray) / patterns, 
	      LEARN_PARAM2(parameterInArray));

    return (ret_code);
}



/*****************************************************************************
  FUNCTION : LEARN_QPTT

  PURPOSE  : Quickprop through time learning function
  RETURNS  : kernel error code
  NOTES    : Input Parameters:   1 : step_size
                                 2 : maximum step growth
				 3 : decay factor
				 4 : nhist
             Output Parameters:  1 : error of the network (sum of all cycles)

  UPDATE   : 06.11.1993 by Guenter Mamier
******************************************************************************/
krui_err  LEARN_QPTT(int start_pattern, int end_pattern, 
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             ret_code, pattern_no, sub_pat_no, patterns;
    int             nhist;	/* number of steps back in time */
    register struct Unit *unit_ptr;

    if (NoOfUnits == 0)
	return (KRERR_NO_UNITS);/* No Units defined	 */
    if (NoOfInParams < 1)	/* snns habit ? */
	return (KRERR_PARAMETERS);	/* Not enough input parameters  */

    *NoOfOutParams = 1;		/* One return value is available (the
				   learning error)  */
    *parameterOutArray = OutParameter;	/* set the output parameter reference */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)) {
	/* Net has been modified or topologic array isn't initialized */
	/* any connected topology allowed */
	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);

	/* sort units by ''topologic type'', criterion is visibility
	   (input,hidden,output), not topology */
	ret_code = kr_topoSort(TOPOLOGIC_TYPE);

	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return (ret_code);
	/* sites are not supported, check absence */
	FOR_ALL_UNITS(unit_ptr)
	    if UNIT_HAS_SITES
	    (unit_ptr)
		return (KRERR_SITES_NO_SUPPORT);
	NetModified = FALSE;
    }
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified or
						   initialized, clear weight
						   changes */
	ret_code = BPTT_clear_deltaw();
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);
    }
    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    NoOfLearnedPatterns = 0;	/* correct bits using threshold of 0.5 */
    nhist = LEARN_PARAM4(parameterInArray);
    if (nhist > MAX_BPTT_BACKSTEP)
	return (KRERR_NET_DEPTH);	/* actbuf and learning functions
					   support only MAX_BPTT_BACKSTEP net
					   copies */


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    patterns = 0;
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	/* FORWARD-BPTT */
	/* 1st parameter is the pattern number 2nd parameter is the number of
	   steps back in time */
	BPTT_propagateNetForward(pattern_no,sub_pat_no,nhist); /*Forward pass */

	/* Backward propagation  */
	NET_ERROR(OutParameter)
	    += BPTT_propagateNetBackward(pattern_no, sub_pat_no, nhist);

	patterns++;
    }
    MODI_quickprop(LEARN_PARAM1(parameterInArray) / patterns,
		   LEARN_PARAM2(parameterInArray),
		   LEARN_PARAM3(parameterInArray));

    return (ret_code);
}


/*****************************************************************************

  GROUP        : kohonen_learning

  PURPOSE      : learning algorithm for Kohonen Feature Map
  AUTHOR       : Marc Seemann

******************************************************************************/


/*****************************************************************************
  FUNCTION : propagateNet_kohonen

  PURPOSE  : Propagate and train a pattern
  NOTES    :
  UPDATE   : 07.02 1994 by Sven Doering

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

static float propagateNet_kohonen(int pattern_no, int sub_pat_no, float height,
				  float radius, int sizehor)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    register struct Unit *winner_ptr;
    register Patterns in_pat;
    register int    NoOfCompounds, sizever, verwin, horwin, hor, ver, helpver,
    helphor, range;
    float           maximum, sum_error, deviat, learn_error, sum;
    float           unit_ptr_net;
    register TopoPtrArray topo_ptr;
    float           adapt;
    int             winner, current_no;


    /* calculate the activation and the output values         */
    /* of the input units (Input Layer)                       */

    NoOfCompounds = NoOfInputUnits;
    sizever = NoOfHiddenUnits / sizehor;

    sum = 0.0;

    /* calculate startaddress for input pattern array  */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);

    topo_ptr = topo_ptr_array;

    /* copy pattern into input unit's activation and calculate output of the
       input units */
    while ((unit_ptr = *++topo_ptr) != NULL) { /* topo_ptr points to the
						  unit stuctures (sorted by:
						  input-, hidden- and
						  output-units, separated
						  with NULL pointers) */
	sum += *in_pat * *in_pat;

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /* identity output function: there is no need to call the output
	       function  */
	    unit_ptr->Out.output = unit_ptr->act = *in_pat++;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act = *in_pat++);
	else
	    /* no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = 
		(*unit_ptr->out_func) (unit_ptr->act = *in_pat++);
    }

    if (sum != 0.0)
	/* normalize the inputvector */
	normalize_inputvector(sum);

    /* propagate Kohonen Layer  */

    /* calculate the activation and the output values */
    /* of the cmpetitive units (hidden layer) */

    /* winner is determined using the dot product */


    winner_ptr = NULL;
    maximum = -1.0e30;		/* contains the maximum of the activations */
    current_no = 0;

    /* propagate hidden units  */
    while ((unit_ptr = *++topo_ptr) != NULL) {	/* topo_ptr points to a
						   (topological sorted) unit
						   stucture */
	unit_ptr_net = 0.0;
	if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)) {	/* the unit has direct links */
	    FOR_ALL_LINKS(unit_ptr, link_ptr)
		unit_ptr_net += (link_ptr->weight * link_ptr->to->Out.output);
	} else {		/* the unit has sites  */
	    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
		unit_ptr_net += (link_ptr->weight * link_ptr->to->Out.output);
	}

	if (maximum < unit_ptr_net) {	/* determine winner unit  */
	    winner_ptr = unit_ptr;
	    maximum = unit_ptr_net;
	    winner = current_no;
	}
	current_no++;
	/* reset output and activation of hidden units  */
	unit_ptr->Out.output = unit_ptr->act = (FlintType) 0;
    }

    /* the competitive winner is chosen */

    winner_ptr->Out.output = winner_ptr->act = (FlintType) 1;
    winner_ptr->bias++;
    winner_ptr->value_a = (FlintType) (pattern_no + 1);

    /* store number of according pattern in winner unit */

    horwin = winner % sizehor;
    verwin = winner / sizehor;


    /***************************************************************/
    /* Train  the  SOM                                             */

    /* Only the weights of links that go to the winner and its     */
    /* neighbourhood are adjusted, the others remain the same.     */
    /* The incoming weights to the competitive units are adapted   */
    /* as follows:                                                 */

    /* weight(new) = weight(old) + adapt * (output - weight(old))  */

    /* where adapt is the learning rate (0 < adapt <= 1.0)         */
    /* and output is the value of the input unit vector            */

    /***************************************************************/


    for (ver = 0; ver < sizever; ver++)
	for (hor = 0; hor < sizehor; hor++)
	    if ((hor < radius + horwin) &&
		(hor > horwin - radius) &&
		(ver < radius + verwin) &&
		(ver > verwin - radius)) {
		helpver = (float) ((ver - verwin) * (ver - verwin));
		helphor = (float) ((hor - horwin) * (hor - horwin));
		adapt = height * exp(-(helpver + helphor) / 
				     (float) (radius * radius));

		sum = 0.0;
		range = ver * sizehor + hor + 1 + NoOfCompounds;

		/* get unit pointer of unit in adaptation range */
		unit_ptr = kr_getUnitPtr(range);
		
		if(!IS_SPECIAL_UNIT(unit_ptr)){
		    if (unit_ptr->flags & UFLAG_DLINKS) { /* the unit has  */
							  /* direct links  */
			FOR_ALL_LINKS(unit_ptr, link_ptr) {
			    deviat=link_ptr->to->Out.output - link_ptr->weight;
			    learn_error = adapt * deviat;
			    link_ptr->weight += learn_error;
			    /* this is needed for the normalization of the
			       weight_vector */
			    sum += link_ptr->weight * link_ptr->weight;
			}
		    } else {	/* the winner unit has sites  */
			FOR_ALL_SITES_AND_LINKS(winner_ptr,site_ptr,link_ptr) {
			    deviat=link_ptr->to->Out.output - link_ptr->weight;
			    learn_error = adapt * deviat;
			    link_ptr->weight += learn_error;
			    /* this is needed for the normalization of the
			       weight_vector */
			    sum += link_ptr->weight * link_ptr->weight;
			}
		    }
		    if (sum != 0.0)
			normalize_weight(unit_ptr, sum);
		}
	    }
    sum_error = 0.0;		/* 0.0 is chosen arbitrarily and serves no
				   purpose */
    return (sum_error);
}


/*****************************************************************************
  FUNCTION : initializeKohonenLearning

  PURPOSE  : initialize the SOM
  NOTES    :
  UPDATE   : 19.08.1993

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
static krui_err initializeKohonenLearning(void)
{
    register unsigned short flags;
    register struct Unit *unit_ptr;

    FOR_ALL_UNITS(unit_ptr) {
	flags = unit_ptr->flags;

	if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE)	/* unit is in use  */
	    unit_ptr->value_a = unit_ptr->bias = (FlintType) 0.0;
    }
    return (KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : LEARN_kohonen

  PURPOSE  :  incorporates the body of the kohonen learning algorithm
  NOTES    :  the parameterInArray must contain 4 parameter
                      1) initial adaptation height
                      2) initial adaptation radius
       	              3) multiplication factor for height
                      4) multiplication factor for radius
           	      5) horizontal size of the competitive (hidden) layer

  UPDATE   : july 15 1994
******************************************************************************/
krui_err LEARN_kohonen(int start_pattern, int end_pattern,
		       float parameterInArray[], int NoOfInParams,
		       float **parameterOutArray, int *NoOfOutParams)
{
    static float    OutParameter[1];	/* OutParameter[0] stores the
					   learning error  */
    int             ret_code, pattern_no, sub_pat_no;

    if (NoOfUnits == 0)
	return (KRERR_NO_UNITS);/* No Units defined    */
    if (NoOfInParams < 5)	/* see Note  */
	return (KRERR_PARAMETERS);	/* Not enough input parameters  */

    *NoOfOutParams = 1;		/* one return value is available (the
				   learning error) */
    *parameterOutArray = OutParameter;	/* set output parameter reference  */
    ret_code = KRERR_NO_ERROR;	/* clear return code  */


    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)) {	
	/* Net has been modified or topologic array isn't initialized */
	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code == KRERR_NO_OUTPUT_UNITS)
	    ret_code = KRERR_NO_ERROR;
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);

	/* sort units by topology and by topologic type  */
	ret_code = kr_topoSort(TOPOLOGIC_TYPE);
	if (ret_code == KRERR_NO_OUTPUT_UNITS)
	    ret_code = KRERR_NO_ERROR;
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return (ret_code);

	NetModified = FALSE;
    }
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified or
						   initialized, initialize
						   kohonen now  */
	ret_code = initializeKohonenLearning();
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);
    }
    if ((int) LEARN_PARAM5(parameterInArray) == 0) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }
    if ((LEARN_PARAM3(parameterInArray) > 1.0) ||
	(LEARN_PARAM3(parameterInArray) < 0.0)) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }
    if ((LEARN_PARAM4(parameterInArray) > 1.0) ||
	(LEARN_PARAM4(parameterInArray) < 0.0)) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	NoOfLearnedPatterns++;
	NET_ERROR(OutParameter) +=
	    propagateNet_kohonen(pattern_no,sub_pat_no,
				 LEARN_PARAM1(parameterInArray),
				 LEARN_PARAM2(parameterInArray),
				 (int) LEARN_PARAM5(parameterInArray));

	LEARN_PARAM1(parameterInArray) *= LEARN_PARAM3(parameterInArray);
	LEARN_PARAM2(parameterInArray) *= LEARN_PARAM4(parameterInArray);

    }
    return (ret_code);
}


/*****************************************************************************
  FUNCTION : spanning_tree

  PURPOSE  : calculate the spanning tree of the kohonen feature map
  NOTES    : evaluating the learn function doesn't affect the net itself

  UPDATE   : july 13 1993
******************************************************************************/
krui_err  spanning_tree(void)
{
    register TopoPtrArray topo_ptr;
    register struct Unit *unit_ptr;
    int             ret_code, n, pattern_no, sub_pat_no;


    if (NoOfUnits == 0)
	return (KRERR_NO_UNITS);/* No Units defined    */

    ret_code = KRERR_NO_ERROR;	/* clear return code  */


    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)) {	
	/* Net has been modified or topologic array isn't initialized */
	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code == KRERR_NO_OUTPUT_UNITS)
	    ret_code = KRERR_NO_ERROR;
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);

	/* sort units by topology and by topologic type  */
	ret_code = kr_topoSort(TOPOLOGIC_TYPE);
	if (ret_code == KRERR_NO_OUTPUT_UNITS)
	    ret_code = KRERR_NO_ERROR;
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return (ret_code);

	NetModified = FALSE;
    }
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified or
						   initialized, initialize
						   kohonen now  */
	ret_code = initializeKohonenLearning();
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);
    }
    topo_ptr = topo_ptr_array;

    while ((unit_ptr = *++topo_ptr) != NULL);
    /* topo_ptr points to the units' stucture (sorted by: input-, hidden- and
       output-units, separated by NULL pointers) */

    while ((unit_ptr = *++topo_ptr) != NULL)
	/* topo_ptr points to hidden_units */
	unit_ptr->value_a = 0;	/* the unit next to a pattern stores the
				   number of that pattern in value_a, at the
				   beginning initialized to 0 */


    n = 0;
    while(kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n++)){

	/* To calculate the winning unit we call the  propagateNet_kohonen
	   function, and treat the map as 1-dimensional array */
	propagateNet_kohonen(pattern_no, sub_pat_no, 0.0, 0.0, 1);
    }

    return (ret_code);

}/* spanning_tree */




/*****************************************************************************

  GROUP        : JORDAN / ELMAN networks 

  PURPOSE      : learning functions for JORDAN / ELMAN networks
  AUTHOR       : Tobias Soyez

******************************************************************************/


/*****************************************************************************
  FUNCTION : update_je_context_units

  PURPOSE  : synchronous update of context units
  NOTES    :

  UPDATE   :
******************************************************************************/

static void update_je_context_units (int pattern_no, int sub_pat_no,
				     float use_real_value_percent) 

{
    register TopoPtrArray   topo_ptr, topo_ptr_context ;
    register struct Unit   *unit_ptr ;
    register Patterns       out_pat  ;
    int size;


    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;
    
    topo_ptr_context = topo_ptr_array + (no_of_topo_units + 3) ;


    /* ----  store real output ---- */
 
    if (use_real_value_percent > 1.0)
	use_real_value_percent = 1.0;
    else
	if (use_real_value_percent < 0.0)
	    use_real_value_percent = 0.0;

    topo_ptr = topo_ptr_context ;

    while ((unit_ptr = *--topo_ptr) != NULL)
    {
      unit_ptr->actbuf[0]  = unit_ptr->Out.output ;
      unit_ptr->Out.output = (1.0 - use_real_value_percent) * *--out_pat +
	                     use_real_value_percent * unit_ptr->Out.output;
    }
    

    /* ----  calculate new activation of context units ---- */

   topo_ptr = topo_ptr_context ;

    while ((unit_ptr = *++topo_ptr) != NULL)
    {
      unit_ptr->act = ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(unit_ptr->act_func) (unit_ptr))  ;

      if (unit_ptr->out_func == OUT_IDENTITY)
        unit_ptr->Out.output = unit_ptr->act ;
      else if(unit_ptr->out_func == OUT_Custom_Python)
      	unit_ptr->Out.output = 
		kr_PythonOutFunction(unit_ptr->python_out_func,
			unit_ptr->act); 
      else
        unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act) ;
    }

    /* ----  restore real output  ---- */

    topo_ptr = topo_ptr_context ;

    while ((unit_ptr = *--topo_ptr) != NULL)
    {
      unit_ptr->Out.output = unit_ptr->actbuf[0] ;
    }
}


/*****************************************************************************
  FUNCTION : reset_je_context_units

  PURPOSE  : resets the context units 
  NOTES    :

  UPDATE   :
******************************************************************************/

static void reset_je_context_units (void)

{
  register TopoPtrArray   topo_ptr ;
  register struct Unit   *unit_ptr ;

 
  topo_ptr = topo_ptr_array + (no_of_topo_units + 3) ;
  
  while ((unit_ptr = *++topo_ptr) != NULL)
  {
    unit_ptr->act = unit_ptr->i_act ;

    if (unit_ptr->out_func == OUT_IDENTITY)
      unit_ptr->Out.output = unit_ptr->act ;
    else if(unit_ptr->out_func == OUT_Custom_Python)
    	unit_ptr->Out.output = 
		kr_PythonOutFunction(unit_ptr->python_out_func,
			unit_ptr->act); 
    else
      unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act) ;
  }
}


/*****************************************************************************
  FUNCTION : check_je_network

  PURPOSE  : checks the topology of a partial recurrent network
             (i.e. JORDAN and ELMAN networks)
  NOTES    :

  UPDATE   :
******************************************************************************/
static krui_err check_je_network (void) 

{
    /*  check the topology of the network  */
    (void) kr_topoCheckJE () ;
    if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode) ;

    /*	count the no. of I/O units and check the patterns  */
    if (kr_IOCheck() != KRERR_NO_ERROR) return (KernelErrorCode) ;

    /*	sort units by topology and by topologic type  */
    (void) kr_topoSort (TOPOLOGICAL_JE) ;

    if ((KernelErrorCode != KRERR_NO_ERROR) && 
        (KernelErrorCode != KRERR_DEAD_UNITS))
      return (KernelErrorCode) ;

    NetModified = FALSE;
    return (KRERR_NO_ERROR) ;
}



/*****************************************************************************
  FUNCTION : LEARN_JE_Backprop

  PURPOSE  : backpropagation learning function for JORDAN / ELMAN networks
  NOTES    : input parameters  :   1. learning parameter
                                   2. delta max
				   3. influence of real output
				      (= 0 -> only teacher force)
             output parameters :   1. error of the network (sum of all cycles)
             return value      :   kernel error code
  UPDATE   :
******************************************************************************/
krui_err  LEARN_JE_Backprop (int     start_pattern    , int  end_pattern ,
                             float  *parameterInArray , int  NoOfInParams, 
                             float **parameterOutArray, int *NoOfOutParams)

{
  static float  OutParameter[1] ; /* OutParameter[0] stores the */
                                  /* learning error             */
  int	        n, pattern_no,sub_pat_no ;
  int           start, end;



  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */

  if (NoOfInParams < 3) return (KRERR_PARAMETERS) ;

  *NoOfOutParams     = 1            ; /* one return value is available      */
                                      /* (the learning error)               */
  *parameterOutArray = OutParameter ; /* set the output parameter reference */ 
  NET_ERROR (OutParameter) = 0.0    ; /* reset network error value          */

  if (NetModified || (TopoSortID != TOPOLOGICAL_JE))
  {
     KernelErrorCode = check_je_network () ;
     if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode) ;
  }

  reset_je_context_units () ;


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    start = kr_AbsPosOfFirstSubPat(start_pattern);
    end   = kr_AbsPosOfFirstSubPat(end_pattern);
    end  += kr_NoOfSubPatPairs(end_pattern) - 1;

    for(n=start; n<=end; n++){

	kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n);

	propagateNetForward (pattern_no,sub_pat_no) ;	
	NET_ERROR (OutParameter) +=
	    propagateNetBackward2 (pattern_no,sub_pat_no, 
				   LEARN_PARAM1 (parameterInArray), 
				   LEARN_PARAM2 (parameterInArray)) ;
	update_je_context_units (pattern_no,sub_pat_no, 
				 LEARN_PARAM3(parameterInArray)) ;
    }

  return (KernelErrorCode) ;
}


/*****************************************************************************
  FUNCTION : TEST_JE_Backprop

  PURPOSE  : backpropagation learning function for JORDAN / ELMAN networks
  NOTES    : input parameters  :   1. learning parameter
                                   2. delta max
				   3. influence of real output
				      (= 0 -> only teacher force)
				      has no meaning for validation
             output parameters :   1. error of the network (sum of all cycles)
             return value      :   kernel error code
  UPDATE   :
******************************************************************************/
krui_err  TEST_JE_Backprop (int     start_pattern    , int  end_pattern ,
                             float  *parameterInArray , int  NoOfInParams, 
                             float **parameterOutArray, int *NoOfOutParams)

{
  static float  OutParameter[1] ; /* OutParameter[0] stores the */
                                  /* learning error             */
  int	        n, pattern_no,sub_pat_no ;
  int           start, end;


  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */

  if (NoOfInParams < 3) return (KRERR_PARAMETERS) ;

  *NoOfOutParams     = 1            ; /* one return value is available      */
                                      /* (the learning error)               */
  *parameterOutArray = OutParameter ; /* set the output parameter reference */ 
  NET_ERROR (OutParameter) = 0.0    ; /* reset network error value          */

  if (NetModified || (TopoSortID != TOPOLOGICAL_JE))
  {
     KernelErrorCode = check_je_network () ;
     if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode) ;
  }

  reset_je_context_units () ;


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    start = kr_AbsPosOfFirstSubPat(start_pattern);
    end   = kr_AbsPosOfFirstSubPat(end_pattern);
    end  += kr_NoOfSubPatPairs(end_pattern) - 1;

    for(n=start; n<=end; n++){

	kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n);

	propagateNetForward (pattern_no,sub_pat_no) ;	
	NET_ERROR (OutParameter) +=
	    testNetBackward2 (pattern_no,sub_pat_no, 
				   LEARN_PARAM1 (parameterInArray),
				   LEARN_PARAM2 (parameterInArray)) ;
	test_update_je_context_units (pattern_no,sub_pat_no) ; 
    }

  return (KernelErrorCode) ;
}

/*****************************************************************************
  FUNCTION : TEST_JE_BackpropMomentum

  PURPOSE  : test network with momentum term learning funcyion
             for JORDAN / ELMAN networks
  NOTES    : input parameters  :   4. delta max
                                   5. influence of real output
				      (= 0 -> only teacher force)
				      has no meaning for validation
             output parameters :   1. error of the network (sum of all cycles)
             return value      :   kernel error code
  UPDATE   :
******************************************************************************/
krui_err TEST_JE_BackpropMomentum(int start_pattern, int end_pattern,
				   float *parameterInArray, int NoOfInParams, 
				   float **parameterOutArray, 
				   int *NoOfOutParams)

{
  static float  OutParameter[1] ; /* OutParameter[0] stores the */
                                  /* learning error             */
  int	        n, pattern_no,sub_pat_no ;
  int           start, end;

  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */

  if (NoOfInParams < 5) return (KRERR_PARAMETERS) ;

  *NoOfOutParams     = 1            ; /* one return value is available      */
                                      /* (the learning error)               */
  *parameterOutArray = OutParameter ; /* set the output parameter reference */ 
  NET_ERROR (OutParameter) = 0.0    ; /* reset network error value          */



  if (NetModified || (TopoSortID != TOPOLOGICAL_JE))
  {
     KernelErrorCode = check_je_network () ;
     if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode) ;
  }

  reset_je_context_units () ;


  /* compute the necessary sub patterns */

  KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
  if(KernelErrorCode != KRERR_NO_ERROR)
      return (KernelErrorCode);
  
  start = kr_AbsPosOfFirstSubPat(start_pattern);
  end   = kr_AbsPosOfFirstSubPat(end_pattern);
  end  += kr_NoOfSubPatPairs(end_pattern) - 1;

  for(n=start; n<=end; n++){

      kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n);

      propagateNetForward (pattern_no,sub_pat_no) ;	
      NET_ERROR (OutParameter) +=
	  testNetBackward2  (pattern_no,sub_pat_no,
				 LEARN_PARAM1( parameterInArray ),
				 LEARN_PARAM4( parameterInArray )) ;
      test_update_je_context_units (pattern_no, sub_pat_no) ;
  }

  return (KernelErrorCode) ;
}

/*****************************************************************************
  FUNCTION : test_update_je_context_units           joe

  PURPOSE  : synchronous update of context units
  NOTES    :

  UPDATE   : 03.03.95
******************************************************************************/

static void test_update_je_context_units (int pattern_no, int sub_pat_no) 

{
    register TopoPtrArray   topo_ptr, topo_ptr_context ;
    register struct Unit   *unit_ptr ;
    register Patterns       out_pat  ;
    int size;


    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;
    
    topo_ptr_context = topo_ptr_array + (no_of_topo_units + 3) ;


    /* ----  store real output                        ---- */
 
    topo_ptr = topo_ptr_context ;

    while ((unit_ptr = *--topo_ptr) != NULL)
    {
      unit_ptr->actbuf[0]  = unit_ptr->Out.output ;
      unit_ptr->Out.output = *--out_pat ; 
    }
    

    /* ----  calculate new activation of context units ---- */

   topo_ptr = topo_ptr_context ;

    while ((unit_ptr = *++topo_ptr) != NULL)
    {
      unit_ptr->act = ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(unit_ptr->act_func) (unit_ptr))  ;

      if (unit_ptr->out_func == OUT_IDENTITY)
        unit_ptr->Out.output = unit_ptr->act ;
      else if(unit_ptr->out_func == OUT_Custom_Python)
      	unit_ptr->Out.output = 
		kr_PythonOutFunction(unit_ptr->python_out_func,
			unit_ptr->act); 
      else
        unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act) ;
    }

    /* ----  restore real output  ---- */

    topo_ptr = topo_ptr_context ;

    while ((unit_ptr = *--topo_ptr) != NULL)
    {
      unit_ptr->Out.output = unit_ptr->actbuf[0] ;
    }

}



/*****************************************************************************
  FUNCTION : LEARN_JE_BackpropMomentum

  PURPOSE  : backpropagation with momentum term learning funcyion
             for JORDAN / ELMAN networks
  NOTES    : input parameters  :   1. learning parameter
                                   2. momentum factor
                                   3. flat spot elimination
                                   4. delta max
				   5. influence of real output
				      (= 0 -> only teacher force)
             output parameters :   1. error of the network (sum of all cycles)
             return value      :   kernel error code
  UPDATE   :
******************************************************************************/
krui_err LEARN_JE_BackpropMomentum(int start_pattern, int end_pattern,
				   float *parameterInArray, int NoOfInParams, 
				   float **parameterOutArray, 
				   int *NoOfOutParams)

{
  static float  OutParameter[1] ; /* OutParameter[0] stores the */
                                  /* learning error             */
  int	        n, pattern_no,sub_pat_no ;
  int           start, end;

  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */

  if (NoOfInParams < 5) return (KRERR_PARAMETERS) ;

  *NoOfOutParams     = 1            ; /* one return value is available      */
                                      /* (the learning error)               */
  *parameterOutArray = OutParameter ; /* set the output parameter reference */ 
  NET_ERROR (OutParameter) = 0.0    ; /* reset network error value          */

  if (NetModified || (TopoSortID != TOPOLOGICAL_JE))
  {
     KernelErrorCode = check_je_network () ;
     if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode) ;
  }

  if (NetInitialize || LearnFuncHasChanged)
  {  /*  Net has been modified or initialized, initialize backprop now  */
    KernelErrorCode = initializeBackpropMomentum () ;
    if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode) ;
  }

  reset_je_context_units () ;


  /* compute the necessary sub patterns */

  KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
  if(KernelErrorCode != KRERR_NO_ERROR)
      return (KernelErrorCode);
  
  start = kr_AbsPosOfFirstSubPat(start_pattern);
  end   = kr_AbsPosOfFirstSubPat(end_pattern);
  end  += kr_NoOfSubPatPairs(end_pattern) - 1;

  for(n=start; n<=end; n++){

      kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n);

      propagateNetForward (pattern_no,sub_pat_no) ;	
      NET_ERROR (OutParameter) +=
	  Backprop_momentum_FSE (pattern_no,sub_pat_no,
				 LEARN_PARAM1( parameterInArray ),
				 LEARN_PARAM2( parameterInArray ),
				 LEARN_PARAM3( parameterInArray ),
				 LEARN_PARAM4( parameterInArray )) ;
      update_je_context_units (pattern_no, sub_pat_no,
			       LEARN_PARAM5(parameterInArray)) ;
  }

  return (KernelErrorCode) ;
}


/*****************************************************************************
  FUNCTION : LEARN_JE_Quickprop

  PURPOSE  : quickprop learning function for JORDAN / ELMAN networks
  NOTES    : input parameters  :   1. learning parameter
                                   2. max. growth factor
                                   3. weight decay
                                   4. delta max
				   5. influence of real output
				      (= 0 -> only teacher force)
             output parameters :   1. error of the network (sum of all cycles)
             return value      :   kernel error code
  UPDATE   :
******************************************************************************/
krui_err  LEARN_JE_Quickprop (int     start_pattern    , int  end_pattern ,
                              float  *parameterInArray , int  NoOfInParams, 
                              float **parameterOutArray, int *NoOfOutParams)

{
  static float  OutParameter[1] ; /* OutParameter[0] stores the */
                                  /* learning error             */
  int	        n,pattern_no,sub_pat_no ;
  int           start, end;

  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */

  if (NoOfInParams < 5) return (KRERR_PARAMETERS) ;

  *NoOfOutParams     = 1            ; /* one return value is available      */
                                      /* (the learning error)               */
  *parameterOutArray = OutParameter ; /* set the output parameter reference */ 
  NET_ERROR (OutParameter) = 0.0    ; /* reset network error value          */

  if (NetModified || (TopoSortID != TOPOLOGICAL_JE))
  {
     KernelErrorCode = check_je_network () ;
     if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode) ;
  }

  if (NetInitialize || LearnFuncHasChanged)
  { 
    /*  Net has been modified or initialized, initialize quickprop now  */
    KernelErrorCode = initializeQuickprop () ;
    if (KernelErrorCode != KRERR_NO_ERROR)  return (KernelErrorCode) ;
  }


  reset_je_context_units () ;



  /* compute the necessary sub patterns */

  KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
  if(KernelErrorCode != KRERR_NO_ERROR)
    return (KernelErrorCode);

  start = kr_AbsPosOfFirstSubPat(start_pattern);
  end   = kr_AbsPosOfFirstSubPat(end_pattern);
  end  += kr_NoOfSubPatPairs(end_pattern) - 1;

  for(n=start; n<=end; n++){

      kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n);

      propagateNetForward (pattern_no,sub_pat_no) ;	
      NET_ERROR(OutParameter) +=
	  propagateNetBackwardQuickprop (pattern_no,sub_pat_no,
					 LEARN_PARAM4 (parameterInArray)) ;
      update_je_context_units (pattern_no,sub_pat_no,
			       LEARN_PARAM5(parameterInArray)) ;
  }

  MODI_quickprop (LEARN_PARAM1 (parameterInArray),
                  LEARN_PARAM2 (parameterInArray),
                  LEARN_PARAM3 (parameterInArray)) ;

  return (KernelErrorCode) ;
}



/*****************************************************************************
  FUNCTION : LEARN_JE_Rprop

  PURPOSE  : rprop learning function for JORDAN / ELMAN networks
  NOTES    : input parameters  :   1. delta 0
                                   2. delta max
				   3. ????????
				   4. influence of real output
				      (= 0 -> only teacher force)
             output parameters :   1. error of the network (sum of all cycles)
             return value      :   kernel error code
  UPDATE   :
******************************************************************************/
krui_err  LEARN_JE_Rprop    (int     start_pattern    , int  end_pattern ,
                             float  *parameterInArray , int  NoOfInParams, 
                             float **parameterOutArray, int *NoOfOutParams)

{
  static float  OutParameter[1] ; /* OutParameter[0] stores the */
                                  /* learning error             */
  int	        pattern_no,sub_pat_no ;
  int           n, ret_code, blocksize ;
  float         maxeps, wd, update_value ;
  int           start, end;


  if (NoOfInParams < 4) return (KRERR_PARAMETERS) ;

  if (( update_value = LEARN_PARAM1 (parameterInArray)) == 0.0) 
    update_value = RPROP_DEFAULT_UPDATE_VALUE;
  if ((maxeps = LEARN_PARAM2 (parameterInArray)) == 0.0) 
    maxeps = RPROP_MAXEPS;
  if (!(( wd = LEARN_PARAM3( parameterInArray )) == 0.0))
      wd = (float) pow(10,(double)(- wd));
  if (update_value > maxeps) update_value = maxeps;


  KernelErrorCode = ret_code = KRERR_NO_ERROR;  /*  reset return code  */


  *NoOfOutParams     = 1            ; /* one return value is available      */
                                      /* (the learning error)               */
  *parameterOutArray = OutParameter ; /* set the output parameter reference */ 
  NET_ERROR (OutParameter) = 0.0    ; /* reset network error value          */

  if (NetModified || (TopoSortID != TOPOLOGICAL_JE))
  {
     KernelErrorCode = check_je_network () ;
     if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode) ;
  }

  if (NetInitialize || LearnFuncHasChanged)
  {  /*  Net has been modified or initialized, initialize RPROP */
    ret_code = initializeRprop (update_value) ;
    if (ret_code != KRERR_NO_ERROR)  return (ret_code) ;
  }

  /* DEFAULTS: */
  if ((blocksize = LEARN_PARAM3 (parameterInArray)) == 0)
    blocksize = end_pattern;

  reset_je_context_units () ;


  /* compute the necessary sub patterns */

  KernelErrorCode = kr_initSubPatternOrder(start_pattern,blocksize);
  if(KernelErrorCode != KRERR_NO_ERROR)
      return (KernelErrorCode);

  start = kr_AbsPosOfFirstSubPat(start_pattern);
  end   = kr_AbsPosOfFirstSubPat(end_pattern);
  end  += kr_NoOfSubPatPairs(end_pattern) - 1;

  for(n=start; n<=end; n++){

      kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n);

      propagateNetForward (pattern_no,sub_pat_no) ;	
      NET_ERROR (OutParameter) +=
	  propagateNetBackwardRprop (pattern_no,sub_pat_no) ;
      update_je_context_units (pattern_no,sub_pat_no,
			       LEARN_PARAM4(parameterInArray)) ;
  }
  MODI_rprop (maxeps,wd) ;
  return (KernelErrorCode) ;
}

/*****************************************************************************
  FUNCTION : TEST_JE_Rprop

  PURPOSE  : rprop testing function for JORDAN / ELMAN networks
  NOTES    : input parameters  :   1. delta 0
                                   2. delta max
				   3. ??????????
				   4. influence of real output
				      (= 0 -> only teacher force)
				      has no meaning for validation
             output parameters :   1. error of the network (sum of all cycles)
             return value      :   kernel error code
  UPDATE   : 03.03.95 Joachim Danz
******************************************************************************/
krui_err  TEST_JE_Rprop    (int     start_pattern    , int  end_pattern ,
			    float  *parameterInArray , int  NoOfInParams, 
			    float **parameterOutArray, int *NoOfOutParams)

{
  static float  OutParameter[1] ; /* OutParameter[0] stores the */
                                  /* learning error             */
  int	        pattern_no,sub_pat_no ;
  int           n, ret_code, blocksize ;
  int           start, end;


  if (NoOfInParams < 4) return (KRERR_PARAMETERS) ;

  KernelErrorCode = ret_code = KRERR_NO_ERROR;  /*  reset return code  */


  *NoOfOutParams     = 1            ; /* one return value is available      */
                                      /* (the learning error)               */
  *parameterOutArray = OutParameter ; /* set the output parameter reference */ 
  NET_ERROR (OutParameter) = 0.0    ; /* reset network error value          */


  /* DEFAULTS: */
  if ((blocksize = LEARN_PARAM3 (parameterInArray)) == 0)
    blocksize = end_pattern;

  if (NetModified || (TopoSortID != TOPOLOGICAL_JE))
  {
     KernelErrorCode = check_je_network () ;
     if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode) ;
  }

  reset_je_context_units () ;


  /* compute the necessary sub patterns */

  KernelErrorCode = kr_initSubPatternOrder(start_pattern,blocksize);
  if(KernelErrorCode != KRERR_NO_ERROR)
      return (KernelErrorCode);

  start = kr_AbsPosOfFirstSubPat(start_pattern);
  end   = kr_AbsPosOfFirstSubPat(end_pattern);
  end  += kr_NoOfSubPatPairs(end_pattern) - 1;

  for(n=start; n<=end; n++){

      kr_getSubPatternByNo(&pattern_no,&sub_pat_no,n);

      propagateNetForward (pattern_no,sub_pat_no) ;	
      NET_ERROR (OutParameter) +=
	  testNetBackwardRprop (pattern_no,sub_pat_no) ;
      test_update_je_context_units (pattern_no,sub_pat_no) ;
  }
  return (KernelErrorCode) ;
}



/*****************************************************************************

  GROUP        : Functions for autoassoziative memory networks

  PURPOSE      : Implement autoassoziative memory networks, including learning 
                 functions for Rummelhart & McClelland's Delta Rule and Hebbian
		 learning
  AUTHOR       : Jamie DeCoster

******************************************************************************/

/*****************************************************************************
  FUNCTION : RM_propagate

  PURPOSE  : forward propagation for Rummelhart & McClelland's Delta Rule
  NOTES    : 

  UPDATE   : 17.02.1994
******************************************************************************/
static void RM_propagate (int pattern_no, int sub_pat_no, float prop_step)
{

    int t;
    register struct Unit   *unit_ptr;
    register Patterns      in_pat;
    register TopoPtrArray  topo_ptr;


    /*  calculate startaddress for input pattern array  */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);
    if(in_pat == NULL){
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return;
    }
    
    topo_ptr = topo_ptr_array;

    /*  copy pattern into input unit's activation and calculate output of the 
	input units */
    while ((unit_ptr = *++topo_ptr) != NULL){

	/*  topo_ptr points to a (topological sorted) unit stucture  */
	if (unit_ptr->out_func == OUT_IDENTITY)
	    /*  identity output function: don't call the output function  */
	    unit_ptr->Out.output = unit_ptr->act = *in_pat++;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act = *in_pat++);
	else
	    /*  no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = 
		(*unit_ptr->out_func) (unit_ptr->act = *in_pat++);
    }

    for (t=0; t < prop_step; ++t){ 

	FOR_ALL_UNITS( unit_ptr )
	    if UNIT_IN_USE( unit_ptr ){

		/* update unit activations first  */
		if ( !IS_INPUT_UNIT( unit_ptr)) 
		    /*  unit isn't an input unit and is in use and enabled  */
		    unit_ptr->act = ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(unit_ptr->act_func) (unit_ptr)) ;

		/* update unit outputs  */
		if (unit_ptr->out_func == OUT_IDENTITY)
		    /*  identity output function: don't call output function  */
		    unit_ptr->Out.output = unit_ptr->act;
		else if(unit_ptr->out_func == OUT_Custom_Python)
			unit_ptr->Out.output = 
				kr_PythonOutFunction(unit_ptr->python_out_func,
					unit_ptr->act);
		else
		    /*  calculate unit's output also  */
		    unit_ptr->Out.output = (*unit_ptr->out_func)(unit_ptr->act);
	    }
    }

}


/*****************************************************************************
  FUNCTION : RM_learn

  PURPOSE  : backward propagation for Rummelhart & McClelland's Delta Rule
  NOTES    : 

  UPDATE   : 11.02.1994
******************************************************************************/
static void RM_learn(float learn_parameter)
{
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register struct Unit *unit_ptr;
    float ex_in, in_in, error, eta;

    eta = learn_parameter;

    FOR_ALL_UNITS (unit_ptr)
	if (!IS_INPUT_UNIT (unit_ptr)){
	    /* World units don't learn so their inputs are not examined */

	    in_in = 0;

	    FOR_ALL_LINKS (unit_ptr, link_ptr)
		if (IS_INPUT_UNIT (link_ptr->to))
		    /* Determine the input from the world unit */
		    ex_in = link_ptr->to->act * link_ptr->weight;
		else
		    /* Determine the input from the network */
		    in_in += link_ptr->to->act * link_ptr->weight; 

	    /* Error defined as the difference between the world input and 
	       the input from the net */ 
	    error = ex_in - in_in; 
 
	    /* Modify the weights */ 
	    if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)){
		FOR_ALL_LINKS (unit_ptr, link_ptr)
		    if (!IS_INPUT_UNIT (link_ptr->to))  
			/* The link between a world unit and its corresponding 
			   learning unit is always 1 */
			link_ptr->weight += link_ptr->to->act * eta * error;
	    }else{
		FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		    if (!IS_INPUT_UNIT (link_ptr->to))
			link_ptr->weight += link_ptr->to->act * eta * error;
	    }
	}
}



/*****************************************************************************
  FUNCTION : LEARN_RM_delta 

  PURPOSE  : McClelland & Rumelhart's learning rule
                      Input parameter:   1: learning parameter
		                         2: no. of propagation steps
		      Output parameter:  1:  Learning error
  NOTES    : 

  UPDATE   : 11.02.1994
******************************************************************************/
krui_err LEARN_RM_delta (int start_pattern, int end_pattern,
			 float *parameterInArray, int NoOfInParams, 
			 float **parameterOutArray, int *NoOfOutParams)
{

    static float OutParameter [1];
    int pattern_no,sub_pat_no;
    float Learn_p;
    float prop_step;

    KernelErrorCode = KRERR_NO_ERROR;

    /* Checking for learning parameter */
    if (NoOfInParams < 2){  
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }

    Learn_p = LEARN_PARAM1 (parameterInArray);
    prop_step = LEARN_PARAM2 (parameterInArray);
    if (prop_step == 0){  
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }

    *NoOfOutParams = 1; /* Out Parameter = Learning error */
    *parameterOutArray = OutParameter;

    (void) kr_topoSort (TOPOLOGIC_TYPE);

    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    /* reset network error value  */
    NET_ERROR (OutParameter) = 0.0;

    /* Determine order of pattern presentation */
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	/* Propagate the pattern through the network */
	RM_propagate (pattern_no,sub_pat_no,prop_step);        

	/* Update links */ 
	RM_learn (Learn_p);

	/* Compute network error */ 
	NET_ERROR (OutParameter) += Hebb_error(prop_step); 
    }

    return (KernelErrorCode);
} 


/*****************************************************************************
  FUNCTION : Hebb_error

  PURPOSE  : Compute the error of the network for the Hebbian learning rule
  NOTES    : 

  UPDATE   : 09.03.1994
******************************************************************************/
static float Hebb_error(int NoOfTimes)
{

    struct Unit *unit_ptr;
    struct Link *link_ptr;
    float error, sum_error, ex_in;
    int t;


    /* update unit activations first  */
    for(t=0; t < NoOfTimes; ++t){ 
	FOR_ALL_UNITS( unit_ptr )
	    if ( !IS_INPUT_UNIT( unit_ptr)) 
		if UNIT_IN_USE( unit_ptr )
		    /*  unit isn't an input unit and is in use and enabled  */
		    unit_ptr->act = ((unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(unit_ptr->python_act_func,
						unit_ptr) :
			(unit_ptr->act_func) (unit_ptr)) ;

	/* update unit outputs */
	FOR_ALL_UNITS( unit_ptr )
	    if UNIT_IN_USE( unit_ptr )
		if (unit_ptr->out_func == OUT_IDENTITY)
		    /* there is no need to call the output function  */
		    unit_ptr->Out.output = unit_ptr->act;
		else if(unit_ptr->out_func == OUT_Custom_Python)
			unit_ptr->Out.output = 
				kr_PythonOutFunction(unit_ptr->python_out_func,
					unit_ptr->act);
		else
		    /* calculate unit's output also  */
		    unit_ptr->Out.output = (*unit_ptr->out_func)(unit_ptr->act);
    }


    /* calculate the error defined as the difference between the internal
       and external inputs */

    sum_error = 0.0;

    FOR_ALL_UNITS (unit_ptr){
	FOR_ALL_LINKS (unit_ptr, link_ptr)
	    if (IS_INPUT_UNIT (link_ptr->to)){
		ex_in = link_ptr->to->act;
		error = ex_in - unit_ptr->act;
		sum_error += error * error;
	    }
    }
    return (sum_error);
}



/*****************************************************************************
  FUNCTION : LEARN_Hebb 

  PURPOSE  : Hebbian learning rule
                      Input parameter:   1: learning parameter
		                         2: Maximum absolute weight strength
		      Output parameter:  1: Network error
  NOTES    : 

  UPDATE   : 09.03.1994
******************************************************************************/
krui_err LEARN_HEBB (int start_pattern, int end_pattern,
		     float *parameterInArray, int NoOfInParams, 
		     float **parameterOutArray, int *NoOfOutParams)
{
    static float OutParameter [1];
    int pattern_no, sub_pat_no;
    int NoOfTimes;
    float Learn_p, Weight_MAX;
    register struct Unit *unit_ptr;
    register struct Link *link_ptr;
    register struct Site *site_ptr;
    register Patterns in_pat;
    register TopoPtrArray topo_ptr;


    KernelErrorCode = KRERR_NO_ERROR;

    if (NoOfInParams < 3){  /* Checking for learning parameter */
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }

    *NoOfOutParams = 1;  /* Out Parameter = Learning error */
    *parameterOutArray = OutParameter;

    Learn_p    = LEARN_PARAM1 (parameterInArray);
    Weight_MAX = LEARN_PARAM2 (parameterInArray);
    NoOfTimes  = (int)LEARN_PARAM3 (parameterInArray);

    if (NoOfTimes == 0){  /* Checking for learning parameter */
	KernelErrorCode = KRERR_PARAMETERS;
	return (KernelErrorCode);
    }
    
    kr_topoSort (TOPOLOGIC_TYPE);

    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
        return (KernelErrorCode);


    /* reset network error value  */
    NET_ERROR (OutParameter) = 0.0;

    /* Determine order of pattern presentation */
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){
  
	/* calculate startaddress for input pattern array */
    in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);

	topo_ptr = topo_ptr_array;

	/* copy pattern into input units  and calculate their output */
	while ((unit_ptr = *++topo_ptr) != NULL){
	    /* topo_ptr points to a unit structure (input units first) */
	    if (unit_ptr->out_func == OUT_IDENTITY)
		/* identity output function */
		unit_ptr->Out.output = unit_ptr->act = *in_pat++;
	    else if(unit_ptr->out_func == OUT_Custom_Python)
	    	unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act = *in_pat++);
	    else
		/* calculate unit's output */
		unit_ptr->Out.output = 
		    (*unit_ptr->out_func) (unit_ptr->act = *in_pat++);
	}

	/* copy pattern from the world units to the learning units */
	FOR_ALL_UNITS (unit_ptr)
	    FOR_ALL_LINKS (unit_ptr, link_ptr)
		if (IS_INPUT_UNIT (link_ptr->to))
		    unit_ptr->act = link_ptr->to->act;

	/* Network has the same structure as the RM_delta autoassociative 
	   network. Here we update the learning unit links. */
	FOR_ALL_UNITS (unit_ptr)
	    if (!IS_INPUT_UNIT (unit_ptr)){

		/* Update the links */
		if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)){
		    FOR_ALL_LINKS (unit_ptr, link_ptr)
			if (!IS_INPUT_UNIT (link_ptr->to)){
			    /* Only change learning links */
			    link_ptr->weight += 
				Learn_p * unit_ptr->act * (link_ptr->to->act);
			    if (link_ptr->weight > Weight_MAX)
				link_ptr->weight = Weight_MAX;
			    if (link_ptr->weight < -Weight_MAX)
				link_ptr->weight = -Weight_MAX;
			}
		}else{
		    FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
			if (!IS_INPUT_UNIT (link_ptr->to)){
			    link_ptr->weight += 
				Learn_p * unit_ptr->act * (link_ptr->to->act);
			    if (link_ptr->weight > Weight_MAX)
				link_ptr->weight = Weight_MAX;
			    if (link_ptr->weight < -Weight_MAX)
				link_ptr->weight = -Weight_MAX;
			}
		}
	    }

	NET_ERROR (OutParameter) += Hebb_error (NoOfTimes); 
    }
    return (KernelErrorCode);
}


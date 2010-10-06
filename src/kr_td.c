/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_td.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Learning Functions for Time Delay networks
  NOTES          : with following learning functions:
                   - Backpropagation

  AUTHOR         : Oliver Schnabel, Guenter Mamier
  DATE           : 

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.12 $
  LAST CHANGE    : $Date: 1998/03/13 16:23:57 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <math.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif


#include "kr_typ.h"	 /*  Kernel Types and Constants  */
#include "kr_const.h"	 /*  Constant Declarators for SNNS-Kernel  */
#include "kr_def.h"	 /*  Default Values  */
#include "kernel.h"	 /*  kernel function prototypes  */
#include "kr_mac.h"	 /*  Kernel Macros   */
#include "kr_newpattern.h"

#include "kr_td.ph"



/*****************************************************************************
  FUNCTION : initializeTDBackprop

  PURPOSE  : initialize the learning algorithm TD-backprop
  NOTES    :

  RETURNS  :
  UPDATE   : 19.02.1993 
******************************************************************************/
static krui_err initializeTDBackprop(void)
{
  register FlagWord      flags;
  register struct Link   *link_ptr;
  register struct Unit   *unit_ptr;
  register struct Site   *site_ptr; 

  /*  set unit's bias to zero  */
  FOR_ALL_UNITS( unit_ptr ){
      flags = unit_ptr->flags;

      if ( (unit_ptr->flags & UFLAG_IN_USE) == UFLAG_IN_USE){
	  if (flags & UFLAG_SITES){ /*  unit has sites  */
	      FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
		  link_ptr->value_b = 
		      link_ptr->value_c = 
			  unit_ptr->value_a = 
			      unit_ptr->value_b = 
				  unit_ptr->value_c = (FlintType) 0;
	  }else{		/* TD-units have no sites: direct links */
	      if (flags & UFLAG_DLINKS){ /*  unit has direct links */
		  FOR_ALL_LINKS( unit_ptr, link_ptr ) 
		      link_ptr->value_b = 
			  link_ptr->value_c = 
			      unit_ptr->value_a = 
				  unit_ptr->value_b = 
				      unit_ptr->value_c = (FlintType) 0;
	      }
	  } /* else */
      } /* if */
  } /* FOR_ALL_UNITS */
  return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : propagateTDNetForward

  PURPOSE  : topological TimeDelay forward propagation
  NOTES    : needs only the weight matrix of one receptive field for 
	     propagating one pattern through the net
	     If the provided pattern_no is < 0, no pattern is loaded into
             the input layer but all other layers are propagated as usual
  RETURNS  :
  UPDATE   : 19.02.1993
******************************************************************************/
extern FlintType OUT_Custom_Python(FlintType act);

void  propagateTDNetForward(int pattern_no, int sub_pat_no)
{ 
  register struct Unit    *unit_ptr;
  register struct Link    *link_ptr;
  register Patterns       in_pat;
  register TopoPtrArray   topo_ptr;
  int                     i;

  if (pattern_no >= 0){
      /*  calculate startaddress for input pattern array  */

      in_pat = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);
      topo_ptr = topo_ptr_array;

      /* copy pattern into input unit's activation and calculate output of the 
	 input units. */
      /* order of the topoptrarray: input-, hidden- and then outputunits */    

      unit_ptr = *++topo_ptr;
      while (unit_ptr != (struct Unit *) NULL){
	  /*  topo_ptr points to a (topological sorted) unit stucture 
	      (input units first)  */

	  if (unit_ptr->out_func == OUT_IDENTITY){
	      /*  identity output function: don't call the output function  */
	      unit_ptr->Out.output = unit_ptr->act = *in_pat++;
	  }else if(unit_ptr->out_func == OUT_Custom_Python){
	  	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act = *in_pat++);
	  }else{
	      /*  no identity output function: calculate unit's output also  */
	      unit_ptr->Out.output = 
		  (*unit_ptr->out_func) (unit_ptr->act = *in_pat++);
	  } /*if*/
	  unit_ptr = *++topo_ptr;
      }
  }else{
      /* set unit_ptr and topo_ptr as if an input pattern was provided */
      topo_ptr = topo_ptr_array;
      unit_ptr = *++topo_ptr;
      while (unit_ptr != (struct Unit *) NULL)
      {
	  unit_ptr = *++topo_ptr;
      }
  }
      
  /* Propagate input to hidden, hidden to hidden and hidden to output */

  for (i=0; i<2; i++){
      unit_ptr = *++topo_ptr;
      while (unit_ptr != NULL){
	  /*  initialization for propagating hidden units  */
	  /*  clear error values */
	  unit_ptr->Aux.flint_no = 0.0;

	  if (UNIT_HAS_DIRECT_INPUTS(unit_ptr)){
	      /* this is a reference unit, initialize link weight change */
	      /* and counter of link usage */
	      FOR_ALL_LINKS(unit_ptr, link_ptr){
		  link_ptr->value_b = link_ptr->value_c = 0.0;
	      }
	  }

	  /* reset bias-change and td-step-counter before each lerning epoch */
	  unit_ptr->value_b = unit_ptr->value_c = 0.0;

	  unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);
	  if (unit_ptr->out_func == OUT_IDENTITY){
	      /*  identity output function: don't call the output function  */
	      unit_ptr->Out.output = unit_ptr->act;
	  }else if(unit_ptr->out_func == OUT_Custom_Python){
	  	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	  }else{
	      /*  no identity output function: calculate unit's output also  */
	      unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	  }      
	  unit_ptr = *++topo_ptr;
      }
  }
} /*endfunction*/



/*****************************************************************************
  FUNCTION : propagateTDNetBackward

  PURPOSE  : Time Delay Backward error propagation (topological).
  NOTES    : Start calculating the average of the corresponding links in 
             all TD-steps. This average is used to update the links of the 
	     1st. receptive field.
  RETURNS  : network error
  UPDATE   : 19.02.1993
******************************************************************************/
static	float propagateTDNetBackward(int pattern_no, int sub_pat_no,
				     float learn_parameter, 
				     float delta_max)
{ 
    register struct Link   *link_ptr;
    register struct Site   *site_ptr;
    register struct Unit   *unit_ptr, *unit_ptr1 ;
    register struct Unit   *ref_unit;
    register Patterns      out_pat;
    register float         error,  sum_error,  eta,  devit, learn_error;
    register TopoPtrArray  topo_ptr;
    int                    last_log_layer, i;
    int                    size;

    sum_error = 0.0;		/*  reset network error  */
    eta = learn_parameter;	/*  store learn_parameter in CPU register  */

    /*  calculate address of the output pattern (with number pattern_no + 1) */

    topo_ptr = topo_ptr_array + (no_of_topo_units + 2);
    last_log_layer = (*topo_ptr)->lln;
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;

    /* calculate output units only: begin at the end of topo_pointer_array */
    unit_ptr = *topo_ptr;
    while (unit_ptr != (struct Unit *) NULL){
	devit = *(--out_pat) - unit_ptr->Out.output; /*  calc. devitation */

	if ( (float) fabs( devit ) <= delta_max ){
	    unit_ptr = *--topo_ptr;
	    continue;
	}

	sum_error += devit * devit; /*  sum up the error of the network  */

	/* calculate error for output units	 */
	/* output layer cannot have time delay structure, so no 
	   distinction is necessary*/
	error = devit * (unit_ptr->act_deriv_func) ( unit_ptr ); 

	/* calc. the error for adjusting weights and bias of pred. units  */
	learn_error = eta * error; 

	/* adjust bias value  */
	unit_ptr->value_b += learn_error;
	unit_ptr->value_c += 1.0;

	if (UNIT_HAS_DIRECT_INPUTS( unit_ptr )){ /* the unit has direkt links */
	    /* error must be saved for each unit of the hiddenlayer */
	    FOR_ALL_LINKS( unit_ptr, link_ptr ){
		/* adjust link weights and calc. sum of errors of pred. units*/
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		link_ptr->value_b += learn_error * link_ptr->to->Out.output;
		link_ptr->value_c += 1.0;
	    }
	}else{ /* the unit has sites: not necessary for TD-Network  */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
		/* adjust link weights and calc. sum of errors of pred. units */
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		link_ptr->weight += learn_error * link_ptr->to->Out.output;
            }
	}
     
	unit_ptr = *--topo_ptr; 
    } /* while */


    /*  calculate hidden units only. add the weight changes of all receptive 
	fields ; stored for every linkin the Linkstructure value_c of only the 
	first recept. field! 
    */
  
    unit_ptr = *--topo_ptr;
    while (unit_ptr != (struct Unit *) NULL){
	/* calc. the error of the hidden units */
	error = (unit_ptr->act_deriv_func) (unit_ptr) * unit_ptr->Aux.flint_no;
    
	/* calc. the error for adjusting weights and bias of pred. units */
	learn_error = eta * error;


	if (unit_ptr->TD.td_connect_typ == 1){
	    /* this is a time delay connection type layer */

	    ref_unit = *(unit_ptr->TD.my_topo_ptr + unit_ptr->TD.target_offset);
	    /*	adjust bias value  */
	    ref_unit->value_b += learn_error;
	    ref_unit->value_c += 1.0;
	
	    if (UNIT_HAS_DIRECT_INPUTS( ref_unit )){
		/*  the unit has direkt links	*/
	    
		FOR_ALL_LINKS( ref_unit, link_ptr ) {
		    /* adjust link weights and calc. sum of err of pred. units*/
		    /* unit_ptr1 points to the actual predecessor unit, 
		       determined by the actual link */
		    unit_ptr1 = *(link_ptr->to->TD.my_topo_ptr 
				  + unit_ptr->TD.source_offset);
		
		    if (IS_HIDDEN_UNIT (unit_ptr1)) {
			/* this unit is a hidden unit: add the error from 
			   previous units dependent on type of this predecessor 
			   unit   */
			(unit_ptr1)->Aux.flint_no += link_ptr->weight * error;
		    }
		    /* immediately updating the links cannot fit for TD 
		       Networks! Add the the delta(ij) of all td_steps in the 
		       Linkarray(value_c) of the first recept. field  */
		    link_ptr->value_b += learn_error * (unit_ptr1)->Out.output;
		    link_ptr->value_c += 1.0;
		}
	    }
	}else{
	    /* fully connected layers */
	    /* immediately update of all physical links */
	    unit_ptr->bias += learn_error;

	    if (UNIT_HAS_DIRECT_INPUTS( unit_ptr )){
		/*  the unit has direkt links  */
		/* error must be saved for each unit of the hiddenlayer */
		FOR_ALL_LINKS( unit_ptr, link_ptr ){
		    /* adjust link weights and calc. sum of err of pred units*/
		    if (IS_HIDDEN_UNIT (link_ptr->to))
			link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		  
		    link_ptr->weight += learn_error * link_ptr->to->Out.output;
		}
	    }else{ /* the unit has sites: not necessary for TD-Network  */
		FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
		    /*  adjust link weights and calc. sum of errors of the 
			predecessor units */
		    if (IS_HIDDEN_UNIT (link_ptr->to)) 
			link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		    link_ptr->weight += learn_error * link_ptr->to->Out.output;
		}
	    }
	}
	unit_ptr = *--topo_ptr;
    } /* while */
  

    /* update receptive fields: propagate and calculate all featureunits of 
       the first td-step of each layer.   */
    /* topo_ptr points to the NULL pointer between input and hidden units */

    for (i=0; i<2; i++){
	unit_ptr = *++topo_ptr;
	while (unit_ptr != NULL){
	    if (unit_ptr->TD.td_connect_typ==1 && 
		UNIT_HAS_DIRECT_INPUTS(unit_ptr) &&
		unit_ptr->value_c > 0.0){
		/* this is a reference unit of a time delay layer */

		/* update bias of reference unit by average bias change */
		unit_ptr->bias += unit_ptr->value_b / unit_ptr->value_c;

		FOR_ALL_LINKS( unit_ptr, link_ptr ){ 
		    /* store average linkweigth changes of all Td-steps */
		    link_ptr->weight += link_ptr->value_b / link_ptr->value_c;
		} /*for_all_links*/
	    }
	    unit_ptr = *++topo_ptr;
	}
    }
    return( sum_error );	/*  return the error of the network */
}



/*****************************************************************************
  FUNCTION : LEARN_TDbackprop

  PURPOSE  : Time Delay Backpropagation Learning Function
  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : delta max

             Output Parameters:  1 : error of the network (sum of all cycles)

  RETURNS  :
  UPDATE   : 19.02.1993
******************************************************************************/
krui_err  LEARN_TDbackprop( int start_pattern, int end_pattern, 
			   float parameterInArray[], int NoOfInParams,
			   float * *parameterOutArray, int *NoOfOutParams )

{
    static  float  OutParameter[1]; /* OutParameter[0] stores learning error */
    int     ret_code, pattern_no, sub_pat_no;
    struct Unit *unit_ptr;

    if (NoOfUnits == 0)  
	return( KRERR_NO_UNITS ); /*  No Units defined	*/
    if (NoOfInParams < 1)	/*  #  has to be changed (must be 2) # */
	return( KRERR_PARAMETERS ); /*  Not enough input parameters  */

    *NoOfOutParams = 1;	 /* One return value is available (the learning error)*/
    *parameterOutArray = OutParameter; /* set the output parameter reference  */
    ret_code = KRERR_NO_ERROR;  /*  reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGIC_LOGICAL)){
	/*  Net has been modified or topologic array isn't initialized */
	/*  check the topology of the network  */
	/* first: save the logical layer numbers, restore them after check */
	FOR_ALL_UNITS(unit_ptr)
	    unit_ptr -> Aux.int_no = unit_ptr -> lln;
	ret_code = kr_topoCheck();

	FOR_ALL_UNITS(unit_ptr)
	    unit_ptr -> lln = unit_ptr -> Aux.int_no;
	if (ret_code < KRERR_NO_ERROR)  
	    return( ret_code );	/*  an error has occured  */
	if (ret_code < 2)  
	    return( KRERR_NET_DEPTH ); /* the network has less then 2 layers */

	/*	count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)  
	    return( ret_code );

	ret_code = kr_topoSort( TOPOLOGIC_LOGICAL );
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return( ret_code );

#ifdef DEBUG
	/* M.V. */
	j=1;
	while (krui_setCurrentUnit(j) == KRERR_NO_ERROR) {
	    printf("Unit %d: lln = %d, lun = %d\n",j,
		   kr_getUnitPtr(j) -> lln, kr_getUnitPtr(j) -> lun);
	    j++;
	}
#endif

	NetModified = FALSE;
    }

    if (NetInitialize || LearnFuncHasChanged){
	/*  Net has been modified or initialized, initialize TDbackprop now */
	ret_code = initializeTDBackprop();
	if (ret_code != KRERR_NO_ERROR)  
	    return( ret_code );
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	propagateTDNetForward(pattern_no,sub_pat_no); /* Forward propagation */

	/*  Backward propagation  */
	/*  1st parameter is the learning parameter
	    2nd parameter is the max. devitation between output pattern and
	    the output of the output unit (delta max)
	    */
	NET_ERROR( OutParameter ) +=
	    propagateTDNetBackward(pattern_no,sub_pat_no,
				   LEARN_PARAM1( parameterInArray ),
				   LEARN_PARAM2( parameterInArray ) );
    }

    return( ret_code );
}



/*****************************************************************************
  FUNCTION : LEARN_TDBP_McClelland

  PURPOSE  : Time Delay Backpropagation Learning Function With McClelland
             Error function: E = sum(log(1-(oi-ti)^2))

  NOTES    : Input Parameters:   1 : learning parameter
                                 2 : delta max

             Output Parameters:  1 : error of the network (sum of all cycles)

  RETURNS  :
  UPDATE   : 19.02.1993
******************************************************************************/
krui_err  LEARN_TDBP_McClelland( int start_pattern, int end_pattern, 
				float parameterInArray[], int NoOfInParams,
				float * *parameterOutArray, int *NoOfOutParams )

{
    static  float  OutParameter[1]; /* OutParameter[0] stores learning error*/
    int   ret_code, pattern_no, sub_pat_no;
    struct Unit *unit_ptr;

    if (NoOfUnits == 0)  
	return( KRERR_NO_UNITS ); /*  No Units defined	*/
    if (NoOfInParams < 1)	/*  #  has to be changed (must be 2) # */
	return( KRERR_PARAMETERS ); /*  Not enough input parameters  */

    *NoOfOutParams = 1;	/* One return value is available (the learning error)*/
    *parameterOutArray = OutParameter; /* set the output parameter reference */
    ret_code = KRERR_NO_ERROR;  /*  reset return code  */

    if (NetModified || (TopoSortID != TOPOLOGIC_LOGICAL)){
	/*  Net has been modified or topologic array isn't initialized */
	/*  check the topology of the network  */
	/* first: save the logical layer numbers, restore them after check */
	FOR_ALL_UNITS(unit_ptr)
	    unit_ptr -> Aux.int_no = unit_ptr -> lln;
	ret_code = kr_topoCheck();

	FOR_ALL_UNITS(unit_ptr)
	    unit_ptr -> lln = unit_ptr -> Aux.int_no;
	if (ret_code < KRERR_NO_ERROR)  
	    return( ret_code );	/*  an error has occured  */
	if (ret_code < 2)  
	    return( KRERR_NET_DEPTH ); /* the network has less then 2 layers */

	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)  
	    return( ret_code );

	ret_code = kr_topoSort( TOPOLOGIC_LOGICAL );
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return( ret_code );

#ifdef DEBUG
	/* M.V. */
	j=1;
	while (krui_setCurrentUnit(j) == KRERR_NO_ERROR) {
	    printf("Unit %d: lln = %d, lun = %d\n",  j,
		   kr_getUnitPtr(j) -> lln, kr_getUnitPtr(j) -> lun);
	    j++;
	}
#endif

	NetModified = FALSE;
    }

    if (NetInitialize || LearnFuncHasChanged){
	/*  Net has been modified or initialized, initialize TDbackprop now  */
	ret_code = initializeTDBackprop();
	if (ret_code != KRERR_NO_ERROR)  
	    return( ret_code );
    }


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	propagateTDNetForward(pattern_no,sub_pat_no); /*  Forward propagation */

	/*  Backward propagation  */
	/*  1st parameter is the learning parameter
	    2nd parameter is the max. devitation between output pattern and
	    the output of the output unit (delta max)
	    */
	NET_ERROR( OutParameter ) +=
	    propagateTDNetBackMcClelland(pattern_no,sub_pat_no,
					 LEARN_PARAM1( parameterInArray ),
					 LEARN_PARAM2( parameterInArray ) );
    }

    return( ret_code );
}


/*****************************************************************************
  FUNCTION : propagateTDNetBackMcClelland

  PURPOSE  : Time Delay Backward error propagation (topological).
  NOTES    : Start calculating the average of the corresponding links in 
             all TD-steps. This average is used to update the links of the 
	     1st. receptive field.
  RETURNS  : network error
  UPDATE   : 19.02.1993
******************************************************************************/
static	float propagateTDNetBackMcClelland(int pattern_no, int sub_pat_no,
					   float learn_parameter,
					   float delta_max)
{ 
    register struct Link   *link_ptr;
    register struct Site   *site_ptr;
    register struct Unit   *unit_ptr, *unit_ptr1 ;
    register struct Unit   *ref_unit;
    register Patterns      out_pat;
    register float         error,  sum_error,  eta,  devit, learn_error;
    register TopoPtrArray  topo_ptr;
    int                    last_log_layer, i;
    int                    size;


    sum_error = 0.0;		/*  reset network error  */
    eta = learn_parameter;	/*  store learn_parameter in CPU register  */

    /*  calculate address of the output pattern (with number pattern_no + 1) */

    topo_ptr = topo_ptr_array + (no_of_topo_units + 2);
    last_log_layer = (*topo_ptr)->lln;
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;

    /* calculate output units only: begin at the end of topo_pointer_array */
    unit_ptr = *topo_ptr;
    while (unit_ptr != (struct Unit *) NULL){
	devit = *(--out_pat) - unit_ptr->Out.output; /*  calc. devitation */

	if ( (float) fabs( devit ) <= delta_max ){
	    unit_ptr = *--topo_ptr;
	    continue;
	}

	sum_error += -log10(1- fabs(devit)); /* sum up error of the network */

	/* calculate error for output units	 */
	/* output layer cannot have time delay structure, so no 
	   distinction is necessary*/
	error = log10(1- fabs(devit)) * (unit_ptr->act_deriv_func) (unit_ptr); 
	if(devit > 0) error = -error;

	/* calc. error for adjusting weights and bias of predecessor units */
	learn_error = eta * error; 

	/* adjust bias value  */
	unit_ptr->value_b += learn_error;
	unit_ptr->value_c += 1.0;

	if (UNIT_HAS_DIRECT_INPUTS( unit_ptr )){
	    /*  the unit has direkt links  */
	    /* error must be saved for each unit of the hiddenlayer */
	    FOR_ALL_LINKS( unit_ptr, link_ptr ){
		/* adjust link weights and calc. sum of errors of pred. units*/
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		link_ptr->value_b += learn_error * link_ptr->to->Out.output;
		link_ptr->value_c += 1.0;
	    }
	}else{ /* the unit has sites: not necessary for TD-Network  */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
		/* adjust link weights and calc. sum of errors of 
		   predecessor units */
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		link_ptr->weight += learn_error * link_ptr->to->Out.output;
            }
	}
     
	unit_ptr = *--topo_ptr; 
    } /* while */


    /*  calculate hidden units only. add the weightchanges of all receptive 
	fields ; stored for every link in the Linkstructure value_c of only 
	the first recept. field! 
    */
  
    unit_ptr = *--topo_ptr;
    while (unit_ptr != (struct Unit *) NULL){
	/* calc. the error of the hidden units */
	error = (unit_ptr->act_deriv_func) (unit_ptr) * unit_ptr->Aux.flint_no;
    
	/* calc. error for adjusting weights and bias of predecessor units */
	learn_error = eta * error;


	if (unit_ptr->TD.td_connect_typ == 1){
	    /* this is a time delay connection type layer */

	    ref_unit = *(unit_ptr->TD.my_topo_ptr + unit_ptr->TD.target_offset);
	    /*	adjust bias value  */
	    ref_unit->value_b += learn_error;
	    ref_unit->value_c += 1.0;
	
	    if (UNIT_HAS_DIRECT_INPUTS( ref_unit )){
		/*  the unit has direkt links	*/
	    
		FOR_ALL_LINKS( ref_unit, link_ptr ){
		    /* adjust link weights and calc. sum of err of pred. units*/
		    /* unit_ptr1 points to the actual predecessor unit, 
		       determined by the actual link */
		    unit_ptr1 = *(link_ptr->to->TD.my_topo_ptr 
				  + unit_ptr->TD.source_offset);
		
		    if (IS_HIDDEN_UNIT (unit_ptr1)){
			/* this unit is a hidden unit: add the error from 
			   previous units dependent on type of this predecessor 
			   unit   */
			(unit_ptr1)->Aux.flint_no += link_ptr->weight * error;
		    }
		    /* immediately updating the links cannot fit for TD 
		       Networks! Add the the delta(ij) of all td_steps in the 
		       Linkarray(value_c) of the first recept. field  */
		    link_ptr->value_b += learn_error * (unit_ptr1)->Out.output;
		    link_ptr->value_c += 1.0;
		}
	    }
	}else{
	    /* fully connected layers */
	    /* immediately update of all physical links */
	    unit_ptr->bias += learn_error;

	    if (UNIT_HAS_DIRECT_INPUTS( unit_ptr )){
		/*  the unit has direkt links  */
		/* error must be saved for each unit of the hiddenlayer */
		FOR_ALL_LINKS( unit_ptr, link_ptr ){
		    /* adjust link weights and calc. sum of 
		       errors of predecessor units  */
		    if (IS_HIDDEN_UNIT (link_ptr->to))
			link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		  
		    link_ptr->weight += learn_error * link_ptr->to->Out.output;
		}
	    }else{/* the unit has sites: not necessary for TD-Network  */
		FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
		    /*  adjust link weights and calc. sum of errors of the 
			predecessor units */
		    if (IS_HIDDEN_UNIT (link_ptr->to)) 
			link_ptr->to->Aux.flint_no += link_ptr->weight * error;
		    link_ptr->weight += learn_error * link_ptr->to->Out.output;
		}
	    }
	}
	unit_ptr = *--topo_ptr;
    } /* while */
  

    /* update receptive fields: propagate and calculate all featureunits of 
       the first td-step of each layer.   */
    /* topo_ptr points to the NULL pointer between input and hidden units */

    for (i=0; i<2; i++){
	unit_ptr = *++topo_ptr;
	while (unit_ptr != NULL){
	    if (unit_ptr->TD.td_connect_typ==1 && 
		UNIT_HAS_DIRECT_INPUTS(unit_ptr) &&
		unit_ptr->value_c > 0.0){
		/* this is a reference unit of a time delay layer */

		/* update bias of reference unit by average bias change */
		unit_ptr->bias += unit_ptr->value_b / unit_ptr->value_c;

		FOR_ALL_LINKS( unit_ptr, link_ptr ){ 
		    /* store average linkweigth changes of all Td-steps */
		    link_ptr->weight += link_ptr->value_b / link_ptr->value_c;
		} /*for_all_links*/
	    }
	    unit_ptr = *++topo_ptr;
	}
    }
    return( sum_error );/*  return the error of the network */
}




/*****************************************************************************
  FUNCTION : TEST_TDbackprop

  PURPOSE  : Time Delay Backpropagation testing Function
  NOTES    : Input Parameters:   2 : delta max

             Output Parameters:  1 : error of the network (sum of all cycles)

  RETURNS  : 
  UPDATE   : 21.01.95
******************************************************************************/
krui_err  TEST_TDbackprop( int start_pattern, int end_pattern, 
			   float parameterInArray[], int NoOfInParams,
			   float * *parameterOutArray, int *NoOfOutParams )

{
    static  float  OutParameter[1]; /* OutParameter[0] stores learning error */
    int     ret_code, pattern_no, sub_pat_no;

    if (NoOfInParams < 1)	/*  #  has to be changed (must be 2) # */
	return( KRERR_PARAMETERS ); /*  Not enough input parameters  */

    *NoOfOutParams = 1;	 /* One return value is available (the learning error)*/
    *parameterOutArray = OutParameter; /* set the output parameter reference  */
    ret_code = KRERR_NO_ERROR;  /*  reset return code  */


    /* compute the necessary sub patterns */

    KernelErrorCode = kr_initSubPatternOrder(start_pattern,end_pattern);
    if(KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);


    NET_ERROR(OutParameter) = 0.0;	/* reset network error value  */

    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){

	propagateTDNetForward(pattern_no,sub_pat_no); /* Forward propagation */

	/*  Backward propagation  */
	/*  1st parameter is the learning parameter
	    2nd parameter is the max. devitation between output pattern and
	    the output of the output unit (delta max)
	    */
	NET_ERROR( OutParameter ) +=
	    testTDNetBackward(pattern_no,sub_pat_no,
				   LEARN_PARAM1( parameterInArray ),
				   LEARN_PARAM2( parameterInArray ) );
    }

    return( ret_code );
}




/*****************************************************************************
  FUNCTION : testTDNetBackward

  PURPOSE  : Error Calculation for Time Delay Backward error propagation (topological).
  NOTES    : Start calculating the average of the corresponding links in 
             all TD-steps. This average is used to update the links of the 
	     1st. receptive field.
  RETURNS  : network error
  UPDATE   : 21.01.95
******************************************************************************/
static	float testTDNetBackward(int pattern_no, int sub_pat_no,
				     float learn_parameter, 
				     float delta_max)
{ 
    register struct Unit   *unit_ptr;
    register Patterns      out_pat;
    register float         sum_error,  eta,  devit;
    register TopoPtrArray  topo_ptr;
    int                    last_log_layer;
    int                    size;

    sum_error = 0.0;		/*  reset network error  */
    eta = learn_parameter;	/*  store learn_parameter in CPU register  */

    /*  calculate address of the output pattern (with number pattern_no + 1) */

    topo_ptr = topo_ptr_array + (no_of_topo_units + 2);
    last_log_layer = (*topo_ptr)->lln;
    out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
    out_pat += size;

    /* calculate output units only: begin at the end of topo_pointer_array */
    unit_ptr = *topo_ptr;
    while (unit_ptr != (struct Unit *) NULL){
	devit = *(--out_pat) - unit_ptr->Out.output; /*  calc. devitation */

/* der Fehler wird auch summiert, wenn die Gewichtsfaktoren nicht adaptiert werden */
	sum_error += devit * devit; /*  sum up the error of the network  */
	if ( (float) fabs( devit ) <= delta_max ){
            devit = 0; /* joe: sonst koennen Fehler auftreten*/
	}
	unit_ptr = *--topo_ptr;

    }

    return( sum_error );	/*  return the error of the network */
}


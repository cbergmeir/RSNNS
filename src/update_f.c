/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/update_f.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Network Update Functions
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 18.03.91

  CHANGED BY     : Sven Doering, Michael Vogt (Martin Reczko)
  RCS VERSION    : $Revision: 2.18 $
  LAST CHANGE    : $Date: 1998/03/13 16:24:06 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#include "kr_typ.h"	 /*  Kernel Types and Constants  */
#include "kr_const.h"	 /*  Constant Declarators for SNNS-Kernel  */
#include "kr_def.h"	 /*  Default Values  */
#include "kernel.h"	 /*  Kernel Functions  */
#include "glob_typ.h"
#include "kr_ui.h"
#include "kr_mem.h"	 /*  Memory Manager Functions  */
#include "random.h"	 /*  Randomize Library Function Prototypes  */
#include "kr_mac.h"	 /*  Kernel Macros   */
#include "krart_df.h"    /*  Macros and Definitions for ART */
#include "kr_art1.h"
#include "kr_art2.h"
#include "kr_amap.h"
#include "kr_art.h"      /*  Function Prototypes of ART kernel functions */
#include "kr_td.h"       /*  Function Prototypes of Time Delay functions */
#include "cc_glob.h"
#include "cc_mac.h"
#include "dlvq_learn.h"
#include "update_f.ph"
#include "kr_JordElm.h"
#include "func_mac.h" 
#include "kr_newpattern.h"

extern FlintType OUT_Custom_Python(FlintType act);

/*#################################################

GROUP: Update Functions

#################################################*/

/*****************************************************************************
  FUNCTION : UPDATE_syncPropagate

  PURPOSE  : synchronous propagation
  RETURNS  : 
  NOTES    :

  UPDATE   : 01.12.93
******************************************************************************/
krui_err  UPDATE_syncPropagate(float *parameterArray, int NoOfParams)
{
    register struct Unit   *unit_ptr;


    /*    update unit activations first  */
    FOR_ALL_UNITS( unit_ptr )
	if ( !IS_INPUT_UNIT( unit_ptr) && UNIT_IN_USE( unit_ptr ))
	    /*  unit isn't an input unit and is in use and enabled  */
	    unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

    /*    update unit outputs */
    FOR_ALL_UNITS( unit_ptr )
	if UNIT_IN_USE( unit_ptr )
	    /*  unit is in use and enabled  */
	    if (unit_ptr->out_func == OUT_IDENTITY)
		/*  identity output function: don't call the output function  */
		unit_ptr->Out.output = unit_ptr->act;
	    else if(unit_ptr->out_func == OUT_Custom_Python)
	    	unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	    else
		/* no identity output function: calculate unit's output also  */
		unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);

    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : UPDATE_serialPropagate

  PURPOSE  :  serial propagation
  RETURNS  : 
  NOTES    :

  UPDATE   : 01.12.93
******************************************************************************/
krui_err  UPDATE_serialPropagate(float *parameterArray, int NoOfParams)
{
    register struct Unit   *unit_ptr;


    /*    update unit activations and outputs */
    FOR_ALL_UNITS( unit_ptr )
	if UNIT_IN_USE( unit_ptr ){
	    /*  unit is in use and enabled  */
	    if (!IS_INPUT_UNIT( unit_ptr ))
		/*  this isn't a input unit: calculate the activation of 
		    the unit by calling the activation function   */
		unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

	    if (unit_ptr->out_func == OUT_IDENTITY)
		/*  identity output function: don't call the output function */
		unit_ptr->Out.output = unit_ptr->act;
	    else if(unit_ptr->out_func == OUT_Custom_Python)
	    	unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	    else
		/*  no identity output function: calculate unit's output also */
		unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	}

    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : UPDATE_randomOrderPropagate

  PURPOSE  : random order propagation
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_randomOrderPropagate(float *parameterArray, int NoOfParams)
{
    register struct Unit   *unit_ptr, *u_array;
    register int   no_of_units;
    int   n;


    u_array = unit_array;
    no_of_units = NoOfUnits;

    for (n = 0; n < no_of_units; n++){
	/*	  choose unit  */
	unit_ptr = u_array + (1 + lrand48() % no_of_units);

	if (!IS_INPUT_UNIT( unit_ptr ))
	    /*  this isn't a input unit: calculate the activation of the unit by
		calling the activation function */
	    unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /*  identity output function: don't call the output function  */
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else
	    /*  no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : UPDATE_randomPermutPropagate

  PURPOSE  : random permutation propagation
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_randomPermutPropagate(float *parameterArray, int NoOfParams)
{
    register struct Unit	 *unit_ptr;
    register TopoPtrArray  topo_ptr;
    int  ret_code;


    if (NetModified || (TopoSortID != PERMUTATION)){
	/*  networt was modified or permutation array isn't initialized  */
	ret_code = kr_makeUnitPermutation();
	if (ret_code != KRERR_NO_ERROR)
	    return( ret_code );
    }

    topo_ptr = topo_ptr_array;

    /*  propagate net  */
    while ((unit_ptr = *++topo_ptr) != NULL){
	if (!IS_INPUT_UNIT( unit_ptr ))
	    /*  this isn't a input unit: calculate the activation of the unit
		by calling the activation function */
	    unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /*  identity output function: don't call the output function  */
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else
	    /*  no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : UPDATE_topologicalPropagate

  PURPOSE  : Propagate Units in topological order
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_topologicalPropagate(float *parameterArray, int NoOfParams)
{
    register struct Unit  *unit_ptr;
    register TopoPtrArray  topo_ptr;
    int  ret_code;


    if (NetModified || (TopoSortID != TOPOLOGICAL_FF)){
	/*  networt was modified or topologic array isn't initialized  */
	ret_code = kr_topoSort( TOPOLOGICAL_FF );
	if (ret_code != KRERR_NO_ERROR)
	    return( ret_code );

	NetModified = FALSE;
    }


    topo_ptr = topo_ptr_array + 1;

    /*  propagate input units only  */
    while ((unit_ptr = *topo_ptr++) != NULL){
	/*  input units, don't call the activation function  */

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /*  identity output function: don't call the output function  */
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else
	    /*  no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    /*  propagate hidden units only  */
    while ((unit_ptr = *topo_ptr++) != NULL){
	unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /*  identity output function: don't call the output function  */
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else
	    /*  no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    /*  propagate output units only  */
    while ((unit_ptr = *topo_ptr++) != NULL){
	unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /*  identity output function: don't call the output function  */
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else
	    /*  no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : UPDATE_KohonenPropagate

  PURPOSE  : Propagate Units in topological order for Kohonen networks
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_KohonenPropagate(float *parameterArray, int NoOfParams)
{
    register struct Unit  *unit_ptr;
    register TopoPtrArray  topo_ptr;
    int  ret_code;


    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)){
	/*  networt was modified or topologic array isn't initialized  */
	ret_code = kr_topoSort( TOPOLOGIC_TYPE );
	if (ret_code == KRERR_NO_OUTPUT_UNITS) ret_code = KRERR_NO_ERROR;
	if (ret_code != KRERR_NO_ERROR)
	    return( ret_code );

	NetModified = FALSE;
    }


    topo_ptr = topo_ptr_array + 1;

    /*  propagate input units only  */
    while ((unit_ptr = *topo_ptr++) != NULL){
	if (unit_ptr->out_func == OUT_IDENTITY)
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else
	    /*  no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    /*  propagate hidden units only  */
    while ((unit_ptr = *topo_ptr++) != NULL){
	unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

	if (unit_ptr->out_func == OUT_IDENTITY)
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else
	    /*  no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }
    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : normalize_inputvector

  PURPOSE  : normalize input vector for Counterpropagation Update Function
  RETURNS  :  
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void normalize_inputvector(float sum)
{
    register struct Unit *unit_ptr;
    register float amount;


    amount = 1.0 / sqrt( sum );

    FOR_ALL_UNITS( unit_ptr )
	if (IS_INPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
	    /* this is a input unit */
	    unit_ptr->Out.output = unit_ptr->Out.output * amount;
}  



/*****************************************************************************
  FUNCTION : UPDATE_CPNPropagate

  PURPOSE  : Counterpropagation Update Function
  RETURNS  :  
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_CPNPropagate(float *parameterArray, int NoOfParams)
{
    register struct Unit   *unit_ptr, *winner_ptr;
    register struct Site   *site_ptr;
    register struct Link   *link_ptr;
    register TopoPtrArray  topo_ptr;

    float maximum, unit_ptr_net, sum; 
    int  ret_code;


    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)){
	/*  networt was modified or topologic array isn't initialized  */
	ret_code = kr_topoSort( TOPOLOGIC_TYPE );
	if (ret_code != KRERR_NO_ERROR)
	    return( ret_code );

	NetModified = FALSE;
    }

    topo_ptr = topo_ptr_array;
    sum = 0.0;

    /*  propagagate all input units  */
    while ((unit_ptr = *++topo_ptr) != NULL){
	/*  this is an input unit     */
	unit_ptr->Out.output = unit_ptr->act;
	sum += unit_ptr->Out.output * unit_ptr->Out.output;
    }
  
    if (sum != 0.0)
	/* normalize the inputvector */
	normalize_inputvector( sum );


    /* propagate Kohonen Layer */

    /* calculate the activation and the output values 
       of the hidden units (Kohonen Layer) */

    winner_ptr = NULL;
    maximum = -1.0e30;		/* contains the maximum of the activations */

    /*  propagagate all hidden units  */
    while ((unit_ptr = *++topo_ptr) != NULL){
	/* this is a hidden unit */
	unit_ptr_net = 0.0;

	if (unit_ptr->flags & UFLAG_SITES){
	    /* the unit has sites */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
		unit_ptr_net += (link_ptr->weight * link_ptr->to->Out.output);
	}else{			/* the unit has direct links */
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
		unit_ptr_net += (link_ptr->weight * link_ptr->to->Out.output);
	}

	if (maximum < unit_ptr_net){ /*  determine winner unit  */
	    winner_ptr = unit_ptr;
	    maximum = unit_ptr_net;
	}

	/* reset output and activation of hidden units  */
	unit_ptr->Out.output = unit_ptr->act = (FlintType) 0;
    }

    /* the competitive winner is chosen */
    winner_ptr->Out.output = winner_ptr->act = (FlintType) 1;


    /* propagate the Grossberg Layer */

    /*  propagagate all output units  */
    while ((unit_ptr = *++topo_ptr) != NULL){ /* this is a output unit */
	/* the activation function is the identity function ( weighted sum) */
	unit_ptr->Out.output = unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);
    }

    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : UPDATE_TimeDelayPropagate

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_TimeDelayPropagate(float parameterArray[], int NoOfParams )
{
    register struct Unit    *unit_ptr;
    register TopoPtrArray   topo_ptr;
    int                     ret_code;

    /* initialization if necessary */
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
	    return( KRERR_NET_DEPTH ); /*  the network has less then 2 layers  */

	/*  count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)  return( ret_code );

	ret_code = kr_topoSort( TOPOLOGIC_LOGICAL );
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return( ret_code );

	NetModified = FALSE;
    }

    topo_ptr = topo_ptr_array;
    unit_ptr = *++topo_ptr;

    /*  propagate input units only  */
    while (unit_ptr != (struct Unit *) NULL){
	/* input units doesn't have inputs, so don't call the 
	   activation function */

	if (unit_ptr->out_func == OUT_IDENTITY)
	    /*  identity output function: there is no need to call the 
		output function  */
	    unit_ptr->Out.output = unit_ptr->act;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	else
	    /*  no identity output function: calculate unit's output also  */
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	unit_ptr = *++topo_ptr;
    }

    /* use the propagation function of the learning function for the update */
    /* This way, the necessary time delay code is present only once */
    /* Use the special pattern_no -1, to prevent loading of a pattern */
    propagateTDNetForward(-1,-1);

    return (KRERR_NO_ERROR);  

} /* UPDATE_TimeDelayPropagate */





/*****************************************************************************
  FUNCTION : UPDATE_ART1_syncPropagate

  PURPOSE  : ART 1 update function which does exactly the same as the normal
             synchronous propagate function except that additionally the winner
	     of the ART 1 recognition layer is calculated.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_ART1_syncPropagate(float *parameterArray, int NoOfParams)
{
    krui_err     ret_code      = KRERR_NO_ERROR;

    int          i;
    struct Unit  *winner_ptr; /* recognition unit which is the winner of w.t.a*/
    struct Unit  *unit_ptr;
    TopoPtrArray topo_layer[6];	/* topo_layer[0] : *first input unit
				   topo_layer[1] : *first comp. unit
				   topo_layer[2] : *first rec.  unit
				   topo_layer[3] : *first delay unit
				   topo_layer[4] : *first local reset unit
				   topo_layer[5] : *first special unit
				   (classified_unit)*/
    TopoPtrArray topo_ptr;
    static float rho;
    bool         inp_pat_changed   = FALSE;
    bool         rho_has_changed   = FALSE;


    /* Check vigilance parameter */

    if (NoOfParams < 1) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    } /*if*/

    /* Check if rho has changed from last to actual call of this update function
       If rho has changed, then put new activation value into unit rho */
    if (rho != parameterArray[0]) {
	rho_has_changed = TRUE;
    }

    rho = parameterArray[0];

    if ((rho < 0.0) || (rho > 1.0)) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }


    /* Check if network has been modified or learning function has just
       been changed */

    if (NetModified || (TopoSortID != ART1_TOPO_TYPE)) {
	(void) kr_topoSort (ART1_TOPO_TYPE);
	ret_code = KernelErrorCode;
	if (ret_code != KRERR_NO_ERROR) {
	    NetModified = TRUE;
	    return (ret_code);
	} /*if*/

	NetModified = FALSE;
    }


    /* get pointers to resep. first elements of each layer in topo_ptr_array */

    topo_ptr = topo_ptr_array+1;

    for(i=0; i<=5; i++){
	topo_layer[i] = topo_ptr;
	do {
	} while (*topo_ptr++ != NULL);

    }


    /* Check if input pattern changed since last call to this function */
    if (krart_inp_pat_changed(topo_layer[0])) {
	inp_pat_changed = TRUE;
	krart_save_inp_pat(topo_layer[0]);
    }


    /* Push activation of input units to their output value.
       This is important for the first cycle. */

    topo_ptr = topo_layer[0];
    for (unit_ptr = *topo_ptr; *topo_ptr != NULL; unit_ptr = *topo_ptr++) {
	if (unit_ptr->out_func == OUT_IDENTITY) {
	    unit_ptr->Out.output = unit_ptr->act;
	} else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	} else {
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	}

    } 

    /* if rho had changed from last to this call of this update function then
       reinitialize the values of the i_act field of the unit structure and
       reset the activations of all non input units */
    if (rho_has_changed || inp_pat_changed) {
	ret_code = kra1_init_i_act (rho);
	if (ret_code != KRERR_NO_ERROR) 
	    return (ret_code);

	ret_code = krart_reset_activations ();
	if (ret_code != KRERR_NO_ERROR) 
	    return (ret_code);
    }

    /* 1 propagation step (all units push their information onto
       their output and calculate their new activation.*/

    krart_prop_synch ();

    /* look for the recognition unit with the highest activation
       returns a NULL pointer if all recognition units have
       activation 0.0 */
    winner_ptr = krart_get_winner (topo_layer[2],1.0);

    return (ret_code);

}



/*****************************************************************************
  FUNCTION : UPDATE_ART1_Propagate

  PURPOSE  : ART1 Update function for updating until a stable state is reached,
             e.g. either the 'classified' unit is on or the 'not classifiable' 
	     unit is.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_ART1_Propagate(float *parameterArray, int NoOfParams)
{
    krui_err     ret_code      = KRERR_NO_ERROR;
    int          i;
    struct Unit  *winner_ptr;	/* recogn. unit which is the winner of w.t.a */
    TopoPtrArray topo_layer[6];	/* topo_layer[0] : *first input unit
				   topo_layer[1] : *first comp. unit
				   topo_layer[2] : *first rec.  unit
				   topo_layer[3] : *first delay unit
				   topo_layer[4] : *first local reset unit
				   topo_layer[5] : *first special unit
				   (classified_unit) */
    TopoPtrArray  topo_ptr;
    float         rho;


    /* Check vigilance parameter */

    if (NoOfParams < 1) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }

    rho = parameterArray[0];

    if ((rho < 0.0) || (rho > 1.0)) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }


    /* Check if network has been modified or learning function has just
       been changed  */

    if (NetModified || (TopoSortID != ART1_TOPO_TYPE)) {
	(void) kr_topoSort (ART1_TOPO_TYPE);
	ret_code = KernelErrorCode;
	if (ret_code != KRERR_NO_ERROR) {
	    NetModified = TRUE;
	    return (ret_code);
	} 

	NetModified = FALSE;
    }

    ret_code = kra1_init_i_act (rho);

    if (ret_code != KRERR_NO_ERROR) {
	return (ret_code);
    }

    /* get pointers to resp. first elements of each layer in topo_ptr_array */

    topo_ptr = topo_ptr_array+1;

    for (i=0; i<=5; i++) {
	topo_layer[i] = topo_ptr;
	do {
	} while (*topo_ptr++ != NULL);
    } 

    /* initialize activations of non input units */

    ret_code = krart_reset_activations ();
    if (ret_code != KRERR_NO_ERROR) 
	return (ret_code);

    do {
	/* 1 propagation step (all units push their information onto
	   their output and calculate their new activation */

	krart_prop_synch ();

	/* look for the recognition unit with the highest activation
	   returns a NULL pointer if all recognition units have
	   activation 0.0 */
	winner_ptr = krart_get_winner (topo_layer[2],1.0);

    } while (!(ART1_CLASSIFIED) && !(ART1_NOT_CLASSIFIABLE));

    return (ret_code);

}




/*****************************************************************************
  FUNCTION : UPDATE_ART2_syncPropagate

  PURPOSE  : ART 2 update function which does exactly the same as the normal
             synchronous propagate function except that additionally the winner
	     of the ART 1 recognition layer is calculated.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_ART2_syncPropagate(float *parameterArray, int NoOfParams)
{
    krui_err     ret_code      = KRERR_NO_ERROR;
    int          i;
    struct Unit  *winner_ptr; /* recogn. unit which is the winner of w.t.a */
    struct Unit  *unit_ptr;
    TopoPtrArray topo_layer[12]; /* topo_layer[0] : *first input unit
                                    topo_layer[1] : *first w unit
				    topo_layer[2] : *first x unit
				    topo_layer[3] : *first u unit
				    topo_layer[4] : *first v unit
				    topo_layer[5] : *first p unit
				    topo_layer[6] : *first q unit
				    topo_layer[7] : *first r unit
				    topo_layer[8] : *first rec.  unit
				    topo_layer[9] : *first local reset unit */
    TopoPtrArray topo_ptr;
    static float rho, param_a, param_b, param_c, param_d, theta;
    bool         inp_pat_changed   = FALSE;
    bool         rho_has_changed   = FALSE;
    bool         a_has_changed     = FALSE;
    bool         b_has_changed     = FALSE;
    bool         c_has_changed     = FALSE;
    bool         theta_has_changed = FALSE;




    /* Check vigilance parameter */

    if (NoOfParams < 5) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    } 


    /* Check if input pattern had changed from last step to this one */


    /* Check if one of the parameters has changed from last to actual
       call of this update function.
       If so, then put new activation value into unit rho or change
       the weights of the relevant links. */
    if (rho != parameterArray[0]) 
	rho_has_changed = TRUE;

    if (param_a != parameterArray[1]) 
	a_has_changed = TRUE;

    if (param_b != parameterArray[2]) 
	b_has_changed = TRUE;

    if (param_c != parameterArray[3]) 
	c_has_changed = TRUE;

    if (theta != parameterArray[4]) 
	theta_has_changed = TRUE;

    rho     = parameterArray[0];
    param_a = parameterArray[1];
    param_b = parameterArray[2];
    param_c = parameterArray[3];
    theta   = parameterArray[4];


    /* Check if network has been modified */

    if (NetModified || (TopoSortID != ART2_TOPO_TYPE)) {
	(void) kr_topoSort (ART2_TOPO_TYPE);
	ret_code = KernelErrorCode;
	if (ret_code != KRERR_NO_ERROR) {
	    NetModified = TRUE;
	    return (ret_code);
	} 
	NetModified = FALSE;
    } 

    /* get pointers to resp. first elements of each layer in topo_ptr_array */

    topo_ptr = topo_ptr_array+1;

    for (i=0; i<=9; i++) {
	topo_layer[i] = topo_ptr;
	do {
	} while (*topo_ptr++ != NULL);
    } 

    /* Check if input pattern changed since last call to this function */
    if (krart_inp_pat_changed(topo_layer[0])) {
	inp_pat_changed = TRUE;
	krart_save_inp_pat(topo_layer[0]);
    } 


    /* Read out value of parameter d from bias field of any unit. The
       value has been written into the bias field by the init-function */
    param_d = (*(topo_ptr_array+1))->bias;


    /* Check values of the parameters */

    if ((rho < 0.0) || (rho > 1.0) || (param_a <= 0.0) || (param_b <= 0.0) ||
	((param_c*param_d)/(1-param_d) > 1.0) ||(theta < 0.0) || (theta > 1.0)){
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    } 


    /* if one of the parameters had changed from last to this call
       of this update function then reinitialize the values of the i_act 
       field of the unit structure, set the weights of the relevant links and
       reset the activations of all non input units */
    if (rho_has_changed || a_has_changed || b_has_changed ||
	c_has_changed || theta_has_changed || inp_pat_changed){

	ret_code = kra2_set_params (rho,param_a,param_b,param_c,param_d,theta);

	if (ret_code != KRERR_NO_ERROR) 
	    return (ret_code);

	ret_code = kra2_init_propagate();

	if (ret_code != KRERR_NO_ERROR) 
	    return (ret_code);

	kra2_init_pattern();
    }


    /* Push activation of input units to their output value.
       This is important for the first cycle. */

    topo_ptr = topo_layer[ART2_INP_LAY-1];
    unit_ptr = *topo_ptr;
    while (unit_ptr != NULL) {
	if (unit_ptr->out_func == OUT_IDENTITY) {
	    unit_ptr->Out.output = unit_ptr->act;
	} else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	} else {
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	}
	topo_ptr++;
	unit_ptr = *topo_ptr;
    }

    /* compute vector norms */
    kra2_compute_norms();

    /* save old activation values of f1-units */
    kra2_save_for_stability_check ();

    /* Propagate */
    krart_prop_synch ();

    /* Get winner */
    winner_ptr = krart_get_winner (topo_layer[ART2_REC_LAY-1], param_d);

    /* Check F1 stability */
    kra2_check_f1_stability ();

    /* Check reset */
    kra2_checkReset ();

    return (ret_code);

}



/*****************************************************************************
  FUNCTION : UPDATE_ART2_Propagate 

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_ART2_Propagate(float *parameterArray, int NoOfParams)
{
    krui_err     ret_code      = KRERR_NO_ERROR;
    int          i;
    struct Unit  *winner_ptr; /* recogn. unit which is the winner of w.t.a */
    TopoPtrArray topo_layer[12]; /* topo_layer[0] : *first input unit
                                          topo_layer[1] : *first w unit
                                          topo_layer[2] : *first x unit
                                          topo_layer[3] : *first u unit
                                          topo_layer[4] : *first v unit
                                          topo_layer[5] : *first p unit
                                          topo_layer[6] : *first q unit
                                          topo_layer[7] : *first r unit
                                          topo_layer[8] : *first rec. unit
                                          topo_layer[10] : *first local reset
					                   unit */
    TopoPtrArray topo_ptr;
    FlintType    rho, param_a, param_b, param_c, param_d, theta;


    /* Check number of incoming parameters */

    if (NoOfParams < 5) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    } 

    rho     = parameterArray[0];
    param_a = parameterArray[1];
    param_b = parameterArray[2];
    param_c = parameterArray[3];
    theta   = parameterArray[4];



    /* Check if network has been modified or learning function has just
       been changed */

    if (NetModified || (TopoSortID != ART2_TOPO_TYPE)) {
	(void) kr_topoSort (ART2_TOPO_TYPE);
	ret_code = KernelErrorCode;
	if (ret_code != KRERR_NO_ERROR) {
	    NetModified = TRUE;
	    return (ret_code);
	} 

	NetModified = FALSE;
    } 


    /* Read out value of parameter d from bias field of any unit. The
       value has been written into the bias field by the init-function */
    param_d = (*(topo_ptr_array+1))->bias;


    /* Check values of the parameters */

    if ((rho < 0.0) || (rho > 1.0) ||(param_a <= 0.0) || (param_b <= 0.0) ||
	((param_c*param_d)/(1-param_d)>1.0) || (theta<0.0) || (theta>1.0)){
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }

    ret_code = kra2_set_params (rho, param_a, param_b, param_c, param_d, theta);

    if (ret_code != KRERR_NO_ERROR) 
	return (ret_code);

    ret_code = kra2_init_propagate ();

    if (ret_code != KRERR_NO_ERROR) 
	return (ret_code);

    /* get pointers to resp. first elements of each layer in topo_ptr_array */
    topo_ptr = topo_ptr_array+1;

    for (i=0; i<=9; i++){
	topo_layer[i] = topo_ptr;
	do {
	} while (*topo_ptr++ != NULL);
    }


    /* initialize the unit activations of the whole net */
    ret_code = krart_reset_activations();
    if (ret_code != KRERR_NO_ERROR)
	return (ret_code);

    /* initialize of ART2 Simulator for new pattern */
    kra2_init_pattern ();


    /* repeat synchronous propagation and look for winner until pattern is
       classified or network tells us, that pattern is not classifiable */
    do {
	/* compute vector norms */
	kra2_compute_norms();

	/* save old activation values of f1-units */
	kra2_save_for_stability_check ();

	/* 1 propagation step (all units push their information onto
	   their output and calculate their new activation.  */
	krart_prop_synch ();

	/* look for the recognition unit with the highest activation returns
	   a NULL pointer if all recognition units have activation 0.0 */
	winner_ptr = krart_get_winner (topo_layer[ART2_REC_LAY-1], param_d);

	/* Check if F1-Layer is stable */
	kra2_check_f1_stability();

	/* Check Reset */
	kra2_checkReset ();

    } while (!(ART2_CLASSIFIED) && !(ART2_NOT_CLASSIFIABLE));

    return (ret_code);
}


/*****************************************************************************
  FUNCTION : UPDATE_ARTMAP_syncPropagate

  PURPOSE  : ARTMAP update function which does exactly the same as the normal
             synchronous propagate function except that additionally the winner
	     of the ARTMAP recognition layer is calculated.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_ARTMAP_syncPropagate(float *parameterArray, int NoOfParams)
{
    krui_err     ret_code      = KRERR_NO_ERROR;
    int          i;
    struct Unit  *winner_ptr_a;	/* the winner of wta of ARTa */
    struct Unit  *winner_ptr_b;	/* the winner of w.t.a of ARTb */
    struct Unit  *unit_ptr;
    TopoPtrArray topo_layer[14]; /* topo_layer[0] : *first input unit ARTa
                                    topo_layer[1] : *first comp. unit ARTa
				    topo_layer[2] : *first rec.  unit ARTa
				    topo_layer[3] : *first delay unit ARTa
				    topo_layer[4] : *first local reset unit ARTa
				    topo_layer[5] : *first special unit ARTa
				    (classified_unit)
				    topo_layer[6] : *first input unit ARTb
				    topo_layer[7] : *first comp. unit ARTb
				    topo_layer[8] : *first rec.  unit ARTb
				    topo_layer[9] : *first delay unit ARTb
				    topo_layer[10]: *first local reset unit ARTb
				    topo_layer[11]: *first special unit ARTb
				    (classified_unit)
                                    topo_layer[12]: *first map unit
				    topo_layer[13]: *first special map unit */
    TopoPtrArray topo_ptr;
    static float rho_a = -1.0;
    static float rho_b = -1.0;
    static float rho   = -1.0;
    bool         inp_pat_changed   = FALSE;
    bool         rho_has_changed   = FALSE;


    /* Check vigilance parameter */
    if (NoOfParams < 3) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    } 

    /* Check if rho has changed from last to actual call of this update function
       If rho has changed, then put new activation value into unit rho */
    if ((rho_a != parameterArray[0]) || (rho_b != parameterArray[1]) ||
	(rho   != parameterArray[2]))
	rho_has_changed = TRUE;

    rho_a = parameterArray[0];
    rho_b = parameterArray[1];
    rho   = parameterArray[2];


    if((rho_a<0.0) || (rho_a>1.0) || (rho_b<0.0) || (rho_b>1.0) ||
       (rho<0.0) || (rho>1.0)){
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    }


    /* Check if network has been modified or learn func has just been changed */

    if (NetModified || (TopoSortID != ARTMAP_TOPO_TYPE)) {
	(void) kr_topoSort (ARTMAP_TOPO_TYPE);
	ret_code = KernelErrorCode;
	if (ret_code != KRERR_NO_ERROR) {
	    NetModified = TRUE;
	    return (ret_code);
	} 

	NetModified = FALSE;
    }


    /* get pointers to resp. first elements of each layer in topo_ptr_array */
    topo_ptr = topo_ptr_array+1;

    for (i=0; i<=13; i++) {
	topo_layer[i] = topo_ptr;
	do {
	} while (*topo_ptr++ != NULL);
    }


    /* Check if input pattern changed since last call to this function */
    if (krart_inp_pat_changed(topo_layer[0]) ||
	krart_inp_pat_changed(topo_layer[6])){
	inp_pat_changed = TRUE;
	krart_save_inp_pat(topo_layer[0]);
	krart_save_inp_pat(topo_layer[6]);
    } 


    /* Push activation of input units to their output value.
       This is important for the first cycle. */

    /* inpa - units */
    topo_ptr = topo_layer[0];
    for (unit_ptr = *topo_ptr; *topo_ptr != NULL; unit_ptr = *++topo_ptr) {
	if (unit_ptr->out_func == OUT_IDENTITY) {
	    unit_ptr->Out.output = unit_ptr->act;
	} else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	} else {
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	} 
    } 

    topo_ptr = topo_layer[6];
    for (unit_ptr = *topo_ptr; *topo_ptr != NULL; unit_ptr = *++topo_ptr) {
	if (unit_ptr->out_func == OUT_IDENTITY) {
	    unit_ptr->Out.output = unit_ptr->act;
	} else if(unit_ptr->out_func == OUT_Custom_Python) {
		unit_ptr->Out.output = 
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	} else {
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	}
    }

    /* if rho or input pattern had changed from last to this call of this
       update function then reinitialize the values of the i_act field of the 
       unit structure and reset the activations of all non input units */
    if (rho_has_changed || inp_pat_changed) {

	ret_code = kram_init_i_act (rho_a, rho_b, rho);
	if (ret_code != KRERR_NO_ERROR)
	    return (ret_code);

	ret_code = krart_reset_activations ();
	if (ret_code != KRERR_NO_ERROR) 
	    return (ret_code);
    } 

    /* 1 propagation step (all units push their information onto
       their output and calculate their new activation. */

    krart_prop_synch ();

    /* look for the recognition unit with the highest activation returns a 
       NULL pointer if all recognition units have activation 0.0 */
    winner_ptr_a = krart_get_winner (topo_layer[2],1.0);
    winner_ptr_b = krart_get_winner (topo_layer[8],1.0);

    return (ret_code);

}


/*****************************************************************************
  FUNCTION : UPDATE_ARTMAP_Propagate

  PURPOSE  : ARTMAP Update function for updating until a stable state is 
             reached, e.g. either the 'classified' unit is on or the 'not 
	     classifiable' unit is.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_ARTMAP_Propagate(float *parameterArray, int NoOfParams)
{
    krui_err     ret_code      = KRERR_NO_ERROR;
    int          i;
    struct Unit  *winner_ptr_a;	/* the winner of w.t.a of ARTa */
    struct Unit  *winner_ptr_b;	/* the winner of w.t.a of ARTb */
    TopoPtrArray topo_layer[14]; /* topo_layer[0] : *first input unit ARTa
                                    topo_layer[1] : *first comp. unit ARTa
				    topo_layer[2] : *first rec.  unit ARTa
				    topo_layer[3] : *first delay unit ARTa
				    topo_layer[4] : *first local reset unit ARTa
				    topo_layer[5] : *first special unit ARTa
				    (classified_unit)
                                    topo_layer[6] : *first input unit ARTb
				    topo_layer[7] : *first comp. unit ARTb
				    topo_layer[8] : *first rec.  unit ARTb
				    topo_layer[9] : *first delay unit ARTb
				    topo_layer[10]: *first local reset unit ARTb
				    topo_layer[11]: *first special unit ARTb
				    (classified_unit)
                                    topo_layer[12]: *first map unit
				    topo_layer[13]: *first special map unit */
    TopoPtrArray topo_ptr;
    float        rho_a;
    float        rho_b;
    float        rho;


    /* Check vigilance parameters */

    if (NoOfParams < 3) {
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    } 

    rho_a = parameterArray[0];
    rho_b = parameterArray[1];
    rho   = parameterArray[2];

    if ((rho_a < 0.0) || (rho_a > 1.0) || (rho_b < 0.0) ||
	(rho_b > 1.0) || (rho   < 0.0) || (rho   > 1.0)){
	ret_code = KRERR_PARAMETERS;
	return (ret_code);
    } 


    /* Check if network has been modified or learn func has just been changed */
    if (NetModified || (TopoSortID != ARTMAP_TOPO_TYPE)) {
	(void) kr_topoSort (ARTMAP_TOPO_TYPE);
	ret_code = KernelErrorCode;
	if (ret_code != KRERR_NO_ERROR) {
	    NetModified = TRUE;
	    return (ret_code);
	} 

	NetModified = FALSE;
    }


    ret_code = kram_init_i_act (rho_a, rho_b, rho);

    if (ret_code != KRERR_NO_ERROR) 
	return (ret_code);

    /* get pointers to resp. first elements of each layer in topo_ptr_array */
    topo_ptr = topo_ptr_array+1;

    for (i=0; i<=13; i++) {
	topo_layer[i] = topo_ptr;
	do {
	} while (*topo_ptr++ != NULL);
    } 

    /* initialize activations of non input units */
    ret_code = krart_reset_activations ();
    if (ret_code != KRERR_NO_ERROR) 
	return (ret_code);

    do {
	/* 1 propagation step (all units push their information onto
	   their output and calculate their new activation. */
	krart_prop_synch ();

	/* look for the recognition unit with the highest activation
	   returns a NULL pointer if all recognition units have
	   activation 0.0 */
	winner_ptr_a = krart_get_winner (topo_layer[2],1.0);
	winner_ptr_b = krart_get_winner (topo_layer[8],1.0);

    } while (!(ARTMAP_CLASSIFIED) && !(ARTMAP_NOT_CLASSIFIABLE));

    return (ret_code);

}



/*****************************************************************************
  FUNCTION : UPDATE_CC_Propagate

  PURPOSE  : Propagates a pattern through the net after pressing the test 
             button.
  NOTES    :

  UPDATE   : 5.2.93
******************************************************************************/
krui_err UPDATE_CC_Propagate(float parameterArray[],  int NoOfParams)
{
    register struct Unit  *inputUnitPtr,*outputUnitPtr,*hiddenUnitPtr,*unitPtr;
    register int dummy,o;
  
    if(NetModified || LearnFuncHasChanged) {
    
	NoOfInputUnits = NoOfHiddenUnits = NoOfOutputUnits = 0;
	FOR_ALL_UNITS(unitPtr) {
	    if(IS_INPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
		NoOfInputUnits++;
	    }
	    if(IS_HIDDEN_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
		NoOfHiddenUnits++;
	    }
	    if(IS_OUTPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
		NoOfOutputUnits++;
	    }
	}
	KernelErrorCode = cc_deleteAllSpecialUnits();
	ERROR_CHECK; 

	KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
	ERROR_CHECK; 

	KernelErrorCode = cc_setPointers();
	ERROR_CHECK;

	NetModified = FALSE;
	LearnFuncHasChanged = FALSE;
    }

    FOR_ALL_INPUT_UNITS(inputUnitPtr,dummy){
	if(inputUnitPtr->out_func == OUT_IDENTITY) {
	    inputUnitPtr->Out.output = inputUnitPtr->act;
	} else if(inputUnitPtr->out_func == OUT_Custom_Python) {
		inputUnitPtr->Out.output = 
			kr_PythonOutFunction(inputUnitPtr->python_out_func,
				inputUnitPtr->act);
	}else{
	    inputUnitPtr->Out.output = 
		(*inputUnitPtr->out_func) (inputUnitPtr->act);
	}
    }

    FOR_ALL_HIDDEN_UNITS(hiddenUnitPtr,dummy) {
	hiddenUnitPtr->act = (*hiddenUnitPtr->act_func) (hiddenUnitPtr);
	if(hiddenUnitPtr->out_func == OUT_IDENTITY) {
	    hiddenUnitPtr->Out.output = hiddenUnitPtr->act;
	} else if(hiddenUnitPtr->out_func == OUT_Custom_Python) {
		hiddenUnitPtr->Out.output =
		    kr_PythonOutFunction(hiddenUnitPtr->python_out_func,
		    			hiddenUnitPtr->act);
	}else{
	    hiddenUnitPtr->Out.output = 
		(*hiddenUnitPtr->out_func) (hiddenUnitPtr->act);
	}
    }
    
    FOR_ALL_OUTPUT_UNITS(outputUnitPtr,o) {
	outputUnitPtr->act = (*outputUnitPtr->act_func) (outputUnitPtr);
	if(outputUnitPtr->out_func == OUT_IDENTITY) {
	    outputUnitPtr->Out.output = outputUnitPtr->act;
	}else if(outputUnitPtr->out_func == OUT_Custom_Python){
		outputUnitPtr->Out.output =
			kr_PythonOutFunction(outputUnitPtr->python_out_func,
				outputUnitPtr->act);
	}else{
	    outputUnitPtr->Out.output = 
		(*outputUnitPtr->out_func) (outputUnitPtr->act);
	}
    }
    return(KRERR_NO_ERROR);
}



/*****************************************************************************
  FUNCTION : UPDATE_DLVQ_Propagate

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err UPDATE_DLVQ_Propagate(float parameterArray[], int NoOfParams)
{
    struct Unit *inputUnitPtr,*hiddenUnitPtr,*maxActivatedUnitPtr=NULL;
    double maxAct,act;
    int i,h,startPattern,endPattern,d1,d2,d3,generatedNewUnit,noOfLinks;

    if(newPatternsLoaded){
	newPatternsLoaded = 0;
	startPattern = 0;
/*	endPattern = krui_getNoOfPatterns()-1;*/
	endPattern = kr_TotalNoOfSubPatPairs()-1;
	KernelErrorCode = getNoOfClasses(startPattern,endPattern);
	ERROR_CHECK;

	normPatterns(startPattern,endPattern);
	allocInitialUnitArray();
	initInitialUnitArray(startPattern,endPattern);
    }

    if(NetModified || LearnFuncHasChanged) {
	NoOfInputUnits = NoOfHiddenUnits = NoOfOutputUnits = 0;
	FOR_ALL_UNITS(unitPtr) {
	    if(IS_INPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
		NoOfInputUnits++;
	    }
	    if(IS_HIDDEN_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
		NoOfHiddenUnits++;
	    }
	    if(IS_OUTPUT_UNIT(unitPtr) && UNIT_IN_USE(unitPtr)) {
		NoOfOutputUnits++;
	    }
	}
	if(NoOfOutputUnits != 1){
	    return(DLVQ_ERROR3); /* Wrong no. of output units */
	}
	allocArrays();
	KernelErrorCode = kr_topoSort(TOPOLOGICAL_FF);
	ERROR_CHECK;    

	KernelErrorCode = dlvq_setPointers();
	ERROR_CHECK; 

	krui_getNetInfo(&d1,&noOfLinks,&d2,&d3);
	if(noOfLinks != NoOfInputUnits * NoOfHiddenUnits + NoOfHiddenUnits) {
	    return(DLVQ_ERROR4); /* wrong topology */
	}

	generateMissingClassHiddenUnits(&generatedNewUnit);
	if(generatedNewUnit) {
	    return(DLVQ_ERROR5); /* There is not a class for every unit */
	}
	NetModified = FALSE;
	LearnFuncHasChanged = FALSE;
    }

    FOR_ALL_INPUT_UNITS(inputUnitPtr,i){
	inputUnitPtr->Out.output = inputUnitPtr->act;
    }
  
    maxAct = -1.0;
 
    FOR_ALL_HIDDEN_UNITS(hiddenUnitPtr,h) {
	hiddenUnitPtr->Out.output = hiddenUnitPtr->act = act = 0.0;
	FOR_ALL_LINKS(hiddenUnitPtr,linkPtr) {
	    act += linkPtr->weight * linkPtr->to->Out.output;
	}
	if(maxAct < act){
	    maxAct = act;
	    maxActivatedUnitPtr = hiddenUnitPtr;
	}
    }

    maxActivatedUnitPtr->Out.output = maxActivatedUnitPtr->act = 1.0;
    (*FirstOutputUnitPtr)->Out.output = 
	(*FirstOutputUnitPtr)->act = maxActivatedUnitPtr->bias;
    return(KRERR_NO_ERROR);
} 



/*****************************************************************************
  FUNCTION : UPDATE_BPTT

  PURPOSE  : Backpropagation through time synchronous order using activity 
             buffer for each unit.
  RETURNS  : 
  NOTES    : The "TEST" button in the remote panel first increases the pattern 
             number, copies the input pattern to the input units and, 
	     depending on the setting of the "SHOW" button,
	        - does not copy the output pattern with setting "none"
		- copies the output pattern to unit_ptr->act with setting 
		  "activation"
		- copies the output pattern to unit_ptr->act and 
		  unit_ptr->Out.output with setting "output"
	     An all-zero-input pattern for reset is only effective using 
	     "TEST" if the current pattern is the pattern immediatly before 
	     the reset pattern.
  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_BPTT(float *parameterArray, int NoOfParams)
{
    krui_err ret_code;
    register struct Unit   *unit_ptr;
    register TopoPtrArray  topo_ptr;
    register TopoPtrArray  first_hidden_ptr;
    int all_zero_input=1;	/* flag to reset net-copies */
    int done_hidden;

    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)){
	/* Net has been modified or topologic array isn't initialized */
	/* any connected topology allowed */
	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)  
	    return( ret_code );
      
	/* sort units by ''topologic type'',
	   criterion is visibility (input,hidden,output), not topology */
	ret_code = kr_topoSort( TOPOLOGIC_TYPE );
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return( ret_code );
      
	NetModified = FALSE;
    }
  

    /* check all zero pattern in input layer => reset net_activities */
    topo_ptr = topo_ptr_array;

    while ((unit_ptr = *++topo_ptr) != NULL) {
	unit_ptr->Out.output = unit_ptr->act;
	if(fabs(unit_ptr->act)>0.0001) all_zero_input = 0; /* no reset-input */
    }
    first_hidden_ptr = topo_ptr;

    if (all_zero_input) {	/* clear netact-copies */
	FOR_ALL_UNITS( unit_ptr ) unit_ptr->i_act = 0.0;
    }

    /* copy last unit_ptr->i_act to unit_ptr->Out.output */
    /*  one step back in time, make most recent activity
	visible in unit_ptr->Out.output for subsequent calls to act_func */

    while ((unit_ptr = *++topo_ptr) != NULL) { /* hidden layer */
	unit_ptr->Out.output = unit_ptr->i_act; }

    while ((unit_ptr = *++topo_ptr) != NULL) { /* output layer */
	unit_ptr->Out.output = unit_ptr->i_act; }

    /*  calculate new activities for hidden and output units */
    /* point to first hidden unit */
    topo_ptr = first_hidden_ptr;
    done_hidden=0;
    while ( ((unit_ptr = *++topo_ptr) != NULL) || (done_hidden==0))
	if (unit_ptr == NULL) {
	    done_hidden = 1;
	}else{  
	    /* calc act using i_act copied to Out.output,  SYNCHRONOUS UPDATE:
	       don't update Out.output while updating units, wait until all 
	       units are processed  */
	    unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);
	}

    /*  calculate new Out.output values from act by calling out_func,
	and save values in i_act (since they may be disturbed by show pattern)*/

    /* point to first hidden unit */
    topo_ptr = first_hidden_ptr;
    done_hidden=0;
    while ( ((unit_ptr = *++topo_ptr) != NULL) || (done_hidden==0))
	if (unit_ptr == NULL) {
	    done_hidden = 1;
	}else{  
	    if (unit_ptr->out_func == OUT_IDENTITY) {
		unit_ptr->Out.output = unit_ptr->act;
	    }else if(unit_ptr->out_func == OUT_Custom_Python){
	    	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	    }else{
		unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	    }
	    unit_ptr->i_act = unit_ptr->Out.output;
	}

    return( KRERR_NO_ERROR );
} 



/*****************************************************************************
  FUNCTION : UPDATE_BAM

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_BAM(float *parameterArray, int NoOfParams)
{
    krui_err ret_code;
    register struct Unit   *unit_ptr;
    register TopoPtrArray  topo_ptr;
    register TopoPtrArray  first_hidden_ptr;
    int done_hidden;
    FlintType new_output;

    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)){
	/* Net has been modified or topologic array isn't initialized */
	/* any connected topology allowed */
	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)  
	    return( ret_code );
      
	/* sort units by ''topologic type'',
	   criterion is visibility (input,hidden,output), not topology */
	ret_code = kr_topoSort( TOPOLOGIC_TYPE );
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return( ret_code );
      
	NetModified = FALSE;
    }

    /* Search hidden Units */  
    topo_ptr = topo_ptr_array;
    while ((unit_ptr = *++topo_ptr) != NULL) {
    }
    first_hidden_ptr = topo_ptr;

    /*  calculate new Out.output values from act by calling out_func */
    /* point to first hidden unit and remember the old ones*/
    topo_ptr = first_hidden_ptr;
    done_hidden=0;
    while ( ((unit_ptr = *++topo_ptr) != NULL) || (done_hidden==0))
	if (unit_ptr == NULL) {
	    done_hidden = 1;
	}else{  
	    unit_ptr->value_a = unit_ptr->Out.output;
	    if (unit_ptr->out_func == OUT_IDENTITY) {
		unit_ptr->Out.output = unit_ptr->act;
	    }else if(unit_ptr->out_func == OUT_Custom_Python){
	    	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	    }else{
		unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
	    }
	}


    /*  calculate new activities for hidden and output units */
    /* point to first hidden unit */
    topo_ptr = first_hidden_ptr;
    done_hidden=0;
    while ( ((unit_ptr = *++topo_ptr) != NULL) || (done_hidden==0))
	if (unit_ptr == NULL) {
	    done_hidden = 1;
	}else{ 
	    /* save new value and restore old value from output */
	    new_output = unit_ptr->Out.output; 
	    unit_ptr->Out.output = unit_ptr->value_a; 

	    /* calc act */
	    unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

	    /* restore new value */
	    unit_ptr->Out.output = new_output;
	}

    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : UPDATE_JE_Propagate 

  PURPOSE  : update function for JORDAN / ELMAN networks
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_JE_Propagate (float *parameterArray, int NoOfParams)
{
    register struct Unit  *unit_ptr ;
    register TopoPtrArray  topo_ptr, help_ptr ;
    int      ret_code, i ;


    if (NetModified || (TopoSortID != TOPOLOGICAL_JE)){  
	/* network was modified or topologic array isn't initialized */

	ret_code = kr_topoCheckJE () ;
	if (ret_code != KRERR_NO_ERROR) return (ret_code) ;

	ret_code = kr_topoSort (TOPOLOGICAL_JE) ;
	if (ret_code != KRERR_NO_ERROR) return (ret_code) ;
 
	NetModified = FALSE ;
    }

    topo_ptr = topo_ptr_array ;


    /*  calculate output of input units */

    while ((unit_ptr = *++topo_ptr) != NULL){
	if (unit_ptr->out_func == OUT_IDENTITY)
	    unit_ptr->Out.output = unit_ptr->act ;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act) ;
	else
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act) ;
    }


    /* propagate hidden and output units */

    for (i = 0 ; i < 2 ; i++){
	while ((unit_ptr = *++topo_ptr) != NULL){
	    unit_ptr->act = (*unit_ptr->act_func) (unit_ptr) ;

	    if (unit_ptr->out_func == OUT_IDENTITY)
		unit_ptr->Out.output = unit_ptr->act ;
	    else if(unit_ptr->out_func == OUT_Custom_Python)
	    	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act) ;
	    else
		unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act) ;
	}
    }


    /* update of context units */

    help_ptr = topo_ptr ;

    while ((unit_ptr = *++help_ptr) != NULL){ 
	unit_ptr->act = (*unit_ptr->act_func) (unit_ptr) ;
    }

    while ((unit_ptr = *++topo_ptr) != NULL){
	if (unit_ptr->out_func == OUT_IDENTITY)
	    unit_ptr->Out.output = unit_ptr->act ;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act) ;
	else
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act) ;
    }

    return (KRERR_NO_ERROR) ;
}



/*****************************************************************************
  FUNCTION : UPDATE_JE_Special

  PURPOSE  : update function with dynamic pattern generation for JORDAN / 
             ELMAN networks
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_JE_Special (float *parameterArray, int NoOfParams)
{
    register struct Unit  *unit_ptr  ;
    register TopoPtrArray  topo_ptr, help_ptr ;
    int      ret_code, i ;


    if (NetModified || (TopoSortID != TOPOLOGICAL_JE)){  
	/*  network was modified or topologic array isn't initialized */

	ret_code = kr_topoCheckJE () ;
	if (ret_code != KRERR_NO_ERROR) return (ret_code) ;

	ret_code = kr_topoSort (TOPOLOGICAL_JE) ;
	if (ret_code != KRERR_NO_ERROR) return (ret_code) ;
 
	NetModified = FALSE ;
    }

    if (NoOfInputUnits < NoOfOutputUnits) return (-1) ;


    /* create new input pattern from the output of input and output units */ 

    help_ptr = topo_ptr_array ;
    while (*++help_ptr != NULL) ; /* skip input  units */
    while (*++help_ptr != NULL) ; /* skip hidden units */
  
    topo_ptr = topo_ptr_array ;

    for (i = 1 ; i <= NoOfInputUnits ; i++)
	if (i <= NoOfInputUnits - NoOfOutputUnits)
	    (*(topo_ptr+i))->act = (*(topo_ptr+i+NoOfOutputUnits))->Out.output;
	else
	    (*(topo_ptr+i))->act = (*++help_ptr)->Out.output ;

    topo_ptr = topo_ptr_array ;


    /* calculate output of input units */

    while ((unit_ptr = *++topo_ptr) != NULL){
	if (unit_ptr->out_func == OUT_IDENTITY)
	    unit_ptr->Out.output = unit_ptr->act ;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act) ;
	else
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act) ;
    }


    /* propagate hidden and output units  */

    for (i = 0 ; i < 2 ; i++){
	while ((unit_ptr = *++topo_ptr) != NULL){
	    unit_ptr->act = (*unit_ptr->act_func) (unit_ptr) ;

	    if (unit_ptr->out_func == OUT_IDENTITY)
		unit_ptr->Out.output = unit_ptr->act ;
	    else if(unit_ptr->out_func == OUT_Custom_Python)
	    	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act) ;
	    else
		unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act) ;
	}
    }


    /* synchronous update of context units */

    help_ptr = topo_ptr ;

    while ((unit_ptr = *++help_ptr) != NULL){ 
	unit_ptr->act = (*unit_ptr->act_func) (unit_ptr) ;
    }

    while ((unit_ptr = *++topo_ptr) != NULL){
	if (unit_ptr->out_func == OUT_IDENTITY)
	    unit_ptr->Out.output = unit_ptr->act ;
	else if(unit_ptr->out_func == OUT_Custom_Python)
		unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act) ;
	else
	    unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act) ;
    }

    return (KRERR_NO_ERROR) ;
}



/*****************************************************************************
  FUNCTION : UPDATE_syncPropagateHop

  PURPOSE  : synchronous propagation for Hopfield 
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_syncPropagateHop(float *parameterArray, int NoOfParams)
{
    register struct Unit   *unit_ptr; 

    /* update unit outputs first, because the patterns set only          */
    /* the activations of the input units, and they would be overwritten */
    FOR_ALL_UNITS( unit_ptr ) {
	if UNIT_IN_USE(unit_ptr) {
	    if (unit_ptr->out_func == OUT_IDENTITY) {
		unit_ptr->Out.output = unit_ptr->act;
	    }else if(unit_ptr->out_func == OUT_Custom_Python){ /* the default way */
	    	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	    }else{ /* the default way */
		unit_ptr->Out.output = (*unit_ptr->out_func)(unit_ptr->act);
	    }
	}
    }

    /* update unit activations second */

    /* first non input, then input units, so function can be used for BAM too */
    FOR_ALL_UNITS( unit_ptr ){
	if (UNIT_IN_USE(unit_ptr)&&!IS_INPUT_UNIT(unit_ptr))
	    unit_ptr->act  = (*unit_ptr->act_func) (unit_ptr);
    } 

    /* output update of non input units (for resultfile) */
    FOR_ALL_UNITS( unit_ptr ) {
	if (UNIT_IN_USE(unit_ptr) && !IS_INPUT_UNIT(unit_ptr)) {
	    if (unit_ptr->out_func == OUT_IDENTITY) { 
		unit_ptr->Out.output = unit_ptr->act;
	    }else if(unit_ptr->out_func == OUT_Custom_Python){ /* the default way */
	    	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	    }else{ /* the default way */
		unit_ptr->Out.output = (*unit_ptr->out_func)(unit_ptr->act);
	    }
	}
    }

    /* update input units */

    FOR_ALL_UNITS( unit_ptr ){
	if (UNIT_IN_USE(unit_ptr)&&IS_INPUT_UNIT(unit_ptr))
	    unit_ptr->act  = (*unit_ptr->act_func) (unit_ptr);
    } 

    /* output update of input units (for resultfile) */

    FOR_ALL_UNITS( unit_ptr ) {
	if (UNIT_IN_USE(unit_ptr) && IS_INPUT_UNIT(unit_ptr)) {
	    if (*unit_ptr->out_func == OUT_IDENTITY) { 
		unit_ptr->Out.output = unit_ptr->act;
	    }else if(unit_ptr->out_func == OUT_Custom_Python){ /* the default way */
	    	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	    }else{ /* the default way */
		unit_ptr->Out.output = (*unit_ptr->out_func)(unit_ptr->act);
	    }
	}
    }

    return( KRERR_NO_ERROR );
} 



/*****************************************************************************
  FUNCTION : UPDATE_FixAct_Hop

  PURPOSE  : synchronous update with fixed activity
  RETURNS  : 
  NOTES    : the units updated are given the activity 1 the others 0  
             To decide which units are to  be updated, the arrays 
             'unitsToUpdate' and 'netInputArray' are needed:
             'netInputArray' contains the n highest netinputs ( n = NoOfOnes )
             'unitsToUpdate' contains the corresponding unit pointers, so that
              unitsToUpdate[i] points to the unit which has netinput equal to
              netInputArray[i].
  UPDATE   : 
******************************************************************************/
krui_err  UPDATE_FixAct_Hop(float *parameterArray, int NoOfParams)
{
    register struct Unit   *unit_ptr; 
    FlintType               sum, aux, min;
    ACT_FUNC_DEFS    /* defines link- and site-pointer */
    register int            i;
    int                     NoOfOnes, where;
    struct Unit            **unitsToUpdate;
    FlintType              *netInputArray;
   

    NoOfOnes = parameterArray[0]; /* the fixed Number of 1 */

    /* init netInputArray and unitsToUpdate */
    netInputArray = (FlintType *) calloc(NoOfOnes, sizeof(FlintType));
    unitsToUpdate = (struct Unit * *) calloc(NoOfOnes, sizeof( struct Unit *)); 
    for(i=0; i<= NoOfOnes-1; i++) {
	unitsToUpdate[i] = NULL;
	netInputArray[i] = -9e37;
    }

    FOR_ALL_UNITS(unit_ptr) { 
	if UNIT_IN_USE(unit_ptr) {
	    if (*unit_ptr->out_func == OUT_IDENTITY) { 
		unit_ptr->Out.output = unit_ptr->act;
	    }else if(unit_ptr->out_func == OUT_Custom_Python){/* the default way */
	    	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	    }else{/* the default way */
		unit_ptr->Out.output = (*unit_ptr->out_func)(unit_ptr->act);
	    }
	}
    }


    /* find the units to update (their nr. is given by "NoOfOnes") by 
       saving the units with highest netinputs ( the netinputs are held in 
       the netInputArray and if a higher netinput occures, it replaces the 
       lowest value in the array ) */
 
    FOR_ALL_UNITS(unit_ptr) {
	/* get the netInput of this unit */
	sum =  0.0; aux = 0.0;
	if (GET_FIRST_UNIT_LINK( unit_ptr )){
	    do
		sum += GET_WEIGHTED_OUTPUT;
	    while (GET_NEXT_LINK);
	}
      
	/* get the min of netInputArray, i.e. the worst netinput value already 
           computed */

	min = netInputArray[0];
	where = 0;
	for(i = 1; i <= NoOfOnes - 1; i++) {
	    if( netInputArray[i] < min) {
		min = netInputArray[i];
		where = i;
	    }
	}

	/* replace the lowest netinput with the actual one if this is higher
	   and save the pointer to the actual unit in 'unitsToUpdate' */

	if( sum > min ){
	    netInputArray[where] = sum;
	    unitsToUpdate[where] = unit_ptr;
	}
    }

    /*    update unit activations    */
    FOR_ALL_UNITS(unit_ptr){
	unit_ptr->act =  0.0;
    }
    for(i=0; i<= NoOfOnes-1; i++){
	unit_ptr = unitsToUpdate[i];
	unit_ptr->act =  1.0;
    }

    /* output update for resultfile */
    FOR_ALL_UNITS(unit_ptr) { 
	if UNIT_IN_USE(unit_ptr) {
	    if (*unit_ptr->out_func == OUT_IDENTITY) { 
		unit_ptr->Out.output = unit_ptr->act;
	    }else if(unit_ptr->out_func == OUT_Custom_Python){/* the default way */
	    	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	    }else{/* the default way */
		unit_ptr->Out.output = (*unit_ptr->out_func)(unit_ptr->act);
	    }
	}
    }
    free(netInputArray);
    free(unitsToUpdate);
  
    return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : UPDATE_RM_Propagate

  PURPOSE  : 
  RETURNS  : 
  NOTES    : McClelland & Rummelhart's update rule

  UPDATE   : 
******************************************************************************/
krui_err UPDATE_RM_Propagate (float *parameterArray, int NoOfParams)
{
  register struct Unit   *unit_ptr;
  int t, NoTimes; 

  NoTimes = parameterArray[0];

  for (t=0; t < NoTimes; ++t){ 

      /*  update unit activations */
      FOR_ALL_UNITS( unit_ptr )
	  if UNIT_IN_USE( unit_ptr )
	      if ( !IS_INPUT_UNIT( unit_ptr)) 
		  /*  unit isn't an input unit and is in use and enabled  */
		  unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

      /* update unit outputs  */
      FOR_ALL_UNITS( unit_ptr )
	  if UNIT_IN_USE( unit_ptr )
	      if (unit_ptr->out_func == OUT_IDENTITY)
		  /*  identity output function: don't call output function  */
		  unit_ptr->Out.output = unit_ptr->act;
	      else if(unit_ptr->out_func == OUT_Custom_Python)
	      	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
	      else
		  /* calculate unit's output also  */
		  unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
  }

  return( KRERR_NO_ERROR );

}


/*#########################################################

GROUP: Update Functions for the use with the GA tool Enzo

#########################################################*/



/*****************************************************************************
  FUNCTION : ENZO_PROPAGATE_error_back()

  PURPOSE  : 
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
static krui_err  ENZO_PROPAGATE_error_back(void)
{
  register struct Link *link_ptr;
  register struct Site *site_ptr;
  register struct Unit *unit_ptr;
  register float error;                    /*  error  */
  TopoPtrArray topo_ptr;
  int ret_code;

  if (NetModified || (TopoSortID != TOPOLOGICAL_FF))
    {  /*  Net has been modified or topologic array isn't initialized */
    /*  check the topology of the network  */
    ret_code = kr_topoCheck();
    if(ret_code < KRERR_NO_ERROR)  return(ret_code); /* an error has occured */
    if(ret_code<2)return(KRERR_NET_DEPTH); /* network has less than 2 layers */

    /*	count the no. of I/O units and check the patterns  */
    ret_code = kr_IOCheck();
    if (ret_code < KRERR_NO_ERROR)  return( ret_code );

    /*	sort units by topology and by topologic type  */
    ret_code = kr_topoSort( TOPOLOGICAL_FF );
    if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
      return( ret_code );

    NetModified = FALSE;
    }

  /*  add 3 to no_of_topo_units because the topologic array contains
      4 NULL pointers  */
  topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

  /*  calculate output units only  */
  while ((unit_ptr = *--topo_ptr) != NULL) {
    /* Out.output == dEdw */
    error = - unit_ptr->Out.output * ((unit_ptr->act_deriv_func) ( unit_ptr ));
    unit_ptr->value_c += -error /* * 1 */;   /*  calculate the bias slopes  */
                                            /*  learn bias like a weight  */
    if (UNIT_HAS_DIRECT_INPUTS( unit_ptr ))
      {  /*  the unit has direct links  */
      FOR_ALL_LINKS( unit_ptr, link_ptr )
        {  /*	calculate the slopes  */
        link_ptr->value_c += - error * link_ptr->to->Out.output;
        link_ptr->to->Aux.flint_no += link_ptr->weight * error;
        }
      }
    else
      {  /*  the unit has sites  */
      FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
	{  /*  calculate the value_cs  */
	link_ptr->value_c += - error * link_ptr->to->Out.output;
	link_ptr->to->Aux.flint_no += link_ptr->weight * error;
	}
      }
    }


  /*  calculate hidden units only  */
  while ((unit_ptr = *--topo_ptr) != NULL) {
      error = ((unit_ptr->act_deriv_func) (unit_ptr)) * unit_ptr->Aux.flint_no;

      unit_ptr->value_c += - error /* * 1 */;   /* calculate the bias slopes */
      /*  learn bias like a weight  */
      if (UNIT_HAS_DIRECT_INPUTS( unit_ptr )){
	  /*  the unit has direct links  */
	  FOR_ALL_LINKS( unit_ptr, link_ptr ){
	      /*  calculate the slopes  */
	      if (link_ptr->to->flags & UFLAG_TTYP_HIDD)
		/*  this link points to a hidden unit: 
		    sum up the error's from previos units  */
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
	      
	      link_ptr->value_c += - error * link_ptr->to->Out.output;
	  }
      } else {  /*  the unit has sites  */
	  FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
	      /*  calculate the slopes  */
	      if (link_ptr->to->flags & UFLAG_TTYP_HIDD)
		/*  this link points to a hidden unit: 
		    sum up the error's from previos units  */
		link_ptr->to->Aux.flint_no += link_ptr->weight * error;
	      
	      link_ptr->value_c += - error * link_ptr->to->Out.output;
	  }
      }
  }
  return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : ENZO_PROPAGATE_ff

  PURPOSE  : 
  RETURNS  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
static krui_err ENZO_PROPAGATE_ff(void)
{
  register struct Unit   *unit_ptr;
  register TopoPtrArray     topo_ptr;
  int ret_code;

  if (NetModified || (TopoSortID != TOPOLOGICAL_FF))
    {  /*  Net has been modified or topologic array isn't initialized */
    /*  check the topology of the network  */
    ret_code = kr_topoCheck();
    if(ret_code < KRERR_NO_ERROR) return(ret_code);  /* an error has occured */
    if(ret_code<2)return(KRERR_NET_DEPTH); /* network has less than 2 layers */

    /*	count the no. of I/O units and check the patterns  */
    ret_code = kr_IOCheck();
    if (ret_code < KRERR_NO_ERROR)  return( ret_code );

    /*	sort units by topology and by topologic type  */
    ret_code = kr_topoSort( TOPOLOGICAL_FF );
    if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
      return( ret_code );

    NetModified = FALSE;
    }


  topo_ptr = topo_ptr_array;

  /*  copy pattern into input unit's activation and
      calculate output of the input units
  */
  while ((unit_ptr = *++topo_ptr) != NULL)
    {  /*  topo_ptr points to a (topological sorted) unit stucture (input units first)  */
    if (unit_ptr->out_func == OUT_IDENTITY)
      /*  identity output function: no need to call the output function  */
      unit_ptr->Out.output = unit_ptr->act;
    else if(unit_ptr->out_func == OUT_Custom_Python)
    	unit_ptr->Out.output =
		kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
    else
      /*  no identity output function: calculate unit's output also  */
      unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

  /*  popagate hidden units  */
  while ((unit_ptr = *++topo_ptr) != NULL)
    {  /*  topo_ptr points to a (topological sorted) unit stucture */
    /*  clear error values  */
    unit_ptr->Aux.flint_no = 0.0;

    /*  calculate the activation value of the unit: 
	call the activation function if needed  */
    unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

    if (unit_ptr->out_func == OUT_IDENTITY)
      /*  identity output function: no need to call the output function  */
      unit_ptr->Out.output = unit_ptr->act;
    else if(unit_ptr->out_func == OUT_Custom_Python)
    	unit_ptr->Out.output =
		kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
    else
      /*  no identity output function: calculate unit's output also  */
      unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

  /*  popagate output units  */
  while ((unit_ptr = *++topo_ptr) != NULL)
    {  /*  topo_ptr points to a (topological sorted) unit stucture */
    /*  clear error values  */
    unit_ptr->Aux.flint_no = 0.0;

    /*  calculate the activation value of the unit: 
	call the activation function if needed  */
    unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);

    if (unit_ptr->out_func == OUT_IDENTITY)
      /*  identity output function: no need to call the output function  */
      unit_ptr->Out.output = unit_ptr->act;
    else if(unit_ptr->out_func == OUT_Custom_Python)
    	unit_ptr->Out.output =
		kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
    else
      /*  no identity output function: calculate unit's output also  */
      unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }
  return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : ENZO_propagate

  PURPOSE  : 
  RETURNS  : 
  NOTES    : If the Input-Parameter is 0, the input activation is propagated 
             forward, else the error gradient is propagated backward.
             This is done because NetInitialized is set TRUE every
             time the Update-function is changed.
  UPDATE   : 
******************************************************************************/
krui_err  ENZO_propagate( float *parameterArray, int NoOfParams )
{  
  if (NoOfParams < 1)    
    return( KRERR_PARAMETERS );       /*  not enough input parameters  */

  if (UPDATE_PARAM1 (parameterArray)) 
    return (ENZO_PROPAGATE_error_back());  /* rueckwaerts-Propagieren */
  else
    return (ENZO_PROPAGATE_ff());          /* vorwaerts-Propagieren */
}




/*#################################################

GROUP: User Defined Update Functions

#################################################*/


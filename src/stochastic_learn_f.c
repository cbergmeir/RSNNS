/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/stochastic_learn_f.c,v $
  SHORTNAME      : stochastic_learn_f.c
  SNNS VERSION   : 4.2
  
  PURPOSE        : SNNS-Kernel Learning Functions
  NOTES          : with following learning functions:
                   - Monte-Carlo
                   - Simulated Annealing
  
  AUTHOR         : Joachim Danz
  DATE           : 27.04.1994
  
  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 1.9 $
  LAST CHANGE    : $Date: 1998/03/13 16:24:03 $
  
             Copyright (c) 1994-95 Joachim Danz, TH Darmstadt, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
  
******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <math.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#include "kr_typ.h"		/* Kernel Types and Constants  */
#include "kr_const.h"		/* Constant Declarators for SNNS-Kernel */
#include "kr_def.h"		/* Default Values  */
#include "kernel.h"		/* kernel function prototypes  */
#include "kr_mac.h"		/* Kernel Macros   */
#include "glob_typ.h"
#include "kr_art.h"
#include "kr_art1.h"
#include "kr_art2.h"
#include "kr_amap.h"
#include "krart_df.h"
#include "learn_f.h"
#include "stochastic_learn_f.ph"
#include "random.h"
#include "kr_newpattern.h"

#define NOT_MODIFIED 0
#define LAST_MODIFIED 1


/*****************************************************************************
  FUNCTION : LEARN_MonteCarlo
  
  PURPOSE  : Monte Carlo adjustment of weights and bias
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 19.02.1995
******************************************************************************/

krui_err
LEARN_MonteCarlo(int start_pattern, int end_pattern, float *parameterInArray,
		 int NoOfInParams, float **parameterOutArray,
		 int *NoOfOutParams)
{
    static float    OutParameter[1]; /* OutParameter[0] stores the
				      * learning error  */
    int             ret_code, pattern_no, sub_pat_no;
    float           error;
    register FlagWord flags;
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;

    if (NoOfInParams < 2)
	return (KRERR_PARAMETERS); /* Not enough input parameters  */
    *NoOfOutParams = 1;		/* One return value is available (the
				 * learning error)  */
    *parameterOutArray = OutParameter; /* set the output parameter reference  */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    if (NetModified) {		/* Net has been modified */

	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);

	/* sort units by topology and by topologic type  */
	ret_code = kr_topoSort(TOPOLOGICAL_FF);
	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return (ret_code);
	MinimumError = 10000000;
	NetModified = FALSE;
    }
    if (NetInitialize || LearnFuncHasChanged) {	/* Net has been modified */
	MinimumError = 10000000;
    }
    /* randomize weigths and bias */

    FOR_ALL_UNITS(unit_ptr) {
	unit_ptr->bias = (FlintType) drand48() *
	    (LEARN_PARAM2(parameterInArray) - LEARN_PARAM1(parameterInArray))
		+ LEARN_PARAM1(parameterInArray);
	flags = unit_ptr->flags;
	if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) { /* unit is in use  */
	    unit_ptr->value_a = (FlintType) 0;

	    if (flags & UFLAG_SITES) { /* unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
		    link_ptr->weight = (FlintType) drand48() *
			(LEARN_PARAM2(parameterInArray) -
			 LEARN_PARAM1(parameterInArray)) +
			     LEARN_PARAM1(parameterInArray);
	    } else {		/* unit has no sites   */
		if (flags & UFLAG_DLINKS) { /* unit has direct links */
		    FOR_ALL_LINKS(unit_ptr, link_ptr)
			link_ptr->weight = (FlintType) drand48() *
			    (LEARN_PARAM2(parameterInArray) -
			     LEARN_PARAM1(parameterInArray)) +
				 LEARN_PARAM1(parameterInArray);
		}
	    }
	}
    }

    /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(start_pattern, end_pattern);
    if (KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);
    NET_ERROR(OutParameter) = 0.0; /* reset network error value  */

    /* calculate performance of new net */
    while (kr_getSubPatternByOrder(&pattern_no, &sub_pat_no)) {
	propagateNetForward(pattern_no, sub_pat_no);
	/* Forward propagation */
	if ((error = calculate_SS_error(pattern_no, sub_pat_no)) == -1)
	    return (-1);
	NET_ERROR(OutParameter) += error;
    }

    /* store weights and bias if error decreased */
    if (NET_ERROR(OutParameter) < MinimumError) {
	MinimumError = NET_ERROR(OutParameter);
	FOR_ALL_UNITS(unit_ptr) {
	    flags = unit_ptr->flags;
	    unit_ptr->value_b = unit_ptr->bias;
	    if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {
		/* unit is in use  */
		if (flags & UFLAG_SITES) { /* unit has sites  */
		    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
			link_ptr->value_b = link_ptr->weight;
		} else {	/* unit has no sites   */
		    if (flags & UFLAG_DLINKS) {
			/* unit has direct links         */
			FOR_ALL_LINKS(unit_ptr, link_ptr)
			    link_ptr->value_b = link_ptr->weight;
		    }
		}
	    }
	}

    }

    /* restore best net */
    FOR_ALL_UNITS(unit_ptr) {
	flags = unit_ptr->flags;
	unit_ptr->bias = unit_ptr->value_b;
	if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) { /* unit is in use  */
	    if (flags & UFLAG_SITES) { /* unit has sites  */
		FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
		    link_ptr->weight = link_ptr->value_b;
	    } else {		/* unit has no sites   */
		if (flags & UFLAG_DLINKS) { /* unit has direct links */
		    FOR_ALL_LINKS(unit_ptr, link_ptr)
			link_ptr->weight = link_ptr->value_b;
		}
	    }
	}
    }

    return (ret_code);
}

/*****************************************************************************
  FUNCTION : TEST_MonteCarlo
  
  PURPOSE  : Monte Carlo adjustment of weights and bias
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 17.02.1995
*****************************************************************************/

krui_err
TEST_MonteCarlo(int start_pattern, int end_pattern, float *parameterInArray,
		int NoOfInParams, float **parameterOutArray,
		int *NoOfOutParams)
{
    static float    OutParameter[1]; /* OutParameter[0] stores the
				      * learning error  */
    int             ret_code, pattern_no, sub_pat_no;
    float           error;

    *NoOfOutParams = 1;		/* One return value is available (the
				 * learning error)  */
    *parameterOutArray = OutParameter; /* set the output parameter reference  */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */


    /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(start_pattern, end_pattern);
    if (KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);
    NET_ERROR(OutParameter) = 0.0; /* reset network error value  */

    /* calculate performance of new net */
    while (kr_getSubPatternByOrder(&pattern_no, &sub_pat_no)) {
	propagateNetForward(pattern_no, sub_pat_no);
	/* Forward propagation */
	if ((error = calculate_SS_error(pattern_no, sub_pat_no)) == -1)
	    return (-1);
	NET_ERROR(OutParameter) += error;
    }

    return (ret_code);
}

/*****************************************************************************
  FUNCTION : calculate_WTA_errror
  
  PURPOSE  : Winner Takes All error calculation for one pattern
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 06.10.1994
******************************************************************************/


float 
calculate_WTA_error(int pattern_no, int sub_pat_no)
{
    int             j, wta_pos, out_pat_pos, size;
    float           wta_value, out_pat_value;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register TopoPtrArray topo_ptr;

    /* calculate address of the output pattern (with number pattern_no + 1) */
    out_pat = kr_getSubPatData(pattern_no, sub_pat_no, OUTPUT, &size);
    if (out_pat == NULL) {
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return (-1);
    }
    out_pat += size;

    /*
     * add 3 to no_of_topo_units because the topologic array contains 4 NULL
     * pointers
     */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);
    wta_value = 0;
    wta_pos = 0;
    out_pat_value = 0;
    out_pat_pos = 0;
    j = 0;
    /* Winner Takes All - error computation */
    if (NoOfOutputUnits > 1) {

	/* calculate output units only  */
	while ((unit_ptr = *--topo_ptr) != NULL) {
	    j++;
	    if (wta_value < unit_ptr->Out.output) {
		wta_value = unit_ptr->Out.output;
		wta_pos = j;
	    }
	    if (out_pat_value < *(--out_pat)) {
		out_pat_value = *(out_pat);
		out_pat_pos = j;
	    }
	}
	if (wta_pos != out_pat_pos) {
	    return ((float) 1.0);
	}
    } else {
	unit_ptr = *--topo_ptr;
	if ((float) unit_ptr->Out.output > 0.5) {
	    if (*(--out_pat) < 0.5)
		return ((float) 1.0);
	} else {
	    if (*(--out_pat) > 0.5)
		return ((float) 1.0);
	}
    }
    return ((float) 0.0);
}
/*****************************************************************************
  FUNCTION : calculate_w_WTA_errror
  
  PURPOSE  : weighted Winner Takes All error calculation for one pattern
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 06.10.1994
******************************************************************************/


float 
calculate_w_WTA_error(int pattern_no, int sub_pat_no)
{
    int             j, wta_pos, out_pat_pos, size;
    float           wta_value, out_pat_value, sum_value;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register TopoPtrArray topo_ptr;

    /* calculate address of the output pattern (with number pattern_no + 1) */
    out_pat = kr_getSubPatData(pattern_no, sub_pat_no, OUTPUT, &size);
    if (out_pat == NULL) {
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return (-1);
    }
    out_pat += size;

    /*
     * add 3 to no_of_topo_units because the topologic array contains 4 NULL
     * pointers
     */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);
    wta_value = 0;
    wta_pos = 0;
    sum_value = 0;
    out_pat_value = 0;
    out_pat_pos = 0;
    j = 0;
    /* weighted Winner Takes All - error computation */
    if (NoOfOutputUnits > 1) {

	/* calculate output units only  */
	while ((unit_ptr = *--topo_ptr) != NULL) {
	    j++;
	    sum_value += unit_ptr->Out.output;
	    if (wta_value < unit_ptr->Out.output) {
		wta_value = unit_ptr->Out.output;
		wta_pos = j;
	    }
	    if (out_pat_value < *(--out_pat)) {
		out_pat_value = *(out_pat);
		out_pat_pos = j;
	    }
	}
	if (wta_pos != out_pat_pos)
	    return ((float) 1.0);
	else
	    return ((float) fabs(wta_value - (sum_value - wta_value) / (--j)));
    } else {
	unit_ptr = *--topo_ptr;
	if ((float) unit_ptr->Out.output > 0.5) {
	    if (*(--out_pat) < 0.5)
		return ((float) 1.0);
	    else
		return ((float) fabs(unit_ptr->Out.output - *(out_pat)) / 2);
	} else {
	    if (*(--out_pat) > 0.5)
		return ((float) 1.0);
	    else
		return ((float) fabs(*(out_pat) - unit_ptr->Out.output) / 2);
	}
    }
    return ((float) 0.0);
}

/*****************************************************************************
  FUNCTION : calculate_SS_errror
  
  PURPOSE  : sum squared error for one pattern
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 06.10.1994
******************************************************************************/


float
calculate_SS_error(int pattern_no, int sub_pat_no)
{
    int             size;
    float           sum_error, error;
    register struct Unit *unit_ptr;
    register Patterns out_pat;
    register TopoPtrArray topo_ptr;

    /* calculate address of the output pattern (with number pattern_no + 1) */
    out_pat = kr_getSubPatData(pattern_no, sub_pat_no, OUTPUT, &size);
    if (out_pat == NULL) {
	KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	return (-1);
    }
    out_pat += size;

    /*
     * add 3 to no_of_topo_units because the topologic array contains 4 NULL
     * pointers
     */
    topo_ptr = topo_ptr_array + (no_of_topo_units + 3);

    sum_error = 0;
    /* calculate output units only  */
    while ((unit_ptr = *--topo_ptr) != NULL) {
	error = *(--out_pat) - unit_ptr->Out.output;
	sum_error += error * error;
    }
    /* calc. devitation */

    return ((float) sum_error);
}

/*****************************************************************************
  FUNCTION : LEARN_SimulatedAnnealingSS
  
  PURPOSE  : Simulated Annealing adjustment of weights and bias
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 06.10.1994
******************************************************************************/

krui_err
LEARN_SimulatedAnnealingSS(int start_pattern, int end_pattern,
			   float *parameterInArray, int NoOfInParams,
			   float **parameterOutArray, int *NoOfOutParams)
{
    return (LEARN_SimulatedAnnealing(start_pattern, end_pattern,
				     parameterInArray, NoOfInParams,
				     parameterOutArray, NoOfOutParams, SS_ERROR));
}


/*****************************************************************************
  FUNCTION : LEARN_SimulatedAnnealingWTA
  
  PURPOSE  : Simulated Annealing adjustment of weights and bias
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 06.10.1994
******************************************************************************/

krui_err
LEARN_SimulatedAnnealingWTA(int start_pattern, int end_pattern,
			    float *parameterInArray, int NoOfInParams,
			    float **parameterOutArray, int *NoOfOutParams)
{
    return (LEARN_SimulatedAnnealing(start_pattern, end_pattern,
				     parameterInArray, NoOfInParams,
				     parameterOutArray, NoOfOutParams, WTA_ERROR));
}

/*****************************************************************************
  FUNCTION : LEARN_SimulatedAnnealingWWTA
  
  PURPOSE  : Simulated Annealing adjustment of weights and bias
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 06.10.1994
  ******************************************************************************/

krui_err
LEARN_SimulatedAnnealingWWTA(int start_pattern, int end_pattern,
			     float *parameterInArray, int NoOfInParams,
			     float **parameterOutArray, int *NoOfOutParams)
{
    return (LEARN_SimulatedAnnealing(start_pattern, end_pattern,
				     parameterInArray, NoOfInParams,
				     parameterOutArray, NoOfOutParams, WWTA_ERROR));
}


/*****************************************************************************
  FUNCTION : LEARN_SimulatedAnnealing
  
  PURPOSE  : Simulated Annealing adjustment of weights and bias
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 19.02.1995
******************************************************************************/

krui_err
LEARN_SimulatedAnnealing(int start_pattern, int end_pattern,
			 float *parameterInArray, int NoOfInParams,
			 float **parameterOutArray, int *NoOfOutParams,
			 int errorFunction)
{
    static float    OutParameter[1]; /* OutParameter[0] stores the
				      * learning error  */
    int             ret_code, pattern_no, sub_pat_no;
    float           error;
    long int        NextParameter;
    register FlagWord flags;
    register struct Link *link_ptr;
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;

    if (NoOfInParams < 4)
	return (KRERR_PARAMETERS); /* Not enough input parameters  */
    *NoOfOutParams = 1;		/* One return value is available (the
				 * learning error)  */
    *parameterOutArray = OutParameter; /* set the output parameter reference  */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */

    NextParameter = FALSE;

    if (NetModified || NetInitialize || LearnFuncHasChanged) {
	/* Net has been modified */
	/* count the no. of I/O units and check the patterns  */
	ret_code = kr_IOCheck();
	if (ret_code < KRERR_NO_ERROR)
	    return (ret_code);
	/* sort units by topology and by topologic type  */
	ret_code = kr_topoSort(TOPOLOGIC_TYPE);

	if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
	    return (ret_code);
	FOR_ALL_UNITS(unit_ptr) {
	    flags = unit_ptr->flags;
	    unit_ptr->value_b = NOT_MODIFIED;
	    if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {
		/* unit is in use  */
		if (flags & UFLAG_SITES) { /* unit has sites  */
		    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
			link_ptr->value_b = NOT_MODIFIED;
		} else {	/* unit has no sites   */
		    if (flags & UFLAG_DLINKS) {
			/* unit has direct links         */
			FOR_ALL_LINKS(unit_ptr, link_ptr)
			    link_ptr->value_b = NOT_MODIFIED;
		    }
		}
	    }
	}
	Temperature = LEARN_PARAM3(parameterInArray);
	MinimumError = 99999;
	NetModified = FALSE;
	NextParameter = TRUE;
    }
    /* randomize one weigth or one bias */
    do {
	FOR_ALL_UNITS(unit_ptr) {
	    flags = unit_ptr->flags;
	    if (((flags & UFLAG_IN_USE) == UFLAG_IN_USE) && ((flags & UFLAG_TTYP_IN) != UFLAG_TTYP_IN) && !(IS_SPECIAL_UNIT(unit_ptr))) {
		/* unit is in use  */
		if (NextParameter == TRUE) {
		    unit_ptr->value_c = unit_ptr->bias;
		    unit_ptr->bias = (FlintType) drand48() *
			(LEARN_PARAM2(parameterInArray) -
			 LEARN_PARAM1(parameterInArray)) +
			     LEARN_PARAM1(parameterInArray);
		    NextParameter = FALSE;
		    unit_ptr->value_b = LAST_MODIFIED;
		} else {
		    if (unit_ptr->value_b == LAST_MODIFIED) {
			unit_ptr->value_b = NOT_MODIFIED;
			NextParameter = TRUE;
		    }
		}
		if (flags & UFLAG_SITES) { /* unit has sites  */
		    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
			if (NextParameter == TRUE) {
			    link_ptr->value_c = link_ptr->weight;
			    link_ptr->weight = (FlintType) drand48() *
				(LEARN_PARAM2(parameterInArray) -
				 LEARN_PARAM1(parameterInArray)) +
				     LEARN_PARAM1(parameterInArray);
			    NextParameter = FALSE;
			    link_ptr->value_b = LAST_MODIFIED;
			} else {
			    if (link_ptr->value_b == LAST_MODIFIED) {
				link_ptr->value_b = NOT_MODIFIED;
				NextParameter = TRUE;
			    }
			}
		} else {	/* unit has no sites   */
		    if (flags & UFLAG_DLINKS) {
			/* unit has direct links         */
			FOR_ALL_LINKS(unit_ptr, link_ptr)
			    if (NextParameter == TRUE) {
				link_ptr->value_c = link_ptr->weight;
				link_ptr->weight = (FlintType) drand48() *
				    (LEARN_PARAM2(parameterInArray) -
				     LEARN_PARAM1(parameterInArray)) +
					 LEARN_PARAM1(parameterInArray);
				NextParameter = FALSE;
				link_ptr->value_b = LAST_MODIFIED;

			    } else {
				if (link_ptr->value_b == LAST_MODIFIED) {
				    link_ptr->value_b = NOT_MODIFIED;
				    NextParameter = TRUE;
				}
			    }
		    }
		}
	    }
	}
    } while (NextParameter == TRUE);

    /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(start_pattern, end_pattern);
    if (KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);
    NET_ERROR(OutParameter) = 0.0; /* reset network error value  */

    /* calculate performance of new net */
    while (kr_getSubPatternByOrder(&pattern_no, &sub_pat_no)) {
	propagateNetForward(pattern_no, sub_pat_no);
	/* Forward propagation */

	switch (errorFunction) {
	  case SS_ERROR:
	    if ((error = calculate_SS_error(pattern_no, sub_pat_no)) == -1)
		return (-1);
	    break;
	  case WTA_ERROR:
	    if ((error = calculate_WTA_error(pattern_no, sub_pat_no)) == -1)
		return (-1);
	    break;
	  case WWTA_ERROR:
	    if ((error = calculate_w_WTA_error(pattern_no, sub_pat_no)) == -1)
		return (-1);
	    break;
	}
	NET_ERROR(OutParameter) += error;
    }

    /*
     * throw away new weight or bias if error decreased or propability is to
     * low
     */
    if ((NET_ERROR(OutParameter) > MinimumError) && (Temperature > 0) &&
	(drand48() > exp((MinimumError - NET_ERROR(OutParameter)) /
			 ((end_pattern - start_pattern) * Temperature)))) {
	FOR_ALL_UNITS(unit_ptr) {
	    flags = unit_ptr->flags;
	    if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) {
		/* unit is in use  */
		if (unit_ptr->value_b == LAST_MODIFIED) {
		    unit_ptr->bias = unit_ptr->value_c;
		    break;
		}
		if (flags & UFLAG_SITES) { /* unit has sites  */
		    FOR_ALL_SITES_AND_LINKS(unit_ptr, site_ptr, link_ptr)
			if (link_ptr->value_b == LAST_MODIFIED)
			    link_ptr->weight = link_ptr->value_c;
		} else {	/* unit has no sites   */
		    if (flags & UFLAG_DLINKS) {
			/* unit has direct links         */
			FOR_ALL_LINKS(unit_ptr, link_ptr)
			    if (link_ptr->value_b == LAST_MODIFIED)
				link_ptr->weight = link_ptr->value_c;
		    }
		}
	    }
	}
    } else {
	MinimumError = NET_ERROR(OutParameter);
    }
    
    /* decrease Temperature */
    Temperature *= LEARN_PARAM4(parameterInArray);

    return (ret_code);
}

/*****************************************************************************
  FUNCTION : TEST_SimulatedAnnealingSS
  Squared Sum Error
  PURPOSE  : Test Performance of the Network during Simulated Annealing 
  Learning
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 06.10.1994
******************************************************************************/

krui_err
TEST_SimulatedAnnealingSS(int start_pattern, int end_pattern,
			  float *parameterInArray, int NoOfInParams,
			  float **parameterOutArray, int *NoOfOutParams)
{
    return (TEST_SimulatedAnnealing(start_pattern, end_pattern,
				    parameterInArray, NoOfInParams,
				    parameterOutArray, NoOfOutParams, SS_ERROR));
}


/*****************************************************************************
  FUNCTION : TEST_SimulatedAnnealingWTA
  Winner Takes All Error
  
  PURPOSE  : Test Performance of the Network during Simulated Annealing 
  Learning
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 06.10.1994
******************************************************************************/

krui_err
TEST_SimulatedAnnealingWTA(int start_pattern, int end_pattern,
			   float *parameterInArray, int NoOfInParams,
			   float **parameterOutArray, int *NoOfOutParams)
{
    return (TEST_SimulatedAnnealing(start_pattern, end_pattern,
				    parameterInArray, NoOfInParams,
				    parameterOutArray, NoOfOutParams, WTA_ERROR));
}

/*****************************************************************************
  FUNCTION : TEST_SimulatedAnnealingWWTA
  Weighted Winner Takes All Error
  
  PURPOSE  : Test Performance of the Network during Simulated Annealing 
  Learning
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 06.10.1994
******************************************************************************/

krui_err
TEST_SimulatedAnnealingWWTA(int start_pattern, int end_pattern,
			    float *parameterInArray, int NoOfInParams,
			    float **parameterOutArray, int *NoOfOutParams)
{
    return (TEST_SimulatedAnnealing(start_pattern, end_pattern,
				    parameterInArray, NoOfInParams,
				    parameterOutArray, NoOfOutParams, WWTA_ERROR));
}

/*****************************************************************************
  FUNCTION : TEST_SimulatedAnnealing
  
  PURPOSE  : Test Performance of the Network during Simulated Annealing 
  Learning
  RETURNS  : error
  NOTES    :
  
  UPDATE   : 19.02.1995
******************************************************************************/

krui_err
TEST_SimulatedAnnealing(int start_pattern, int end_pattern,
			float *parameterInArray, int NoOfInParams,
			float **parameterOutArray, int *NoOfOutParams,
			int errorFunction)
{
    static float    OutParameter[1]; /* OutParameter[0] stores the
				      * learning error  */
    int             ret_code, pattern_no, sub_pat_no;
    float           error;
    
    if (NoOfInParams < 4)
	return (KRERR_PARAMETERS); /* Not enough input parameters  */
    *NoOfOutParams = 1;		/* One return value is available (the
				 * learning error)  */
    *parameterOutArray = OutParameter; /* set the output parameter reference  */
    ret_code = KRERR_NO_ERROR;	/* reset return code  */


    /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(start_pattern, end_pattern);
    if (KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);
    NET_ERROR(OutParameter) = 0.0; /* reset network error value  */

    /* calculate performance of new net */
    while (kr_getSubPatternByOrder(&pattern_no, &sub_pat_no)) {
	propagateNetForward(pattern_no, sub_pat_no);
	/* Forward propagation */

	switch (errorFunction) {
	  case SS_ERROR:
	    if ((error = calculate_SS_error(pattern_no, sub_pat_no)) == -1)
		return (-1);
	    break;
	  case WTA_ERROR:
	    if ((error = calculate_WTA_error(pattern_no, sub_pat_no)) == -1)
		return (-1);
	    break;
	  case WWTA_ERROR:
	    if ((error = calculate_w_WTA_error(pattern_no, sub_pat_no)) == -1)
		return (-1);
	    break;
	}
	NET_ERROR(OutParameter) += error;
    }

    return (ret_code);
}

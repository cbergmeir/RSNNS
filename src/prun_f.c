/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/prun_f.c,v $
  SHORTNAME      : prun_f.c
  SNNS VERSION   : 4.2

  PURPOSE        : learning functions with pruning algorithms
  NOTES          :

  AUTHOR         : Tobias Schreiner
  DATE           : 13.3.94

  CHANGED BY     : Jochen Biedermann, 07.07.94
                   Thomas Gern, 07.09.95
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/03/13 16:24:01 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#include "kr_typ.h"		/* Kernel Types and Constants  */
#include "kr_const.h"		/* Constant Declarators for SNNS-Kernel  */
#include "kr_def.h"		/* Default Values  */
#include "kernel.h"		/* kernel function prototypes  */
#include "kr_mac.h"		/* Kernel Macros   */
#include "glob_typ.h"
#include "kr_funcs.h"
#include "kr_newpattern.h"
#include "matrix.h"
#include "learn_f.h"

#include "prun_f.ph"


/*****************************************************************************

  GROUP: General Functions

******************************************************************************/



/*****************************************************************************
  FUNCTION : pr_setInputPruning
  PURPOSE  : updates input pruning flag

  RETURNS  : void
  NOTES    : 
  UPDATE   :
******************************************************************************/

void pr_setInputPruning (int value)
{
    pr_inputPruning = value;
}

/*****************************************************************************
  FUNCTION : pr_setHiddenPruning
  PURPOSE  : updates hidden pruning flag

  RETURNS  : void
  NOTES    : 
  UPDATE   :
******************************************************************************/

void pr_setHiddenPruning (int value)
{
    pr_hiddenPruning = value;
}

/*****************************************************************************
  FUNCTION : pr_updateNoLinks
  PURPOSE  : updates values for one connection

  RETURNS  : void
  NOTES    : called by pr_checkDeadUnits

  UPDATE   :
******************************************************************************/

static void pr_updateNoOfLinks (struct Unit *unit_ptr, struct Link *link_ptr)

{

    if (!IS_SPECIAL_UNIT (link_ptr->to))
    {
	/* increment number of outgoing links for source unit */
	link_ptr->to->value_a++;

	/* increment number of incoming links for target unit */
	unit_ptr->value_b++;
    }

}


/*****************************************************************************
  FUNCTION : pr_checkDeadUnits
  PURPOSE  : deletes/disables dead units in the network

  RETURNS  : void
  NOTES    : 

  UPDATE   :
******************************************************************************/

void pr_checkDeadUnits (void)

{

    struct Unit *unit_ptr, *unit_ptr2;
    struct Site *site_ptr, *site_ptr2;
    struct Link *link_ptr, *link_ptr2;
    int unit_deleted;

    FOR_ALL_UNITS (unit_ptr)
	/* reset special flags */
	unit_ptr->flags &= ~UFLAG_TTYP_SPEC;

    do
    {
        unit_deleted = FALSE;

        /* initialize units */
        FOR_ALL_UNITS (unit_ptr)
        {
	    unit_ptr->value_a = 0; /* outgoing links */
	    unit_ptr->value_b = 0; /* incoming links */
        }

        /* count links for each unit */
        FOR_ALL_UNITS (unit_ptr)
            if (! IS_SPECIAL_UNIT (unit_ptr))
                if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
                    FOR_ALL_LINKS (unit_ptr, link_ptr)
                        /* calculate number of in- and outgoing links */
                        pr_updateNoOfLinks (unit_ptr, link_ptr);
                else
                    FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
                        /* calculate number of in- and outgoing links */
                        pr_updateNoOfLinks (unit_ptr, link_ptr);

        /* delete dead units */
        FOR_ALL_UNITS (unit_ptr)
            if (! IS_SPECIAL_UNIT (unit_ptr)
                && ((IS_INPUT_UNIT (unit_ptr) && pr_inputPruning) || 
                    (IS_HIDDEN_UNIT (unit_ptr) && pr_hiddenPruning))) 
            {
		if (IS_INPUT_UNIT (unit_ptr) && (unit_ptr->value_a == 0))
                {
                    /* no outgoing links, disable unit */
                    unit_ptr->flags |= UFLAG_TTYP_SPEC;
                    unit_deleted = TRUE;
                    NetModified = TRUE;
                }

                if (IS_HIDDEN_UNIT (unit_ptr) &&
		    ((unit_ptr->value_a == 0) ||
		     (unit_ptr->value_b == 0)))
                {
		    if (unit_ptr->value_b == 0)
                        /* add output to successors' biases */
                    {
                        FOR_ALL_UNITS (unit_ptr2)
                            if (UNIT_HAS_DIRECT_INPUTS (unit_ptr2))
                                FOR_ALL_LINKS (unit_ptr2, link_ptr2)
                                {
                                    if (link_ptr2->to == unit_ptr)
                                        unit_ptr2->bias +=   
                                            (*unit_ptr->act_func) (unit_ptr) 
						* link_ptr2->weight;
                                }
                            else
                                FOR_ALL_SITES_AND_LINKS
                                    (unit_ptr2, site_ptr2, link_ptr2)
					if (link_ptr2->to == unit_ptr)
					    unit_ptr2->bias +=   
						(*unit_ptr->act_func) (unit_ptr) 
						    * link_ptr2->weight;
                        unit_ptr->bias = 0;
                        unit_ptr->Out.output = 0;
                        unit_ptr->act = 0;
                    }

		    /* delete unit */
		    kr_removeUnit (unit_ptr);
		    kr_forceUnitGC ();
                    unit_deleted = TRUE;
                    NetModified = TRUE;
                }


		if (IS_OUTPUT_UNIT (unit_ptr) && (unit_ptr->value_b == 0))
                {
                    /* unit has no incoming links, disable it */
                    unit_ptr->flags |= UFLAG_TTYP_SPEC;
                    unit_deleted = TRUE;
                    NetModified = TRUE;
                }
            }
    }
    while (unit_deleted);

}

/*****************************************************************************
  FUNCTION : pr_calcMeanDeviation
  PURPOSE  : calculates the mean deviation for each output unit

  RETURNS  : sum error of the net or negative value, if error occurs
  NOTES    : result is placed value_a

  UPDATE   :
******************************************************************************/

krui_err pr_calcMeanDeviation (int pattern, float *sum_error)

{

    register struct Unit *unit_ptr;
    register TopoPtrArray topo_ptr;
    register Patterns out_pat;
    int size, pattern_no, sub_pat_no, no_of_patterns;
    
    /* initialize sum error */
    *sum_error = 0.0;

    /* initialize value_a of each unit */
    FOR_ALL_UNITS (unit_ptr)
	unit_ptr->value_a = 0;

    /* compute the necessary sub patterns */
    if (pattern == PR_ALL_PATTERNS)
	/* all patterns */
	KernelErrorCode = kr_initSubPatternOrder
	    (0, kr_np_pattern (PATTERN_GET_NUMBER, 0, 0) - 1);
    else
	/* single pattern */
	KernelErrorCode = kr_initSubPatternOrder (pattern, pattern);
    if (KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    /* calculate total number of subpatterns */
    no_of_patterns = kr_TotalNoOfSubPatPairs ();

    while (kr_getSubPatternByOrder (&pattern_no, &sub_pat_no))
    {
	/* calculate address of the output pattern */
	out_pat = kr_getSubPatData (pattern_no, sub_pat_no, OUTPUT, &size);
	out_pat += size;
	
	/* propagate pattern through net */
	propagateNetForward (pattern_no, sub_pat_no);

	/* calculate deviation for output units */
	for (topo_ptr = topo_ptr_array + no_of_topo_units + 2,
	     unit_ptr = *(topo_ptr--); unit_ptr != NULL;
	     unit_ptr = *(topo_ptr--))
	    unit_ptr->value_a += (float) fabs (*(--out_pat) - unit_ptr->Out.output);
    }
    
    /* divide values by total number of pattern */
    for (topo_ptr = topo_ptr_array + no_of_topo_units + 2,
	 unit_ptr = *(topo_ptr--); unit_ptr != NULL;
	 unit_ptr = *(topo_ptr--))
    {
	*sum_error += unit_ptr->value_a;
	unit_ptr->value_a /= no_of_patterns;
    }

    return (KernelErrorCode);

}


/*****************************************************************************

  GROUP: Magnitude based pruning

******************************************************************************/


/*****************************************************************************
  FUNCTION : pr_mag_processLink

  PURPOSE  : processes one link for Mag
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

static void pr_mag_processLink (struct Unit *unit_ptr, struct Link *link_ptr)

{

    if ((pr_candidateLink == NULL) ||
	(abs (link_ptr->weight) < pr_candidateSaliency))
	/* found first or least important link so far */
    {
	pr_candidateTargetUnit = unit_ptr;
	pr_candidateLink = link_ptr;
	pr_candidateSaliency = abs (link_ptr->weight);
    }

}


/*****************************************************************************
  FUNCTION : PRUNE_Mag

  PURPOSE  : performs magnitude based pruning
             sets global variables 'pr_candidateTargetUnit' and
	     'pr_candidateLink'
	     to the smallest weight in the network,
	     no deletion
  RETURNS  :
  NOTES    : 'pr_candidateTargetUnit' and 'pr_candidateLink' must be set to
             NULL before calling this function

  UPDATES  :
*****************************************************************************/

krui_err PRUNE_Mag (int pattern)

{

    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register struct Link *link_ptr;

    FOR_ALL_UNITS (unit_ptr)
	/* process links to all units */
	if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
	    /* unit has direct inputs */
	    FOR_ALL_LINKS (unit_ptr, link_ptr)
		pr_mag_processLink (unit_ptr, link_ptr);
	else
	    /* unit has sites */
	    FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		pr_mag_processLink (unit_ptr, link_ptr);

    return (KernelErrorCode);

}


/*****************************************************************************

  GROUP: Optimal Brain Damage

******************************************************************************/


/*****************************************************************************
  FUNCTION : pr_obd_processLink

  PURPOSE  : processes one link for OBD
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

static void pr_obd_processLink (struct Unit *unit_ptr, struct Link *link_ptr,
				float delta, float delta2)

{

    float bp_delta2;

    /* calculate saliency of link */
    bp_delta2 = delta2 * link_ptr->weight * link_ptr->weight;
    link_ptr->value_a +=
	bp_delta2 * link_ptr->to->Out.output * link_ptr->to->Out.output;

    /* propagate initial values back */
    link_ptr->to->value_a += delta * link_ptr->weight;
    link_ptr->to->value_b += bp_delta2;

}


/*****************************************************************************
  FUNCTION : pr_obd_checkLink

  PURPOSE  : check saliency of one link for OBD
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

static void pr_obd_checkLink (struct Unit *unit_ptr, struct Link *link_ptr)

{

    /* check, if saliency smallest so far */
    if ((pr_candidateLink == NULL) || 
	(link_ptr->value_a < pr_candidateSaliency))
    {
	pr_candidateTargetUnit = unit_ptr;
	pr_candidateLink = link_ptr;
	pr_candidateSaliency = link_ptr->value_a;
    }

}


/*****************************************************************************
  FUNCTION : PRUNE_OBD

  PURPOSE  : performs pruning according to Optimal Brain Damage,
             sets global variables 'pr_candidateTargetUnit' and
	     'pr_candidateLink',
	     no deletion
  RETURNS  :
  NOTES    : 'pr_candidateTargetUnit' and 'pr_candidateLink' must be set to
             NULL before calling this function

  UPDATE   :
******************************************************************************/

krui_err PRUNE_OBD (int pattern)

{

    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register struct Link *link_ptr;
    register TopoPtrArray topo_ptr;
    register Patterns out_pat;
    register float deriv, delta, delta2, devit;
    int size, pattern_no, sub_pat_no;

    /* reset auxiliary variables in unit array */
    FOR_ALL_UNITS (unit_ptr)
    {
	unit_ptr->value_a = 0.0;
	unit_ptr->value_b = 0.0;

	if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
	    /* unit has direct inputs */
	    FOR_ALL_LINKS (unit_ptr, link_ptr)
		link_ptr->value_a = 0.0;
	else
	    /* unit has sites */
	    FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		link_ptr->value_a = 0.0;
    }

    /* compute the necessary sub patterns */
    if (pattern == PR_ALL_PATTERNS)
	/* all patterns */
	KernelErrorCode = kr_initSubPatternOrder
	    (0, kr_np_pattern (PATTERN_GET_NUMBER, 0, 0) - 1);
    else
	/* single pattern */
	KernelErrorCode = kr_initSubPatternOrder (pattern, pattern);
    if (KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    while (kr_getSubPatternByOrder (&pattern_no, &sub_pat_no))
    {
	/* propagate input pattern through net */
	propagateNetForward (pattern_no, sub_pat_no);

	/* calculate address of the output pattern */
	out_pat = kr_getSubPatData (pattern_no, sub_pat_no, OUTPUT, &size);
	out_pat += size;

	/* calculate all output units */
	for (topo_ptr = topo_ptr_array + no_of_topo_units + 2,
	     unit_ptr = *(topo_ptr--); unit_ptr != NULL;
	     unit_ptr = *(topo_ptr--))
	{
	    devit = (unit_ptr->Out.output - *(--out_pat));
	    deriv = (unit_ptr->act_deriv_func) (unit_ptr);

	    delta = deriv * devit;
	    delta2 = devit * (unit_ptr->act_2_deriv_func) (unit_ptr)
		+ deriv * deriv;
	    
	    /* process all links to that unit */
	    if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
		/* unit has direct inputs */
		FOR_ALL_LINKS (unit_ptr, link_ptr)
		    pr_obd_processLink (unit_ptr, link_ptr, delta, delta2);
	    else
		/* unit has sites */
		FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		    pr_obd_processLink (unit_ptr, link_ptr, delta, delta2);
	}
	
	/* calculate all hidden units */
	for (unit_ptr = *(topo_ptr--);
	     (unit_ptr != NULL);
	     unit_ptr = *(topo_ptr--))
	{
	    /* calculate deltas for unit */
	    deriv = (unit_ptr->act_deriv_func) (unit_ptr);
	    delta = deriv * unit_ptr->value_a;
	    delta2 =
		unit_ptr->value_a * (unit_ptr->act_2_deriv_func) (unit_ptr)
		    + unit_ptr->value_b * deriv * deriv;
	    
	    /* process all links to that unit */
	    if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
		/* unit has direct inputs */
		FOR_ALL_LINKS (unit_ptr, link_ptr)
		    pr_obd_processLink (unit_ptr, link_ptr, delta, delta2);
	    else
		/* unit has sites */
		FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		    pr_obd_processLink (unit_ptr, link_ptr, delta, delta2);
	}
    }

    /* measure least saliency */
    FOR_ALL_UNITS (unit_ptr)
	/* check all links to that unit */
	if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
	    /* unit has direct inputs */
	    FOR_ALL_LINKS (unit_ptr, link_ptr)
		pr_obd_checkLink (unit_ptr, link_ptr);
	else
	    /* unit has sites */
	    FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		pr_obd_checkLink (unit_ptr, link_ptr);

    return (KernelErrorCode);

}


/*****************************************************************************

  GROUP: Optimal Brain Surgeon

******************************************************************************/


/*****************************************************************************
  FUNCTION : pr_obs_setInitParameter

  PURPOSE  : set init parameter for obs
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

void pr_obs_setInitParameter (float param)

{

    pr_obs_initParameter = param;

}


/*****************************************************************************
  FUNCTION : pr_obs_countLinks

  PURPOSE  : count links, result in global variable 'pr_noOfLinks'
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

static void pr_obs_countLinks ()

{

    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register struct Link *link_ptr;

    /* initialize global variable */
    pr_noOfLinks = 0;

    FOR_ALL_UNITS (unit_ptr)
	if (!IS_SPECIAL_UNIT (unit_ptr))
	    if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
		FOR_ALL_LINKS (unit_ptr, link_ptr)
		{
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			pr_noOfLinks++;
		}
	    else
		FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			pr_noOfLinks++;

}


/*****************************************************************************
  FUNCTION : pr_obs_calculateDerivative

  PURPOSE  : calculate the derivative for one link and propagate
             derivative information back
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

static void pr_obs_calculateDerivative (struct Link *link_ptr,
					float help, int weight_no)

{

    RbfMatrixSetValue
	(&pr_derivVector, 0, weight_no, link_ptr->to->Out.output * help);
    link_ptr->to->value_a += link_ptr->weight * help;

}

/*****************************************************************************
  FUNCTION : pr_obs_calculateDerivVector

  PURPOSE  : calculate the derivative vector for certain output unit
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

static void pr_obs_calculateDerivVector (struct Unit *output_unit)

{

    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register struct Link *link_ptr;
    register TopoPtrArray topo_ptr;
    float help;
    int weight_no = 0;

    /* initialize units */
    FOR_ALL_UNITS (unit_ptr)
	unit_ptr->value_a = 0.0;

    /* process output units */
    for (topo_ptr = topo_ptr_array + no_of_topo_units + 2,
	 unit_ptr = *(topo_ptr--);
	 unit_ptr != NULL;
	 unit_ptr = *(topo_ptr--))
	if (!IS_SPECIAL_UNIT (unit_ptr))
	{
	    if (unit_ptr == output_unit)
		help = (*unit_ptr->act_deriv_func) (unit_ptr);
	    else
		help = 0;

	    if (UNIT_HAS_DIRECT_INPUTS (unit_ptr))
		FOR_ALL_LINKS (unit_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			/* calculate derivative and propagate information back */
			pr_obs_calculateDerivative (link_ptr, help, weight_no++);
	    else
		FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			/* calculate derivative and propagate information back */
			pr_obs_calculateDerivative (link_ptr, help, weight_no++);
	}

    /* process hidden units */
    for (unit_ptr = *(topo_ptr--);
	 unit_ptr != NULL;
	 unit_ptr = *(topo_ptr--))
	if (!IS_SPECIAL_UNIT (unit_ptr))
	{
	    help = (*unit_ptr->act_deriv_func) (unit_ptr) *
		unit_ptr->value_a;
	    if (UNIT_HAS_DIRECT_INPUTS (unit_ptr))
		FOR_ALL_LINKS (unit_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			/* calculate derivative and propagate information back */
			pr_obs_calculateDerivative (link_ptr, help, weight_no++);
	    else
		FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			/* calculate derivative and propagate information back */
			pr_obs_calculateDerivative (link_ptr, help, weight_no++);
	}

}


/*****************************************************************************
  FUNCTION : pr_obs_updateInverseHessian

  PURPOSE  : add the next step to the inverse Hessian
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

static krui_err pr_obs_updateInverseHessian (int no_of_patterns)

{

    float hx, xh, denominator, factor;
    int i, j;

    /* calculate pr_helpHX, pr_helpXH and denominator */
    denominator = (float) no_of_patterns;
    for (i = 0; i < pr_noOfLinks; i++)
    {
	hx = 0.0;
	xh = 0.0;

	for (j = 0 ; j < pr_noOfLinks; j++)
	{
	    hx += RbfMatrixGetValue (&pr_inverseHessian, i, j) *
		RbfMatrixGetValue (&pr_derivVector, 0, j);
	    xh += RbfMatrixGetValue (&pr_derivVector, 0, j) *
		RbfMatrixGetValue (&pr_inverseHessian, j, i);
	}

	RbfMatrixSetValue (&pr_helpHX, 0, i, hx);
	RbfMatrixSetValue (&pr_helpXH, 0, i, xh);
	denominator += RbfMatrixGetValue (&pr_derivVector, 0, i) * hx;
    }

    /* calculate new Hessian matrix */
    for (i = 0; i < pr_noOfLinks; i++)
    {
	factor = RbfMatrixGetValue (&pr_helpHX, 0, i) / denominator;
	for (j = 0; j < pr_noOfLinks; j++)
	    RbfMatrixSetValue
		(&pr_inverseHessian, i, j,
		 RbfMatrixGetValue (&pr_inverseHessian, i, j) -
		 RbfMatrixGetValue (&pr_helpXH, 0, j) *
		 factor);
    }

    return (KernelErrorCode);

}


/*****************************************************************************
  FUNCTION : pr_obs_calculateInverseHessian

  PURPOSE  : calculate the inverse Hessian matrix of the network
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

static krui_err pr_obs_calculateInverseHessian (int pattern)

{

    register struct Unit *unit_ptr;
    register TopoPtrArray topo_ptr;
    int no_of_patterns, pattern_no, sub_pat_no, i;

    /* initialize matrix */
    RbfClearMatrix (&pr_inverseHessian, 0.0);
    for (i = 0; i < pr_noOfLinks; i++)
	/* diagonal elements are set */
	RbfMatrixSetValue (&pr_inverseHessian, i, i, 1/pr_obs_initParameter);

    /* compute the necessary sub patterns */
    if (pattern == PR_ALL_PATTERNS)
	/* all patterns */
	KernelErrorCode = kr_initSubPatternOrder
	    (0, kr_np_pattern (PATTERN_GET_NUMBER, 0, 0) - 1);
    else
	/* single pattern */
	KernelErrorCode = kr_initSubPatternOrder (pattern, pattern);
    if (KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    /* calculate total number of subpatterns */
    no_of_patterns = kr_TotalNoOfSubPatPairs ();

    /* add up inverse Hessian for each (sub-)pattern */
    while (kr_getSubPatternByOrder (&pattern_no, &sub_pat_no))
    {
	/* propagate pattern through net */
	propagateNetForward (pattern_no, sub_pat_no);

	/* add up inverse Hessian for each output unit */
	for (topo_ptr = topo_ptr_array + no_of_topo_units + 2,
	     unit_ptr = *(topo_ptr--);
	     unit_ptr != NULL;
	     unit_ptr = *(topo_ptr--))
	{
	    /* calculate derivative vector for output unit */
	    pr_obs_calculateDerivVector (unit_ptr);

	    /* add this step to inverse Hessian */
	    KernelErrorCode =
		pr_obs_updateInverseHessian (no_of_patterns);
	    if (KernelErrorCode != KRERR_NO_ERROR)
		return (KernelErrorCode);
	}
    }

    return (KernelErrorCode);

}

/*****************************************************************************
  FUNCTION : pr_obs_processLink

  PURPOSE  : process one link for OBS
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

static void pr_obs_processLink (struct Unit *unit_ptr,
				struct Link *link_ptr,
				int weight_no)

{

    float double_saliency;

    /* calculate saliency of link */
    double_saliency = link_ptr->weight * link_ptr->weight /
	RbfMatrixGetValue (&pr_inverseHessian, weight_no, weight_no);

    if ((pr_candidateLink == NULL) ||
	(double_saliency < pr_candidateSaliency))
	/* found first or least important link so far */
    {
	pr_candidateTargetUnit = unit_ptr;
	pr_candidateLink = link_ptr;
	pr_candidateSaliency = double_saliency;
	pr_candidateLinkNo = weight_no;
    }

}


/*****************************************************************************
  FUNCTION : pr_obs_updateLink

  PURPOSE  : update one link for OBS
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/

static void pr_obs_updateLink (struct Link *link_ptr,
			       float update_const,
			       int weight_no)

{

    link_ptr->weight -= update_const *
	RbfMatrixGetValue (&pr_inverseHessian, weight_no, pr_candidateLinkNo);

}


/*****************************************************************************
  FUNCTION : PRUNE_OBS

  PURPOSE  : performs pruning according to Optimal Brain Surgeon,
             sets global variables 'pr_candidateTargetUnit' and
	     'pr_candidateLink',
	     no deletion
  RETURNS  :
  NOTES    : 'pr_candidateTargetUnit' and 'pr_candidateLink' must be set to
             NULL before calling this function

  UPDATE   :
******************************************************************************/

krui_err PRUNE_OBS (int pattern)
{

    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register struct Link *link_ptr;
    register TopoPtrArray topo_ptr;
    float update_const;
    int err, link_no;

    /* count links */
    pr_obs_countLinks ();

    /* create inverse Hessian matrix */
    err = RbfAllocMatrix (pr_noOfLinks, pr_noOfLinks, &pr_inverseHessian);
    if (err == 0)
	/* matrix didn't fit into memory */
    {
	KernelErrorCode = KRERR_INSUFFICIENT_MEM;
	return (KernelErrorCode);
    }

    /* create derivative vector */
    err = RbfAllocMatrix (1, pr_noOfLinks, &pr_derivVector);
    if (err == 0)
	/* vector didn't fit into memory */
    {
	KernelErrorCode = KRERR_INSUFFICIENT_MEM;
	return (KernelErrorCode);
    }

    /* create auxiliary vectors */
    err = RbfAllocMatrix (1, pr_noOfLinks, &pr_helpHX);
    if (err == 0)
	/* vector didn't fit into memory */
    {
	KernelErrorCode = KRERR_INSUFFICIENT_MEM;
	return (KernelErrorCode);
    }
    err = RbfAllocMatrix (1, pr_noOfLinks, &pr_helpXH);
    if (err == 0)
	/* vector didn't fit into memory */
    {
	KernelErrorCode = KRERR_INSUFFICIENT_MEM;
	return (KernelErrorCode);
    }

    /* calculate inverse Hessian */
    pr_obs_calculateInverseHessian (pattern);

    /* find link with smallest saliency*/
    link_no = 0;
    for (topo_ptr = topo_ptr_array + no_of_topo_units + 2,
	 unit_ptr = *(topo_ptr--);
	 unit_ptr != NULL;
	 unit_ptr = *(topo_ptr--))
	/* process links to all output units */
	if (!IS_SPECIAL_UNIT (unit_ptr))
	    if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
		/* unit has direct inputs */
		FOR_ALL_LINKS (unit_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			pr_obs_processLink (unit_ptr, link_ptr, link_no++);
	    else
		/* unit has sites */
		FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			pr_obs_processLink (unit_ptr, link_ptr, link_no++);

    for (unit_ptr = *(topo_ptr--);
	 unit_ptr != NULL;
	 unit_ptr = *(topo_ptr--))
	/* process links to all hidden units */
	if (!IS_SPECIAL_UNIT (unit_ptr))
	    if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
		/* unit has direct inputs */
		FOR_ALL_LINKS (unit_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			pr_obs_processLink (unit_ptr, link_ptr, link_no++);
	    else
		/* unit has sites */
		FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			pr_obs_processLink (unit_ptr, link_ptr, link_no++);

    /* calculate auxiliary variable for efficient update of weights */
    update_const = pr_candidateLink->weight /
	RbfMatrixGetValue (&pr_inverseHessian, pr_candidateLinkNo,
			   pr_candidateLinkNo);

    /* update all links */
    link_no = 0;
    for (topo_ptr = topo_ptr_array + no_of_topo_units + 2,
	 unit_ptr = *(topo_ptr--);
	 unit_ptr != NULL;
	 unit_ptr = *(topo_ptr--))
	/* process links to all output units */
	if (!IS_SPECIAL_UNIT (unit_ptr))
	    if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
		/* unit has direct inputs */
		FOR_ALL_LINKS (unit_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			pr_obs_updateLink (link_ptr, update_const, link_no++);
	    else
		/* unit has sites */
		FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			pr_obs_updateLink (link_ptr, update_const, link_no++);

    for (unit_ptr = *(topo_ptr--);
	 unit_ptr != NULL;
	 unit_ptr = *(topo_ptr--))
	/* process links to all hidden units */
	if (!IS_SPECIAL_UNIT (unit_ptr))
	    if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
		/* unit has direct inputs */
		FOR_ALL_LINKS (unit_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			pr_obs_updateLink (link_ptr, update_const, link_no++);
	    else
		/* unit has sites */
		FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
		    if (!IS_SPECIAL_UNIT (link_ptr->to))
			pr_obs_updateLink (link_ptr, update_const, link_no++);

    /* free matrixes */
    RbfFreeMatrix (&pr_inverseHessian);
    RbfFreeMatrix (&pr_derivVector);
    RbfFreeMatrix (&pr_helpXH);
    RbfFreeMatrix (&pr_helpHX);

    return (KernelErrorCode);

}


/*****************************************************************************

  GROUP: Skeletonization

******************************************************************************/


/*****************************************************************************
  FUNCTION : pr_skel_processLink

  PURPOSE  : process one link for Skeletonization
  RETURNS  : void
  NOTES    : 

  UPDATE   : 
******************************************************************************/

static void pr_skel_processLink (struct Unit *unit_ptr, struct Link *link_ptr)

{

    link_ptr->to->value_a += unit_ptr->value_a * link_ptr->weight;

}


/*****************************************************************************
  FUNCTION : pr_skel_check_saliency

  PURPOSE  : update minimum saliency of net
  RETURNS  : void
  NOTES    : 

  UPDATE   : Thomas Gern, 07.09.1995
             - saliency = actbuf[0]
	     - search for absolute minimum
******************************************************************************/

static void pr_skel_check_saliency (struct Unit *unit_ptr)

{

    float saliency;

    /* calculate saliency of unit */
    unit_ptr->actbuf[0] = 0.8 * unit_ptr->actbuf[0] + 0.2 * unit_ptr->value_b;
    saliency = unit_ptr->actbuf[0];
    if (saliency != 0.0)   /* is it the initial-value? (for already pruned input units)*/ 
      if ((pr_candidateUnit == NULL) ||
	  (fabs(saliency) < pr_candidateSaliency))
	/* found first or least important unit so far */
	{
	  pr_candidateUnit = unit_ptr;
	  pr_candidateSaliency = fabs(saliency);
	}

}


/*****************************************************************************
  FUNCTION : PRUNE_Skeletonization

  PURPOSE  : 
  RETURNS  :
  NOTES    : 

  UPDATE   :
******************************************************************************/

krui_err PRUNE_Skeletonization (int pattern)

{

    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register struct Link *link_ptr;
    register TopoPtrArray topo_ptr;
    static bool first = TRUE;

    register Patterns out_pat;
    int size, pattern_no, sub_pat_no, no_of_patterns;

    if (first){
      FOR_ALL_UNITS (unit_ptr)
	unit_ptr->actbuf[0] = 0.0;
      first = FALSE;
    }

    FOR_ALL_UNITS (unit_ptr)
	unit_ptr->value_b = 0.0;
    
    /* compute the necessary sub patterns */
    if (pattern == PR_ALL_PATTERNS)
	/* all patterns */
	KernelErrorCode = kr_initSubPatternOrder
	    (0, kr_np_pattern (PATTERN_GET_NUMBER, 0, 0) - 1);
    else
	/* single pattern */
	KernelErrorCode = kr_initSubPatternOrder (pattern, pattern);
    if (KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    /* calculate total number of subpatterns */
    no_of_patterns = kr_TotalNoOfSubPatPairs ();

    while (kr_getSubPatternByOrder (&pattern_no, &sub_pat_no))
    {
      /* initialize value_a of each unit */
      FOR_ALL_UNITS (unit_ptr)
	unit_ptr->value_a = 0.0;


      /* calculate address of the output pattern */
      out_pat = kr_getSubPatData (pattern_no, sub_pat_no, OUTPUT, &size);
      out_pat += size;
	
      /* propagate pattern through net */
      propagateNetForward (pattern_no, sub_pat_no);

      /* calculate deviation for output units */
      for (topo_ptr = topo_ptr_array + no_of_topo_units + 2,
	   unit_ptr = *(topo_ptr--); unit_ptr != NULL;
	   unit_ptr = *(topo_ptr--))
	unit_ptr->value_a = (float) *(--out_pat) - unit_ptr->Out.output;

      /* calculate output units */
      for (topo_ptr = topo_ptr_array + no_of_topo_units + 2,
	   unit_ptr = *(topo_ptr--);
	   unit_ptr != NULL;
	   unit_ptr = *(topo_ptr--))
	{
	  /* calculate delta */
	  if (unit_ptr->value_a > 0)
	    unit_ptr->value_a = (unit_ptr->act_deriv_func) (unit_ptr);
	  else
	    if (unit_ptr->value_a < 0)
	      unit_ptr->value_a = - (unit_ptr->act_deriv_func) (unit_ptr);
	    else
	      unit_ptr->value_a = 0;

	  /* process all links to that unit */
	  if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
	    /* unit has direct inputs */
	    FOR_ALL_LINKS (unit_ptr, link_ptr)
	      pr_skel_processLink (unit_ptr, link_ptr);
	  else
	    /* unit has sites */
	    FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
	      pr_skel_processLink (unit_ptr, link_ptr);
	}
      
      /* calculate hidden units */
      for (unit_ptr = *(topo_ptr--);
	   unit_ptr != NULL;
	   unit_ptr = *(topo_ptr--))
	{
	  /* check saliency of unit if not input pruning */
	  unit_ptr->value_b += unit_ptr->value_a * unit_ptr->Out.output;
	  
	  /* calculate delta */
	  unit_ptr->value_a *= (unit_ptr->act_deriv_func) (unit_ptr);
	  
	  /* process all links to that unit */
	  if UNIT_HAS_DIRECT_INPUTS (unit_ptr)
	    /* unit has direct inputs */
	    FOR_ALL_LINKS (unit_ptr, link_ptr)
	      pr_skel_processLink (unit_ptr, link_ptr);
	  else
	    /* unit has sites */
	    FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
	      pr_skel_processLink (unit_ptr, link_ptr);
	}

      if (pr_inputPruning) 
	/* calculate input units */
	for (unit_ptr = *(topo_ptr--);
	     unit_ptr != NULL;
	     unit_ptr = *(topo_ptr--))
	  /* check saliency of unit */
	  unit_ptr->value_b += unit_ptr->value_a * unit_ptr->Out.output;
      
      
    }

    for (topo_ptr = topo_ptr_array + no_of_topo_units + 2,
	   unit_ptr = *(topo_ptr--);
	   unit_ptr != NULL;
	   unit_ptr = *(topo_ptr--));

    /* check hidden units */
    for (unit_ptr = *(topo_ptr--);
	 unit_ptr != NULL;
	 unit_ptr = *(topo_ptr--)){
      if (pr_hiddenPruning)
	pr_skel_check_saliency (unit_ptr); 
    }

    /* check input units */
    if (pr_inputPruning) 
      /* calculate input units */
      for (unit_ptr = *(topo_ptr--);
	   unit_ptr != NULL;
	   unit_ptr = *(topo_ptr--))
	pr_skel_check_saliency (unit_ptr); 


    if (pr_candidateUnit != NULL){
      pr_candidateUnit->actbuf[0] = 0.0;
    }
      
    return (KRERR_NO_ERROR);

}


/*****************************************************************************

  GROUP: Noncontributing Units
  AUTHOR: Jochen Biedermann

******************************************************************************/


/*****************************************************************************
  FUNCTION : pr_nc_calc_stddev
  PURPOSE  : calculates mean and standard deviation of output 
             of each unit (pr_Pass: PR_CONST) resp. mean of each unit output
             and stddev between output of 'this_unit' and each other unit 
             (pr_Pass: PR_SAME) resp. stddev between output of 'this_unit' and
             the reverse (1 - output) of each other unit (pr_Pass: PR_REVERSE) 
  NOTES    : mean in value_b, stddev in value_c
  RETURNS  : kernel error code
******************************************************************************/

static krui_err pr_nc_calc_stddev (int pattern, struct Unit *this_unit_ptr)
{
    struct Unit *unit_ptr;
    int pattern_no, sub_pat_no, no_of_patterns;

    /* initialize value_b and value_c of each unit */
    FOR_ALL_UNITS (unit_ptr) 
        if (! IS_SPECIAL_UNIT (unit_ptr)) {
           unit_ptr->value_b = 0;
           unit_ptr->value_c = 0;
        }

    /* compute the necessary sub patterns */
    if (pattern == PR_ALL_PATTERNS)        /* all patterns */
        KernelErrorCode = kr_initSubPatternOrder
            (0, kr_np_pattern (PATTERN_GET_NUMBER, 0, 0) - 1);
    else        /* single pattern */
        KernelErrorCode = kr_initSubPatternOrder (pattern, pattern);
    if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode);

    /* calculate total number of subpatterns */
    no_of_patterns = kr_TotalNoOfSubPatPairs ();

    while (kr_getSubPatternByOrder (&pattern_no, &sub_pat_no)) {
        /* propagate pattern through net */
        propagateNetForward (pattern_no, sub_pat_no);

        /* calculate mean for all units */
        FOR_ALL_UNITS (unit_ptr)
            if (! IS_SPECIAL_UNIT (unit_ptr))
                unit_ptr->value_b += unit_ptr->Out.output;
    }

    FOR_ALL_UNITS (unit_ptr)
        if (! IS_SPECIAL_UNIT (unit_ptr)) 
            unit_ptr->value_b /= (float) no_of_patterns;
        
    /* compute the necessary sub patterns */
    if (pattern == PR_ALL_PATTERNS)        /* all patterns */
        KernelErrorCode = kr_initSubPatternOrder
            (0, kr_np_pattern (PATTERN_GET_NUMBER, 0, 0) - 1);
    else        /* single pattern */
        KernelErrorCode = kr_initSubPatternOrder (pattern, pattern);
    if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode);

    while (kr_getSubPatternByOrder (&pattern_no, &sub_pat_no)) {
        /* propagate pattern through net */
        propagateNetForward (pattern_no, sub_pat_no);

        /* calculate mean for all units */
        FOR_ALL_UNITS (unit_ptr)
            if (! IS_SPECIAL_UNIT (unit_ptr)) 
                if (pr_Pass == PR_CONST) {
                    unit_ptr->value_c += (unit_ptr->Out.output - unit_ptr->value_b)
                                         * (unit_ptr->Out.output - unit_ptr->value_b);
		} else if (pr_Pass == PR_SAME) {
                    unit_ptr->value_c += (unit_ptr->Out.output 
                                             - this_unit_ptr->Out.output)
                                         * (unit_ptr->Out.output 
                                             - this_unit_ptr->Out.output);
                } else {
                    unit_ptr->value_c += (unit_ptr->Out.output 
                                             + this_unit_ptr->Out.output - 1.0)
                                         * (unit_ptr->Out.output 
                                             + this_unit_ptr->Out.output - 1.0);
                } 
    }

    FOR_ALL_UNITS (unit_ptr)
        if (! IS_SPECIAL_UNIT (unit_ptr)) {
            unit_ptr->value_c = sqrt ((double) unit_ptr->value_c 
                                      / (double) no_of_patterns);
        }        

    return (KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : pr_nc_check_stddev
  PURPOSE  : checks, if 'unit' beats minimum stddev of the net 
  NOTES    : if so, saves unit_ptr, target_unit_ptr (if any), stddev and current
             pr_Pass
  RETURNS  : kernel error code
******************************************************************************/

static void pr_nc_check_stddev (struct Unit *unit_ptr, struct Unit *unit_ptr2)
{
    float stddev;

    stddev = unit_ptr->value_c;

    if ((stddev >= 0 && (pr_candidateUnit == NULL)) || (stddev < pr_candidateStddev)) {
        /* found first or smallest unit so far */
        pr_candidateUnit       = unit_ptr;
        pr_candidateTargetUnit = unit_ptr2;
        pr_candidateStddev     = stddev;
        pr_candidatePass       = pr_Pass;
    }
}

/*****************************************************************************
  FUNCTION : pr_nc_process_succ_unit
  PURPOSE  : bias of a succeeding unit (of the pruning candidate unit) and link
             between this succ. unit and the candidate target unit 
	     (the unit which will replace the candidate unit) 
	     are changed (or created)
  NOTES    : pr_Pass: 
             PR_CONST:   to the bias the product of the link weight 
                         between the candidate unit and the succ. unit 
		         and the mean output of the candidate unit is added;
	     PR_SAME:    to the link weight between the target candidate 
	                 unit and the succ. unit the link weight between
			 the candidate unit and the succ. unit is added;
             PR_REVERSE: the link weight between the candidate unit and 
	                 the succ. unit is subtracted from the link weight
			 between the candidate target and the succ. unit. 
			 Additionally, this link weight is added to the 
			 bias of the succ. unit
  RETURNS  : kernel error code
******************************************************************************/

static krui_err pr_nc_process_succ_unit (struct Unit *unit_ptr, struct Link *link_ptr)
{ 
    struct Site *site_ptr2;
    struct Link *link_ptr2;
    FlintType w;
    int       unit_no, target_unit_no;

    if (unit_ptr == NULL) return (KRERR_UNIT_MISSING);
    unit_no = unit_ptr - unit_array;
    w = link_ptr->weight;

    if (pr_candidatePass == PR_CONST) {
        unit_ptr->bias += w * pr_candidateUnit->value_b;
    } else {
        target_unit_no = pr_candidateTargetUnit - unit_array;

        if (pr_candidatePass == PR_REVERSE) w = -w;

        KernelErrorCode = kr_setCurrUnit (unit_no);
        if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode);
        KernelErrorCode = kr_createLink (target_unit_no, w); 
        if (KernelErrorCode != KRERR_NO_ERROR) {
            if (KernelErrorCode == KRERR_ALREADY_CONNECTED) {
                if UNIT_HAS_DIRECT_INPUTS (unit_ptr) { /* unit has direct inputs */
                    FOR_ALL_LINKS (unit_ptr, link_ptr2)
                        if (link_ptr2->to == pr_candidateTargetUnit) {
                            link_ptr2->weight += w;
  		        }
                } else {                               /* unit has sites */
                    FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr2, link_ptr2)
                        if (link_ptr2->to == pr_candidateTargetUnit) {
                            link_ptr2->weight += w;
		        }
		}
            } else { 
                return (KernelErrorCode);
	    }
        } 

        if (pr_candidatePass == PR_REVERSE) unit_ptr->bias -= w;
    }

    return (KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : pr_nc_remove_unit
  PURPOSE  : calls 'pr_nc_process_succ_unit' for each succeeding unit of the
             pruning candidate unit and deletes (if pr_reallyDelete is TRUE)
             or disables (if pr_reallyDelete is FALSE) the candidate unit
  RETURNS  : kernel error code
******************************************************************************/

static krui_err pr_nc_remove_unit (void)
{ 
    struct Unit *unit_ptr;
    struct Site *site_ptr;
    struct Link *link_ptr;

    if (pr_candidateUnit == NULL) return (KRERR_NO_ERROR);
    if (pr_candidatePass != PR_CONST && pr_candidateTargetUnit == NULL) 
        return (KRERR_UNIT_MISSING);

    FOR_ALL_UNITS (unit_ptr) {
        if (! IS_SPECIAL_UNIT (unit_ptr))
            if UNIT_HAS_DIRECT_INPUTS (unit_ptr) {  /* unit has direct inputs */
                FOR_ALL_LINKS (unit_ptr, link_ptr)
                    if (link_ptr->to == pr_candidateUnit) {
                        KernelErrorCode = pr_nc_process_succ_unit (unit_ptr, link_ptr);
                        if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode);
		    }
            } else {                                /* unit has sites */
                FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
                    if (link_ptr->to == pr_candidateUnit) {
                        KernelErrorCode = pr_nc_process_succ_unit (unit_ptr, link_ptr);
                        if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode);
		    }
	    }
    }

    pr_candidateUnit->bias = 0;
    pr_candidateUnit->Out.output = 0;
    pr_candidateUnit->act = 0;

    kr_deleteAllInputs (pr_candidateUnit);        /* delete incoming links */
    kr_deleteAllOutputLinks (pr_candidateUnit);   /* delete outgoing links */

    return (KRERR_NO_ERROR);
}

#if 0

/*****************************************************************************
  FUNCTION : pr_nc_output
  PURPOSE  : name, number, type and layer no. for the pruning candidate unit
             and the target unit (if any) together with the stddev value are
             printed out
  RETURNS  : void
******************************************************************************/

static void pr_nc_output (void)
{
    int candidate_unit_no, target_unit_no;

    if (pr_candidateUnit == NULL) return;
    candidate_unit_no = pr_candidateUnit - unit_array;
    if (pr_candidatePass != PR_CONST) {
        if (pr_candidateTargetUnit == NULL) return;
        target_unit_no = pr_candidateTargetUnit - unit_array;
    }

    printf ("  ");
    switch (krui_getUnitTType (candidate_unit_no)) {
      case INPUT:   printf ("Input-"); break; 
      case OUTPUT:  printf ("Output-"); break; 
      case HIDDEN:  printf ("Hidden-"); break; 
      default:      break;
    }
    printf ("Unit ");
    if (pr_candidateUnit -> unit_name != NULL) 
        printf ("%s, ", pr_candidateUnit -> unit_name);
    printf ("No. %d pruned ", candidate_unit_no);
    switch (pr_candidatePass) {
      case PR_CONST:    printf ("(const"); break;
      case PR_SAME:     printf ("(same as "); break;
      case PR_REVERSE:  printf ("(reverse to "); break;
      default:          break;  
    }
    if (pr_candidatePass != PR_CONST) {
        switch (krui_getUnitTType (target_unit_no)) {
          case INPUT:   printf ("Input-"); break; 
          case OUTPUT:  printf ("Output-"); break; 
          case HIDDEN:  printf ("Hidden-"); break; 
          default:      break;
        }
        printf ("Unit ");
        if (pr_candidateTargetUnit -> unit_name != NULL) 
            printf ("%s, ", pr_candidateTargetUnit -> unit_name);
        printf ("No. %d", target_unit_no);
    }
    printf ("), StdDev: %lf\n", (double) pr_candidateStddev);      
}

#endif

/*****************************************************************************
  FUNCTION : pr_nc_clear_marks
  PURPOSE  : clear all marks 
  RETURNS  : void
******************************************************************************/

static void pr_nc_clear_marks (void)
{
    struct Unit *unit_ptr;

    FOR_ALL_UNITS (unit_ptr)
        if (! IS_SPECIAL_UNIT (unit_ptr)) unit_ptr->flags &= ~UFLAG_PRUNEFLAG; 
}

/*****************************************************************************
  FUNCTION : pr_nc_mark_all_pred
  PURPOSE  : marks recursively all predating units
  RETURNS  : void
******************************************************************************/

static void pr_nc_mark_all_pred (struct Unit *unit_ptr)
{
    struct Site *site_ptr;
    struct Link *link_ptr;

    if (unit_ptr == NULL || IS_SPECIAL_UNIT (unit_ptr)) return;

    unit_ptr->flags |= UFLAG_PRUNEFLAG;

    if UNIT_HAS_DIRECT_INPUTS (unit_ptr) {  /* unit has direct inputs */
        FOR_ALL_LINKS (unit_ptr, link_ptr)
            pr_nc_mark_all_pred (link_ptr->to);
    } else {                                /* unit has sites */
        FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
            pr_nc_mark_all_pred (link_ptr->to);
    }
}

/*****************************************************************************
  FUNCTION : PRUNE_Noncontributing
  PURPOSE  : this procedure looks for the best unit to prune. In the first pass
             (pr_Pass: PR_CONST) a nearly constant unit is searched, in the
             2nd pass (pr_Pass: PR_SAME) two units with nearly identical outputs
             and in the 3rd pass (pr_Pass: PR_REVERSE) two units with nearly
             reverse outputs are searched 
  NOTES    : the procedure 'pr_nc_check_stddev' is called for saving the 'best'
             unit to prune in 'pr_candidateUnit'
  RETURNS  : kernel error code
******************************************************************************/

krui_err PRUNE_Noncontributing (int pattern)
{
    struct Unit *unit_ptr, *unit_ptr2;

    /* Looking for unit to prune */
    pr_Pass = PR_CONST;

    KernelErrorCode = pr_nc_calc_stddev (pattern, NULL); 
    if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode);

    FOR_ALL_UNITS (unit_ptr)
        if (! IS_SPECIAL_UNIT (unit_ptr) && 
            ((IS_INPUT_UNIT (unit_ptr) && pr_inputPruning) || 
             (IS_HIDDEN_UNIT (unit_ptr) && pr_hiddenPruning))) 
            pr_nc_check_stddev (unit_ptr, NULL);


    FOR_ALL_UNITS (unit_ptr)
        if (! IS_SPECIAL_UNIT (unit_ptr) && ! IS_OUTPUT_UNIT (unit_ptr)) {
            pr_nc_clear_marks ();
            pr_nc_mark_all_pred (unit_ptr);


            pr_Pass = PR_SAME;
            KernelErrorCode = pr_nc_calc_stddev (pattern, unit_ptr); 
            if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode);
 
            FOR_ALL_UNITS (unit_ptr2)
                if (! IS_SPECIAL_UNIT (unit_ptr2) && unit_ptr != unit_ptr2
                    && ((IS_INPUT_UNIT (unit_ptr2) && pr_inputPruning) || 
                       (IS_HIDDEN_UNIT (unit_ptr2) && pr_hiddenPruning)) 
                    && ! (unit_ptr2->flags & UFLAG_PRUNEFLAG)) {
                    pr_nc_check_stddev (unit_ptr2, unit_ptr);
		}


            pr_Pass = PR_REVERSE;
  
            KernelErrorCode = pr_nc_calc_stddev (pattern, unit_ptr); 
            if (KernelErrorCode != KRERR_NO_ERROR) return (KernelErrorCode);

            FOR_ALL_UNITS (unit_ptr2)
                if (! IS_SPECIAL_UNIT (unit_ptr2) && unit_ptr != unit_ptr2
                    && ((IS_INPUT_UNIT (unit_ptr2) && pr_inputPruning) || 
                       (IS_HIDDEN_UNIT (unit_ptr2) && pr_hiddenPruning)) 
                    && ! (unit_ptr2->flags & UFLAG_PRUNEFLAG))
                    pr_nc_check_stddev (unit_ptr2, unit_ptr);
        }

#if 0
    pr_nc_output ();
#endif
    KernelErrorCode = pr_nc_remove_unit ();

    return (KernelErrorCode);

}

/*****************************************************************************

  GROUP: Calling Function

******************************************************************************/


/*****************************************************************************
  FUNCTION : pr_callPrunFunc
  PURPOSE  : calls the current pruning function and prunes link or
             unit with smallest saliency
  NOTES    : special flags are reset!
  RETURNS  : returns error passed by pruning function
******************************************************************************/

krui_err pr_callPrunFunc (int pattern)
{
    FunctionPtr func_ptr;
    NetFunctionPtr net_func_ptr;
    char *curr_func;
    int nodePruning;
    FlintType dummy;

    /* get current pruning function */
    if ((curr_func = krf_getCurrentNetworkFunc (PRUNING_FUNC)) == NULL)
        return (KernelErrorCode);
    if (!krf_funcSearch (curr_func, PRUNING_FUNC, &func_ptr))
        return (KernelErrorCode);
    net_func_ptr = (NetFunctionPtr) func_ptr;

    /* check node pruning */
    nodePruning = (strcmp (curr_func, "Skeletonization") == 0) ||
	(strcmp (curr_func, "Noncontributing_Units") == 0);

    /* initialize global variables */
    pr_candidateUnit       = NULL;
    pr_candidateTargetUnit = NULL;
    pr_candidateLink       = NULL;

    /* invoke function */
    KernelErrorCode = (*(PrunFuncPtr) net_func_ptr) (pattern);
    if (KernelErrorCode != KRERR_NO_ERROR)
	return (KernelErrorCode);

    if (nodePruning)
    {
        kr_deleteAllInputs (pr_candidateUnit);        /* delete incoming links */
        kr_deleteAllOutputLinks (pr_candidateUnit);   /* delete outgoing links */
    }
    else
    {
        unitPtr = pr_candidateTargetUnit;
        unitNo = unitPtr - unit_array;
        pr_candidateSourceUnitNo =
            pr_candidateLink->to - unit_array;
        kr_isConnected (pr_candidateSourceUnitNo, &dummy);
        kr_deleteLink ();
    }

    NetModified = TRUE;

    /* delete dead units, if necessary */
    pr_checkDeadUnits ();
    
    return (KernelErrorCode);

}

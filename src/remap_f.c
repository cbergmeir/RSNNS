/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/remap_f.c,v $
  SHORTNAME      : remap_f
  SNNS VERSION   : 4.2

  PURPOSE        : pattern remapping functions
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/04/08 09:17:37 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <math.h>

#include "glob_typ.h"

#include "remap_f.ph"


/*****************************************************************************
  FUNCTION : REMAP_none

  PURPOSE  : default pattern remapping function that does no remapping at all.
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
krui_err  REMAP_none(float *pat_data, int pat_size, 
		     float *remap_params, int no_of_remap_params)
{
    KernelErrorCode = KRERR_NO_ERROR;
    return(KernelErrorCode);
}


/*****************************************************************************
  FUNCTION : REMAP_binary

  PURPOSE  : produces the patterns for a binary classifier. All patterns 
             greater than 0.5 are mapped to one, all others to 0
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
krui_err  REMAP_binary(float *pat_data, int pat_size, 
		       float *remap_params, int no_of_remap_params)
{

    register int   i;

    for(i=0; i<pat_size; i++){
	*pat_data = (*pat_data < 0.5)? 0.0: 1.0;
	pat_data++;
    }

    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : REMAP_invers

  PURPOSE  : inverts the patterns of a binary classifier
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
krui_err  REMAP_invers(float *pat_data, int pat_size, 
		       float *remap_params, int no_of_remap_params)
{
    register int   i;

    for(i=0; i<pat_size; i++){
	*pat_data = (*pat_data < 0.5)? 1.0: 0.0;
	pat_data++;
    }

    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : REMAP_clip

  PURPOSE  : clips values below and above the limits
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
krui_err  REMAP_clip(float *pat_data, int pat_size, 
		     float *remap_params, int no_of_remap_params)
{
    register float param1;
    register float param2;
    register int   i;

    param1 = remap_params[0];
    param2 = remap_params[1];

    for(i=0; i<pat_size; i++){
	*pat_data = (*pat_data < param1)? param1: 
	                                  ((*pat_data > param2)? param2:
					                         *pat_data);
	pat_data++;
    }

    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : REMAP_norm

  PURPOSE  : produces normalized patterns, i.e. the length of the pattern 
             vector is 1 after calling this function.
	     Some learning algorithms like DLVQ require the output patterns 
	     to be normalized. 
  RETURNS  :
  UPDATE   : 
******************************************************************************/
krui_err  REMAP_norm(float *pat_data, int pat_size, 
		     float *remap_params, int no_of_remap_params)
{
    register int i;
    double length = 0.0;

    for(i=0; i<pat_size; i++){
	length += pat_data[i] * pat_data[i];
    }
    length = sqrt(length);

    if(length == 0.0)
	return(KRERR_CANT_NORM);

    for(i=0; i<pat_size; i++)
	pat_data[i] = pat_data[i] / length;

    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : REMAP_threshold

  PURPOSE  : produces a pattern with only two output values. All values 
             within [lower threshold value, upper threshold value] are mapped 
	     to "background" value, the others to "signal" value. If the two 
	     thresholds are equal, values below it will be mapped to background.
  NOTES    : Parameter meanings: 0 - lower threshold value
                                 1 - upper threshold value
				 2 - background value
				 3 - signal value
  RETURNS  : 0 or kernel error code KRERR_PARAMETERS
  UPDATE   : 
******************************************************************************/
krui_err  REMAP_threshold(float *pat_data, int pat_size, 
		     float *remap_params, int no_of_remap_params)
{

    register float param1;
    register float param2;
    register float param3;
    register float param4;
    register int   i;

    param1 = remap_params[0];
    param2 = remap_params[1];
    param3 = remap_params[2];
    param4 = remap_params[3];

    if(param1 == param2){
	for(i=0; i<pat_size; i++){
	    *pat_data = (*pat_data < param1)? param3: param4;
	    pat_data++;
	}
    }else{
	for(i=0; i<pat_size; i++){
	    *pat_data = (*pat_data < param1)? param4:
		                              ((*pat_data > param2)? param4:
		                                                     param3);
	    pat_data++;
	}
    }
    return(KRERR_NO_ERROR);
}

/*****************************************************************************
  FUNCTION : REMAP_linearscale

  PURPOSE  : rescales the pattern outputs to a new range like
             out = a*orig_out + b

  NOTES    : Parameter meanings: 0 - factor a
                                 1 - addin b

  RETURNS  : 0 or kernel error code KRERR_PARAMETERS
  UPDATE   : 
******************************************************************************/
krui_err  REMAP_linearscale(float *pat_data, int pat_size, 
			    float *remap_params, int no_of_remap_params)
{

    register float param1;
    register float param2;
    register int   i;

    param1 = remap_params[0];
    param2 = remap_params[1];

    for(i=0; i<pat_size; i++){
	*pat_data = param1 * *pat_data + param2;
	pat_data++;
    }

    return(KRERR_NO_ERROR);
}

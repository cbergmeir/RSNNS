/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/remap_f.h,v $
  SHORTNAME      : remap_f
  SNNS VERSION   : 4.2

  PURPOSE        : public header
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.5 $
  LAST CHANGE    : $Date: 1998/04/08 09:17:37 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _REMAP_F_DEFINED_
#define  _REMAP_F_DEFINED_

extern krui_err  REMAP_none(float *pat_data, int pat_size, 
			    float *remap_params, int no_of_remap_params);
extern krui_err  REMAP_binary(float *pat_data, int pat_size, 
			      float *remap_params, int no_of_remap_params);
extern krui_err  REMAP_invers(float *pat_data, int pat_size, 
			      float *remap_params, int no_of_remap_params);
extern krui_err  REMAP_clip(float *pat_data, int pat_size, 
			    float *remap_params, int no_of_remap_params);
extern krui_err  REMAP_norm(float *pat_data, int pat_size, 
			    float *remap_params, int no_of_remap_params);
extern krui_err  REMAP_threshold(float *pat_data, int pat_size, 
				 float *remap_params, int no_of_remap_params);
extern krui_err  REMAP_linearscale(float *pat_data, int pat_size, 
				  float *remap_params, int no_of_remap_params);
#endif 



/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/init_f.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Network Initialisation Functions
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 18.03.91

  CHANGED BY     : Sven Doering, Ralf Huebner, Marc Seemann (Uni Tuebingen)
  RCS VERSION    : $Revision: 2.23 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:30 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <math.h>
#include <stdio.h>

#include "kr_typ.h"	 /*  Kernel Types and Constants             */
#include "kr_const.h"	 /*  Constant Declarators for SNNS-Kernel   */
#include "kr_def.h"	 /*  Default Values                         */
#include "kernel.h"	 /*  SNNS-Kernel Function Prototypes        */
#include "glob_typ.h"    /*  Global Types                           */
#include "random.h"	 /*  Randomize Library Function Prototypes  */
#include "kr_mac.h"	 /*  Kernel Macros                          */
#include "kr_art.h"      /*  Prototypes and global defs for ART     */
#include "kr_art1.h"     /*  Prototypes and global defs for ART1    */
#include "kr_art2.h"     /*  Prototypes and global defs for ART2    */
#include "kr_amap.h"     /*  Prototypes and global defs for ARTMAP  */
#include "krart_df.h"    /*  Definitions for ART functions          */
#include "learn_f.h"     /*  For the RBF functions                  */
#include "kr_ui.h"
#include "cc_type.h"
#include "cc_glob.h"      /* Definitions for cascade                 */
#include "matrix.h"
#include "kr_newpattern.h"

#include "init_f.ph"

extern FlintType ACT_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_DERIV_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_2_DERIV_Custom_Python(struct Unit * unit_ptr);



/*****************************************************************************
  FUNCTION : INIT_randomizeWeights

  PURPOSE  : Initializes connection weights with uniform distributed 
             random values.
  RETURNS  : 
  NOTES    : Function calls drand48(). <min> must be less then <max>.

  UPDATE   : 05.04.94 by Sven Doering
******************************************************************************/
krui_err  INIT_randomizeWeights(float *parameterArray, int NoOfParams)
{
    register unsigned short    flags;
    register struct Link   *link_ptr;
    register struct Site   *site_ptr;
    register struct Unit   *unit_ptr;
    register FlintType     range, min_weight;
    FlintType   max_weight;
    /* make sure the random number generator is initialized before
       this function is run */
    static char notfirstrun;

    if(notfirstrun == 0) {
	krui_setSeedNo(0);
	notfirstrun = 1;
    }

    if ( (unit_array == NULL) || (NoOfUnits == 0) )
	return( KRERR_NO_UNITS ); /*  there is nothing to do  */

    min_weight = INIT_PARAM1( parameterArray );
    max_weight = INIT_PARAM2( parameterArray ); 
    range = max_weight - min_weight;

    if (range == 0.0)  {
	FOR_ALL_UNITS( unit_ptr )  {
	    flags = unit_ptr->flags;

	    if ( ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) 
		&& (!IS_SPECIAL_UNIT(unit_ptr))) {
		/*  unit is in use  */
		unit_ptr->bias = min_weight;

		if ( (flags & UFLAG_INPUT_PAT) == UFLAG_SITES ) {
		    /*  unit has sites  */
		    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
			link_ptr->weight = min_weight;
		}else{
		    /*  unit has no sites   */
		    if ( (flags & UFLAG_INPUT_PAT) == UFLAG_DLINKS ) {
			/*  unit has direct links   */
			FOR_ALL_LINKS( unit_ptr, link_ptr )
			    link_ptr->weight = min_weight;
		    }
		}
	    }
	}
    } else  {
	FOR_ALL_UNITS( unit_ptr )  {
	    flags = unit_ptr->flags;

	    if ( ((flags & UFLAG_IN_USE) == UFLAG_IN_USE)
		&& (!IS_SPECIAL_UNIT(unit_ptr))) {
		/*  unit is in use  */
		unit_ptr->bias = (FlintType) drand48() * range + min_weight;

		if ( (flags & UFLAG_INPUT_PAT) == UFLAG_SITES ) {
		    /*  unit has sites  */
		    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
			link_ptr->weight = 
			    (FlintType)drand48() * range + min_weight;
		} else  {
		    /*  unit has no sites   */
		    if ( (flags & UFLAG_INPUT_PAT) == UFLAG_DLINKS ){
			/*  unit has direct links   */
			FOR_ALL_LINKS( unit_ptr, link_ptr )
			    link_ptr->weight = 
				(FlintType) drand48() * range + min_weight;
		    }
		}
	    }
	}
    }

    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : INIT_RM_randomizeWeights

  PURPOSE  : Initializes connection weights between hidden units with uniform 
             distributed random values. Connections from input units are not 
	     changed. This kind of initialization is necessary for 
	     autoassociative memory networks.
  RETURNS  : 
  NOTES    : Function calls drand48(). <min> must be less then <max>.

  UPDATE   : 11.02.94
******************************************************************************/
krui_err  INIT_RM_randomizeWeights(float *parameterArray, int NoOfParams)
{
  register unsigned short    flags;
  register struct Link   *link_ptr;
  register struct Site   *site_ptr;
  register struct Unit   *unit_ptr;
  register FlintType     range, min_weight;
  FlintType   max_weight;

  if ( (unit_array == NULL) || (NoOfUnits == 0) )
    return( KRERR_NO_UNITS );  /*  there is nothing to do  */

  min_weight = INIT_PARAM1( parameterArray );
  max_weight = INIT_PARAM2( parameterArray ); 
  range = max_weight - min_weight;

  if (range == 0.0)  {
    FOR_ALL_UNITS( unit_ptr )  {
      flags = unit_ptr->flags;

      if ( ((flags & UFLAG_IN_USE) == UFLAG_IN_USE)
	  && (!IS_SPECIAL_UNIT(unit_ptr)))
        {  /*  unit is in use  */
        unit_ptr->bias = min_weight;

        if ( (flags & UFLAG_INPUT_PAT) == UFLAG_SITES )
          {  /*  unit has sites  */
	  FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
            if (!IS_INPUT_UNIT (link_ptr->to))
              link_ptr->weight = min_weight;
        }else
          {       /*  unit has no sites   */
          if ( (flags & UFLAG_INPUT_PAT) == UFLAG_DLINKS )
            {     /*  unit has direct links   */
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
              if (!IS_INPUT_UNIT (link_ptr->to))
              link_ptr->weight = min_weight;
          }
        }
      }
    }
  }else  {
    FOR_ALL_UNITS( unit_ptr )  {
      flags = unit_ptr->flags;

      if ( ((flags & UFLAG_IN_USE) == UFLAG_IN_USE)
	  && (!IS_SPECIAL_UNIT(unit_ptr)))
        {     /*  unit is in use  */
        unit_ptr->bias = (FlintType) drand48() * range + min_weight;

        if ( (flags & UFLAG_INPUT_PAT) == UFLAG_SITES )
          {       /*  unit has sites  */
	  FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
            if (!IS_INPUT_UNIT (link_ptr->to))
            link_ptr->weight = (FlintType) drand48() * range + min_weight;
        }else  {
          /*  unit has no sites   */
          if ( (flags & UFLAG_INPUT_PAT) == UFLAG_DLINKS )
            {     /*  unit has direct links   */
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
              if (!IS_INPUT_UNIT (link_ptr->to))
              link_ptr->weight = (FlintType) drand48() * range + min_weight;
          }
        }
      }
    }
  }

  return( KRERR_NO_ERROR );
}




/*****************************************************************************
  FUNCTION : INIT_randomizeWeights_perc

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err  INIT_randomizeWeights_perc(float *parameterArray, int NoOfParams)
{
    register unsigned short    flags;
    register struct Link   *link_ptr;
    register struct Site   *site_ptr;
    register struct Unit   *unit_ptr;
    register FlintType     range, min_weight;
    FlintType              max_weight;
    FlintType              ar;

    if ( (unit_array == NULL) || (NoOfUnits == 0) )
	return( KRERR_NO_UNITS ); /*  there is nothing to do  */

    min_weight = INIT_PARAM1( parameterArray );
    max_weight = INIT_PARAM2( parameterArray );
    range = max_weight - min_weight;

    /* compute no of predecessor Units */
    FOR_ALL_UNITS( unit_ptr ){
	unit_ptr->value_c=0.0;
	FOR_ALL_LINKS( unit_ptr, link_ptr ){
	    unit_ptr->value_c++;
	}
    }


    if (range == 0.0)  {
	FOR_ALL_UNITS( unit_ptr )  {
	    flags = unit_ptr->flags;

	    if ( ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) 
		&& (!IS_SPECIAL_UNIT(unit_ptr))) {
		/*  unit is in use  */
		unit_ptr->bias = min_weight;

		if ( (flags & UFLAG_INPUT_PAT) == UFLAG_SITES ){
		    /*  unit has sites  */
		    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
			link_ptr->weight = min_weight;
		} else  {
		    /*  unit has no sites   */
		    if ( (flags & UFLAG_INPUT_PAT) == UFLAG_DLINKS ) {
			/*  unit has direct links   */
			FOR_ALL_LINKS( unit_ptr, link_ptr )
			    link_ptr->weight = min_weight;
		    }
		}
	    }
	}
    } else  {
	FOR_ALL_UNITS( unit_ptr )  {
	    flags = unit_ptr->flags;

	    if ( ((flags & UFLAG_IN_USE) == UFLAG_IN_USE) 
		&& (!IS_SPECIAL_UNIT(unit_ptr))) {
		/*  unit is in use  */
		unit_ptr->bias = 0.0;

		if ( (flags & UFLAG_INPUT_PAT) == UFLAG_SITES ){
		    /*  unit has sites  */
		    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ){
			ar = unit_ptr->value_c ;
			link_ptr->weight = 
			    (FlintType) drand48() * 
			    (max_weight/ar - min_weight/ar) + (min_weight/ar);
		    }
		} else  {
		    /*  unit has no sites   */
		    if ( (flags & UFLAG_INPUT_PAT) == UFLAG_DLINKS ){
			/*  unit has direct links   */

			FOR_ALL_LINKS( unit_ptr, link_ptr ) {
			    ar = unit_ptr->value_c ;
			    link_ptr->weight = 
				(FlintType) drand48() * 
				(max_weight/ar - min_weight/ar) + 
				(min_weight/ar);
			}
		    }
		}
	    }
	}
    }
    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : INIT_Weights_CPNv32

  PURPOSE  : Old (Version 3.2) style initialization. Initialization vectors are
             taken from the RECTANGLE bounded by param1 and param2.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err INIT_Weights_CPNv32(float *parameterArray, int NoOfParams)
{
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register struct Link *link_ptr;
    register TopoPtrArray  topo_ptr;
    register FlintType  sum, amount, range;
    FlintType min, max;
    int  ret_code;


    if ( (unit_array == NULL) || (NoOfUnits == 0) )
	return( KRERR_NO_UNITS ); /*  there is nothing to do  */

    min = INIT_PARAM1( parameterArray );
    max = INIT_PARAM2( parameterArray );
    if(max < min){
	range = min; min = max; max = range;
    }
    range = max - min;

    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)){
	/*  networt was modified or topologic array isn't initialized  */
	ret_code = kr_topoSort( TOPOLOGIC_TYPE );
	if (ret_code != KRERR_NO_ERROR)
	    return( ret_code );

	NetModified = FALSE;
    }

    topo_ptr = topo_ptr_array + (NoOfInputUnits + 1);

    /*  initialize weights of the hidden units  */
    while ((unit_ptr = *++topo_ptr) != NULL){
	/*  this is a hidden unit  */

	/*     initialize the weights to the Kohonen Layer         */
	sum = 0.0;
	if UNIT_HAS_SITES( unit_ptr ){
	    /* the unit has sites */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )  {
		link_ptr->weight = (FlintType) drand48() * range + min;
		sum += link_ptr->weight * link_ptr->weight;
	    }
	}else{
	    /* the unit has direct links */
	    FOR_ALL_LINKS( unit_ptr, link_ptr )  {
		link_ptr->weight = (FlintType) drand48() * range + min;
		sum += link_ptr->weight * link_ptr->weight;
	    }
	}

	/* normalize the weightvector to the Kohonen Layer */
	amount = 1.0 / sqrt( sum );

	if UNIT_HAS_SITES( unit_ptr )
	    /* the unit has sites */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
		link_ptr->weight = link_ptr->weight * amount;
	else
	    /* the unit has direct links */
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
		link_ptr->weight = link_ptr->weight * amount;
    }


    while ((unit_ptr = *++topo_ptr) != NULL){
	/* this is a output unit */

	/*     initialize the weights to the Grossberg Layer       */
	if UNIT_HAS_SITES( unit_ptr ){
	    /* the unit has sites */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
		link_ptr->weight = (FlintType) drand48() * range + min;
	}else{
	    /* the unit has direct links */
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
		link_ptr->weight = (FlintType) drand48() * range + min;
	}
    }

    return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : INIT_Weights_CPNv33

  PURPOSE  : New (Version 3.3) style initialization. Initialization vectors are
             taken from the CIRCLE bounded by param1 and param2.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err INIT_Weights_CPNv33(float *parameterArray, int NoOfParams)
{
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register struct Link *link_ptr;
    register TopoPtrArray  topo_ptr;
    register FlintType  sum, amount, range, intervall;
    FlintType min, max, offset;
    int  ret_code;


    if ( (unit_array == NULL) || (NoOfUnits == 0) )
	return( KRERR_NO_UNITS ); /*  there is nothing to do  */

    min = INIT_PARAM1( parameterArray );
    max = INIT_PARAM2( parameterArray );
    range = max - min;
    if (((min < 0.0)) && ((max < 0.0)))
    { intervall = 1.0; offset = -1.0;
    } else if (((min < 0.0)) && (!(max < 0.0)))
    { intervall = 2.0; offset = -1.0;
    } else if ((!(min < 0.0)) && ((max < 0.0)))
    { intervall = 2.0; offset = -1.0;
    } else if ((!(min < 0.0)) && (!(max < 0.0)))
    { intervall = 1.0; offset = 0.0;
    }

    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE)){
	/*  networt was modified or topologic array isn't initialized  */
	ret_code = kr_topoSort( TOPOLOGIC_TYPE );
	if (ret_code != KRERR_NO_ERROR)
	    return( ret_code );

	NetModified = FALSE;
    }

    topo_ptr = topo_ptr_array + (NoOfInputUnits + 1);

    /*  initialize weights of the hidden units  */
    while ((unit_ptr = *++topo_ptr) != NULL){
	/*  this is a hidden unit  */

	/*     initialize the weights to the Kohonen Layer         */
	if UNIT_HAS_SITES( unit_ptr ){
	    /* the unit has sites */
          do { sum = 0.0;
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )  {
		link_ptr->weight = (FlintType) drand48() * intervall + offset;
		sum += link_ptr->weight * link_ptr->weight;
	    }
          } while ((sum > 1.0) || (sum == 0.0));
	}else{
	    /* the unit has direct links */
          do { sum = 0.0;
	    FOR_ALL_LINKS( unit_ptr, link_ptr )  {
		link_ptr->weight = (FlintType) drand48() * intervall + offset;
		sum += link_ptr->weight * link_ptr->weight;
	    }
          } while ((sum > 1.0) || (sum == 0.0));
	}

	/* normalize the weightvector to the Kohonen Layer */
	amount = 1.0 / sqrt( sum );

	if UNIT_HAS_SITES( unit_ptr )
	    /* the unit has sites */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
		link_ptr->weight = link_ptr->weight * amount;
	else
	    /* the unit has direct links */
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
		link_ptr->weight = link_ptr->weight * amount;
    }


    while ((unit_ptr = *++topo_ptr) != NULL){
	/* this is a output unit */

	/*     initialize the weights to the Grossberg Layer       */
	if UNIT_HAS_SITES( unit_ptr ){
	    /* the unit has sites */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
		link_ptr->weight = (FlintType) drand48() * range + min;
	}else{
	    /* the unit has direct links */
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
		link_ptr->weight = (FlintType) drand48() * range + min;
	}
    }

    return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : INIT_Weights_CPN_Rand_Pat

  PURPOSE  : Initializes all weight vectors of the Kohonen layer with
  random input patterns from the training set. The weights of the
  Grossberg layer are all initialized to 1. 

  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err INIT_Weights_CPN_Rand_Pat(float *parameterArray, int NoOfParams)
{
    register struct Unit *unit_ptr;
    register struct Site *site_ptr;
    register struct Link *link_ptr;
    register TopoPtrArray  topo_ptr;
    krui_err ret_code;

    if ( (unit_array == NULL) || (NoOfUnits == 0) )
	return( KRERR_NO_UNITS ); /*  there is nothing to do  */

    /* test if patterns available and valid:			*/
    if (kr_TotalNoOfSubPatPairs() == 0)  
	return( KRERR_NO_PATTERNS );	/* no patterns defined  */

    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE))
    {
	/*  networt was modified or topologic array isn't initialized  */
	ret_code = kr_topoSort( TOPOLOGIC_TYPE );
	if (ret_code != KRERR_NO_ERROR)
	    return( ret_code );

	NetModified = FALSE;
    }

    /*  initialize weights of the hidden units  */

    /* random initialization of Kohonen layer, the parameters are:
     * use all patterns (0, kr_TotalNoOfPattern() - 1)
     * unused learning rate (0.0)
     * no learning cycles (0)
     * shuffle pattern flag (1)
     */
    ret_code = RbfKohonenInit(0, kr_TotalNoOfPattern() - 1, 0.0, 0, 1);
    if (ret_code != KRERR_NO_ERROR)
	return ret_code;

    /* now initialize the Grossberg layer with 1.0 weights: */
    topo_ptr = topo_ptr_array + (NoOfInputUnits + NoOfHiddenUnits + 2);
    while ((unit_ptr = *++topo_ptr) != NULL)
    {
	/* this is an output unit */

	/* initialize the weights to the Grossberg Layer */
	if UNIT_HAS_SITES( unit_ptr )
	{
	    /* the unit has sites */
	    FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
		link_ptr->weight = (FlintType) 1.0;
	}
	else
	{
	    /* the unit has direct links */
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
		link_ptr->weight = (FlintType) 1.0;
	}
    }

    return KRERR_NO_ERROR;
}

/*****************************************************************************
  FUNCTION : RbfInitSetCenter

  PURPOSE  :
  RETURNS  : 
  NOTES    : The pattern is loaded into the input layer and propagated to the 
             output of the INPUT layer. After that, the weights of the links 
	     leading to the <hidden_unit> are set to the output values of the 
	     corresponding input units. (A center of a RBF is set).
	     The value of <deviation> is used for symmetry breaking: it gives 
	     the percentage of the maximum random change of the input pattern. 
	     deviation 1.0 means 100% which means that an input value of x will
	     lead to a stored weight between 0.0 and 2*x. A value of 0.0 will 
	     not change the weights!
	     The bias of the <hidden_unit> is set to 1.0 (parameter of the RBF)

  UPDATE   : 
******************************************************************************/
extern FlintType OUT_Custom_Python(FlintType act);

void RbfInitSetCenter(int pattern_no, int sub_pat_no, 
		      struct Unit *hidden_unit, float deviation, float bias)
{
    register struct Unit	*unit_ptr;
    register struct Link	*link_ptr;
    register Patterns	current_in_pattern;
    register TopoPtrArray	topo_ptr;

    /* calculate index of the input pattern in Pattern array:	*/
    current_in_pattern = kr_getSubPatData(pattern_no,sub_pat_no,INPUT,NULL);

    /* activate input units with the pattern and calculate the output value */
    topo_ptr = topo_ptr_array;
    while ((unit_ptr = *(++topo_ptr)) != NULL){

	/* go through all input units, set activation and calculate output: */
	unit_ptr -> act = *current_in_pattern++;
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

    /* set the weights of the links: */
    if (deviation == 0.0){
	FOR_ALL_LINKS(hidden_unit, link_ptr){
	    link_ptr -> weight = link_ptr -> to -> Out.output;
	}
    }else{
	deviation /= 6.3137515;
	FOR_ALL_LINKS(hidden_unit, link_ptr){
	    link_ptr->weight = link_ptr->to->Out.output *
		(1.0 + deviation * 
		 tan(((float)drand48() * 2.8274334 - 1.4137167)));
	}
    }

    hidden_unit->bias = bias;
}



/*****************************************************************************
  FUNCTION : RbfInitBPCenter 

  PURPOSE  :
  RETURNS  : 
  NOTES    : The weights of the links leading to the <hidden_unit> are 
             propagated back to the Output value of the corresponding input 
	     units. Only the Out.output of the input units is set. The 
	     activation remains unchanged.

  UPDATE   : 
******************************************************************************/
void RbfInitBPCenter(struct Unit *hidden_unit)
{
    register struct Link	*curr_link;

    FOR_ALL_LINKS(hidden_unit, curr_link){
	curr_link -> to -> Out.output = curr_link -> weight;
    }
}



/*****************************************************************************
  FUNCTION : RbfInitNetwork

  PURPOSE  : Initialization Function for Use of Radial Basis Functions
  RETURNS  : 
  NOTES    : First initialization of centers and direct calculation of all 
             weights as first step during learning.
	     The initialization of a center consists of setting the 
	     coordinates of the center (the weights of the links, leading to 
	     it) and setting the RBF function parameter (bias of the unit) to 
	     i_bias.

  UPDATE   : 
******************************************************************************/
krui_err  RbfInitNetwork(int start_pat, int end_pat, float i_bias, 
			 float i_devi, float i_f_0, float i_f_1, 
			 float i_smooth, int init_type)
{
    register struct Unit    *unit_ptr;
    register struct Unit    *h_unit_ptr;
    register struct Link    *link_ptr;
    register Patterns       current_out_pattern;
    register TopoPtrArray   topo_ptr;
    register TopoPtrArray   topo_hidden_ptr;
    register TopoPtrArray   topo_work;
    register int	    hidden_units;
    register int	    output_units;
    register int	    unit_nr;
    register int	    h_unit_nr;
    register int	    abs_sub_nr;
    register int	    pattern_anz;
    int	                    pattern_no;
    int                     sub_pat_no;
    register float	    deviation;
    register int	    abort;
    register int	    tmp_err;
    register int            start_sp;
    register int            end_sp;

    RbfFloatMatrix	    hidden_act;
    RbfFloatMatrix	    t_hidden_act;
    RbfFloatMatrix	    hidden_produkt;
    RbfFloatMatrix	    inter_act;
    RbfFloatMatrix	    hidden_sum;
    RbfFloatMatrix	    m_p_inverse;
    RbfFloatMatrix	    y_vektor;
    RbfFloatMatrix	    weights_vektor;
#ifdef RBF_MATRIX_TEST
    RbfFloatMatrix	    alt_hidden_sum;
    RbfFloatMatrix	    soll_einheit_sein;
    int			    s,z;
#endif
    int			    malloc_fault;


    abort = FALSE;

    if (init_type == RBF_INIT_FULL){
	fprintf(stderr,"RBF_Weights called, start initialization:\n");
    }else{
	fprintf(stderr,"RBF_Weights_Redo called, start initialization:\n");
    }
	
    fprintf(stderr, "... preparing initialization\n");

    /* count the units of the hidden layer (only one) and the output layer */
    hidden_units = 0;
    output_units = 0;
    FOR_ALL_UNITS(unit_ptr){
	if ((unit_ptr -> flags & UFLAG_IN_USE) == UFLAG_IN_USE) {
	    if (unit_ptr -> flags & UFLAG_TTYP_HIDD)
		hidden_units++;
	    if (unit_ptr -> flags & UFLAG_TTYP_OUT)
		output_units++; 
	}
    }

    /* set <unit_ptr> to the NULL between first hidden and last input unit: */
    topo_ptr = topo_ptr_array;
    while ((unit_ptr = *(++topo_ptr)) != NULL);	/* empty loop!	*/

    /* reference to first hidden unit */
    topo_hidden_ptr = topo_ptr;
    topo_hidden_ptr++;

    /* compute the necessary sub patterns */
    KernelErrorCode = kr_initSubPatternOrder(start_pat,end_pat);
    if(KernelErrorCode != KRERR_NO_ERROR)
    {
	if (KernelErrorCode == KRERR_NP_NO_TRAIN_SCHEME)
	    KernelErrorCode = KRERR_NP_WORKAROUND;
	return (KernelErrorCode);
    }
    start_sp = kr_AbsPosOfFirstSubPat(start_pat);
    end_sp = kr_AbsPosOfFirstSubPat(end_pat) + kr_NoOfSubPatPairs(end_pat) - 1;
    pattern_anz = end_sp - start_sp + 1;

    /* Output bias is treated as additional hidden unit	*/
    hidden_units += 1;

    /* Allocate memory for all matrixes: */
    malloc_fault = 0;
    if (!RbfAllocMatrix(pattern_anz, hidden_units, &hidden_act))
	malloc_fault = 0;
    else if (!RbfAllocMatrix(hidden_units, pattern_anz, &t_hidden_act))
	malloc_fault = 1;
    else if (!RbfAllocMatrix(hidden_units, hidden_units, &hidden_produkt))
	malloc_fault = 2;
    else if (!RbfAllocMatrix(hidden_units, hidden_units, &inter_act))
	malloc_fault = 3;
    else if (!RbfAllocMatrix(hidden_units, hidden_units, &hidden_sum))
	malloc_fault = 4;
    else if (!RbfAllocMatrix(pattern_anz, 1, &y_vektor))
	malloc_fault = 5;
    else if (!RbfAllocMatrix(hidden_units, 1, &weights_vektor))
	malloc_fault = 6;
    else if (!RbfAllocMatrix(hidden_units, pattern_anz, &m_p_inverse))
	malloc_fault = 7;
#ifdef RBF_MATRIX_TEST
    else if (!RbfAllocMatrix(hidden_units, hidden_units, &alt_hidden_sum))
	malloc_fault = 8;
    else if (!RbfAllocMatrix(hidden_units,hidden_units,&soll_einheit_sein))
	malloc_fault = 9;
#endif


    if (malloc_fault != 0){
	if (malloc_fault >= 1)
	    RbfFreeMatrix(&hidden_act);
	if (malloc_fault >= 2)
	    RbfFreeMatrix(&t_hidden_act);
	if (malloc_fault >= 3)
	    RbfFreeMatrix(&hidden_produkt);
	if (malloc_fault >= 4)
	    RbfFreeMatrix(&inter_act);
	if (malloc_fault >= 5)
	    RbfFreeMatrix(&hidden_sum);
	if (malloc_fault >= 6)
	    RbfFreeMatrix(&y_vektor);
	if (malloc_fault >= 7)
	    RbfFreeMatrix(&weights_vektor);
#ifdef RBF_MATRIX_TEST
	if (malloc_fault >= 8)
	    RbfFreeMatrix(&alt_hidden_sum);
	if (malloc_fault >= 9)
	    RbfFreeMatrix(&soll_einheit_sein);
#endif

	return KRERR_INSUFFICIENT_MEM;
    }

    /* change the following line into '#if 1' to allow deviation only in  */
    /* case that there are more hidden units than learn patterns	  */
#if 0
    /* test if more hidden units than learn patterns: */
    if (hidden_units - 1 >= pattern_anz){

	/* more hidden units than learn patterns symmetry breaking necessary!*/
	deviation = i_devi;	/* maximum change = i_devi * activation	*/
    }else{
	/* less hidden units than learn patterns; no symmetry breaking	*/
	deviation = 0.0;       
    }
#else
    /* set deviation to parameter */
    deviation = i_devi;
	
#endif

    fprintf(stderr,"... compute activation of hidden layer on centers\n");

    /* Now set the centers and fill the inter activation matrix: */
    unit_nr = 0;
    while ((unit_ptr = *(++topo_ptr)) != NULL){
	/* set weights of links leading to <unit_ptr>: */
	if (init_type == RBF_INIT_FULL){
	    abs_sub_nr = (((pattern_anz-1)*unit_nr)/(hidden_units-2)) + 
		         start_sp;
	    kr_getSubPatternByNo(&pattern_no, &sub_pat_no, abs_sub_nr);
	    RbfInitSetCenter(pattern_no, sub_pat_no, unit_ptr, 
			     deviation, i_bias);
	}else{
	    RbfInitBPCenter(unit_ptr);
	}

	/* calculate activation of previously defined centers on the current */
	/* pattern and store it into the inter activation matrix:	     */
	topo_work = topo_hidden_ptr;
	for (h_unit_nr = 0; h_unit_nr <= unit_nr; h_unit_nr++){
	    h_unit_ptr = *(topo_work++);

	    /* calculate activation: */
	    h_unit_ptr -> act = h_unit_ptr -> Out.output =
		((h_unit_ptr->act_func == ACT_Custom_Python) ? 
			kr_PythonActFunction(h_unit_ptr->python_act_func,
						h_unit_ptr) :
			(h_unit_ptr->act_func) (h_unit_ptr)) ;

	    /* store it into the symmetric matrix: */
	    RbfMatrixSetValue(&inter_act, h_unit_nr, unit_nr,
			      h_unit_ptr -> act * i_smooth);
	    RbfMatrixSetValue(&inter_act, unit_nr, h_unit_nr,
			      h_unit_ptr -> act * i_smooth);
	}
	unit_nr++;
    }
	
    /* Add entrys for the additional bias: */
    for (h_unit_nr = 0; h_unit_nr < hidden_units; h_unit_nr++){
	RbfMatrixSetValue(&inter_act, h_unit_nr, hidden_units - 1, 
			  i_smooth);
	RbfMatrixSetValue(&inter_act, hidden_units - 1, h_unit_nr,
			  i_smooth);
    }

    fprintf(stderr,"... compute activation of hidden layer on patterns\n");

    /* Fill the hidden units activation matrix */
    for (abs_sub_nr = start_sp; abs_sub_nr <= end_sp; abs_sub_nr++){
	kr_getSubPatternByNo(&pattern_no, &sub_pat_no, abs_sub_nr);
	RbfLearnForward(pattern_no, sub_pat_no);
	topo_ptr = topo_hidden_ptr;
	for (unit_nr = 0; unit_nr < hidden_units - 1; unit_nr++){
	    RbfMatrixSetValue(&hidden_act, abs_sub_nr - start_sp, unit_nr,
			      (*(topo_ptr++)) -> Out.output);
	}
    }
	
    /* Add entrys for the additional bias: */
    for (h_unit_nr = 0; h_unit_nr < pattern_anz; h_unit_nr++){
	RbfMatrixSetValue(&hidden_act, h_unit_nr, hidden_units - 1, 1.0);
    }
	
    fprintf(stderr,"... calculate the moore-penrose inverse matrix\n");

    /* Now calculate the Moore-Penrose Pseudoinverse: */
    fprintf(stderr,"...... transposing\n");
    RbfTranspMatrix(&t_hidden_act, &hidden_act);
    fprintf(stderr,"...... multiplying\n");
    RbfMulTranspMatrix(&hidden_produkt, &t_hidden_act);
    fprintf(stderr,"...... adding\n");
    RbfAddMatrix(&hidden_sum, &hidden_produkt, &inter_act);

#ifdef RBF_MATRIX_TEST
    RbfSetMatrix(&alt_hidden_sum, &hidden_sum);
#endif

    fprintf(stderr,"...... inverting\n");
    if ((tmp_err = RbfInvMatrix(&hidden_sum)) != 1){
	fprintf(stderr,"... impossible to invert matrix!\n");
	abort = TRUE;
    }

#ifdef RBF_MATRIX_TEST
    RbfMulMatrix(&soll_einheit_sein, &alt_hidden_sum, &hidden_sum);
    printf("Einheitsmatrix:\n");
    RbfPrintMatrix(&soll_einheit_sein, stdout);
#endif

    if (!abort){
	fprintf(stderr,"...... multiplying\n");
	RbfMulMatrix(&m_p_inverse, &hidden_sum, &t_hidden_act);

	fprintf(stderr,
		"... calculate weights between hidden and output layer\n");
	
	/* set topo_ptr to the NULL between hidden and output layer: */
	topo_ptr = topo_hidden_ptr;
	while(*(++topo_ptr) != NULL);

	/* direct calc. of all weights of links leading to the output layer: */
	unit_nr = 0;  /* counts the output units */
	while((unit_ptr = *(++topo_ptr)) != NULL){

	    /* fill the y_vektor with the desired outputs for all patterns: */
	    for (abs_sub_nr = start_sp; abs_sub_nr <= end_sp; abs_sub_nr++){
		kr_getSubPatternByNo(&pattern_no, &sub_pat_no, abs_sub_nr);
		current_out_pattern = kr_getSubPatData(pattern_no,
						       sub_pat_no,
						       OUTPUT, NULL);
		RbfMatrixSetValue(&y_vektor, abs_sub_nr - start_sp, 0,
				  i_f_0 + (i_f_1 - i_f_0)*
				  *(current_out_pattern + unit_nr));
	    }

	    /* calculate the weights, leading to the current output unit*/
	    RbfMulMatrix(&weights_vektor, &m_p_inverse, &y_vektor);

	    /* temporarely store the weights in the value_c field of	*/
	    /* the corresponding hidden units:				*/
	    topo_work = topo_hidden_ptr;
	    h_unit_nr = 0;
	    do {
		(*(topo_work++)) -> value_c = 
		    RbfMatrixGetValue(&weights_vektor, h_unit_nr++, 0); 
	    } while (*topo_work != NULL);

	    /* set the bias of the output unit:	*/
	    unit_ptr->bias = 
		RbfMatrixGetValue(&weights_vektor, hidden_units - 1, 0);

	    /* set the weights of the links: */
	    FOR_ALL_LINKS(unit_ptr, link_ptr) {
		link_ptr -> weight = link_ptr -> to -> value_c;
	    }
	    unit_nr++;
	}

	fprintf(stderr,"Initialization done !\n");
    }else{
	if (tmp_err == 0)
	    fprintf(stderr,"singular matrix !\n");
	fprintf(stderr,"Initialization aborted !\n");
    }
    RbfFreeMatrix(&hidden_act);
    RbfFreeMatrix(&t_hidden_act);
    RbfFreeMatrix(&hidden_produkt);
    RbfFreeMatrix(&inter_act);
    RbfFreeMatrix(&hidden_sum);
    RbfFreeMatrix(&y_vektor);
    RbfFreeMatrix(&weights_vektor);
    RbfFreeMatrix(&m_p_inverse);

    if (abort){
	return tmp_err == 0 ? KRERR_NO_ERROR : tmp_err;
    }else{
	return KRERR_NO_ERROR;
    }
}



#ifdef RBF_INCLUDE_KOHONEN_CONVEX
/*****************************************************************************
  FUNCTION : RbfKohonenConvexInit

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void RbfKohonenConvexInit(int start_pattern,int end_pattern,float alpha_start,
			  float alpha_increment,float learn_rate,int count)
{
	register float		scalar_prod;	/* act. scalar product	*/
	register float		maximum;	/* max scalar product	*/
	register struct Link	*link_ptr;	/* current Link		*/
	register struct Unit	*unit_ptr;	/* current Unit		*/
	register TopoPtrArray	topo_ptr;
	register Patterns	current_in_pattern;	/* in pattern	*/
	register int		pattern_no;
	register int            sub_pat_no;
	register TopoPtrArray	topo_hidden_ptr;/* first hidden Unit	*/
	register float		alpha;		/* convex combination	*/
	register struct Unit	*winner;	/* Unit who's links	*/
						/* change		*/
	register float		norm_alpha;	/* convex combination	*/
	float			norm_init;	/* initialization value	*/
	
	/* search for the first hidden unit				*/
	topo_ptr = topo_ptr_array;
	while (*(++topo_ptr) != NULL);		/* empty loop!		*/

	/* reference to first hidden unit				*/
	topo_hidden_ptr = topo_ptr;
	topo_hidden_ptr++;

	/* initialize all weights leading to hidden units		*/
	norm_init = 1.0 / (float) sqrt((float) NoOfInputUnits);
	while ((unit_ptr = *(++topo_ptr)) != NULL)
	{
	    FOR_ALL_LINKS(unit_ptr, link_ptr)
	    {
		link_ptr -> weight = norm_init;
	    }
	}

	/* do the kohonen training <count> times with increasing alpha	*/
	for (alpha = alpha_start; count > 0; alpha += alpha_increment, count--)
	{
	    /* precalculate the constant value for the convex		*/
	    /* combination method for the current alpha			*/
	    norm_alpha = (1.0 - alpha) * norm_init;

	    /* compute the necessary sub patterns */
	    KernelErrorCode = kr_initSubPatternOrder(start_pattern,
						     end_pattern);
	    if(KernelErrorCode != KRERR_NO_ERROR)
	    {
		if (KernelErrorCode == KRERR_NP_NO_TRAIN_SCHEME)
		    KernelErrorCode = KRERR_NP_WORKAROUND;
		return (KernelErrorCode);
	    }

	    /* present all input patterns and train the hidden layer */
	    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no))
	    {
		/* calculate index of first component of input pattern */
		current_in_pattern = kr_getSubPatData(pattern_no,
						      sub_pat_no,
						      INPUT, NULL);

		/* activate input units with the pattern and calculate	*/
		/* their output value:					*/
		topo_ptr = topo_ptr_array;
		while ((unit_ptr = *(++topo_ptr)) != NULL)
		{
		    /* go through all input units, set activation and	*/
		    /* calculate output using the convex combination	*/
		    /* method						*/
		    unit_ptr -> act = *current_in_pattern++;

			if (unit_ptr->out_func == OUT_IDENTITY)
			/*  identity output function: don't call the output function */
				unit_ptr->Out.output = alpha * 
					(unit_ptr -> act) + norm_alpha;
			    else if(unit_ptr->out_func == OUT_Custom_Python)
					unit_ptr->Out.output =
					alpha *
					kr_PythonOutFunction(unit_ptr->python_out_func,
						unit_ptr->act) + norm_alpha;
			    else
				/*  no identity output function: calculate unit's output also */
				unit_ptr->Out.output = alpha * (*unit_ptr->out_func) (unit_ptr->act) + norm_alpha;

		    
		}

		/* determine the hidden unit with maximum scalar product*/
		/* between its weights and the output of the input layer*/
		winner = (struct Unit *) NULL;
		maximum = (float) -MY_HUGE_VAL;	/* -oo, see init_f.ph	*/
		topo_ptr = topo_hidden_ptr;
		while ((unit_ptr = *(topo_ptr++)) != NULL)
		{
		    /* calculate scalar product of current hidden unit	*/
		    scalar_prod = (float) 0.0;
		    FOR_ALL_LINKS(unit_ptr, link_ptr)
		    {
			scalar_prod += link_ptr -> weight * 
				link_ptr -> to -> Out.output;
		    }

		    /* change winner if s.p. is > than the current best	*/
		    if (scalar_prod > maximum)
		    {
			maximum = scalar_prod;
			winner = unit_ptr;
		    }
		}

		/* adjust weights of the hidden winner Unit		*/
		if (winner != NULL)
		{
		    FOR_ALL_LINKS(winner, link_ptr)
		    {
			link_ptr -> weight += learn_rate * 
			    (link_ptr->to->Out.output - link_ptr->weight);
		    }
		    printf("(%d,%d) ", winner -> unit_pos.x, 
			winner -> unit_pos.y); 
		}
		else
		{
		    fprintf(stderr,"Internal error in RbfKohonenConvexInit\n");
		}
	    }
	}
}

#endif

krui_err RbfKohonenInit(int start_pattern, int end_pattern, float learn_rate, int count, int shuffle)
{
	register float		scalar_prod;	/* act. scalar product	*/
	register float		maximum;	/* max scalar product	*/
	register struct Link	*link_ptr;	/* current Link		*/
	register struct Unit	*unit_ptr;	/* current Unit		*/
	register TopoPtrArray	topo_ptr;
	register Patterns	current_in_pattern;	/* in pattern	*/
	int	         	pattern_no;
	int                     sub_pat_no;
	register int            start_sp;
	register int            end_sp;
	register int            act_sub_nr;
	register TopoPtrArray	topo_hidden_ptr;/* first hidden Unit	*/
	register TopoPtrArray	help_topo_ptr;
	register struct Unit	*winner;	/* Unit who's links	*/
						/* change		*/
	float			norm_init;	/* initialization value	*/
	register struct Unit	*hidden_unit;	/* current hidden unit	*/
	register int		hidden_units;	/* number of hidden u.	*/
	register int		act_hidden_num;	/* number of current hu.*/
	int			reshuffle;	/* restore shuffled p.	*/

#ifdef RBF_DEBUG
	fprintf(stderr, "RBF_Weights_Kohonen called, start initialization:\n");
#endif

	/* search for the first hidden unit				*/
	topo_ptr = topo_ptr_array;
	while (*(++topo_ptr) != NULL);		/* empty loop!		*/

	/* count hidden units and reference to first hidden unit	*/
	topo_hidden_ptr = topo_ptr;
	hidden_units = 0;
	while (*(++topo_hidden_ptr) != NULL)
		hidden_units++;
	topo_hidden_ptr = topo_ptr;
	topo_hidden_ptr++;

	if (shuffle)
	{
	    reshuffle = FALSE;
	    if (!kr_np_pattern(GET_SHUFFLE_FLAG, 0, 0))
	    {
		kr_np_pattern(PATTERN_SHUFFLE_ON, 0, 0);
		reshuffle = TRUE;
	    }
	}

	/* compute the necessary sub patterns */
	KernelErrorCode = kr_initSubPatternOrder(start_pattern, end_pattern);
	if(KernelErrorCode != KRERR_NO_ERROR)
	{
	    if (KernelErrorCode == KRERR_NP_NO_TRAIN_SCHEME)
		KernelErrorCode = KRERR_NP_WORKAROUND;
	    return (KernelErrorCode);
	}

	start_sp = kr_AbsPosOfFirstSubPat(start_pattern);
	end_sp = kr_AbsPosOfFirstSubPat(end_pattern) + 
	    kr_NoOfSubPatPairs(end_pattern) - 1;
	
#ifdef RBF_DEBUG
	fprintf(stderr, "... init weights between input and hidden layer\n");
#endif

	/* initialize all weights leading to hidden units		*/
	norm_init = 1.0 / (float) sqrt((float) NoOfInputUnits);
	act_hidden_num = 0;
	while ((hidden_unit = *(++topo_ptr)) != NULL)
	{
	    if (shuffle)
	    {
		/* shuffle						*/
		if (!kr_getSubPatternByOrder(&pattern_no, &sub_pat_no))
		    return KRERR_PATTERN_NO;
	    }
	    else
	    {
		/* do not shuffle					*/
		act_sub_nr = start_sp + 
		    ((end_sp-start_sp) * act_hidden_num) / (hidden_units - 1);
		if (!kr_getSubPatternByNo(&pattern_no, &sub_pat_no, 
					  act_sub_nr))
		    return KRERR_PATTERN_NO;
	    }

	    /* calculate index of input pattern */
	    current_in_pattern = kr_getSubPatData(pattern_no,
						  sub_pat_no, INPUT,
						  NULL);

	    /* activate input units with the pattern and calculate	*/
	    /* their output value:					*/
	    help_topo_ptr = topo_ptr_array;
	    while ((unit_ptr = *(++help_topo_ptr)) != NULL)
	    {
		/* go through all input units, set activation and	*/
		/* calculate output using the convex combination	*/
		/* method						*/
		unit_ptr -> act = *current_in_pattern++;

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

	    FOR_ALL_LINKS(hidden_unit, link_ptr)
	    {
		link_ptr -> weight = link_ptr -> to -> Out.output;
	    }

	    act_hidden_num++;
	}

	if (shuffle && reshuffle)
	{
	    kr_np_pattern(PATTERN_SHUFFLE_OFF, 0, 0);
	}

	/* do the kohonen training <count> times			*/
#ifdef RBF_DEBUG
	if (count > 0)
	{
	   fprintf(stderr, "... begin kohonen training\n");
	}
#endif
	for (; count > 0; count--)
	{
	    /* compute the necessary sub patterns */
	    KernelErrorCode = kr_initSubPatternOrder(start_pattern, 
						     end_pattern);
	    if(KernelErrorCode != KRERR_NO_ERROR)
	    {
		if (KernelErrorCode == KRERR_NP_NO_TRAIN_SCHEME)
		    KernelErrorCode = KRERR_NP_WORKAROUND;
		return (KernelErrorCode);
	    }

	    /* present all input patterns and train the hidden layer	*/
	    while(kr_getSubPatternByOrder(&pattern_no, &sub_pat_no))
	    {
		/* calculate index of input pattern */
		current_in_pattern = kr_getSubPatData(pattern_no,
						      sub_pat_no,
						      INPUT, NULL);

		/* activate input units with the pattern and calculate	*/
		/* their output value:					*/
		topo_ptr = topo_ptr_array;
		while ((unit_ptr = *(++topo_ptr)) != NULL)
		{
		    /* go through all input units, set activation and	*/
		    /* calculate output using the convex combination	*/
		    /* method						*/
		    unit_ptr -> act = *current_in_pattern++;

		    if (unit_ptr->out_func == OUT_IDENTITY)
			/*  identity output function: don't call the output function */
			unit_ptr->Out.output = unit_ptr->act;
		    else if(unit_ptr->out_func == OUT_Custom_Python)
			unit_ptr->Out.output = 
				kr_PythonOutFunction(unit_ptr->python_out_func,
					unit_ptr->act);
		    else
			unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);

		
		    
		}

		/* determine the hidden unit with maximum scalar product*/
		/* between its weights and the output of the input layer*/
		winner = (struct Unit *) NULL;
		maximum = (float) -MY_HUGE_VAL;	/* -oo, see init_f.ph	*/
		topo_ptr = topo_hidden_ptr;
		while ((unit_ptr = *(topo_ptr++)) != NULL)
		{
		    /* calculate scalar product of current hidden unit	*/
		    scalar_prod = (float) 0.0;
		    FOR_ALL_LINKS(unit_ptr, link_ptr)
		    {
			scalar_prod += link_ptr -> weight * 
				link_ptr -> to -> Out.output;
		    }

		    /* change winner if s.p. is > than the current best	*/
		    if (scalar_prod > maximum)
		    {
			maximum = scalar_prod;
			winner = unit_ptr;
		    }
		}

		/* adjust weights of the hidden winner Unit		*/
		if (winner != NULL)
		{
		    FOR_ALL_LINKS(winner, link_ptr)
		    {
			link_ptr -> weight += learn_rate * 
			    (link_ptr->to->Out.output - link_ptr->weight);
		    }
		}
		else
		{
		    fprintf(stderr,"Internal error in RbfKohonenConvexInit\n");
		}
	    }
	}
#ifdef RBF_DEBUG
	fprintf(stderr, "Initialization done\n");
#endif
	return KRERR_NO_ERROR;
}

krui_err RbfStartInit(float *parameterArray, int NoOfParams, int init_type)
{
	krui_err	ret_code;	/* error return code		*/
	float		bias;		/* bias of hidden units		*/
	float		deviation;	/* deviation of centers		*/
	float		f_0_lin;	/* learning value for pattern==0*/
	float		f_1_lin;	/* learning value for pattern==1*/
	float		smoothness;	/* see documentation 		*/

	float		learn_rate;	/* kohonen training rate	*/
	int		count;		/* cycles for kohonen training  */

	/* for future use, now uncommented: 				*/
	/* check the number of input parameters				*/

	/* test if patterns available and valid:			*/
	if (kr_TotalNoOfSubPatPairs() == 0)  
	    return( KRERR_NO_PATTERNS );	/* no patterns defined  */

	/* test net topology:						*/
	if (NetModified || (TopoSortID != TOPOLOGICAL_FF))
	{
	    ret_code = RbfTopoCheck();	/* in learn_f.c			*/

	    if ((ret_code != KRERR_NO_ERROR) && (ret_code != KRERR_DEAD_UNITS))
		return( ret_code );

	    NetModified = FALSE;
	}

	switch(init_type)
	{
	    case RBF_INIT_FULL:
	    case RBF_INIT_REINIT:
		/* read input parameters:				*/
		bias = INIT_PARAM4(parameterArray);
		deviation = INIT_PARAM5(parameterArray);
		f_0_lin = INIT_PARAM1(parameterArray);
		f_1_lin = INIT_PARAM2(parameterArray);
		smoothness = INIT_PARAM3(parameterArray);

		/* call real initialization function:			*/
		ret_code = RbfInitNetwork(0, kr_TotalNoOfPattern() - 1, bias, 
			deviation, f_0_lin, f_1_lin, smoothness, init_type);
		break;

	    case RBF_INIT_KOHONEN:
		/* read input parameters:				*/
		count = (int) (INIT_PARAM1(parameterArray));
		learn_rate = INIT_PARAM2(parameterArray);
		ret_code = RbfKohonenInit(0, kr_TotalNoOfPattern() - 1,
					  learn_rate, count, 
					  INIT_PARAM3(parameterArray) != 0.0);
		break;
	}

	return ret_code;
}

/*
 * Use of initialization parameters:
 * RBF_Weights: (5)
 *   INIT_PARAM1: interpolation value for teaching pattern == 0.0
 *                (see documentation)
 *   INIT_PARAM2: interpolation value for teaching pattern == 1.0
 *                (see documentation)
 *   INIT_PARAM3: smoothness parameter
 *   INIT_PARAM4: initialization bias of hidden units
 *   INIT_PARAM5: deviation parameter for symmetry breaking
 * RBF_Weights_redo: (3)
 *   INIT_PARAM1: interpolation value for teaching pattern == 0.0
 *                (see documentation)
 *   INIT_PARAM2: interpolation value for teaching pattern == 1.0
 *                (see documentation)
 *   INIT_PARAM3: smoothness parameter
 * RBF_Weights_Kohonen: (3)
 *   INIT_PARAM1: number of cycles for the kohonen training
 *   INIT_PARAM2: learn_rate for kohonen training
 *   INIT_PARAM3: shuffle flag:
 *                0.0 = the patterns are normally distributed over the links
 *                      to the hidden units.
 *             != 0.0 = the patterns to use are randomly taken from all
 *                      available patterns.
 */

krui_err INIT_RBF_Weights(float *parameterArray, int NoOfParams)
{
	return RbfStartInit(parameterArray, NoOfParams, RBF_INIT_FULL);
}

krui_err INIT_RBF_Weights_redo(float *parameterArray, int NoOfParams)
{
	return RbfStartInit(parameterArray, NoOfParams, RBF_INIT_REINIT);
}

krui_err INIT_RBF_Weights_kohonen(float *parameterArray, int NoOfParams)
{
	return RbfStartInit(parameterArray, NoOfParams, RBF_INIT_KOHONEN);
}




/* Initializes an ART1 network
*/
krui_err INIT_Weights_ART1(float *parameterArray, int NoOfParams)
{
  register struct Unit    *unit_ptr;
  register struct Link    *link_ptr;

  TopoPtrArray            topo_cmp_ptr,
                          topo_rec_ptr,
                          topo_ptr;

  int                     ret_code       = KRERR_NO_ERROR;

  FlintType               beta;
  FlintType               gamma;
  double                  eta;

  int                     j;


  if ( (unit_array == NULL) || (NoOfUnits == 0) ) {
     ret_code = KRERR_NO_UNITS;
     return( ret_code );         /*  there is nothing to do  */
  } /*if*/

  if (NoOfParams < 2) {
     ret_code = KRERR_PARAMETERS;
     return( ret_code );  /*  Not the same no. of input parameters */
  } /*if*/

  beta = parameterArray [0];
  gamma = parameterArray [1];


  if ((beta <= 0.0) || (gamma <= 0.0)) {
     /* the parameters beta and gamma have to be greater than 0.0
     */
     ret_code = KRERR_PARAMETERS;
     return (ret_code);
  } /*if*/

  ret_code = kr_topoSort (ART1_TOPO_TYPE);
  if (ret_code != KRERR_NO_ERROR) {
     NetModified = TRUE;
     return (ret_code);
  } /*if*/
  NetModified = FALSE;


  /* Now we will write the value of beta in each of the units of the
     network for to be able to recall in the learning algorithm.
     The value will be written to the bias field of the unit structure
     which is not needed for ART 1 learning in any other way and which
     has the property that it is written to the netfile when the network
     is saved.
  */
  FOR_ALL_UNITS (unit_ptr) {
     unit_ptr->bias = beta;
  } /*FOR_ALL_UNITS*/

  topo_cmp_ptr = topo_ptr_array + NoOfInputUnits + 2;
  topo_rec_ptr = topo_cmp_ptr   + NoOfInputUnits + 1;


  /* To initialize the bottom up weight values we have to choose one
     value for each recognition unit. That is, each weight value for
     a link to a recognition unit j is set to

                     b(i,j) = alpha(j),

     where the alpha(j) are to be choosen as follows

          alpha(1) > alpha(2) > .... > alpha (M) where M is the no. of
                                                 rec. unit.

     and    0 < alpha(j) < 1/(beta + |I|)  for all 1 <= j <= M.

     For this reason we partition gamma
     into M parts (eta=(gamma/M) and init as follows

      b(i,j) = alpha(1) = 1/(beta + (1.0 + j*eta) * |I|)
      for all 1 <= j <= M
  */

  eta = gamma / Art1_NoOfRecUnits;

  /* init weights from comparison units to recognition units */
  topo_ptr = topo_rec_ptr;

  j = 1;
  while ((unit_ptr = *topo_ptr++) != NULL) {

     if (UNIT_HAS_SITES (unit_ptr)) {
        ret_code = KRERR_TOPOLOGY;
        return (ret_code);
     } /*if*/

     FOR_ALL_LINKS (unit_ptr, link_ptr) {

        if (link_ptr->to->lln == ART1_CMP_LAY) {
           link_ptr->weight = ART1_LINK_CMP_REC(beta, (1.0+j*eta));
        } /*if*/

      } /*FOR_ALL_LINKS*/

      j++;

  } /*while*/


  /* init weights from delay units to comparison units */
  topo_ptr = topo_cmp_ptr;

  while ((unit_ptr = *topo_ptr++) != NULL) {

     if (UNIT_HAS_SITES (unit_ptr)) {
        ret_code = KRERR_TOPOLOGY;
        return (ret_code);
     } /*if*/

     FOR_ALL_LINKS (unit_ptr, link_ptr) {

        if (link_ptr->to->lln == ART1_DEL_LAY) {
           link_ptr->weight = ART1_LINK_DEL_CMP;
        } /*if*/

      } /*FOR_ALL_LINKS*/

  } /*while*/



  return (ret_code);

} /* INIT_Weights_ART1 */





/* Initializes an ART2 network
*/
krui_err INIT_Weights_ART2(float *parameterArray, int NoOfParams)
{
  register struct Unit    *unit_ptr;
  register struct Link    *link_ptr;

  TopoPtrArray            topo_p_ptr,
                          topo_rec_ptr,
                          topo_ptr;

  int                     ret_code       = KRERR_NO_ERROR;

  FlintType               param_d;
  FlintType               gamma;


  if ( (unit_array == NULL) || (NoOfUnits == 0) ) {
     ret_code = KRERR_NO_UNITS;
     return( ret_code );         /*  there is nothing to do  */
  } /*if*/

  if (NoOfParams < 1) {
     ret_code = KRERR_PARAMETERS;
     return( ret_code );  /*  Not the same no. of input parameters */
  } /*if*/

  param_d = parameterArray [0];
  gamma   = parameterArray [1];


  if ((param_d <= 0.0) || (param_d >= 1.0) ||
      (gamma < 1.0))
  {
     /* the parameters d has to fit the constraint 0 < d < 1,
        gamma >= 1.0
     */
     ret_code = KRERR_PARAMETERS;
     return (ret_code);
  } /*if*/

  ret_code = kr_topoSort (ART2_TOPO_TYPE);
  if (ret_code != KRERR_NO_ERROR) {
     NetModified = TRUE;
     return (ret_code);
  } /*if*/
  NetModified = FALSE;


  /* Now we will write the value of param_d in each of the units of the
     network for to be able to recall in the learning algorithm.
     The value will be written to the bias field of the unit structure
     which is not needed for ART 2 learning in any other way and which
     has the property that it is written to the netfile when the network
     is saved.
  */
  FOR_ALL_UNITS (unit_ptr) {
     unit_ptr->bias = param_d;
  } /*FOR_ALL_UNITS*/

  topo_p_ptr   = topo_ptr_array + 5*NoOfInputUnits + 6;
  topo_rec_ptr = topo_ptr_array + 8*NoOfInputUnits + 9;

  /*
     The ART 2 bottom up links are initialized with the following values:

         zij(0) = 1 / (gamma * (1-d) * SQRT(N))

     where gamma is parameter gamma, d is parameter param_d, N
     is the number of F1-nodes.

     Choosing gamma as small as possible (gamma=1.0) biases the
     ART2 system as much as possible toward choosing uncommited
     nodes.
  */



  /* init weights from p units to recognition units (Bottom-Up-Weights) */
  topo_ptr = topo_rec_ptr;

  while ((unit_ptr = *topo_ptr++) != NULL) {

     if (UNIT_HAS_SITES (unit_ptr)) {
        ret_code = KRERR_TOPOLOGY;
        return (ret_code);
     } /*if*/

     FOR_ALL_LINKS (unit_ptr, link_ptr) {

        if (link_ptr->to->lln == ART2_P_LAY) {
           link_ptr->weight = ART2_LINK_P_REC(param_d, gamma);
        } /*if*/

      } /*FOR_ALL_LINKS*/

  } /*while*/


  /* init weights from delay units to p units (Top-Down-Weights) */
  topo_ptr = topo_p_ptr;

  while ((unit_ptr = *topo_ptr++) != NULL) {

     if (UNIT_HAS_SITES (unit_ptr)) {
        ret_code = KRERR_TOPOLOGY;
        return (ret_code);
     } /*if*/

     FOR_ALL_LINKS (unit_ptr, link_ptr) {

        if (link_ptr->to->lln == ART2_REC_LAY) {
           link_ptr->weight = ART2_LINK_REC_P;
        } /*if*/

      } /*FOR_ALL_LINKS*/

  } /*while*/



  return (ret_code);

} /* INIT_Weights_ART2 */





/* Initializes an ARTMAP network
*/
krui_err INIT_Weights_ARTMAP(float *parameterArray, int NoOfParams)
{
  register struct Unit    *unit_ptr;
  register struct Link    *link_ptr;

  TopoPtrArray            topo_cmpa_ptr,
                          topo_reca_ptr,
                          topo_cmpb_ptr,
                          topo_recb_ptr,
                          topo_map_ptr,
                          topo_ptr;

  int                     ret_code       = KRERR_NO_ERROR;

  FlintType               beta_a, beta_b;
  FlintType               gamma_a, gamma_b;
  double                  eta_a, eta_b;

  int                     j;


  if ( (unit_array == NULL) || (NoOfUnits == 0) ) {
     ret_code = KRERR_NO_UNITS;
     return( ret_code );         /*  there is nothing to do  */
  } /*if*/

  if (NoOfParams < 4) {
     ret_code = KRERR_PARAMETERS;
     return( ret_code );  /*  Not the same no. of input parameters */
  } /*if*/

  beta_a  = parameterArray [0];
  gamma_a = parameterArray [1];
  beta_b  = parameterArray [2];
  gamma_b = parameterArray [3];


  if ((beta_a <= 0.0) || (gamma_a <= 0.0) || 
      (beta_b <= 0.0) || (gamma_b <= 0.0)) {
     /* the parameters beta and gamma have to be greater than 0.0
     */
     ret_code = KRERR_PARAMETERS;
     return (ret_code);
  } /*if*/

  ret_code = kr_topoSort (ARTMAP_TOPO_TYPE);
  if (ret_code != KRERR_NO_ERROR) {
     NetModified = TRUE;
     return (ret_code);
  } /*if*/
  NetModified = FALSE;


  /* Now we will write the value of beta in each of the units of the
     network for to be able to recall in the learning algorithm.
     The value will be written to the bias field of the unit structure
     which is not needed for ART 1 learning in any other way and which
     has the property that it is written to the netfile when the network
     is saved.
  */
  FOR_ALL_UNITS (unit_ptr) {
     switch (unit_ptr->lln) {
     case ARTMAP_INPa_LAY:
     case ARTMAP_CMPa_LAY:
     case ARTMAP_RECa_LAY:
     case ARTMAP_DELa_LAY:
     case ARTMAP_RSTa_LAY:
     case ARTMAP_SPECa_LAY:
        unit_ptr->bias = beta_a;
        break;
     case ARTMAP_INPb_LAY:
     case ARTMAP_CMPb_LAY:
     case ARTMAP_RECb_LAY:
     case ARTMAP_DELb_LAY:
     case ARTMAP_RSTb_LAY:
     case ARTMAP_SPECb_LAY:
        unit_ptr->bias = beta_b;
        break;
     default:
        break;
     } /*switch*/
  } /*FOR_ALL_UNITS*/

  topo_cmpa_ptr = topo_ptr_array + ArtMap_NoOfInpUnits_a + 2;

  topo_reca_ptr = topo_cmpa_ptr  + ArtMap_NoOfInpUnits_a + 1;

  topo_cmpb_ptr = topo_reca_ptr  + 3*ArtMap_NoOfRecUnits_a
                                 + ARTMAP_NO_OF_SPECa_UNITS
                                 + ArtMap_NoOfInpUnits_b
                                 + 8;

  topo_recb_ptr = topo_cmpb_ptr  + ArtMap_NoOfInpUnits_b + 1;

  topo_map_ptr  = topo_recb_ptr  + 3*ArtMap_NoOfRecUnits_b
                                 + ARTMAP_NO_OF_SPECb_UNITS
                                 + 7;


  /* To initialize the bottom up weight values we have to choose one
     value for each recognition unit. That is, each weight value for
     a link to a recognition unit j is set to

                     b(i,j) = alpha(j),

     where the alpha(j) are to be choosen as follows

          alpha(1) > alpha(2) > .... > alpha (M) where M is the no. of
                                                 rec. unit.

     and    0 < alpha(j) < 1/(beta + |I|)  for all 1 <= j <= M.

     For this reason we partition gamma
     into M parts (eta=(gamma/M) and init as follows

      b(i,j) = alpha(1) = 1/(beta + (1.0 + j*eta) * |I|)
      for all 1 <= j <= M
  */

  eta_a = gamma_a / ArtMap_NoOfRecUnits_a;
  eta_b = gamma_b / ArtMap_NoOfRecUnits_b;

  /* init weights from comparison units to recognition units for ARTa
  */
  topo_ptr = topo_reca_ptr;

  j = 1;
  while ((unit_ptr = *topo_ptr++) != NULL) {

     if (UNIT_HAS_SITES (unit_ptr)) {
        ret_code = KRERR_TOPOLOGY;
        return (ret_code);
     } /*if*/

     FOR_ALL_LINKS (unit_ptr, link_ptr) {

        if (link_ptr->to->lln == ARTMAP_CMPa_LAY) {
           link_ptr->weight = ARTMAP_LINK_CMPa_RECa(beta_a, (1.0+j*eta_a));
        } /*if*/

      } /*FOR_ALL_LINKS*/

      j++;

  } /*while*/


  /* init weights from delay units to comparison units for ARTa
  */
  topo_ptr = topo_cmpa_ptr;

  while ((unit_ptr = *topo_ptr++) != NULL) {

     if (UNIT_HAS_SITES (unit_ptr)) {
        ret_code = KRERR_TOPOLOGY;
        return (ret_code);
     } /*if*/

     FOR_ALL_LINKS (unit_ptr, link_ptr) {

        if (link_ptr->to->lln == ARTMAP_DELa_LAY) {
           link_ptr->weight = ARTMAP_LINK_DELa_CMPa;
        } /*if*/

      } /*FOR_ALL_LINKS*/

  } /*while*/


  /* init weights from comparison units to recognition units for ARTb
  */
  topo_ptr = topo_recb_ptr;

  j = 1;

  while ((unit_ptr = *topo_ptr++) != NULL) {

     if (UNIT_HAS_SITES (unit_ptr)) {
        ret_code = KRERR_TOPOLOGY;
        return (ret_code);
     } /*if*/

     FOR_ALL_LINKS (unit_ptr, link_ptr) {

        if (link_ptr->to->lln == ARTMAP_CMPb_LAY) {
           link_ptr->weight = ARTMAP_LINK_CMPb_RECb(beta_b, (1.0+j*eta_b));
        } /*if*/

     } /*FOR_ALL_LINKS*/

     j++;

  } /*while*/


  /* init weights from delay units to comparison units */
  topo_ptr = topo_cmpb_ptr;

  while ((unit_ptr = *topo_ptr++) != NULL) {

     if (UNIT_HAS_SITES (unit_ptr)) {
        ret_code = KRERR_TOPOLOGY;
        return (ret_code);
     } /*if*/

     FOR_ALL_LINKS (unit_ptr, link_ptr) {

        if (link_ptr->to->lln == ARTMAP_DELb_LAY) {
           link_ptr->weight = ARTMAP_LINK_DELb_CMPb;
        } /*if*/

      } /*FOR_ALL_LINKS*/

  } /*while*/


  /* init weights from delay units of ARTa to Map field units
  */
  topo_ptr = topo_map_ptr;

  while ((unit_ptr = *topo_ptr++) != NULL) {

     if (UNIT_HAS_SITES (unit_ptr)) {
        ret_code = KRERR_TOPOLOGY;
        return (ret_code);
     } /*if*/

     FOR_ALL_LINKS (unit_ptr, link_ptr) {

        if (link_ptr->to->lln == ARTMAP_DELa_LAY) {
           link_ptr->weight = ARTMAP_LINK_DELa_MAP;
        } /*if*/

     } /*FOR_ALL_LINKS*/

  } /*while*/



  return (ret_code);

} /* INIT_Weights_ARTMAP */


/*****************************************************************************
  FUNCTION : INIT_CC_Weights

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err INIT_CC_Weights(float *parameterArray, int NoOfParams)
{
 cc_freeStorage(0,krui_getNoOfPatterns()-1,1);
 return(INIT_randomizeWeights(parameterArray,NoOfParams));
}

/*****************************************************************************
  FUNCTION : INIT_TACOMA_Weights

  PURPOSE  :
  RETURNS  : 
  NOTES    :

  UPDATE   : 11.12.95 <Juergen Gatter>
******************************************************************************/
krui_err  INIT_TACOMA_Weights(float *parameterArray, int NoOfParams)
{
    return(INIT_randomizeWeights(parameterArray,NoOfParams));
}


/*****************************************************************************
  FUNCTION : INIT_SOM_Rand_Pat

  PURPOSE  : Initializes all weight vectors of the Kohonen layer with
  random input patterns from the training set. 

  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err INIT_SOM_Rand_Pat(float *parameterArray, int NoOfParams)
{
    krui_err ret_code;

    if ( (unit_array == NULL) || (NoOfUnits == 0) )
	return( KRERR_NO_UNITS ); /*  there is nothing to do  */

    /* test if patterns available and valid:			*/
    if (kr_TotalNoOfSubPatPairs() == 0)  
	return( KRERR_NO_PATTERNS );	/* no patterns defined  */

    if (NetModified || (TopoSortID != TOPOLOGIC_TYPE))
    {
	/*  networt was modified or topologic array isn't initialized  */
	ret_code = kr_topoSort( TOPOLOGIC_TYPE );
	if (ret_code == KRERR_NO_OUTPUT_UNITS) 
	    ret_code = KRERR_NO_ERROR;
	if (ret_code != KRERR_NO_ERROR)
	    return( ret_code );

	NetModified = FALSE;
    }

    /*  initialize weights of the hidden units  */

    /* random initialization of Kohonen layer, the parameters are:
     * use all patterns (0, kr_TotalNoOfPattern() - 1)
     * unused learning rate (0.0)
     * no learning cycles (0)
     * shuffle pattern flag (1)
     */
    ret_code = RbfKohonenInit(0, kr_TotalNoOfPattern() - 1, 0.0, 0, 1);
    if (ret_code != KRERR_NO_ERROR)
	return ret_code;

    return KRERR_NO_ERROR;
}

/*****************************************************************************
  FUNCTION : INIT_SOM_Weights_v32

  PURPOSE  : Old (Version 3.2) style initialization. Initialization vectors are
             taken from the RECTANGLE bounded by param1 and param2.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err INIT_SOM_Weights_v32(float *parameterArray, int NoOfParams)
{
  register struct Unit *unit_ptr;
  register struct Site *site_ptr;
  register struct Link *link_ptr;
  register TopoPtrArray  topo_ptr;

  register FlintType  sum, amount, range;
  FlintType min, max;
  int  ret_code;


  if ( (unit_array == NULL) || (NoOfUnits == 0) )
    return( KRERR_NO_UNITS );  /*  there is nothing to do  */

  min = INIT_PARAM1( parameterArray );
  max = INIT_PARAM2( parameterArray );

  if(max < min){
      range = min; min = max; max = range;
  }
  range = max - min;

  if (NetModified || (TopoSortID != TOPOLOGIC_TYPE))
    {  /*  networt was modified or topologic array isn't initialized  */

      ret_code = kr_topoSort( TOPOLOGIC_TYPE );
      if (ret_code == KRERR_NO_OUTPUT_UNITS) ret_code = KRERR_NO_ERROR;
      if (ret_code != KRERR_NO_ERROR)
	return( ret_code );
      NetModified = FALSE;
    }

  topo_ptr = topo_ptr_array + (NoOfInputUnits + 1);

  /*  initialize weights of the hidden units  */
  while ((unit_ptr = *++topo_ptr) != NULL)
    { /*  this is a hidden unit  */
      /***********************************************************/
      /*     initialize the weights to the Kohonen Layer         */
      /***********************************************************/
    sum = 0.0;
    if UNIT_HAS_SITES( unit_ptr )
      { /* the unit has sites */
      FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )  {
        link_ptr->weight = (FlintType) drand48() * range + min;
        sum += link_ptr->weight * link_ptr->weight;
      }
    }
    else
      { /* the unit has direct links */
      FOR_ALL_LINKS( unit_ptr, link_ptr )  {
        link_ptr->weight = (FlintType) drand48() * range + min;
        sum += link_ptr->weight * link_ptr->weight;
      }
    }
    /* normalize the weightvector to the Kohonen Layer */


    amount = ((sum==0.0)? 0.0 : (1.0 / sqrt( sum )));
   
    unit_ptr->bias = 0.0;
    unit_ptr->value_a = 0.0; /*initialisation is necessary for spanning tree */
   
    if UNIT_HAS_SITES( unit_ptr )
      /* the unit has sites */
      FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
          link_ptr->weight = link_ptr->weight * amount;
    else
      /* the unit has direct links */
      FOR_ALL_LINKS( unit_ptr, link_ptr )
        link_ptr->weight = link_ptr->weight * amount;
  }

  return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : INIT_SOM_Weights_const

  PURPOSE  : constant initialization of Kohonen networks.
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err INIT_SOM_Weights_const(float *parameterArray, int NoOfParams)
{
  register struct Unit *unit_ptr;
  register struct Site *site_ptr;
  register struct Link *link_ptr;
  register TopoPtrArray  topo_ptr;

  int  ret_code;


  if ( (unit_array == NULL) || (NoOfUnits == 0) )
    return( KRERR_NO_UNITS );  /*  there is nothing to do  */

  if (NetModified || (TopoSortID != TOPOLOGIC_TYPE))
    {  /*  networt was modified or topologic array isn't initialized  */

      ret_code = kr_topoSort( TOPOLOGIC_TYPE );
      if (ret_code == KRERR_NO_OUTPUT_UNITS) ret_code = KRERR_NO_ERROR;
      if (ret_code != KRERR_NO_ERROR)
	return( ret_code );
      NetModified = FALSE;
    }

  topo_ptr = topo_ptr_array + (NoOfInputUnits + 1);

  /*  initialize weights of the hidden units  */
  while ((unit_ptr = *++topo_ptr) != NULL){
      /*  this is a hidden unit  */
      if UNIT_HAS_SITES( unit_ptr ){
	  /* the unit has sites */
	  FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )  {
	      link_ptr->weight = (FlintType) 1.0/sqrt(NoOfInputUnits);
	  }
      }else{
	  /* the unit has direct links */
	  FOR_ALL_LINKS( unit_ptr, link_ptr )  {
	      link_ptr->weight = (FlintType) 1.0/sqrt(NoOfInputUnits);
	  }
      }

      unit_ptr->bias = 0.0;
      unit_ptr->value_a = 0.0; /*initialisation is necessary for spanning tree*/
  }

  return( KRERR_NO_ERROR );
}



/*****************************************************************************
  FUNCTION : INIT_JE_Weights

  PURPOSE  : initialization function for JORDAN / ELMAN networks
  NOTES    : 

  UPDATE   : 
******************************************************************************/

krui_err  INIT_JE_Weights (float *parameterArray, int NoOfParams)

{
  register unsigned short   flags    ;
  register struct   Link   *link_ptr ;
  register struct   Site   *site_ptr ;
  register struct   Unit   *unit_ptr ;
  FlintType                 range     , min_weight, max_weight ;
  FlintType                 srl_weight, rec_weight, con_iact   ;


  if ((unit_array == NULL) || (NoOfUnits == 0)) return (KRERR_NO_UNITS) ;  
 
  if (NoOfParams != 5) return (KRERR_PARAMETERS) ;  

  min_weight = INIT_PARAM1 (parameterArray) ;
  max_weight = INIT_PARAM2 (parameterArray) ;
                                              /* context units:              */
  srl_weight = INIT_PARAM3 (parameterArray) ; /* self recurrent links        */
  rec_weight = INIT_PARAM4 (parameterArray) ; /* other links to context units*/
  con_iact   = INIT_PARAM5 (parameterArray) ; /* initial activation          */
 
  range = max_weight - min_weight ;

  /*
   * PATCHED 2005-08-01 -- Sabit Cakmak
   * Ensure the random number generator has been initialized, or an error may occur
   */
  krui_setSeedNo(0);

  /* if (range < 0.0) return (KRERR_PARAMETERS) ; */

  FOR_ALL_UNITS (unit_ptr) 
  {
    flags = unit_ptr->flags ;

    if ((flags & UFLAG_IN_USE) == UFLAG_IN_USE)
    {
      if (IS_HIDDEN_UNIT (unit_ptr) && IS_SPECIAL_UNIT (unit_ptr))
      {
        /* special hidden is the type of context units */

        unit_ptr->bias        = 0.5 ;
        unit_ptr->i_act       = con_iact ;
        unit_ptr->act         = con_iact ;
        unit_ptr->Out.output  = con_iact ;

        if ((flags & UFLAG_INPUT_PAT) == UFLAG_SITES)
        { 
          /*  unit has sites  */
	  FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
            if (link_ptr->to == unit_ptr)
	    { link_ptr->weight = srl_weight ; }
            else
            { link_ptr->weight = rec_weight ; }
        }
        else 
        {
          /*  unit has no sites   */
          if ((flags & UFLAG_INPUT_PAT) == UFLAG_DLINKS)
          { /*  unit has direct links   */
	    FOR_ALL_LINKS (unit_ptr, link_ptr)
              if (link_ptr->to == unit_ptr)
	      { link_ptr->weight = srl_weight ; }
              else
              { link_ptr->weight = rec_weight ; }
          }
        } /* else */
      }
      else
      {
        unit_ptr->bias = (FlintType) drand48() * range + min_weight ;

        if ((flags & UFLAG_INPUT_PAT) == UFLAG_SITES)
        { 
          /*  unit has sites  */
	  FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
            link_ptr->weight = (FlintType) drand48() * range + min_weight ;
        } 
        else  
        {
          /*  unit has no sites   */
          if ((flags & UFLAG_INPUT_PAT) == UFLAG_DLINKS)
          { /*  unit has direct links   */
	    FOR_ALL_LINKS (unit_ptr, link_ptr)
              link_ptr->weight = (FlintType) drand48() * range + min_weight ;
          }
        } /* else */
      } /* if */
    } /* if */
  } /* for */

  return (KRERR_NO_ERROR) ;
}




/*****************************************************************************
  FUNCTION : INIT_Hebb

  PURPOSE  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/

krui_err INIT_Hebb(float *parameterArray, int NoOfParams)
{
  register struct Unit  *unit_ptr;
  register struct Link  *link_ptr;
  register Patterns     in_pat,  out_pat;
  FlintType             BiasIn, BiasOut;
  int                   pattern_no, sub_pat_no;
  
  if ( (unit_array == NULL) || (NoOfUnits == 0) ) {
    return( KRERR_NO_UNITS  );         /*  there is nothing to do  */
  } /*if*/
  if ( (kr_TotalNoOfSubPatPairs() == 0) || (NoOfUnits == 0) ) {
    return( KRERR_NO_PATTERNS  );         /*  there is nothing to do  */
  } /*if*/

  NoOfInputUnits = krui_getNoOfInputUnits();
  
  /* init bias */
  
  BiasIn = parameterArray[0];
  BiasOut= parameterArray[1];
  
  FOR_ALL_UNITS(unit_ptr) {
    if (IS_INPUT_UNIT(unit_ptr)){
      if (BiasIn == 1.0)   
	unit_ptr->bias = (FlintType) log((double) NoOfOutputUnits); 
      else 
	unit_ptr->bias = BiasIn;
    }
    else if IS_OUTPUT_UNIT(unit_ptr) {
      if (BiasOut == -1.0)   
	unit_ptr->bias = (FlintType) log((double) NoOfInputUnits);
      else 
	unit_ptr->bias = BiasOut;
    }
  } /* for all units */
  
  /* init links */
  
  FOR_ALL_UNITS(unit_ptr)
    FOR_ALL_LINKS(unit_ptr, link_ptr)
      { link_ptr->weight = 0.0;
      }
  
  /* compute the necessary sub patterns */
  KernelErrorCode = kr_initSubPatternOrder(0, kr_TotalNoOfPattern() - 1);
  if(KernelErrorCode != KRERR_NO_ERROR)
  {
      if (KernelErrorCode == KRERR_NP_NO_TRAIN_SCHEME)
	  KernelErrorCode = KRERR_NP_WORKAROUND;
      return (KernelErrorCode);
  }

  while(kr_getSubPatternByOrder(&pattern_no, &sub_pat_no))
  {  /*  calc. startaddress of pattern entries  */
      in_pat = kr_getSubPatData(pattern_no, sub_pat_no, INPUT, NULL);
      out_pat = kr_getSubPatData(pattern_no, sub_pat_no, OUTPUT, NULL);
      
      FOR_ALL_UNITS( unit_ptr )
	  /*        if UNIT_IN_USE( unit_ptr )
	   */      {
	       if IS_INPUT_UNIT( unit_ptr )
		   unit_ptr->act = *in_pat++;
	       if IS_OUTPUT_UNIT( unit_ptr )
		   unit_ptr->act = *out_pat++;
	   }
      
      FOR_ALL_UNITS(unit_ptr)
	  FOR_ALL_LINKS(unit_ptr, link_ptr)
          { 
	      link_ptr->weight += (unit_ptr->act) * (link_ptr->to->act);
          } 
  }/* all patterns */
  
  return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : INIT_ClippHebb

  PURPOSE  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
krui_err INIT_ClippHebb(float *parameterArray, int NoOfParams)
{
  register struct Unit *unit_ptr;
  register struct Link *link_ptr;
  register Patterns     in_pat,  out_pat;
  FlintType             BiasIn, BiasOut; 
  int                   pattern_no, sub_pat_no;
  
  if ( (unit_array == NULL) || (NoOfUnits == 0) ) {
    return( KRERR_NO_UNITS  );         /*  there is nothing to do  */
  } /*if*/
  if ( (kr_TotalNoOfSubPatPairs() == 0) || (NoOfUnits == 0) ) {
    return( KRERR_NO_PATTERNS  );         /*  there is nothing to do  */
  } /*if*/
  
  NoOfInputUnits = krui_getNoOfInputUnits();
  
  /* init bias */
  BiasIn = parameterArray[0];
  BiasOut= parameterArray[1];
  
  FOR_ALL_UNITS(unit_ptr) {
    if (IS_INPUT_UNIT(unit_ptr)){
      if (BiasIn == 1.0)   
	unit_ptr->bias = (FlintType) log((double) NoOfOutputUnits); 
      else 
	unit_ptr->bias = BiasIn;
    }
    else if IS_OUTPUT_UNIT(unit_ptr) {
      if (BiasOut == -1.0)   
	unit_ptr->bias = (FlintType) log((double) NoOfInputUnits);
      else 
	unit_ptr->bias = BiasOut;
    }
  }
  /* init links */
  
  FOR_ALL_UNITS(unit_ptr)
    FOR_ALL_LINKS(unit_ptr, link_ptr)
      { link_ptr->weight = 0.0;
      }
  
  /* compute the necessary sub patterns */
  KernelErrorCode = kr_initSubPatternOrder(0, kr_TotalNoOfPattern() - 1);
  if(KernelErrorCode != KRERR_NO_ERROR)
  {
      if (KernelErrorCode == KRERR_NP_NO_TRAIN_SCHEME)
	  KernelErrorCode = KRERR_NP_WORKAROUND;
      return (KernelErrorCode);
  }

  while(kr_getSubPatternByOrder(&pattern_no, &sub_pat_no))
  {  /*  calc. startaddress of pattern entries  */
      in_pat = kr_getSubPatData(pattern_no, sub_pat_no, INPUT, NULL);
      out_pat = kr_getSubPatData(pattern_no, sub_pat_no, OUTPUT, NULL);

      FOR_ALL_UNITS( unit_ptr )
	  /*        if UNIT_IN_USE( unit_ptr ) */
      {
	  if IS_INPUT_UNIT( unit_ptr ) 
	      unit_ptr->act = *in_pat++; 
	  if IS_OUTPUT_UNIT( unit_ptr )
	      unit_ptr->act = *out_pat++;
      }
      
      FOR_ALL_UNITS(unit_ptr)
	  FOR_ALL_LINKS(unit_ptr, link_ptr)
	  { 
	      if (link_ptr->weight == 0) { 
		  link_ptr->weight += (unit_ptr->act) * (link_ptr->to->act); 
	      }
	      /*debugg*/
	      /*	  fprintf(stderr, "jetzt gesetzte gewichte");
			  fprintf(stderr, "%f", link_ptr->weight);     
			  */ 
	  } 
  } /* all patterns */
  
  return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : INIT_HOP_FixAct

  PURPOSE  : 
  NOTES    : 

  UPDATE   : 
******************************************************************************/
krui_err INIT_HOP_FixAct(float *parameterArray, int NoOfParams)
{
  register struct Unit       *unit_ptr;
  register struct Link       *link_ptr;
  register Patterns          in_pat;
  
  FlintType                  activity;
  FlintType                  error_probability;
  FlintType                   Bias;
  FlintType                   activityHoch3;
  int                         ret_code = KRERR_NO_ERROR;
  int                        pattern_no, sub_pat_no;
  
  if (NoOfParams < 2) {
    ret_code = KRERR_PARAMETERS;
    return( ret_code );  /*  Not the same no. of input parameters */
  } 
  if ( (kr_TotalNoOfSubPatPairs() == 0) || (NoOfUnits == 0) ) {
    return( KRERR_NO_PATTERNS  );         /*  there is nothing to do  */
  } /*if*/
  
  NoOfUnits = krui_getNoOfUnits();
  
  if ( (unit_array == NULL) || (NoOfUnits == 0) ) {
    return( KRERR_NO_UNITS  );         /*  there is nothing to do  */
  } 
  
  
  activity  = (float)parameterArray [0]/(float)NoOfUnits;
  error_probability = parameterArray [1]/100.0;
  
  
  /* set bias to the value, that Amari published in Neur.Netw. Vol.2, 451-457 '89 */  
  
  activityHoch3 = ( FlintType ) pow( (double) activity, (double) 3.0 );
  Bias = kr_TotalNoOfSubPatPairs()*( activityHoch3 ) + 0.5*activity*(1 - error_probability);
  
  FOR_ALL_UNITS(unit_ptr)
    unit_ptr->bias = Bias;
  
  /* init links */
  
  FOR_ALL_UNITS(unit_ptr) {
    FOR_ALL_LINKS(unit_ptr, link_ptr)
      link_ptr->weight = 0.0;
  }
  
  /* compute the necessary sub patterns */
  KernelErrorCode = kr_initSubPatternOrder(0, kr_TotalNoOfPattern() - 1);
  if(KernelErrorCode != KRERR_NO_ERROR)
  {
      if (KernelErrorCode == KRERR_NP_NO_TRAIN_SCHEME)
	  KernelErrorCode = KRERR_NP_WORKAROUND;
      return (KernelErrorCode);
  }

  while(kr_getSubPatternByOrder(&pattern_no, &sub_pat_no))
  {  /*  calc. startaddress of pattern entries  */
      in_pat = kr_getSubPatData(pattern_no, sub_pat_no, INPUT, NULL);

      FOR_ALL_UNITS( unit_ptr ) {
	if IS_INPUT_UNIT( unit_ptr )
	  unit_ptr->act = *in_pat++;
      }
      
      FOR_ALL_UNITS(unit_ptr) {
	FOR_ALL_LINKS(unit_ptr, link_ptr)
	  link_ptr->weight += (1.0/NoOfUnits)*(unit_ptr->act) * (link_ptr->to->act);
      }
    }/* all patterns */
  
  return(KRERR_NO_ERROR );
}

/*****************************************************************************
  FUNCTION : PseudoInv

  PURPOSE  : rekursive calculation of the Pseudoinverse 
  NOTES    : Pseudoinverse calculation of the weights  (Hopfield) 
             with Theorem of Greville ( Kohonen 87 )                          

  UPDATE   : 
******************************************************************************/
krui_err   PseudoInv(RbfFloatMatrix *source, int NoOfColumns, RbfFloatMatrix *target )
{
  int            i,j;
  int            malloc_fault;
  float          qnorm;
  krui_err       ret_code;
  RbfFloatMatrix A;  /* source without last column                          */
  RbfFloatMatrix PA; /* PseudoInverse of A                                  */
  RbfFloatMatrix PP; /* auxiliary matrix with the same dimensions as PA     */
  RbfFloatMatrix a;  /* last column of source                               */
  RbfFloatMatrix p;  /* auxiliary vector with the same dimensions as 'a'    */
  RbfFloatMatrix P;  /* aux. square matrix with the same nr. of rows as source*/
  RbfFloatMatrix aux;
  RbfFloatMatrix pT; /* transposed of 'p'  */
  

  NoOfInputUnits = krui_getNoOfInputUnits();
  
  if (NoOfColumns <= 1)

    /* end of recursion */

    if (NoOfColumns <= 0)
      return(KRERR_NO_PATTERNS);
    else {
      /* a+ = aT / aT*a if a != 0 and
	 = 0T        if a == 0      
	 */
      
      qnorm = 0.0;
      for (i = 0; i <= (source->rows - 1); i++)
	qnorm += RbfMatrixGetValue(source, i, 0)*RbfMatrixGetValue(source, i, 0);
      
      for (i = 0; i <= (source->rows - 1); i++)
	if (qnorm != 0.0)
	  /* set the 1-st row of target to the 1-st column of source */
	  RbfMatrixSetValue(target, 0, i, RbfMatrixGetValue(source, i, 0)/qnorm);
	else 
	  RbfMatrixSetValue(target, 0, i, RbfMatrixGetValue(source, i, 0));    
    }
  else
    { 
      /* recursion */
      ret_code = PseudoInv(source, NoOfColumns - 1, target);
      if (ret_code != KRERR_NO_ERROR)
	return(ret_code);
      malloc_fault = 0;
      if (!RbfAllocMatrix(source->rows, NoOfColumns - 1, &A) ) 
	malloc_fault = 1;
      else if (!RbfAllocMatrix(NoOfColumns - 1, (source->rows), &PA))
	malloc_fault = 2;
      else if (!RbfAllocMatrix(source->rows, 1, &a))
	malloc_fault = 3;
      else if (!RbfAllocMatrix(source->rows, 1, &p))
	malloc_fault = 4;
      else if (!RbfAllocMatrix( source->rows, source->rows, &P ))
	malloc_fault = 5;     
      else if (!RbfAllocMatrix(NoOfColumns - 1, 1, &aux))
	malloc_fault = 6;
      else if (!RbfAllocMatrix(1, source->rows, &pT))
	malloc_fault = 7;
      else if (!RbfAllocMatrix(NoOfColumns - 1, (source->rows), &PP))
	malloc_fault = 8;
       
      if (malloc_fault != 0) {
	if (malloc_fault >= 2)
	  RbfFreeMatrix(&A);
	if (malloc_fault >= 3)
	  RbfFreeMatrix(&PA);
	if (malloc_fault >= 4)
	  RbfFreeMatrix(&a);
	if (malloc_fault >= 5)
	  RbfFreeMatrix(&p);
	if (malloc_fault >= 6)
	  RbfFreeMatrix(&P);
	if (malloc_fault >= 7)
	  RbfFreeMatrix(&aux);
	if (malloc_fault >= 8)
	  RbfFreeMatrix(&pT);
	return(KRERR_INSUFFICIENT_MEM);
      }
      
      /* init matrices */
      for (i = A.rows - 1 ; i>=0; i--){ 
	for(j = A.columns - 1; j >=0; j--){
	  RbfMatrixSetValue(&A, i, j, RbfMatrixGetValue(source, i, j));
	}
      }
      
       for (i = PA.rows - 1; i>=0; i--){ 
	for(j = PA.columns - 1; j >=0; j--){
	  RbfMatrixSetValue(&PA, i, j, RbfMatrixGetValue(target, i, j));
	}
      }
     
      for (i = a.rows - 1 ; i>=0; i--){ 
	for(j = a.columns - 1; j >=0; j--){
	  RbfMatrixSetValue(&a, i, j, RbfMatrixGetValue(source, i, NoOfColumns));
	}
      }


      /* calculate  'p'  */
      RbfMulMatrix(&P, &A, &PA);                                         /* P = A*PA   */
      RbfMulScalarMatrix(&P, -1.0);                                      /* P = Id - P */
      for (i = (P.rows -1) ; i>=0; i--) {                                 
	RbfMatrixSetValue(&P, i, i, RbfMatrixGetValue(&P, i, i) + 1);
      }
      RbfMulMatrix(&p, &P, &a);                                           /* p = P*a   */
      qnorm = RbfSquareOfNorm(&p);
      if (qnorm != 0)
	RbfMulScalarMatrix(&p, 1/qnorm);
      else {
	RbfMulMatrix(&aux, &PA, &a);
	qnorm = RbfSquareOfNorm(&aux);
	RbfTranspMatrix(&A, &PA);                                        /* A = PA transposed */
	RbfMulMatrix(&p, &A, &aux);
	RbfMulScalarMatrix(&p, 1/(1 + qnorm));
      }
      /* calculate target */
      RbfTranspMatrix(&pT, &p);
      RbfMulMatrix(&P, &a, &pT);                                         /* P = A*pT   */ 
      RbfMulScalarMatrix(&P, -1.0);                                      /* P = Id - P */
      for (i = (P.rows -1) ; i>=0; i--){                                 
	RbfMatrixSetValue(&P, i, i, RbfMatrixGetValue(&P, i, i) + 1);
      }
      RbfMulMatrix(&PP, &PA, &P);
      /* set target elements */
      for (i = (PP.rows - 1) ; i>=0; i--)
	{
	  for (j = (PP.columns -1) ; j>=0; j--)
	    RbfMatrixSetValue(target, i, j, RbfMatrixGetValue(&PP, i, j));
	};
      for (j = (PP.columns -1) ; j>=0; j--)
	RbfMatrixSetValue(target, PP.rows , j, RbfMatrixGetValue(&pT, 0, j)); 
      
      /* free matrices */

      RbfFreeMatrix(&PP);      
      RbfFreeMatrix(&A);
      RbfFreeMatrix(&PA);
      RbfFreeMatrix(&a);
      RbfFreeMatrix(&p);
      RbfFreeMatrix(&P);
      RbfFreeMatrix(&aux);
      RbfFreeMatrix(&pT);

      
    }
  return(KRERR_NO_ERROR);  
}


/*****************************************************************************
  FUNCTION : INIT_PseudoInv

  PURPOSE  : 
  NOTES    : 
             

  UPDATE   : 
******************************************************************************/

krui_err INIT_PseudoInv(float *parameterArray, int NoOfParams)
{
  register struct Unit *unit_ptr;
  register struct Link *link_ptr;
  register Patterns     in_pat,  out_pat;
  int          unit_no;
  float       *ptr_to_W;
  RbfFloatMatrix       X;  /* rows = NoOfInputUnits columns = kr_TotalNoOfSubPatPairs()*/
  RbfFloatMatrix       PIofX;   /* is to store the Pseudoinverse of X */
  RbfFloatMatrix       W;       /* stores the weights of the net */
  RbfFloatMatrix       Y;       /* stores the output Patterns    */
  int	      malloc_fault;
  krui_err             ret_code = KRERR_NO_ERROR;
  int                  pattern_no, sub_pat_no;
  int                  end_sp;
  int                  act_sub_nr;
  
  if ( (unit_array == NULL) || (NoOfUnits == 0) ) {
    return( KRERR_NO_UNITS  );         /*  there is nothing to do  */
  } /*if*/
  
  
  NoOfInputUnits = krui_getNoOfInputUnits();
  
  NoOfOutputUnits = krui_getNoOfOutputUnits();
  
  /* init links */
  
  FOR_ALL_UNITS(unit_ptr)
    FOR_ALL_LINKS(unit_ptr, link_ptr)
      { link_ptr->weight = 0.0;
      }
  /* allocate memory for the matrices */
  
  malloc_fault = 0;
  if (!RbfAllocMatrix(NoOfInputUnits, kr_TotalNoOfSubPatPairs(), &X))
    malloc_fault = 1;
  else if (!RbfAllocMatrix(NoOfOutputUnits, NoOfInputUnits, &W))
    malloc_fault = 2;
  else if (!RbfAllocMatrix(kr_TotalNoOfSubPatPairs(), NoOfInputUnits, &PIofX))
    malloc_fault = 3;
  else if (!RbfAllocMatrix(NoOfOutputUnits, kr_TotalNoOfSubPatPairs(), &Y))
    malloc_fault = 4;
  
  if (malloc_fault != 0)
    {
      if (malloc_fault >= 2)
	RbfFreeMatrix(&X);
      if (malloc_fault >= 3)
	RbfFreeMatrix(&W);
      if (malloc_fault >= 4)
	RbfFreeMatrix(&PIofX);
      return(KRERR_INSUFFICIENT_MEM);
    }
  
  RbfClearMatrix(&X, 0.0); 
  RbfClearMatrix(&W, 0.0);  
  RbfClearMatrix(&PIofX, 0.0);
  RbfClearMatrix(&Y, 0.0);
   
  /* set X  and Y */
  
  end_sp = kr_TotalNoOfSubPatPairs() - 1;
  for (act_sub_nr=0; act_sub_nr <= end_sp; act_sub_nr++)
    {  /*  calc. startaddress of pattern entries  */
	kr_getSubPatternByNo(&pattern_no, &sub_pat_no, act_sub_nr);
	in_pat = kr_getSubPatData(pattern_no, sub_pat_no, INPUT, NULL);
	out_pat = kr_getSubPatData(pattern_no, sub_pat_no, OUTPUT, NULL);

      for ( unit_no = 0; unit_no <= NoOfInputUnits -1; unit_no++)
	RbfMatrixSetValue(&X, unit_no, act_sub_nr, *in_pat++);
      for ( unit_no = 0; unit_no <= NoOfOutputUnits -1; unit_no++)
	RbfMatrixSetValue(&Y, unit_no, act_sub_nr, *out_pat++);
      
    };
  
  /* calculate the Pseudoinverse of X */
  
  ret_code = PseudoInv(&X, X.columns, &PIofX);
  if(ret_code != KRERR_NO_ERROR)
    return(ret_code);
  
  /* calculate the weight matrix */
  
  RbfMulMatrix(&W, &Y, &PIofX);
  
  
  /* set the corresponding link weights */
  
  ptr_to_W = W.field;
  /* There is a full connection between the units */
  FOR_ALL_UNITS(unit_ptr)     
    FOR_ALL_LINKS(unit_ptr, link_ptr)
      {
	link_ptr->weight =  *ptr_to_W++ ;
	/*debugg*/
	/*	  fprintf(stderr, "jetzt gesetzte gewichte");
		  fprintf(stderr, "%f", link_ptr->weight);     
		  */ 
      } 
 
  /* free matrices */
  
  RbfFreeMatrix(&X);
  RbfFreeMatrix(&W);
  RbfFreeMatrix(&PIofX);
  RbfFreeMatrix(&Y);
  
  return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : ENZO_noinit

  PURPOSE  : This initialization function doesn't initialize. This is to 
             prevent initialization in the case that a function has to be set. 
  NOTES    : 
             

  UPDATE   : 
******************************************************************************/

krui_err ENZO_noinit()
{
  return ( KRERR_NO_ERROR );
}






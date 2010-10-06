/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_JordElm.c,v $
  SHORTNAME      : kr_JordElm.c
  SNNS VERSION   : 4.2

  PURPOSE        : Topological sorting for Jordan and Elman networks 
  NOTES          :

  AUTHOR         : Tobias Soyez
  DATE           : 09.11.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
******************************************************************************/
#include <config.h>


#include "kr_typ.h"	
#include "kr_const.h"	 
#include "kr_def.h"	 
#include "kr_mac.h"
#include "kernel.h"
#include "kr_JordElm.ph"



/*****************************************************************************
  FUNCTION : kr_recTouchContextUnits

  PURPOSE  : touches only context units recursively
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/

static void kr_recTouchContextUnits (struct Unit *unit_ptr)

{
  struct Site   *site_ptr ;
  struct Link   *link_ptr ;
  bool           unit_has_incoming_links ;

  if (unit_ptr->flags & UFLAG_REFRESH) return ;

  if ((IS_HIDDEN_UNIT (unit_ptr)) && (IS_SPECIAL_UNIT (unit_ptr)))
  {
    /* -----------  touch only context units ------------- */

    unit_ptr->flags |= UFLAG_REFRESH ;             /* set the 'touch' flag  */

    unit_has_incoming_links = FALSE ;

    switch (unit_ptr->flags & UFLAG_INPUT_PAT)
    {
      case  UFLAG_DLINKS:                          /* unit has direct links  */
        FOR_ALL_LINKS (unit_ptr, link_ptr)
	{ 
          kr_recTouchContextUnits (link_ptr->to) ; 
          unit_has_incoming_links = TRUE ;
        }
        break ;

      case  UFLAG_SITES:	                         /*  unit has sites  */
        FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
	{
	  kr_recTouchContextUnits (link_ptr->to) ; 
          unit_has_incoming_links = TRUE ;
        }
        break ;
    }
  }

  if ((! unit_has_incoming_links) && (! IS_INPUT_UNIT (unit_ptr)))
  {
    /* unit has no incoming links -> dead unit -> delete touch flag */

    unit_ptr->flags &= ~UFLAG_REFRESH ;
  }
}



/*****************************************************************************
  FUNCTION : kr_recTopoCheckJE

  PURPOSE  : recursive topology check, called by kr_topoCheckJE
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/

static void  kr_recTopoCheckJE (struct Unit *unit_ptr, int depth)

{
  struct Site   *site_ptr ;
  struct Link   *link_ptr ;
  bool           unit_has_incoming_links ;



  if ((IS_HIDDEN_UNIT (unit_ptr)) && (IS_SPECIAL_UNIT (unit_ptr))) 
  {
    /* --------  touch context units recursively  -------- */
 
    kr_recTouchContextUnits (unit_ptr) ;
    return ;
  }

  if (unit_ptr->flags & UFLAG_REFRESH)
  {  
    if (unit_ptr->lln == 0)
    {  
      /*  logical layer no. isn't set => Cycle found  */
      topo_msg.no_of_cycles++ ;
      
      if (topo_msg.error_code == KRERR_NO_ERROR)
      { 
        topo_msg.src_error_unit = unit_ptr - unit_array ;
        topo_msg.error_code     = KRERR_CYCLES ;
      }
    }
    return ;
  }

 
  /* -----  unit is not a context unit and has not been touched yet  ------ */
  /* -----  continue recursive depth search                          ------ */

  unit_ptr->flags |= UFLAG_REFRESH ;             /* set the 'touch' flag  */

  unit_has_incoming_links = FALSE ;

  switch (unit_ptr->flags & UFLAG_INPUT_PAT)
  {
    case  UFLAG_DLINKS:                          /* unit has direct links  */
      FOR_ALL_LINKS (unit_ptr, link_ptr)
      {
	kr_recTopoCheckJE (link_ptr->to, depth + 1) ; 
        unit_has_incoming_links = TRUE ;
      }
      break ;

    case  UFLAG_SITES:	                         /*  unit has sites  */
      FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
      {
	kr_recTopoCheckJE (link_ptr->to, depth + 1) ; 
        unit_has_incoming_links = TRUE ;
      }
      break ;
  }

  /*  remember the depth (for cycle detection and statistics)  */
  unit_ptr->lln = depth ;

  /*  store highest layer no.  */
  if (depth > topo_msg.no_of_layers) topo_msg.no_of_layers = depth ;


  if ((! unit_has_incoming_links) && (! IS_INPUT_UNIT (unit_ptr)))
  {
    /* unit has no incoming links -> dead unit -> delete touch flag */

    unit_ptr->flags &= ~UFLAG_REFRESH ;
  }
}



/*****************************************************************************
  FUNCTION : kr_topoCheckJE

  PURPOSE  : Checks the topology of partial recurrent networks 
             (i.e. JORDAN or ELMAN networks) :
	     only recurrent links to context units are allowed
  NOTES    : 

  RETURNS  : kernel error code
  UPDATE   : 
******************************************************************************/

krui_err kr_topoCheckJE (void)

{
  struct Unit  *unit_ptr ;
  bool          o_units  ;


  topo_msg.no_of_cycles     = 
  topo_msg.no_of_dead_units =
  topo_msg.dest_error_unit  = 
  topo_msg.src_error_unit   =
  topo_msg.no_of_layers     = 0 ;
  topo_msg.error_code       = KernelErrorCode = KRERR_NO_ERROR ;


  if (NoOfUnits == 0)
  {
    /*  no units defined  */
    KernelErrorCode = KRERR_NO_UNITS ;
    return (KernelErrorCode) ;
  }


  /* -------------------  reset units 'touch' flags  ----------------------- */

  FOR_ALL_UNITS (unit_ptr)
    if (UNIT_IN_USE (unit_ptr))
    {
      unit_ptr->flags &= ~UFLAG_REFRESH ;
      unit_ptr->lln = 0 ;
    }


  /* ----------  begin depth search at the first output unit  -------------- */

  o_units = FALSE ;
  FOR_ALL_UNITS (unit_ptr)
    if (IS_OUTPUT_UNIT (unit_ptr) && UNIT_IN_USE (unit_ptr))
    {
      o_units = TRUE ;
      kr_recTopoCheckJE (unit_ptr, 1) ;
      if (topo_msg.error_code != KRERR_NO_ERROR)
      {  /*  stop if an error occured  */
        KernelErrorCode = topo_msg.error_code ;
        return (KernelErrorCode) ;
      }
    }
      
  if (!o_units)
  {  /*  no output units */
    KernelErrorCode = KRERR_NO_OUTPUT_UNITS ;
    return (KernelErrorCode) ;
  }


  /* ----------  search for dead units i.e. units without inputs  ---------- */

  FOR_ALL_UNITS (unit_ptr)
    if (!(unit_ptr->flags & UFLAG_REFRESH) && UNIT_IN_USE (unit_ptr))
    {
      topo_msg.error_code = KRERR_DEAD_UNITS ;
      topo_msg.no_of_dead_units++ ;
      if (topo_msg.src_error_unit == 0)
        topo_msg.src_error_unit = unit_ptr - unit_array ;
    }

  if (topo_msg.no_of_dead_units != 0)
    KernelErrorCode = KRERR_DEAD_UNITS ;

  return (topo_msg.error_code) ;
}



/*****************************************************************************
  FUNCTION : kr_recTopoSortJE

  PURPOSE  : stores only hidden (NOT special hidden !) units in the topologic 
             array
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/

static void  kr_recTopoSortJE (struct Unit *unit_ptr, int depth)

{
  struct Site   *site_ptr ;
  struct Link   *link_ptr ;


  /* ---------------------  ignore context units  ------------------------- */

  if ((IS_HIDDEN_UNIT (unit_ptr)) && (IS_SPECIAL_UNIT (unit_ptr)))
  {
    unit_ptr->flags |= UFLAG_REFRESH ;    /* set the 'touch' flag */
    return ;
  } 


  if (unit_ptr->flags & UFLAG_REFRESH)
  {  
    /* the 'touch' flag is set: don't continue search */
    topo_msg.src_error_unit = unit_ptr - unit_array ;  /* store unit number */

    if IS_OUTPUT_UNIT (unit_ptr)
    {  
      /*  this output unit has a output connection to another unit  */
      if (topo_msg.error_code == KRERR_NO_ERROR)
        topo_msg.error_code = KRERR_O_UNITS_CONNECT ;
    }
    else
      if (unit_ptr->lln == 0)
      {  
        /*  logical layer no. isn't set => Cycle found  */
          topo_msg.no_of_cycles++ ;
        if (topo_msg.error_code == KRERR_NO_ERROR)
          topo_msg.error_code = KRERR_CYCLES ;
      }

    return ;
  }
  else
    unit_ptr->flags |= UFLAG_REFRESH ; /* set the 'touch' flag */


  /* -------------------------  continue search  -------------------------- */

  switch (unit_ptr->flags & UFLAG_INPUT_PAT)
  {
    case  UFLAG_DLINKS:   
      FOR_ALL_LINKS (unit_ptr, link_ptr)
	kr_recTopoSortJE (link_ptr->to, depth + 1) ; 
      break ;

    case  UFLAG_SITES:	
      FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr)
	kr_recTopoSortJE (link_ptr->to, depth + 1) ; 
      break ;
  }

  /*  remember the depth (for cycle detection and statistics)  */
  unit_ptr->lln = depth ; 


  /* ------------------------  store hidden units  ------------------------ */

  if IS_HIDDEN_UNIT( unit_ptr )
  {
    *topo_ptr++ = unit_ptr ; 
    no_of_topo_units++ ;
  }
}



/*****************************************************************************
  FUNCTION : kr_topoSortJE

  PURPOSE  : sorts units of a partial recurrent network by their topologic 
             type input, hidden, output, context and stores the pointers to 
             this units in the topologic array
  NOTES    : ###########  V E R Y    I M P O R T A N T   N O T E  ############
             no_of_topo_units contains only the number of input, hidden and
             output units 

             no_of_topo_units DOES NOT contain the no. of context units !!!

             all special hidden units (io-type = SPECIAL_H) are assumed to
             be context units 

             !! before calling this function:                  !!
             !! check the network topology with kr_topoCheckJE !!

  RETURNS  : error code 
  UPDATE   : 
******************************************************************************/

krui_err kr_topoSortJE (void)

{ 
  register struct Unit	 *unit_ptr ;


  KernelErrorCode  = KRERR_NO_ERROR ;  /* reset return code                  */
  topo_ptr         = topo_ptr_array ;  /* initialize global pointer          */
  NoOfInputUnits   = 0 ;
  NoOfOutputUnits  = 0 ;
  no_of_topo_units = 0 ;
  *topo_ptr++      = NULL ;            /* limit left side of the topological */
                                       /* array with NULL pointer            */


  /* ------------------  reset 'touch' flags of all units  ----------------- */

  FOR_ALL_UNITS (unit_ptr)
    if (UNIT_IN_USE (unit_ptr))
    {
      unit_ptr->flags &= ~UFLAG_REFRESH ;
      unit_ptr->lln = 0 ;
    }


  /* -----------  store all input units in the topologic array  ------------ */

  FOR_ALL_UNITS (unit_ptr)
    if (IS_INPUT_UNIT (unit_ptr) && UNIT_IN_USE (unit_ptr))
    {
      if UNIT_HAS_INPUTS (unit_ptr)
      { 
        /* links to input units are not allowed */ 
        topo_msg.dest_error_unit = unit_ptr - unit_array ;  
	KernelErrorCode          = KRERR_I_UNITS_CONNECT ; 
        return (KernelErrorCode) ;
      }

      NoOfInputUnits++   ;	
      no_of_topo_units++ ;	
      *topo_ptr++ = unit_ptr ;  
    }

  *topo_ptr++ = NULL ;  /* limit input units in the topological array with   */
                        /* NULL pointer                                      */

  if (NoOfInputUnits == 0)
  {  
    KernelErrorCode = KRERR_NO_INPUT_UNITS ;
    return( KernelErrorCode ) ;
  }


  /* -----------  store all hidden units in the topologic array  ----------- */
  /* sorts hidden units by their topological order using recursive depth     */
  /* search starting at the first output unit                                */

  FOR_ALL_UNITS( unit_ptr )
    if (IS_OUTPUT_UNIT (unit_ptr) && UNIT_IN_USE (unit_ptr))
    {
      kr_recTopoSortJE (unit_ptr, 1) ;  
      if (topo_msg.error_code != KRERR_NO_ERROR)
      {  
        KernelErrorCode = topo_msg.error_code ;
        return (KernelErrorCode) ;
      }
    }

  *topo_ptr++ = NULL ;  /* limit hidden units in the topological array with  */
                        /* NULL pointer                                      */


  /* -----------  store all output units in the topologic array  ----------- */

  FOR_ALL_UNITS (unit_ptr)
    if (IS_OUTPUT_UNIT (unit_ptr) && UNIT_IN_USE (unit_ptr))
    {
      NoOfOutputUnits++  ;	
      no_of_topo_units++ ;
      *topo_ptr++ = unit_ptr ;
    }

  if (NoOfOutputUnits == 0)
  { 
    KernelErrorCode = KRERR_NO_OUTPUT_UNITS ;
    return (KernelErrorCode) ;
  }

  *topo_ptr++ = NULL ;  /* limit output units in the topological array with  */
                        /* NULL pointer                                      */


  /* ----------  store all context units in the topologic array  ----------- */
  /*       !!!  no_of_topo_units MUST NOT BE INCREMENTED ANY MORE  !!!       */
  
  FOR_ALL_UNITS (unit_ptr)
    if (IS_HIDDEN_UNIT (unit_ptr) && IS_SPECIAL_UNIT (unit_ptr) &&
        UNIT_IN_USE (unit_ptr))
    {
      *topo_ptr++ = unit_ptr ;
    }

  *topo_ptr++ = NULL ;  /* limit context units in the topological array with */
                        /* NULL pointer                                      */


  /* ---------  search for dead units i.e. units without inputs  ----------- */

  FOR_ALL_UNITS (unit_ptr)
    if (!(unit_ptr->flags & UFLAG_REFRESH) && UNIT_IN_USE (unit_ptr))
    {
      topo_msg.no_of_dead_units++ ;
      if (topo_msg.src_error_unit == 0)
        topo_msg.src_error_unit = unit_ptr - unit_array ; 
    }

  if (topo_msg.no_of_dead_units != 0) KernelErrorCode = KRERR_DEAD_UNITS ;


  return( KernelErrorCode ) ;
}



/*****************************************************************************
                        E N D     O F     F I L E
******************************************************************************/





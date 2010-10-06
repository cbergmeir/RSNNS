/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kernel.c,v $
  SHORTNAME      : kernel.c
  SNNS VERSION   : 4.2
 
  PURPOSE        : SNNS Kernel
  NOTES          :
 
  AUTHOR         : Niels Mache
  DATE           : 20.02.90
 
  CHANGED BY     : Sven Doering, Michael Vogt, Guenter Mamier,Christine Bagdi,
                   Thomas Gern
  RCS VERSION    : $Revision: 2.24 $
  LAST CHANGE    : $Date: 1998/05/15 13:12:06 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#include <config.h>
#define  SNNS_KERNEL

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include <math.h>

#include "kr_typ.h"	 /*  Kernel Types and Constants  */
#include "kr_const.h"	 /*  Constant Declarators for SNNS-Kernel  */
#include "kr_def.h"	 /*  Default Values  */

#ifndef rand
#include "random.h"	 /*  Randomize Library Function Prototypes  */
#endif

#include "kernel.ph"	 /*  Function Prototypes  */
#include "kr_mem.h"	 /*  Function Prototypes  */
#include "kr_funcs.h"	 /*  Function Prototypes  */
#include "kr_mac.h"	 /*  Kernel Macros  */
#include "cc_glob.h"
#include "kr_newpattern.h"
#include "prun_f.h"
#include "learn_f.h"
#include "matrix.h"

#ifdef MASPAR_KERNEL

#include "kr_feedf.h"	 /*  Function Prototypes */

#endif

#include "kr_art.h"      /*  Function Prototypes  */
#include "kr_art1.h"     /*  Prototypes and global defs for ART1  */
#include "kr_art2.h"     /*  Prototypes and global defs for ART2  */
#include "kr_amap.h"     /*  Prototypes and global defs for ARTMAP  */
#include "kr_JordElm.h"

extern FlintType OUT_Custom_Python(FlintType act);
extern FlintType ACT_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_DERIV_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_2_DERIV_Custom_Python(struct Unit * unit_ptr);

PyObject *kr_findPythonFunction(char *name, int type);
FlintType kr_PythonOutFunction(PyObject *func, FlintType activation);

/*****************************************************************************
  FUNCTION : kr_countUnits

  PURPOSE  : count units according to their topological type 
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static void  kr_countUnits(struct Unit *unit_ptr, int mode)
{
  if (mode == UNIT_ADD)  {
    /*  add unit  */
    switch (unit_ptr->flags & UFLAG_TTYP_PAT)  {
      case  UFLAG_TTYP_IN:
        NoOfInputUnits++;
        break;
      case  UFLAG_TTYP_OUT:
        NoOfOutputUnits++;
        break;
      case  UFLAG_TTYP_HIDD:
        NoOfHiddenUnits++;
        break;
    }
    return;
  }
  if (mode == UNIT_DELETE)  {
    /*  delete unit  */
    switch (unit_ptr->flags & UFLAG_TTYP_PAT)  {
      case  UFLAG_TTYP_IN:
        --NoOfInputUnits;
        break;
      case  UFLAG_TTYP_OUT:
        --NoOfOutputUnits;
        break;
      case  UFLAG_TTYP_HIDD:
        --NoOfHiddenUnits;
        break;
    }
    return;
  }
}

/*****************************************************************************
  FUNCTION : kr_symbolCheck 

  PURPOSE  : spell checker  (check identifiers for matching [A-Za-z]^[|, ]*
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
bool    kr_symbolCheck(char *symbol)
{
  register  char  c;


  KernelErrorCode = KRERR_SYMBOL;

  if (!isalpha( *symbol ))
    /*	Symbol pattern invalid (must match [A-Za-z]^[|, ]*)  */
    return( FALSE );

  while ( (c = *(++symbol)) != '\0' )
    {
    if (!isgraph( c ))
      /*  Symbol pattern invalid (must match [A-Za-z]^[|, ]*)  */
      return( FALSE );

    if ( c == '|' || c == ',')
      /*  Symbol pattern invalid (must match [A-Za-z]^[|, ]*)  */
      return( FALSE );
  }

  KernelErrorCode = KRERR_NO_ERROR;
  return( TRUE );
}

/*****************************************************************************
  FUNCTION : kr_getUnitPtr

  PURPOSE  : 
  NOTES    :

  RETURNS  : returns the pointer to the given unit, returns NULL if unit 
             doesn't exist
  UPDATE   : 
******************************************************************************/
struct Unit  *kr_getUnitPtr(int unit_no)
{
  struct Unit   *unit_ptr;

  KernelErrorCode = KRERR_NO_ERROR;

  if ((unit_no != 0) &&
      (unit_no >= MinUnitNo) && (unit_no <= MaxUnitNo) &&
      UNIT_IN_USE( unit_ptr = unit_array + unit_no ))
    return( unit_ptr );

  /*  invalid unit no.  */
  KernelErrorCode = KRERR_UNIT_NO;
  return( NULL );
}

/*****************************************************************************
  FUNCTION : kr_getUnitValues

  PURPOSE  : 
  NOTES    :

  RETURNS  : Returns the value of the specified unit component
  UPDATE   : 
******************************************************************************/
FlintType  kr_getUnitValues(int unit_no, int component_selector)
{
  struct Unit   *unit_ptr;

  unit_ptr = kr_getUnitPtr( unit_no );
  if (KernelErrorCode != KRERR_NO_ERROR)
    return( (FlintType) 0);    /*  invalid unit no.  */

  switch (component_selector)
    {
    case SEL_UNIT_ACT:
      return( (FlintType) unit_ptr->act );
    case SEL_UNIT_OUT:
      return( (FlintType) unit_ptr->Out.output );
    case SEL_UNIT_IACT:
      return( (FlintType) unit_ptr->i_act );
    case SEL_UNIT_BIAS:
      return( (FlintType) unit_ptr->bias );
    case SEL_UNIT_VALA: 
      return( (FlintType) unit_ptr->value_a );
    default:
      KernelErrorCode = KRERR_PARAMETERS;
      return ((FlintType) 0);    /*  invalid selector */
  }
}
/*****************************************************************************
  FUNCTION : kr_setUnitValues

  PURPOSE  : Sets the value of the specified unit component
  NOTES    :

  RETURNS  : Returns the errorcode
  UPDATE   : 
******************************************************************************/
krui_err  kr_setUnitValues(int unit_no, int component_selector, 
			   FlintTypeParam value)
{
  struct Unit   *unit_ptr;

  unit_ptr = kr_getUnitPtr( unit_no );
  if (KernelErrorCode != KRERR_NO_ERROR)
    return( KernelErrorCode );	  /*  invalid unit no.	*/

  switch (component_selector)
    {
    case SEL_UNIT_ACT:
      unit_ptr->act = (FlintType) value;
      break;
    case SEL_UNIT_OUT:
      unit_ptr->Out.output = (FlintType) value;
      break;
    case SEL_UNIT_IACT:
      unit_ptr->i_act = (FlintType) value;
      break;
    case SEL_UNIT_BIAS:
      unit_ptr->bias = (FlintType) value;
      break;
    case SEL_UNIT_VALA: 
      unit_ptr->value_a = (FlintType) value;
      break;
    default:
      KernelErrorCode = KRERR_PARAMETERS;
      break;   /*  invalid selector */
  }

  return( KernelErrorCode );
}
/*****************************************************************************
  FUNCTION : kr_setAllUnitValues

  PURPOSE  : Sets all unit components of the specified unit
  NOTES    :

  RETURNS  : Returns the errorcode
  UPDATE   : 
******************************************************************************/
krui_err  kr_setAllUnitValues(int unit_no, FlintTypeParam out, 
			      FlintTypeParam act, FlintTypeParam i_act, 
			      FlintTypeParam bias)
{
  struct Unit   *unit_ptr;

  unit_ptr = kr_getUnitPtr( unit_no );
  if (KernelErrorCode != KRERR_NO_ERROR)
    return( KernelErrorCode );

  unit_ptr->Out.output = (FlintType) out;
  unit_ptr->act = (FlintType) act;
  unit_ptr->i_act = (FlintType) i_act;
  unit_ptr->bias = (FlintType) bias;

  return( KernelErrorCode );
}

/*****************************************************************************
  FUNCTION : kr_makeDefaultUnit

  PURPOSE  : Creates a unit with default values
  NOTES    :

  RETURNS  : 
  UPDATE   : Thomas Gern, 07.09.95 -> actbuf is initialized
******************************************************************************/
int  kr_makeDefaultUnit(void)
{
  struct Unit  *unit_ptr;
  FunctionPtr  func_ptr;
  int  unit_no;
  int  i;

  if ((unit_no = krm_getUnit()) == 0)
    return( KernelErrorCode );
  unit_no = abs(unit_no);

  if (KernelErrorCode != KRERR_NO_ERROR)
    return( KernelErrorCode );

  (void) kr_setAllUnitValues( unit_no, (FlintTypeParam) DEF_OUT, DefaultIAct,
			      DefaultIAct, DefaultBias );

  unit_ptr = unit_array + unit_no;

  unit_ptr->Ftype_entry = NULL;
  unit_ptr->value_a = (FlintType) 0;          /*previous bias change*/
  unit_ptr->value_b = (FlintType) 0;          /*previous bias slope*/
  unit_ptr->value_c = (FlintType) 0;          /*actual bias slope*/

  for (i = 0; i < MAX_BPTT_BACKSTEP; i++)
    unit_ptr->actbuf[i] = 0.0;


  if (DefaultUFuncAct == NULL)  {
    if (!krf_funcSearch( krf_getCurrentNetworkFunc( ACT_FUNC ), 
                         ACT_FUNC, &func_ptr))
      return( KernelErrorCode );

    DefaultUFuncAct = (ActFuncPtr) func_ptr;

    if (!krf_funcSearch( krf_getCurrentNetworkFunc( ACT_FUNC ), 
                         ACT_DERIV_FUNC, &func_ptr))
      return( KernelErrorCode );

    DefaultUFuncActDeriv = (ActDerivFuncPtr) func_ptr;
    
    if (!krf_funcSearch( krf_getCurrentNetworkFunc( ACT_FUNC ), 
                         ACT_2_DERIV_FUNC, &func_ptr))
      return( KernelErrorCode );

    DefaultUFuncAct2Deriv = (ActDerivFuncPtr) func_ptr;
    
    if (!krf_funcSearch( krf_getCurrentNetworkFunc( OUT_FUNC ), 
                         OUT_FUNC, &func_ptr))
      return( KernelErrorCode );

    DefaultUFuncOut = (OutFuncPtr) func_ptr;
  }

  unit_ptr->out_func = DefaultUFuncOut;       /*  default output function  */
  unit_ptr->act_func = DefaultUFuncAct;       /*  default activation function */
  unit_ptr->act_deriv_func = DefaultUFuncActDeriv; /* def. derivation actfunc */
  unit_ptr->act_2_deriv_func = DefaultUFuncAct2Deriv; 	/*  default derivation act. function */
  unit_ptr->python_out_func = DefaultUPythonFuncOut;       /*  default output function  */
  unit_ptr->python_act_func = DefaultUPythonFuncAct;       /*  default activation function */
  unit_ptr->python_act_deriv_func = DefaultUPythonFuncActDeriv; /* def. derivation actfunc */
  unit_ptr->python_act_2_deriv_func = DefaultUPythonFuncAct2Deriv; 	/*  default derivation act. function */
  unit_ptr->unit_name= NULL;			/*  default is no unit name */
  unit_ptr->subnet_no  = DefaultSubnetNo;
  unit_ptr->layer_no   = DefaultLayerNo;
  unit_ptr->unit_pos.x = DefaultPosX;
  unit_ptr->unit_pos.y = DefaultPosY;

  unit_ptr->unit_pos.z = DefaultPosZ;

  /*  set unit flags  */
  unit_ptr->flags = UFLAG_INITIALIZED | DefaultSType;

  /*  count units  */
  kr_countUnits( unit_ptr, UNIT_ADD );

  return( unit_no );
}

/*****************************************************************************
  FUNCTION : kr_createUnit

  PURPOSE  : Creates a user defined unit
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_createUnit(char *unit_name, char *out_func_name, char *act_func_name, 
		   FlintTypeParam i_act, FlintTypeParam bias)
{
  FunctionPtr   out_func_ptr, act_func_ptr, act_deriv_func_ptr,
                act_2_deriv_func_ptr;
  char  *str_ptr;
  int   unit_no;
  struct Unit  *unit_ptr;



  if (!kr_symbolCheck( unit_name ))
    return( KernelErrorCode );	/*  Symbol pattern invalid 
				    (must match [A-Za-z]^[|, ]*)  */

  if ( !krf_funcSearch( out_func_name, OUT_FUNC, &out_func_ptr ) )
    return( KernelErrorCode );
  if ( !krf_funcSearch( act_func_name, ACT_FUNC, &act_func_ptr ) )
    return( KernelErrorCode );

  /*  set the derivation function of the activation function  */ 
  if ( !krf_funcSearch( act_func_name, ACT_DERIV_FUNC, &act_deriv_func_ptr ))
    return( KernelErrorCode );

  /*  set the second derivation function of the activation function  */ 
  if ( !krf_funcSearch( act_func_name, ACT_2_DERIV_FUNC, &act_2_deriv_func_ptr ))
    return( KernelErrorCode );

  if ( (str_ptr = krm_NTableInsertSymbol( unit_name, UNIT_SYM ) ) == NULL)
    return( KernelErrorCode );

  unit_no = kr_makeDefaultUnit();
  if (KernelErrorCode != KRERR_NO_ERROR)
    return( KernelErrorCode );

  (void) kr_setAllUnitValues( unit_no, (FlintTypeParam) DEF_OUT,
			      i_act, i_act, bias );

  unit_ptr = unit_array + unit_no;

  unit_ptr->out_func  = (OutFuncPtr) out_func_ptr;
  if(unit_ptr->out_func == OUT_Custom_Python) {
  	unit_ptr->python_out_func =
		kr_findPythonFunction(out_func_name, OUT_FUNC);
  }		
  unit_ptr->act_func  = (ActFuncPtr) act_func_ptr;
  unit_ptr->act_deriv_func = (ActDerivFuncPtr) act_deriv_func_ptr;
  unit_ptr->act_2_deriv_func = (ActDerivFuncPtr) act_2_deriv_func_ptr;
  if(unit_ptr->act_func == ACT_Custom_Python) {
  	unit_ptr->python_act_func =
		kr_findPythonFunction(act_func_name, ACT_FUNC);
  	unit_ptr->python_act_deriv_func =
		kr_findPythonFunction(act_func_name, ACT_DERIV_FUNC);
  	unit_ptr->python_act_2_deriv_func =
		kr_findPythonFunction(act_func_name, ACT_DERIV_FUNC);
  }
  unit_ptr->unit_name = str_ptr;

  NetModified = TRUE;
  return( unit_no );
}

/*****************************************************************************
  FUNCTION : kr_unitSetTType

  PURPOSE  : Sets the topologic type of the unit
  NOTES    :

  RETURNS  : Returns the errorcode
  UPDATE   : 
******************************************************************************/
krui_err  kr_unitSetTType(int unit_no, int UnitTType)
{
  struct  Unit	*unit_ptr;
  int  intflags;

  if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
    return( KernelErrorCode );

  intflags = kr_TType2Flags( UnitTType );
  if (KernelErrorCode != KRERR_NO_ERROR)
    return( KernelErrorCode );

  if (((FlagWord) intflags == UFLAG_TTYP_SPEC_X) ||
      ((FlagWord) intflags == UFLAG_TTYP_N_SPEC_X)) {
      if ((FlagWord) intflags == UFLAG_TTYP_SPEC_X) {
	  /*  the topologic type of the unit will change  */
	  NetModified = TRUE;
	  /*  count units  */
	  kr_countUnits( unit_ptr, UNIT_DELETE );
	  
	  /*  change topologic type of the unit,  add special Flag  */
	  unit_ptr->flags |= (FlagWord)  UFLAG_TTYP_SPEC;
	  
	  /*  count units  */
	  kr_countUnits( unit_ptr, UNIT_ADD );
      }else{
	  if((unit_ptr->flags & UFLAG_TTYP_PAT)!= UFLAG_TTYP_SPEC){ 
	      /*  the topologic type of the unit will change  */
	      NetModified = TRUE;
	      /*  count units  */
	      kr_countUnits( unit_ptr, UNIT_DELETE );

	      /*  change topologic type of the unit,  delete special Flag  */
	      unit_ptr->flags &= (FlagWord)  ~UFLAG_TTYP_SPEC;
	  
	      /*  count units  */
	      kr_countUnits( unit_ptr, UNIT_ADD );
	  }
      }
  }else{
      if ((unit_ptr->flags & UFLAG_TTYP_PAT) != (FlagWord) intflags) {  
	  /*  the topologic type of the unit will change  */
	  NetModified = TRUE;
	  /*  count units  */
	  kr_countUnits( unit_ptr, UNIT_DELETE );
	  
	  /*  change topologic type of the unit  */
	  unit_ptr->flags &= ~UFLAG_TTYP_PAT;
	  unit_ptr->flags |= (FlagWord)  intflags;
	  
	  /*  count units  */
	  kr_countUnits( unit_ptr, UNIT_ADD );
      }
  }

  return( KernelErrorCode );
}



/*****************************************************************************
  FUNCTION : kr_setSite

  PURPOSE  : initialize the first/next site or the named site at the current 
             unit for access
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_setSite(int mode, char *site_name)
{
  struct SiteTable  *stbl_ptr;

  if (unitPtr == NULL)  {
    KernelErrorCode = KRERR_UNIT_NO;
    return( KernelErrorCode );
  }

  KernelErrorCode = KRERR_NO_ERROR;

  switch (mode)  {
    case  FIRST:
      prevSitePtr = NULL;

      if UNIT_HAS_SITES( unitPtr )
	{  /*  Unit has sites  */
	sitePtr = unitPtr->sites;
	return( TRUE );
      }
      else  {
	sitePtr = NULL;
	return( FALSE );
      }

    case  NEXT:
      if ((sitePtr == NULL) || (sitePtr->next == NULL))  return( FALSE );

      prevSitePtr = sitePtr;
      sitePtr	  = sitePtr->next;
      return( TRUE );

    case  NAME:
      if (!UNIT_HAS_SITES( unitPtr ))
	{  /*  Current unit doesn't have sites  */
	KernelErrorCode = KRERR_NO_SITES;
	return( KernelErrorCode );
      }

      if ((stbl_ptr = krm_STableSymbolSearch( site_name )) == NULL)
	{  /*	site name isn't defined */
	KernelErrorCode = KRERR_UNDEF_SITE_NAME;
	return( KernelErrorCode );
      }

      for (sitePtr = unitPtr->sites, prevSitePtr = NULL;
	   sitePtr != NULL;
	   prevSitePtr = sitePtr, sitePtr = sitePtr->next)
	if (sitePtr->site_table == stbl_ptr)
	  return( KRERR_NO_ERROR );  /*  site was found  */

      sitePtr = prevSitePtr = NULL;

      /*  Current unit doesn't have a site with this name  */
      KernelErrorCode = KRERR_NO_SUCH_SITE;  
      return( KernelErrorCode );

    default:
      KernelErrorCode = KRERR_PARAMETERS;
      return( KernelErrorCode );
  }
}


/*****************************************************************************
  FUNCTION : kr_getUnit

  PURPOSE  : returns the number of the first/next/current unit of the unit array
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_getUnit(int mode)
{
  register struct Unit   *unit_ptr;


  if (NoOfUnits == 0)  return( 0 );

  switch (mode)
    {
    case  FIRST:
      unitNo = MinUnitNo;
      unitPtr = unit_array + MinUnitNo;

      if UNIT_HAS_SITES( unitPtr )
	{  /*  Initialize current site pointer to the first available site */
	prevSitePtr = NULL;
	sitePtr = unitPtr->sites;
      }
      else
	{  /*  No sites available  */
	prevSitePtr = NULL;
	sitePtr     = NULL;
      }

      return( unitNo );

    case  NEXT:
      unit_ptr = unitPtr;
      if ((unit_ptr - unit_array) >= MaxUnitNo)  return( 0 );

      while (!UNIT_IN_USE( ++unit_ptr )) ;

      unitNo = unit_ptr - unit_array;
      unitPtr = unit_ptr;

      if UNIT_HAS_SITES( unit_ptr )
	{  /*  Initialize current site pointer to the first available site */
	prevSitePtr = NULL;
	sitePtr = unit_ptr->sites;
      }
      else
	{  /*  No sites available  */
	prevSitePtr = NULL;
	sitePtr     = NULL;
      }

      return( unitNo );

    case  CURRENT:
      return( unitNo );

    default:
      KernelErrorCode = KRERR_PARAMETERS;
      return( 0 );
  }
}

/*****************************************************************************
  FUNCTION : kr_setCurrUnit

  PURPOSE  : initializes the given unit for access
  NOTES    :

  RETURNS  : Returns the errorcode 
  UPDATE   : 
******************************************************************************/
krui_err  kr_setCurrUnit(int unit_no)
{
  struct Unit   *unit_ptr;


  if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
    return( KernelErrorCode );

  unitNo = unit_no;
  unitPtr = unit_ptr;

  if UNIT_HAS_SITES( unit_ptr )
    {  /*  Initialize current site pointer to the first available site */
    prevSitePtr = NULL;
    sitePtr = unit_ptr->sites;
  }
  else
    {  /*  No sites available  */
    prevSitePtr = NULL;
    sitePtr	= NULL;
  }

  return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : kr_getPredecessorUnit

  PURPOSE  : Returns the no. of first, next or current predecessor unit of the
             current unit/site and the connection weight
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_getPredecessorUnit(int mode, FlintType *weight, float* val_a, float* val_b, float* val_c)
{
  static struct Link  *link_ptr = NULL;


  if (unitPtr == NULL)
    {  /*  no current unit  */
    KernelErrorCode = KRERR_NO_CURRENT_UNIT;
    return( 0 );
  }

  switch (mode)
    {
    case  FIRST:  /*  first predecessor link wanted  */
      if UNIT_HAS_SITES( unitPtr )
	{
	if (sitePtr == NULL)
	  /*  site not initialized  */
	  link_ptr = unitPtr->sites->links;
	else
	  link_ptr = sitePtr->links;
      }
      else
	link_ptr = (struct Link *) unitPtr->sites;

      linkPtr = link_ptr;
      prevLinkPtr = NULL;
      if (link_ptr == NULL)  return( 0 );  /*  No inputs   */

      *weight = link_ptr->weight;
      *val_a  = link_ptr->value_a;
      *val_b  = link_ptr->value_b;
      *val_c  = link_ptr->value_c;
      return( link_ptr->to - unit_array );  /*	Return unit number  */

    case  NEXT:
      if (link_ptr == NULL)
	 {  /*	no current link  */
	 KernelErrorCode = KRERR_NO_CURRENT_LINK;
	 return( 0 );
       }

      prevLinkPtr = link_ptr;
      if ((linkPtr = link_ptr = link_ptr->next) == NULL)
	{
	prevLinkPtr = NULL;
	return( 0 );  /*  no successor unit  */
      }


      *weight = link_ptr->weight;
      *val_a  = link_ptr->value_a;
      *val_b  = link_ptr->value_b;
      *val_c  = link_ptr->value_c;
      return( link_ptr->to - unit_array );  /*	Return unit number  */

    case  CURRENT:
      if (link_ptr == NULL)
	 {  /*	no current link  */
	 KernelErrorCode = KRERR_NO_CURRENT_LINK;
	 return( 0 );
       }

      *weight = link_ptr->weight;
      *val_a  = link_ptr->value_a;
      *val_b  = link_ptr->value_b;
      *val_c  = link_ptr->value_c;
      return( link_ptr->to - unit_array );  /*	Return unit number  */

    default:
      KernelErrorCode = KRERR_PARAMETERS;
      return( 0 );
  }
}

/*****************************************************************************
  FUNCTION : kr_searchOutputConnection

  PURPOSE  :
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
static int  kr_searchOutputConnection(struct Unit *start_unit_ptr, 
				      struct Unit *source_unit_ptr, 
				      FlintType *weight)
{
  register struct  Link  *link_ptr, *prev_link_ptr;
  register struct  Unit  *source_unit;
  register struct  Site  *site_ptr, *prev_site_ptr;
  register struct  Unit  *unit_ptr;


  source_unit = source_unit_ptr;

  if ((sitePtr != NULL))
    {  /*  current unit has sites, so search for another connection at the
	   other sites of the unit  */
    for (site_ptr = sitePtr->next, prev_site_ptr = sitePtr;
	 site_ptr != NULL;
	 prev_site_ptr = site_ptr, site_ptr = site_ptr->next)
      for (link_ptr = site_ptr->links, prev_link_ptr = NULL;
	   link_ptr != NULL;
	   prev_link_ptr = link_ptr, link_ptr = link_ptr->next)
	if (link_ptr->to == source_unit)
	  {
	  sitePtr = site_ptr;  /*  set current site  */
	  prevSitePtr = prev_site_ptr;	/*  set previous site  */
	  linkPtr = link_ptr;  /*  set current link  */
	  prevLinkPtr = prev_link_ptr;	/*  set previous link  */

	  *weight = link_ptr->weight;
	  return( unitNo );
        }

    start_unit_ptr++;  /*  no connection found at the current site,
                           so start search at the next units  */
  }

  for(unit_ptr = start_unit_ptr; unit_ptr <= unit_array + MaxUnitNo; unit_ptr++)
    if UNIT_IN_USE( unit_ptr )
       {
       if UNIT_HAS_DIRECT_INPUTS( unit_ptr )
	 {
	 for (link_ptr = (struct Link *) unit_ptr->sites, prev_link_ptr = NULL;
	      link_ptr != NULL;
	      prev_link_ptr = link_ptr, link_ptr = link_ptr->next)
	   if (link_ptr->to == source_unit)
	     {
	     unitPtr = unit_ptr;  /*  set current unit pointer	*/
	     unitNo = unit_ptr - unit_array;  /*  set current unit no.	*/
	     sitePtr = prevSitePtr = NULL;  /*	no current site  */
	     linkPtr = link_ptr;  /*  set current link	*/
	     prevLinkPtr = prev_link_ptr;  /*  set previous link  */

	     *weight = link_ptr->weight;
	     return( unitNo );
	   }
       }
       else
	 if UNIT_HAS_SITES( unit_ptr )
	   {
	   for (site_ptr = unit_ptr->sites, prev_site_ptr = NULL;
		site_ptr != NULL;
		prev_site_ptr = site_ptr, site_ptr = site_ptr->next)
	     for (link_ptr = site_ptr->links, prev_link_ptr = NULL;
		  link_ptr != NULL;
		  prev_link_ptr = link_ptr, link_ptr = link_ptr->next)
	       if (link_ptr->to == source_unit)
		 {
		 unitPtr = unit_ptr;  /*  set current unit pointer  */
		 unitNo = unit_ptr - unit_array;  /*  set current unit no.  */
		 sitePtr = site_ptr;  /*  set current site  */
		 prevSitePtr = prev_site_ptr;  /*  set previous site  */
		 linkPtr = link_ptr;  /*  set current link  */
		 prevLinkPtr = prev_link_ptr;  /*  set previous link  */

		 *weight = link_ptr->weight;
		 return( unitNo );
	       }
	 }
    }

  /*  no successor unit found  */
  unitPtr = NULL; unitNo = 0;  /*  no current unit  */
  sitePtr = prevSitePtr = NULL;  /*  no current site  */
  linkPtr = prevLinkPtr = NULL;  /*  no current link  */

  return( 0 );
}


/*****************************************************************************
  FUNCTION : kr_getSuccessorUnit

  PURPOSE  : Returns the no. of first or next succecessor unit of the
             given unit and the connection strenght.
             Sets the current unit/site.
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_getSuccessorUnit(int mode, int source_unit_no, FlintType *weight)
{
  static struct Unit  *source_unit_ptr,
		      *current_unit_ptr = NULL;
  static struct Site  *current_site_ptr = NULL;
  int  unit_no;


  switch (mode)
    {
    case  FIRST:  /*  first successor link wanted  */
      if ((source_unit_ptr = kr_getUnitPtr( source_unit_no )) == NULL)
	return( KernelErrorCode );

      sitePtr = NULL;  /*  no current Site  */
      unit_no = kr_searchOutputConnection( unit_array + MinUnitNo,
					   source_unit_ptr, weight );
      current_unit_ptr = unitPtr;
      current_site_ptr = sitePtr;

      return( unit_no );

    case  NEXT:  /*  next successor link wanted  */
      if (current_unit_ptr == NULL)
	{  /*  no current unit	*/
	KernelErrorCode = KRERR_NO_CURRENT_UNIT;
	return( 0 );
      }

      sitePtr = current_site_ptr;

      unit_no = kr_searchOutputConnection( current_unit_ptr + 1,
					   source_unit_ptr, weight );
      current_unit_ptr = unitPtr;
      current_site_ptr = sitePtr;

      return( unit_no );

    default:
      KernelErrorCode = KRERR_PARAMETERS;
      return( 0 );
  }
}



/*****************************************************************************
  FUNCTION : kr_areConnected

  PURPOSE  : True if there exists a connection between source unit 
             <source_unit_no> and target unit <target_unit_no>, otherwise false.
	     If there exist a  connection between these units, kr_areConnected 
	     returns the connection strength also.
  NOTES    : This function is slow (Units are backward chained only)
  IMPORTANT: If there exist a connection, the current unit and site will be
	     set to the target unit/site.

  RETURNS  : Returns FALSE if unit doesn't exist.
  UPDATE   : 
******************************************************************************/
bool  kr_areConnected(int source_unit_no, int target_unit_no, FlintType *weight)
{
  register struct  Link  *link_ptr, *prev_link_ptr;
  register struct  Unit  *source_unit_ptr;
  register struct  Site  *site_ptr, *prev_site_ptr;
  struct  Unit	*target_unit_ptr;


  if ( (source_unit_ptr = kr_getUnitPtr( source_unit_no ) ) == NULL)
    return( FALSE );    /*  invalid unit #  */
  if ( (target_unit_ptr = kr_getUnitPtr( target_unit_no ) ) == NULL)
    return( FALSE );    /*  invalid unit #  */

  if UNIT_HAS_DIRECT_INPUTS( target_unit_ptr )
    {
    for(link_ptr = (struct Link *) target_unit_ptr->sites, prev_link_ptr = NULL;
	 link_ptr != NULL;
	 prev_link_ptr = link_ptr, link_ptr = link_ptr->next)
      if (link_ptr->to == source_unit_ptr)
	{  /*  connection found  */
	unitPtr = target_unit_ptr;  /*	set current unit pointer  */
	unitNo = target_unit_no;  /*  set current unit no.  */
	sitePtr = prevSitePtr = NULL;  /*  no current site  */
	linkPtr = link_ptr;  /*  set current link  */
	prevLinkPtr = prev_link_ptr;  /*  set previous link  */

	*weight = link_ptr->weight;
	return( TRUE );
      }
  }
  else
    if UNIT_HAS_SITES( target_unit_ptr )
      for (site_ptr = target_unit_ptr->sites, prev_site_ptr = NULL;
	   site_ptr != NULL;
	   prev_site_ptr = site_ptr, site_ptr = site_ptr->next)
	for (link_ptr = site_ptr->links, prev_link_ptr = NULL;
	     link_ptr != NULL;
	     prev_link_ptr = link_ptr, link_ptr = link_ptr->next)
	  if (link_ptr->to == source_unit_ptr)
	    {  /*  connection found  */
	    unitPtr = target_unit_ptr;	/*  set current unit pointer  */
	    unitNo = target_unit_no;  /*  set current unit no.	*/
	    sitePtr = site_ptr;  /*  set current site  */
	    prevSitePtr = prev_site_ptr;  /*  set previous site  */
	    linkPtr = link_ptr;  /*  set current link  */
	    prevLinkPtr = prev_link_ptr;  /*  set previous link  */

	    *weight = link_ptr->weight;
	    return( TRUE );
	  }

  /*  no successor unit found  */
  unitPtr = NULL; unitNo = 0;  /*  no current unit  */
  sitePtr = prevSitePtr = NULL;  /*  no current site  */
  linkPtr = prevLinkPtr = NULL;  /*  no current link  */

  return( FALSE );
}

/*****************************************************************************
  FUNCTION : kr_isConnected

  PURPOSE  : 
  NOTES    : If there exists a connection between the two units, the current 
             link is set to the link between the two units.
  RETURNS  : True if there exists a connection between source unit 
             <source_unit_no> and the current unit/site, otherwise false.
  UPDATE   : 
******************************************************************************/
bool  kr_isConnected(int source_unit_no, FlintType *weight)
{
  register struct  Link  *link_ptr, *prev_link_ptr;
  register struct  Unit  *source_unit_ptr;
  struct  Link	*start_link_ptr;


  if (unitPtr == NULL)
    {  /*  no current unit  */
    KernelErrorCode = KRERR_NO_CURRENT_UNIT;
    return( FALSE );
  }
  if ((source_unit_ptr = kr_getUnitPtr( source_unit_no ) ) == NULL)
    return( FALSE );  /*  invalid unit #  */

  if UNIT_HAS_DIRECT_INPUTS( unitPtr )
    start_link_ptr = (struct Link *) unitPtr->sites;
  else
    if UNIT_HAS_SITES( unitPtr )
      start_link_ptr = sitePtr->links;
    else
      return( FALSE );

  for (link_ptr = start_link_ptr, prev_link_ptr = NULL;
       link_ptr != NULL;
       prev_link_ptr = link_ptr, link_ptr = link_ptr->next)
    if (link_ptr->to == source_unit_ptr)
      {  /*  connection found  */
      linkPtr = link_ptr;  /*  set current link  */
      prevLinkPtr = prev_link_ptr;  /*	set previous link  */

      *weight = link_ptr->weight;
      return( TRUE );
    }

  /*  no successor unit found  */
  linkPtr = prevLinkPtr = NULL;  /*  no current link  */

  return( FALSE );
}


/*****************************************************************************
  FUNCTION : kr_getLinkWeight

  PURPOSE  : 
  NOTES    : 

  RETURNS  : Returns the link weight of the current link
  UPDATE   : 
******************************************************************************/
FlintType  kr_getLinkWeight(void)
{
  if (linkPtr != NULL)	return( linkPtr->weight );

  KernelErrorCode = KRERR_NO_CURRENT_LINK;
  return( (FlintType) 0 );
}


/*****************************************************************************
  FUNCTION : kr_setLinkWeight

  PURPOSE  : Sets the link weight of the current link
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void  kr_setLinkWeight(FlintTypeParam weight)
{
  if (linkPtr != NULL)
    {
    linkPtr->weight = weight;
    return;
  }

  KernelErrorCode = KRERR_NO_CURRENT_LINK;
}


/*****************************************************************************
  FUNCTION : kr_createLink 

  PURPOSE  : Creates a link between source unit and the current unit/site 
  NOTES    : kr_createLink DO NOT set the current link
             If you want to create a link and its unknown if there exists 
	     already a connection between the two units, use krui_createLink 
	     and test the return code, instead of the sequence kr_isConnected 
	     and kr_createLink
  RETURNS  : Returns an error code:
             - if memory allocation fails
             - if source unit doesn't exist or
             - if there exists already a connection between current unit/site 
	     and the source unit
             0 otherwise.    
  UPDATE   : 
******************************************************************************/
krui_err  kr_createLink(int source_unit_no, FlintTypeParam weight)
{
  register struct Link	*link_ptr;
  register struct Unit	*source_unit_ptr;


  KernelErrorCode = KRERR_NO_ERROR;

  if (unitPtr == NULL)
    {  /*  no current unit  */
    KernelErrorCode = KRERR_NO_CURRENT_UNIT;
    return( KernelErrorCode );
  }

  if ((source_unit_ptr = kr_getUnitPtr( source_unit_no ) ) == NULL)
    return( KernelErrorCode );	/*  invalid unit #  */

  switch ((int) (unitPtr->flags & UFLAG_INPUT_PAT))
    {
    case  UFLAG_NO_INP:  /*  current unit doesn't have inputs  */
      if ((link_ptr = krm_getLink()) == NULL)
	return( KernelErrorCode );

      link_ptr->to     = source_unit_ptr;
      link_ptr->weight = (FlintType) weight;
      link_ptr->next   = NULL;

      unitPtr->sites = (struct Site *) link_ptr;
      unitPtr->flags |= UFLAG_DLINKS;  /*  unit has direkt inputs now  */

      break;

    case  UFLAG_DLINKS:  /*  current unit has direct inputs  */
      FOR_ALL_LINKS( unitPtr, link_ptr )
	if (link_ptr->to == source_unit_ptr)
	  {  /*  there exists already a connection  */
	  KernelErrorCode = KRERR_ALREADY_CONNECTED;
	  return( KRERR_ALREADY_CONNECTED );
	}

      if ((link_ptr = krm_getLink()) == NULL)
	return( KernelErrorCode );

      link_ptr->to     = source_unit_ptr;
      link_ptr->weight = (FlintType) weight;
      link_ptr->next   = (struct Link *) unitPtr->sites;
      unitPtr->sites   = (struct Site *) link_ptr;

      break;

    case  UFLAG_SITES:	/*  current unit has sites  */
      FOR_ALL_LINKS_AT_SITE( sitePtr, link_ptr )
	if (link_ptr->to == source_unit_ptr)
	  {  /*  there exists already a connection  */
	  KernelErrorCode = KRERR_ALREADY_CONNECTED;
	  return( KRERR_ALREADY_CONNECTED );
	}

      if ((link_ptr = krm_getLink()) == NULL)
	return( KernelErrorCode );

      link_ptr->to     = source_unit_ptr;
      link_ptr->weight = (FlintType) weight;
      link_ptr->next   = (struct Link *) sitePtr->links;
      sitePtr->links   = link_ptr;

      break;

    default:
      KernelErrorCode = KRERR_PARAMETERS;
      return( KernelErrorCode );
  }

  NetModified = TRUE;
  return( KRERR_NO_ERROR );
}

/*****************************************************************************
  FUNCTION : kr_createLinkWithAdditionalParameters

  PURPOSE  : Creates a link between source unit and the current unit/site 
             and sets the values for value_a, value_b and value_c.
  NOTES    : this version returns pointer to the new link and no error value.
             This means that you have to use the routine as follows :
               NewLink=kr_createLinkWithAdditionalParameters(...);
               if (KernelErrorCode!=KRERR_NO_ERROR) return(KernelErroCode);
             
             See notes of kr_createLink, too

  UPDATE   : 13.05.96 <Juergen Gatter>
******************************************************************************/
 struct Link*  kr_createLinkWithAdditionalParameters
                      (int source_unit_no, FlintTypeParam weight,
                       float val_a,float val_b,float val_c)
{
  register struct Link	*link_ptr=NULL;
  register struct Unit	*source_unit_ptr;


  KernelErrorCode = KRERR_NO_ERROR;

  if (unitPtr == NULL)
    {  /*  no current unit  */
    KernelErrorCode = KRERR_NO_CURRENT_UNIT;
    return ( link_ptr );
  }

  if ((source_unit_ptr = kr_getUnitPtr( source_unit_no ) ) == NULL)
    return ( link_ptr );	/*  invalid unit #  */

  switch ((int) (unitPtr->flags & UFLAG_INPUT_PAT))
    {
    case  UFLAG_NO_INP:  /*  current unit doesn't have inputs  */
      if ((link_ptr = krm_getLink()) == NULL)
	return ( link_ptr );

      link_ptr->to     = source_unit_ptr;
      link_ptr->weight = (FlintType) weight;
      link_ptr->next   = NULL;
      link_ptr->value_a = val_a;
      link_ptr->value_b = val_b;
      link_ptr->value_c = val_c;

      unitPtr->sites = (struct Site *) link_ptr;
      unitPtr->flags |= UFLAG_DLINKS;  /*  unit has direkt inputs now  */

      break;

    case  UFLAG_DLINKS:  /*  current unit has direct inputs  */
      FOR_ALL_LINKS( unitPtr, link_ptr )
	if (link_ptr->to == source_unit_ptr)
	  {  /*  there exists already a connection  */
	  KernelErrorCode = KRERR_ALREADY_CONNECTED;
	  return( link_ptr );
	}

      if ((link_ptr = krm_getLink()) == NULL)
	return( link_ptr );

      link_ptr->to     = source_unit_ptr;
      link_ptr->weight = (FlintType) weight;
      link_ptr->next   = (struct Link *) unitPtr->sites;
      link_ptr->value_a = val_a;
      link_ptr->value_b = val_b;
      link_ptr->value_c = val_c;
      unitPtr->sites   = (struct Site *) link_ptr;

      break;

    case  UFLAG_SITES:	/*  current unit has sites  */
      FOR_ALL_LINKS_AT_SITE( sitePtr, link_ptr )
	if (link_ptr->to == source_unit_ptr)
	  {  /*  there exists already a connection  */
	  KernelErrorCode = KRERR_ALREADY_CONNECTED;
	  return( link_ptr );
	}

      if ((link_ptr = krm_getLink()) == NULL)
	return( link_ptr );

      link_ptr->to     = source_unit_ptr;
      link_ptr->weight = (FlintType) weight;
      link_ptr->next   = (struct Link *) sitePtr->links;
      link_ptr->value_a = val_a;
      link_ptr->value_b = val_b;
      link_ptr->value_c = val_c;
      sitePtr->links   = link_ptr;

      break;

    default:
      KernelErrorCode = KRERR_PARAMETERS;
      return( link_ptr );
  }

  NetModified = TRUE;
  return( link_ptr );
}


/*****************************************************************************
  FUNCTION : kr_deleteLink 

  PURPOSE  : Deletes the current link
  NOTES    : To delete a link between the current unit/site and the source unit
             <source_unit_no>, call krui_isConnected( source_unit_no ) and
             krui_deleteLink()

  RETURNS  : Returns the errorcode
  UPDATE   : 
******************************************************************************/
krui_err  kr_deleteLink(void)
{
  register struct Link	 *next_link_ptr;


  if (linkPtr == NULL)
    {  /*  no current link  */
    KernelErrorCode = KRERR_NO_CURRENT_LINK;
    return( KernelErrorCode );
  }

  if (unitPtr == NULL)
    {  /*  no current unit  */
    KernelErrorCode = KRERR_NO_CURRENT_UNIT;
    return( KernelErrorCode );
  }

  KernelErrorCode = KRERR_NO_ERROR;
  switch ((int) (unitPtr->flags & UFLAG_INPUT_PAT))
    {
    case  UFLAG_NO_INP:  /*  current unit doesn't have inputs  */
      KernelErrorCode = KRERR_UNIT_NO_INPUTS;
      return( KernelErrorCode );

    case  UFLAG_DLINKS:  /*  current unit has direct inputs  */
      next_link_ptr = linkPtr->next;
      krm_releaseLink( linkPtr );
      linkPtr = next_link_ptr;

      if (prevLinkPtr != NULL)	/*  current link isn't first link at the unit */
	prevLinkPtr->next = next_link_ptr;  /*	chain previous link pointer
						with next link pointer	*/
      else
	{  /*  current link is the first link at the unit  */
	unitPtr->sites = (struct Site *) next_link_ptr;
	if (next_link_ptr == NULL)
	  unitPtr->flags &= (~UFLAG_INPUT_PAT);  /* last input deleted: 
						    the unit has no inputs now*/
      }

      NetModified = TRUE;
      return( KRERR_NO_ERROR );

    case  UFLAG_SITES:	/*  current unit has sites  */
      next_link_ptr = linkPtr->next;
      krm_releaseLink( linkPtr );
      linkPtr = next_link_ptr;

      if (prevLinkPtr != NULL)	/*  current link isn't first link at the unit */
	prevLinkPtr->next = next_link_ptr;  /*	chain previous link pointer
						with next link pointer	*/
      else  /*	current link is the first link at the unit  */
	sitePtr->links = next_link_ptr;

      NetModified = TRUE;
      return( KRERR_NO_ERROR );
   }

  KernelErrorCode = KRERR_PARAMETERS;
  return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : kr_deleteAllLinks

  PURPOSE  : Deletes all input links at current unit/site
  NOTES    : 

  RETURNS  : Returns the errorcode
  UPDATE   : 
******************************************************************************/
krui_err  kr_deleteAllLinks(int mode)
{
  if (unitPtr == NULL)
    {  /*  no current unit  */
    KernelErrorCode = KRERR_NO_CURRENT_UNIT;
    return( KernelErrorCode );
  }

  linkPtr = NULL;
  NetModified = TRUE;

  switch (mode)
    {
    case  INPUTS:  /*  delete all inputs  */
      if UNIT_HAS_DIRECT_INPUTS( unitPtr )
	{
	krm_releaseAllLinks( (struct Link *) unitPtr->sites );
	unitPtr->sites = NULL;
	unitPtr->flags &= (~UFLAG_INPUT_PAT);  /*  unit don't has inputs now  */

	return( KernelErrorCode );
      }

      if UNIT_HAS_SITES( unitPtr )
	{
	krm_releaseAllLinks( sitePtr->links );
	sitePtr->links = NULL;	/*  site has no inputs now   */

	return( KernelErrorCode );
      }

      return( KernelErrorCode );

    case  OUTPUTS:  /*	delete all outputs  */
      kr_deleteAllOutputLinks( unitPtr );

      return( KernelErrorCode );
  }

  KernelErrorCode = KRERR_PARAMETERS;
  return( KernelErrorCode );
}








/*#################################################

GROUP: Low-Level Kernel Functions

#################################################*/


/*****************************************************************************
  FUNCTION : kr_deleteAllInputs

  PURPOSE  : delete all links and sites at the given unit
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void    kr_deleteAllInputs(struct Unit *unit_ptr)
{
  register struct Site	*site_ptr;


  if (UNIT_HAS_SITES( unit_ptr ))
    {   /*  Unit has sites  */
    FOR_ALL_SITES( unit_ptr, site_ptr )
      /*  Release all links   */
      krm_releaseAllLinks( site_ptr->links );

    krm_releaseAllSites( unit_ptr->sites );
  }
  else
    {   /*  Unit don't has sites   */
    if (UNIT_HAS_DIRECT_INPUTS( unit_ptr ))
      krm_releaseAllLinks( (struct Link *) unit_ptr->sites );
  }

  unit_ptr->sites = NULL;

  /*  The unit has no inputs now  */
  unit_ptr->flags &= (~UFLAG_INPUT_PAT);
}

/*****************************************************************************
  FUNCTION : kr_deleteAllOutputLinks

  PURPOSE  : Deletes all output links at <source_unit>
  NOTES    : This function is slow

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void  kr_deleteAllOutputLinks(struct Unit *source_unit_ptr)
{
  register struct Link   *link_ptr,
                         *pred_link_ptr;
  register struct Site   *site_ptr;
  register struct Unit   *unit_ptr;


  FOR_ALL_UNITS( unit_ptr )
    if UNIT_IN_USE( unit_ptr )
      if UNIT_HAS_SITES( unit_ptr )
        {  /*  unit has sites  */
	FOR_ALL_SITES( unit_ptr, site_ptr )
          for (link_ptr = site_ptr->links, pred_link_ptr = NULL;
               link_ptr != NULL;
               pred_link_ptr = link_ptr, link_ptr = link_ptr->next)

            if (link_ptr->to == source_unit_ptr)
              {     /*  Connection between unit and source_unit found   */
              if (pred_link_ptr == NULL)
                site_ptr->links = link_ptr->next;
              else
                pred_link_ptr->next = link_ptr->next;

              krm_releaseLink( link_ptr );

              break;    /*  next site/unit  */
	    }
      }
      else  /*	unit has no sites   */
	if UNIT_HAS_DIRECT_INPUTS( unit_ptr )
          for (link_ptr = (struct Link *) unit_ptr->sites, pred_link_ptr = NULL;
               link_ptr != NULL;
               pred_link_ptr = link_ptr, link_ptr = link_ptr->next)
            if (link_ptr->to == source_unit_ptr)
              {     /*  Connection between unit and source_unit found   */
              if (pred_link_ptr == NULL)
                {
                unit_ptr->sites = (struct Site *) link_ptr->next;
                if (link_ptr->next == NULL)
                  /*  The unit has no inputs now  */
                  unit_ptr->flags &= (~UFLAG_INPUT_PAT);
	      }
              else
                pred_link_ptr->next = link_ptr->next;

              krm_releaseLink( link_ptr );

              break;    /*  next unit  */
	    }
}


/*****************************************************************************
  FUNCTION : kr_copyOutputLinks

  PURPOSE  : Copies all output links at <source_unit> to <new_unit>.
  NOTES    : This function is slow

  RETURNS  : Returns error code if memory allocation fails.
  UPDATE   : 
******************************************************************************/
static krui_err  kr_copyOutputLinks(struct Unit *source_unit_ptr, 
				    struct Unit *new_unit_ptr)
{
  register struct Link   *link_ptr,
			 *new_link;
  register struct Site   *site_ptr;
  register struct Unit   *unit_ptr;


  KernelErrorCode = KRERR_NO_ERROR;

  FOR_ALL_UNITS( unit_ptr )
    if UNIT_IN_USE( unit_ptr )
      if UNIT_HAS_DIRECT_INPUTS( unit_ptr )
	FOR_ALL_LINKS( unit_ptr, link_ptr )
	  if (link_ptr->to == source_unit_ptr)
	    {  /*  Connection between unit and source_unit found   */
	    if ( (new_link = krm_getLink() ) == NULL)
	      return( KernelErrorCode );

	    memcpy( (char *) new_link, (char *) link_ptr, LINK_SIZE );
	    new_link->next = (struct Link *) unit_ptr->sites;
	    unit_ptr->sites = (struct Site *) new_link;

	    new_link->to = new_unit_ptr;
	    new_link->weight = link_ptr->weight;
	    break;    /*  next unit  */
	  }
      else
	if UNIT_HAS_SITES( unit_ptr )
	  FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
	    if (link_ptr->to == source_unit_ptr)
	      {  /*  Connection between unit and source_unit found   */
	      if ( (new_link = krm_getLink() ) == NULL)
		return( KernelErrorCode );

	      new_link->next = site_ptr->links;
	      site_ptr->links = new_link;

	      new_link->to = new_unit_ptr;
	      new_link->weight = link_ptr->weight;

	      break;	/*  next site/unit  */
	    }
  return( KernelErrorCode );
}

/*****************************************************************************
  FUNCTION : kr_copyInputLinks

  PURPOSE  : Copy all input links from <source_unit> to <new_unit>
  NOTES    : 

  RETURNS  : Returns error code 
  UPDATE   : 
******************************************************************************/
static krui_err  kr_copyInputLinks(struct Unit *source_unit_ptr, 
				   struct Unit *new_unit_ptr)
{
  register struct Link	 *link_ptr, *new_link,
			 *last_link_ptr;
  register struct Site	 *source_site_ptr, *dest_site_ptr;


  KernelErrorCode = KRERR_NO_ERROR;

  if UNIT_HAS_DIRECT_INPUTS( source_unit_ptr )
    {
    last_link_ptr = new_link = NULL;
    FOR_ALL_LINKS( source_unit_ptr, link_ptr )
      {
      if ((new_link = krm_getLink()) == NULL)
	{
	new_unit_ptr->sites = (struct Site *) last_link_ptr;
	return( KernelErrorCode );
      }

      memcpy( (char *) new_link, (char *) link_ptr, LINK_SIZE );
      new_link->next = last_link_ptr;
      last_link_ptr = new_link;
    }

    new_unit_ptr->sites = (struct Site *) new_link;
    new_unit_ptr->flags &= ~UFLAG_INPUT_PAT;
    if (new_link != NULL)  new_unit_ptr->flags |= UFLAG_DLINKS;
  }
  else
    if UNIT_HAS_SITES( source_unit_ptr )
      FOR_ALL_SITES( source_unit_ptr, source_site_ptr )
	FOR_ALL_SITES( new_unit_ptr, dest_site_ptr )
	  if (source_site_ptr->site_table == dest_site_ptr->site_table)
	    {
	    last_link_ptr = new_link = NULL;
	    FOR_ALL_LINKS_AT_SITE( source_site_ptr, link_ptr )
	      {
	      if ((new_link = krm_getLink()) == NULL)
		{
		dest_site_ptr->links = last_link_ptr;
		return( KernelErrorCode );
	      }

	      memcpy( (char *) new_link, (char *) link_ptr, LINK_SIZE );
	      new_link->next = last_link_ptr;
	      last_link_ptr = new_link;
	      }

	    dest_site_ptr->links = new_link;
	  }

  return( KernelErrorCode );
}




/*#################################################

GROUP: Site Name/Func functions

#################################################*/

/*****************************************************************************
  FUNCTION : kr_searchUnitSite

  PURPOSE  : search for a site at a unit
  NOTES    : 

  RETURNS  : Returns the site or NULL
  UPDATE   : 
******************************************************************************/
struct Site *kr_searchUnitSite(struct Unit *unit_ptr,struct SiteTable *stbl_ptr)
{
  register struct Site	*site_ptr;

  FOR_ALL_SITES( unit_ptr, site_ptr )
    if (site_ptr->site_table == stbl_ptr)
      return( site_ptr );

  return( NULL );   /*  there is no site at this unit with this name    */
}


/*****************************************************************************
  FUNCTION : kr_searchNetSite

  PURPOSE  : searches for a site in the network
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_searchNetSite(struct SiteTable *stbl_ptr)
{
  register struct Site   *site_ptr;
  register struct Unit   *unit_ptr;


  if (NoOfUnits == 0)
    return( 0 ); /*  no units -> no sites */

  FOR_ALL_UNITS( unit_ptr )
    if (UNIT_HAS_SITES( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
      {  /*  unit has sites and is in use  */
      FOR_ALL_SITES( unit_ptr, site_ptr )
        if (site_ptr->site_table == stbl_ptr)
          return( unit_ptr - unit_array );  /*  return unit no. */
    }

  return( 0 );  /*  site isn't in use   */
}


/*#################################################

GROUP: Link Functions

#################################################*/
/*****************************************************************************
  FUNCTION : kr_jogWeights

  PURPOSE  : Add random uniform distributed values to connection weights.
             <minus> must be less then <plus>.
             New:
             Value and range depends on the given parameters and the current
             weight. e.g.:
             -0.02, 0.04 means that the new weight will be in the range of
             100-2% to 100+4% = 98% to 104% of its previous value.

  NOTES    : The old way of just adding noise may be achieved by defining
             -DJOGWEIGHTS_BY_ADDING during compilation

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void  kr_jogWeights(FlintTypeParam minus, FlintTypeParam plus)
{
  register  struct Link   *link_ptr;
  FlagWord	flags;
  struct Unit   *unit_ptr;
  struct Site   *site_ptr;
  register FlintType  range, min;


  if (NoOfUnits == 0)  return;  /*  no. units  */
  range = plus - minus;
  min = minus;

  FOR_ALL_UNITS( unit_ptr )  {
    flags = unit_ptr->flags;

      if(((flags & UFLAG_IN_USE) == UFLAG_IN_USE) 
	  && !IS_SPECIAL_UNIT(unit_ptr))
      /*  unit is in use  */
      if (flags & UFLAG_DLINKS)
	/*  unit has direct links   */
	FOR_ALL_LINKS( unit_ptr, link_ptr )
#ifdef JOGWEIGHTS_BY_ADDING
          link_ptr->weight += (FlintType) drand48() * range + min;
#else
          link_ptr->weight += link_ptr->weight * ((FlintType) drand48() * range + min);
      
#endif
	  else
	if (flags & UFLAG_SITES)
	  /*  unit has sites  */
	  FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
#ifdef JOGWEIGHTS_BY_ADDING
            link_ptr->weight += (FlintType) drand48() * range + min;
#else
            link_ptr->weight += link_ptr->weight * ((FlintType) drand48() * range + min);
#endif
  }
}

/*****************************************************************************

  GROUP: correlation

******************************************************************************/


/*****************************************************************************
  FUNCTION : kr_getCorrelatedHiddens
  PURPOSE  : calculate correlation coefficients between all non-special
             hidden units, determine the two hidden units with maximum
	     correlation, return the pointers to these units and their
	     correlation coefficient.
	     If no such units exist return NULL pointers instead of unit 
	     pointers.

	     calculate correlation coefficients between all non-special
             hidden units of the current network over all pattern. 
             Store these values into the 
	     kr_CorrMatrix in the following way:

	     for all non-special hidden units c and r 
             (c, r = unit_array indexed from first hidden unit)
	     with r > c
	     kr_CorrMatrix[row=r][column=c] = 
	         correlation coefficient between these two units, based on 
		 unit output values for all patterns of the current pattern set.

	     Note that the following values are not set since they are obsolete:
	     kr_CorrMatrix[row=n][column=n] == 1
	     kr_CorrMatrix[row=c][column=r] == kr_CorrMatrix[row=r][column=c]

  NOTES    : mean in value_b, stddev in value_c
  RETURNS  : kernel error code
******************************************************************************/

static krui_err kr_getCorrelatedHiddens(struct Unit **hn1, struct Unit **hn2, 
					double *res_corr)
{
    static RbfFloatMatrix kr_CorrMatrix = {0, 0, NULL, NULL};

    int no_of_layers;

    struct Unit *unit_ptr;
    struct Unit *unit_ptr2;
    struct Unit *first_hidden;
    int pattern_no, sub_pat_no, no_of_patterns;
    int MyNoOfHidden;
    int col, row;
    double covadd;
    double corr;
    double mincorr, maxcorr;
    int mincorrcol, mincorrrow;
    int maxcorrcol, maxcorrrow;

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

    /* initialize value_b and value_c of each unit */
    MyNoOfHidden = 0;
    first_hidden = NULL;
    FOR_ALL_UNITS (unit_ptr) 
	if (IS_HIDDEN_UNIT (unit_ptr))
	{
	    MyNoOfHidden++;
	    if (!first_hidden)
		first_hidden = unit_ptr;
	    if (! IS_SPECIAL_UNIT (unit_ptr)) 
	    {
		unit_ptr->value_b = 0;
		unit_ptr->value_c = 0;
	    }
	}

    /* allocate space for new correlation matrix if necessary: */
    if (kr_CorrMatrix.rows < MyNoOfHidden || 
	kr_CorrMatrix.columns < MyNoOfHidden)
    {
	if (kr_CorrMatrix.field != NULL)
	    RbfFreeMatrix(&kr_CorrMatrix);

	if (!RbfAllocMatrix(MyNoOfHidden, MyNoOfHidden, &kr_CorrMatrix))
	{
	    KernelErrorCode = KRERR_INSUFFICIENT_MEM;
	    return KernelErrorCode;
	}
    }

    /* reset correlation matrix */
    RbfClearMatrix(&kr_CorrMatrix, 0.0);
	
    /* compute the necessary sub patterns (allways work on all patterns) */
    KernelErrorCode = 
	kr_initSubPatternOrder(0, kr_np_pattern(PATTERN_GET_NUMBER, 0, 0) - 1);
    if (KernelErrorCode != KRERR_NO_ERROR) 
    {
	if (KernelErrorCode == KRERR_NP_NO_TRAIN_SCHEME)
	    KernelErrorCode = KRERR_NP_WORKAROUND;
	return (KernelErrorCode);
    }

    /* calculate total number of subpatterns */
    no_of_patterns = 0;

    /* calculate the following values:
       sum(x)   -> value_b
       sum(x^2) -> value_c
       sum(x*y) -> matrix[col for x][row for y]

       x and y are output values of arbitraty hidden units
     */
    while (kr_getSubPatternByOrder (&pattern_no, &sub_pat_no)) {
        /* propagate pattern through net */
        propagateNetForward (pattern_no, sub_pat_no);
	no_of_patterns++;

        /* calculate mean for all units */
        FOR_ALL_UNITS (unit_ptr)
            if (! IS_SPECIAL_UNIT (unit_ptr) && IS_HIDDEN_UNIT (unit_ptr))
	    {
		/* sum up x and x^2 */
                unit_ptr->value_b += unit_ptr->Out.output;
		unit_ptr->value_c += unit_ptr->Out.output * unit_ptr->Out.output;

		col = unit_ptr - first_hidden;
		
		/* sum up x*y */
		FOR_ALL_UNITS (unit_ptr2)
		    if (! IS_SPECIAL_UNIT (unit_ptr2) 
			&& IS_HIDDEN_UNIT (unit_ptr2)
			&& unit_ptr2 > unit_ptr)
		    {
			row = unit_ptr2 - first_hidden;
			covadd = (unit_ptr->Out.output * unit_ptr2->Out.output);
			RbfMatrixSetValue(&kr_CorrMatrix, row, col,
			    RbfMatrixGetValue(&kr_CorrMatrix, row, col) + covadd);
		    }
	    }
    }

    /* now finish computation of correlation matrix. compute:

       matrix[col for x][row for y] = 
       (n * sum(x*y) - sum(x) * sum(y))   /
       sqrt((n * sum(x^2) - sum^2(x)) * (n * sum(y^2) - sum^2(y)))

       also find minimum and maximum correlation (hint: 0 means not correlated, 
       1.0 means correlated -1.0 means anti-correlated
    */
    mincorr = maxcorr = 0.0;
    FOR_ALL_UNITS (unit_ptr)
        if (! IS_SPECIAL_UNIT (unit_ptr) && IS_HIDDEN_UNIT (unit_ptr)) 
	{
	    col = unit_ptr - first_hidden;
	    
	    FOR_ALL_UNITS (unit_ptr2)
		if (! IS_SPECIAL_UNIT (unit_ptr2) 
		    && IS_HIDDEN_UNIT (unit_ptr2)
		    && unit_ptr2 > unit_ptr)
		{
		    row = unit_ptr2 - first_hidden;
		    corr = no_of_patterns * RbfMatrixGetValue(&kr_CorrMatrix, row, col)
			- unit_ptr->value_b * unit_ptr2->value_b;
		    corr /= sqrt(
			(no_of_patterns * unit_ptr->value_c 
			 - unit_ptr->value_b * unit_ptr->value_b)
			*(no_of_patterns * unit_ptr2->value_c 
			  - unit_ptr2->value_b * unit_ptr2->value_b)
			);
		    RbfMatrixSetValue(&kr_CorrMatrix, row, col, corr);

		    if (corr > maxcorr)
		    {
			maxcorr = corr;
			maxcorrcol = col;
			maxcorrrow = row;
		    }
		    if (corr < mincorr)
		    {
			mincorr = corr;
			mincorrcol = col;
			mincorrrow = row;
		    }
		}
	}
    
#ifdef CORR_DEBUG
    RbfPrintMatrix(&kr_CorrMatrix, stderr);
#endif

    if (-mincorr > maxcorr && -mincorr > 0.0)
    {
	*res_corr = -mincorr;
	*hn1 = first_hidden + mincorrcol;
	*hn2 = first_hidden + mincorrrow;
    }
    else if (maxcorr > -mincorr && maxcorr > 0.0)
    {
	*res_corr = maxcorr;
	*hn1 = first_hidden + maxcorrcol;
	*hn2 = first_hidden + maxcorrrow;
    }
    else
    {
	*res_corr = 0.0;
	*hn1 = *hn2 = NULL;
    }
	
    return (KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : kr_jogCorrWeights

  PURPOSE  : Add uniform distributed random values to connection weights of
             highly correlated, non-special hidden units.
             <minus> must be less then <plus>.
	     The two hidden units with maximum positive or negative correlation 
	     with an absolute value higher then mincorr are searched. The 
	     incoming weights of one of these units are jogged.

             New:
             Value and range depends on the given parameters and the current
             weight. e.g.:
             -0.02, 0.04 means that the new weight will be in the range of
             100-2% to 100+4% = 98% to 104% of its previous value.

  NOTES    : The old way of just adding noise may be achieved by defining
             -DJOGWEIGHTS_BY_ADDING during compilation

  RETURNS  : error code
  UPDATE   : 
******************************************************************************/
krui_err  kr_jogCorrWeights(FlintTypeParam minus, FlintTypeParam plus, 
			    FlintTypeParam mincorr)
{
    register  struct Link   *link_ptr;
    FlagWord	flags;
    struct Unit   *unit_ptr = NULL;
    struct Unit   *unit_ptr1 = NULL;
    struct Unit   *unit_ptr2 = NULL;
    double         correlation;
    struct Site   *site_ptr;
    register FlintType  range, min;
    double maxweight;

    if (NoOfUnits == 0)  return KRERR_NO_UNITS;  /*  no. units  */
    range = plus - minus;
    min = minus;

    KernelErrorCode = KRERR_NO_ERROR;
    if (kr_getCorrelatedHiddens(&unit_ptr1, &unit_ptr2, &correlation) 
	!= KRERR_NO_ERROR)
	return KernelErrorCode;

    if (unit_ptr1 == NULL || unit_ptr2 == NULL || fabs(correlation) < mincorr)
	return KRERR_NO_ERROR;

    unit_ptr = drand48() > 0.5 ? unit_ptr2 : unit_ptr1;

#ifdef CORR_DEBUG
    printf("maximum correlation is %g between %s and %s, jogging %s\n", 
	   correlation,
	   unit_ptr1->unit_name == NULL ? "noname" : unit_ptr1->unit_name,
	   unit_ptr2->unit_name == NULL ? "noname" : unit_ptr2->unit_name,
	   unit_ptr->unit_name == NULL ? "noname" : unit_ptr->unit_name);
#endif

    flags = unit_ptr->flags;

    if(((flags & UFLAG_IN_USE) == UFLAG_IN_USE) 
       && !IS_SPECIAL_UNIT(unit_ptr))
	/*  unit is in use  */
	if (flags & UFLAG_DLINKS)
	{
	    /*  unit has direct links   */
	    maxweight = 0.0;
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
	    {
		if (fabs(link_ptr->weight) > maxweight)
		    maxweight = fabs(link_ptr->weight);
	    }
	    if (maxweight > 1.0)
		maxweight = 1.0;
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
                link_ptr->weight += 
		    maxweight * ((FlintType) drand48() * range + min);
	}
	else
	{
	    if (flags & UFLAG_SITES)
		/*  unit has sites  */
		FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
#ifdef JOGWEIGHTS_BY_ADDING
		    link_ptr->weight += (FlintType) drand48() * range + min;
#else
                    link_ptr->weight += 
			link_ptr->weight * ((FlintType) drand48() * range + min);
#endif
	}

    return KRERR_NO_ERROR;		    
}



/*#################################################

GROUP: Site Functions

#################################################*/
/*****************************************************************************
  FUNCTION : kr_createDefaultSite

  PURPOSE  : Creates a new site with default initialisation
  NOTES    : 

  RETURNS  : the new site 
  UPDATE   : 
******************************************************************************/
struct Site  *kr_createDefaultSite(void)
{
  struct Site   *site_ptr;


  if ( (site_ptr = krm_getSite() ) == NULL)   return( NULL );

  site_ptr->links = NULL;
  site_ptr->next  = NULL;

  return( site_ptr );
}



/*#################################################

GROUP: Unit Functions

#################################################*/


/*****************************************************************************
  FUNCTION : kr_unitNameSearch

  PURPOSE  : Searches for a unit with the given symbol pointer.
  NOTES    : 

  RETURNS  : Returns the first unit no. if a unit with the given name was found,
             0 otherwise
 
  UPDATE   : 
******************************************************************************/
int  kr_unitNameSearch(int min_unit_no, char *unit_symbol_ptr)
{
  register char   *symbol;
  register struct Unit   *unit_ptr;


  if ((symbol = unit_symbol_ptr) == NULL)
    return( 0 );

  /*  search for symbol pointer  */
  for (unit_ptr = unit_array + min_unit_no; unit_ptr <= unit_array + MaxUnitNo; unit_ptr++)
    if UNIT_IN_USE( unit_ptr )
      if (unit_ptr->unit_name == symbol)
        return( unit_ptr - unit_array );

  return( 0 );
}

/*****************************************************************************
  FUNCTION : kr_copyUnitFrame

  PURPOSE  : copy the source unit with sites, but no links
  NOTES    : 

  RETURNS  : Returns the error code 
  UPDATE   : 
******************************************************************************/
static krui_err kr_copyUnitFrame(struct Unit *source_unit_ptr, 
				 struct Unit *new_unit_ptr)
{
  struct Site	*site_ptr,
		*new_site_ptr,
		*last_site_ptr;


  KernelErrorCode = KRERR_NO_ERROR;

  memcpy( (char *) new_unit_ptr, (char *) source_unit_ptr, UNIT_SIZE );

  if (source_unit_ptr->unit_name != NULL)
    (void) krm_NTableInsertSymbol( source_unit_ptr->unit_name, UNIT_SYM );

  /*  unit has no inputs now  */
  new_unit_ptr->flags &= ~UFLAG_INPUT_PAT;
  new_unit_ptr->sites = NULL;

  if UNIT_HAS_SITES( source_unit_ptr )
    {  /*  Copy all sites, but no links.  */
    last_site_ptr = new_site_ptr = NULL;
    FOR_ALL_SITES( source_unit_ptr, site_ptr )  {
      if ((new_site_ptr = krm_getSite()) == NULL)  {
	new_unit_ptr->sites = last_site_ptr;
	return( KernelErrorCode );
      }

      memcpy( (char *) new_site_ptr, (char *) site_ptr, SITE_SIZE );
      new_site_ptr->links = NULL;
      new_site_ptr->next = last_site_ptr;
      last_site_ptr = new_site_ptr;
    }

    new_unit_ptr->sites = new_site_ptr;
    if (new_site_ptr != NULL)  new_unit_ptr->flags |= UFLAG_SITES;
  }

  return( KernelErrorCode );
}

/*****************************************************************************
  FUNCTION : kr_removeUnit

  PURPOSE  : Remove unit and all links from network
  NOTES    : 

  RETURNS  : Returns the error code 
  UPDATE   : 
******************************************************************************/
krui_err  kr_removeUnit(struct Unit *unit_ptr)
{

  /*  delete inputs   */
  kr_deleteAllInputs( unit_ptr );
  /*  delete output links */
  kr_deleteAllOutputLinks( unit_ptr );
  /*  check references to the unit symbol   */
  krm_NTableReleaseSymbol( unit_ptr->unit_name, UNIT_SYM );
  /*  count units  */
  kr_countUnits( unit_ptr, UNIT_DELETE );
  /*  delete Unit */
  krm_releaseUnit( unit_ptr - unit_array );

  return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : kr_copyUnit

  PURPOSE  : Copy a given unit, according to the copy mode
             1. copy unit (with it sites, if available) and input/output links
             2. copy unit (with it sites, if available) and input links
             3. copy unit (with it sites, if available) and output links
             4. copy unit (with it sites, if available) but no links
	     Function has no effect on the current unit.
  NOTES    : Copying of output links is slow.
             If return code < 0, an error occured.

  RETURNS  : Returns the unit number of the new unit or error message < 0 , 
             if errors occured.
  UPDATE   : 
******************************************************************************/
krui_err  kr_copyUnit(int copy_mode, int source_unit)
{
  struct Unit	*source_unit_ptr,
		*new_unit_ptr;
  int  new_unit_no;


  KernelErrorCode = KRERR_NO_ERROR;

  if ((source_unit_ptr = kr_getUnitPtr( source_unit )) == NULL)
    return( KernelErrorCode );
  if ((new_unit_no = krm_getUnit()) == 0)
    return( KernelErrorCode );
  if (new_unit_no != abs(new_unit_no)){
      /* new unit block allocated; need to update unit pointer */
      new_unit_no = abs(new_unit_no);
      source_unit_ptr = kr_getUnitPtr( source_unit );
  }

  new_unit_ptr = unit_array + new_unit_no;

  /*  copy unit (with it sites, if available) but no input/output links  */
  if (kr_copyUnitFrame( source_unit_ptr, new_unit_ptr ) != KRERR_NO_ERROR)
    return( KernelErrorCode );

  switch (copy_mode)
    {
    case ONLY_UNIT:
      break;

    case ONLY_INPUTS:
    /*	copy unit (with it sites, if available) and input links */
      (void) kr_copyInputLinks( source_unit_ptr, new_unit_ptr );
      break;

    case ONLY_OUTPUTS:
    /*  copy unit (with it sites, if available) and output links    */
      (void) kr_copyOutputLinks( source_unit_ptr, new_unit_ptr);
      break;

    case INPUTS_AND_OUTPUTS:
    /*  copy unit (with it sites, if available) and input/output links  */
      if (kr_copyOutputLinks( source_unit_ptr, new_unit_ptr) != KRERR_NO_ERROR)
	break;

      (void) kr_copyInputLinks( source_unit_ptr, new_unit_ptr );
      break;

    default:
      KernelErrorCode = KRERR_COPYMODE;
  }

  if (KernelErrorCode != KRERR_NO_ERROR)
    {
    kr_removeUnit( new_unit_ptr );  /*	delete Unit  */
    return( KernelErrorCode );
  }
  else
    {  /*  Successful copy   */
    new_unit_ptr->flags = source_unit_ptr->flags;  /*  copy flags  */
    /*  count units  */
    kr_countUnits( new_unit_ptr, UNIT_ADD );
    NetModified = TRUE;
    return( new_unit_no );
  }
}

/*#################################################

GROUP: Ftype Unit Functions

#################################################*/
/*****************************************************************************
  FUNCTION : kr_changeFtypeUnits

  PURPOSE  : changes all units in the network with the given functionality type
             to the new functions of the (new) functionality type
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void   kr_changeFtypeUnits(struct FtypeUnitStruct *Ftype_entry)
{
  register struct Unit   *unit_ptr;


  if (NoOfUnits == 0)  return;  /*  no units  */

  FOR_ALL_UNITS( unit_ptr )
    if UNIT_IN_USE( unit_ptr )
      {     /*  unit is in use  */
      if (unit_ptr->Ftype_entry == Ftype_entry)
	{  /*  unit with this type was found. Now change the transfer functions
	       of the unit to the modified functionality type */
        unit_ptr->act_func = Ftype_entry->act_func;
        unit_ptr->out_func = Ftype_entry->out_func;
        unit_ptr->act_deriv_func = Ftype_entry->act_deriv_func;
        unit_ptr->act_2_deriv_func = Ftype_entry->act_2_deriv_func;
        unit_ptr->python_act_func = Ftype_entry->python_act_func;
        unit_ptr->python_out_func = Ftype_entry->python_out_func;
        unit_ptr->python_act_deriv_func = Ftype_entry->python_act_deriv_func;
        unit_ptr->python_act_2_deriv_func = Ftype_entry->python_act_2_deriv_func;
        }
      }

  NetModified = TRUE;
}

/*****************************************************************************
  FUNCTION : kr_deleteUnitsFtype

  PURPOSE  : delete the functionality type of the units with the given type
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void  kr_deleteUnitsFtype(struct FtypeUnitStruct *ftype_ptr)
{
  register struct Unit   *unit_ptr;


  if (NoOfUnits == 0)  return;  /*  no units  */

  FOR_ALL_UNITS( unit_ptr )
    if UNIT_IN_USE( unit_ptr )
      /*  unit is in use  */
      if (unit_ptr->Ftype_entry == ftype_ptr)
        unit_ptr->Ftype_entry = NULL;
}

/*****************************************************************************
  FUNCTION : kr_makeFtypeUnit

  PURPOSE  : create a new unit with the given functionality type
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_makeFtypeUnit(char *Ftype_symbol)
{
  register struct Site	*ftype_site, *site_ptr;
  struct Unit  *unit_ptr;
  struct FtypeUnitStruct  *ftype_ptr;
  int  unit_no;


  KernelErrorCode = KRERR_NO_ERROR;

  if (!kr_symbolCheck( Ftype_symbol ))
    return( KernelErrorCode );

  if ((ftype_ptr = krm_FtypeSymbolSearch( Ftype_symbol ) ) == NULL)
    {  /*  Ftype name isn't defined    */
    KernelErrorCode = KRERR_FTYPE_SYMBOL;
    return( KernelErrorCode );
  }

  unit_no = kr_makeDefaultUnit();
  if (KernelErrorCode != KRERR_NO_ERROR)
    return( KernelErrorCode );

  unit_ptr = unit_array + unit_no;

  unit_ptr->Ftype_entry = ftype_ptr;
  unit_ptr->out_func    = ftype_ptr->out_func;
  unit_ptr->act_func    = ftype_ptr->act_func;
  unit_ptr->act_deriv_func = ftype_ptr->act_deriv_func;
  unit_ptr->act_2_deriv_func = ftype_ptr->act_2_deriv_func;
  unit_ptr->python_out_func    = ftype_ptr->python_out_func;
  unit_ptr->python_act_func    = ftype_ptr->python_act_func;
  unit_ptr->python_act_deriv_func = ftype_ptr->python_act_deriv_func;
  unit_ptr->python_act_2_deriv_func = ftype_ptr->python_act_2_deriv_func;

  ftype_site = ftype_ptr->sites;

  /*  make sites  */
  while (ftype_site != NULL)
    {   /*  Ftype has sites */
    if ((site_ptr = krm_getSite()) == NULL)
      {  /*  memory alloc failed */
      krm_releaseAllSites( unit_ptr->sites );
      unit_ptr->sites = NULL;
      KernelErrorCode = KRERR_INSUFFICIENT_MEM;
      return( KernelErrorCode );
    }

    site_ptr->next = unit_ptr->sites;
    unit_ptr->sites = site_ptr;

    site_ptr->site_table = ftype_site->site_table;
    ftype_site = ftype_site->next;
  }

  if (ftype_ptr->sites != NULL)
    unit_ptr->flags |= UFLAG_SITES;     /*  unit has now sites  */

  return( unit_no );
}

/*****************************************************************************
  FUNCTION : kr_FtypeSiteSearch

  PURPOSE  : 
  NOTES    : 

  RETURNS  : returns TRUE, if there exists the given site at the given ftype 
             entry
  UPDATE   : 
******************************************************************************/
bool  kr_FtypeSiteSearch(struct Site *ftype_first_site, 
			 struct SiteTable *site_table_ptr)
{
  register struct  Site      *site_ptr;


  for (site_ptr = ftype_first_site; site_ptr != NULL; site_ptr = site_ptr->next)
    if (site_ptr->site_table == site_table_ptr)
      return( TRUE );

  return( FALSE );
}

/*****************************************************************************
  FUNCTION : kr_changeFtypeUnit

  PURPOSE  : change the properties of the given unit to the properties of the
             given F-Type 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void    kr_changeFtypeUnit(struct Unit *unit_ptr, 
			   struct FtypeUnitStruct *ftype_ptr)
{
  FlagWord	flags;
  struct  Site  *site_ptr,
                *pred_site_ptr,
                *tmp_ptr,
                *ftype_site;


  unit_ptr->out_func = ftype_ptr->out_func;
  unit_ptr->act_func = ftype_ptr->act_func;
  unit_ptr->act_deriv_func = ftype_ptr->act_deriv_func;
  unit_ptr->act_2_deriv_func = ftype_ptr->act_2_deriv_func;
  unit_ptr->python_out_func = ftype_ptr->python_out_func;
  unit_ptr->python_act_func = ftype_ptr->python_act_func;
  unit_ptr->python_act_deriv_func = ftype_ptr->python_act_deriv_func;
  unit_ptr->python_act_2_deriv_func = ftype_ptr->python_act_2_deriv_func;


  flags = unit_ptr->flags & UFLAG_INPUT_PAT;

  switch (flags)
    {
    case  UFLAG_NO_INP:
      /*  Unit has no inputs  */
      if (ftype_ptr->sites != NULL)
        /*    Ftype has sites, delete unit's Ftype  */
        unit_ptr->Ftype_entry = NULL;
      else
        /*    Ftype and unit don't have sites */
        unit_ptr->Ftype_entry = ftype_ptr;    /* unit accept Ftype and inputs */

      return;     /*  done !  */

    case  UFLAG_SITES:
      /*  Unit has sites  */
      ftype_site = ftype_ptr->sites;
      if (ftype_site == NULL)
        {  /* unit has sites, but Ftype hasn't sites, 
	      delete unit's Ftype and all inputs  */
        unit_ptr->Ftype_entry = NULL;

        kr_deleteAllInputs( unit_ptr );
        unit_ptr->flags = UFLAG_INITIALIZED;  /*  unit has no inputs now !    */
        }
      else
        {     /*  both unit and Ftype have sites: check sites  */
        unit_ptr->Ftype_entry = ftype_ptr;

        site_ptr = unit_ptr->sites;
        pred_site_ptr = NULL;

        do
          {
          if ( ! kr_FtypeSiteSearch( ftype_site, site_ptr->site_table ))
            {  /*  Ftype and unit site definitions are not equivalent: 
		   remove site    */
            if (pred_site_ptr == NULL)
              {   /*  this is the first site at the unit  */
              unit_ptr->sites = site_ptr->next;

              if (site_ptr->next == NULL)
                /*  unit don't has any inputs   */
                unit_ptr->flags &= (~UFLAG_INPUT_PAT);
              }
            else
              {   /*  this site isn't the first site at the unit  */
              pred_site_ptr->next = site_ptr->next;
              pred_site_ptr = site_ptr;
              }

	    /*  work with temporary pointer and get */
            tmp_ptr = site_ptr;

	    /*  next site pointer BEFORE krm_releaseSite    */
            site_ptr = site_ptr->next;

	    /*  (important in a multiprocessor system   */
            krm_releaseAllLinks( tmp_ptr->links );    
            krm_releaseSite( tmp_ptr );
            /*    delete unit's Ftype */
            unit_ptr->Ftype_entry = NULL;
            }
          else
            {
            pred_site_ptr = site_ptr;
            site_ptr = site_ptr->next;
            }
          }
        while (site_ptr != NULL);

        if (unit_ptr->sites == NULL)
          unit_ptr->flags = UFLAG_INITIALIZED;  /*  unit has no inputs now !  */
        }

      return;


    case  UFLAG_DLINKS:
    /*  Unit has direct links   */
      if (ftype_ptr->sites != NULL)
        { /*  unit has direct links, but Ftype entry has sites: delete links  */
        unit_ptr->Ftype_entry = NULL;

        kr_deleteAllInputs( unit_ptr );
        unit_ptr->flags = UFLAG_INITIALIZED;  /*  unit has no inputs now !    */
        }
      else
        { /* unit has direct links and Ftype has no sites: use direct links  */
        unit_ptr->Ftype_entry = ftype_ptr;
        }

    }
}

/*****************************************************************************
  FUNCTION : kr_changeFtypeSites

  PURPOSE  : change a site at the F-Type
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void    kr_changeFtypeSites(struct FtypeUnitStruct *Ftype_entry, 
			    struct SiteTable *old_site_table, 
			    struct SiteTable *new_site_table)
{
  struct Unit   *unit_ptr;
  struct Site   *site_ptr;


  if (NoOfUnits == 0)  return;  /*  no units  */

  FOR_ALL_UNITS( unit_ptr )
    if UNIT_IN_USE( unit_ptr )
      {     /*  unit is in use  */
      if (unit_ptr->Ftype_entry == Ftype_entry)
        {
	FOR_ALL_SITES( unit_ptr, site_ptr )
          if (site_ptr->site_table == old_site_table)
            site_ptr->site_table = new_site_table;
      }
    }

  NetModified = TRUE;
}


/*#################################################

GROUP: Miscellanous

#################################################*/
/*****************************************************************************
  FUNCTION : kr_flags2TType

  PURPOSE  : translate unit flags to the topological type of the unit
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_flags2TType(int flags)
{
  KernelErrorCode = KRERR_NO_ERROR;

  switch (flags)
    {
    case UFLAG_TTYP_UNKN:  return( UNKNOWN );
    case UFLAG_TTYP_IN  :  return( INPUT );
    case UFLAG_TTYP_OUT :  return( OUTPUT );
    case UFLAG_TTYP_DUAL:  return( DUAL );
    case UFLAG_TTYP_HIDD:  return( HIDDEN );
    case UFLAG_TTYP_SPEC:  return( SPECIAL );
    case UFLAG_TTYP_SPEC_I: return (SPECIAL_I) ;
    case UFLAG_TTYP_SPEC_O: return (SPECIAL_O) ;
    case UFLAG_TTYP_SPEC_H: return (SPECIAL_H) ;
    case UFLAG_TTYP_SPEC_D: return (SPECIAL_D) ;
      /* case UFLAG_TTYP_SPEC_X and 
	 case UFLAG_TTYP_N_SPEC_X are no true TType*/
    
    default: KernelErrorCode = KRERR_TTYPE;
	     return( UNKNOWN );
  }
}

/*****************************************************************************
  FUNCTION : kr_TType2Flags

  PURPOSE  : translate the topological type to unit flags
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_TType2Flags(int ttype)
{
  KernelErrorCode = KRERR_NO_ERROR;

  switch (ttype)
    {
    case UNKNOWN:  return( UFLAG_TTYP_UNKN );
    case INPUT	:  return( UFLAG_TTYP_IN );
    case OUTPUT :  return( UFLAG_TTYP_OUT );
    case DUAL	:  return( UFLAG_TTYP_DUAL );
    case HIDDEN :  return( UFLAG_TTYP_HIDD );
    case SPECIAL:  return( UFLAG_TTYP_SPEC );
    case SPECIAL_I: return (UFLAG_TTYP_SPEC_I) ;
    case SPECIAL_O: return (UFLAG_TTYP_SPEC_O) ;
    case SPECIAL_H: return (UFLAG_TTYP_SPEC_H) ;
    case SPECIAL_D: return (UFLAG_TTYP_SPEC_D) ;
    case SPECIAL_X: return (UFLAG_TTYP_SPEC_X) ;
    case N_SPECIAL_X: return (UFLAG_TTYP_N_SPEC_X) ;
    default:  KernelErrorCode = KRERR_TTYPE;
	      /*  return( KernelErrorCode );  */
	      return( -1 );
  }
}
/*****************************************************************************
  FUNCTION : kr_updateUnitOutputs

  PURPOSE  : update the outputs of all units in the network
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void    kr_updateUnitOutputs(void)
{
  register struct Unit   *unit_ptr;


  FOR_ALL_UNITS( unit_ptr )
    if ( (unit_ptr->flags & UFLAG_INITIALIZED) == UFLAG_INITIALIZED)
      {     /*  unit is in use and enabled  */
      if (unit_ptr->out_func == NULL)
        /*  Identity Function   */
        unit_ptr->Out.output = unit_ptr->act;
      else if(unit_ptr->out_func == OUT_Custom_Python)
      	unit_ptr->Out.output = kr_PythonOutFunction(unit_ptr->python_out_func,
						unit_ptr->act);
      else						
        unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
      }
}


/*****************************************************************************
  FUNCTION : kr_getNoOfUnits

  PURPOSE  : returns the no. of units of the specified topologic type
             (i.e. Input, Hidden, Output or Special units)
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_getNoOfUnits(int UnitTType)
{
  register struct Unit   *unit_ptr;
  register int   no_of_units;
  register FlagWord      ttyp_flg;
  int   flg;


  if ((NoOfUnits == 0) || ((flg = kr_TType2Flags( UnitTType )) == -1))
    return( 0 );  /*  no units or this topologic type doesn't exist  */

  ttyp_flg = (FlagWord) flg;
  no_of_units = 0;
  FOR_ALL_UNITS( unit_ptr )
    if ( ((unit_ptr->flags & UFLAG_TTYP_PAT) == ttyp_flg) &&
         UNIT_IN_USE( unit_ptr ) )
      no_of_units++;

  return( no_of_units );
}


/*****************************************************************************
  FUNCTION : kr_getNoOfSpecialUnits

  PURPOSE  : returns the no. of special units of the specified topologic type
             (i.e. Input, Hidden, Output or Special units)
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
int  kr_getNoOfSpecialUnits(int UnitTType)
{
  register struct Unit   *unit_ptr;
  register int   no_of_units;
  register FlagWord      ttyp_flg;
  int   flg;


  if ((NoOfUnits == 0) || ((flg = kr_TType2Flags( UnitTType )) == -1))
    return( 0 );  /*  no units or this topologic type doesn't exist  */

  ttyp_flg = (FlagWord) flg;
  no_of_units = 0;
  FOR_ALL_UNITS( unit_ptr )
    if ( ((unit_ptr->flags & UFLAG_TTYP_PAT) == (ttyp_flg | UFLAG_TTYP_SPEC)) &&
         UNIT_IN_USE( unit_ptr ) )
      no_of_units++;

  return( no_of_units );
}


/*****************************************************************************
  FUNCTION : kr_forceUnitGC

  PURPOSE  : force unit array garbage collection
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void  kr_forceUnitGC(void)
{
  krm_unitArrayGC();
}


/*#################################################

GROUP: Functions default presettings

#################################################*/

/*****************************************************************************
  FUNCTION : kr_getUnitDefaults

  PURPOSE  : 
  NOTES    : 

  RETURNS  : Returns information about the unit default settings. 
  UPDATE   : 
******************************************************************************/
void	kr_getUnitDefaults(FlintType *act, FlintType *bias, int *ttflags, 
			   int *subnet_no, int *layer_no, char **act_func, 
			   char **out_func)
{
  static char  activation_func[FUNCTION_NAME_MAX_LEN],
               output_func[FUNCTION_NAME_MAX_LEN];


  *act          = DefaultIAct;
  *bias         = DefaultBias;
  *ttflags	= (int) DefaultSType;
  *subnet_no    = DefaultSubnetNo;
  *layer_no     = DefaultLayerNo;

  strcpy( activation_func, krf_getCurrentNetworkFunc( ACT_FUNC ) );
  *act_func = activation_func;
  strcpy( output_func, krf_getCurrentNetworkFunc( OUT_FUNC ) );
  *out_func = output_func;
}

/*****************************************************************************
  FUNCTION : kr_setUnitDefaults

  PURPOSE  : Changes the unit default settings.
  NOTES    : 

  RETURNS  : Returns error code
  UPDATE   : 
******************************************************************************/
krui_err  kr_setUnitDefaults(FlintTypeParam act, FlintTypeParam bias, 
			     int ttflags, int subnet_no, int layer_no, 
			     char *act_func, char *out_func)
{
  FunctionPtr  act_func_ptr,
               act_deriv_func_ptr,
               act_2_deriv_func_ptr,
               out_func_ptr;


  KernelErrorCode = KRERR_NO_ERROR;


  if (!krf_funcSearch( act_func, ACT_FUNC, &act_func_ptr))
    return( KernelErrorCode );
  if (!krf_funcSearch( act_func, ACT_DERIV_FUNC, &act_deriv_func_ptr))
    return( KernelErrorCode );
  if (!krf_funcSearch( act_func, ACT_2_DERIV_FUNC, &act_2_deriv_func_ptr))
    return( KernelErrorCode );
  if (!krf_funcSearch( out_func, OUT_FUNC, &out_func_ptr))
    return( KernelErrorCode );

  if (krf_setCurrentNetworkFunc( act_func, ACT_FUNC ) != KRERR_NO_ERROR)
    return( KernelErrorCode );
  if (krf_setCurrentNetworkFunc( out_func, OUT_FUNC ) != KRERR_NO_ERROR)
    return( KernelErrorCode );

  DefaultIAct       = (FlintType) act;
  DefaultBias       = (FlintType) bias;
  DefaultSType	    = (FlagWord) ttflags;
  DefaultPosX       = DEF_POS_X;
  DefaultPosY       = DEF_POS_Y;

  DefaultPosZ       = DEF_POS_Z;


  DefaultSubnetNo   = subnet_no;
  DefaultLayerNo    = layer_no;

  DefaultUFuncOut   = (OutFuncPtr) out_func_ptr;
  DefaultUFuncAct   = (ActFuncPtr) act_func_ptr;
  DefaultUFuncActDeriv = (ActDerivFuncPtr) act_deriv_func_ptr;
  DefaultUFuncAct2Deriv = (ActDerivFuncPtr) act_2_deriv_func_ptr;
  if(DefaultUFuncOut == OUT_Custom_Python) {
	  DefaultUPythonFuncOut   = kr_findPythonFunction(out_func,OUT_FUNC);
  }  
  if(DefaultUFuncAct == ACT_Custom_Python) {	  
	  DefaultUPythonFuncAct   = kr_findPythonFunction(act_func,ACT_FUNC);
	  DefaultUPythonFuncActDeriv = kr_findPythonFunction(act_func,ACT_DERIV_FUNC);
	  DefaultUPythonFuncAct2Deriv = kr_findPythonFunction(act_func,ACT_2_DERIV_FUNC);
  }

  return( KernelErrorCode );
}


/*#################################################

GROUP: Topological Sorting Functions

#################################################*/

/*****************************************************************************
  FUNCTION : clr_T_flags

  PURPOSE  : Clears the 'touch' (refresh) flag of all units
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
static void  clr_T_flags(void)
{
  register struct Unit   *unit_ptr;


  FOR_ALL_UNITS( unit_ptr )
    if (UNIT_IN_USE( unit_ptr ))
      {
      unit_ptr->flags &= ~UFLAG_REFRESH;
      unit_ptr->lln = 0;
    }
}

/*****************************************************************************
  FUNCTION : DepthFirst1

  PURPOSE  : Depth search routine for topological sorting
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
static void  DepthFirst1(struct Unit *unit_ptr, int depth)
{
  struct Site   *site_ptr;
  struct Link   *link_ptr;


  if (unit_ptr->flags & UFLAG_REFRESH)
    {  /*  the 'touch' flag is set: don't continue search  */
    if (unit_ptr->lln == 0)
      {  /*  logical layer no. isn't set => Cycle found  */
      topo_msg.no_of_cycles++;
      if (topo_msg.error_code == KRERR_NO_ERROR)
	{  /*  remember the cycle unit	*/
        topo_msg.src_error_unit = unit_ptr - unit_array;
        topo_msg.error_code = KRERR_CYCLES;
      }
    }

    return;
  }
  else
    /*	set the 'touch' flag  */
    unit_ptr->flags |= UFLAG_REFRESH;

  switch (unit_ptr->flags & UFLAG_INPUT_PAT)
    {
    case  UFLAG_DLINKS:   /*  unit has direct links  */
      FOR_ALL_LINKS( unit_ptr, link_ptr )
	DepthFirst1( link_ptr->to, depth + 1 );  /*  increase depth  */

      break;

    case  UFLAG_SITES:	/*  unit has sites  */
	FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
	  DepthFirst1( link_ptr->to, depth + 1 );  /*  increase depth  */

      break;
  }

  /*  remember the depth (for cycle detection and statistics)  */
  unit_ptr->lln = depth;  
  *global_topo_ptr++ = unit_ptr;  /*  store sorted unit pointer  */
}

/*****************************************************************************
  FUNCTION : DepthFirst2

  PURPOSE  : Depth search routine for topology check function
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
static void  DepthFirst2(struct Unit *unit_ptr, int depth)
{
  struct Site   *site_ptr;
  struct Link   *link_ptr;


  if (unit_ptr->flags & UFLAG_REFRESH)
    {  /*  the 'touch' flag is set: don't continue search  */
    if (unit_ptr->lln == 0)
      {  /*  logical layer no. isn't set => Cycle found  */
      topo_msg.no_of_cycles++;
      if (topo_msg.error_code == KRERR_NO_ERROR)
	{  /*  remember the cycle unit	*/
        topo_msg.src_error_unit = unit_ptr - unit_array;
        topo_msg.error_code = KRERR_CYCLES;
      }
    }

    return;
  }
  else
    /*	set the 'touch' flag  */
    unit_ptr->flags |= UFLAG_REFRESH;

  switch (unit_ptr->flags & UFLAG_INPUT_PAT)
    {
    case  UFLAG_DLINKS:   /*  unit has direct links  */
      FOR_ALL_LINKS( unit_ptr, link_ptr )
	DepthFirst2( link_ptr->to, depth + 1 );  /*  increase depth  */

      break;

    case  UFLAG_SITES:	/*  unit has sites  */
      FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
	DepthFirst2( link_ptr->to, depth + 1 );  /*  increase depth  */

      break;
  }

  /*  remember the depth (for cycle detection and statistics)  */
  unit_ptr->lln = depth;

  /*  store highest layer no.  */
  if (depth > topo_msg.no_of_layers)  topo_msg.no_of_layers = depth;
}

/*****************************************************************************
  FUNCTION : DepthFirst3

  PURPOSE  : Depth search routine for topological sorting in feedforward networks
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
static void  DepthFirst3(struct Unit *unit_ptr, int depth)
{
  struct Site   *site_ptr;
  struct Link   *link_ptr;


  if (unit_ptr->flags & UFLAG_REFRESH)
    {  /*  the 'touch' flag is set: don't continue search  */
    topo_msg.src_error_unit = unit_ptr - unit_array; /*  store unit number  */

    if IS_OUTPUT_UNIT( unit_ptr )
      {  /*  this output unit has a output connection to another unit  */
      if (topo_msg.error_code == KRERR_NO_ERROR)
        topo_msg.error_code = KRERR_O_UNITS_CONNECT;
    }
    else
      if (unit_ptr->lln == 0)
        {  /*  logical layer no. isn't set => Cycle found  */
        topo_msg.no_of_cycles++;
        if (topo_msg.error_code == KRERR_NO_ERROR)
          topo_msg.error_code = KRERR_CYCLES;
      }

    return;
  }
  else
    /*	set the 'touch' flag  */
    unit_ptr->flags |= UFLAG_REFRESH;

  switch (unit_ptr->flags & UFLAG_INPUT_PAT)
    {
    case  UFLAG_DLINKS:   /*  unit has direct links  */
      FOR_ALL_LINKS( unit_ptr, link_ptr )
	DepthFirst3( link_ptr->to, depth + 1 );  /*  increase depth  */

      break;

    case  UFLAG_SITES:	/*  unit has sites  */
      FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
	DepthFirst3( link_ptr->to, depth + 1 );  /*  increase depth  */

      break;
  }

  /*  remember the depth (for cycle detection and statistics)  */
  unit_ptr->lln = depth;

  /*  store only hidden units  */
  if IS_HIDDEN_UNIT( unit_ptr )
    *global_topo_ptr++ = unit_ptr;  /*	store sorted unit pointer  */
}


/*****************************************************************************
  FUNCTION : kr_topoSortT

  PURPOSE  : Sort units topological (general version) and stores the
             pointers to this units in the topologic array
  NOTES    : Units are not sorted by their topologic type (that's not possible 
             in the general case)

  RETURNS  : error code
  UPDATE   : 
******************************************************************************/
static krui_err  kr_topoSortT(void)
{
  register struct Unit	 *unit_ptr;
  int	io_units;


  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */
  clr_T_flags();    /*	reset units 'touch' flags  */
  global_topo_ptr = topo_ptr_array;  /*  initialize global pointer */

  /*  limit left side of the topological array with NULL pointer  */
  *global_topo_ptr++ = NULL;

  /*  put all input units in the topologic array  */
  io_units = 0;
  FOR_ALL_UNITS( unit_ptr )
    if (IS_INPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
      io_units++;	/*  there is a input unit  */

  if ((NoOfInputUnits = io_units) == 0)
    {  /*  no input units */
    KernelErrorCode = KRERR_NO_INPUT_UNITS;
    return( KernelErrorCode );
  }

  /*  begin depth search at the first output unit  */
  io_units = 0;
  FOR_ALL_UNITS( unit_ptr )
    if ( IS_OUTPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ) )
      {
      io_units++;  /*  there is a output unit  */

      /*  sort the units topological (using depth search algorithm, 
	  starting at this output unit */
      DepthFirst1( unit_ptr, 1 );  
      if (topo_msg.error_code != KRERR_NO_ERROR)
        {  /*  stop if an error occured  */
        KernelErrorCode = topo_msg.error_code;
        return( KernelErrorCode );
      }
    }

  if ((NoOfOutputUnits = io_units) == 0)
    {  /*  no output units */
    KernelErrorCode = KRERR_NO_OUTPUT_UNITS;
    return( KernelErrorCode );
  }

  /*  limit right side of the topologic array with NULL pointer  */
  *global_topo_ptr++ = NULL;

  /*  calc. no. of sorted units  */
  no_of_topo_units = (global_topo_ptr - topo_ptr_array) - 2;

  /*  search for dead units i.e. units without inputs  */
  FOR_ALL_UNITS( unit_ptr )
    if ( !(unit_ptr->flags &  (UFLAG_REFRESH | UFLAG_TTYP_SPEC)) &&
         UNIT_IN_USE( unit_ptr ) )
      {
      topo_msg.no_of_dead_units++;
      if (topo_msg.src_error_unit == 0)
        topo_msg.src_error_unit = unit_ptr - unit_array; /* store the unit no.*/
    }

  if (topo_msg.no_of_dead_units != 0)
    KernelErrorCode = KRERR_DEAD_UNITS;

  return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : kr_topoSortT

  PURPOSE  : Sorts units topological in feed-forward networks and stores the
             pointers to these units in the topologic array in the following 
	     order:
	     - input,
	     - hidden and
	     - output units

             This function make following assumtions (like all learning 
	     functions for feed-forward networks):
             a) input units doesn't have input connections to other units and
             b) output units doesn't have outputs connections to other units.
  NOTES    : 

  RETURNS  : error code
  UPDATE   : 
******************************************************************************/
static krui_err  kr_topoSortFF(void)
{
  register struct Unit	 *unit_ptr;
  int	io_units;


  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */
  clr_T_flags();    /*	reset units 'touch' flags  */
  global_topo_ptr = topo_ptr_array;  /*  initialize global pointer */

  /*  limit left side of the topological array with NULL pointer  */
  *global_topo_ptr++ = NULL;

  /*  put all input units in the topologic array  */
  io_units = 0;
  FOR_ALL_UNITS( unit_ptr )
    if (IS_INPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
      {
      if UNIT_HAS_INPUTS( unit_ptr )
	{  /*  this input unit has a connection to another unit  */

	/*  store the unit no.  */
	topo_msg.dest_error_unit = unit_ptr - unit_array;  

	KernelErrorCode = KRERR_I_UNITS_CONNECT;  /*  input unit has input  */
        return( KernelErrorCode );
      }

      io_units++;	/*  there is a input unit  */
      *global_topo_ptr++ = unit_ptr;  /*  save input unit  */
    }

  if ((NoOfInputUnits = io_units) == 0)
    {  /*  no input units */
    KernelErrorCode = KRERR_NO_INPUT_UNITS;
    return( KernelErrorCode );
  }

  /*  limit input units in the topological array with NULL pointer  */
  *global_topo_ptr++ = NULL;

  /*  begin depth search at the first output unit  */
  io_units = 0;
  FOR_ALL_UNITS( unit_ptr )
    if (IS_OUTPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
      {
      io_units++;	/*  there is a output unit  */

      /*  sort the units topological (using depth search algorithm, 
	  starting at this output unit */
      DepthFirst3( unit_ptr, 1 );  
      if (topo_msg.error_code != KRERR_NO_ERROR)
        {  /*  stop if an error occured  */
        KernelErrorCode = topo_msg.error_code;
        return( KernelErrorCode );
      }
    }

  if ((NoOfOutputUnits = io_units) == 0)
    {  /*  no output units */
    KernelErrorCode = KRERR_NO_OUTPUT_UNITS;
    return( KernelErrorCode );
  }

  /*  limit hidden units in the topological array with NULL pointer  */
  *global_topo_ptr++ = NULL;

  /*  put all output units in the topological array  */
  FOR_ALL_UNITS( unit_ptr )
    if (IS_OUTPUT_UNIT(unit_ptr ) && UNIT_IN_USE( unit_ptr ))
      *global_topo_ptr++ = unit_ptr;  /*  save output unit  */

  /*  limit right side of the topologic array with NULL pointer  */
  *global_topo_ptr++ = NULL;

  /*  calc. no. of sorted units  */
  no_of_topo_units = (global_topo_ptr - topo_ptr_array) - 4;

  /*  search for dead units i.e. units without inputs  */
  FOR_ALL_UNITS( unit_ptr )
    if (!(unit_ptr->flags & (UFLAG_REFRESH | UFLAG_TTYP_SPEC)) && UNIT_IN_USE( unit_ptr ))
      {
      topo_msg.no_of_dead_units++;
      if (topo_msg.src_error_unit == 0)
        topo_msg.src_error_unit = unit_ptr - unit_array;  /*  store unit no.  */
    }

  if (topo_msg.no_of_dead_units != 0)
    KernelErrorCode = KRERR_DEAD_UNITS;

  return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : kr_topoSortIHO

  PURPOSE  : Sort units by their topologic type, i.e. Input, Hidden, Output 
             units and stores the pointers to this units in the topologic array.
  NOTES    : 

  RETURNS  : error code 
  UPDATE   : 
******************************************************************************/
static krui_err  kr_topoSortIHO(void)
{
  TopoPtrArray     topo_ptr;
  register struct Unit   *unit_ptr;
  int  no_of_units;
  int has_no_dual;


  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */
  topo_ptr = topo_ptr_array;

  /*  limit left side of the topological array with NULL pointer  */
  *topo_ptr++ = NULL;

  /*  get input units  */
  no_of_units = 0;
  FOR_ALL_UNITS( unit_ptr )
    if (IS_INPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
      {
      *topo_ptr++ = unit_ptr;
      no_of_units++;
    }


  if ((NoOfInputUnits = no_of_units) == 0)
    {
    KernelErrorCode = KRERR_NO_INPUT_UNITS;
    return( KernelErrorCode );
  }

  /*  limit input units in the topological array with NULL pointer  */
  *topo_ptr++ = NULL;

  /*  get hidden units  */
  no_of_units = 0;
  FOR_ALL_UNITS( unit_ptr )
    if (IS_HIDDEN_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
      {
      *topo_ptr++ = unit_ptr;
      no_of_units++;
    }

  if ((NoOfHiddenUnits = no_of_units) == 0)
    {
    /* In special case for DUAL units */ 
    FOR_ALL_UNITS( unit_ptr )
      if (IS_DUAL_UNIT( unit_ptr ) )
        has_no_dual = FALSE;
    if ( has_no_dual ){ 
	KernelErrorCode = KRERR_NO_HIDDEN_UNITS;
	return( KernelErrorCode );
    }
  }

  /*  limit hidden units in the topological array with NULL pointer  */
  *topo_ptr++ = NULL;

  /*  get output units  */
  no_of_units = 0;
  FOR_ALL_UNITS( unit_ptr )
    if (IS_OUTPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
      {
      *topo_ptr++ = unit_ptr;
      no_of_units++;
    }

  if ((NoOfOutputUnits = no_of_units) == 0)
    {
    KernelErrorCode = KRERR_NO_OUTPUT_UNITS;
    return( KernelErrorCode );
  }

  /*  limit right side of the topologic array with NULL pointer  */
  *topo_ptr++ = NULL;

  /*  calc. no. of sorted units  */
  no_of_topo_units = (topo_ptr - topo_ptr_array) - 4;

  return( KernelErrorCode );
}

/*****************************************************************************
  FUNCTION : kr_topoSortLOG() 

  PURPOSE  : Sort units by their logical Layer- and Unitnumber

  NOTES    : 

  RETURNS  : error code 
  UPDATE   : 
******************************************************************************/

static int llncompare(const struct Unit **a, const struct Unit **b)
{ 
  int llndiff, lundiff;

  llndiff = ((*a)->lln - (*b)->lln);
  lundiff = ((*a)->lun - (*b)->lun);
  return(llndiff != 0 ? llndiff : lundiff);
}

static krui_err  kr_topoSortLOG(void)
{
  struct Unit      *unit_ptr;
  TopoPtrArray     topo_ptr;
  TopoPtrArray     topo_ptr_save;
  int              no_of_units = 0;

  topo_ptr = topo_ptr_array;
  *topo_ptr++ = (struct Unit *) NULL;

  
  FOR_ALL_UNITS( unit_ptr )
    if ( (unit_ptr->flags & UFLAG_IN_USE) == UFLAG_IN_USE) 
      {
      *topo_ptr = unit_ptr;
      topo_ptr++;
      no_of_units++;
      }
  *topo_ptr = (struct Unit *) NULL;
  topo_ptr_save = topo_ptr;

  no_of_topo_units = no_of_units;
  topo_ptr = topo_ptr_array;
  topo_ptr++;
  qsort(topo_ptr, no_of_units, sizeof(*topo_ptr), 
	(int (*)(const void *, const void *)) llncompare);

  /* insert NULL pointer between input units and rest */
  topo_ptr = topo_ptr_save;
  while (*topo_ptr == (struct Unit *) NULL ||
         !(IS_INPUT_UNIT(*topo_ptr)))
  {
      *(topo_ptr + 1) = *topo_ptr;
      topo_ptr--;
  }
  topo_ptr++;
  *topo_ptr = (struct Unit *) NULL;
  topo_ptr_save++;

  /* insert NULL pointer between output units and rest */
  topo_ptr = topo_ptr_save;
  while (*topo_ptr == (struct Unit *) NULL ||
         (IS_OUTPUT_UNIT(*topo_ptr)))
  {
      *(topo_ptr + 1) = *topo_ptr;
      topo_ptr--;
  }
  topo_ptr++;
  *topo_ptr = (struct Unit *) NULL;
  topo_ptr_save++;

  /* create pointers from units to topo_ptr_array */
  topo_ptr = topo_ptr_array;
  while (topo_ptr != topo_ptr_save)
  {
      if (*topo_ptr != (struct Unit *) NULL)
      {
          (*topo_ptr) -> TD.my_topo_ptr = topo_ptr;
      }
      topo_ptr++;
  }
  return (KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : kr_topoSort

  PURPOSE  : 

    Sort units according to the given mode:
    TOPOLOGICAL:
      Sort units topological (general version) and stores the
      pointers to this units in the topologic array.
      NOTE: Units are not sorted by their topologic type (that's not
	    possible in general case).

    TOPOLOGICAL_FF:
      Sorts unit topological in feed-forward networks and stores the
      pointers to this units in the topologic array in the following order:
       - input,
       - hidden and
       - output units

      This function make following assumtions (like all learning functions for
      feed-forward networks):
       a) input units doesn't have input connections to other units and
       b) output units doesn't have outputs connections to other units.

    TOPOLOGIC_TYPE:
      Sort units by their topologic type, i.e. Input, Hidden, Output units and
      stores the pointers to this units in the topologic array.

    TOPOLOGIC_LOGICAL:
      Sort Units according to their logical Layers- and Unitsnumbers.
      The entry TD.my_topo_ptr in every unit is set to point to coresponding
      entry in the topo_ptr_array.

    ART1_TOPO_TYPE:
      Sort units in ART1 manner. For informations about the structure of
      ART1 networks see Diplomarbeit No.929; Kai-Uwe Herrmann; University of
      Stuttgart; Germany 1992. The pointers are sorted as follows:

      NULL, pointers to input units, NULL, pointers to comparison units,
      NULL, pointers to recognition units, NULL, pointers to delay units,
      NULL, pointers to local reset units, NULL, pointers to special units,
      NULL, NULL, ...

    ART2_TOPO_TYPE:
      Sort units in ART2 manner. For informations about the structure of
      ART2 networks see Diplomarbeit No.929; Kai-Uwe Herrmann; University of
      Stuttgart; Germany 1992. The pointers are sorted as follows:

      NULL, pointers to input units, NULL, pointers to w units,
      NULL, pointers to x units, NULL, pointers to u units,
      NULL, pointers to v units, NULL, pointers to p units,
      NULL, pointers to q units, NULL, pointers to r units,
      NULL, pointers to recognition units, NULL, pointers to delay units,
      NULL, pointers to local reset units, NULL, pointers to special units,
      NULL, NULL, ...

    ARTMAP_TOPO_TYPE:
      Sort units in ARTMAP manner. For informations about the structure of
      ARTMAP networks see Diplomarbeit No.929; Kai-Uwe Herrmann; University of
      Stuttgart; Germany 1992. The pointers are sorted as follows:

      NULL, ARTa inp units, NULL, ARTa cmp units, NULL, ARTa rec units ...,
      NULL, ARTb inp units, NULL, ARTb cmp units, NULL, ARTb rec units ...,
      NULL, map field units, NULL, map field special units, NULL

  NOTES    : 

  RETURNS  : error code 
  UPDATE   : 
******************************************************************************/
krui_err  kr_topoSort(int topo_sorting_mode)
{
  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */
  TopoSortID = NOT_SORTED;
  if (NoOfUnits == 0)
    {  /*  No units defined  */
    KernelErrorCode = KRERR_NO_UNITS;
    return( KernelErrorCode );
  }


  if (krm_allocUnitTopoArray( NoOfUnits + 15) != KRERR_NO_ERROR)
    return( KernelErrorCode );

  /*  clear error codes  */
  topo_msg.no_of_cycles = topo_msg.no_of_dead_units =
  topo_msg.dest_error_unit = topo_msg.src_error_unit = 0;
  topo_msg.error_code = KRERR_NO_ERROR;

  switch (topo_sorting_mode)
    {
    case  TOPOLOGICAL:
	(void) kr_topoSortT();
	break;
    case  TOPOLOGICAL_FF:
	(void) kr_topoSortFF();
	break;
    case  TOPOLOGIC_TYPE:
	(void) kr_topoSortIHO();
	break;
    case  TOPOLOGIC_LOGICAL:
        KernelErrorCode = kr_topoSortLOG();
        break;
    case  ART1_TOPO_TYPE:
        KernelErrorCode = kra1_sort ();
        break;

    case ART2_TOPO_TYPE:
        KernelErrorCode = kra2_sort ();
        break;

    case ARTMAP_TOPO_TYPE:
        KernelErrorCode = kram_sort ();
        break;
    case TOPOLOGICAL_CC:
        (void) cc_topoSort(TOPOLOGICAL_CC);
        break; 
    case TOPOLOGICAL_BCC:
        (void) cc_topoSort(TOPOLOGICAL_BCC);
        break;
    case TOPOLOGICAL_JE:
	KernelErrorCode = kr_topoSortJE () ;
	break ;

    default:
	KernelErrorCode = KRERR_TOPOMODE;
    }

  if ((KernelErrorCode == KRERR_NO_ERROR) || 
      (KernelErrorCode == KRERR_DEAD_UNITS))
    TopoSortID = topo_sorting_mode;

  return( KernelErrorCode );
}



/*****************************************************************************
  FUNCTION : kr_topoCheck

  PURPOSE  : Checks the topology of the network:
             a) counts the number of layers of the network and
	     b) determines if the network has cycles.
  NOTES    : 

  RETURNS  : Returns the no. of layers of the network.
  UPDATE   : 
******************************************************************************/
int  kr_topoCheck(void)
{
  struct Unit   *unit_ptr;
  bool      o_units;


  topo_msg.no_of_cycles = topo_msg.no_of_dead_units =
  topo_msg.dest_error_unit = topo_msg.src_error_unit =
  topo_msg.no_of_layers = 0;
  topo_msg.error_code = KernelErrorCode = KRERR_NO_ERROR;

  if (NoOfUnits == 0)
    {  /*  no units defined  */
    KernelErrorCode = KRERR_NO_UNITS;
    return( KernelErrorCode );
  }

  clr_T_flags();    /*	reset units 'touch' flags  */

  /*  begin depth search at the first output unit  */
  o_units = FALSE;
  FOR_ALL_UNITS( unit_ptr )
    if ( IS_OUTPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ) )
      {
      o_units = TRUE;
      DepthFirst2( unit_ptr, 1 );
      if (topo_msg.error_code != KRERR_NO_ERROR)
        {  /*  stop if an error occured  */
        KernelErrorCode = topo_msg.error_code;
        return( KernelErrorCode );
      }
    }
      
  if (!o_units)
    {  /*  no output units */
    KernelErrorCode = KRERR_NO_OUTPUT_UNITS;
    return( KernelErrorCode );
  }

  /*  return the no. of layers of the network  */
  return( topo_msg.no_of_layers );
}

/*****************************************************************************
  FUNCTION : kr_makeUnitPermutation

  PURPOSE  : 
  NOTES    : 

  RETURNS  : Returns error code
  UPDATE   : 
******************************************************************************/
krui_err  kr_makeUnitPermutation(void)
{
  register struct Unit   *unit_ptr;
  register int	   no_of_units, i;
  TopoPtrArray     topo_ptr,  t_ptr1,  t_ptr2;


  TopoSortID = NOT_SORTED;
  if (NoOfUnits == 0)  return( KRERR_NO_UNITS );  /*  no units defined	*/

  if ( krm_allocUnitTopoArray( NoOfUnits + 2) != 0)
    return( KRERR_INSUFFICIENT_MEM );

  topo_ptr = topo_ptr_array;

  /*  limit left side of the topological array with NULL pointer  */
  *topo_ptr++ = NULL;

  /*  initialize permutation array  */
  FOR_ALL_UNITS( unit_ptr )
    if ( (unit_ptr->flags & UFLAG_INITIALIZED) == UFLAG_INITIALIZED)
      /*  unit is in use and enabled  */
      *topo_ptr++ = unit_ptr;

  no_of_topo_units = topo_ptr - topo_ptr_array;  /* calc no. of sorted units */
  no_of_units = no_of_topo_units;

  topo_ptr = topo_ptr_array;
  /*  permutate unit order  */
  for (i = 0; i < no_of_units; i++)
    {
    t_ptr1 = topo_ptr + (lrand48() % no_of_units);
    t_ptr2 = topo_ptr + (lrand48() % no_of_units);

    unit_ptr = *t_ptr1;
    *t_ptr1 = *t_ptr2;
    *t_ptr2 = unit_ptr;
    }

  /*  limit right side of the topologic array with NULL pointer  */
  *topo_ptr++ = NULL;

  TopoSortID = PERMUTATION;
  NetModified = FALSE;

  return( KRERR_NO_ERROR );
}


/*#################################################

GROUP: Functions for pattern management

#################################################*/
/*****************************************************************************
  FUNCTION : kr_IOCheck

  PURPOSE  :  Count the no. of input and output units and return an error code
              if the no. do not fit to the loaded patterns.
  NOTES    : 

  RETURNS  : Returns error code
  UPDATE   : 
******************************************************************************/
krui_err  kr_IOCheck(void)
{
  register struct Unit   *unit_ptr;
  register int  no_of_i_units, no_of_o_units;

  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */

  /*  count no. of input and output units  */
  no_of_i_units = no_of_o_units = 0;
  FOR_ALL_UNITS( unit_ptr )
    if UNIT_IN_USE( unit_ptr ){
      if IS_INPUT_UNIT( unit_ptr )
        no_of_i_units++;
      if IS_OUTPUT_UNIT( unit_ptr )
          no_of_o_units++;
    }
  NoOfInputUnits = no_of_i_units;
  NoOfOutputUnits = no_of_o_units;

  return( KernelErrorCode );
}

/*#################################################

GROUP: other functions

#################################################*/

 /*****************************************************************************
  FUNCTION :  kr_NA_Error

  PURPOSE  : calculates the error for the network-analyzer tool
  NOTES    : 

  RETURNS  : Returns the float value of the error
  UPDATE   : 
******************************************************************************/

float kr_NA_Error(int currentPattern, int error_unit, int error, bool ave)
{
  register struct   Unit *unit_ptr, *error_unit_ptr ;
  register Patterns       out_pat  ;
  register float          error_lin, error_sqr, error_su, devit ;
  int                     pattern_no, sub_pat_no;


  kr_initSubPatternOrder(currentPattern, currentPattern);
  kr_getSubPatternByOrder(&pattern_no, &sub_pat_no);
  out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT, NULL);

  error_lin = 0 ;
  error_sqr= 0 ;
  error_su  = 0 ;

  if (error_unit != 0)
    error_unit_ptr = kr_getUnitPtr (error_unit) ;
    
  FOR_ALL_UNITS (unit_ptr)
  {
    if (IS_OUTPUT_UNIT (unit_ptr))
    {
      devit = (float) (*(out_pat++) - unit_ptr->Out.output) ;
      error_lin += fabs (devit) ;
      error_sqr += devit * devit ;  

      if (unit_ptr == error_unit_ptr) error_su = fabs (devit) ;
    }
  }
 

  switch (error)
  {
    case NA_ERROR_LIN : 
    {
      if (ave) return (error_lin / (float) NoOfOutputUnits);
      else        return (error_lin) ; 
      break ; 
    }

    case NA_ERROR_SQR : 
    {
      if (ave) return (error_sqr / (float) NoOfOutputUnits);
      else        return (error_sqr) ;
      break ; 
    }

    case NA_ERROR_SU :
    {
      return (error_su) ;
      break ;
    }
  }
  /* Only for the warning */
  return(0.0);
}

/*#################################################

GROUP: Functions for handeling network propagation,
       update and learning functions.

#################################################*/
/*****************************************************************************
  FUNCTION : kr_callNetworkFunctionSTD

  PURPOSE  : calls the current network function
  NOTES    : 

  RETURNS  : Returns error code
  UPDATE   : 
******************************************************************************/
static krui_err  kr_callNetworkFunctionSTD(int type, float *parameterInArray, 
					   int NoOfInParams, 
					   float **parameterOutArray, 
					   int *NoOfOutParams, 
					   int start_pattern, int end_pattern)
{
  FunctionPtr  func_ptr;
  NetFunctionPtr  net_func_ptr;
  char  *curr_func;
  int size;


  if ( (curr_func = krf_getCurrentNetworkFunc( type )) == NULL)
    return( KernelErrorCode );
  if (!krf_funcSearch( curr_func, type, &func_ptr ) )
    return( KernelErrorCode );

  KernelErrorCode = KRERR_NO_ERROR;
  net_func_ptr = (NetFunctionPtr) func_ptr;

  switch (type)  {
    case  UPDATE_FUNC:
      KernelErrorCode = 
	  (*(UpdateFuncPtr)net_func_ptr) ( parameterInArray, NoOfInParams );
      return( KernelErrorCode );

    case TEST_FUNC:
    case  LEARN_FUNC:
      if (kr_TotalNoOfPattern() == 0)
	{  /*  no patterns defined  */
	KernelErrorCode = KRERR_NO_PATTERNS;
	return( KernelErrorCode );
      }
      if ((start_pattern < 0) || (end_pattern >= kr_TotalNoOfPattern()) )
	{  /*  Invalid pattern number  */
	KernelErrorCode = KRERR_PATTERN_NO;
	return( KernelErrorCode );
      }

      /* check whether sub pattern fits onto network */
      if (NetModified)
	  kr_IOCheck();
      size = kr_SizeOfInputSubPat();
      if (NoOfInputUnits != size)
      {
	  if (size < 0)
	      KernelErrorCode = size;
	  else
	      KernelErrorCode = KRERR_NP_DOES_NOT_FIT;
	  return KernelErrorCode;
      }
      size = kr_SizeOfOutputSubPat();
      if (NoOfOutputUnits != size)
      {
	  if (size < 0)
	      KernelErrorCode = size;
	  else if (size == 0)
	      KernelErrorCode = KRERR_NP_NO_OUTPUT_PATTERN;
	  else
	      KernelErrorCode = KRERR_NP_DOES_NOT_FIT;
	  return KernelErrorCode;
      }

      /*  call current learning function  */
      KernelErrorCode = 
	  (*(LearnFuncPtr)net_func_ptr) (start_pattern, end_pattern,
					 parameterInArray, NoOfInParams,
					 parameterOutArray, NoOfOutParams);

      if (KernelErrorCode == KRERR_NO_ERROR)
	{  /*  learning function has initialized the network  */
	NetInitialize = FALSE;
	LearnFuncHasChanged = FALSE;
      }

      return( KernelErrorCode );

    case (FF_LEARN_FUNC | LEARN_FUNC):

      /* check whether sub pattern fits onto network */
      if (NetModified)
	  kr_IOCheck();
      size = kr_SizeOfInputSubPat();
      if (NoOfInputUnits != size)
      {
	  if (size < 0)
	      KernelErrorCode = size;
	  else
	      KernelErrorCode = KRERR_NP_DOES_NOT_FIT;
	  return KernelErrorCode;
      }
      size = kr_SizeOfOutputSubPat();
      if (NoOfOutputUnits != size)
      {
	  if (size < 0)
	      KernelErrorCode = size;
	  else if (size == 0)
	      KernelErrorCode = KRERR_NP_NO_OUTPUT_PATTERN;
	  else
	      KernelErrorCode = KRERR_NP_DOES_NOT_FIT;
	  return KernelErrorCode;
      }
      /*  call embedded feed forward learning function for pruning */
      KernelErrorCode = 
	  (*(LearnFuncPtr) net_func_ptr) (start_pattern, end_pattern,
					  parameterInArray, NoOfInParams,
					  parameterOutArray, NoOfOutParams);

      if (KernelErrorCode == KRERR_NO_ERROR)
	  /*  learning function has initialized the network  */
      { 
	  NetInitialize = FALSE;
	  LearnFuncHasChanged = FALSE;
      }

      return( KernelErrorCode );

    case  INIT_FUNC:
      NetInitialize = TRUE;
      KernelErrorCode = 
	  (*(InitFuncPtr)net_func_ptr) ( parameterInArray, NoOfInParams );

      return( KernelErrorCode );

   default:
     KernelErrorCode = KRERR_PARAMETERS;
     return( KernelErrorCode );
  }
}

/*****************************************************************************
  FUNCTION : kr_callNetworkFunction

  PURPOSE  : calls the current network function
  NOTES    : 

  RETURNS  : Returns error code
  UPDATE   : 
******************************************************************************/
krui_err  kr_callNetworkFunction(int type, float *parameterInArray, 
				 int NoOfInParams, float **parameterOutArray, 
				 int *NoOfOutParams, int start_pattern, 
				 int end_pattern)
{
#ifdef MASPAR_KERNEL
  static struct NetFuncParameters  net_func_params;
#endif

  krui_err dummy;
  if (NoOfUnits == 0)
    {  /*  No Units defined  */
    KernelErrorCode = KRERR_NO_UNITS;
    return( KRERR_NO_UNITS );
  }

  KernelErrorCode = KRERR_NO_ERROR;

  switch (specialNetworkType)  {
    case NET_TYPE_GENERAL:
      /*  normal network presentation  */
      /*  the result of this call has been void which is not compatible */
      /*  to the declaration; therefor the dummy error variable is included */
      dummy = kr_callNetworkFunctionSTD( type, parameterInArray, NoOfInParams,
                                        parameterOutArray, NoOfOutParams,
                                        start_pattern, end_pattern );
      break;

#ifdef MASPAR_KERNEL
    case NET_TYPE_FF1:

      /*  feedforward net on MasPar  */
      net_func_params.start_pattern_no = start_pattern;
      net_func_params.end_pattern_no   = end_pattern;
      net_func_params.no_of_input_parameters = NoOfInParams;

      memcpy( net_func_params.input_parameters, parameterInArray,
              sizeof (float) * NoOfInParams );

      (void) krff_callMasParNetworkFunction( type, &net_func_params );

      if (NoOfOutParams != NULL)
        *NoOfOutParams = net_func_params.no_of_output_parameters;
      if (parameterOutArray != NULL)
        *parameterOutArray = net_func_params.output_parameters;

      break;
#endif

    default:
      KernelErrorCode = KRERR_PARAMETERS;
  }

  return( KernelErrorCode );
}



#ifdef HAVE_QSORT
static int  transTableCompare( const void *node1, const void *node2)
{
  short  z1, z2;

  z1=((struct TransTable *) node1)->z;
  z2=((struct TransTable *) node2)->z;

  if (z1 < z2)  return -1;
  if (z1 > z2)  return 1;
  return 0;
}
#endif

krui_err  kr_xyTransTable(int op, int *x, int *y, int z)
{
  struct TransTable  *transTablePtr,
                     *new_transTable,
                     transTableEntry;


  switch(op)  {
    case OP_TRANSTABLE_GET:
      if (transTable != NULL)  {

#ifdef HAVE_QSORT
        transTableEntry.z = z;
        transTablePtr = 
	    (struct TransTable *) bsearch( &transTableEntry,
					  (struct TransTable *) transTable,
					  transTableSize,
					  sizeof(struct TransTable),
					  transTableCompare );
#else
	for (transTablePtr = transTable;
             transTablePtr < (transTable + transTableSize);
             transTablePtr++)
          if (transTablePtr->z == z)  break;

          if (transTablePtr == (transTable + transTableSize))
            transTablePtr=NULL;
#endif

        if (transTablePtr == NULL)  {
          *x = *y = 0;
        }
        else  {
          *x=transTablePtr->x;
          *y=transTablePtr->y;
        }
      }
      else  {
        *x = *y = 0;
      }

      KernelErrorCode = KRERR_NO_ERROR;
      return( KRERR_NO_ERROR );

    case OP_TRANSTABLE_SET:
      if (transTable == NULL)  {
        if((new_transTable =
           (struct TransTable *) malloc( sizeof(struct TransTable) )) == NULL){
          KernelErrorCode = KRERR_INSUFFICIENT_MEM;
          return( KRERR_INSUFFICIENT_MEM );
        }

        transTable=new_transTable;

        transTable->z = z;
        transTable->x = *x;
        transTable->y = *y;
        transTableSize=1;
      }
      else  {
#ifdef HAVE_QSORT
        transTableEntry.z = z;
        transTablePtr = 
	    (struct TransTable *) bsearch( &transTableEntry,
					  (struct TransTable *) transTable,
					  transTableSize,
					  sizeof(struct TransTable),
					  transTableCompare );
#else
    	for (transTablePtr = transTable;
             transTablePtr < (transTable + transTableSize);
             transTablePtr++)
          if (transTablePtr->z == z)  break;

          if (transTablePtr == (transTable + transTableSize))
            transTablePtr=NULL;
#endif

        if (transTablePtr == NULL)  {
          if ((new_transTable = 
	       (struct TransTable *) realloc( (void *) transTable,
		sizeof(struct TransTable) * (transTableSize + 1) )) == NULL)  {
            KernelErrorCode = KRERR_INSUFFICIENT_MEM;
            return( KRERR_INSUFFICIENT_MEM );
          }

          transTable=new_transTable;

          transTable[transTableSize].z = z;
          transTable[transTableSize].x = *x;
          transTable[transTableSize].y = *y;
          ++transTableSize;

#ifdef HAVE_QSORT
          qsort( (struct TransTable *) transTable,
                 transTableSize,
                 sizeof(struct TransTable),
                 transTableCompare );
#endif
        }
        else  {
          transTablePtr->x = *x;
          transTablePtr->y = *y;
        }
      }

      KernelErrorCode=KRERR_NO_ERROR;
      return( KRERR_NO_ERROR );

    case OP_TRANSTABLE_CLEAR:
      if (transTable != NULL)  {
        free( (void *) transTable );
        transTable = NULL;
        transTableSize = 0;
      }

      KernelErrorCode=KRERR_NO_ERROR;
      return( KRERR_NO_ERROR );

    default:
      KernelErrorCode=KRERR_PARAMETERS;
      return( KRERR_PARAMETERS );
  }
}


/*#################################################

GROUP: Functions for the parallel kernel

#################################################*/

/*****************************************************************************
  FUNCTION : kr_setSpecialNetworkType

  PURPOSE  : Sets the topologic type of the current network and checks the 
             topology of the current network.
	     Returns an error if the topologic type of the current network  
	     doesn't fit to this type.
	     Topologic types are:
	     - NET_TYPE_GENERAL
	     general purpose network type with no limitations
	     - NET_TYPE_FF1
	     feedforward network with fully connected units in
	     neighbour layers
  NOTES    : 

  RETURNS  : Returns error code
  UPDATE   : 
******************************************************************************/
krui_err  kr_setSpecialNetworkType(int net_type)
{
  KernelErrorCode = KRERR_NO_ERROR;

  if (net_type == specialNetworkType)  return( KRERR_NO_ERROR );

  if (NoOfUnits == 0)
    {  /*  no units defined  */
    KernelErrorCode = KRERR_NO_UNITS;
    return( KernelErrorCode );
  }

  switch (net_type)  {
    case  NET_TYPE_GENERAL:

      switch (specialNetworkType)  {
        case  NET_TYPE_FF1:
          /*  change special network presentation to standard presentation  */

#ifdef  MASPAR_KERNEL
          (void) krff_standardNetPresentationFF1();
          specialNetworkType = NET_TYPE_GENERAL;
#else
          KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
#endif
          break;

        default:
          KernelErrorCode = KRERR_PARAMETERS;
      }

      break;

    case  NET_TYPE_FF1:
      /*  change standart network presentation to special presentation  */
#ifdef  MASPAR_KERNEL
      (void) krff_determineNetFF1Params();
      /*  change internal network presentation	*/
      if (KernelErrorCode != KRERR_NO_ERROR)  break;

      (void) krff_initMasPar();
      if (KernelErrorCode != KRERR_NO_ERROR)  break;

      (void) krff_changeNetPresentationFF1();

#else
      KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
#endif
      break;

    default:
      KernelErrorCode = KRERR_PARAMETERS;
  }

  if (KernelErrorCode == KRERR_NO_ERROR)
    specialNetworkType = net_type;

  return( KernelErrorCode );
}

/*****************************************************************************
  FUNCTION : kr_getSpecialNetworkType

  PURPOSE  : Returns the special topologic type of the current network, if set.
  NOTES    : 

  RETURNS  : Returns the special topologic type of the current network, if set.
  UPDATE   : 
******************************************************************************/
int  kr_getSpecialNetworkType(void)
{
  return( specialNetworkType );
}


/*****************************************************************************
  FUNCTION : kr_validateOperation

  PURPOSE  : Validate a network modifying operation according to
             the kernel mode 
  NOTES    : 

  RETURNS  : error code
  UPDATE   : 
******************************************************************************/
krui_err  kr_validateOperation(void)
{
  switch (specialNetworkType)
    {
    case NET_TYPE_GENERAL:
      /*  normal network presentation, no limitations  */
      KernelErrorCode = KRERR_NO_ERROR;
      break;
    case NET_TYPE_FF1:
      /*  feedforward net with limitations  */
      KernelErrorCode = KRERR_MODE_FF1_INVALID_OP;
      break;
  }

  return( KernelErrorCode );
}


/* #############################################################
 
  Functions for the MasPar kernel

############################################################# */

#ifdef  MASPAR_KERNEL
/*****************************************************************************
  FUNCTION : kr_initMasPar

  PURPOSE  : Connects and Disconnects the MasPar.
             The mode switches are:  MASPAR_CONNECT and MASPAR_DISCONNECT.
  NOTES    : 

  RETURNS  : error code
  UPDATE   : 
******************************************************************************/
krui_err  kr_initMasPar(int mode )
{
  if (specialNetworkType == NET_TYPE_GENERAL)  {
    KernelErrorCode = KRERR_NOT_PARALLEL_MODE;
    return( KernelErrorCode );
  }

  KernelErrorCode = KRERR_NO_ERROR;

  switch (mode)
    {
    case  MASPAR_CONNECT:
      /*  connect maspar  */
      if (krff_initMasPar() == KRERR_NO_ERROR)
        masParStatus = MASPAR_CONNECT;

      break;
    case  MASPAR_DISCONNECT:
      /*  disconnect maspar  */
      masParStatus = MASPAR_DISCONNECT;

      break;
    default:
      KernelErrorCode = KRERR_PARAMETERS;
  }

  return( KernelErrorCode );
}

/*****************************************************************************
  FUNCTION : kr_getMasParStatus

  PURPOSE  : 
  NOTES    : 

  RETURNS  : Returns the Status of the MasPar or an error code
  UPDATE   : 
******************************************************************************/
krui_err  kr_getMasParStatus(void)
{
  KernelErrorCode = KRERR_NO_ERROR;

  return( masParStatus );
}

#endif

/*****************************************************************************

  Note: This is not part of the official SNNS distribution, but provided
  by the snns-dev project (http://snns-dev.berlios.de)

  The point where the Python extension hooks in - this wrapper layer shall
  make sure that the SNNS kernel can still be compiled without having Python,
  even with the Python patch applied.

******************************************************************************/

FlintType (*kr_PythonOutFunctionHook)(PyObject *func, FlintType activation);
FlintType (*kr_PythonActFunctionHook)(PyObject *func, struct Unit *unit_ptr);
PyObject *(*kr_findPythonFunctionHook)(char *func, int type);
int (*kr_getNoOfPythonFunctionsHook)();
krui_err (*kr_getPythonFuncInfoHook)(int mode, struct FuncInfoDescriptor *descr);

int kr_getNoOfPythonFunctions()
{
	if(!kr_getNoOfPythonFunctionsHook) {
		fputs("No callback for number of Python functions found\n",
			stderr);
		return 0;	
	} else {	
		return kr_getNoOfPythonFunctionsHook();
	}	
}

krui_err kr_getPythonFuncInfo(int mode, struct FuncInfoDescriptor *descr)
{
	if(!kr_getPythonFuncInfoHook) {
		fputs("No Python function info callback found\n",stderr);
		return KRERR_PARAMETERS;
	} else {
		return kr_getPythonFuncInfoHook(mode, descr);
	}
}


FlintType kr_PythonOutFunction(PyObject *func, FlintType activation)
{
	if(!kr_PythonOutFunctionHook) {
		fputs("The Python output function you are requesting "
		        "can not be called because there is no callback "
			"registered.\n",stderr);
		return 0;
	} else {
		return kr_PythonOutFunctionHook(func, activation);
	}
}

FlintType kr_PythonActFunction(PyObject *func, struct Unit *unit_ptr)
{
	if(!kr_PythonActFunctionHook) {
		fputs("The Python activation function you are requesting "
		        "can not be called because there is no callback "
			"registered.\n",stderr);
		return 0;
	} else {
		return kr_PythonActFunctionHook(func, unit_ptr);
	}
}


PyObject *kr_findPythonFunction(char *name, int type)
{
	if(!kr_findPythonFunctionHook) {
		fputs("Can't check for Python functions because there is "
		      "no callback registered.\n",stderr);
		return NULL;      
	} else {
		return kr_findPythonFunctionHook(name, type);
	}	
}

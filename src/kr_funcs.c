/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_funcs.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel: Management routines for user functions 
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 12.02.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:37 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <string.h>

#include "kr_typ.h"         /*  Kernel types and constants  */
#include "kr_const.h"       /*  Kernel constants  */
#include "kr_def.h"	    /*	Default values	*/
#include "kr_funcs.ph"       /*  Function prototypes  */
#include "kernel.h"
#include "func_tbl.h"
#include "glob_typ.h"


/*#################################################

GROUP: MasPar functions

#################################################*/

#ifdef MASPAR_KERNEL
#ifndef MASPAR_KERNEL_EMULATION

extern  krmp_getMasParFuncInfo();

#endif
#endif


/*#################################################

GROUP: Functions

#################################################*/
/*****************************************************************************
  FUNCTION : krf_getInternalFuncInfo 

  PURPOSE  : 
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
extern FlintType OUT_Custom_Python(FlintType act);
extern FlintType ACT_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_DERIV_Custom_Python(struct Unit * unit_ptr);
extern FlintType ACT_2_DERIV_Custom_Python(struct Unit * unit_ptr);

static krui_err  krf_getInternalFuncInfo(int mode, struct FuncInfoDescriptor *func_descr)
{
  struct FuncTable  *ftbl_ptr;
  unsigned short  func_type;

  KernelErrorCode = KRERR_NO_ERROR;

  switch (mode)  {
    case  GET_NO_OF_FUNCS:
      func_descr->number = NoOfKernelFuncs + kr_getNoOfPythonFunctions();
      break;

    case  GET_FUNC_INFO:  /*  return all info about given function  */
      if ((func_descr->number < 0) || (func_descr->number >= 
      			(NoOfKernelFuncs + kr_getNoOfPythonFunctions())))  {
        KernelErrorCode = KRERR_PARAMETERS;
        return( KernelErrorCode );
      }

      if(func_descr->number < NoOfKernelFuncs) {
	      ftbl_ptr = kernel_func_table + func_descr->number;

	      strcpy( func_descr->func_name, ftbl_ptr->func_name );
	      func_descr->func_type = ftbl_ptr->func_type & ~DEFAULT_FUNC;
	      func_descr->no_of_input_parameters = ftbl_ptr->no_of_input_parameters;
	      func_descr->no_of_output_parameters = ftbl_ptr->no_of_output_parameters;
	      func_descr->function = ftbl_ptr->function;
	#ifdef PARAGON_KERNEL
	      func_descr->parallelized = ftbl_ptr->parallelized;
	#endif
	} else { /* Must be a Python function */
		KernelErrorCode = kr_getPythonFuncInfo(mode, func_descr);
	}
      break;

    case  SEARCH_FUNC:  /*  search for the given function and return the
                            info about the function  */

      for (ftbl_ptr = kernel_func_table;
           ftbl_ptr < kernel_func_table + NoOfKernelFuncs;
           ftbl_ptr++)  {
        func_type = func_descr->func_type;
        if (((func_type & FUNC_TYPE_MASK) == (ftbl_ptr->func_type & FUNC_TYPE_MASK)) &&
            (strcmp( func_descr->func_name, ftbl_ptr->func_name ) == 0))  {
          func_descr->no_of_input_parameters = ftbl_ptr->no_of_input_parameters;
          func_descr->no_of_output_parameters = ftbl_ptr->no_of_output_parameters;
          func_descr->function = ftbl_ptr->function;
#ifdef PARAGON_KERNEL
	  func_descr->parallelized = ftbl_ptr->parallelized;
#endif
          return( KRERR_NO_ERROR );
        }
      }
      KernelErrorCode = kr_getPythonFuncInfo(mode,func_descr);
      if(!KernelErrorCode)  {
      		switch (func_descr->func_type) {
			case OUT_FUNC:
				func_descr->function = 
					(FunctionPtr)OUT_Custom_Python;
				break;
			case ACT_FUNC:
				func_descr->function = 
					(FunctionPtr)ACT_Custom_Python;
				break;
			case ACT_DERIV_FUNC:
				func_descr->function =
					(FunctionPtr)ACT_DERIV_Custom_Python;
				break;
			case ACT_2_DERIV_FUNC:
				func_descr->function =
					(FunctionPtr)ACT_2_DERIV_Custom_Python;
				break;	
			default:
				fputs("Unhandled Python function type\n",
					stderr);
		}
      		return KRERR_NO_ERROR;
      }
      func_descr->function = NULL;
#ifdef PARAGON_KERNEL
      func_descr->parallelized = ftbl_ptr->parallelized;
#endif

      switch (func_descr->func_type)  {
        case  OUT_FUNC:
          KernelErrorCode = KRERR_OUTFUNC;
          break;
        case  ACT_FUNC:
          KernelErrorCode = KRERR_ACTFUNC;
          break;
        case  SITE_FUNC:
          KernelErrorCode = KRERR_SITEFUNC;
          break;
        case  LEARN_FUNC:
          KernelErrorCode = KRERR_LEARNING_FUNC;
          break;
        case  UPDATE_FUNC:
          KernelErrorCode = KRERR_UPDATE_FUNC;
          break;
        case  INIT_FUNC:
          KernelErrorCode = KRERR_INIT_FUNC;
          break;
        case  ACT_DERIV_FUNC:
        case  ACT_2_DERIV_FUNC:
          KernelErrorCode = KRERR_DERIV_FUNC;
          break;
	case REMAP_FUNC:
	  KernelErrorCode = KRERR_REMAP_FUNC;
          break;

        default:
          KernelErrorCode = KRERR_PARAMETERS;
      }

      break;

    case  GET_FUNC_NAME:   /*  search for the given function pointer and
                               returns the name of the function  */

      for (ftbl_ptr = kernel_func_table;
           ftbl_ptr < kernel_func_table + NoOfKernelFuncs;
           ftbl_ptr++)
        if (func_descr->function == ftbl_ptr->function)  {
          strcpy( func_descr->func_name, ftbl_ptr->func_name );
          func_descr->func_type = ftbl_ptr->func_type & ~DEFAULT_FUNC;
          func_descr->no_of_input_parameters = ftbl_ptr->no_of_input_parameters;
          func_descr->no_of_output_parameters = ftbl_ptr->no_of_output_parameters;
#ifdef PARAGON_KERNEL
	  func_descr->parallelized = ftbl_ptr->parallelized;
#endif

          return( KRERR_NO_ERROR );
        }
      KernelErrorCode = kr_getPythonFuncInfo(GET_FUNC_NAME,func_descr);
      if(KernelErrorCode == KRERR_NO_ERROR) return KRERR_NO_ERROR;
      func_descr->func_type = 0;
      break;

    case  GET_DEFAULT_FUNC:   /*  search for the given function type and
                                  returns the default function of this type  */

      func_type = (func_descr->func_type | DEFAULT_FUNC) & ~FF_LEARN_FUNC;
      for (ftbl_ptr = kernel_func_table;
           ftbl_ptr < kernel_func_table + NoOfKernelFuncs;
           ftbl_ptr++)  {
        if (func_type == (ftbl_ptr->func_type & ~FF_LEARN_FUNC))  {
          func_descr->no_of_input_parameters = ftbl_ptr->no_of_input_parameters;
          func_descr->no_of_output_parameters = ftbl_ptr->no_of_output_parameters;
          func_descr->function = ftbl_ptr->function;
          strcpy( func_descr->func_name, ftbl_ptr->func_name );
          func_descr->number = ftbl_ptr - kernel_func_table;
          return( KRERR_NO_ERROR );
        }
      }

      func_descr->number = 0;
      KernelErrorCode = KRERR_MISSING_DEFAULT_FUNC;
      break;

    default:
      KernelErrorCode = KRERR_PARAMETERS;
  }

  return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krf_getFuncInfo

  PURPOSE  : 
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/

krui_err  krf_getFuncInfo(int mode, struct FuncInfoDescriptor *func_descr)
{
  switch (specialNetworkType)  {
    case NET_TYPE_GENERAL:
      (void) krf_getInternalFuncInfo( mode, func_descr );
      break;

#ifdef MASPAR_KERNEL

    case NET_TYPE_FF1:

#ifndef MASPAR_KERNEL_EMULATION
      
  
      KernelErrorCode = callRequest( krmp_getMasParFuncInfo,
                                     sizeof (int) +
                                     sizeof (struct FuncInfoDescriptor *),
                                     mode, func_descr );
#else
      (void) krf_getInternalFuncInfo( mode, func_descr );
#endif

      break;
#endif

    default:
      KernelErrorCode = KRERR_PARAMETERS;
  }

  return( KernelErrorCode );
}

/*****************************************************************************
  FUNCTION : krf_getNoOfFuncs

  PURPOSE  : Returns the number of functions in the function table
  NOTES    :

  RETURNS  : Returns the number of functions in the function table
  UPDATE   : 
******************************************************************************/
int  krf_getNoOfFuncs(void)
{
  struct FuncInfoDescriptor  functionDescr;


  KernelErrorCode = krf_getFuncInfo( GET_NO_OF_FUNCS, &functionDescr );
  return( functionDescr.number );
}

/*****************************************************************************
  FUNCTION : krf_funcSearch

  PURPOSE  : seaches for the given function (name and type) and returns a pointer to
             this function. krf_funcSearch(...)
  NOTES    :

  RETURNS  : TRUE if the given function was found, FALSE otherwise.
  UPDATE   : 
******************************************************************************/
bool  krf_funcSearch(char *func_name, int func_type, FunctionPtr *func_ptr)
{
  struct FuncInfoDescriptor  functionDescr;

  functionDescr.func_type = func_type;
  strcpy( functionDescr.func_name, func_name );

  KernelErrorCode = krf_getFuncInfo( SEARCH_FUNC, &functionDescr );

  if (KernelErrorCode != KRERR_NO_ERROR)  return( FALSE );
  *func_ptr = functionDescr.function;
  return( TRUE );
}
/*****************************************************************************
  FUNCTION : krf_getFuncName

  PURPOSE  : 
  NOTES    :

  RETURNS  : Returns the name of the given function
  UPDATE   : 
******************************************************************************/
char  *krf_getFuncName(FunctionPtr func_ptr)
{
  static struct FuncInfoDescriptor  functionDescr;

  functionDescr.function = func_ptr;

  KernelErrorCode = krf_getFuncInfo( GET_FUNC_NAME, &functionDescr );

  if (functionDescr.func_type == 0)  return( NULL );

  return( functionDescr.func_name );
}


/*#################################################

GROUP: Functions for managing current and
       default network functions

#################################################*/
/*****************************************************************************
  FUNCTION : krf_getCurrentNetworkFunc

  PURPOSE  : 
  NOTES    :

  RETURNS  : returns the name of the current network function
  UPDATE   : 
******************************************************************************/
char  *krf_getCurrentNetworkFunc(int type)
{
  int  sel;
  static struct FuncInfoDescriptor  func_descr;


  KernelErrorCode = KRERR_NO_ERROR;

  if (specialNetworkType == NET_TYPE_GENERAL)  sel = 0;
  else  sel = NO_OF_FUNC_TYPES;

  switch (type)  {
    case  UPDATE_FUNC:
      break;
    case  LEARN_FUNC:
      sel += 1;
      break;
    case  INIT_FUNC:
      sel += 2;
      break;
    case  OUT_FUNC:
      sel += 3;
      break;
    case  ACT_FUNC:
      sel += 4;
      break;
    case  SITE_FUNC:
      sel += 5;
      break;
    case  UPDATE_FUNC | BENCH_FUNC:
      sel += 6;
      break;
    case  LEARN_FUNC | BENCH_FUNC:
      sel += 7;
      break;
    case  LEARN_FUNC | FF_LEARN_FUNC:
      sel += 8;
      break;
    case  PRUNING_FUNC:
      sel += 9;
      break;
    case  TEST_FUNC:  
      sel += 10;
      break;
   default:
     KernelErrorCode = KRERR_PARAMETERS;
     return( NULL );
  }

  if (netFuncInit[sel])  return( &CurrNetworkFunc[sel][0] );

  func_descr.func_type = type;
  if (krf_getFuncInfo( GET_DEFAULT_FUNC, &func_descr ) == KRERR_NO_ERROR)
    return( func_descr.func_name );

  return( NULL );
}

/*****************************************************************************
  FUNCTION : krf_storeCurrentNetworkFunc

  PURPOSE  : 
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/

static void  krf_storeCurrentNetworkFunc(char *function_name, int type)
{
  int  sel;

  if (specialNetworkType == NET_TYPE_GENERAL)  sel = 0;
  else  sel = 3;


  switch (type)  {
    case  UPDATE_FUNC:
      break;
    case  LEARN_FUNC:
      sel += 1;
      break;
    case  INIT_FUNC:
      sel += 2;
      break;
    case  OUT_FUNC:
      sel += 3;
      break;
    case  ACT_FUNC:
      sel += 4;
      break;
    case  SITE_FUNC:
      sel += 5;
      break;
    case  UPDATE_FUNC | BENCH_FUNC:
      sel += 6;
      break;
    case  LEARN_FUNC | BENCH_FUNC:
      sel += 7;
      break;
    case  LEARN_FUNC | FF_LEARN_FUNC:
      sel += 8;
      break;
    case  PRUNING_FUNC:
      sel += 9;
      break;
    case  TEST_FUNC: 
      sel += 10;
      break;
   default:
     KernelErrorCode = KRERR_PARAMETERS;
     return;
    }

  netFuncInit[sel] = TRUE;
  strcpy( &CurrNetworkFunc[sel][0], function_name );
  return;
}



/*****************************************************************************
  FUNCTION : krf_setCurrentNetworkFunc

  PURPOSE  : sets the current network function.
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
krui_err  krf_setCurrentNetworkFunc(char *function_name, int type)
{
  FunctionPtr func_ptr;
  bool	func_has_changed;
  char  *current_func;

  KernelErrorCode = KRERR_NO_ERROR;

  if (!krf_funcSearch( function_name, type, &func_ptr ))
    return( KernelErrorCode );
  if ((current_func = krf_getCurrentNetworkFunc( type )) == NULL)
    return( KernelErrorCode );

  func_has_changed = strcmp( function_name, current_func ) != 0;

  switch (type)  {
    case  UPDATE_FUNC:
      if (func_has_changed)
	{  /*  update function has changed, initialize the network  */
        NetInitialize = TRUE;
        krf_storeCurrentNetworkFunc( function_name, type );
      }

      return( KernelErrorCode );

    case  LEARN_FUNC:
      if (func_has_changed)
	{  /*  Learning function has changed  */
	LearnFuncHasChanged = TRUE;
        krf_storeCurrentNetworkFunc( function_name, type );
        krf_storeCurrentNetworkFunc( function_name, TEST_FUNC ); 
      }

      return( KernelErrorCode );

    case PRUNING_FUNC:
      if (func_has_changed)
        krf_storeCurrentNetworkFunc (function_name, type);
      break;

    case (FF_LEARN_FUNC | LEARN_FUNC):
      if (func_has_changed)
        krf_storeCurrentNetworkFunc (function_name, type);
        krf_storeCurrentNetworkFunc( function_name, TEST_FUNC );
      break;

    case  INIT_FUNC:
    case  OUT_FUNC:
    case  ACT_FUNC:
    case  SITE_FUNC:
      krf_storeCurrentNetworkFunc( function_name, type );
      break;

   default:
      KernelErrorCode = KRERR_PARAMETERS;
      return( KernelErrorCode );
  }

  return( KernelErrorCode );
}





/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_funcs.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel: Routines for User-Function Management
		               (Function Prototypes)
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 12.02.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:43 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _KR_FUNCS_DEFINED_
#define  _KR_FUNCS_DEFINED_

/* begin global definition section */

krui_err  krf_getFuncInfo( int  mode, struct FuncInfoDescriptor  *func_descr );

/*  Returns the number of functions in the function table
*/
int krf_getNoOfFuncs( void );

/*  seaches for the given function (name and type) and returns a pointer to
    this function. krf_funcSearch(...) returns TRUE if the given function was
    found, FALSE otherwise.
*/
bool krf_funcSearch( char *func_name, int  func_type, FunctionPtr  *func_ptr );

/*  Returns the name of the given function
*/
char *krf_getFuncName( FunctionPtr func_ptr );

/*  returns the name of the current network function
*/
char  *krf_getCurrentNetworkFunc( int  type );

/*  sets the current network function.
*/
krui_err  krf_setCurrentNetworkFunc( char  *function_name, int  type );


/* end global definition section */

/* begin private definition section */

/*#################################################

GROUP: Local Vars

#################################################*/


#define  NO_OF_FUNC_TYPES  11
#define  NO_OF_KERNELS  2
static char  CurrNetworkFunc[NO_OF_FUNC_TYPES * NO_OF_KERNELS][FUNCTION_NAME_MAX_LEN];

static bool  netFuncInit[NO_OF_FUNC_TYPES * NO_OF_KERNELS] = { 
                         FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                         FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE  };

static krui_err  krf_getInternalFuncInfo(int mode, struct FuncInfoDescriptor *func_descr);
static void  krf_storeCurrentNetworkFunc(char *function_name, int type);


/* end private definition section */

#endif 



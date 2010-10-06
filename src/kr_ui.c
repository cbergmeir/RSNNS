/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_ui.c,v $
  SHORTNAME      : kr_ui.c
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel User Interface
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 27.02.90

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.51 $
  LAST CHANGE    : $Date: 1998/05/25 16:00:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <string.h>
#include <memory.h>
#include <math.h>

#ifndef rand
#include "random.h"      /*  Randomize Library Function Prototypes  */
#endif

#include "kr_typ.h"      /*  Kernel Types and Constants  */
#include "glob_typ.h"
#include "kr_const.h"    /*  Constant Declarators for SNNS-Kernel  */
#include "kr_def.h"      /*  Default Values  */

#include "kr_io.h"       /*  Kernel I/O-Routines */
#include "kr_funcs.h"    /*  Function Prototypes */
#include "kernel.h"      /*  Function Prototypes */
#include "kr_mem.h"      /*  Function Prototypes */
#include "kr_ui.ph"      /*  Interface function prototypes  */
#include "version.h"     /*  Version and Patchlevel  */
#include "kr_mac.h"      /*  Kernel Macros  */
#include "kr_newpattern.h"   /* new style pattern handling */
#include "kr_inversion.h" /* inversion algorithm routines */
#include "cc_glob.h"
#include "cc_display.h"
#include "trans_f.h"
#include "learn_f.h"

#ifdef MASPAR_KERNEL

#include "kr_feedf.h"    /*  Function Prototypes */

#endif

/************ Random seed storage **************/
long randomSeedVal=0;
/***********************************************/

/*****************************************************************************
  FUNCTION : krui_getNoOfUnits

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the number of units used by the network.
  UPDATE   :
******************************************************************************/
int krui_getNoOfUnits(void)
{
    return( NoOfUnits );
}


/*****************************************************************************
  FUNCTION : krui_getFirstUnit

  PURPOSE  : Initializes the first available unit for access. If the unit has
             sites, the first site will be set to the current site.
  NOTES    :

  RETURNS  : Returns the unit no. of the first unit or 0 if no units available.
  UPDATE   :
******************************************************************************/
int  krui_getFirstUnit(void)
{
    return( kr_getUnit( FIRST ) );
}

/*****************************************************************************
  FUNCTION : krui_getNextUnit

  PURPOSE  : Initializes the next available unit for access. If the unit has 
             sites, the first site will be set to the current site.
  NOTES    :

  RETURNS  : Returns the unit no. of the next unit or 0 if no more units 
             available.
  UPDATE   :
******************************************************************************/
int  krui_getNextUnit(void)
{
    return( kr_getUnit( NEXT ) );
}

/*****************************************************************************
  FUNCTION : krui_getCurrentUnit

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the no. of the current unit or 0 if no units available
  UPDATE   :
******************************************************************************/
int  krui_getCurrentUnit(void)
{
    return( kr_getUnit( CURRENT ) );
}

/*****************************************************************************
  FUNCTION : krui_setCurrentUnit

  PURPOSE  : Initializes a unit for access. If the unit has sites, the first site
    will be set to the current site.
  NOTES    :

  RETURNS  : Returns error code if the given unit doesn't exist, 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_setCurrentUnit(int unit_no)
{
    return( kr_setCurrUnit( unit_no ) );
}

/*****************************************************************************
  FUNCTION : krui_getUnitName

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the name of the unit. (NULL if not available)
  UPDATE   :
******************************************************************************/
char  *krui_getUnitName(int UnitNo)
{
    struct Unit   *unit_ptr;

    if ((unit_ptr = kr_getUnitPtr( UnitNo ) ) == NULL)
        return( NULL );         /*  invalid unit #  */

    return( unit_ptr->unit_name );
}


/*****************************************************************************
  FUNCTION : krui_setUnitName

  PURPOSE  : Sets the name of the unit <UnitNo>.
             If the unit_name pointer is NULL, the unit's symbol will be deleted.
             Function has no effect on the current unit.
  NOTES    :

  RETURNS  : Returns error code if memory allocation fails, 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_setUnitName(int unit_no, char *unit_name)
{
    struct Unit   *unit_ptr;
    char    *str_ptr;


    KernelErrorCode = KRERR_NO_ERROR;
    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    NetModified = TRUE;

    if (unit_name == NULL)
        {
            krm_NTableReleaseSymbol( unit_ptr->unit_name, UNIT_SYM );
            unit_ptr->unit_name = NULL;
            return( KernelErrorCode );
        }

    if (!kr_symbolCheck( unit_name ))  return( KernelErrorCode );

    if ( (str_ptr = krm_NTableInsertSymbol( unit_name, UNIT_SYM ) ) == NULL)
        return( KernelErrorCode );

    unit_ptr->unit_name = str_ptr;

    return( KernelErrorCode );
}

/*****************************************************************************
  FUNCTION : krui_searchUnitName

  PURPOSE  : Searches for a unit with the given name
  NOTES    :

  RETURNS  : Returns the first unit no. if a unit with the given name was found,
             0 otherwise.
  UPDATE   :
******************************************************************************/
int  krui_searchUnitName(char *unit_name)
{
    struct  NameTable  *n_ptr;

    if (NoOfUnits <= 0)  {
        UICurrentNameSearchUnitNo = 0;
        return( (int) KRERR_NO_UNITS ); /*  No Units defined  */
    }

    if ((n_ptr = krm_NTableSymbolSearch( unit_name, UNIT_SYM )) == NULL)  {
        UICurrentNameSearchUnitNo = 0;
        return( 0 );
    }

    UICurrentNameSearchUnitSymbolPtr = n_ptr->Entry.symbol;
    UICurrentNameSearchUnitNo = 
	kr_unitNameSearch( MinUnitNo, UICurrentNameSearchUnitSymbolPtr );

    return( UICurrentNameSearchUnitNo );
}


/*****************************************************************************
  FUNCTION : krui_searchNextUnitName

  PURPOSE  : Searches for the next unit with the given name.
  NOTES    : Call krui_searchUnitName( unit_name ) before calling krui_searchNextUnitName().
             Returns error code if no units defined.

  RETURNS  : Returns the first unit no. if a unit with the given name was found,
             0 otherwise.
  UPDATE   :
******************************************************************************/
int  krui_searchNextUnitName(void)
{

    if (NoOfUnits <= 0) {
        UICurrentNameSearchUnitNo = 0;
        return( KRERR_NO_UNITS ); /*  No Units defined  */
    }

    if (UICurrentNameSearchUnitNo != 0) {
        UICurrentNameSearchUnitNo = 
	    kr_unitNameSearch( UICurrentNameSearchUnitNo + 1, 
			       UICurrentNameSearchUnitSymbolPtr );
        return( UICurrentNameSearchUnitNo );
    }
    else
        return( 0 );
}

/*****************************************************************************
  FUNCTION : krui_getUnitOutFuncName

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the output function name of the unit.
  UPDATE   :
******************************************************************************/
char  *krui_getUnitOutFuncName(int UnitNo)
{
    struct Unit   *unit_ptr;
    static char  out_func_name[FUNCTION_NAME_MAX_LEN];

    if ( (unit_ptr = kr_getUnitPtr( UnitNo ) ) == NULL)
        return( NULL );         /*  invalid unit #  */

    if(unit_ptr->out_func == OUT_Custom_Python) {
    	strncpy(out_func_name, 
		krf_getFuncName((FunctionPtr) unit_ptr->python_out_func),
		sizeof(out_func_name)-1);
	out_func_name[sizeof(out_func_name)-1]=0;		
    } else strcpy( out_func_name, krf_getFuncName( (FunctionPtr) unit_ptr->out_func ));

    return( out_func_name );
}

/*****************************************************************************
  FUNCTION : krui_setUnitOutFunc(

  PURPOSE  : he unit's FType will be set to 0, i.e. the
             unit's functionality type will be deleted.
             Function has no effect on the current unit.
  NOTES    :

  RETURNS  : returns 0, if the function is a valid output function,
             error code otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_setUnitOutFunc(int unit_no, char *unitOutFuncName)
{
    struct Unit   *unit_ptr;
    FunctionPtr func_ptr;


#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    if ( !krf_funcSearch( unitOutFuncName, OUT_FUNC, &func_ptr ))
        return( KernelErrorCode );
    
    unit_ptr->out_func = (OutFuncPtr) func_ptr;
    unit_ptr->Ftype_entry = NULL;
    if((OutFuncPtr)func_ptr == OUT_Custom_Python) {
    	unit_ptr->python_out_func 
    			= kr_findPythonFunction(unitOutFuncName, OUT_FUNC);
    }

    NetModified = TRUE;

    return( KRERR_NO_ERROR );
}

/*****************************************************************************
  FUNCTION : krui_getUnitActFuncName

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the activation function name of the unit.
  UPDATE   :
******************************************************************************/
char  *krui_getUnitActFuncName(int UnitNo)
{
    struct Unit   *unit_ptr;
    static char  act_func_name[FUNCTION_NAME_MAX_LEN];

    if ( (unit_ptr = kr_getUnitPtr( UnitNo ) ) == NULL)
        return( NULL );         /*  invalid unit #  */

    if(unit_ptr->act_func == ACT_Custom_Python) {
    	strncpy(act_func_name, 
		krf_getFuncName((FunctionPtr) unit_ptr->python_act_func),
		sizeof(act_func_name)-1);
	act_func_name[sizeof(act_func_name)-1]=0;		
    } else strcpy( act_func_name, krf_getFuncName( (FunctionPtr) unit_ptr->act_func ));

    return( act_func_name );
}

/*****************************************************************************
  FUNCTION : krui_setUnitActFunc

  PURPOSE  : The unit's FType will be set to 0, i.e. the
             unit's functionality type will be deleted.
             Function has no effect on the current unit.
  NOTES    :

  RETURNS  : returns 0, if the function is a valid activation function,
             error code otherwise.
  UPDATE   :
******************************************************************************/

extern FlintType ACT_Custom_Python(struct Unit *unit_ptr);

krui_err  krui_setUnitActFunc(int unit_no, char *unitActFuncName)
{
    struct Unit   *unit_ptr;
    FunctionPtr   act_func_ptr, act_deriv_func_ptr, act_2_deriv_func_ptr;

#ifdef MASPAR_KERNE
    MASPAR_FF1_VALIDATE_OP;
#endif

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );
    if ( !krf_funcSearch( unitActFuncName, ACT_FUNC, &act_func_ptr ))
        return( KernelErrorCode );
    /*  set the derivation function of the activation function  */
    if( !krf_funcSearch( unitActFuncName, ACT_DERIV_FUNC, &act_deriv_func_ptr ))
        return( KernelErrorCode );
    /*  set the second derivation function of the activation function  */
    if ( !krf_funcSearch( unitActFuncName, ACT_2_DERIV_FUNC, 
			  &act_2_deriv_func_ptr ))
        return( KernelErrorCode );


    unit_ptr->act_func = (ActFuncPtr) act_func_ptr;
    unit_ptr->act_deriv_func = (ActDerivFuncPtr) act_deriv_func_ptr;
    unit_ptr->act_2_deriv_func = (ActDerivFuncPtr) act_2_deriv_func_ptr;
    unit_ptr->Ftype_entry = NULL;

    if((ActFuncPtr)act_func_ptr == ACT_Custom_Python) {
	 unit_ptr->python_act_func =
	 	kr_findPythonFunction(unitActFuncName,ACT_FUNC);
	 unit_ptr->python_act_deriv_func =
	 	kr_findPythonFunction(unitActFuncName,ACT_DERIV_FUNC);
	 unit_ptr->python_act_2_deriv_func =
	 	kr_findPythonFunction(unitActFuncName,ACT_2_DERIV_FUNC);
    }

    NetModified = TRUE;

    return( KRERR_NO_ERROR );
}

/*****************************************************************************
  FUNCTION : krui_getUnitFTypeName

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns the functionality type name of the unit.
             Function has no effect on the current unit.
             Returns NULL if unit has no FType.
  UPDATE   :
******************************************************************************/
char  *krui_getUnitFTypeName(int UnitNo)
{
    struct FtypeUnitStruct    *Ftype_entry;
    struct Unit   *unit_ptr;

    if ( (unit_ptr = kr_getUnitPtr( UnitNo ) ) == NULL)
        return( NULL );         /*  invalid unit #  */

    if ( (Ftype_entry = unit_ptr->Ftype_entry) == NULL)
        return( NULL );

    return( Ftype_entry->Ftype_symbol->Entry.symbol );
}




/*****************************************************************************
  FUNCTION : krui_getUnitActivation

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns the activation value of the unit.
  UPDATE   :
******************************************************************************/
FlintType  krui_getUnitActivation(int UnitNo)
{

    if KERNEL_STANDARD {
        return( kr_getUnitValues( UnitNo, SEL_UNIT_ACT ) );
    }
    else  {

#ifdef MASPAR_KERNEL

        return( krff_getUnitValues( UnitNo, SEL_UNIT_ACT ) );

#else

        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( 0.0 );

#endif
    }
}


/*****************************************************************************
  FUNCTION : krui_setUnitActivation

  PURPOSE  : Sets the activation value of the unit.
             Function has no effect on the current unit.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_setUnitActivation(int UnitNo, FlintTypeParam unit_activation)
{
    krui_err err = KRERR_NO_ERROR;

    if KERNEL_STANDARD  {
        err = kr_setUnitValues( UnitNo, SEL_UNIT_ACT, unit_activation );
    }
    else  {

#ifdef MASPAR_KERNEL

        (void) krff_setUnitValues( UnitNo, SEL_UNIT_ACT, unit_activation );

#else

        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;

#endif
    }
    return(err);
}

/*****************************************************************************
  FUNCTION : krui_getUnitInitialActivation

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns the initial activation value of the unit.
  UPDATE   :
******************************************************************************/
FlintType  krui_getUnitInitialActivation(int UnitNo)
{

    if KERNEL_STANDARD
        return( kr_getUnitValues( UnitNo, SEL_UNIT_IACT ) );
    else  {

#ifdef MASPAR_KERNEL

        return( krff_getUnitValues( UnitNo, SEL_UNIT_IACT ) );

#else

        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( 0.0 );

#endif
    }
}

/*****************************************************************************
  FUNCTION : krui_setUnitInitialActivation

  PURPOSE  : Sets the initial activation value of the unit.
             Function has no effect on the current unit.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_setUnitInitialActivation(int UnitNo, 
				    FlintTypeParam unit_i_activation)
{

    if KERNEL_STANDARD
        kr_setUnitValues( UnitNo, SEL_UNIT_IACT, unit_i_activation );
    else  {

#ifdef MASPAR_KERNEL

        krff_setUnitValues( UnitNo, SEL_UNIT_IACT, unit_i_activation );

#else

        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;

#endif
    }
}


/*****************************************************************************
  FUNCTION : krui_getUnitOutput

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns the output value of the unit.
  UPDATE   :
******************************************************************************/
FlintType  krui_getUnitOutput(int UnitNo)
{

    if KERNEL_STANDARD
        return( kr_getUnitValues( UnitNo, SEL_UNIT_OUT ) );
    else  {

#ifdef MASPAR_KERNEL

        return( krff_getUnitValues( UnitNo, SEL_UNIT_OUT ) );

#else

        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( 0.0 );

#endif
    }
}

/*****************************************************************************
  FUNCTION : krui_setUnitOutput

  PURPOSE  : Sets the output value of the unit.
             Function has no effect on the current unit.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_setUnitOutput(int unit_no, FlintTypeParam unit_output)
{

    if KERNEL_STANDARD  {
        kr_setUnitValues( unit_no, SEL_UNIT_OUT, unit_output );
    }
    else  {

#ifdef MASPAR_KERNEL

        krff_setUnitValues( unit_no, SEL_UNIT_OUT, unit_output );

#else

        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;

#endif
    }

    return( KernelErrorCode );
}

/*****************************************************************************
  FUNCTION : krui_getUnitBias

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns the bias value of the unit.
  UPDATE   :
******************************************************************************/
FlintType  krui_getUnitBias(int UnitNo)
{

    if KERNEL_STANDARD  {
        return( kr_getUnitValues( UnitNo, SEL_UNIT_BIAS ) );
    }
    else  {

#ifdef MASPAR_KERNEL

        return( krff_getUnitValues( UnitNo, SEL_UNIT_BIAS ) );

#else

        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( 0.0 );

#endif
    }
}
/*****************************************************************************
  FUNCTION : krui_setUnitBias

  PURPOSE  : Sets the bias value of the unit.
             Function has no effect on the current unit.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_setUnitBias(int UnitNo, FlintTypeParam unit_bias)
{

    if KERNEL_STANDARD  {
        kr_setUnitValues( UnitNo, SEL_UNIT_BIAS, unit_bias );
    }
    else  {

#ifdef MASPAR_KERNEL

        krff_setUnitValues( UnitNo, SEL_UNIT_BIAS, unit_bias );

#else

        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;

#endif
    }
}

/****************************************************************************
  FUNCTION : krui_getUnitValueA
  PURPOSE  :
  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns Value A of the unit.
  UPDATE   :
****************************************************************************/
FlintType  krui_getUnitValueA(int UnitNo)
{

    if KERNEL_STANDARD  {
        return( kr_getUnitValues( UnitNo, SEL_UNIT_VALA ) );
    }
    else  {

#ifdef MASPAR_KERNEL

        return( krff_getUnitValues( UnitNo, SEL_UNIT_VALA ) );

#else

        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( 0.0 );

#endif
    }
}

/******************************************************************************
  FUNCTION : krui_setUnitValueA

  PURPOSE  : Sets value a of the unit.
  NOTES    : Function has no effect on the current unit.


  RETURNS  :
  UPDATE   :
*****************************************************************************/
void  krui_setUnitValueA(int UnitNo, FlintTypeParam unit_valueA)
{

    if KERNEL_STANDARD  {
        kr_setUnitValues( UnitNo, SEL_UNIT_VALA, unit_valueA );
    }
    else  {

#ifdef MASPAR_KERNEL

        krff_setUnitValues( UnitNo, SEL_UNIT_VALA, unit_valueA );

#else

        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;

#endif
    }
}

/*****************************************************************************
  FUNCTION : krui_getUnitSubnetNo

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.
             The range of the subnet no. is -32736...+32735

  RETURNS  : Returns the subnet number of the unit.
  UPDATE   :
******************************************************************************/
int  krui_getUnitSubnetNo(int UnitNo)
{
    struct Unit   *unit_ptr;

    if ( (unit_ptr = kr_getUnitPtr( UnitNo ) ) == NULL)
        return( 0 );            /*  invalid unit #  */

    return( (int) unit_ptr->subnet_no );
}


/*****************************************************************************
  FUNCTION : krui_setUnitSubnetNo

  PURPOSE  : Sets the subnet number of the unit.
             Function has no effect on the current unit.
  NOTES    : The range of the subnet no. is -32736...+32735

  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_setUnitSubnetNo(int UnitNo, int subnet_no)
{
    struct Unit   *unit_ptr;

    if ( (unit_ptr = kr_getUnitPtr( UnitNo ) ) == NULL)
        return;                 /*  invalid unit #  */

    NetModified = TRUE;

    unit_ptr->subnet_no = (short) subnet_no;
}


/*****************************************************************************
  FUNCTION : krui_getUnitLayerNo

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.
             The range of the layer no. is -32736...+32735

  RETURNS  : Returns the layer number of the unit.
  UPDATE   :
******************************************************************************/
unsigned short  krui_getUnitLayerNo(int UnitNo)
{
    struct Unit   *unit_ptr;

    if ( (unit_ptr = kr_getUnitPtr( UnitNo ) ) == NULL)
        return( 0 );            /*  invalid unit #  */

    return( (unsigned short) unit_ptr->layer_no );
}


/*****************************************************************************
  FUNCTION : krui_setUnitLayerNo

  PURPOSE  : Sets the layer number of the unit.
  NOTES    : Function has no effect on the current unit.
             The range of the layer no. is -32736...+32735
  RETURNS  :
  UPDATE   :
******************************************************************************/
void    krui_setUnitLayerNo(int UnitNo, int layer_no)
{
    struct Unit   *unit_ptr;

    if ( (unit_ptr = kr_getUnitPtr( UnitNo ) ) == NULL)
        return;                 /*  invalid unit #  */

    NetModified = TRUE;

    unit_ptr->layer_no = (unsigned short) layer_no;
}


/*****************************************************************************
  FUNCTION : krui_getUnitPosition

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns the position of the unit.
  UPDATE   :
******************************************************************************/
void    krui_getUnitPosition(int UnitNo, struct PosType *position)
{
    struct Unit   *unit_ptr;

    if ( (unit_ptr = kr_getUnitPtr( UnitNo ) ) == NULL)
        return;                 /*  invalid unit #  */

    position->x = unit_ptr->unit_pos.x;
    position->y = unit_ptr->unit_pos.y;
    position->z = unit_ptr->unit_pos.z;
}


/*****************************************************************************
  FUNCTION : krui_setUnitPosition

  PURPOSE  : Sets the position of the unit.
  NOTES    : Function has no effect on the current unit

  RETURNS  :
  UPDATE   :
******************************************************************************/
void    krui_setUnitPosition(int UnitNo, struct PosType *position)
{
    struct Unit   *unit_ptr;

    if ( (unit_ptr = kr_getUnitPtr( UnitNo ) ) == NULL)
        return;                 /*  invalid unit #  */

    unit_ptr->unit_pos.x = position->x;
    unit_ptr->unit_pos.y = position->y;
    unit_ptr->unit_pos.z = position->z;
}


/*****************************************************************************
  FUNCTION : krui_getUnitNoAtPosition

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.
             This function is slow because it uses linear search to
             find the unit with the given position.
  REMARK   : getUnitNoAtPosition is for downward compatibility only.
             Do not use this function in future applications.

  RETURNS  : Returns the unit no. at the given position and subnet number or 0, if
             no unit exists at this position
  UPDATE   :
******************************************************************************/
int   krui_getUnitNoAtPosition(struct PosType *position, int subnet_no)
{
    register int       i;
    register short     x, y, net_no;
    struct Unit     *unit_ptr;

    x = position->x;
    y = position->y;

    net_no = (short) subnet_no;

    for (i = 1, unit_ptr = unit_array + 1;
         i <= MaxUnitNo;
         i++, unit_ptr++)
        {
            if ( UNIT_IN_USE( unit_ptr ) &&
                (unit_ptr->subnet_no == net_no) &&
                (unit_ptr->unit_pos.x == x) && (unit_ptr->unit_pos.y == y) )

                return( i );
        }

    return( 0 );
}


/*****************************************************************************
  FUNCTION : krui_getUnitNoNearPosition

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.
             This function is slow because it uses linear search to
             find the unit with the given position.

  RETURNS  : Returns the unit no. near the given position and the specified
             subnet no or 0, if no unit exists at this position.
  UPDATE   :
******************************************************************************/
int  krui_getUnitNoNearPosition(struct PosType *position, int subnet_no, int range, int gridWidth)
{
    register int       i, devit, width;
    register short     x, y, net_no;
    struct Unit        *unit_ptr;

    x = position->x;
    y = position->y;
    net_no = (short) subnet_no;
    devit = range;
    width = gridWidth;

    for (i = 1, unit_ptr = unit_array + 1; i <= MaxUnitNo;
         i++, unit_ptr++)
        {
            if ( UNIT_IN_USE( unit_ptr ) &&
                (unit_ptr->subnet_no == net_no) &&
                (abs( unit_ptr->unit_pos.x - x) * width <= devit) &&
                (abs( unit_ptr->unit_pos.y - y) * width <= devit) )
                return( i );
        }

    return( 0 );
}


/*****************************************************************************
  FUNCTION : krui_getUnitTType

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns the topologic type, i.e. input, output, hidden
  UPDATE   :
******************************************************************************/
int  krui_getUnitTType(int unit_no)
{
    struct Unit   *unit_ptr;

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    return( kr_flags2TType( (int) unit_ptr->flags & UFLAG_TTYP_PAT ) );
}


/*****************************************************************************
  FUNCTION : krui_setUnitTType

  PURPOSE  : Sets the topologic type of the unit.
             Function has no effect on the current unit.
  NOTES    :

  RETURNS  : Returns error code if topologic type or unit number is invalid.
  UPDATE   :
******************************************************************************/
krui_err  krui_setUnitTType(int unit_no, int UnitTType)
{

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    return( kr_unitSetTType( unit_no, UnitTType ) );
}


/*****************************************************************************
  FUNCTION : krui_freezeUnit

  PURPOSE  : Freezes the output of a unit, i.e. the unit is disabled.
             Function has no effect on the current unit.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_freezeUnit(int unit_no)
{
    struct Unit   *unit_ptr;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    unit_ptr->flags &= (~UFLAG_ENABLED);
    return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : krui_unfreezeUnit

  PURPOSE  : Enables a previosly freezed unit.
             Function has no effect on the current unit.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_unfreezeUnit(int unit_no)
{
    struct Unit   *unit_ptr;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    unit_ptr->flags |= UFLAG_ENABLED;
    return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : krui_isUnitFrozen

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns true, if unit is frozen
  UPDATE   :
******************************************************************************/
bool  krui_isUnitFrozen(int unit_no)
{
    struct Unit   *unit_ptr;

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    return( (unit_ptr->flags & UFLAG_ENABLED) == 0 );
}


/*****************************************************************************
  FUNCTION : krui_getUnitInputType

  PURPOSE  :
  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns the input type of the unit:
             NO_INPUTS    : if the unit has not inputs (at least not now)
             SITES        : if the unit has one or more sites (and no direct 
	                    input links !)
             DIRECT_LINKS : if the unit has direct input links (and no sites !)

  UPDATE   :
******************************************************************************/
int  krui_getUnitInputType(int unit_no)
{
    struct Unit  *unit_ptr;

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    switch ((int) unit_ptr->flags & UFLAG_INPUT_PAT)    {
      case UFLAG_NO_INP :  return( NO_INPUTS    );
      case UFLAG_SITES  :  return( SITES        );
      case UFLAG_DLINKS :  return( DIRECT_LINKS );

      default :  return( NO_INPUTS );
    }
}


/*****************************************************************************
  FUNCTION : krui_createDefaultUnit

  PURPOSE  :  creates a unit with default properties:
              1. default activation and output functions
              2. default activation and bias
              3. default position, subnet and layer number
              4. no functionality type
              5. no sites
              6. no inputs and outputs
              7. no unit_name
  NOTES    :  See file "kr_def.h" for more details about default presettings.
              Function has no effect on the current unit.

  RETURNS  :  Returns an (negative) error code, if memory allocation fails or
              invalid functions occured. Returns (positive) unit number otherwise.
  UPDATE   :
******************************************************************************/
int  krui_createDefaultUnit(void)
{

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    NetModified = TRUE;
    return( kr_makeDefaultUnit() );
}


/*****************************************************************************
  FUNCTION : krui_createUnit

  PURPOSE  : Creates a user defined unit.
             Returns an (negative) error code, if memory allocation fails or
             invalid functions occured. Returns (positive) unit number otherwise.
             Function has no effect on the current unit.

             Unit has additional default settings:
             1. default position, subnet and layer number
             2. no functionality type
             3. no sites
             4. no inputs and outputs

  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
int  krui_createUnit(char *unit_name, char *out_func_name, char *act_func_name,
		     FlintTypeParam i_act, FlintTypeParam bias)
{

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    return( kr_createUnit( unit_name, out_func_name, act_func_name, 
			   i_act, bias) );
}


/*****************************************************************************
                 UNDOCUMENTED FUNCTION SINCE SNNS VERSION 3.0
                   !!!! DO NOT USE THIS FUNCTION ANY MORE  !!!
                   !!!! USE: krui_deleteUnitList() INSTEAD !!!

  FUNCTION : krui_deleteUnit

  PURPOSE  : Deletes a unit from network. Removes all links to other units.
  NOTES    : !!!! DO NOT USE THIS FUNCTION ANY MORE !!!
             !!!! USE: krui_deleteUnitList() INSTEAD !!!

  RETURNS  : Returns error code if unit doesn't exist.
  UPDATE   :
******************************************************************************/
krui_err  krui_deleteUnit(int unit_no)
{
    struct  Unit      *unit_ptr;


#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    (void) kr_removeUnit( unit_ptr );
    NetModified = TRUE;
    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_deleteUnitList

  PURPOSE  : Deletes 'no_of_units' from the network. The numbers of the
             units that have to be deleted are listed up in an array
             of integers beginning with index 0.
             This array is passed to parameter 'unit_list'.
             Removes all links to and from these units.

  NOTES    : Always use this function instead of krui_deleteUnit()

  RETURNS  : Returns error code if unit doesn't exist.
  UPDATE   :
******************************************************************************/
krui_err  krui_deleteUnitList(int no_of_units, int unit_list[])
{
    struct  Unit      *unit_ptr;
    int               i;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    for (i=0; i < no_of_units; i++) {
        if ((unit_ptr = kr_getUnitPtr( unit_list[i] )) == NULL) {
            return( KernelErrorCode );
        } /*if*/

        (void) kr_removeUnit( unit_ptr );
    } /*for*/

    /* Now perform Garbage Collection */
    kr_forceUnitGC();

    NetModified = TRUE;
    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_createFTypeUnit

  PURPOSE  : creates a unit with FType properties, but:
             1. no inputs and outputs
             2. default position, subnet and layer

  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns the unit number or (negative) error code if memory 
             allocation fails or functionality type isn't defined.
  UPDATE   :
******************************************************************************/
int  krui_createFTypeUnit(char *Ftype_symbol)
{
    int     unit_no;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    unit_no = kr_makeFtypeUnit( Ftype_symbol );
    if (KernelErrorCode != KRERR_NO_ERROR)
        return( KernelErrorCode );

    NetModified = TRUE;
    return( unit_no );
}


/*****************************************************************************
  FUNCTION : krui_setUnitFType

  PURPOSE  : Changes the properties of unit <UnitNo> to FType properties.
             Changes: activation/output function and site name/functions.
  NOTES    : Function has no effect on the current unit.

  RETURNS  : Returns an error code if
             - FType name doesn't exist or
             - unit doesn't exist or
             - memory allocation fails
             otherwise 0.

  UPDATE   :
******************************************************************************/
krui_err  krui_setUnitFType(int unit_no, char *Ftype_symbol)
{
    struct  Unit      *unit_ptr;
    struct  FtypeUnitStruct   *ftype_ptr;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );
    if (!kr_symbolCheck( Ftype_symbol ))
        return( KernelErrorCode );

    if ( (ftype_ptr = krm_FtypeSymbolSearch( Ftype_symbol ) ) == NULL)
        {                       /*  Ftype name isn't defined    */
            KernelErrorCode = KRERR_FTYPE_SYMBOL;
            return( KernelErrorCode );
        }

    NetModified = TRUE;
    kr_changeFtypeUnit( unit_ptr, ftype_ptr );
    return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : krui_copyUnit

  PURPOSE  : Copy a given unit, according to the copy mode
             1. copy unit (with it sites, if available) and input/output links
             2. copy unit (with it sites, if available) and input links
             3. copy unit (with it sites, if available) and output links
             4. copy unit (with it sites, if available) but no input/output link
  NOTES    : Function has no effect on the current unit.
             Copying of output links is slow.
             If return code < 0, an error occured.

  RETURNS  : Returns the unit number of the new unit or error message < 0 
  UPDATE   :
******************************************************************************/
int   krui_copyUnit(int unit_no, int copy_mode)
{
    int   new_unit_no;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    new_unit_no = kr_copyUnit( copy_mode, unit_no );
    if (KernelErrorCode != KRERR_NO_ERROR)
        return( KernelErrorCode );

    return( new_unit_no );
}



/*###########################################################################

GROUP: Functions for manipulation of the Unit-Functionality-List
REMEMBER: The Unit-Functionality-List stores:
            1. unit activation and output functions
            2. if sites:  3. all site functions
                          4. all site names


#############################################################################*/

/*****************************************************************************
  FUNCTION : krui_setFirstFTypeEntry

  PURPOSE  : Initializes the first FType entry.
  NOTES    :

  RETURNS  : Returns true, if an entry is available
  UPDATE   :
******************************************************************************/
bool  krui_setFirstFTypeEntry(void)
{
    UICurrentFtypeEntry = krm_getFtypeFirstEntry();
    UICurrentFtypeSite = NULL;
    return( UICurrentFtypeEntry != NULL );
}


/*****************************************************************************
  FUNCTION : krui_setNextFTypeEntry

  PURPOSE  : Initializes the next FType entry.
  NOTES    :

  RETURNS  : Returns true, if an entry is available
  UPDATE   :
******************************************************************************/
bool  krui_setNextFTypeEntry(void)
{
    struct  FtypeUnitStruct   *ftype_ptr;

    UICurrentFtypeSite = NULL;
    if ( (ftype_ptr = krm_getFtypeNextEntry() ) != NULL)
        {
            UICurrentFtypeEntry = ftype_ptr;
            return( TRUE );
        }

    return( FALSE );
}


/*****************************************************************************
  FUNCTION : krui_setFTypeEntry

  PURPOSE  : Initializes the FType entry with the given name.
  NOTES    :

  RETURNS  : Returns true, if an entry with this name is available.
  UPDATE   :
******************************************************************************/
bool  krui_setFTypeEntry(char *Ftype_symbol)
{
    struct  FtypeUnitStruct   *ftype_ptr;

    if ( (ftype_ptr = krm_FtypeSymbolSearch( Ftype_symbol ) ) != NULL)
        {
            UICurrentFtypeEntry = ftype_ptr;
            return( TRUE );
        }

    return( FALSE );
}


/*****************************************************************************
  FUNCTION : krui_getFTypeName

  PURPOSE  :
  NOTES    : The FType name is definite and will never be
             a NULL pointer.

  RETURNS  : Returns the name of the current FType entry.
  UPDATE   :
******************************************************************************/
char  *krui_getFTypeName(void)
{

    if (UICurrentFtypeEntry == NULL)
        return( NULL );

    return( UICurrentFtypeEntry->Ftype_symbol->Entry.symbol );
}


/*****************************************************************************
  FUNCTION : krui_setFTypeName

  PURPOSE  : Sets the name of the current FType entry.
  NOTES    : The new FType name have to be definite and must not be
             a NULL pointer.

  RETURNS  : Returns error code if memory allocation fails or Ftype name
             isn't definite, 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_setFTypeName(char *Ftype_symbol)
{
    struct NameTable    *NTable_ptr;

    if (UICurrentFtypeEntry == NULL)
        return( KRERR_FTYPE_ENTRY ); /*  Current Ftype entry isn't defined   */

    if (Ftype_symbol == NULL)
        return( KRERR_FTYPE_NAME ); /*  Ftype name isn't definite   */

    if (!kr_symbolCheck( Ftype_symbol ))
        return( KRERR_SYMBOL ); /*  Symbol pattern invalid */

    if (krm_NTableSymbolSearch( Ftype_symbol, FTYPE_UNIT_SYM ) != NULL)
        return( KRERR_FTYPE_NAME ); /*  Ftype name isn't definite   */

    if((NTable_ptr=krm_NTableCreateEntry(Ftype_symbol, FTYPE_UNIT_SYM)) == NULL)
        return( KRERR_INSUFFICIENT_MEM ); /*  Insufficient memory  */

    krm_NTableReleaseEntry( UICurrentFtypeEntry->Ftype_symbol );

    UICurrentFtypeEntry->Ftype_symbol = NTable_ptr;

    return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : krui_getFTypeActFuncName

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the name of the activation function in the current
             FType entry
  UPDATE   :
******************************************************************************/
char  *krui_getFTypeActFuncName(void)
{
    static char  act_func_name[FUNCTION_NAME_MAX_LEN];

    if (UICurrentFtypeEntry == NULL)
        return( NULL );

    strcpy( act_func_name, 
	    krf_getFuncName( (FunctionPtr) UICurrentFtypeEntry->act_func ) );

    return( act_func_name );
}


/*****************************************************************************
  FUNCTION : krui_setFTypeActFunc

  PURPOSE  :
  NOTES    : All units (in the existing network) of the current Ftype changes
             their activation function.

  RETURNS  : Sets the activation function in the current FType entry
             returns 0, if the function is a valid activation function,
             error code otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_setFTypeActFunc(char *act_func_name)
{
    FunctionPtr  act_func_ptr, act_deriv_func_ptr, act_2_deriv_func_ptr;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    KernelErrorCode = KRERR_NO_ERROR;
    if (UICurrentFtypeEntry == NULL)
        {                       /*  Current Ftype entry isn't defined   */
            KernelErrorCode = KRERR_FTYPE_ENTRY;
            return( KernelErrorCode );
        }

    if ( !krf_funcSearch( act_func_name, ACT_FUNC, &act_func_ptr ) )
        return( KernelErrorCode );
    /*  set the derivation function of the activation function  */
    if ( !krf_funcSearch( act_func_name, ACT_DERIV_FUNC, &act_deriv_func_ptr ))
        return( KernelErrorCode );
    /*  set the second derivation function of the activation function  */
    if ( !krf_funcSearch( act_func_name, ACT_2_DERIV_FUNC, 
			  &act_2_deriv_func_ptr ))
        return( KernelErrorCode );

    UICurrentFtypeEntry->act_func = (ActFuncPtr) act_func_ptr;
    UICurrentFtypeEntry->act_deriv_func = (ActDerivFuncPtr) act_deriv_func_ptr;
    UICurrentFtypeEntry->act_2_deriv_func = 
	(ActDerivFuncPtr) act_2_deriv_func_ptr;

    kr_changeFtypeUnits( UICurrentFtypeEntry );
    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_getFTypeOutFuncName

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the name of the output function in the current
             FType entry
  UPDATE   :
******************************************************************************/

char  *krui_getFTypeOutFuncName(void)
{
    static char  out_func_name[FUNCTION_NAME_MAX_LEN];

    if (UICurrentFtypeEntry == NULL)
        return( NULL );

    strcpy( out_func_name, 
	    krf_getFuncName( (FunctionPtr) UICurrentFtypeEntry->out_func ) );

    return( out_func_name );
}


/*****************************************************************************
  FUNCTION : krui_setFTypeOutFunc

  PURPOSE  : Sets the output function in the current FType entry
  NOTES    : All units (in the existing network) of the current Ftype changes
             their output function.

  RETURNS  : Returns 0, if the function is a valid output function,
             error code otherwise.
  UPDATE   :
******************************************************************************/
krui_err   krui_setFTypeOutFunc(char *out_func_name)
{
    FunctionPtr  out_func_ptr;

    KernelErrorCode = KRERR_NO_ERROR;
    if (UICurrentFtypeEntry == NULL)
        {   /*  Current Ftype entry isn't defined   */
            KernelErrorCode = KRERR_FTYPE_ENTRY;
            return( KernelErrorCode );
        }

    if ( !krf_funcSearch( out_func_name, OUT_FUNC, &out_func_ptr ) )
        return( KernelErrorCode );

    UICurrentFtypeEntry->out_func = (OutFuncPtr) out_func_ptr;

    kr_changeFtypeUnits( UICurrentFtypeEntry );
    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_setFirstFTypeSite

  PURPOSE  : Initializes the first site of the current FType.
  NOTES    :

  RETURNS  : Returns FALSE, if no sites are available in the current
             FType entry.
  UPDATE   :
******************************************************************************/
bool  krui_setFirstFTypeSite(void)
{

    if (UICurrentFtypeEntry == NULL)  {
        UICurrentFtypeSite = NULL;
        return( FALSE );
    }

    if (UICurrentFtypeEntry->sites == NULL)  {
        UICurrentFtypeSite = NULL;
        return( FALSE );
    } else {
        UICurrentFtypeSite = UICurrentFtypeEntry->sites;
        return( TRUE );
    }
}


/*****************************************************************************
  FUNCTION : krui_setNextFTypeSite

  PURPOSE  : Initializes the next FType site.
  NOTES    :

  RETURNS  : Returns FALSE, if no more sites are available in the current
             FType entry.
  UPDATE   :
******************************************************************************/
bool  krui_setNextFTypeSite(void)
{

    if (UICurrentFtypeSite == NULL)
        return( FALSE );

    if (UICurrentFtypeSite->next == NULL)
        return( FALSE );

    UICurrentFtypeSite = UICurrentFtypeSite->next;
    return( TRUE );
}


/*****************************************************************************
  FUNCTION : krui_getFTypeSiteName

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the name of the current FType site (in the current
             Ftype entry).
  UPDATE   :
******************************************************************************/
char  *krui_getFTypeSiteName(void)
{

    if (UICurrentFtypeSite == NULL)
        return( NULL );

    return( ((UICurrentFtypeSite->site_table)->Entry.site_name)->Entry.symbol );
}


/*****************************************************************************
  FUNCTION : krui_setFTypeSiteName

  PURPOSE  : Sets the name and function of the current FType site (in the
             current FType entry).
             All sites (in the existing network) of the current Ftype and the
             same (old) name changes their names and site functions.

  NOTES    :

  RETURNS  : Returns an error code if
             - current Ftype site isn't defined
             - site name does not exist in the site name table
             0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_setFTypeSiteName(char *FType_site_name)
{
    struct  SiteTable     *stbl_ptr;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    KernelErrorCode = KRERR_NO_ERROR;
    if (UICurrentFtypeEntry == NULL)
        {                       /*  Current Ftype entry isn't defined   */
            KernelErrorCode = KRERR_FTYPE_ENTRY;
            return( KernelErrorCode );
        }

    if (!kr_symbolCheck( FType_site_name ))
        return( KernelErrorCode );

    if ( (stbl_ptr = krm_STableSymbolSearch( FType_site_name )) == NULL)
        {                       /*   site name isn't defined */
            KernelErrorCode = KRERR_UNDEF_SITE_NAME;
            return( KernelErrorCode );
        }

    kr_changeFtypeSites( UICurrentFtypeEntry, UICurrentFtypeSite->site_table, 
			 stbl_ptr);
    UICurrentFtypeSite->site_table = stbl_ptr;

    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_createFTypeEntry

  PURPOSE  : Create a new functionality type, needs a definite FType symbol,
             the unit output and activation function
             and the number of sites provided for this unit FType.
             An additional array with N elements of pointers to site names
             is required for the definition of the sites.
  NOTES    : The number of Ftype entries and the number of sites per Ftype is
             only limited by the size of system memory.

  RETURNS  : Returns error code if:
             - memory allocation fails
             - FType name isn't definite (symbol is used for another FType
               or symbol is a NULL pointer)
             - one or more site names are undefined

             0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_createFTypeEntry(char *Ftype_symbol, char *act_func_name, 
				char *out_func_name, int no_of_sites, 
				char **array_of_site_names)
{
    FunctionPtr act_func, out_func, act_deriv_func, act_2_deriv_func;
    struct  FtypeUnitStruct   *Ftype_entry;
    struct  SiteTable         *STable_entry;
    int       i;
    char      *Ftype_site_name;


    KernelErrorCode = KRERR_NO_ERROR;

    if (Ftype_symbol == NULL)
    {   /*  Ftype name isn't defined   */
        KernelErrorCode = KRERR_FTYPE_NAME;
        return( KernelErrorCode );
    }

    if (!kr_symbolCheck( Ftype_symbol ))
        return( KernelErrorCode );
    if (krm_NTableSymbolSearch( Ftype_symbol, FTYPE_UNIT_SYM ) != NULL)
        return( KernelErrorCode );
    if ( !krf_funcSearch( out_func_name, OUT_FUNC, &out_func ) )
        return( KernelErrorCode );
    if ( !krf_funcSearch( act_func_name, ACT_FUNC, &act_func ) )
        return( KernelErrorCode );
    /*  set the derivation function of the activation function  */
    if ( !krf_funcSearch( act_func_name, ACT_DERIV_FUNC, &act_deriv_func ))
        return( KernelErrorCode );
    /*  set the second derivation function of the activation function  */
    if ( !krf_funcSearch( act_func_name, ACT_2_DERIV_FUNC, &act_2_deriv_func ))
	return( KernelErrorCode );

    if ((Ftype_entry = krm_FtypeCreateEntry( Ftype_symbol
                                            ,(OutFuncPtr) out_func
                                            ,(ActFuncPtr) act_func
                                            ,(ActDerivFuncPtr) act_deriv_func
                                            ,(ActDerivFuncPtr) act_2_deriv_func
					    ,kr_findPythonFunction(out_func_name,OUT_FUNC)
					    ,kr_findPythonFunction(act_func_name, ACT_FUNC)
					    ,kr_findPythonFunction(act_func_name,ACT_DERIV_FUNC)
					    ,kr_findPythonFunction(act_func_name,ACT_2_DERIV_FUNC)
                                            )) == NULL)
        return( KernelErrorCode );

    /*  create sites at the Ftype  */
    for (i = 0; i < no_of_sites; i++)  {
        if ( (Ftype_site_name = array_of_site_names[ i ]) == NULL){
	    /*   site name isn't defined because it is a NULL pointer  */
	    KernelErrorCode = KRERR_UNDEF_SITE_NAME;
	    return( KernelErrorCode );
	}

        if ((STable_entry = krm_STableSymbolSearch( Ftype_site_name )) == NULL){
	    /*  site isn't defined  */
	    krm_releaseFtypeEntry( Ftype_entry );
	    KernelErrorCode = KRERR_UNDEF_SITE_NAME;
	    return( KernelErrorCode );
	}

        if (krm_FtypeAddSite( Ftype_entry , STable_entry ) == NULL){
	    /*  memory alloc failed */
	    krm_releaseFtypeEntry( Ftype_entry );
	    return( KernelErrorCode );
	}
    }

    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_deleteFTypeEntry

  PURPOSE  : Deletes the specified FType entry. If there exists units in the
             network with this FType, all these units will lost their FType
             but the functionality of the units will not be changed.
  NOTES    :

  RETURNS  : Returns error code if FType symbol dosn't exist, 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err    krui_deleteFTypeEntry(char *Ftype_symbol)
{
    struct  FtypeUnitStruct   *ftype_ptr;

    if ( (ftype_ptr = krm_FtypeSymbolSearch( Ftype_symbol ) ) == NULL)
        return( KRERR_FTYPE_SYMBOL ); /*  FType symbol isn't defined  */

    kr_deleteUnitsFtype( ftype_ptr );
    krm_releaseFtypeEntry( ftype_ptr );
    return( KRERR_NO_ERROR );
}



/*############################################################################

GROUP: Functions for reading of the function table

############################################################################*/


/*****************************************************************************
  FUNCTION : krui_getNoOfFunctions

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the number of available functions
  UPDATE   :
******************************************************************************/
int  krui_getNoOfFunctions(void)
{
    return( krf_getNoOfFuncs() );
}


/*****************************************************************************
  FUNCTION : krui_getFuncInfo

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the name of the function and the function type (Output,
             Activation, Site).
  UPDATE   :
******************************************************************************/
void  krui_getFuncInfo(int func_no, char **func_name, int *func_type)
{
    static struct FuncInfoDescriptor  functionDescr;
    functionDescr.number = func_no - 1;
    KernelErrorCode = krf_getFuncInfo( GET_FUNC_INFO, &functionDescr );

    *func_type = functionDescr.func_type;
    *func_name = functionDescr.func_name;
}


/*****************************************************************************
  FUNCTION : krui_isFunction

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns true if the given function name and type exists.
  UPDATE   :
******************************************************************************/
bool  krui_isFunction(char *func_name, int func_type)
{
    FunctionPtr  dummy_func_ptr;
    bool        is_func;

    is_func = krf_funcSearch( func_name, func_type, &dummy_func_ptr );
    KernelErrorCode = KRERR_NO_ERROR;
    return( is_func );
}


/*****************************************************************************
  FUNCTION : krui_getFuncParamInfo

  PURPOSE  : Returns the no. of input and output parameters of the given
             function (only relevant for learning, update and initialisation
             functions).
  NOTES    :

  RETURNS  : Returns TRUE if the given function exists, FALSE otherwise.
  UPDATE   :
******************************************************************************/
bool  krui_getFuncParamInfo(char *func_name, int func_type, 
			    int *no_of_input_params, int *no_of_output_params)
{
    static struct FuncInfoDescriptor  functionDescr;

    functionDescr.func_type = func_type;
    strcpy( functionDescr.func_name, func_name );

    KernelErrorCode = krf_getFuncInfo( SEARCH_FUNC, &functionDescr );

    if (KernelErrorCode != KRERR_NO_ERROR)  return( FALSE );

    *no_of_input_params = functionDescr.no_of_input_parameters;
    *no_of_output_params = functionDescr.no_of_output_parameters;
    return( TRUE );
}



/*############################################################################

GROUP: Site Table Functions

#############################################################################*/


/*****************************************************************************
  FUNCTION : krui_getFirstSiteTableEntry

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the first site name/function pair in the site table.
             Returns FALSE and NULL, if not available.
  UPDATE   :
******************************************************************************/
bool   krui_getFirstSiteTableEntry(char **site_name, char **site_func)
{
    struct  SiteTable   *s_ptr;

    if ( (s_ptr = krm_getSTableFirstEntry() ) == NULL) {
	*site_name = NULL;
	*site_func = NULL;
	return( FALSE );
    } else {
	*site_name = (s_ptr->Entry.site_name)->Entry.symbol;
	*site_func = krf_getFuncName( (FunctionPtr) s_ptr->site_func );
	return( TRUE );
    }
}


/*****************************************************************************
  FUNCTION : krui_getNextSiteTableEntry

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the next site name/function pair in the site table.
             Returns FALSE and NULL, if not available.
  UPDATE   :
******************************************************************************/
bool  krui_getNextSiteTableEntry(char **site_name, char **site_func)
{
    struct  SiteTable   *s_ptr;

    if ( (s_ptr = krm_getSTableNextEntry() ) == NULL) {
	*site_name = NULL;
	*site_func = NULL;
	return( FALSE );
    } else {
	*site_name = (s_ptr->Entry.site_name)->Entry.symbol;
	*site_func = krf_getFuncName( (FunctionPtr) s_ptr->site_func );
	return( TRUE );
    }
}


/*****************************************************************************
  FUNCTION : krui_getSiteTableFuncName

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the name of the site function that is associated with
             the site name.
             If the site name do not exist, function returns NULL.
  UPDATE   :
******************************************************************************/
char  *krui_getSiteTableFuncName(char *site_name)
{
    struct SiteTable  *s_ptr;

    if ( (s_ptr = krm_STableSymbolSearch( site_name )) == NULL)
        return( NULL );

    return( krf_getFuncName( (FunctionPtr) s_ptr->site_func ) );
}


/*****************************************************************************
  FUNCTION : krui_createSiteTableEntry

  PURPOSE  : Creates a new site name and associate this name with a site
             function.
  NOTES    :

  RETURNS  : Returns error code if:
             - site name already exists or
             - site function is invalid or
             - memory allocation has failed
            0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_createSiteTableEntry(char *site_name, char *site_func)
{
    FunctionPtr   func_ptr;

    KernelErrorCode = KRERR_NO_ERROR;
    if ( !krf_funcSearch( site_func, SITE_FUNC, &func_ptr ) )
        return( KernelErrorCode );
    if (!kr_symbolCheck( site_name ))
        return( KernelErrorCode );
    if (krm_STableSymbolSearch( site_name ) != NULL) {
	/*  symbol is already in the site table */
	KernelErrorCode = KRERR_REDEF_SITE_NAME;
	return( KernelErrorCode );
    }

    (void) krm_STableCreateEntry( site_name, (SiteFuncPtr) func_ptr );

    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_changeSiteTableEntry

  PURPOSE  : Changes the site function of a previously defined site name.
  NOTES    : All sites in the network with the name <old_site_name>
             changes their names and functions.

  RETURNS  : Returns error code if <old_site_name> or <new_site_func>
             isn't defined, 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_changeSiteTableEntry(char *old_site_name, char *new_site_name, 
				    char *new_site_func)
{
    FunctionPtr   func_ptr;
    SiteFuncPtr     site_func_ptr;
    struct SiteTable  *stbl_ptr1,
    *stbl_ptr2;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    KernelErrorCode = KRERR_NO_ERROR;
    if (!kr_symbolCheck( new_site_name ))
        return( KernelErrorCode );
    if ( !krf_funcSearch( new_site_func, SITE_FUNC, &func_ptr ) )
        return( KernelErrorCode );

    site_func_ptr = (SiteFuncPtr) func_ptr;

    if ( (stbl_ptr1 = krm_STableSymbolSearch( old_site_name )) == NULL) {
        /*  old site name isn't defined */
        KernelErrorCode = KRERR_UNDEF_SITE_NAME;
        return( KernelErrorCode );
    }

    stbl_ptr2 = krm_STableSymbolSearch( new_site_name );
    if ( (stbl_ptr2 != NULL) && (stbl_ptr2 != stbl_ptr1) ){
        /*  new symbol is already in the site table (and new_site_name and
            old_site_name are not identical) */
        KernelErrorCode = KRERR_REDEF_SITE_NAME;
        return( KernelErrorCode );
    }

    (void) krm_STableChangeEntry( stbl_ptr1 , new_site_name , site_func_ptr );
    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_deleteSiteTableEntry

  PURPOSE  : Removes the current site name entry from the site table.
  NOTES    :

  RETURNS  : Returns an error code if
             - there exists sites with the given name in the network or
             - <site_name> isn't defined
             0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_deleteSiteTableEntry(char *site_name)
{
    struct SiteTable    *st_ptr;

    if ((st_ptr = krm_STableSymbolSearch( site_name )) == NULL)
        return( KRERR_UNDEF_SITE_NAME ); /*  Site name isn't defined */

    if (kr_searchNetSite( st_ptr ) != 0)
        return( KRERR_INUSE_SITE ); /* Site is in use and must not be deleted */

    krm_STableRemoveEntry( st_ptr );
    return( KRERR_NO_ERROR );
}


/*############################################################################

GROUP: Site Functions

############################################################################*/


/*****************************************************************************
  FUNCTION : krui_setFirstSite

  PURPOSE  : Initializes the first site at the current unit.
  NOTES    :

  RETURNS  : Returns false if no site available or if no sites permitted at 
             this unit.
  UPDATE   :
******************************************************************************/
bool  krui_setFirstSite(void)
{
    if KERNEL_STANDARD
        return( kr_setSite( FIRST, NULL ) );

    KernelErrorCode = KRERR_SITES_NO_SUPPORT;
    return( FALSE );
}


/*****************************************************************************
  FUNCTION : krui_setNextSite

  PURPOSE  : Initializes the next site at the current unit.
  NOTES    :

  RETURNS  : Returns false if no more sites available.
  UPDATE   :
******************************************************************************/
bool  krui_setNextSite(void)
{
    if KERNEL_STANDARD
        return( kr_setSite( NEXT, NULL ) );

    KernelErrorCode = KRERR_SITES_NO_SUPPORT;
    return( FALSE );
}


/*****************************************************************************
  FUNCTION : krui_setSite

  PURPOSE  : Initializes the given site at the current unit.
  NOTES    :

  RETURNS  : Returns error code if
             - unit dosn't exist
             - site name doesn't exist
             - unit don't has sites
             - unit don't has a site with this name
             0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_setSite(char *site_name)
{

    if KERNEL_STANDARD
        return( kr_setSite( NAME, site_name ) );

    KernelErrorCode = KRERR_SITES_NO_SUPPORT;
    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_getSiteValue

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the actual value of the current site
  UPDATE   :
******************************************************************************/
FlintType   krui_getSiteValue(void)
{

    if KERNEL_STANDARD  {
        if (sitePtr == NULL)
            KernelErrorCode = KRERR_NO_SITES;
        else
            return( (*sitePtr->site_table->site_func) (sitePtr) );
    }else
        KernelErrorCode = KRERR_SITES_NO_SUPPORT;

    return( (FlintType) 0 );
}


/*****************************************************************************
  FUNCTION : krui_getSiteName

  PURPOSE  :
  NOTES    :

  RETURNS  : returns the name of the current unit/site,
             NULL if not available.
  UPDATE   :
******************************************************************************/
char  *krui_getSiteName(void)
{

    if KERNEL_STANDARD    {
        if (sitePtr == NULL)
            KernelErrorCode = KRERR_NO_SITES;
        else
            return( ((sitePtr->site_table)->Entry.site_name)->Entry.symbol );
    }else
        KernelErrorCode = KRERR_SITES_NO_SUPPORT;

    return( NULL );
}


/*****************************************************************************
  FUNCTION : krui_setSiteName

  PURPOSE  : Sets the name/function of the current unit/site.
             Current Unit will loose the functionality type.
  NOTES    :

  RETURNS  : Returns error code if site name isn't defined.
  UPDATE   :
******************************************************************************/
krui_err  krui_setSiteName(char *site_name)
{
    struct  SiteTable     *stbl_ptr;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    if (sitePtr == NULL) {
	KernelErrorCode = KRERR_FTYPE_SITE;
	return( KernelErrorCode );
    }

    if (!kr_symbolCheck( site_name ))
        return( KernelErrorCode );
    if ((stbl_ptr = krm_STableSymbolSearch( site_name )) == NULL){
	/*   site name isn't defined */
	KernelErrorCode = KRERR_UNDEF_SITE_NAME;
	return( KernelErrorCode );
    }

    KernelErrorCode = KRERR_NO_ERROR;
    unitPtr->Ftype_entry = NULL;
    sitePtr->site_table  = stbl_ptr;

    NetModified = TRUE;

    return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : krui_getSiteFuncName

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the name of the current unit/site function
  UPDATE   :
******************************************************************************/
char  *krui_getSiteFuncName(void)
{
    static char  site_func_name[FUNCTION_NAME_MAX_LEN];

    if (sitePtr == NULL){
        KernelErrorCode = KRERR_FTYPE_SITE;
        return( NULL );
    }

    strcpy( site_func_name, 
	    krf_getFuncName( (FunctionPtr) sitePtr->site_table->site_func ) );

    return( site_func_name );
}


/*****************************************************************************
  FUNCTION : krui_addSite

  PURPOSE  : Adds a site at the current unit. If the unit has already sites, 
             this new site will be inserted above all other sites, i.e. the 
	     new created site will be the first site at this unit.
             If the unit has direct input links, i.e the unit has input links
             but no sites, the creation of sites is not permitted 
	     (krui_addSite will return an error code).
             If there exists already a site with the given name, the creation 
	     of the new site is prohibited and krui_addSite returns an error 
	     code.
             krui_addSite has no effect on the current site. To change the 
	     current site to this new site, call krui_setFirstSite().
             The unit's FType will be deleted.
  NOTES    : The number of sites per unit is nearly unlimited (2^32).

  RETURNS  : Returns error code if
             - memory allocation fails or
             - unit has direct input links or
             - site name isn't defined or
             - site with the given name exists already at this unit
             0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_addSite(char *site_name)
{
    FlagWord    flags;
    struct Site       *site_ptr,
    *site_ptr1;
    struct SiteTable  *stbl_ptr;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    KernelErrorCode = KRERR_NO_ERROR;
    if ( (stbl_ptr = krm_STableSymbolSearch( site_name )) == NULL)
        {                       /*  site name isn't defined */
            KernelErrorCode = KRERR_UNDEF_SITE_NAME;
            return( KernelErrorCode );
        }

    flags = unitPtr->flags & UFLAG_INPUT_PAT;

    switch (flags){
    case UFLAG_NO_INP :
	/*  Unit has no inputs  */
	if ((site_ptr = kr_createDefaultSite() ) == NULL)
	    return( KernelErrorCode );

	unitPtr->sites = site_ptr; /*  Connect site    */
	unitPtr->flags |= UFLAG_SITES; /*  Set site flag   */
	unitPtr->Ftype_entry = NULL; /*  Delete Ftype    */

	break;

    case UFLAG_SITES :
	/*  Unit has already sites  */
	if (kr_searchUnitSite( unitPtr, stbl_ptr ) != NULL){
	    /* there exists already a site with this name at this unit */
	    KernelErrorCode = KRERR_DUPLICATED_SITE;
	    return( KernelErrorCode );
	}

	if ( (site_ptr = kr_createDefaultSite() ) == NULL)
	    return( KernelErrorCode );

	site_ptr1 = unitPtr->sites;
	unitPtr->sites = site_ptr; /*  Connect site    */
	site_ptr->next = site_ptr1;
	unitPtr->Ftype_entry = NULL; /*  Delete Ftype    */

	break;

    case UFLAG_DLINKS :
	/*  Unit has direct input links and can't have sites */
	KernelErrorCode = KRERR_CREATE_SITE;
	return( KernelErrorCode );

    default :
	KernelErrorCode = KRERR_CREATE_SITE;
	return( KernelErrorCode );
    }

    site_ptr->site_table = stbl_ptr;

    NetModified = TRUE;
    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_deleteSite

  PURPOSE  : Removes the current site at the current unit and removes all
             links from predecessor units to this site.
             krui_setFirstSite (krui_setNextSite) must be called at least once
             before using this function.
             The current site will be set to the next available site, if no more
             sites available, krui_deleteSite returns 0 otherwise 1.
             Returns an error code if ther was a problem.
             The unit's FType will be set to 0, i.e. the unit's functionality
             type will be deleted.

  NOTES    : To delete all sites at a unit:
               if ( krui_setFirstSite() )
                   while ( krui_deleteSite() > 0) { }

  RETURNS  :
  UPDATE   :
******************************************************************************/
bool  krui_deleteSite(void)
{
    struct Site   *next_site_ptr;
    struct Unit   *unit_ptr;

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    unit_ptr = unitPtr;

    if ( UNIT_HAS_SITES( unit_ptr ) && (unit_ptr->sites != NULL) &&
	 (sitePtr != NULL) ){
	/*  Unit has sites  */
	NetModified = TRUE;

	next_site_ptr = sitePtr->next;

	krm_releaseAllLinks( sitePtr->links ); /*   Remove links    */
	krm_releaseSite( sitePtr ); /*      Remove site     */

	if (prevSitePtr == NULL){
	    /*  This site is the first site at the current unit */

	    if (next_site_ptr == NULL){
		/*  Unit has only this site */
		unit_ptr->sites = NULL; /*  Clear site pointer  */
		unit_ptr->flags &= (~UFLAG_INPUT_PAT); /* Clear input flags */
		sitePtr     = NULL; /*  No more sites available  */
		prevSitePtr = NULL;
	    } else {
		/*  It is the first site at the unit but not the only one   */
		unit_ptr->sites  = next_site_ptr; /*  Connect the other sites */
		sitePtr = next_site_ptr; 
	    }
	} else {
	    /*  This is not the first site at the unit  */
	    prevSitePtr->next = next_site_ptr; /*  Connect the previous site
						   with the next site  */
	    sitePtr = next_site_ptr; 
	}

	unit_ptr->Ftype_entry = NULL; /*  Delete unit's Ftype */
    }

    if (sitePtr != NULL)
        return( TRUE );         /*  Returns TRUE if more sites available  */
    else
        return( FALSE );
}


/*############################################################################

GROUP: Link Functions

############################################################################*/


/*****************************************************************************
  FUNCTION : krui_getFirstPredUnit

  PURPOSE  :
  NOTES    : If a predecessor unit exists, the current link is set to the link
             between the two units.

  RETURNS  : Returns the no. of first predecessor unit of the current unit/site
             and the connection strenght.
             Returns 0 if no predecessor unit available, i.e. if the current 
	     unit and/or site has no inputs.
  UPDATE   :
******************************************************************************/
int  krui_getFirstPredUnit(FlintType *strength)
{
  float dummy1,dummy2,dummy3;


    if KERNEL_STANDARD  {
        return(kr_getPredecessorUnit(FIRST, strength ,&dummy1,&dummy2,&dummy3));
    } else {

#ifdef MASPAR_KERNEL
        return( krff_getPredecessorUnit( FIRST, strength ) );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( KernelErrorCode );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_getFirstPredUnitAndData

  PURPOSE  :
  NOTES    : If a predecessor unit exists, the current link is set to the link
             between the two units.

  RETURNS  : Returns the no. of first predecessor unit of the current unit/site
             and the connection strenght.
             Returns 0 if no predecessor unit available, i.e. if the current 
	     unit and/or site has no inputs.
  UPDATE   :
******************************************************************************/
int  krui_getFirstPredUnitAndData(FlintType *strength,float *val_a,
				  float *val_b, float *val_c)
{
    if KERNEL_STANDARD  {
        return( kr_getPredecessorUnit( FIRST, strength ,val_a ,val_b ,val_c ) );
    } else {

#ifdef MASPAR_KERNEL
        return( krff_getPredecessorUnit( FIRST, strength ) );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( KernelErrorCode );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_getNextPredUnit

  PURPOSE  : Returns the number of the next logical predecessor unit in a TDNN
  NOTES    : If another predecessor unit exists, the current link is set to
             the link between the two units.


  RETURNS  : Returns the no. of the next predecessor unit of the current
             unit/site and the connection strenght.
             Returns 0 if no more predecessor units exists.
  UPDATE   :
******************************************************************************/
int  krui_getNextPredUnit(FlintType *strength)
{
    float dummy1,dummy2,dummy3;

    if KERNEL_STANDARD  {
        return(kr_getPredecessorUnit(NEXT, strength ,&dummy1,&dummy2,&dummy3));
    } else {

#ifdef MASPAR_KERNEL
        return( krff_getPredecessorUnit( NEXT, strength ) );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( KernelErrorCode );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_getNextPredUnitAndData

  PURPOSE  : Returns the number of the next logical predecessor unit in a TDNN
  NOTES    : If another predecessor unit exists, the current link is set to
             the link between the two units.


  RETURNS  : Returns the no. of the next predecessor unit of the current
             unit/site and the connection strenght.
             Returns 0 if no more predecessor units exists.
  UPDATE   :
******************************************************************************/
int  krui_getNextPredUnitAndData(FlintType *strength,float *val_a,
				 float *val_b, float *val_c)
{

    if KERNEL_STANDARD  {
        return(kr_getPredecessorUnit( NEXT, strength, val_a, val_b, val_c ) );
    } else {

#ifdef MASPAR_KERNEL
        return( krff_getPredecessorUnit( NEXT, strength ) );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( KernelErrorCode );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_getCurrentPredUnit

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the no. of the current predecessor unit (of the current
             unit/site) and the connection strenght.
             Returns 0 if no predecessor unit available, i.e. if the current 
	     unit and/or site has no inputs
  UPDATE   :
******************************************************************************/
int  krui_getCurrentPredUnit(FlintType *strength)
{
    float dummy1,dummy2,dummy3;

    if KERNEL_STANDARD  {
        return(kr_getPredecessorUnit(CURRENT, strength, &dummy1, &dummy2, 
				     &dummy3 ) );
    } else {

#ifdef MASPAR_KERNEL
        return( krff_getPredecessorUnit( CURRENT, strength ) );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( KernelErrorCode );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_getFirstSuccUnit

  PURPOSE  :
  NOTES    : This function is slow (Units are backward chained only).
  IMPORTANT: If a successor unit exists, the current unit and site will be
             set to this successor unit and the attached site.

  RETURNS  : Returns the no. of the first successor unit of the unit 
             <source_unit_no> and the connection strenght.
             Returns (negative) error code if unit doesn't exist.
             Returns 0 if no successor unit available, i.e. if the given unit
             has no output connection.
  UPDATE   :
******************************************************************************/
int  krui_getFirstSuccUnit(int source_unit_no, FlintType *weight)
{

    if KERNEL_STANDARD  {
        return( kr_getSuccessorUnit( FIRST, source_unit_no, weight ) );
    } else {

#ifdef MASPAR_KERNEL
        return( krff_getSuccessorUnit( FIRST, source_unit_no, weight ) );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( KernelErrorCode );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_getNextSuccUnit

  PURPOSE  :
  NOTES    : This function is slow  (Units are backward chained only)
  IMPORTANT: If a successor unit exists, the current unit and site will be
             set to this successor unit and the attached site.

  RETURNS  : Returns the no. of the next successor unit and the connection 
             strenght.
  UPDATE   :
******************************************************************************/
int  krui_getNextSuccUnit(FlintType *weight)
{

    if KERNEL_STANDARD  {
        return( kr_getSuccessorUnit( NEXT, 0, weight ) );
    } else {

#ifdef MASPAR_KERNEL
        return( krff_getSuccessorUnit( NEXT, 0, weight ) );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( KernelErrorCode );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_areConnected

  PURPOSE  : True if there exists a connection between source unit 
             <source_unit_no> and target unit <target_unit_no>, otherwise false.
  NOTES    : This function is slow (Units are backward chained only).
  RETURNS  :
  UPDATE   :
******************************************************************************/
bool  krui_areConnected(int sourceNo, int targetNo)
{
    register struct Link *link_ptr ;
    register struct Unit *s_unit_ptr, *t_unit_ptr ;
    register struct Site *site_ptr ;

    s_unit_ptr = kr_getUnitPtr (sourceNo) ;
    t_unit_ptr = kr_getUnitPtr (targetNo) ;

    if( UNIT_HAS_DIRECT_INPUTS (t_unit_ptr)){
        for (link_ptr = (struct Link *) t_unit_ptr->sites;
             link_ptr != NULL; link_ptr = link_ptr->next)
            if (link_ptr->to == s_unit_ptr) 
		return (TRUE) ;
    }else if( UNIT_HAS_SITES (t_unit_ptr)){
	for (site_ptr = t_unit_ptr->sites; site_ptr != NULL ;
	     site_ptr = site_ptr->next)
	    for (link_ptr = site_ptr->links; link_ptr != NULL ;
		 link_ptr = link_ptr->next)
		if (link_ptr->to == s_unit_ptr) 
		    return (TRUE) ;
    }

    return (FALSE) ;
}


/*****************************************************************************
  FUNCTION : krui_areConnectedWeight

  PURPOSE  : True if there exists a connection between source unit 
             <source_unit_no> and target unit <target_unit_no>, otherwise 
	     false. If there exist a connection between these units, 
	     krui_areConnectedWeight returns the connection strength also.
             Returns FALSE if unit doesn't exist.
  NOTES    : This function is slow (Units are backward chained only).
  IMPORTANT: If there exist a connection, the current unit and site will be
             set to the target unit/site.
  RETURNS  :
  UPDATE   :
******************************************************************************/
bool  krui_areConnectedWeight(int source_unit_no, int target_unit_no, 
			      FlintType *weight)
{
    if KERNEL_STANDARD  {
        return( kr_areConnected( source_unit_no, target_unit_no, weight ) );
    } else {

#ifdef MASPAR_KERNEL
        return( krff_areConnected( source_unit_no, target_unit_no, weight ) );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( FALSE );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_isConnected

  PURPOSE  : True if there exists a connection between source unit 
             <source_unit_no> and the current unit/site, otherwise false.
  NOTES    : If there exists a connection between the two units, the current 
             link is set to the link between the two units. (alter the link 
	     weight with krui_setLinkWeight)

  RETURNS  :
  UPDATE   :
******************************************************************************/
bool  krui_isConnected(int source_unit_no)
{
    FlintType  weight;

    if KERNEL_STANDARD  {
        return( kr_isConnected( source_unit_no, &weight ) );
    } else {

#ifdef MASPAR_KERNEL
        return( krff_isConnected( source_unit_no, &weight ) );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( FALSE );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_getLinkWeight

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the link weight of the current link.
  UPDATE   :
******************************************************************************/
FlintType  krui_getLinkWeight(void)
{

    if KERNEL_STANDARD  {
        return( kr_getLinkWeight() );
    } else {

#ifdef MASPAR_KERNEL
        return( krff_getLinkWeight() );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( 0.0 );
#endif

    }
}

/*****************************************************************************
  FUNCTION : krui_setLinkWeight

  PURPOSE  : Sets the link weight of the current link
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_setLinkWeight(FlintTypeParam weight)
{

    if KERNEL_STANDARD  {
        kr_setLinkWeight( weight );
    } else {

#ifdef MASPAR_KERNEL
        krff_setLinkWeight( weight );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_createLink

  PURPOSE  : Creates a link between source unit and the current unit/site.
  NOTES    : krui_createLink DO NOT set the current link.
             If you want to create a link and its unknown if there exists 
	     already a connection between the two units, use krui_createLink 
	     and test the return code, instead of the sequence 
	     krui_isConnected and krui_createLink.
  RETURNS  : Returns an error code:
             - if memory allocation fails
             - if source unit doesn't exist or
             - if there exists already a connection between current unit/site 
	       and the source unit
             0 otherwise.

  UPDATE   :
******************************************************************************/
krui_err  krui_createLink(int source_unit_no, FlintTypeParam weight)
{

    if KERNEL_STANDARD  {
        return( kr_createLink( source_unit_no, weight ) );
    } else {

#ifdef MASPAR_KERNEL
        return( krff_createLink( source_unit_no, weight ) );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( KernelErrorCode );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_createLinkWithAdditionalParameters

  PURPOSE  : Creates a link between source unit and the current unit/site.
  NOTES    : krui_createLink DO NOT set the current link.
             If you want to create a link and its unknown if there exists 
	     already a connection between the two units, use krui_createLink 
	     and test the return code, instead of the sequence 
	     krui_isConnected and krui_createLink.
  RETURNS  : Returns pointer to new unit.

  UPDATE   : 13.05.96 <Juergen Gatter>
******************************************************************************/
struct Link*  krui_createLinkWithAdditionalParameters(int source_unit_no, 
						      FlintTypeParam weight,
						      float val_a, float val_b,
						      float val_c)
{

    if KERNEL_STANDARD  {
        return( kr_createLinkWithAdditionalParameters(source_unit_no, weight, 
						      val_a, val_b, val_c ) );
    } else {

#ifdef MASPAR_KERNEL
        KernelErrorCode=krff_createLink( source_unit_no, weight ) 
        return( NULL );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
        return( NULL );
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_deleteLink

  PURPOSE  : Deletes the current link.
  NOTES    : To delete a link between the current unit/site and the source unit
             <source_unit_no>, call krui_isConnected( source_unit_no ) and
             krui_deleteLink().

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_deleteLink(void)
{

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    return( kr_deleteLink() );
}


/*****************************************************************************
  FUNCTION : krui_deleteAllInputLinks

  PURPOSE  : Deletes all input links at current unit/site.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_deleteAllInputLinks(void)
{

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    return( kr_deleteAllLinks( INPUTS ) );
}


/*****************************************************************************
  FUNCTION : krui_deleteAllOutputLinks

  PURPOSE  : Deletes all output links at current unit.
  NOTES    : This function is slow.

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_deleteAllOutputLinks(void)
{

#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    return( kr_deleteAllLinks( OUTPUTS ) );
}


/*****************************************************************************
  FUNCTION : krui_jogWeights

  PURPOSE  : Add uniform distributed random values to connection weights.
             <minus> must be less then <plus>.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_jogWeights(FlintTypeParam minus, FlintTypeParam plus)
{

    if (minus >= plus)  {
        KernelErrorCode = KRERR_PARAMETERS;
        return; 
    }

    if KERNEL_STANDARD  {
        kr_jogWeights( minus, plus );
    } else {

#ifdef MASPAR_KERNEL
        krff_jogWeights( minus, plus );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
#endif

    }
}


/*****************************************************************************
  FUNCTION : krui_jogCorrWeights

  PURPOSE  : Add uniform distributed random values to connection weights of
             highly correlated, non-special hidden units.
             <minus> must be less then <plus>.
	     The two hidden units with maximum positive or negative correlation 
	     with an absolute value higher then mincorr are searched. The 
	     incoming weights of one of these units are jogged.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_jogCorrWeights(FlintTypeParam minus, FlintTypeParam plus,
			      FlintTypeParam mincorr)
{
    krui_err res = KRERR_NO_ERROR;

    if (minus >= plus)  {
        KernelErrorCode = KRERR_PARAMETERS;
        return KernelErrorCode;  
    }

    if KERNEL_STANDARD  {
        res = kr_jogCorrWeights( minus, plus, mincorr );
    } else {

#ifdef MASPAR_KERNEL
        krff_jogCorrWeights( minus, plus, mincorr );
#else
        KernelErrorCode = KRERR_NO_MASPAR_KERNEL;
#endif

    }
    return res;
}


/*###########################################################################

GROUP: Functions for network propagation

############################################################################*/


/*****************************************************************************
  FUNCTION : krui_getVariance

  PURPOSE  :
  NOTES    :

  RETURNS  : Variance

******************************************************************************/
float krui_getVariance (void)
{
    register struct Unit   *unit_ptr;
    int   pattern_no=0, o, noOfOutputUnits, size, noOfPatternPairs, sub_pat_no;
    Patterns  out_pat;
    register float *OutputUnitSumVariance, *OutputUnitVariance,Variance=0;

    noOfOutputUnits=krui_getNoOfOutputUnits();
    noOfPatternPairs=kr_np_pattern( PATTERN_GET_NUMBER,0, 0 );
    OutputUnitVariance = (float *)calloc(noOfOutputUnits,sizeof (float));
    OutputUnitSumVariance = (float *)calloc(noOfOutputUnits,sizeof (float));

    KernelErrorCode = KRERR_NO_ERROR;

    KernelErrorCode = kr_initSubPatternOrder(0,kr_np_pattern(PATTERN_GET_NUMBER,
							     0, 0) - 1);
    if(KernelErrorCode != KRERR_NO_ERROR) {
	free (OutputUnitSumVariance);
	free (OutputUnitVariance);
	return (KernelErrorCode);
    }
    while(kr_getSubPatternByOrder(&pattern_no,&sub_pat_no)){
	out_pat = kr_getSubPatData(pattern_no,sub_pat_no,OUTPUT,&size);
	if(out_pat == NULL){
	    KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	    free (OutputUnitSumVariance);
	    free (OutputUnitVariance);
	    return(-1);
	}
	out_pat += size;
	o=0;
	FOR_ALL_UNITS( unit_ptr )
	    if (IS_OUTPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))  {
		--out_pat;
		OutputUnitVariance[o] += (*out_pat) * (*out_pat);
		OutputUnitSumVariance [o] += *out_pat;
		o++;
	    }
    }
    o=0;
    FOR_ALL_UNITS( unit_ptr )
	if (IS_OUTPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))  {
	    Variance += (OutputUnitVariance[o]/noOfPatternPairs)-
		pow(OutputUnitSumVariance[o]/noOfPatternPairs,2) ;
	    o++;
	}
    free (OutputUnitSumVariance);
    free (OutputUnitVariance);
    return(Variance);
}


/*****************************************************************************
  FUNCTION : krui_countLinks

  PURPOSE  :
  NOTES    :

  RETURNS  : Number of Parameters
  UPDATE   :
******************************************************************************/
int krui_countLinks(void)
{
  register struct Unit   *unit_ptr;
  register struct Link   *link_ptr;
  register int i=0;

  FOR_ALL_UNITS( unit_ptr )
    if ((IS_OUTPUT_UNIT(unit_ptr) || IS_HIDDEN_UNIT(unit_ptr))) {
      i++;
      FOR_ALL_LINKS (unit_ptr,link_ptr)
        i ++;
    }
  return i;
}


/*****************************************************************************
  FUNCTION : krui_updateSingleUnit

  PURPOSE  : Updates a single unit.
  NOTES    : Updates also frozen Units.

  RETURNS  : Returns error code if unit doesn't exist, 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err   krui_updateSingleUnit(int unit_no)
{
    register struct Unit   *unit_ptr;


#ifdef MASPAR_KERNEL
    MASPAR_FF1_VALIDATE_OP;
#endif

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    if (unit_ptr->out_func == NULL)
        /*  Identity Function   */
        unit_ptr->Out.output = unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);
    else{
	unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);
	unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
    }

    return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : krui_getUpdateFunc

  PURPOSE  : Returns the current update function. The default update function is
             SerialOrder() (see also kr_def.h).
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
char  *krui_getUpdateFunc(void)
{
    static char  updateFunc[FUNCTION_NAME_MAX_LEN];

    strcpy( updateFunc, krf_getCurrentNetworkFunc( UPDATE_FUNC ) );
    return( updateFunc );
}


/*****************************************************************************
  FUNCTION : krui_setRemapFunc

  PURPOSE  : Changes the current pattern remap function.
  NOTES    :

  RETURNS  : Returns error code if remap function is invalid.
  UPDATE   :
******************************************************************************/
krui_err krui_setRemapFunc(char *name, float *params)
{
    return( kr_npui_setRemapFunction(name, params) );
}


/*****************************************************************************
  FUNCTION : krui_setUpdateFunc

  PURPOSE  : Changes the current update function.
  NOTES    :

  RETURNS  : Returns error code if update function is invalid.
  UPDATE   :
******************************************************************************/
krui_err   krui_setUpdateFunc(char *update_func)
{

    return( krf_setCurrentNetworkFunc( update_func, UPDATE_FUNC ) );
}



/*****************************************************************************
  FUNCTION : krui_updateNet

  PURPOSE  : Updates the network according to update function:
  NOTES    : The network should be feedforward in topological mode,
             otherwise function will return a warning message.

             See also krui_setSeedNo for initializing the pseudo
             random generator.

  RETURNS  : Returns error code if an error occured, 0 othrwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_updateNet(float *parameterInArray, int NoOfInParams)
{

    return( kr_callNetworkFunction( UPDATE_FUNC, parameterInArray, NoOfInParams,
                                   NULL, NULL, 0, 0 ) );
}


/*############################################################################

GROUP: Initialisation Functions

############################################################################*/


/*****************************************************************************
  FUNCTION : krui_getInitialisationFunc

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the current initialisation function. The default 
             initialisation function is RandomizeWeights (see also kr_def.h).
  UPDATE   :
******************************************************************************/
char  *krui_getInitialisationFunc(void)
{

    return( krf_getCurrentNetworkFunc( INIT_FUNC ) );
}


/*****************************************************************************
  FUNCTION : krui_setInitialisationFunc

  PURPOSE  : Changes the current learning function.  Returns error code if learning
    function is invalid.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err   krui_setInitialisationFunc(char *initialisation_func)
{

    return( krf_setCurrentNetworkFunc( initialisation_func, INIT_FUNC ) );
}


/*****************************************************************************
  FUNCTION : krui_initializeNet

  PURPOSE  : Initializes the network
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_initializeNet(float *parameterInArray, int NoOfInParams)
{

    return( kr_callNetworkFunction( INIT_FUNC, parameterInArray, NoOfInParams,
                                   NULL, NULL, 0, 0 ) );
}


/*############################################################################

GROUP: Learning Functions

#############################################################################*/


/*****************************************************************************
  FUNCTION : krui_getLearnFunc

  PURPOSE  : Returns the current learning function. The default learning 
             function is Backpropagation (see also kr_def.h).
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
char  *krui_getLearnFunc(void)
{
    static char  learnFunc[FUNCTION_NAME_MAX_LEN];

    strcpy( learnFunc, krf_getCurrentNetworkFunc( LEARN_FUNC ) );

    return( learnFunc );
}


/*****************************************************************************
  FUNCTION : krui_setLearnFunc

  PURPOSE  : hanges the current learning function.
  NOTES    :

  RETURNS  : Returns error code if learning function is invalid
  UPDATE   :
******************************************************************************/
krui_err   krui_setLearnFunc(char *learning_func)
{

    return( krf_setCurrentNetworkFunc( learning_func, LEARN_FUNC ) );
}


/*****************************************************************************
  FUNCTION : krui_checkPruning

  PURPOSE  : checks if pruning is enabled
  NOTES    :

  RETURNS  : TRUE or FALSE
  UPDATE   :
******************************************************************************/
int krui_checkPruning ()
{

    return (!strcmp (krf_getCurrentNetworkFunc (LEARN_FUNC),
                     "PruningFeedForward"));

}


/*****************************************************************************
  FUNCTION : krui_trainNetwork

  PURPOSE  :  Learn all pattern pairs using current learning method.
              parameterInArray contains the learning parameter(s). 
	      NoOfInParams stores the number of learning parameters.
              parameterOutArray returns the results from the learning function.
              this array is a static array defined in the learning function.
              *NoOfOutParams points to a integer value that contains the number
              of output parameters from the current learning function.
  NOTES    :  Patterns must be loaded before calling this function.
  REMEMBER :  The backpropagation learning function takes the learning
              parameter from parameterInArray[ 0 ]. parameterOutArray[ 0 ]
              returns the current net error.


  RETURNS  : Returns an error code if memory allocation has failed or if
             the parameters are invalid.
  UPDATE   :
******************************************************************************/
krui_err krui_trainNetwork(NetLearnParameters *parameters)
{
    register int i;
    krui_err error;
    float parameterInArray[NO_OF_LEARN_PARAMS];
    float *parameterOutArray;


    parameters->noOfErrors = 0;
    /* This must be done for Type double => float */
    for (i = 0 ; i < parameters->noOfParameters ; i++)
        parameterInArray[i] = (float) parameters->parameter[i];


    noOfStoredErrors =0;
    for(i = 0 , dotraining = TRUE , error = KRERR_NO_ERROR;
        dotraining && i < parameters->noOfEpochs && error == KRERR_NO_ERROR;
        i++){

        error = kr_callNetworkFunction( LEARN_FUNC,
                            parameterInArray, parameters->noOfParameters,
                            &parameterOutArray, &parameters->noOfResults,
                            parameters->firstPattern,
                            parameters->lastPattern);
        if (( parameters->noOfEpochs < NO_OF_STORED_ERRORS) ||
            (((i+1)%((parameters->noOfEpochs / NO_OF_STORED_ERRORS)+1)) == 0)){
            storedLearnErrors[noOfStoredErrors] =
                parameters->learnErrors[parameters->noOfErrors] =
                    (double) parameterOutArray[0];
            storedAtEpoch[noOfStoredErrors] =
                parameters->atEpoch[parameters->noOfErrors] = i;
            noOfStoredErrors++;
            parameters->noOfErrors++;
        }

    }
    storedLearnErrors[noOfStoredErrors] =
        parameters->learnErrors[parameters->noOfErrors] =
            (double) parameterOutArray[0];
    storedAtEpoch[noOfStoredErrors++] =
        parameters->atEpoch[parameters->noOfErrors++] = i;
    parameters->netError = (double) parameterOutArray[0];
    if( dotraining ){
        parameters->lastEpoch = parameters->noOfEpochs;
        parameters->interrupted = FALSE;
    }else{
        parameters->lastEpoch = i;
        parameters->interrupted = TRUE;
    }

    /* This must be done for Type float => double */
    for (i = 0 ; i < parameters->noOfResults ; i++)
        parameters->result[i] = (double) parameterOutArray[i];

    return(error);
}


/*****************************************************************************
  FUNCTION : krui_getNetworkErrorArray

  PURPOSE  : Returns the current errorArray
  NOTES    : Returns pointer to static Arrays

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err krui_getNetworkErrorArray(double **learnErrors,int **atEpoch,
				   int *noOfErrors)
{
    *learnErrors = &storedLearnErrors[0];
    *atEpoch = &storedAtEpoch[0];
    *noOfErrors = noOfStoredErrors;
    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : krui_stopTraining

  PURPOSE  : Stop the current trainNetwork training
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err krui_stopTraining(void)
{
    dotraining = FALSE;
    return(KRERR_NO_ERROR);
}


/*****************************************************************************
  FUNCTION : krui_learnAllPatterns

  PURPOSE  :  Learn all pattern pairs using current learning method.
              parameterInArray contains the learning parameter(s). 
	      NoOfInParams stores the number of learning parameters.
              parameterOutArray returns the results from the learning function.
              this array is a static array defined in the learning function.
              *NoOfOutParams points to a integer value that contains the number
              of output parameters from the current learning function.
  NOTES    :  Patterns must be loaded before calling this function.
  REMEMBER :  The backpropagation learning function takes the learning
              parameter from parameterInArray[ 0 ]. parameterOutArray[ 0 ]
              returns the current net error.


  RETURNS  : Returns an error code if memory allocation has failed or if
             the parameters are invalid.
  UPDATE   :
******************************************************************************/
krui_err   krui_learnAllPatterns(float *parameterInArray, int NoOfInParams
                               , float **parameterOutArray, int *NoOfOutParams)
/*  REMEMBER:  parameterOutArray[ 0 ] returns the current net error
               parameterInArray[ 0 ] contains the 1st learning parameter
*/
{

    /*  learn all patterns  */
    return( kr_callNetworkFunction( LEARN_FUNC,
				    parameterInArray, NoOfInParams,
				    parameterOutArray, NoOfOutParams,  
				    0,kr_np_pattern(PATTERN_GET_NUMBER,0,0)-1));
}


/*****************************************************************************
  FUNCTION : krui_testAllPatterns

  PURPOSE  :  Test all pattern pairs using current learning method.
              parameterInArray contains the learning parameter(s). 
	      NoOfInParams stores the number of learning parameters.
              parameterOutArray returns the results from the learning function.
              this array is a static array defined in the learning function.
              *NoOfOutParams points to a integer value that contains the number
              of output parameters from the current learning function.
  NOTES    :  Patterns must be loaded before calling this function.

              direct copy of krui_learnAllPatterns!!!  joe

  REMEMBER :  The backpropagation learning function takes the learning
              parameter from parameterInArray[ 0 ]. parameterOutArray[ 0 ]
              returns the current net error.


  RETURNS  : Returns an error code if memory allocation has failed or if
             the parameters are invalid.
  UPDATE   : 19.02.95
******************************************************************************/
krui_err   krui_testAllPatterns(float *parameterInArray, int NoOfInParams,
                                float **parameterOutArray, int *NoOfOutParams)
/*  REMEMBER:  parameterOutArray[ 0 ] returns the current net error
               parameterInArray[ 0 ] contains the 1st learning parameter
*/
{

    /*  test all patterns  */
    return( kr_callNetworkFunction( TEST_FUNC,
                                   parameterInArray, NoOfInParams,
                                   parameterOutArray, NoOfOutParams,   
				   0, kr_np_pattern(PATTERN_GET_NUMBER,0,0)-1));
}


/*****************************************************************************
  FUNCTION : krui_learnSinglePattern

  PURPOSE  : Same as krui_learnAllPatterns( ... ) but learns only the current
             pattern pair.
  NOTES    : Patterns must be loaded before calling this function.

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err   krui_learnSinglePattern(int pattern_no, float *parameterInArray, 
				   int NoOfInParams, float **parameterOutArray,
				   int *NoOfOutParams)
{

    KernelErrorCode = KRERR_NO_ERROR;
    return( kr_callNetworkFunction( LEARN_FUNC,
                                   parameterInArray, NoOfInParams,
                                   parameterOutArray, NoOfOutParams,
                                   pattern_no - 1, pattern_no - 1 ) );
}


/*****************************************************************************
  FUNCTION : krui_testSinglePattern

  PURPOSE  : Same as krui_learnAllPatterns( ... ) but learns only the current
             pattern pair.
  NOTES    : Patterns must be loaded before calling this function.

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err   krui_testSinglePattern(int pattern_no, float *parameterInArray, 
				  int NoOfInParams, float **parameterOutArray, 
				  int *NoOfOutParams)
{

    KernelErrorCode = KRERR_NO_ERROR;
    return( kr_callNetworkFunction( TEST_FUNC,
                                   parameterInArray, NoOfInParams,
                                   parameterOutArray, NoOfOutParams,
                                   pattern_no - 1, pattern_no - 1 ) );
}


/*****************************************************************************
  FUNCTION : krui_learnAllPatternsFF

  PURPOSE  :  Learn all pattern pairs using current feed forward
              learning method.
              parameterInArray contains the learning parameter(s). 
	      NoOfInParams stores the number of learning parameters.
              parameterOutArray returns the results from the learning function.
              this array is a static array defined in the learning function.
              *NoOfOutParams points to a integer value that contains the number
              of output parameters from the current learning function.
  NOTES    :  Patterns must be loaded before calling this function.
  REMEMBER :  The backpropagation learning function takes the learning
              parameter from parameterInArray[ 0 ]. parameterOutArray[ 0 ]
              returns the current net error.


  RETURNS  : Returns an error code if memory allocation has failed or if
             the parameters are invalid.
  UPDATE   :
******************************************************************************/
krui_err krui_learnAllPatternsFF (float *parameterInArray, int NoOfInParams,
                                  float **parameterOutArray, int *NoOfOutParams)
{

  return(kr_callNetworkFunction(FF_LEARN_FUNC | LEARN_FUNC, parameterInArray, 
				NoOfInParams, parameterOutArray, NoOfOutParams,
				0, kr_np_pattern(PATTERN_GET_NUMBER, 0, 0)-1));

}


/*****************************************************************************
  FUNCTION : krui_learnSinglePatternFF

  PURPOSE  : Same as krui_learnAllPatternsFF ( ... ) but learns only
             the current pattern pair.
  NOTES    : Patterns must be loaded before calling this function.

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err krui_learnSinglePatternFF (int pattern_no, float *parameterInArray,
                                    int NoOfInParams, float **parameterOutArray,
                                    int *NoOfOutParams)
{

  KernelErrorCode = KRERR_NO_ERROR;
  return (kr_callNetworkFunction (FF_LEARN_FUNC | LEARN_FUNC, parameterInArray,
				  NoOfInParams, parameterOutArray, 
				  NoOfOutParams, pattern_no-1, pattern_no-1));

}


/*############################################################################

GROUP: Pruning Functions

############################################################################*/


/*****************************************************************************
  FUNCTION : krui_getPrunFunc

  PURPOSE  : returns the current pruning function
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
char *krui_getPrunFunc (void)
{

    static char prunFunc [FUNCTION_NAME_MAX_LEN];

    strcpy (prunFunc, krf_getCurrentNetworkFunc (PRUNING_FUNC));
    return (prunFunc);

}


/*****************************************************************************
  FUNCTION : krui_setPrunFunc

  PURPOSE  : changes the current pruning function
  NOTES    :

  RETURNS  : returns error code if pruning function is invalid
  UPDATE   :
******************************************************************************/
krui_err krui_setPrunFunc (char *pruning_func)
{

    return (krf_setCurrentNetworkFunc (pruning_func, PRUNING_FUNC));
}


/*############################################################################

GROUP: FF-Learning Functions

#############################################################################*/


/*****************************************************************************
  FUNCTION : krui_getFFLearnFunc

  PURPOSE  : returns the current FF-learning function
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
char *krui_getFFLearnFunc (void)
{

    static char FFLearnFunc [FUNCTION_NAME_MAX_LEN];

    strcpy (FFLearnFunc, krf_getCurrentNetworkFunc(FF_LEARN_FUNC | LEARN_FUNC));
    return (FFLearnFunc);

}


/*****************************************************************************
  FUNCTION : krui_setFFLearnFunc

  PURPOSE  : changes the current FF-learning function
  NOTES    :

  RETURNS  : returns error code if pruning function is invalid
  UPDATE   :
******************************************************************************/
krui_err krui_setFFLearnFunc (char *FF_learning_func)
{

    return (krf_setCurrentNetworkFunc(FF_learning_func, 
				      FF_LEARN_FUNC | LEARN_FUNC));
}


/*############################################################################

GROUP: Pattern Management

############################################################################*/

/*****************************************************************************
  FUNCTION : krui_setClassDistribution

  PURPOSE  : Sets the class distribution in the current pattern.
  NOTES    : Patterns must be loaded before calling this function.

  RETURNS  : Returns kernel error code 
  UPDATE   :
******************************************************************************/
krui_err krui_setClassDistribution(unsigned int *classDist)
{

    return(kr_npui_setClassDistribution(classDist));
}


/*****************************************************************************
  FUNCTION : krui_setClassInfo

  PURPOSE  : Sets the class name in the current pattern.
  NOTES    : 

  RETURNS  : Returns kernel error code 
  UPDATE   :
******************************************************************************/
krui_err krui_setClassInfo(char *name)
{
    printf("Kernel debug: new pattern class : %s \n",name);
    return(kr_npui_setClass(name));
}


/*****************************************************************************
  FUNCTION : krui_useClassDistribution

  PURPOSE  : Toggles the use of the user defined distribution of patterns 
             (TRUE) as compared to the distribution in the pattern file
  NOTES    : 

  RETURNS  : kernel error code
  UPDATE   :
******************************************************************************/
krui_err  krui_useClassDistribution(bool use_it)
{

    return( kr_npui_useChunk(use_it) );
}

/*****************************************************************************
  FUNCTION : krui_setPatternNo

  PURPOSE  : Sets the current pattern.
  NOTES    : Patterns must be loaded before calling this function.

  RETURNS  : Returns a error code if pattern number is invalid.
  UPDATE   :
******************************************************************************/
krui_err  krui_setPatternNo(int pattern_no)
{

    return( kr_np_pattern( PATTERN_SET, 0, pattern_no ) );
}

/*****************************************************************************
  FUNCTION : krui_getPatternNo

  PURPOSE  : Returns the current pattern number.
  NOTES    : 

  RETURNS  : 
  UPDATE   :
******************************************************************************/
krui_err  krui_getPatternNo(void)
{

    return( kr_np_pattern( PATTERN_GET, 0, 0 ) );
}

/*****************************************************************************
  FUNCTION : krui_deletePattern

  PURPOSE  :delete the current pattern pair
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_deletePattern(void)
{

    return( kr_np_pattern( PATTERN_DELETE, 0 , 0 ) );
}


/*****************************************************************************
  FUNCTION : krui_modifyPattern

  PURPOSE  : modify the current pattern pair
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_modifyPattern(void)
{

    return( kr_np_pattern( PATTERN_MODIFY, 0 , 0 ) );
}


/*****************************************************************************
  FUNCTION : krui_showPattern

  PURPOSE  : According to the mode krui_showPattern stores the current
             Pattern into the units activation (and/or output) values.
             The modes are:
               - OUTPUT_NOTHING
                  store input pattern into input units activations
               - OUTPUT_ACT
                  store input pattern into input units activations and
                  store output pattern into output units activations
               - OUTPUT_OUT
                  store input pattern into input units activations,
                  store output pattern into output units activations and
                  update output units output

  NOTES    : See include file glob_typ.h for mode constants.

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_showPattern(int mode)
{

    return( kr_np_pattern( PATTERN_SHOW, mode, 0 ) );
}


/*****************************************************************************
  FUNCTION : krui_allocNewPatternSet

  PURPOSE  : Allocate a new empty pattern set
  NOTES    :

  RETURNS  : Returns the number of the allcated pattern set;
             Returns error code if an error occured
  UPDATE   :
******************************************************************************/
krui_err  krui_allocNewPatternSet(int *set_no)
{

    KernelErrorCode = kr_npui_allocNewPatternSet(set_no);
    return KernelErrorCode;
}


/*****************************************************************************
  FUNCTION : krui_newPattern

  PURPOSE  : Creates a new pattern pair.
             A pattern pair can be created by modifying the activation
             value of the input/output units.
  NOTES    : krui_newPattern switches pattern shuffeling off.
             For shuffeling the new pattern pairs call
                 krui_newPattern(...)
                 krui_shufflePattern( TRUE )

  RETURNS  : Returns error code if memory is insufficent or no. of
             input/output units is incompatible, 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_newPattern(void)
{

    return( kr_np_pattern( PATTERN_NEW, 0, 0 ) );
}


/*****************************************************************************
  FUNCTION : krui_getNoOfPatterns

  PURPOSE  : Returns the no. of available pattern pairs.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
int  krui_getNoOfPatterns(void)
{
    int number;
    number = kr_np_pattern( PATTERN_GET_NUMBER, 0, 0 );
    return (number >= 0) ? number : 0;
}


/*****************************************************************************
  FUNCTION : krui_getTotalNoOfSubPatterns

  PURPOSE  : Returns the total no. of available subpattern pairs.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
int  krui_getTotalNoOfSubPatterns(void)
{
    int number;
    number = kr_np_pattern( SUBPATTERN_GET_NUMBER, 0, 0 );
    return (number >= 0) ? number : 0;
}


/*****************************************************************************
  FUNCTION : krui_deleteAllPatterns
  PURPOSE  : [ Release previously defined patterns from memory.
              Call krui_releasePatterns() if you want to create totally
              new patterns with krui_newPattern(). ]
  NOTES    : This function is of no effect any longer since there exists
             the new function krui_deletePatSet (see below)


  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_deleteAllPatterns(void)
{

    (void) kr_np_pattern( PATTERN_DELETE_ALL, 0, 0 );
}


/*****************************************************************************
  FUNCTION : krui_shufflePatterns

  PURPOSE  : Shuffle pattern pairs by using pseudo random generator.
             Shuffeling of patterns is used by krui_learnAllPatterns(...).
             krui_shufflePatterns( TRUE ) switches shuffeling of patterns
             on, krui_shufflePatterns( FALSE ) switches shuffeling of
             patterns off.
             The default presetting is krui_shufflePatterns( FALSE ).
  NOTES    : See also krui_setSeedNo( seed )

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_shufflePatterns(bool on_or_off)
{

    if (on_or_off)
        return kr_np_pattern( PATTERN_SHUFFLE_ON, 0, 0 );
    else
        return kr_np_pattern( PATTERN_SHUFFLE_OFF, 0, 0 );

}


/*****************************************************************************
  FUNCTION : krui_shuffleSubPatterns

  PURPOSE  : Shuffle sub pattern pairs by using pseudo random generator.
             Shuffeling of patterns is used by krui_learnAllPatterns(...).
             krui_shuffleSubPatterns(TRUE) switches shuffeling of sub patterns
             on, krui_shuffleSubPatterns( FALSE ) switches shuffeling of
             sub patterns off.
             The default presetting is krui_shuffleSubPatterns( FALSE ).
  NOTES    : See also krui_setSeedNo( seed )

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_shuffleSubPatterns(bool on_or_off)
{

    if (on_or_off)
        return kr_np_pattern( PATTERN_SUB_SHUFFLE_ON, 0, 0 );
    else
        return kr_np_pattern( PATTERN_SUB_SHUFFLE_OFF, 0, 0 );
}


/*****************************************************************************
  FUNCTION : krui_setCurrPatSet

  PURPOSE  : Choose the number of the current pattern set.
             <number> starts from 0.
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/
krui_err  krui_setCurrPatSet(int number)
{

    return kr_npui_setCurrPatSet(number);
}


/*****************************************************************************
  FUNCTION : krui_deletePatSet

  PURPOSE  : Delete all patterns of pattern set with number <number>
             The ordering of the remaining pattern sets is reorganized by
             decrementing the pattern set numbers which are higher than
             <number> by 1. For example: there exist the pattern sets 0, 1, 2
             and 3. After deleting pattern set 1 the pattern sets 2 and 3 are
             renamed to 1 and 2.
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/
krui_err  krui_deletePatSet(int number)
{

    return kr_npui_deletePatSet(number);
}


/*****************************************************************************
  FUNCTION : krui_GetPatInfo

  PURPOSE  : Get all available information concerning the current pattern set
             and the current pattern.
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/
krui_err  krui_GetPatInfo(pattern_set_info *set_info,
			  pattern_descriptor *pat_info)
{

    return kr_npui_GetPatInfo(set_info, pat_info);
}


/*****************************************************************************
  FUNCTION : krui_DefShowSubPat

  PURPOSE  : Define the sub pattern to be shown with next call to
             krui_showPattern
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/
krui_err  krui_DefShowSubPat(int *insize, int *outsize, int *inpos, int *outpos)
{

    return kr_npui_DefShowSubPat(insize, outsize, inpos, outpos);
}


/*****************************************************************************
  FUNCTION : krui_DefTrainSubPat

  PURPOSE  : Define how sub patterns should be generated during training
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/
krui_err  krui_DefTrainSubPat(int *insize, int *outsize,
                              int *instep, int *outstep, int *max_n_pos)
{

    return kr_npui_DefTrainSubPat(insize, outsize, instep, outstep, max_n_pos);
}


/*****************************************************************************
  FUNCTION : krui_AlignSubPat

  PURPOSE  : Align the given sub pattern position (<inpos> and <outpos>)
             to a valid position which fits the defined sub pattern training
             scheme (krui_DefTrainSubPat).

  RETURNS  : kernel error code
             <inpos> and <outpos> returns the aligned position
             <no> returns the resulting ordering position of the sub pattern
  NOTES    :

  UPDATE   :
******************************************************************************/
krui_err krui_AlignSubPat(int *inpos, int *outpos, int *no)
{

    return kr_npui_AlignSubPat(inpos, outpos, no);
}


/*****************************************************************************
  FUNCTION : krui_GetShapeOfSubPattern

  PURPOSE  : Get shape of sub pattern <n_pos> using current set,
             current pattern and current train scheme (defined with
             krui_DefTrainSubPat)
  RETURNS  :
  NOTES    :

  UPDATE   :
******************************************************************************/
krui_err  krui_GetShapeOfSubPattern(int *insize, int *outsize,
                                    int *inpos, int *outpos, int n_pos)
{

    return kr_npui_GetShapeOfSubPat(insize, outsize, inpos, outpos, n_pos);
}


/*############################################################################

GROUP: I/O Functions

############################################################################*/


/*****************************************************************************
  FUNCTION : krui_saveNet

  PURPOSE  : Save a network.
             If netname is a NULL pointer, the net will get the name "UNTITLED"
  NOTES    :

  RETURNS  :  Returns error code if an error occured, or 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_saveNet(char *filename, char *netname)
{

    return( krio_saveNet( filename, netname ) );
}


/*****************************************************************************
  FUNCTION : krui_loadNet

  PURPOSE  : Load a network file and create a network.
             Returns the name of the net or "UNTITLED" if unknown.
  NOTES    :

  RETURNS  : Returns error code if an error occured during
             loading/memory allocation, or 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_loadNet(char *filename, char **netname)
{
    char        *netfile_version; /*  isn't used now  */

    KernelErrorCode = KRERR_NO_ERROR;

    KernelErrorCode = krio_loadNet( filename, netname, &netfile_version );
    if (KernelErrorCode != KRERR_NO_ERROR)
        return( KernelErrorCode );

    kr_updateUnitOutputs();

    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_loadNewPatterns

  PURPOSE  : Load file containing pattern pairs of new Style conventions.
  NOTES    :

  RETURNS  : Returns the number of the loaded pattern set;
             Returns error code if an error occured during
             loading/memory allocation, or 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_loadNewPatterns(char *filename, int *set_no)
{

    KernelErrorCode = kr_npui_loadNewPatterns(filename, set_no);
    return KernelErrorCode;
}


/*****************************************************************************
  FUNCTION : krui_saveNewPatterns

  PURPOSE  : Save pattern file with new style conventions
             The pattern set with number <number> is saved.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_saveNewPatterns(char *filename, int set_no)
{

    KernelErrorCode = kr_npui_saveNewPatterns(filename, set_no);
    return KernelErrorCode;
}


/*****************************************************************************
  FUNCTION : krui_saveResultParam

  PURPOSE  : Save the network result which depends on the loaded patterns.
             If create is false, the new file will be appended to an existing
             file. startpattern and endpattern determine the range of patterns
             to use. The input patterns and the teaching output patterns can
             be included.
  NOTES    : This is a new version of the old function krui_saveResult with
             added parameter passing for the update functions.

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_saveResultParam(char *filename, bool create,
                               int startpattern, int endpattern,
                               bool includeinput, bool includeoutput,
                               float *Update_param_array, int NoOfUpdateParam)
{

    return( krio_saveResult(filename, create, startpattern, endpattern,
                            includeinput, includeoutput,
                            Update_param_array, NoOfUpdateParam));
}


/*############################################################################

GROUP: Functions for memory management

############################################################################*/


/*****************************************************************************
  FUNCTION : krui_allocateUnits

  PURPOSE  : Allocates a given number of units, additional units
             may allocated by calling this function again.
             This function is called automatically if the user
             construct more units than have been allocated before, but
             it is recommended to use this function if a large amount
             of units is needed (the UNIX System can manage system resources
             much better, if the amount of memory used for the network is
             approximately known before the network is created).
  NOTES    : If krui_create_unit has been called before using this
             function, at least <UNIT_BLOCK> numbers of units have been
             allocated.
             See "kr_def.h" for more information about memory allocation
             block sizes.

  RETURNS  : Returns error code if memory allocation fails, 0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_allocateUnits(int no_of_units)
{

    return( krm_allocUnits( no_of_units ) );
}


/*****************************************************************************
  FUNCTION : krui_deleteNet

  PURPOSE  :  Delete network, names and unit types.
              Frees all memory used for the network.
  NOTES    : If krui_loadNet is called more then once,  krui_deleteNet will
             be called by krui_loadNet, because the kernel have to free the
             memory used for the old network.
             It is recommended (but not neccessary) to call krui_deleteNet
             before terminating the SNNS-Kernel.

  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_deleteNet(void)
{

    krm_releaseMem();
}


/*############################################################################

GROUP: Functions for reading/searching the symbol table

############################################################################*/


/*****************************************************************************
  FUNCTION : krui_getFirstSymbolTableEntry

  PURPOSE  : Returns the first symbol/symbol type entry in the
             symbol table. Returns true if this entry is available,
             false otherwise.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
bool  krui_getFirstSymbolTableEntry(char **symbol_name, int *symbol_type)
{
    struct NameTable      *n_tbl;

    n_tbl = krm_getNTableFirstEntry();

    while ( (n_tbl != NULL) && ((int) n_tbl->sym_type == UNUSED_SYM) )
        n_tbl = krm_getNTableNextEntry();

    if (n_tbl == NULL) {
	*symbol_name = NULL;
	*symbol_type = 0;
	return( FALSE );
    }

    *symbol_name = n_tbl->Entry.symbol;
    *symbol_type = (int) n_tbl->sym_type;
    return( TRUE );
}


/*****************************************************************************
  FUNCTION : krui_getNextSymbolTableEntry

  PURPOSE  : Returns the next symbol/symbol type entry in the
             symbol table. Returns true if another entry is available,
             false otherwise.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
bool  krui_getNextSymbolTableEntry(char **symbol_name, int *symbol_type)
{
    struct NameTable      *n_tbl;

    n_tbl = krm_getNTableNextEntry();

    while ( (n_tbl != NULL) && ((int) n_tbl->sym_type == UNUSED_SYM) )
        n_tbl = krm_getNTableNextEntry();

    if (n_tbl == NULL) {
	*symbol_name = NULL;
	*symbol_type = 0;
	return( FALSE );
    }

    *symbol_name = n_tbl->Entry.symbol;
    *symbol_type = (int) n_tbl->sym_type;
    return( TRUE );
}


/*****************************************************************************
  FUNCTION : krui_symbolSearch

  PURPOSE  : Searches the symbol table for a given symbol and
             symbol type (unit name symbol, site name symbol,
             functionality unit name symbol)
  NOTES    :

  RETURNS  : Returns true, if the symbol exists.
  UPDATE   :
******************************************************************************/
bool  krui_symbolSearch(char *symbol, int symbol_type)
{

    return( krm_NTableSymbolSearch( symbol, symbol_type ) != NULL );
}


/*###########################################################################

GROUP: Miscellanous

############################################################################*/


/*****************************************************************************
  FUNCTION : krui_getVersion

  PURPOSE  : Returns the current Version of the SNNS-Kernel.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
char  *krui_getVersion(void)
{
    static char snns_version[128];

    strcpy( snns_version, SNNS_VERSION );
    strcat( snns_version, KERNEL_PATCH );
    return( snns_version );
}


/*****************************************************************************
  FUNCTION : krui_getNetInfo

  PURPOSE  : Returns miscellanous information about the current network.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_getNetInfo(int *no_of_sites, int *no_of_links, 
		      int *no_of_STable_entries, int *no_of_FTable_entries)
{
    int   array_size,
    info_array[ 10 ];

    krm_getMemoryManagerInfo( &array_size, info_array );

    *no_of_sites          = info_array[ 0 ];
    *no_of_links          = info_array[ 1 ];
    *no_of_STable_entries = info_array[ 2 ];
    *no_of_FTable_entries = info_array[ 3 ];
}


/*****************************************************************************
  FUNCTION : krui_getMemoryManagerInfo

  PURPOSE  : Returns the number of ALLOCATED (not the number of USED) bytes
    per entry.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_getMemoryManagerInfo(int *unit_bytes, int *site_bytes, 
				int *link_bytes, int *NTable_bytes, 
				int *STable_bytes, int *FTable_bytes)
{
    int   array_size,
    info_array[ 10 ];

    krm_getMemoryManagerInfo( &array_size, info_array );

    *unit_bytes   = info_array[ 4 ] * UNIT_SIZE;
    *site_bytes   = info_array[ 5 ] * SITE_SIZE;
    *link_bytes   = info_array[ 6 ] * LINK_SIZE;
    *NTable_bytes = info_array[ 7 ] * NTABLE_SIZE;
    *STable_bytes = info_array[ 8 ] * STABLE_SIZE;
    *FTable_bytes = info_array[ 9 ] * FTYPE_UNIT_SIZE;
}


/*****************************************************************************
  FUNCTION : krui_getUnitDefaults

  PURPOSE  : Returns Information about the unit default settings.
             For more information about default settings see 
	     krui_createDefaultUnit() and krui_createFTypeUnit( .. ).
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_getUnitDefaults(FlintType *act, FlintType *bias, int *st, 
			   int *subnet_no, int *layer_no, char **act_func, 
			   char **out_func)
{
    int ttflags;

    kr_getUnitDefaults( act, bias, &ttflags, subnet_no, layer_no,
                       act_func, out_func );

    *st = kr_flags2TType( ttflags );
}


/*****************************************************************************
  FUNCTION :  krui_setUnitDefaults

  PURPOSE  : Changes the unit default settings.
             For more information about default settings see 
	     krui_createDefaultUnit() and krui_createFTypeUnit( .. ).
  NOTES    :

  RETURNS  : Returns error code if
             - activation/output function is invalid
             - Topologic type is invalid
             0 otherwise.
  UPDATE   :
******************************************************************************/
krui_err  krui_setUnitDefaults(FlintTypeParam act, FlintTypeParam bias, 
			       int st, int subnet_no, int layer_no, 
			       char *act_func, char *out_func)
{
    int        ttflags;

    ttflags = kr_TType2Flags( st );
    if (KernelErrorCode != KRERR_NO_ERROR)
        return( KernelErrorCode );

    kr_setUnitDefaults( act, bias, ttflags, subnet_no, layer_no,
                       act_func, out_func );
    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krui_resetNet

  PURPOSE  : Reset the network by changeing the unit activation
             to the initial activation value.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
extern FlintType OUT_Custom_Python(FlintType act);

void  krui_resetNet(void)
{
    register int   i;
    register struct Unit   *unit_ptr;

    if ( (unit_array == NULL) || (NoOfUnits == 0) )
        return;


    for (i = MinUnitNo, unit_ptr = unit_array + MinUnitNo; i <= MaxUnitNo; 
	 i++, unit_ptr++)
        if UNIT_IN_USE( unit_ptr )  {
            /*  unit is in use  */
            unit_ptr->act = unit_ptr->i_act;

            if (unit_ptr->out_func == OUT_IDENTITY)
                unit_ptr->Out.output = unit_ptr->act;
            else if(unit_ptr->out_func == OUT_Custom_Python)
	    	unit_ptr->Out.output =
			kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
            else
                /*  no identity output function: calculate unit's output also */
                unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
        }
}


/*****************************************************************************
  FUNCTION : krui_setSeedNo

  PURPOSE  : Initialize the pseudo random generator.
             0 as argument reinitializes the random generator.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void  krui_setSeedNo(long int seed)
{

    if (seed != 0) {
	randomSeedVal = seed;
        srand48( seed );  
    } else {
        if (randomSeedVal == 0) {
            randomSeedVal = (long) time( (time_t *) 0);
            srand48( randomSeedVal ); 
        }
    }
}


/*****************************************************************************
  FUNCTION : krui_getNoOfInputUnits

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the no. of input units
  UPDATE   :
******************************************************************************/
int  krui_getNoOfInputUnits(void)
{

    return( kr_getNoOfUnits( INPUT ) + kr_getNoOfUnits( DUAL ) +
	    kr_getNoOfUnits( SPECIAL_I ) + kr_getNoOfUnits( SPECIAL_D ) );
}


/*****************************************************************************
  FUNCTION : krui_getNoOfOutputUnits

  PURPOSE  :
  NOTES    :

  RETURNS  : returns the no. of output units
  UPDATE   :
******************************************************************************/
int  krui_getNoOfOutputUnits(void)
{

    return( kr_getNoOfUnits( OUTPUT ) + kr_getNoOfUnits( DUAL ) +
	    kr_getNoOfUnits( SPECIAL_O ) + kr_getNoOfUnits( SPECIAL_D ) );
}
/*****************************************************************************
  FUNCTION : krui_getNoOfTTypeUnits

  PURPOSE  : returns the no. of units of the specified topologic type
             (i.e. Input, Hidden, Output or Special units)
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
int  krui_getNoOfTTypeUnits(int UnitTType)
{

    return( kr_getNoOfUnits( UnitTType ) );
}


/*****************************************************************************
  FUNCTION : krui_getNoOfSpecialInputUnits

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the no. of special input units
  UPDATE   :
******************************************************************************/
int  krui_getNoOfSpecialInputUnits(void)
{

  return( kr_getNoOfSpecialUnits( INPUT ) + kr_getNoOfUnits( DUAL ) );
}


/*****************************************************************************
  FUNCTION : krui_getNoOfSpecialOutputUnits

  PURPOSE  :
  NOTES    :

  RETURNS  : returns the no. of special output units
  UPDATE   :
******************************************************************************/
int  krui_getNoOfSpecialOutputUnits(void)
{

  return( kr_getNoOfSpecialUnits( OUTPUT ) + kr_getNoOfUnits( DUAL ) );
}


/*****************************************************************************
  FUNCTION : krui_xyTransTable

  PURPOSE  : Get/modify the XY-Translation table
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_xyTransTable(int op, int *x, int *y, int z)
{

    return(kr_xyTransTable(op,x,y,z));
}

/*****************************************************************************
  FUNCTION : krui_getUnitCenters

  PURPOSE  : Returns the 3D transform center of the specified unit and center number
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_getUnitCenters(int unit_no, int center_no, 
			      struct PositionVector **unit_center)
{
    struct Unit   *unit_ptr;

    KernelErrorCode = KRERR_NO_ERROR;

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    if ((center_no < 0) || (center_no >= NO_OF_UNIT_CENTER_POS))  {
        KernelErrorCode = KRERR_PARAMETERS;
        return( KRERR_PARAMETERS );
    }

    *unit_center = unit_ptr->unit_center_pos + center_no;
    return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION :  krui_setUnitCenters

  PURPOSE  : Sets the 3D transform center of the specified unit and center 
             number
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_setUnitCenters(int unit_no, int center_no, 
			      struct PositionVector *unit_center)
{
    struct Unit   *unit_ptr;
    struct PositionVector    *unit_posvec_ptr;

    KernelErrorCode = KRERR_NO_ERROR;

    if ((unit_ptr = kr_getUnitPtr( unit_no )) == NULL)
        return( KernelErrorCode );

    if ((center_no < 0) || (center_no >= NO_OF_UNIT_CENTER_POS))  {
        KernelErrorCode = KRERR_PARAMETERS;
        return( KRERR_PARAMETERS );
    }

    unit_posvec_ptr = unit_ptr->unit_center_pos + center_no;

    memcpy( (char *) unit_posvec_ptr, (char *) unit_center, 
	    sizeof( struct PositionVector ) );

    return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : krui_topo_err_msg

  PURPOSE  : generate a message about an error found while doing a 
             topologiacal sorting of the network units
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
static char  *krui_topo_err_msg(void)
{
    char  *dest_unit_name,  *src_unit_name;
    static char  msg1[512];
    static char  msg2[512];

    /* Init */
    msg1[0] = '\0';
    msg2[0] = '\0';

    if (topo_msg.dest_error_unit > 0)
        dest_unit_name = krui_getUnitName( topo_msg.dest_error_unit );

    if (topo_msg.src_error_unit > 0)
        src_unit_name = krui_getUnitName( topo_msg.src_error_unit );

    if (topo_msg.dest_error_unit > 0)
        if (dest_unit_name == NULL)
            sprintf( msg1, "Unit #%d is the destination unit. ", 
		     topo_msg.dest_error_unit );
        else
            sprintf( msg1, "Unit #%d (%s) is the destination unit. ", 
		     topo_msg.dest_error_unit, dest_unit_name );

    if (topo_msg.src_error_unit > 0)
        if (src_unit_name == NULL)
            sprintf( msg2, "Unit #%d is the source unit. ", 
		     topo_msg.src_error_unit );
        else
            sprintf( msg2, "Unit #%d (%s) is the source unit. ", 
		     topo_msg.src_error_unit, src_unit_name );

    if (topo_msg.dest_error_unit == 0)
        return( msg2 );

    if (topo_msg.src_error_unit > 0)
        strcat( msg1, msg2 );

    return( msg1 );
}


/*****************************************************************************
  FUNCTION : krui_error

  PURPOSE  : Returns an error message, depending on the error code.
             If a function returns an error code use krui_error to
             get the message.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
char  *krui_error(int error_code)
{
    static char *err_message[] = {
        "Invalid error code",
        "Insufficient memory",
        "Invalid unit number",
        "Invalid unit output function",
        "Invalid unit activation function",
        "Invalid site function",
        "Creation of sites isn't permitted because unit has direct input links",
        "Creation of a link isn't permitted because there exists already a link between these units",
        "Memory allocation failed during critical operation. Have lost some pointers, but consistency of the network is guaranteed",
        "Ftype name isn't definite",
        /*10*/"Current Ftype entry isn't defined",
        "Invalid copy mode",
        "Current unit doesn't have sites",
        "Can't update unit because unit is frozen",
        "Redefinition of site name isn't permitted (site name already exists)",
        "Site name isn't defined",
        "This is not a 3D-Kernel",
        "This unit has already a site with this name",
        "Can't delete site table entry because site is in use",
        "Current Ftype site isn't defined",
        /*20*/"Ftype symbol isn't defined",
        "I/O error: ",
        "Creation of output file failed (line length limit exceeded)",
        "The network has not enough layers: ",
        "No Units defined",
        "Unexpected EOF",
        "Line length exceeded",
        "Incompatible file format",
        "Can't open file",
        "Syntax error",
        /*30*/"Memory allocation error #01",
        "Topologic type invalid",
        "Symbol pattern invalid (must match [A-Za-z][^|, ]*)",
        "Current unit doesn't have a site with this name",
        "No hidden units defined",
        "Network contains cycle(s): ",
        "Network contains dead unit(s): ",
        "Pattern file contains not the same no. of input units as the network",
        "Pattern file contains not the same no. of output units as the network",
        "No. of input units have changed",
        /*40*/"No. of output units have changed",
        "No input units defined",
        "No output units defined",
        "No patterns defined",
        "In-Core patterns incompatible with current network (remove In-Core patterns before loading a new network)",
        "Invalid pattern number",
        "Invalid learning function",
        "Invalid parameters",
        "Invalid update function",
        "Invalid initialisation function",
        /*50*/"Derivation function of the activation function doesn't exist",
        "Input unit(s) with input connection(s) to other units: ",
        "Output unit(s) with output connection(s) to other units: ",
        "Invalid topological sorting mode",
        "Learning function doesn't support sites",
        "Sites are not supported",
        "This isn't a MasPar Kernel",
        "Connection(s) between unit(s) in non-neighbour layers are not supported: ",
        "The network has too much layers: ",
        "The network layers aren't fully connected",
        /*60*/"This operation isn't allowed in the parallel kernel mode",
        "Change of network type isn't possible in parallel kernel mode",
        "No current link defined",
        "No current unit defined",
        "Current unit doesn't have any inputs",
        "Invalid parameter in topologic definition section",
        "Creation of link between these units isn't permitted",
        "MasPar don't respond",
        "This function isn't implemented yet",
        "Kernel isn't in parallel mode",
        /*70*/"MasPar ran out of memory",
        "MasPar communication error",
        "MasPar ran out of processors",
        "Missing default function (check function table)",
        "MasPar kernel doesn't support multiple unit output functions",
        "MasPar kernel doesn't support multiple unit activation functions",
        "The depth of the network doesn't fit to the learning function",
        "Wrong no of units in layer: ",
        "Unit is missing or not correctly connected: ",
        "Unit doesn't belong to a defined layer in the network: ",
        /*80*/"Unit has wrong activation function: ",
        "Unit has wrong output function: ",
        "Unexpected site function at unit: ",
        "Unit is not expected to have sites: ",
        "Unit is expected to have sites: ",
        "Site missing at unit: ",
        "Unexpected link: ",
        "Missing link(s) to unit: ",
        "Link ends at wrong site of destination unit: ",
        "This network is not fitting the required topology",
        /*90*/"Wrong beta parameter in unit bias value: ",
        "CC error: Topo_ptr_array is sorted in the wrong way", /*CC_ERROR1*/
        "CC error: There is no memory allocated", /*CC_ERROR2*/
        "CC error: Not enough memory to run Casscade", /*CC_ERROR3*/
        "Invalid error code", /*RCC_ERROR4*/
        "Invalid error code", /*RCC_ERROR5*/
        "CC error: Hidden layer is not fitting the required topology",
        "Invalid error code", /*RCC_ERROR7*/
        "Invalid error code", /*RCC_ERROR8*/
        "Invalid error code", /*RCC_ERROR9*/
        /*100*/ "Wrong update function",/*CC_ERROR10*/
        "Wrong init function",  /*CC_ERROR11*/
        "DLVQ error: There are empty classes",
        "DLVQ error: There exists a class lower than zero",
        "DLVQ error: Wrong no. of output units",
        "DLVQ error: This network is not fitting the required topology",
        "DLVQ error: There does not exist a unit for every class",
        "No more free pattern sets available",
        "No such pattern set defined",
        "No current pattern defined",
        /*110*/ "Specified sub pattern does not fit into pattern",
        "No such pattern available",
        "No current pattern set defined",
        "Pattern (sub pattern) does not fit the network",
        "No sub pattern shifting scheme defined",
        "Pattern contains no output information",
        "New pattern does not fit into existing set",
        "Paragon kernel not initialized",
        "Paragon kernel already initialized",
        "Sending a message fails",
        /*120*/ "Syntax error in received message",
        "Receive unknown command",
        "Less patterns then allocated nodes",
        "Weight update with global sum fails",
        "Learning function not parallelized",
        "Invalid error code", /* RPC-Call failed */
        "Invalid error code", /* RPC-Timeout */
        "Invalid error code", /* RPC: No Server defined */
        "Invalid error code", /* RPC: Cooperativ failed */
        "Invalid error code", /* RPC: No response from the slaves */
        /*130*/ "Algorithm needs patterns. Please press TEST first to check patterns.",
	"RBF-DDA: First input parameter out of range 0<theta_pos<=1.",
	"RBF-DDA: Second input parameter out of range 0<theta_neg<=1.",
	"RBF-DDA: Third input parameter must be >=0.",
	"RBF-DDA: More than one desired class in output pattern.",
	"RBF-DDA: Input-hidden connection pointer problem.",
	"RBF-DDA: Input-output shortcut connections are not allowed.",
	"RBF-DDA: Activation function of input units must be Act_Identity.",
	"RBF-DDA: Activation function of hidden units must be Act_RBF_Gaussian.",
	"RBF-DDA: Activation function of output units must be Act_Identity.",
        /*140*/ "CC error : Invalid additional Parameters.",
        "Activation-functions have to be Act_Threshold.",
        "Learning function must be online Backpropagation",
	"No learning possible with only one class",
	"Invalid pattern remap function",
        "Patterns don't have class information",
	"Illegal virtual class distribution",
	"Patterns can not be normalized"
    };  /* 147 error messages */

    static char *ext_messages[] = {
        "SNNS-Kernel No Errors",
        "SNNS-Kernel Error: ",
        "SNNS-Kernel Network Topologic Error: "
        };

    int  NoOfMessages = (sizeof (err_message)) / (sizeof (err_message[0]));
    static char  mesg[512], aux[512];


    if (error_code == 0)        return( ext_messages[0] ); /*   No errors  */

    error_code = abs( error_code );
    if ( error_code >= NoOfMessages )  error_code = 0; /* invalid error code */

    switch (-error_code)
        {
          case KRERR_CYCLES:
          case KRERR_DEAD_UNITS:
          case KRERR_I_UNITS_CONNECT:
          case KRERR_O_UNITS_CONNECT:
          case KRERR_NOT_NEIGHBOUR_LAYER:
          case KRERR_ACT_FUNC:
          case KRERR_OUT_FUNC:
          case KRERR_SITE_FUNC:
          case KRERR_UNEXPECTED_SITES:
          case KRERR_UNEXPECTED_DIRECT_INPUTS:
          case KRERR_SITE_MISSING:
          case KRERR_UNEXPECTED_LINK:
          case KRERR_LINK_MISSING:
          case KRERR_LINK_TO_WRONG_SITE:
          case KRERR_PARAM_BETA:
          case KRERR_UNDETERMINED_UNIT:

            strcpy( mesg, ext_messages[2] );
            strcat( mesg, err_message[ error_code ] );

            switch (-error_code){
	    case KRERR_CYCLES:
		sprintf( aux, "%d cycle(s) in the network. ", 
			 topo_msg.no_of_cycles );
		strcat( mesg, aux );
		break;
	    case KRERR_DEAD_UNITS:
		sprintf( aux, "%d dead unit(s) in the network. ", 
			 topo_msg.no_of_dead_units );
		strcat( mesg, aux );
		break;

	    default: break;
	    }

            strcat( mesg, krui_topo_err_msg() );
            return( mesg );


          case KRERR_FEW_LAYERS:
          case KRERR_MUCH_LAYERS:
          case KRERR_NOT_FULLY_CONNECTED:
	      strcpy( mesg, ext_messages[2] );
	      strcat( mesg, err_message[ error_code ] );

	      switch (-error_code){
	      case KRERR_FEW_LAYERS:
		  sprintf(aux, "Only %d layers found.", topo_msg.no_of_layers );
		  break;
	      case KRERR_MUCH_LAYERS:
		  sprintf( aux, "%d layers found.", topo_msg.no_of_layers );
		  break;

	      default: break;
	      }

	      strcat( mesg, aux );
	      return( mesg );

          case KRERR_NO_OF_UNITS_IN_LAYER:
	      strcpy (mesg, ext_messages[2]);
	      strcat (mesg, err_message [error_code]);
	      sprintf (aux, "The name of the layer is: %s", topo_msg.name);
	      strcat (mesg, aux);
	      return (mesg);

          case KRERR_UNIT_MISSING:
	      strcpy (mesg, ext_messages[2]);
	      strcat (mesg, err_message [error_code]);
	      sprintf (aux, "The missing unit is the %s unit.", topo_msg.name);
	      strcat (mesg, aux);
	      return (mesg);



          case KRERR_FILE_OPEN:
	      lineno = 0;

          default:
	      if (lineno != 0){               /*  file I/O error  */
		  strcpy( mesg, ext_messages[1] );
		  sprintf( aux, "Loading file failed at line %d : ", lineno );
		  strcat( mesg, aux );
		  strcat( mesg, err_message[ error_code ] );

		  lineno = 0;
		  return( mesg );
	      } else {               /*  standard error  */
		  strcpy( mesg, ext_messages[1] );
		  strcat( mesg, err_message[ error_code ] );
		  return( mesg );
	      }
        }
}


/*****************************************************************************
  FUNCTION :  krui_NA_Error

  PURPOSE  : calculates the error for the network-analyzer tool
  NOTES    :

  RETURNS  : Returns the float value of the error
  UPDATE   :
******************************************************************************/
float krui_NA_Error(int currentPattern, int error_unit, int error, bool ave)
{

  return( kr_NA_Error(currentPattern, error_unit, error, ave) );
}


/*############################################################################

  Functions for the external kernels

########################################################################### */


/*****************************************************************************
  FUNCTION : krui_setSpecialNetworkType

  PURPOSE  : Sets the topologic type of the current network.
             Returns an error if the topologic type of the current network
             doesn't fit to this type.
             Topologic types are:
               - NET_TYPE_GENERAL
                 general purpose network type with no limitations
               - NET_TYPE_FF1
                 feedforward network with fully connected units in
                 neighbour layers
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_setSpecialNetworkType(int net_type)
{

    (void) kr_setSpecialNetworkType( net_type );
    return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION :  krui_getSpecialNetworkType

  PURPOSE  :
  NOTES    :

  RETURNS  : Returns the special topologic type of the current network, if set.
  UPDATE   :
******************************************************************************/
int  krui_getSpecialNetworkType(void)
{

    return( kr_getSpecialNetworkType() );
}


/*****************************************************************************
  FUNCTION : krui_initInversion

  PURPOSE  : initialize net for inversion algorithm
  NOTES    :
  UPDATE   : 06.02.92
******************************************************************************/
int krui_initInversion(void)
{

    return( kr_initInversion() );
}


/*****************************************************************************
  FUNCTION : krui_inv_forwardPass

  PURPOSE  : topological forward propagation of the inversion algorithm
  NOTES    :
  UPDATE   : 29.01.92
******************************************************************************/
void  krui_inv_forwardPass(struct UnitList *inputs)
{

    kr_inv_forwardPass(inputs);
}


/*****************************************************************************
  FUNCTION : krui_inv_backwardPass

  PURPOSE  : Backward error propagation (topological)
  NOTES    :
  UPDATE   : 04.02.92
*****************************************************************************/
double krui_inv_backwardPass(float learn, float delta_max, int *err_units,
                             float ratio, struct UnitList *inputs,
                             struct UnitList *outputs)
{

    return(kr_inv_backwardPass(learn,delta_max,err_units,
                               ratio,inputs,outputs) ) ;
}


/*###########################################################################

  Functions for the MasPar kernel

########################################################################### */


#ifdef MASPAR_KERNEL
/*****************************************************************************
  FUNCTION : krui_MasPar

  PURPOSE  : Connects and disconnects the MasPar.
             The mode switches are:  MASPAR_CONNECT and MASPAR_DISCONNECT.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_MasPar(int mode )
{

  return( kr_initMasPar( mode ) );
}


/*****************************************************************************
  FUNCTION : krui_getMasParStatus

  PURPOSE  : Returns the status of the MasPar.
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_getMasParStatus(void)
{

  return( kr_getMasParStatus() );
}


/*****************************************************************************
  FUNCTION : krui_MasParBenchmark

  PURPOSE  : Perform benchmark tests
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err  krui_MasParBenchmark(int func_type,int cycles,float  *result )
{
    float  parameterInArray[NO_OF_LEARN_PARAMS];
    float  *parameterOutArray;
    int  NoOfOutParams;


    parameterInArray[0] = (float) cycles;
    (void) kr_callNetworkFunction( func_type | BENCH_FUNC,
				   parameterInArray, 1, &parameterOutArray, 
				   &NoOfOutParams, 0, 0 );

    *result = parameterOutArray[0];
    return( KernelErrorCode );
}


#endif


/*****************************************************************************
  FUNCTION :  krui_kohonen_SetExtraParameter

  PURPOSE  :
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void krui_kohonen_SetExtraParameter(int x)
{

    kohonen_SetExtraParameter(x);
}


/*****************************************************************************
  FUNCTION :  krui_spanning_tree

  PURPOSE  :
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void krui_spanning_tree(void)
{

    spanning_tree();
}


/*****************************************************************************
  FUNCTION :  krui_cc_updatePosOfSpecialUnits

  PURPOSE  :
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void krui_cc_updatePosOfSpecialUnits(void)
{

    cc_updatePosOfSpecialUnits();
}


/*****************************************************************************
  FUNCTION :  krui_cc_deleteAllSpecialUnits
  PURPOSE  :
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err krui_cc_deleteAllSpecialUnits(void)
{

    return(cc_deleteAllSpecialUnits());
}


/*****************************************************************************
  FUNCTION :  krui_getErrorCode
  PURPOSE  :
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
int krui_getErrorCode(void)
{

    return(krui_error_code);
}


/*****************************************************************************
  FUNCTION :  krui_setErrorHandler
  PURPOSE  :
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
krui_err krui_setErrorHandler(void(* error_Handler )(int))
{

    krui_errorHandler = error_Handler;
    return(0);
}

/*****************************************************************************
  FUNCTION :   krui_execHandler
  PURPOSE  :
  NOTES    :

  RETURNS  :
  UPDATE   :
******************************************************************************/
void krui_execHandler(int error_code)
{

    if(krui_errorHandler != NULL)  ((*krui_errorHandler) (error_code));
}

/************************************************************************************

   This file is part of SnnsCLib, a fork of the kernel and parts of the gui of 
   the Stuttgart Neural Network Simulator (SNNS), version 4.3.

   The file's original version is part of SNNS 4.3. It's source code can be found at

   http://www.ra.cs.uni-tuebingen.de/SNNS/

   SNNS 4.3 is under the license LGPL v2. We note that source code files of SNNS 4.3 
   state as version "4.2". Base of this fork is SNNS 4.3 with a reverse-applied 
   python patch (see http://developer.berlios.de/projects/snns-dev/).

   SnnsCLib was developed in 2010 by Christoph Bergmeir under supervision of 
   José M. Benítez, both affiliated to DiCITS Lab, Sci2s group, DECSAI, 
   University of Granada

   Changes done to the original code were performed with the objective to
   port it from C to C++ and to encapsulate all code in one class named SnnsCLib.

   Changes in header files mainly include:
   * removed all static keywords
   * moved initializations of variables to the constructor of SnnsCLib

   Changes in cpp code files mainly include:
   * changed file ending from .c to .cpp
   * removed all SNNS internal includes and only include SnnsCLib   
   * static variables within functions were turned into member variables of SnnsCLib
   * function declarations were changed to method declarations, i.e. "SnnsCLib::.."
     was added
   * calls to the function table are now "C++-style", using the "this"-pointer

   License of SnnsCLib:
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

************************************************************************************/


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
bool krf_funcSearch( const char *func_name, int  func_type, FunctionPtr  *func_ptr );

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
char  CurrNetworkFunc[NO_OF_FUNC_TYPES * NO_OF_KERNELS][FUNCTION_NAME_MAX_LEN];

bool  netFuncInit[NO_OF_FUNC_TYPES * NO_OF_KERNELS];

krui_err  krf_getInternalFuncInfo(int mode, struct FuncInfoDescriptor *func_descr);
void  krf_storeCurrentNetworkFunc(char *function_name, int type);


/* end private definition section */

#endif 



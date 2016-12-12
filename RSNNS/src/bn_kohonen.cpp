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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_kohonen.c,v $
  SHORTNAME      : bn_kohonen
  SNNS VERSION   : 4.2

  PURPOSE        : Bignet generator of n-Component Kohonen Networks 
  NOTES          :

  AUTHOR         : Marcus Ritt, Marc Seemann 
  DATE           : July 13 1993

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#include "SnnsCLib.h"

#define CHECK_RETURN(ret_code)  if (ret_code != KRERR_NO_ERROR) \
                                   return (ret_code)


/*****************************************************************************
  FUNCTION : bn_kohonen_createNet

  PURPOSE  : create the Kohonen Network 
  NOTES    : 
  RETURNS  :

  UPDATE   : june 6 1993
******************************************************************************/

krui_err SnnsCLib::bn_kohonen_createNet(int X, int Y, int IUnits, int HUnits)
{
  int i,j,unit_no;
  struct PosType    unit_pos;
  krui_err ret;

  unit_pos.z = 0;

  /*  Allocate units */
  
  ret = krui_allocateUnits( HUnits + IUnits );
  CHECK_RETURN ( ret );


  /*  Create standard (input) Units  */

  unit_pos.x = 1;
  for (i = 1; i <= IUnits; i++) {
    unit_no = krui_createDefaultUnit();
    if (unit_no < 0)  CHECK_RETURN( unit_no );
    ret = krui_setUnitTType( unit_no, INPUT );
    CHECK_RETURN( ret );
    
    unit_pos.y = (IUnits<Y)?i+(Y-IUnits)/2:i;
    krui_setUnitPosition( unit_no, &unit_pos );
  }


  /* Create standard hidden Units. The size of the feature map is X*Y */
  
  for (i = 1; i <= Y; i++)
    for (j = 1; j <= X; j++) {
      unit_pos.x = 4+j;
      unit_no = krui_createDefaultUnit();
      if (unit_no < 0)  CHECK_RETURN( unit_no );
      ret = krui_setUnitTType( unit_no, HIDDEN );
      CHECK_RETURN( ret );
      
      unit_pos.y = i;
      krui_setUnitPosition( unit_no, &unit_pos );
    }
  

  /* Make connections between input units and hidden units  */

  /* set all link weights to zero */

  for (i = IUnits + 1; i <= IUnits + HUnits; i++) {

      /*  Make hidden unit to current unit  */
      ret = krui_setCurrentUnit( i );
      CHECK_RETURN( ret );
    
      /* (backward) connect current (hidden) unit with input unit */
      for (j = 1; j <= IUnits; j++) {
	  ret = krui_createLink( j,0.0);
	  CHECK_RETURN( ret );
      }
  } 
         

  /*  set the update function  */

  ret = krui_setUpdateFunc (const_cast<char*>(KOHONEN_UPDATE_FUNC_NAME));
  CHECK_RETURN( ret );


  /* set the learning function */

  ret = krui_setLearnFunc (const_cast<char*>(KOHONEN_LEARN_FUNC_NAME));
  CHECK_RETURN( ret );


  /* set the init function */

  ret = krui_setInitialisationFunc (const_cast<char*>(KOHONEN_INIT_FUNC_NAME));
  //CHECK_RETURN( ret );

  return(ret);

} /* bn_kohonen_createNet */

/* end of file */

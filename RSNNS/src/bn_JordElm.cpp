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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_JordElm.c,v $
  SHORTNAME      : bn_JordElm.c
  SNNS VERSION   : 4.2

  PURPOSE        : Bignet panel for Jordan and Elman networks
  NOTES          :

  AUTHOR         : Tobias Soyez
  DATE           : 09.11.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.13 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:36 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#include <string.h>

#include "SnnsCLib.h"

/*****************************************************************************
  FUNCTION : createUnitLayer

  PURPOSE  : creates a layer of no_of_units units 
  RETURNS  : the number of the first unit of the created layer or a kernel
             error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
 int SnnsCLib::createUnitLayer (int  no_of_units, 
  			    int  io_type,
		    	    char *act_func_name,
			    char *out_func_name,
			    char *unit_name)

{
    krui_err     error_code = KRERR_NO_ERROR ;
    int          i, unit_no, first_unit = -1;
    char         name[100] ;

  
    if (no_of_units < 1) return (KRERR_NO_UNITS) ;

    for (i = 1 ; i <= no_of_units ; i++){
	sprintf (name, "%s%d", unit_name, i) ;

	unit_no = krui_createUnit (name, out_func_name, act_func_name, 0, 0) ; 
	if (unit_no < 0) IF_ERROR_RETURN (unit_no) ;

	if (i == 1) first_unit = unit_no ;

	error_code = krui_setUnitTType (unit_no, io_type) ;

	IF_ERROR_RETURN (error_code) ;
    }

    return (first_unit) ;
}



/*****************************************************************************
  FUNCTION : connectFull

  PURPOSE  : connects every unit of one set (no_of_source_units units, 
             beginning with from_unit) to every unit of a second set 
             no_of_target_units units, beginning with to_unit)
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
 krui_err SnnsCLib::connectFull (int from_unit, int no_of_source_units,
			     int to_unit,   int no_of_target_units,
			     FlintType weight) 
{
    krui_err  error_code = KRERR_NO_ERROR ;
    int       i, j ;

  
    for (i = to_unit ; i < to_unit + no_of_target_units ; i++){
	error_code = krui_setCurrentUnit (i) ;
	IF_ERROR_RETURN (error_code) ;

	for (j = from_unit ; j < from_unit + no_of_source_units ; j++){
	    error_code = krui_createLink (j, weight) ;
	    IF_ERROR_RETURN (error_code) ;
	} 
    }

    return (error_code) ;
}



/*****************************************************************************
  FUNCTION : connectOneToOne

  PURPOSE  : connects every unit of one set (no_of_units units, beginning 
             with from_unit) to the corresponding unit in the second set
             (no_of_units units, beginning with to_unit)
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   :
******************************************************************************/
 krui_err SnnsCLib::connectOneToOne (int from_unit, int to_unit,
			         int no_of_units, FlintType weight) 
{
    krui_err  error_code = KRERR_NO_ERROR ;
    int       i ;


    for (i = 0 ; i < no_of_units ; i++){
	error_code = krui_setCurrentUnit (to_unit + i) ;
	IF_ERROR_RETURN (error_code) ;

	error_code = krui_createLink (from_unit + i, weight) ;
	IF_ERROR_RETURN (error_code) ;
    }

    return (error_code) ;
}



/*****************************************************************************
  FUNCTION : connectSelfRec

  PURPOSE  : connects every unit of a set (no_of_units units, beginning with
             unit_no) to itself
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
 krui_err SnnsCLib::connectSelfRec (int unit_no, int no_of_units, FlintType weight) 
{
    krui_err  error_code = KRERR_NO_ERROR ;
    int       i ;


    for (i = unit_no ; i < unit_no + no_of_units ; i++)
    {
	error_code = krui_setCurrentUnit (i) ;
	IF_ERROR_RETURN (error_code) ;
 
	error_code = krui_createLink (i, weight) ;
	IF_ERROR_RETURN (error_code) ;
    }
    return (error_code) ;
}



/*****************************************************************************
  FUNCTION : setPosRow

  PURPOSE  : sets the positions of a set of units (no_of_units units,
             beginning with unit_no) starting at (x, y) with max. r units
             per row
  RETURNS  : void
  NOTES    :

  UPDATE   : 
******************************************************************************/
 void SnnsCLib::setPosRow (int unit_no, int no_of_units, int x, int y, int r)
{
    struct PosType  unit_pos ;
    int             col, row ;
    int             i ;
 
    unit_pos.z = 0;

    col = x ;
    row = y ;

    for (i = unit_no ; i < unit_no + no_of_units ; i++){
	unit_pos.x = col ;
	unit_pos.y = row ;
	krui_setUnitPosition (i, &unit_pos) ;
	col++ ;
	if ((col - x) == r){
	    col = x ;
	    row++   ;
	}
    }
}

/*****************************************************************************
  FUNCTION : bn_jordan_createNet

  PURPOSE  : generates the JORDAN network 
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   :
******************************************************************************/
 krui_err SnnsCLib::bn_jordan_createNet (int IUnits, int HUnits, int OUnits,
                                     int ICols , int HCols , int OCols )
{
  krui_err  error_code = KRERR_NO_ERROR;
  int       i_unit, h_unit, o_unit, s_unit ;
  int       idy, hdy, maxdy ;

 
  /* ---------------------------   create units  ----------------------------*/

  i_unit = createUnitLayer(IUnits,INPUT,const_cast<char*>("Act_Identity"),const_cast<char*>("Out_Identity"),const_cast<char*>("inp"));
  if (i_unit < 0) IF_ERROR_RETURN (i_unit) ;

  h_unit = createUnitLayer(HUnits,HIDDEN,const_cast<char*>("Act_Logistic"),const_cast<char*>("Out_Identity"),const_cast<char*>("hid"));
  if (h_unit < 0) IF_ERROR_RETURN (h_unit) ;

  o_unit = createUnitLayer(OUnits,OUTPUT,const_cast<char*>("Act_Logistic"),const_cast<char*>("Out_Identity"),const_cast<char*>("out"));
  if (o_unit < 0) IF_ERROR_RETURN (o_unit) ;

  s_unit =createUnitLayer(OUnits,SPECIAL_H,const_cast<char*>("Act_Identity"),const_cast<char*>("Out_Identity"),const_cast<char*>("con"));
  if (s_unit < 0) IF_ERROR_RETURN (s_unit) ;


  /* ---------------------------   create links  ----------------------------*/

  error_code = connectFull     (i_unit, IUnits, h_unit, HUnits, 0.0) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = connectFull     (h_unit, HUnits, o_unit, OUnits, 0.0) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = connectFull     (s_unit, OUnits, h_unit, HUnits, 0.0) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = connectOneToOne (o_unit, s_unit, OUnits, 0.0) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = connectSelfRec  (s_unit, OUnits, 0.0) ; 
  IF_ERROR_RETURN (error_code) ;


  /* -------------  calculate y-coordinate of state layer  ------------------*/

  idy = (int) ((IUnits + ICols - 1) / ICols) ;
  hdy = (int) ((HUnits + HCols - 1) / HCols) ;

  if (idy > hdy) maxdy = idy ;
  else           maxdy = hdy ;


  /*  --------------------------  set positions  ----------------------------*/

  setPosRow (i_unit, IUnits, 1                  , 1        , ICols) ;
  setPosRow (h_unit, HUnits, 5+ICols+OCols      , 1        , HCols) ;
  setPosRow (o_unit, OUnits, 9+ICols+HCols+OCols, 1        , OCols) ;
  setPosRow (s_unit, OUnits, 3+ICols            , 1 + maxdy, OCols) ;


  /*  -----------------------  set default functions  -----------------------*/

  error_code = krui_setLearnFunc (const_cast<char*>("JE_BP")) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = krui_setInitialisationFunc (const_cast<char*>("JE_Weights")) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = krui_setUpdateFunc (const_cast<char*>("JE_Order")) ;

  return (error_code) ;
} 


/*****************************************************************************
  FUNCTION : bn_elman_createNet

  PURPOSE  : generates the ELMAN network 
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/

 krui_err SnnsCLib::bn_elman_createNet (int layer[], int columns[], int no_of_layers, bool out_context)

{
/*
Parameters:
 int      no_of_layers  = 3 ;
 int      layer[MAX_NO_OF_LAYERS]   ;
 int      columns[MAX_NO_OF_LAYERS] ;
 bool     out_context ;
*/

  krui_err  error_code = KRERR_NO_ERROR;
  int       i_unit, o_unit ;
  int       h_unit[MAX_NO_OF_LAYERS], c_unit[MAX_NO_OF_LAYERS] ;
  int       i, no_of_context_layers ;
  int       no_of_hidden_layers, o_layer ;
  int       dx, cx, cy ;
  char      name[100] ;


  no_of_hidden_layers = no_of_layers - 2 ;
  o_layer             = no_of_layers - 1 ;   /* no. of the output layer */


  /* ---------------------------   create units  ----------------------------*/

  i_unit = createUnitLayer (layer[0], INPUT, const_cast<char*>("Act_Identity"), 
                            const_cast<char*>("Out_Identity"), const_cast<char*>("inp")) ;
  if (i_unit < 0) IF_ERROR_RETURN (i_unit) ;

  strcpy (name, "hid") ;
  for (i = 1 ; i <= no_of_hidden_layers ; i++)
  {
    if (no_of_hidden_layers > 1) sprintf (name, "hid%d", i) ;  
    h_unit[i] = createUnitLayer(layer[i], HIDDEN, const_cast<char*>("Act_Logistic"), 
                                const_cast<char*>("Out_Identity"), name) ;
    if (h_unit[i] < 0) IF_ERROR_RETURN (h_unit[i]) ;
  }

  o_unit = createUnitLayer (layer[o_layer], OUTPUT,  const_cast<char*>("Act_Logistic"), 
                            const_cast<char*>("Out_Identity"), const_cast<char*>("out")) ;
  if (o_unit < 0) IF_ERROR_RETURN (o_unit) ;

  if (out_context) 
  {
    no_of_context_layers = no_of_hidden_layers + 1 ;
    h_unit[no_of_context_layers] = o_unit ;
  }
  else 
    no_of_context_layers = no_of_hidden_layers ;

  strcpy (name, "con") ;
  for (i = 1 ; i <= no_of_context_layers ; i++)
  {
    if (no_of_context_layers > 1) sprintf (name, "con%d", i) ;  
    c_unit[i] = createUnitLayer (layer[i], SPECIAL_H, const_cast<char*>("Act_Identity"), 
                                 const_cast<char*>("Out_Identity"), name ) ;
    if (c_unit[i] < 0) IF_ERROR_RETURN (c_unit[i]) ;
  }


  /* ---------------------------   create links  ----------------------------*/

  error_code = connectFull (i_unit, layer[0], h_unit[1], layer[1], 0.0) ;
  IF_ERROR_RETURN (error_code) ;

  for (i = 1 ; i < no_of_hidden_layers ; i++)
  {
    error_code = 
      connectFull (h_unit[i], layer[i], h_unit[i+1], layer[i+1], 0) ;
    IF_ERROR_RETURN (error_code) ;
  }

  error_code = connectFull (h_unit[i], layer[i], o_unit, layer[o_layer], 0) ;
  IF_ERROR_RETURN (error_code) ;
  
  for (i = 1 ; i <= no_of_context_layers ; i++)
  {
    error_code = connectFull (c_unit[i], layer[i],  h_unit[i], layer[i], 0) ;
    IF_ERROR_RETURN (error_code) ;

    error_code = connectOneToOne (h_unit[i], c_unit[i], layer[i], 0) ;
    IF_ERROR_RETURN (error_code) ;

    error_code = connectSelfRec (c_unit[i], layer[i], 0) ;
    IF_ERROR_RETURN (error_code) ;
  }
  

  /* --------------------------   set positions  ----------------------------*/

  cx = 1 ; /* position of the upper left */
  cy = 1 ; /* corner of a context layer  */

  for (i = 0 ; i < no_of_layers ; i++)
  {
    if ((int) (layer[i] / columns[i]) > cy - 1)
      cy = 1 + (int) (layer[i] / columns[i]) ;
  }

  cy += 1 ;

  dx  = 1 ; /* x position of the upper left corner of an input, */
            /* hidden or output layer                           */

  setPosRow (i_unit, layer[0], 1, 1, columns[0]) ;

  cx = dx + columns[0] + 2 ;
  dx = cx + columns[1] + 2 ;

  for (i = 1 ; i <= no_of_context_layers ; i++)
  {
    setPosRow (h_unit[i], layer[i],  dx, 1 , columns[i]) ;
    setPosRow (c_unit[i], layer[i],  cx, cy, columns[i]) ;

    cx = dx + columns[i]   + 2 ;
    dx = cx + columns[i+1] + 2 ;
  }

  if (! out_context)
    setPosRow (o_unit, layer[o_layer], dx, 1, columns[o_layer]) ;


  /* ----------------------   set default functions  ------------------------*/

  error_code = krui_setLearnFunc (const_cast<char*>("JE_BP")) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = krui_setInitialisationFunc (const_cast<char*>("JE_Weights")) ;
  IF_ERROR_RETURN (error_code) ;

  error_code = krui_setUpdateFunc (const_cast<char*>("JE_Order")) ;

  return (error_code) ;
}







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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_JordElm.ph,v $
  SHORTNAME      : bn_JordElm.ph
  SNNS VERSION   : 4.2

  PURPOSE        : private header file for corresponding '.c' file
  NOTES          :

  AUTHOR         : Tobias Soyez
  DATE           : 09.11.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:24 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifndef _BN_JORDANELMAN_DEFINED_
#define _BN_JORDANELMAN_DEFINED_


#define MAX_NO_OF_LAYERS        10
#define IF_ERROR_RETURN(error_code)  if (error_code != KRERR_NO_ERROR) \
                                     return (error_code)


 krui_err bn_jordan_createNet  (int IUnits, int HUnits, int OUnits,
                                      int ICols , int HCols , int OCols ) ;

 krui_err bn_elman_createNet        (int layer[], int columns[], int no_of_layers, bool out_context) ;


 int      createUnitLayer (int  no_of_units, 
                                 int  io_type,
                                 char *act_func_name,
                                 char *out_func_name,
                                 char *unit_name) ;


 krui_err connectFull     (int       from_unit,
                                 int       no_of_source_units,
                                 int       to_unit,
                                 int       no_of_target_units,
                                 FlintType weight) ;


 krui_err connectOneToOne (int       from_unit,
                                 int       to_unit,
                                 int       no_of_units,
                                 FlintType weight) ;


 krui_err connectSelfRec  (int       unit_no,
                                 int       no_of_units,
                                 FlintType weight) ;


 void setPosRow (int unit_no, int no_of_units, int x, int y, int r);

#endif



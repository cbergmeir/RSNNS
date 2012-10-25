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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/ext_typ.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS MasPar Kernel: Datatypes and Constants for Internal Usage
  NOTES          :

  AUTHOR         : Niels Mache 
  DATE           : 01.01.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:11 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef  KR_EXTERNAL_TYPES
#define  KR_EXTERNAL_TYPES

class SnnsCLib ;

typedef  void	(SnnsCLib::*FunctionPtr) (void *,...);
typedef  krui_err  (SnnsCLib::*NetFunctionPtr) (void *,...);


struct  UnitValues  {
  unsigned short  selector;
  int  lun, lln;
  FlintType  out, act, i_act, bias;
};

struct UnitDescriptor  {
  int  srcUnitLLN,
       destUnitLLN,
       srcUnitLUN,
       destUnitLUN;
};


struct NetFuncParameters  {
  int  start_pattern_no,
       end_pattern_no,
       cycles,
       
       no_of_input_parameters,
       no_of_output_parameters;

  float  input_parameters[NO_OF_LEARN_PARAMS],
         output_parameters[NO_OF_LEARN_PARAMS];

  FunctionPtr  function;              /*  pointer to the function  */
};





/*#################################################

GROUP: Type Definition of Function Table

#################################################*/

/*  Function table structure
*/
struct  FuncTable  {
  const char    *func_name;                 /*  name of the function  */
  unsigned short  func_type,          /*  type of the function  */
           no_of_input_parameters,    /*  no. of input parameters used by this function  */
                                      /*  (only for Update, Init and Learning functions)  */
           no_of_output_parameters;   /*  no. of ouput parameters returned by this function  */
                                      /*  (only for Update and Learning functions)  */
  FunctionPtr  function;              /*  pointer to the function  */
#ifdef PARAGON_KERNEL
  bool     parallelized;	      /*  learning function parallelized for */ 
				      /*  the Intel Paragon service process  */
#endif
};



#define  FUNCTION_NAME_MAX_LEN   100

struct FuncInfoDescriptor  {
  int      number;
  char     func_name[FUNCTION_NAME_MAX_LEN];   /*  name of the function  */
  unsigned short    
           func_type,                 /*  type of the function  */
           no_of_input_parameters,    /*  no. of input parameters used by this function  */
                                      /*  (only for Update, Init and Learning functions)  */
           no_of_output_parameters;   /*  no. of ouput parameters returned by this function  */
                                      /*  (only for Update and Learning functions)  */
  FunctionPtr  function;              /*  pointer to the function  */
#ifdef PARAGON_KERNEL
  bool     parallelized;              /*  learning function parallelized for */
				      /*  the Intel Paragon  */
#endif
};


/*#################################################

GROUP: Feedforward network descriptor

#################################################*/

#ifdef MASPAR_KERNEL

/* max. number of layers of a feedforward network in superscalar kernel mode  */
#define  FF_MAX_LAYERS  8

struct  FFnetDescriptor  {
  int  no_of_layers;   /*  no. of layers of the network */
  int  no_of_weights;	/*  no. of weights in network  */

  struct
    {  /*  describes the layers of the network
           (layers[i] describes the units in layer i, layer 0 is the input layer)  */
    int  no_of_units;          /*  no. of units in this layer  */

    struct
      {  /*  inputs[k] describes the input connections of the units in
             the layer i to units in layer k  */
      int  no_of_inputs;         /*  stores the total no. of connections
                                     from units in layer k with units in layer i.
                                 */
      FlintType  *weight_array;  /*  pointer to the weight array containing the
                                     connection weights of connections from
                                     layer i to layer k.
                                 */

      /*  if the network has partial connected units:  */
      /*  first input connection of unit i to unit <first_connection>
          in layer k
      */
      /*  gap count of the input connections of units in layer i with
          units in layer k
      */
    }  inputs[FF_MAX_LAYERS - 1];

  }  layers[FF_MAX_LAYERS];
};


struct  MasParInfo  {
  int  no_of_PEs,
       memory_perPE,
       no_of_network_copies,
       no_of_used_PEs;

  float  efficiency;
};

#endif

#endif





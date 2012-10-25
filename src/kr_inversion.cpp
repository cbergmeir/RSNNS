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


/**********************************************************************
  FILE           : %W%

  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : Implement Kindermann/Linden-inversion-method

  NOTES          : The functions implemented here closely resemble the
                   the functions propagateNetForward and propagateNetBackward2 
		   in the file learn_f.c.
  FUNCTIONS      : -- kr_initInversion
                      Purpose : initialize net for inversion algorithm
	               Calls   : int kr_topoCheck();
		                 int kr_IOCheck();
		            	  int kr_topoSort();
 
                   -- kr_inv_forwardPass
 	              Purpose : topological forward propagation
	              Calls   : nothing
 
	           -- kr_inv_backwardPass
                      Purpose : Backward error propagation (topological) 
	              Calls   : nothing
 
  AUTHOR         : Guenter Mamier
  DATE           : 04.02.92

  CHANGED BY     : Sven Doering, Michael Vogt
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/13 16:23:56 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

**********************************************************************/

#include <stdio.h>
#include <math.h>

#include "SnnsCLib.h"


/*****************************************************************************
  FUNCTION : kr_initInversion

  PURPOSE  : initialize net for inversion algorithm
  NOTES    :
  UPDATE   : 06.02.92
******************************************************************************/
int SnnsCLib::kr_initInversion(void)
{
    int ret_code = KRERR_NO_ERROR;

    if (NetModified || (TopoSortID != TOPOLOGICAL_FF &&
			TopoSortID != TOPOLOGIC_LOGICAL)){
       /*  Net has been modified or topologic array isn't initialized */
       /*  check the topology of the network  */
      ret_code = kr_topoCheck();
    if (ret_code < KRERR_NO_ERROR)  
       return( ret_code );  /*  an error has occured  */
    if (ret_code < 2)  
       return( KRERR_NET_DEPTH );  /*  the network has less then 2 layers  */


    /*  count the no. of I/O units and check the patterns  */
    ret_code = kr_IOCheck();
    if(ret_code < KRERR_NO_ERROR)  
       return( ret_code );

    /*  sort units by topology and by topologic type  */

    ret_code = kr_topoSort( TOPOLOGICAL_FF );
  }
  return(ret_code);
}



/*****************************************************************************
  FUNCTION : kr_inv_forwardPass

  PURPOSE  : topological forward propagation
  NOTES    :
  UPDATE   : 29.01.92
******************************************************************************/
void  SnnsCLib::kr_inv_forwardPass(struct UnitList *inputs)
{

   register struct Unit   *unit_ptr;
   register TopoPtrArray  topo_ptr;    /* points to a topological sorted    */
				       /* unit stucture (input units first) */
   struct UnitList        *IUnit;      /* working list of input units       */


   /* initialize the topological pointer */

   topo_ptr = topo_ptr_array;


   /*  calculate the activation and output value of the input units */ 

   IUnit = inputs;
   while((unit_ptr = *++topo_ptr) != NULL){

     /*  clear error values  */
     unit_ptr->Aux.flint_no = 0.0;

     if(unit_ptr->out_func == OUT_IDENTITY)
        unit_ptr->Out.output = unit_ptr->act = IUnit->act;
     else  /* no identity output function: calculate unit's output also  */
       unit_ptr->Out.output = (this->*unit_ptr->out_func)(unit_ptr->act = IUnit->act);
     IUnit = IUnit->next;
   }


   /*  popagate hidden units  */
 
   while((unit_ptr = *++topo_ptr) != NULL){

     /*  clear error values  */
     unit_ptr->Aux.flint_no = 0.0;

     /*  calculate the activation value of the unit: 
	 call the activation function if needed  */
     unit_ptr->act = (this->*unit_ptr->act_func) (unit_ptr);

     if(unit_ptr->out_func == OUT_IDENTITY)
       unit_ptr->Out.output = unit_ptr->act;
     else
       /*  no identity output function: calculate unit's output also  */
       unit_ptr->Out.output = (this->*unit_ptr->out_func) (unit_ptr->act);
   }
 
 
   /*  popagate output units  */
 
   while((unit_ptr = *++topo_ptr) != NULL){

     /*  clear error values  */
     unit_ptr->Aux.flint_no = 0.0;

     /*  calculate the activation value of the unit: 
	 call the activation function if needed  */
     unit_ptr->act = (this->*unit_ptr->act_func) (unit_ptr);

     if(unit_ptr->out_func == OUT_IDENTITY)
       unit_ptr->Out.output = unit_ptr->act;
     else      /*  no identity output function: calculate unit's output also  */
       unit_ptr->Out.output = (this->*unit_ptr->out_func) (unit_ptr->act);
   }
}



/*****************************************************************************
  FUNCTION : kr_inv_backwardPass

  PURPOSE  : Backward error propagation (topological) 
  NOTES    :
  UPDATE   : 04.02.92
*****************************************************************************/
double SnnsCLib::kr_inv_backwardPass(float learn, float delta_max, int *err_units, 
			   float ratio, struct UnitList *inputs, 
			   struct UnitList *outputs)
{
   register struct Link   *link_ptr;
   register struct Site   *site_ptr;
   register struct Unit   *unit_ptr;
   register float         error,  sum_error,  eta,  devit;
   register TopoPtrArray  topo_ptr;
   struct UnitList        *IUnit, *OUnit;


   sum_error = 0.0;    /*  reset network error  */
   *err_units = 0;     /*  reset error units */
   eta = learn;        /*  store learn_parameter in CPU register  */


   /* add 3 to no_of_topo_units because topologic array contains 4 NULL 
      pointers  */

   topo_ptr = topo_ptr_array + (no_of_topo_units + 3);


   /*  calculate output units only  */

   OUnit = outputs;
   while(OUnit->next != NULL)OUnit = OUnit->next;
   while((unit_ptr = *--topo_ptr) != NULL){


     /*  calc. devitation */
     devit = OUnit->i_act - unit_ptr->Out.output;
     OUnit->act = unit_ptr->Out.output;
     OUnit = OUnit->prev;
     if ( (devit > -delta_max) && (devit < delta_max) ){
       continue;
     }else{
       *err_units += 1;
     }

     /*  sum up the error of the network  */
     sum_error += devit * devit;  

     /*	calc. error for output units	 */
     error = devit * (this->*unit_ptr->act_deriv_func) ( unit_ptr );
     /*     error = devit;*/

     /* Calculate sum of errors of predecessor units  */
     if(UNIT_HAS_DIRECT_INPUTS( unit_ptr )){
       FOR_ALL_LINKS( unit_ptr, link_ptr )
         link_ptr->to->Aux.flint_no += link_ptr->weight * error;
     }else{        /*	the unit has sites  */
       FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
         link_ptr->to->Aux.flint_no += link_ptr->weight * error;
     }
   }


   /*  calculate hidden units only  */

   while((unit_ptr = *--topo_ptr) != NULL){

     /*	calc. the error of the (hidden) unit  */
     error = (this->*unit_ptr->act_deriv_func) ( unit_ptr ) * unit_ptr->Aux.flint_no;
     error = unit_ptr->Aux.flint_no;

     /* Calculate sum of errors of predecessor units  */
     if(UNIT_HAS_DIRECT_INPUTS( unit_ptr )){
       FOR_ALL_LINKS( unit_ptr, link_ptr )
	  link_ptr->to->Aux.flint_no += link_ptr->weight * error;
     }else{       /*  the unit has sites  */
       FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
  	  link_ptr->to->Aux.flint_no += link_ptr->weight * error;
     }
     unit_ptr->act = unit_ptr->i_act;
   }


   /*  calculate input units only  */

   IUnit = inputs;
   while(IUnit->next != NULL)IUnit = IUnit->next;
   while((unit_ptr = *--topo_ptr) != NULL){

     /*	calc. the error of the (input) unit  */
     error = (this->*unit_ptr->act_deriv_func) ( unit_ptr ) * unit_ptr->Aux.flint_no;
     error = unit_ptr->Aux.flint_no;

     /* Calculate the new activation for the input units */
     IUnit->im_act += eta * error + ratio*(IUnit->i_act - (float)unit_ptr->act);
     unit_ptr->act = 1.0 / (1.0 + exp((double)(-IUnit->im_act)));
     IUnit->act = unit_ptr->act;
     IUnit = IUnit->prev;
   }


   /*  return the error of the network */

   sum_error *= 0.5;
   return( sum_error ); 


}






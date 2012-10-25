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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_pat_parse.ph,v $
  SHORTNAME      : pat_parse
  SNNS VERSION   : 4.2

  PURPOSE        : parser for new pattern format; bison format 
  NOTES          : impossible to use with yacc

  AUTHOR         : Michael Vogt
  DATE           : 10.9.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:00 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _KR_PAT_PARSE_DEFINED_
#define _KR_PAT_PARSE_DEFINED_

/* begin global definition section */

#define CURRENT_VERSION_V       4  /* current version of pattern file        */
#define CURRENT_VERSION_R       2  /* format                                 */

int parse_pattern_file(int *set, np_pattern_set_info *this_set);


/* end global definition section */

/* begin private definition section */

 int	i;
 int	dims[MAX_NO_OF_VAR_DIM];
 int	max_i_dims[MAX_NO_OF_VAR_I_DIM];
 int	max_o_dims[MAX_NO_OF_VAR_O_DIM];
 int	actual_dim_count;
 int	variable_input_dim;
 int	variable_output_dim;
 int	current_pattern;
 int	next_pattern_is_input;
 int	no_of_input;
 int	no_of_output;
 int	no_of_pattern;
 int	act_size;
 float	*pat_mem;
 int      pattern_set;
 np_pattern_descriptor *pattern;
 int      with_classes;
 struct np_symtab *this_class;
 int      num_param;
 float    *parameter;

 np_pattern_set_info *this_set;

 void yyerror(char *error);

/* end private definition section */

#endif


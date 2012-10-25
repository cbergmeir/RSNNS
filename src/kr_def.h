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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_def.h,v $
  SHORTNAME      : def.h
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Defaults
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 01.03.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef KR_DEFAULTS

#define KR_DEFAULTS

#ifndef NULL
#include <stdio.h>	/*  include "stdio.h" if the NULL-pointer isn't
			    defined  */
#endif


/*  default constants for block memory allocation in # of components
*/
#ifdef __BORLANDC__

#define  UNIT_BLOCK     100
#define  LINK_BLOCK     500
#define  SITE_BLOCK     20
#define  NTABLE_BLOCK   50
#define  STABLE_BLOCK   20
#define  PATTERN_BLOCK  50

#else
#ifdef  __ENZO__

/* A population of networks doesn't allow to use this much memory ... */

#define  UNIT_BLOCK     300
#define  LINK_BLOCK     1000
#define  SITE_BLOCK     20
#define  NTABLE_BLOCK   50
#define  STABLE_BLOCK   20
#define  PATTERN_BLOCK  200

#else

#define  UNIT_BLOCK     1000
#define  LINK_BLOCK     5000
#define  SITE_BLOCK     200
#define  NTABLE_BLOCK   500
#define  STABLE_BLOCK   200
#define  PATTERN_BLOCK  200

#endif /*__ENZO__*/
#endif /*__BORLANDC__*/

/*  Unit/Site Default Initialisations
*/

#define     DEF_OUT         0
#define     DEF_I_ACT       0
#define     DEF_BIAS        0

#define     DEF_SUBNET_NO   0
#define     DEF_LAYER_NO    1
#define     DEF_POS_X       0
#define     DEF_POS_Y       0

#define     DEF_POS_Z       0


#define     DEF_STYPE       UFLAG_TTYP_HIDD


/*#################################################

GROUP: File I/O Constants

#################################################*/

/*  max. line length for the kernel input/output files
*/
#define  MAX_LIN_LEN    250
#define  BEST_LINE_LEN  132



#endif

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


#include <stdlib.h>

#include "u_lrand48.h"

#include <R_ext/Random.h>

long u_currentSeedVal = 0;

long u_lrand48(void)
{
    //using system RNG:
    //return (long) rand();

    //Using R's RNG
    GetRNGstate();
    long l = (long) (RAND_MAX * unif_rand());
    PutRNGstate();

    return l;


}

void u_srand48(long seedval)
{
    //using system RNG:
    //srand(seedval);
}

double u_drand48(void)
{
    //using system RNG:
    //RAND_MAX 0x7fffffff
    //return((double) (rand() & RAND_MAX) / (double) RAND_MAX);

    //Using R's RNG
    GetRNGstate();
    double d = unif_rand();
    PutRNGstate();
    return d;
}

void u_setCurrentSeedVal(long seedval)
{
  u_currentSeedVal = seedval;
}

long u_getCurrentSeedVal()
{
  return(u_currentSeedVal);
}


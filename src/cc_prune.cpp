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
  FILE           :  $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_prune.c,v $
  SHORTNAME      : cc_prune.c
  SNNS VERSION   : 4.2

  PURPOSE        : Routines of Pruned Cascade Correlation, 
  NOTES          : 

  AUTHOR         : Christian Wehrfritz
  DATE           : 

  CHANGED BY     : Juergen Gatter
# IDENTIFICATION : $State: Exp $ $Locker:  $
# RCS VERSION    : $Revision: 1.8 $
  LAST CHANGE    : 

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <memory.h>

#include "SnnsCLib.h"
#include "SnnsCLibGeneric_util.h"

/*****************************************************************************
  FUNCTION : float cc_calculatePruneError(int p,int n,float sse)

  PURPOSE  : return the pruning-errror related value

  UPDATE   : 17.11.95 <Juergen Gatter>
******************************************************************************/

float SnnsCLib::cc_calculatePruneError(int prune_func,int p,int n,float sse)
{
    switch (prune_func){
      case SBC:
	return n * log(sse/n) + p*log((double)n);
      case AIC:
	return n* log(sse/n) + p*2;
      case CMSEP:
	return sse/(n-2*p);
      default:
	return 0.0;
    }
}

/*****************************************************************************
  FUNCTION : float cc_getPruningError(int StartPattern,int EndPattern,bool before)

  PURPOSE  : return the pruning-errror related value
  NOTES    : mode==0 no printing, mode==1 before installing, mode==2 after.

  UPDATE   : 17.11.95 <Juergen Gatter>
******************************************************************************/

float SnnsCLib::cc_getPruningError(int pruneFunc,int StartPattern,int EndPattern,int mode)
{
  int p,n;
  float sse,GeTe;

  const char *cc_pruningFuncArray[] ={" SBC "," AIC ","CMSEP"};

  p=krui_countLinks();
  sse = cc_getErr (StartPattern, EndPattern);
  n=kr_np_pattern( PATTERN_GET_NUMBER, 0, 0 );
  GeTe = cc_calculatePruneError(pruneFunc,p,n,sse);

  if (mode>0){
    if (mode==1){
       SNNSprintf("Selection criterion is %s\n", cc_pruningFuncArray[pruneFunc]);
    } 
    SNNSprintf("%s %s inserting unit (p=%i): %f\n",
          cc_pruningFuncArray[pruneFunc],(mode==1)?"before":"after",
          p,GeTe);
  }
  return (GeTe);
}

/*****************************************************************************
  FUNCTION : cc_pruneNet

  PURPOSE  : remove nonessential links
  NOTES    :

  UPDATE   : 
******************************************************************************/
void SnnsCLib::cc_pruneNet (int StartPattern, int EndPattern, int pruneFunc)
{
  struct Unit *unit_ptr, *outputUnit_ptr;
  struct Link *link_ptr;
  int n,o, source=0, target=0,p;
  float tmp, sse, sbc=50000, sbc_ifKilled;
    
  /* compute no of params (p), sample size (n), error (sse) and 
   * the selection criterion 
   */
  p=krui_countLinks();
  sse = cc_getErr (StartPattern, EndPattern);
  n=kr_np_pattern( PATTERN_GET_NUMBER, 0, 0 );

  sbc=cc_getPruningError(pruneFunc,StartPattern,EndPattern,0);

  unit_ptr=kr_getUnitPtr(LastInsertedHiddenUnit);

  /* before attempting to remove weights, decrement p */
  p--;

  /* check for useless connections to output units */
  FOR_ALL_OUTPUT_UNITS(outputUnit_ptr, o) {
    FOR_ALL_LINKS(outputUnit_ptr,link_ptr) {
      if (link_ptr->to == unit_ptr) {
	tmp = link_ptr->weight;
	link_ptr->weight = 0.0;
	sse = cc_getErr (StartPattern, EndPattern);
	link_ptr->weight = tmp;
        sbc_ifKilled=cc_calculatePruneError(pruneFunc,p,n,sse);

	SNNSprintf("selection criterion if link %i-->%i gets killed: %f\n",
	       GET_UNIT_NO(link_ptr->to),GET_UNIT_NO(outputUnit_ptr),
	       sbc_ifKilled);
	if (sbc_ifKilled<sbc) {
	  sbc=sbc_ifKilled;
	  target= GET_UNIT_NO(outputUnit_ptr);
	  source= GET_UNIT_NO(link_ptr->to);
	}
      }
    }
  }
  /* check for useless connections to input/hidden units */
  FOR_ALL_LINKS (unit_ptr,link_ptr) {
    tmp = link_ptr->weight;
    link_ptr->weight = 0.0;
    sse = cc_getErr (StartPattern, EndPattern);
    link_ptr->weight = tmp;

    sbc_ifKilled=cc_calculatePruneError(pruneFunc,p,n,sse);

    SNNSprintf("selection criterion if link %i-->%i gets killed: %f\n",
	   GET_UNIT_NO(link_ptr->to),GET_UNIT_NO(unit_ptr),
	   sbc_ifKilled);
    if (sbc_ifKilled<sbc) {
      sbc=sbc_ifKilled;
      target= GET_UNIT_NO(unit_ptr);
      source= GET_UNIT_NO(link_ptr->to);
    }
  }
  if (source) {
    /* remove this links, which is least important; then try again */
    cc_killLink (source, target);
    ERROR_CHECK_WRC;
    cc_pruneNet(StartPattern, EndPattern, pruneFunc);
  }

  KernelErrorCode = 0;
}


/*****************************************************************************
  FUNCTION : cc_removeUnit

  PURPOSE  : Should remove Unit, does nothing. 

  NOTES    : Parameteruebergabe richten, falls Routine aktiviert wird.

  UPDATE   : 24.11.95 <ausgelagert by Juergen Gatter>
******************************************************************************/

void SnnsCLib::cc_remove_Unit(void)
{


      /* remove unit, train output connections and 
       * return with error to stop learning-process 
       */
      /*
	krui_deleteUnit(LastInsertedHiddenUnit);
	KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
	ERROR_CHECK;
	cc_xPS_trainNet(MAX_NO_ERROR_UPDATE_CYCLES, BACKFITT_PATIENCE, 
	MINIMAL_ERROR_CHANGE,
	OUT_PATIEN,StartPattern,EndPattern,PARAM1,
	PARAM2,PARAM3,ParameterOutArray,NoOfOutParams); 
	*/
      SNNSprintf("\nNOTE: selection criterion is increasing, the net is becoming too big!\n=====================================================================\n\n");
}


/*****************************************************************************
  FUNCTION : cc_killLink

  PURPOSE  : delete the link between source and target 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void SnnsCLib::cc_killLink (int source, int target)
{
  struct Unit *unit_ptr;

  /* set pointers and... */
  KernelErrorCode = kr_setCurrUnit(target);
  ERROR_CHECK_WRC;
  krui_isConnected(source);
  ERROR_CHECK_WRC;

  /* ...kill that beast */
  KernelErrorCode=krui_deleteLink();
  if(KernelErrorCode!=KRERR_NO_ERROR) { 
    SNNSprintf("\nLink not deleted%i",KernelErrorCode); 
  } else {
    SNNSprintf("link %i ----> %i killed\n",source,target);
  }
  KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
  if(KernelErrorCode==KRERR_DEAD_UNITS) { 
    SNNSprintf("\nlast link removed, killing Unit !"); 
    unit_ptr=kr_getUnitPtr(topo_msg.src_error_unit); 
    KernelErrorCode = kr_removeUnit(unit_ptr);
    if(KernelErrorCode!=KRERR_NO_ERROR) { 
      SNNSprintf("\nSNNS-kernel panic:%i cannot remove unit! aborting",
	     KernelErrorCode);
      return;
      //fflush(stdout);
      //exit (0);
    }
    KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
  }  
}


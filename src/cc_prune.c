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
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <memory.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#include "random.h"
#include "kr_typ.h"      /*  Kernel Types and Constants  */
#include "kr_const.h"    /*  Constant Declarators for SNNS-Kernel  */
#include "kr_def.h"      /*  Default Values  */
#include "kernel.h"      /*  kernel function prototypes  */
#include "kr_mac.h"      /*  Kernel Macros   */


#include "kr_ui.h"
#include "cc_type.h"
#include "cc_mac.h"
#include "cc_glob.h"
#include "kr_newpattern.h"
#include "cc_prune.ph"

/*****************************************************************************
  FUNCTION : float cc_calculatePruneError(int p,int n,float sse)

  PURPOSE  : return the pruning-errror related value

  UPDATE   : 17.11.95 <Juergen Gatter>
******************************************************************************/

float cc_calculatePruneError(int prune_func,int p,int n,float sse)
{
    switch (prune_func){
      case SBC:
	return n * log(sse/n) + p*log(n);
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

float cc_getPruningError(int pruneFunc,int StartPattern,int EndPattern,int mode)
{
  int p,n;
  float sse,GeTe;

  p=krui_countLinks();
  sse = cc_getErr (StartPattern, EndPattern);
  n=kr_np_pattern( PATTERN_GET_NUMBER, 0, 0 );
  GeTe = cc_calculatePruneError(pruneFunc,p,n,sse);

  if (mode>0){
    if (mode==1){
       printf("Selection criterion is %s\n", cc_pruningFuncArray[pruneFunc]);
    } 
    printf("%s %s inserting unit (p=%i): %f\n",
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
void cc_pruneNet (int StartPattern, int EndPattern, int pruneFunc)
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

	printf("selection criterion if link %i-->%i gets killed: %f\n",
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

    printf("selection criterion if link %i-->%i gets killed: %f\n",
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
}


/*****************************************************************************
  FUNCTION : cc_removeUnit

  PURPOSE  : Should remove Unit, does nothing. 

  NOTES    : Parameteruebergabe richten, falls Routine aktiviert wird.

  UPDATE   : 24.11.95 <ausgelagert by Juergen Gatter>
******************************************************************************/

void cc_remove_Unit(void)
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
      printf("\nNOTE: selection criterion is increasing, the net is becoming too big!\n=====================================================================\n\n");
}


/*****************************************************************************
  FUNCTION : cc_killLink

  PURPOSE  : delete the link between source and target 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void cc_killLink (int source, int target)
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
    printf("\nLink not deleted%i",KernelErrorCode); 
  } else {
    printf("link %i ----> %i killed\n",source,target);
  }
  KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
  if(KernelErrorCode==KRERR_DEAD_UNITS) { 
    printf("\nlast link removed, killing Unit !"); 
    unit_ptr=kr_getUnitPtr(topo_msg.src_error_unit); 
    KernelErrorCode = kr_removeUnit(unit_ptr);
    if(KernelErrorCode!=KRERR_NO_ERROR) { 
      printf("\nSNNS-kernel panic:%i cannot remove unit! aborting",
	     KernelErrorCode);
      fflush(stdout);
      exit (0);
    }
    KernelErrorCode = kr_topoSort(TOPOLOGICAL_CC);
  }  
}


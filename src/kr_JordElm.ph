/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_JordElm.ph,v $
  SHORTNAME      : kr_JordElm.ph
  SNNS VERSION   : 4.2

  PURPOSE        : private header file for corresponding '.c' file
  NOTES          :

  AUTHOR         : Tobias Soyez
  DATE           : 09.11.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:26 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/


#ifndef _KR_JORDANELMAN_DEFINED_
#define _KR_JORDANELMAN_DEFINED_


krui_err kr_topoCheckJE (void) ;
krui_err kr_topoSortJE  (void) ;


static void  kr_recTopoCheckJE (struct Unit *unit_ptr, int depth) ;
static void  kr_recTopoSortJE  (struct Unit *unit_ptr, int depth) ;

static TopoPtrArray  topo_ptr ;


#endif


/*****************************************************************************
                        E N D     O F     F I L E
******************************************************************************/










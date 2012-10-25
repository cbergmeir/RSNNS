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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/enzo_mem_typ.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2
 
  PURPOSE        : declaration of all Enzo related variables
  NOTES          :
 
  AUTHOR         : Thomas Ragg
  DATE           : 12.11.95
 
  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.3 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:10 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#ifndef _ENZO_MEM_TYP_H
#define _ENZO_MEM_TYP_H

typedef struct
{
    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*   These are all the variables you need to describe the internal        */
    /*   state of an SNNS-net in memory.                                      */
    /*                                                                        */
    /*   Note that this is HIGHLY dependend on the version of SNNS!           */
    /*   Manipulate the net only via the krui-functions!                      */
    /*   Do NOT use this structure for anything but                           */
    /*   krm_get_net() and krm_put_net() ...                                  */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */
    
    int       NoOfLinks;
    int       NoOfAllocLinks;
    LinkArray link_array;
    LinkArray link_block_list;
    LinkArray free_link_ptr;
    
    int       NoOfSites;
    int       NoOfNetSites;
    int       NoOfAllocSites;
    SiteArray site_array;
    SiteArray site_block_list;
    SiteArray free_site_ptr;

    int       NoOfAllocUnits;
    int       FreeUnitIndex;
    int       NoOfUnits;
    int       NoOfInputUnits;
    int       NoOfOutputUnits;
    int       NoOfHiddenUnits;
    int       MaxUnitNo;
    int       MinUnitNo;
    UnitArray unit_array;

    TopoPtrArray topo_ptr_array;
    
    int         NoOfNTableEntries;
    int         NoOfAllocNTableEntries;
    NTableArray NTable_array;
    NTableArray NTable_block_list;
    NTableArray free_NTable_entry;
    NTableArray curr_NTable_entry;
    NTableArray curr_NTable_block;
    
    int         NoOfSTableEntries;
    int         NoOfAllocSTableEntries;
    STableArray STable_array;
    STableArray STable_block_list;
    STableArray free_STable_entry;
    STableArray curr_STable_entry;
    STableArray curr_STable_block;
    
    
    struct FtypeUnitStruct  *Ftype_list_root;
    struct FtypeUnitStruct  *curr_Ftype_entry;
    int                      NoOfFTableEntries;

    char update_func[FUNCTION_NAME_MAX_LEN],
         learn_func [FUNCTION_NAME_MAX_LEN],
         init_func  [FUNCTION_NAME_MAX_LEN];
    
    FlintType u_act, u_bias;
    int u_ttflags, u_subnet_no, u_layer_no;
    char u_act_func[FUNCTION_NAME_MAX_LEN],
         u_out_func[FUNCTION_NAME_MAX_LEN];

    bool NetModified, NetInitialize, LearnFuncHasChanged;
    struct Unit *unitPtr;
    struct Site *sitePtr, *prevSitePtr;
    struct Link *linkPtr, *prevLinkPtr;
    int unitNo;
    int specialNetworkType;

    int TopoSortID;
    int no_of_topo_units;
    /* global_topo_ptr seems to be unproblematic; its only used inside
       the topol. sorting functions */
    
} memNet;


typedef struct
{
    int      number;
} memPat;

#endif

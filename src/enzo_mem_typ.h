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

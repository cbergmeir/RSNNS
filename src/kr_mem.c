/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_mem.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2
 
  PURPOSE        : SNNS-Kernel Memory Manager 
  NOTES          :
 
  AUTHOR         : Niels Mache
  DATE           : 21.2.90
 
  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.17 $
  LAST CHANGE    : $Date: 1998/05/15 13:12:08 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#ifdef HAVE_VALUES_H
#include <values.h>
#else
#include <math.h>
#endif
#include <limits.h>

#include "kr_typ.h"	 /*  Kernel Types and Constants  */
#include "kernel.h"
#include "kr_const.h"	 /*  Constant Declarators for SNNS-Kernel  */
#include "kr_def.h"	 /*  Default Values  */

#include "kr_mem.ph"	 /*  Function prototypes  */
#include "kr_mac.h"	 /*  Kernel Macros  */


#ifndef MAXSHORT
#define MAXSHORT SHRT_MAX
#endif

/*  ULTRIX-32 Operating System	*/
#if defined (ultrix) || defined (__BORLANDC__) 
#include "strdup.h"	/*  include strdup function because strdup is
			    missing in ULTRIX-32  */
#endif


/*****************************************************************************
  FUNCTION : krm_allocLinks

  PURPOSE  : allocate another link array (with size N entries)
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static  int     krm_allocLinks(int N)
{
  LinkArray     tmp_ptr;


  tmp_ptr = (LinkArray) calloc((unsigned int)( N + 1), LINK_SIZE );
  if (tmp_ptr == NULL)  return( 1 );

  if (link_array == NULL)
    {
    tmp_ptr->next = NULL;           /*  free link/block sentinel
                                    */
    free_link_ptr = tmp_ptr;
    }
  else
    {
    tmp_ptr->next = link_block_list;
    }

  link_block_list = tmp_ptr;    /* free link block sentinel */
  NoOfAllocLinks += N;
  link_array = tmp_ptr;         /* link_array points to the first link entry */
  return( 0 );
}


/*****************************************************************************
  FUNCTION : krm_getLink

  PURPOSE  : get one link structure
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
struct Link  *krm_getLink(void)
{
  struct Link   *tmp_ptr;


  if ((NoOfLinks == NoOfAllocLinks) || (link_array == NULL))
    if (krm_allocLinks( LINK_BLOCK ) != 0)
      {  /*  memory allocation failed  */
      KernelErrorCode = KRERR_INSUFFICIENT_MEM;
      return( NULL );
    }

  NoOfLinks++;

  if (free_link_ptr->next != NULL)
    {
    tmp_ptr = free_link_ptr;
    free_link_ptr = free_link_ptr->next;
  }
  else
    {
    tmp_ptr = ++link_array;
  }

  return( tmp_ptr );
}


/*****************************************************************************
  FUNCTION : krm_releaseLink

  PURPOSE  : release one link structure
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_releaseLink(struct Link *link_ptr)
{
  --NoOfLinks;

  link_ptr->next = free_link_ptr;
  free_link_ptr  = link_ptr;
}


/*****************************************************************************
  FUNCTION : krm_releaseAllLinks

  PURPOSE  : release the link and all following links
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_releaseAllLinks(struct Link *first_link_ptr)
{
  struct Link  *curr,
               *next,
               *free;


  free = free_link_ptr;
  curr = first_link_ptr;

  while (curr != NULL)  {
    --NoOfLinks;

    next = curr->next;
    curr->next = free;

    free = curr;
    curr = next;
  }

  free_link_ptr = free;
}


/*****************************************************************************
  FUNCTION : krm_releaseLinkArrays

  PURPOSE  : free all link array
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void  krm_releaseLinkArrays(void)
{
  register struct Link  *tmp_ptr1, *tmp_ptr2;


  NoOfLinks = NoOfAllocLinks = 0;

  if (link_array != NULL)  {
    tmp_ptr2 = link_block_list;
    while (tmp_ptr2 != NULL)  {
      tmp_ptr1 = tmp_ptr2->next;
      free( (char *) tmp_ptr2 );
      tmp_ptr2 = tmp_ptr1;
    }

    free_link_ptr = link_array = NULL;
  }
}



/*#################################################

GROUP: Site Functions

#################################################*/
/*****************************************************************************
  FUNCTION : krm_allocSites

  PURPOSE  : allocate another site array (with size N entries)
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static  int     krm_allocSites(int N)
{
  SiteArray     tmp_ptr;


  tmp_ptr = (SiteArray) calloc((unsigned int)(N + 1), SITE_SIZE );
  if (tmp_ptr == NULL)  return( 1 );

  if (site_array == NULL)  {
    tmp_ptr->next = NULL;           /*  free site/block sentinel   */
    free_site_ptr = tmp_ptr;
  }
  else  {
    tmp_ptr->next = site_block_list;
  }

  site_block_list = tmp_ptr;    /*  free site block sentinel  */
  NoOfAllocSites += N;
  site_array = tmp_ptr;         /*  site_array points to the sentinel */
  return( 0 );
}


/*****************************************************************************
  FUNCTION : krm_getSite

  PURPOSE  : get one unit-site structure
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
struct Site    *krm_getSite(void)
{
  struct Site   *tmp_ptr;


  if ((site_array == NULL) || (NoOfSites == NoOfAllocSites))
    if (krm_allocSites( SITE_BLOCK ) != 0)  {
      KernelErrorCode = KRERR_INSUFFICIENT_MEM;
      return( NULL );
    }

  NoOfSites++;
  NoOfNetSites++;

  if (free_site_ptr->next != NULL)  {
    tmp_ptr = free_site_ptr;
    free_site_ptr = free_site_ptr->next;
  }
  else  {
    tmp_ptr = ++site_array;
  }

  return( tmp_ptr );
}


/*****************************************************************************
  FUNCTION : krm_getFtypeSite

  PURPOSE  : get one site structure for functionality use only
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static struct Site  *krm_getFtypeSite(void)
{
  struct Site  *tmp_ptr;

  KernelErrorCode = KRERR_NO_ERROR;

  if ((site_array == NULL) || (NoOfSites == NoOfAllocSites))
    if (krm_allocSites( SITE_BLOCK ) != 0)  {
      KernelErrorCode = KRERR_INSUFFICIENT_MEM;
      return( NULL );
    }

  NoOfSites++;

  if (free_site_ptr->next != NULL)  {
    tmp_ptr = free_site_ptr;
    free_site_ptr = free_site_ptr->next;
  }
  else  {
    tmp_ptr = ++site_array;
  }

  return( tmp_ptr );
}


/*****************************************************************************
  FUNCTION : krm_releaseSite

  PURPOSE  : release one unit-site structure
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_releaseSite(struct Site *site_ptr)
{
  --NoOfSites;
  --NoOfNetSites;

  site_ptr->next = free_site_ptr;
  free_site_ptr  = site_ptr;
}


/*****************************************************************************
  FUNCTION :krm_releaseFtypeSite

  PURPOSE  : 
  NOTES    : Future Use: 
             release one Ftype-site structure

  RETURNS  :
  UPDATE   : 
******************************************************************************/
/*
static void    krm_releaseFtypeSite(struct Site *site_ptr )
{
  --NoOfSites;

  site_ptr->next = free_site_ptr;
  free_site_ptr  = site_ptr;
}
*/

/*****************************************************************************
  FUNCTION : krm_releaseAllSites

  PURPOSE  : release the unit-site and all following sites (at this unit)
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_releaseAllSites(struct Site *first_site_ptr)
{
  struct Site  *curr,
               *next,
               *free;


  free = free_site_ptr;
  curr = first_site_ptr;

  while (curr != NULL)  {
    --NoOfSites;
    --NoOfNetSites;

    next = curr->next;
    curr->next = free;

    free = curr;
    curr = next;
  }

  free_site_ptr = free;
}


/*****************************************************************************
  FUNCTION : krm_releaseAllFtypeSites

  PURPOSE  : release the Ftype-site and all following sites
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static void    krm_releaseAllFtypeSites(struct Site *first_site_ptr)
{
  struct Site  *curr,
               *next,
               *free;


  free = free_site_ptr;
  curr = first_site_ptr;

  while (curr != NULL)  {
    --NoOfSites;

    next = curr->next;
    curr->next = free;

    free = curr;
    curr = next;
  }

  free_site_ptr = free;
}

/*****************************************************************************
  FUNCTION :  krm_releaseSiteArrays

  PURPOSE  : free all site arrays
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static  void    krm_releaseSiteArrays(void)
{
  struct Site      *tmp_ptr;


  NoOfSites      = 0;
  NoOfNetSites   = 0;
  NoOfAllocSites = 0;

  if (site_array != NULL)  {
    while (site_block_list != NULL)  {
      tmp_ptr =  site_block_list->next;
      free( (char *) site_block_list );
      site_block_list = tmp_ptr;
    }

    free_site_ptr = NULL;
    site_array    = NULL;
  }
}



/*#################################################

GROUP: Unit Functions

#################################################*/
/*****************************************************************************
  FUNCTION : krm_unitArrayGC

  PURPOSE  : garbage collection of unit array 
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void  krm_unitArrayGC(void)
{
  register struct Link   *link_ptr;
  register struct Site   *site_ptr;
  register struct Unit   *unit_ptr;
  register struct Unit   *new_unit_ptr;
  struct Unit   *dest_unit_ptr;


  /*  find first unused unit stucture  */
  dest_unit_ptr = NULL;
  FOR_ALL_UNITS( unit_ptr )
    if (!UNIT_IN_USE( unit_ptr ))
      {  /*  unit isn't in use  */
      dest_unit_ptr = unit_ptr;  /*  store the first unused unit stucture  */
      break;
    }

  if (dest_unit_ptr != NULL)  
    {  /*  do garbage collection  */
    NetModified = TRUE;

    /*  store continous unit pointers in each unit struct  */
    new_unit_ptr = unit_array;
    FOR_ALL_UNITS( unit_ptr )
      if UNIT_IN_USE( unit_ptr )
        /*  unit is in use  */
        unit_ptr->Aux.ptr = (char *) ++new_unit_ptr;

    /*  adjust the link pointers  */
    FOR_ALL_UNITS( unit_ptr )
      if UNIT_IN_USE( unit_ptr )  {
	if UNIT_HAS_SITES( unit_ptr )
	  FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
              link_ptr->to = (struct Unit *) link_ptr->to->Aux.ptr;
        else
	  if UNIT_HAS_DIRECT_INPUTS( unit_ptr )
	    FOR_ALL_LINKS( unit_ptr, link_ptr )
              link_ptr->to = (struct Unit *) link_ptr->to->Aux.ptr;
      }


    /*  compress unit array  */
    for (unit_ptr = dest_unit_ptr+1; unit_ptr<=unit_array+MaxUnitNo; unit_ptr++)
      if UNIT_IN_USE( unit_ptr )
        memcpy( (char *) dest_unit_ptr++, (char *) unit_ptr, UNIT_SIZE );

    MinUnitNo = 1;
    MaxUnitNo = NoOfUnits;
    FreeUnitIndex = 0;
  }

  /*  reduce size of unit array, if needed  */
  if ((NoOfAllocUnits - NoOfUnits) >= (2 * UNIT_BLOCK))  {
    unit_ptr = (UnitArray) realloc( (char *) unit_array, (unsigned)
                                   ((NoOfAllocUnits + 1 - UNIT_BLOCK) * 
				    UNIT_SIZE) );
    if (unit_ptr != NULL)  {
      unit_array = unit_ptr;
      NoOfAllocUnits -= UNIT_BLOCK;
    }
  }
}


/*****************************************************************************
  FUNCTION : krm_relocateLinkPtrs

  PURPOSE  : relocate the link pointers. (If the address of the unit array was modified
             because the memory was reallocated, the link pointers must be relocated)

  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static  void    krm_relocateLinkPtrs(int offset)
{
  register struct Link   *link_ptr;
  register struct Site   *site_ptr;
  register struct Unit   *unit_ptr;


  FOR_ALL_UNITS( unit_ptr )
    if UNIT_IN_USE( unit_ptr )  {
      if UNIT_HAS_SITES( unit_ptr )
	FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr )
	  link_ptr->to = (struct Unit *) ((char *) link_ptr->to + offset);
      else
	if UNIT_HAS_DIRECT_INPUTS( unit_ptr )
	  FOR_ALL_LINKS( unit_ptr, link_ptr )
            link_ptr->to = (struct Unit *) ((char *) link_ptr->to + offset);
    }
}


/*****************************************************************************
  FUNCTION : krm_allocUnits

  PURPOSE  : allocate the unit array
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
krui_err  krm_allocUnits(int N)
{
  UnitArray  tmp_ptr;
  int  offset;

  if ((NoOfAllocUnits - NoOfUnits) < N)
    {  /*  alloc units	*/
    N = (N / UNIT_BLOCK + 1) * UNIT_BLOCK;
  }

  if (unit_array == NULL)  {
    tmp_ptr = (UnitArray) calloc((unsigned int)( NoOfAllocUnits + N + 1 ), 
				 UNIT_SIZE );
    if (tmp_ptr == NULL)
      {  /*  mem alloc failed	 */
      KernelErrorCode = KRERR_INSUFFICIENT_MEM;
      return( KernelErrorCode );
    }
    FreeUnitIndex = 0;
    tmp_ptr[0].Out.nextFreeUnit = 0;   /*  sentinel of free unit list
                                       */
  }
  else  {
    tmp_ptr = (UnitArray) realloc( (char *) unit_array, (unsigned)
                                   ((NoOfAllocUnits + N + 1 ) * UNIT_SIZE) );
    if (tmp_ptr == NULL)
      {  /*  mem alloc failed	 */
      KernelErrorCode = KRERR_INSUFFICIENT_MEM;
      return( KernelErrorCode );
    }
    offset = (char *) tmp_ptr - (char *) unit_array;
    if (offset != 0)  krm_relocateLinkPtrs( offset );
  }

  NoOfAllocUnits += N;
  unit_array = tmp_ptr;

  KernelErrorCode = KRERR_NO_ERROR;
  return( KernelErrorCode );
}

/*****************************************************************************
  FUNCTION : krm_getUnit

  PURPOSE  : get one unit structure
  NOTES    :

  RETURNS  : the number of the current unit structure. This number is negative 
             if a new block had to be allocated, to signal the calling program 
             that it has to update its unit pointers.
  UPDATE   : 
******************************************************************************/
int  krm_getUnit(void)
{
  register int   unit_no;
  int ret_fact = 1;

  KernelErrorCode = KRERR_NO_ERROR;
  if ((unit_array == NULL) || (NoOfUnits == NoOfAllocUnits)){
    if (krm_allocUnits( UNIT_BLOCK ) != 0)
      {  /*  Insufficient memory  */
      KernelErrorCode = KRERR_INSUFFICIENT_MEM;
      return( 0 );
    }
    ret_fact = -1;
  }
  NoOfUnits++;

  if (FreeUnitIndex != 0)
    {  /*  reuse unit  */
    unit_no = FreeUnitIndex;
    FreeUnitIndex = unit_array[unit_no].Out.nextFreeUnit;
  }
  else
    unit_no = NoOfUnits;

  /*  this unit is ready to use  */
  unit_array[unit_no].flags = UFLAG_IN_USE;
  unit_array[unit_no].sites = NULL;

  if (NoOfUnits == 1)
    MinUnitNo = MaxUnitNo = unit_no;
  else  {
    /*	store the highest allocated unit number  */
    if (unit_no > MaxUnitNo)  MaxUnitNo = unit_no;
    /*	store the lowest allocated unit number	*/
    if (unit_no < MinUnitNo)  MinUnitNo = unit_no;
  }

  return( unit_no * ret_fact);
}


/*****************************************************************************
  FUNCTION : krm_releaseUnit

  PURPOSE  : release unit

  NOTES    : NEW VERSION in SNNS V3.0
             Now the garbage collection is performed on every deletion of 
             a unit.

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_releaseUnit(int UnitNo)
{
  if (unit_array[UnitNo].flags != UFLAG_FREE) {  /*  don't release twice
                                                 */
    --NoOfUnits;

    unit_array[UnitNo].flags = UFLAG_FREE;

  }
}


/*****************************************************************************
  FUNCTION : krm_releaseUnitArrays

  PURPOSE  : free the unit array
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static	void	krm_releaseUnitArrays(void)
{
  NoOfAllocUnits = FreeUnitIndex =
  NoOfUnits = NoOfInputUnits = NoOfOutputUnits =
  NoOfHiddenUnits = MaxUnitNo = 0;

  if (unit_array != NULL)  {
    free( (char *) unit_array );
    unit_array = NULL;
  }
}



/*#################################################

GROUP: General Purpose Functions

#################################################*/
/*****************************************************************************
  FUNCTION : krm_getMemoryManagerInfo

  PURPOSE  : get information about memory usage
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_getMemoryManagerInfo(int *array_size, int info_array[])
{
  info_array[ 0 ] = NoOfNetSites;

#ifdef MASPAR_KERNEL
  if (specialNetworkType == NET_TYPE_FF1)
    info_array[ 1 ] = descrFFnet.no_of_weights;
  else
    info_array[ 1 ] = NoOfLinks;
#else
  info_array[ 1 ] = NoOfLinks;
#endif

  info_array[ 2 ] = NoOfSTableEntries;
  info_array[ 3 ] = NoOfFTableEntries;

  info_array[ 4 ] = NoOfAllocUnits;
  info_array[ 5 ] = NoOfAllocSites;
  info_array[ 6 ] = NoOfAllocLinks;
  info_array[ 7 ] = NoOfAllocNTableEntries;
  info_array[ 8 ] = NoOfAllocSTableEntries;
  info_array[ 9 ] = NoOfFTableEntries;

  *array_size = 10;
}


/*****************************************************************************
  FUNCTION : krm_allocUnitTopoArray

  PURPOSE  : allocate the array for topological sorting of the units in the 
             network
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
krui_err  krm_allocUnitTopoArray(int N)
{
  KernelErrorCode = KRERR_NO_ERROR;  /*  reset return code  */

  if (topo_ptr_array != NULL)
    {  /*  reallocate array for topologic sorting  */
    topo_ptr_array = (TopoPtrArray) realloc( (char *) topo_ptr_array,
					     (unsigned) (N * TOPO_PTR_SIZE) );
  }
  else
    {  /*  allocate new array for topologic sorting  */
    topo_ptr_array = (TopoPtrArray) calloc((unsigned int) N, TOPO_PTR_SIZE);
  }

  if (topo_ptr_array == NULL)  KernelErrorCode = KRERR_INSUFFICIENT_MEM;

  return( KernelErrorCode );
}


/*****************************************************************************
  FUNCTION : krm_releaseUnitTopoArray

  PURPOSE  : release the topolocic array
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void  krm_releaseUnitTopoArray(void)
{
  if (topo_ptr_array != NULL)  {
    free( (char *) topo_ptr_array );
    topo_ptr_array = NULL;
  }
}



/*#################################################

GROUP: NameTable Functions

#################################################*/
/*****************************************************************************
  FUNCTION : krm_allocNTableArray

  PURPOSE  : allocate one name-table block
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static  int     krm_allocNTableArray(void)
{
  NTableArray     tmp_ptr;


  tmp_ptr = (NTableArray) calloc( NTABLE_BLOCK + 1, NTABLE_SIZE );
  if (tmp_ptr == NULL)  return( 1 );

  if (NTable_array == NULL)  {
    tmp_ptr->Entry.next = NULL;     /*  free name-table block sentinel  */
    free_NTable_entry = tmp_ptr;    /*  free name-table entry sentinel  */
  }
  else  {
    tmp_ptr->Entry.next = NTable_block_list;    /*  append new name-table block
                                                    to block list   */
  }

  NTable_block_list = tmp_ptr;      /* update block list ptr */
  NoOfAllocNTableEntries += NTABLE_BLOCK;
  NTable_array = tmp_ptr + 1;       /* NTable_array points to the first entry */
  return( 0 );
}


/*****************************************************************************
  FUNCTION : krm_getNTableEntry

  PURPOSE  : get one name-table entry
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static struct NameTable  *krm_getNTableEntry(void)
{
  struct NameTable  *tmp_ptr;


  if ((NTable_array == NULL) || (NoOfNTableEntries == NoOfAllocNTableEntries))
    if (krm_allocNTableArray() != 0)  {
      KernelErrorCode = KRERR_INSUFFICIENT_MEM;
      return( NULL );
    }

  NoOfNTableEntries++;

  if (free_NTable_entry->Entry.next != NULL)
    {   /*  a previous released name-table entry is availabe    */
    tmp_ptr = free_NTable_entry;
    free_NTable_entry = free_NTable_entry->Entry.next;
  }
  else  {
    tmp_ptr = NTable_array++;
  }

  KernelErrorCode = KRERR_NO_ERROR;
  tmp_ptr->ref_count = 1;
  return( tmp_ptr );
}


/*****************************************************************************
  FUNCTION : krm_NTableReleaseEntry

  PURPOSE  : release one name-table entry
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_NTableReleaseEntry(struct NameTable *NTable_ptr)
{
  --NoOfNTableEntries;

  free( NTable_ptr->Entry.symbol );
  NTable_ptr->sym_type = UNUSED_SYM;
  NTable_ptr->Entry.next = free_NTable_entry;
  free_NTable_entry = NTable_ptr;
}


/*****************************************************************************
  FUNCTION : krm_releaseNTableArrays

  PURPOSE  : release all name-table blocks
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static void  krm_releaseNTableArrays(void)
{
  struct NameTable     *tmp_ptr;


  NoOfNTableEntries      = 0;
  NoOfAllocNTableEntries = 0;

  if (NTable_array != NULL)  {
    for (tmp_ptr = NTable_array - 1; tmp_ptr > NTable_block_list; --tmp_ptr)  {
      if (tmp_ptr->sym_type != UNUSED_SYM)
        free( tmp_ptr->Entry.symbol );    /*  free symbols */
    }

    tmp_ptr = NTable_block_list->Entry.next;
    free( (char *) NTable_block_list );
    NTable_block_list = tmp_ptr;

    while (NTable_block_list != NULL)  {
      for (tmp_ptr = NTable_block_list + NTABLE_BLOCK;
           tmp_ptr > NTable_block_list; --tmp_ptr)  {
        if (tmp_ptr->sym_type != UNUSED_SYM)
          free( tmp_ptr->Entry.symbol );   /*  free symbols */
      }

      tmp_ptr =  NTable_block_list->Entry.next;
      free( (char *) NTable_block_list );
      NTable_block_list = tmp_ptr;
    }

    free_NTable_entry = NULL;
    NTable_array      = NULL;
  }
}


/*****************************************************************************
  FUNCTION : krm_NTableSymbolSearch

  PURPOSE  : Searches for a given symbol and symbol-type in the name table.
  NOTES    :

  RETURNS  : Returns symbol ptr if symbol was found, NULL otherwise.
  UPDATE   : 
******************************************************************************/
struct  NameTable   *krm_NTableSymbolSearch(char *symbol, int sym_type)
{
  int   symbol_type;
  struct  NameTable   *n_ptr,
                      *block_list;


  if (NTable_array == NULL)  return( NULL );

  block_list = NTable_block_list;
  for (n_ptr = NTable_array - 1; n_ptr > block_list; n_ptr--)  {
    symbol_type = (int) n_ptr->sym_type;
    if ( (symbol_type != UNUSED_SYM) &&
         (symbol_type == sym_type) &&
         (strcmp( n_ptr->Entry.symbol, symbol ) == 0) )
      return( n_ptr );    /*  symbol was found    */
  }

  for (block_list = block_list->Entry.next;
       block_list != NULL;
       block_list = block_list->Entry.next)  {
    for (n_ptr = block_list + NTABLE_BLOCK; n_ptr > block_list; n_ptr--)  {
      symbol_type = (int) n_ptr->sym_type;
      if ( (symbol_type != UNUSED_SYM) &&
           (symbol_type == sym_type) &&
           (strcmp( n_ptr->Entry.symbol, symbol ) == 0) )
        return( n_ptr );    /*  symbol was found    */
    }
  }

  return( NULL );
}


/*****************************************************************************
  FUNCTION : krm_NTableCreateEntry

  PURPOSE  : Creates a new symbol in the name-table
  NOTES    :

  RETURNS  : Returns name-table ptr or NULL if memory alloc has failed.
  UPDATE   : 
******************************************************************************/
struct NameTable  *krm_NTableCreateEntry(char *symbol_name, int symbol_type)
{
  char   *str_ptr;
  struct  NameTable     *n_ptr;


  KernelErrorCode = KRERR_NO_ERROR;

  if ( (n_ptr = krm_getNTableEntry() ) == NULL)
    return( NULL );     /*  memory alloc failed */

  if ((str_ptr = (char *) strdup( symbol_name ) ) == NULL)
    {  /*  memory alloc failed */
    KernelErrorCode = KRERR_INSUFFICIENT_MEM;
    return( NULL );
  }

  n_ptr->Entry.symbol = str_ptr;
  n_ptr->sym_type = (unsigned short) symbol_type;

  return( n_ptr );
}


/*****************************************************************************
  FUNCTION : krm_NTableInsertSymbol

  PURPOSE  : Inserts a symbol in the name-table. This function duplicates 
             symbol ptrs if the symbol was found in the name-table.
  NOTES    :

  RETURNS  : Returns symbol ptr or NULL if memory alloc has failed
  UPDATE   : 
******************************************************************************/
char  *krm_NTableInsertSymbol(char *symbol_name, int symbol_type)
{
  struct NameTable  *n_ptr;


  if ( (n_ptr = krm_NTableSymbolSearch( symbol_name, symbol_type ) ) != NULL)
    {   /*  symbol is already in the name table  */
    if ((n_ptr->ref_count) < ((unsigned short) MAXSHORT)) {
        n_ptr->ref_count++;
    }
    return( n_ptr->Entry.symbol );
  }

  n_ptr = krm_NTableCreateEntry( symbol_name, symbol_type );
  return( n_ptr->Entry.symbol );
}


/*****************************************************************************
  FUNCTION : krm_NTableReleaseSymbol

  PURPOSE  : release name-table entry if there is no other reference to this 
             symbol
  NOTES    :

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
void  krm_NTableReleaseSymbol(char *symbol_name, int symbol_type)
{
  struct NameTable  *n_ptr;


  if (symbol_name == NULL)  return;
  if ( (n_ptr = krm_NTableSymbolSearch( symbol_name, symbol_type ) ) != NULL)
    {   /*  symbol is in the name table  */
    if ((n_ptr->ref_count) < ((unsigned short) MAXSHORT))
      {   /*    No. of references to this symbol don't exceed the max. reference
                count. This means it is possible to delete the symbol if the
                reference count is zero.
          */
      if (--(n_ptr->ref_count) == 0)
        krm_NTableReleaseEntry( n_ptr );
    }
  }
}


/*****************************************************************************
  FUNCTION : krm_getNTableFirstEntry

  PURPOSE  : get the first name-table entry
  NOTES    :

  RETURNS  : the nametable 
  UPDATE   : 
******************************************************************************/
struct NameTable       *krm_getNTableFirstEntry(void)
{
  if (NTable_array == NULL)  return( NULL );

  curr_NTable_block = NTable_block_list;
  curr_NTable_entry = NTable_array - 1;
  return( curr_NTable_entry );
}


/*****************************************************************************
  FUNCTION : krm_getNTableNextEntry

  PURPOSE  : get the next name-table entry
  NOTES    :

  RETURNS  : the nametable 
  UPDATE   : 
******************************************************************************/
struct NameTable       *krm_getNTableNextEntry(void)
{
  if ((NTable_array == NULL) || (curr_NTable_block == NULL))
    return( NULL );

  if (--curr_NTable_entry == curr_NTable_block)  {
    if ( (curr_NTable_block = curr_NTable_block->Entry.next) == NULL)
      return( NULL );

    curr_NTable_entry = curr_NTable_block + NTABLE_BLOCK;
  }

  return( curr_NTable_entry );
}





/*#################################################

GROUP: SiteTable Functions

#################################################*/
/*****************************************************************************
  FUNCTION : krm_allocSTableArray

  PURPOSE  : allocate another site-table block
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static int  krm_allocSTableArray(void)
{
  STableArray     tmp_ptr;


  tmp_ptr = (STableArray) calloc( STABLE_BLOCK + 1, STABLE_SIZE );
  if (tmp_ptr == NULL)  return( 1 );

  if (STable_array == NULL)  {
    tmp_ptr->Entry.next = NULL;     /*  free site-table block sentinel  */
    free_STable_entry = tmp_ptr;    /*  free site-table entry sentinel  */
  }
  else  {
    tmp_ptr->Entry.next = STable_block_list;    /*  append new site-table block
                                                    to block list   */
  }

  STable_block_list = tmp_ptr;      /*  update block list ptr  */
  NoOfAllocSTableEntries += STABLE_BLOCK;
  STable_array = tmp_ptr + 1;       /* STable_array points to the first entry */
  return( 0 );
}


/*****************************************************************************
  FUNCTION : krm_allocSTableArray

  PURPOSE  : get one site-table entry
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static  struct SiteTable    *krm_getSTableEntry(void)
{
  struct SiteTable  *tmp_ptr;


  KernelErrorCode = KRERR_NO_ERROR;
  if ((STable_array == NULL) || (NoOfSTableEntries == NoOfAllocSTableEntries))
    if (krm_allocSTableArray() != 0)  {
      KernelErrorCode = KRERR_INSUFFICIENT_MEM;
      return( NULL );
    }

  NoOfSTableEntries++;

  if (free_STable_entry->Entry.next != NULL)
    {   /*  a previous released site-table entry is availabe    */
    tmp_ptr = free_STable_entry;
    free_STable_entry = free_STable_entry->Entry.next;
  }
  else  {
    tmp_ptr = STable_array++;
  }

  return( tmp_ptr );
}


/*****************************************************************************
  FUNCTION : krm_releaseSTableEntry

  PURPOSE  : release site table block
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static  void    krm_releaseSTableEntry(struct SiteTable *STable_ptr)
{
  --NoOfSTableEntries;

  STable_ptr->site_func  = NULL;
  STable_ptr->Entry.next = free_STable_entry;
  free_STable_entry = STable_ptr;
}


/*****************************************************************************
  FUNCTION : krm_releaseSTableArrays

  PURPOSE  : release all site-table blocks
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
static  void    krm_releaseSTableArrays(void)
{
  struct SiteTable     *tmp_ptr;


  NoOfSTableEntries      = 0;
  NoOfAllocSTableEntries = 0;

  if (STable_array != NULL)  {
    while (STable_block_list != NULL)  {
      tmp_ptr =  STable_block_list->Entry.next;
      free( (char *) STable_block_list );
      STable_block_list = tmp_ptr;
    }

    free_STable_entry = NULL;
    STable_array      = NULL;
  }
}


/*****************************************************************************
  FUNCTION : krm_STableCreateEntry

  PURPOSE  : create new site-table entry
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
struct SiteTable *krm_STableCreateEntry(char *site_symbol, 
					SiteFuncPtr site_func)
{
  struct  NameTable     *n_ptr;
  struct  SiteTable     *s_ptr;


  if ( (s_ptr = krm_getSTableEntry() ) == NULL)
    return( NULL );     /*  memory alloc failed */

  if ((n_ptr = krm_NTableCreateEntry( site_symbol, SITE_SYM ) ) == NULL)  {
    krm_releaseSTableEntry( s_ptr );
    return( NULL );     /*  memory alloc failed */
  }

  s_ptr->Entry.site_name = n_ptr;
  s_ptr->site_func       = site_func;

  return( s_ptr );
}


/*****************************************************************************
  FUNCTION : krm_STableChangeEntry

  PURPOSE  : change the properties of the given site-table entry
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
struct  SiteTable   *krm_STableChangeEntry(struct SiteTable *stbl_ptr, 
					   char *new_site_name, 
					   SiteFuncPtr new_site_func)
{
  struct  NameTable     *n_ptr;


  if ((n_ptr = krm_NTableCreateEntry( new_site_name, SITE_SYM ) ) == NULL)
    return( NULL );     /*  memory alloc failed */

  krm_NTableReleaseEntry( stbl_ptr->Entry.site_name );

  stbl_ptr->Entry.site_name = n_ptr;
  stbl_ptr->site_func       = new_site_func;

  return( stbl_ptr );
}


/*****************************************************************************
  FUNCTION : krm_STableRemoveEntry

  PURPOSE  : release a previosly defined site-table entry
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_STableRemoveEntry(struct SiteTable *STable_ptr)
{
  krm_NTableReleaseEntry( STable_ptr->Entry.site_name );
  krm_releaseSTableEntry( STable_ptr );
}


/*****************************************************************************
  FUNCTION : krm_STableSymbolSearch

  PURPOSE  : searches for a symbol in the site-table
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
struct SiteTable    *krm_STableSymbolSearch(char *site_symbol)
{
  struct  NameTable   *n_ptr;
  struct  SiteTable   *s_ptr,
                      *block_list;


  if (STable_array == NULL)
    return( NULL );     /*  there are no site-table entries */

  if ( (n_ptr = krm_NTableSymbolSearch( site_symbol , SITE_SYM ) ) == NULL)
    return( NULL );     /*  symbol dosn't exist */


  block_list = STable_block_list;
  for (s_ptr = STable_array - 1; s_ptr > block_list; s_ptr--)
    if ( (s_ptr->site_func != NULL) &&
         (s_ptr->Entry.site_name == n_ptr ) )
      return( s_ptr );

  for (block_list = block_list->Entry.next;
       block_list != NULL;
       block_list = block_list->Entry.next)  {
    for (s_ptr = block_list + STABLE_BLOCK; s_ptr > block_list; s_ptr--)
      if ( (s_ptr->site_func != NULL) &&
           (s_ptr->Entry.site_name == n_ptr ) )
        return( s_ptr );
  }

  return( NULL );  /*  the site symbol is in the name table, but not in 
		       the site table: Error */
}


/*****************************************************************************
  FUNCTION : krm_getSTableNextRawEntry

  PURPOSE  : 
  NOTES    :

  RETURNS  : returns a pointer to the next (used or unused) site-table entry
  UPDATE   : 
******************************************************************************/
static struct SiteTable       *krm_getSTableNextRawEntry(void)
{
  if ((STable_array == NULL) || (curr_STable_block == NULL))
    return( NULL );

  if (--curr_STable_entry == curr_STable_block)
    {  /*  get new site-table block  */
    if ( (curr_STable_block = curr_STable_block->Entry.next) == NULL)  {
      curr_STable_block = NULL;
      curr_STable_entry = NULL;

      return( NULL );
    }

    /* next site-table block  */
    curr_STable_entry = curr_STable_block + STABLE_BLOCK; 
  }

  return( curr_STable_entry );
}


/*****************************************************************************
  FUNCTION : krm_getSTableNextEntry

  PURPOSE  : 
  NOTES    :

  RETURNS  : returns a pointer to the next used site-table entry
  UPDATE   : 
******************************************************************************/
struct SiteTable       *krm_getSTableNextEntry(void)
{
  struct  SiteTable   *stbl_ptr;


  if ((stbl_ptr = krm_getSTableNextRawEntry()) == NULL)
    return( NULL );

  while ( stbl_ptr->site_func == NULL)	/*  return only used site-table entries  */
    if ((stbl_ptr = krm_getSTableNextRawEntry()) == NULL)
      return( NULL );
        
  return( stbl_ptr );
}


/*****************************************************************************
  FUNCTION : krm_getSTableFirstEntry

  PURPOSE  : 
  NOTES    :

  RETURNS  : returns a pointer to the first used site-table entry
  UPDATE   : 
******************************************************************************/
struct SiteTable       *krm_getSTableFirstEntry(void)
{
  struct  SiteTable   *stbl_ptr;
  

  if (STable_array == NULL)  return( NULL );

  curr_STable_block = STable_block_list;
  curr_STable_entry = STable_array - 1;
  stbl_ptr = curr_STable_entry;

  if (stbl_ptr->site_func == NULL)  /*	return only used site-table entries  */
    if ((stbl_ptr = krm_getSTableNextEntry()) == NULL)
      return( NULL );

  return( stbl_ptr );
}





/*#################################################

GROUP: Ftype entry functions

#################################################*/

/*****************************************************************************
  FUNCTION : krm_getFtypeEntry

  PURPOSE  : allocate a new Ftype entry
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
struct FtypeUnitStruct  *krm_getFtypeEntry(void)
{
  struct FtypeUnitStruct  *Ftype_entry;


  KernelErrorCode = KRERR_NO_ERROR;
  if ((Ftype_entry = 
           (struct FtypeUnitStruct *) malloc( FTYPE_UNIT_SIZE ) ) == NULL)
    {  /*  memory alloc failed */
    KernelErrorCode = KRERR_INSUFFICIENT_MEM;
    return( NULL );
  }

  if (Ftype_list_root != NULL)  {
    Ftype_list_root->prev = Ftype_entry;
    Ftype_entry->next = Ftype_list_root;
  }
  else
    Ftype_entry->next = NULL;

  Ftype_list_root   = Ftype_entry;
  Ftype_entry->prev = NULL;

  Ftype_entry->sites = NULL;
  Ftype_entry->Ftype_symbol = NULL;

  ++NoOfFTableEntries;

  return( Ftype_entry );
}


/*****************************************************************************
  FUNCTION : krm_releaseFtypeEntry

  PURPOSE  : free a previosly defined Ftype entry
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_releaseFtypeEntry(struct FtypeUnitStruct *Ftype_entry)
{
  struct  FtypeUnitStruct   *next_entry,
                            *prev_entry;


  /*  release sites first */
  if ( Ftype_entry->sites != NULL)
    krm_releaseAllSites( Ftype_entry->sites );

  /*  release Ftype symbol name */
  if ( Ftype_entry->Ftype_symbol != NULL )
    krm_NTableReleaseEntry( Ftype_entry->Ftype_symbol );

  next_entry = Ftype_entry->next;
  prev_entry = Ftype_entry->prev;

  if (prev_entry != NULL)
    prev_entry->next = next_entry;
  else
    Ftype_list_root = next_entry;

  if (next_entry != NULL)
    next_entry->prev = prev_entry;

  /*  release unit entry  */
  free( (char *) Ftype_entry );       /*  release Ftype entry   */

  --NoOfFTableEntries;
}

/*****************************************************************************
  FUNCTION : krm_FtypeCreateEntry

  PURPOSE  : create and define a Ftype entry
  NOTES    :

  RETURNS  : the ftype
  UPDATE   : 
******************************************************************************/
struct FtypeUnitStruct  *krm_FtypeCreateEntry(char *Ftype_symbol, OutFuncPtr out_func, ActFuncPtr act_func, ActDerivFuncPtr act_deriv_func, ActDerivFuncPtr act_2_deriv_func, PyObject *python_out_func,PyObject *python_act_func,PyObject *python_act_deriv_func,   PyObject *python_act_2_deriv_func)
{
  struct  FtypeUnitStruct   *Ftype_entry;
  struct  NameTable         *n_ptr;


  if ( (Ftype_entry = krm_getFtypeEntry() ) == NULL)
    return( NULL );     /*  memory alloc failed */

  if((n_ptr = krm_NTableCreateEntry( Ftype_symbol, FTYPE_UNIT_SYM ) ) == NULL) {
    krm_releaseFtypeEntry( Ftype_entry );
    return( NULL );     /*  memory alloc failed */
  }

  Ftype_entry->Ftype_symbol = n_ptr;
  Ftype_entry->out_func = out_func;
  Ftype_entry->act_func = act_func;
  Ftype_entry->act_deriv_func = act_deriv_func;
  Ftype_entry->act_2_deriv_func = act_2_deriv_func;
  Ftype_entry->python_out_func = python_out_func;
  Ftype_entry->python_act_func = python_act_func;
  Ftype_entry->python_act_deriv_func = python_act_deriv_func;
  Ftype_entry->python_act_2_deriv_func = python_act_2_deriv_func;
  Ftype_entry->sites = NULL;

  return( Ftype_entry );
}


/*****************************************************************************
  FUNCTION : krm_FtypeAddSite

  PURPOSE  : add a site to a previosly defined Ftype entry
  NOTES    :

  RETURNS  : the site
  UPDATE   : 
******************************************************************************/
struct Site  *krm_FtypeAddSite(struct FtypeUnitStruct *Ftype_entry, 
			       struct SiteTable *STable_entry)
{
  struct  Site   *site_ptr;


  if ( (site_ptr = krm_getFtypeSite() ) == NULL)
    return( NULL );     /*  memory alloc failed */

  site_ptr->next = Ftype_entry->sites;
  Ftype_entry->sites = site_ptr;

  site_ptr->site_table = STable_entry;

  return( site_ptr );
}


/*****************************************************************************
  FUNCTION : krm_getFtypeFirstEntry

  PURPOSE  : 
  NOTES    :

  RETURNS  : returns a pointer to first Ftype entry
  UPDATE   : 
******************************************************************************/
struct FtypeUnitStruct  *krm_getFtypeFirstEntry(void)
{
  curr_Ftype_entry = Ftype_list_root;
  return( Ftype_list_root );
}


/*****************************************************************************
  FUNCTION : krm_getFtypeNextEntry

  PURPOSE  : 
  NOTES    :

  RETURNS  : returns a pointer to next Ftype entry
  UPDATE   : 
******************************************************************************/
struct FtypeUnitStruct  *krm_getFtypeNextEntry(void)
{
  if (curr_Ftype_entry != NULL)  {
    if (curr_Ftype_entry->next != NULL)
      curr_Ftype_entry = curr_Ftype_entry->next;
    else
      return( NULL );
  }

  return( curr_Ftype_entry );
}


/*****************************************************************************
  FUNCTION : krm_getFtypeNextEntry

  PURPOSE  : searches for a Ftype entry with the given name
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
struct  FtypeUnitStruct  *krm_FtypeSymbolSearch(char *Ftype_symbol)
{
  struct  FtypeUnitStruct   *ftype_entry;


  if (Ftype_symbol == NULL)  return( NULL);

  ftype_entry = Ftype_list_root;
  while (ftype_entry != NULL)  {
    if (strcmp( Ftype_symbol, (ftype_entry->Ftype_symbol)->Entry.symbol ) == 0)
      return( ftype_entry );

    ftype_entry = ftype_entry->next;
  }

  return( NULL );
}


/*****************************************************************************
  FUNCTION : krm_releaseFtypeList

  PURPOSE  : releases all Ftype entries
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_releaseFtypeList(void)
{
  struct  FtypeUnitStruct   *Ftype_entry,
                            *ft_ptr;


  Ftype_entry = Ftype_list_root;

  while( Ftype_entry != NULL )  {
    /*  release sites first */
    if ( Ftype_entry->sites != NULL)
      krm_releaseAllFtypeSites( Ftype_entry->sites );

    /*  release Ftype symbol name */
    if ( Ftype_entry->Ftype_symbol != NULL )
      krm_NTableReleaseEntry( Ftype_entry->Ftype_symbol );

    ft_ptr = Ftype_entry;
    Ftype_entry = Ftype_entry->next;
    /*  release unit entry  */
    free( (char *) ft_ptr );       /*  release Ftype entry   */
  }

  Ftype_list_root = NULL;
  NoOfFTableEntries = 0;
}



#ifdef MASPAR_KERNEL
#ifdef MASPAR_KERNEL_EMULATION

/*#################################################

GROUP: Functions for the MasPar kernel

#################################################*/
/*****************************************************************************
  FUNCTION : krm_releaseWeightArrays

  PURPOSE  : release weight arrays from memory
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void  krm_releaseWeightArrays(void)
{
  int  dest_layer, src_layer;
  FlintType  *weight_array;

  /*  get weight array pointers  */
  for (dest_layer = 1; dest_layer < descrFFnet.no_of_layers; dest_layer++)
    for (src_layer = 0; src_layer < dest_layer; src_layer++)  {
      weight_array = 
	  descrFFnet.layers[dest_layer].inputs[src_layer].weight_array;
      if (weight_array != NULL)  {
        free( (char *) weight_array );
	descrFFnet.layers[ dest_layer ].inputs[src_layer].weight_array = NULL;
	descrFFnet.layers[ dest_layer ].inputs[src_layer].no_of_inputs = 0;
      }
    }

  NoOfWeights = 0;
  specialNetworkType == NET_TYPE_GENERAL;
}


/*****************************************************************************
  FUNCTION :  krm_createWeightArrays

  PURPOSE  :  creates arrays containing connection weights for feedforward 
              networks
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
krui_err  krm_createWeightArrays(void)
{
  int  dest_layer, src_layer, no_of_inputs;
  FlintType  *weight_array;

  KernelErrorCode = KRERR_NO_ERROR;
  NoOfWeights = 0;

  /*  allocate weight arrays  */
  for (dest_layer = 1; dest_layer < descrFFnet.no_of_layers; dest_layer++)
    for (src_layer = 0; src_layer < dest_layer; src_layer++)  {
	no_of_inputs = 
	    descrFFnet.layers[ dest_layer ].inputs[src_layer].no_of_inputs;
      if (no_of_inputs > 0)
        {  /*  there are <no_of_inputs> connections between layer <src_layer>
               and layer <dest_layer>.  */
	weight_array = (FlintType *) calloc( no_of_inputs, sizeof (FlintType) );
        if (weight_array == NULL)
          {  /*  insufficient memory  */
          KernelErrorCode = KRERR_INSUFFICIENT_MEM;
          krm_releaseWeightArrays();
          return( KernelErrorCode );
        }

	descrFFnet.layers[dest_layer].inputs[src_layer].weight_array = 
	    weight_array;
      }
    }

  NoOfWeights = descrFFnet.no_of_weights;
  return( KernelErrorCode );
}

#endif
#endif



/*#################################################

GROUP: Memory cleanup

#################################################*/
/*****************************************************************************
  FUNCTION :  krm_releaseMem

  PURPOSE  :  frees all memory used for the internal representation of the 
              network
  NOTES    :

  RETURNS  :
  UPDATE   : 
******************************************************************************/
void    krm_releaseMem(void)
{
#ifdef MASPAR_KERNEL
#ifdef MASPAR_KERNEL_EMULATION

  if (specialNetworkType == NET_TYPE_FF1)
    krm_releaseWeightArrays();

#endif
#endif

  specialNetworkType = NET_TYPE_GENERAL;
  krm_releaseFtypeList();
  krm_releaseSTableArrays();
  krm_releaseNTableArrays();
  krm_releaseLinkArrays();
  krm_releaseSiteArrays();
  krm_releaseUnitArrays();
  krm_releaseUnitTopoArray();


  if (transTable != NULL)  {
    free( (void *) transTable );
    transTable = NULL;
    transTableSize = 0;
  }


}


/*  ---------------------  End of Memory Management Functions  -------------  */

/*  ---------------------    Begin Enzo Interface Functions   -------------  */
#ifdef __ENZO__
/*****************************************************************************
  FUNCTION : krm_getNet

  PURPOSE  : Receiving a network from the Enzo network manager modul
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void krm_getNet( memNet *n )
{                              /* the patterns stay untouched! */
    char *afunc, *ofunc;
    
    strcpy(n->update_func, krui_getUpdateFunc());
    strcpy(n->learn_func,  krui_getLearnFunc());
    strcpy(n->init_func,   krui_getInitialisationFunc());
    
    kr_getUnitDefaults( &n->u_act, &n->u_bias, &n->u_ttflags,
		        &n->u_subnet_no, &n->u_layer_no,
			&afunc, &ofunc                         );

    strcpy( n->u_act_func, afunc );
    strcpy( n->u_out_func, ofunc );

    n->NoOfLinks   = NoOfLinks;
    n->NoOfAllocLinks  = NoOfAllocLinks;
    n->link_array  = link_array;
    n->link_block_list   = link_block_list;
    n->free_link_ptr   = free_link_ptr;
    
    n->NoOfSites   = NoOfSites;
    n->NoOfNetSites  = NoOfNetSites;
    n->NoOfAllocSites  = NoOfAllocSites;
    n->site_array  = site_array;
    n->site_block_list   = site_block_list;
    n->free_site_ptr   = free_site_ptr;
    n->site_array  = site_array;


    n->NoOfAllocUnits  = NoOfAllocUnits;
    n->FreeUnitIndex   = FreeUnitIndex;
    n->NoOfUnits   = NoOfUnits;
    n->NoOfInputUnits  = NoOfInputUnits;
    n->NoOfOutputUnits   = NoOfOutputUnits;
    n->NoOfHiddenUnits   = NoOfHiddenUnits;
    n->MaxUnitNo   = MaxUnitNo;
    n->MinUnitNo   = MinUnitNo;
    n->unit_array  = unit_array;

    n->topo_ptr_array  = topo_ptr_array;
    
    n->NoOfNTableEntries   = NoOfNTableEntries;
    n->NoOfAllocNTableEntries  = NoOfAllocNTableEntries;
    n->NTable_array  = NTable_array;
    n->NTable_block_list   = NTable_block_list;
    n->free_NTable_entry   = free_NTable_entry;
    n->NTable_array  = NTable_array;
    
    n->NoOfSTableEntries   = NoOfSTableEntries;
    n->NoOfAllocSTableEntries  = NoOfAllocSTableEntries;
    n->STable_array  = STable_array;
    n->STable_block_list   = STable_block_list;
    n->free_STable_entry   = free_STable_entry;
    n->STable_array  = STable_array;
    
    n->Ftype_list_root   = Ftype_list_root;
    n->NoOfFTableEntries   = NoOfFTableEntries;



    n->curr_Ftype_entry = curr_Ftype_entry;
    n->curr_STable_entry = curr_STable_entry;
    n->curr_STable_block = curr_STable_block;
    n->curr_NTable_entry = curr_NTable_entry;
    n->curr_NTable_block = curr_NTable_block;
    n->NetModified =  NetModified;
    n->NetInitialize = NetInitialize;
    n->LearnFuncHasChanged = LearnFuncHasChanged;
    n->unitPtr =  unitPtr;
    n->sitePtr = sitePtr;
    n->prevSitePtr = prevSitePtr;
    n->linkPtr = linkPtr;
    n->prevLinkPtr = prevLinkPtr;
    n->unitNo = unitNo;
    n->specialNetworkType = specialNetworkType;
    n->TopoSortID = TopoSortID;
    n->no_of_topo_units = no_of_topo_units;
    
    /* treat this just as the net would have been deleted */

    NoOfLinks   = 0;
    NoOfAllocLinks  = 0;
    link_array  = NULL;
    link_block_list   = NULL;
    free_link_ptr   = NULL;
    
    NoOfSites   = 0;
    NoOfNetSites  = 0;
    NoOfAllocSites  = 0;
    site_array  = NULL;
    site_block_list   = NULL;
    free_site_ptr   = NULL;
    
    NoOfAllocUnits  = 0;
    FreeUnitIndex   = 0;
    NoOfUnits   = 0;
    NoOfInputUnits  = 0;
    NoOfOutputUnits   = 0;
    NoOfHiddenUnits   = 0;
    MaxUnitNo   = 0;
    unit_array  = NULL;

    topo_ptr_array  = NULL; 
    
    NoOfNTableEntries   = 0;
    NoOfAllocNTableEntries  = 0;
    NTable_array  = NULL;
    NTable_block_list   = NULL;
    free_NTable_entry   = NULL;
    NTable_array  = NULL;
    
    NoOfSTableEntries   = 0;
    NoOfAllocSTableEntries  = 0;
    STable_array  = NULL;
    STable_block_list   = NULL;
    free_STable_entry   = NULL;
    
    Ftype_list_root   = NULL;
    NoOfFTableEntries   = 0;
}



/*****************************************************************************
  FUNCTION : krm_putNet

  PURPOSE  : Putting a network back under the control of the Enzo network 
             manager modul
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void krm_putNet( memNet *n )
{
    
    krm_releaseFtypeList();
    krm_releaseSTableArrays();
    krm_releaseNTableArrays();
    krm_releaseLinkArrays();
    krm_releaseSiteArrays();
    krm_releaseUnitArrays();

    
    krui_setUpdateFunc( n->update_func );
    krui_setLearnFunc( n->learn_func );
    krui_setInitialisationFunc( n->init_func );

    kr_setUnitDefaults( n->u_act, n->u_bias, n->u_ttflags,
		        n->u_subnet_no, n->u_layer_no,
		        n->u_act_func, n->u_out_func       );

    NoOfLinks   = n->NoOfLinks;
    NoOfAllocLinks  = n->NoOfAllocLinks;
    link_array  = n->link_array;
    link_block_list   = n->link_block_list;
    free_link_ptr   = n->free_link_ptr;
    
    NoOfSites   = n->NoOfSites;
    NoOfNetSites  = n->NoOfNetSites;
    NoOfAllocSites  = n->NoOfAllocSites;
    site_array  = n->site_array;
    site_block_list   = n->site_block_list;
    free_site_ptr   = n->free_site_ptr;
    site_array  = n->site_array;

    NoOfAllocUnits  = n->NoOfAllocUnits;
    FreeUnitIndex   = n->FreeUnitIndex;
    NoOfUnits   = n->NoOfUnits;
    NoOfInputUnits  = n->NoOfInputUnits;
    NoOfOutputUnits   = n->NoOfOutputUnits;
    NoOfHiddenUnits   = n->NoOfHiddenUnits;
    MinUnitNo   = n->MinUnitNo;
    MaxUnitNo   = n->MaxUnitNo;
    unit_array  = n->unit_array;
    
    topo_ptr_array  = n->topo_ptr_array;
    
    NoOfNTableEntries   = n->NoOfNTableEntries;
    NoOfAllocNTableEntries  = n->NoOfAllocNTableEntries;
    NTable_array  = n->NTable_array;
    NTable_block_list   = n->NTable_block_list;
    free_NTable_entry   = n->free_NTable_entry;
    NTable_array  = n->NTable_array;
    
    NoOfSTableEntries   = n->NoOfSTableEntries;
    NoOfAllocSTableEntries  = n->NoOfAllocSTableEntries;
    STable_array  = n->STable_array;
    STable_block_list   = n->STable_block_list;
    free_STable_entry   = n->free_STable_entry;
    STable_array  = n->STable_array;
    
    Ftype_list_root   = n->Ftype_list_root;
    NoOfFTableEntries   = n->NoOfFTableEntries;

    curr_Ftype_entry = n->curr_Ftype_entry;
    curr_STable_entry = n->curr_STable_entry;
    curr_STable_block = n->curr_STable_block;
    curr_NTable_entry = n->curr_NTable_entry;
    curr_NTable_block = n->curr_NTable_block;
    NetModified =  n->NetModified;
    NetInitialize = n->NetInitialize;
    LearnFuncHasChanged = n->LearnFuncHasChanged;
    unitPtr =  n->unitPtr;
    sitePtr = n->sitePtr;
    prevSitePtr = n->prevSitePtr;
    linkPtr = n->linkPtr;
    prevLinkPtr = n->prevLinkPtr;
    unitNo = n->unitNo;
    specialNetworkType = n->specialNetworkType;
    TopoSortID = n->TopoSortID;
    no_of_topo_units = n->no_of_topo_units;
}



/*****************************************************************************
  FUNCTION : krm_getPattern

  PURPOSE  : Receiving a pattern set from the Enzo  manager modul

  RETURNS  : 
  NOTES    : This is just a dummy function

  UPDATE   : 
******************************************************************************/
void krm_getPattern( memPat *p )
{

}



/*****************************************************************************
  FUNCTION : krm_putPattern

  PURPOSE  : Receiving a pattern set back under the control of the Enzo 
             manager modul
  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
void krm_putPattern( memPat *p )
{
    static int in[MAX_NO_OF_VAR_I_DIM],out[MAX_NO_OF_VAR_I_DIM];

    krui_setCurrPatSet( p->number );
    in[0] = 1; /*krui_getNoOfInputUnits();*/
    in[1] = 1;
    out[0] = 1; /*krui_getNoOfOutputUnits();*/
    out[1] = 1;
    krui_DefTrainSubPat(in, out, in, out,NULL);
}

#endif

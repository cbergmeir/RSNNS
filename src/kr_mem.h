/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_mem.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Memory Manager Function Prototypes
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 21.2.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:53 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef  _KR_MEM_DEFINED_
#define _KR_MEM_DEFINED_



/*  get one link structure */
extern struct Link *krm_getLink( void );

/*  release one link structure */
extern void krm_releaseLink( struct Link * link_ptr);

/*  release the link and all following links */
extern void krm_releaseAllLinks( struct Link *first_link_ptr );

/*  get one unit-site structure */
extern struct Site *krm_getSite( void );

/*  release one unit-site structure */
extern void krm_releaseSite( struct Site *site_ptr );

/*  release the unit-site and all following sites (at this unit) */
extern void krm_releaseAllSites( struct Site *first_site_ptr );

/*  garbage collection of unit array   */
extern void  krm_unitArrayGC( void );

/*  allocate the unit array */
extern krui_err krm_allocUnits( int  N );

/*  get one unit structure */
extern int krm_getUnit( void );

/*  release unit */
extern void krm_releaseUnit( int  UnitNo );

/*  get information about memory usage */
extern void krm_getMemoryManagerInfo( int  *array_size, int info_array[] );

/*  allocate the array for topological sorting of the units in the network */
extern krui_err  krm_allocUnitTopoArray( int  N );

/*  release the topolocic array */
extern void  krm_releaseUnitTopoArray( void );

/*  release one name-table entry */
extern void krm_NTableReleaseEntry( struct NameTable  *NTable_ptr );

/*  Searches for a given symbol and symbol-type in the name table.
    Returns symbol ptr if symbol was found, NULL otherwise. */
extern struct NameTable *krm_NTableSymbolSearch( char  *symbol, int  sym_type );

/*  Creates a new symbol in the name-table.   Returns name-table ptr or NULL if
    memory alloc has failed.  */
extern struct NameTable *krm_NTableCreateEntry(char  *symbol_name, 
					       int  symbol_type );

/*  Inserts a symbol in the name-table. This function duplicates symbol ptrs
    if the symbol was found in the name-table.  Returns symbol ptr or NULL if
    memory alloc has failed. */
extern char *krm_NTableInsertSymbol( char  *symbol_name, int  symbol_type );

/*  release name-table entry if there is no other reference to this symbol */
extern void krm_NTableReleaseSymbol( char  *symbol_name, int  symbol_type );

/*  get the first name-table entry */
extern struct NameTable *krm_getNTableFirstEntry( void );

/*  get the next name-table entry */
extern struct NameTable *krm_getNTableNextEntry( void );

/*  create new site-table entry */
extern struct SiteTable *krm_STableCreateEntry(char  *site_symbol, 
					       SiteFuncPtr  site_func );

/*  change the properties of the given site-table entry */
extern struct SiteTable *krm_STableChangeEntry(struct  SiteTable  *stbl_ptr, 
					       char  *new_site_name,
                                         SiteFuncPtr  new_site_func );

/*  release a previosly defined site-table entry */
extern void krm_STableRemoveEntry( struct SiteTable  *STable_ptr );

/*  searches for a symbol in the site-table */
extern struct SiteTable *krm_STableSymbolSearch( char  *site_symbol );

/*  returns a pointer to the first used site-table entry */
extern struct SiteTable *krm_getSTableFirstEntry( void );

/*  returns a pointer to the next used site-table entry */
extern struct SiteTable *krm_getSTableNextEntry( void );

/*  allocate a new Ftype entry */
extern struct FtypeUnitStruct *krm_getFtypeEntry( void );

/*  free a previosly defined Ftype entry */
extern void krm_releaseFtypeEntry( struct FtypeUnitStruct  *Ftype_entry );

/*  create and define a Ftype entry
*/
extern struct FtypeUnitStruct *krm_FtypeCreateEntry( char *Ftype_symbol, OutFuncPtr out_func, 
                                              ActFuncPtr act_func, ActDerivFuncPtr act_deriv_func, ActDerivFuncPtr act_2_deriv_func );

/*  add a site to a previosly defined Ftype entry */
extern struct Site *krm_FtypeAddSite(struct FtypeUnitStruct  *Ftype_entry, 
				     struct SiteTable  *STable_entry );

/*  returns a pointer to first Ftype entry */
extern struct FtypeUnitStruct *krm_getFtypeFirstEntry( void );

/*  returns a pointer to next Ftype entry */
extern struct FtypeUnitStruct *krm_getFtypeNextEntry( void );

/*  searches for a Ftype entry with the given name */
extern struct FtypeUnitStruct *krm_FtypeSymbolSearch( char  *Ftype_symbol );

/*  releases all Ftype entries */
extern void krm_releaseFtypeList( void );

/*  free all link arrays */
extern void  krm_releaseLinkArrays( void );


/*  Functions for the MasPar kernel  */
#ifdef MASPAR_KERNEL
#ifdef MASPAR_KERNEL_EMULATION

/*  release weight arrays from memory */
extern void  krm_releaseWeightArrays( void );

/*  creates arrays containing connection weights for feedforward networks */
extern krui_err  krm_createWeightArrays( void );

#endif
#endif

/*  frees all memory used for the internal representation of the network */
extern void krm_releaseMem( void );

/* Functions for the Enzo interface */
#ifdef __ENZO__
#include "enzo_mem_typ.h"

extern void krm_getNet( memNet *n );
extern void krm_putNet( memNet *n );
extern void krm_getPattern( memPat *p );
extern void krm_putPattern( memPat *p );
#endif





/*#################################################

GROUP: Global Var's (as declared by the Memory Manager)

#################################################*/

extern int  NoOfAllocPatternPairs;  /*  no. of allocated pattern pairs  */

#endif 


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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_mem.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2
 
  PURPOSE        : SNNS-Kernel Memory Manager Function Prototypes
  NOTES          :
 
  AUTHOR         : Niels Mache
  DATE           : 21.2.90
 
  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.14 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:53 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#ifndef  _KR_MEM_DEFINED_
#define _KR_MEM_DEFINED_

/* begin global definition section */


/*  get one link structure */
struct Link *krm_getLink( void );

/*  release one link structure */
void krm_releaseLink( struct Link * link_ptr);

/*  release the link and all following links */
void krm_releaseAllLinks( struct Link *first_link_ptr );

/*  get one unit-site structure */
struct Site *krm_getSite( void );

/*  release one unit-site structure */
void krm_releaseSite( struct Site *site_ptr );

/*  release the unit-site and all following sites (at this unit) */
void krm_releaseAllSites( struct Site *first_site_ptr );

/*  garbage collection of unit array */
void  krm_unitArrayGC( void );

/*  allocate the unit array */
krui_err krm_allocUnits( int  N );

/*  get one unit structure */
int krm_getUnit( void );

/*  release unit */
void krm_releaseUnit( int  UnitNo );

/*  get information about memory usage */
void krm_getMemoryManagerInfo( int  *array_size, int info_array[] );

/*  allocate the array for topological sorting of the units in the network */
krui_err  krm_allocUnitTopoArray( int  N );

/*  release the topolocic array */
void  krm_releaseUnitTopoArray( void );

/*  release one name-table entry */
void krm_NTableReleaseEntry( struct NameTable  *NTable_ptr );

/*  Searches for a given symbol and symbol-type in the name table.
    Returns symbol ptr if symbol was found, NULL otherwise. */
struct NameTable *krm_NTableSymbolSearch( char  *symbol, int  sym_type );

/*  Creates a new symbol in the name-table.   Returns name-table ptr or NULL if
    memory alloc has failed. */
struct NameTable *krm_NTableCreateEntry( char  *symbol_name, int  symbol_type );

/*  Inserts a symbol in the name-table. This function duplicates symbol ptrs
    if the symbol was found in the name-table.  Returns symbol ptr or NULL if
    memory alloc has failed. */
char *krm_NTableInsertSymbol( char  *symbol_name, int  symbol_type );

/*  release name-table entry if there is no other reference to this symbol */
void krm_NTableReleaseSymbol( char  *symbol_name, int  symbol_type );

/*  get the first name-table entry */
struct NameTable *krm_getNTableFirstEntry( void );

/*  get the next name-table entry */
struct NameTable *krm_getNTableNextEntry( void );

/*  create new site-table entry */
struct SiteTable *krm_STableCreateEntry( char  *site_symbol, SiteFuncPtr  site_func );

/*  change the properties of the given site-table entry */
struct SiteTable *krm_STableChangeEntry(struct  SiteTable  *stbl_ptr, 
					char  *new_site_name,
                                         SiteFuncPtr  new_site_func );

/*  release a previosly defined site-table entry */
void krm_STableRemoveEntry( struct SiteTable  *STable_ptr );

/*  searches for a symbol in the site-table */
struct SiteTable *krm_STableSymbolSearch( char  *site_symbol );

/*  returns a pointer to the first used site-table entry */
struct SiteTable *krm_getSTableFirstEntry( void );

/*  returns a pointer to the next used site-table entry */
struct SiteTable *krm_getSTableNextEntry( void );

/*  allocate a new Ftype entry */
struct FtypeUnitStruct *krm_getFtypeEntry( void );

/*  free a previosly defined Ftype entry */
void krm_releaseFtypeEntry( struct FtypeUnitStruct  *Ftype_entry );

/*  create and define a Ftype entry
*/
struct FtypeUnitStruct *krm_FtypeCreateEntry( char *Ftype_symbol, OutFuncPtr out_func, 
                                              ActFuncPtr act_func, ActDerivFuncPtr act_deriv_func, ActDerivFuncPtr act_2_deriv_func );

/*  add a site to a previosly defined Ftype entry */
struct Site *krm_FtypeAddSite(struct FtypeUnitStruct  *Ftype_entry, 
			      struct SiteTable  *STable_entry );

/*  returns a pointer to first Ftype entry */
struct FtypeUnitStruct *krm_getFtypeFirstEntry( void );

/*  returns a pointer to next Ftype entry */
struct FtypeUnitStruct *krm_getFtypeNextEntry( void );

/*  searches for a Ftype entry with the given name */
struct FtypeUnitStruct *krm_FtypeSymbolSearch( char  *Ftype_symbol );

/*  releases all Ftype entries */
void krm_releaseFtypeList( void );

/*  free all link arrays */
void  krm_releaseLinkArrays( void );


/*  Functions for the MasPar kernel  */
#ifdef MASPAR_KERNEL
#ifdef MASPAR_KERNEL_EMULATION

/*  release weight arrays from memory */
void  krm_releaseWeightArrays( void );

/*  creates arrays containing connection weights for feedforward networks */
krui_err  krm_createWeightArrays( void );

#endif
#endif

/*  frees all memory used for the internal representation of the network */
void krm_releaseMem( void );

/* Functions for the Enzo interface */
#ifdef __ENZO__
#include "enzo_mem_typ.h"

void krm_getNet( memNet *n );
void krm_putNet( memNet *n );
void krm_getPattern( memPat *p );
void krm_putPattern( memPat *p );
#endif

/*
extern struct TransTable  *transTable;
extern int transTableSize;
*/



/*#################################################

GROUP: Global Var's (as declared by the Memory Manager)

#################################################*/

int  NoOfAllocPatternPairs;  /*  no. of allocated pattern pairs  */


/* end global definition section */

/* begin private definition section */


/*#################################################

GROUP: Local Var's

#################################################*/

int  FreeUnitIndex, /* no. of the next free unit in the unit array*/
	    NoOfAllocUnits,  /*  no. of allocated units  */
	    NoOfSites, /* no. of used sites in the network and Ftype 
				   sites (including sites for functionality 
				   type use only) */
	    NoOfNetSites, /* no. of used sites in the network (only unit 
				   sites)  */
	    NoOfAllocSites,  /*  no. of allocated sites  */
	    NoOfLinks,  /*  no. of links used for the network	*/
	    NoOfAllocLinks,  /*  no. of allocated links  */
	    NoOfNTableEntries, /*no. of name table entries  */
	    NoOfAllocNTableEntries, /*no. of allocated name table entries*/
	    NoOfSTableEntries, /*no. of site table entries  */
	    NoOfAllocSTableEntries, /*no. of allocated site table entries*/
	    NoOfFTableEntries; /*no. of functionality types  */

SiteArray    
            site_array,  /*  pointer to first site array  */
		       free_site_ptr,  /*  pointer to first free site  */
		       site_block_list;  /*  pointer to first free site block */

LinkArray       
            link_array,  /*  pointer to first link array  */
		       free_link_ptr,  /*  pointer to first free link  */
		       link_block_list;  /*  pointer to first free link block */


NTableArray  
            NTable_array,  /* pointer to name table  */
	    free_NTable_entry,  /* ptr to first free name table entry  */
	    NTable_block_list,  /* ptr to first free name table block  */
		       curr_NTable_entry,  /*  pointer to current name table entry  */
		       curr_NTable_block;  /*  pointer to current name table block  */


STableArray  
            STable_array,  /* pointer to site table  */
            free_STable_entry,  /* ptr to first free site table entry  */
	    STable_block_list,  /* ptr to first free site table block  */
		       curr_STable_entry,  /*  pointer to current name site entry  */
		       curr_STable_block;  /*  pointer to current name site block  */


struct FtypeUnitStruct  
            *Ftype_list_root,  /*  pointer to root of the Ftype list  */
			       *curr_Ftype_entry;  /*  pointer to current Ftype entry  */


 int     krm_allocLinks(int N);
 int     krm_allocSites(int N);
struct Site  *krm_getFtypeSite(void);

/* Future Use: release one Ftype-site structure */
/*void    krm_releaseFtypeSite(struct Site *site_ptr ); */

void    krm_releaseAllFtypeSites(struct Site *first_site_ptr);
 void    krm_releaseSiteArrays(void);
 void    krm_relocateLinkPtrs(long int offset);
	void	krm_releaseUnitArrays(void);
 int     krm_allocNTableArray(void);
struct NameTable  *krm_getNTableEntry(void);
void  krm_releaseNTableArrays(void);
int  krm_allocSTableArray(void);
//int  krm_allocSTableArray(void);
 struct SiteTable    *krm_getSTableEntry(void);
 void    krm_releaseSTableEntry(struct SiteTable *STable_ptr);
 void    krm_releaseSTableArrays(void);
struct SiteTable       *krm_getSTableNextRawEntry(void);


/* end private definition section */

#endif 


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
                                              ActFuncPtr act_func, ActDerivFuncPtr act_deriv_func, ActDerivFuncPtr act_2_deriv_func,
	PyObject *python_out_func, PyObject *python_act_func,
	PyObject *python_act_deriv_func, PyObject *python_act_2_deriv_func);

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


extern struct TransTable  *transTable;
extern int transTableSize;




/*#################################################

GROUP: Global Var's (as declared by the Memory Manager)

#################################################*/

int  NoOfAllocPatternPairs = 0;  /*  no. of allocated pattern pairs  */


/* end global definition section */

/* begin private definition section */


/*#################################################

GROUP: Local Var's

#################################################*/

static int  FreeUnitIndex  = 0, /* no. of the next free unit in the unit array*/
	    NoOfAllocUnits = 0,  /*  no. of allocated units  */
	    NoOfSites	   = 0, /* no. of used sites in the network and Ftype 
				   sites (including sites for functionality 
				   type use only) */
	    NoOfNetSites   = 0, /* no. of used sites in the network (only unit 
				   sites)  */
	    NoOfAllocSites = 0,  /*  no. of allocated sites  */
	    NoOfLinks	   = 0,  /*  no. of links used for the network	*/
	    NoOfAllocLinks = 0,  /*  no. of allocated links  */
	    NoOfNTableEntries	    = 0, /*no. of name table entries  */
	    NoOfAllocNTableEntries  = 0, /*no. of allocated name table entries*/
	    NoOfSTableEntries	    = 0, /*no. of site table entries  */
	    NoOfAllocSTableEntries  = 0, /*no. of allocated site table entries*/
	    NoOfFTableEntries	    = 0; /*no. of functionality types  */

static SiteArray    
            site_array        = NULL,  /*  pointer to first site array  */
		       free_site_ptr	 = NULL,  /*  pointer to first free site  */
		       site_block_list	 = NULL;  /*  pointer to first free site block */

static LinkArray       
            link_array        = NULL,  /*  pointer to first link array  */
		       free_link_ptr	 = NULL,  /*  pointer to first free link  */
		       link_block_list	 = NULL;  /*  pointer to first free link block */


static NTableArray  
            NTable_array      = NULL,  /* pointer to name table  */
	    free_NTable_entry = NULL,  /* ptr to first free name table entry  */
	    NTable_block_list = NULL,  /* ptr to first free name table block  */
		       curr_NTable_entry = NULL,  /*  pointer to current name table entry  */
		       curr_NTable_block = NULL;  /*  pointer to current name table block  */


static STableArray  
            STable_array      = NULL,  /* pointer to site table  */
            free_STable_entry = NULL,  /* ptr to first free site table entry  */
	    STable_block_list = NULL,  /* ptr to first free site table block  */
		       curr_STable_entry = NULL,  /*  pointer to current name site entry  */
		       curr_STable_block = NULL;  /*  pointer to current name site block  */


static struct FtypeUnitStruct  
            *Ftype_list_root  = NULL,  /*  pointer to root of the Ftype list  */
			       *curr_Ftype_entry = NULL;  /*  pointer to current Ftype entry  */


static  int     krm_allocLinks(int N);
static  int     krm_allocSites(int N);
static struct Site  *krm_getFtypeSite(void);

/* Future Use: release one Ftype-site structure */
/*static void    krm_releaseFtypeSite(struct Site *site_ptr ); */

static void    krm_releaseAllFtypeSites(struct Site *first_site_ptr);
static  void    krm_releaseSiteArrays(void);
static  void    krm_relocateLinkPtrs(int offset);
static	void	krm_releaseUnitArrays(void);
static  int     krm_allocNTableArray(void);
static struct NameTable  *krm_getNTableEntry(void);
static void  krm_releaseNTableArrays(void);
static int  krm_allocSTableArray(void);
static int  krm_allocSTableArray(void);
static  struct SiteTable    *krm_getSTableEntry(void);
static  void    krm_releaseSTableEntry(struct SiteTable *STable_ptr);
static  void    krm_releaseSTableArrays(void);
static struct SiteTable       *krm_getSTableNextRawEntry(void);


/* end private definition section */

#endif 


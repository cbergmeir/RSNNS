/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kernel.ph,v $
  SHORTNAME      : kernel.ph
  SNNS VERSION   : 4.2
 
  PURPOSE        : SNNS Kernel Function Prototypes
  NOTES          :
 
  AUTHOR         : Niels Mache
  DATE           : 20.02.90
 
  CHANGED BY     : Michael Vogt, Guenter Mamier,Christine Bagdi
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:22 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#ifndef _KENREL_DEFINED_
#define  _KENREL_DEFINED_

/* begin global definition section */

/*#################################################

GROUP:  Parallel kernel functions

#################################################*/

#ifdef  MASPAR_KERNEL
/*  Connects and Disconnects the MasPar.
    The mode switches are:  MASPAR_CONNECT and MASPAR_DISCONNECT. */
krui_err  kr_initMasPar( int  mode );

/* Returns the Status of the MasPar or an error code. */
krui_err  kr_getMasParStatus( void );
#endif

/*  Validate a network modifying operation according to the kernel mode. */
krui_err  kr_validateOperation( void );

/*  Sets the topologic type of the current network.
    Returns an error if the topologic type of the current network  
    doesn't fit to this type.
    Topologic types are:
      - NET_TYPE_GENERAL
        general purpose network type with no limitations
      - NET_TYPE_FF1
        feedforward network with fully connected units in
        neighbour layers */
krui_err  kr_setSpecialNetworkType( int  net_type );

/*  Returns the special topologic type of the current network, if set. */
int  kr_getSpecialNetworkType( void );

/*  Creates a user defined unit. */
int  kr_createUnit( char  *unit_name, char *out_func_name, char *act_func_name,
		   FlintTypeParam  i_act, FlintTypeParam  bias );

/*  Deletes all input links at current unit/site. */
krui_err  kr_deleteAllLinks( int  mode );

/*  Deletes the current link.
    NOTE: To delete a link between the current unit/site and the source unit
      <source_unit_no>, call krui_isConnected( source_unit_no ) and
      krui_deleteLink(). */
krui_err  kr_deleteLink( void );

/*  Creates a link between source unit and the current unit/site.
    Returns an error code:
     - if memory allocation fails
     - if source unit doesn't exist or
     - if there exists already a connection between current unit/site and
       the source unit
    0 otherwise.
    kr_createLink DO NOT set the current link.
   NOTE: If you want to create a link and its unknown if there exists already a
      connection between the two units, use krui_createLink and test the return
      code, instead of the sequence kr_isConnected and kr_createLink. */
krui_err  kr_createLink( int  source_unit_no, FlintTypeParam  weight );

/* Creates a link between source unit and the current unit/site 
   and sets the values for value_a, value_b and value_c. 
   Returns pointer to new unit */
struct Link*  kr_createLinkWithAdditionalParameters(int source_unit_no, 
			FlintTypeParam weight,
                       float val_a,float val_b,float val_c);

/*  Sets the link weight of the current link */
void  kr_setLinkWeight( FlintTypeParam	weight );

/*  Returns the link weight of the current link. */
FlintType  kr_getLinkWeight( void );

/*  True if there exists a connection between source unit <source_unit_no>
    and the current unit/site, otherwise false.
    NOTE: If there exists a connection between the two units, the current 
      link is set to the link between the two units. */
bool  kr_isConnected( int  source_unit_no, FlintType   *weight );

/*  True if there exists a connection between source unit <source_unit_no>
    and target unit <target_unit_no>, otherwise false. If there exist a
    connection between these units, kr_areConnected returns the connection
    strength also.
    Returns FALSE if unit doesn't exist.
    IMPORTANT: If there exist a connection, the current unit and site will be
	   set to the target unit/site.
    NOTE: This function is slow (Units are backward chained only). */
bool  kr_areConnected( int  source_unit_no, int  target_unit_no, 
		      FlintType   *weight );

/*  Returns the no. of first or next succecessor unit of the
    given unit and the connection strenght.
    Sets the current unit/site. */
int  kr_getSuccessorUnit( int  mode, int  source_unit_no, FlintType  *weigth );

/*  initialize the first/next site or the named site at the current unit
    for access */
int  kr_setSite( int  mode, char  *site_name );

/*  initializes the given unit for access */
krui_err  kr_setCurrUnit( int  unit_no );

/*  returns the number of the first/next/current unit of the unit array */
int  kr_getUnit( int  mode );

/*  Returns the no. of first, next or current predecessor unit of the
    current unit/site and the connection weight. */
int  kr_getPredecessorUnit( int  mode, FlintType  *weight, float* val_a, 
			    float* val_b, float* val_c);

/*  Remove unit and all links from network. */
krui_err  kr_removeUnit( struct Unit  *unit_ptr );

/*  Returns the value of the specified unit component. */
FlintType   kr_getUnitValues( int  unit_no, int  component_selector );

/*  Sets the value of the specified unit component. */
krui_err  kr_setUnitValues( int  unit_no, int  component_selector,
			   FlintTypeParam  value );

/*  Sets all unit components of the specified unit. */
krui_err  kr_setAllUnitValues( int unit_no, FlintTypeParam out, 
			      FlintTypeParam act,
			      FlintTypeParam i_act, FlintTypeParam bias );

/*  delete all inputs at the given unit */
void kr_deleteAllInputs( struct Unit  *unit_ptr );

/*  Deletes all output links at <source_unit>
    NOTE: This function is slow. */
void kr_deleteAllOutputLinks( struct Unit  *source_unit_ptr );

/*  search for a site at a unit */
struct Site *kr_searchUnitSite( struct Unit  *unit_ptr, 
			       struct SiteTable  *stbl_ptr );

/*  search for a site at a unit and returns also the predecessor site ptr */
void kr_searchUnitSiteWithPred( struct Unit  *unit_ptr, 
			       struct SiteTable  *stbl_ptr,
			       struct Site  * *Rsite_ptr, 
			       struct Site  * *Rsite_pred_ptr );

/*  searches for a site in the network  */
int kr_searchNetSite( struct SiteTable  *stbl_ptr );

/*  Creates a new site with default initialisation */
struct Site *kr_createDefaultSite( void );

/* returns the pointer to the given unit, returns NULL if unit doesn't exist */
struct Unit *kr_getUnitPtr( int  unit_no );

/*  Searches for a unit with the given symbol pointer.
    Returns the first unit no. if a unit with the given name was found,
    0 otherwise.*/
int kr_unitNameSearch( int  min_unit_no, char  *unit_symbol_ptr );

/*  Creates a unit with default values. */
int  kr_makeDefaultUnit( void );


/*  Copy a given unit, according to the copy mode
        1. copy unit (with it sites, if available) and input/output links
        2. copy unit (with it sites, if available) and input links
        3. copy unit (with it sites, if available) and output links
        4. copy unit (with it sites, if available) but no input/output links

    Returns the unit number of the new unit or error message < 0 , if errors 
    occured. Function has no effect on the current unit.
    NOTE: Copying of output links is slow.
      If return code < 0, an error occured.*/
krui_err  kr_copyUnit( int  copy_mode, int  source_unit );

/*  Sets the topologic type of the unit. */
krui_err  kr_unitSetTType( int  unit_no, int  UnitTType );


/*  changes all units in the network with the given functionality type
    to the new functions of the (new) functionality type */
void kr_changeFtypeUnits( struct  FtypeUnitStruct  *Ftype_entry );

/*   delete the functionality type of the units with the given type */
void kr_deleteUnitsFtype( struct  FtypeUnitStruct  *ftype_ptr );

/*  create a new unit with the given functionality type */
int  kr_makeFtypeUnit( char  *Ftype_symbol );

/*  returns TRUE, if there exists the given site at the given ftype entry */
bool kr_FtypeSiteSearch( struct  Site  *ftype_first_site, 
			struct  SiteTable  *site_table_ptr );

/*  change the properties of the given unit to the properties of the
    given F-Type */
void kr_changeFtypeUnit( struct  Unit  *unit_ptr, 
			struct  FtypeUnitStruct  *ftype_ptr );

/*  change a site at the F-Type */
void kr_changeFtypeSites( struct  FtypeUnitStruct  *Ftype_entry, 
                          struct  SiteTable  *old_site_table,
                          struct  SiteTable  *new_site_table );

/*  spell checker  (check identifiers for matching [A-Za-z]^[|, ]*) */
bool kr_symbolCheck( char    *symbol );

/*  translate unit flags to the topological type of the unit */
int kr_flags2TType( int  flags );

/*  translate the topological type to unit flags */
int kr_TType2Flags( int  ttype );

/*  update the outputs of all units in the network */
void kr_updateUnitOutputs( void );

/*  returns the no. of units of the specified topologic type
    (i.e. Input, Hidden, Output or Special units) */
int kr_getNoOfUnits( int  UnitTType );

/*  returns the no. of special units of the specified topologic type
    (i.e. Input, Hidden, Output or Special units) */
int kr_getNoOfSpecialUnits( int  UnitTType );

/*  force unit array garbage collection */
void  kr_forceUnitGC( void );


/*#################################################

GROUP: Topological Sorting Functions

#################################################*/


/*  Sort units according to the given mode:
    TOPOLOGICAL:
      Sort units topological (general version) and stores the
      pointers to this units in the topologic array.
      NOTE: Units are not sorted by their topologic type (that's not
	    possible in general case).

    TOPOLOGICAL_FF:
      Sorts unit topological in feed-forward networks and stores the
      pointers to this units in the topologic array in the following order:
       - input,
       - hidden and
       - output units

      This function make following assumtions (like all learning functions for
      feed-forward networks):
       a) input units doesn't have input connections to other units and
       b) output units doesn't have outputs connections to other units.

    TOPOLOGIC_TYPE:
      Sort units by their topologic type, i.e. Input, Hidden, Output units and
      stores the pointers to this units in the topologic array. */
krui_err kr_topoSort( int sort_mode );


/*  Checks the topology of the network:
     a) counts the number of layers of the network and
     b) determines if the network has cycles.
    Returns the no. of layers of the network. */
int kr_topoCheck( void );

/*  Count the no. of input and output units and return an error code
    if the no. do not fit to the loaded patterns. */
krui_err  kr_IOCheck( void );

krui_err  kr_makeUnitPermutation( void );


/*#################################################

GROUP: Other functions

#################################################*/


/*  Add random uniform distributed values to connection weights.
    <minus> must be less then <plus>. */
void kr_jogWeights( FlintTypeParam  minus, FlintTypeParam  plus );
krui_err kr_jogCorrWeights(FlintTypeParam minus, FlintTypeParam plus, 
			   FlintTypeParam mincorr);

/*  calls a network function */
krui_err  kr_callNetworkFunction( int  type, float  *parameterInArray, 
				 int  NoOfInParams, float * *parameterOutArray,
				 int  *NoOfOutParams,
				 int  start_pattern, int  end_pattern );

/*  Returns information about the unit default settings. */
void kr_getUnitDefaults( FlintType  *act, FlintType  *bias, int	 *ttflags, 
			int  *subnet_no, int  *layer_no, 
			char * *act_func, char * *out_func );

/*  Changes the unit default settings. */
krui_err kr_setUnitDefaults( FlintTypeParam  act, FlintTypeParam  bias, 
			    int  ttflags, int  subnet_no, int  layer_no, 
			    char  *act_func_ptr, char  *out_func_ptr );

/* for the network-analyzer tool
*/
float kr_NA_Error(int currentPattern, int error_unit, int error, bool ave);

/*#################################################

GROUP: Global Var's

#################################################*/

bool	NetModified = FALSE,   /* TRUE, if the network topology was modified */
        NetInitialize = TRUE,  /* TRUE, if the network has been initialized  */
        LearnFuncHasChanged = TRUE;  /* TRUE, if the learning func changed  */

FlagWord  DefaultSType   = DEF_STYPE;  /*  default topological type	*/

int  NoOfUnits	 = 0,    /*  no. of units in the network  */
     MinUnitNo	 = 0,    /*  the first (lowest) used unit no. in the network */
     MaxUnitNo	 = 0,	 /*  the last (highest) used unit no. in the network */
     NoOfInputUnits   = 0,	/*  no. of input units	*/
     NoOfOutputUnits  = 0,      /*  no. of output units  */
     NoOfHiddenUnits  = 0,	/*  no. of hidden units  */
     TopoSortID       = NOT_SORTED;  /*  topologic mode identifier  */

UnitArray     unit_array       = NULL;  /* the unit array  */

TopoPtrArray  topo_ptr_array   = NULL;  /* pointers to topological sorted units
					   used by kr_topoSort()  */

int  no_of_topo_units = 0;  /*  no. of unit pointers in the topo_ptr_array  */



/*  Kernel Interface Error Code */
krui_err  KernelErrorCode = KRERR_NO_ERROR;

/*  File I/O: Line number of the network file. */
int  lineno = 0;

/*  Stores the error codes and messages of the
    topologic sorting and network checking
    functions. */
struct TopologicMessages  topo_msg;


/*  Pointers and numbers for storing the current unit, site or link.
    Used by unit/site/link searching routines. */
struct Unit  *unitPtr = NULL;
struct Site  *sitePtr = NULL,
	     *prevSitePtr = NULL;
struct Link  *linkPtr = NULL,
	     *prevLinkPtr = NULL;
int  unitNo = 0;


int   specialNetworkType = NET_TYPE_GENERAL; /* topologic type of a network  */

float MinimumError = 1000000;     /* best recognition rate */
float Temperature;                /* temperature for Simulated Annealing */

#ifdef MASPAR_KERNEL

/*#################################################

GROUP:  Global var's of the parallel MasPar kernel

#################################################*/

int   masParStatus = MASPAR_DISCONNECT;   /*  holds the status of the MasPar */

/* stores the topologic description of a feedforward network */
struct FFnetDescriptor  descrFFnet,
                        descrFFnetIO;

#endif

/*#################################################

GROUP:  3D-Functions

#################################################*/

krui_err  kr_xyTransTable(int op, int *x, int *y, int z);



/* end global definition section */

/* begin private definition section */

/*#################################################

GROUP: Local Vars

#################################################*/

/*  topological sorting: global pointer to the topologic array
    (reduces memory consumption in the recursive depth search routine) */
static TopoPtrArray	global_topo_ptr;

static int  DefaultSubnetNo  = DEF_SUBNET_NO,	 /*  default subnet no.  */
            DefaultLayerNo   = DEF_LAYER_NO,	 /*  default layer no.	*/
            DefaultPosX      = DEF_POS_X,	 /*  default x-position  */
            DefaultPosY      = DEF_POS_Y;	 /*  default y-position  */

static int  DefaultPosZ = DEF_POS_Z;	 /*  default z-position  */


static FlintType
     DefaultIAct      = DEF_I_ACT,	 /*  default initial activation  */
     DefaultBias      = DEF_BIAS;	 /*  default bias */


static OutFuncPtr  DefaultUFuncOut  = NULL; /*  default output function     */
static ActFuncPtr  DefaultUFuncAct  = NULL; /*  default activation function */

/*  default derivation act. function  */
static ActDerivFuncPtr	DefaultUFuncActDeriv = NULL;

/*  default second derivation act. function  */
static ActDerivFuncPtr	DefaultUFuncAct2Deriv = NULL;

static PyObject * DefaultUPythonFuncOut;
static PyObject * DefaultUPythonFuncAct;
static PyObject * DefaultUPythonFuncActDeriv;
static PyObject * DefaultUPythonFuncAct2Deriv;

struct TransTable  *transTable = NULL;
int transTableSize = 0;




/*#################################################

GROUP: Macros

#################################################*/



/*#################################################

GROUP: Functions 

#################################################*/

/* count units according to their topological type */
static void  kr_countUnits(struct Unit *unit_ptr, int mode);

/*  */
static int  kr_searchOutputConnection(struct Unit *start_unit_ptr, 
				      struct Unit *source_unit_ptr, 
				      FlintType *weight);

/* Copies all output links at <source_unit> to <new_unit>. */
static krui_err  kr_copyOutputLinks(struct Unit *source_unit_ptr, 
				    struct Unit *new_unit_ptr);

/* Copy all input links from <source_unit> to <new_unit> */
static krui_err  kr_copyInputLinks(struct Unit *source_unit_ptr, 
				   struct Unit *new_unit_ptr);

/* copy the source unit with sites, but no links */
static krui_err kr_copyUnitFrame(struct Unit *source_unit_ptr, 
				 struct Unit *new_unit_ptr);

/* calculate correlated hidden units */
static krui_err kr_getCorrelatedHiddens(struct Unit **hn1, struct Unit **hn2, 
					double *res_corr);

/*  calls the current network function */
static krui_err  kr_callNetworkFunctionSTD(int type, float *parameterInArray, 
					   int NoOfInParams, 
					   float **parameterOutArray, 
					   int *NoOfOutParams, 
					   int start_pattern, int end_pattern);


/* end private definition section */

#endif  /* _KERNEL_DEFINED_ */

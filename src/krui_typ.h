/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/krui_typ.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2
 
  PURPOSE        : SNNS-Kernel User Interface Function Prototypes
  NOTES          : ANSI-C only
 
  AUTHOR         : Niels Mache
  DATE           : 27.02.90
 
  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:20 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/


/* -----------------------    THAT'S NEW:   -------------------------------


Development History of the SNNS Kernel (reverse order):

******************************************************************************

22.07.92:
   NEW FUNCTIONS:
     A set of new activation/initialization/learning functions for make use of
     Radial Basis Functions. Changes in:
     trans_f.c
     init_f.c
     learn_f.c
     func_tbl.c
   NEW MODULES:
     matrix.c
     matrix.h
      - general purpose matrix operations
     
*******************************************************************************


*******************************************************************************

22.05.92:
   NEW FUNCTIONS:
     krui_saveResult( ... )
      - saves a result file. Depends on the current loaded net and patterns

*******************************************************************************

*******************************************************************************

SNNS Kernel Version 2.1 with parallel kernel capability:

1.01.92:
   CHANGES:
     krui_deleteLink(),
     krui_deleteAllInputLinks(),
     krui_deleteAllOutputLinks()
       now returns and error code.

     krui_getCurrentSuccUnit( ... )
       has been removed from the interface.

   NEW FUNCTIONS:
     krui_setSpecialNetworkType( ... )
      - sets the topologic type of the current network.

     krui_getSpecialNetworkType()
      - returns the special topologic type of the current network, if set.

     krui_MasPar( ... )
      - connects and disconnects the MasPar.

     krui_getMasParStatus()
      - returns the status of the MasPar.

     krui_newVectorPatternPair( ... )
      - creates a new pattern vector.

     krui_getPatternArrays( ... )
      - determines the addresses of the internal pattern arrays.

     krui_allocatePatterns( ... )
      - allocate pattern pairs.

     krui_setNoOfPatterns( ... )
      - set the number of available pattern pairs.


*******************************************************************************

SNNS Kernel Version 2.0:

21.08.91:
   NEW FUNCTIONS:
     krui_searchUnitName( ... )
       - searches for a unit with the given name
     krui_searchNextUnitName()
       - searches for the next unit with the given name
     krui_testNet( ... )
       - calculates the network error whith a given pattern
     krui_getNoOfTTypeUnits( ... )
       - returns the no. of units of the specified topologic type
       (should now be used instead of "krui_getNoOfInputUnits" and
	"krui_getNoOfOutputUnits")

     krui_getFuncParamInfo( ... )
       - returns the no. of input and output parameters of the given function


   NEW FUNCTIONS for the 3D-Kernel:
     krui_getXYTransTable( ... )
       - returns the pointer of the XY-Translation Table

     krui_getUnitCenters( ... )
       - returns the 3D transform center of the specified unit and center number

     krui_setUnitCenters( ... )
       - sets the 3D transform center of the specified unit and center number


*******************************************************************************

SNNS Kernel Version 1.3:

26.04.91:
   CHANGES:
      krui_showPattern( mode ) 
      - has now only one parameter <mode> (the parameter <pattern_no> has left)
        krui_showPattern(...) shows now the CURRENT pattern.
        
      - The pattern file format has been changed but the scanner is downward kompatible.
        The new file format has now a SNNS-header with time stamp and is better readable.
        Comments (beginning with a #) are also possible.

   NEW FUNCTIONS:
     krui_setPatternNo( ... )
       - sets the current pattern for access
     krui_deletePattern()
       - deletes the current pattern
     krui_modifyPattern()
       - modifies the current pattern



*******************************************************************************

SNNS Kernel Version 1.2:

18.03.91:
   CHANGES:
     krui_randomizeWeights( ... )  
       - has been removed from the user interface
     krui_updateNet( ... )
       - parameters of krui_updateNet has changed

   NEW FUNCTIONS:
     krui_setInitialisationFunc( ... )
       - sets the initialisation function (i.e. randomize weights)
     krui_getInitialisationFunc()
       - returns the current initialisation function
     krui_initializeNet( ... )
       - initializes the net with the current initialisation function
     krui_setUpdateFunc( ... )
       - sets the update function (i.e. serial propagation)
     krui_getUpdateFunc()
       - returns the current update function
     krui_updateNet( ... )
       - updates the network

*******************************************************************************



*******************************************************************************

SNNS Kernel Version 1.1:

19.12.90:
   CHANGES:
     krui_learnAllPatterns( ... ) and krui_learnSinglePattern( ... )
     have parameter arrays now. The return type has changed to krui_err. 

   NEW FUNCTIONS:
     krui_getLearnFunc( ... )
       - Returns the name of the current learning function
     krui_setLearnFunc( ... )
       - Changes the learning function

        
*******************************************************************************

###############################################################################

18.10.90  SNNS Kernel Version 1.0

###############################################################################


18.10.90:
   NEW FUNCTIONS:
     krui_getUnitNoNearPosition( ... )
       - Returns the unit no. near the given position 


04.10.90:
   CHANGES:
     krui_getUnitNoAtPosition( ... )  needs now another parameter <subnet_no>.


25.09.90:
   NEW FUNCTIONS:
     krui_randomizeWeights( ... )
       - Initializes connection weights with uniform distributed random values.

     krui_jogWeights( ... )
       - Add uniform distributed random values to connection weights.

     krui_areConnected( ... )
       - Determines a connection AND the attached site between two units
         and returns the connection weight.


05.06.90:
   NEW FUNCTIONS:
     krui_setSite( char    *site_name )
       -  initializes the given site at the current unit.


04.06.90:
   All Symbols (Unit, Site, FType) will be spelling checked now.
   Symbols must match ([A-Za-z]^[|, ]*).

   krui_loadNet( ... )
    - now admit user friendly free style format
    - loads default presettings

   krui_saveNet( ... )  now saves default presettings.

   NEW FUNCTIONS:  krui_getUnitDefaults( ... ) and
                   krui_setUnitDefaults( ... )

                   - Determines and changes the default presettings.
                   NOTE: Presettings may now be changed dynamically, i.e
                         during creation of units.


29.05.90:
   NEW FUNCTIONS:
      krui_getMemoryManagerInfo( ... )
       - returns the number of ALLOCATED (not the number of USED) bytes
         for the units, sites, name-table and site-table.

      krui_getNetInfo( ... )
       -  returns miscellanous information about the current network.

      krui_getVersion()
       -  returns the current version of the SNNS-Kernel.


*******************************************************************************


    For additional type definitions and constants see include file "glob_typ.h" !
    For kernel user interface function prototypes use this file !
*/



/*#################################################

GROUP: Unit Functions

#################################################*/


int  krui_getNoOfUnits( void );
/*  Returns the number of units used by the network.
*/

int  krui_getFirstUnit( void );
/*  Initializes the first available unit for access. If the unit has sites, the
    first site will be set to the current site.
    Returns the unit no. of the first unit or 0 if no units available.
*/

int  krui_getNextUnit( void );
/*  Initializes the next available unit for access. If the unit has sites, the
    first site will be set to the current site.
    Returns the unit no. of the next unit or 0 if no more units available.
*/

int  krui_getCurrentUnit( void );
     /*  Returns the no. of the current unit or 0 if no units available.
     */

krui_err    krui_setCurrentUnit( int  UnitNo );
/*  Initializes a unit for access. If the unit has sites, the first site
    will be set to the current site.
    Returns error code if the given unit doesn't exist, 0 otherwise.
*/


char  *krui_getUnitName( int  UnitNo );
/*  Returns the name of the unit. (NULL if not available).
*/

krui_err  krui_setUnitName( int  UnitNo, char * unit_name );
/*  Sets the name of the unit <UnitNo>.
    If the unit_name pointer is NULL, the unit's symbol will be deleted.
    Function has no effect on the current unit.
    Returns error code if memory allocation fails, 0 otherwise.
*/

int  krui_searchUnitName( char  *unit_name );
/*  Searches for a unit with the given name.
    Returns the first unit no. if a unit with the given name was found,
    0 otherwise.

    Returns error code if no units defined.
*/

int  krui_searchNextUnitName( void );
/*  Searches for the next unit with the given name.
    Returns the next unit no. if a unit with the given name was found,
    0 otherwise.

NOTE: Call krui_searchUnitName( unit_name ) before calling krui_searchNextUnitName().

Returns error code if no units defined.
*/



char  *krui_getUnitOutFuncName( int  UnitNo );
/*  Returns the output function name of the unit.
    Do not use invalid unit numbers!
*/

krui_err   krui_setUnitOutFunc( int  UnitNo, char *  unitOutFuncName );
/*  The unit's FType will be set to 0, i.e. the
    unit's functionality type will be deleted.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
NOTE: Returns 0, if the function is a valid output function,
      error code otherwise.
*/

char  *krui_getUnitActFuncName( int  UnitNo );
/*  Returns the activation function name of the unit.
    Do not use invalid unit numbers!
*/

krui_err  krui_setUnitActFunc( int  UnitNo, char *  unitActFuncName );
/*  The unit's FType will be set to 0, i.e. the
    unit's functionality type will be deleted.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
NOTE: Returns 0, if the function is a valid activation function,
      error code otherwise.
*/


char  *krui_getUnitFTypeName( int UnitNo );
/*  Returns the functionality type name of the unit.
    Function has no effect on the current unit.
    Returns NULL if unit has no FType.
    Do not use invalid unit numbers!
*/


FlintType   krui_getUnitActivation( int  UnitNo );
/*  Returns the activation value of the unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
*/

krui_err  krui_setUnitActivation(int  UnitNo, FlintTypeParam  unit_activation);
/*  Sets the activation value of the unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
*/

FlintType  krui_getUnitInitialActivation( int  UnitNo );
/*  Returns the initial activation value of the unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
*/

void  krui_setUnitInitialActivation( int  UnitNo, FlintTypeParam  unit_i_activation );
/*  Sets the initial activation value of the unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
*/

FlintType  krui_getUnitOutput( int  UnitNo );
/*  Returns the output value of the unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
*/

krui_err  krui_setUnitOutput( int  UnitNo, FlintTypeParam  unit_output );
/*  Sets the output value of the unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
*/


FlintType  krui_getUnitBias( int  UnitNo );
/*  Returns the bias value of the unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
*/

void  krui_setUnitBias( int  UnitNo, FlintTypeParam  unit_bias );
/*  Sets the bias value of the unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
*/


int   krui_getUnitSubnetNo( int UnitNo );
/*  Returns the subnet number of the unit.
    Function has no effect on the current unit.
NOTE: The range of the subnet no. is -32736...+32735
    Do not use invalid unit numbers!
*/

void  krui_setUnitSubnetNo( int UnitNo, int  subnet_no);
/*  Sets the subnet number of the unit.
    Function has no effect on the current unit.
NOTE: The range of the subnet no. is -32736...+32735
    Do not use invalid unit numbers!
*/

unsigned short  krui_getUnitLayerNo( int UnitNo );
/*  Returns the layer number of the unit.
    Function has no effect on the current unit.
NOTE: Bit fields are 16 bit integers
    Do not use invalid unit numbers!
*/

void  krui_setUnitLayerNo( int UnitNo, int layer_bitField );
/*  Sets the layer number of the unit.
    Function has no effect on the current unit.
NOTE: Bit fields are 16 bit integers
    Do not use invalid unit numbers!
*/

void  krui_getUnitPosition( int  UnitNo, struct PosType  *position );
/*  Returns the position of the unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!

NOTE: See include file glob_typ.h for PosType definition !
*/

void  krui_setUnitPosition( int  UnitNo, struct PosType  *position );
/*  Sets the position of the unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!

NOTE: See include file glob_typ.h for PosType definition !
*/


int  krui_getUnitNoAtPosition( struct PosType  *position, int subnet_no );
/*  Returns the unit no. at the given position and the specified
    subnet no or 0, if no unit exists at this position.
    Function has no effect on the current unit.

NOTE: This function is slow because it uses linear search to
      find the unit with the given position.

REMARK: getUnitNoAtPosition is for downward compatibility only.
        Do not use this function in future applications.
*/

int  krui_getUnitNoNearPosition( struct PosType  *position, int subnet_no ,
                                 int range, int gridWidth );
/*  Returns the unit no. near the given position and the specified
    subnet no or 0, if no unit exists at this position.
    Function has no effect on the current unit.

NOTE: This function is slow because it uses linear search to
      find the unit with the given position.
*/


/*  Functions for the 3D-Kernel  */
krui_err  krui_getUnitCenters( int unit_no, int center_no, struct PositionVector  * *unit_center );
/*  Returns the 3D transform center of the specified unit and center number.
    Function has no effect on the current unit.
    Returns error number if unit or center no. is invalid
*/

krui_err  krui_setUnitCenters( int unit_no, int center_no, struct PositionVector  *unit_center );
/*  sets the 3D transform center of the specified unit and center number
    Function has no effect on the current unit.
    Returns error number if unit or center no. is invalid
*/

krui_err  krui_xyTransTable(int op, int *x, int *y, int z);
/*  Returns the x y coordinate of the given z-value.
    Returns error code if the SNNS-Kernel isn't a 3D-Kernel.
*/





int  krui_getUnitTType( int  UnitNo );
/*  Returns the topologic type, i.e. input, output, hidden
    Function has no effect on the current unit.

NOTE: See include file glob_typ.h for TType constants !

    Do not use invalid unit numbers!
*/

krui_err  krui_setUnitTType( int  UnitNo, int  UnitTType );
/*  Sets the topologic type of the unit.
    Function has no effect on the current unit.

NOTE: See include file glob_typ.h for TType constants !

    Returns error code if topologic type or unit number is invalid.
*/

krui_err  krui_freezeUnit( int  UnitNo );
/*  Freezes the output of a unit, i.e. the unit is disabled.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
*/

krui_err  krui_unfreezeUnit( int  UnitNo );
/*  Disabels a previosly freezed unit.
    Function has no effect on the current unit.
    Do not use invalid unit numbers!
*/

bool  krui_isUnitFrozen( int  UnitNo );
/*  Returns true, if unit is frozen
    Do not use invalid unit numbers!
*/


int  krui_getUnitInputType( int UnitNo );
/*  Returns the input type of the unit:
     NO_INPUTS    : if the unit has not inputs (at least not now)
     SITES        : if the unit has one or more sites (and no direct input links !)
     DIRECT_LINKS : if the unit has direct input links (and no sites !)

    Function has no effect on the current unit.

NOTE: See include file glob_typ.h for input type constants !
*/

/*  *************************************   */

int  krui_createDefaultUnit( void );
/*  Creates a unit with default properties:
    1. default activation and output functions
    2. default activation and bias
    3. default position, subnet and layer number
    4. no functionality type
    5. no sites
    6. no inputs and outputs
    7. no unit_name

    Returns an (negative) error code, if memory allocation fails or
    invalid functions occured. Returns (positive) unit number otherwise.
    Function has no effect on the current unit.

NOTE: See file "kr_def.h" for more details about default presettings.
*/

int  krui_createUnit( char  *unit_name, char  *out_func_name, char  *act_func_name,
                      FlintTypeParam  i_act,  FlintTypeParam  bias);
/*  Creates a user defined unit.
    Returns an (negative) error code, if memory allocation fails or
    invalid functions occured. Returns (positive) unit number otherwise.
    Function has no effect on the current unit.

    Unit has additional default settings:
    1. default position, subnet and layer number
    2. no functionality type
    3. no sites
    4. no inputs and outputs

*/


krui_err  krui_deleteUnit( int UnitNo );
/*  Deletes a unit from network. Removes all links to other
    units. Returns error code if unit doesn't exist.
*/

int  krui_createFTypeUnit( char *  FType_name);
/*  creates a unit with FType properties, but:
    1. no inputs and outputs
    2. default position, subnet and layer

    Returns the unit number or (negative) error code if memory allocation
    fails or functionality type isn't defined.
    Function has no effect on the current unit.
*/


krui_err  krui_setUnitFType( int  UnitNo, char *  FTypeName );
/*  Changes the properties of unit <UnitNo> to FType properties.

    Returns an error code if
     - FType name doesn't exist or
     - unit doesn't exist or
     - memory allocation fails
     otherwise 0.

    Function has no effect on the current unit.
*/


int  krui_copyUnit( int  UnitNo, int  copy_mode);
/*  Copy a given unit, according to the copy mode
        1. copy unit (with it sites, if available) and input/output links
        2. copy unit (with it sites, if available) and input links
        3. copy unit (with it sites, if available) and output links
        4. copy unit (with it sites, if available) but no input/output links

    Returns the unit number of the new unit or error message < 0 , if errors occured.
    Function has no effect on the current unit.

NOTE: Copying of output links is slow.
      If return code < 0, an error occured.

      See include file glob_typ.h for copy mode constants !

*/


/*#################################################

GROUP: Functions for manipulation of the Unit-Functionality-List
REMEMBER: The Unit-Functionality-List stores:
            1. unit activation and output functions
            2. if sites:  3. all site functions
                          4. all site names


#################################################*/

bool  krui_setFirstFTypeEntry( void );
/*  Initializes the first FType entry.
    Returns true, if an entry is available
*/
bool  krui_setNextFTypeEntry( void );
/*  Initializes the next FType entry.
    Returns true, if an entry is available
*/

bool  krui_setFTypeEntry( char *Ftype_symbol );
/*  Initializes the FType entry with the given name.
    Returns true, if an entry with this name is available.
*/

char *krui_getFTypeName( void );
/*  Returns the name of the current FType entry.
NOTE: The FType name is definite and will never be
      a NULL pointer.
*/

krui_err  krui_setFTypeName( char  *unitFType_name );
/*  Sets the name of the current FType entry.
    Returns error code if memory allocation fails or Ftype name
    isn't definite, 0 otherwise.
NOTE: The new FType name have to be definite and must not be
      a NULL pointer.
*/

char  *krui_getFTypeActFuncName( void );
/*  Returns the name of the activation function in the current
    FType entry.
*/

krui_err  krui_setFTypeActFunc( char * act_func_name );
/*  Sets the activation function in the current FType entry
    returns 0, if the function is a valid activation function,
    error code otherwise.
    All units (in the existing network) of the current Ftype changes
    their activation function.
*/

char  *krui_getFTypeOutFuncName( void );
/*  Returns the name of the output function in the current
    FType entry.
*/

krui_err  krui_setFTypeOutFunc( char * out_func_name );
/*  Sets the output function in the current FType entry
    Returns 0, if the function is a valid output function,
    error code otherwise.
    All units (in the existing network) of the current Ftype changes
    their output function.
*/

/*  FType sites
*/
bool  krui_setFirstFTypeSite( void );
/*  Initializes the first site of the current FType.
    Returns FALSE, if no sites are available in the current
    FType entry.
*/

bool  krui_setNextFTypeSite( void );
/*  Initializes the next FType site.
    Returns FALSE, if no more sites are available in the current
    FType entry.
*/

char  *krui_getFTypeSiteName( void );
/*  Returns the name of the current FType site (in the current
    Ftype entry).
*/

krui_err  krui_setFTypeSiteName( char *  FType_site_name );
/*  Sets the name and function of the current FType site (in the
    current FType entry).
    All sites (in the existing network) of the current Ftype and the
    same (old) name changes their names and site functions.

    Returns an error code if
      - current Ftype site isn't defined
      - site name does not exist in the site name table
      0 otherwise.
*/

krui_err  krui_createFTypeEntry( char *  FType_symbol,
                                 char *  act_func, char *  out_func,
                                 int  no_of_sites, char * *array_of_site_names );

/*  Create a new functionality type, needs a definite FType symbol,
    the unit output and activation function
    and the number of sites provided for this unit FType.
    An additional array with N elements of pointers to site names
    is required for the definition of the sites.

    Returns error code if:
     - memory allocation fails
     - FType name isn't definite (symbol is used for another FType
       or symbol is a NULL pointer)
     - one or more site names are undefined

     0 otherwise.

NOTE: The number of Ftype entries and the number of sites per Ftype is
      only limited by the size of system memory.
*/

krui_err  krui_deleteFTypeEntry( char  *FType_symbol );
/*  Deletes the specified FType entry. If there exists units in the
    network with this FType, all these units will lost their FType
    but the functionality of the units will not be changed.
    Returns error code if FType symbol dosn't exist, 0 otherwise.
*/


/*#################################################

GROUP: Functions for reading of the function table
       The function table holds the names, types and no. of parameters
       of all functions (transfer, propagate, learning and initialisation
       functions)

#################################################*/

int  krui_getNoOfFunctions( void );
/*  Returns the number of available functions
*/

void  krui_getFuncInfo( int  func_no, char * *func_name, int  *func_type );
/*  Returns the name of the function and the function type (Output,
    Activation, Site).

    NOTE: See include file glob_typ.h for function type constants !
*/

bool  krui_isFunction( char  *func_name, int  func_type );
/*  Returns true if the given function name and type exists.

    NOTE: See include file glob_typ.h for function type constants !
*/

bool  krui_getFuncParamInfo( char *func_name, int  func_type,
                             int *no_of_input_params,
                             int *no_of_output_params );

/*  Returns the no. of input and output parameters of the given
    function (only relevant for learning, update and initialisation
    functions).

    Returns TRUE if the given function exists, FALSE otherwise.
*/

/*#################################################

GROUP: Site Table Functions

#################################################*/

bool  krui_getFirstSiteTableEntry( char * *site_name, char * *site_func );
/*  Returns the first site name/function pair in the site table.
    Returns FALSE and NULL, if not available.
*/

bool  krui_getNextSiteTableEntry( char * *site_name, char * *site_func );
/*  Returns the next site name/function pair in the site table.
    Returns FALSE and NULL, if not available.
*/

char  *krui_getSiteTableFuncName( char  *site_name );
/*  Returns the name of the site function that is associated with
    the site name.
    If the site name do not exist, function returns NULL.
*/

krui_err  krui_createSiteTableEntry( char  *site_name, char  *site_func );
/*  Creates a new site name and associate this name with a site
    function.
    Returns error code if:
     - site name already exists or
     - site function is invalid or
     - memory allocation has failed
    0 otherwise.
*/

krui_err  krui_changeSiteTableEntry( char  *old_site_name, char  *new_site_name, char  *new_site_func );
/*  Changes the site function of a previously defined site name.
    Returns error code if <old_site_name> or <new_site_func>
    are not defined, 0 otherwise.

NOTE: All sites in the network with the name <old_site_name>
      changes their names and functions.
*/

krui_err  krui_deleteSiteTableEntry( char *site_name );
/*  Removes the current site name entry from the site table.

    Returns an error code if there exists sites in the network
    with the given name, 0 otherwise.
*/

/*#################################################

GROUP: Site Functions

#################################################*/

bool  krui_setFirstSite( void );
/*  Initializes the first site at the current unit.
    Returns false if no site available or if no sites permitted
    at this unit.
*/

bool  krui_setNextSite( void );
/*  Initializes the next site at the current unit.
    Returns false if no more sites available.
*/

krui_err  krui_setSite( char    *site_name );
/*  Initializes the given site at the current unit.
    Returns error code if
     - unit dosn't exist
     - site name doesn't exist
     - unit don't has sites
     - unit don't has a site with this name
    0 otherwise.
*/


FlintType  krui_getSiteValue( void );
/*  Returns the actual value of the current site.
*/

char  *krui_getSiteName( void );
/*  Returns the name of the current unit/site,
    NULL if not available.
*/

krui_err  krui_setSiteName( char  *site_name );
/*  Sets the name/function of the current unit/site.
    Current Unit will loose the functionality type.
    Returns error code if site name isn't defined.
*/

char  *krui_getSiteFuncName( void );
/*  Returns the name of the current unit/site function.
*/

krui_err  krui_addSite( char *  site_name );
/*  Adds a site at the current unit. If the unit has already sites, this new
    site will be inserted above all other sites, i.e. the new created site will
    be the first site at this unit.
    If the unit has direct input links, i.e the unit has input links
    but no sites, the creation of sites is not permitted (krui_addSite will
    return an error code).
    If there exists already a site with the given name, the creation of the
    new site is prohibited and krui_addSite returns an error code.

    krui_addSite has no effect on the current site. To change the current site
    to this new site, call krui_setFirstSite().
    The unit's FType will be deleted.

    Returns error code if
      - memory allocation fails or
      - unit has direct input links or
      - site name isn't defined or
      - site with the given name exists already at this unit
      0 otherwise.

NOTE: The number of sites per unit is nearly unlimited (2^32).
*/


bool  krui_deleteSite( void );
/*  Removes the current site at the current unit and removes all
    links from predecessor units to this site.
    krui_setFirstSite (krui_setNextSite) must be called at least once
    before using this function.
    The current site will be set to the next available site, if no more
    sites available, krui_deleteSite returns 0 otherwise 1.
    Returns an error code if ther was a problem.
    The unit's FType will be set to 0, i.e. the unit's functionality
    type will be deleted.

NOTE: To delete all sites at a unit:
        if ( krui_setFirstSite() )
          while ( krui_deleteSite() > 0) { }
*/


/*#################################################

GROUP: Link Functions

#################################################*/

int  krui_getFirstPredUnit( FlintType  *strength );
/*  Returns the no. of first predecessor unit of the current unit/site
    and the connection strenght.
    Returns 0 if no predecessor unit available, i.e. if the current
    unit and/or site has no inputs.

NOTE: If a predecessor unit exists, the current link is set to the link
      between the two units.
*/

int  krui_getNextPredUnit( FlintType   *strength );
/*  Returns the no. of the next predecessor unit of the current
    unit/site and the connection strenght.
    Returns 0 if no more predecessor units exists.
NOTE: If another predecessor unit exists, the current link is set to the link
      between the two units.
*/

int  krui_getCurrentPredUnit( FlintType   *strength );
/*  Returns the no. of the current predecessor unit (of the current
    unit/site) and the connection strenght.
    Returns 0 if no predecessor unit available, i.e. if the current unit
    and/or site has no inputs.
*/

int  krui_getFirstSuccUnit( int  UnitNo, FlintType   *strength );
/*  Returns the no. of the first successor unit of the unit <source_unit_no>
    and the connection strenght.
    Returns (negative) error code if unit doesn't exist.
    Returns 0 if no successor unit available, i.e. if the given unit
    has no output connection.

IMPORTANT: If a successor unit exists, the current unit and site will be
           set to this successor unit and the attached site.

NOTE: This function is slow (Units are backward chained only).

REMARK: getFirstSuccUnit is for downward compatibility only.
        Do not use this function in future applications.
*/


int  krui_getNextSuccUnit( FlintType  *strength );
/*  Returns the no. of the next successor unit and the connection strenght.

IMPORTANT: If a successor unit exists, the current unit and site will be
           set to this successor unit and the attached site.

NOTE: This function is slow  (Units are backward chained only)

REMARK: getNextSuccUnit is for downward compatibility only.
        Do not use this function in future applications.
*/


extern bool  krui_areConnected(int source_unit_no, int target_unit_no);
extern bool  krui_areConnectedWeight(int source_unit_no, int target_unit_no, FlintType *weight);
/*  Returns true if there exists a connection between source unit <source_unit_no>
    and target unit <target_unit_no>, otherwise false. If there exist a
    connection between these units, krui_areConnected returns the connection
    strength also.
    Returns FALSE if unit doesn't exist.

IMPORTANT: If there exist a connection, the current unit and site will be
           set to the target unit and the attached site.

NOTE: This function is slow (Units are backward chained only).
*/

bool krui_isConnected( int  source_unit_no );
/*  True if there exists a connection between source unit <source_unit_no>
    and the current unit/site, otherwise false.

NOTE: If there exists a connection between the two units, the current link is set
      to the link between the two units. (alter the link weight with krui_setLinkWeight)
*/

FlintType  krui_getLinkWeight( void );
/*  Returns the link weight of the current link.
*/

void  krui_setLinkWeight( FlintTypeParam  strength );
/*  Sets the link weight of the current link.
*/

krui_err  krui_createLink( int source_unit_no, FlintTypeParam  strength  );
/*  Creates a link between source unit and the current unit/site.
    Returns an error code:
     - if memory allocation fails
     - if source unit doesn't exist or
     - if there exists already a connection between current unit/site and
       the source unit
    0 otherwise.
    krui_createLink DO NOT set the current link.

NOTE: If you want to create a link and its unknown if there exists already a
      connection between the two units, use krui_createLink and test the return
      code, instead of the sequence krui_isConnected and krui_createLink.
*/

krui_err  krui_deleteLink( void );
/*  Deletes the current link.

NOTE: To delete a link between the current unit/site and the source unit
      <source_unit_no>, call krui_isConnected( source_unit_no ) and
      krui_deleteLink().
*/

krui_err  krui_deleteAllInputLinks( void );
/*  Deletes all input links at current unit/site.
*/

krui_err  krui_deleteAllOutputLinks( void );
/*  Deletes all output links at current unit.
NOTE: This function is slow.
*/

void  krui_jogWeights( FlintTypeParam  minus, FlintTypeParam  plus);
/*  Add uniform distributed random values to connection weights.
    <minus> must be less then <plus>.
*/



/*#################################################

GROUP: Functions for network updating

#################################################*/

krui_err  krui_updateSingleUnit( int  UnitNo );
/*  Updates a single unit. Returns error code if unit doesn't exist,
    0 otherwise.

NOTE: Updates also frozen Units.
*/

char  *krui_getUpdateFunc( void );
/*  Returns the current update function. The default update function is
    SerialOrder() (see also kr_def.h).
*/

krui_err  krui_setUpdateFunc( char  *update_func );
/*  Changes the current update function.  Returns error code if update
    function is invalid.
*/

krui_err  krui_updateNet( float *parameterArray, int  NoOfParams );
/*  Updates the network according to update function:

      To propagate a pattern thru the network the use of following
      function calls is recommended:

        krui_setPatternNo( pat_no );
        krui_showPattern( OUTPUT_NOTHING );
        krui_updateNet( parameterArray, NoOfParams );

      See also krui_setSeedNo for initializing the pseudo
      random generator.

      Returns error code if an error occured, 0 othrwise.

NOTE: The network should be feedforward in topological mode,
      otherwise function will return a warning message.
*/

krui_err  krui_testNet( int  pattern_no, 
                        float  *updateParameterArray, int  NoOfUpdateParams,
                        float  *parameterInArray, int  NoOfInParams,
                        float  * *parameterOutArray, int  *NoOfOutParams );
/*  Calculates the network error whith the given pattern.
    Uses the current update function to propagate the network.

    UpdateParameterArray contains the parameters of the update function.
    NoOfUpdateParams contains the number of input parameters of the update function.

    parameterInArray[0] contains the max. devitation.
    Set NoOfInParams to 1.

    parameterOutArray[0] contains the error of the network/pattern,
    parameterOutArray[1] contains the number of output units with a
    higher error value than the given max. devitation.
    NoOfOutParams will be set to 2.

NOTE: Patterns must be loaded before calling this function.

Returns error code if an error occured, 0 otherwise.
*/


/*#################################################

GROUP: Functions for network initialisation

#################################################*/

char  *krui_getInitialisationFunc( void );
/*  Returns the current initialisation function. The default initialisation function is
    Randomize_Weights (see also kr_def.h).
*/

krui_err  krui_setInitialisationFunc( char  *init_func );
/*  Changes the current initialisation function.  Returns error code if initialisation
    function is invalid.
*/

krui_err  krui_initializeNet( float *parameterArray, int  NoOfParams );
/*  Initializes the network
*/


/*#################################################

GROUP: Learning Functions

#################################################*/

char  *krui_getLearnFunc( void );
/*  Returns the name of the current learning function.
*/

krui_err  krui_setLearnFunc( char  *learning_func );
/*  Changes the learning function. Returns a error code if the
    given learning function is invalid.
*/

krui_err  krui_learnAllPatterns( float   *parameterInArray, int NoOfInParams,
                                 float  * *parameterOutArray, int *NoOfOutParams );
/*  Learn all pattern pairs using current learning method.
    parameterInArray contains the learning
    parameter(s). NoOfInParams stores the number of learning parameters.
    parameterOutArray returns the results from the learning function.
    this array is a static array defined in the learning function.
    *NoOfOutParams points to a integer value that contains the number
    of output parameters from the current learning function.
    Returns an error code if memory allocation has failed or if
    the parameters are invalid.

    Returns error code of the learning function.
REMEMBER:  The backpropagation learning function takes the learning
           parameter from parameterInArray[ 0 ]. parameterOutArray[ 0 ]
           returns the current net error.

NOTE: Patterns must be loaded before calling this function.
*/

krui_err  krui_learnSinglePattern( int  pattern_no, float   *parameterInArray,
                                   int NoOfInParams,
                                   float  * *parameterOutArray, int *NoOfOutParams );
/*  Same as krui_learnAllPatterns( ... ) but learns only the current
    pattern pair.

NOTE: Patterns must be loaded before calling this function.
*/

krui_err  krui_setPatternNo( int  patter_no );
/*  Sets the current pattern.
    Returns a error code if pattern number is invalid.

NOTE: Patterns must be loaded before calling this function.
*/

krui_err  krui_deletePattern( void );
/*  Deletes the current pattern.
*/

krui_err  krui_modifyPattern( void );
/*  Modifies the current pattern.
*/

krui_err  krui_showPattern( int  mode );
/*  According to the mode krui_showPattern stores the current
    Pattern into the units activation (and/or output) values.
    The modes are:
      - OUTPUT_NOTHING
         store input pattern into input units activations
      - OUTPUT_ACT
         store input pattern into input units activations and
         store output pattern into output units activations
      - OUTPUT_OUT
         store input pattern into input units activations,
         store output pattern into output units activations and
         update output units output

NOTE: See include file glob_typ.h for mode constants.
*/

krui_err  krui_newPattern( void );
/*  Creates a new pattern pair.
    A pattern pair can be created by modifying the activation
    value of the input/output units.
    Returns error code if memory is insufficent or no. of
    input/output units is incompatible, 0 otherwise.

NOTE: krui_newPattern switches pattern shuffeling off.
      For shuffeling the new pattern pairs call
        krui_newPattern(...)
        krui_shufflePattern( TRUE )
*/


int  krui_getNoOfPatterns( void );
/*  Returns the no. of available pattern pairs.
*/


void  krui_deleteAllPatterns( void );
/*  Release previously defined patterns from memory.
    Call krui_releasePatterns() if you want to create totally
    new patterns with krui_newPattern().
*/

krui_err  krui_shufflePatterns( bool  on_or_off );
/*  Shuffle pattern pairs by using pseudo random generator.
    Returns error code if memory allocation fails.
    Shuffeling of patterns is used by krui_learnAllPatterns(...).
    krui_shufflePatterns( TRUE ) switches shuffeling of patterns
    on, krui_shufflePatterns( FALSE ) switches shuffeling of
    patterns off.
    The default presetting is krui_shufflePatterns( FALSE ).

NOTE:  See also krui_setSeedNo( seed )
*/


/*#################################################

GROUP: I/O Functions

#################################################*/

krui_err  krui_loadNet( char  *filename, char  * *netname );
/*  Load a network file and create a network.
    Returns the name of the net or "UNTITLED" if unknown.
    Returns error code if an error occured during
    loading/memory allocation, or 0 otherwise.
*/

krui_err  krui_saveNet( char  *filename, char  *netname);
/*  Save a network.
    If netname is a NULL pointer, the net will get the name "UNTITLED"
    Returns error code if an error occured, or 0 otherwise.
*/

/*#################################################

GROUP: Functions for reading/searching the symbol table
       The symbol table holds the names and types of all symbols
       (unit and site symbols).

#################################################*/

bool  krui_getFirstSymbolTableEntry( char * *symbol_name, int  *symbol_type );
/*  Returns the first symbol/symbol type entry in the
    symbol table. Returns true if this entry is available,
    false otherwise.
*/

bool  krui_getNextSymbolTableEntry( char * *symbol_name, int  *symbol_type );
/*  Returns the next symbol/symbol type entry in the
    symbol table. Returns true if another entry is available,
    false otherwise.

Example:  To get all symbols in the symbol table
          if (krui_getFirstSymbolTableEntry( &symbol, &symtype))
            do
              {
                .
                .
                .
              }
            while (krui_getNextSymbolTableEntry( &symbol, &symtype));
*/


bool  krui_symbolSearch( char * symbol, int  symbol_type);
/*  Searches the symbol table for a given symbol and
    symbol type (unit name symbol, site name symbol,
    functionality unit name symbol)
    Returns true, if the symbol exists.
*/

/*#################################################

GROUP: Miscellanous

#################################################*/


char  *krui_getVersion( void );
/*  Returns the current Version of the SNNS-Kernel.
*/

void  krui_getNetInfo( int  *no_of_sites, int  *no_of_links,
                       int  *no_of_STable_entries, int  *no_of_FTable_entries );
/*  Returns miscellanous information about the current network.
*/

void  krui_getUnitDefaults( FlintType  *act, FlintType  *bias, int  *st,
                            int  *subnet_no, int  *layer_no,
                            char * *act_func, char  * *out_func );
/*  Returns Information about the unit default settings.
    For more information about default settings see krui_createDefaultUnit() and
    krui_createFTypeUnit( .. ).
*/

krui_err  krui_setUnitDefaults( FlintTypeParam  act, FlintTypeParam  bias, int  st,
                                int  subnet_no, int  layer_no,
                                char  *act_func, char  *out_func );
/*  Changes the unit default settings.
    For more information about default settings see krui_createDefaultUnit() and
    krui_createFTypeUnit( .. ).

    Returns error code if
     - activation/output function is invalid
     - Topologic type is invalid
    0 otherwise.
*/



void  krui_setSeedNo( long seed );
/*  Initialize the pseudo random generator.
    0 as argument reinitializes the random generator.
*/


int  krui_getNoOfInputUnits( void );
/*  Returns no. of input units
*/

int  krui_getNoOfOutputUnits( void );
/*  returns no. of output units
*/

int  krui_getNoOfTTypeUnits( int  UnitTType );
/*  returns the no. of units of the specified topologic type
    (i.e. Input, Hidden, Output, Dual or Special units)
*/

void  krui_resetNet( void );
/*  Reset the network by changeing the unit activation
    to the initial activation value.
*/
char  *krui_error( int  error_code );
/*  Returns an error message, depending on the error code.
    If a function returns an error code use krui_error to
    get the message.

Available error messages are:

"Invalid error code",
"Insufficient memory",
"Invalid unit number",
"Invalid unit output function",
"Invalid unit activation function",
"Invalid site function",
"Creation of sites isn't permitted because unit has direct input links",
"Creation of a link isn't permitted because there exists already a link between these units",
"Memory allocation failed during critical operation. Have lost some pointers, but consistency of the network is guaranteed",
"Ftype name isn't definite",
"Current Ftype entry isn't defined",
"Invalid copy mode",
"Current unit doesn't have sites",
"Can't update unit because unit is frozen",
"Redefinition of site name isn't permitted (site name already exists)",
"Site name isn't defined",
"This is not a 3D-Kernel",
"This unit has already a site with this name",
"Can't delete site table entry because site is in use",
"Current Ftype site isn't defined",
"Ftype symbol isn't defined",
"Physical I/O error",
"Creation of output file failed (line length limit exceeded)",
"The network has not enough layers: ",
"No Units defined",
"Unexpected EOF",
"Line length exceeded",
"Incompatible file format",
"Can't open file",
"Syntax error at line",
"Memory allocation error 1",
"Topologic type invalid",
"Symbol pattern invalid (must start with a letter)",
"Current unit doesn't have a site with this name",
"No hidden units defined",
"Network contains cycle(s): ",
"Network contains dead unit(s): ",
"Pattern file contains not the same no. of input units as the network",
"Pattern file contains not the same no. of output units as the network",
"No. of input units have changed",
"No. of output units have changed",
"No input units defined",
"No output units defined",
"No patterns defined",
"In-Core patterns incompatible with current network (remove In-Core patterns before loading a new network)",
"Invalid pattern number",
"Invalid learning function",
"Invalid parameters",
"Invalid update function",
"Invalid initialisation function",
"Derivation function of the activation function doesn't exist",
"Input unit(s) with input connection(s) to other units: ",
"Output unit(s) with output connection(s) to other units: ",
"Invalid topological sorting mode",
"Learning function doesn't support sites",
"Sites are not supported",
"This isn't a MasPar Kernel",
"Connection(s) between unit(s) in non-neighbour layers are not supported: ",
"The network has too much layers: ",
"The network layers aren't fully connected",
"This operation isn't allowed in the superscalar kernel mode",
"Change of network type isn't possible in superscalar kernel mode",
"No current link defined",
"No current unit defined",
"Current unit doesn't have any inputs",
"Invalid parameter in topologic definition section",
"Creation of link between these units isn't permitted",
"MasPar don't respond",
"This function isn't implemented yet",
"Kernel isn't in parallel mode",
"MasPar ran out of memory",
"MasPar communication error",
"MasPar ran out of processors",
"Missing default function (check function table)",
"MasPar kernel doesn't support multiple unit output functions",
"MasPar kernel doesn't support multiple unit activation functions",
"The depth of the network doesn't fit to the learning function"


*/


/*#################################################

GROUP: Functions for memory management

#################################################*/

krui_err  krui_allocateUnits( int  number );
/*  Allocates a given number of units, additional units
    may allocated by calling this function again.
    This function is called automatically if the user
    construct more units than have been allocated before, but
    it is recommended to use this function if a large amount
    of units is needed (the UNIX System can manage system resources
    much better, if the amount of memory used for the network is
    approximately known before the network is created).

    Returns error code if memory allocation fails, 0 otherwise.

NOTE: If krui_create_unit has been called before using this
      function, at least <UNIT_BLOCK> numbers of units have been
      allocated.
      See "kr_def.h" for more information about memory allocation
      block sizes.
*/

void  krui_getMemoryManagerInfo( int  *unit_bytes, int  *site_bytes, int  *link_bytes,
                                 int  *NTable_bytes, int  *STable_bytes, int  *FTable_bytes );
/*  Returns the number of ALLOCATED (not the number of USED) bytes
    for the units, sites, name-table and site-table.
*/

void  krui_deleteNet( void );
/*  Delete network, names and unit types.
    Frees all memory used for the network.

NOTE: If krui_loadNet is called more then once,  krui_deleteNet will
      be called by krui_loadNet, because the kernel have to free the
      memory used for the old network.
      It is recommended (but not neccessary) to call krui_deleteNet
      before terminating  program.
*/


/* #############################################################
 
  Functions for the extern kernels

############################################################# */

krui_err  krui_setSpecialNetworkType( int  net_type );
/*  Sets the topologic type of the current network.
    Returns an error if the topologic type of the current network  
    doesn't fit to this type.
    Topologic types are:
      - NET_TYPE_GENERAL
        general purpose network type with no limitations
      - NET_TYPE_FF1
        feedforward network with fully connected units in
        neighbour layers
*/

int  krui_getSpecialNetworkType( void );
/*  Returns the special topologic type of the current network, if set.
*/

int krui_initInversion(void);
void  krui_inv_forwardPass(struct UnitList *inputs);
double krui_inv_backwardPass(float learn, float delta_max, int *err_units, 
			     float ratio, struct UnitList *inputs,
			     struct UnitList *outputs);

#ifdef MASPAR_KERNEL

/* #############################################################
 
  Functions for the parallel kernel

############################################################# */

krui_err  krui_MasPar( int  mode );
/*  Connects and Disconnects the MasPar.
    The mode switches are:  MASPAR_CONNECT, MASPAR_DISCONNECT and
    MASPAR_NOT_AVAILABLE.
*/


krui_err  krui_getMasParStatus( void );
/*  Returns the status of the MasPar.
*/

krui_err  krui_MasParBenchmark( int  func_type, int cycles, float  *result );
/*  Perform benchmark tests
*/

#endif


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_typ.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2
 
  PURPOSE        : SNNS-Kernel: Datatypes and Constants for Internal Useage
  NOTES          : 12.02.90
 
  AUTHOR         : Niels Mache
  DATE           : 
 
  CHANGED BY     : Sven Doering, Michael Vogt
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/04/08 09:26:39 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#include "glob_typ.h"	 /*  Global datatypes and constants  */
#include "ext_typ.h"     /*  Types and constants for extern kernel */

#ifndef KR_TYPES
#define KR_TYPES


/*#################################################

GROUP: Type Definitions

#################################################*/

#ifdef __BORLANDC__

#ifndef strdup
#define  strdup  bsd_strdup
#endif

/*
#define  int long
*/

#endif

/*  type definition of input/output pattern array
*/
typedef  FlintType     *Patterns;

/*  type definition of integer array for shuffeling input/output patterns
*/
typedef  int    *PatternNumbers;

struct Unit ; /* Dummy declaration */
struct Site ; /* Dummy declaration */
typedef  FlintType   (* SiteFuncPtr) (struct Site *);
typedef  FlintType   (* ActFuncPtr)  (struct Unit *);
typedef  FlintType   (* ActDerivFuncPtr) (struct Unit *);


typedef  FlintType   (* OutFuncPtr)  (FlintType);
typedef  krui_err    (* LearnFuncPtr) (int, int, float *, int, float * *, int *);
typedef  krui_err    (* UpdateFuncPtr) (float *, int);
typedef  krui_err    (* InitFuncPtr) (float *, int);

typedef  krui_err    (* RemapFuncPtr) (float *pat_data, int pat_size, 
				       float *remap_params, int no_of_remap_params);

/*#################################################

GROUP: Type Definition of Name Table

#################################################*/

/*  Name table structure
*/
struct   NameTable  {
  union  {
    char               *symbol;             /*  stores symbol name  */
    struct  NameTable  *next;               /*  stores name-table block ptr
                                                and free name-table entry   */
  }  Entry;

  unsigned short       sym_type,            /*  stores type of entry   */
                       ref_count;           /*  stores no. of references to
                                                this symbol (If more then MAXSHORT
                                                references are detected the symbol will
                                                released only by krm_releaseMem()
                                            */
};



/*#################################################

GROUP: Type Definition of Site-Table

#################################################*/

/*  Site table structure
*/
struct   SiteTable  {
  union  {
    struct NameTable	*site_name;  /*  stores the site symbol (it's not a direkt
					 pointer to the symbol, but a pointer to the
					 name-table entry */
    struct SiteTable	*next;	     /*  stores next site-table block ptr
					 and free site-table entries */
  }  Entry;

  SiteFuncPtr   site_func;      /*  stores site function    */
};

#ifndef Py_PYTHON_H
/* dummy declaration */

typedef struct PyObject PyObject;

#endif



/*#################################################

GROUP: Type definition of unit functionality table

#################################################*/

/*  Unit prototype structure
*/
struct   FtypeUnitStruct  {
  struct  NameTable     *Ftype_symbol;

  OutFuncPtr      out_func;
  ActFuncPtr      act_func;
  ActDerivFuncPtr  act_deriv_func;
  ActDerivFuncPtr  act_2_deriv_func;
  PyObject *      python_out_func;
  PyObject *      python_act_func;
  PyObject *      python_act_deriv_func;
  PyObject *      python_act_2_deriv_func;


  struct Site              *sites;

  struct FtypeUnitStruct   *next;
  struct FtypeUnitStruct   *prev;
};



/*#################################################

GROUP: Unit/Site/Link type definitions

#################################################*/

/*m.r.*/
/* BPTT: maximum number additional unit activity copies back in time,
 (= number of non-input layers for backpropagation)
*/
#define MAX_BPTT_BACKSTEP 10

/*  Link structure
*/
struct   Link  {
  struct Unit   *to;      /*  points to the source unit  */
  FlintType     weight;   /*  link weight  */

  FlintType     value_a,  /*  general purpose elements for the  */
                value_b,  /*  learning functions  */
                value_c;

  struct Link   *next;    /*  next link  */
};


/*  Site structure
*/
struct   Site  {
  struct Link   *links;   /*  input links of the site  */
  struct SiteTable      *site_table;  /*  : site name and function  */
  struct Site   *next;    /*  next site  */
};

/*  Type of unit flags
*/
typedef  unsigned short  FlagWord;

/*  Unit structure
*/

struct   Unit  {
  /*  output MUST be the first element in unit structure !
      (access is faster if functions can assume that <output>
      is the FIRST element of the unit structure)
  */
  union  {
    FlintType     output;       /*  unit's output  */
    int           nextFreeUnit; /*  used by the unit memory manager  */
  } Out;

  FlagWord        flags;        /*  unit flags  */

  int             lun;          /*  logical unit number  */
  int             lln;          /*  logical layer number  */

  struct  FtypeUnitStruct   *Ftype_entry;  /*  pointer to the unit's prototype  */

  union  {
    FlintType     flint_no;     /*  used by backpropagation: holds errors from successors   */
    int           int_no;       /*  used by layer sorting: holds the layer no of the unit   */
    char         *ptr;          /*  used for garbage collection  */
  }  Aux;   /*  general purpose use  */

  struct
    {
    struct Unit   **my_topo_ptr;   /* pointer to coresponding entry in */
                                   /* topo_ptr_array */
    int           target_offset;   /* offset to prototype target unit via */
                                   /* topo_ptr_array */
    int           source_offset;   /* offset to logioal source unit via */
                                   /* prototype target and topo_ptr_array */
    int           td_connect_typ;  /* recept. field or not */
    }  TD;  /* TimeDelay net datas */

  FlintType	  act,		/*  current activation	*/
		  i_act,	/*  initial activation	*/
		  bias; 	/*  bias  */

  FlintType       value_a,      /*  general purpose elements for the  */
                  value_b,      /*  learning functions  */
                  value_c;

  unsigned long int usr_flags;  /* long flag word for general purpose usage
                                   within learning functions */

/*m.r.*/
  FlintType       olddelta,     /* BPTT-section: delta value at time t */
                  newdelta;     /* delta value at time t-1 
				   (deltas are propagated back in time )*/
  FlintType       actbuf[MAX_BPTT_BACKSTEP]; /*outacts at previous time steps*/
/*m.r.*/

  OutFuncPtr	  out_func;	    /*	output function  */
  ActFuncPtr	  act_func;	    /*	activation function */
  ActDerivFuncPtr  act_deriv_func;  /*	derivation act. function */
  ActDerivFuncPtr act_2_deriv_func; /*  second derivation act. function */

  PyObject       *python_out_func; /* Same as above, only if Python */
  PyObject       *python_act_func; /* functions should be used */
  PyObject       *python_act_deriv_func;
  PyObject       *python_act_2_deriv_func;

  char           *unit_name;    /*  unit name */
  short           subnet_no;    /*  subnet no.  */
  unsigned short  layer_no;     /*  display layer (bitfield)  */

  struct PosType  unit_pos;     /*  unit position  */


  /*  unit's center position  */
  struct PositionVector unit_center_pos[ NO_OF_UNIT_CENTER_POS ];


  struct Site     *sites;           /*  points to unit's input links or sites  */
};



/*#################################################

GROUP: Var types used by kernel functions

#################################################*/

/*  stucture for storing the error codes and messages of the topologic sorting
    and network checking functions
*/
struct TopologicMessages  {
  int  error_code,         /*  stores the internal error code  */
       no_of_cycles,       /*  no. of detected cycles in the network  */
       no_of_dead_units,   /*  no. of dead units in the network  */
       no_of_layers,       /*  no. of layers of the network */
       dest_error_unit,    /*  first error unit (destination unit)  */
       src_error_unit;     /*  first error unit (source unit)  */
  char name[20];           /*  a chararcter string that can be passed */
};


/*#################################################

GROUP: Link/Site/Unit and Unit Ptrs Array Type Definitions

#################################################*/

/*  definition of link array
*/
typedef  struct Link    *LinkArray;

/*  definition of site array
*/
typedef  struct Site    *SiteArray;

/*  definition of pointer array for topological sorting
*/
typedef  struct Unit *  *TopoPtrArray;

/*  definition of unit array
*/
typedef  struct Unit    *UnitArray;

/*  Definition of Name Table Array
*/
typedef  struct NameTable  *NTableArray;

/*  Definition of Site Name Table Array
*/
typedef  struct SiteTable  *STableArray;




struct   TransTable  {
  short  z,
         x,
	 y;
};



/*#################################################

GROUP: Structure Size Constants

#################################################*/

#define   UNIT_SIZE             sizeof (struct Unit)
#define   SITE_SIZE             sizeof (struct Site)
#define   LINK_SIZE             sizeof (struct Link)

#define   PATTERN_SIZE          sizeof (FlintType)
#define   PATTERN_NO_SIZE       sizeof (int)

#define   TOPO_PTR_SIZE         sizeof (struct Unit *)

#define   NTABLE_SIZE           sizeof (struct NameTable)

#define   STABLE_SIZE           sizeof (struct SiteTable)

#define   FTYPE_UNIT_SIZE       sizeof (struct FtypeUnitStruct)


#endif

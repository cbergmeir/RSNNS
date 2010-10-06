/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/glob_typ.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2
 
  PURPOSE        : SNNS-Kernel: Global Datatypes and Constants
  NOTES          : For User's Application Programs, User-Interface and Kernel
 
  AUTHOR         : Niels Mache
  DATE           : 14.02.90 
 
  CHANGED BY     : Michael Vogt, Guenter Mamier, Sven Doering, Christine Bagdi
  RCS VERSION    : $Revision: 2.31 $
  LAST CHANGE    : $Date: 1998/04/20 11:54:49 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#ifndef KR_GLOBAL_TYPES
#define KR_GLOBAL_TYPES

#include <stdio.h>

#ifdef ultrix
   /* DEC's pseudo ansi C-compiler doesn't understand const  */
#  define const
#endif


#ifndef __BORLANDC__



/*  define MASPAR_KERNEL to build the parallel MasPar Kernel
*/
/*
#define MASPAR_KERNEL
*/

#endif

#ifdef MASPAR_KERNEL

/*  define MASPAR_KERNEL_EMULATION to build a (sequentiel) kernel that simulates
    the MasPar interface
*/
/*
#define  MASPAR_KERNEL_EMULATION
*/

/*
#define  KERNEL_VERBOSE
*/

#endif


/* ******************************************   */

#ifndef TRUE
#define     TRUE    1
#endif
#ifndef FALSE
#define     FALSE   0
#endif

/*#################################################

GROUP: Public Constants

#################################################*/

/*  Kernel user interface error codes
*/
#define  KRERR_NO_ERROR         0
#define  KRERR_INSUFFICIENT_MEM -1
#define  KRERR_UNIT_NO          -2
#define  KRERR_OUTFUNC          -3
#define  KRERR_ACTFUNC          -4
#define  KRERR_SITEFUNC         -5
#define  KRERR_CREATE_SITE      -6
#define  KRERR_ALREADY_CONNECTED  -7
#define  KRERR_CRITICAL_MALLOC  -8
#define  KRERR_FTYPE_NAME       -9
#define  KRERR_FTYPE_ENTRY      -10
#define  KRERR_COPYMODE         -11
#define  KRERR_NO_SITES         -12
#define  KRERR_FROZEN           -13
#define  KRERR_REDEF_SITE_NAME  -14
#define  KRERR_UNDEF_SITE_NAME  -15
#define  KRERR_NOT_3D           -16
#define  KRERR_DUPLICATED_SITE  -17
#define  KRERR_INUSE_SITE       -18
#define  KRERR_FTYPE_SITE       -19
#define  KRERR_FTYPE_SYMBOL     -20
#define  KRERR_IO               -21
#define  KRERR_SAVE_LINE_LEN    -22
#define  KRERR_FEW_LAYERS       -23
#define  KRERR_NO_UNITS         -24
#define  KRERR_EOF              -25
#define  KRERR_LINE_LENGTH      -26
#define  KRERR_FILE_FORMAT      -27
#define  KRERR_FILE_OPEN        -28
#define  KRERR_FILE_SYNTAX      -29
#define  KRERR_MALLOC1          -30
#define  KRERR_TTYPE            -31
#define  KRERR_SYMBOL           -32
#define  KRERR_NO_SUCH_SITE     -33
#define  KRERR_NO_HIDDEN_UNITS  -34
#define  KRERR_CYCLES           -35
#define  KRERR_DEAD_UNITS       -36
#define  KRERR_INPUT_PATTERNS   -37
#define  KRERR_OUTPUT_PATTERNS  -38
#define  KRERR_CHANGED_I_UNITS  -39
#define  KRERR_CHANGED_O_UNITS  -40
#define  KRERR_NO_INPUT_UNITS   -41
#define  KRERR_NO_OUTPUT_UNITS  -42
#define  KRERR_NO_PATTERNS      -43
#define  KRERR_INCORE_PATTERNS  -44
#define  KRERR_PATTERN_NO       -45
#define  KRERR_LEARNING_FUNC    -46
#define  KRERR_PARAMETERS       -47
#define  KRERR_UPDATE_FUNC      -48
#define  KRERR_INIT_FUNC        -49
#define  KRERR_DERIV_FUNC       -50
#define  KRERR_I_UNITS_CONNECT	-51
#define  KRERR_O_UNITS_CONNECT	-52
#define  KRERR_TOPOMODE 	-53
#define  KRERR_LEARNING_SITES   -54
#define  KRERR_SITES_NO_SUPPORT -55
#define  KRERR_NO_MASPAR_KERNEL -56
#define  KRERR_NOT_NEIGHBOUR_LAYER -57
#define  KRERR_MUCH_LAYERS      -58
#define  KRERR_NOT_FULLY_CONNECTED -59
#define  KRERR_MODE_FF1_INVALID_OP -60
#define  KRERR_NET_TRANSFORM    -61
#define  KRERR_NO_CURRENT_LINK	-62
#define  KRERR_NO_CURRENT_UNIT	-63
#define  KRERR_UNIT_NO_INPUTS	-64
#define  KRERR_TOPO_DEFINITION	-65
#define  KRERR_BAD_CONNECTION   -66
#define  KRERR_MASPAR_NO_RESPOND -67
#define  KRERR_NOT_IMPEMENTED_YET -68
#define  KRERR_NOT_PARALLEL_MODE  -69
#define  KRERR_MASPAR_OUT_OF_PE_MEM -70
#define  KRERR_MASPAR_COMMUNICATE -71
#define  KRERR_MASPAR_OUT_OF_PE   -72
#define  KRERR_MISSING_DEFAULT_FUNC  -73
#define  KRERR_MASPAR_MULTI_OUT_FUNC -74
#define  KRERR_MASPAR_MULTI_ACT_FUNC -75
#define  KRERR_NET_DEPTH        -76
#define  KRERR_NO_OF_UNITS_IN_LAYER         -77
#define  KRERR_UNIT_MISSING                 -78
#define  KRERR_UNDETERMINED_UNIT            -79
#define  KRERR_ACT_FUNC                     -80
#define  KRERR_OUT_FUNC                     -81
#define  KRERR_SITE_FUNC                    -82
#define  KRERR_UNEXPECTED_SITES             -83
#define  KRERR_UNEXPECTED_DIRECT_INPUTS     -84
#define  KRERR_SITE_MISSING                 -85
#define  KRERR_UNEXPECTED_LINK              -86
#define  KRERR_LINK_MISSING                 -87
#define  KRERR_LINK_TO_WRONG_SITE           -88
#define  KRERR_TOPOLOGY                     -89
#define  KRERR_PARAM_BETA                   -90
#define  KRERR_CC_ERROR1                    -91
#define  KRERR_CC_ERROR2                    -92
#define  KRERR_CC_ERROR3                    -93
#define  KRERR_CC_ERROR4                    -94
#define  KRERR_CC_ERROR5                    -95
#define  KRERR_CC_ERROR6                    -96
#define  KRERR_CC_ERROR7                    -97
#define  KRERR_CC_ERROR8                    -98
#define  KRERR_CC_ERROR9                    -99
#define  KRERR_CC_ERROR10                  -100
#define  KRERR_CC_ERROR11                  -101
#define  DLVQ_ERROR1                       -102
#define  DLVQ_ERROR2                       -103
#define  DLVQ_ERROR3                       -104
#define  DLVQ_ERROR4                       -105
#define  DLVQ_ERROR5                       -106
#define  KRERR_NP_NO_MORE_ENTRIES          -107
#define  KRERR_NP_NO_SUCH_PATTERN_SET      -108
#define  KRERR_NP_NO_CURRENT_PATTERN       -109
#define  KRERR_NP_DIMENSION                -110
#define  KRERR_NP_NO_SUCH_PATTERN          -111
#define  KRERR_NP_NO_CURRENT_PATTERN_SET   -112
#define  KRERR_NP_DOES_NOT_FIT             -113
#define  KRERR_NP_NO_TRAIN_SCHEME          -114
#define  KRERR_NP_NO_OUTPUT_PATTERN        -115
#define  KRERR_NP_INCOMPATIBLE_NEW         -116
#define  KRERR_IP_ISNOTINITED              -117
#define  KRERR_IP_ISINITED                 -118
#define  KRERR_IP_SENDMSGFAILED            -119
#define  KRERR_IP_MSGSYNTAX                -120
#define  KRERR_IP_UNKNOWENCMD              -121
#define  KRERR_IP_LESSPAT                  -122
#define  KRERR_IP_WEIGHTUPDATE             -123
#define  KRERR_IP_LEARNFNOTPAR             -124
/* codes 125 to 129 belonged to delted rpc sources */
#define  KRERR_NP_WORKAROUND               -130
#define  DDA_PARAM_ONE                     -131
#define  DDA_PARAM_TWO                     -132
#define  DDA_PARAM_THREE                   -133
#define  DDA_DESIRED_CLASS                 -134
#define  DDA_CONN_POINTER                  -135
#define  DDA_SHORTCUTS                     -136
#define  DDA_INPUT_ACT_FUNC                -137
#define  DDA_HIDDEN_ACT_FUNC               -138
#define  DDA_OUTPUT_ACT_FUNC               -139
#define  KRERR_CC_INVALID_ADD_PARAMETERS   -140
#define  KRERR_UPS_ACT_NOT_THRESHOLD       -141
#define  KRERR_UPS_LEARN_NOT_BACKPROP      -142
#define  KRERR_SINGLE_CLASS                -143
#define  KRERR_REMAP_FUNC                  -144
#define  KRERR_NO_CLASSES                  -145
#define  KRERR_ILL_CLASS_DISTRIB           -146
#define  KRERR_CANT_NORM                   -147

/*  Function types
*/
#define  OUT_FUNC        1
#define  ACT_FUNC        2
#define  SITE_FUNC       3
#define  LEARN_FUNC      4
#define  UPDATE_FUNC     5
#define  INIT_FUNC       6
#define  ACT_DERIV_FUNC  7
#define  JOG_WEIGHT_FUNC 8
#define  ACT_2_DERIV_FUNC 9
#define  PRUNING_FUNC    10
#define  TEST_FUNC       11  
#define  REMAP_FUNC       12  

#define  FUNC_TYPE_MASK  0x00ff
#define  DEFAULT_FUNC    0x0100
#define  BENCH_FUNC      0x0200
#define  FF_LEARN_FUNC   0x0400          /* used by pruning algorithms */



/*   entry types for name table
*/
#define  UNUSED_SYM         0
#define  UNIT_SYM           1
#define  SITE_SYM           2
#define  FTYPE_UNIT_SYM     3

/* constants for the network-analyzer tool
*/

#define MAX_ARRAY_SIZE 5000
#define NA_ERROR_LIN      1
#define NA_ERROR_SQR      2
#define NA_ERROR_SU       3

/*  Copy Modes
*/
#define  INPUTS_AND_OUTPUTS     1
#define  ONLY_INPUTS            2
#define  ONLY_OUTPUTS           3
#define  ONLY_UNIT              4


/*  Topological Unit Types
*/
#define  UNKNOWN    0
#define  INPUT      1
#define  OUTPUT     2
#define  HIDDEN     3
#define  DUAL       4
#define  SPECIAL    5
#define  SPECIAL_I  6
#define  SPECIAL_O  7
#define  SPECIAL_H  8
#define  SPECIAL_D     9
#define  SPECIAL_X    10 /* Only used for toggle special flag */
#define  N_SPECIAL_X  11 /* Only used for toggle special flag */

/*  Unit Input Types
*/
#define  NO_INPUTS      0
#define  SITES          1
#define  DIRECT_LINKS   2

/*  Pattern update modes
*/
#define  OUTPUT_NOTHING  1
#define  OUTPUT_ACT      2
#define  OUTPUT_OUT      3

/*  Maximum number of pattern sets
*/
#define  NO_OF_PAT_SETS 5

/*  Maximum Dimensions of Pattern
*/
#define MAX_NO_OF_VAR_I_DIM     2  /* maximum number of variable input       */
				   /* dimensions                             */
#define MAX_NO_OF_VAR_O_DIM     2  /* maximum number of variable             */
				   /* output dimensions                      */
#define MAX_NO_OF_VAR_DIM       2  /* maximum of MAX_NO_OF_VAR_I_DIM         */
				   /* MAX_NO_OF_VAR_O_DIM                    */

/*  Maximum no. of learning parameters for learning functions
*/
#define     NO_OF_LEARN_PARAMS	28

/*  Maximum no. of results from the learning functions
*/
#define     NO_OF_RESULTS	10

/*  Maximum no. of stored error from the learning functions
*/
#define     NO_OF_STORED_ERRORS	50

/*  Maximum no. of init parameters for initialization functions
*/
#define     NO_OF_INIT_PARAMS   10

/*  Maximum no. of update parameters for update functions
*/
#define     NO_OF_UPDATE_PARAMS   10

/*  Maximum no. of parameters for pattern remap functions
*/
#define     NO_OF_REMAP_PARAMS   5




/*  Translation Table op's
*/
#define     OP_TRANSTABLE_GET           1
#define     OP_TRANSTABLE_SET           2
#define     OP_TRANSTABLE_CLEAR         3



/*#################################################

GROUP: Global Var's

#################################################*/

/*  Kernel Interface Error Code
*/
#ifndef SNNS_KERNEL
extern int  KernelErrorCode;
#endif

/*#################################################

GROUP: Global Datatypes

#################################################*/

typedef int     bool;
typedef float   FlintType;
typedef int     krui_err;


typedef struct
{
    double parameter[NO_OF_LEARN_PARAMS]; 
    /*  UI_NO_OF_CASCADE_PARAMS is declared in ui.h the learn parameter     */ 
    int noOfParameters;                      /* number of learn parameters  */
    char **parameterDescription;             /* not supported yet           */
    double result[NO_OF_RESULTS];            /* the result of training      */
    int noOfResults;                         /* number of results           */
    int firstPattern;                        /* the first pattern to train  */
    int lastPattern;                         /* the last pattern to train   */
    int noOfEpochs;                          /* number of epochs to learn   */
    double learnErrors[NO_OF_STORED_ERRORS]; /* stores the Error            */
    int atEpoch[NO_OF_STORED_ERRORS];        /* the epoch for the error     */
    int noOfErrors;                          /* the number of stored errrors*/
    float netError;                          /* the last net error          */
    int   lastEpoch;                         /* the last epoch learned      */
    bool interrupted;                        /* marks if all epochs learned */
} NetLearnParameters;

typedef struct 
{
    int number_of_pattern;     /* the number of pattern (pairs) in           */
                               /* this set                                   */
    int virtual_no_of_pattern; /* the number of pattern (pairs) if           */
                               /* class_distrib_active == TRUE               */
    bool output_present;       /* TRUE if output pattern present             */
    bool fixed_fixsizes;       /* the fixsizes of all pattern are equal      */
    int in_fixsize;            /* if fixed_fixsizes TRUE, fixsize of         */
                               /* the input pattern, else -1                 */
    int out_fixsize;           /* if fixed_fixsizes TRUE, fixsize of         */
                               /* the output pattern, else -1                */
    int in_number_of_dims;     /* number of variable input dimensions        */
    int out_number_of_dims;    /* number of variable output dimensions       */
    int in_max_dim_sizes[MAX_NO_OF_VAR_I_DIM];  /* maximum values of         */
                                                /* the input dimension       */
    int out_max_dim_sizes[MAX_NO_OF_VAR_O_DIM]; /* maximum values of the     */
                                                /* output dimension          */
    int in_min_dim_sizes[MAX_NO_OF_VAR_I_DIM];  /* minimum values of the     */
                                                /* input dimensions          */
    int out_min_dim_sizes[MAX_NO_OF_VAR_O_DIM]; /* minimum values of the     */
                                                /* output dimensions         */

    int classes;               /* number of pattern classes if > 0           */
    char **class_names;        /* array of <classes> class names, ordered    */
    bool class_distrib_active; /* class amount redistribution is active      */
    int *class_redistribution; /* amounts for redistrib. <classes> entries   */
    
    char *remap_function;      /* name of remap function or NULL             */
    float remap_params[NO_OF_REMAP_PARAMS];     /* remap function parameters */
    int no_of_remap_params;    /* number of remap function parameters        */
} pattern_set_info;

typedef struct
{
    int input_dim;             /* number of variable input dimensions        */
    int input_dim_sizes[MAX_NO_OF_VAR_I_DIM];  /* actual sizes of the input  */
                                               /* dimensions                 */
    int input_fixsize;         /* size of the fixed part of the in pattern   */
                               /* or 0 if no input pattern present           */
    int output_dim;            /* number of variable output dimensions       */
    int output_dim_sizes[MAX_NO_OF_VAR_O_DIM]; /* actual sizes of the output */
                                               /* dimensions                 */
    int output_fixsize;        /* size of the fixed part of the out pattern  */
                               /* or 0 if no output pattern present          */
    int my_class;              /* class index of this pattern, if classes    */
                               /* available, -1 otherwise                    */
} pattern_descriptor;

#ifdef __STDC__
typedef double  FlintTypeParam;
#else
typedef float  FlintTypeParam;
#endif



/*#################################################

GROUP: 3D-Kernel Definitions

#################################################*/

#define HAVE_QSORT

struct   PosType  {
  short  x,
	 y,
	 z;
};



#define  NO_OF_UNIT_CENTER_POS  1

struct   PositionVector  {
  float  x,
	 y,
	 z,
	 w;
};


/* List of Inversion Units */
struct UnitList{
    int             no;
    FlintType       act;
    FlintType       i_act;
    FlintType       im_act;
    struct PosType  gridPos;
    struct UnitList *next;
    struct UnitList *prev;
};

/*#################################################

GROUP: Constants for the parallel kernel

#################################################*/

/*  Topologic network types
*/
#define  NET_TYPE_GENERAL  0   /*  normal network presentation, no limitations  */
#define  NET_TYPE_FF1      1   /*  feedforward net with limitations  */

/*  MasPar modes
*/
#define  MASPAR_DISCONNECT  0
#define  MASPAR_CONNECT  1


#endif

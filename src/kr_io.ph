/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_io.ph,v $
  SHORTNAME      : kr_io.ph
  SNNS VERSION   : 4.2
 
  PURPOSE        : SNNS-Kernel User Interface File I/O Function Prototypes 
  NOTES          :
 
  AUTHOR         : Niels Mache
  DATE           : 28.05.90
 
  CHANGED BY     : Sven Doering, Michael Vogt
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:49 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#ifndef _KR_IO_DEFINED_
#define  _KR_IO_DEFINED_

/* begin global definition section */

/*  write complete network to disk
*/
krui_err  krio_saveNet( char  *filename, char  *netname );

/*  load network form disk
*/
krui_err krio_loadNet( char  *filename, char  **netname, char  **netfile_version );

/*  loading pattern file from disk
*/
krui_err krio_loadPatterns( char  *filename );

/*  write patterns to disk
*/
krui_err krio_savePatterns( char  *filename );

krui_err  krio_saveResult( char *filename,
                           bool create,
                           int  startpattern,
                           int  endpattern,
                           bool includeinput,
                           bool includeoutput,
			   float *Update_param_array, 
			   int NoOfUpdateParam);



extern struct TransTable  *transTable;
extern int transTableSize;



void krio_readTimeDelayDefs(void);

/* end global definition section */

/* begin private definition section */

/*#################################################

GROUP: Local Defines

#################################################*/

#define LIN_MAX      (MAX_LIN_LEN + 1)

#define SITE_DEF     1
#define TYPE_DEF     2
#define DEFAULT_DEF  3
#define UNIT_DEF     4
#define CONNECT_DEF  5
#define SUBNET_DEF   6
#define LAYER_DEF    7


#define EOS	'\0'	/*  End Of String  */


/*#################################################

GROUP: Local Var's

#################################################*/

typedef char   SymbolType[LIN_MAX];

static FILE  *file_in;
static FILE  *file_out;

static bool  is_subnet_info,
	     is_layer_info,
	     units_have_sites;

static int   site_name_len,
	     unit_name_len,
	     type_name_len,
	     site_func_len,
	     act_func_len,
	     out_func_len,
	     def_act_func_len,
	     def_out_func_len,
             unit_no_len,
             max_connects_per_line,
             max_subnets_per_line,
             max_layers_per_line,
             pos_no_len,
             subnet_no_len,
             layer_no_len,
             NoOfLinks,
             NoOfSiteTypes,
             NoOfUnitTypes,
             NetfileVersion;


static char  fmt_shape1 [ LIN_MAX ],
	     fmt_shape2 [ LIN_MAX ],
	     fmt_shape3 [ LIN_MAX ],
	     fmt_shape4 [ LIN_MAX ],
	     fmt_hdr1	[ LIN_MAX ],
	     fmt_hdr2	[ LIN_MAX ],
	     fmt_blank	[ LIN_MAX ];


static char  *title[] = {
    "SNNS network definition file",
    "generated at",
    "network name",
    "no. of units",
    "no. of connections",
    "no. of unit types",
    "no. of site types",
    "learning function",
    "site definition section",
    "type definition section",
    "unit definition section",
    "connection definition section",
    "subnet definition section",
    "unit default section",
    "source files",
    "layer definition section",
    "update function",
    "3D translation section",
    "time delay section",
    "pruning function",
    "subordinate learning function"
/*
    "topologic definition section",
    "feedforward network"
*/
    };	 /*  20  */

static int  NoOfTitles = (sizeof title) / (sizeof title[0]);

static char *resHeader[] = {
    "SNNS result file %s\n",
    "generated at",
    "No. of patterns     : %d\n",
    "No. of input units  : %d\n",
    "No. of output units : %d\n",
    "startpattern        : %d\n",
    "endpattern          : %d\n",
    "input patterns included\n",
    "teaching output included\n"
    };

static char  *headers[] = {
    " site name | site function",
    " name | act func | out func | sites",
    " act | bias | st | subnet | layer | act func | out func",
    " no. | typeName | unitName | act | bias | st | position | act func | out func | sites",
    " target | site | source:weight",
    " subnet | unitNo.",
    " layer | unitNo.",
    " delta x | delta y | z",
    " no. | LLN | LUN | Toff | Soff | Ctype"
    };



/*
static void  mstrcat(int __builtin_va_alist);
static void  mstrcpy(int __builtin_va_alist);
*/
static void  mstrcat(char *va_alist,...);
static void  mstrcpy(char *va_alist,...);
static char  *krio_getIOVersion(void);
static krui_err  krio_writeHeader(char *version, char *net_name);
static void krio_cutTrailingZeros(char *string);
static char  *krio_repchar(char c, int N);
static void  krio_stringLimits(void);
static krui_err  krio_fmtShapeing(int choose_me);
static krui_err  krio_writeSiteDefinitions(void);
static krui_err  krio_writeTypeDefinitions(void);
static char  *getTType(int st);
static krui_err  krio_writeDefaultDefinitions(void);
static krui_err  krio_writeUnitDefinitions(void);
static krui_err  krio_writeSourcesAndWeights(void);
static krui_err  krio_writeConnectionDefs(void);
static krui_err  krio_writeSubnetDefs(void);
static krui_err  krio_writeLayerDefs(void);
static krui_err  writeXYTransTable(void);
static bool  skipComments(void);
static bool  skipSpace(void);
static bool  comma(void);
static bool  get_nl(void);
static bool  get_pipe(void);
static bool  skip_pipe(void);
static bool  get_alpha(void);
static bool  getSymbol(char *symbol);
static char  *getSection(char *line, int *title_no);
static bool  matchHead2(int N);
static char  *my_strstr(char *s, char *find);
static void  krio_readHeader(char *netfile_version, char *net_name,
			     char *learn_func, char *update_func, char
			     *pruning_func, char *ff_learn_func, int
			     *no_of_units, int *no_of_connect, int
			     *no_of_unitTypes, int *no_of_siteTypes);
static int  str_to_Ttype(char *str);
static void  krio_readSiteDefinitions(void);
static void  krio_readTypeDefinitions(void);
static void  krio_readDefaultDefinitions(void);
static void  krio_readUnitDefinitions(void);
static void  krio_readConnectionDefs(void);
static void  krio_readSubnetDefs(void);
static void  krio_readLayerDefs(void);
static void  readXYTransTable(void);


/* end private definition section */

#endif 



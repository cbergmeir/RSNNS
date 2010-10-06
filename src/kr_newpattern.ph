/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_newpattern.ph,v $
  SHORTNAME      : newpattern
  SNNS VERSION   : 4.2

  PURPOSE        : handling of new pattern format
  NOTES          : 

  AUTHOR         : Michael Vogt
  DATE           : 10.9.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/04/22 16:10:17 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _KR_NEWPATTERN_DEFINED_
#define _KR_NEWPATTERN_DEFINED_

/* begin global definition section */

/*****************************************************************************
 FUNCTIONS WHICH ARE CALLED BY THE KERNEL USER INTERFACE TO PERFORM
 THE KERNEL INTERFACE OF THE NEW PATTERN MANAGEMENT
******************************************************************************/

krui_err kr_npui_setCurrPatSet(int number);
/*****************************************************************************
determines the number of the current pattern set (in kernel terminology) 
numbering starts with 0
******************************************************************************/

krui_err kr_npui_deletePatSet(int number);
/*****************************************************************************
deletes the specified pattern set from memory and undefines the
current pattern set, pattern, training scheme and display scheme
******************************************************************************/

krui_err kr_npui_GetPatInfo(pattern_set_info *set_info, 
			    pattern_descriptor *pat_info);
/*****************************************************************************
retrieves all available information concerning the current pattern set
and the current pattern which both must be defined. The given
parameter fields are filled with the information.
******************************************************************************/

krui_err kr_npui_DefShowSubPat(int *insize, int *outsize, 
			       int *inpos, int *outpos);
/*****************************************************************************
Define the display scheme:
Size and position of a sub pattern for the current pattern in the
current pattern set is defined. <insize> is a pointer to an array of
integer values which define the dimensional sizes of the input sub
pattern. <inpos> is a pointer to an array of integer values which
defines the offset (position) of this sub pattern inside the
pattern. <outsize> and <outpos> are used to define the respective
output sub pattern
******************************************************************************/

krui_err kr_npui_DefTrainSubPat(int *insize, int *outsize, 
				int *instep, int *outstep, int *max_n_pos);
/*****************************************************************************
Define the training scheme:
Size and step size of sub pattern for the current pattern in the
current pattern set is defined for training and testing. <insize> is a
pointer to an array of integer values which define the dimensional
sizes of the input sub pattern. <instep> is a pointer to an array of
integer values which defines the step size which is used to move the
sub pattern over the pattern.
<outsize> and <outpos> are used to define the respective output sub
pattern.
<max_n_pos> (if not NULL) returns the number of valid input sub
pattern positions for the current pattern and the given training
scheme.
******************************************************************************/

krui_err kr_npui_AlignSubPat(int *inpos, int *outpos, int *no);
/*****************************************************************************
Align the position of a sub pattern:
Using the current training scheme and the current pattern of the
current pattern set, the given position of an input sub pattern
<inpos> and the given position of the corresponding output sub pattern
<outpos> is aligned to fit the currently defined training scheme.
E.g. if the training scheme defines a step width of 5 for a specific
dimension, only the positions 0, 5, 10, 15 ...  are valid positions
for a sub pattern.
The position of each dimension is aligned independently from all other
dimensions by moving to the next valid position which is lower or
equal to the given position.  <no> (if not NULL) returns the number of
the sub pattern which corresponds to the new aligned position which is
returned in place (<inpos> <outpos>).
******************************************************************************/

krui_err kr_npui_allocNewPatternSet(int *set_no);
/*****************************************************************************
Allocate an (additional) empty pattern set: A new pattern set is
allocated if the maximum number of loaded pattern sets
(NO_OF_PAT_SETS) is not exceeded. The corresponding pattern set handle
is returned in <set_no>.  The new allocated pattern set becomes the
current set.  There is no current pattern defined.  Training scheme
and display scheme both become undefined.
******************************************************************************/

krui_err kr_npui_loadNewPatterns(char *filename, int *set_no);
/*****************************************************************************
Load an (additional) pattern file:
The file with name <filename> is loaded into memory if existent and if
the maximum number of loaded pattern sets (NO_OF_PAT_SETS) is not
exceeded. The corresponding pattern set handle is returned in
<set_no>.
The new loaded pattern set becomes the current set. The first pattern
inside this set becomes the current pattern. Training scheme and
display scheme both become undefined.
******************************************************************************/

krui_err kr_npui_saveNewPatterns(char *filename, int set_no);
/*****************************************************************************
The given pattern set <set_no> is written to file <filename> in new
style format. No side effects.
******************************************************************************/

krui_err kr_npui_GetShapeOfSubPat(int *insize, int *outsize, 
				  int *inpos, int *outpos, int n_pos);
/*****************************************************************************
Get the shape of a sub pattern which is specified by a number:
After kr_npui_DefTrainSubPat has been called for the current pattern
set and a current pattern is defined, this function retrieves the
<n_pos>th valid sub pattern pair which matches the defined training
scheme. Size and position of the sub pattern pair is returned in
<insize> <inpos> <outsize> and <outpos> which are all pointer to
integer arrays.
******************************************************************************/

krui_err kr_npui_setRemapFunction(char *name, float *params);
/*****************************************************************************
Set the remap function for the current pattern set and use the given 
parameters.
<name> points to a remap function name, provided by the user interface. The 
function name is checked and the pointer for this function is looked up in the 
function table. If no function name is given (NULL pointer) or if the default 
remaping function is given the default remap function is used 
(no remaping of patterns occurs). The name of the remap function is stored in
the pattern set description for future access by the interface.
<params> points to an array of function parameters of size NO_OF_REMAP_PARAMS
provided by the user interface. The parameters used for the given remap 
function are read out of the array and copied to the local pattern set
description.
******************************************************************************/

krui_err kr_npui_setClassDistribution(unsigned int *class_sizes);
/*****************************************************************************
Set the class distribution for the current pattern set.
This function may only be called if the current pattern set provides class 
information. If <class_sizes> is not NULL it must locate an integer array of 
size that matches the number of classes in the current set. The values of 
this array are copied to the pattern set area. If <class_sizes> is the NULL
pointer, then the real distribution of classes in the pattern set is 
computed and stored in the class distribution area of the pattern set. So the
interface may retrieve default values for the class distribution which match
the real distribution of the pattern set.
******************************************************************************/

krui_err kr_npui_useChunk(bool active);
/*****************************************************************************
Switch on or off the class distribution process.
If <active> is TRUE, the distribution process is activated. Therefore the 
internal representation of the pattern set is virtually reorganized to match
the requested values of <class_sizes>. After that step the number of patterns
in the set virtually increases or decreases because each epoch must cover 
every pattern of all classes that have a requested distribution amount bigger
than 0. This also implies that several patterns from a specific class are 
reused several times during one epoch.
******************************************************************************/

krui_err kr_npui_setClass(char *classname);
/*****************************************************************************
Set the class name for the current pattern.
If the pattern set already contains class information, only the
current pattern is affected. If there are no classes in the pattern
set so far, all patterns of the current set are set to the given class
name, since every pattern must belong to a class in that case.
If the given class name is the NULL pointer, nothing happens at all.
******************************************************************************/


/*****************************************************************************
 FUNCTIONS WHICH ARE CALLED BY OTHER KERNEL FUNCTIONS LIKE TRAINING
 AND INITIALIZATION FUNCTIONS:
******************************************************************************/

struct np_symtab
{
    char *symname;             /* name of the class */
    int set_amount;            /* total number of patterns of this
				  class in the set */ 
    int chunk_amount;          /* defined chunk size of this class
				  within the set */
    int symnum;                /* index to user visible class names
				  list. This index has to be reset
				  whenever the symbol table changes */

    int my_chunks_per_set;     /* minimum number of chunks to use all
				  patterns from this class */
    int global_chunks;         /* global number of chunks to use all
				  patterns from all classes from this
				  set */
    int pat_nums_size;         /* number of entries (size) of the
				  pattern number list for this class
				  */
    int pat_nums_valid;        /* number of valid entries in the
				  pattern number list (usually
				  global_chunks * chunk_amount >= set_amount
				  entries) */
    int *pat_nums;             /* array of duplicated pattern numbers
				  for this class */
    int within_chunk_pos;      /* position counter within current
				  chunk ( < chunk_amount) */
    int chunk_comp_count;      /* backward counter used for chunk composition */
    int chunk_comp_base;       /* base value used for chunk composition */
    int pat_num_pos;           /* index to pat_nums[] for next pattern
				  in chunk ( < pat_nums_valid) */

    struct np_symtab *next;    /* next entry in symbol table */
};

typedef struct 
{
    pattern_set_info pub;      /* public part of the pattern set             */

    bool chunk_shuffle;        /* chunk shuffle flag from last ordering      */
    bool chunk_order_valid;    /* TRUE if all chunk arrays are valid         */
    RemapFuncPtr rmf_ptr;      /* remap function pointer or NULL             */
} np_pattern_set_info;

typedef struct
{
    pattern_descriptor pub;    /* public part of the current pattern         */

    float *input_pattern;      /* pointer to the input pattern data          */
    float *output_pattern;     /* pointer to the output pattern data         */

    struct np_symtab *mysym;   /* pointer to my symbol table entry */
} np_pattern_descriptor;

int  kr_np_pattern(int mode ,int mode1 ,int pattern_no);
/*****************************************************************************
multiple pattern handling functions depending on mode and mode1
*****************************************************************************/

krui_err kr_initSubPatternOrder(int start, int end);
/*****************************************************************************
The sub pattern ordering for the current pattern set is reset for the
next training or initialization run. During this run all sub patterns
from pattern <start> up to pattern <end> are generated according to
current shuffle flags for patterns and sub patterns.
kr_getSubPatByOrder is to be called to get the next sub pattern number
during the run (see below)
*****************************************************************************/

bool kr_getSubPatternByOrder(int *pattern, int *sub);
/*****************************************************************************
According to the last call to kr_initSubPatternOrder, the last call to
this function and the shuffle flags, the next position of pattern and
sub pattern is determined. This numbers are returned in <pattern> and
<sub> (beginning with 0). If there are no more sub pattern avaliable
the return value is FALSE, otherwise TRUE.
*****************************************************************************/

bool kr_getSubPatternByNo(int *pattern, int *sub, int n);
/*****************************************************************************
According to the current pattern set, the position of the <n>th sub
pattern is determined and returned in <pattern> (the pattern which
includes the subpattern) and <sub> (the sub pattern inside the
pattern) (beginning with 0). 
This function does not effect the ordering of the function
kr_getSubPatByOrder. <n> ranges from 0 to kr_TotalNoOfSubPatPairs()-1.
If the sub pattern is available, TRUE is returned, otherwise FALSE.
*****************************************************************************/

int kr_TotalNoOfSubPatPairs(void);
/*****************************************************************************
This function returns the total number of available sub patterns for
the current pattern set or 0 if no pattern set is defined.
The result is the sum of the numbers of subpattern for all patterns in
the current set.
*****************************************************************************/

int kr_NoOfSubPatPairs(int pattern);
/*****************************************************************************
This function returns the number of available sub patterns for the
pattern <pattern> of the current pattern set or 0 if this pattern is
not defined.
*****************************************************************************/

int kr_AbsPosOfFirstSubPat(int pattern);
/*****************************************************************************
This function returns the absolute position of the first sub pattern
of pattern <pattern> in the current pattern set. This position is
defined as the Sum of kr_NoOfSubPatPairs(i) where i runs from 0 to
<pattern>-1.
The absolute position of the first sub pattern of pattern 0 is 0.
The returned value may be used as argument for the function
kr_getSubPatternByNo.
*****************************************************************************/

int kr_TotalNoOfPattern(void);
/*****************************************************************************
This function returns the total number of available patterns for
the current pattern set or 0 if no pattern set is defined.
*****************************************************************************/

Patterns kr_getSubPatData(int pat_no, int sub_no, int io_type, int *size);
/*****************************************************************************
For the current pattern set and the specified sub pattern size, the
data array of the <sub_no>th sub pattern of the <pat_no>th pattern is
returned. io_type spcifies whether the input (INPUT) or output
(OUTPUT) data is requested. If <size> is != NULL the size of the data
array is returned is this parameter.
The function returns a pointer to the data array (type Patterns) or
NULL if an error occured.
*****************************************************************************/

int kr_getSubPatClass(int pat_no, int sub_no);
/*****************************************************************************
For the current pattern set and the specified sub pattern, the class index 
(>=0) is returned. If the pattern set does not provide class information, the 
return value is -1
*****************************************************************************/

int kr_SizeOfInputSubPat(void);
/*****************************************************************************
For the current pattern set and the specified sub pattern size, the size of 
the input part of the first sub pattern of the first pattern is returned. 
Negative return values indicate KernelErrorCode. Size 0 is a valid return value
since the pattern may contain no data.
*****************************************************************************/

int kr_SizeOfOutputSubPat(void);
/*****************************************************************************
For the current pattern set and the specified sub pattern size, the size of 
the output part of the first sub pattern of the first pattern is returned. 
Negative return values indicate KernelErrorCode. Size 0 is a valid return value
since the pattern may contain no data.
*****************************************************************************/

/*****************************************************************************
 FUNCTIONS WHICH ARE CALLED BY THE PATTERN PARSER OR FROM INSIDE THIS
 MODULE. DON'T USE THESE FUNCTIONS FOR OTHER PURPOSES !!!!!
******************************************************************************/

krui_err kr_np_AllocatePatternSet(int *pat_set, int number);
/*****************************************************************************
A new set of patterns with <number> number of patterns is defined. An
internal pattern set number is assigned to reference this pattern set.
It is returned in <pat_set>.
******************************************************************************/

krui_err kr_np_AllocatePattern(np_pattern_descriptor *pattern,
			       bool input);
/*****************************************************************************
Depending on the entries input_dim, input_dim_sizes, input_fixsize resp.
output_dim, output_dim_sizes, output_fixsize of the pattern descriptor
<pattern> new memory for this pattern is allocated (input_pattern resp.
output_pattern). input_info resp. output_info is set to (char *) NULL.
The function allocates space for an input pattern if input is TRUE,
output pattern otherwise.
******************************************************************************/

void kr_np_FreePattern(np_pattern_descriptor *pattern);
/*****************************************************************************
frees the area of the pattern data for this pattern
******************************************************************************/

krui_err kr_np_GetDescriptor(int pat_set, int number, 
			     np_pattern_descriptor **pattern);
/*****************************************************************************
A pointer to the pattern pattern descriptor of pattern <number> in
pattern set <pat_set> is returned in <pattern>. The structure pattern
must be provided by the calling routine. A call to this function makes
the specified pattern to become the current pattern.  Return value:
error status
******************************************************************************/

krui_err kr_np_lookupSym(int pat_set, char *symbol, 
			 struct np_symtab **entry);
/*****************************************************************************
Looks for the class name <symbol> ind set <pat_set>. If the name is
not present up to now, a new entry is allocated and initialized. In
case of success <*entry> is set to the symbol table entry.
******************************************************************************/

/* end global definition section */

/* begin private definition section */

/* size of memory pages for fixed size pattern: 0.5MB, 2KB in debug mode */
#ifdef DEBUG_PATTERN
#define KRNP_FIXED_PAGESIZE 0x800
#else
#define KRNP_FIXED_PAGESIZE 0x80000
#endif

/* data structure to handle pattern space management */
struct patternpage
{
    int slotsize;              /* size of slots in multiples of sizeof(float) */
    int no_of_slots;           /* total number of slots in this page */
    int no_of_free_slots;      /* number of free slots in this page */
    int firstfree;             /* index (base: *float) to first free slot or -1 */
    float *data;               /* data area, total size:
				  (no_of_slots * slotsize * sizeof(float)) */
    struct patternpage *next;  /* next page */
};

/*****************************************************************************
 INTERNAL FUNCTIONS OF THIS MODULE. IMPOSSIBLE TO CALL THESE FUNCTIONS
 FROM OUTSIDE THE MODULE (IF NOBODY CHANGES THE STATIC DECLARATION) !!!!!!!
******************************************************************************/

static float *kr_np_floatmalloc(int size);
/*****************************************************************************
allocate pattern memory within a private memory page
<size> is based on sizeof(float) !!
******************************************************************************/

static void kr_np_floatfree(float *ptr);
/*****************************************************************************
free the pattern memory within a private memory page
******************************************************************************/

static krui_err kr_np_InitPattern(void);
/*****************************************************************************
initialization of pattern descriptor array
******************************************************************************/

static krui_err kr_np_ReallocatePatternSet(int pat_set, int new_number);
/*****************************************************************************
reallocates the pattern set <pat_set> to contain <new_number> 
of pattern entries.
******************************************************************************/

static krui_err kr_np_DeletePatternSet(int pat_set);
/*****************************************************************************
The pattern set <pat_set> is deleted. All memory of all pattern is
given back to the operating system.
******************************************************************************/

static krui_err kr_np_DeletePattern(int pat_set, int pattern);
/*****************************************************************************
delete a specific pattern form a pattern set
******************************************************************************/

static void kr_np_delSym(int pat_set, struct np_symtab *entry);
/*****************************************************************************
delete the symbol table entry from the given pattern set.
******************************************************************************/

static krui_err kr_np_AddPattern(int pat_set, int *pattern);
/*****************************************************************************
Add an empty pattern descriptor to the specified pattern set. The position
of the pattern inside the set is returned in <pattern>.
******************************************************************************/

static krui_err kr_np_LoadPatternFile(FILE *pat_file, int *pat_set);
/*****************************************************************************
The pattern file <pat_file> is loaded into memory. An internal pattern
set number is assigned to this pattern file and returned in <pat_set>.
Return value: error status
******************************************************************************/

static krui_err kr_np_SavePatternFile(FILE *pat_file, int pat_set);
/*****************************************************************************
The pattern set <pat_set> is saved to FILE <pat_file>.
Return value: error status
******************************************************************************/

static krui_err kr_np_GetInfo(int pat_set, np_pattern_set_info *info);
/*****************************************************************************
All information concerning pattern set <pat_set> is copied into the
informational structure <info> which must be provided by the calling
routine.
The current pattern set is set to pat_set.
Return value: error status
******************************************************************************/

static krui_err kr_np_ValidateInfo(int pat_set);
/*****************************************************************************
update pattern set information if necessary
******************************************************************************/

static krui_err kr_np_GetSubPatSizes(int *input_size, int *output_size);
/*****************************************************************************
Depending on the current pattern set and the sub pattern training scheme,
which must be defined, the size of the first input sub pattern and the size
of the first output sub pattern is computed.
******************************************************************************/

static krui_err kr_np_GetSubPat(bool input, int *pos_coord, int *size_coord,
				float **data, int *entries);
/*****************************************************************************
A sub pattern is cut out of the current pattern at position
<pos_coord> with size <size_coord>. According to <input> the input
part or the output part of the current pattern is used. A pointer to
an array of float which contains the cut part is returned in <data>.
<pos_coord> is an array of int which defines the position of the sub
pattern inside the variable sized dimensions of the pattern.
<size_coord> is an array of int which defines the size of the sub
pattern in each dimensional direction. Both arrays must contain
<input_dim> (or <output_dim>) entries (see pattern descriptor).

Example: pattern with input_fixsize 2, input_dim 2, input_dim_sizes [4 5]
(hint: the values of the pattern represent the position)

{
0.00, 0.01, 0.10, 0.11, 0.20, 0.21, 0.30, 0.31, 0.40, 0.41,
1.00, 1.01, 1.10, 1.11, 1.20, 1.21, 1.30, 1.31, 1.40, 1.41,
2.00, 2.01, 2.10, 2.11, 2.20, 2.21, 2.30, 2.31, 2.40, 2.41,
3.00, 3.01, 3.10, 3.11, 3.20, 3.21, 3.30, 3.31, 3.40, 3.41,
}

the sub pattern with <pos_coord> [1 2], <size_coord> [3 2] looks like
this:

{
1.20, 1.21, 1.30, 1.31,
2.20, 2.21, 2.30, 2.31,
3.20, 3.21, 3.30, 3.31,
}

The parameter entries returns the number of entries in the data field.

******************************************************************************/

static krui_err kr_np_SetSubPat(bool input, int *pos_coord, int *size_coord,
				float *data, int entries);
/*****************************************************************************
Equivalent to kr_np_GetSubPat, but copies the <data> into the specified 
sub pattern
*****************************************************************************/

static bool kr_np_align_sub_pos(int dim, int *n, int *psize, int *ssize, 
				int *sstep, int *spos);
/*****************************************************************************
For a given pattern dimension size <psize> of <dim> dimensions and a
given sub pattern size <ssize> the position <spos> of the sub pattern
is alligned to a valid position and the ordering number of this sub
pattern is returned in <n>. The shift pattern given in <sstep> is used
to find valid allignment positions
*****************************************************************************/

static bool kr_np_gen_sub_pos(int dim, int *n, int *psize, int *ssize, 
			      int *sstep, int *spos, bool count);
/*****************************************************************************
For a given pattern dimension size <psize> of <dim> dimensions and a
given sub pattern size <ssize> the position <spos> of the <n>th sub
pattern is calculated. The shift pattern given in <sstep> is used to
move sub pattern over the pattern.
If <count> is TRUE, only the number of possible positions is
determined and returned in <n> if at least 1 valid position
exists. <spos> is of no effect and may be NULL.
RETURNS  : 
TRUE if the <n>th subpattern exists or if <count> is TRUE and at least
1 valid position exists, FALSE if <n> < 0.  If <n> is higher than the
available number of subpattern, a wraparound occurs.
<n> starts with 0 and ends with <number of possible positions> - 1 or
returns number of possible positions.
*****************************************************************************/

static bool kr_np_allocate_pat_train_entries(int n);
/*****************************************************************************
allocate or reallocate an array which will later include the sorted or
shuffled pattern order (during training)
*****************************************************************************/

static bool kr_np_allocate_sub_pat_train_entries(int n);
/*****************************************************************************
allocate or reallocate an array which will later include the sorted or
shuffled order of the sub pattern of the current pattern
*****************************************************************************/

static bool kr_np_allocate_pat_mapping_entries(int n);
/*****************************************************************************
allocate or reallocate an array which will later include
the virtual to physical pattern number mapping
*****************************************************************************/

static void kr_np_order_pat_entries(int start, int end);
/*****************************************************************************
Fills the allocated array for the pattern ordering with increasing
numbers if patterns are sorted or with a random permutation if
patterns are shuffled.  <start> and <end> define the first and last
pattern number to be used
*****************************************************************************/

static void kr_np_order_sub_pat_entries(int start, int end);
/*****************************************************************************
Fills the allocated array for the sub pattern ordering with increasing
numbers if sub patterns are sorted or with a random permutation if sub
patterns are shuffled.  <start> and <end> define the first and last
sub pattern number to be used
*****************************************************************************/

static void kr_np_order_chunk_arrays(bool shuffle, int pattern_set);
/*****************************************************************************
Fills the allocated array for the pattern ordering with increasing
numbers if patterns are sorted or with a random permutation if
patterns are shuffled.
*****************************************************************************/

static void kr_np_order_chunked_pat_entries(int pat_set, int start, int end); 
/*****************************************************************************
Fills the allocated array for the pattern ordering with the
contents of all chunk arrays.  <start> and <end> define the first and
last pattern number to be used
*****************************************************************************/


static void kr_np_fill_virtual_to_void_mapping(int pat_set);
/*****************************************************************************
Fills the allocated mapping array for the pattern ordering with
the contents of all chunk arrays.
*****************************************************************************/

static int kr_np_virtual_to_physical(int vnum);
/*****************************************************************************
mapping of virtual pattern numbers to physically stored patterns
*****************************************************************************/

static int kr_np_physical_to_virtual(int pnum);
/*****************************************************************************
mapping of physical pattern number to a virtual number
*****************************************************************************/

static krui_err kr_np_DefineSubPatternOrdering(int pat_set, bool input, 
					       int *size_coord, 
					       int *incr_coord);
/*****************************************************************************
The shape and ordering of sub patterns for training and display is
defined.  <pat_set> specifies the pattern set to use. The flag <input>
determines whether input or output sub patterns are to be defined. The
array size_coord spezifies the shape of the sub pattern like in
kr_np_GetSubPat. The array incr_coord specifies how sub patterns are
generated from the whole pattern by shifting the shape over this
pattern. Each value in this array gives an incremental offset for one
dimensional direction. The start position is at [0 0 0 ...]. New
positions are generated by adding the rightmost value of incr_coord to
the old position. If an overflow in this dimension occurs, this
coordinate is reset to 0 and the next position to the left is
incremented (and so on ...). After all sub patterns of one pattern are
generated the next pattern is used. Depending on npui_shuffle_pattern
and npui_shuffle_sub_pattern shuffling is activated.
*****************************************************************************/

static krui_err kr_np_showPatternSTD(int mode);
/*****************************************************************************
According to the mode kr_np_showPatternSTD stores the current
Pattern/sub Pattern into the units activation (and/or output) values.
The modes are:
- OUTPUT_NOTHING
store input pattern into input units activations
- OUTPUT_ACT
store input pattern into input units activations and store output
pattern into output units activations
- OUTPUT_OUT
store input pattern into input units activations, store output pattern
into output units activations and update output units output
*****************************************************************************/

static krui_err kr_np_modifyPattern(void);
/*****************************************************************************
The current activation of the input and output units is used to
modify the current sub pattern.
*****************************************************************************/

/*****************************************************************************
 STATIC VARIABLE DEFINITIONS
*****************************************************************************/

/* table for mapping of external pattern set numbers to internal
   numbers. This array is always filled from the beginning. If a set
   is deleted, all higher entries are moved down inside the table. */
static int                npui_pat_sets[NO_OF_PAT_SETS];

/* the number of used entries in the above table */
static int                npui_number_pat_sets             = 0;

/* the index of the current pattern set into the above table */
static int                npui_curr_pat_set                = -1;

/* the number of the current pattern for the current pattern set. This
 is the pattern which is used for several user actions (modify, show,
 ...). This is a virtual pattern number. */
static int                npui_curr_pattern                = -1;

/* stored values for the sub pattern size, position and shifting step
   size */
static int                npui_insize[MAX_NO_OF_VAR_DIM];
static int                npui_outsize[MAX_NO_OF_VAR_DIM];
static int                npui_inpos[MAX_NO_OF_VAR_DIM];
static int                npui_outpos[MAX_NO_OF_VAR_DIM];
static int                npui_instep[MAX_NO_OF_VAR_DIM];
static int                npui_outstep[MAX_NO_OF_VAR_DIM];

/* TRUE if size and position fields above are valid */
static bool               npui_show_defined                = FALSE;

/* TRUE if size and step sizes fields above are valid */
static bool               npui_train_defined               = FALSE;

/* shuffle flags as set by the user */
static bool               npui_shuffle_pattern             = FALSE;
static bool               npui_shuffle_sub_pattern         = FALSE;




/* array of (sub) pattern numbers that are to be trained with the
   current pattern set (pattern). numbers within this array may be
   shuffled */
static int                *np_pat_train_order              = NULL;
static int                *np_sub_pat_train_order          = NULL;
static int                *np_pat_mapping_order            = NULL;

/* contents of np_(sub_)pat_train_order is valid, if this variable is
 TRUE */
static bool               np_pat_train_valid               = FALSE;
static bool               np_sub_pat_train_valid           = FALSE;
static bool               np_pat_mapping_valid             = FALSE;

/* this is the allocated size of np_(sub_)pat_train_order. It is used
   during reallocation for other pattern sets or different training
   situations */
static int                np_pat_train_size                = 0;
static int                np_sub_pat_train_size            = 0;
static int                np_pat_mapping_size              = 0;

/* actual number of valid entries in the np_(sub_)pat_train_order
   field. This number may be smaller than np_(sub_)pat_train_size */
static int                np_pat_train_number              = 0;
static int                np_sub_pat_train_number          = 0;

/* index into np_(sub_)pat_train_order field which defines the
   position of the next pattern number that is to be returned by an
   ordered (or shuffled) pattern access. -1 means that alle field
   antries have been processed in an ordered (shuffled) way. */
static int                np_next_train_pat                = -1;
static int                np_next_train_sub_pat            = -1;

/* if pattern shuffling AND sub pattern shuffling is activated at the
   same time, one epoch does not match the usual criterion that each
   (sub-)pattern has to be presented once. Instead one epoch means
   that np_random_train_number sub patterns from the current pattern
   set are trained where several sub patterns may be trained more than
   once. This done due to big overhead, if normal epoch definition is
   used.
   np_random_train_number equals the total number of sub patterns for
   a mixed shuffled access */
static int                np_random_train_number           = 0;

/* This is the pattern-in-epoch counter for the mixed shuffled
   access. -1 means that the epoch is completed */
static int                np_next_train_random             = -1;



/* This is the sub pattern definition/shifting scheme for the current
   pattern set */
static int                np_t_insize[MAX_NO_OF_VAR_DIM];
static int                np_t_outsize[MAX_NO_OF_VAR_DIM];
static int                np_t_instep[MAX_NO_OF_VAR_DIM];
static int                np_t_outstep[MAX_NO_OF_VAR_DIM];

/* size of the input and output parts of sub pattern for the current
   sub pattern definition. */
static int                np_sub_pat_input_size            = 0;
static int                np_sub_pat_output_size           = 0;

/* this flag denotes whether the above two variables are valid */
static bool               np_sub_pat_sizes_valid           = FALSE;




/* internal memory for the total number of sub patterns in the current
   pattern set */
static int                np_abs_count_No                  = 0;

/* internal flag that determines, whether np_abs_count_No has a valid
   entry or has to be recomputed otherwise. This flag is set to FALSE
   during every operation, which may change the access structure to
   the pattern set. */
static bool               np_abs_count_valid               = FALSE;

/* the field np_abs_count[n] contains the accumulated number of sub
   patterns up to the pattern n. This field is used to find a specific
   sub pattern by number. */
static int                *np_abs_count                    = NULL;

/* This is the allocated size of the field np_abs_count. */
static int                np_abs_count_size                = 0;



/* size of all following set specific arrays (equals NO_OF_PAT_SETS,
   but kept as variable for further virtualization) */
static int                   np_used_pat_set_entries       = 0;

/* array if flags which are TRUE if the index is used for a loaded
   pattern set */
static bool                  *np_pat_set_used              = NULL;

/* array of fields of pattern descriptors, here the patterns are stored
   as defined in the pattern file */
static np_pattern_descriptor **np_pat_sets                 = NULL;

/* array of set descriptors which hold set specific data independent
   from a specific pattern */
static np_pattern_set_info   *np_info                      = NULL;

/* array of flags which are true if the indexed set descriptor holds
   valid data. These flags are reset if the structure of the pattern
   set changes. To make the set descriptor valid, usually all patterns
   in the set have to be examined */
static bool                  *np_info_valid                = NULL;

/* array of symbol tables to store the class names of the indexed
   set. If NULL, the symbol table is empty which is equivalent that
   the pattern set holds no class information */
static struct np_symtab      **np_st                       = NULL;



/* internal pointer to the current physical pattern in the current set */
static np_pattern_descriptor *np_current_pattern           = NULL;



/* data areas and allocated sizes to hold a sub pattern which is ready
   for external usage (training, etc...). Note: the data within these
   fields is read only, which means that modifications have no effect
   on the stored pattern set */
static float                 *np_i_subpat                  = NULL;
static int                   np_i_subpatsize               = 0;
static float                 *np_o_subpat                  = NULL;
static int                   np_o_subpatsize               = 0;

/* entry to private memory management */
static struct patternpage    *np_pages                     = NULL;

/* end private definition section */

#endif

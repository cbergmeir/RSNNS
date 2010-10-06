/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_newpattern.h,v $
  SHORTNAME      : newpattern
  SNNS VERSION   : 4.2

  PURPOSE        : handling of new pattern format
  NOTES          : 

  AUTHOR         : Michael Vogt
  DATE           : 10.9.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.12 $
  LAST CHANGE    : $Date: 1998/04/08 09:16:51 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _KR_NEWPATTERN_DEFINED_
#define _KR_NEWPATTERN_DEFINED_


/*****************************************************************************
 FUNCTIONS WHICH ARE CALLED BY THE KERNEL USER INTERFACE TO PERFORM
 THE KERNEL INTERFACE OF THE NEW PATTERN MANAGEMENT
******************************************************************************/

extern krui_err kr_npui_setCurrPatSet(int number);
/*****************************************************************************
determines the number of the current pattern set (in kernel terminology) 
numbering starts with 0
******************************************************************************/

extern krui_err kr_npui_deletePatSet(int number);
/*****************************************************************************
deletes the specified pattern set from memory and undefines the
current pattern set, pattern, training scheme and display scheme
******************************************************************************/

extern krui_err kr_npui_GetPatInfo(pattern_set_info *set_info, 
			    pattern_descriptor *pat_info);
/*****************************************************************************
retrieves all available information concerning the current pattern set
and the current pattern which both must be defined. The given
parameter fields are filled with the information.
******************************************************************************/

extern krui_err kr_npui_DefShowSubPat(int *insize, int *outsize, 
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

extern krui_err kr_npui_DefTrainSubPat(int *insize, int *outsize, 
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

extern krui_err kr_npui_AlignSubPat(int *inpos, int *outpos, int *no);
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

extern krui_err kr_npui_allocNewPatternSet(int *set_no);
/*****************************************************************************
Allocate an (additional) empty pattern set: A new pattern set is
allocated if the maximum number of loaded pattern sets
(NO_OF_PAT_SETS) is not exceeded. The corresponding pattern set handle
is returned in <set_no>.  The new allocated pattern set becomes the
current set.  There is no current pattern defined.  Training scheme
and display scheme both become undefined.
******************************************************************************/

extern krui_err kr_npui_loadNewPatterns(char *filename, int *set_no);
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

extern krui_err kr_npui_saveNewPatterns(char *filename, int set_no);
/*****************************************************************************
The given pattern set <set_no> is written to file <filename> in new
style format. No side effects.
******************************************************************************/

extern krui_err kr_npui_GetShapeOfSubPat(int *insize, int *outsize, 
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

extern krui_err kr_npui_setRemapFunction(char *name, float *params);
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

extern krui_err kr_npui_setClassDistribution(unsigned int *class_sizes);
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

extern krui_err kr_npui_useChunk(bool active);
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

extern krui_err kr_npui_setClass(char *classname);
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
				  global_chunks * chunk_amount
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

extern int  kr_np_pattern(int mode ,int mode1 ,int pattern_no);
/*****************************************************************************
multiple pattern handling functions depending on mode and mode1
*****************************************************************************/

extern krui_err kr_initSubPatternOrder(int start, int end);
/*****************************************************************************
The sub pattern ordering for the current pattern set is reset for the
next training or initialization run. During this run all sub patterns
from pattern <start> up to pattern <end> are generated according to
current shuffle flags for patterns and sub patterns.
kr_getSubPatByOrder is to be called to get the next sub pattern number
during the run (see below)
*****************************************************************************/

extern bool kr_getSubPatternByOrder(int *pattern, int *sub);
/*****************************************************************************
According to the last call to kr_initSubPatternOrder, the last call to
this function and the shuffle flags, the next position of pattern and
sub pattern is determined. This numbers are returned in <pattern> and
<sub> (beginning with 0). If there are no more sub pattern avaliable
the return value is FALSE, otherwise TRUE.
*****************************************************************************/

extern bool kr_getSubPatternByNo(int *pattern, int *sub, int n);
/*****************************************************************************
According to the current pattern set, the position of the <n>th sub
pattern is determined and returned in <pattern> (the pattern which
includes the subpattern) and <sub> (the sub pattern inside the
pattern) (beginning with 0). 
This function does not effect the ordering of the function
kr_getSubPatByOrder. <n> ranges from 0 to kr_TotalNoOfSubPatPairs()-1.
If the sub pattern is available, TRUE is returned, otherwise FALSE.
*****************************************************************************/

extern int kr_TotalNoOfSubPatPairs(void);
/*****************************************************************************
This function returns the total number of available sub patterns for
the current pattern set or 0 if no pattern set is defined.
The result is the sum of the numbers of subpattern for all patterns in
the current set.
*****************************************************************************/

extern int kr_NoOfSubPatPairs(int pattern);
/*****************************************************************************
This function returns the number of available sub patterns for the
pattern <pattern> of the current pattern set or 0 if this pattern is
not defined.
*****************************************************************************/

extern int kr_AbsPosOfFirstSubPat(int pattern);
/*****************************************************************************
This function returns the absolute position of the first sub pattern
of pattern <pattern> in the current pattern set. This position is
defined as the Sum of kr_NoOfSubPatPairs(i) where i runs from 0 to
<pattern>-1.
The absolute position of the first sub pattern of pattern 0 is 0.
The returned value may be used as argument for the function
kr_getSubPatternByNo.
*****************************************************************************/

extern int kr_TotalNoOfPattern(void);
/*****************************************************************************
This function returns the total number of available patterns for
the current pattern set or 0 if no pattern set is defined.
*****************************************************************************/

extern Patterns kr_getSubPatData(int pat_no, int sub_no, int io_type, 
				 int *size);
/*****************************************************************************
For the current pattern set and the specified sub pattern size, the
data array of the <sub_no>th sub pattern of the <pat_no>th pattern is
returned. io_type spcifies whether the input (INPUT) or output
(OUTPUT) data is requested. If <size> is != NULL the size of the data
array is returned is this parameter.
The function returns a pointer to the data array (type Patterns) or
NULL if an error occured.
*****************************************************************************/

extern int kr_getSubPatClass(int pat_no, int sub_no);
/*****************************************************************************
For the current pattern set and the specified sub pattern, the class index 
(>=0) is returned. If the pattern set does not provide class information, the 
return value is -1
*****************************************************************************/

extern int kr_SizeOfInputSubPat(void);
/*****************************************************************************
For the current pattern set and the specified sub pattern size, the size of 
the input part of the first sub pattern of the first pattern is returned. 
Negative return values indicate KernelErrorCode. Size 0 is a valid return value
since the pattern may contain no data.
*****************************************************************************/

extern int kr_SizeOfOutputSubPat(void);
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

extern krui_err kr_np_AllocatePatternSet(int *pat_set, int number);
/*****************************************************************************
A new set of patterns with <number> number of patterns is defined. An
internal pattern set number is assigned to reference this pattern set.
It is returned in <pat_set>.
******************************************************************************/

extern krui_err kr_np_AllocatePattern(np_pattern_descriptor *pattern,
			       bool input);
/*****************************************************************************
Depending on the entries input_dim, input_dim_sizes, input_fixsize resp.
output_dim, output_dim_sizes, output_fixsize of the pattern descriptor
<pattern> new memory for this pattern is allocated (input_pattern resp.
output_pattern). input_info resp. output_info is set to (char *) NULL.
The function allocates space for an input pattern if input is TRUE,
output pattern otherwise.
******************************************************************************/

extern krui_err kr_np_FreePattern(np_pattern_descriptor *pattern);
/*****************************************************************************
frees the area of the pattern data for this pattern
******************************************************************************/

extern krui_err kr_np_GetDescriptor(int pat_set, int number, 
			     np_pattern_descriptor **pattern);
/*****************************************************************************
A pointer to the pattern pattern descriptor of pattern <number> in
pattern set <pat_set> is returned in <pattern>. The structure pattern
must be provided by the calling routine. A call to this function makes
the specified pattern to become the current pattern.  Return value:
error status
******************************************************************************/

extern krui_err kr_np_lookupSym(int pat_set, char *symbol, 
				struct np_symtab **entry);
/*****************************************************************************
Looks for the class name <symbol> ind set <pat_set>. If the name is
not present up to now, a new entry is allocated and initialized. In
case of success <*entry> is set to the symbol table entry.
******************************************************************************/



#endif

/* 230 lines generated by deleteprivatedefinitions.awk */

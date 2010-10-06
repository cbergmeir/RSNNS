/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_newpattern.c,v $
  SHORTNAME      : newpattern
  SNNS VERSION   : 4.2

  PURPOSE        : handling of new pattern files;
  NOTES          :

  AUTHOR         : Michael Vogt
  DATE           : 10.9.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.28 $
  LAST CHANGE    : $Date: 1998/05/20 09:36:18 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef NeXT
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#else
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif
#include <string.h>
#include <time.h>
#include <sys/types.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "kr_typ.h"
#include "glob_typ.h"
#include "kr_mac.h"

#ifndef rand
#include "random.h"      /*  Randomize Library Function Prototypes  */
#endif

#include "kernel.h"
#include "kr_const.h"
#include "kr_funcs.h"
/* for dlvq */
#include "dlvq_learn.h"
#include "kr_newpattern.ph"
#include "kr_pat_scan.h"
#include "kr_pat_parse.h"


#ifndef __GNUC__
#ifndef __FUNCTION__
#define __FUNCTION__ "(unknown function)"
#endif
#endif

#if defined(DEBUG_PATTERN) && defined(TRACE_PATTERN)
static _t_indent = 0;
#define TRACE_IN(); \
{ \
    int _i; \
    if (_t_indent == 0) printf("\n"); \
    for (_i=0; _i<_t_indent; _i++) \
	printf(" "); \
    printf("%s\n", __FUNCTION__); \
    _t_indent += 2; \
}
#define TRACE_RETURN(x) \
{ \
    long _t_tmp; \
    int _i; \
    _t_tmp = (x); \
    _t_indent -= 2; \
    for (_i=0; _i<_t_indent; _i++) \
	printf(" "); \
    printf("<- %ld\n", _t_tmp); \
    return(_t_tmp); \
}
#define TRACE_RETURN_VOID() \
{ \
    _t_indent -= 2; \
    return; \
}
#define TRACE_INDENT() \
{ \
    int _i; \
    for (_i=0; _i<_t_indent-2; _i++) \
	printf(" "); \
} 
#else
#define TRACE_IN()
#define TRACE_RETURN(x) return(x)
#define TRACE_RETURN_VOID() return
#define TRACE_INDENT()
#endif

/*****************************************************************************
 FUNCTIONS WHICH ARE CALLED BY THE KERNEL USER INTERFACE TO PERFORM
 THE KERNEL INTERFACE OF THE NEW PATTERN MANAGEMENT
******************************************************************************/

/*****************************************************************************
  FUNCTION : kr_npui_setCurrPatSet

  PURPOSE  : determines the number of the current pattern set (in
             kernel terminology) numbering starts with 0
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_setCurrPatSet(int number)
{
    TRACE_IN();

    if (number<0 || number>=npui_number_pat_sets)
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    npui_curr_pat_set = number;
    npui_curr_pattern = 1;
    npui_train_defined = FALSE;
    npui_show_defined = FALSE;
    np_abs_count_valid = FALSE;
    np_sub_pat_sizes_valid = FALSE;
    np_pat_mapping_valid = FALSE;
    
    /* for dlvq */
    newPatternsLoaded = 1;

    TRACE_RETURN(kr_np_ValidateInfo(npui_pat_sets[npui_curr_pat_set]));
}

/*****************************************************************************
  FUNCTION : kr_npui_deletePatSet

  PURPOSE  : deletes the specified pattern set from memory and
             undefines the current pattern set, pattern, training
	     scheme and display scheme
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_deletePatSet(int number)
{
    int i;
    int pat_set;

    TRACE_IN();

    if (number<0 || number>=npui_number_pat_sets)
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    pat_set = npui_pat_sets[number];

    for (i=number; i<npui_number_pat_sets-1; i++)
	npui_pat_sets[i] = npui_pat_sets[i+1];
    npui_number_pat_sets--;
    npui_curr_pat_set = -1;
    npui_curr_pattern = -1;
    npui_train_defined = FALSE;
    npui_show_defined = FALSE;
    np_abs_count_valid = FALSE;
    np_sub_pat_sizes_valid = FALSE;
    np_pat_mapping_valid = FALSE;
    
    TRACE_RETURN(kr_np_DeletePatternSet(pat_set));
}

/*****************************************************************************
  FUNCTION : kr_npui_GetPatInfo

  PURPOSE  : retrieves all available information concerning the
             current pattern set and the current pattern which both
	     must be defined. The given parameter fields are filled
	     with the information.
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_GetPatInfo(pattern_set_info *set_info, 
                            pattern_descriptor *pat_info)
{
    np_pattern_set_info int_pat_set_info;
    np_pattern_descriptor *int_pat_info;
    krui_err err_code;

    TRACE_IN();

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN_SET); }

    if (npui_curr_pattern == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN); }

    err_code = kr_np_GetInfo(npui_pat_sets[npui_curr_pat_set],
			     &int_pat_set_info);
    if (err_code != KRERR_NO_ERROR)
	{ TRACE_RETURN(err_code); }

    *set_info = int_pat_set_info.pub;

    err_code = kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
		   kr_np_virtual_to_physical(npui_curr_pattern-1),
		   &int_pat_info);
    if (err_code != KRERR_NO_ERROR)
	{ TRACE_RETURN(err_code); }

    *pat_info = int_pat_info->pub;

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_npui_DefShowSubPat

  PURPOSE  : Define the display scheme:
             Size and position of a sub pattern for the current
             pattern in the current pattern set is defined. <insize>
	     is a pointer to an array of integer values which define
	     the dimensional sizes of the input sub pattern. <inpos>
	     is a pointer to an array of integer values which defines
	     the offset (position) of this sub pattern inside the
	     pattern. <outsize> and <outpos> are used to define the
	     respective output sub pattern 
  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_DefShowSubPat(int *insize, int *outsize, 
                               int *inpos, int *outpos)
{
    krui_err err_code;
    np_pattern_descriptor *pattern;
    int i;

    TRACE_IN();

    npui_show_defined = FALSE;

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN_SET); }

    if (npui_curr_pattern == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN); }

    err_code = kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
		   kr_np_virtual_to_physical(npui_curr_pattern-1),
		   &pattern);

    if (err_code != KRERR_NO_ERROR)
	{ TRACE_RETURN(err_code); }

    for (i=0; i<pattern->pub.input_dim; i++)
    {
	if ((pattern->pub.input_dim_sizes)[i] < insize[i] + inpos[i] - 1)
	    err_code = KRERR_NP_DIMENSION;
    }
    for (i=0; i<pattern->pub.output_dim; i++)
    {
	if ((pattern->pub.output_dim_sizes)[i] < outsize[i] + outpos[i] - 1)
	    err_code = KRERR_NP_DIMENSION;
    }

    if (err_code != KRERR_NO_ERROR)
	{ TRACE_RETURN(err_code); }

    memcpy(npui_insize, insize, MAX_NO_OF_VAR_DIM * sizeof(int));
    memcpy(npui_outsize, outsize, MAX_NO_OF_VAR_DIM * sizeof(int));
    memcpy(npui_inpos, inpos, MAX_NO_OF_VAR_DIM * sizeof(int));
    memcpy(npui_outpos, outpos, MAX_NO_OF_VAR_DIM * sizeof(int));
    for (i=0; i<MAX_NO_OF_VAR_DIM; i++)
    {
	npui_inpos[i]--;
	npui_outpos[i]--;
    }

    npui_show_defined = TRUE;

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_npui_DefTrainSubPat

  PURPOSE  : Define the training scheme:
             Size and step size of sub pattern for the current pattern
             in the current pattern set is defined for training and
	     testing. <insize> is a pointer to an array of integer
	     values which define the dimensional sizes of the input sub
	     pattern. <instep> is a pointer to an array of integer
	     values which defines the step size which is used to move
	     the sub pattern over the pattern.
             <outsize> and <outpos> are used to define the respective
	     output sub pattern. 
             <max_n_pos> (if not NULL) returns the number of valid
	     input sub pattern positions for the current pattern and
	     the given training scheme.

  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_DefTrainSubPat(int *insize, int *outsize, 
                                int *instep, int *outstep, int *max_n_pos)
{
    krui_err err_code;
    int n;
    np_pattern_descriptor *p;
    int changes = 0;

    TRACE_IN();

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN_SET); }

    if (npui_curr_pattern == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN); }

    if (npui_train_defined)
    {
	for (n=0; n<MAX_NO_OF_VAR_DIM && changes==0; n++)
	    if (npui_insize[n] != insize[n] ||
		npui_outsize[n] != outsize[n] ||
		npui_instep[n] != instep[n] ||
		npui_outstep[n] != outstep[n])
		changes++;
    }

    if (!npui_train_defined  || changes != 0)
    {
	npui_train_defined = FALSE;
	np_abs_count_valid = FALSE;
	np_sub_pat_sizes_valid = FALSE;

	memcpy(npui_insize, insize, MAX_NO_OF_VAR_DIM * sizeof(int));
	memcpy(npui_outsize, outsize, MAX_NO_OF_VAR_DIM * sizeof(int));
	memcpy(npui_instep, instep, MAX_NO_OF_VAR_DIM * sizeof(int));
	memcpy(npui_outstep, outstep, MAX_NO_OF_VAR_DIM * sizeof(int));
	
	err_code = 
	    kr_np_DefineSubPatternOrdering(npui_pat_sets[npui_curr_pat_set],
					   TRUE, npui_insize, npui_instep);
	if (err_code != KRERR_NO_ERROR)
	    { TRACE_RETURN(err_code); }

	err_code = 
	    kr_np_DefineSubPatternOrdering(npui_pat_sets[npui_curr_pat_set],
					   FALSE, npui_outsize,
					   npui_outstep);
	if (err_code != KRERR_NO_ERROR)
	    { TRACE_RETURN(err_code); }

	npui_train_defined = TRUE;

	err_code = kr_np_ValidateInfo(npui_pat_sets[npui_curr_pat_set]);
	if (err_code != KRERR_NO_ERROR)
	    { TRACE_RETURN(err_code); }
    }

    if (max_n_pos != (int *) NULL)
    {
	err_code = kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
		       kr_np_virtual_to_physical(npui_curr_pattern-1), &p);
	if (err_code != KRERR_NO_ERROR)
	    { TRACE_RETURN(err_code); }

	if (!kr_np_gen_sub_pos(p->pub.input_dim, &n, p->pub.input_dim_sizes, 
			       npui_insize, npui_instep, (int *) NULL, 
			       TRUE))
	    { TRACE_RETURN(KRERR_NP_DIMENSION); }
	*max_n_pos = n;
    }

    { TRACE_RETURN(KRERR_NO_ERROR); }
}


/*****************************************************************************
  FUNCTION : kr_npui_AlignSubPat

  PURPOSE  : Align the position of a sub pattern:
             Using the current training scheme and the current pattern
	     of the current pattern set, the given position of an
	     input sub pattern <inpos> and the given position of the
	     corresponding output sub pattern <outpos> is aligned to fit
	     the currently defined training scheme.
	     E.g. if the training scheme defines a step width of 5 for
	     a specific dimension, only the positions 0, 5, 10, 15 ...
	     are valid positions for a sub pattern.
	     The position of each dimension is aligned independently
	     from all other dimensions by moving to the next valid
	     position which is lower or equal to the given position.
             <no> (if not NULL) returns the number of the sub pattern
	     which corresponds to the new aligned position which is
	     returned in place (<inpos> <outpos>).

  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_AlignSubPat(int *inpos, int *outpos, int *no)
{
    krui_err err_code;
    np_pattern_descriptor *p;
    int pos;
    int i;

    TRACE_IN();

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN_SET); }

    if (npui_curr_pattern == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN); }

    if (npui_train_defined == FALSE)
	{ TRACE_RETURN(KRERR_NP_NO_TRAIN_SCHEME); }

    err_code = kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
		   kr_np_virtual_to_physical(npui_curr_pattern-1), &p);
    if (err_code != KRERR_NO_ERROR)
	{ TRACE_RETURN(err_code); }

    /* enumeration starts with 0, not with 1 */
    for (i=0; i< p->pub.input_dim; i++)
	inpos[i] -= 1;

    /* align the position of the input sub pattern */
    if (!kr_np_align_sub_pos(p->pub.input_dim, &pos, 
			     p->pub.input_dim_sizes, npui_insize, 
			     npui_instep, inpos))
    {
	for (i=0; i< p->pub.input_dim; i++)
	    inpos[i] = 1;
	{ TRACE_RETURN(KRERR_NP_DIMENSION); }
    }

    /* now enumeration starts with 1 again */
    for (i=0; i< p->pub.input_dim; i++)
	inpos[i] += 1;

    /* find the output sub pattern which is related to the aligned */
    /* input sub pattern */
    if (!kr_np_gen_sub_pos(p->pub.output_dim, &pos, 
			   p->pub.output_dim_sizes, npui_outsize, 
			   npui_outstep, outpos, FALSE))
    {
	for (i=0; i< p->pub.output_dim; i++)
	    outpos[i] = 1;
	{ TRACE_RETURN(KRERR_NP_DIMENSION); }
    }

    /* now enumeration starts with 1 again */
    for (i=0; i< p->pub.output_dim; i++)
	outpos[i] += 1;

    /* return the absolute position */
    *no = pos+1;

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_npui_allocNewPatternSet

  PURPOSE : Allocate an (additional) empty pattern set: A new pattern
  set is allocated if the maximum number of loaded pattern sets
  (NO_OF_PAT_SETS) is not exceeded. The corresponding pattern set
  handle is returned in <set_no>.  The new allocated pattern set
  becomes the current set.  There is no current pattern defined.
  Training scheme and display scheme both become undefined.

  NOTES    : 

  RETURNS  : 
  UPDATE   : 
******************************************************************************/
krui_err kr_npui_allocNewPatternSet(int *set_no)
{
    int pat_set;
    krui_err err_code;

    TRACE_IN();

    if (npui_number_pat_sets >= NO_OF_PAT_SETS)
	{ TRACE_RETURN(KRERR_NP_NO_MORE_ENTRIES); }

    /* allocate pattern set with zero pattern */
    err_code = kr_np_AllocatePatternSet(&pat_set, 0);
    if (err_code == KRERR_NO_ERROR)
    {
	npui_curr_pat_set = npui_number_pat_sets;
	npui_number_pat_sets++;
	npui_curr_pattern = -1;
	npui_pat_sets[npui_curr_pat_set] = pat_set;
	*set_no = npui_curr_pat_set;
	npui_train_defined = FALSE;
	npui_show_defined = FALSE;
	np_abs_count_valid = FALSE;
	np_sub_pat_sizes_valid = FALSE;
	np_pat_mapping_valid = FALSE;
	np_info_valid[pat_set] = FALSE;
	err_code = kr_np_ValidateInfo(pat_set);
    }

    { TRACE_RETURN(err_code); }
}

/*****************************************************************************
  FUNCTION : kr_npui_loadNewPatterns

  PURPOSE  : Load an (additional) pattern file:
             The file with name <filename> is loaded into memory if
	     existent and if the maximum number of loaded pattern sets
	     (NO_OF_PAT_SETS) is not exceeded. The corresponding
	     pattern set handle is returned in <set_no>.

	     The new loaded pattern set becomes the current set. The
	     first pattern inside this set becomes the current
	     pattern. Training scheme and display scheme both become
	     undefined.

  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_loadNewPatterns(char *filename, int *set_no)
{
    FILE *infile;
    int pat_set;
    int read_from_pipe = 0;
    char *buf;
    krui_err err_code;

    TRACE_IN();

    if (npui_number_pat_sets >= NO_OF_PAT_SETS)
	{ TRACE_RETURN(KRERR_NP_NO_MORE_ENTRIES); }

    if (access(filename, F_OK) != 0)
	{ TRACE_RETURN(KRERR_FILE_OPEN); }
    if (strcmp(&filename[strlen(filename)-2], ".Z") == 0)
    {
	buf = (char *) malloc(strlen(filename)+strlen("zcat ")+1);
	if (buf == (char *) NULL)
	    { TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
	sprintf(buf, "zcat %s", filename);
	infile = popen(buf,"r");
	read_from_pipe = 1;
    }
    else if (strcmp(&filename[strlen(filename)-3], ".gz") == 0)
    {
	buf = (char *) malloc(strlen(filename)+strlen("gunzip -c ")+1);
	if (buf == (char *) NULL)
	    { TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
	sprintf(buf, "gunzip -c %s", filename);
	infile = popen(buf,"r");
	read_from_pipe = 1;
    }
    else
	infile = fopen(filename, "r");
    if (infile == (FILE *) NULL)
	{ TRACE_RETURN(KRERR_FILE_OPEN); }

    err_code = kr_np_LoadPatternFile(infile, &pat_set);
    if (err_code == KRERR_NO_ERROR)
    {
	/* for dlvq */
	newPatternsLoaded = 1;
	
	npui_curr_pat_set = npui_number_pat_sets;
	npui_number_pat_sets++;
	npui_curr_pattern = 1;
	npui_pat_sets[npui_curr_pat_set] = pat_set;
	*set_no = npui_curr_pat_set;
	npui_train_defined = FALSE;
	npui_show_defined = FALSE;
	np_abs_count_valid = FALSE;
	np_sub_pat_sizes_valid = FALSE;
	np_pat_mapping_valid = FALSE;
	np_info_valid[pat_set] = FALSE;
	err_code = kr_np_ValidateInfo(pat_set);
    }

    if (read_from_pipe)
    {
	pclose(infile);
	free(buf);
    }
    else
	fclose(infile);

    { TRACE_RETURN(err_code); }
}

/*****************************************************************************
  FUNCTION : kr_npui_saveNewPatterns

  PURPOSE  : The given pattern set <set_no> is written to file
             <filename> in new style format. No side effects.

  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_saveNewPatterns(char *filename, int set_no)
{
    FILE *outfile;
    krui_err err_code;

    TRACE_IN();

    if (set_no<0 || set_no>=npui_number_pat_sets)
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    outfile = fopen(filename, "w");
    if (outfile == (FILE *) NULL)
	{ TRACE_RETURN(KRERR_FILE_OPEN); }

    err_code = kr_np_SavePatternFile(outfile, npui_pat_sets[set_no]);

    fclose(outfile);

    { TRACE_RETURN(err_code); }
}

/*****************************************************************************
  FUNCTION : kr_npui_GetShapeOfSubPat

  PURPOSE  : Get the shape of a sub pattern which is specified by a
             number:
             After kr_npui_DefTrainSubPat has been called for the
	     current pattern set and a current pattern is defined,
	     this function retrieves the <n_pos>th valid sub pattern
	     pair which matches the defined training scheme. Size and
	     position of the sub pattern pair is returned in <insize>
	     <inpos> <outsize> and <outpos> which are all pointer to
	     integer arrays.

  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_GetShapeOfSubPat(int *insize, int *outsize, 
                                int *inpos, int *outpos, int n_pos)
{
    int sp[MAX_NO_OF_VAR_DIM];
    np_pattern_descriptor *p;
    krui_err err_code;
    int n;
    int i;

    TRACE_IN();

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN_SET); }

    if (npui_curr_pattern == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN); }

    if (!npui_train_defined)
	{ TRACE_RETURN(KRERR_NP_NO_TRAIN_SCHEME); }

    err_code = kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
		   kr_np_virtual_to_physical(npui_curr_pattern-1), &p);
    if (err_code != KRERR_NO_ERROR)
	{ TRACE_RETURN(err_code); }

    n = n_pos-1;
    if (!kr_np_gen_sub_pos(p->pub.input_dim, &n, p->pub.input_dim_sizes, 
			   npui_insize, npui_instep, sp, FALSE))
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN); }
    
    memcpy((char *) inpos, (char *) sp, p->pub.input_dim * sizeof(int));
    for (i=0; i<p->pub.input_dim; i++)
	inpos[i]++;

    if (!kr_np_gen_sub_pos(p->pub.output_dim, &n, p->pub.output_dim_sizes, 
			   npui_outsize, npui_outstep, sp, FALSE))
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN); }

    memcpy((char *) outpos, (char *) sp, p->pub.output_dim *sizeof(int));
    for (i=0; i<p->pub.output_dim; i++)
	outpos[i]++;

    memcpy((char *) insize, (char *) npui_insize, p->pub.input_dim *sizeof(int));
    memcpy((char *) outsize, (char *) npui_outsize, p->pub.input_dim *sizeof(int));

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_npui_setRemapFunction

  PURPOSE  : 

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

  RETURNS  : kernel error code
  NOTES    : 

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_setRemapFunction(char *name, float *params)
{
    krui_err err_code = KRERR_NO_ERROR;
    krui_err err_code_2 = KRERR_NO_ERROR;
    int pattern_set;
    int use_default = 0;
    int i;
    struct FuncInfoDescriptor func_descr;
    int has_changed = 0;

    TRACE_IN();

#ifdef DEBUG_REMAP
    TRACE_INDENT();
    printf("%s(%s, %s)\n", __FUNCTION__, 
	   name ? name : "<default>",
	   params ? "float[]" : "NULL");
#endif

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN_SET); }

    pattern_set = npui_pat_sets[npui_curr_pat_set];

    if (name == NULL)
	use_default = 1;
    else
    {
	func_descr.func_type = REMAP_FUNC;
	err_code = krf_getFuncInfo(GET_DEFAULT_FUNC, &func_descr);
	if (err_code != KRERR_NO_ERROR ||
	    strcmp(func_descr.func_name, name) == 0)
	    use_default = 1;
	else
	{
	    func_descr.func_type = REMAP_FUNC;
	    strcpy(func_descr.func_name, name);
	    err_code = krf_getFuncInfo(SEARCH_FUNC, &func_descr);
	    if (err_code == KRERR_NO_ERROR)
	    {	    
		if (np_info[pattern_set].rmf_ptr !=
		    (RemapFuncPtr) func_descr.function)
		    has_changed++;

		np_info[pattern_set].rmf_ptr = 
		    (RemapFuncPtr) func_descr.function;
		for (i=0; i<func_descr.no_of_input_parameters; i++)
		    np_info[pattern_set].pub.remap_params[i] = params[i];
		np_info[pattern_set].pub.no_of_remap_params = 
		    func_descr.no_of_input_parameters;
		if (np_info[pattern_set].pub.remap_function != NULL)
		    free(np_info[pattern_set].pub.remap_function);
		np_info[pattern_set].pub.remap_function = strdup(name);
		if (np_info[pattern_set].pub.remap_function == NULL)
		    err_code = KRERR_INSUFFICIENT_MEM;
	    }
	    else
	    {
		use_default = 1;
	    }
	}
    }

    if (use_default)
    {
	if (np_info[pattern_set].rmf_ptr != NULL)
	    has_changed++;
	np_info[pattern_set].rmf_ptr = NULL;
	if (np_info[pattern_set].pub.remap_function != NULL)
	    free(np_info[pattern_set].pub.remap_function);
	np_info[pattern_set].pub.remap_function = NULL;		
	np_info[pattern_set].pub.no_of_remap_params = 0;
    }

    if (has_changed>0)
    {
	/* everything is done already, so I don't have to do this (?): */
	/*
	np_info_valid[npui_pat_sets[npui_curr_pat_set]] = FALSE;
	err_code_2 = kr_np_ValidateInfo(npui_pat_sets[npui_curr_pat_set]);
	*/
    }

    { TRACE_RETURN(err_code != KRERR_NO_ERROR ? err_code : err_code_2); }
}


/*****************************************************************************
  FUNCTION : kr_npui_setClassDistribution

  PURPOSE  : 

Set the class distribution for the current pattern set.
This function may only be called if the current pattern set provides class 
information. If <class_sizes> is not NULL it must locate an integer array of 
size that matches the number of classes in the current set. The values of 
this array are copied to the pattern set area. If <class_sizes> is the NULL
pointer, then the real distribution of classes in the pattern set is 
computed and stored in the class distribution area of the pattern set. So the
interface may retrieve default values for the class distribution which match
the real distribution of the pattern set.

  RETURNS  : kernel error code
  NOTES    : 

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_setClassDistribution(unsigned int *class_sizes)
{
    int has_changed = 0;
    krui_err err_code = KRERR_NO_ERROR;
    struct np_symtab *list;
    int index;
    int chunk_sum;

    TRACE_IN();

#ifdef DEBUG_CLASSES
    TRACE_INDENT();
    printf("%s(%s)\n", __FUNCTION__, class_sizes ? "unsigned int[]" : "NULL");
#endif

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN_SET); }

    if (np_info[npui_pat_sets[npui_curr_pat_set]].pub.classes <= 0)
	{ TRACE_RETURN(KRERR_NO_CLASSES); }

    list = np_st[npui_pat_sets[npui_curr_pat_set]];
    if (class_sizes == NULL)
    {
	while (list)
	{
	    if (list->chunk_amount != list->set_amount)
	    {
		list->chunk_amount = list->set_amount;
		has_changed++;
	    }
	    list = list->next;
	}
    }
    else
    {
	chunk_sum = 0;
	for (index=0; index<np_info[npui_pat_sets[npui_curr_pat_set]].pub.classes; 
	     index++)
	    chunk_sum += class_sizes[index];
	if (chunk_sum == 0)
	    { TRACE_RETURN(KRERR_ILL_CLASS_DISTRIB); }

	index = 0;
	while (list)
	{
	    if (list->chunk_amount != class_sizes[index])
	    {
		list->chunk_amount = class_sizes[index];
		has_changed++;
	    }
	    list = list->next;
	    index++;
	}
	if (index != np_info[npui_pat_sets[npui_curr_pat_set]].pub.classes)
	    fprintf(stderr, "internal error, file %s, line %d", __FILE__, __LINE__);
    }

    if (has_changed > 0)
    {
	np_pat_mapping_valid = FALSE;
	np_abs_count_valid = FALSE;
	err_code = kr_np_ValidateInfo(npui_pat_sets[npui_curr_pat_set]);
    }

    { TRACE_RETURN(err_code); }
}

/*****************************************************************************
  FUNCTION : kr_npui_useChunk

  PURPOSE  : 

Switch on or off the class distribution process.
If <active> is TRUE, the distribution process is activated. Therefore the 
internal representation of the pattern set is virtually reorganized to match
the requested values of <class_sizes>. After that step the number of patterns
in the set virtually increases or decreases because each epoch must cover 
every pattern of all classes that have a requested distribution amount bigger
than 0. This also implies that several patterns from a specific class are 
reused several times during one epoch.

  RETURNS  : kernel error code
  NOTES    : 

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_useChunk(bool active)
{
    krui_err err_code = KRERR_NO_ERROR;

    TRACE_IN();

#ifdef DEBUG_CLASSES
    TRACE_INDENT();
    printf("%s(%s)\n", __FUNCTION__, active ? "TRUE" : "FALSE");
#endif

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN_SET); }

    if (np_info[npui_pat_sets[npui_curr_pat_set]].pub.classes <= 0)
	{ TRACE_RETURN(KRERR_NO_CLASSES); }

    if (np_info[npui_pat_sets[npui_curr_pat_set]].pub.class_distrib_active !=
	active)
    {
	np_info[npui_pat_sets[npui_curr_pat_set]].pub.class_distrib_active =
	active;

	np_pat_mapping_valid = FALSE;
	np_abs_count_valid = FALSE;
	err_code = kr_np_ValidateInfo(npui_pat_sets[npui_curr_pat_set]);
    }

    { TRACE_RETURN(err_code); }
}

/*****************************************************************************
  FUNCTION : kr_npui_setClass

  PURPOSE  : 

Set the class name for the current pattern.
If the pattern set already contains class information, only the
current pattern is affected. If there are no classes in the pattern
set so far, all patterns of the current set are set to the given class
name, since every pattern must belong to a class in that case.
If the given class name is the NULL pointer, nothing happens at all.

  RETURNS  : kernel error code
  NOTES    : 

  UPDATE   : 
******************************************************************************/
krui_err kr_npui_setClass(char *classname)
{
    np_pattern_descriptor *p;
    krui_err err_code = KRERR_NO_ERROR;
    struct np_symtab *old_entry;
    struct np_symtab *new_entry;
    int phys_num;

    TRACE_IN();

#ifdef DEBUG_CLASSES
    TRACE_INDENT();
    printf("%s(%s)\n", __FUNCTION__, classname ? classname : "NULL");
#endif

    if (!classname)
	{ TRACE_RETURN(KRERR_NO_ERROR); }

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN_SET); }

    if (npui_curr_pattern == -1)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN); }

    phys_num = kr_np_virtual_to_physical(npui_curr_pattern-1);
    err_code = kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
		   phys_num, &p);
    if (err_code != KRERR_NO_ERROR)
	{ TRACE_RETURN(err_code); }

    old_entry = p->mysym;
    err_code = kr_np_lookupSym(npui_pat_sets[npui_curr_pat_set],
				   classname, &new_entry);
    if (err_code != KRERR_NO_ERROR)
	{ TRACE_RETURN(err_code); }

    if (old_entry == NULL)
    {
	int num, i;

	/* all physical patterns are used here! */
	num = np_info[npui_pat_sets[npui_curr_pat_set]].pub.number_of_pattern;

	p = np_pat_sets[npui_pat_sets[npui_curr_pat_set]];
	for (i=0; i<num; i++)
	{
	    p->mysym = new_entry;
	    p++;
	    new_entry->set_amount++;
	}
	new_entry->chunk_amount = new_entry->set_amount;
	np_info[npui_pat_sets[npui_curr_pat_set]].pub.classes++;
    }
    else
    {
	if (new_entry == old_entry)
	{ TRACE_RETURN(KRERR_NO_ERROR); }
	else
	{
	    old_entry->set_amount--;
	    if (old_entry->set_amount < old_entry->chunk_amount)
		old_entry->chunk_amount = old_entry->set_amount;
	    if (old_entry->set_amount <= 0)
	    {
		old_entry->chunk_amount = 0;
		np_info[npui_pat_sets[npui_curr_pat_set]].pub.classes--;
		kr_np_delSym(npui_pat_sets[npui_curr_pat_set], old_entry);
	    }

	    if (new_entry->set_amount == 0)
		np_info[npui_pat_sets[npui_curr_pat_set]].pub.classes++;
	    new_entry->set_amount++;
	    if (new_entry->chunk_amount == 0)
		new_entry->chunk_amount = 1;
	}
    }
    p->mysym = new_entry;
    np_pat_mapping_valid = FALSE;
    np_abs_count_valid = FALSE;
    err_code = kr_np_ValidateInfo(npui_pat_sets[npui_curr_pat_set]);

    npui_curr_pattern = 1 + kr_np_physical_to_virtual(phys_num);

    { TRACE_RETURN(err_code); }
}

/*****************************************************************************
 FUNCTIONS WHICH ARE CALLED BY OTHER KERNEL FUNCTIONS LIKE TRAINING
 AND INITIALIZATION FUNCTIONS:
******************************************************************************/

/*****************************************************************************
  FUNCTION : kr_np_pattern

  PURPOSE  : multiple pattern handling functions depending on mode and mode1
  RETURNS  : result, depending on operation or kernel error code
  NOTES    : KernelErrorCode is set to the valid kernel error code

  UPDATE   : 
******************************************************************************/
int  kr_np_pattern(int mode ,int mode1 ,int pattern_no)
{
    int return_code;
    int new_pattern;
    np_pattern_set_info info;
    np_pattern_descriptor *pattern;
    np_pattern_descriptor *c_pattern;


    TRACE_IN();

#ifdef DEBUG_PATTERN
    TRACE_INDENT();
    printf("%s(", __FUNCTION__);
    switch (mode)
    {
      case PATTERN_SET: printf("PATTERN_SET"); break;
      case PATTERN_GET: printf("PATTERN_GET"); break;
      case PATTERN_DELETE: printf("PATTERN_DELETE"); break;
      case PATTERN_MODIFY: printf("PATTERN_MODIFY"); break;
      case PATTERN_SHOW: printf("PATTERN_SHOW"); break;
      case PATTERN_NEW: printf("PATTERN_NEW"); break;
      case PATTERN_DELETE_ALL: printf("PATTERN_DELETE_ALL"); break;
      case PATTERN_SHUFFLE_ON: printf("PATTERN_SHUFFLE_ON"); break;
      case PATTERN_SHUFFLE_OFF: printf("PATTERN_SHUFFLE_OFF"); break;
      case PATTERN_VECTOR_NEW: printf("PATTERN_VECTOR_NEW"); break;
      case PATTERN_ARRAY_ALLOC: printf("PATTERN_ARRAY_ALLOC"); break;
      case PATTERN_SET_NUMBER: printf("PATTERN_SET_NUMBER"); break;
      case PATTERN_GET_NUMBER: printf("PATTERN_GET_NUMBER"); break;
      case PATTERN_SUB_SHUFFLE_ON: printf("PATTERN_SUB_SHUFFLE_ON"); break;
      case PATTERN_SUB_SHUFFLE_OFF: printf("PATTERN_SUB_SHUFFLE_OFF"); break;
      case GET_SHUFFLE_FLAG: printf("GET_SHUFFLE_FLAG"); break;
      case GET_SUB_SHUFFLE_FLAG: printf("GET_SUB_SHUFFLE_FLAG"); break;
      case SUBPATTERN_GET_NUMBER: printf("SUBPATTERN_GET_NUMBER"); break;
      default: printf("%d", mode); break;
    }
    printf(", %d, %d)\n", mode1, pattern_no);
#endif

    return_code = (int) (KernelErrorCode = KRERR_NO_ERROR);

    if (npui_curr_pat_set == -1)
    {
	KernelErrorCode = KRERR_NP_NO_CURRENT_PATTERN_SET;
	TRACE_RETURN((int) KernelErrorCode);
    }

    /* for some modes, test current pattern definition */ 
    switch (mode)
    {
      case  PATTERN_GET:	/*  returns the current pattern  */
      case  PATTERN_DELETE:	/*  delete the current pattern pair  */
      case  PATTERN_MODIFY:	/*  modify the current pattern pair  */
      case  PATTERN_SHOW:	/*  show pattern  */
	if (npui_curr_pattern == -1 && mode != PATTERN_SET)
	{
	    KernelErrorCode = KRERR_NP_NO_CURRENT_PATTERN;
	    TRACE_RETURN((int) KernelErrorCode);
	}
      default:
	break;
    }

    switch (mode)
    {
      case  PATTERN_SET:	/*  set the current pattern  */
	if (pattern_no < 1 || pattern_no >
	np_info[npui_pat_sets[npui_curr_pat_set]].pub.virtual_no_of_pattern)
	{
	    return_code = (int) (KernelErrorCode = KRERR_PATTERN_NO);
	    npui_curr_pattern = -1;
	}
	else
	    npui_curr_pattern = pattern_no;
	break;

      case  PATTERN_GET:	/*  returns the current pattern  */
	return_code = npui_curr_pattern;
	break;

      case  PATTERN_DELETE:	/*  delete the current pattern pair  */
	return_code = kr_np_DeletePattern(npui_pat_sets[npui_curr_pat_set],
			  kr_np_virtual_to_physical(npui_curr_pattern-1));
	npui_curr_pattern = -1;

	np_info_valid[npui_pat_sets[npui_curr_pat_set]] = FALSE;
	np_pat_mapping_valid = FALSE;
	np_abs_count_valid = FALSE;
	return_code = kr_np_ValidateInfo(npui_pat_sets[npui_curr_pat_set]);

	break;

      case  PATTERN_MODIFY:	/*  modify the current pattern pair  */
	return_code = (int) (KernelErrorCode = kr_np_modifyPattern());
	break;

      case  PATTERN_SHOW:	/*  show pattern  */
	return_code = (int) (KernelErrorCode = kr_np_showPatternSTD(mode1));
	break;

      case  PATTERN_NEW:	/*  new pattern  */
	/* before creating a new pattern, first ensure that new pattern */
	/* fits into existing set */
	return_code = kr_np_GetInfo(npui_pat_sets[npui_curr_pat_set], &info);
	if (return_code != KRERR_NO_ERROR)
	    break;
	(void) kr_IOCheck();
	if (NoOfInputUnits == 0)
	{
	    return_code = KernelErrorCode = KRERR_NO_INPUT_UNITS;
	    break;
	}

	if (info.pub.number_of_pattern > 0)
	{
	    if (info.pub.in_number_of_dims != 0 || 
		info.pub.out_number_of_dims != 0 ||
		info.pub.in_fixsize != NoOfInputUnits || 
		 info.pub.out_fixsize != NoOfOutputUnits)
	    {
		/* creation of variable sized patterns is not possible */
		/* also the new pattern must be of equal size compared to */
		/* existing pattern */
		return_code = KRERR_NP_INCOMPATIBLE_NEW;
		break;
	    }
	}

	if (npui_curr_pattern != -1)
	    (void) kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
		  kr_np_virtual_to_physical(npui_curr_pattern-1), 
		  &c_pattern);
	else
	    c_pattern = NULL;

	return_code = KernelErrorCode =
	    kr_np_AddPattern(npui_pat_sets[npui_curr_pat_set], &new_pattern);
	if (return_code != KRERR_NO_ERROR)
	    break;

	(void) kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
				   new_pattern, &pattern);

	if (np_info[npui_pat_sets[npui_curr_pat_set]].pub.classes > 0)
	{
	    /* the new pattern gets a symbol which is in use for the
	       current chunk composition */
	    if (c_pattern != NULL)
		pattern->mysym = c_pattern->mysym;
	    else
	    {
		pattern->mysym = np_st[npui_pat_sets[npui_curr_pat_set]];
		while (pattern->mysym && pattern->mysym->chunk_amount <= 0)
		    pattern->mysym = pattern->mysym->next;
		if (pattern->mysym == NULL)
		{
		    pattern->mysym = np_st[npui_pat_sets[npui_curr_pat_set]];
		    pattern->mysym->chunk_amount = 1;
		}
	    }
	    pattern->mysym->set_amount++;
	}

	
	/* update internal information which is used to compute the new 
	   current pattern. To do this, the right pattern sizes must be set. 
	   They must be reset to 0 to allow allocation of memory space 
	   during later pattern modifikation */
	pattern->pub.input_fixsize = NoOfInputUnits;
	pattern->pub.output_fixsize = NoOfOutputUnits;
	np_info_valid[npui_pat_sets[npui_curr_pat_set]] = FALSE;
	np_pat_mapping_valid = FALSE;
	np_abs_count_valid = FALSE;
	return_code = kr_np_ValidateInfo(npui_pat_sets[npui_curr_pat_set]);
	npui_curr_pattern = 1 + kr_np_physical_to_virtual(new_pattern);
	pattern->pub.input_fixsize = 0;
	pattern->pub.output_fixsize = 0;

	return_code = (int) (KernelErrorCode = kr_np_modifyPattern());
	if (return_code != KRERR_NO_ERROR)
	{
	    (void) kr_np_DeletePattern(npui_pat_sets[npui_curr_pat_set],
				       new_pattern);
	    npui_curr_pattern = -1;

	    np_info_valid[npui_pat_sets[npui_curr_pat_set]] = FALSE;
	    np_pat_mapping_valid = FALSE;
	    np_abs_count_valid = FALSE;
	    (void) kr_np_ValidateInfo(npui_pat_sets[npui_curr_pat_set]);
	    break;
	}

	break;

      case  PATTERN_DELETE_ALL:	/*  delete all pattern  */
	break;

      case  PATTERN_SHUFFLE_ON:	/*  shuffle pattern  */
	npui_shuffle_pattern = TRUE;
	break;

      case  PATTERN_SHUFFLE_OFF: /*  shuffle pattern off */
	npui_shuffle_pattern = FALSE;
	break;

      case  PATTERN_SET_NUMBER:
	break;

      case  PATTERN_GET_NUMBER:
	return_code = 
	    np_info[npui_pat_sets[npui_curr_pat_set]].pub.virtual_no_of_pattern;
	break;

      case  SUBPATTERN_GET_NUMBER:
	return_code = 
            kr_TotalNoOfSubPatPairs();
	break;

	
      case  PATTERN_SUB_SHUFFLE_ON:
	npui_shuffle_sub_pattern = TRUE;
	break;

      case  PATTERN_SUB_SHUFFLE_OFF:
	npui_shuffle_sub_pattern = FALSE;
	break;

      case GET_SHUFFLE_FLAG:
	return_code = npui_shuffle_pattern;
	break;

      case GET_SUB_SHUFFLE_FLAG:
	return_code = npui_shuffle_sub_pattern;
	break;

      default:
	KernelErrorCode = KRERR_PARAMETERS;
    }

    { TRACE_RETURN(return_code); }
}

/*****************************************************************************
  FUNCTION : kr_initSubPatternOrder

  PURPOSE  : The sub pattern ordering for the current pattern set is
             reset for the next training or initialization run. During
	     this run all sub patterns from pattern <start> up to
	     pattern <end> are generated according to current shuffle
	     flags for patterns and sub patterns.  
             kr_getSubPatByOrder is to be called to get the next sub
	     pattern number during the run (see below) 

  RETURNS  : kernel error code
  NOTES    :

  UPDATE   : 
******************************************************************************/
krui_err kr_initSubPatternOrder(int start, int end)
{
    int pat_set;
    int n_sub;

    TRACE_IN();

    if (!npui_train_defined)
	{ TRACE_RETURN(KRERR_NP_NO_TRAIN_SCHEME); }

    pat_set = npui_pat_sets[npui_curr_pat_set];

    if (npui_shuffle_pattern && npui_shuffle_sub_pattern)
    {
	np_random_train_number = kr_TotalNoOfSubPatPairs();
	np_next_train_random = 0;
	{ TRACE_RETURN(KRERR_NO_ERROR); }
    }

    if (!kr_np_allocate_pat_train_entries(np_info[pat_set].pub.virtual_no_of_pattern))
	{ TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }

    if (np_info[pat_set].pub.class_distrib_active)
    {
	kr_np_order_chunk_arrays(npui_shuffle_pattern, pat_set);
	kr_np_order_chunked_pat_entries(pat_set, start, end);	
    }
    else
    {
	kr_np_order_pat_entries(start, end);
    }

    np_current_pattern = 
	&(np_pat_sets[pat_set][np_pat_train_order[0]]);
    np_next_train_pat = 0;

    kr_np_gen_sub_pos(np_current_pattern -> pub.input_dim, &n_sub,
		      np_current_pattern -> pub.input_dim_sizes, np_t_insize, 
		      np_t_instep, NULL, TRUE);

    if (!kr_np_allocate_sub_pat_train_entries(n_sub))
	{ TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }

    kr_np_order_sub_pat_entries(0, n_sub-1);
    np_next_train_sub_pat = 0;

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_getSubPatternByOrder

  PURPOSE : According to the last call to kr_initSubPatternOrder, the
  last call to this function and the shuffle flags, the next position
  of pattern and sub pattern is determined. This numbers are returned
  in <pattern> and <sub> (beginning with 0). 

  RETURNS : If there are no more sub pattern avaliable the return
  value is FALSE, otherwise TRUE.
  NOTES    :

  UPDATE   : 
******************************************************************************/
bool kr_getSubPatternByOrder(int *pattern, int *sub)
{
    int n_sub;

    TRACE_IN();

    if (npui_shuffle_pattern && npui_shuffle_sub_pattern)
    {
	if (np_next_train_random == -1)
	    { TRACE_RETURN(FALSE); }

	if (++np_next_train_random >= np_random_train_number)
	    np_next_train_random = -1;

	TRACE_RETURN((kr_getSubPatternByNo(pattern, sub, 
				     lrand48() % np_random_train_number)));
    }

    if (np_next_train_pat == -1)
	{ TRACE_RETURN(FALSE); }

    *pattern = np_pat_train_order[np_next_train_pat];
    *sub = np_sub_pat_train_order[np_next_train_sub_pat];

    np_next_train_sub_pat++;
    if (np_next_train_sub_pat >= np_sub_pat_train_number)
    {
	np_next_train_pat++;
	if (np_next_train_pat >= np_pat_train_number)
	{
	    np_next_train_pat = -1;
	    { TRACE_RETURN(TRUE); }
	}

	np_current_pattern = 
	    &(np_pat_sets[npui_pat_sets[npui_curr_pat_set]]
	                 [np_pat_train_order[np_next_train_pat]]);
    
	kr_np_gen_sub_pos(np_current_pattern -> pub.input_dim, &n_sub,
			  np_current_pattern -> pub.input_dim_sizes, np_t_insize, 
			  np_t_instep, NULL, TRUE);

	if (!kr_np_allocate_sub_pat_train_entries(n_sub))
	{
	    KernelErrorCode = KRERR_INSUFFICIENT_MEM;
	    { TRACE_RETURN(FALSE); }
	}

	kr_np_order_sub_pat_entries(0, n_sub-1);
	np_next_train_sub_pat = 0;
    }

    { TRACE_RETURN(TRUE); }
}

/*****************************************************************************
  FUNCTION : kr_getSubPatternByNo

  PURPOSE : According to the current pattern set, the position of the
  <n>th sub pattern is determined and returned in <pattern> (the
  pattern which includes the subpattern) and <sub> (the sub pattern
  inside the pattern) (beginning with 0).  
  This function does not effect the ordering of the function
  kr_getSubPatByOrder. <n> ranges from 0 to kr_TotalNoOfSubPatPairs()-1.

  RETURNS : If the sub pattern is available, TRUE is returned,
  otherwise FALSE.

  NOTES    :

  UPDATE   : 
******************************************************************************/
bool kr_getSubPatternByNo(int *pattern, int *sub, int n)
{
    register int ts;
    register int tp;
    register int low, high, mid;

    TRACE_IN();

    ts = kr_TotalNoOfSubPatPairs();
    if (n<0 || n>=ts)
	{ TRACE_RETURN(FALSE); }

    tp = np_info[npui_pat_sets[npui_curr_pat_set]].pub.virtual_no_of_pattern;

    if (tp == 1)
	/* only one pattern present, sub pattern must be here */
	high = 0;
    else
    {
	/* more than one pattern present.... */
	/* first try to find the right position by a direct jump */
	high = (int)(((double)n*(double)tp)/(double)ts);

	if (np_abs_count[high] <= n || (high != 0 && np_abs_count[high-1] > n))
	{
	    /* direct jump was not sucessfull, now perform binary search */
	    low = 0;
	    high = tp-1;
	    while (low < high)
	    {
		mid = (high+low)/2;
		if (np_abs_count[mid] > n)
		    high = mid;
		else
		    low = mid+1;
	    }
	}
    }

    /* now high gives the index of the pattern where the sub pattern is in */
    /* but np_abs_count is based on virtual pattern, while this function must 
       return a physical pattern number: */
    *pattern = kr_np_virtual_to_physical(high);

    /* calculate the position of the sub pattern inside the found pattern */
    if (high != 0)
	*sub = n - np_abs_count[high-1];
    else
	*sub = n;

    { TRACE_RETURN(TRUE); }
}

/*****************************************************************************
  FUNCTION : kr_TotalNoOfSubPatPairs

  PURPOSE : This function returns the total number of available sub
  patterns for the current pattern set or 0 if no pattern set is
  defined.  The result is the sum of the numbers of subpattern for all
  patterns in the current set.

  RETURNS  : number of sub pattern or 0
  NOTES    :

  UPDATE   : 
******************************************************************************/
int kr_TotalNoOfSubPatPairs(void)
{
    int n;
    int i;
    int sum;
    int n_sub;
    np_pattern_descriptor *pat;

    TRACE_IN();

    if (np_abs_count_valid)
	{ TRACE_RETURN(np_abs_count_No); }

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(0); }

    n = np_info[npui_pat_sets[npui_curr_pat_set]].pub.virtual_no_of_pattern;

    if (n > np_abs_count_size)
    {
	if (np_abs_count != (int *) NULL)
	    free(np_abs_count);
	np_abs_count_size = 0;
	np_abs_count = (int *) malloc(n * sizeof(int));
	if (np_abs_count == (int *) NULL && n != 0)
	    { TRACE_RETURN(0); }
	np_abs_count_size = n;
    }

    sum = 0;
    for (i=0; i<n; i++)
    {
	kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
			    kr_np_virtual_to_physical(i), &pat);
	kr_np_gen_sub_pos(pat -> pub.input_dim, &n_sub, 
			  pat -> pub.input_dim_sizes, np_t_insize,
			  np_t_instep, NULL, TRUE);
	sum += n_sub;
	np_abs_count[i] = sum;
    }
    np_abs_count_No = sum;
    np_abs_count_valid = TRUE;

    { TRACE_RETURN(np_abs_count_No); }
}

/*****************************************************************************
  FUNCTION : kr_NoOfSubPatPairs

  PURPOSE  : This function returns the number of available sub patterns
  for the pattern <pattern> of the current pattern set or 0 if this
  pattern is not defined.

  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
int kr_NoOfSubPatPairs(int pattern)
{
    register int ps;

    TRACE_IN();

    if (!np_abs_count_valid)
    {
	if (kr_TotalNoOfSubPatPairs() == 0)
	    { TRACE_RETURN(0); }
    }

    if (pattern<0 || 
	pattern > np_info[npui_pat_sets[npui_curr_pat_set]].pub.virtual_no_of_pattern)
	{ TRACE_RETURN(0); }

    ps = np_abs_count[pattern];

    if (pattern == 0)
    { TRACE_RETURN(ps); }
    else
	TRACE_RETURN((ps - np_abs_count[pattern-1]));
}

/*****************************************************************************
  FUNCTION : kr_AbsPosOfFirstSubPat

  PURPOSE  : This function returns the absolute position of the first
  sub pattern of pattern <pattern> in the current pattern set. This
  position is defined as the Sum of kr_NoOfSubPatPairs(i) where i runs
  from 0 to <pattern>-1.  The absolute position of the first sub
  pattern of pattern 0 is 0.  The returned value may be used as
  argument for the function kr_getSubPatternByNo.

  RETURNS  : 
  NOTES    :

  UPDATE   : 
******************************************************************************/
int kr_AbsPosOfFirstSubPat(int pattern)
{
    TRACE_IN();

    if (!np_abs_count_valid)
    {
	if (kr_TotalNoOfSubPatPairs() == 0)
	    { TRACE_RETURN(0); }
    }

    if (pattern <= 0 || 
	pattern > np_info[npui_pat_sets[npui_curr_pat_set]].pub.virtual_no_of_pattern)
	{ TRACE_RETURN(0); }

    { TRACE_RETURN(np_abs_count[pattern-1]); }
}

/*****************************************************************************
  FUNCTION : kr_TotalNoOfPattern

  PURPOSE : This function returns the total number of available
  patterns for the current pattern set or 0 if no pattern set is
  defined.  

  RETURNS  : number of pattern or 0
  NOTES    :

  UPDATE   : 
******************************************************************************/
int kr_TotalNoOfPattern(void)
{
    TRACE_IN();

    if (npui_curr_pat_set == -1)
	{ TRACE_RETURN(0); }

    { TRACE_RETURN(np_info[npui_pat_sets[npui_curr_pat_set]].pub.virtual_no_of_pattern); }
}

/*****************************************************************************
  FUNCTION : kr_getSubPatData

  PURPOSE : For the current pattern set and the specified sub pattern
  size, the data array of the <sub_no>th sub pattern of the <pat_no>th
  pattern is returned. io_type spcifies whether the input (INPUT) or
  output (OUTPUT) data is requested. If <size> is != NULL the size of
  the data array is returned is this parameter.  

  RETURNS : The function returns a pointer to the data array (type
  Patterns) or NULL if an error occured.
  NOTES    : KernelErrorCode is set if an error occured

  UPDATE   : 
******************************************************************************/
Patterns kr_getSubPatData(int pat_no, int sub_no, int io_type, int *size)
{
    int set;
    np_pattern_descriptor *pat;
    int subpos[MAX_NO_OF_VAR_DIM];
    float *data;
    krui_err err;
    static float dummy_data = 0.0;
    int intsize;

    TRACE_IN();

#ifdef DEBUG_PATTERN
    TRACE_INDENT();
    fprintf(stderr, "pattern %d, sub %d, %s\n", pat_no, sub_no, io_type == INPUT ? "INPUT" : "OUTPUT");
#endif

    set = npui_pat_sets[npui_curr_pat_set];
    pat = np_pat_sets[set];
    
    if (pat_no >= np_info[set].pub.number_of_pattern)
	TRACE_RETURN((Patterns) NULL);

    pat += pat_no;
    np_current_pattern = pat;
    switch (io_type)
    {
      case INPUT:
	if (!kr_np_gen_sub_pos(pat->pub.input_dim, &sub_no, pat->pub.input_dim_sizes,
			       np_t_insize, np_t_instep, subpos, FALSE))
	{
	    KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	    TRACE_RETURN((Patterns) NULL);
	}
	if ((err = kr_np_GetSubPat(TRUE, subpos, np_t_insize, &data, &intsize))
	    != KRERR_NO_ERROR)
	{
	    KernelErrorCode = err;
	    TRACE_RETURN((Patterns) NULL);
	}
	if (size != NULL)
	    *size = intsize;

#ifdef PRINT_USED_PATTERNS
	{
	    int ind;
	    printf("I %d %d '%s' ", pat_no, sub_no, 
		   pat->mysym != NULL && pat->mysym->symname ? 
		   pat->mysym->symname : "");
	    for (ind=0; ind<intsize; ind++)
		printf("%g ", data[ind]);
	    printf("\n");
	}
#endif

	TRACE_RETURN((Patterns) data);
	break;
      case OUTPUT:
	if (!kr_np_gen_sub_pos(pat->pub.output_dim, &sub_no, pat->pub.output_dim_sizes,
			       np_t_outsize, np_t_outstep, subpos, FALSE))
	{
	    KernelErrorCode = KRERR_NP_NO_SUCH_PATTERN;
	    TRACE_RETURN((Patterns) NULL);
	}
	if ((err = kr_np_GetSubPat(FALSE, subpos, np_t_outsize, &data, &intsize))
	    != KRERR_NO_ERROR)
	{
	    KernelErrorCode = err;
	    TRACE_RETURN((Patterns) NULL);
	}
	if (size != NULL)
	    *size = intsize;

#ifdef PRINT_USED_PATTERNS
	{
	    int ind;
	    printf("O %d %d '%s' ", pat_no, sub_no, 
		   pat->mysym != NULL && pat->mysym->symname ? 
		   pat->mysym->symname : "");
	    for (ind=0; ind<intsize; ind++)
		printf("%g ", data[ind]);
	    printf("\n");
	}
#endif

	/* kr_np_GetSubPat reports no error if a zero length output sub */
	/* pattern is requested. Instead a NULL pointer is returned but */
	/* must be exchanged by a dummy non NULL pointer because this */
	/* function reports errors by NULL pointers */
	if (data == 0)
	    TRACE_RETURN((Patterns) &dummy_data);

	TRACE_RETURN((Patterns) data);
	break;
      default:
	KernelErrorCode = KRERR_PARAMETERS;
	TRACE_RETURN((Patterns) NULL);
    }
}

/*****************************************************************************
  FUNCTION : kr_getPatClass

  PURPOSE : For the current pattern set and the specified sub pattern,
  the class index (>=0) is returned. If the pattern set does not
  provide class information, the return value is -1
  RETURNS  : class index or -1, if no classes available
  NOTES    : sub_no is of no effect, since class information only refers
             to patterns and not to sub patterns
             The parameter is kept for future improvements.

  UPDATE   : 
*****************************************************************************/
extern int kr_getSubPatClass(int pat_no, int sub_no)
{
    int set;
    np_pattern_descriptor *pat;

    TRACE_IN();

    set = npui_pat_sets[npui_curr_pat_set];
    pat = np_pat_sets[set];
    
    if (pat_no < 0 || pat_no >= np_info[set].pub.number_of_pattern)
	TRACE_RETURN(-1);

    pat += pat_no;
    TRACE_RETURN(pat->pub.my_class);
}

/*****************************************************************************
  FUNCTION : kr_SizeOfInputSubPat

  PURPOSE  : For the current pattern set and the specified sub pattern size, 
  the size of the input part of the first sub pattern of the first pattern is 
  returned. Negative return values indicate KernelErrorCode. Size 0 is a valid 
  return value since the pattern may contain no data.
  RETURNS  : negative Kernel Error Codes or positive valid value
  NOTES    :

  UPDATE   : 
******************************************************************************/
int kr_SizeOfInputSubPat(void)
{
    krui_err err;

    TRACE_IN();

    if (np_sub_pat_sizes_valid)
	{ TRACE_RETURN(np_sub_pat_input_size); }

    err = kr_np_GetSubPatSizes(&np_sub_pat_input_size, 
			       &np_sub_pat_output_size);

    if (err == KRERR_NO_ERROR)
    {
	np_sub_pat_sizes_valid = TRUE;
	{ TRACE_RETURN(np_sub_pat_input_size); }
    }

    { TRACE_RETURN(err); }
}

/*****************************************************************************
  FUNCTION : kr_SizeOfOutputSubPat

  PURPOSE  : For the current pattern set and the specified sub pattern size, 
  the size of the output part of the first sub pattern of the first pattern is 
  returned. Negative return values indicate KernelErrorCode. Size 0 is a valid 
  return value since the pattern may contain no data.
  RETURNS  : negative Kernel Error Codes or positive valid value
  NOTES    :

  UPDATE   : 
******************************************************************************/
int kr_SizeOfOutputSubPat(void)
{
    krui_err err;

    TRACE_IN();

    if (np_sub_pat_sizes_valid)
	{ TRACE_RETURN(np_sub_pat_output_size); }
    
    err = kr_np_GetSubPatSizes(&np_sub_pat_input_size, 
			       &np_sub_pat_output_size);

    if (err == KRERR_NO_ERROR)
    {
	np_sub_pat_sizes_valid = TRUE;
	{ TRACE_RETURN(np_sub_pat_output_size); }
    }

    { TRACE_RETURN(err); }
}

/*****************************************************************************
 FUNCTIONS WHICH ARE CALLED BY THE PATTERN PARSER OR FROM INSIDE THIS
 MODULE. DON'T USE THESE FUNCTIONS FOR OTHER PURPOSES !!!!!
******************************************************************************/

/*****************************************************************************
  FUNCTION : kr_np_AllocatePatternSet

  PURPOSE  : looks for a free slot in pattern set array and allocates <number>
             of free pattern descriptors

  RETURNS  : kernel error code
  NOTES    : don't call this function. This function is only to be
             called by the parser or by functions inside this module

  UPDATE   : 
******************************************************************************/
krui_err kr_np_AllocatePatternSet(int *pat_set, int number)
{
    krui_err err_code;
    int i;
    int set = -1;

    TRACE_IN();

    if (np_used_pat_set_entries == 0)
    {
	/* never patterns allocated */
	err_code = kr_np_InitPattern();
	if (err_code != KRERR_NO_ERROR)
	    { TRACE_RETURN(err_code); }
    }

    /* check for free pattern set entry */
    for (i=0; i<np_used_pat_set_entries; i++)
    {
	if (!np_pat_set_used[i])
	{
	    set = i;
	    break;
	}
    }
    if (set == -1)
    {
	/* no more free entries */
	{ TRACE_RETURN(KRERR_NP_NO_MORE_ENTRIES); }
    }

    /* allocate array of pattern descriptors */
    np_pat_sets[set] = 
	(np_pattern_descriptor *) malloc(number * sizeof(np_pattern_descriptor));
    if (np_pat_sets[set] == (np_pattern_descriptor *) NULL && number != 0)
	{ TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }

    /* initialize part of the pattern descriptors */
    for (i=0; i<number; i++)
    {
	np_pat_sets[set][i].pub.input_fixsize = 0;
	np_pat_sets[set][i].pub.output_fixsize = 0;
	np_pat_sets[set][i].pub.my_class = -1;

	np_pat_sets[set][i].input_pattern = (float *) NULL;
	np_pat_sets[set][i].output_pattern = (float *) NULL;

	np_pat_sets[set][i].mysym = (struct np_symtab *) NULL;
    }

    /* store number of allocated descriptors */
    np_info[set].pub.number_of_pattern = number;
    np_info[set].pub.virtual_no_of_pattern = number;
    np_info[set].pub.classes = 0;
    np_info[set].pub.class_names = NULL;
    np_info[set].pub.class_distrib_active = FALSE;
    np_info[set].pub.class_redistribution = NULL;
    np_info[set].pub.remap_function = NULL;
    np_info[set].pub.no_of_remap_params = 0;
    np_info[set].rmf_ptr = NULL;
    np_info_valid[set] = FALSE; /* only number_of_pattern is valid */

    /* sucessfull return */
    np_pat_set_used[set] = TRUE;
    *pat_set = set;
    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_AllocatePattern

  PURPOSE  : uses the information provided in given pattern descriptor to
             allocate memory for pattern data

  RETURNS  : kernel error code
  NOTES    : don't call this function. This function is only to be
             called by the parser or by functions inside this module

  UPDATE   : 
******************************************************************************/
krui_err kr_np_AllocatePattern(np_pattern_descriptor *pattern,
			       bool input)
{
    int i;
    int size;

    TRACE_IN();

    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    if (input)
    {
	size = pattern -> pub.input_fixsize;
	for (i=0; i<pattern -> pub.input_dim; i++)
	    size *= (pattern -> pub.input_dim_sizes)[i];

	if (pattern -> pub.input_dim > 0)
	    pattern -> input_pattern = (float *) malloc(size * sizeof(float));
	else
	    pattern -> input_pattern = kr_np_floatmalloc(size);

	if (pattern -> input_pattern == (float *) NULL && size != 0)
	{ TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
	else
	{ TRACE_RETURN(KRERR_NO_ERROR); }
    }
    else
    {
	size = pattern -> pub.output_fixsize;
	for (i=0; i<pattern -> pub.output_dim; i++)
	    size *= (pattern -> pub.output_dim_sizes)[i];

	if (pattern -> pub.output_dim > 0)
	    pattern -> output_pattern = (float *) malloc(size * sizeof(float));
	else
	    pattern -> output_pattern = kr_np_floatmalloc(size);

	if (pattern -> output_pattern == (float *) NULL && size != 0)
	    { TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
	else
	    { TRACE_RETURN(KRERR_NO_ERROR); }
    }
}

/*****************************************************************************
  FUNCTION : kr_np_freePattern

  PURPOSE  : frees the area of the pattern data for this pattern

  RETURNS  : 
  NOTES    : don't call this function. This function is only to be
             called by the parser or by functions inside this module

  UPDATE   : 
******************************************************************************/
void kr_np_FreePattern(np_pattern_descriptor *pattern)
{
    if (pattern -> input_pattern)
    {
	if (pattern -> pub.input_dim > 0)
	    free(pattern -> input_pattern);
	else
	    kr_np_floatfree(pattern -> input_pattern);
    }

    if (pattern -> output_pattern)
    {
	if (pattern -> pub.output_dim > 0)
	    free(pattern -> output_pattern);
	else
	    kr_np_floatfree(pattern -> output_pattern);
    }
}

/*****************************************************************************
  FUNCTION : kr_np_GetDescriptor

  PURPOSE  : determine a pointer to a specified pattern descriptor
             and make this pattern to be the current pattern

  RETURNS  : kernel error code
  NOTES    : don't call this function. This function is only to be
             called by the parser or by functions inside this module

	     <number> is a physical pattern number!

  UPDATE   : 
******************************************************************************/
krui_err kr_np_GetDescriptor(int pat_set, int number, 
			     np_pattern_descriptor **pattern)
{
    TRACE_IN();

    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    if (pat_set < 0 || pat_set >= np_used_pat_set_entries ||
	!np_pat_set_used[pat_set])
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    if (number >= np_info[pat_set].pub.number_of_pattern)
	{ TRACE_RETURN(KRERR_PATTERN_NO); }

    *pattern = np_current_pattern = &(np_pat_sets[pat_set][number]);

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_lookupSym

  PURPOSE  : Find an entry in the set specific symbol table or create a 
  new entry. If a new entry is created, also the numbering of symbols
  must be re-updated. 

  RETURNS  : kernel error code
  NOTES    : don't call this function. This function is only to be
             called by the parser or by functions inside this module

  UPDATE   : 
******************************************************************************/
krui_err kr_np_lookupSym(int pat_set, char *symbol, struct np_symtab **entry)
{
    struct np_symtab *list;
    struct np_symtab *newsym;
    struct np_symtab *last = NULL;
    int list_cmp;
    int last_cmp;
    int i;

    TRACE_IN();

    list = np_st[pat_set];
    while (list != NULL && (list_cmp = strcmp(symbol, list->symname)) > 0)
    {
	last = list;
	last_cmp = list_cmp;
	list = list->next;
    }
    
    if (list != NULL && list_cmp == 0)
	*entry = list;
    else
    {
	newsym = (struct np_symtab *) malloc(sizeof(struct np_symtab));
	if (!newsym)
	    { TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
	newsym->symname = strdup(symbol);
	if (!newsym->symname)
	    { TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
	newsym->set_amount = 0;
	newsym->chunk_amount = 0;
	newsym->symnum = 0;
	newsym->my_chunks_per_set = 0;
	newsym->global_chunks = 0;
	newsym->pat_nums_size = 0;
	newsym->pat_nums_valid = 0;
	newsym->pat_nums = NULL;
	newsym->within_chunk_pos = -1;
	newsym->pat_num_pos = -1;
	newsym->next = list;
	if (last)
	    last->next = newsym;
	else
	    np_st[pat_set] = newsym;
	*entry = newsym;

	list = np_st[pat_set];
	i = 0;
	while (list)
	{
	    list->symnum = i++;
	    list = list->next;
	}
    }

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
 INTERNAL FUNCTIONS OF THIS MODULE. IMPOSSIBLE TO CALL THESE FUNCTIONS
 FROM OUTSIDE THE MODULE (IF NOBODY CHANGES THE STATIC DECLARATION) !!!!!!!
******************************************************************************/

#ifdef DEBUG_PATTERN
static print_page_info(struct patternpage *page)
{
    int fl;
    int cnt;

    if (!page)
	printf("empty page\n");
    else
    {
	printf("id: %0lx, size: %d, slots: %d, free_slots: %d, list: ",
	       (long) page, page->slotsize, page->no_of_slots, page->no_of_free_slots);
	fl = page->firstfree;
	cnt = 0;
	do
	{
	    printf("%d ", fl);
	    fl = *((int *) ((float *) page->data + fl));
	    cnt++;
	} while (fl != -1 && cnt < 20);
	if (fl != -1)
	    printf("...");
	printf("\n");
    }
}
#endif

/*****************************************************************************
  FUNCTION : kr_np_floatmalloc

  PURPOSE  : allocate pattern memory within a private memory page
             <size> is based on sizeof(float) !!!
  RETURNS  : float* pointer or NULL if
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static float *kr_np_floatmalloc(int size)
{
    struct patternpage *page;
    int no_of_slots;
    int i;
    float *retval;

    /* size must be >= 0 */
    if (size < 0)
	return NULL;

    /* size must be at least big enough to hold the index free list */
    if (size*sizeof(float) < sizeof(int))
	size = ((sizeof(int)-1)/sizeof(float) + 1);

    /* check for page with requested slotsize */
    page = np_pages;
    while (page != NULL && (page->slotsize != size || page->no_of_free_slots == 0))
	page = page->next;

    if (!page)
    {
	/* we need to allocate a new page */
	no_of_slots = KRNP_FIXED_PAGESIZE/(size * sizeof(float));
	if (no_of_slots < 1)
	    no_of_slots = 1;

	page = (struct patternpage *) malloc(sizeof(struct patternpage));
	if (!page)
	    return NULL;
#ifdef DEBUG_PATTERN
	printf("allocating 0x%lx bytes for new pattern page\n",
	       size * no_of_slots * sizeof(float));
#endif
	page->data = (float *) malloc(size * no_of_slots * sizeof(float));
	if (!page->data)
	    return NULL;

	page->slotsize = size;
	page->no_of_slots = page->no_of_free_slots = no_of_slots;

	/* this is tricky:
	   one slot is at least big enough to hold one integer value.
	   To optimize memory consumption, free list management is done
	   in place: each empty slot stores the index (based on float *) to
	   the next free slot.
	*/
	for (i=0; i<no_of_slots-1; i++)
	{
	    *((int *) ((float *) (page->data + i*size))) = (i+1)*size;
	}
	*((int *) ((float *) (page->data + (no_of_slots-1)*size))) = -1;

	page->firstfree = 0;
	page->next = np_pages;
	np_pages = page;
    }

    /* here we have a page with at least one empty slot of right size: */
    page->no_of_free_slots--;
    retval = page->data + page->firstfree;
    page->firstfree = *((int *) retval);

#ifdef DEBUG_PATTERN
    print_page_info(page);
#endif

    return retval;
}

/*****************************************************************************
  FUNCTION : kr_np_floatfree

  PURPOSE  : free the pattern memory within a private memory page
  RETURNS  : nothing
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static void kr_np_floatfree(float *ptr)
{
    struct patternpage *page;
    struct patternpage *prev;

    /* look for the page which includes the given address */
    prev = NULL;
    page = np_pages;
    while (page != NULL && 
	   (ptr < page->data || 
	    ptr >= (page->data + page->no_of_slots*page->slotsize)))
    {
	prev = page;
	page = page->next;
    }

    if (!page)
	/* we have not allocated this memory slot !
	   gracefull return but print out error */
    {
	fprintf(stderr, 
		"kr_np_floatfree: function called with illegal argument\n");
	return;
    }

    *((int *) ptr) = page->firstfree;
    page->firstfree = (ptr - page->data);
    page->no_of_free_slots++;

    if (page->no_of_free_slots == page->no_of_slots)
    {
	/* page is no longer used, give it back to the OS */
	free(page->data);
	if (prev != NULL)
	    prev->next = page->next;
	else
	    np_pages = page->next;
	free(page);
	page = NULL;
    }

#ifdef DEBUG_PATTERN
    print_page_info(page);
#endif
}

/*****************************************************************************
  FUNCTION : kr_np_InitPattern

  PURPOSE  : initialization of pattern descriptor array
  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_InitPattern(void)
{
    int i;

    TRACE_IN();

    np_pat_sets = (np_pattern_descriptor **) 
	malloc(NO_OF_PAT_SETS * sizeof(np_pattern_descriptor *));

    np_pat_set_used = (bool *) 
	malloc(NO_OF_PAT_SETS * sizeof(bool));

    np_info = (np_pattern_set_info *) 
	malloc(NO_OF_PAT_SETS * sizeof(np_pattern_set_info));

    np_info_valid = (bool *) 
	malloc(NO_OF_PAT_SETS * sizeof(bool));

    np_st = (struct np_symtab **) 
	malloc(NO_OF_PAT_SETS * sizeof(struct np_symtab *));

    if (np_pat_sets == (np_pattern_descriptor **) NULL ||
	np_info == (np_pattern_set_info *) NULL ||
	np_info_valid == (bool *) NULL ||
	np_st == (struct np_symtab **) NULL)
    {
	{ TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
    }
    else
    {
	np_used_pat_set_entries = NO_OF_PAT_SETS;
	for (i=0; i<NO_OF_PAT_SETS; i++)
	{
	    np_pat_sets[i] = (np_pattern_descriptor *) NULL;
	    np_pat_set_used[i] = FALSE;
	    np_info_valid[i] = FALSE;
	    np_st[i] = NULL;
	}

	{ TRACE_RETURN(KRERR_NO_ERROR); }
    }
}


/*****************************************************************************
  FUNCTION : kr_np_ReallocatePatternSet

  PURPOSE  : reallocates the pattern set <pat_set> to contain <new_number> 
             of pattern entries.

  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_ReallocatePatternSet(int pat_set, int new_number)
{
    np_pattern_descriptor *new_mem;

    TRACE_IN();

    /* check whether patterns are allocated */
    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    /* check whether the pattern set is present */
    if (!np_pat_set_used[pat_set])
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    /* reallocate array of pattern descriptors */
    if (np_pat_sets[pat_set] != (np_pattern_descriptor *) NULL)
    {
	new_mem = (np_pattern_descriptor *) 
	    realloc(np_pat_sets[pat_set],
		    new_number * sizeof(np_pattern_descriptor));
    }
    else
    {
	new_mem = (np_pattern_descriptor *) 
	    malloc(new_number * sizeof(np_pattern_descriptor));
    }

    if (new_mem == (np_pattern_descriptor *) NULL && new_number != 0)
	{ TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }

    np_pat_sets[pat_set] = new_mem;

    /* store number of allocated descriptors */
    np_info[pat_set].pub.number_of_pattern = new_number;
    np_info[pat_set].pub.virtual_no_of_pattern = new_number;
    np_info_valid[pat_set] = FALSE; /* only number_of_pattern is valid */
    /* do not revalidate here, since this done by the calling function! */

    /* sucessfull return */
    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_DeletePatternSet

  PURPOSE  : delete a pattern set and free all memory
  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_DeletePatternSet(int pat_set)
{
    int i;
    np_pattern_descriptor *p;

    TRACE_IN();

    /* check for valid number of pattern set */
    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    if (pat_set<0 || pat_set>=np_used_pat_set_entries)
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }
	
    p = np_pat_sets[pat_set];
    if (!np_pat_set_used[pat_set])
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    /* free all pattern */
    for (i=0; i<np_info[pat_set].pub.number_of_pattern; i++)
    {
	kr_np_FreePattern(p);
	p++;
    }

    /* free the pattern descriptors */ 
    if (np_pat_sets[pat_set] != (np_pattern_descriptor *) NULL)
	free(np_pat_sets[pat_set]);
    np_pat_sets[pat_set] = (np_pattern_descriptor *) NULL;
    np_pat_set_used[pat_set] = FALSE;

    np_info[pat_set].pub.number_of_pattern = 0;
    np_info[pat_set].pub.virtual_no_of_pattern = 0;
    np_info_valid[pat_set] = FALSE;
    np_current_pattern = (np_pattern_descriptor *) NULL;

    /* free the symbol table */
    while (np_st[pat_set])
    {
	struct np_symtab *help;

	free(np_st[pat_set]->symname);
	help = np_st[pat_set]->next;
	free(np_st[pat_set]);
	np_st[pat_set] = help;
    }

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_DeletePattern

  PURPOSE  : delete a specific pattern form a pattern set
  RETURNS  : kernel error code
  NOTES    : internal use only

             <pattern> is a physical pattern number

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_DeletePattern(int pat_set, int pattern)
{
    np_pattern_descriptor *p;
    int i;

    TRACE_IN();

    /* check for valid number of pattern set */
    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    if (pat_set<0 || pat_set>=np_used_pat_set_entries)
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }
	
    p = np_pat_sets[pat_set];
    if (!np_pat_set_used[pat_set])
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    if (pattern < 0 || pattern >= np_info[pat_set].pub.number_of_pattern)
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN); }

    /* free memory for the specified pattern */
    p += pattern;
    kr_np_FreePattern(p);

    /* if classes available fix class information */
    if (p->mysym != NULL)
    {
	p->mysym->set_amount--;
	if (p->mysym->set_amount < p->mysym->chunk_amount)
	    p->mysym->chunk_amount = p->mysym->set_amount;
	if (p->mysym->set_amount <= 0)
	{
	    p->mysym->chunk_amount = 0;
	    np_info[pat_set].pub.classes--;
	    kr_np_delSym(pat_set, p->mysym);
	}
    }

    /* move tail of array one position to fill up the empty entry */
    for (i=pattern+1; i<np_info[pat_set].pub.number_of_pattern; i++)
    {
	(void) memcpy((char *) p, (char *) (p+1), sizeof(np_pattern_descriptor));
	p++;
    }

    np_info_valid[pat_set] = FALSE;
    /* do not revalidate here, since this done by the calling function */

    /* last entry of the array is no longer active */
    TRACE_RETURN(kr_np_ReallocatePatternSet(pat_set, 
				   np_info[pat_set].pub.number_of_pattern-1));
}

/*****************************************************************************
  FUNCTION : kr_np_delSym

  PURPOSE  : Delete the symbol table entry from the given pattern set.

  RETURNS  : 
  NOTES    : don't call this function. This function is only to be
             called by the parser or by functions inside this module

  UPDATE   : 
******************************************************************************/
static void kr_np_delSym(int pat_set, struct np_symtab *entry)
{
    struct np_symtab *list;
    struct np_symtab *last = NULL;
    int i;

    TRACE_IN();

    list = np_st[pat_set];
    while (list != NULL && list != entry)
    {
	last = list;
	list = list->next;
    }
    
    if (list != NULL && list == entry)
    {
	if (last != NULL)
	    last -> next = list->next;
	else
	    np_st[pat_set] = list->next;

	if (list->symname != NULL)
	    free(list->symname);
	if (list->pat_nums != NULL)
	    free(list->pat_nums);
	free(list);
    }
    else
    {
	/* symbol must be in the list of the given pattern
           set. Otherwise an error inside the pattern handling occured
           at some other place.... */

	fprintf(stderr, "internal error, file %s, line %d", __FILE__, __LINE__);
    }

    list = np_st[pat_set];
    i = 0;
    while (list)
    {
	list->symnum = i++;
	list = list->next;
    }

    { TRACE_RETURN_VOID(); }
}

/*****************************************************************************
  FUNCTION : kr_np_AddPattern

  PURPOSE  : Add an empty pattern descriptor to the specified pattern set. 
             The position of the pattern inside the set is returned 
             in <pattern>.
  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_AddPattern(int pat_set, int *pattern)
{
    krui_err err;
    np_pattern_descriptor *p;
    int number;

    TRACE_IN();

    /* check for valid number of pattern set */
    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    if (pat_set<0 || pat_set>=np_used_pat_set_entries)
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }
	
    p = np_pat_sets[pat_set];
    if (!np_pat_set_used[pat_set])
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    number = np_info[pat_set].pub.number_of_pattern + 1;
    err = kr_np_ReallocatePatternSet(pat_set, number);

    if (err != KRERR_NO_ERROR)
	{ TRACE_RETURN(err); }

    p = np_pat_sets[pat_set];
    p += (number-1);

    /* initialize part of the new pattern descriptors */
    {
	p -> pub.input_fixsize = 0;
	p -> pub.output_fixsize = 0;
	p -> pub.input_dim = 0;
	p -> pub.output_dim = 0;
	p -> pub.my_class = -1;
	p -> input_pattern = (float *) NULL;
	p -> output_pattern = (float *) NULL;
	p -> mysym = (struct np_symtab *) NULL;
    }

    *pattern = (number-1);
    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_LoadPatternFile

  PURPOSE  : loads new or old pattern file from pat_file. The number of the 
             associated pattern set is returned in pat_set.
  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_LoadPatternFile(FILE *pat_file, int *pat_set)
{
    int i;
    bool free;
    krui_err err_code = KRERR_NO_ERROR;
    int pattern_set;
    np_pattern_set_info set_info;

    TRACE_IN();

    /* be sure to have a place where to put the patterns */
    if (np_used_pat_set_entries == 0)
    {
	/* never patterns allocated */
	err_code = kr_np_InitPattern();
	if (err_code != KRERR_NO_ERROR)
	    { TRACE_RETURN(err_code); }
    }
    else
    {
	free = FALSE;

	/* check for free entry before starting parser */
	for (i=0; i<np_used_pat_set_entries; i++)
	{
	    if (!np_pat_set_used[i])
	    {
		free = TRUE;
		break;
	    }
	}

	if (!free)
	    { TRACE_RETURN(KRERR_NP_NO_MORE_ENTRIES); }	    
    }

    /* reset the scanner and the parser */
    scanner_init_scanner(pat_file);

    /* parse pattern file and check for errors */
    if (parse_pattern_file(&pattern_set, &set_info) != 0)
    {
	(void) kr_np_DeletePatternSet(pattern_set);
	{ TRACE_RETURN(KRERR_FILE_FORMAT); }
    }
    else
    {
	int i;
	struct np_symtab *list;

	*pat_set = pattern_set;
	np_info[pattern_set].pub.classes = set_info.pub.classes;
	np_info[pattern_set].pub.class_names = set_info.pub.class_names;
	np_info[pattern_set].pub.class_distrib_active = set_info.pub.class_distrib_active;
	np_info[pattern_set].pub.class_redistribution = set_info.pub.class_redistribution;

	list = np_st[pattern_set];
	i = 0;
	while (list)
	{
	    if (np_info[pattern_set].pub.class_distrib_active)
		list->chunk_amount =
		    np_info[pattern_set].pub.class_redistribution[i];
	    else
		list->chunk_amount = list->set_amount;
	    i++;
	    list = list -> next;
	}

	np_info[pattern_set].pub.remap_function = set_info.pub.remap_function;
	np_info[pattern_set].pub.no_of_remap_params = set_info.pub.no_of_remap_params;
	for (i=0; i<NO_OF_REMAP_PARAMS; i++)
	    np_info[pattern_set].pub.remap_params[i] = set_info.pub.remap_params[i];

	if (np_info[pattern_set].pub.remap_function != NULL)
	{
	    struct FuncInfoDescriptor func_descr;
	    
	    func_descr.func_type = REMAP_FUNC;
	    strcpy(func_descr.func_name, np_info[pattern_set].pub.remap_function);
	    err_code = krf_getFuncInfo(SEARCH_FUNC, &func_descr);

	    if (err_code == KRERR_NO_ERROR)
	    {
		np_info[pattern_set].rmf_ptr = (RemapFuncPtr) func_descr.function;
		if (np_info[pattern_set].pub.no_of_remap_params 
		    != func_descr.no_of_input_parameters)
		{
		    np_info[pattern_set].rmf_ptr = NULL;
		    np_info[pattern_set].pub.remap_function = NULL;
		    np_info[pattern_set].pub.no_of_remap_params = 0;
		    err_code = KRERR_PARAMETERS;
		}
	    }
	    else
	    {
		np_info[pattern_set].rmf_ptr = NULL;
		np_info[pattern_set].pub.remap_function = NULL;		
		np_info[pattern_set].pub.no_of_remap_params = 0;	
	    }
	}
	{ TRACE_RETURN(err_code); }
    }
}

/*****************************************************************************
  FUNCTION : kr_np_SavePatternFile

  PURPOSE  : save the pattern set pat_set to out_file
  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_SavePatternFile(FILE *out_file, int pat_set)
{
    np_pattern_set_info info;
    krui_err err_code;
    int i, j, n;
    int with_classes = 0;
    float *in_pat, *out_pat;
    time_t clock;
    
    TRACE_IN();

    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    if (pat_set<0 || pat_set>=np_used_pat_set_entries)
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }
	
    if (!np_pat_set_used[pat_set])
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    err_code = kr_np_GetInfo(pat_set, &info);
    if (err_code != KRERR_NO_ERROR)
	{ TRACE_RETURN(err_code); }

    if (info.pub.number_of_pattern == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    fprintf(out_file, "SNNS pattern definition file V%d.%d\n",
	    CURRENT_VERSION_V, CURRENT_VERSION_R);
    clock = time((time_t *) NULL);
    fprintf(out_file, "generated at %s\n\n", (char *) ctime(&clock));
    fprintf(out_file, "No. of patterns : %d\n", info.pub.number_of_pattern);
    fprintf(out_file, "No. of input units : %d\n", info.pub.in_fixsize);
    if (info.pub.out_fixsize != 0)
	fprintf(out_file, "No. of output units : %d\n", info.pub.out_fixsize);
    if (info.pub.in_number_of_dims != 0)
    {
	fprintf(out_file, "No. of variable input dimensions : %d\n", 
		info.pub.in_number_of_dims);
	fprintf(out_file, "Maximum input dimensions : [ ");
	for (i=0; i<info.pub.in_number_of_dims; i++)
	{
	    fprintf(out_file, "%d ", info.pub.in_max_dim_sizes[i]);
	}
	fprintf(out_file, "]\n");
    }
    if (info.pub.out_fixsize != 0 && info.pub.out_number_of_dims != 0)
    {
	fprintf(out_file, "No. of variable output dimensions : %d\n", 
		info.pub.out_number_of_dims);
	fprintf(out_file, "Maximum output dimensions : [ ");
	for (i=0; i<info.pub.out_number_of_dims; i++)
	{
	    fprintf(out_file, "%d ", info.pub.out_max_dim_sizes[i]);
	}
	fprintf(out_file, "]\n");
    }
    if (info.pub.classes > 0)
    {
	with_classes = 1;
	fprintf(out_file, "No. of classes : %d\n", info.pub.classes);
	if (info.pub.class_distrib_active)
	{
	    fprintf(out_file, "Class redistribution : [ ");
	    for (i=0; i<info.pub.classes; i++)
		fprintf(out_file, "%d ", info.pub.class_redistribution[i]);
	    fprintf(out_file, "]\n");
	}
    }
    if (info.pub.remap_function)
    {
	fprintf(out_file, "Remap function : %s\n",
		info.pub.remap_function);
	if (info.pub.no_of_remap_params > 0)
	{
	    fprintf(out_file, "Remap parameters : [ ");
	    for (i=0; i<info.pub.no_of_remap_params; i++)
		fprintf(out_file, "%g ", info.pub.remap_params[i]);
	    fprintf(out_file, "]\n");
	}
    }
    fprintf(out_file, "\n");

    for (j=0; j<info.pub.number_of_pattern; j++)
    {
	if (np_pat_sets[pat_set][j].pub.input_fixsize > 0)
	{
	    fprintf(out_file, "# Input pattern %d:\n", j+1);
	    n = info.pub.in_fixsize;
	    if (info.pub.in_number_of_dims != 0)
	    {
		fprintf(out_file, "[ ");
		for (i=0; i<np_pat_sets[pat_set][j].pub.input_dim; i++)
		{
		    fprintf(out_file, "%d ", 
			    np_pat_sets[pat_set][j].pub.input_dim_sizes[i]);
		    n *= np_pat_sets[pat_set][j].pub.input_dim_sizes[i];
		}
		fprintf(out_file, "]\n");
	    }
	    in_pat = np_pat_sets[pat_set][j].input_pattern;
	    for (i=0; i<n; i++)
	    {
		fprintf(out_file, "%g ", *in_pat++);
		if (i == n - 1 || i%10 == 9)
		    fprintf(out_file, "\n");
	    }
	}

	if (np_pat_sets[pat_set][j].pub.output_fixsize > 0)
	{
	    fprintf(out_file, "# Output pattern %d:\n", j+1);
	    n = info.pub.out_fixsize;
	    if (info.pub.out_number_of_dims != 0)
	    {
		fprintf(out_file, "[ ");
		for (i=0; i<np_pat_sets[pat_set][j].pub.output_dim; i++)
		{
		    fprintf(out_file, "%d ", 
			    np_pat_sets[pat_set][j].pub.output_dim_sizes[i]);
		    n *= np_pat_sets[pat_set][j].pub.output_dim_sizes[i];
		}
		fprintf(out_file, "]\n");
	    }
	    out_pat = np_pat_sets[pat_set][j].output_pattern;
	    for (i=0; i<n; i++)
	    {
		fprintf(out_file, "%g ", *out_pat++);
		if (i == n - 1 || i%10 == 9)
		    fprintf(out_file, "\n");
	    }
	}

	if (with_classes)
	{
	    fprintf(out_file, "# Class:\n");
	    fprintf(out_file, "%s\n", np_pat_sets[pat_set][j].mysym->symname);
	}
    }

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_GetInfo

  PURPOSE  : get some information about the specified pattern set
  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_GetInfo(int pat_set, np_pattern_set_info *info)
{
    krui_err err_code;

    TRACE_IN();

    err_code = kr_np_ValidateInfo(pat_set);
    if (err_code == KRERR_NO_ERROR)
	*info = np_info[pat_set];;

    { TRACE_RETURN(err_code); }
}

/*****************************************************************************
  FUNCTION : kr_np_ValidateInfo

  PURPOSE  : update pattern set information if necessary
  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_ValidateInfo(int pat_set)
{
    int i, j;
    np_pattern_descriptor *p;

    TRACE_IN();

    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    if (pat_set < 0 || pat_set >= np_used_pat_set_entries ||
	!np_pat_set_used[pat_set])
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    if (np_info_valid[pat_set] && np_abs_count_valid && np_pat_mapping_valid)
    {
	{ TRACE_RETURN(KRERR_NO_ERROR); }
    }

    /* controlled by np_info_valid flag: */
    if (!np_info_valid[pat_set])
    {
	p = np_pat_sets[pat_set];
	np_info[pat_set].pub.fixed_fixsizes = TRUE;
	if (np_info[pat_set].pub.number_of_pattern > 0 && p != (np_pattern_descriptor *) NULL)
	{
	    np_info[pat_set].pub.output_present = p-> pub.output_fixsize != 0;
	    np_info[pat_set].pub.in_fixsize = p -> pub.input_fixsize;
	    np_info[pat_set].pub.out_fixsize = p-> pub.output_fixsize;
	    np_info[pat_set].pub.in_number_of_dims = p->pub.input_dim;
	    np_info[pat_set].pub.out_number_of_dims = p->pub.output_dim;

	    for (j=0; j < p->pub.input_dim; j++)
	    {
		(np_info[pat_set].pub.in_max_dim_sizes)[j] = (np_info[pat_set].pub.in_min_dim_sizes)[j] =
		    (p -> pub.input_dim_sizes)[j];
	    }
	    for (j=0; j < p->pub.output_dim; j++)
	    {
		(np_info[pat_set].pub.out_max_dim_sizes)[j] = (np_info[pat_set].pub.out_min_dim_sizes)[j] =
		    (p -> pub.output_dim_sizes)[j];
	    }
	}
    
	p = np_pat_sets[pat_set];
	for (i=0; i<np_info[pat_set].pub.number_of_pattern; i++)
	{
	    if (p -> pub.input_fixsize != np_info[pat_set].pub.in_fixsize)
	    {
		np_info[pat_set].pub.fixed_fixsizes = FALSE;
		np_info[pat_set].pub.in_fixsize = -1;
	    }
	    if (p -> pub.output_fixsize != np_info[pat_set].pub.out_fixsize)
	    {
		np_info[pat_set].pub.fixed_fixsizes = FALSE;
		np_info[pat_set].pub.out_fixsize = -1;
	    }

	    for (j=0; j < p->pub.input_dim; j++)
	    {
		if ((p -> pub.input_dim_sizes)[j] > (np_info[pat_set].pub.in_max_dim_sizes)[j])
		    (np_info[pat_set].pub.in_max_dim_sizes)[j] = p -> pub.input_dim_sizes[j];
		if ((p -> pub.input_dim_sizes)[j] < (np_info[pat_set].pub.in_min_dim_sizes)[j])
		    (np_info[pat_set].pub.in_min_dim_sizes)[j] = p -> pub.input_dim_sizes[j];
	    }
	    for (j=0; j < p->pub.output_dim; j++)
	    {
		if ((p -> pub.output_dim_sizes)[j] > (np_info[pat_set].pub.out_max_dim_sizes)[j])
		    (np_info[pat_set].pub.out_max_dim_sizes)[j] = p -> pub.output_dim_sizes[j];
		if ((p -> pub.output_dim_sizes)[j] < (np_info[pat_set].pub.out_min_dim_sizes)[j])
		    (np_info[pat_set].pub.out_min_dim_sizes)[j] = p -> pub.output_dim_sizes[j];
	    }
	    p++;
	}
	np_info[pat_set].pub.virtual_no_of_pattern = np_info[pat_set].pub.number_of_pattern;
	np_info_valid[pat_set] = TRUE;
    }

    if (!np_pat_mapping_valid)
    {
	/* reallocate and refill class information */
	if (np_info[pat_set].pub.classes > 0)
	{
	    struct np_symtab *list;
	    int count=0;
	    int global_chunks = 0;
	    int total_virtual = 0;

	    p = np_pat_sets[pat_set];
	    for (i=0; i<np_info[pat_set].pub.number_of_pattern; i++)
	    {
		p->pub.my_class = p->mysym ? p->mysym->symnum : -1;
		p++;
	    }

#ifdef DEBUG_CLASSES
	    TRACE_INDENT();
	    printf("%s: there are now %d classes:\n", __FUNCTION__, np_info[pat_set].pub.classes);
#endif

	    np_info[pat_set].chunk_order_valid = FALSE;

	    if (np_info[pat_set].pub.class_names)
		np_info[pat_set].pub.class_names = 
		    (char **) realloc(np_info[pat_set].pub.class_names, 
				      np_info[pat_set].pub.classes * sizeof(char *));
	    else
		np_info[pat_set].pub.class_names = 
		    (char **) malloc(np_info[pat_set].pub.classes * sizeof(char *));
	
	    if (np_info[pat_set].pub.class_redistribution)
		np_info[pat_set].pub.class_redistribution = 
		    (int *) realloc(np_info[pat_set].pub.class_redistribution, 
				    np_info[pat_set].pub.classes * sizeof(int));
	    else
		np_info[pat_set].pub.class_redistribution = 
		    (int *) malloc(np_info[pat_set].pub.classes * sizeof(int));
	
	    if (!(np_info[pat_set].pub.class_names) || !(np_info[pat_set].pub.class_redistribution))
		{ TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }

	    list = np_st[pat_set];
	    while (count < np_info[pat_set].pub.classes && list != NULL)
	    {
		np_info[pat_set].pub.class_names[count] = list->symname;
		np_info[pat_set].pub.class_redistribution[count] = list->chunk_amount;
	    
		list->my_chunks_per_set = (list->chunk_amount > 0) ?
		    (list->set_amount-1)/list->chunk_amount + 1 : 0;

		if (list->my_chunks_per_set > global_chunks)
		    global_chunks = list->my_chunks_per_set;

		count++;
		list = list->next;
	    }
	    if (count != np_info[pat_set].pub.classes || list != NULL)
	    {
		fprintf(stderr, "internal error, file %s, line %d\n",
			__FILE__, __LINE__);
		{ TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
	    }
	    list = np_st[pat_set];
	    count = 0;
	    while (list != NULL)
	    {
		int n;

		list->global_chunks = global_chunks;

#ifdef DEBUG_CLASSES
		TRACE_INDENT();
		printf("%s: index %d, name %s, set amount %d, chunk amount %d\n", 
		       __FUNCTION__, count, list->symname,
		       list->set_amount, list->chunk_amount);
#endif

		n = list->global_chunks * list->chunk_amount;
		if (n < list->set_amount)
		    n = list->set_amount;
		if (n > list->pat_nums_size)
		{
		    if (list->pat_nums != NULL)
			free(list->pat_nums);
		    list->pat_nums_size = 0;
		    list->pat_nums = (int *) malloc(n * sizeof(int));
		    if (list->pat_nums == NULL && n != 0)
			{ TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
		    list->pat_nums_size = n;
		}

#ifdef DEBUG_CLASSES
		TRACE_INDENT();
		printf("%s: epoch chunks %d, pat nums table %d\n",
		       __FUNCTION__, list->global_chunks, list->pat_nums_size);
#endif

		total_virtual += list->global_chunks * list->chunk_amount;
		count++;
		list = list->next;
	    }
	    if (np_info[pat_set].pub.class_distrib_active)
	    {
		np_info[pat_set].pub.virtual_no_of_pattern = total_virtual;
		kr_np_order_chunk_arrays(FALSE, pat_set);
	    }
	    else
		np_info[pat_set].pub.virtual_no_of_pattern = 
		    np_info[pat_set].pub.number_of_pattern;
	}

#ifdef DEBUG_CLASSES
	TRACE_INDENT();
	printf("%s: physical %d, virtual %d, chunk active %s\n",
	       __FUNCTION__, 
	       np_info[pat_set].pub.number_of_pattern, 
	       np_info[pat_set].pub.virtual_no_of_pattern,
	       np_info[pat_set].pub.class_distrib_active ? "TRUE" : "FALSE");
#endif

	if (!kr_np_allocate_pat_mapping_entries(np_info[pat_set].pub.virtual_no_of_pattern))
	    { TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
	kr_np_fill_virtual_to_void_mapping(pat_set);
    }

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_GetSubPatSizes

  PURPOSE  : Depending on the current pattern set and the sub pattern 
  training scheme, which must be defined, the size of the first input sub 
  pattern and the size of the first output sub pattern is computed.
  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_GetSubPatSizes(int *input_size, int *output_size)
{
    int pat_set;
    np_pattern_descriptor *pattern;
    int i;

    TRACE_IN();

    if (!npui_train_defined)
	{ TRACE_RETURN(KRERR_NP_NO_TRAIN_SCHEME); }

    pat_set = npui_pat_sets[npui_curr_pat_set];
    pattern = &(np_pat_sets[pat_set][0]);

    *input_size = pattern -> pub.input_fixsize;
    for (i=0; i<pattern -> pub.input_dim; i++)
    {
	*input_size *= np_t_insize[i];
    }

    *output_size = pattern -> pub.output_fixsize;
    for (i=0; i<pattern -> pub.output_dim; i++)
    {
	*output_size *= np_t_outsize[i];
    }

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_GetSubPat

  PURPOSE  : copy a sub pattern into another area 

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

  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_GetSubPat(bool input, int *pos_coord, int *size_coord,
				float **data, int *entries)
{
    int u_dim;                   /* copy of all necessary variables from */
    int u_size;                  /* pattern descriptor, depending on */
    int *u_dim_sizes;            /* <input> */
    float *u_pattern;
    int npu_subpatsize;
    float *npu_subpat;

    int f_size;                  /* size of copy area */
    int i;
    int c[MAX_NO_OF_VAR_DIM];    /* counter for the dimensions */
    int copy_size;               /* size of copy block for each memcpy */
    float *cf;                   /* pointer into pattern (copy source) */
    float *cdest;                /* copy destination */
    bool stop;                   /* TRUE if all done */
    int offset;                  /* offset in multidimensional array */

    krui_err err;

    int set = npui_pat_sets[npui_curr_pat_set];

    TRACE_IN();

    /* check for valid pattern */
    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    if (np_current_pattern == (np_pattern_descriptor *) NULL)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN); }

    /* make a copy of all pattern info to work with */
    if (input)
    {
	u_dim = np_current_pattern -> pub.input_dim;
	u_size = np_current_pattern -> pub.input_fixsize;
	u_dim_sizes = np_current_pattern -> pub.input_dim_sizes;
	u_pattern = np_current_pattern -> input_pattern;
	npu_subpatsize = np_i_subpatsize;
	npu_subpat = np_i_subpat;
    }
    else
    {
	u_dim = np_current_pattern -> pub.output_dim;
	u_size = np_current_pattern -> pub.output_fixsize;
	u_dim_sizes = np_current_pattern -> pub.output_dim_sizes;
	u_pattern = np_current_pattern -> output_pattern;
	npu_subpatsize = np_o_subpatsize;
	npu_subpat = np_o_subpat;
    }

    /* check whether this pattern exists */
    if (u_size == 0)
    {
	if (input)
	    { TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN); }
	else
	{
	    /* this may be a network without output neurons, so don't */
	    /* produce an error but set the data pointer to NULL */

	    *data = (float *) NULL;
	    if (entries != (int *) NULL)
		*entries = 0;
	    { TRACE_RETURN(KRERR_NO_ERROR); }
	}
    }

    /* calculate size of the sub pattern and check dimensions */
    f_size = u_size;
    for (i=0; i<u_dim; i++)
    {
	f_size *= size_coord[i];
	if (pos_coord[i] + size_coord[i] > u_dim_sizes[i])
	    { TRACE_RETURN(KRERR_NP_DIMENSION); }
	c[i] = 0;
    }

    /* allocate space for sub pattern if necessary */
    if (f_size > npu_subpatsize)
    {
	if (npu_subpat != (float *) NULL)
	    free(npu_subpat);
	npu_subpat = (float *) malloc(f_size * sizeof(float));
	if (npu_subpat == (float *) NULL && f_size != 0)
	{
	    { TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }
	}
	npu_subpatsize = f_size;

	/* copy back new address and size */
	if (input)
	{
	    np_i_subpatsize = npu_subpatsize;
	    np_i_subpat = npu_subpat;
	}
	else
	{
	    np_o_subpatsize = npu_subpatsize;
	    np_o_subpat = npu_subpat;
	}
    }


    /* calculate size of one copy block. this includes the size of the */
    /* dimension with highest index (least significant dimension) */
    copy_size = u_size;
    if (u_dim>0)
	copy_size *= size_coord[u_dim-1];

    /* copy the sub pattern */
    cdest = npu_subpat;
    do
    {
	/* calculate offset in source pattern */
	offset = 0;
	for (i=0; i<u_dim; i++)
	{
	    offset *= u_dim_sizes[i];
	    offset += pos_coord[i] + c[i];
	}
	offset *= u_size;
	cf = u_pattern + offset;

	/* copy part of the pattern. this inlcudes the least significant */
	/* dimension */
	(void) memcpy((char *) cdest, (char *) cf, copy_size * sizeof(float));
	cdest += copy_size;

	/* count the dimensions and check whether we have to go on. */
	/* the least significant dimension is omitted, because it is already */
	/* copied */
	stop = TRUE;
	for (i=u_dim-2; i>=0; i--)
	{
	    c[i]++;
	    if (c[i] == size_coord[i])
		c[i] = 0;
	    else
	    {
		stop = FALSE;
		break;
	    }
	}
    } while (!stop);

    /* remaping if necessary */
    if (!input && np_info[set].rmf_ptr != NULL)
    {
	err = np_info[set].rmf_ptr(npu_subpat, f_size,
				   np_info[set].pub.remap_params,
				   np_info[set].pub.no_of_remap_params);
	if (err != KRERR_NO_ERROR)
	    { TRACE_RETURN(err); }
    }

    /* all done, return results */
    *data = npu_subpat;
    if (entries != (int *) NULL)
	*entries = f_size;
    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_SetSubPat

  PURPOSE  : copy another area into a sub pattern

A sub pattern is cut out of the current pattern at position
<pos_coord> with size <size_coord>. According to <input> the input
part or the output part of the current pattern is used. 
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

  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_SetSubPat(bool input, int *pos_coord, int *size_coord,
				float *data, int entries)
{
    int u_dim;                   /* copy of all necessary variables from */
    int u_size;                  /* pattern descriptor, depending on */
    int *u_dim_sizes;            /* <input> */
    float *u_pattern;

    int f_size;                  /* size of copy area */
    int i;
    int c[MAX_NO_OF_VAR_DIM];    /* counter for the dimensions */
    int copy_size;               /* size of copy block for each memcpy */
    float *cf;                   /* pointer into pattern (copy destination) */
    float *csource;              /* copy source */
    bool stop;                   /* TRUE if all done */
    int offset;                  /* offset in multidimensional array */

    TRACE_IN();

    /* check for valid pattern */
    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    if (np_current_pattern == (np_pattern_descriptor *) NULL)
	{ TRACE_RETURN(KRERR_NP_NO_CURRENT_PATTERN); }

    /* make a copy of all pattern info to work with */
    if (input)
    {
	u_dim = np_current_pattern -> pub.input_dim;
	u_size = np_current_pattern -> pub.input_fixsize;
	u_dim_sizes = np_current_pattern -> pub.input_dim_sizes;
	u_pattern = np_current_pattern -> input_pattern;
    }
    else
    {
	u_dim = np_current_pattern -> pub.output_dim;
	u_size = np_current_pattern -> pub.output_fixsize;
	u_dim_sizes = np_current_pattern -> pub.output_dim_sizes;
	u_pattern = np_current_pattern -> output_pattern;
    }

    /* check whether this pattern exists or whether all is done */
    if (u_size == 0)
    {
	if (entries == 0)
	    { TRACE_RETURN(KRERR_NO_ERROR); }
	else
	    { TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN); }
    }

    /* calculate size of the sub pattern and check dimensions */
    f_size = u_size;
    for (i=0; i<u_dim; i++)
    {
	f_size *= size_coord[i];
	if (pos_coord[i] + size_coord[i] > u_dim_sizes[i])
	    { TRACE_RETURN(KRERR_NP_DIMENSION); }
	c[i] = 0;
    }

    /* check whether calculated space fits the delievered data block */
    if (f_size != entries)
	{ TRACE_RETURN(KRERR_NP_DOES_NOT_FIT); }

    /* calculate size of one copy block. this includes the size of the */
    /* dimension with highest index (least significant dimension) */
    copy_size = u_size;
    if (u_dim>0)
	copy_size *= size_coord[u_dim-1];

    /* copy the sub pattern */
    csource = data;
    do
    {
	/* calculate offset in destination pattern */
	offset = 0;
	for (i=0; i<u_dim; i++)
	{
	    offset *= u_dim_sizes[i];
	    offset += pos_coord[i] + c[i];
	}
	offset *= u_size;
	cf = u_pattern + offset;

	/* copy part of the pattern. this inlcudes the least significant */
	/* dimension */
	(void) memcpy((char *) cf, (char *)csource, copy_size * sizeof(float));
	csource += copy_size;

	/* count the dimensions and check whether we have to go on. */
	/* the least significant dimension is omitted, because it is already */
	/* copied */
	stop = TRUE;
	for (i=u_dim-2; i>=0; i--)
	{
	    c[i]++;
	    if (c[i] == size_coord[i])
		c[i] = 0;
	    else
	    {
		stop = FALSE;
		break;
	    }
	}
    } while (!stop);

    /* all done, return results */
    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_align_sub_pos

  PURPOSE  : For a given pattern dimension size <psize> of <dim>
             dimensions and a given sub pattern size <ssize> the position
	     <spos> of the sub pattern is alligned to a valid position
	     and the ordering number of this sub pattern is returned
	     in <n>. The shift pattern given in <sstep> is used to
	     find valid allignment positions

  RETURNS  : TRUE if succesfull, FALSE if inconsistent parameters

             <n> starts with 0 and ends with <number of possible
	     positions> - 1
	     <spos> gives the new aligned position

  NOTES    : internal function

  UPDATE   : 
******************************************************************************/
static bool kr_np_align_sub_pos(int dim, int *n, int *psize, int *ssize, 
				int *sstep, int *spos)
{
    int ns[MAX_NO_OF_VAR_DIM];
    register int i;
    register int ts = 0;

    TRACE_IN();

    for (i=0; i<dim; i++)
    {
	/* calculate how much steps could be made in each dimension */
	ns[i] = (psize[i] - ssize[i] + sstep[i]) / sstep[i];
	if (ns[i] == 0)
	    { TRACE_RETURN(FALSE); }

	/* align the position of this dimension */
	spos[i] -= spos[i] % sstep[i];
	if (spos[i]+ssize[i] >= psize[i]+1)
	    spos[i] = 0;

	/* add the position of this dimension to the absolute position */
	ts *= ns[i];
	ts += spos[i]/sstep[i];
    }

    *n = ts;
    { TRACE_RETURN(TRUE); }
}

/*****************************************************************************
  FUNCTION : kr_np_gen_sub_pos

  PURPOSE  : For a given pattern dimension size <psize> of <dim>
             dimensions and a given sub pattern size <ssize> the position
	     <spos> of the <n>th sub pattern is calculated. The shift
	     pattern given in <sstep> is used to move sub pattern over
	     the pattern.

	     If <count> is TRUE, only the number of possible positions
	     is determined and returned in <n> if at least 1 valid
	     position exists. <spos> is of no effect and may be NULL.

  RETURNS  : TRUE if the <n>th subpattern exists or if <count> is TRUE
             and at least 1 valid position exists, FALSE if <n> < 0.
	     If <n> is higher than the available number of subpattern, a 
	     wraparound occurs.

             <n> starts with 0 and ends with <number of possible
	     positions> - 1 or returns number of possible positions.

  NOTES    : internal function

  UPDATE   : 
******************************************************************************/
static bool kr_np_gen_sub_pos(int dim, int *n, int *psize, int *ssize, 
			      int *sstep, int *spos, bool count)
{
    int ns[MAX_NO_OF_VAR_DIM];
    int i;
    int ts;
    int nn;

    TRACE_IN();

    nn = *n;

    /* calculate how much steps could be made in each dimension */
    /* and the total number of positions ts (define the bases of a */
    /* multi base numerative system and determine the highest */
    /* representable number) */
    ts = 1;
    for (i=dim-1; i>=0; i--)
    {
	ns[i] = (psize[i] - ssize[i] + sstep[i]) / sstep[i];
	if (ns[i] == 0)
	    { TRACE_RETURN(FALSE); }
	ts *= ns[i];
    }

    /* check whether this is all what we want to do now */
    if (count)
    {
	*n = ts;
	{ TRACE_RETURN(TRUE); }
    }
	
    /* check range of requested step position */ 
    if (nn<0)
	{ TRACE_RETURN(FALSE); }

    /* if requested position is higher than available positions, perform */
    /* wraparaound (this is usefull/necessary for pattern without subpattern */
    nn = nn%ts;

    /* calculate step position of the <n>th sub pattern (like counting */
    /* in a multi base numerative system) */
    for (i=dim-1; i>=0; i--)
    {
	spos[i] = (nn % ns[i]) * sstep[i];
	nn /= ns[i];
    }
    
    { TRACE_RETURN(TRUE); }
}

/*****************************************************************************
  FUNCTION : kr_np_allocate_pat_train_entries

  PURPOSE  : allocate or reallocate an array which will later include
             the sorted or shuffled pattern order (during training)
  RETURNS  : FALSE if malloc fails
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static bool kr_np_allocate_pat_train_entries(int n)
{
    TRACE_IN();

    if (n > np_pat_train_size)
    {
	if (np_pat_train_order != (int *) NULL)
	    free(np_pat_train_order);
	np_pat_train_size = 0;
	np_pat_train_order = (int *) malloc(n * sizeof(int));
	if (np_pat_train_order == (int *) NULL && n != 0)
	    { TRACE_RETURN(FALSE); }
	np_pat_train_size = n;
	np_pat_train_valid = FALSE;
    }
    { TRACE_RETURN(TRUE); }
}

/*****************************************************************************
  FUNCTION : kr_np_allocate_sub_pat_train_entries

  PURPOSE  : allocate or reallocate an array which will later include
             the sorted or shuffled order of the sub pattern of the
	     current pattern
  RETURNS  : FALSE if malloc fails
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static bool kr_np_allocate_sub_pat_train_entries(int n)
{
    TRACE_IN();

    if (n > np_sub_pat_train_size)
    {
	if (np_sub_pat_train_order != (int *) NULL)
	    free(np_sub_pat_train_order);
	np_sub_pat_train_size = 0;
	np_sub_pat_train_order = (int *) malloc(n * sizeof(int));
	if (np_sub_pat_train_order == (int *) NULL && n != 0)
	    { TRACE_RETURN(FALSE); }
	np_sub_pat_train_size = n;
	np_sub_pat_train_valid = FALSE;
    }
    { TRACE_RETURN(TRUE); }
}

/*****************************************************************************
  FUNCTION : kr_np_allocate_pat_mapping_entries

  PURPOSE  : allocate or reallocate an array which will later include
             the virtual to physical pattern number mapping
  RETURNS  : FALSE if malloc fails
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static bool kr_np_allocate_pat_mapping_entries(int n)
{
    TRACE_IN();

    if (n > np_pat_mapping_size)
    {
	if (np_pat_mapping_order != (int *) NULL)
	    free(np_pat_mapping_order);
	np_pat_mapping_size = 0;
	np_pat_mapping_order = (int *) malloc(n * sizeof(int));
	if (np_pat_mapping_order == (int *) NULL && n != 0)
	    { TRACE_RETURN(FALSE); }
	np_pat_mapping_size = n;
	np_pat_mapping_valid = FALSE;
    }
    { TRACE_RETURN(TRUE); }
}

/*****************************************************************************
  FUNCTION : kr_np_order_pat_entries

  PURPOSE  : Fills the allocated array for the pattern ordering with
             increasing numbers if patterns are sorted or with a
	     random permutation if patterns are shuffled.
	     <start> and <end> define the first and last pattern
	     number to be used
  RETURNS  : nothing
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static void kr_np_order_pat_entries(int start, int end)
{
    static bool shuffle;
    static int c_start;
    static int c_end;

    register int i;
    register int *fp;
    register int h;
    register int s;
    register int n;

    TRACE_IN();

    if (!np_pat_train_valid || c_start != start || c_end != end 
	|| shuffle != npui_shuffle_pattern)
    {
	fp = np_pat_train_order;
	for (i=start; i<=end; i++)
	    *fp++ = i;
	np_pat_train_valid = TRUE;
	np_pat_train_number = end - start + 1;
	c_start = start;
	c_end = end;
	shuffle = npui_shuffle_pattern;
    }

    if (shuffle)
    {
	n = np_pat_train_number;
	fp = np_pat_train_order;
	for (i=0; i<n; i++)
	{
	    s = lrand48() % (n-i);
	    h = *fp;
	    *fp++ = np_pat_train_order[s+i];
	    np_pat_train_order[s+i] = h;
	}
    }

    { TRACE_RETURN_VOID(); }
}

/*****************************************************************************
  FUNCTION : kr_np_order_sub_pat_entries

  PURPOSE  : Fills the allocated array for the sub pattern ordering with
             increasing numbers if sub patterns are sorted or with a
	     random permutation if sub patterns are shuffled.
	     <start> and <end> define the first and last sub pattern
	     number to be used
  RETURNS  : nothing
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static void kr_np_order_sub_pat_entries(int start, int end)
{
    static bool shuffle;
    static int c_start;
    static int c_end;

    register int i;
    register int *fp;
    register int h;
    register int s;
    register int n;

    TRACE_IN();

    if (!np_sub_pat_train_valid || c_start != start || c_end != end 
	|| shuffle != npui_shuffle_sub_pattern)
    {
	fp = np_sub_pat_train_order;
	for (i=start; i<=end; i++)
	    *fp++ = i;
	np_sub_pat_train_valid = TRUE;
	np_sub_pat_train_number = end - start + 1;
	c_start = start;
	c_end = end;
	shuffle = npui_shuffle_sub_pattern;
    }

    if (shuffle)
    {
	n = np_sub_pat_train_number;
	fp = np_sub_pat_train_order;
	for (i=0; i<n; i++)
	{
	    s = lrand48() % (n-i);
	    h = *fp;
	    *fp++ = np_sub_pat_train_order[s+i];
	    np_sub_pat_train_order[s+i] = h;
	}
    }

    { TRACE_RETURN_VOID(); }
}

/*****************************************************************************
  FUNCTION : kr_np_order_chunk_arrays

  PURPOSE  : Fills the allocated array for the pattern ordering with
             increasing numbers if patterns are sorted or with a
	     random permutation if patterns are shuffled.
  RETURNS  : nothing
  NOTES    : internal use only

             ordering within pat_nums list after finishing this function:

                     s_a              s_a              s_a        p s_a
	     |                |                |                |        |  
	     -------------------------------------------------------------
	     |     |     |     |     |     |     |     |     |     |     |
               c_a   c_a   c_a   c_a   c_a   c_a   c_a   c_a   c_a   c_a


	     c_a: chunk_amount
             s_a: set_amount
             p s_a: partial set_amount

             global_chunks    = number of c_a repetitions
             pat_nums_valid   = global_chunks * chunk_amount
	     pat_num_pos      = 0

	     algorithm:
	     1: fill in first s_a section
	     2: (partially) copy this section to further positions while
	        shuffling if necessary

  UPDATE   : 
******************************************************************************/
static void kr_np_order_chunk_arrays(bool shuffle, int pattern_set)
{
    struct np_symtab *list;
    np_pattern_descriptor *pattern;
    int no_of_pattern;
    int i;
    int sc, ps;
    int total = 0;

    TRACE_IN();

    list = np_st[pattern_set];
    while (list != NULL)
    {
	list->pat_num_pos = 0;
	list = list->next;
    }

    if (np_info[pattern_set].chunk_order_valid &&
	!np_info[pattern_set].chunk_shuffle &&
	!shuffle)
	{ TRACE_RETURN_VOID(); }

    /* fill first set amount */
    if (!np_info[pattern_set].chunk_order_valid)
    {
	no_of_pattern = np_info[pattern_set].pub.number_of_pattern;

	list = np_st[pattern_set];
	while (list != NULL)
	{
	    list->pat_nums_valid = 0;
	    list = list->next;
	}

	pattern = np_pat_sets[pattern_set];
	for (i=0; i<no_of_pattern; i++, pattern++)
	    pattern->mysym->pat_nums[pattern->mysym->pat_nums_valid++] = i;
    }
    
    /* copy set amounts without shuffling */
    if (!np_info[pattern_set].chunk_order_valid && !shuffle)
    {
	list = np_st[pattern_set];
	while (list != NULL)
	{
	    if (list->chunk_amount > 0)
	    {
		sc = 1 + (list->global_chunks * list->chunk_amount - 1) / 
		  list->set_amount;
		ps = list->global_chunks * list->chunk_amount -
		  (sc-1) * list->set_amount;
		for (i=1; i<sc; i++)
		  {
		    if (i!=(sc-1))
		      memcpy(list->pat_nums + i*list->set_amount,
			     list->pat_nums, list->set_amount * sizeof(int));
		    else
		      memcpy(list->pat_nums + i*list->set_amount,
			     list->pat_nums, ps * sizeof(int));
		  }
		
		total += list->global_chunks * list->chunk_amount;
	    }
	    list = list->next;
	}

	if (total != np_info[pattern_set].pub.virtual_no_of_pattern)
	    fprintf(stderr, "internal error, file %s, line %d", __FILE__, __LINE__);
    }

    /* copy set amounts with shuffling */
    np_info[pattern_set].chunk_shuffle = shuffle;
    if (shuffle)
    {
	int n,j,s,h;
	int *fp;

	list = np_st[pattern_set];
	while (list != NULL)
	{
	    if (list->chunk_amount > 0)
	    {
	        sc = 1 + (list->global_chunks * list->chunk_amount - 1) / 
		  list->set_amount;
		ps = list->global_chunks * list->chunk_amount -
		  (sc-1) * list->set_amount;
		for (i=1; i<sc; i++)
		{
		    /* shuffle first copy of set */
		    n = list->set_amount;
		    fp = list->pat_nums;
		    for (j=0; j<n; j++)
		    {
			s = lrand48() % (n-j);
			h = *fp;
			*fp++ = list->pat_nums[s+j];
			list->pat_nums[s+j] = h;
		    }
		    
		    /* copy shuffled set */
		    if (i!=(sc-1))
		      memcpy(list->pat_nums + i*list->set_amount,
			     list->pat_nums, list->set_amount * sizeof(int));
		    else
		      memcpy(list->pat_nums + i*list->set_amount,
			     list->pat_nums, ps * sizeof(int));
		}

		/* reshuffle first copy of set, because part of it has been copied */
		n = list->set_amount;
		fp = list->pat_nums;
		for (j=0; j<n; j++)
		{
		    s = lrand48() % (n-j);
		    h = *fp;
		    *fp++ = list->pat_nums[s+j];
		    list->pat_nums[s+j] = h;
		}
	    }
	    list = list->next;
	}
    }

    np_info[pattern_set].chunk_order_valid = TRUE;

    { TRACE_RETURN_VOID(); }
}

/*****************************************************************************
  FUNCTION : kr_np_order_chunked_pat_entries

  PURPOSE  : Fills the allocated array for the pattern ordering with
             the contents of all chunk arrays.
	     <start> and <end> define the first and last pattern
	     number to be used
  RETURNS  : nothing
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static void kr_np_order_chunked_pat_entries(int pat_set, int start, int end)
{
    static int c_start;
    static int c_end;

    struct np_symtab *list;
    int insert_pos;
    int total_pos;
    int within_chunk_count;
    int chunk_size;

    TRACE_IN();

    if (np_pat_train_valid && c_start == start && c_end == end 
	&& !npui_shuffle_pattern)
	{ TRACE_RETURN_VOID(); }

    /* chunk lists are already prepared and shuffled if necessary */

    list = np_st[pat_set];
    chunk_size = 0;
    while (list != NULL)
    {
	chunk_size += list->chunk_amount;
	list = list->next;
    }
    if (chunk_size == 0)
	fprintf(stderr, "internal error, file %s, line %d", __FILE__, __LINE__);

    list = np_st[pat_set];
    while (list != NULL)
    {
	list->chunk_comp_base = (list->chunk_amount > 0) ?
	    chunk_size / list->chunk_amount : 0;
	list = list->next;
    }

    insert_pos = 0;
    total_pos = 0;
    list = NULL;
    within_chunk_count = 0;
    while (total_pos <= end)
    {
	if (within_chunk_count == 0)
	{
	    list = np_st[pat_set];
	    while (list != NULL)
	    {
		list->chunk_comp_count = list->chunk_comp_base;
		list->within_chunk_pos = 0;
		list = list->next;
	    }
	    within_chunk_count = chunk_size;
	}
	if (list == NULL)
	    list = np_st[pat_set];
	if (list->within_chunk_pos < list->chunk_amount && 
	    --(list->chunk_comp_count) == 0)
	{
	    if (total_pos >= start)
	    {
		np_pat_train_order[insert_pos++] =
		    list->pat_nums[list->pat_num_pos];
	    }
	    list->chunk_comp_count = list->chunk_comp_base;
	    list->within_chunk_pos++;
	    list->pat_num_pos++;
	    total_pos++;
	    within_chunk_count--;
	}
	list = list->next;
    }

/*
    while (total_pos <= end)
    {
	list = np_st[pat_set];
	while (list != NULL && total_pos <= end)
	{
	    list->within_chunk_pos = 0;
	    while (list->within_chunk_pos < list->chunk_amount && total_pos <= end)
	    {
		if (total_pos >= start)
		{
		    np_pat_train_order[insert_pos++] =
			list->pat_nums[list->pat_num_pos];
		}
		list->within_chunk_pos++;
		list->pat_num_pos++;
		total_pos++;
	    }
	    list = list->next;
	}
    }
*/

    c_start  = start;
    c_end = end;
    np_pat_train_valid = TRUE;
    np_pat_train_number = end - start + 1;

    { TRACE_RETURN_VOID(); }
}

/*****************************************************************************
  FUNCTION : kr_np_fill_virtual_to_void_mapping

  PURPOSE  : Fills the allocated mapping array for the pattern ordering with
             the contents of all chunk arrays.
  RETURNS  : nothing
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static void kr_np_fill_virtual_to_void_mapping(int pat_set)
{
    struct np_symtab *list;
    int insert_pos;
    int chunk_size;
    int within_chunk_count;

    TRACE_IN();

    if (np_pat_mapping_valid)
	{ TRACE_RETURN_VOID(); }

    if (np_info[pat_set].pub.class_distrib_active)
    {
	/* chunk lists are already prepared (not shuffled!) */
	list = np_st[pat_set];
	chunk_size = 0;
	while (list != NULL)
	{
	    chunk_size += list->chunk_amount;
	    list = list->next;
	}
	if (chunk_size == 0)
	    fprintf(stderr, "internal error, file %s, line %d", __FILE__, __LINE__);
	
	list = np_st[pat_set];
	while (list != NULL)
	{
	    list->chunk_comp_base = (list->chunk_amount > 0) ?
		chunk_size / list->chunk_amount : 0;
	    list = list->next;
	}
	
	insert_pos = 0;
	list = NULL;
	within_chunk_count = 0;
	while (insert_pos < np_info[pat_set].pub.virtual_no_of_pattern)
	{
	    if (within_chunk_count == 0)
	    {
		list = np_st[pat_set];
		while (list != NULL)
		{
		    list->chunk_comp_count = list->chunk_comp_base;
		    list->within_chunk_pos = 0;
		    list = list->next;
		}
		within_chunk_count = chunk_size;
	    }
	    if (list == NULL)
		list = np_st[pat_set];
	    if (list->within_chunk_pos < list->chunk_amount && 
		--(list->chunk_comp_count) == 0)
	    {
		np_pat_mapping_order[insert_pos++] =
		    list->pat_nums[list->pat_num_pos++];
		list->chunk_comp_count = list->chunk_comp_base;
		list->within_chunk_pos++;
		within_chunk_count--;
	    }
	    list = list->next;
	}

/*
	insert_pos = 0;
	list = NULL;
	
	while (insert_pos < np_info[pat_set].pub.virtual_no_of_pattern)
	{
	    list = np_st[pat_set];
	    while (list != NULL)
	    {
		list->within_chunk_pos = 0;
		while (list->within_chunk_pos < list->chunk_amount)
		{
		    np_pat_mapping_order[insert_pos++] =
			list->pat_nums[list->pat_num_pos++];
		    list->within_chunk_pos++;
		}
		list = list->next;
	    }
	}
*/

	if (insert_pos != np_info[pat_set].pub.virtual_no_of_pattern)
	    fprintf(stderr, "internal error, file %s, line %d\n", __FILE__, __LINE__);
    }
    else
    {
	/* no class distribution, virtual numbers = physical numbers */
	for (insert_pos=0; insert_pos < np_info[pat_set].pub.virtual_no_of_pattern;
	     insert_pos++)
	    np_pat_mapping_order[insert_pos] = insert_pos;
    }

    np_pat_mapping_valid = TRUE;

    { TRACE_RETURN_VOID(); }
}


/*****************************************************************************
  FUNCTION : kr_np_virtual_to_physical

  PURPOSE  : mapping of virtual pattern numbers to physically stored patterns

  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static int kr_np_virtual_to_physical(int vnum)
{
    TRACE_IN();

    { TRACE_RETURN(np_pat_mapping_order[vnum]); }
}

/*****************************************************************************
  FUNCTION : kr_np_physical_to_virtual

  PURPOSE  : mapping of physical pattern number to a virtual number

  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static int kr_np_physical_to_virtual(int pnum)
{
    int vnum;
    TRACE_IN();

    if (np_pat_mapping_order[pnum] == pnum)
	/* first guess: if no chunks are defined, virtual numbers are
	   equal to physical numbers */
	vnum = pnum;
    else
    {
	/* we need to search for a virtual number.
	   The field is only partially sorted, so it is not possible
	   to perform a binary search. There is no other way than to
	   start at the beginning and find the first occurence of pnum
	*/
	vnum = 0;
	while (np_pat_mapping_order[vnum] != pnum)
	    vnum++;	
    }
    
    { TRACE_RETURN(vnum); }
}

/*****************************************************************************
  FUNCTION : kr_np_DefineSubPatternOrdering

  PURPOSE : definition of generation of sub pattern during training

  The shape and ordering of sub patterns for training and display is
  defined.  <pat_set> specifies the pattern set to use. The flag
  <input> determines whether input or output sub patterns are to be
  defined. The array size_coord spezifies the shape of the sub pattern
  like in kr_np_GetSubPat. The array incr_coord specifies how sub
  patterns are generated from the whole pattern by shifting the shape
  over this pattern. Each value in this array gives an incremental
  offset for one dimensional direction. The start position is at [0 0
  0 ...]. New positions are generated by adding the rightmost value of
  incr_coord to the old position. If an overflow in this dimension
  occurs, this coordinate is reset to 0 and the next position to the
  left is incremented (and so on ...). After all sub patterns of one
  pattern are generated the next pattern is used.  Depending on
  npui_shuffle_pattern and npui_shuffle_sub_pattern shuffling is
  activated.

  RETURNS  : kernel error code
  NOTES    : internal use only

  UPDATE   : 
******************************************************************************/
static krui_err kr_np_DefineSubPatternOrdering(int pat_set, bool input, 
					       int *size_coord, 
					       int *incr_coord)
{
    krui_err err_code;
    np_pattern_descriptor *p;

    TRACE_IN();

    if (np_used_pat_set_entries == 0)
	{ TRACE_RETURN(KRERR_NO_PATTERNS); }

    if (pat_set < 0 || pat_set >= np_used_pat_set_entries ||
	!np_pat_set_used[pat_set])
	{ TRACE_RETURN(KRERR_NP_NO_SUCH_PATTERN_SET); }

    err_code = kr_np_GetDescriptor(pat_set, 0, &p);
    if (err_code != KRERR_NO_ERROR)
	{ TRACE_RETURN(err_code); }

    if (input)
    {
	memcpy((char *) np_t_insize, (char *) size_coord, 
	       p->pub.input_dim * sizeof(int));
	memcpy((char *) np_t_instep, (char *) incr_coord, 
	       p->pub.input_dim * sizeof(int));
    }
    else
    {
	memcpy((char *) np_t_outsize, (char *) size_coord, 
	       p->pub.input_dim * sizeof(int));
	memcpy((char *) np_t_outstep, (char *) incr_coord, 
	       p->pub.input_dim * sizeof(int));
    }

    np_pat_train_valid = FALSE;
    np_sub_pat_train_valid = FALSE;
    np_pat_mapping_valid = FALSE;

    { TRACE_RETURN(KRERR_NO_ERROR); }
}

/*****************************************************************************
  FUNCTION : kr_np_showPatternSTD

  PURPOSE  :  According to the mode kr_np_showPatternSTD stores the current
              Pattern/sub Pattern into the units activation (and/or output) 
	      values.
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
  RETURNS  :  kernel error code
  NOTES    :  See include file glob_typ.h for mode constants.
              internal use only
  UPDATE   : 
******************************************************************************/
static krui_err kr_np_showPatternSTD(int mode)
{
    register struct Unit *unit_ptr;
    float *in_pat, *out_pat;
    np_pattern_descriptor *pattern;
    int in_pat_size, out_pat_size;

    TRACE_IN();

    /* make the pattern to become the current pattern */ 
    if ((KernelErrorCode = 
	 kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
	     kr_np_virtual_to_physical(npui_curr_pattern-1), &pattern)) 
	!= KRERR_NO_ERROR)
	{ TRACE_RETURN(KernelErrorCode); }
  
    /*  calc. startaddress of patterns  */
    if ((KernelErrorCode = 
	 kr_np_GetSubPat(TRUE, npui_inpos, npui_insize, &in_pat, 
			 &in_pat_size)) 
	!= KRERR_NO_ERROR)
	{ TRACE_RETURN(KernelErrorCode); }

    if ((KernelErrorCode = 
	 kr_np_GetSubPat(FALSE, npui_outpos, npui_outsize, &out_pat, 
			 &out_pat_size)) 
	!= KRERR_NO_ERROR)
	{ TRACE_RETURN(KernelErrorCode); }

    /* calculate the units, ignore error code concerning old pattern format */
    (void) kr_IOCheck();
    KernelErrorCode = KRERR_NO_ERROR;

    /* check whether pattern fits the network, do not complain about */
    /* missing output pattern */
    if (NoOfInputUnits != in_pat_size ||
	(NoOfOutputUnits != out_pat_size && out_pat_size != 0))
	{ TRACE_RETURN(KernelErrorCode = KRERR_NP_DOES_NOT_FIT); }

    switch (mode)
    {
      case  OUTPUT_NOTHING:
	FOR_ALL_UNITS( unit_ptr )
	    if ( IS_INPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
		if (in_pat_size--)
		    unit_ptr->act = *in_pat++;
	break;

      case  OUTPUT_ACT:
	FOR_ALL_UNITS( unit_ptr )
	    if UNIT_IN_USE( unit_ptr )
	    {
		if IS_INPUT_UNIT( unit_ptr )
		    unit_ptr->act = *in_pat++;
		if (IS_OUTPUT_UNIT( unit_ptr ) && out_pat_size != 0)
		    unit_ptr->act = *out_pat++;
	    }
	break;

      case  OUTPUT_OUT:
	FOR_ALL_UNITS( unit_ptr )
	    if UNIT_IN_USE( unit_ptr )
	    {
		if IS_INPUT_UNIT( unit_ptr )
		    unit_ptr->act = *in_pat++;
		if (IS_OUTPUT_UNIT( unit_ptr ) && out_pat_size != 0)
		{
		    unit_ptr->act = *out_pat++;
		    if (unit_ptr->out_func == NULL)
			/*  Identity Function   */
			unit_ptr->Out.output = unit_ptr->act;
		    else
			unit_ptr->Out.output = 
			    (*unit_ptr->out_func) (unit_ptr->act);
		}
	    }
	break;
    
      default:
	KernelErrorCode = KRERR_PARAMETERS;
    }

    { TRACE_RETURN(KernelErrorCode); }
}

/*****************************************************************************
  FUNCTION : kr_np_modifyPattern

  PURPOSE  : The current activation of the input and output units is used to
             modify the current sub pattern.

  RETURNS  : kernel error code
  NOTES    : internal use only
  UPDATE   : 
******************************************************************************/
static krui_err kr_np_modifyPattern(void)
{
    register struct Unit *unit_ptr;
    float *in_pat, *out_pat;
    float *ip, *op;
    np_pattern_descriptor *pattern;

    TRACE_IN();

    /* make the pattern to become the current pattern */ 
    if ((KernelErrorCode = 
	 kr_np_GetDescriptor(npui_pat_sets[npui_curr_pat_set],
	     kr_np_virtual_to_physical(npui_curr_pattern-1), &pattern)) 
	!= KRERR_NO_ERROR)
	{ TRACE_RETURN(KernelErrorCode); }
  
    /* calculate the units, ignore error code concerning old pattern format */
    (void) kr_IOCheck();
    KernelErrorCode = KRERR_NO_ERROR;

    /* allocate memory to hold the sub pattern */
    in_pat = (float *) malloc(NoOfInputUnits * sizeof(float));
    out_pat = (float *) malloc(NoOfOutputUnits * sizeof(float));
    if ((in_pat == (float *) NULL && NoOfInputUnits != 0) 
	|| (out_pat == (float *) NULL && NoOfOutputUnits != 0))
	{ TRACE_RETURN(KRERR_INSUFFICIENT_MEM); }

    /* copy unit activations into sub pattern area */
    ip = in_pat;
    op = out_pat;

    FOR_ALL_UNITS(unit_ptr)
        if (UNIT_IN_USE(unit_ptr))
	{
	    if (IS_INPUT_UNIT(unit_ptr))
		*ip++ = unit_ptr->act;
	    if (IS_OUTPUT_UNIT(unit_ptr))
		*op++ =  unit_ptr->act;
        }

    KernelErrorCode = KRERR_NO_ERROR;

    /* test whether pattern already contains data. */
    /* allocate space if necessary (for new allocated patterns) */
    if (pattern -> pub.input_fixsize == 0)
    {
	pattern -> pub.input_fixsize = NoOfInputUnits;
	pattern -> pub.output_fixsize = NoOfOutputUnits;
	pattern -> pub.input_dim = 0;
	pattern -> pub.output_dim = 0;
	KernelErrorCode = kr_np_AllocatePattern(pattern, TRUE);
	if (KernelErrorCode == KRERR_NO_ERROR)
	    KernelErrorCode = kr_np_AllocatePattern(pattern, FALSE);
    }

    /* modify the copied sub pattern */
    if (KernelErrorCode == KRERR_NO_ERROR)
    {
	KernelErrorCode = kr_np_SetSubPat(TRUE, npui_inpos, npui_insize,
					  in_pat, NoOfInputUnits);
    }

    if (KernelErrorCode == KRERR_NO_ERROR)
    {
	KernelErrorCode = kr_np_SetSubPat(FALSE, npui_outpos, npui_outsize,
					  out_pat, NoOfOutputUnits);
    }

    /* free the memory */
    if (in_pat != (float *) NULL)
	free(in_pat);
    if (out_pat != (float *) NULL)
	free(out_pat);

    { TRACE_RETURN(KernelErrorCode); }
}

/*****************************************************************************
END OF FILE
******************************************************************************/

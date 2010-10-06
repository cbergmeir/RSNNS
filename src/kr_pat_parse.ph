/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_pat_parse.ph,v $
  SHORTNAME      : pat_parse
  SNNS VERSION   : 4.2

  PURPOSE        : parser for new pattern format; bison format 
  NOTES          : impossible to use with yacc

  AUTHOR         : Michael Vogt
  DATE           : 10.9.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:00 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef _KR_PAT_PARSE_DEFINED_
#define _KR_PAT_PARSE_DEFINED_

/* begin global definition section */

#define CURRENT_VERSION_V       4  /* current version of pattern file        */
#define CURRENT_VERSION_R       2  /* format                                 */

int parse_pattern_file(int *set, np_pattern_set_info *this_set);


/* end global definition section */

/* begin private definition section */

static int	i;
static int	dims[MAX_NO_OF_VAR_DIM];
static int	max_i_dims[MAX_NO_OF_VAR_I_DIM];
static int	max_o_dims[MAX_NO_OF_VAR_O_DIM];
static int	actual_dim_count;
static int	variable_input_dim;
static int	variable_output_dim;
static int	current_pattern;
static int	next_pattern_is_input;
static int	no_of_input;
static int	no_of_output;
static int	no_of_pattern;
static int	act_size;
static float	*pat_mem;
static int      pattern_set;
static np_pattern_descriptor *pattern;
static int      with_classes;
static struct np_symtab *this_class;
static int      num_param = 0;
static float    *parameter = NULL;

static np_pattern_set_info *this_set;

static void yyerror(char *error);

/* end private definition section */

#endif


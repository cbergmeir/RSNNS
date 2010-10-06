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

 int	i;
 int	dims[MAX_NO_OF_VAR_DIM];
 int	max_i_dims[MAX_NO_OF_VAR_I_DIM];
 int	max_o_dims[MAX_NO_OF_VAR_O_DIM];
 int	actual_dim_count;
 int	variable_input_dim;
 int	variable_output_dim;
 int	current_pattern;
 int	next_pattern_is_input;
 int	no_of_input;
 int	no_of_output;
 int	no_of_pattern;
 int	act_size;
 float	*pat_mem;
 int      pattern_set;
 np_pattern_descriptor *pattern;
 int      with_classes;
 struct np_symtab *this_class;
 int      num_param;
 float    *parameter;

 np_pattern_set_info *this_set;

 void yyerror(char *error);

/* end private definition section */

#endif


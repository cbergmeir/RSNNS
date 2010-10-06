/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_pat_parse_bison.y,v $
  SHORTNAME      : pat_parse
  SNNS VERSION   : 4.2

  PURPOSE        : parser for new pattern format; bison format 
  NOTES          : impossible to use with yacc

  AUTHOR         : Michael Vogt
  DATE           : 10.9.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/31 13:07:57 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
%{
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kr_typ.h"
#include "glob_typ.h"
#include "kernel.h"
#include "kr_pat_scan.h"
#include "kr_newpattern.h"
#include "kr_pat_parse.ph"
%}

%union
{
	float	value;		/* allgemeine Zahl */	
        char    *name;          /* allgemeiner String */
	struct
	{
		int	v;
		int	r;
	}	version;	/* Versionsnummer #.# */
}

%token	L_BRACKET R_BRACKET
%token	VERSION_HEADER
%token	GENERATED_AT
%token	NO_OF_PATTERN NO_OF_INPUT NO_OF_OUTPUT
%token	NO_OF_VAR_IDIM NO_OF_VAR_ODIM
%token	MAXIMUM_IDIM MAXIMUM_ODIM
%token  NO_OF_CLASSES CLASS_REDISTRIB
%token  REMAPFUNCTION REMAP_PARAM
%token	ERROR PATTERNEND PATTERNNOCLASS

%token	<value> NUMBER
%token  <name> NAME
%token	<version> V_NUMBER

%%

pattern_file:	header
		{	
		    current_pattern = 0;
		    next_pattern_is_input = 1;

		    if (kr_np_AllocatePatternSet(&pattern_set, no_of_pattern)
			!= KRERR_NO_ERROR)
		    { 
			yyerror("can't allocate memory"); 
			YYABORT; 
		    }
		}		
		pattern_list
		{
		    if (current_pattern < no_of_pattern ||
			!next_pattern_is_input)
		    { 
			yyerror("unexpected end of file"); 
			YYABORT; 
		    }
		    if (with_classes != this_set->pub.classes)
		    {
			yyerror("real number of classes does not match pattern file header");
			YYABORT;
		    }
		}
;

header:	VERSION_HEADER V_NUMBER 
	{
		if (($2.v == CURRENT_VERSION_V && $2.r > CURRENT_VERSION_R) ||
		    $2.v > CURRENT_VERSION_V)
		{ 
		    yyerror("version of pattern file not supported"); 
		    YYABORT; 
		}
	}
	GENERATED_AT NO_OF_PATTERN NUMBER
	{
		no_of_pattern = (int) $6;
		if (no_of_pattern <= 0)
		{ 
		    yyerror("illegal number of pattern"); 
		    YYABORT; 
		}
	}
        i_head o_head vi_head vo_head 
        {
	    this_set->pub.classes = 0;
	    this_set->pub.class_names = NULL;
	    this_set->pub.class_distrib_active = FALSE;
	    this_set->pub.class_redistribution = NULL;
        }
	cl_head
        {
	    this_set->pub.remap_function = NULL;
	    this_set->pub.no_of_remap_params = 0;
	}
	rm_head
;

i_head:	NO_OF_INPUT NUMBER
	{ 
		no_of_input = (int) $2;
		if (no_of_input < 0)
		{ 
		    yyerror("illegal number of input units"); 
		    YYABORT; 
		} 
	}
;

o_head:	NO_OF_OUTPUT NUMBER
	{ 
		no_of_output = (int) $2;
		if (no_of_output < 0)
		{ 
		    yyerror("illegal number of output units"); 
		    YYABORT; 
		} 
	}
	|
	{ no_of_output = 0; }
;

vi_head: NO_OF_VAR_IDIM NUMBER MAXIMUM_IDIM actual_dim
	{
		variable_input_dim = $2; 
		if (variable_input_dim < 0 || 
		    variable_input_dim > MAX_NO_OF_VAR_I_DIM)
		{ 
		    yyerror("illegal variable input dimensions"); 
		    YYABORT; 
		}
		if (actual_dim_count != variable_input_dim)
		{ 
		    yyerror("illegal number of entries in dimension list"); 
		    YYABORT; 
		}
		for (i=0; i<variable_input_dim; i++)
			max_i_dims[i] = dims[i];
	}
	|
	{ variable_input_dim = 0; }
;

vo_head: NO_OF_VAR_ODIM NUMBER MAXIMUM_ODIM actual_dim
	{
		variable_output_dim = $2; 
		if (variable_output_dim < 0 || 
		    variable_output_dim > MAX_NO_OF_VAR_O_DIM ||
		    no_of_output == 0)
		{ 
		    yyerror("illegal variable output dimensions"); 
		    YYABORT; 
		}
		if (actual_dim_count != variable_output_dim)
		{ 
		    yyerror("illegal number of entries in dimension list"); 
		    YYABORT; 
		}
		for (i=0; i<variable_output_dim; i++)
			max_o_dims[i] = dims[i];	
	}
	|
	{ variable_output_dim = 0; }
;

cl_head: NO_OF_CLASSES NUMBER
{
    with_classes = (int) $2;
}
| NO_OF_CLASSES NUMBER CLASS_REDISTRIB L_BRACKET 
{
    with_classes = (int) $2;
    num_param = 0;
}
paramlist R_BRACKET
{
    int i;

    this_set->pub.class_redistribution = 
	(int *) malloc(num_param * sizeof(int));
    if (!this_set->pub.class_redistribution)
    {
	yyerror("can't allocate memory"); 
	YYABORT; 
    }
    for (i=0; i<num_param; i++)
	this_set->pub.class_redistribution[i] = abs((int) parameter[i]);
    this_set->pub.class_distrib_active = TRUE;
    if (num_param != with_classes)
    {
	yyerror("contradictory definition of class redistribution and number of classes in pattern file header");
	YYABORT;
    }
}
| /* empty */
{
    with_classes = 0;
}
;

rm_head: REMAPFUNCTION NAME
{
    this_set->pub.remap_function = strdup($2);
    if (!this_set->pub.remap_function)
    {
	yyerror("can't allocate memory"); 
	YYABORT; 
    }
}
| REMAPFUNCTION NAME REMAP_PARAM L_BRACKET 
{
    num_param = 0;
}
paramlist R_BRACKET
{
    int i;

    this_set->pub.remap_function = strdup($2);
    if (!this_set->pub.remap_function)
    {
	yyerror("can't allocate memory"); 
	YYABORT; 
    }

    if (num_param > NO_OF_REMAP_PARAMS)
    {
	yyerror("to many parameters for remap function");
	YYABORT;
    }

    this_set->pub.no_of_remap_params = num_param;
    for (i=0; i<num_param; i++)
	this_set->pub.remap_params[i] = parameter[i];
}
| /* empty */
;

paramlist: param
| paramlist param
;

param: NUMBER
{
    num_param++;
    if (num_param > 1)
	parameter = (float *) realloc(parameter, num_param * sizeof(float));
    else
	parameter = (float *) malloc(sizeof(float));

    parameter[num_param-1] = $1;
}
;

actual_dim: L_BRACKET 
		{
			actual_dim_count = 0;
		}
	    actual_dim_rest R_BRACKET
        | L_BRACKET R_BRACKET                
                {
		        actual_dim_count = 0;
		}
;

actual_dim_rest: dim_entry
	| actual_dim_rest dim_entry
;

dim_entry: NUMBER
	{
		if (actual_dim_count >= MAX_NO_OF_VAR_DIM)
		{ 
		    yyerror("to many entries in dimension list"); 
		    YYABORT; 
		}
		dims[actual_dim_count] = (int) $1;
		if (dims[actual_dim_count] <= 0)
		{ 
		    yyerror("illegal size of dimension"); 
		    YYABORT; 
		}
		actual_dim_count++;
	}
;

pattern_list: pattern
	|     pattern_list pattern
;

pattern: 
{
    if (current_pattern >= no_of_pattern)
    { 
	yyerror("to many patterns"); 
	YYABORT; 
    }
} 
pattern_start 
{
    if (kr_np_GetDescriptor(pattern_set, current_pattern, &pattern)
	!= KRERR_NO_ERROR)
    {
	yyerror("pattern parser internal error");
	YYABORT;
    }
    if (next_pattern_is_input)
    {
	pattern -> pub.input_fixsize = no_of_input;
	pattern -> pub.input_dim = variable_input_dim;
	pattern -> pub.output_fixsize = no_of_output;
	pattern -> pub.output_dim = variable_output_dim;
	if (actual_dim_count != variable_input_dim)
	{ 
	    yyerror("illegal number of entries in dimension list"); 
	    YYABORT; 
	}
	act_size = no_of_input;
	for (i=0; i<variable_input_dim; i++)
	{
	    if (dims[i] > max_i_dims[i])
	    { 
		yyerror("variable dimension overflow"); 
		YYABORT; 
	    }
	    act_size *= dims[i];
	    (pattern -> pub.input_dim_sizes)[i] = dims[i];
	}
	if (kr_np_AllocatePattern(pattern, next_pattern_is_input)
	    != KRERR_NO_ERROR)
	{ 
	    yyerror("can't allocate memory"); 
	    YYABORT; 
	}
	pat_mem = pattern -> input_pattern;
    }
    else
    {
	if (actual_dim_count != variable_output_dim)
	{ 
	    yyerror("illegal number of entries in dimension list"); 
	    YYABORT; 
	}
	act_size = no_of_output;
	for (i=0; i<variable_output_dim; i++)
	{
	    if (dims[i] > max_o_dims[i])
	    { 
		yyerror("variable dimension overflow"); 
		YYABORT; 
	    }
	    act_size *= dims[i];
	    (pattern -> pub.output_dim_sizes)[i] = dims[i];
	}
	if (kr_np_AllocatePattern(pattern, next_pattern_is_input)
	    != KRERR_NO_ERROR)
	{ 
	    yyerror("can't allocate memory"); 
	    YYABORT; 
	}
	pat_mem = pattern -> output_pattern;
    }
}
pattern_body 
{
    if (act_size > 0)
    { 
	yyerror("to little values in pattern"); 
	YYABORT; 
    }
}
PATTERNEND
{
    if (with_classes == 0 || (next_pattern_is_input && no_of_output > 0))
	scanner_await_no_class();
    else
	scanner_await_class();
}
pattern_class
{
    if (no_of_output > 0)
	next_pattern_is_input = !next_pattern_is_input;
    
    if (next_pattern_is_input)
	current_pattern++;
}
;

pattern_start: actual_dim
        | /* empty */
{
    actual_dim_count = 0;
}
;


pattern_body: pattern_entry
	|     pattern_body pattern_entry
;

pattern_entry: NUMBER
	{
		if (act_size == 0)
		{ 
		    yyerror("to many values in pattern"); 
		    YYABORT; 
		}
		*pat_mem++ = $1;
		if (--act_size == 0)
		    scanner_await_pattern_end(); 
	}
;

pattern_class: 
PATTERNNOCLASS
{
    pattern->mysym = NULL;
}
| NAME
{
    if (kr_np_lookupSym(pattern_set, $1, &this_class) != KRERR_NO_ERROR)
    { 
	yyerror("can't allocate memory"); 
	YYABORT; 
    }
    if (this_class->set_amount == 0)
	this_set->pub.classes++;
    this_class->set_amount++;
    pattern->mysym = this_class;    
}
;

%%

/*****************************************************************************
  FUNCTION : yyerror

  PURPOSE  : must be provided for the parser.
             reports errors in pattern file to stderr
  RETURNS  : nothing
  NOTES    :

  UPDATE   : 
******************************************************************************/
static void yyerror(char *error)
{
    fprintf(stderr, "Parse error in pattern file at line %d:\n%s\n",
	    lineno, error);
}

/*****************************************************************************
  FUNCTION : parse_pattern_file

  PURPOSE  : calls the real parser

  RETURNS  : parser error code and the handle to the loaded pattern set
  NOTES    :

  UPDATE   : 
******************************************************************************/
int parse_pattern_file(int *set, np_pattern_set_info *set_info)
{
    int err;

#if YYDEBUG != 0
#ifdef DEBUG_PARSER
    yydebug = 1;
#else
    yydebug = 0;
#endif
#endif

    lineno = 1;
    this_set = set_info;
    err = yyparse();
    *set = pattern_set;

    if (err == 0)
	lineno = 0;

    return err;
}

/*****************************************************************************
END OF FILE
******************************************************************************/

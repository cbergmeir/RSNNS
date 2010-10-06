%{
/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_pat_scan.l,v $
  SHORTNAME      : pattern scanner
  SNNS VERSION   : 4.2

  PURPOSE        : pattern file scanner, lex file
  NOTES          : preliminary

  AUTHOR         : Michael Vogt
  DATE           : 10.9.93

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/03/31 13:08:01 $

    Copyright (c) 1990-1998  SNNS Group, IPVR, Univ. Stuttgart, Germany

******************************************************************************/

#include "y.tab.h"
#include <stdlib.h>
#include <string.h>
#include "glob_typ.h"
#include "kr_typ.h"
#include "kernel.h"
#include "kr_newpattern.h"
#include "kr_pat_parse.h"
#include "kr_pat_scan.ph"

%}

digit           [0-9]
white           [ \t]
free            [^\r\n\032]
exp		[Ee][-+]?{digit}+
instring        [^ #\t\r\n\032]

%%

\#{free}*				|
{white}*					;

\r\n						|
\r						|
\n						lineno++;

\032						return(0); /* end of text */

\[						return(L_BRACKET);

\]						return(R_BRACKET);

[Vv]{digit}+"."{digit}+(-{free}*)?      {
					s_dotpos = strchr((char *) yytext, '.');
					*s_dotpos = '\0';
					yylval.version.v = 
						atoi((char *) yytext+1);
					yylval.version.r =
						atoi((char *) s_dotpos+1);
					return(V_NUMBER);
				        }

[-+]?{digit}+				|
[-+]?{digit}+{exp}			|
[-+]?{digit}+"."{digit}*({exp})?	|
[-+]?{digit}*"."{digit}+({exp})?	        {
                                                    if (numbers_as_names)
						    {
							yylval.name = strdup(yytext);
							return(NAME);
						    }
						    else
						    {
							yylval.value=atof((char *) yytext);
							return(NUMBER);
						    }
						}

"SNNS pattern definition file"			return(VERSION_HEADER);

"generated at"{free}*				return(GENERATED_AT);

"No. of patterns"{white}*:			return(NO_OF_PATTERN);

"No. of input units"{white}*:			return(NO_OF_INPUT);

"No. of output units"{white}*:			return(NO_OF_OUTPUT);

"No. of variable input dimensions"{white}*:	return(NO_OF_VAR_IDIM);

"No. of variable output dimensions"{white}*:	return(NO_OF_VAR_ODIM);

"Maximum input dimensions"{white}*:		return(MAXIMUM_IDIM);

"Maximum output dimensions"{white}*:		return(MAXIMUM_ODIM);

"No. of classes"{white}*:                       return(NO_OF_CLASSES);

"Class redistribution"{white}*:                 return(CLASS_REDISTRIB);

"Remap function"{white}*:                       return(REMAPFUNCTION);

"Remap parameters"{white}*:                     return(REMAP_PARAM);

{instring}*                                     {
                                                    yylval.name = strdup(yytext);
                                                    return(NAME);
                                                }

[^ \t\n\[\]\032]+			       	return(ERROR);

%%

int pplex(void)
{
    int scanned;

    if (scanner_pattern_end_flag)
    {
	scanner_pattern_end_flag = 0;
	yylval.value = 0.0;
	return PATTERNEND;
    }
    else if (scanner_no_class_flag)
    {
	scanner_no_class_flag = 0;
	yylval.value = 0.0;
	return PATTERNNOCLASS;
    }
    else
    {
	scanned = yylex();
	numbers_as_names = 0;
	return scanned;
    }
}

void scanner_await_pattern_end(void)
{
    scanner_pattern_end_flag = 1;
}

void scanner_await_no_class(void)
{
    scanner_no_class_flag = 1;
}

void scanner_await_class(void)
{
    numbers_as_names = 1;
}

void scanner_init_scanner(FILE *in_file)
{
    yyin = in_file;
#ifdef FLEX_SCANNER
    yyrestart(yyin);
#endif
    scanner_pattern_end_flag = 0;
    scanner_no_class_flag = 0;
    numbers_as_names = 0;
}

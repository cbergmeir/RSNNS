
/*  A Bison parser, made from kr_pat_parse_bison.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse ppparse
#define yylex pplex
#define yyerror pperror
#define yylval pplval
#define yychar ppchar
#define yydebug ppdebug
#define yynerrs ppnerrs
#define	L_BRACKET	258
#define	R_BRACKET	259
#define	VERSION_HEADER	260
#define	GENERATED_AT	261
#define	NO_OF_PATTERN	262
#define	NO_OF_INPUT	263
#define	NO_OF_OUTPUT	264
#define	NO_OF_VAR_IDIM	265
#define	NO_OF_VAR_ODIM	266
#define	MAXIMUM_IDIM	267
#define	MAXIMUM_ODIM	268
#define	NO_OF_CLASSES	269
#define	CLASS_REDISTRIB	270
#define	REMAPFUNCTION	271
#define	REMAP_PARAM	272
#define	ERROR	273
#define	PATTERNEND	274
#define	PATTERNNOCLASS	275
#define	NUMBER	276
#define	NAME	277
#define	V_NUMBER	278

#line 59 "kr_pat_parse_bison.y"

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

#line 74 "kr_pat_parse_bison.y"
typedef union
{
	float	value;		/* allgemeine Zahl */	
        char    *name;          /* allgemeiner String */
	struct
	{
		int	v;
		int	r;
	}	version;	/* Versionsnummer #.# */
} YYSTYPE;
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		74
#define	YYFLAG		-32768
#define	YYNTBASE	24

#define YYTRANSLATE(x) ((unsigned)(x) <= 278 ? yytranslate[x] : 55)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     5,     6,     7,     8,     9,    25,    28,    31,
    32,    37,    38,    43,    44,    47,    48,    56,    57,    60,
    61,    69,    70,    72,    75,    77,    78,    83,    86,    88,
    91,    93,    95,    98,    99,   100,   101,   102,   111,   113,
   114,   116,   119,   121,   123
};

static const short yyrhs[] = {    -1,
    26,    25,    45,     0,     0,     0,     0,     0,     5,    23,
    27,     6,     7,    21,    28,    31,    32,    33,    34,    29,
    35,    30,    37,     0,     8,    21,     0,     9,    21,     0,
     0,    10,    21,    12,    41,     0,     0,    11,    21,    13,
    41,     0,     0,    14,    21,     0,     0,    14,    21,    15,
     3,    36,    39,     4,     0,     0,    16,    22,     0,     0,
    16,    22,    17,     3,    38,    39,     4,     0,     0,    40,
     0,    39,    40,     0,    21,     0,     0,     3,    42,    43,
     4,     0,     3,     4,     0,    44,     0,    43,    44,     0,
    21,     0,    46,     0,    45,    46,     0,     0,     0,     0,
     0,    47,    51,    48,    52,    49,    19,    50,    54,     0,
    41,     0,     0,    53,     0,    52,    53,     0,    21,     0,
    20,     0,    22,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   101,   114,   129,   138,   147,   155,   160,   162,   173,   182,
   186,   203,   207,   225,   229,   233,   238,   258,   264,   273,
   277,   298,   301,   302,   305,   317,   321,   322,   328,   329,
   332,   349,   350,   353,   362,   427,   435,   442,   451,   452,
   459,   460,   463,   476,   481
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","L_BRACKET",
"R_BRACKET","VERSION_HEADER","GENERATED_AT","NO_OF_PATTERN","NO_OF_INPUT","NO_OF_OUTPUT",
"NO_OF_VAR_IDIM","NO_OF_VAR_ODIM","MAXIMUM_IDIM","MAXIMUM_ODIM","NO_OF_CLASSES",
"CLASS_REDISTRIB","REMAPFUNCTION","REMAP_PARAM","ERROR","PATTERNEND","PATTERNNOCLASS",
"NUMBER","NAME","V_NUMBER","pattern_file","@1","header","@2","@3","@4","@5",
"i_head","o_head","vi_head","vo_head","cl_head","@6","rm_head","@7","paramlist",
"param","actual_dim","@8","actual_dim_rest","dim_entry","pattern_list","pattern",
"@9","@10","@11","@12","pattern_start","pattern_body","pattern_entry","pattern_class", NULL
};
#endif

static const short yyr1[] = {     0,
    25,    24,    27,    28,    29,    30,    26,    31,    32,    32,
    33,    33,    34,    34,    35,    36,    35,    35,    37,    38,
    37,    37,    39,    39,    40,    42,    41,    41,    43,    43,
    44,    45,    45,    47,    48,    49,    50,    46,    51,    51,
    52,    52,    53,    54,    54
};

static const short yyr2[] = {     0,
     0,     3,     0,     0,     0,     0,    15,     2,     2,     0,
     4,     0,     4,     0,     2,     0,     7,     0,     2,     0,
     7,     0,     1,     2,     1,     0,     4,     2,     1,     2,
     1,     1,     2,     0,     0,     0,     0,     8,     1,     0,
     1,     2,     1,     1,     1
};

static const short yydefact[] = {     0,
     0,     1,     3,    34,     0,    34,    32,    40,     0,    33,
    26,    39,    35,     0,    28,     0,     0,     4,    31,     0,
    29,    43,    36,    41,     0,    27,    30,     0,    42,     0,
    10,    37,     8,     0,    12,     0,     9,     0,    14,    44,
    45,    38,     0,     0,     5,     0,     0,    18,    11,     0,
     0,     6,    13,    15,    22,     0,     0,     7,    16,    19,
     0,     0,    25,     0,    23,    20,    17,    24,     0,     0,
    21,     0,     0,     0
};

static const short yydefgoto[] = {    72,
     4,     2,     5,    25,    48,    55,    31,    35,    39,    45,
    52,    61,    58,    69,    64,    65,    12,    16,    20,    21,
     6,     7,     8,    17,    28,    36,    13,    23,    24,    42
};

static const short yypact[] = {     1,
   -13,-32768,-32768,-32768,     5,    12,-32768,    10,     7,-32768,
    11,-32768,-32768,    -5,-32768,    -1,     0,-32768,-32768,    -4,
-32768,-32768,     0,-32768,    14,-32768,-32768,     4,-32768,     3,
    16,-32768,-32768,     6,    18,   -15,-32768,     8,    15,-32768,
-32768,-32768,    19,     9,-32768,    10,    20,    21,-32768,    10,
    13,-32768,-32768,    17,    22,    33,    23,-32768,-32768,    24,
    25,    34,-32768,    -3,-32768,-32768,-32768,-32768,    25,    -2,
-32768,    39,    40,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,   -27,   -61,   -42,-32768,-32768,    27,
-32768,    37,-32768,-32768,-32768,-32768,-32768,-32768,    26,-32768
};


#define	YYLAST		49


static const short yytable[] = {    26,
    67,    71,    68,    49,    40,     1,    41,    53,    68,     3,
     9,    -2,    11,    14,    15,    18,    19,    63,    63,    19,
    22,    30,    32,    33,    34,    44,    37,    38,    43,    47,
    46,    56,    50,    54,    51,    59,    66,    57,    73,    74,
    62,    70,    10,     0,    60,    63,    27,     0,    29
};

static const short yycheck[] = {     4,
     4,     4,    64,    46,    20,     5,    22,    50,    70,    23,
     6,     0,     3,     7,     4,    21,    21,    21,    21,    21,
    21,     8,    19,    21,     9,    11,    21,    10,    21,    21,
    12,    15,    13,    21,    14,     3,     3,    16,     0,     0,
    17,    69,     6,    -1,    22,    21,    20,    -1,    23
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/local/share/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/local/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 102 "kr_pat_parse_bison.y"
{	
		    current_pattern = 0;
		    next_pattern_is_input = 1;

		    if (kr_np_AllocatePatternSet(&pattern_set, no_of_pattern)
			!= KRERR_NO_ERROR)
		    { 
			yyerror("can't allocate memory"); 
			YYABORT; 
		    }
		;
    break;}
case 2:
#line 114 "kr_pat_parse_bison.y"
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
		;
    break;}
case 3:
#line 130 "kr_pat_parse_bison.y"
{
		if ((yyvsp[0].version.v == CURRENT_VERSION_V && yyvsp[0].version.r > CURRENT_VERSION_R) ||
		    yyvsp[0].version.v > CURRENT_VERSION_V)
		{ 
		    yyerror("version of pattern file not supported"); 
		    YYABORT; 
		}
	;
    break;}
case 4:
#line 139 "kr_pat_parse_bison.y"
{
		no_of_pattern = (int) yyvsp[0].value;
		if (no_of_pattern <= 0)
		{ 
		    yyerror("illegal number of pattern"); 
		    YYABORT; 
		}
	;
    break;}
case 5:
#line 148 "kr_pat_parse_bison.y"
{
	    this_set->pub.classes = 0;
	    this_set->pub.class_names = NULL;
	    this_set->pub.class_distrib_active = FALSE;
	    this_set->pub.class_redistribution = NULL;
        ;
    break;}
case 6:
#line 155 "kr_pat_parse_bison.y"
{
	    this_set->pub.remap_function = NULL;
	    this_set->pub.no_of_remap_params = 0;
	;
    break;}
case 8:
#line 163 "kr_pat_parse_bison.y"
{ 
		no_of_input = (int) yyvsp[0].value;
		if (no_of_input < 0)
		{ 
		    yyerror("illegal number of input units"); 
		    YYABORT; 
		} 
	;
    break;}
case 9:
#line 174 "kr_pat_parse_bison.y"
{ 
		no_of_output = (int) yyvsp[0].value;
		if (no_of_output < 0)
		{ 
		    yyerror("illegal number of output units"); 
		    YYABORT; 
		} 
	;
    break;}
case 10:
#line 183 "kr_pat_parse_bison.y"
{ no_of_output = 0; ;
    break;}
case 11:
#line 187 "kr_pat_parse_bison.y"
{
		variable_input_dim = yyvsp[-2].value; 
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
	;
    break;}
case 12:
#line 204 "kr_pat_parse_bison.y"
{ variable_input_dim = 0; ;
    break;}
case 13:
#line 208 "kr_pat_parse_bison.y"
{
		variable_output_dim = yyvsp[-2].value; 
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
	;
    break;}
case 14:
#line 226 "kr_pat_parse_bison.y"
{ variable_output_dim = 0; ;
    break;}
case 15:
#line 230 "kr_pat_parse_bison.y"
{
    with_classes = (int) yyvsp[0].value;
;
    break;}
case 16:
#line 234 "kr_pat_parse_bison.y"
{
    with_classes = (int) yyvsp[-2].value;
    num_param = 0;
;
    break;}
case 17:
#line 239 "kr_pat_parse_bison.y"
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
;
    break;}
case 18:
#line 259 "kr_pat_parse_bison.y"
{
    with_classes = 0;
;
    break;}
case 19:
#line 265 "kr_pat_parse_bison.y"
{
    this_set->pub.remap_function = strdup(yyvsp[0].name);
    if (!this_set->pub.remap_function)
    {
	yyerror("can't allocate memory"); 
	YYABORT; 
    }
;
    break;}
case 20:
#line 274 "kr_pat_parse_bison.y"
{
    num_param = 0;
;
    break;}
case 21:
#line 278 "kr_pat_parse_bison.y"
{
    int i;

    this_set->pub.remap_function = strdup(yyvsp[-5].name);
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
;
    break;}
case 25:
#line 306 "kr_pat_parse_bison.y"
{
    num_param++;
    if (num_param > 1)
	parameter = (float *) realloc(parameter, num_param * sizeof(float));
    else
	parameter = (float *) malloc(sizeof(float));

    parameter[num_param-1] = yyvsp[0].value;
;
    break;}
case 26:
#line 318 "kr_pat_parse_bison.y"
{
			actual_dim_count = 0;
		;
    break;}
case 28:
#line 323 "kr_pat_parse_bison.y"
{
		        actual_dim_count = 0;
		;
    break;}
case 31:
#line 333 "kr_pat_parse_bison.y"
{
		if (actual_dim_count >= MAX_NO_OF_VAR_DIM)
		{ 
		    yyerror("to many entries in dimension list"); 
		    YYABORT; 
		}
		dims[actual_dim_count] = (int) yyvsp[0].value;
		if (dims[actual_dim_count] <= 0)
		{ 
		    yyerror("illegal size of dimension"); 
		    YYABORT; 
		}
		actual_dim_count++;
	;
    break;}
case 34:
#line 354 "kr_pat_parse_bison.y"
{
    if (current_pattern >= no_of_pattern)
    { 
	yyerror("to many patterns"); 
	YYABORT; 
    }
;
    break;}
case 35:
#line 362 "kr_pat_parse_bison.y"
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
;
    break;}
case 36:
#line 427 "kr_pat_parse_bison.y"
{
    if (act_size > 0)
    { 
	yyerror("to little values in pattern"); 
	YYABORT; 
    }
;
    break;}
case 37:
#line 435 "kr_pat_parse_bison.y"
{
    if (with_classes == 0 || (next_pattern_is_input && no_of_output > 0))
	scanner_await_no_class();
    else
	scanner_await_class();
;
    break;}
case 38:
#line 442 "kr_pat_parse_bison.y"
{
    if (no_of_output > 0)
	next_pattern_is_input = !next_pattern_is_input;
    
    if (next_pattern_is_input)
	current_pattern++;
;
    break;}
case 40:
#line 453 "kr_pat_parse_bison.y"
{
    actual_dim_count = 0;
;
    break;}
case 43:
#line 464 "kr_pat_parse_bison.y"
{
		if (act_size == 0)
		{ 
		    yyerror("to many values in pattern"); 
		    YYABORT; 
		}
		*pat_mem++ = yyvsp[0].value;
		if (--act_size == 0)
		    scanner_await_pattern_end(); 
	;
    break;}
case 44:
#line 478 "kr_pat_parse_bison.y"
{
    pattern->mysym = NULL;
;
    break;}
case 45:
#line 482 "kr_pat_parse_bison.y"
{
    if (kr_np_lookupSym(pattern_set, yyvsp[0].name, &this_class) != KRERR_NO_ERROR)
    { 
	yyerror("can't allocate memory"); 
	YYABORT; 
    }
    if (this_class->set_amount == 0)
	this_set->pub.classes++;
    this_class->set_amount++;
    pattern->mysym = this_class;    
;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/local/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 495 "kr_pat_parse_bison.y"


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

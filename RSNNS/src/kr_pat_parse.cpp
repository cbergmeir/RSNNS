/************************************************************************************

   This file is part of SnnsCLib, a fork of the kernel and parts of the gui of 
   the Stuttgart Neural Network Simulator (SNNS), version 4.3.

   The file's original version is part of SNNS 4.3. It's source code can be found at

   http://www.ra.cs.uni-tuebingen.de/SNNS/

   SNNS 4.3 is under the license LGPL v2. We note that source code files of SNNS 4.3 
   state as version "4.2". Base of this fork is SNNS 4.3 with a reverse-applied 
   python patch (see http://developer.berlios.de/projects/snns-dev/).

   SnnsCLib was developed in 2010 by Christoph Bergmeir under supervision of 
   José M. Benítez, both affiliated to DiCITS Lab, Sci2s group, DECSAI, 
   University of Granada

   Changes done to the original code were performed with the objective to
   port it from C to C++ and to encapsulate all code in one class named SnnsCLib.

   Changes in header files mainly include:
   * removed all static keywords
   * moved initializations of variables to the constructor of SnnsCLib

   Changes in cpp code files mainly include:
   * changed file ending from .c to .cpp
   * removed all SNNS internal includes and only include SnnsCLib   
   * static variables within functions were turned into member variables of SnnsCLib
   * function declarations were changed to method declarations, i.e. "SnnsCLib::.."
     was added
   * calls to the function table are now "C++-style", using the "this"-pointer

   License of SnnsCLib:
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

************************************************************************************/


/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse ppparse
//#define yylex   pplex
#define yyerror pperror
#define yylval  pplval
#define yychar  ppchar
#define yydebug ppdebug
#define yynerrs ppnerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     L_BRACKET = 258,
     R_BRACKET = 259,
     VERSION_HEADER = 260,
     GENERATED_AT = 261,
     NO_OF_PATTERN = 262,
     NO_OF_INPUT = 263,
     NO_OF_OUTPUT = 264,
     NO_OF_VAR_IDIM = 265,
     NO_OF_VAR_ODIM = 266,
     MAXIMUM_IDIM = 267,
     MAXIMUM_ODIM = 268,
     NO_OF_CLASSES = 269,
     CLASS_REDISTRIB = 270,
     REMAPFUNCTION = 271,
     REMAP_PARAM = 272,
     ERROR = 273,
     PATTERNEND = 274,
     PATTERNNOCLASS = 275,
     NUMBER = 276,
     NAME = 277,
     V_NUMBER = 278
   };
#endif
/* Tokens.  */
/*
#define L_BRACKET 258
#define R_BRACKET 259
#define VERSION_HEADER 260
#define GENERATED_AT 261
#define NO_OF_PATTERN 262
#define NO_OF_INPUT 263
#define NO_OF_OUTPUT 264
#define NO_OF_VAR_IDIM 265
#define NO_OF_VAR_ODIM 266
#define MAXIMUM_IDIM 267
#define MAXIMUM_ODIM 268
#define NO_OF_CLASSES 269
#define CLASS_REDISTRIB 270
#define REMAPFUNCTION 271
#define REMAP_PARAM 272
#define ERROR 273
#define PATTERNEND 274
#define PATTERNNOCLASS 275
#define NUMBER 276
#define NAME 277
#define V_NUMBER 278
*/



/* Copy the first part of user declarations.  */
//#line 20 "kr_pat_parse_bison.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SnnsCLib.h"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
//#line 36 "kr_pat_parse_bison.y"
{
	float	value;		/* allgemeine Zahl */	
        char    *name;          /* allgemeiner String */
	struct
	{
		int	v;
		int	r;
	}	version;	/* Versionsnummer #.# */
}
/* Line 187 of yacc.c.  */
//#line 175 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
//#line 188 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

/*20161216 Fix for kr_pat_parse.cpp:267:20: warning: ISO C++ 1998 does not support 'long long' [-Wlong-long]
 #  define YYSIZE_T __SIZE_TYPE__
*/

#define YYSIZE_T unsigned int

/*
#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> 
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif
*/

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   56

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  24
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  46
/* YYNRULES -- Number of states.  */
#define YYNSTATES  74

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   278

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23
};

/*
#if YYDEBUG
// YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
//   YYRHS.
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     8,     9,    10,    11,    12,    28,
      31,    34,    35,    40,    41,    46,    47,    50,    51,    59,
      60,    63,    64,    72,    73,    75,    78,    80,    81,    86,
      89,    91,    94,    96,    98,   101,   102,   103,   104,   105,
     114,   116,   117,   119,   122,   124,   126
};

// YYRHS -- A `-1'-separated list of the rules' RHS.
static const yytype_int8 yyrhs[] =
{
      25,     0,    -1,    -1,    27,    26,    46,    -1,    -1,    -1,
      -1,    -1,     5,    23,    28,     6,     7,    21,    29,    32,
      33,    34,    35,    30,    36,    31,    38,    -1,     8,    21,
      -1,     9,    21,    -1,    -1,    10,    21,    12,    42,    -1,
      -1,    11,    21,    13,    42,    -1,    -1,    14,    21,    -1,
      -1,    14,    21,    15,     3,    37,    40,     4,    -1,    -1,
      16,    22,    -1,    -1,    16,    22,    17,     3,    39,    40,
       4,    -1,    -1,    41,    -1,    40,    41,    -1,    21,    -1,
      -1,     3,    43,    44,     4,    -1,     3,     4,    -1,    45,
      -1,    44,    45,    -1,    21,    -1,    47,    -1,    46,    47,
      -1,    -1,    -1,    -1,    -1,    48,    52,    49,    53,    50,
      19,    51,    55,    -1,    42,    -1,    -1,    54,    -1,    53,
      54,    -1,    21,    -1,    20,    -1,    22,    -1
};

// YYRLINE[YYN] -- source line where rule number YYN was defined.
static const yytype_uint16 yyrline[] =
{
       0,    63,    63,    62,    91,   100,   109,   116,    90,   123,
     134,   144,   147,   165,   168,   187,   190,   195,   194,   220,
     225,   235,   234,   259,   262,   263,   266,   279,   278,   283,
     289,   290,   293,   310,   311,   315,   323,   388,   396,   315,
     412,   414,   420,   421,   424,   438,   442
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
// YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
//   First, the terminals, then, starting at YYNTOKENS, nonterminals. 
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "L_BRACKET", "R_BRACKET",
  "VERSION_HEADER", "GENERATED_AT", "NO_OF_PATTERN", "NO_OF_INPUT",
  "NO_OF_OUTPUT", "NO_OF_VAR_IDIM", "NO_OF_VAR_ODIM", "MAXIMUM_IDIM",
  "MAXIMUM_ODIM", "NO_OF_CLASSES", "CLASS_REDISTRIB", "REMAPFUNCTION",
  "REMAP_PARAM", "ERROR", "PATTERNEND", "PATTERNNOCLASS", "NUMBER", "NAME",
  "V_NUMBER", "$accept", "pattern_file", "@1", "header", "@2", "@3", "@4",
  "@5", "i_head", "o_head", "vi_head", "vo_head", "cl_head", "@6",
  "rm_head", "@7", "paramlist", "param", "actual_dim", "@8",
  "actual_dim_rest", "dim_entry", "pattern_list", "pattern", "@9", "@10",
  "@11", "@12", "pattern_start", "pattern_body", "pattern_entry",
  "pattern_class", 0
};
#endif
*/


# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    24,    26,    25,    28,    29,    30,    31,    27,    32,
      33,    33,    34,    34,    35,    35,    36,    37,    36,    36,
      38,    39,    38,    38,    40,    40,    41,    43,    42,    42,
      44,    44,    45,    46,    46,    48,    49,    50,    51,    47,
      52,    52,    53,    53,    54,    55,    55
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     0,     0,     0,     0,    15,     2,
       2,     0,     4,     0,     4,     0,     2,     0,     7,     0,
       2,     0,     7,     0,     1,     2,     1,     0,     4,     2,
       1,     2,     1,     1,     2,     0,     0,     0,     0,     8,
       1,     0,     1,     2,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     4,     1,    35,     0,    35,    33,
      41,     0,    34,    27,    40,    36,     0,    29,     0,     0,
       5,    32,     0,    30,    44,    37,    42,     0,    28,    31,
       0,    43,     0,    11,    38,     9,     0,    13,     0,    10,
       0,    15,    45,    46,    39,     0,     0,     6,     0,     0,
      19,    12,     0,     0,     7,    14,    16,    23,     0,     0,
       8,    17,    20,     0,     0,    26,     0,    24,    21,    18,
      25,     0,     0,    22
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     6,     3,     7,    27,    50,    57,    33,    37,
      41,    47,    54,    63,    60,    71,    66,    67,    14,    18,
      22,    23,     8,     9,    10,    19,    30,    38,    15,    25,
      26,    44
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -64
static const yytype_int8 yypact[] =
{
       1,   -13,    11,   -64,   -64,   -64,   -64,     6,    13,   -64,
      12,     7,   -64,    16,   -64,   -64,    -5,   -64,     0,     2,
     -64,   -64,    -4,   -64,   -64,     2,   -64,    14,   -64,   -64,
       5,   -64,     4,    17,   -64,   -64,     8,    18,   -15,   -64,
       9,    20,   -64,   -64,   -64,    15,    19,   -64,    12,    21,
      22,   -64,    12,    23,   -64,   -64,    24,    25,    29,    26,
     -64,   -64,    28,    30,    32,   -64,    -3,   -64,   -64,   -64,
     -64,    30,    -2,   -64
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -38,   -63,   -44,   -64,
     -64,    27,   -64,    34,   -64,   -64,   -64,   -64,   -64,   -64,
      31,   -64
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -4
static const yytype_int8 yytable[] =
{
      28,    69,    73,    70,    51,    42,     1,    43,    55,    70,
       4,     5,    11,    -3,    16,    13,    20,    21,    65,    65,
      17,    21,    32,    24,    34,    35,    36,    48,    40,    39,
      45,    46,    61,    72,    52,    68,    53,     0,     0,    58,
      49,    59,    12,     0,    56,    64,     0,     0,    62,    29,
       0,    65,     0,     0,     0,     0,    31
};

static const yytype_int8 yycheck[] =
{
       4,     4,     4,    66,    48,    20,     5,    22,    52,    72,
      23,     0,     6,     0,     7,     3,    21,    21,    21,    21,
       4,    21,     8,    21,    19,    21,     9,    12,    10,    21,
      21,    11,     3,    71,    13,     3,    14,    -1,    -1,    15,
      21,    16,     8,    -1,    21,    17,    -1,    -1,    22,    22,
      -1,    21,    -1,    -1,    -1,    -1,    25
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    25,    27,    23,     0,    26,    28,    46,    47,
      48,     6,    47,     3,    42,    52,     7,     4,    43,    49,
      21,    21,    44,    45,    21,    53,    54,    29,     4,    45,
      50,    54,     8,    32,    19,    21,     9,    33,    51,    21,
      10,    34,    20,    22,    55,    21,    11,    35,    12,    21,
      30,    42,    13,    14,    36,    42,    21,    31,    15,    16,
      38,     3,    22,    37,    17,    21,    40,    41,     3,     4,
      41,    39,    40,     4
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX pplex (YYLEX_PARAM)
#else
# define YYLEX pplex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
/*  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }*/									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
/*
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}
*/

/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/
/*
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}
*/

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  //YYFPRINTF (stderr, "Stack now");
  //for (; bottom <= top; ++bottom)
    //YYFPRINTF (stderr, " %d", *bottom);
  //YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  //unsigned long int yylno = yyrline[yyrule];
  //YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	//     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      //fprintf (stderr, "   $%d = ", yyi + 1);
      //yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
	//	       &(yyvsp[(yyi + 1) - (yynrhs)])
	//	       		       );
      //fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

//#ifdef YYPARSE_PARAM
//#if defined __STDC__ || defined __cplusplus
//int SnnsCLib::yyparse (void *YYPARSE_PARAM);
//#else
//int SnnsCLib::yyparse ();
//#endif
//#else /* ! YYPARSE_PARAM */
//#if defined __STDC__ || defined __cplusplus
//int SnnsCLib::yyparse (void);
//#else
//int SnnsCLib::yyparse ();
//#endif
//#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
SnnsCLib::yyparse (void *YYPARSE_PARAM)
#else
int
SnnsCLib::yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
SnnsCLib::yyparse (void)
#else
int
SnnsCLib::yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  //YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = (short) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (unsigned int) (yyssp - yyss + 1);

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      //YYDPRINTF ((stderr, "Stack size increased to %lu\n",
	//	  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  //YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      //YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      //YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
//#line 63 "kr_pat_parse_bison.y"
    {	
		    current_pattern = 0;
		    next_pattern_is_input = 1;

		    if (kr_np_AllocatePatternSet(&pattern_set, no_of_pattern)
			!= KRERR_NO_ERROR)
		    { 
			yyerror(const_cast<char*>("can't allocate memory")); 
			YYABORT; 
		    }
		}
    break;

  case 3:
//#line 75 "kr_pat_parse_bison.y"
    {
		    if (current_pattern < no_of_pattern ||
			!next_pattern_is_input)
		    { 
			yyerror(const_cast<char*>("unexpected end of file")); 
			YYABORT; 
		    }
		    if (with_classes != this_set->pub.classes)
		    {
			yyerror(const_cast<char*>("real number of classes does not match pattern file header"));
			YYABORT;
		    }
		}
    break;

  case 4:
//#line 91 "kr_pat_parse_bison.y"
    {
		if (((yyvsp[(2) - (2)].version).v == CURRENT_VERSION_V && (yyvsp[(2) - (2)].version).r > CURRENT_VERSION_R) ||
		    (yyvsp[(2) - (2)].version).v > CURRENT_VERSION_V)
		{ 
		    yyerror(const_cast<char*>("version of pattern file not supported")); 
		    YYABORT; 
		}
	}
    break;

  case 5:
//#line 100 "kr_pat_parse_bison.y"
    {
		no_of_pattern = (int) (yyvsp[(6) - (6)].value);
		if (no_of_pattern <= 0)
		{ 
		    yyerror(const_cast<char*>("illegal number of pattern")); 
		    YYABORT; 
		}
	}
    break;

  case 6:
//#line 109 "kr_pat_parse_bison.y"
    {
	    this_set->pub.classes = 0;
	    this_set->pub.class_names = NULL;
	    this_set->pub.class_distrib_active = FALSE;
	    this_set->pub.class_redistribution = NULL;
        }
    break;

  case 7:
//#line 116 "kr_pat_parse_bison.y"
    {
	    this_set->pub.remap_function = NULL;
	    this_set->pub.no_of_remap_params = 0;
	}
    break;

  case 9:
//#line 124 "kr_pat_parse_bison.y"
    { 
		no_of_input = (int) (yyvsp[(2) - (2)].value);
		if (no_of_input < 0)
		{ 
		    yyerror(const_cast<char*>("illegal number of input units")); 
		    YYABORT; 
		} 
	}
    break;

  case 10:
//#line 135 "kr_pat_parse_bison.y"
    { 
		no_of_output = (int) (yyvsp[(2) - (2)].value);
		if (no_of_output < 0)
		{ 
		    yyerror(const_cast<char*>("illegal number of output units")); 
		    YYABORT; 
		} 
	}
    break;

  case 11:
//#line 144 "kr_pat_parse_bison.y"
    { no_of_output = 0; }
    break;

  case 12:
//#line 148 "kr_pat_parse_bison.y"
    {
		variable_input_dim = (int) (yyvsp[(2) - (4)].value); 
		if (variable_input_dim < 0 || 
		    variable_input_dim > MAX_NO_OF_VAR_I_DIM)
		{ 
		    yyerror(const_cast<char*>("illegal variable input dimensions")); 
		    YYABORT; 
		}
		if (actual_dim_count != variable_input_dim)
		{ 
		    yyerror(const_cast<char*>("illegal number of entries in dimension list")); 
		    YYABORT; 
		}
		for (i=0; i<variable_input_dim; i++)
			max_i_dims[i] = dims[i];
	}
    break;

  case 13:
//#line 165 "kr_pat_parse_bison.y"
    { variable_input_dim = 0; }
    break;

  case 14:
//#line 169 "kr_pat_parse_bison.y"
    {
		variable_output_dim = (int) (yyvsp[(2) - (4)].value); 
		if (variable_output_dim < 0 || 
		    variable_output_dim > MAX_NO_OF_VAR_O_DIM ||
		    no_of_output == 0)
		{ 
		    yyerror(const_cast<char*>("illegal variable output dimensions")); 
		    YYABORT; 
		}
		if (actual_dim_count != variable_output_dim)
		{ 
		    yyerror(const_cast<char*>("illegal number of entries in dimension list")); 
		    YYABORT; 
		}
		for (i=0; i<variable_output_dim; i++)
			max_o_dims[i] = dims[i];	
	}
    break;

  case 15:
//#line 187 "kr_pat_parse_bison.y"
    { variable_output_dim = 0; }
    break;

  case 16:
//#line 191 "kr_pat_parse_bison.y"
    {
    with_classes = (int) (yyvsp[(2) - (2)].value);
}
    break;

  case 17:
//#line 195 "kr_pat_parse_bison.y"
    {
    with_classes = (int) (yyvsp[(2) - (4)].value);
    num_param = 0;
}
    break;

  case 18:
//#line 200 "kr_pat_parse_bison.y"
    {
    int i;

    this_set->pub.class_redistribution = 
	(int *) malloc(num_param * sizeof(int));
    if (!this_set->pub.class_redistribution)
    {
	yyerror(const_cast<char*>("can't allocate memory")); 
	YYABORT; 
    }
    for (i=0; i<num_param; i++)
	this_set->pub.class_redistribution[i] = abs((int) parameter[i]);
    this_set->pub.class_distrib_active = TRUE;
    if (num_param != with_classes)
    {
	yyerror(const_cast<char*>("contradictory definition of class redistribution and number of classes in pattern file header"));
	YYABORT;
    }
}
    break;

  case 19:
//#line 220 "kr_pat_parse_bison.y"
    {
    with_classes = 0;
}
    break;

  case 20:
//#line 226 "kr_pat_parse_bison.y"
    {
    this_set->pub.remap_function = strdup((yyvsp[(2) - (2)].name));
    if (!this_set->pub.remap_function)
    {
	yyerror(const_cast<char*>("can't allocate memory")); 
	YYABORT; 
    }
}
    break;

  case 21:
//#line 235 "kr_pat_parse_bison.y"
    {
    num_param = 0;
}
    break;

  case 22:
//#line 239 "kr_pat_parse_bison.y"
    {
    int i;

    this_set->pub.remap_function = strdup((yyvsp[(2) - (7)].name));
    if (!this_set->pub.remap_function)
    {
	yyerror(const_cast<char*>("can't allocate memory")); 
	YYABORT; 
    }

    if (num_param > NO_OF_REMAP_PARAMS)
    {
	yyerror(const_cast<char*>("to many parameters for remap function"));
	YYABORT;
    }

    this_set->pub.no_of_remap_params = num_param;
    for (i=0; i<num_param; i++)
	this_set->pub.remap_params[i] = parameter[i];
}
    break;

  case 26:
//#line 267 "kr_pat_parse_bison.y"
    {
    num_param++;
    if (num_param > 1)
	parameter = (float *) realloc(parameter, num_param * sizeof(float));
    else
	parameter = (float *) malloc(sizeof(float));

    parameter[num_param-1] = (yyvsp[(1) - (1)].value);
}
    break;

  case 27:
//#line 279 "kr_pat_parse_bison.y"
    {
			actual_dim_count = 0;
		}
    break;

  case 29:
//#line 284 "kr_pat_parse_bison.y"
    {
		        actual_dim_count = 0;
		}
    break;

  case 32:
//#line 294 "kr_pat_parse_bison.y"
    {
		if (actual_dim_count >= MAX_NO_OF_VAR_DIM)
		{ 
		    yyerror(const_cast<char*>("to many entries in dimension list")); 
		    YYABORT; 
		}
		dims[actual_dim_count] = (int) (yyvsp[(1) - (1)].value);
		if (dims[actual_dim_count] <= 0)
		{ 
		    yyerror(const_cast<char*>("illegal size of dimension")); 
		    YYABORT; 
		}
		actual_dim_count++;
	}
    break;

  case 35:
//#line 315 "kr_pat_parse_bison.y"
    {
    if (current_pattern >= no_of_pattern)
    { 
	yyerror(const_cast<char*>("to many patterns")); 
	YYABORT; 
    }
}
    break;

  case 36:
//#line 323 "kr_pat_parse_bison.y"
    {
    if (kr_np_GetDescriptor(pattern_set, current_pattern, &pattern)
	!= KRERR_NO_ERROR)
    {
	yyerror(const_cast<char*>("pattern parser internal error"));
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
	    yyerror(const_cast<char*>("illegal number of entries in dimension list")); 
	    YYABORT; 
	}
	act_size = no_of_input;
	for (i=0; i<variable_input_dim; i++)
	{
	    if (dims[i] > max_i_dims[i])
	    { 
		yyerror(const_cast<char*>("variable dimension overflow")); 
		YYABORT; 
	    }
	    act_size *= dims[i];
	    (pattern -> pub.input_dim_sizes)[i] = dims[i];
	}
	if (kr_np_AllocatePattern(pattern, next_pattern_is_input)
	    != KRERR_NO_ERROR)
	{ 
	    yyerror(const_cast<char*>("can't allocate memory")); 
	    YYABORT; 
	}
	pat_mem = pattern -> input_pattern;
    }
    else
    {
	if (actual_dim_count != variable_output_dim)
	{ 
	    yyerror(const_cast<char*>("illegal number of entries in dimension list")); 
	    YYABORT; 
	}
	act_size = no_of_output;
	for (i=0; i<variable_output_dim; i++)
	{
	    if (dims[i] > max_o_dims[i])
	    { 
		yyerror(const_cast<char*>("variable dimension overflow")); 
		YYABORT; 
	    }
	    act_size *= dims[i];
	    (pattern -> pub.output_dim_sizes)[i] = dims[i];
	}
	if (kr_np_AllocatePattern(pattern, next_pattern_is_input)
	    != KRERR_NO_ERROR)
	{ 
	    yyerror(const_cast<char*>("can't allocate memory")); 
	    YYABORT; 
	}
	pat_mem = pattern -> output_pattern;
    }
}
    break;

  case 37:
//#line 388 "kr_pat_parse_bison.y"
    {
    if (act_size > 0)
    { 
	yyerror(const_cast<char*>("to little values in pattern")); 
	YYABORT; 
    }
}
    break;

  case 38:
//#line 396 "kr_pat_parse_bison.y"
    {
    if (with_classes == 0 || (next_pattern_is_input && no_of_output > 0))
	scanner_await_no_class();
    else
	scanner_await_class();
}
    break;

  case 39:
//#line 403 "kr_pat_parse_bison.y"
    {
    if (no_of_output > 0)
	next_pattern_is_input = !next_pattern_is_input;
    
    if (next_pattern_is_input)
	current_pattern++;
}
    break;

  case 41:
//#line 414 "kr_pat_parse_bison.y"
    {
    actual_dim_count = 0;
}
    break;

  case 44:
//#line 425 "kr_pat_parse_bison.y"
    {
		if (act_size == 0)
		{ 
		    yyerror(const_cast<char*>("to many values in pattern")); 
		    YYABORT; 
		}
		*pat_mem++ = (yyvsp[(1) - (1)].value);
		if (--act_size == 0)
		    scanner_await_pattern_end(); 
	}
    break;

  case 45:
//#line 439 "kr_pat_parse_bison.y"
    {
    pattern->mysym = NULL;
}
    break;

  case 46:
//#line 443 "kr_pat_parse_bison.y"
    {
    if (kr_np_lookupSym(pattern_set, (yyvsp[(1) - (1)].name), &this_class) != KRERR_NO_ERROR)
    { 
	yyerror(const_cast<char*>("can't allocate memory")); 
	YYABORT; 
    }
    if (this_class->set_amount == 0)
	this_set->pub.classes++;
    this_class->set_amount++;
    pattern->mysym = this_class;    
}
    break;


/* Line 1267 of yacc.c.  */
//#line 1886 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_(const_cast<char*>("syntax error")));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_(const_cast<char*>("syntax error")));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct (const_cast<char*>("Error: discarding"),
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct (const_cast<char*>("Error: popping"),
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT (const_cast<char*>("Shifting"), yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_(const_cast<char*>("memory exhausted")));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct (const_cast<char*>("Cleanup: discarding lookahead"),
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct (const_cast<char*>("Cleanup: popping"),
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


//#line 456 "kr_pat_parse_bison.y"


/*****************************************************************************
  FUNCTION : yyerror

  PURPOSE  : must be provided for the parser.
             reports errors in pattern file to stderr
  RETURNS  : nothing
  NOTES    :

  UPDATE   : 
******************************************************************************/
void SnnsCLib::yyerror(char *error)
{
    //fprintf(stderr, "Parse error in pattern file at line %d:\n%s\n",
	//    lineno, error);
}

/*****************************************************************************
  FUNCTION : parse_pattern_file

  PURPOSE  : calls the real parser

  RETURNS  : parser error code and the handle to the loaded pattern set
  NOTES    :

  UPDATE   : 
******************************************************************************/
int SnnsCLib::parse_pattern_file(int *set, np_pattern_set_info *set_info)
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


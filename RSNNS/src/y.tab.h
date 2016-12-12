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

/* Skeleton interface for Bison's Yacc-like parsers in C

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



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 36 "kr_pat_parse_bison.y"
{
	float	value;		/* allgemeine Zahl */	
        char    *name;          /* allgemeiner String */
	struct
	{
		int	v;
		int	r;
	}	version;	/* Versionsnummer #.# */
}
/* Line 1489 of yacc.c.  */
#line 105 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE pplval;


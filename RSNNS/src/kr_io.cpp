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


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_io.c,v $
  SHORTNAME      : kr_io.c
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel User Interface File I/O
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 28.05.90

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.18 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:39 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

 ******************************************************************************/

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#include "SnnsCLib.h"
//#include "SnnsCLib_util.h"

/*
#ifdef ultrix
#define retchk( ret_code )  if ((ret_code) == EOF)  return( KRERR_IO )
#define RETCHKGTO( ret_code )  if ((ret_code) == EOF)  goto ende
#else
#define retchk( ret_code )  if ((ret_code) < 0)  return( KRERR_IO )
#define RETCHKGTO( ret_code )  if ((ret_code) < 0)  goto ende
#endif
 */

#define retchk( stream )  if (!stream->good())  return( KRERR_IO )
#define RETCHKGTO( stream )  if (!stream->good())  goto ende



//--------------------------------
//kr_io.ph
//--------------------------------

const char* SnnsCLib::resHeader[] = {
		"SNNS result file %s\n",
		"generated at",
		"No. of patterns     : %d\n",
		"No. of input units  : %d\n",
		"No. of output units : %d\n",
		"startpattern        : %d\n",
		"endpattern          : %d\n",
		"input patterns included\n",
		"teaching output included\n"
};
/*
const char* SnnsCLib::headers[] = {
		" site name | site function",
		" name | act func | out func | sites",
		" act | bias | st | subnet | layer | act func | out func",
		" no. | typeName | unitName | act | bias | st | position | act func | out func | sites",
		" target | site | source:weight",
		" subnet | unitNo.",
		" layer | unitNo.",
		" delta x | delta y | z",
		" no. | LLN | LUN | Toff | Soff | Ctype"
};
*/
const char* SnnsCLib::title[] = {
		"SNNS network definition file",
		"generated at",
		"network name",
		"no. of units",
		"no. of connections",
		"no. of unit types",
		"no. of site types",
		"learning function",
		"site definition section",
		"type definition section",
		"unit definition section",
		"connection definition section",
		"subnet definition section",
		"unit default section",
		"source files",
		"layer definition section",
		"update function",
		"3D translation section",
		"time delay section",
		"pruning function",
		"subordinate learning function"
		/*
    "topologic definition section",
    "feedforward network"
		 */
};	 /*  20  */

const int SnnsCLib::NoOfTitles = (sizeof title) / (sizeof title[0]);


/*****************************************************************************
  FUNCTION : mstrcat 

  PURPOSE  : 
  NOTES    : mstrcat is called
             mstrcat( dest, src1, src2, ... , (char *) 0 )

  RETURNS  :
  UPDATE   : 
 ******************************************************************************/
void  SnnsCLib::mstrcat(char *va_alist,...)

{
	va_list  args;
	char	  *dest, *src;


	va_start( args, va_alist );
	if ((dest = va_alist) == (char *) 0)  return;

	while ((src = va_arg( args, char * )) != (char *) 0)
		strcat( dest, src );

	va_end( args );
}

/*****************************************************************************
  FUNCTION : mstrcpy 

  PURPOSE  : 
  NOTES    : mstrcpy is called
             mstrcpy ( dest, src1, src2, ... , (char *) 0 )

  RETURNS  :
  UPDATE   : 
 ******************************************************************************/
void  SnnsCLib::mstrcpy(char *va_alist,...)
{
	va_list  args;
	char	  *dest, *src;


	va_start( args, va_alist );
	if ((dest = va_alist) == (char *) 0)  return;
	if ((src  = va_arg( args, char * )) == (char *) 0)  return;
	while( *src != EOS )
		*dest++ = *src++;

	while ((src = va_arg( args, char * )) != (char *) 0)
	{
		while( *src != EOS )
			*dest++ = *src++;
	}

	va_end( args );

	*dest = EOS;
}


/*****************************************************************************
  FUNCTION : krio_getIOVersion

  PURPOSE  : returns the current file I/O version number
  NOTES    :

  RETURNS  :
  UPDATE   : 
 ******************************************************************************/
char  *SnnsCLib::krio_getIOVersion(void)
{
	//static char	krio_getIOVersion_getIOVersion[128];

	strcpy( krio_getIOVersion_getIOVersion, NETFILE_VERSION );

	strcat( krio_getIOVersion_getIOVersion, KERNEL3D_NETFILE_VERSION );


	return( krio_getIOVersion_getIOVersion );
}


/*#################################################

GROUP: Kernel File Output Functions

#################################################*/

/*****************************************************************************
  FUNCTION : krio_writeHeader

  PURPOSE  : 
  NOTES    :

  RETURNS  :
  UPDATE   : 
 ******************************************************************************/
krui_err  SnnsCLib::krio_writeHeader(char *version, char *net_name)
{
	long  clock;
	//  int   err;
	int   no_of_sites,
	no_of_STable_entries,
	no_of_FTable_entries;
	char *learn_func;


	krui_getNetInfo( &no_of_sites, &kr_io_NoOfLinks, &no_of_STable_entries,
			&no_of_FTable_entries );
	units_have_sites = no_of_sites > 0;

	clock = 1;
	(void) time( (time_t *) &clock);

	char buf[MAX_LIN_LEN];

	snprintf( buf, sizeof(buf), "%s %s\n%s %s\n%s : ",
			title[0], version, title[1], 
			ctime( (time_t *) &clock), title[2] );

	*stream_out << buf;
	retchk( stream_out );

	if (net_name == NULL) {
		snprintf( buf, sizeof(buf), "UNTITLED\n" );
		*stream_out << buf;
	}
	else {
		snprintf( buf, sizeof(buf), "%s\n", net_name );
		*stream_out << buf;
	}

	retchk( stream_out );
	snprintf( buf, sizeof(buf), "%s :\n%s : %i\n%s : %i\n%s : %i\n%s : %i\n",
			title[14],
			title[3], NoOfUnits,
			title[4], kr_io_NoOfLinks,
			title[5], no_of_FTable_entries,
			title[6], no_of_STable_entries );
	*stream_out << buf;
	retchk( stream_out );

	learn_func = krui_getLearnFunc();

	snprintf( buf, sizeof(buf), "\n\n%s : %s\n",
			title[7], learn_func );
	*stream_out << buf;
	retchk( stream_out );
	snprintf( buf, sizeof(buf), "%s   : %s\n",
			title[16], krui_getUpdateFunc() );
	*stream_out << buf;
	retchk( stream_out );

	if (strcmp (learn_func, "PruningFeedForward") == 0)
	{
		snprintf( buf, sizeof(buf), "%s   : %s\n",
				title[19], krui_getPrunFunc() );
		*stream_out << buf;
		retchk( stream_out );
		snprintf( buf, sizeof(buf), "%s   : %s\n",
				title[20], krui_getFFLearnFunc() );
		*stream_out << buf;
		retchk( stream_out );
	}

	return( KRERR_NO_ERROR );
}
/*****************************************************************************
  FUNCTION : krio_cutTrailingZeros

  PURPOSE  : cut all trailing '0'  
  NOTES    :

  RETURNS  :
  UPDATE   : 
 ******************************************************************************/
void SnnsCLib::krio_cutTrailingZeros(char *string)
{
	int j;


	if (*string == EOS)  return;
	for (j = strlen( string ) - 1; ((string[j] == '0') && (j > 0)); j--) {}

	if (string[j] == '.')
		string[j] = EOS;
	else
		string[j + 1] = EOS;
}

/*****************************************************************************
  FUNCTION : krio_repchar 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ****************************************************************************/
char  *SnnsCLib::krio_repchar(char c, int N)
{
	int   i;
	//static char  krio_repchar_str[180];

	if ( (N - 1) >= (int) sizeof (krio_repchar_str))   N = sizeof (krio_repchar_str) - 1;

	for (i = 0; i < N; i++)
		krio_repchar_str[ i ] = c;

	krio_repchar_str[N] = 0;
	return( krio_repchar_str );
}

/*****************************************************************************
  FUNCTION : krio_stringLimits 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/


void  SnnsCLib::krio_stringLimits(void)
{
	char  *symbol, *name;
	int   sym_type,
	unit_no,
	len, i1, i2, i3, i4, i5,
	subnet_no, layer_no;
	FlintType  act, bias;
	int	st;
	char	*def_act_func,	*def_out_func;

	struct  PosType   pos;
	bool	  neg1, neg2, neg3;


	unit_name_len = site_name_len = type_name_len = 0;
	out_func_len = act_func_len = site_func_len = 0;
	def_out_func_len = def_act_func_len = 0;

	/*  get default subnet and layer numbers  */
	krui_getUnitDefaults( &act, &bias, &st, &subnet_no, &layer_no,
			&def_act_func, &def_out_func );

	if ( krui_getFirstSymbolTableEntry( &symbol, &sym_type) )  {
		do  {
			len = strlen( symbol );

			switch (sym_type)  {
			case  UNIT_SYM:
				unit_name_len = Max( unit_name_len, len );
				break;
			case  SITE_SYM:
				site_name_len = Max( site_name_len, len );
				break;
			case  FTYPE_UNIT_SYM:
				type_name_len = Max( type_name_len, len );
			}
		}
		while ( krui_getNextSymbolTableEntry( &symbol, &sym_type ) );
	}

	unit_no = krui_getFirstUnit();
	do  {
		symbol = krui_getUnitOutFuncName( unit_no );
		if (strcmp( symbol, def_out_func ) != 0)
			out_func_len = Max( out_func_len, (int) strlen( symbol ));

		symbol = krui_getUnitActFuncName( unit_no );
		if (strcmp( symbol, def_act_func ) != 0)
			act_func_len = Max( act_func_len, (int) strlen( symbol ));
	}
	while ((unit_no = krui_getNextUnit() ) != 0);


	def_out_func_len = strlen( def_out_func );
	def_act_func_len = strlen( def_act_func );

	if (krui_getFirstSiteTableEntry( &name, &symbol ))  {  
		do
			site_func_len = Max( site_func_len, (int) strlen( symbol ));
		while (krui_getNextSiteTableEntry( &name, &symbol ));
	}

	unit_no = krui_getFirstUnit();

	neg1 = neg2 = neg3 = is_subnet_info =
			is_layer_info = FALSE;
	i1 = i2 = i3 = 0;

	do  {
		krui_getUnitPosition( unit_no, &pos );
		i1 = Max( i1, abs( pos.x ) );
		i1 = Max( i1, abs( pos.y ) );
		if (pos.x < 0) neg1 = TRUE;
		if (pos.y < 0) neg1 = TRUE;

		i5 = krui_getUnitSubnetNo( unit_no );
		if (i5 < 0) neg2 = TRUE;
		i2 = Max( i2, abs( i5 ) );
		if (i2 != subnet_no)  is_subnet_info = TRUE;

		i4 = (int) krui_getUnitLayerNo( unit_no );
		if (i4 < 0) neg2 = TRUE;
		i3 = Max( i3, abs( i4 ) );
		if (i3 != layer_no) is_layer_info = TRUE;
	}
	while ( (unit_no = krui_getNextUnit() ) > 0);

	pos_no_len = 1;
	while ( (i1 /= 10) != 0)
		++pos_no_len;
	if (neg1)  pos_no_len++;

	subnet_no_len = 1;
	while ( (i2 /= 10) != 0)
		++subnet_no_len;
	if (neg2)  subnet_no_len++;

	layer_no_len = 1;
	while ( (i3 /= 10) != 0)
		++layer_no_len;
	if (neg3)  layer_no_len++;

	len = NoOfUnits;
	unit_no_len = 1;
	while ( (len /= 10) != 0)
		++unit_no_len;

}


/*****************************************************************************
  FUNCTION : krio_fmtShapeing

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

krui_err  SnnsCLib::krio_fmtShapeing(int choose_me)
{
	//static char   *krio_fmtShapeing_formats[] = {
	//              /*  0   */   "site name",
	//              /*  1   */   "sites",
	//              /*  2   */   "unitName",
	//              /*  3   */   "typeName",
	//              /*  4   */   "name",
	//              /*  5   */   "site function",
	//              /*  6   */   "act func",
	//              /*  7   */   "out func",
	//              /*  8   */   "no.",
	//              /*  9   */   "target",
	//              /*  10  */   "act",
	//              /*  11  */   "bias",
	//              /*  12  */   "position",
	//              /*  13  */   "site",
	//              /*  14  */   " source:weight",
	//              /*  15  */   "subnet",
	//              /*  16  */   "layer",
	//              /*  17  */   " unitNo."
	//                           };

	char  fmt [ MAX_LIN_LEN ],
	aux [ MAX_LIN_LEN ];

	int	len, len1, act_len, out_len;


	switch( choose_me )
	{
	case  SITE_DEF:
		if (site_name_len + site_func_len + 5 > MAX_LIN_LEN)
			return( KRERR_SAVE_LINE_LEN );
		len = Max( site_name_len, (int) strlen( krio_fmtShapeing_formats[ 0 ] ) );
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt_shape1, " %-", aux, "s | %s\n", (char *) 0 );

		mstrcpy( fmt, " %-", aux, "s | ", (char *) 0 );
		snprintf( fmt_hdr1, sizeof(fmt_hdr1), fmt, krio_fmtShapeing_formats[ 0 ] );
		mstrcat( fmt_hdr1, krio_fmtShapeing_formats[ 5 ], "\n", (char *) 0 );

		mstrcpy( fmt_hdr2 ,krio_repchar( '-', len + 2 ), "|", (char *) 0 );

		len = Max( site_func_len, (int) strlen( krio_fmtShapeing_formats[ 6 ] ));
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "\n", (char *) 0 );

		break;

	case  TYPE_DEF:
		if (act_func_len == 0)	act_len = def_act_func_len;
		else  act_len = act_func_len;
		if (out_func_len == 0)	out_len = def_out_func_len;
		else  out_len = out_func_len;

		len = act_len + out_len + site_name_len + type_name_len + 11;
		if (len > MAX_LIN_LEN)	return( KRERR_SAVE_LINE_LEN );

		len = Max( type_name_len, (int) strlen( krio_fmtShapeing_formats[ 4 ] ));
		len1 = len + 2;
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt_shape1, "%-", aux, "s |", (char *) 0 );
		snprintf( fmt_hdr1, sizeof(fmt_hdr1), fmt_shape1, krio_fmtShapeing_formats[ 4 ] );
		mstrcpy( fmt_hdr2, krio_repchar( '-', len + 1 ), "|", (char *) 0 );


		len = Max( act_len, (int) strlen( krio_fmtShapeing_formats[ 6 ] ) );
		len1 = len1 + len + 3;
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );
		strcat( fmt_shape1, fmt );
		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 6 ] );
		strcat( fmt_hdr1, aux );
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );

		len = Max( out_len, (int) strlen( krio_fmtShapeing_formats[ 7 ] ) );
		len1 = len1 + len + 3;
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );
		strcat( fmt_shape1, fmt );

		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 7 ] );
		mstrcat( fmt_hdr1, aux, krio_fmtShapeing_formats[1], "\n", (char *) 0 );
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );
		mstrcpy( fmt_blank, ",\n", krio_repchar( ' ', len1 ), (char *) 0 );

		len = Max( site_name_len, (int) strlen( krio_fmtShapeing_formats[ 1 ] ) );
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "\n", (char *) 0 );

		break;

	case  DEFAULT_DEF:
		len = Max( 8, strlen( krio_fmtShapeing_formats[ 10 ] ) );
		len1 = len + 2;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		strcpy( fmt_shape1, "%8.5f |" );

		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, "%-", aux, "s |", (char *) 0 );
		snprintf( fmt_hdr1, sizeof(fmt_hdr1), fmt, krio_fmtShapeing_formats[ 10 ] );

		mstrcpy( fmt_hdr2, krio_repchar( '-', len + 1 ), "|", (char *) 0 );

		len = Max( 8, strlen( krio_fmtShapeing_formats[ 11 ] ) );
		len1 = len1 + len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		strcat( fmt_shape1, " %8.5f |" );

		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );
		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 11 ] );
		strcat( fmt_hdr1, aux );

		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );

		len1 = len1 + 5;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		strcat( fmt_shape1, " %s  |" );
		strcat( fmt_hdr1, " st |" );
		strcat( fmt_hdr2, "----|" );

		len = Max( subnet_no_len, (int) strlen( krio_fmtShapeing_formats[ 15 ] ) );
		len1 += len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcat( fmt_shape1, " %", aux, "d |", (char *) 0 );

		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );
		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 15 ] );
		strcat( fmt_hdr1, aux );

		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );

		len = Max( layer_no_len, (int) strlen( krio_fmtShapeing_formats[ 16 ] ) );
		len1 += len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcat( fmt_shape1, " %", aux, "d |", (char *) 0 );

		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );
		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 16 ] );
		strcat( fmt_hdr1, aux );

		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );

		len = Max( def_act_func_len, (int) strlen( krio_fmtShapeing_formats[ 6 ] ) );
		len1 += len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, " %-", aux, "s | ", (char *) 0 );
		strcat( fmt_shape1, fmt );
		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 6 ] );
		mstrcat( fmt_hdr1, aux, krio_fmtShapeing_formats[7], "\n", (char *) 0 );

		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );


		len = Max( def_out_func_len, (int) strlen( krio_fmtShapeing_formats[ 7 ] ) );
		len1 += len + 1;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcat( fmt_shape1, "%-", aux, "s \n", (char *) 0 );

		mstrcat( fmt_hdr2, krio_repchar( '-', len + 1 ), "\n", (char *) 0 );

		break;

	case  UNIT_DEF:
		len = Max( unit_no_len, (int) strlen( krio_fmtShapeing_formats[ 8 ] ) );
		len1 = len + 2;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );

		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt_shape1, "%", aux, "d |", (char *) 0 );

		mstrcpy( fmt, "%", aux, "s |", (char *) 0 );
		snprintf( fmt_hdr1, sizeof(fmt_hdr1), fmt, krio_fmtShapeing_formats[ 8 ] );

		mstrcpy( fmt_hdr2, krio_repchar( '-', len + 1 ), "|", (char *) 0);


		len = Max( type_name_len, (int) strlen( krio_fmtShapeing_formats[ 3 ] ) );
		len1 += len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );

		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );
		strcat( fmt_shape1, fmt );

		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 3 ] );
		strcat( fmt_hdr1, aux );
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );


		len = Max( unit_name_len, (int) strlen( krio_fmtShapeing_formats[ 2 ] ) );
		len1 += len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );

		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );
		strcat( fmt_shape1, fmt );

		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 2 ] );
		strcat( fmt_hdr1, aux );
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );


		len = Max( 8, (int) strlen( krio_fmtShapeing_formats[ 10 ] ) );
		len1 += len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );

		strcat( fmt_shape1, " %8.5f |" );
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );

		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 10 ] );
		strcat( fmt_hdr1, aux );
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );


		len = Max( 8, (int) strlen( krio_fmtShapeing_formats[ 11 ] ) );
		len1 = len1 + len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		strcat( fmt_shape1, " %8.5f |" );
		snprintf( aux, sizeof(aux), "%d", len );

		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );

		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 11 ] );
		strcat( fmt_hdr1, aux );
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );


		len1 += 5;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		strcat( fmt_shape1, " %s  |" );
		strcat( fmt_hdr1, " st |" );
		strcat( fmt_hdr2, "----|" );


		len = Max( pos_no_len * 3 + 2, (int) strlen( krio_fmtShapeing_formats[ 12 ] ) );

		len1 += len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		snprintf( aux, sizeof(aux), "%d", pos_no_len );
		mstrcpy( fmt, " %", aux, "d,", (char *) 0 );
		strcat( fmt_shape1, fmt );

		snprintf( aux, sizeof(aux), "%d", pos_no_len );
		mstrcpy( fmt, "%", aux, "d", (char *) 0 );


		strcat( fmt_shape1, fmt );
		snprintf( aux, sizeof(aux), "%d", pos_no_len );
		mstrcpy( fmt, ",%", aux, "d", (char *) 0 );


		if ((pos_no_len * 3 + 2) < len)
			strcat( fmt, krio_repchar( ' ', len - (pos_no_len * 3 + 2)) );


		strcat( fmt, " |" );
		strcat( fmt_shape1, fmt );

		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );

		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 12 ] );
		strcat( fmt_hdr1, aux );
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );


		len = Max( act_func_len, (int) strlen( krio_fmtShapeing_formats[ 6 ] ) );
		len1 += len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		strcpy( fmt_shape2, fmt_shape1 );
		strcat( fmt_shape2, "||" );
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );
		strcat( fmt_shape1, fmt );

		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 6 ] );
		strcat( fmt_hdr1, aux );
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );


		len = Max( out_func_len, (int) strlen( krio_fmtShapeing_formats[ 7 ] ) );
		len1 += len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, " %-", aux, "s | ", (char *) 0 );
		strcat( fmt_shape1, fmt );

		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 7 ] );
		mstrcat( fmt_hdr1, aux, krio_fmtShapeing_formats[1], "\n", (char *) 0 );

		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );
		mstrcpy( fmt_blank, ",\n", krio_repchar( ' ', len1 ), (char *) 0 );


		len = Max( site_name_len, (int) strlen( krio_fmtShapeing_formats[ 1 ] ) );
		len1 += len + 2;
		if (len1 + len > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "\n", (char *) 0 );

		break;

	case  CONNECT_DEF:
		snprintf( aux, sizeof(aux), "%d", unit_no_len );
		mstrcpy( fmt_shape3, " %", aux, "d:%8.5f", (char *) 0 );
		mstrcpy( fmt_shape4, " %", aux, "d:%8.5f (%8.5f,%8.5f) ", (char *) 0 );
		len = Max( unit_no_len, (int) strlen( krio_fmtShapeing_formats[ 9 ] ) );
		len1 = len + 2;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt_shape1, "%", aux, "d |", (char *) 0 );

		mstrcpy( fmt, "%", aux, "s |", (char *) 0 );
		snprintf( fmt_hdr1, sizeof(fmt_hdr1), fmt, krio_fmtShapeing_formats[ 9 ] );
		mstrcpy( fmt_hdr2, krio_repchar( '-', len + 1 ), "|", (char *) 0 );
		strcpy( fmt_shape2, krio_repchar( ' ', len1 ) );

		len = Max( site_name_len, (int) strlen( krio_fmtShapeing_formats[ 13 ] ) );
		len1 = len1 + len + 3;
		if (len1 > MAX_LIN_LEN)  return( KRERR_SAVE_LINE_LEN );
		snprintf( aux, sizeof(aux), "%d", len );


		mstrcpy( fmt, " %-", aux, "s |", (char *) 0 );
		strcat( fmt_shape1, fmt );
		strcat( fmt_shape2, fmt );
		snprintf( aux, sizeof(aux), fmt, krio_fmtShapeing_formats[ 13 ] );
		mstrcat( fmt_hdr1, aux, krio_fmtShapeing_formats[14], "\n", (char *) 0 );

		mstrcat( fmt_hdr2, krio_repchar( '-', len + 2 ), "|", (char *) 0 );
		mstrcpy( fmt_blank, "\n", krio_repchar( ' ', len1 ), (char *) 0 );

		if ((len1 + unit_no_len + 14) > MAX_LIN_LEN)  
			return( KRERR_SAVE_LINE_LEN );
		max_connects_per_line = (BEST_LINE_LEN - len1) / (unit_no_len + 11);
		mstrcat( fmt_hdr2,
				krio_repchar( '-', 
						(unit_no_len + 11) * max_connects_per_line ),
						"\n", (char *) 0 );

		break;

	case  SUBNET_DEF:
		len = Max( subnet_no_len, (int) strlen( krio_fmtShapeing_formats[ 15 ] ) );
		len1 = len + 1;
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt_shape1, "\n%", aux, "d |", (char *) 0 );

		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt, "%", aux, "s |", (char *) 0 );
		snprintf( fmt_hdr1, sizeof(fmt_hdr1), fmt, krio_fmtShapeing_formats[ 15 ] );
		strcat( fmt_hdr1, krio_fmtShapeing_formats[17] );
		mstrcpy( fmt_hdr2, "\n", krio_repchar( '-', len1 ), "|", (char *) 0 );
		mstrcpy( fmt_blank, "\n", krio_repchar( ' ', len1 + 1 ), (char *) 0 );

		snprintf( aux, sizeof(aux), "%d", unit_no_len );
		mstrcat( fmt_shape1, " %", aux, "d", (char *) 0 );
		mstrcpy( fmt_shape2, " %", aux, "d", (char *) 0 );

		max_subnets_per_line = (BEST_LINE_LEN - len1 - 1) / (unit_no_len + 2);
		
		//fixing CRAN-fun, 20210808
		//strcat( fmt_hdr2, krio_repchar( '-', (unit_no_len + 2) * max_subnets_per_line ) );
                mstrcat( fmt_hdr2, krio_repchar( '-', (unit_no_len + 2) * max_subnets_per_line ), (char *) 0 );
		break;

	case  LAYER_DEF:
		len = Max( layer_no_len, (int) strlen( krio_fmtShapeing_formats[ 16 ] ) );
		len1 = len + 1;
		snprintf( aux, sizeof(aux), "%d", len );
		mstrcpy( fmt_shape1, "\n%", aux, "d |", (char *) 0 );

		mstrcpy( fmt, "%", aux, "s |", (char *) 0 );
		snprintf( fmt_hdr1, sizeof(fmt_hdr1), fmt, krio_fmtShapeing_formats[ 16 ] );
		strcat( fmt_hdr1, krio_fmtShapeing_formats[17] );

		mstrcpy( fmt_hdr2, "\n", krio_repchar( '-', len1 ), "|", (char *) 0 );
		mstrcpy( fmt_blank, "\n", krio_repchar( ' ', len1 + 1 ), (char *) 0 );

		snprintf( aux, sizeof(aux), "%d", unit_no_len );
		mstrcat( fmt_shape1, " %", aux, "d", (char *) 0 );
		mstrcpy( fmt_shape2, " %", aux, "d", (char *) 0 );

		max_layers_per_line = (BEST_LINE_LEN - len1 - 1) / (unit_no_len + 2);
		
		//fixing CRAN-fun, 20210808
		//strcat( fmt_hdr2, krio_repchar( '-', (unit_no_len + 2) * max_layers_per_line ) );
                mstrcat( fmt_hdr2, krio_repchar( '-', (unit_no_len + 2) * max_layers_per_line ), (char *) 0 );
		break;
	}

	return( 0 );
}



/*****************************************************************************
  FUNCTION : krio_writeSiteDefinitions 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

krui_err  SnnsCLib::krio_writeSiteDefinitions(void)
{
	//int   err;
	char  *site_name,
	*site_func;

	char buf[MAX_LIN_LEN];

	if ( !krui_getFirstSiteTableEntry( &site_name, &site_func ) )
		return( 0 );

	//err = 
	krio_fmtShapeing( SITE_DEF );
	retchk( stream_out );

	snprintf( buf, sizeof(buf), "\n\n%s :\n\n", title[8] );
	*stream_out << buf;
	retchk( stream_out );

	*stream_out << fmt_hdr1;
	retchk( stream_out );

	*stream_out << fmt_hdr2;
	retchk( stream_out );

	do
	{
		snprintf( buf, sizeof(buf), fmt_shape1,
				site_name, site_func );
		*stream_out << buf;
		retchk( stream_out );
	}
	while (krui_getNextSiteTableEntry( &site_name, &site_func ) );


	*stream_out << fmt_hdr2;
	retchk( stream_out );
	return( 0 );
}



/*****************************************************************************
  FUNCTION : krio_writeTypeDefinitions 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

krui_err  SnnsCLib::krio_writeTypeDefinitions(void)
{
	int   err;
	bool  second;

	char buf[MAX_LIN_LEN];

	if ( !krui_setFirstFTypeEntry() )
		return( 0 );

	err = krio_fmtShapeing( TYPE_DEF );
	retchk( stream_out );

	snprintf( buf, sizeof(buf), "\n\n%s :\n\n", title[9] );
	*stream_out << buf;
	retchk( stream_out );

	*stream_out << fmt_hdr1;
	retchk( stream_out );

	*stream_out << fmt_hdr2;
	retchk( stream_out );

	do  {
		snprintf( buf, sizeof(buf), fmt_shape1,
				krui_getFTypeName(),
				krui_getFTypeActFuncName(), krui_getFTypeOutFuncName() );
		*stream_out << buf;
		retchk( stream_out );

		if ( krui_setFirstFTypeSite() )  {
			second = FALSE;
			do  {
				if (second)  {

					*stream_out << fmt_blank;
					retchk( stream_out );
				}

				snprintf( buf, sizeof(buf), " %-s", krui_getFTypeSiteName() );
				*stream_out << buf;
				retchk( stream_out );
				second = TRUE;
			}
			while ( krui_setNextFTypeSite() );
		}

		snprintf( buf, sizeof(buf), "\n" );
		*stream_out << buf;
		retchk( stream_out );
	}
	while (krui_setNextFTypeEntry() );


	*stream_out << fmt_hdr2;
	if (err <= 0)  return( err );

	return( 0 );
}

/*****************************************************************************
  FUNCTION : getTType 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

char  *SnnsCLib::getTType(int st)
{
	//static char   *krio_getTType_ttype[] = { "-", "i", "o", "d", "h", "s" , "si", "so", "sh", "sd"}; 

	switch ( st )  {
	case  INPUT:
		return( krio_getTType_ttype[1] );
	case  OUTPUT:
		return( krio_getTType_ttype[2] );
	case  DUAL:
		return( krio_getTType_ttype[3] );
	case  HIDDEN:
		return( krio_getTType_ttype[4] );
	case  SPECIAL:
		return( krio_getTType_ttype[5] );
	case  SPECIAL_I:      
		return( krio_getTType_ttype[6] ); 
	case  SPECIAL_O:      
		return( krio_getTType_ttype[7] ); 
	case  SPECIAL_H:      
		return( krio_getTType_ttype[8] ); 
	case  SPECIAL_D:      
		return( krio_getTType_ttype[9] ); 
	default:
		return( krio_getTType_ttype[0] );
	}
}


/*****************************************************************************
  FUNCTION : krio_writeDefaultDefinitions 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

krui_err  SnnsCLib::krio_writeDefaultDefinitions(void)
{
	//int   err;
	FlintType  act, bias;
	int   st, subnet_no, layer_no;
	char  *act_func,  *out_func;

	char buf[MAX_LIN_LEN];

	//err = 
	krio_fmtShapeing( DEFAULT_DEF );
	retchk( stream_out );

	snprintf( buf, sizeof(buf), "\n\n%s :\n\n", title[13] );
	*stream_out << buf;
	retchk( stream_out );

	*stream_out << fmt_hdr1;
	retchk( stream_out );

	*stream_out << fmt_hdr2;
	retchk( stream_out );

	krui_getUnitDefaults( &act, &bias, &st, &subnet_no, &layer_no,
			&act_func, &out_func );

	if ((act_func != NULL) && (out_func != NULL))  {
		snprintf( buf, sizeof(buf), fmt_shape1, act, bias, getTType( st ),
				subnet_no, layer_no, act_func, out_func );
		*stream_out << buf;
	}
	else  {
		snprintf( buf, sizeof(buf), fmt_shape1, act, bias, getTType( st ),
				subnet_no, layer_no, " ", " " );
		*stream_out << buf;
	}

	retchk( stream_out );


	*stream_out << fmt_hdr2;
	retchk( stream_out );

	return( 0 );
}


/*****************************************************************************
  FUNCTION : krio_writeUnitDefinitions 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

krui_err  SnnsCLib::krio_writeUnitDefinitions(void)
{
	//static char   *krio_writeUnitDefinitions_blank = "\0";

	struct  PosType   pos;

	char buf[MAX_LIN_LEN];

	char  *u_type,
	*u_name,
	*act_func,
	*act_func_def,
	*out_func_def,
	*out_func;

	bool  no_Ftype, second,
	writeUnitActFuncName, writeUnitOutFuncName;
	int   unit_no,
	u_no,
	st_def, subnet_no, layer_no; //err,

	FlintType  act_def, bias_def;



	if (NoOfUnits <= 0)  return( KRERR_NO_ERROR );

	//err = 
	krio_fmtShapeing( UNIT_DEF );
	retchk( stream_out );

	snprintf( buf, sizeof(buf), "\n\n%s :\n\n", title[10] );
	*stream_out << buf;
	retchk( stream_out );

	*stream_out << fmt_hdr1;
	retchk( stream_out );

	*stream_out << fmt_hdr2;
	retchk( stream_out );

	krui_getUnitDefaults( &act_def, &bias_def, &st_def, &subnet_no, &layer_no,
			&act_func_def, &out_func_def );

	unit_no = krui_getFirstUnit();
	u_no = 1;

	do  {
		krui_getUnitPosition( unit_no, &pos );

//pos.z = 0;
		if ( (u_name = krui_getUnitName( unit_no )) == NULL)
			u_name = krio_writeUnitDefinitions_blank;

		if ( (u_type = krui_getUnitFTypeName( unit_no )) == NULL)
		{  /*  no ftype  */
			no_Ftype = TRUE;

			act_func = krui_getUnitActFuncName( unit_no );
			out_func = krui_getUnitOutFuncName( unit_no );

			writeUnitActFuncName = strcmp( act_func, act_func_def) != 0;
			writeUnitOutFuncName = strcmp( out_func, out_func_def) != 0;

			if (!writeUnitActFuncName && !writeUnitOutFuncName)  {

				snprintf( buf, sizeof(buf), fmt_shape2,
						u_no, krio_writeUnitDefinitions_blank, u_name,
						krui_getUnitActivation( unit_no ),
						krui_getUnitBias( unit_no ),
						getTType( krui_getUnitTType( unit_no ) ),
						pos.x, pos.y, pos.z );

/*------------------------------------------
				*stream_out << fmt_shape2;
				*stream_out << u_no;
				*stream_out << krio_writeUnitDefinitions_blank;
				*stream_out << u_name;
				*stream_out << krui_getUnitActivation( unit_no );
				*stream_out << krui_getUnitBias( unit_no );
				*stream_out << getTType( krui_getUnitTType( unit_no ) );
				*stream_out << pos.x;
				*stream_out << pos.y;
				*stream_out << pos.z;
//------------------------------------------*/

				*stream_out << buf;
			}
			else  {
				if (!writeUnitActFuncName)  act_func = krio_writeUnitDefinitions_blank;
				if (!writeUnitOutFuncName)  out_func = krio_writeUnitDefinitions_blank;


				snprintf( buf, sizeof(buf), fmt_shape1,
						u_no, krio_writeUnitDefinitions_blank, u_name,
						krui_getUnitActivation( unit_no ),
						krui_getUnitBias( unit_no ),
						getTType( krui_getUnitTType( unit_no ) ),
						pos.x, pos.y, pos.z, act_func, out_func );
/*//------------------------------------------
				*stream_out << fmt_shape1;
				*stream_out << u_no;
				*stream_out << krio_writeUnitDefinitions_blank;
				*stream_out << u_name;
				*stream_out << krui_getUnitActivation( unit_no );
				*stream_out << krui_getUnitBias( unit_no );
				*stream_out << getTType( krui_getUnitTType( unit_no ) );
				*stream_out << pos.x;
				*stream_out << pos.y;
				*stream_out << pos.z;
				*stream_out << act_func;
				*stream_out << out_func;
//------------------------------------------
*/
				*stream_out << buf;
			}
		}
		else  {

			snprintf( buf, sizeof(buf), fmt_shape2,
					u_no, u_type, u_name,
					krui_getUnitActivation( unit_no ),
					krui_getUnitBias( unit_no ),
					getTType( krui_getUnitTType( unit_no ) ),
					pos.x, pos.y, pos.z );
			*stream_out << buf;

			no_Ftype = FALSE;
		}


		retchk( stream_out );

		if ( no_Ftype )  {
			if ( krui_setFirstSite() )  {
				second = FALSE;
				do  {
					if (second)  {

						*stream_out << fmt_blank;
						retchk( stream_out );
					}

					snprintf( buf, sizeof(buf), " %-s", krui_getSiteName() );
					*stream_out << buf;
					retchk( stream_out );
					second = TRUE;
				}
				while ( krui_setNextSite() );
			}
		}

		snprintf( buf, sizeof(buf), "\n" );
		*stream_out << buf;
		retchk( stream_out );
		u_no++;
	}
	while ( (unit_no = krui_getNextUnit() ) > 0);


	*stream_out << fmt_hdr2;
	retchk( stream_out );

	return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : krio_writeTimeDelayDefs

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/
krui_err    SnnsCLib::krio_writeTimeDelayDefs(void)
{

	struct Unit  *unit_ptr;

	char buf[MAX_LIN_LEN];

	// err,
	int   unit_no,
	u_no,
	lln,
	lun,
	toff,
	soff,
	conn_type;

	if (NoOfUnits <= 0)
		return( 0 );


	if ((strcmp (krui_getLearnFunc(), "TimeDelayBackprop") == 0) || 
			(strcmp (krui_getLearnFunc(), "TDBP_McClelland") == 0)) {

		snprintf( buf, sizeof(buf), "\n\n%s :\n\n", title[18] ); 
		*stream_out << buf;
		retchk( stream_out );
		snprintf( buf, sizeof(buf), "%s\n", " no. | LLN | LUN | Toff | Soff | Ctype");
		*stream_out << buf;
		//retchk (err);
		snprintf( buf, sizeof(buf), "-----|-----|-----|------|------|-------\n");
		*stream_out << buf;
		//retchk (err);

		unit_no = krui_getFirstUnit ();
		u_no = 1;

		do {

			unit_ptr = kr_getUnitPtr (unit_no);

			lln = unit_ptr->lln;
			lun = unit_ptr->lun;
			toff = unit_ptr->TD.target_offset;
			soff = unit_ptr->TD.source_offset;
			conn_type = unit_ptr->TD.td_connect_typ;

			snprintf( buf, sizeof(buf), "%4d |%4d |%4d |%5d |%5d |%6d\n", 
					u_no, lln, lun, toff, soff, conn_type);
			*stream_out << buf;
			//retchk (err);

			unit_no = krui_getNextUnit ();
			u_no++;

		} while (unit_no != 0);

		snprintf( buf, sizeof(buf), "-----|-----|-----|------|------|-------\n");
		*stream_out << buf;
		//retchk (err);

	}

	return (KRERR_NO_ERROR);
}




/*****************************************************************************
  FUNCTION : krio_writeSourcesAndWeights  

  PURPOSE  : write link weights
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/
krui_err  SnnsCLib::krio_writeSourcesAndWeights(void)
{
	bool  second;
	int   i, //err,
	source_unit,
	tacoma_mode;

	char buf[MAX_LIN_LEN];

	FlintType     weight;
	float val_a,val_b,val_c;


	tacoma_mode = (!strcmp(krui_getUnitActFuncName(GET_UNIT_NO(unitPtr)),"Act_TACOMA"));
	i = 0;
	source_unit = krui_getFirstPredUnitAndData( &weight, &val_a, &val_b, &val_c );
	second = FALSE;

	do  {
		if (second)  {
			snprintf( buf, sizeof(buf), "," );
			*stream_out << buf;
			retchk( stream_out );
		}

		if (++i > max_connects_per_line)  {
			i = 1;

			*stream_out << fmt_blank;
			retchk( stream_out );
		}
		if ((tacoma_mode)&&((val_a != 0.0f)&&(val_b != 0.0f))) {
			snprintf( buf, sizeof(buf), fmt_shape4, source_unit, weight, val_b, val_a);
			*stream_out << buf;
		}   else {
			snprintf( buf, sizeof(buf), fmt_shape3, source_unit, weight );
			*stream_out << buf;
		}
		retchk( stream_out );

		second = TRUE;
	}
	while ( (source_unit = krui_getNextPredUnitAndData( &weight, &val_a, &val_b, &val_c )) > 0);

	snprintf( buf, sizeof(buf), "\n" );
	*stream_out << buf;
	retchk( stream_out );

	return( KRERR_NO_ERROR );
}

/*****************************************************************************
  FUNCTION : krio_writeConnectionDefs 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

krui_err  SnnsCLib::krio_writeConnectionDefs(void)
{
	bool  second;
	int   target_unit, unit_no;
	//        err;

	char buf[MAX_LIN_LEN];

	FlintType  weight;


	if (kr_io_NoOfLinks == 0)  return( 0 );

	//err = 
	krio_fmtShapeing( CONNECT_DEF );
	retchk( stream_out );

	snprintf( buf, sizeof(buf), "\n\n%s :\n\n", title[11] );
	*stream_out << buf;
	retchk( stream_out );

	*stream_out << fmt_hdr1;
	retchk( stream_out );

	*stream_out << fmt_hdr2;
	retchk( stream_out );

	unit_no = krui_getFirstUnit();
	target_unit = 1;

	do  {
		switch (krui_getUnitInputType( unit_no ))  {
		case  DIRECT_LINKS:
			snprintf( buf, sizeof(buf), fmt_shape1, target_unit, " ");
			*stream_out << buf;
			retchk( stream_out );
			//err = 
			krio_writeSourcesAndWeights();
			retchk( stream_out );

			break;

		case  SITES:
			krui_setFirstSite();
			second = FALSE;

			do  {
				if (krui_getFirstPredUnit( &weight ) > 0)  {
					if (second)  {
						snprintf( buf, sizeof(buf), fmt_shape2, krui_getSiteName() );
						*stream_out << buf;
						retchk( stream_out );
					}
					else  {
						snprintf( buf, sizeof(buf), fmt_shape1, target_unit, 
								krui_getSiteName() );
						*stream_out << buf;
						retchk( stream_out );
					}

					//err = 
					krio_writeSourcesAndWeights();
					retchk( stream_out );

					second = TRUE;
				}
			}
			while ( krui_setNextSite() );

			break;
		}

		target_unit++;
	}
	while ( (unit_no = krui_getNextUnit()) > 0 );


	*stream_out << fmt_hdr2;
	retchk( stream_out );
	return( KRERR_NO_ERROR );
}


/*****************************************************************************
  FUNCTION : krio_writeSubnetDefs 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/
krui_err  SnnsCLib::krio_writeSubnetDefs(void)
{

	char buf[MAX_LIN_LEN];

	struct Unit   *unit_ptr,
	*unit_ptr2;
	int	i,  k, elem_no, dummy2, def_subnet_no; //err,
	short  subnet_no;
	FlintType  dummy1;
	char	*dummy3;


	if (!is_subnet_info)  return( 0 );

	//err = 
	krio_fmtShapeing( SUBNET_DEF );
	retchk( stream_out );

	snprintf( buf, sizeof(buf), "\n\n%s :\n\n", title[12] );
	*stream_out << buf;
	retchk( stream_out );

	*stream_out << fmt_hdr1;
	retchk( stream_out );

	*stream_out << fmt_hdr2;
	retchk( stream_out );

	/*  get default subnet number  */
	krui_getUnitDefaults( &dummy1, &dummy1, &dummy2, &def_subnet_no, &dummy2,
			&dummy3, &dummy3 );

	/*  clear refresh flags  */
	FOR_ALL_UNITS( unit_ptr )
	unit_ptr->flags &= ~UFLAG_REFRESH;

	for (i = MinUnitNo, unit_ptr = unit_array + MinUnitNo; 
			i <= MaxUnitNo; 
			i++, unit_ptr++)
		if (!UNIT_REFRESHED( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
		{				/*  unit is in use and 'fresh' */
			subnet_no = unit_ptr->subnet_no;
			if (subnet_no == def_subnet_no)
				continue;

			snprintf( buf, sizeof(buf), fmt_shape1, subnet_no, i);
			*stream_out << buf;
			retchk( stream_out );
			elem_no = 0;

			for (k = i + 1, unit_ptr2 = unit_ptr + 1;
					k <= MaxUnitNo;
					k++, unit_ptr2++ )
			{
				if (!UNIT_REFRESHED( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
				{			/*  unit is in use and 'fresh' */
					if (unit_ptr2->subnet_no == subnet_no)  {
						unit_ptr2->flags |= UFLAG_REFRESH;

						snprintf( buf, sizeof(buf), "," );
						*stream_out << buf;
						retchk( stream_out );

						if ( (++elem_no % max_subnets_per_line) == 0)  {

							*stream_out << fmt_blank;
							retchk( stream_out );
						}

						snprintf( buf, sizeof(buf), fmt_shape2,  k);
						*stream_out << buf;
						retchk( stream_out );
					}
				}
			}
		}


	*stream_out << fmt_hdr2;
	retchk( stream_out );

	*stream_out << "\n";
	retchk( stream_out );

	return( KRERR_NO_ERROR );
}

/*****************************************************************************
  FUNCTION : krio_writeLayerDefs 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

krui_err  SnnsCLib::krio_writeLayerDefs(void)
{
	struct Unit   *unit_ptr,
	*unit_ptr2;

	int	i,  k, elem_no, dummy2, def_layer_no; //err,
	short  layer_no;
	FlintType  dummy1;
	char	    *dummy3;

	char buf[MAX_LIN_LEN];

	if (!is_layer_info)  return( KRERR_NO_ERROR );

	//err = 
	krio_fmtShapeing( LAYER_DEF );
	retchk( stream_out );

	snprintf( buf, sizeof(buf), "\n\n%s :\n\n", title[15] );
	*stream_out << buf;
	retchk( stream_out );

	*stream_out << fmt_hdr1;
	retchk( stream_out );

	*stream_out << fmt_hdr2;
	retchk( stream_out );

	/*  get default layer number	*/
	krui_getUnitDefaults( &dummy1, &dummy1, &dummy2, &dummy2, &def_layer_no,
			&dummy3, &dummy3 );

	/*  clear refresh flags  */
	FOR_ALL_UNITS( unit_ptr )
	unit_ptr->flags &= ~UFLAG_REFRESH;

	for (i = MinUnitNo, unit_ptr = unit_array + MinUnitNo; 
			i <= MaxUnitNo; 
			i++, unit_ptr++)
		if (!UNIT_REFRESHED( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
		{				/*  unit is in use and 'fresh' */
			layer_no = (int) unit_ptr->layer_no;
			if (layer_no == def_layer_no)
				continue;

			snprintf( buf, sizeof(buf), fmt_shape1, layer_no, i);
			*stream_out << buf;
			retchk( stream_out );
			elem_no = 0;

			for (k = i + 1, unit_ptr2 = unit_ptr + 1;
					k <= MaxUnitNo;
					k++, unit_ptr2++ )  {
				if (!UNIT_REFRESHED( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
				{			/*  unit is in use and 'fresh' */
					if ((int) unit_ptr2->layer_no == layer_no)  {
						unit_ptr2->flags |= UFLAG_REFRESH;

						snprintf( buf, sizeof(buf), "," );
						*stream_out << buf;
						retchk( stream_out );

						if ( (++elem_no % max_layers_per_line) == 0)  {

							*stream_out << fmt_blank;
							retchk( stream_out );
						}

						snprintf( buf, sizeof(buf), fmt_shape2, k);
						*stream_out << buf;
						retchk( stream_out );
					}
				}
			}
		}


	*stream_out << fmt_hdr2;
	retchk( stream_out );

	*stream_out << "\n";
	retchk( stream_out );

	return( KRERR_NO_ERROR );
}




krui_err  SnnsCLib::writeXYTransTable(void)
{
	//  int err;
	int z_index;

	char buf[MAX_LIN_LEN];

	if (transTableSize > 0)  {
		snprintf( buf, sizeof(buf), "\n\n%s :\n\n", title[17]);
		*stream_out << buf;
		retchk( stream_out );
		snprintf( buf, sizeof(buf), " delta x | delta y |    z    \n");
		*stream_out << buf;
		retchk( stream_out );
		snprintf( buf, sizeof(buf), "---------|---------|---------\n");
		*stream_out << buf;
		retchk( stream_out );
		for (z_index = 0; z_index < transTableSize; z_index++)  {
			snprintf( buf, sizeof(buf), "%8d |", transTable[z_index].x);
			*stream_out << buf;
			retchk( stream_out );
			snprintf( buf, sizeof(buf), "%8d |", transTable[z_index].y);
			*stream_out << buf;
			retchk( stream_out );
			snprintf( buf, sizeof(buf), "%8d \n", transTable[z_index].z);
			*stream_out << buf;
			retchk( stream_out );
		}

		snprintf( buf, sizeof(buf), "---------|---------|---------\n");
		*stream_out << buf;
		retchk( stream_out );
		snprintf( buf, sizeof(buf), "\n" );
		*stream_out << buf;
		retchk( stream_out );
	}

	return( KRERR_NO_ERROR );
}

/*****************************************************************************
  FUNCTION : krio_saveNet  

  PURPOSE  : write complete network to disk
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/
krui_err  SnnsCLib::krio_saveNet(char *filename, char *netname)
{

	stream_out = new std::ofstream( filename, std::ios::out );

	int  err = krio_serializeNetInternal(netname);

	static_cast<std::ofstream*>(stream_out)->close();

        delete stream_out;

	return( err );
}


/*****************************************************************************
  FUNCTION : krio_serializeNet  

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/
krui_err  SnnsCLib::krio_serializeNet(std::stringstream *buf, char *netname)
{

	stream_out = buf;

	int  err = krio_serializeNetInternal(netname);

	return( err );
}


/*****************************************************************************
  FUNCTION : krio_serializeNetInternal  

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/
krui_err  SnnsCLib::krio_serializeNetInternal(char *netname)
{
	int  err;

	if (NoOfUnits == 0)  return( KRERR_NO_UNITS );  /*  no units defined	*/

	//stream_out = new std::ofstream( filename, std::ios::out );
	//stream_out = new ofstream( fullFileName.c_str(), ios::out );
	//*stream_out->open(filename);

	if (!stream_out->good())  {
		err = KRERR_IO;  //  file open error
		goto  ende;
	}

	/*  if ((file_out = fopen( filename, "w" )) == NULL)  {
    err = KRERR_IO;  //  file open error
    goto  ende;
  }*/

	kr_forceUnitGC();  /*  do unit garbage-collection  */

	/*  write file header  */
	err = krio_writeHeader( krio_getIOVersion(), netname );

	/*  calc. maximum string length of all identifiers  */
	krio_stringLimits();
	if (err < 0)	goto ende;

	err = krio_writeSiteDefinitions();
	if (err < 0)	goto ende;

	err = krio_writeTypeDefinitions();
	if (err < 0)	goto ende;

	err = krio_writeDefaultDefinitions();
	if (err < 0)	goto ende;

	err = krio_writeUnitDefinitions();
	if (err < 0)	goto ende;
	/*
  err = krio_writeTopologicDef();
  if (err < 0)	goto ende;
	 */
	err = krio_writeConnectionDefs();
	if (err < 0)	goto ende;

	err = krio_writeSubnetDefs();
	if (err < 0)	goto ende;
	err = krio_writeLayerDefs();
	if (err < 0)	goto ende;
	err = krio_writeTimeDelayDefs ();
	if (err < 0)	goto ende;

	err = writeXYTransTable();
	if (err < 0)	goto ende;

	//err = fclose( file_out );
	//*stream_out->close();
	//static_cast<std::ofstream*>(stream_out)->close();

	ende:

	KernelErrorCode = err;

	return( KernelErrorCode );
}



/*#################################################

GROUP: Kernel I/O Input Functions

#################################################*/
/*****************************************************************************
  FUNCTION : skipComments 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

bool  SnnsCLib::skipComments(void)
{
	 int   c;

	while (TRUE)  {
		do  {
			c = getc( file_in );
			if (c == '\n')  lineno++;
		}
		while ( isspace( c ) );

		if (c != '#')  break;

		do
			c = getc( file_in );
		while (c != EOF && c != '\n');

		if (c == '\n')  lineno++;
	}

	if (c == EOF)  return( FALSE );

	ungetc( c, file_in );
	return( TRUE );
}
/*****************************************************************************
  FUNCTION : skipSpace 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/


bool  SnnsCLib::skipSpace(void)
{
	 int  c;

	do  {
		c = getc( file_in );
		if (c == '\n')  lineno++;
	}
	while ( isspace( c ) );

	if (c == EOF)  {
		KernelErrorCode = KRERR_EOF;
		return( FALSE );
	}

	ungetc( c, file_in );
	return( TRUE );
}

/*****************************************************************************
  FUNCTION : comma  

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

bool  SnnsCLib::comma(void)
{
	 int  c;

	do  {
		c = getc( file_in );
		if (c == '\n')  lineno++;
	}
	while ( isspace( c ) );

	if (c == EOF)  {  KernelErrorCode = KRERR_EOF;  return( FALSE ); }
	if (c != ',')  {
		ungetc( c, file_in );
		return( FALSE );
	}

	do  {
		c = getc( file_in );
		if (c == '\n')  lineno++;
	}
	while ( isspace( c ) );

	ungetc( c, file_in );
	return( TRUE );
}


/*****************************************************************************
  FUNCTION :  get_nl

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

bool  SnnsCLib::get_nl(void)
{
	 int  c;

	do  {
		c = getc( file_in );
		if (c == '\n')  {
			lineno++;
			return( TRUE );
		}
	}
	while ( isspace( c ) );

	if (c == EOF)  KernelErrorCode = KRERR_EOF;
	else  ungetc( c, file_in );

	return( FALSE );
}

/*****************************************************************************
  FUNCTION : get_pipe 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

bool  SnnsCLib::get_pipe(void)
{
	 int  c;

	do  {
		c = getc( file_in );
		if (c == '\n')  lineno++;
	}
	while ( isspace( c ) );

	if (c == '|')  return( TRUE );

	if (c == EOF)  KernelErrorCode = KRERR_EOF;
	else  ungetc( c, file_in );

	return( FALSE );
}
/*****************************************************************************
  FUNCTION : skip_pipe  

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

bool  SnnsCLib::skip_pipe(void)
{
	 int  c;

	do  {
		c = getc( file_in );
		if (c == '\n')  lineno++;
	}
	while ( isspace( c ) );

	if (c == '|')  return( TRUE );

	if (c == EOF)  KernelErrorCode = KRERR_EOF;
	else  {
		ungetc( c, file_in );
		KernelErrorCode = KRERR_FILE_SYNTAX;
	}

	return( FALSE );
}

/*****************************************************************************
  FUNCTION : get_alpha   

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/


bool  SnnsCLib::get_alpha(void)
{
	 int  c;

	do  {
		c = getc( file_in );
		if (c == '\n')  lineno++;
	}
	while ( isspace( c ) );

	if (c == EOF)  KernelErrorCode = KRERR_EOF;
	else  {
		ungetc( c, file_in );
		if (isalpha( c ))  return( TRUE );
	}

	return( FALSE );
}
/*****************************************************************************
  FUNCTION : getSymbol 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

bool  SnnsCLib::getSymbol(char *symbol)
{
	 int  c;
	 char  *s_ptr;

	do  {
		c = getc( file_in );
		if (c == '\n')  lineno++;
	}
	while ( isspace( c ) );

	if (c == EOF)  {
		KernelErrorCode = KRERR_EOF;
		return( FALSE );
	}
	else  {
		s_ptr = symbol;
		while (isgraph( c ))  {
			if ((c == ',') || (c == '|'))  break;
			*s_ptr++ = c;
			c = getc( file_in );
		}

		ungetc( c, file_in );
		*s_ptr = EOS;
		if (s_ptr == symbol)  {
			KernelErrorCode = KRERR_FILE_SYNTAX;
			return( FALSE );
		}
		return( TRUE );
	}
}
/*****************************************************************************
  FUNCTION : getSection 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

char  *SnnsCLib::getSection(char *line, int *title_no)
{
	int   i;
	char  *s1_ptr;
	const char *s2_ptr;


	if ( !skipComments() )
	{  /* EOF  */
		*title_no = -2;
		return( NULL );
	}

	if (fgets( line, LIN_MAX, file_in ) == NULL)
	{  /*  I/O error  */
		*title_no = -3;
		return( NULL );
	}

	for (i = 0; i < NoOfTitles; i++)  {
		s1_ptr = line;
		s2_ptr = title[ i ];
		while (*s2_ptr != EOS)
		{  /* compare strings, don't examine white space  */
			if (*s1_ptr == ' ') s1_ptr++;
			if (*s2_ptr == ' ') s2_ptr++;
			if (*s1_ptr++ != *s2_ptr++)  break;

			if (*s2_ptr == EOS)
			{   /*	strings are equal   */
				*title_no = i;
				return( s1_ptr );
			}
		}
	}


	*title_no = -1;  /*  title not found  */
	return( NULL );
}

/*****************************************************************************
  FUNCTION : matchHead

  PURPOSE  : read /dev/null
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

bool  SnnsCLib::matchHead(int N)
{
	 int  c;
	int   i;


	c = getc( file_in );
	if (c != '-')  {
		ungetc( c, file_in );
		return( FALSE );
	}

	for (i = 0; i < N; i++)  {
		c = getc( file_in );
		if (c != '-')
			return( FALSE );

		do
			c = getc( file_in );
		while ( c == '-' );

		if (c != '|')  {  KernelErrorCode = KRERR_FILE_SYNTAX;  return( FALSE );  }
	}

	c = getc( file_in );
	if (c != '-')  {  KernelErrorCode = KRERR_FILE_SYNTAX;  return( FALSE );  }

	do
		c = getc( file_in );
	while ( c == '-' );

	if (c == EOF)  {  KernelErrorCode = KRERR_EOF;  return( FALSE ); }
	ungetc( c, file_in );

	if ( !get_nl() )  return( FALSE );

	return( TRUE );
}

/*****************************************************************************
  FUNCTION : matchHead2 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

bool  SnnsCLib::matchHead2(int N)
{
	if (!skipSpace())  return( FALSE );

	return( matchHead(N) );
}


/*****************************************************************************
  FUNCTION : my_strstr 

  PURPOSE  : Find the first occurrence of find in s.
             (slow version)
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/
char  *SnnsCLib::my_strstr(char *s, char *find)
{
	 char c, sc;
	 size_t len;

	if ((c = *find++) != 0)  {
		len = strlen(find);
		do  {
			do  {
				if ((sc = *s++) == 0)	return (NULL);
			}
			while (sc != c);
		}
		while (strncmp(s, find, len) != 0);
		s--;
	}

	return ((char *) s);
}


/*****************************************************************************
  FUNCTION : krio_readHeader 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

void  SnnsCLib::krio_readHeader(char *netfile_version, char *net_name, 
		char *learn_func, char *update_func,
		char *pruning_func, char *ff_learn_func,
		int *no_of_units, int *no_of_connect, 
		int *no_of_unitTypes, int *no_of_siteTypes)
{
	char  *cursor;
	int   ret_code,  i, no_of_scan_params,
	title_no;


	ret_code = 1;

	/*  get file version  */
	cursor = getSection( fmt_shape1, &title_no );
	if ((cursor == NULL) || (title_no != 0))  {
		/*  "SNNS network definition file" not found  */
		KernelErrorCode = KRERR_FILE_FORMAT;
		return;
	}
	if (sscanf( cursor, "%s", netfile_version ) != 1)  {
		/*  version string not found */
		KernelErrorCode = KRERR_FILE_FORMAT;
		return;
	}

	/*  determine version number of the network file  */
	if (my_strstr( netfile_version, const_cast<char*>(KERNEL3D_NETFILE_VERSION) ) != NULL)
		NetfileVersion = 1;
	else
		NetfileVersion = 0;

	if (my_strstr( netfile_version, const_cast<char*>(NETFILE_VERSION) ) != NULL ||
			my_strstr( netfile_version, const_cast<char*>(NETFILE_VERSION2) ) != NULL)
		/*  current netfile version  */
		no_of_scan_params = 9;
	else
		no_of_scan_params = 8;

	for (i = 0; i < no_of_scan_params; i++)  {
		if ( (cursor = getSection( fmt_shape1, &title_no )) == NULL)  return;

		switch (title_no)  {
		case  -3:  /*  Physical I/O error  */
			KernelErrorCode = KRERR_IO;
			return;
		case  -2:  /*  unexpected EOF  */ 
			KernelErrorCode = KRERR_EOF;
			return;
		case  -1:  /*  not found  */ 
			KernelErrorCode = KRERR_FILE_FORMAT;  /*  Incompatible file format  */
			return;
		case  1:  /*  "generated at"  */
			break;
		case  2:  /*  network name"  */
			ret_code = sscanf( cursor, " :%s", net_name );
			if ((ret_code != 1) && (ret_code != 0))  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
			if (ret_code == 0) {
				*net_name = EOS;
				ret_code = 1;
			}
			break;
		case  3:  /*  "no. of units"  */
			ret_code = sscanf( cursor, " :%d", no_of_units );
			break; 
		case  4:  /*  "no. of connections"  */
			ret_code = sscanf( cursor, " :%d", no_of_connect );
			break;
		case  5:  /*  "no. of unit types"  */
			ret_code = sscanf( cursor, " :%d", no_of_unitTypes );
			break;
		case  6:  /*  "no. of site types"  */
			ret_code = sscanf( cursor, " :%d", no_of_siteTypes );
			break;
		case  7:  /*  "learning function"  */
			ret_code = sscanf( cursor, " :%s\n", learn_func );
			if ((ret_code != 1) && (ret_code != 0))  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
			if (ret_code == 0)  {
				*learn_func = '\0';
				ret_code = 1;
			}
			if (strcmp (learn_func, "PruningFeedForward") == 0)
				/* pruning functions need two more lines */
				no_of_scan_params += 2;
			break;
		case 14:  /*  "source files"  */
			break;
		case  16:  /*  "update function"  */
			ret_code = sscanf( cursor, " :%s\n", update_func );
			if ((ret_code != 1) && (ret_code != 0))  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}

			if (ret_code == 0)  {
				*update_func = '\0';
				ret_code = 1;
			}
			break;
		case 19:  /* pruning function */
			ret_code = sscanf( cursor, " :%s\n", pruning_func );
			if ((ret_code != 1) && (ret_code != 0))  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}

			if (ret_code == 0)  {
				*pruning_func = '\0';
				ret_code = 1;
			}
			break;
		case 20:  /* feed forward learning function */
			ret_code = sscanf( cursor, " :%s\n", ff_learn_func );
			if ((ret_code != 1) && (ret_code != 0))  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}

			if (ret_code == 0)  {
				*ff_learn_func = '\0';
				ret_code = 1;
			}
			break;
		default:
			KernelErrorCode = KRERR_FILE_FORMAT;  /*  Incompatible file format  */
			return;
		}

		switch (ret_code)  {
		case 1:    /*  one argument matched  */
			break;
		case EOF:  /*  unexpected EOF  */
			KernelErrorCode = KRERR_EOF;
			return;
		default:   /*  Syntax error at line  */
			KernelErrorCode = KRERR_FILE_SYNTAX;
			return;
		}

		lineno++;
	}

}

/*****************************************************************************
  FUNCTION : str_to_Ttype 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

int  SnnsCLib::str_to_Ttype(char *str)
{
	if (strcmp (str, "i" ) == 0) return (INPUT    ) ;
	if (strcmp (str, "o" ) == 0) return (OUTPUT   ) ;
	if (strcmp (str, "h" ) == 0) return (HIDDEN   ) ;
	if (strcmp (str, "d" ) == 0) return (DUAL     ) ;
	if (strcmp (str, "s" ) == 0) return (SPECIAL  ) ;
	if (strcmp (str, "si") == 0) return (SPECIAL_I) ;
	if (strcmp (str, "so") == 0) return (SPECIAL_O) ;
	if (strcmp (str, "sh") == 0) return (SPECIAL_H) ;
	if (strcmp (str, "sd") == 0) return (SPECIAL_D) ;
	return( 0 );
}

/*****************************************************************************
  FUNCTION : krio_readSiteDefinitions 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/


void  SnnsCLib::krio_readSiteDefinitions(void)
{
	if ( !skipComments() )  return;
//	if ((fscanf( file_in, headers[0] ) != 0) ||
	if ((fscanf( file_in, " site name | site function" ) != 0) ||
			!matchHead2( 1 ) )  {
		KernelErrorCode = KRERR_FILE_SYNTAX;
		return;
	}

	NoOfSiteTypes = 0;
	while (TRUE)  {
		if (matchHead2( 1 )) return;
		if (!skipComments())  return;

		if (!getSymbol( fmt_shape1 ))  return;

		/*  skip "|" character  */
		if (!skip_pipe())  return;

		if (!getSymbol( fmt_shape2 ))  return;

		if (krui_createSiteTableEntry( fmt_shape1, fmt_shape2 ) != KRERR_NO_ERROR)
			return;

		NoOfSiteTypes++;
	}
}

/*****************************************************************************
  FUNCTION : krio_readTypeDefinitions 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

void  SnnsCLib::krio_readTypeDefinitions(void)
{
	int  no_of_sites;
	SymbolType  *site_names = NULL;
	char  * *site_name_ptrs = NULL;


	NoOfUnitTypes = 0;

	if ( !skipComments() )  return;
	//if ((fscanf( file_in, headers[1] ) != 0) ||
        if ((fscanf( file_in, " name | act func | out func | sites" ) != 0) ||
			!matchHead2( 3 ) )  {
		KernelErrorCode = KRERR_FILE_SYNTAX;
		return;
	}

	if (NoOfSiteTypes > 0)  {
		site_names = (SymbolType *) 
			calloc( (unsigned int) NoOfSiteTypes, sizeof (SymbolType) );
		site_name_ptrs = (char * *) 
			calloc( (unsigned int) NoOfSiteTypes, sizeof (site_name_ptrs) );
		if (site_names == NULL || site_name_ptrs == NULL)  {
			KernelErrorCode = KRERR_INSUFFICIENT_MEM;
			return;
		}
	}

	while (TRUE)  {
		if ( matchHead2( 3 ) )  break;
		if ( !skipComments() )  break;

		/*  read unit type name, activation and output function  */
		if (fscanf( file_in, "%s | %s | %s",
				fmt_shape1, fmt_shape2,  fmt_shape3 ) != 3)  {
			KernelErrorCode = KRERR_FILE_SYNTAX;
			return;
		}

		/*  skip "|" character  */
		if (!skip_pipe())  return;

		NoOfUnitTypes++;

		if (get_nl())  {
			/*  no sites  */
			if (krui_createFTypeEntry( fmt_shape1, fmt_shape2,  fmt_shape3,
					0, site_name_ptrs ) != KRERR_NO_ERROR)
				return;
			continue;
		}

		no_of_sites = 0;
		while (TRUE)  {
			if (!getSymbol( site_names[ no_of_sites ] ))  return;

			site_name_ptrs[ no_of_sites ] = site_names[ no_of_sites ];
			no_of_sites++;
			if ( !comma() )  break;
			if (no_of_sites > NoOfSiteTypes)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
		}

		if (krui_createFTypeEntry( fmt_shape1, fmt_shape2,  fmt_shape3,
				no_of_sites, site_name_ptrs ) != KRERR_NO_ERROR)
			return;
	}

	if (site_names != NULL)  {
		free( (char *) site_names );
		free( (char *) site_name_ptrs );
	}
}


/*****************************************************************************
  FUNCTION :  krio_readDefaultDefinitions

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

void  SnnsCLib::krio_readDefaultDefinitions(void)
{
	FlintType  act, bias;
	int  st, subnet_no, layer_no;


	if ( !skipComments() )  return;
	//if ((fscanf( file_in, headers[2] ) != 0) ||
        if ((fscanf( file_in, " act | bias | st | subnet | layer | act func | out func" ) != 0) ||
			!matchHead2( 6 ) )  {
		KernelErrorCode = KRERR_FILE_SYNTAX;
		return;
	}

	if (fscanf( file_in, "%f | %f | %s | %d | %d | %s | %s",
			&act, &bias, fmt_shape1, &subnet_no, &layer_no,
			fmt_shape2, fmt_shape3 ) != 7)  {
		KernelErrorCode = KRERR_FILE_SYNTAX;
		return;
	}

	if ( (st = str_to_Ttype( fmt_shape1 )) == 0)  {
		KernelErrorCode = KRERR_TTYPE;
		return;
	}

	if (krui_setUnitDefaults( act, bias, st, subnet_no, layer_no,
			fmt_shape2, fmt_shape3 )  != KRERR_NO_ERROR)
		return;

	if ( !matchHead2( 6 ) )  KernelErrorCode = KRERR_FILE_SYNTAX;
}

/*****************************************************************************
  FUNCTION : krio_readUnitDefinitions 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

void  SnnsCLib::krio_readUnitDefinitions(void)
{
	struct PosType  pos;
	FlintType  fp;
	int   unit_no, def_unit_no, st, x, y, z;
	bool  is_ftype_unit;


	if ( !skipComments() )  return;
//	if ((fscanf( file_in, headers[3] ) != 0) ||
        if ((fscanf( file_in, " no. | typeName | unitName | act | bias | st | position | act func | out func | sites" ) != 0) ||
			!matchHead2( 9 ) )  {
		KernelErrorCode = KRERR_FILE_SYNTAX;
		return;
	}

	def_unit_no = 0;
	while (TRUE)  {
		if ( matchHead2( 9 ) )  return;
		if ( !skipComments() )  return;

		/*  read unit no.  */
		if (fscanf( file_in, "%d", &unit_no ) != 1)  {
			KernelErrorCode = KRERR_FILE_SYNTAX;
			return;
		}
		if (unit_no != ++def_unit_no)  {
			KernelErrorCode = KRERR_UNIT_NO;
			return;
		}

		/*  skip "|" character  */
		if (!skip_pipe())  return;

		/*  read unit type name, if exist  */
		if (!get_pipe())  {
			if (KernelErrorCode != KRERR_NO_ERROR)  return;
			/*  read unit type name  */
			if (fscanf( file_in, "%s", fmt_shape1 ) != 1)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}

			krui_createFTypeUnit( fmt_shape1 );
			if (KernelErrorCode != KRERR_NO_ERROR)  return;
			/*  skip next "|" character  */
			if (!skip_pipe())  return;

			is_ftype_unit = TRUE;
		}
		else  {
			if (krui_createDefaultUnit() != unit_no)  {
				KernelErrorCode = KRERR_MALLOC1;
				return;
			}
			is_ftype_unit = FALSE;
		}

		/*  read unit name, if exist  */
		if (!get_pipe())  {
			if (KernelErrorCode != KRERR_NO_ERROR)  return;
			/*  read unit name  */
			if (fscanf( file_in, "%s", fmt_shape1 ) != 1)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}

			if (krui_setUnitName(def_unit_no, fmt_shape1) != KRERR_NO_ERROR) return;
			/*  skip next "|" character  */
			if (!skip_pipe())  return;
		}

		/*  read unit's activation value, if exist  */
		if (!get_pipe())  {
			if (KernelErrorCode != KRERR_NO_ERROR)  return;
			/*  read unit activation value  */
			if (fscanf( file_in, "%f", &fp ) != 1)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
			krui_setUnitInitialActivation( def_unit_no, fp );
			krui_setUnitActivation( def_unit_no, fp );
			/*  skip next "|" character  */
			if (!skip_pipe())  return;
		}

		/*  read unit's bias value, if exist  */
		if (!get_pipe())  {
			if (KernelErrorCode != KRERR_NO_ERROR)  return;
			/*  read unit bias value  */
			if (fscanf( file_in, "%f", &fp ) != 1)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
			krui_setUnitBias( def_unit_no, fp );
			/*  skip next "|" character  */
			if (!skip_pipe())  return;
		}

		/*  read unit's topologic type, if exist  */
		if (!get_pipe())  {
			if (KernelErrorCode != KRERR_NO_ERROR)  return;
			/*  read unit's topologic type  */
			if (fscanf( file_in, "%s", fmt_shape1 ) != 1)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
			if ( (st = str_to_Ttype( fmt_shape1 )) == 0)  {
				KernelErrorCode = KRERR_TTYPE;
				return;
			}

			if (krui_setUnitTType( def_unit_no, st ) != KRERR_NO_ERROR)  return;
			/*  skip next "|" character  */
			if (!skip_pipe())  return;
		}

		/*  read unit position  */
		switch (NetfileVersion)  {
		case 0:
			if (fscanf( file_in, "%d , %d", &x, &y ) != 2)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
			pos.x = x;
			pos.y = y;

			pos.z = 0;

			krui_setUnitPosition( def_unit_no, &pos );

			break;

		case 1:
			if (fscanf( file_in, "%d , %d, %d", &x, &y, &z ) != 3)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
			pos.x = x;
			pos.y = y;

			pos.z = z;

			krui_setUnitPosition( def_unit_no, &pos );

			break;
		}

		/*  skip "|" character  */
		if (!skip_pipe())  return;

		if (is_ftype_unit)  {
			/*  skip 2 "|" characters  */
			if (!skip_pipe())  return;
			if (!skip_pipe())  return;
		}
		else  {
			/*  read unit's activation function, if exist  */
			if (!get_pipe())  {
				if (KernelErrorCode != KRERR_NO_ERROR)  return;
				/*  read unit's activation function  */
				if (fscanf( file_in, "%s", fmt_shape1 ) != 1)  {
					KernelErrorCode = KRERR_FILE_SYNTAX;
					return;
				}
				if (krui_setUnitActFunc( def_unit_no, fmt_shape1 ) != KRERR_NO_ERROR)
					return;
				/*  skip next "|" character  */
				if (!skip_pipe())  return;
			}

			/*  read unit's output function, if exist  */
			if (!get_pipe())  {
				if (KernelErrorCode != KRERR_NO_ERROR)  return;
				/*  read unit's output function  */
				if (fscanf( file_in, "%s", fmt_shape1 ) != 1)  {
					KernelErrorCode = KRERR_FILE_SYNTAX;
					return;
				}
				if (krui_setUnitOutFunc( def_unit_no, fmt_shape1 ) != KRERR_NO_ERROR)
					return;
				/*  skip next "|" character  */
				if (!skip_pipe())  return;
			}
		}

		if (get_alpha())  {
			/*  a site name was detected  */
			if (krui_setCurrentUnit( def_unit_no ) != KRERR_NO_ERROR)  return;

			/*  get site names  */
			while (TRUE)  {
				if (!getSymbol( fmt_shape1 ))  {
					KernelErrorCode = KRERR_FILE_SYNTAX;
					return;
				}
				if (krui_addSite( fmt_shape1 ) != KRERR_NO_ERROR)  return;

				if ( !comma() )  break;
			}
		}
	}
}
/*****************************************************************************
  FUNCTION : krio_readConnectionDefs 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

void  SnnsCLib::krio_readConnectionDefs(void)
{
	FlintType  weight;
	int   unit_no, src_unit_no;
	bool  new_unit, unit_has_sites;
	float tacoma_xi,tacoma_ri;
	int fscanRet;
	//struct Link* LinkDummy;


	if ( !skipComments() )  return;
	//if ((fscanf( file_in, headers[4] ) != 0) ||
        if ((fscanf( file_in, " target | site | source:weight" ) != 0) ||
			(!matchHead2( 2 )) )  {
		KernelErrorCode = KRERR_FILE_SYNTAX;
		return;
	}

	new_unit = TRUE;
	unit_has_sites = FALSE;

	while (TRUE)  {
		if ( matchHead2( 2 ) )  return;
		if ( !skipComments() )  return;

		if (new_unit)  {
			if (fscanf( file_in, "%d", &unit_no ) != 1)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}

			if (krui_setCurrentUnit( unit_no ) != KRERR_NO_ERROR)  return;

			/*  skip "|" character  */
			if (!skip_pipe())  return;
		}

		/*  test for occurence of the "|" character, if exist the unit
        has no sites  */
		if (unit_has_sites || !get_pipe())  {
			if (KernelErrorCode != KRERR_NO_ERROR)  return;
			/*  read site name  */
			if (fscanf( file_in, "%s", fmt_shape1 ) != 1)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}

			if (krui_setSite( fmt_shape1 ) != KRERR_NO_ERROR)  return;
			unit_has_sites = TRUE;

			/* following two lines were taken off between v1.3 and v1.5 of
         this file. the result was that when reading artmap nets, there
         occured a syntax error
			 */
			/* skip "|" character */
			if (!skip_pipe()) return;

		}
		else
			unit_has_sites = FALSE;

		while (TRUE)  {
			/*  read link weights  */
			fscanRet = fscanf( file_in, "%d:%f (%f,%f) ", &src_unit_no, &weight, 
					&tacoma_xi, &tacoma_ri );
			if (fscanRet==2) 
				tacoma_xi=tacoma_ri=0.0f;
			if ((fscanRet!=2)&&(fscanRet!=4)){
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
			//LinkDummy=
			krui_createLinkWithAdditionalParameters
			( src_unit_no, weight,tacoma_ri,tacoma_xi,0.0f);
			if (KernelErrorCode!=KRERR_NO_ERROR) return;
			if ( !comma() )  break;
		}

		if (unit_has_sites)  {
			if (get_alpha())  {
				/*  a new site name was found  */
				new_unit = FALSE;
			}
			else  {
				if (KernelErrorCode != KRERR_NO_ERROR)  return;
				/*  a digit was read -> a new unit number was found  */
				new_unit = TRUE;
				unit_has_sites = FALSE;
			}
		}
	}
}

/*****************************************************************************
  FUNCTION : krio_readSubnetDefs 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

void  SnnsCLib::krio_readSubnetDefs(void)
{
	int   unit_no, subnet_no;


	if ( !skipComments() )  return;
	//if ((fscanf( file_in, headers[5] ) != 0) ||
        if ((fscanf( file_in, " subnet | unitNo." ) != 0) ||
			!matchHead2( 1 ) )  {
		KernelErrorCode = KRERR_FILE_SYNTAX;
		return;
	}

	while (TRUE)  {
		if (matchHead2( 1 )) return;
		if ( !skipComments() )  return;
		if (fscanf( file_in, "%d", &subnet_no ) != 1)  {
			KernelErrorCode = KRERR_FILE_SYNTAX;
			return;
		}

		if ( !get_pipe() )  {
			KernelErrorCode = KRERR_FILE_SYNTAX;
			return;
		}

		while (TRUE)  {
			if (fscanf( file_in, "%d", &unit_no ) != 1)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
			if (kr_getUnitPtr( unit_no ) == NULL)  return;

			krui_setUnitSubnetNo( unit_no, subnet_no );

			if ( !comma() )  break;
		}
	}
}

/*****************************************************************************
  FUNCTION : krio_readLayerDefs 

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

void  SnnsCLib::krio_readLayerDefs(void)
{
	int   unit_no, layer_no;


	if ( !skipComments() )  return;
	//if ((fscanf( file_in, headers[6] ) != 0) ||
        if ((fscanf( file_in, " layer | unitNo." ) != 0) ||
			!matchHead2( 1 ) )  {
		KernelErrorCode = KRERR_FILE_SYNTAX;
		return;
	}

	while (TRUE)  {
		if (matchHead2( 1 )) return;
		if ( !skipComments() )  return;
		if (fscanf( file_in, "%d", &layer_no ) != 1)  {
			KernelErrorCode = KRERR_FILE_SYNTAX;
			return;
		}

		if ( !get_pipe() )  {
			KernelErrorCode = KRERR_FILE_SYNTAX;
			return;
		}

		while (TRUE)  {
			if (fscanf( file_in, "%d", &unit_no ) != 1)  {
				KernelErrorCode = KRERR_FILE_SYNTAX;
				return;
			}
			if (kr_getUnitPtr( unit_no ) == NULL)  return;

			krui_setUnitLayerNo( unit_no, (unsigned short) layer_no );

			if ( !comma() )  break;
		}
	}
}



/*****************************************************************************
  FUNCTION : readXYTransTable 

  PURPOSE  : read the XY-Tranlation Table from input file 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/

void  SnnsCLib::readXYTransTable(void)
{
	int x, y, z, c;


	kr_xyTransTable( OP_TRANSTABLE_CLEAR, &x, &y, 0 );

	if ( !skipComments() )  return;
	//if ((fscanf( file_in, headers[7] ) != 0) ||
        if ((fscanf( file_in, " delta x | delta y | z" ) != 0) ||
			!matchHead2( 2 ) )  {
		KernelErrorCode = KRERR_FILE_SYNTAX;
		return;
	}

	while (TRUE)  {
		if (matchHead( 2 )) return;
		if ( !skipComments() )  return;

		if (fscanf( file_in, "%d | %d | %d", &x, &y, &z ) != 3)  {
			KernelErrorCode = KRERR_FILE_SYNTAX;
			return;
		}

		/* skip remaining spaces */
		do{
			c = getc( file_in );
			if (c == '\n')  lineno++;
		}while(c == ' ');

		kr_xyTransTable( OP_TRANSTABLE_SET, &x, &y, z );
	}
}



/*****************************************************************************
  FUNCTION : krio_readTimeDelayDefs

  PURPOSE  : 
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/
void SnnsCLib::krio_readTimeDelayDefs(void)
{
	int u_no,
	lln, 
	lun,
	toff,
	soff,
	conn_type,
	ret;

	struct Unit *unit_ptr;

	if ( !skipComments() )  return;
		
	//if (fscanf( file_in, headers[8] ) != 0)
	if (fscanf( file_in, " no. | LLN | LUN | Toff | Soff | Ctype" ) != 0)
	{  KernelErrorCode = KRERR_FILE_SYNTAX;  return;  }
	if ( !matchHead2( 5 ) )
	{  KernelErrorCode = KRERR_FILE_SYNTAX;  return;  }

	while (TRUE)
	{
		if (matchHead2( 5 )) return;
		if ( !skipComments() )  return;

		ret = fscanf (file_in, "%4d |%4d |%4d |%5d |%5d |%6d", 
				&u_no, &lln, &lun, &toff, &soff, &conn_type);
		if (ret != 6)  {  KernelErrorCode = KRERR_FILE_SYNTAX;  return;  }

		unit_ptr = kr_getUnitPtr (u_no);

		unit_ptr->lln = lln;
		unit_ptr->lun = lun;
		unit_ptr->TD.target_offset = toff;
		unit_ptr->TD.source_offset = soff;
		unit_ptr->TD.td_connect_typ = conn_type;
		unit_ptr++;
	}
}   



/*****************************************************************************
  FUNCTION : krio_loadNet  

  PURPOSE  : load network from disk
  NOTES    : 

  RETURNS  : 
  UPDATE   : 
 ******************************************************************************/
krui_err  SnnsCLib::krio_loadNet(char *filename, char **netname, char **netfile_version)
{
	/*
  static char  krio_loadNet_netname_str[81],
               krio_loadNet_netfile_version_str[81],
               krio_loadNet_learn_func[81],
               krio_loadNet_update_func[81],
               krio_loadNet_pruning_func[81],
               krio_loadNet_ff_learn_func[81];
	 */

	int  no_units, no_connect, no_unitT,
	no_siteT, title_no, status;


	KernelErrorCode = KRERR_NO_ERROR;
	lineno = status = 0;

	file_in = fopen( filename, "r" );
	if (file_in == NULL)  {
		KernelErrorCode = KRERR_FILE_OPEN;  /*  Can't open file  */
		goto ende;
	}

	lineno = 1;
	if (NoOfUnits > 0)  krui_deleteNet();

	krio_readHeader( krio_loadNet_netfile_version_str, krio_loadNet_netname_str, krio_loadNet_learn_func, krio_loadNet_update_func,
			krio_loadNet_pruning_func, krio_loadNet_ff_learn_func, &no_units, &no_connect, &no_unitT,
			&no_siteT );
	if (KernelErrorCode)  goto ende;

	if(*krio_loadNet_learn_func != '\0')  {
		(void) krui_setLearnFunc( krio_loadNet_learn_func );
		if (KernelErrorCode)  goto ende;
	}
	if(*krio_loadNet_update_func != '\0')  {
		(void)  krui_setUpdateFunc( krio_loadNet_update_func );
		if (KernelErrorCode)  goto ende;
	}
	if(*krio_loadNet_pruning_func != '\0')  {
		(void) krui_setPrunFunc( krio_loadNet_pruning_func );
		if (KernelErrorCode)  goto ende;
	}
	if(*krio_loadNet_ff_learn_func != '\0')  {
		(void)  krui_setFFLearnFunc( krio_loadNet_ff_learn_func );
		if (KernelErrorCode)  goto ende;
	}

	*netname = krio_loadNet_netname_str;
	*netfile_version = krio_loadNet_netfile_version_str;

	while ( getSection( fmt_shape1, &title_no ) != NULL )  {
		if (title_no >= 0)  lineno++;  /* increment line number */

		switch (title_no)  {
		case  -3:  /*  Physical I/O error  */
			KernelErrorCode = KRERR_IO;
			return( KernelErrorCode );
		case  -2:  /*   EOF  */
			if (status < 2)  KernelErrorCode = KRERR_EOF;  /*  unexpected EOF  */
			return( KernelErrorCode );
		case  -1:  /*  not found  */
			KernelErrorCode = KRERR_FILE_SYNTAX;  /*  Syntax error at line  */
			break;
		case  8:  /*  "site definition section"  */
			krio_readSiteDefinitions();
			break;
		case  9:  /*  "type definition section"  */
			krio_readTypeDefinitions();
			break;
		case  10:  /*  "unit definition section"  */
			(void) krui_allocateUnits( no_units );
			if (KernelErrorCode)  goto ende;

			krio_readUnitDefinitions();
			status++;
			break;
		case  11:  /*  "connection definition section"  */
			krio_readConnectionDefs();
			status++;
			break;
		case  12:  /*  "subnet definition section"  */
			krio_readSubnetDefs();
			break;
		case  13:  /*  "unit default section"  */
			krio_readDefaultDefinitions();
			break;
		case  15:  /*  "layer definition section"  */
			krio_readLayerDefs();
			break;

		case  17:  /*  "3D translation section"  */
			readXYTransTable();
			break;

		case  18:  /*  "time delay section"  */
			krio_readTimeDelayDefs ();
			break;
		default:
			KernelErrorCode = KRERR_FILE_FORMAT;  /*  Incompatible file format  */
			break;
		}

		if (KernelErrorCode)  break;
	}

	if (KernelErrorCode == KRERR_NO_ERROR)  lineno = 0;

	ende:
	if (file_in != NULL)
		fclose( file_in );

	return( KernelErrorCode );
}

/*#################################################

GROUP: Functions for Pattern Management

This group has been moved to kr_newpattern.c

#################################################*/

/*#################################################

GROUP: Functions for Result Management

#################################################*/
/*****************************************************************************
  FUNCTION : krio_saveResult 

  PURPOSE  : write result to disk 
  NOTES    : 

  RETURNS  : 
  UPDATE   : now the actual update function is used instead of 
             Topological_Order (M. Vogt, 20.02.93, necessary for TD networks)
 ******************************************************************************/
krui_err SnnsCLib::krio_saveResult(char *filename, bool create, int startpattern, 
		int endpattern, bool includeinput, bool includeoutput,
		float *Update_param_array, int NoOfUpdateParam)
{
	char buf[MAX_LIN_LEN];

	char	work_str[ LIN_MAX ];
	struct Unit	*unit_ptr;
	long	clock;
	int  i, j; //ret,
	Patterns  in_pat, out_pat, save_in_pat;
	krui_err	temp_error;
	int start, end, i_size, o_size, in_pat_size;
	int pat,sub;

	lineno = 1;
	KernelErrorCode = KRERR_NO_ERROR;

	if (krui_getNoOfPatterns() == 0)
	{
		KernelErrorCode = KRERR_NO_PATTERNS;
		return( KernelErrorCode );
	}

	if (endpattern < startpattern || endpattern > krui_getNoOfPatterns())
	{
		return(KernelErrorCode = KRERR_PATTERN_NO);
	}

	/* calculate absolute numbers of first and last subpattern */
	KernelErrorCode = kr_initSubPatternOrder(startpattern-1,endpattern-1);
	if (KernelErrorCode != KRERR_NO_ERROR)
		return( KernelErrorCode );

	start = kr_AbsPosOfFirstSubPat(startpattern-1);
	end   = kr_AbsPosOfFirstSubPat(endpattern-1);
	end  += kr_NoOfSubPatPairs(endpattern-1)-1;

	if (create) {
		//file_out = fopen(filename, "w");
		//*stream_out->open(filename);
		stream_out = new std::ofstream( filename, std::ios::out );
	} else {
		//file_out = fopen(filename, "a");
		//*stream_out->open(filename, std::fstream::app);
		stream_out = new std::ofstream( filename, std::ios::app );
	};

	if (!stream_out->good())
	{ //  Can't open file
		KernelErrorCode = KRERR_FILE_OPEN;
		return( KernelErrorCode );
	}

	/*
  if (file_out == NULL)
  { //  Can't open file
    KernelErrorCode = KRERR_FILE_OPEN;
    return( KernelErrorCode );
  }
	 */

	KernelErrorCode = KRERR_IO;

	/*  write header and version number  */
	snprintf( buf, sizeof(buf), resHeader[0], krio_getIOVersion() );
	*stream_out << buf;
	RETCHKGTO( stream_out );
	lineno++;

	/*  write date  */
	clock = 1;
	(void) time( (time_t *) &clock);

	strcpy( work_str, resHeader[1] );
	strcat( work_str, " %s\n" );
	snprintf( buf, sizeof(buf), work_str, ctime( (time_t *) &clock) );
	*stream_out << buf;
	RETCHKGTO( stream_out );
	lineno++;

	/*  write no. of patterns, input units, output units  */
	strcpy( work_str, resHeader[2] );
	strcat( work_str, resHeader[3] );
	strcat( work_str, resHeader[4] );
	snprintf( buf, sizeof(buf), work_str, end - start + 1, 
			krui_getNoOfInputUnits() + krui_getNoOfSpecialInputUnits(),
			krui_getNoOfOutputUnits() + krui_getNoOfSpecialOutputUnits() );
	*stream_out << buf;
	RETCHKGTO( stream_out );
	lineno += 3;

	/* write numbers of used patterns */
	strcpy( work_str, resHeader[5] );
	strcat( work_str, resHeader[6] );
	snprintf( buf, sizeof(buf), work_str, startpattern, endpattern);
	*stream_out << buf;
	RETCHKGTO( stream_out );
	lineno += 2;

	/* write additional format information */
	if (includeinput)
	{

		*stream_out << resHeader[7];
		RETCHKGTO( stream_out );
		lineno += 1;
	}
	if (includeoutput)
	{

		*stream_out << resHeader[8];
		RETCHKGTO( stream_out );
		lineno += 1;
	}

	/* process network update for all subpattern */
	for(i=start; i<=end;i++){
		kr_getSubPatternByNo(&pat,&sub,i);
		in_pat = kr_getSubPatData(pat,sub,INPUT,&i_size);
		out_pat = kr_getSubPatData(pat,sub,OUTPUT,&o_size);

		if (in_pat == (Patterns) NULL || out_pat == (Patterns) NULL)
			goto ende;

		if (o_size == 0 && includeoutput)
		{
			KernelErrorCode = KRERR_NP_NO_OUTPUT_PATTERN;
			goto ende;
		}

		snprintf( buf, sizeof(buf), "#%d.%d\n", pat+1, sub+1 );
		*stream_out << buf;
		RETCHKGTO( stream_out );

		if (includeinput)
		{
			save_in_pat = in_pat;
			for (j = 1; j <= i_size; j++)
			{
				snprintf( work_str, sizeof(work_str), "%.5f", *in_pat );
				krio_cutTrailingZeros( work_str );

				if (j < i_size) {
					if (j % 10 == 0)  
						strcat( work_str, "\n" );
					else
						strcat( work_str, " " );
				}

				//ret = fputs( work_str, file_out );
				*stream_out << work_str;

				RETCHKGTO( stream_out );  

				lineno++;
				in_pat++;
			}
			//fputs( "\n", file_out );
			*stream_out << "\n";
			in_pat = save_in_pat;
		}

		if (includeoutput)
		{
			for (j = 1; j <= o_size; j++)
			{
				snprintf( work_str, sizeof(work_str), "%.5f", *out_pat );
				krio_cutTrailingZeros( work_str );

				if (j < o_size) {
					if (j % 10 == 0)  
						strcat( work_str, "\n" );
					else  
						strcat( work_str, " " );
				}

				//ret = fputs( work_str, file_out );
				*stream_out << work_str;
				RETCHKGTO( stream_out );  

				lineno++;
				out_pat++;
			}
			//fputs( "\n", file_out );
			*stream_out << "\n";
		}

		/* activate input layer */
		in_pat_size = i_size;
		FOR_ALL_UNITS( unit_ptr )
		if ( IS_INPUT_UNIT( unit_ptr ) && UNIT_IN_USE( unit_ptr ))
			if (in_pat_size--)
				unit_ptr->act = *in_pat++;

		/* propagate network */
		if ((temp_error = krui_updateNet(Update_param_array, 
				NoOfUpdateParam)) != KRERR_NO_ERROR)
		{
			KernelErrorCode = temp_error;
			goto ende;
		}

		/* write network output */
		j = 1;
		FOR_ALL_UNITS( unit_ptr )
		if UNIT_IN_USE( unit_ptr )
		{
			if IS_OUTPUT_UNIT( unit_ptr )
            		{
				snprintf( work_str, sizeof(work_str), "%.5f", unit_ptr->Out.output );
				krio_cutTrailingZeros( work_str );
				if (j % 10 == 0)  
					strcat( work_str, "\n" );
				else  
					strcat( work_str, " " );
				j++;
				*stream_out << work_str;
				//ret = fputs( work_str, file_out );
				RETCHKGTO( stream_out );  

				lineno++;
            		}
		}
		//fputs( "\n", file_out );
		*stream_out << "\n";
	}


	ende:
	if (KernelErrorCode == KRERR_NO_ERROR)
		lineno = 0;
	//if (fclose(file_out) != 0 && KernelErrorCode == KRERR_NO_ERROR)
	//KernelErrorCode = KRERR_IO;
	//*stream_out->close();
	static_cast<std::ofstream*>(stream_out)->close();

	return KernelErrorCode;
}

//err = fprintf( file_out,
//snprintf( buf, sizeof(buf),

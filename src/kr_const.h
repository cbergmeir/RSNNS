/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_const.h,v $
  SHORTNAME      : const.h
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Constants
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 01.03.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:39 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef KR_CONSTANTS

#define KR_CONSTANTS

/*  flag modifier:

		   |------------------------------ Direct Link Flag
		   |  |--------------------------- Site Flag
		   |  |  |------------------------ |
		   |  |  |  |--------------------- | Topologic Type Flags
		   |  |  |  |  |------------------ |
		   |  |  |  |  |  |--------------- |
		   |  |  |  |  |  |  |------------ Refresh Flag
		   |  |  |  |  |  |  |	|--------- Enable Flag
		   |  |  |  |  |  |  |	|  |------ In-Use Flag
		   |  |  |  |  |  |  |	|  |  |--- Allocation Flag

15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
-----------------------------------------------
                                           0  0  : Unit is free and may be allocated
                                           0  1  : Unit is now allocated, but not ready to use
					   1  0  : Unit is allocated and ready to use
					   1  1  : Unit is allocated and ready to use

                                        0 -----  : Unit is freezed
                                        1 -----  : Unit is active (will be updated during next cycle)

                                     0 --------  : Unit is not updated now
                                     1 --------  : Unit is currently updated

			 0  0  0  0 -----------  : Unknown topologic type
			 0  0  0  1 -----------  : Input type
			 0  0  1  0 -----------  : Output type
			 0  0  1  1 -----------  : Dual type (input and output)
			 0  1  0  0 -----------  : Hidden type
			 1  0  0  0 -----------  : Special type (no weight adjustment of input links)

		   0  0 -----------------------  : Unit has no inputs (no links and no Sites)
		   0  1 -----------------------  : Unit has at least one site (and no direct wired inputs)
		   1  0 -----------------------  : Unit has direct input links (and no sites)
		   1  1 -----------------------  : -- NOT USED --

*/

#define  UFLAG_FREE	 0x0000
#define  UFLAG_NOT_FREE  0x0001
#define  UFLAG_IN_USE	 0x0002

#define  UFLAG_INITIALIZED   0x0007
#define  UFLAG_ENABLED	 0x0004

#define  UFLAG_REFRESH	 0x0008

#define  UFLAG_TTYP_PAT  0x00f0
#define  UFLAG_TTYP_UNKN 0x0000
#define  UFLAG_TTYP_IN	 0x0010
#define  UFLAG_TTYP_OUT  0x0020
#define  UFLAG_TTYP_DUAL 0x0030
#define  UFLAG_TTYP_HIDD 0x0040
#define  UFLAG_TTYP_SPEC 0x0080
#define  UFLAG_TTYP_SPEC_I 0x0090
#define  UFLAG_TTYP_SPEC_O 0x00a0
#define  UFLAG_TTYP_SPEC_H 0x00c0
#define  UFLAG_TTYP_SPEC_D 0x00b0
#define  UFLAG_TTYP_SPEC_X 0x00FE   /* only used to identfy toggle */
#define  UFLAG_TTYP_N_SPEC_X 0x00FF /* only used to identfy toggle */

#define  UFLAG_INPUT_PAT 0x0300
#define  UFLAG_NO_INP	 0x0000
#define  UFLAG_SITES	 0x0100
#define  UFLAG_DLINKS	 0x0200


/*  The unit output identity function is the NULL pointer
*/
#define     OUT_IDENTITY    NULL



/* ***********************************************************

          Constants for selectors

*********************************************************** */
/*  modes for unit counting
*/
#define  UNIT_DELETE  1
#define  UNIT_ADD     2

/*  topologic sorting modes
*/

#define  NOT_SORTED	  0
#define  TOPOLOGICAL	  1
#define  TOPOLOGICAL_FF   2
#define  TOPOLOGIC_TYPE   3
#define  PERMUTATION	  4
#define  ART1_TOPO_TYPE   5
#define  ART2_TOPO_TYPE   6
#define  ARTMAP_TOPO_TYPE 7
#define  TOPOLOGICAL_CC  8
#define  TOPOLOGICAL_RCC 9
#define  TOPOLOGICAL_BCC 10
#define  TOPOLOGIC_LOGICAL 11
#define  TOPOLOGICAL_JE  12

/*  unit component selectors
*/
#define  SEL_UNIT_ACT  1
#define  SEL_UNIT_OUT  2
#define  SEL_UNIT_IACT  3
#define  SEL_UNIT_BIAS  4
#define  SEL_UNIT_ALL   5
#define  SEL_UNIT_VALA 6

/*  link searching selectors
*/
#define  FIRST	  1
#define  NEXT	  2
#define  CURRENT  3
#define  NAME	  4

/*  function searching in function table
*/
#define  GET_NO_OF_FUNCS  1
#define  GET_FUNC_INFO    2
#define  SEARCH_FUNC      3
#define  GET_FUNC_NAME    4
#define  GET_DEFAULT_FUNC 5

/*  link delete selectors
*/
#define  INPUTS   0
#define  OUTPUTS  1

/*  pattern function selectors
*/
#define  PATTERN_SET  1
#define  PATTERN_GET  2
#define  PATTERN_DELETE  3
#define  PATTERN_MODIFY  4
#define  PATTERN_SHOW	 5
#define  PATTERN_NEW  6
#define  PATTERN_DELETE_ALL  7
#define  PATTERN_SHUFFLE_ON  8
#define  PATTERN_SHUFFLE_OFF 9
#define  PATTERN_VECTOR_NEW  10
#define  PATTERN_ARRAY_ALLOC 11
#define  PATTERN_SET_NUMBER  12
#define  PATTERN_GET_NUMBER  13
#define  PATTERN_SUB_SHUFFLE_ON 14
#define  PATTERN_SUB_SHUFFLE_OFF 15
#define  GET_SHUFFLE_FLAG 16
#define  GET_SUB_SHUFFLE_FLAG 17
#define  SUBPATTERN_GET_NUMBER 18

/*#################################################

GROUP: Parallel kernel constants

#################################################*/


#endif







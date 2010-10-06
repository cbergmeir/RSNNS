/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_mac.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel Macros
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 09.12.91

  CHANGED BY     : Michael Vogt
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:50 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
/*  Macro Definitions  */
#ifndef numeric
#define numeric(c)	       (c >= '0' && c <= '9')
#endif
#ifndef Max
#define Max(a,b)	       (a<b ? b : a)
#endif
#ifndef Min
#define Min(a,b)	       (a>b ? b : a)
#endif
#ifndef abs
#define abs(x)		       (x>=0 ? x : -(x))
#endif


/*  Macros for stepping thru the unit array and the site/link lists
*/
#define FOR_ALL_UNITS( unit_ptr ) \
for ((unit_ptr) = (unit_array != NULL ? \
		   unit_array + MinUnitNo : unit_array + MaxUnitNo + 1); \
     (unit_ptr) <= unit_array + MaxUnitNo; (unit_ptr)++)

#define FOR_ALL_SITES( unit_ptr, site_ptr ) \
for ((site_ptr) = (unit_ptr)->sites; (site_ptr) != NULL; \
     (site_ptr) = (site_ptr)->next)

#define FOR_ALL_SITES_AND_LINKS( unit_ptr, site_ptr, link_ptr ) \
for ((site_ptr) = (unit_ptr)->sites; (site_ptr) != NULL; \
     (site_ptr) = (site_ptr)->next) \
  for ((link_ptr) = (site_ptr)->links; (link_ptr) != NULL; \
       (link_ptr) = (link_ptr)->next)

#define FOR_ALL_LINKS( unit_ptr, link_ptr ) \
for ((link_ptr) = (struct Link *) (unit_ptr)->sites; (link_ptr) != NULL; \
     (link_ptr) = (link_ptr)->next)

#define FOR_ALL_LINKS_AT_SITE( site_ptr, link_ptr ) \
for ((link_ptr) = (site_ptr)->links; (link_ptr) != NULL; \
     (link_ptr) = (link_ptr)->next)



/*  Some useful macros to determine the input type of a given unit
*/
#define UNIT_HAS_SITES( unit_ptr ) \
((unit_ptr)->flags & UFLAG_SITES)

#define UNIT_HAS_DIRECT_INPUTS( unit_ptr ) \
((unit_ptr)->flags & UFLAG_DLINKS)

#define UNIT_HAS_INPUTS( unit_ptr ) \
((unit_ptr)->flags & UFLAG_INPUT_PAT)

#define IS_INPUT_UNIT( unit_ptr ) \
((unit_ptr)->flags & UFLAG_TTYP_IN)

#define IS_OUTPUT_UNIT( unit_ptr ) \
((unit_ptr)->flags & UFLAG_TTYP_OUT)

#define IS_HIDDEN_UNIT( unit_ptr ) \
((unit_ptr)->flags & UFLAG_TTYP_HIDD)

#define IS_DUAL_UNIT( unit_ptr ) \
(((unit_ptr)->flags & UFLAG_TTYP_DUAL) == UFLAG_TTYP_DUAL)

#define IS_SPECIAL_UNIT( unit_ptr ) \
((unit_ptr)->flags & UFLAG_TTYP_SPEC)

#define IS_SPECIAL_I_UNIT( unit_ptr ) \
(((unit_ptr)->flags & UFLAG_TTYP_SPEC_I) == UFLAG_TTYP_SPEC_I)
  
#define IS_SPECIAL_H_UNIT( unit_ptr ) \
(((unit_ptr)->flags & UFLAG_TTYP_SPEC_H) == UFLAG_TTYP_SPEC_H)
 
#define IS_SPECIAL_O_UNIT( unit_ptr ) \
(((unit_ptr)->flags & UFLAG_TTYP_SPEC_O) == UFLAG_TTYP_SPEC_O)

#define IS_SPECIAL_D_UNIT( unit_ptr ) \
(((unit_ptr)->flags & UFLAG_TTYP_SPEC_D) == UFLAG_TTYP_SPEC_D)



/*  Macro to determine if a given unit is in use or not
*/
#define UNIT_IN_USE( unit_ptr ) \
((unit_ptr)->flags & UFLAG_IN_USE)

#define UNIT_REFRESHED( unit_ptr ) \
((unit_ptr)->flags & UFLAG_REFRESH)


/*  Some macros for determine the current kernel type
*/
#define  KERNEL_STANDARD  (specialNetworkType == NET_TYPE_GENERAL)




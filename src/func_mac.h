/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/func_mac.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel: Macros for transfer functions
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 21.06.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:12 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef KR_FUNC_MACROS
#define KR_FUNC_MACROS

#define  UNIT_PTR   struct Unit *
#define  SITE_PTR   struct Site *

#define  ACT_FUNC_DEFS      register struct Link  *__link_ptr; \
                            register struct Site  *__site_ptr;

#define  SITE_FUNC_DEFS     register struct Link  *__link_ptr;


/*#################################################

GROUP: Link Macros

#################################################*/

#define  GET_FIRST_UNIT_LINK( u_ptr )   (__link_ptr = ( (u_ptr)->flags & UFLAG_DLINKS) ? \
                                         ( (struct Link *) (u_ptr)->sites) : (NULL) )

#define  GET_FIRST_SITE_LINK( s_ptr )   (__link_ptr = (s_ptr)->links)

#define  GET_NEXT_LINK      (__link_ptr = __link_ptr->next)

#define  GET_WEIGHTED_OUTPUT    ( (__link_ptr->weight) * (__link_ptr->to->Out.output) )

#define  GET_OUTPUT         (__link_ptr->to->Out.output)

#define  GET_WEIGHT         (__link_ptr->weight)

#define  GET_TACOMA_RADIUS  (__link_ptr->value_a)

#define  GET_TACOMA_COORD   (__link_ptr->value_b)

#define  LINK_FROM_INPUT_UNIT (IS_INPUT_UNIT(__link_ptr->to))

/* calculate each component of the euclidic distance */ 
#define  GET_EUCLID_COMP    ( ((__link_ptr->weight) - (__link_ptr->to->Out.output))*((__link_ptr->weight) - (__link_ptr->to->Out.output)) )


/*#################################################

GROUP: Site Macros

#################################################*/

#define  GET_FIRST_SITE( u_ptr )    (__site_ptr = ( (u_ptr)->flags & UFLAG_SITES) ? \
                                     ( (u_ptr)->sites ) : (NULL) )

#define  GET_NEXT_SITE      (__site_ptr = __site_ptr->next)

#define  GET_SITE_NAME      (__site_ptr->site_table->Entry.site_name->Entry.symbol)

#define  GET_SITE_VALUE     ( (*__site_ptr->site_table->site_func) (__site_ptr) )

/*  Future Application (in SNNS-Kernel V1.4 the sites don't have weights).
*/
#define  GET_SITE_WEIGHT    ((FlintType) 1)


/*#################################################

GROUP: Unit Macros

#################################################*/

#define  GET_UNIT_BIAS( u_ptr )  ((u_ptr)->bias)
#define  GET_UNIT_ACT( u_ptr )   ((u_ptr)->act)


#endif

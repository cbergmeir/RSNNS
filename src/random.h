/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/random.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : System V Library Functions
  NOTES          :

  AUTHOR         : Niels Mache
  DATE           : 01.10.90

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:34 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef SYS_FUNCS
#define SYS_FUNCS

double	drand48( void );
#ifndef __linux__
int	rand( void );
#endif
long    lrand48(void );
void    srand48( long );

#endif

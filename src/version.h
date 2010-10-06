/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/version.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel: Current Version and Patchlevel
  NOTES          :

  AUTHOR         : Niels Mache 
  DATE           : 15.08.9

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:59 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef VERSION_INCLUDE
#define VERSION_INCLUDE

/*  define the current patchlevel
*/
#define KERNEL_DATE    "01-01-1996"
#define KERNEL_PATCH   "0"


#ifdef MASPAR_KERNEL
#define  SNNS_VERSION   "SNNS MasPar 3D-Kernel V3.3 (Spontanous Warp Speed)"
#else
#define  SNNS_VERSION   "SNNS 3D-Kernel V4.2"
#endif

#ifdef PARAGON_KERNEL
#define  SNNS_VERSION   "SNNS 3D-Paragon Kernel V3.3"
#endif


/*  Version of I/O Manager  */
#define NETFILE_VERSION  "V1.4"
#define NETFILE_VERSION2  "V2.1"

/*  3D-Kernel Netfile Version  */
#define KERNEL3D_NETFILE_VERSION  "-3D"

#endif

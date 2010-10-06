/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/art_typ.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel: Global Datatypes and Constants for ART Networks
  NOTES          : For User's Application Programs, User-Interface and Kernel

  AUTHOR         : Niels Mache
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:25:39 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef ART_GLOBAL_TYPES

#define ART_GLOBAL_TYPES



/* define the possible values for the status of an ART-network
*/
#define ART_NO_CLASSIFICATION 0  /* The network hasn't reched a stable state
                                    yet
                                 */
#define ART_CLASSIFIED        1  /* The network has found a class for the
                                    actual input pattern
                                 */
#define ART_NOT_CLASSIFIABLE  2  /* The network was not able to find a
                                    class for the actual input pattern
                                 */
#define ART_DONT_KNOW         3  /* The ARTMAP network does not find an
                                    appropriate class -> all map units
                                    are turned on.
                                 */






/* types */
typedef   int                    art_cl_status;

#endif

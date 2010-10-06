/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_art1.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Function Prototypes for ART1-Networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann 
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:35 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _KR_ART1_DEFINED_
#define  _KR_ART1_DEFINED_

/* begin global definition section */

 int             Art1_NoOfRecUnits;

 struct Unit     *Art1_cl_unit; /* Pointer to ART1 cl-unit */
 struct Unit     *Art1_nc_unit; /* Pointer to ART1 nc-unit */



/***************************************************************************/
/* kra1_sort ()

   Set logical layer numbers and logical unit numbers in an ART1 network.
   Also this function checks, whether the network is an ART1 network or not.
   Returns an error code, when actual network is no ART1 architecture.
*/
 krui_err  kra1_sort (

                            void

                           );

/***************************************************************************/
/* kra1_init_i_act ()

   Sets the initial activation values of the units
*/
 krui_err  kra1_init_i_act (

                                  double rho

                                 );


/***************************************************************************/
/* kra1_getClassNo ()

   Returns the number of the actually activated class J, 1 <= J <= M
*/
 int  kra1_getClassNo (

                             void

                            );



/* end global definition section */

/* begin private definition section */

static TopoPtrArray      Art1_del_layer = NULL;   /* Points to first pointer
                                                     within delay-layer
                                                  */


/**************** functions, local to this module ******************************/

static int                kra1_get_NoOfRecUnits (

                                                 void

                                                );

static krui_err           kra1_get_InpUnits (

                                             TopoPtrArray *topo_ptr

                                            );

static krui_err           kra1_get_CmpUnits (

                                             TopoPtrArray *topo_ptr,
                                             int *no_of_cmp_units

                                            );

static krui_err           kra1_get_RecUnits (

                                             TopoPtrArray *topo_ptr

                                            );

static krui_err           kra1_get_DelUnits (

                                             TopoPtrArray *topo_ptr,
                                             int *no_of_del_units

                                            );

static krui_err           kra1_get_RstUnits (

                                             TopoPtrArray *topo_ptr,
                                             int *no_of_rst_units

                                            );

static krui_err           kra1_get_SpecUnits (

                                              TopoPtrArray *topo_ptr,
                                              int *no_of_spec_units

                                             );

static krui_err           kra1_TopoPtrArray (

                                             void

                                            );

static krui_err           kra1_Sites (

                                      void

                                     );

static krui_err           kra1_LinksToInpUnits (

                                                TopoPtrArray *topo_inp_ptr

                                               );

static krui_err           kra1_LinksToCmpUnits (

                                                TopoPtrArray *topo_cmp_ptr

                                               );

static krui_err           kra1_LinksToRecUnits (

                                                TopoPtrArray *topo_rec_ptr

                                               );

static krui_err           kra1_LinksToDelUnits (

                                                TopoPtrArray *topo_del_ptr

                                               );

static krui_err           kra1_LinksToRstUnits (

                                                TopoPtrArray *topo_rst_ptr

                                               );

static krui_err           kra1_LinksToSpecUnits (

                                                 TopoPtrArray *topo_spec_ptr

                                                );

static void               kra1_set_fix_weight (

                                               struct Unit *src_unit,
                                               struct Unit *trgt_unit,
                                               FlintType   *weight

                                              );

static krui_err           kra1_init_fix_weights (

                                                 void

                                                );


/* end private definition section */

 
#endif

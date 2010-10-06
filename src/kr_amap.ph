/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_amap.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Function Prototypes for ARTMAP-Networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:29 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _KR_AMAP_DEFINED_
#define  _KR_AMAP_DEFINED_

/* begin global definition section */



 int             ArtMap_NoOfInpUnits_a;
 int             ArtMap_NoOfInpUnits_b;
 int             ArtMap_NoOfRecUnits_a;
 int             ArtMap_NoOfRecUnits_b;

 struct Unit    *ArtMap_cl_unit;  /* Pointer to cl-unit */
 struct Unit    *ArtMap_nc_unit;  /* Pointer to nc-unit */




/***************************************************************************/
/* kram_init_i_act ()

   Sets the initial activation values of the units
*/
 krui_err kram_init_i_act (

                                 double rho_a,
                                 double rho_b,
                                 double rho

                                );


/***************************************************************************/
/* kram_sort ()

   Check for ARTMAP topology and create topo ptr array
*/
 krui_err  kram_sort (

                            void

                           );




/***************************************************************************/
/* kram_getClassNo ()

   Returns the number of the actually activated class K, 1 <= K <= Mb
*/
 int  kram_getClassNo (

                             void

                            );



/***************************************************************************/
/* kram_AllMapUnitsActive ()

   Returns TRUE, if all MAP-Field-Units are active, else, FALSE
*/
 bool  kram_AllMapUnitsActive (

                                     void

                                    );


/* end global definition section */

/* begin private definition section */

static TopoPtrArray      ArtMap_map_layer = NULL;   /* Points to first pointer
                                                       within map-layer
                                                    */




static krui_err  kram_get_InpUnits_a (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );

static krui_err  kram_get_CmpUnits_a (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );

static krui_err  kram_get_RecUnits_a (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


static krui_err  kram_get_DelUnits_a (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


static krui_err  kram_get_RstUnits_a (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


static krui_err  kram_get_SpecUnits_a (

                                       TopoPtrArray      *topo_ptr,
                                       int               *no_of_units

                                      );


static krui_err  kram_get_InpUnits_b (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );

static krui_err  kram_get_CmpUnits_b (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );

static krui_err  kram_get_RecUnits_b (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


static krui_err  kram_get_DelUnits_b (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


static krui_err  kram_get_RstUnits_b (

                                      TopoPtrArray      *topo_ptr,
                                      int               *no_of_units

                                     );


static krui_err  kram_get_SpecUnits_b (

                                       TopoPtrArray      *topo_ptr,
                                       int               *no_of_units

                                      );



static krui_err  kram_get_MapUnits (

                                    TopoPtrArray      *topo_ptr,
                                    int               *no_of_units

                                   );


static krui_err  kram_get_SpecUnits (

                                     TopoPtrArray      *topo_ptr,
                                     int               *no_of_units

                                    );


static krui_err  kram_TopoPtrArray (

                                    void

                                   );


static krui_err  kram_Sites (

                             void

                            );


static krui_err  kram_LinksToInpUnits_a (

                                         TopoPtrArray *topo_ptr

                                        );


static krui_err  kram_LinksToCmpUnits_a (

                                         TopoPtrArray *topo_ptr

                                        );


static krui_err  kram_LinksToRecUnits_a (

                                         TopoPtrArray *topo_ptr

                                        );


static krui_err  kram_LinksToDelUnits_a (

                                         TopoPtrArray *topo_ptr

                                        );


static krui_err  kram_LinksToRstUnits_a (

                                         TopoPtrArray *topo_ptr

                                        );


static krui_err  kram_LinksToSpecUnits_a (

                                          TopoPtrArray *topo_ptr

                                         );


static krui_err  kram_LinksToInpUnits_b (

                                         TopoPtrArray *topo_ptr

                                        );


static krui_err  kram_LinksToCmpUnits_b (

                                         TopoPtrArray *topo_ptr

                                        );


static krui_err  kram_LinksToRecUnits_b (

                                         TopoPtrArray *topo_ptr

                                        );


static krui_err  kram_LinksToDelUnits_b (

                                         TopoPtrArray *topo_ptr

                                        );


static krui_err  kram_LinksToRstUnits_b (

                                         TopoPtrArray *topo_ptr

                                        );


static krui_err  kram_LinksToSpecUnits_b (

                                          TopoPtrArray *topo_ptr

                                         );


static krui_err  kram_LinksToMapUnits (

                                       TopoPtrArray *topo_ptr

                                      );


static krui_err  kram_LinksToSpecUnits (

                                        TopoPtrArray *topo_ptr

                                       );


static krui_err  kram_init_fix_weights (

                                        void

                                       );


static void  kram_set_fix_weight (

                                  struct Unit *src_unit,
                                  struct Unit *trgt_unit,
                                  FlintType   *weight

                                 );




/* end private definition section */

#endif 

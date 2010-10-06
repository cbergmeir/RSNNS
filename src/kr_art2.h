/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_art2.h,v $
  SHORTNAME      : kr_art2 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Function Prototypes for ART2-Networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:37 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _KR_ART2_DEFINED_
#define  _KR_ART2_DEFINED_


/************* Global variables
*************/
extern int              Art2_NoOfRecUnits;
extern  struct Unit     *Art2_cl_unit;
extern  struct Unit     *Art2_nc_unit;

/**************** Function Prototypes
****************/

/***************************************************************************/
/* kra2_init_propagate ()

   initializes net for propagation.
*/
extern  krui_err  kra2_init_propagate (
                                      void
                                     );


/***************************************************************************/
/* kra2_sort ()

   Set logical layer numbers and logical unit numbers in an ART2 network.
   Also this function checks, whether the network is an ART2 network or not.
   Returns an error code, when actual network is no ART2 architecture.
*/
extern  krui_err  kra2_sort (
                            void
                           );



/***************************************************************************/
/* kra2_set_params ()

   Sets the value of Parameters rho, a, b, c, d, theta, which are stored locally
   in this Module.
*/
extern  krui_err kra2_set_params (
                                FlintType rho,
                                FlintType param_a,
                                FlintType param_b,
                                FlintType param_c,
                                FlintType param_d,
                                FlintType theta
                               );


/***************************************************************************/
/* kra2_get_rho ()

   returns the actual value of Parameter rho.
*/
extern  FlintType kra2_get_rho (
                               void
                              );

/***************************************************************************/
/* kra2_get_a ()

   returns the actual value of Parameter a.
*/
 FlintType kra2_get_a (
                             void
                            );

/***************************************************************************/
/* kra2_get_b ()

   returns the actual value of Parameter b.
*/
extern  FlintType kra2_get_b (
                             void
                            );

/***************************************************************************/
/* kra2_get_c ()

   returns the actual value of Parameter c.
*/
extern  FlintType kra2_get_c (
                             void
                            );


/***************************************************************************/
/* kra2_get_d ()

   returns the actual value of Parameter d.
*/
extern  FlintType kra2_get_d (
                             void
                            );


/***************************************************************************/
/* kra2_get_theta ()

   returns the actual value of Parameter theta.
*/
extern  FlintType kra2_get_theta (
                                 void
                                );


/***************************************************************************/
/* kra2_checkReset ()

   checks if global reset has to be sent into network
*/
extern  void kra2_checkReset (
                             void
                            );




/***************************************************************************/
/* kra2_Reset ()

   returns TRUE if global reset is actually active, else FALSE
*/
extern  bool kra2_Reset (
                        void
                       );




/***************************************************************************/
/* kra2_init_pattern ()

   sets current phase to bottom up
*/
extern  void kra2_init_pattern (
                               void
                              );



/***************************************************************************/
/* kra2_top_dn_phase ()

   sets current phase to bottom up
*/
extern  bool kra2_topdn_phase (
                             void
                            );



/***************************************************************************/
/* kra2_compute_norms ()

   computes the L2 vector norms of inp, w, u, v, p, r
*/

extern  void kra2_compute_norms (
                                void
                               );






/***************************************************************************/
/* kra2_L2_Norm ()

   returns the L2-Norm of a vector which is determined by the number
   of the Layer in the topo_ptr_array.
*/

extern  FlintType kra2_L2_Norm (
                               int VectorNr
                              );



/***************************************************************************/
/* kra2_classified ()

   returns TRUE if net has classified input pattern
*/
extern  bool kra2_classified (
                             void
                            );



/***************************************************************************/
/* kra2_not_classifiable ()

   returns TRUE if net is not able to classify input pattern
*/
extern  bool kra2_not_classifiable (
                                   void
                                  );



/***************************************************************************/
/* kra2_save_for_stability_check ()

   saves informaion of relevant unit in F1-Layer for stability check
*/
extern  void kra2_save_for_stability_check (
                                           void
                                          );



/***************************************************************************/
/* kra2_check_f1_stability ()

   checks, if F1-Layer is stable.
*/
extern  void kra2_check_f1_stability (
                                     void
                                    );





/***************************************************************************/
/* kra2_f1_stable ()

   returns TRUE if F1-Layer is stable
*/
extern  bool kra2_f1_stable (
                            void
                           );




/***************************************************************************/
/* kra2_getClassNo ()

   Returns the number of the actually activated class J, 1 <= J <= M
*/
extern  int  kra2_getClassNo (void
                            );




#endif 


/* 275 lines generated by deleteprivatedefinitions.awk */

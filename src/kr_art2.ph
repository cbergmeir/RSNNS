/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_art2.ph,v $
  SHORTNAME      : kr_art2 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Function Prototypes for ART2-Networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:38 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _KR_ART2_DEFINED_
#define  _KR_ART2_DEFINED_

/* begin global definition section */

/************* Global variables
*************/
 int              Art2_NoOfRecUnits;
 struct Unit     *Art2_cl_unit;
 struct Unit     *Art2_nc_unit;

/**************** Function Prototypes
****************/

/***************************************************************************/
/* kra2_init_propagate ()

   initializes net for propagation.
*/
 krui_err  kra2_init_propagate (
                                      void
                                     );


/***************************************************************************/
/* kra2_sort ()

   Set logical layer numbers and logical unit numbers in an ART2 network.
   Also this function checks, whether the network is an ART2 network or not.
   Returns an error code, when actual network is no ART2 architecture.
*/
 krui_err  kra2_sort (
                            void
                           );



/***************************************************************************/
/* kra2_set_params ()

   Sets the value of Parameters rho, a, b, c, d, theta, which are stored locally
   in this Module.
*/
 krui_err kra2_set_params (
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
 FlintType kra2_get_rho (
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
 FlintType kra2_get_b (
                             void
                            );

/***************************************************************************/
/* kra2_get_c ()

   returns the actual value of Parameter c.
*/
 FlintType kra2_get_c (
                             void
                            );


/***************************************************************************/
/* kra2_get_d ()

   returns the actual value of Parameter d.
*/
 FlintType kra2_get_d (
                             void
                            );


/***************************************************************************/
/* kra2_get_theta ()

   returns the actual value of Parameter theta.
*/
 FlintType kra2_get_theta (
                                 void
                                );


/***************************************************************************/
/* kra2_checkReset ()

   checks if global reset has to be sent into network
*/
 void kra2_checkReset (
                             void
                            );




/***************************************************************************/
/* kra2_Reset ()

   returns TRUE if global reset is actually active, else FALSE
*/
 bool kra2_Reset (
                        void
                       );




/***************************************************************************/
/* kra2_init_pattern ()

   sets current phase to bottom up
*/
 void kra2_init_pattern (
                               void
                              );



/***************************************************************************/
/* kra2_top_dn_phase ()

   sets current phase to bottom up
*/
 bool kra2_topdn_phase (
                             void
                            );



/***************************************************************************/
/* kra2_compute_norms ()

   computes the L2 vector norms of inp, w, u, v, p, r
*/

 void kra2_compute_norms (
                                void
                               );






/***************************************************************************/
/* kra2_L2_Norm ()

   returns the L2-Norm of a vector which is determined by the number
   of the Layer in the topo_ptr_array.
*/

 FlintType kra2_L2_Norm (
                               int VectorNr
                              );



/***************************************************************************/
/* kra2_classified ()

   returns TRUE if net has classified input pattern
*/
 bool kra2_classified (
                             void
                            );



/***************************************************************************/
/* kra2_not_classifiable ()

   returns TRUE if net is not able to classify input pattern
*/
 bool kra2_not_classifiable (
                                   void
                                  );



/***************************************************************************/
/* kra2_save_for_stability_check ()

   saves informaion of relevant unit in F1-Layer for stability check
*/
 void kra2_save_for_stability_check (
                                           void
                                          );



/***************************************************************************/
/* kra2_check_f1_stability ()

   checks, if F1-Layer is stable.
*/
 void kra2_check_f1_stability (
                                     void
                                    );





/***************************************************************************/
/* kra2_f1_stable ()

   returns TRUE if F1-Layer is stable
*/
 bool kra2_f1_stable (
                            void
                           );




/***************************************************************************/
/* kra2_getClassNo ()

   Returns the number of the actually activated class J, 1 <= J <= M
*/
 int  kra2_getClassNo (void
                            );


/* end global definition section */

/* begin private definition section */


/*#################################################

GROUP: local defines

#################################################*/



#define MIN_NO_OF_DELAY_STEPS   5                /* when checking if pattern has been
                                                    classified F1 layer has to be stable
                                                    over MIN_NO_OF_DELAY_STEPS cycles
                                                 */

#define F1_STABILITY_PARAM      0.0001           /* if difference of activation in all
                                                    u units between two prop cycles are
                                                    less or equal to F1_STABILITY_PARAM
                                                    then F1 is stable
                                                 */


/*#################################################

GROUP: global variables, local to this module

#################################################*/

/* Global variable for parameter values */

static FlintType      Param_rho   = 0.8;
static FlintType      Param_a     = 0.0;
static FlintType      Param_b     = 0.0;
static FlintType      Param_c     = 0.0;
static FlintType      Param_d     = 0.0;
static FlintType      Param_theta = 0.5;

/* Global variables for vector norms */

static FlintType      NormInp     = 0.0;
static FlintType      NormW       = 0.0;
static FlintType      NormU       = 0.0;
static FlintType      NormV       = 0.0;
static FlintType      NormP       = 0.0;
static FlintType      NormR       = 0.0;


static TopoPtrArray   topo_layer[10];      /* contains pointers to first pointer
                                              to inp unit, first pointer
                                              to w unit, x unit ... in the topo
                                              pointer array
                                           */

static int            NoOfDelaySteps = 0;

static bool           GlobalReset     = FALSE;
static bool           TopDownPhase;
static bool           f1_stable = FALSE;   /* becomes TRUE if F1-Layer is stable
                                              (see kra2_check_f1_stability)
                                           */

/* functions that are local to this module
*/

static void   kra2_set_fix_weight (

                                   struct Unit   *src_unit,
                                   struct Unit   *trgt_unit,
                                   FlintType     *weight

                                  );


static FlintType  kra2_compute_l2_norm (

                                        int Layer

                                       );

static int  kra2_get_NoOfRecUnits (

                                   void

                                  );


static krui_err  kra2_get_InpUnits (

                                    TopoPtrArray  *topo_ptr

                                   );


static krui_err  kra2_get_WUnits (

                                  TopoPtrArray *topo_ptr,
                                  int          *no_of_w_units

                                 );


static krui_err  kra2_get_XUnits (

                                  TopoPtrArray *topo_ptr,
                                  int          *no_of_x_units

                                 );


static krui_err  kra2_get_UUnits (

                                  TopoPtrArray *topo_ptr,
                                  int          *no_of_u_units

                                 );


static krui_err  kra2_get_VUnits (

                                  TopoPtrArray *topo_ptr,
                                  int          *no_of_v_units

                                 );


static krui_err  kra2_get_PUnits (

                                  TopoPtrArray *topo_ptr,
                                  int          *no_of_p_units

                                 );


static krui_err  kra2_get_QUnits (

                                  TopoPtrArray *topo_ptr,
                                  int          *no_of_q_units

                                 );


static krui_err  kra2_get_RUnits (

                                  TopoPtrArray *topo_ptr,
                                  int          *no_of_r_units

                                 );


static krui_err  kra2_get_RecUnits (

                                    TopoPtrArray  *topo_ptr

                                   );


static krui_err  kra2_get_RstUnits (

                                    TopoPtrArray  *topo_ptr,
                                    int          *no_of_rst_units

                                   );


static krui_err  kra2_TopoPtrArray (

                                    void

                                   );


static krui_err  kra2_LinksToInpUnits (

                                       TopoPtrArray *topo_ptr

                                      );


static krui_err  kra2_LinksToWUnits (

                                     TopoPtrArray *topo_ptr

                                    );


static krui_err  kra2_LinksToXUnits (

                                     TopoPtrArray *topo_ptr

                                    );


static krui_err  kra2_LinksToUUnits (

                                     TopoPtrArray *topo_ptr

                                    );


static krui_err  kra2_LinksToVUnits (

                                     TopoPtrArray *topo_ptr

                                    );


static krui_err  kra2_LinksToPUnits (

                                     TopoPtrArray *topo_ptr

                                    );


static krui_err  kra2_LinksToQUnits (

                                     TopoPtrArray *topo_ptr

                                    );


static krui_err  kra2_LinksToRUnits (

                                     TopoPtrArray *topo_ptr

                                    );


static krui_err  kra2_LinksToRecUnits (

                                       TopoPtrArray *topo_ptr

                                      );


static krui_err  kra2_LinksToRstUnits (
                                       TopoPtrArray *topo_ptr
                                      );

static krui_err kra2_init_i_act (void);
static krui_err kra2_init_fix_weights (void);

/* end private definition section */

#endif 


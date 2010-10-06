/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_art.ph,v $
  SHORTNAME      : kr_art.c
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Function Prototypes for ART-Networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:26:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _KR_ART_DEFINED_
#define  _KR_ART_DEFINED_

/* begin global definition section */

/***************************************************************************/
/* krart_prop_synch ()

   synchronous propagation (1 cycle) i.e. each unit puts its output onto
   its outgoing links and calculates its new activation.
*/
 void  krart_prop_synch (void);

/***************************************************************************/
/* krart_get_winner ()

   get the winner of the last propagation cycle. returns a pointer to
   the winning recognition unit.
*/
 struct Unit  *krart_get_winner (TopoPtrArray wta_layer,FlintType winner_output);

/***************************************************************************/
/* krart_reset_activations

   reset activation values of all non input units to their i_act value.
*/
 krui_err   krart_reset_activations (void);


/***************************************************************************/
/* krart_deleteTouchFlags

   delete touch-flags of all units in unit-array
*/
 void               krart_deleteTouchFlags (void);

/***************************************************************************/
/* krart_init_sorting

   initialize unit-array in order to prepare it for sorting routine
*/
 void               krart_init_sorting (void);

/***************************************************************************/
/* krart_get_NoOfInputUnits

   returns no of input units of the network
*/
 int                krart_get_NoOfInputUnits (void);

/***************************************************************************/
/* krart_check_undeterminedUnits

   checks network for undetermined units. if there are any, returns true
   else false.
*/
 bool               krart_check_undeterminedUnits (void);

/***************************************************************************/
/* krart_save_inp_pat()

   copies activation values of input units to value_a field of unit-struct.
*/
 void krart_save_inp_pat (TopoPtrArray topo_inp_ptr);




/***************************************************************************/
/* krart_inp_pat_changed ()

   returns TRUE, if one or more activation values of input units
   differ from value_a i.e. their value changed.
*/
 bool krart_inp_pat_changed (TopoPtrArray topo_inp_ptr);


/* end global definition section */

/* begin private definition section */

/* end private definition section */

#endif 





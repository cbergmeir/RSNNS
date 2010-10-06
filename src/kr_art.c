/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_art.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Functions for ART networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.7 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:34 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdlib.h>

#ifndef NULL /* if NULL pointer is not defined include stdio.h */
#include <stdio.h>
#endif

#include "krart_df.h"   /*  Definitions for ART networks */

#include "kr_const.h"
#include "kr_mac.h"
#include "kr_def.h"
#include "kr_typ.h"
#include "kr_funcs.h"
#include "kernel.h"
#include "glob_typ.h"
#include "kr_art.ph"

/* declaration of module functions
*/

/**************** functions, visible to other modules **************************/

/*#################################################

GROUP: ART kernel functions
       by Kai-Uwe Herrmann

#################################################*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/* initialization of activations of all input units according to their i_act
   value. this initialization is necessary each time a new pattern is
   presented to the network.
*/

krui_err krart_reset_activations (void)
{
   krui_err        ret_code = KRERR_NO_ERROR;

   struct Unit     *unit_ptr;


   FOR_ALL_UNITS (unit_ptr) {

      if (! IS_INPUT_UNIT (unit_ptr)) {
         unit_ptr->act = unit_ptr->i_act;
      } /*if*/

      /* The output is set for input units, too
      */
      unit_ptr->Out.output = unit_ptr->act;

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* krart_reset_activations () */
/*___________________________________________________________________________*/





/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/* synchronous propagation (1 cycle) i.e. each unit puts its output onto
   its outgoing links and calculates its new activation.
*/

extern FlintType OUT_Custom_Python(FlintType act);

void  krart_prop_synch (void)
{
  struct Unit    *unit_ptr;

  /* calculate new activations of non input units
  */

  FOR_ALL_UNITS (unit_ptr) {

     if (!IS_INPUT_UNIT(unit_ptr)) {
        unit_ptr->act = (*unit_ptr->act_func) (unit_ptr);
     } /*if*/

  } /*FOR_ALL_UNITS*/

  /* set new output values
  */

  FOR_ALL_UNITS (unit_ptr) {

     if (unit_ptr->out_func == OUT_IDENTITY) {
        unit_ptr->Out.output = unit_ptr->act;
     } else if(unit_ptr->out_func == OUT_Custom_Python) {
     	unit_ptr->Out.output =
		kr_PythonOutFunction(unit_ptr->python_out_func,
				unit_ptr->act);
     } else {
        unit_ptr->Out.output = (*unit_ptr->out_func) (unit_ptr->act);
     } /*if*/

  } /*FOR_ALL_UNITS*/

} /* krart_prop_synch */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/* get the winner of the last propagation cycle. returns a pointer to
   the winning recognition unit.
*/
struct Unit  *krart_get_winner (TopoPtrArray wta_layer, FlintType winner_output)
                            /* points to first pointer to recognition unit
                            */
                                /* output value of the winning unit */

{
   TopoPtrArray   topo_ptr;

   struct Unit    *unit_ptr,
                  *winner_ptr = NULL;  /* points to the unit with the maximal
                                          activation
                                       */

   FlintType      max_out  = 0.0;      /* contains the maximal activation
                                       */


   topo_ptr = wta_layer;

   while ((unit_ptr = *topo_ptr++) != NULL) {

      if (unit_ptr->Out.output > max_out) {
         max_out    = unit_ptr->Out.output;
         winner_ptr = unit_ptr;
         continue;
      } /*if*/

      /* The foollowing statements assure, that a winner is returned, if
         no Reset signal is sent from Reset general unit. So we will
         find a winner, even if the winner unit's activation is 0.0
         This is neccessary to assure, that all recognition units are
         tested for to get a 'not classifiable' signal when all and really
         all local reset units are turned on.
      */
      if ((winner_ptr == NULL) && (unit_ptr->Out.output >= max_out)) {
         max_out    = unit_ptr->Out.output;
         winner_ptr = unit_ptr;
      } /*if*/

   } /*while*/



   /* set activation and output of winner unit to 1.0, the one of the
      other recognition units to 0.0
   */

   topo_ptr = wta_layer;

   while ((unit_ptr = *topo_ptr++) != NULL) {

      if (unit_ptr != winner_ptr) {
         unit_ptr->Out.output = 0.0;
      } else {
         unit_ptr->Out.output = winner_output;
      } /*if*/

   } /*while*/


   return (winner_ptr);

} /* krart_get__winner () */
/*___________________________________________________________________________*/





/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void  krart_deleteTouchFlags (void)
{
   register struct Unit *unit_ptr;

   FOR_ALL_UNITS (unit_ptr) {

      /* delete touch flags
      */
      unit_ptr->flags &= ~UFLAG_REFRESH;

   } /*FOR_ALL_UNITS*/

} /* krart_deleteTouchFlags () */
/*___________________________________________________________________________*/





/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void  krart_init_sorting (void)
{
   register struct Unit *unit_ptr;


   krart_deleteTouchFlags ();

   FOR_ALL_UNITS (unit_ptr) {

      /* init lln and lun
      */
      unit_ptr->lln = unit_ptr->lun = 0;

   } /*FOR_ALL_UNITS*/

} /* krart_init_sorting () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
int  krart_get_NoOfInputUnits (void)
{
   register struct Unit  *unit_ptr;
   int                   count           = 0;


   FOR_ALL_UNITS (unit_ptr) {

      if (IS_INPUT_UNIT(unit_ptr)) {
         count++;
      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (count);


} /* krart_get_NoOfInputUnits () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
bool   krart_check_undeterminedUnits (void)
{
   register struct Unit   *unit_ptr;

   FOR_ALL_UNITS (unit_ptr) {

      if (unit_ptr->lln == 0) {
         TOPO_MSG_UNDETERMINED_UNIT (unit_ptr);
         return (TRUE);
      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (FALSE);

} /* krart_check_undeterminedUnits () */
/*___________________________________________________________________________*/







/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void  krart_save_inp_pat (TopoPtrArray topo_inp_ptr)
{
  TopoPtrArray   topo_ptr = topo_inp_ptr;

  while (*topo_ptr != NULL) {
     if (IS_INPUT_UNIT (*topo_ptr)) {
        (*topo_ptr)->value_a = (*topo_ptr)->act;
     } /*if*/
     topo_ptr++;
  } /*while*/
  return;
} /* krart_save_inp_pat () */
/*___________________________________________________________________________*/






/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
bool  krart_inp_pat_changed (TopoPtrArray topo_inp_ptr)
{

   TopoPtrArray    topo_ptr = topo_inp_ptr;

   while (*topo_ptr != NULL) {
      if ((IS_INPUT_UNIT(*topo_ptr)) && ((*topo_ptr)->value_a != (*topo_ptr)->act)) {
         return (TRUE);
      } /*if*/
      topo_ptr++;
   } /*FOR_ALL_UNITS*/

   return (FALSE);
} /* krart_inp_pat_changed () */
/*___________________________________________________________________________*/














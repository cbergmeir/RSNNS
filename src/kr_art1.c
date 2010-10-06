/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_art1.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Functions for ART1 networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:35 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdlib.h>


#ifndef NULL /* if NULL pointer is not defined include stdio.h */
#include <stdio.h>
#endif

#include <string.h>

#include "kr_const.h"
#include "kr_mac.h"
#include "kr_def.h"
#include "kr_typ.h"
#include "kr_funcs.h"
#include "kernel.h"
#include "glob_typ.h"
#include "kr_art.h"     /*  Function prototypes for ART networks */
#include "krart_df.h"   /*  Definitions for ART networks */
#include "kr_art1.ph"



/*#################################################

GROUP: ART 1 kernel functions
       by Kai-Uwe Herrmann

#################################################*/






/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/* NOTE: Don't call this function unless net has been topologically sorted
         with type ART1_TOPO_TYPE
*/

krui_err  kra1_init_i_act (double rho)
{
   int                    ret_code = KRERR_NO_ERROR;

   register struct Unit   *unit_ptr;


   FOR_ALL_UNITS (unit_ptr) {

      switch (unit_ptr->lln) {

      case ART1_SPEC_LAY:
         switch (unit_ptr->lun) {
         case ART1_G1_UNIT :
            unit_ptr->i_act = 0.0;
            break;
         case ART1_RI_UNIT :
            unit_ptr->i_act = 1.0;
            break;
         case ART1_RG_UNIT :
            unit_ptr->i_act = 1.0;
            break;
         case ART1_RHO_UNIT :
            unit_ptr->i_act = (FlintType) rho;
            break;
         default :
            unit_ptr->i_act = 0.0;
            break;
         } /*switch*/
         break;

      default :
         unit_ptr->i_act = 0.0;
         break;
      } /*switch*/

   } /*FOR_ALL_UNITS*/


   return (ret_code);

} /* kra1_init_i_act () */
/*___________________________________________________________________________*/






/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
krui_err  kra1_sort (void)
{

   int                  no_of_cmp_units = 0;
   int                  no_of_del_units = 0;
   int                  no_of_rst_units = 0;
   int                  no_of_spec_units = 0;

   TopoPtrArray         topo_ptr = topo_ptr_array;

   int                  ret_code = KRERR_NO_ERROR;  /*  reset return code  */



   /* initialize ART1 sorting
   */
   krart_init_sorting ();


   /* get no of input units and no of recognition units
   */
   NoOfInputUnits         = krart_get_NoOfInputUnits ();
   Art1_NoOfRecUnits      = kra1_get_NoOfRecUnits ();


   if (NoOfInputUnits == 0) {
      ret_code = KRERR_NO_INPUT_UNITS;
      return (ret_code);
   } /*if*/

   if (Art1_NoOfRecUnits == 0) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("recognition");
   } /*if*/



   /* insert a NULL pointer to topo ptr array for left limitation
   */
   *topo_ptr++ = NULL;


   /* determine unit types and insert them into topo ptr array
   */

   /* determine input units
   */
   ret_code = kra1_get_InpUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine comparison units
   */
   ret_code = kra1_get_CmpUnits (&topo_ptr, &no_of_cmp_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_cmp_units != NoOfInputUnits) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("comparison");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine recognition units
   */
   ret_code = kra1_get_RecUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine delay units
   */
   Art1_del_layer = topo_ptr;
   ret_code = kra1_get_DelUnits (&topo_ptr, &no_of_del_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   if (no_of_del_units != Art1_NoOfRecUnits + 3) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("delay");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine local reset units
   */
   ret_code = kra1_get_RstUnits (&topo_ptr, &no_of_rst_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_rst_units != Art1_NoOfRecUnits) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("reset");
   } /*if*/

   *topo_ptr++ = NULL;

   ret_code = kra1_get_SpecUnits (&topo_ptr, &no_of_spec_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_spec_units != ART1_NO_OF_SPEC_UNITS) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("special");
   } /*if*/

   *topo_ptr++ = NULL;


   /* check if the logical type of really all units is determined
   */
   if (krart_check_undeterminedUnits ()) {
      ret_code = topo_msg.error_code;
      return (ret_code);
   } /*if*/


   /* Now check the topo ptr array
   */
   ret_code = kra1_TopoPtrArray ();

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check the sites
   */

   ret_code = kra1_Sites ();

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check link structure
   */
   topo_ptr = topo_ptr_array + 1;

   /* Check links of input units
   */
   ret_code = kra1_LinksToInpUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* check links of comparison units
   */
   ret_code = kra1_LinksToCmpUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* check links of recognition units
   */
   ret_code = kra1_LinksToRecUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* check links of delay units
   */
   ret_code = kra1_LinksToDelUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* check links of local reset units
   */
   ret_code = kra1_LinksToRstUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* check links of special function units
   */
   ret_code = kra1_LinksToSpecUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   ret_code = kra1_init_fix_weights ();

   return (ret_code);

} /* kra1_sort () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
int   kra1_getClassNo (void)
{
    TopoPtrArray   topo_ptr = Art1_del_layer;
    int            i;

    /* if ART1 sorting wasn't performed then return negative value
       to indicate mistake
    */
    if (topo_ptr == NULL) {
       return (-1);
    } /*if*/

    /* look for winning unit */
    for (i = 1; (i <= Art1_NoOfRecUnits) || ((*topo_ptr)->act >= 0.9);
         i++, topo_ptr++
        );

    if ((i > Art1_NoOfRecUnits) && ((*topo_ptr)->act < 0.9)) {
       return (-1);
    } else {
       return (topo_ptr - Art1_del_layer + 1);
    } /*if*/

} /* kra1_getClassNo () */
/*___________________________________________________________________________*/





/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static void   kra1_set_fix_weight (struct Unit *src_unit, struct Unit *trgt_unit, FlintType *weight)
{

   if ((src_unit == NULL) || (trgt_unit == NULL)) {
      return;
   } /*if*/

   switch (src_unit->lln) {

   case ART1_INP_LAY:

      switch (trgt_unit->lln) {
      case ART1_CMP_LAY:
         *weight = ART1_LINK_INP_CMP;
         break;
      case ART1_SPEC_LAY:
         switch (trgt_unit->lun) {
         case ART1_G1_UNIT:
            *weight = ART1_LINK_INP_G1;
            break;
         case ART1_RI_UNIT:
            *weight = ART1_LINK_INP_RI;
            break;
         case ART1_G2_UNIT:
            *weight = ART1_LINK_INP_G2;
            break;
         } /* switch */
         break;
      } /*switch*/
      break;

   case ART1_CMP_LAY:

      switch (trgt_unit->lln) {
      case ART1_SPEC_LAY:
         *weight = ART1_LINK_CMP_RC;
         break;
      } /*switch*/
      break;

   case ART1_REC_LAY:

      switch (trgt_unit->lln) {
      case ART1_DEL_LAY:
         *weight = ART1_LINK_REC_DEL;
         break;
      case ART1_SPEC_LAY:
         *weight = ART1_LINK_REC_G1;
         break;
      } /*switch*/
      break;

   case ART1_DEL_LAY:

      switch (trgt_unit->lln) {
      case ART1_DEL_LAY:
         *weight = ART1_LINK_DEL_DEL;
         break;
      case ART1_RST_LAY:
         *weight = ART1_LINK_DEL_RST;
         break;
      case ART1_SPEC_LAY:
         *weight = ART1_LINK_DEL_CL;
         break;
      } /*switch*/
      break;

   case ART1_RST_LAY:

      switch (trgt_unit->lln) {
      case ART1_REC_LAY:
         *weight = ART1_LINK_RST_REC;
         break;
      case ART1_RST_LAY:
         *weight = ART1_LINK_RST_RST;
         break;
      case ART1_SPEC_LAY:
         *weight = ART1_LINK_RST_NCL;
         break;
      } /*switch*/
      break;

   case ART1_SPEC_LAY:

      switch (src_unit->lun) {
      case ART1_G1_UNIT:
         *weight = ART1_LINK_G1_CMP;
         break;
      case ART1_RI_UNIT:
         *weight = ART1_LINK_RI_RG;
         break;
      case ART1_RC_UNIT:
         *weight = ART1_LINK_RC_RG;
         break;
      case ART1_RG_UNIT:
         switch (trgt_unit->lln) {
         case ART1_REC_LAY:
            *weight = ART1_LINK_RG_REC;
            break;
         case ART1_RST_LAY:
            *weight = ART1_LINK_RG_RST;
            break;
         case ART1_SPEC_LAY:
            *weight = ART1_LINK_RG_CL;
            break;
         } /*switch*/
         break;
      case ART1_RHO_UNIT:
         if (trgt_unit->lln == ART1_SPEC_LAY) {
            switch (trgt_unit->lun) {
            case ART1_RI_UNIT:
               *weight = ART1_LINK_RHO_RI;
               break;
            case ART1_RHO_UNIT:
               *weight = ART1_LINK_RHO_RHO;
               break;
            } /*switch*/
         } /*if*/
         break;
      case ART1_G2_UNIT:
         switch (trgt_unit->lln) {
         case ART1_REC_LAY:
            *weight = ART1_LINK_G2_REC;
            break;
         case ART1_SPEC_LAY:
            *weight = ART1_LINK_G2_CL;
            break;
         } /*switch*/
      } /*switch*/
      break;

   } /* switch */


} /* kra1_set_fix_weights () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/* NOTE: Don't call this function unless net has been topologically sorted
         with type ART1_TOPO_TYPE
*/
static krui_err   kra1_init_fix_weights (void)
{

   int                    ret_code  = KRERR_NO_ERROR;

   register struct Unit   *unit_ptr;
   register struct Site   *site_ptr;
   register struct Link   *link_ptr;




   FOR_ALL_UNITS (unit_ptr) {

      if (UNIT_HAS_INPUTS (unit_ptr)) {

         if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               kra1_set_fix_weight (link_ptr->to, unit_ptr, &(link_ptr->weight));

            } /*FOR_ALL_LINKS*/

         } else {

            FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

               kra1_set_fix_weight (link_ptr->to, unit_ptr, &(link_ptr->weight));

            } /*FOR_ALL_SITES_AND_LINKS*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/


   return (ret_code);

} /* kra1_init_fix_weights () */
/*___________________________________________________________________________*/






/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static int  kra1_get_NoOfRecUnits (void)
{
   register struct Unit  *unit_ptr;
   int                   count           = 0;


   FOR_ALL_UNITS (unit_ptr) {

      if (IS_SPECIAL_UNIT(unit_ptr)) {
         count++;
      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (count);


} /* kra1_get_NoOfRecUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/* topo_ptr points on first position, where an input unit has to placed in
   the topo ptr array
*/

static krui_err kra1_get_InpUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit  *unit_ptr;

   krui_err              ret_code = KRERR_NO_ERROR;


   FOR_ALL_UNITS (unit_ptr) {

      if (IS_INPUT_UNIT (unit_ptr)) {

         if (!(CHECK_ACT_FUNC(unit_ptr, ART1_ACTF_INP))) {
            TOPO_MSG_ACT_FUNC (unit_ptr);
         } /*if*/

         if (!(CHECK_OUT_FUNC(unit_ptr, ART1_OUTFUNC))) {
            TOPO_MSG_OUT_FUNC (unit_ptr);
         } /*if*/

         unit_ptr->lln = ART1_INP_LAY;
         **topo_ptr = unit_ptr;
         unit_ptr->flags |= UFLAG_REFRESH;
         (*topo_ptr)++;
      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra1_get_InpUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

static krui_err   kra1_get_CmpUnits (TopoPtrArray *topo_ptr, int *no_of_cmp_units)
{
   register struct Unit   *unit_ptr;
   struct Unit            *unit_src_ptr;
   register struct Link   *link_ptr;
   struct Link            *link_src_ptr;

   bool                   is_cmp_unit;
   bool                   has_link_to_inp;
   bool                   has_links_to_other_than_inp;

   krui_err               ret_code = KRERR_NO_ERROR;


   /* look for a recognition unit */

   for (unit_ptr=unit_array+1; !IS_SPECIAL_UNIT(unit_ptr); unit_ptr++);


   /* the recognition unit is not supposed to have sites
   */

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/


   /* unit_ptr points to a recognition unit.
      We follow the incoming links of this unit. If the source unit
      we got by doing this has the following properties, it is a
      comparison unit:
         - has no sites
         - has link to input unit
         - has link to other than input unit
   */

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      unit_src_ptr = link_ptr->to;

      if (UNIT_HAS_SITES (unit_src_ptr)) {
         continue;
      } /*if*/

      is_cmp_unit                  = FALSE;
      has_link_to_inp              = FALSE;
      has_links_to_other_than_inp  = FALSE;

      FOR_ALL_LINKS (unit_src_ptr, link_src_ptr) {

         if (IS_INPUT_UNIT(link_src_ptr->to)) {
            has_link_to_inp = TRUE;
         } else {
            has_links_to_other_than_inp = TRUE;
         } /*if*/

         if (has_link_to_inp && has_links_to_other_than_inp) {
            is_cmp_unit = TRUE;
            break;
         } /*if*/

      } /*FOR_ALL_LINKS*/


      if (is_cmp_unit) {

         if (!(CHECK_ACT_FUNC (unit_src_ptr, ART1_ACTF_CMP))) {
            TOPO_MSG_ACT_FUNC (unit_src_ptr);
         } /*if*/

         if (!(CHECK_OUT_FUNC (unit_src_ptr, ART1_OUTFUNC))) {
            TOPO_MSG_OUT_FUNC (unit_src_ptr);
         } /*if*/

         if (! (UNIT_REFRESHED (unit_src_ptr))) {

            unit_src_ptr->lln = ART1_CMP_LAY;
            (*no_of_cmp_units)++;
            **topo_ptr = unit_src_ptr;
            unit_src_ptr->flags |= UFLAG_REFRESH;
            (*topo_ptr)++;

         } /*if*/

      } /*if*/

   }/*FOR_ALL_LINKS*/

   return (ret_code);

} /* kra1_get_CmpUnits () */
/*___________________________________________________________________________*/







/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kra1_get_RecUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit  *unit_ptr;

   krui_err                   ret_code = KRERR_NO_ERROR;

   FOR_ALL_UNITS (unit_ptr) {

      if (IS_SPECIAL_UNIT (unit_ptr)) {

         if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_REC))) {
            TOPO_MSG_ACT_FUNC (unit_ptr);
         } /*if*/

         if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
            TOPO_MSG_OUT_FUNC (unit_ptr);
         } /*if*/

         if (!(UNIT_REFRESHED (unit_ptr))) {
            unit_ptr->lln = ART1_REC_LAY;
            **topo_ptr = unit_ptr;
            unit_ptr->flags |= UFLAG_REFRESH;
            (*topo_ptr)++;
         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kra1_get_RecUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

static krui_err   kra1_get_DelUnits (TopoPtrArray *topo_ptr, int *no_of_del_units)
{
   register struct Unit   *unit_ptr;
   register struct Link   *link_ptr,
                          *this_link = NULL;

   int                    count;

   bool                   checked_del_rec_units   = FALSE;
   bool                   checked_first_del_unit  = FALSE;
   bool                   checked_secnd_del_unit  = FALSE;
   bool                   checked_third_del_unit  = FALSE;


   krui_err               ret_code = KRERR_NO_ERROR;


   /* first we determine the delay units that correspond to a recognition
      unit. These are units that have exactly one incoming link, coming
      from a recognition unit.
   */

   FOR_ALL_UNITS (unit_ptr) {

      count     = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {

         count++;
         this_link = link_ptr;

      } /* FOR_ALL_LINKS */

      if ((count == 1) && (this_link->to->lln == ART1_REC_LAY)) {

         if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_DEL))) {
            TOPO_MSG_ACT_FUNC (unit_ptr);
         } /*if*/

         if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
            TOPO_MSG_OUT_FUNC (unit_ptr);
         } /*if*/

         unit_ptr->lln = ART1_DEL_LAY;
         unit_ptr->lun = ART1_DEL_REC_UNIT;
         (*no_of_del_units)++;
         **topo_ptr = unit_ptr;
         (*topo_ptr)++;
         unit_ptr->flags |= UFLAG_REFRESH;

      } /*if*/

   } /* FOR_ALL_UNITS*/

   checked_del_rec_units = TRUE;


   /* Now we are looking for the first other delay unit which has got incoming
      links from all delay units we determined above
   */

   FOR_ALL_UNITS (unit_ptr) {

      if ((! (UNIT_REFRESHED (unit_ptr))) &&
          (UNIT_HAS_DIRECT_INPUTS(unit_ptr))
         )
      {
         count = 0;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            if (link_ptr->to->lln == ART1_DEL_LAY) {
               count++;
               if (count > 1) {
                  break;
               } /*if*/
            } else {
               /* break FOR_ALL_LINKS */
               break;
            } /*if*/

         } /*FOR_ALL_LINKS*/

         if (count > 1) {

            if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_D))) {
               TOPO_MSG_ACT_FUNC (unit_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_ptr);
            } /*if*/


            unit_ptr->lln = ART1_DEL_LAY;
            unit_ptr->lun = ART1_D1_UNIT;
            checked_first_del_unit = TRUE;
            (*no_of_del_units)++;
            **topo_ptr = unit_ptr;
            (*topo_ptr)++;
            unit_ptr->flags |= UFLAG_REFRESH;

            /* break FOR_ALL_UNITS, 'cause the unit we searched for, was found
            */
            break;

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/


   /* Now we look for the d2 unit
   */

   FOR_ALL_UNITS (unit_ptr) {

      if (! (UNIT_REFRESHED (unit_ptr))) {

         count = 0;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            count++;
            this_link = link_ptr;

         } /*FOR_ALL_LINKS*/

         if ((count == 1) && (this_link->to->lln == ART1_DEL_LAY) &&
             (this_link->to->lun == ART1_D1_UNIT)
            )
         {

               unit_ptr->lln = ART1_DEL_LAY;
               unit_ptr->lun = ART1_D2_UNIT;
               checked_secnd_del_unit = TRUE;
               (*no_of_del_units)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               **topo_ptr = unit_ptr;
               (*topo_ptr)++;
               break; /* terminate search for d2 unit */

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/



   /* At last  we look for the d3 unit
   */

   FOR_ALL_UNITS (unit_ptr) {

      if (! (UNIT_REFRESHED (unit_ptr))) {

         count = 0;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            count++;
            this_link = link_ptr;

         } /*FOR_ALL_LINKS*/

         if ((count == 1) && (this_link->to->lln == ART1_DEL_LAY) &&
             (this_link->to->lun == ART1_D2_UNIT)
            )
         {

               unit_ptr->lln = ART1_DEL_LAY;
               unit_ptr->lun = ART1_D3_UNIT;
               checked_third_del_unit = TRUE;
               (*no_of_del_units)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               **topo_ptr = unit_ptr;
               (*topo_ptr)++;
               break; /* terminate search for d3 unit */

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/


   if (!checked_first_del_unit) {
      TOPO_MSG_UNIT_MISSING ("d1");
   } /*if*/

   if (!checked_secnd_del_unit) {
      TOPO_MSG_UNIT_MISSING ("d2");
   } /*if*/

   if (!checked_third_del_unit) {
      TOPO_MSG_UNIT_MISSING ("d3");
   } /*if*/


   return (ret_code);

} /* kra1_get_DelUnits () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

static krui_err   kra1_get_RstUnits (TopoPtrArray *topo_ptr, int *no_of_rst_units)
{
   register struct Unit   *unit_ptr;
   register struct Site   *site_ptr;
   register struct Link   *link_ptr;

   bool                   has_link_to_itself,
                          has_link_to_del_rec_unit;

   krui_err               ret_code = KRERR_NO_ERROR;

   /* determine rst units of which each one has sites, an incoming link
      from itself , one of a delay unit that corresponds to a recognition
      unit and one from the reset general unit (which wasn't determined yet)
   */

   FOR_ALL_UNITS (unit_ptr) {

      if ((! (UNIT_REFRESHED (unit_ptr))) && (UNIT_HAS_SITES (unit_ptr))) {

         has_link_to_itself = FALSE;
         has_link_to_del_rec_unit = FALSE;

         FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

            if (link_ptr->to == unit_ptr) {
               has_link_to_itself = TRUE;
            } else {
               if ((link_ptr->to->lln == ART1_DEL_LAY) &&
                   (link_ptr->to->lun == ART1_DEL_REC_UNIT)
                  )
               {
                  has_link_to_del_rec_unit = TRUE;
               } /*if*/
            } /*if*/

         } /*FOR_ALL_SITES_AND_LINKS*/

         if (has_link_to_itself && has_link_to_del_rec_unit) {

            if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_RST))) {
                TOPO_MSG_ACT_FUNC (unit_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
                 TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

            unit_ptr->lln = ART1_RST_LAY;
            (*no_of_rst_units)++;
            **topo_ptr = unit_ptr;
            (*topo_ptr)++;
            unit_ptr->flags |= UFLAG_REFRESH;

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/


   return (ret_code);


} /* kra1_get_RstUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kra1_get_SpecUnits (TopoPtrArray *topo_ptr, int *no_of_spec_units)
{
   register struct Unit     *unit_ptr;
   register struct Site     *site_ptr;
   register struct Link     *link_ptr;

   TopoPtrArray             topo_spec_ptr = *topo_ptr;

   bool                     got_g1_unit    = FALSE;
   bool                     got_ri_unit    = FALSE;
   bool                     got_rc_unit    = FALSE;
   bool                     got_rg_unit    = FALSE;
   bool                     got_cl_unit    = FALSE;
   bool                     got_ncl_unit   = FALSE;
   bool                     got_rho_unit   = FALSE;
   bool                     got_g2_unit    = FALSE;

   bool                     links_to_rst_units,
                            links_to_del_units,
                            links_to_rec_units,
                            links_to_cmp_units,
                            links_to_rho_unit,
                            links_to_inp_units;

   krui_err                 ret_code = KRERR_NO_ERROR;




   FOR_ALL_UNITS (unit_ptr) {

      if (! (UNIT_REFRESHED (unit_ptr))) {


         if (UNIT_HAS_SITES (unit_ptr)) {

            /* This could be the g1 or the ri unit */

            /* This is the Gain 1 unit if it has links to
               a recognition unit
            */
            links_to_rec_units = FALSE;
            FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {
               if (link_ptr->to->lln == ART1_REC_LAY) {
                  links_to_rec_units = TRUE;
                  break;
               } /*if*/
            } /*FOR_ALL_SITES_AND_LINKS*/

            if (links_to_rec_units) {

               if (!got_g1_unit) {

                  if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_G1))) {
                     TOPO_MSG_ACT_FUNC (unit_ptr);
                  } /*if*/

                  if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
                      TOPO_MSG_OUT_FUNC (unit_ptr);
                  } /*if*/

                  unit_ptr->lun = ART1_G1_UNIT;
                  unit_ptr->lln = ART1_SPEC_LAY;
                  (*topo_ptr)++;
                  (*no_of_spec_units)++;
                  unit_ptr->flags |= UFLAG_REFRESH;
                  *(topo_spec_ptr + 2) = unit_ptr;
                  got_g1_unit = TRUE;
                  continue;

               } /*if*/

            } else {

               /* This should be the ri unit */
               if (!got_ri_unit) {

                  if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_RI))) {
                     TOPO_MSG_ACT_FUNC (unit_ptr);
                  } /*if*/

                  if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
                      TOPO_MSG_OUT_FUNC (unit_ptr);
                  } /*if*/

                  unit_ptr->lun = ART1_RI_UNIT;
                  unit_ptr->lln = ART1_SPEC_LAY;
                  (*topo_ptr)++;
                  (*no_of_spec_units)++;
                  unit_ptr->flags |= UFLAG_REFRESH;
                  *(topo_spec_ptr + 3) = unit_ptr;
                  got_ri_unit = TRUE;
                  continue;

               } /*if*/

            } /*if*/

         } else {

            links_to_rst_units = FALSE;
            links_to_del_units = FALSE;
            links_to_cmp_units = FALSE;
            links_to_rho_unit  = FALSE;
            links_to_inp_units = FALSE;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               switch (link_ptr->to->lln) {
               case ART1_INP_LAY:
                  links_to_inp_units = TRUE;
                  break;
               case ART1_CMP_LAY:
                  links_to_cmp_units = TRUE;
                  break;
               case ART1_DEL_LAY:
                  links_to_del_units = TRUE;
                  break;
               case ART1_RST_LAY:
                  links_to_rst_units = TRUE;
                  break;
               default:
                  if (unit_ptr == link_ptr->to) {
                     links_to_rho_unit = TRUE;
                  } /*if*/
                  break;
               } /* switch */

            } /*FOR_ALL_LINKS*/

            if ((links_to_inp_units) && (!got_g2_unit)) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_G2))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lun = ART1_G2_UNIT;
               unit_ptr->lln = ART1_SPEC_LAY;
               (*topo_ptr)++;
               (*no_of_spec_units)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               *(topo_spec_ptr + 7) = unit_ptr;
               got_g2_unit = TRUE;
               continue;
            } /*if*/

            if (links_to_rho_unit && (!got_rho_unit)) {
               if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_RHO))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lun = ART1_RHO_UNIT;
               unit_ptr->lln = ART1_SPEC_LAY;
               (*topo_ptr)++;
               (*no_of_spec_units)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               *(topo_spec_ptr + 6) = unit_ptr;
               got_rho_unit = TRUE;
               continue;
            } /*if*/

            if (links_to_cmp_units && (!got_rc_unit)) {
               if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_RC))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lun = ART1_RC_UNIT;
               unit_ptr->lln = ART1_SPEC_LAY;
               (*topo_ptr)++;
               (*no_of_spec_units)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               *(topo_spec_ptr + 4) = unit_ptr;
               got_rc_unit = TRUE;
               continue;
            } /*if*/

            if (links_to_del_units && (!got_cl_unit)) {
               if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_CL))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lun = ART1_CL_UNIT;
               unit_ptr->lln = ART1_SPEC_LAY;
               (*topo_ptr)++;
               (*no_of_spec_units)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               *topo_spec_ptr = unit_ptr;
               Art1_cl_unit = unit_ptr;
               got_cl_unit = TRUE;
               continue;
            } /*if*/

            if (links_to_rst_units && (!got_ncl_unit)) {
               if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_NCL))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lun = ART1_NCL_UNIT;
               unit_ptr->lln = ART1_SPEC_LAY;
               (*topo_ptr)++;
               (*no_of_spec_units)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               *(topo_spec_ptr+1) = unit_ptr;
               Art1_nc_unit = unit_ptr;
               got_ncl_unit = TRUE;
               continue;
            } /*if*/

            if (!got_rg_unit) {
               if (!(CHECK_ACT_FUNC (unit_ptr, ART1_ACTF_RG))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ART1_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lun = ART1_RG_UNIT;
               unit_ptr->lln = ART1_SPEC_LAY;
               (*topo_ptr)++;
               (*no_of_spec_units)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               *(topo_spec_ptr+5) = unit_ptr;
               got_rg_unit = TRUE;
               continue;
            } /*if*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   if (!got_cl_unit) {
      TOPO_MSG_UNIT_MISSING ("cl");
   } /*if*/

   if (!got_ncl_unit) {
      TOPO_MSG_UNIT_MISSING ("nc");
   } /*if*/

   if (!got_ri_unit) {
      TOPO_MSG_UNIT_MISSING ("ri");
   } /*if*/

   if (!got_rc_unit) {
      TOPO_MSG_UNIT_MISSING ("rc");
   } /*if*/

   if (!got_rg_unit) {
      TOPO_MSG_UNIT_MISSING ("rg");
   } /*if*/

   if (!got_g1_unit) {
      TOPO_MSG_UNIT_MISSING ("g1");
   } /*if*/

   if (!got_rho_unit) {
      TOPO_MSG_UNIT_MISSING ("rho");
   } /*if*/

   if (!got_g2_unit) {
      TOPO_MSG_UNIT_MISSING ("g2");
   } /*if*/

   return (ret_code);

} /* kra1_get_SpecUnits () */
/*___________________________________________________________________________*/


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kra1_TopoPtrArray (void)
{
   TopoPtrArray    topo_inp_cmp_sep,
                   topo_cmp_rec_sep,
                   topo_rec_del_sep,
                   topo_del_rst_sep,
                   topo_rst_spec_sep,
                   topo_ptr_array_end;

   int             ret_code = KRERR_NO_ERROR;


   topo_inp_cmp_sep = topo_ptr_array + NoOfInputUnits + 1;
   topo_cmp_rec_sep = topo_inp_cmp_sep + NoOfInputUnits + 1;
   topo_rec_del_sep = topo_cmp_rec_sep + Art1_NoOfRecUnits + 1;
   topo_del_rst_sep = topo_rec_del_sep + Art1_NoOfRecUnits + 4;
   topo_rst_spec_sep = topo_del_rst_sep + Art1_NoOfRecUnits + 1;
   topo_ptr_array_end = topo_rst_spec_sep + ART1_NO_OF_SPEC_UNITS + 1;

   if ((*topo_ptr_array != NULL) ||
       (*topo_inp_cmp_sep != NULL) ||
       (*topo_cmp_rec_sep != NULL) ||
       (*topo_rec_del_sep != NULL) ||
       (*topo_del_rst_sep != NULL) ||
       (*topo_rst_spec_sep != NULL) ||
       (*topo_ptr_array_end != NULL) ||
       (*(topo_ptr_array_end+1) != NULL)
      )
   {
      ART1_RETURN_NET_ERROR (ret_code);
   } /*if*/


   return (ret_code);


} /* kra1_TopoPtrArray */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kra1_Sites (void)
{
   register struct Unit   *unit_ptr;
   register struct Site   *site_ptr;

   TopoPtrArray           topo_rst_ptr,
                          topo_g1_ptr,
                          topo_ri_ptr;

   bool                   got_site1,
                          got_site2;

   krui_err               ret_code = KRERR_NO_ERROR;



   topo_rst_ptr = topo_ptr_array + 2*NoOfInputUnits + 2*Art1_NoOfRecUnits + 8;
   topo_g1_ptr  = topo_rst_ptr + Art1_NoOfRecUnits + 3;
   topo_ri_ptr  = topo_g1_ptr + 1;

   /* check sites of local reset units
   */
   while ((unit_ptr = *topo_rst_ptr++) != NULL) {

      got_site1 = FALSE;
      got_site2 = FALSE;

      if (UNIT_HAS_SITES (unit_ptr)) {

         FOR_ALL_SITES (unit_ptr, site_ptr) {

            if ((CHECK_SITE_FUNC (site_ptr, ART1_SITEF_RST_SELF)) && (!got_site1)) {
               got_site1 = TRUE;
               continue;
            } /*if*/

            if ((CHECK_SITE_FUNC (site_ptr, ART1_SITEF_RST_SIGNAL)) && (!got_site2)) {
               got_site2 = TRUE;
               continue;
            } /*if*/

            TOPO_MSG_SITE_FUNC (unit_ptr);

         } /*FOR_ALL_SITES*/


         if (!got_site1 || !got_site2) {
            TOPO_MSG_SITE_MISSING  (unit_ptr);
         } /*if*/


      } else {

         TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (unit_ptr);

      } /*if*/

   } /*while*/



   /* Check sites of unit Gain 1 */

   if (UNIT_HAS_DIRECT_INPUTS (*topo_g1_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (*topo_g1_ptr);
   } /*if*/

   got_site1 = FALSE;
   got_site2 = FALSE;

   FOR_ALL_SITES (*topo_g1_ptr, site_ptr) {

      if ((CHECK_SITE_FUNC (site_ptr, ART1_SITEF_INP_G1)) && (!got_site1)) {
         got_site1 = TRUE;
         continue;
      } /*if*/

      if ((CHECK_SITE_FUNC (site_ptr, ART1_SITEF_REC_G1)) && (!got_site2)) {
         got_site2 = TRUE;
         continue;
      } /*if*/

      TOPO_MSG_SITE_FUNC (*topo_g1_ptr);

   } /*FOR_ALL_SITES*/

   if (!got_site1 || !got_site2) {
      TOPO_MSG_SITE_MISSING (*topo_g1_ptr);
   } /*if*/


   /* Check sites of unit RI */

   if (UNIT_HAS_DIRECT_INPUTS (*topo_ri_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (*topo_ri_ptr);
   } /*if*/

   got_site1 = FALSE;
   got_site2 = FALSE;

   FOR_ALL_SITES (*topo_ri_ptr, site_ptr) {

      if (CHECK_SITE_FUNC (site_ptr, ART1_SITEF_RI)) {
         if (!got_site1) {
            got_site1 = TRUE;
         } else {
            if (!got_site2) {
               got_site2 = TRUE;
            } else {
               TOPO_MSG_SITE_FUNC (*topo_ri_ptr);
            } /*if*/
         } /*if*/
      } /*if*/

   } /*FOR_ALL_SITES*/

   if (!got_site1 || !got_site2) {
      TOPO_MSG_SITE_MISSING (*topo_ri_ptr);
   } /*if*/

   return (ret_code);

} /* kra1_Sites () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kra1_LinksToInpUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit   *unit_ptr;

   krui_err               ret_code = KRERR_NO_ERROR;

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      if (UNIT_HAS_INPUTS (unit_ptr)) {
         topo_msg.error_code = KRERR_I_UNITS_CONNECT;
         topo_msg.dest_error_unit = unit_ptr-unit_array;
         topo_msg.src_error_unit = 0;
         ret_code = topo_msg.error_code;
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra1_LinksToInpUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kra1_LinksToCmpUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit     *unit_ptr;
   register struct Link     *link_ptr;

   int                      count_inp,
                            count_spec,
                            count_del;


   krui_err                 ret_code = KRERR_NO_ERROR;


   /* - each comparison unit has a link to exactly one input unit which itself
        is connected to only this comparison unit and no other.
      - each comparison unit is linked to the gain 1 unit
      - each comparison unit is linked to each delay unit that corresponds to
        a recognition unit
   */

   krart_deleteTouchFlags();


   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      if (UNIT_HAS_SITES (unit_ptr)) {

         TOPO_MSG_UNEXPECTED_SITES (unit_ptr);

      } else {

         count_inp = 0;
         count_spec = 0;
         count_del = 0;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            switch (link_ptr->to->lln) {
            case ART1_DEL_LAY:
               if (link_ptr->to->lun == ART1_DEL_REC_UNIT) {
                  count_del++;
               } else {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               break;
            case ART1_INP_LAY:
               if ((UNIT_REFRESHED (link_ptr->to)) || (count_inp > 0)) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               link_ptr->to->flags |= UFLAG_REFRESH;
               count_inp++;
               break;
            case ART1_SPEC_LAY:
               if (link_ptr->to->lun != ART1_G1_UNIT) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               count_spec++;
               break;
            default :
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /* switch */

         } /*FOR_ALL_LINKS*/

         if ((count_inp != 1) || (count_spec != 1) ||
             (count_del != Art1_NoOfRecUnits)
            )
         {
            TOPO_MSG_LINK_MISSING (unit_ptr);
         } /*if*/

      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra1_LinksToCmpUnits () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kra1_LinksToRecUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit     *unit_ptr;
   register struct Link     *link_ptr;

   int                      count_cmp;
   int                      count_rst;
   int                      count_rg;
   int                      count_g2;

   krui_err                 ret_code = KRERR_NO_ERROR;


   /* - each recognition unit is linked to all comparison units
      - each recognition unit is linked to the reset general unit
      - each recognition unit is linked to exactly one local reset unit which
        itself is linked to only this recognition unit.
   */

   krart_deleteTouchFlags();


   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      if (UNIT_HAS_SITES (unit_ptr)) {

         TOPO_MSG_UNEXPECTED_SITES (unit_ptr);

      } else {

         count_cmp = 0;
         count_rst = 0;
         count_rg  = 0;
         count_g2  = 0;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            switch (link_ptr->to->lln) {
            case ART1_CMP_LAY:
               count_cmp++;
               break;
            case ART1_RST_LAY:
               if ((UNIT_REFRESHED (link_ptr->to)) || (count_rst > 0)) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               link_ptr->to->flags |= UFLAG_REFRESH;
               count_rst++;
               break;
            case ART1_SPEC_LAY:
               switch (link_ptr->to->lun) {
               case ART1_RG_UNIT:
                  count_rg++;
                  break;
               case ART1_G2_UNIT:
                  count_g2++;
                  break;
               default:
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
                  break;
               } /*switch*/
               break;
            default :
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /* switch */

         } /*FOR_ALL_LINKS*/

         if ((count_rst != 1) || (count_rg != 1) || (count_g2 != 1) ||
             (count_cmp != NoOfInputUnits)
            )
         {
            TOPO_MSG_LINK_MISSING  (unit_ptr);
         } /*if*/

      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra1_LinksToRecUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kra1_LinksToDelUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit     *unit_ptr;
   register struct Link     *link_ptr;

   int                      count_rec;
   int                      count_del;

   krui_err                 ret_code = KRERR_NO_ERROR;


   /* There are different cases to distinguish
      1. the delay unit is one that corresponds to a recogniton unit
         then
              - it has only one incoming link which comes from the corresponding
                recognition unit

      2. it is the first other delay unit then
              - it has links to all delay units that correspond to a
                recognition unit
      3. it is the second other delay unit then
              - it has exactly one link to the first other delay unit
      4. it is the third other delay unit then
              - it has exactly one link to the second other delay unit.
   */

   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      if (UNIT_HAS_SITES (unit_ptr)) {

         TOPO_MSG_UNEXPECTED_SITES (unit_ptr);

      } else {

         switch (unit_ptr->lun) {

         case ART1_DEL_REC_UNIT:

            count_rec = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if (link_ptr->to->lln == ART1_REC_LAY) {

                  if ((UNIT_REFRESHED (link_ptr->to)) || (count_rec > 0)) {
                     TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
                  } else {
                     link_ptr->to->flags |= UFLAG_REFRESH;
                  } /*if*/
                  count_rec++;

               } else {

                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);

               } /*if*/

            } /*FOR_ALL_LINKS*/

            if (count_rec != 1) {
               TOPO_MSG_LINK_MISSING (unit_ptr);
            } /*if*/

            break;

         case ART1_D1_UNIT:

            count_del = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ART1_DEL_LAY) &&
                   (link_ptr->to->lun == ART1_DEL_REC_UNIT)
                  )
               {
                  count_del++;
               } else {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/

            } /*FOR_ALL_LINKS*/

            if (count_del != Art1_NoOfRecUnits) {
               TOPO_MSG_LINK_MISSING (unit_ptr);
            } /*if*/

            break;

         case ART1_D2_UNIT:

            count_del = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ART1_DEL_LAY) &&
                   (link_ptr->to->lun == ART1_D1_UNIT)
                  )
               {
                  count_del++;
               } else {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/

            } /*FOR_ALL_LINKS*/

            if (count_del != 1) {
               TOPO_MSG_LINK_MISSING (unit_ptr);
            } /*if*/

            break;

         case ART1_D3_UNIT:

            count_del = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ART1_DEL_LAY) &&
                   (link_ptr->to->lun == ART1_D2_UNIT)
                  )
               {
                  count_del++;
               } else {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/

            } /*FOR_ALL_LINKS*/

            if (count_del != 1) {
               TOPO_MSG_LINK_MISSING (unit_ptr);
            } /*if*/

            break;

         default :
            ART1_RETURN_NET_ERROR (ret_code);
         } /* switch */

      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra1_LinksToDelUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kra1_LinksToRstUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit     *unit_ptr;
   register struct Link     *link_ptr;

   struct Site              *site_ptr,
                            *site_ptr1,
                            *site_ptr2;



   int                      count_rst;
   int                      count_spec;
   int                      count_del;

   krui_err                 ret_code = KRERR_NO_ERROR;


   /* - each local reset unit is linked to via site 1
      - each local reset unit is linked to the reset general unit via site 2
      - each local reset unit is linked to exactly one delay unit that
        corresponds to a recognition unit and which itself is linked
        to only this local reset unit, via site 2.
   */

   krart_deleteTouchFlags();


   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {

         TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (unit_ptr);

      } else {

         count_rst = 0;
         count_spec = 0;
         count_del = 0;

         site_ptr1 = NULL;
         site_ptr2 = NULL;

         FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

            switch (link_ptr->to->lln) {
            case ART1_RST_LAY:

               count_rst++;

               if (link_ptr->to != unit_ptr) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/

               site_ptr1 = site_ptr;

               if (site_ptr1 == site_ptr2) {
                  TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
               } /*if*/

               break;

            case ART1_DEL_LAY:

               if (UNIT_REFRESHED (link_ptr->to)) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/

               link_ptr->to->flags |= UFLAG_REFRESH;

               count_del++;

               if ((site_ptr2 != NULL) && (site_ptr2 != site_ptr)) {
                  TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
               } /*if*/

               site_ptr2 = site_ptr;

               if (site_ptr1 == site_ptr2) {
                  TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
               } /*if*/

               break;

            case ART1_SPEC_LAY:

               if (link_ptr->to->lun != ART1_RG_UNIT) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/

               count_spec++;

               if ((site_ptr2 != NULL) && (site_ptr2 != site_ptr)) {
                  TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
               } /*if*/

               site_ptr2 = site_ptr;

               if (site_ptr1 == site_ptr2) {
                  TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
               } /*if*/

               break;

            default :
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /* switch */

         } /*FOR_ALL_SITES_AND_LINKS*/

         if ((count_rst != 1) || (count_spec != 1) || (count_del != 1)) {
            TOPO_MSG_LINK_MISSING (unit_ptr);
         } /*if*/

      } /*if*/

   } /*while*/

   return (ret_code);

} /* kra1_LinksToRstUnits () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kra1_LinksToSpecUnits (TopoPtrArray *topo_ptr)
{
   register struct Unit     *unit_ptr;
   register struct Link     *link_ptr;

   struct Site              *site_ptr,
                            *site_ptr1,
                            *site_ptr2;



   int                      count_rst;
   int                      count_del;
   int                      count_inp;
   int                      count_rec;
   int                      count_cmp;
   int                      count_ri;
   int                      count_rc;
   int                      count_rg;
   int                      count_rho;
   int                      count_g2;

   krui_err                 ret_code = KRERR_NO_ERROR;


   /* topo_ptr points to classified unit which has three links:
      1. from third other delay unit
      2. from reset general unit
      3. from gain 2 unit
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   count_del = 0;
   count_rg  = 0;
   count_g2  = 0;

   site_ptr1 = NULL;
   site_ptr2 = NULL;

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      switch (link_ptr->to->lln) {
      case ART1_DEL_LAY:

         count_del++;

         if (link_ptr->to->lun != ART1_D3_UNIT) {
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ART1_SPEC_LAY:

         switch (link_ptr->to->lun) {
         case ART1_RG_UNIT:
            count_rg++;
            break;
         case ART1_G2_UNIT:
            count_g2++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/

         break;

      default :
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         break;
      } /* switch */

   } /*FOR_ALL_LINKS*/

   if ((count_del != 1) || (count_rg != 1) || (count_g2 != 1)) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/


   /* topo_ptr points to not classifiable unit which is linked to all
      local reset units
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   count_rst = 0;

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      if (link_ptr->to->lln != ART1_RST_LAY) {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

      count_rst++;

   } /*FOR_ALL_LINKS*/

   if (count_rst != Art1_NoOfRecUnits) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/


   /* topo_ptr points to Gain 1 unit which is linked to all input units
      via site 1 and to all recognition units via site 2
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (unit_ptr);
   } /*if*/

   count_inp = 0;
   count_rec = 0;

   site_ptr1 = NULL;
   site_ptr2 = NULL;

   FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

      switch (link_ptr->to->lln) {

      case ART1_REC_LAY :

         count_rec++;

         if ((site_ptr2 != NULL) && (site_ptr2 != site_ptr)) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         site_ptr2 = site_ptr;

         if (site_ptr1 == site_ptr2) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ART1_INP_LAY :

         count_inp++;

         if ((site_ptr1 != NULL) && (site_ptr1 != site_ptr)) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         site_ptr1 = site_ptr;

         if (site_ptr1 == site_ptr2) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      default :
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);

      } /*switch*/

   } /*FOR_ALL_SITES_AND_LINKS*/

   if ((count_inp != NoOfInputUnits) || (count_rec != Art1_NoOfRecUnits)) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/



   /* topo_ptr points to reset I unit which is linked to all input units
      via one site and to the RHO unit via the other site
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (unit_ptr);
   } /*if*/

   count_inp = 0;
   count_rho = 0;

   site_ptr1 = NULL;
   site_ptr2 = NULL;

   FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

      switch (link_ptr->to->lln) {

      case ART1_INP_LAY:

         count_inp++;

         if ((site_ptr1 != NULL) && (site_ptr1 != site_ptr)) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         site_ptr1 = site_ptr;

         if (site_ptr1 == site_ptr2) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ART1_SPEC_LAY:

         if (link_ptr->to->lun != ART1_RHO_UNIT) {
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         } /*if*/

         count_rho++;

         if ((site_ptr2 != NULL) && (site_ptr2 != site_ptr)) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         site_ptr2 = site_ptr;

         if (site_ptr2 == site_ptr1) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      default:
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);

      } /*switch*/

   } /*FOR_ALL_LINKS*/

   if ((count_inp != NoOfInputUnits) || (count_rho != 1)) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/



   /* topo_ptr points to reset C unit which is linked to all comparison units
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   count_cmp = 0;

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      if (link_ptr->to->lln != ART1_CMP_LAY) {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

      count_cmp++;

   } /*FOR_ALL_LINKS*/

   if (count_cmp != NoOfInputUnits) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/


   /* topo_ptr points to reset general unit which is linked to
      the reset I unit and the reset C unit
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   count_ri = 0;
   count_rc = 0;

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      if (link_ptr->to->lln != ART1_SPEC_LAY) {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

      if (link_ptr->to->lun == ART1_RI_UNIT) {
         count_ri++;
         continue;
      } /*if*/

      if (link_ptr->to->lun == ART1_RC_UNIT) {
         count_rc++;
         continue;
      } /*if*/

      TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);

   } /*FOR_ALL_LINKS*/


   if ((count_ri != 1) || (count_rc != 1)) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/



   /* topo_ptr points to RHO unit which has an incoming link
      from itself
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   count_rho = 0;

   FOR_ALL_LINKS (unit_ptr, link_ptr) {
      if ((link_ptr->to->lln == ART1_SPEC_LAY) &&
          (link_ptr->to->lun == ART1_RHO_UNIT)
         )
      {
         count_rho++;
      } else {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      }  /*if*/
   } /*FOR_ALL_LINKS*/

   if (count_rho != 1) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/


   /* topo_ptr points to G2 unit which has incoming links
      from all input units
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   count_inp = 0;

   FOR_ALL_LINKS (unit_ptr, link_ptr) {
      if (link_ptr->to->lln == ART1_INP_LAY) {
         count_inp++;
      } else {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      }  /*if*/
   } /*FOR_ALL_LINKS*/

   if (count_inp != NoOfInputUnits) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/

   return (ret_code);

} /* kra1_LinksToSpecUnits () */
/*___________________________________________________________________________*/

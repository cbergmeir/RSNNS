/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/kr_amap.c,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel Functions for ARTMAP networks
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.8 $
  LAST CHANGE    : $Date: 1998/03/03 14:08:33 $

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
#include "kr_amap.ph"



/*#################################################

GROUP: ARTMAP kernel functions
       by Kai-Uwe Herrmann

#################################################*/

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
krui_err kram_init_i_act (double rhoa, double rhob, double rho)
{
   int                    ret_code = KRERR_NO_ERROR;

   register struct Unit   *unit_ptr;


   FOR_ALL_UNITS (unit_ptr) {

      switch (unit_ptr->lln) {

      case ARTMAP_SPECa_LAY:
         switch (unit_ptr->lun) {
         case ARTMAP_G1a_UNIT :
            unit_ptr->i_act = 0.0;
            break;
         case ARTMAP_RIa_UNIT :
            unit_ptr->i_act = 1.0;
            break;
         case ARTMAP_RGa_UNIT :
            unit_ptr->i_act = 1.0;
            break;
         case ARTMAP_RHOa_UNIT :
            unit_ptr->i_act = (FlintType) rhoa;
            break;
         default :
            unit_ptr->i_act = 0.0;
            break;
         } /*switch*/
         break;

      case ARTMAP_SPECb_LAY:
         switch (unit_ptr->lun) {
         case ARTMAP_G1b_UNIT :
            unit_ptr->i_act = 0.0;
            break;
         case ARTMAP_RIb_UNIT :
            unit_ptr->i_act = 1.0;
            break;
         case ARTMAP_RGb_UNIT :
            unit_ptr->i_act = 1.0;
            break;
         case ARTMAP_RHOb_UNIT :
            unit_ptr->i_act = (FlintType) rhob;
            break;
         default :
            unit_ptr->i_act = 0.0;
            break;
         } /*switch*/
         break;

      case ARTMAP_SPEC_LAY:
         switch (unit_ptr->lun) {
         case ARTMAP_G_UNIT:
            unit_ptr->i_act = 1.0;
            break;
         case ARTMAP_RHO_UNIT:
            unit_ptr->i_act = (FlintType) rho;
            break;
         default:
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

} /* kram_init_i_act () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
krui_err  kram_sort (void)
{
   krui_err             ret_code    = KRERR_NO_ERROR;

   TopoPtrArray         topo_ptr = topo_ptr_array;

   int                  no_of_cmpa_units  = 0;
   int                  no_of_dela_units  = 0;
   int                  no_of_rsta_units  = 0;
   int                  no_of_speca_units = 0;
   int                  no_of_cmpb_units  = 0;
   int                  no_of_delb_units  = 0;
   int                  no_of_rstb_units  = 0;
   int                  no_of_specb_units = 0;
   int                  no_of_map_units   = 0;
   int                  no_of_spec_units  = 0;



   /* initialize ARTMAP sorting
   */
   krart_init_sorting ();

   /* get no of input units
   */
   NoOfInputUnits = krart_get_NoOfInputUnits ();

   if (NoOfInputUnits == 0) {
      ret_code = KRERR_NO_INPUT_UNITS;
      return (ret_code);
   } /*if*/

   /* insert the initial NULL-ptr into the topo ptr array
   */
   *topo_ptr++ = NULL;

   /**********  determine unit types for ARTa and insert them into topo ptr array
   **********/

   /* determine input units of ARTa
   */
   ret_code = kram_get_InpUnits_a (&topo_ptr, &ArtMap_NoOfInpUnits_a);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (ArtMap_NoOfInpUnits_a == 0) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTa input");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine comparison units of ARTa
   */
   ret_code = kram_get_CmpUnits_a (&topo_ptr, &no_of_cmpa_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_cmpa_units != ArtMap_NoOfInpUnits_a) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTa comparison");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine recognition units of ARTa
   */
   ret_code = kram_get_RecUnits_a (&topo_ptr, &ArtMap_NoOfRecUnits_a);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (ArtMap_NoOfRecUnits_a == 0) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTa recognition");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine delay units of ARTa
   */
   ret_code = kram_get_DelUnits_a (&topo_ptr, &no_of_dela_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_dela_units != ArtMap_NoOfRecUnits_a + 3) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTa delay");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine local reset units of ARTa
   */
   ret_code = kram_get_RstUnits_a (&topo_ptr, &no_of_rsta_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_rsta_units != ArtMap_NoOfRecUnits_a) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTa reset");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine special units of ARTa
   */
   ret_code = kram_get_SpecUnits_a (&topo_ptr, &no_of_speca_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_speca_units != ARTMAP_NO_OF_SPECa_UNITS) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTa special");
   } /*if*/

   *topo_ptr++ = NULL;


   /**********  determine unit types for ARTb  and insert them into topo ptr array
   **********/

   /* determine input units of ARTb
   */
   ret_code = kram_get_InpUnits_b (&topo_ptr, &ArtMap_NoOfInpUnits_b);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (ArtMap_NoOfInpUnits_b == 0) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTb input");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine comparison units of ARTb
   */
   ret_code = kram_get_CmpUnits_b (&topo_ptr, &no_of_cmpb_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_cmpb_units != ArtMap_NoOfInpUnits_b) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTb comparison");
   } /*if*/

   *topo_ptr++ = NULL;



   /* determine recognition units of ARTb
   */
   ret_code = kram_get_RecUnits_b (&topo_ptr, &ArtMap_NoOfRecUnits_b);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (ArtMap_NoOfRecUnits_b == 0) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTb recognition");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine delay units of ARTb
   */
   ret_code = kram_get_DelUnits_b (&topo_ptr, &no_of_delb_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_delb_units != ArtMap_NoOfRecUnits_b + 3) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTb delay");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine local reset units of ARTb
   */
   ret_code = kram_get_RstUnits_b (&topo_ptr, &no_of_rstb_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_rstb_units != ArtMap_NoOfRecUnits_b) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTb reset");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine special units of ARTb
   */
   ret_code = kram_get_SpecUnits_b (&topo_ptr, &no_of_specb_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_specb_units != ARTMAP_NO_OF_SPECb_UNITS) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("ARTb special");
   } /*if*/

   *topo_ptr++ = NULL;



   /**********  determine unit types for MAP-Field and insert them into topo ptr array
   **********/

   /* determine map units
   */
   ArtMap_map_layer = topo_ptr;
   ret_code = kram_get_MapUnits (&topo_ptr,&no_of_map_units);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_map_units != ArtMap_NoOfRecUnits_b) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("map");
   } /*if*/

   *topo_ptr++ = NULL;


   /* determine special units
   */
   ret_code = kram_get_SpecUnits (&topo_ptr, &no_of_spec_units);;

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/

   if (no_of_spec_units != 10) {
      TOPO_MSG_NO_OF_UNITS_IN_LAYER ("Map Field special");
   } /*if*/

   *topo_ptr++ = NULL;
   *topo_ptr   = NULL;  /* set second NULL pointer at the end */







   /* check if the logical type of really all units is determined
   */
   if (krart_check_undeterminedUnits ()) {
      ret_code = topo_msg.error_code;
      return (ret_code);
   } /*if*/


   /* Now check the topo ptr array
   */
   ret_code = kram_TopoPtrArray ();

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check the sites
   */

   ret_code = kram_Sites ();

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check link structure
   */

   topo_ptr = topo_ptr_array + 1;

   /* Check links of inpa units
   */
   ret_code = kram_LinksToInpUnits_a (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of cmpa units
   */
   ret_code = kram_LinksToCmpUnits_a (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of reca units
   */
   ret_code = kram_LinksToRecUnits_a (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of dela units and d1a, d2a, d3a
   */
   ret_code = kram_LinksToDelUnits_a (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of rsta units
   */
   ret_code = kram_LinksToRstUnits_a (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of special units of ARTa
   */
   ret_code = kram_LinksToSpecUnits_a (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of inpb units
   */
   ret_code = kram_LinksToInpUnits_b (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of cmpb units
   */
   ret_code = kram_LinksToCmpUnits_b (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of recb units
   */
   ret_code = kram_LinksToRecUnits_b (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of delb units and d1b, d2b, d3b
   */
   ret_code = kram_LinksToDelUnits_b (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of rstb units
   */
   ret_code = kram_LinksToRstUnits_b (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of special units of ARTb
   */
   ret_code = kram_LinksToSpecUnits_b (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of map units
   */
   ret_code = kram_LinksToMapUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/


   /* Check links of special units of MAPFIELD
   */
   ret_code = kram_LinksToSpecUnits (&topo_ptr);

   if (ret_code != KRERR_NO_ERROR) {
      return (ret_code);
   } /*if*/




   /* initialize the fix weights of ARTMAP
   */

   ret_code = kram_init_fix_weights ();

   return (ret_code);

} /* kram_sort () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
int   kram_getClassNo (void)
{
    TopoPtrArray   topo_ptr = ArtMap_map_layer;
    int            i;

    /* if ARTMAP sorting wasn't performed then return negative value
       to indicate mistake
    */
    if (topo_ptr == NULL) {
       return (-1);
    } /*if*/

    /* look for winning unit */
    for (i = 1; (i <= ArtMap_NoOfRecUnits_b) || ((*topo_ptr)->act >= 0.9);
         i++, topo_ptr++
        );

    if ((i > ArtMap_NoOfRecUnits_b) && ((*topo_ptr)->act < 0.9)) {
       return (-1);
    } else {
       return (topo_ptr - ArtMap_map_layer + 1);
    } /*if*/

} /* kram_getClassNo () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
bool  kram_AllMapUnitsActive (void)
{
    TopoPtrArray   topo_ptr = ArtMap_map_layer;

    if (topo_ptr == NULL) {
       return (FALSE);
    } /*if*/

    while (*topo_ptr != NULL) {
       if ((*topo_ptr)->act < 0.9) {
          return (FALSE);
       } /*if*/
    } /*while*/

    return (TRUE);

} /* kram_AllMapUnitsActive () */
/*___________________________________________________________________________*/



/************* functions to determine units of ARTa
*************/


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_InpUnits_a (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                   ret_code = KRERR_NO_ERROR;

   register struct Unit       *unit_src_ptr, *unit_trg_ptr;
   register struct Site       *site_trg_ptr;
   register struct Link       *link_trg_ptr;

   int                        count_links;

   /* for the inpa units it is characteristical that they have got 5
      links to other units while the ones of ARTb just have 4
   */

   *no_of_units = 0;
   FOR_ALL_UNITS (unit_src_ptr) {

      if (IS_INPUT_UNIT (unit_src_ptr)) {

         if (UNIT_HAS_INPUTS (unit_src_ptr)) {
            topo_msg.error_code = KRERR_I_UNITS_CONNECT;
            topo_msg.dest_error_unit = unit_src_ptr-unit_array;
            topo_msg.src_error_unit = 0;
            return (topo_msg.error_code);
         } /*if*/

         count_links = 0;

         FOR_ALL_UNITS (unit_trg_ptr) {

            if (UNIT_HAS_SITES (unit_trg_ptr)) {
               FOR_ALL_SITES_AND_LINKS (unit_trg_ptr, site_trg_ptr, link_trg_ptr) {
                  if (link_trg_ptr->to == unit_src_ptr) {
                     count_links++;
                  } /*if*/
               } /*FOR_ALL_SITES_AND_LINKS*/
            } else {
               FOR_ALL_LINKS (unit_trg_ptr, link_trg_ptr) {
                  if (link_trg_ptr->to == unit_src_ptr) {
                     count_links++;
                  } /*if*/
               } /*FOR_ALL_LINKS*/
            } /*if*/

         } /*FOR_ALL_UNITS*/

         if (count_links == 5) {

            if (!(CHECK_ACT_FUNC (unit_src_ptr, ARTMAP_ACTF_INPa))) {
               TOPO_MSG_ACT_FUNC (unit_src_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_src_ptr, ARTMAP_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_src_ptr);
            } /*if*/

            (*no_of_units)++;
            **topo_ptr = unit_src_ptr;
            unit_src_ptr->lln = ARTMAP_INPa_LAY;
            (*topo_ptr)++;
            unit_src_ptr->flags |= UFLAG_REFRESH;
         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kram_get_InpUnits_a () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_CmpUnits_a (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                 ret_code  = KRERR_NO_ERROR;

   register struct Unit     *unit_ptr;
   struct Unit              *unit_src_ptr;
   register struct Link     *link_ptr;
   struct Link              *link_src_ptr;

   bool                     is_cmpa_unit;
   bool                     has_link_to_inpa;
   bool                     has_link_to_other_than_inpa;


   *no_of_units = 0;

   /* look for a reca unit */

   FOR_ALL_UNITS (unit_ptr) {

      if (!IS_SPECIAL_UNIT (unit_ptr)) {
         /* we can continue the search, 'cause reca units have got
            ttype special
         */
         continue;
      } /*if*/

      /* the recognition unit is not supposed to have sites
      */
      if (UNIT_HAS_SITES (unit_ptr)) {
         TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
      } /*if*/


      /* unit_ptr points to a recogniton unit.
         we follow the incoming links of this unit. if the source unit we
         got by doing this has the following properties, this is a comparison
         unit of ARTa
                - has no sites
                - has incoming link from inpa unit
                - has incoming link from other than inpa unit
      */

      FOR_ALL_LINKS (unit_ptr, link_ptr) {

         unit_src_ptr = link_ptr->to;

         is_cmpa_unit                 = FALSE;
         has_link_to_inpa             = FALSE;
         has_link_to_other_than_inpa  = FALSE;

         if (UNIT_HAS_DIRECT_INPUTS (unit_src_ptr)) {

            FOR_ALL_LINKS (unit_src_ptr, link_src_ptr) {

               if (link_src_ptr->to->lln == ARTMAP_INPa_LAY) {
                  has_link_to_inpa = TRUE;
               } else {
                  has_link_to_other_than_inpa = TRUE;
               } /*if*/

               if (has_link_to_inpa && has_link_to_other_than_inpa) {
                  is_cmpa_unit = TRUE;
                  break;
               } /*if*/

            } /*FOR_ALL_LINKS*/

         } /*if*/

         if (is_cmpa_unit) {

            if (!(CHECK_ACT_FUNC (unit_src_ptr, ARTMAP_ACTF_CMPa))) {
               TOPO_MSG_ACT_FUNC (unit_src_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_src_ptr, ARTMAP_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_src_ptr);
            } /*if*/

            unit_src_ptr->lln = ARTMAP_CMPa_LAY;
            (*no_of_units)++;
            **topo_ptr = unit_src_ptr;
            unit_src_ptr->flags |= UFLAG_REFRESH;
            (*topo_ptr)++;

         } /*if*/

      } /*FOR_ALL_LINKS*/

      if (*no_of_units > 0) {
         /* stop searching for reca unit */
         break;
      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kram_get_CmpUnits_a () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_RecUnits_a (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                   ret_code   = KRERR_NO_ERROR;

   register struct Unit       *unit_ptr;
   register struct Link       *link_ptr;

   bool                       is_reca_unit;


   *no_of_units = 0;

   /* properties of reca units:
          - has no sites
          - has incoming links from cmpa units
   */

   FOR_ALL_UNITS (unit_ptr) {

      if (IS_SPECIAL_UNIT (unit_ptr)) {

         if (UNIT_HAS_SITES (unit_ptr)) {
            TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
         } /*if*/

         is_reca_unit = FALSE;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            if (link_ptr->to->lln == ARTMAP_CMPa_LAY) {
               is_reca_unit = TRUE;
               break;
            } /*if*/

         } /*FOR_ALL_LINKS*/

         if (is_reca_unit) {

            if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RECa))) {
               TOPO_MSG_ACT_FUNC (unit_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_ptr);
            } /*if*/

            unit_ptr->lln = ARTMAP_RECa_LAY;
            **topo_ptr = unit_ptr;
            (*no_of_units)++;
            (*topo_ptr)++;
            unit_ptr->flags |= UFLAG_REFRESH;

         } /*if*/

      } /*if*/

    } /*FOR_ALL_UNITS*/

    return (ret_code);

} /* kram_get_RecUnits_a */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_DelUnits_a (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                 ret_code      = KRERR_NO_ERROR;

   register struct Unit     *unit_ptr;
   register struct Link     *link_ptr;
   struct Link              *this_link = NULL;

   int                      count_links;
   int                      no_of_dela_units = 0;
   bool                     got_d1a_unit     = FALSE;
   bool                     got_d2a_unit     = FALSE;
   bool                     got_d3a_unit     = FALSE;


   *no_of_units = 0;

   /* the dela units each have got just one incoming link from their
      corresponding reca unit. They don't have sites.
   */

   FOR_ALL_UNITS (unit_ptr) {

      if ((! (UNIT_REFRESHED (unit_ptr))) && (UNIT_HAS_DIRECT_INPUTS (unit_ptr))) {

         count_links = 0;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            count_links++;
            this_link = link_ptr;

         } /*FOR_ALL_LINKS*/

         if ((count_links == 1) && (this_link->to->lln == ARTMAP_RECa_LAY)) {

            if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_DELa))) {
               TOPO_MSG_ACT_FUNC (unit_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_ptr);
            } /*if*/

            unit_ptr->lln = ARTMAP_DELa_LAY;
            unit_ptr->lun = ARTMAP_DELa_REC_UNIT;
            no_of_dela_units++;
            **topo_ptr = unit_ptr;
            (*topo_ptr)++;
            unit_ptr->flags |= UFLAG_REFRESH;

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/


   /* now we are looking for d1a which has to have links to all dela units
   */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr)) && (UNIT_HAS_DIRECT_INPUTS (unit_ptr))) {

         count_links = 0;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            if ((link_ptr->to->lln == ARTMAP_DELa_LAY) &&
                (link_ptr->to->lun == ARTMAP_DELa_REC_UNIT))
            {
               count_links ++;
            } /*if*/

         } /*FOR_ALL_LINKS*/

         if (count_links == no_of_dela_units) {

            if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_Da))) {
               TOPO_MSG_ACT_FUNC (unit_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_ptr);
            } /*if*/

            unit_ptr->lln = ARTMAP_DELa_LAY;
            unit_ptr->lun = ARTMAP_D1a_UNIT;
            **topo_ptr = unit_ptr;
            (*topo_ptr)++;
            got_d1a_unit = TRUE;
            unit_ptr->flags |= UFLAG_REFRESH;

            /* Now we can break the search, 'cause we got the d1a unit */
            break;

         } /*if*/

      } /*FOR_ALL_LINKS*/

   } /*FOR_ALL_UNITS*/

   if (!got_d1a_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: d1 (=d1a)");
   } /*if*/


   /* Now we are looking for the d2a unit */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr)) && (UNIT_HAS_DIRECT_INPUTS (unit_ptr))) {

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            if ((link_ptr->to->lln == ARTMAP_DELa_LAY) &&
                (link_ptr->to->lun == ARTMAP_D1a_UNIT)
               )
            {
               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_Da))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_DELa_LAY;
               unit_ptr->lun = ARTMAP_D2a_UNIT;
               **topo_ptr = unit_ptr;
               (*topo_ptr)++;
               got_d2a_unit = TRUE;
               unit_ptr->flags |= UFLAG_REFRESH;

               /* we can break the search for the d2a unit */
               break;
            } /*if*/

         } /*FOR_ALL_LINKS*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   if (!got_d2a_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: d2 (=d2a)");
   } /*if*/


   /* Now we are looking for the d3a unit */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr)) && (UNIT_HAS_DIRECT_INPUTS (unit_ptr))) {

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            if ((link_ptr->to->lln == ARTMAP_DELa_LAY) &&
                (link_ptr->to->lun == ARTMAP_D2a_UNIT)
               )
            {
               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_Da))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_DELa_LAY;
               unit_ptr->lun = ARTMAP_D3a_UNIT;
               **topo_ptr = unit_ptr;
               (*topo_ptr)++;
               got_d3a_unit = TRUE;
               unit_ptr->flags |= UFLAG_REFRESH;

               /* we can break the search for the d3a unit */
               break;
            } /*if*/

         } /*FOR_ALL_LINKS*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   if (!got_d3a_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: d3 (=d3a)");
   } /*if*/

   *no_of_units = no_of_dela_units + 3;

   return (ret_code);

} /* kram_get_DelUnits_a () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_RstUnits_a (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                 ret_code   = KRERR_NO_ERROR;

   register struct Unit     *unit_ptr;
   register struct Site     *site_ptr;
   register struct Link     *link_ptr;

   bool                     has_link_to_itself,
                            has_link_to_dela_unit;

   /* each rsta unit has got sites, a link to itself, one from a dela unit
      and one from the rga unit which wasn't determined yet.
   */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr)) && (UNIT_HAS_SITES (unit_ptr))) {

         has_link_to_itself = FALSE;
         has_link_to_dela_unit = FALSE;

         FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

            if (link_ptr->to == unit_ptr) {
               has_link_to_itself = TRUE;
            } /*if*/

            if ((link_ptr->to->lln == ARTMAP_DELa_LAY) &&
                (link_ptr->to->lun == ARTMAP_DELa_REC_UNIT)
               )
            {
               has_link_to_dela_unit = TRUE;
            } /*if*/

         } /*FOR_ALL_SITES_AND_LINKS*/

         if (has_link_to_itself && has_link_to_dela_unit) {

            if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RSTa))) {
                TOPO_MSG_ACT_FUNC (unit_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                 TOPO_MSG_OUT_FUNC (unit_ptr);
            } /*if*/

            unit_ptr->lln = ARTMAP_RSTa_LAY;
            (*no_of_units)++;
            **topo_ptr = unit_ptr;
            (*topo_ptr)++;
            unit_ptr->flags |= UFLAG_REFRESH;

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kram_get_RstUnits_a () */
/*___________________________________________________________________________*/


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_SpecUnits_a (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                ret_code  = KRERR_NO_ERROR;

   register struct Unit     *unit_ptr;
   register struct Site     *site_ptr;
   register struct Link     *link_ptr;

   TopoPtrArray             topo_spec_ptr = *topo_ptr;

   bool                     got_g1a_unit    = FALSE;
   bool                     got_ria_unit    = FALSE;
   bool                     got_rca_unit    = FALSE;
   bool                     got_rga_unit    = FALSE;
   bool                     got_cla_unit    = FALSE;
   bool                     got_nca_unit    = FALSE;
   bool                     got_rhoa_unit   = FALSE;
   bool                     got_g2a_unit    = FALSE;

   bool                     links_to_rsta_units,
                            links_to_d3a_unit,
                            links_to_reca_units,
                            links_to_cmpa_units,
                            links_to_inpa_units;


   *no_of_units = 0;

   /* looking for special units of ARTa */

   FOR_ALL_UNITS (unit_ptr) {

      if ( (!(UNIT_REFRESHED (unit_ptr))) &&
           (!got_g1a_unit || !got_ria_unit || !got_rca_unit ||
            !got_cla_unit || !got_nca_unit || !got_g2a_unit
           )
         )
      {

         if (UNIT_HAS_SITES (unit_ptr)) {


            /* if a unit with sites does have links from inpa units
               and reca units this can only be the g1a unit
            */
            links_to_reca_units = FALSE;
            links_to_inpa_units = FALSE;
            links_to_cmpa_units = FALSE;

            FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

               switch (link_ptr->to->lln) {
               case ARTMAP_RECa_LAY:
                  links_to_reca_units = TRUE;
                  break;
               case ARTMAP_INPa_LAY:
                  links_to_inpa_units = TRUE;
                  break;
               case ARTMAP_CMPa_LAY:
                  links_to_cmpa_units = TRUE;
                  break;
               } /*switch*/

               if ((links_to_reca_units && links_to_inpa_units) ||
                   (links_to_reca_units && !links_to_cmpa_units)
                  )
               {
                  /* we found the g1a or the ria unit */
                  break;
               } /*if*/

            } /*FOR_ALL_SITES_AND_LINKS*/

            if (links_to_reca_units && links_to_inpa_units) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_G1a))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_SPECa_LAY;
               unit_ptr->lun = ARTMAP_G1a_UNIT;
               (*no_of_units)++;
               *(topo_spec_ptr + 2) = unit_ptr;
               got_g1a_unit = TRUE;
               unit_ptr->flags |= UFLAG_REFRESH;
               (*topo_ptr)++;

            } else {

               if (links_to_inpa_units && !links_to_cmpa_units) {

                  /* if there are links to the inpa units but none to the
                     reca units then we found the ria unit
                  */
                  if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RIa))) {
                     TOPO_MSG_ACT_FUNC (unit_ptr);
                  } /*if*/

                  if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                      TOPO_MSG_OUT_FUNC (unit_ptr);
                  } /*if*/

                  unit_ptr->lln = ARTMAP_SPECa_LAY;
                  unit_ptr->lun = ARTMAP_RIa_UNIT;
                  (*no_of_units)++;
                  *(topo_spec_ptr + 3) = unit_ptr;
                  got_ria_unit = TRUE;
                  unit_ptr->flags |= UFLAG_REFRESH;
                  (*topo_ptr)++;

               } /*if*/

            } /*if*/

         } else {

            /* unit has direct inputs */

            links_to_rsta_units = FALSE;
            links_to_d3a_unit   = FALSE;
            links_to_cmpa_units = FALSE;
            links_to_inpa_units = FALSE;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               switch (link_ptr->to->lln) {
               case ARTMAP_INPa_LAY:
                  links_to_inpa_units = TRUE;
                  break;
               case ARTMAP_CMPa_LAY:
                  links_to_cmpa_units = TRUE;
                  break;
               case ARTMAP_DELa_LAY:
                  if (link_ptr->to->lun == ARTMAP_D3a_UNIT) {
                     links_to_d3a_unit = TRUE;
                  } /*if*/
                  break;
                case ARTMAP_RSTa_LAY:
                   links_to_rsta_units = TRUE;
                   break;
                } /*switch*/

            } /*FOR_ALL_LINKS*/

            if (links_to_inpa_units && !got_g2a_unit) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_G2a))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_SPECa_LAY;
               unit_ptr->lun = ARTMAP_G2a_UNIT;
               (*no_of_units)++;
               *(topo_spec_ptr + 7) = unit_ptr;
               (*topo_ptr)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               got_g2a_unit = TRUE;
               continue;
            } /*if*/


            if (links_to_cmpa_units && !links_to_inpa_units &&
                !got_rca_unit
               )
            {

               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RCa))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_SPECa_LAY;
               unit_ptr->lun = ARTMAP_RCa_UNIT;
               (*no_of_units)++;
               *(topo_spec_ptr + 4) = unit_ptr;
               (*topo_ptr)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               got_rca_unit = TRUE;
               continue;

            } /*if*/


            if (links_to_d3a_unit && !got_cla_unit) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_CLa))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_SPECa_LAY;
               unit_ptr->lun = ARTMAP_CLa_UNIT;
               (*no_of_units)++;
               *topo_spec_ptr = unit_ptr;
               (*topo_ptr)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               got_cla_unit = TRUE;
               continue;

            } /*if*/


            if (links_to_rsta_units && !links_to_cmpa_units &&
                !got_nca_unit
               )
            {
               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_NCa))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_SPECa_LAY;
               unit_ptr->lun = ARTMAP_NCa_UNIT;
               (*no_of_units)++;
               *(topo_spec_ptr + 1) = unit_ptr;
               (*topo_ptr)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               got_nca_unit = TRUE;
               continue;

            } /*if*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/


   /* the rga and the rhoa unit remain to be determined */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr))) {

         /* looking for rga unit */

         if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ARTMAP_SPECa_LAY) &&
                   (link_ptr->to->lun == ARTMAP_RIa_UNIT)
                  )
               {
                  if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RGa))) {
                     TOPO_MSG_ACT_FUNC (unit_ptr);
                  } /*if*/

                  if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                     TOPO_MSG_OUT_FUNC (unit_ptr);
                  } /*if*/

                  /* this is the rga unit */
                  unit_ptr->lln = ARTMAP_SPECa_LAY;
                  unit_ptr->lun = ARTMAP_RGa_UNIT;
                  (*no_of_units)++;
                  *(topo_spec_ptr + 5) = unit_ptr;
                  got_rga_unit =TRUE;
                  unit_ptr->flags |= UFLAG_REFRESH;
                  (*topo_ptr)++;
                  break;

               } /*if*/

            } /*FOR_ALL_LINKS*/

         } /*if*/

      } else {

         /* looking for rhoa unit via ria */

         if ((unit_ptr->lln == ARTMAP_SPECa_LAY) &&
             (unit_ptr->lun == ARTMAP_RIa_UNIT)
            )
         {
             FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

                if (link_ptr->to->lln != ARTMAP_INPa_LAY) {

                   if (!(CHECK_ACT_FUNC (link_ptr->to, ARTMAP_ACTF_RHOa))) {
                      TOPO_MSG_ACT_FUNC (link_ptr->to);
                   } /*if*/

                   if (!(CHECK_OUT_FUNC (link_ptr->to, ARTMAP_OUTFUNC))) {
                      TOPO_MSG_OUT_FUNC (link_ptr->to);
                   } /*if*/

                   /* the link_ptr->to unit is the rhoa unit */

                   link_ptr->to->lln = ARTMAP_SPECa_LAY;
                   link_ptr->to->lun = ARTMAP_RHOa_UNIT;
                   (*no_of_units)++;
                   *(topo_spec_ptr + 6) = link_ptr->to;
                   (*topo_ptr)++;
                   got_rhoa_unit = TRUE;
                   link_ptr->to->flags |= UFLAG_REFRESH;
                   break;

                } /*if*/

             } /*FOR_ALL_SITES_AND_LINKS*/

          } /*if*/

       } /*if*/

       if (got_rga_unit && got_rhoa_unit) {
          break;
       } /*if*/

   } /*FOR_ALL_UNITS*/

   if (!got_cla_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: cl (=cla)");
   } /*if*/

   if (!got_nca_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: nc (=nca)");
   } /*if*/

   if (!got_ria_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: ri (=ria)");
   } /*if*/

   if (!got_rca_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: rc (=rca)");
   } /*if*/

   if (!got_rga_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: rg (=rga)");
   } /*if*/

   if (!got_g1a_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: g1 (=g1a)");
   } /*if*/

   if (!got_rhoa_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: rho (=rhoa)");
   } /*if*/

   if (!got_g2a_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTa: g2 (=g2a)");
   } /*if*/


   return (ret_code);

} /* kram_get_SpecUnits_a () */
/*___________________________________________________________________________*/




/*************** functions to determine units of ARTb
***************/


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_InpUnits_b (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                   ret_code = KRERR_NO_ERROR;

   register struct Unit       *unit_src_ptr, *unit_trg_ptr;
   register struct Site       *site_trg_ptr;
   register struct Link       *link_trg_ptr;

   int                        count_links;

   /* for the inpb units it is characteristical that they have got 2
      links to other units while the ones of ARTa have 3
   */

   *no_of_units = 0;
   FOR_ALL_UNITS (unit_src_ptr) {

      if (IS_INPUT_UNIT (unit_src_ptr)) {

         if (UNIT_HAS_INPUTS (unit_src_ptr)) {
            topo_msg.error_code = KRERR_I_UNITS_CONNECT;
            topo_msg.dest_error_unit = unit_src_ptr-unit_array;
            topo_msg.src_error_unit = 0;
            return (topo_msg.error_code);
         } /*if*/

         count_links = 0;

         FOR_ALL_UNITS (unit_trg_ptr) {

            if (UNIT_HAS_SITES (unit_trg_ptr)) {
               FOR_ALL_SITES_AND_LINKS (unit_trg_ptr, site_trg_ptr, link_trg_ptr) {
                  if (link_trg_ptr->to == unit_src_ptr) {
                     count_links++;
                  } /*if*/
               } /*FOR_ALL_SITES_AND_LINKS*/
            } else {
               FOR_ALL_LINKS (unit_trg_ptr, link_trg_ptr) {
                  if (link_trg_ptr->to == unit_src_ptr) {
                     count_links++;
                  } /*if*/
               } /*FOR_ALL_LINKS*/
            } /*if*/

         } /*FOR_ALL_UNITS*/

         if (count_links == 4) {

            if (!(CHECK_ACT_FUNC (unit_src_ptr, ARTMAP_ACTF_INPb))) {
               TOPO_MSG_ACT_FUNC (unit_src_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_src_ptr, ARTMAP_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_src_ptr);
            } /*if*/

            (*no_of_units)++;
            **topo_ptr = unit_src_ptr;
            unit_src_ptr->lln = ARTMAP_INPb_LAY;
            (*topo_ptr)++;
            unit_src_ptr->flags |= UFLAG_REFRESH;

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kram_get_InpUnits_b () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_CmpUnits_b (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                 ret_code  = KRERR_NO_ERROR;

   register struct Unit     *unit_ptr;
   struct Unit              *unit_src_ptr;
   register struct Link     *link_ptr;
   struct Link              *link_src_ptr;

   bool                     is_cmpb_unit;
   bool                     has_link_to_inpb;
   bool                     has_link_to_other_than_inpb;


   *no_of_units = 0;

   /* look for a recb unit */

   FOR_ALL_UNITS (unit_ptr) {

      if (!IS_SPECIAL_UNIT (unit_ptr)) {
         continue;
      } /*if*/

      /* the recognition unit is not supposed to have sites
      */
      if (UNIT_HAS_SITES (unit_ptr)) {
         TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
      } /*if*/


      /* unit_ptr points to a recogniton unit.
         we follow the incoming links of this unit. if the source unit we
         got by doing this has the following properties, this is a comparison
         unit of ARTb
                - has no sites
                - has incoming link from inpb unit
      */

      FOR_ALL_LINKS (unit_ptr, link_ptr) {

         unit_src_ptr = link_ptr->to;

         is_cmpb_unit                = FALSE;
         has_link_to_inpb            = FALSE;
         has_link_to_other_than_inpb = FALSE;

         if (UNIT_HAS_DIRECT_INPUTS (unit_src_ptr)) {

            FOR_ALL_LINKS (unit_src_ptr, link_src_ptr) {

               if (link_src_ptr->to->lln == ARTMAP_INPb_LAY) {
                  has_link_to_inpb = TRUE;
               } else {
                  has_link_to_other_than_inpb = TRUE;
               } /*if*/

               if (has_link_to_inpb && has_link_to_other_than_inpb) {
                  is_cmpb_unit = TRUE;
                  break;
               } /*if*/

            } /*FOR_ALL_LINKS*/

         } /*if*/

         if (is_cmpb_unit) {

            if (!(CHECK_ACT_FUNC (unit_src_ptr, ARTMAP_ACTF_CMPb))) {
               TOPO_MSG_ACT_FUNC (unit_src_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_src_ptr, ARTMAP_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_src_ptr);
            } /*if*/

            unit_src_ptr->lln = ARTMAP_CMPb_LAY;
            (*no_of_units)++;
            **topo_ptr = unit_src_ptr;
            unit_src_ptr->flags |= UFLAG_REFRESH;
            (*topo_ptr)++;

         } /*if*/

      } /*FOR_ALL_LINKS*/

      if (*no_of_units > 0) {
         /* stop searching for reca unit */
         break;
      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kram_get_CmpUnits_b () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_RecUnits_b (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                   ret_code   = KRERR_NO_ERROR;

   register struct Unit       *unit_ptr;
   register struct Link       *link_ptr;

   bool                       is_recb_unit;


   *no_of_units = 0;

   /* properties of recb units:
          - has no sites
          - has incoming links from cmpb units
   */

   FOR_ALL_UNITS (unit_ptr) {

      if (IS_SPECIAL_UNIT (unit_ptr)) {

         if (UNIT_HAS_SITES (unit_ptr)) {
            TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
         } /*if*/

         is_recb_unit = FALSE;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            if (link_ptr->to->lln == ARTMAP_CMPb_LAY) {
               is_recb_unit = TRUE;
               break;
            } /*if*/

         } /*FOR_ALL_LINKS*/

         if (is_recb_unit) {

            if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RECb))) {
               TOPO_MSG_ACT_FUNC (unit_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_ptr);
            } /*if*/

            unit_ptr->lln = ARTMAP_RECb_LAY;
            **topo_ptr = unit_ptr;
            (*no_of_units)++;
            (*topo_ptr)++;
            unit_ptr->flags |= UFLAG_REFRESH;

         } /*if*/

      } /*if*/

    } /*FOR_ALL_UNITS*/

    return (ret_code);

} /* kram_get_RecUnits_b */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_DelUnits_b (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                 ret_code      = KRERR_NO_ERROR;

   register struct Unit     *unit_ptr;
   register struct Link     *link_ptr;

   int                      count_links;
   int                      no_of_delb_units = 0;
   bool                     has_link_to_recb_unit;
   bool                     got_d1b_unit     = FALSE;
   bool                     got_d2b_unit     = FALSE;
   bool                     got_d3b_unit     = FALSE;


   *no_of_units = 0;

   /* the delb units each have got just two incoming links, one of which
      is from their corresponding reca unit. They don't have sites.
   */

   FOR_ALL_UNITS (unit_ptr) {

      if ((! (UNIT_REFRESHED (unit_ptr))) && (UNIT_HAS_DIRECT_INPUTS (unit_ptr))) {

         count_links = 0;
         has_link_to_recb_unit = FALSE;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            count_links++;
            if (link_ptr->to->lln == ARTMAP_RECb_LAY) {
               has_link_to_recb_unit = TRUE;
            } /*if*/

         } /*FOR_ALL_LINKS*/

         if ((count_links == 2) && (has_link_to_recb_unit)) {

            if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_DELb))) {
              TOPO_MSG_ACT_FUNC (unit_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_ptr);
            } /*if*/

            unit_ptr->lln = ARTMAP_DELb_LAY;
            unit_ptr->lun = ARTMAP_DELb_REC_UNIT;
            no_of_delb_units++;
            **topo_ptr = unit_ptr;
            (*topo_ptr)++;
            unit_ptr->flags |= UFLAG_REFRESH;

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/


   /* now we are looking for d1b which has to have links to all delb units
   */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr)) && (UNIT_HAS_DIRECT_INPUTS (unit_ptr))) {

         count_links = 0;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            if ((link_ptr->to->lln == ARTMAP_DELb_LAY) &&
                (link_ptr->to->lun == ARTMAP_DELb_REC_UNIT))
            {
               count_links ++;
            } /*if*/

         } /*FOR_ALL_LINKS*/

         if (count_links == no_of_delb_units) {

            /* in this case we don't really check the Activation and
               output function, we use it to detemine the correct unit
               (d1b) 'cause the d1 unit has the same properties as
               d1b except its activation function.
            */
            if ((CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_Db)) &&
                (CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))
               )
            {
               unit_ptr->lln = ARTMAP_DELb_LAY;
               unit_ptr->lun = ARTMAP_D1b_UNIT;
               **topo_ptr = unit_ptr;
               (*topo_ptr)++;
               got_d1b_unit = TRUE;
               unit_ptr->flags |= UFLAG_REFRESH;

               /* Now we can break the search, 'cause we got the d1b unit */
               break;

            } /*if*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   if (!got_d1b_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: d1 (=d1b)");
   } /*if*/


   /* Now we are looking for the d2b unit */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr)) && (UNIT_HAS_DIRECT_INPUTS (unit_ptr))) {

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            if ((link_ptr->to->lln == ARTMAP_DELb_LAY) &&
                (link_ptr->to->lun == ARTMAP_D1b_UNIT)
               )
            {
               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_Db))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_DELb_LAY;
               unit_ptr->lun = ARTMAP_D2b_UNIT;
               **topo_ptr = unit_ptr;
               (*topo_ptr)++;
               got_d2b_unit = TRUE;
               unit_ptr->flags |= UFLAG_REFRESH;

               /* we can break the search for the d2b unit */
               break;

            } /*if*/

         } /*FOR_ALL_LINKS*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   if (!got_d2b_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: d2 (=d2b)");
   } /*if*/


   /* Now we are looking for the d3b unit */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr)) && (UNIT_HAS_DIRECT_INPUTS (unit_ptr))) {

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            if ((link_ptr->to->lln == ARTMAP_DELb_LAY) &&
                (link_ptr->to->lun == ARTMAP_D2b_UNIT)
               )
            {
               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_Db))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_DELb_LAY;
               unit_ptr->lun = ARTMAP_D3b_UNIT;
               **topo_ptr = unit_ptr;
               (*topo_ptr)++;
               got_d3b_unit = TRUE;
               unit_ptr->flags |= UFLAG_REFRESH;

               /* we can break the search for the d3b unit */
               break;

            } /*if*/

         } /*FOR_ALL_LINKS*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   if (!got_d3b_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: d3 (=d3b)");
   } /*if*/


   *no_of_units = no_of_delb_units + 3;

   return (ret_code);

} /* kram_get_DelUnits_b () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_RstUnits_b (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                 ret_code   = KRERR_NO_ERROR;

   register struct Unit     *unit_ptr;
   register struct Site     *site_ptr;
   register struct Link     *link_ptr;

   bool                     has_link_to_itself,
                            has_link_to_delb_unit;

   /* each rstb unit has got sites, a link to itself, one from a delb unit
      and one from the rgb unit which wasn't determined yet.
   */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr)) && (UNIT_HAS_SITES (unit_ptr))) {

         has_link_to_itself = FALSE;
         has_link_to_delb_unit = FALSE;

         FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

            if (link_ptr->to == unit_ptr) {
               has_link_to_itself = TRUE;
            } /*if*/

            if ((link_ptr->to->lln == ARTMAP_DELb_LAY) &&
                (link_ptr->to->lun == ARTMAP_DELb_REC_UNIT)
               )
            {
               has_link_to_delb_unit = TRUE;
            } /*if*/

         } /*FOR_ALL_SITES_AND_LINKS*/

         if (has_link_to_itself && has_link_to_delb_unit) {

            if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RSTb))) {
                TOPO_MSG_ACT_FUNC (unit_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                 TOPO_MSG_OUT_FUNC (unit_ptr);
            } /*if*/

            unit_ptr->lln = ARTMAP_RSTb_LAY;
            (*no_of_units)++;
            **topo_ptr = unit_ptr;
            (*topo_ptr)++;
            unit_ptr->flags |= UFLAG_REFRESH;

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kram_get_RstUnits_b () */
/*___________________________________________________________________________*/


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_SpecUnits_b (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                ret_code  = KRERR_NO_ERROR;

   register struct Unit     *unit_ptr;
   register struct Site     *site_ptr;
   register struct Link     *link_ptr;

   TopoPtrArray             topo_spec_ptr = *topo_ptr;

   bool                     got_g1b_unit    = FALSE;
   bool                     got_rib_unit    = FALSE;
   bool                     got_rcb_unit    = FALSE;
   bool                     got_rgb_unit    = FALSE;
   bool                     got_clb_unit    = FALSE;
   bool                     got_ncb_unit    = FALSE;
   bool                     got_rhob_unit   = FALSE;
   bool                     got_g2b_unit    = FALSE;

   bool                     links_to_rstb_units,
                            links_to_d3b_unit,
                            links_to_recb_units,
                            links_to_cmpb_units,
                            links_to_inpb_units;


   *no_of_units = 0;

   /* looking for special units of ARTb */

   FOR_ALL_UNITS (unit_ptr) {

      if ( (!(UNIT_REFRESHED (unit_ptr))) &&
           (!got_g1b_unit || !got_rib_unit || !got_rcb_unit ||
            !got_clb_unit || !got_ncb_unit || !got_g2b_unit
           )
         )
      {

         if (UNIT_HAS_SITES (unit_ptr)) {


            /* if a unit with sites does have links from inpb units
               and recb units this can only be the g1b unit
            */
            links_to_recb_units = FALSE;
            links_to_inpb_units = FALSE;

            FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

               if (link_ptr->to->lln == ARTMAP_RECb_LAY) {
                  links_to_recb_units = TRUE;
               } /*if*/

               if (link_ptr->to->lln == ARTMAP_INPb_LAY) {
                  links_to_inpb_units = TRUE;
               } /*if*/

               if (links_to_inpb_units) {
                  /* we found the g1b unit */
                  break;
               } /*if*/

            } /*FOR_ALL_SITES_AND_LINKS*/

            if (links_to_recb_units && links_to_inpb_units) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_G1b))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_SPECb_LAY;
               unit_ptr->lun = ARTMAP_G1b_UNIT;
               (*no_of_units)++;
               *(topo_spec_ptr + 2) = unit_ptr;
               got_g1b_unit = TRUE;
               unit_ptr->flags |= UFLAG_REFRESH;
               (*topo_ptr)++;

            } else {

               if (links_to_inpb_units) {

                  /* if there are links to the inpb units but none to the
                     recb units then we found the rib unit
                  */
                  if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RIb))) {
                     TOPO_MSG_ACT_FUNC (unit_ptr);
                  } /*if*/

                  if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                      TOPO_MSG_OUT_FUNC (unit_ptr);
                  } /*if*/

                  unit_ptr->lln = ARTMAP_SPECb_LAY;
                  unit_ptr->lun = ARTMAP_RIb_UNIT;
                  (*no_of_units)++;
                  *(topo_spec_ptr + 3) = unit_ptr;
                  got_rib_unit = TRUE;
                  unit_ptr->flags |= UFLAG_REFRESH;
                  (*topo_ptr)++;

               } /*if*/

            } /*if*/

         } else {

            /* unit has direct inputs */

            links_to_rstb_units = FALSE;
            links_to_d3b_unit   = FALSE;
            links_to_cmpb_units = FALSE;
            links_to_inpb_units = FALSE;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               switch (link_ptr->to->lln) {
               case ARTMAP_INPb_LAY:
                  links_to_inpb_units = TRUE;
                  break;
               case ARTMAP_CMPb_LAY:
                  links_to_cmpb_units = TRUE;
                  break;
               case ARTMAP_DELb_LAY:
                  if (link_ptr->to->lun == ARTMAP_D3b_UNIT) {
                     links_to_d3b_unit = TRUE;
                  } /*if*/
                  break;
               case ARTMAP_RSTb_LAY:
                  links_to_rstb_units = TRUE;
                  break;
                } /*switch*/


            } /*FOR_ALL_LINKS*/


            if (links_to_inpb_units && !got_g2b_unit) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_G2b))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_SPECb_LAY;
               unit_ptr->lun = ARTMAP_G2b_UNIT;
               (*no_of_units)++;
               *(topo_spec_ptr + 7) = unit_ptr;
               (*topo_ptr)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               got_g2b_unit = TRUE;
               continue;
            } /*if*/


            if (links_to_cmpb_units && !links_to_inpb_units &&
                !got_rcb_unit
               )
            {

               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RCb))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_SPECb_LAY;
               unit_ptr->lun = ARTMAP_RCb_UNIT;
               (*no_of_units)++;
               *(topo_spec_ptr + 4) = unit_ptr;
               (*topo_ptr)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               got_rcb_unit = TRUE;
               continue;

            } /*if*/


            if (links_to_d3b_unit && !got_clb_unit) {

               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_CLb))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_SPECb_LAY;
               unit_ptr->lun = ARTMAP_CLb_UNIT;
               (*no_of_units)++;
               *topo_spec_ptr = unit_ptr;
               (*topo_ptr)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               got_clb_unit = TRUE;
               continue;

            } /*if*/


            if (links_to_rstb_units && !links_to_cmpb_units &&
                !got_ncb_unit
               )
            {
               if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_NCb))) {
                  TOPO_MSG_ACT_FUNC (unit_ptr);
               } /*if*/

               if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                  TOPO_MSG_OUT_FUNC (unit_ptr);
               } /*if*/

               unit_ptr->lln = ARTMAP_SPECb_LAY;
               unit_ptr->lun = ARTMAP_NCb_UNIT;
               (*no_of_units)++;
               *(topo_spec_ptr + 1) = unit_ptr;
               (*topo_ptr)++;
               unit_ptr->flags |= UFLAG_REFRESH;
               got_ncb_unit = TRUE;
               continue;

            } /*if*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/


   /* the rgb and the rhob unit remain to be determined */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr))) {

         /* looking for rgb unit */

         if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ARTMAP_SPECb_LAY) &&
                   (link_ptr->to->lun == ARTMAP_RIb_UNIT)
                  )
               {

                  if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RGb))) {
                     TOPO_MSG_ACT_FUNC (unit_ptr);
                  } /*if*/

                  if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                     TOPO_MSG_OUT_FUNC (unit_ptr);
                  } /*if*/

                   /* this is the rgb unit */
                   unit_ptr->lln = ARTMAP_SPECb_LAY;
                   unit_ptr->lun = ARTMAP_RGb_UNIT;
                   (*no_of_units)++;
                   *(topo_spec_ptr + 5) = unit_ptr;
                   got_rgb_unit =TRUE;
                   (*topo_ptr)++;
                   unit_ptr->flags |= UFLAG_REFRESH;
                   break;

               } /*if*/

            } /*FOR_ALL_LINKS*/

         } /*if*/

      } else {

         /* looking for rhob unit via ria */

         if ((unit_ptr->lln == ARTMAP_SPECb_LAY) &&
             (unit_ptr->lun == ARTMAP_RIb_UNIT)
            )
         {
             FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

                if (link_ptr->to->lln != ARTMAP_INPb_LAY) {

                   if (!(CHECK_ACT_FUNC (link_ptr->to, ARTMAP_ACTF_RHOb))) {
                      TOPO_MSG_ACT_FUNC (link_ptr->to);
                   } /*if*/

                   if (!(CHECK_OUT_FUNC (link_ptr->to, ARTMAP_OUTFUNC))) {
                      TOPO_MSG_OUT_FUNC (link_ptr->to);
                   } /*if*/

                   /* the link_ptr->to unit is the rhoa unit */

                   /* the link_ptr->to unit is the rhob unit */

                   link_ptr->to->lln = ARTMAP_SPECb_LAY;
                   link_ptr->to->lun = ARTMAP_RHOb_UNIT;
                   (*no_of_units)++;
                   *(topo_spec_ptr + 6) = link_ptr->to;
                   (*topo_ptr)++;
                   got_rhob_unit = TRUE;
                   link_ptr->to->flags |= UFLAG_REFRESH;
                   break;

                } /*if*/

             } /*FOR_ALL_SITES_AND_LINKS*/

          } /*if*/

       } /*if*/

       if (got_rgb_unit && got_rhob_unit) {
          break;
       } /*if*/

   } /*FOR_ALL_UNITS*/

   if (!got_clb_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: cl (=clb)");
   } /*if*/

   if (!got_ncb_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: nc (=ncb)");
   } /*if*/

   if (!got_rib_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: ri (=rib)");
   } /*if*/

   if (!got_rcb_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: rc (=rcb)");
   } /*if*/

   if (!got_rgb_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: rg (=rgb)");
   } /*if*/

   if (!got_g1b_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: g1 (=g1b)");
   } /*if*/

   if (!got_rhob_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: rho (=rhob)");
   } /*if*/

   if (!got_g2b_unit) {
      TOPO_MSG_UNIT_MISSING ("ARTb: g2 (=g2b)");
   } /*if*/


   return (ret_code);

} /* kram_get_SpecUnits_b () */
/*___________________________________________________________________________*/






/*************** functions to determine units of map field of ARTMAP net
***************/


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_get_MapUnits (TopoPtrArray *topo_ptr, int *no_of_units)
{
   krui_err                ret_code  = KRERR_NO_ERROR;

   register struct Unit    *unit_ptr;
   register struct Link    *link_ptr;

   bool                    has_link_to_dela_unit,
                           has_link_to_delb_unit;

   *no_of_units = 0;

   /* looking for map units. These units have got incoming
      links from the dela units and the delb units. They have
      no sites.
   */

   FOR_ALL_UNITS (unit_ptr) {

      if (!(UNIT_REFRESHED (unit_ptr)) && (UNIT_HAS_DIRECT_INPUTS (unit_ptr))) {

         has_link_to_dela_unit = FALSE;
         has_link_to_delb_unit = FALSE;

         FOR_ALL_LINKS (unit_ptr, link_ptr) {

            if (link_ptr->to->lln == ARTMAP_DELa_LAY) {
               has_link_to_dela_unit = TRUE;
               continue;
            } /*if*/

            if (link_ptr->to->lln == ARTMAP_DELb_LAY) {
               has_link_to_delb_unit = TRUE;
               continue;
            } /*if*/

            if (has_link_to_dela_unit && has_link_to_delb_unit) {
               break;
            } /*if*/

         } /*FOR_ALL_LINKS*/

         if (has_link_to_dela_unit && has_link_to_delb_unit) {

            if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_MAP))) {
               TOPO_MSG_ACT_FUNC (unit_ptr);
            } /*if*/

            if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
               TOPO_MSG_OUT_FUNC (unit_ptr);
            } /*if*/

            unit_ptr->lln = ARTMAP_MAP_LAY;
            (*no_of_units)++;
            **topo_ptr = unit_ptr;
            (*topo_ptr)++;
            unit_ptr->flags |= UFLAG_REFRESH;

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/

   return (ret_code);

} /* kram_get_MapUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_get_SpecUnits (TopoPtrArray *topo_ptr, int *no_of_units)
{
    krui_err                  ret_code  = KRERR_NO_ERROR;

    register struct Unit       *unit_ptr;
    register struct Site       *site_ptr;
    register struct Link       *link_ptr;

    TopoPtrArray               topo_spec_ptr = *topo_ptr;

    bool                       has_links_to_reca_units,
                               has_links_to_inpa_units,
                               has_link_to_cla_unit,
                               has_link_to_nca_unit,
                               has_link_to_itself,
                               has_links_to_map_units,
                               has_link_to_rhoa_unit,
                               has_link_to_delb_unit,
                               has_link_to_rga_unit;

    bool                       got_d1_unit    = FALSE;
    bool                       got_cl_unit    = FALSE;
    bool                       got_nc_unit    = FALSE;
    bool                       got_G_unit     = FALSE;
    bool                       got_rb_unit    = FALSE;
    bool                       got_rm_unit    = FALSE;
    bool                       got_rg_unit    = FALSE;
    bool                       got_rho_unit   = FALSE;
    bool                       got_qu_unit    = FALSE;
    bool                       got_drho_unit  = FALSE;


    FOR_ALL_UNITS (unit_ptr) {

       if (!(UNIT_REFRESHED (unit_ptr))) {

          if (UNIT_HAS_SITES (unit_ptr)) {

             /* G, rb or qu unit */

             has_links_to_reca_units = FALSE;
             has_links_to_inpa_units = FALSE;

             FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

                switch (link_ptr->to->lln) {
                case ARTMAP_RECa_LAY:
                   has_links_to_reca_units = TRUE;
                   break;
                case ARTMAP_INPa_LAY:
                   has_links_to_inpa_units = TRUE;
                   break;
                } /*if*/

             } /*FOR_ALL_SITES_AND_LINKS*/

             if (has_links_to_reca_units && !got_G_unit) {

                if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_G))) {
                   TOPO_MSG_ACT_FUNC (unit_ptr);
                } /*if*/

                if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                   TOPO_MSG_OUT_FUNC (unit_ptr);
                } /*if*/

                /* This is the G unit */
                unit_ptr->lln = ARTMAP_SPEC_LAY;
                unit_ptr->lun = ARTMAP_G_UNIT;
                *(topo_spec_ptr + 3) = unit_ptr;
                (*topo_ptr)++;
                (*no_of_units)++;
                unit_ptr->flags |= UFLAG_REFRESH;
                got_G_unit = TRUE;
                continue;

              } /*if*/


             if (has_links_to_inpa_units && !got_qu_unit) {

                if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_QU))) {
                   TOPO_MSG_ACT_FUNC (unit_ptr);
                } /*if*/

                if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                   TOPO_MSG_OUT_FUNC (unit_ptr);
                } /*if*/

                /* This is the qu unit */
                unit_ptr->lln = ARTMAP_SPEC_LAY;
                unit_ptr->lun = ARTMAP_QU_UNIT;
                *(topo_spec_ptr + 8) = unit_ptr;
                (*topo_ptr)++;
                (*no_of_units)++;
                unit_ptr->flags |= UFLAG_REFRESH;
                got_qu_unit = TRUE;
                continue;

              } /*if*/

              if (!got_rb_unit) {

                if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RB))) {
                   TOPO_MSG_ACT_FUNC (unit_ptr);
                } /*if*/

                if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                   TOPO_MSG_OUT_FUNC (unit_ptr);
                } /*if*/

                /* This is the rb unit */
                unit_ptr->lln = ARTMAP_SPEC_LAY;
                unit_ptr->lun = ARTMAP_RB_UNIT;
                *(topo_spec_ptr + 4) = unit_ptr;
                (*topo_ptr)++;
                (*no_of_units)++;
                unit_ptr->flags |= UFLAG_REFRESH;
                got_rb_unit = TRUE;

              } /*if*/

          }  else {

              /* unit has no sites, so this is the cl, nc, rho, rm,
                 drho or rg unit
              */

              has_link_to_cla_unit   = FALSE;
              has_link_to_nca_unit   = FALSE;
              has_link_to_itself     = FALSE;
              has_links_to_map_units = FALSE;
              has_link_to_rhoa_unit  = FALSE;
              has_link_to_delb_unit  = FALSE;
              has_link_to_rga_unit   = FALSE;


              FOR_ALL_LINKS (unit_ptr, link_ptr) {

                 switch (link_ptr->to->lln) {
                 case ARTMAP_SPECa_LAY :

                    switch (link_ptr->to->lun) {
                    case ARTMAP_CLa_UNIT:
                       has_link_to_cla_unit = TRUE;
                       break;
                    case ARTMAP_NCa_UNIT:
                       has_link_to_nca_unit = TRUE;
                       break;
                    case ARTMAP_RHOa_UNIT:
                       has_link_to_rhoa_unit = TRUE;
                       break;
                    case ARTMAP_RGa_UNIT:
                       has_link_to_rga_unit = TRUE;
                       break;
                    } /*switch*/

                    break;

                 case ARTMAP_MAP_LAY:
                    has_links_to_map_units = TRUE;
                    break;

                 case ARTMAP_DELb_LAY:
                    has_link_to_delb_unit = TRUE;
                    break;

                 default:
                    if (link_ptr->to == unit_ptr) {
                       has_link_to_itself = TRUE;
                    } /*if*/

                 } /*switch*/

              } /*FOR_ALL_LINKS*/

              if (has_link_to_cla_unit && !got_cl_unit) {

                 if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_CL))) {
                    TOPO_MSG_ACT_FUNC (unit_ptr);
                 } /*if*/

                 if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                    TOPO_MSG_OUT_FUNC (unit_ptr);
                 } /*if*/

                 unit_ptr->lln = ARTMAP_SPEC_LAY;
                 unit_ptr->lun = ARTMAP_CL_UNIT;
                 ArtMap_cl_unit = unit_ptr;
                 *(topo_spec_ptr + 1) = unit_ptr;
                 (*topo_ptr)++;
                 (*no_of_units)++;
                 unit_ptr->flags |= UFLAG_REFRESH;
                 got_cl_unit = TRUE;
                 continue;

              } /*if*/

              if (has_link_to_nca_unit && !got_nc_unit) {

                 if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_NC))) {
                    TOPO_MSG_ACT_FUNC (unit_ptr);
                 } /*if*/

                 if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                    TOPO_MSG_OUT_FUNC (unit_ptr);
                 } /*if*/

                 unit_ptr->lln = ARTMAP_SPEC_LAY;
                 unit_ptr->lun = ARTMAP_NC_UNIT;
                 ArtMap_nc_unit = unit_ptr;
                 *(topo_spec_ptr + 2) = unit_ptr;
                 (*topo_ptr)++;
                 (*no_of_units)++;
                 unit_ptr->flags |= UFLAG_REFRESH;
                 got_nc_unit = TRUE;
                 continue;

              } /*if*/

              if (has_link_to_itself && !got_rho_unit) {

                 if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RHO))) {
                    TOPO_MSG_ACT_FUNC (unit_ptr);
                 } /*if*/

                 if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                    TOPO_MSG_OUT_FUNC (unit_ptr);
                 } /*if*/

                 unit_ptr->lln = ARTMAP_SPEC_LAY;
                 unit_ptr->lun = ARTMAP_RHO_UNIT;
                 *(topo_spec_ptr + 7) = unit_ptr;
                 (*topo_ptr)++;
                 (*no_of_units)++;
                 unit_ptr->flags |= UFLAG_REFRESH;
                 got_rho_unit = TRUE;
                 continue;

              } /*if*/

              if (has_links_to_map_units && !got_rm_unit) {

                 if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RM))) {
                    TOPO_MSG_ACT_FUNC (unit_ptr);
                 } /*if*/

                 if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                    TOPO_MSG_OUT_FUNC (unit_ptr);
                 } /*if*/

                 unit_ptr->lln = ARTMAP_SPEC_LAY;
                 unit_ptr->lun = ARTMAP_RM_UNIT;
                 *(topo_spec_ptr + 5) = unit_ptr;
                 (*topo_ptr)++;
                 (*no_of_units)++;
                 unit_ptr->flags |= UFLAG_REFRESH;
                 got_rm_unit = TRUE;
                 continue;

              } /*if*/

              if (has_link_to_rga_unit && has_link_to_rhoa_unit &&
                  ! got_drho_unit
                 )
              {

                 if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_DRHO))) {
                    TOPO_MSG_ACT_FUNC (unit_ptr);
                 } /*if*/

                 if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                    TOPO_MSG_OUT_FUNC (unit_ptr);
                 } /*if*/

                 unit_ptr->lln = ARTMAP_SPEC_LAY;
                 unit_ptr->lun = ARTMAP_DRHO_UNIT;
                 *(topo_spec_ptr + 9) = unit_ptr;
                 (*topo_ptr)++;
                 (*no_of_units)++;
                 unit_ptr->flags |= UFLAG_REFRESH;
                 got_drho_unit = TRUE;
                 continue;

              } /*if*/

              if (has_link_to_delb_unit && !got_d1_unit) {

                 if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_D1))) {
                    TOPO_MSG_ACT_FUNC (unit_ptr);
                 } /*if*/

                 if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                    TOPO_MSG_OUT_FUNC (unit_ptr);
                 } /*if*/

                 unit_ptr->lln = ARTMAP_SPEC_LAY;
                 unit_ptr->lun = ARTMAP_D1_UNIT;
                 *topo_spec_ptr = unit_ptr;
                 (*topo_ptr)++;
                 (*no_of_units)++;
                 unit_ptr->flags |= UFLAG_REFRESH;
                 got_d1_unit = TRUE;
                 continue;

              } /*if*/

              if (!got_rg_unit) {

                 if (!(CHECK_ACT_FUNC (unit_ptr, ARTMAP_ACTF_RG))) {
                    TOPO_MSG_ACT_FUNC (unit_ptr);
                 } /*if*/

                 if (!(CHECK_OUT_FUNC (unit_ptr, ARTMAP_OUTFUNC))) {
                    TOPO_MSG_OUT_FUNC (unit_ptr);
                 } /*if*/

                 unit_ptr->lln = ARTMAP_SPEC_LAY;
                 unit_ptr->lun = ARTMAP_RG_UNIT;
                 *(topo_spec_ptr + 6) = unit_ptr;
                 (*topo_ptr)++;
                 (*no_of_units)++;
                 unit_ptr->flags |= UFLAG_REFRESH;
                 got_rg_unit = TRUE;

              } /*if*/

          } /*if (UNIT_HAS_SITES)*/

       } /* if (!UNIT_REFRESHED)*/

    } /*FOR_ALL_UNITS*/

   if (!got_cl_unit) {
      TOPO_MSG_UNIT_MISSING ("MAP: cl");
   } /*if*/

   if (!got_nc_unit) {
      TOPO_MSG_UNIT_MISSING ("MAP: nc");
   } /*if*/

   if (!got_rb_unit) {
      TOPO_MSG_UNIT_MISSING ("MAP: rb");
   } /*if*/

   if (!got_rm_unit) {
      TOPO_MSG_UNIT_MISSING ("MAP: rm");
   } /*if*/

   if (!got_rg_unit) {
      TOPO_MSG_UNIT_MISSING ("MAP: rg");
   } /*if*/

   if (!got_G_unit) {
      TOPO_MSG_UNIT_MISSING ("MAP: G");
   } /*if*/

   if (!got_d1_unit) {
      TOPO_MSG_UNIT_MISSING ("MAP: d1");
   } /*if*/

   if (!got_rho_unit) {
      TOPO_MSG_UNIT_MISSING ("MAP: rho");
   } /*if*/

   if (!got_qu_unit) {
      TOPO_MSG_UNIT_MISSING ("MAP: qu");
   } /*if*/

   if (!got_drho_unit) {
      TOPO_MSG_UNIT_MISSING ("MAP: drho");
   } /*if*/

    return (ret_code);

} /* kram_get_SpecUnits */
/*___________________________________________________________________________*/





/**************** funciton for checking topo ptr array of ARTMAP net
****************/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_TopoPtrArray (void)
{
   TopoPtrArray    topo_inpa_cmpa_sep,
                   topo_cmpa_reca_sep,
                   topo_reca_dela_sep,
                   topo_dela_rsta_sep,
                   topo_rsta_speca_sep,
                   topo_speca_inpb_sep,
                   topo_inpb_cmpb_sep,
                   topo_cmpb_recb_sep,
                   topo_recb_delb_sep,
                   topo_delb_rstb_sep,
                   topo_rstb_specb_sep,
                   topo_specb_map_sep,
                   topo_map_spec_sep,
                   topo_ptr_array_end;

   int             ret_code = KRERR_NO_ERROR;


   topo_inpa_cmpa_sep = topo_ptr_array + ArtMap_NoOfInpUnits_a + 1;
   topo_cmpa_reca_sep = topo_inpa_cmpa_sep + ArtMap_NoOfInpUnits_a + 1;
   topo_reca_dela_sep = topo_cmpa_reca_sep + ArtMap_NoOfRecUnits_a + 1;
   topo_dela_rsta_sep = topo_reca_dela_sep + ArtMap_NoOfRecUnits_a + 4;
   topo_rsta_speca_sep = topo_dela_rsta_sep + ArtMap_NoOfRecUnits_a + 1;
   topo_speca_inpb_sep = topo_rsta_speca_sep + ARTMAP_NO_OF_SPECa_UNITS + 1;
   topo_inpb_cmpb_sep = topo_speca_inpb_sep + ArtMap_NoOfInpUnits_b + 1;
   topo_cmpb_recb_sep = topo_inpb_cmpb_sep + ArtMap_NoOfInpUnits_b + 1;
   topo_recb_delb_sep = topo_cmpb_recb_sep + ArtMap_NoOfRecUnits_b + 1;
   topo_delb_rstb_sep = topo_recb_delb_sep + ArtMap_NoOfRecUnits_b + 4;
   topo_rstb_specb_sep = topo_delb_rstb_sep + ArtMap_NoOfRecUnits_b + 1;
   topo_specb_map_sep = topo_rstb_specb_sep + ARTMAP_NO_OF_SPECb_UNITS + 1;
   topo_map_spec_sep = topo_specb_map_sep + ArtMap_NoOfRecUnits_b + 1;

   topo_ptr_array_end = topo_map_spec_sep + ARTMAP_NO_OF_SPEC_UNITS + 1;

   if ((*topo_ptr_array != NULL)      ||
       (*topo_inpa_cmpa_sep != NULL)  ||
       (*topo_cmpa_reca_sep != NULL)  ||
       (*topo_reca_dela_sep != NULL)  ||
       (*topo_dela_rsta_sep != NULL)  ||
       (*topo_rsta_speca_sep != NULL) ||
       (*topo_speca_inpb_sep != NULL) ||
       (*topo_inpb_cmpb_sep != NULL)  ||
       (*topo_cmpb_recb_sep != NULL)  ||
       (*topo_recb_delb_sep != NULL)  ||
       (*topo_delb_rstb_sep != NULL)  ||
       (*topo_rstb_specb_sep != NULL) ||
       (*topo_specb_map_sep != NULL)  ||
       (*topo_map_spec_sep != NULL)   ||
       (*topo_ptr_array_end != NULL)  ||
       (*(topo_ptr_array_end+1) != NULL)
      )
   {
      ARTMAP_RETURN_NET_ERROR (ret_code);
   } /*if*/


   return (ret_code);


} /* kram_TopoPtrArray */
/*___________________________________________________________________________*/






/****************** function for checking sites of ARTMAP network
******************/


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_Sites (void)
{
   register struct Unit   *unit_ptr;
   register struct Site   *site_ptr;

   TopoPtrArray           topo_rsta_ptr,
                          topo_g1a_ptr,
                          topo_ria_ptr,
                          topo_rstb_ptr,
                          topo_g1b_ptr,
                          topo_rib_ptr,
                          topo_G_ptr,
                          topo_rb_ptr,
                          topo_qu_ptr;

   bool                   got_site1,
                          got_site2;

   krui_err               ret_code = KRERR_NO_ERROR;



   topo_rsta_ptr = topo_ptr_array + 2*ArtMap_NoOfInpUnits_a +
                   2*ArtMap_NoOfRecUnits_a + 8;
   topo_g1a_ptr  = topo_rsta_ptr + ArtMap_NoOfRecUnits_a + 3;
   topo_ria_ptr  = topo_g1a_ptr + 1;
   topo_rstb_ptr = topo_ria_ptr + 5 + 2*ArtMap_NoOfInpUnits_b +
                   2*ArtMap_NoOfRecUnits_b + 8;
   topo_g1b_ptr  = topo_rstb_ptr + ArtMap_NoOfRecUnits_b + 3;
   topo_rib_ptr  = topo_g1b_ptr + 1;
   topo_G_ptr    = topo_rib_ptr + 5 + ArtMap_NoOfRecUnits_b + 5;
   topo_rb_ptr   = topo_G_ptr + 1;
   topo_qu_ptr   = topo_rb_ptr + 4;

   /* check sites of local reset units of ARTa
   */
   while ((unit_ptr = *topo_rsta_ptr++) != NULL) {

      got_site1 = FALSE;
      got_site2 = FALSE;

      if (UNIT_HAS_SITES (unit_ptr)) {

         FOR_ALL_SITES (unit_ptr, site_ptr) {

            if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_RSTa_SELF)) && (!got_site1)) {
               got_site1 = TRUE;
               continue;
            } /*if*/

            if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_RSTa_SIGNAL)) && (!got_site2)) {
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



   /* Check sites of unit Gain 1 of ARTa */

   if (UNIT_HAS_DIRECT_INPUTS (*topo_g1a_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (*topo_g1a_ptr);
   } /*if*/

   got_site1 = FALSE;
   got_site2 = FALSE;

   FOR_ALL_SITES (*topo_g1a_ptr, site_ptr) {

      if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_INPa_G1a)) && (!got_site1)) {
         got_site1 = TRUE;
         continue;
      } /*if*/

      if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_RECa_G1a)) && (!got_site2)) {
         got_site2 = TRUE;
         continue;
      } /*if*/

      TOPO_MSG_SITE_FUNC (*topo_g1a_ptr);

   } /*FOR_ALL_SITES*/

   if (!got_site1 || !got_site2) {
      TOPO_MSG_SITE_MISSING  (*topo_g1a_ptr);
   } /*if*/


   /* Check sites of unit RIa */

   if (UNIT_HAS_DIRECT_INPUTS (*topo_ria_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (*topo_ria_ptr);
   } /*if*/

   got_site1 = FALSE;
   got_site2 = FALSE;

   FOR_ALL_SITES (*topo_ria_ptr, site_ptr) {

      if (CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_RIa)) {
         if (!got_site1) {
            got_site1 = TRUE;
         } else {
            if (!got_site2) {
               got_site2 = TRUE;
            } else {
               TOPO_MSG_SITE_FUNC (*topo_ria_ptr);
            } /*if*/
         } /*if*/
      } /*if*/

   } /*FOR_ALL_SITES*/

   if (!got_site1 || !got_site2) {
      TOPO_MSG_SITE_MISSING (*topo_ria_ptr);
   } /*if*/



   /* check sites of local reset units of ARTb
   */
   while ((unit_ptr = *topo_rstb_ptr++) != NULL) {

      got_site1 = FALSE;
      got_site2 = FALSE;

      if (UNIT_HAS_SITES (unit_ptr)) {

         FOR_ALL_SITES (unit_ptr, site_ptr) {

            if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_RSTb_SELF)) && (!got_site1)) {
               got_site1 = TRUE;
               continue;
            } /*if*/

            if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_RSTb_SIGNAL)) && (!got_site2)) {
               got_site2 = TRUE;
               continue;
            } /*if*/

            TOPO_MSG_SITE_FUNC (unit_ptr);

         } /*FOR_ALL_SITES*/


         if (!got_site1 || !got_site2) {
            TOPO_MSG_SITE_MISSING (unit_ptr);
         } /*if*/


      } else {

         TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (unit_ptr);

      } /*if*/

   } /*while*/



   /* Check sites of unit Gain 1 of ARTb */

   if (UNIT_HAS_DIRECT_INPUTS (*topo_g1b_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (*topo_g1b_ptr);
   } /*if*/

   got_site1 = FALSE;
   got_site2 = FALSE;

   FOR_ALL_SITES (*topo_g1b_ptr, site_ptr) {

      if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_INPb_G1b)) && (!got_site1)) {
         got_site1 = TRUE;
         continue;
      } /*if*/

      if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_RECb_G1b)) && (!got_site2)) {
         got_site2 = TRUE;
         continue;
      } /*if*/

      TOPO_MSG_SITE_FUNC (*topo_g1b_ptr);

   } /*FOR_ALL_SITES*/

   if (!got_site1 || !got_site2) {
      TOPO_MSG_SITE_MISSING (*topo_g1b_ptr);
   } /*if*/


   /* Check sites of unit RIb */

   if (UNIT_HAS_DIRECT_INPUTS (*topo_rib_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (*topo_rib_ptr);
   } /*if*/

   got_site1 = FALSE;
   got_site2 = FALSE;

   FOR_ALL_SITES (*topo_rib_ptr, site_ptr) {

      if (CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_RIb)) {
         if (!got_site1) {
            got_site1 = TRUE;
         } else {
            if (!got_site2) {
               got_site2 = TRUE;
            } else {
               TOPO_MSG_SITE_FUNC (*topo_rib_ptr);
            } /*if*/
         } /*if*/
      } /*if*/

   } /*FOR_ALL_SITES*/

   if (!got_site1 || !got_site2) {
      TOPO_MSG_SITE_MISSING (*topo_rib_ptr);
   } /*if*/




   /* Check sites of unit G */

   if (UNIT_HAS_DIRECT_INPUTS (*topo_G_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (*topo_G_ptr);
   } /*if*/

   got_site1 = FALSE;
   got_site2 = FALSE;

   FOR_ALL_SITES (*topo_G_ptr, site_ptr) {

      if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_ARTa_G)) && (!got_site1)) {
         got_site1 = TRUE;
         continue;
      } /*if*/

      if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_ARTb_G)) && (!got_site2)) {
         got_site2 = TRUE;
         continue;
      } /*if*/

      TOPO_MSG_SITE_FUNC (*topo_G_ptr);

   } /*FOR_ALL_SITES*/

   if (!got_site1 || !got_site2) {
      TOPO_MSG_SITE_MISSING (*topo_G_ptr);
   } /*if*/




   /* Check sites of unit rb */

   if (UNIT_HAS_DIRECT_INPUTS (*topo_rb_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (*topo_rb_ptr);
   } /*if*/

   got_site1 = FALSE;
   got_site2 = FALSE;

   FOR_ALL_SITES (*topo_rb_ptr, site_ptr) {

      if (CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_RB)) {
         if (!got_site1) {
            got_site1 = TRUE;
         } else {
            if (!got_site2) {
               got_site2 = TRUE;
            } else {
               TOPO_MSG_SITE_FUNC (*topo_rb_ptr);
            } /*if*/
         } /*if*/
      } /*if*/

   } /*FOR_ALL_SITES*/

   if (!got_site1 || !got_site2) {
      TOPO_MSG_SITE_MISSING (*topo_rb_ptr);
   } /*if*/




   /* Check sites of unit qu */

   if (UNIT_HAS_DIRECT_INPUTS (*topo_qu_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (*topo_qu_ptr);
   } /*if*/

   got_site1 = FALSE;
   got_site2 = FALSE;

   FOR_ALL_SITES (*topo_qu_ptr, site_ptr) {

      if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_CMPa_QU)) && (!got_site1)) {
         got_site1 = TRUE;
         continue;
      } /*if*/

      if ((CHECK_SITE_FUNC (site_ptr, ARTMAP_SITEF_INPa_QU)) && (!got_site2)) {
         got_site2 = TRUE;
         continue;
      } /*if*/

      TOPO_MSG_SITE_FUNC (*topo_qu_ptr);

   } /*FOR_ALL_SITES*/

   if (!got_site1 || !got_site2) {
      TOPO_MSG_SITE_MISSING (*topo_qu_ptr);
   } /*if*/




   return (ret_code);

} /* kram_Sites () */
/*___________________________________________________________________________*/









/*####################  functions for checking links of ARTa
####################*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_LinksToInpUnits_a (TopoPtrArray *topo_ptr)
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

} /* kram_LinksToInpUnits_a () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToCmpUnits_a (TopoPtrArray *topo_ptr)
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
            case ARTMAP_DELa_LAY:
               if (link_ptr->to->lun == ARTMAP_DELa_REC_UNIT) {
                  count_del++;
               } else {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               break;
            case ARTMAP_INPa_LAY:
               if (UNIT_REFRESHED (link_ptr->to)) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               link_ptr->to->flags |= UFLAG_REFRESH;
               count_inp++;
               break;
            case ARTMAP_SPECa_LAY:
               if (link_ptr->to->lun != ARTMAP_G1a_UNIT) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               count_spec++;
               break;
            default :
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /* switch */

         } /*FOR_ALL_LINKS*/

         if ((count_inp != 1) || (count_spec != 1) ||
             (count_del != ArtMap_NoOfRecUnits_a)
            )
         {
            TOPO_MSG_LINK_MISSING (unit_ptr);
         } /*if*/

      } /*if*/

   } /*while*/

   return (ret_code);

} /* kram_LinksToCmpUnits_a () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToRecUnits_a (TopoPtrArray *topo_ptr)
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
            case ARTMAP_CMPa_LAY:
               count_cmp++;
               break;
            case ARTMAP_RSTa_LAY:
               if (UNIT_REFRESHED (link_ptr->to)) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               link_ptr->to->flags |= UFLAG_REFRESH;
               count_rst++;
               break;
            case ARTMAP_SPECa_LAY:
               switch (link_ptr->to->lun) {
               case ARTMAP_RGa_UNIT:
                  count_rg++;
                  break;
               case ARTMAP_G2a_UNIT:
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
             (count_cmp != ArtMap_NoOfInpUnits_a)
            )
         {
           TOPO_MSG_LINK_MISSING  (unit_ptr);
         } /*if*/

      } /*if*/

   } /*while*/

   return (ret_code);

} /* kram_LinksToRecUnits_a () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToDelUnits_a (TopoPtrArray *topo_ptr)
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

         case ARTMAP_DELa_REC_UNIT:

            count_rec = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if (link_ptr->to->lln == ARTMAP_RECa_LAY) {

                  count_rec++;
                  if (UNIT_REFRESHED (link_ptr->to)) {
                     TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
                  } else {
                     link_ptr->to->flags |= UFLAG_REFRESH;
                  } /*if*/

               } else {

                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);

               } /*if*/

            } /*FOR_ALL_LINKS*/

            if (count_rec != 1) {
               TOPO_MSG_LINK_MISSING (unit_ptr);
            } /*if*/

            break;

         case ARTMAP_D1a_UNIT:

            count_del = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ARTMAP_DELa_LAY) &&
                   (link_ptr->to->lun == ARTMAP_DELa_REC_UNIT)
                  )
               {
                  count_del++;
               } else {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/

            } /*FOR_ALL_LINKS*/

            if (count_del != ArtMap_NoOfRecUnits_a) {
               TOPO_MSG_LINK_MISSING (unit_ptr);
            } /*if*/

            break;

         case ARTMAP_D2a_UNIT:

            count_del = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ARTMAP_DELa_LAY) &&
                   (link_ptr->to->lun == ARTMAP_D1a_UNIT)
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

         case ARTMAP_D3a_UNIT:

            count_del = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ARTMAP_DELa_LAY) &&
                   (link_ptr->to->lun == ARTMAP_D2a_UNIT)
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
            ARTMAP_RETURN_NET_ERROR (ret_code);
         } /* switch */

      } /*if*/

   } /*while*/

   return (ret_code);

} /* kram_LinksToDelUnits_a () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToRstUnits_a (TopoPtrArray *topo_ptr)
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
            case ARTMAP_RSTa_LAY:

               count_rst++;

               if (link_ptr->to != unit_ptr) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/

               site_ptr1 = site_ptr;

               if (site_ptr1 == site_ptr2) {
                  TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
               } /*if*/

               break;

            case ARTMAP_DELa_LAY:

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

            case ARTMAP_SPECa_LAY:

               if (link_ptr->to->lun != ARTMAP_RGa_UNIT) {
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

} /* kram_LinksToRstUnits_a () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToSpecUnits_a (TopoPtrArray *topo_ptr)
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
   int                      count_drho;

   krui_err                 ret_code = KRERR_NO_ERROR;


   /* topo_ptr points to classified unit which has two links:
      1. to d3a unit
      2. to reset general unit
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
      case ARTMAP_DELa_LAY:

         count_del++;

         if (link_ptr->to->lun != ARTMAP_D3a_UNIT) {
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ARTMAP_SPECa_LAY:

         switch (link_ptr->to->lun) {
         case ARTMAP_RGa_UNIT:
            count_rg++;
            break;
         case ARTMAP_G2a_UNIT:
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

   if ((count_del != 1) || (count_rg != 1) || count_g2 != 1) {
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

      if (link_ptr->to->lln != ARTMAP_RSTa_LAY) {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

      count_rst++;

   } /*FOR_ALL_LINKS*/

   if (count_rst != ArtMap_NoOfRecUnits_a) {
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

      case ARTMAP_RECa_LAY :

         count_rec++;

         if ((site_ptr2 != NULL) && (site_ptr2 != site_ptr)) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         site_ptr2 = site_ptr;

         if (site_ptr1 == site_ptr2) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ARTMAP_INPa_LAY :

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

   if ((count_inp != ArtMap_NoOfInpUnits_a) ||
       (count_rec != ArtMap_NoOfRecUnits_a)
      )
   {
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

      case ARTMAP_INPa_LAY:

         count_inp++;

         if ((site_ptr1 != NULL) && (site_ptr1 != site_ptr)) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         site_ptr1 = site_ptr;

         if (site_ptr1 == site_ptr2) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ARTMAP_SPECa_LAY:

         if (link_ptr->to->lun != ARTMAP_RHOa_UNIT) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
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

   if ((count_inp != ArtMap_NoOfInpUnits_a) || (count_rho != 1)) {
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

      if (link_ptr->to->lln != ARTMAP_CMPa_LAY) {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

      count_cmp++;

   } /*FOR_ALL_LINKS*/

   if (count_cmp != ArtMap_NoOfInpUnits_a) {
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

      if (link_ptr->to->lln != ARTMAP_SPECa_LAY) {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

      if (link_ptr->to->lun == ARTMAP_RIa_UNIT) {
         count_ri++;
         continue;
      } /*if*/

      if (link_ptr->to->lun == ARTMAP_RCa_UNIT) {
         count_rc++;
         continue;
      } /*if*/

      TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);

   } /*FOR_ALL_LINKS*/


   if ((count_ri != 1) || (count_rc != 1)) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/




   /* topo_ptr points to RHOa unit which has an incoming link
      from itself and one from the drho unit.
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   count_rho = 0;
   count_drho = 0;

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      switch (link_ptr->to->lln) {
      case ARTMAP_SPECa_LAY:
         if (link_ptr->to->lun == ARTMAP_RHOa_UNIT) {
            count_rho++;
         } else {
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         }  /*if*/
         break;
      case ARTMAP_SPEC_LAY:
         if (link_ptr->to->lun == ARTMAP_DRHO_UNIT) {
            count_drho++;
         } else {
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         } /*if*/
         break;
      default:
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*switch*/

   } /*FOR_ALL_LINKS*/

   if ((count_rho != 1) || (count_drho != 1)) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/




   /* topo_ptr points to g2a unit which has incoming links
      from all inpa units
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   count_inp = 0;

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

     if (link_ptr->to->lln == ARTMAP_INPa_LAY) {
        count_inp++;
     } else {
        TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
     } /*if*/

   } /*FOR_ALL_LINKS*/

   if (count_inp != ArtMap_NoOfInpUnits_a) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/

   (*topo_ptr)++;  /* increment topo_ptr to next used position in array */

   return (ret_code);

} /* kram_LinksToSpecUnits_a () */
/*___________________________________________________________________________*/





/************* functions for checking links of ARTb
*************/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err  kram_LinksToInpUnits_b (TopoPtrArray *topo_ptr)
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

} /* kram_LinksToInpUnits_b () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToCmpUnits_b (TopoPtrArray *topo_ptr)
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
            case ARTMAP_DELb_LAY:
               if (link_ptr->to->lun == ARTMAP_DELb_REC_UNIT) {
                  count_del++;
               } else {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               break;
            case ARTMAP_INPb_LAY:
               if (UNIT_REFRESHED (link_ptr->to)) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               link_ptr->to->flags |= UFLAG_REFRESH;
               count_inp++;
               break;
            case ARTMAP_SPECb_LAY:
               if (link_ptr->to->lun != ARTMAP_G1b_UNIT) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               count_spec++;
               break;
            default :
              TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /* switch */

         } /*FOR_ALL_LINKS*/

         if ((count_inp != 1) || (count_spec != 1) ||
             (count_del != ArtMap_NoOfRecUnits_b)
            )
         {
            TOPO_MSG_LINK_MISSING (unit_ptr);
         } /*if*/

      } /*if*/

   } /*while*/

   return (ret_code);

} /* kram_LinksToCmpUnits_b () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToRecUnits_b (TopoPtrArray *topo_ptr)
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
            case ARTMAP_CMPb_LAY:
               count_cmp++;
               break;
            case ARTMAP_RSTb_LAY:
               if (UNIT_REFRESHED (link_ptr->to)) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/
               link_ptr->to->flags |= UFLAG_REFRESH;
               count_rst++;
               break;
            case ARTMAP_SPECb_LAY:
               switch (link_ptr->to->lun) {
               case ARTMAP_RGb_UNIT:
                  count_rg++;
                  break;
               case ARTMAP_G2b_UNIT:
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
             (count_cmp != ArtMap_NoOfInpUnits_b)
            )
         {
            TOPO_MSG_LINK_MISSING  (unit_ptr);
         } /*if*/

      } /*if*/

   } /*while*/

   return (ret_code);

} /* kram_LinksToRecUnits_b () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToDelUnits_b (TopoPtrArray *topo_ptr)
{
   register struct Unit     *unit_ptr;
   register struct Link     *link_ptr;

   int                      count_rec;
   int                      count_del;
   int                      count_map;

   krui_err                 ret_code = KRERR_NO_ERROR;


   /* There are different cases to distinguish
      1. the delay unit is one that corresponds to a recogniton unit
         then
              - it has  one incoming link which comes from the corresponding
                recognition unit
              - and one incoming link, which comes from the corresponding
                map-unit.

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

         ARTMAP_RETURN_NET_ERROR (ret_code);

      } else {

         switch (unit_ptr->lun) {

         case ARTMAP_DELb_REC_UNIT:

            count_rec = 0;
            count_map = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               switch (link_ptr->to->lln) {
               case ARTMAP_RECb_LAY:
                  count_rec++;
                  if (UNIT_REFRESHED (link_ptr->to)) {
                     TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
                  } else {
                     link_ptr->to->flags |= UFLAG_REFRESH;
                  } /*if*/
                  break;
               case ARTMAP_MAP_LAY:
                  count_map++;
                  if (UNIT_REFRESHED (link_ptr->to)) {
                     TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
                  } else {
                     link_ptr->to->flags |= UFLAG_REFRESH;
                  } /*if*/
                  break;
               default:
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*switch*/

            } /*FOR_ALL_LINKS*/

            if ((count_rec != 1) || (count_map != 1)) {
               TOPO_MSG_LINK_MISSING (unit_ptr);
            } /*if*/

            break;

         case ARTMAP_D1b_UNIT:

            count_del = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ARTMAP_DELb_LAY) &&
                   (link_ptr->to->lun == ARTMAP_DELb_REC_UNIT)
                  )
               {
                  count_del++;
               } else {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/

            } /*FOR_ALL_LINKS*/

            if (count_del != ArtMap_NoOfRecUnits_b) {
               TOPO_MSG_LINK_MISSING (unit_ptr);
            } /*if*/

            break;

         case ARTMAP_D2b_UNIT:

            count_del = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ARTMAP_DELb_LAY) &&
                   (link_ptr->to->lun == ARTMAP_D1b_UNIT)
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

         case ARTMAP_D3b_UNIT:

            count_del = 0;

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               if ((link_ptr->to->lln == ARTMAP_DELb_LAY) &&
                   (link_ptr->to->lun == ARTMAP_D2b_UNIT)
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
            ARTMAP_RETURN_NET_ERROR (ret_code);
         } /* switch */

      } /*if*/

   } /*while*/

   return (ret_code);

} /* kram_LinksToDelUnits_b () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToRstUnits_b (TopoPtrArray *topo_ptr)
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
            case ARTMAP_RSTb_LAY:

               count_rst++;

               if (link_ptr->to != unit_ptr) {
                  TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
               } /*if*/

               site_ptr1 = site_ptr;

               if (site_ptr1 == site_ptr2) {
                  TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
               } /*if*/

               break;

            case ARTMAP_DELb_LAY:

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

            case ARTMAP_SPECb_LAY:

               if (link_ptr->to->lun != ARTMAP_RGb_UNIT) {
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

} /* kram_LinksToRstUnits_b () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToSpecUnits_b (TopoPtrArray *topo_ptr)
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


   /* topo_ptr points to classified unit which has two links:
      1. to d3b unit
      2. to reset general unit
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
      case ARTMAP_DELb_LAY:

         count_del++;

         if (link_ptr->to->lun != ARTMAP_D3b_UNIT) {
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ARTMAP_SPECb_LAY:

         switch (link_ptr->to->lun) {
         case ARTMAP_RGb_UNIT:
            count_rg++;
            break;
         case ARTMAP_G2b_UNIT:
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

   if ((count_del != 1) || (count_rg != 1) || (count_g2 != 1))
   {
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

      if (link_ptr->to->lln != ARTMAP_RSTb_LAY) {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

      count_rst++;

   } /*FOR_ALL_LINKS*/

   if (count_rst != ArtMap_NoOfRecUnits_b) {
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

      case ARTMAP_RECb_LAY :

         count_rec++;

         if ((site_ptr2 != NULL) && (site_ptr2 != site_ptr)) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         site_ptr2 = site_ptr;

         if (site_ptr1 == site_ptr2) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ARTMAP_INPb_LAY :

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

   if ((count_inp != ArtMap_NoOfInpUnits_b) ||
       (count_rec != ArtMap_NoOfRecUnits_b)
      )
   {
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

      case ARTMAP_INPb_LAY:

         count_inp++;

         if ((site_ptr1 != NULL) && (site_ptr1 != site_ptr)) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         site_ptr1 = site_ptr;

         if (site_ptr1 == site_ptr2) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ARTMAP_SPECb_LAY:

         if (link_ptr->to->lun != ARTMAP_RHOb_UNIT) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
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

   if ((count_inp != ArtMap_NoOfInpUnits_b) || (count_rho != 1)) {
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

      if (link_ptr->to->lln != ARTMAP_CMPb_LAY) {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

      count_cmp++;

   } /*FOR_ALL_LINKS*/

   if (count_cmp != ArtMap_NoOfInpUnits_b) {
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

      if (link_ptr->to->lln != ARTMAP_SPECb_LAY) {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

      if (link_ptr->to->lun == ARTMAP_RIb_UNIT) {
         count_ri++;
         continue;
      } /*if*/

      if (link_ptr->to->lun == ARTMAP_RCb_UNIT) {
         count_rc++;
         continue;
      } /*if*/

      TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);

   } /*FOR_ALL_LINKS*/


   if ((count_ri != 1) || (count_rc != 1)) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/




   /* topo_ptr points to RHOb unit which has an incoming links
      from itself
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   count_rho = 0;

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      switch (link_ptr->to->lln) {
      case ARTMAP_SPECb_LAY:
         if (link_ptr->to->lun == ARTMAP_RHOb_UNIT) {
            count_rho++;
         } else {
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         }  /*if*/
         break;
      default:
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*switch*/

   } /*FOR_ALL_LINKS*/

   if (count_rho != 1) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/




   /* topo_ptr points to g2b unit which has incoming links
      from all inpb units
   */

   unit_ptr = *(*topo_ptr)++;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   count_inp = 0;

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

     if (link_ptr->to->lln == ARTMAP_INPb_LAY) {
        count_inp++;
     } else {
        TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
     } /*if*/

   } /*FOR_ALL_LINKS*/

   if (count_inp != ArtMap_NoOfInpUnits_b) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/


   (*topo_ptr)++;  /* increment topo_ptr to next used position in array */

   return (ret_code);

} /* kram_LinksToSpecUnits_b () */
/*___________________________________________________________________________*/



/*********** functions to check links of map field of ARTMAP net
***********/


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToMapUnits (TopoPtrArray *topo_ptr)
{

   krui_err                  ret_code    = KRERR_NO_ERROR;

   register struct Unit      *unit_ptr;
   register struct Link      *link_ptr;

   int                       count_dela,
                             count_delb,
                             count_G;


   /* - each map unit is linked to all dela units
      - each map unit is linked to its corresponding delb unit
      - each map unit is linked to the G unit
   */

   krart_deleteTouchFlags();


   while ((unit_ptr = *(*topo_ptr)++) != NULL) {

      if (UNIT_HAS_SITES (unit_ptr)) {
         TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
      } /*if*/

      count_dela = 0;
      count_delb = 0;
      count_G    = 0;

      FOR_ALL_LINKS (unit_ptr, link_ptr) {

         switch (link_ptr->to->lln) {
         case ARTMAP_DELa_LAY:
            if (link_ptr->to->lun == ARTMAP_DELa_REC_UNIT) {
               count_dela++;
            } else {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            break;
         case ARTMAP_DELb_LAY:
            if ((link_ptr->to->lun == ARTMAP_DELb_REC_UNIT) &&
                (!UNIT_REFRESHED (link_ptr->to))
               )
            {
               count_delb++;
               link_ptr->to->flags |= UFLAG_REFRESH;
            } else {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            break;
         case ARTMAP_SPEC_LAY:
            if (link_ptr->to->lun == ARTMAP_G_UNIT) {
               count_G++;
            } else {
               TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            } /*if*/
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         } /*switch*/

      } /*FOR_ALL_LINKS*/

      if ((count_dela != ArtMap_NoOfRecUnits_a) ||
          (count_delb != 1) || (count_G != 1)
         )
      {
         TOPO_MSG_LINK_MISSING (unit_ptr);
      } /*if*/

   } /*while*/

   return (ret_code);

} /* kram_LinksToMapUnits () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static krui_err   kram_LinksToSpecUnits (TopoPtrArray *topo_ptr)
{
    krui_err                  ret_code  = KRERR_NO_ERROR;

    register struct Unit      *unit_ptr;
    register struct Site      *site_ptr;
    struct Site               *site_ptr1,
                              *site_ptr2;
    register struct Link      *link_ptr;

    TopoPtrArray              topo_d1_ptr,
                              topo_cl_ptr,
                              topo_nc_ptr,
                              topo_G_ptr,
                              topo_rb_ptr,
                              topo_rm_ptr,
                              topo_rg_ptr,
                              topo_rho_ptr,
                              topo_qu_ptr,
                              topo_drho_ptr;

    int                       count_delb,
                              count_d1,
                              count_cla,
                              count_clb,
                              count_nca,
                              count_ncb,
                              count_reca,
                              count_recb,
                              count_map,
                              count_rb,
                              count_rho,
                              count_rm,
                              count_inpa,
                              count_cmpa,
                              count_qu,
                              count_rhoa,
                              count_drho,
                              count_rg,
                              count_rga;

    /* let's set the special topo pointers
    */
    topo_d1_ptr      = *topo_ptr;
    topo_cl_ptr      = topo_d1_ptr + 1;
    topo_nc_ptr      = topo_cl_ptr + 1;
    topo_G_ptr       = topo_nc_ptr + 1;
    topo_rb_ptr      = topo_G_ptr + 1;
    topo_rm_ptr      = topo_rb_ptr + 1;
    topo_rg_ptr      = topo_rm_ptr + 1;
    topo_rho_ptr     = topo_rg_ptr + 1;
    topo_qu_ptr      = topo_rho_ptr + 1;
    topo_drho_ptr    = topo_qu_ptr + 1;


    /* first we check the links to the d1 unit :
       it has inputs from all delb units.
    */
    count_delb = 0;
    unit_ptr   = *topo_d1_ptr;

    if (UNIT_HAS_SITES (unit_ptr)) {
       TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
    } /*if*/

    FOR_ALL_LINKS (unit_ptr, link_ptr) {

       if ((link_ptr->to->lln == ARTMAP_DELb_LAY) &&
           (link_ptr->to->lun == ARTMAP_DELb_REC_UNIT)
          )
       {
          count_delb++;
       } else {
          TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
       } /*if*/

    } /*FOR_ALL_LINKS*/

    if (count_delb != ArtMap_NoOfRecUnits_b) {
       TOPO_MSG_LINK_MISSING (unit_ptr);
    } /*if*/


    /* next we check the links to the cl unit
       it has inputs from the cla, the clb unit and the rg unit
    */

    count_cla   = 0;
    count_clb   = 0;
    count_rg    = 0;
    unit_ptr    = *topo_cl_ptr;

    if (UNIT_HAS_SITES (unit_ptr)) {
       TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
    } /*if*/

    FOR_ALL_LINKS (unit_ptr, link_ptr) {

       switch (link_ptr->to->lln) {
       case ARTMAP_SPECa_LAY:
          if (link_ptr->to->lun == ARTMAP_CLa_UNIT) {
             count_cla++;
          } else {
             TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
          } /*if*/
          break;
       case ARTMAP_SPECb_LAY:
          if (link_ptr->to->lun == ARTMAP_CLb_UNIT) {
             count_clb++;
          } else {
             TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
          } /*if*/
          break;
       case ARTMAP_SPEC_LAY:
          if (link_ptr->to->lun == ARTMAP_RG_UNIT) {
             count_rg++;
          } else {
             TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
          } /*if*/
          break;
       default:
          TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
       } /*switch*/

    } /*FOR_ALL_LINKS*/

    if ((count_cla != 1) || (count_clb != 1) || (count_rg != 1)) {
       TOPO_MSG_LINK_MISSING (unit_ptr);
    } /*if*/


    /* next we check the links of the nc unit:
       it has inputs from the nca and the ncb unit
    */

    count_nca = 0;
    count_ncb = 0;
    unit_ptr  = *topo_nc_ptr;

    if (UNIT_HAS_SITES (unit_ptr)) {
       TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
    } /*if*/

    FOR_ALL_LINKS (unit_ptr, link_ptr) {

       switch (link_ptr->to->lln) {
       case ARTMAP_SPECa_LAY:
          if (link_ptr->to->lun == ARTMAP_NCa_UNIT) {
             count_nca++;
          } else {
             TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
          } /*if*/
          break;
       case ARTMAP_SPECb_LAY:
          if (link_ptr->to->lun == ARTMAP_NCb_UNIT) {
             count_ncb++;
          } else {
             TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
          } /*if*/
          break;
       default:
          TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
       } /*switch*/

    } /*FOR_ALL_LINKS*/

    if ((count_nca != 1) || (count_ncb != 1)) {
       TOPO_MSG_LINK_MISSING (unit_ptr);
    } /*if*/


    /* next we check the links to the G unit
       it has inputs from all reca to one of its sites and from the recb
       units to the other site.
    */

    count_reca   = 0;
    count_recb   = 0;
    unit_ptr     = *topo_G_ptr;
    site_ptr1 = NULL;
    site_ptr2 = NULL;

    if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {
       TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (unit_ptr);
    } /*if*/


   FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

      switch (link_ptr->to->lln) {

      case ARTMAP_RECb_LAY :

         count_recb++;

         if ((site_ptr2 != NULL) && (site_ptr2 != site_ptr)) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         site_ptr2 = site_ptr;

         if (site_ptr1 == site_ptr2) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ARTMAP_RECa_LAY :

         count_reca++;

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

   if ((count_reca != ArtMap_NoOfRecUnits_a) ||
       (count_recb != ArtMap_NoOfRecUnits_b)
      )
   {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/



   /* next we check the links of the rb unit
      which  has one link from the d1 unit to one of its sites and
      one link from the rho unit to the other site.
   */


    count_d1     = 0;
    count_rho    = 0;
    unit_ptr     = *topo_rb_ptr;
    site_ptr1 = NULL;
    site_ptr2 = NULL;

    if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {
       TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (unit_ptr);
    } /*if*/


   FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

      if (link_ptr->to->lln == ARTMAP_SPEC_LAY) {

         switch (link_ptr->to->lun) {
         case ARTMAP_D1_UNIT:
            count_d1++;

            if ((site_ptr2 != NULL) && (site_ptr2 != site_ptr)) {
               TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
            } /*if*/

            site_ptr2 = site_ptr;

            if (site_ptr1 == site_ptr2) {
               TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
            } /*if*/

            break;

         case ARTMAP_RHO_UNIT:
            count_rho++;

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

      } else {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

   } /*FOR_ALL_SITES_AND_LINKS*/

   if ((count_d1 != 1) || (count_rho != 1)) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/


   /* next we check the links to the rm unit
      it has inputs from all map units
   */

   count_map  = 0;
   unit_ptr   = *topo_rm_ptr;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      if (link_ptr->to->lln == ARTMAP_MAP_LAY) {
         count_map++;
      } else {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

   } /*FOR_ALL_LINKS*/

   if (count_map != ArtMap_NoOfRecUnits_b) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/



   /* next we check the links to the rg unit
      it has inputs from the rb and the rm unit
   */

   count_rb   = 0;
   count_rm   = 0;
   unit_ptr   = *topo_rg_ptr;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      switch (link_ptr->to->lln) {

      case ARTMAP_SPEC_LAY :
         switch (link_ptr->to->lun) {
         case ARTMAP_RB_UNIT:
            count_rb++;
            break;
         case ARTMAP_RM_UNIT:
            count_rm++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         } /*switch*/
         break;

      default:
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         break;

      } /*switch*/

   } /*FOR_ALL_LINKS*/

   if ((count_rb != 1) || (count_rm != 1)) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/


   /* next we check the links to the rho unit
      it has inputs just from itself
   */

   count_rho = 0;
   unit_ptr  = *topo_rho_ptr;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      if (unit_ptr == link_ptr->to) {
         count_rho++;
      } else {
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*if*/

   } /*FOR_ALL_LINKS*/

   if (count_rho != 1) {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/



   /* next we check the links to the qu unit
      it has inputs from all inpa units to one of its sites and
      from all cmpa units to the other site.
   */

   count_inpa  = 0;
   count_cmpa  = 0;
   unit_ptr    = *topo_qu_ptr;
   site_ptr1 = NULL;
   site_ptr2 = NULL;

   if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_DIRECT_INPUTS (unit_ptr);
   } /*if*/


   FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

      switch (link_ptr->to->lln) {

      case ARTMAP_INPa_LAY :

         count_inpa++;

         if ((site_ptr2 != NULL) && (site_ptr2 != site_ptr)) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         site_ptr2 = site_ptr;

         if (site_ptr1 == site_ptr2) {
            TOPO_MSG_LINK_TO_WRONG_SITE (link_ptr->to, unit_ptr);
         } /*if*/

         break;

      case ARTMAP_CMPa_LAY :

         count_cmpa++;

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

   if ((count_inpa != ArtMap_NoOfInpUnits_a) ||
       (count_cmpa != ArtMap_NoOfInpUnits_a)
      )
   {
      TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/


   /* finally we check the links to the drho unit
      it has inputs from itself, from the rg unit, from the qu unit
      and from the rhoa unit
   */

   count_drho   = 0;
   count_rg     = 0;
   count_qu     = 0;
   count_rhoa   = 0;
   count_rga    = 0;
   count_clb    = 0;
   unit_ptr     = *topo_drho_ptr;

   if (UNIT_HAS_SITES (unit_ptr)) {
      TOPO_MSG_UNEXPECTED_SITES (unit_ptr);
   } /*if*/

   FOR_ALL_LINKS (unit_ptr, link_ptr) {

      switch (link_ptr->to->lln) {
      case ARTMAP_SPEC_LAY:

         switch (link_ptr->to->lun) {
         case ARTMAP_DRHO_UNIT:
            count_drho++;
            break;
         case ARTMAP_RG_UNIT:
            count_rg++;
            break;
         case ARTMAP_QU_UNIT:
            count_qu++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
         } /*switch*/

         break;

      case ARTMAP_SPECa_LAY:

         switch (link_ptr->to->lun) {
         case ARTMAP_RHOa_UNIT:
            count_rhoa++;
            break;
         case ARTMAP_RGa_UNIT:
            count_rga++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/

         break;

      case ARTMAP_SPECb_LAY:

         switch (link_ptr->to->lun) {
         case ARTMAP_CLb_UNIT:
            count_clb++;
            break;
         default:
            TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
            break;
         } /*switch*/

         break;

      default:
         TOPO_MSG_UNEXPECTED_LINK (link_ptr->to, unit_ptr);
      } /*switch*/

   } /*FOR_ALL_LINKS*/

   if ((count_drho != 1) || (count_rg   != 1) ||
       (count_qu   != 1) || (count_rhoa != 1) ||
       (count_rga  != 1) || (count_clb  != 1)
      )
   {
       TOPO_MSG_LINK_MISSING (unit_ptr);
   } /*if*/

   return (ret_code);


} /* kram_LinksToSpecUnits () */
/*___________________________________________________________________________*/




/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/* NOTE: Don't call this function unless net has been topologically sorted
         with type ARTMAP_TOPO_TYPE
*/

static krui_err kram_init_fix_weights (void)
{

   int                    ret_code  = KRERR_NO_ERROR;

   register struct Unit   *unit_ptr;
   register struct Site   *site_ptr;
   register struct Link   *link_ptr;




   FOR_ALL_UNITS (unit_ptr) {

      if (UNIT_HAS_INPUTS (unit_ptr)) {

         if (UNIT_HAS_DIRECT_INPUTS (unit_ptr)) {

            FOR_ALL_LINKS (unit_ptr, link_ptr) {

               kram_set_fix_weight (link_ptr->to, unit_ptr, &(link_ptr->weight));

            } /*FOR_ALL_LINKS*/

         } else {

            FOR_ALL_SITES_AND_LINKS (unit_ptr, site_ptr, link_ptr) {

               kram_set_fix_weight (link_ptr->to, unit_ptr, &(link_ptr->weight));

            } /*FOR_ALL_SITES_AND_LINKS*/

         } /*if*/

      } /*if*/

   } /*FOR_ALL_UNITS*/


   return (ret_code);

} /* kram_init_fix_weights () */
/*___________________________________________________________________________*/



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
static void  kram_set_fix_weight (struct Unit *src_unit, struct Unit *trgt_unit, FlintType *weight)
{
   switch (src_unit->lln) {

   case ARTMAP_INPa_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_CMPa_LAY:
         *weight = ARTMAP_LINK_INPa_CMPa;
         break;
      case ARTMAP_SPECa_LAY:
         switch (trgt_unit->lun) {
         case ARTMAP_G1a_UNIT:
            *weight = ARTMAP_LINK_INPa_G1a;
            break;
         case ARTMAP_RIa_UNIT:
            *weight = ARTMAP_LINK_INPa_RIa;
            break;
         case ARTMAP_G2a_UNIT:
            *weight = ARTMAP_LINK_INPa_G2a;
            break;
         } /*switch*/
         break;
      case ARTMAP_SPEC_LAY:
         *weight = ARTMAP_LINK_INPa_QU;
         break;
      } /*switch*/

      break;

   case ARTMAP_CMPa_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_SPECa_LAY:
         *weight = ARTMAP_LINK_CMPa_RCa;
         break;
      case ARTMAP_SPEC_LAY:
         *weight = ARTMAP_LINK_CMPa_QU;
         break;
      } /*switch*/

      break;

   case ARTMAP_RECa_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_DELa_LAY:
         *weight = ARTMAP_LINK_RECa_DELa;
         break;
      case ARTMAP_SPECa_LAY:
         *weight = ARTMAP_LINK_RECa_G1a;
         break;
      case ARTMAP_SPEC_LAY:
         *weight = ARTMAP_LINK_RECa_G;
         break;
      } /*switch*/

      break;

   case ARTMAP_DELa_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_DELa_LAY:
         *weight = ARTMAP_LINK_DELa_DELa;
         break;
      case ARTMAP_RSTa_LAY:
         *weight = ARTMAP_LINK_DELa_RSTa;
         break;
      case ARTMAP_SPECa_LAY:
         *weight = ARTMAP_LINK_DELa_CLa;
         break;
      } /*switch*/

      break;

   case ARTMAP_RSTa_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_RECa_LAY:
         *weight = ARTMAP_LINK_RSTa_RECa;
         break;
      case ARTMAP_RSTa_LAY:
         *weight = ARTMAP_LINK_RSTa_RSTa;
         break;
      case ARTMAP_SPECa_LAY:
         *weight = ARTMAP_LINK_RSTa_NCa;
         break;
      } /*switch*/

      break;

   case ARTMAP_SPECa_LAY:

      switch (src_unit->lun) {

      case ARTMAP_CLa_UNIT:

         *weight = ARTMAP_LINK_CLa_CL;
         break;

      case ARTMAP_NCa_UNIT:

         *weight = ARTMAP_LINK_NCa_NC;
         break;

      case ARTMAP_G1a_UNIT:

         *weight = ARTMAP_LINK_G1a_CMPa;
         break;

      case ARTMAP_RIa_UNIT:

         *weight = ARTMAP_LINK_RIa_RGa;
         break;

      case ARTMAP_RCa_UNIT:

         *weight = ARTMAP_LINK_RCa_RGa;
         break;

      case ARTMAP_RGa_UNIT:

         switch (trgt_unit->lln) {
         case ARTMAP_RECa_LAY:
            *weight = ARTMAP_LINK_RGa_RECa;
            break;
         case ARTMAP_RSTa_LAY:
            *weight = ARTMAP_LINK_RGa_RSTa;
            break;
         case ARTMAP_SPECa_LAY:
            *weight = ARTMAP_LINK_RGa_CLa;
            break;
         case ARTMAP_SPEC_LAY:
            *weight = ARTMAP_LINK_RGa_DRHO;
         } /*switch*/
         break;

      case ARTMAP_RHOa_UNIT:

         switch (trgt_unit->lln) {
         case ARTMAP_SPECa_LAY:
            switch (trgt_unit->lun) {
            case ARTMAP_RIa_UNIT:
               *weight = ARTMAP_LINK_RHOa_RIa;
               break;
            case ARTMAP_RHOa_UNIT:
               *weight = ARTMAP_LINK_RHOa_RHOa;
               break;
            } /*switch*/
            break;
         case ARTMAP_SPEC_LAY:
            *weight = ARTMAP_LINK_RHOa_DRHO;
            break;
         } /*switch*/

         break;

      case ARTMAP_G2a_UNIT:

         switch (trgt_unit->lln) {
         case ARTMAP_RECa_LAY:
            *weight = ARTMAP_LINK_G2a_RECa;
            break;
         case ARTMAP_SPECa_LAY:
            *weight = ARTMAP_LINK_G2a_CLa;
            break;
         } /*switch*/

         break;

      } /*switch*/

      break;

   case ARTMAP_INPb_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_CMPb_LAY:
         *weight = ARTMAP_LINK_INPb_CMPb;
         break;
      case ARTMAP_SPECb_LAY:
         switch (trgt_unit->lun) {
         case ARTMAP_G1b_UNIT:
            *weight = ARTMAP_LINK_INPb_G1b;
            break;
         case ARTMAP_RIb_UNIT:
            *weight = ARTMAP_LINK_INPb_RIb;
            break;
         case ARTMAP_G2b_UNIT:
            *weight = ARTMAP_LINK_INPb_G2b;
            break;
         } /*switch*/
         break;
      } /*switch*/

      break;

   case ARTMAP_CMPb_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_SPECb_LAY:
         *weight = ARTMAP_LINK_CMPb_RCb;
         break;
      } /*switch*/

   case ARTMAP_RECb_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_DELb_LAY:
         *weight = ARTMAP_LINK_RECb_DELb;
         break;
      case ARTMAP_SPECb_LAY:
         *weight = ARTMAP_LINK_RECb_G1b;
         break;
      case ARTMAP_SPEC_LAY:
         *weight = ARTMAP_LINK_RECb_G;
         break;
      } /*switch*/

      break;

   case ARTMAP_DELb_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_DELb_LAY:
         *weight = ARTMAP_LINK_DELb_DELb;
         break;
      case ARTMAP_RSTb_LAY:
         *weight = ARTMAP_LINK_DELb_RSTb;
         break;
      case ARTMAP_SPECb_LAY:
         *weight = ARTMAP_LINK_DELb_CLb;
         break;
      case ARTMAP_MAP_LAY:
         *weight = ARTMAP_LINK_DELb_MAP;
         break;
      case ARTMAP_SPEC_LAY:
         *weight = ARTMAP_LINK_DELb_D1;
         break;
      } /*switch*/

      break;

   case ARTMAP_RSTb_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_RECb_LAY:
         *weight = ARTMAP_LINK_RSTb_RECb;
         break;
      case ARTMAP_RSTb_LAY:
         *weight = ARTMAP_LINK_RSTb_RSTb;
         break;
      case ARTMAP_SPECb_LAY:
         *weight = ARTMAP_LINK_RSTb_NCb;
         break;
      } /*switch*/

      break;

   case ARTMAP_SPECb_LAY:

      switch (src_unit->lun) {

      case ARTMAP_CLb_UNIT:

         switch (trgt_unit->lun) {
         case ARTMAP_CL_UNIT:
            *weight = ARTMAP_LINK_CLb_CL;
            break;
         case ARTMAP_DRHO_UNIT:
            *weight = ARTMAP_LINK_CLb_DRHO;
            break;
         } /*switch*/

         break;

      case ARTMAP_NCa_UNIT:

         *weight = ARTMAP_LINK_NCb_NC;
         break;

      case ARTMAP_G1b_UNIT:

         *weight = ARTMAP_LINK_G1b_CMPb;
         break;

      case ARTMAP_RIb_UNIT:

         *weight = ARTMAP_LINK_RIb_RGb;
         break;

      case ARTMAP_RCb_UNIT:

         *weight = ARTMAP_LINK_RCb_RGb;
         break;

      case ARTMAP_RGb_UNIT:

         switch (trgt_unit->lln) {
         case ARTMAP_RECb_LAY:
            *weight = ARTMAP_LINK_RGb_RECb;
            break;
         case ARTMAP_RSTb_LAY:
            *weight = ARTMAP_LINK_RGb_RSTb;
            break;
         case ARTMAP_SPECb_LAY:
            *weight = ARTMAP_LINK_RGb_CLb;
            break;
         } /*switch*/
         break;

      case ARTMAP_RHOb_UNIT:

         switch (trgt_unit->lun) {
         case ARTMAP_RIb_UNIT:
            *weight = ARTMAP_LINK_RHOa_RIa;
            break;
         case ARTMAP_RHOb_UNIT:
            *weight = ARTMAP_LINK_RHOa_RHOa;
            break;
         } /*switch*/

         break;

      case ARTMAP_G2b_UNIT:

         switch (trgt_unit->lln) {
         case ARTMAP_RECb_LAY:
            *weight = ARTMAP_LINK_G2b_RECb;
            break;
         case ARTMAP_SPECb_LAY:
            *weight = ARTMAP_LINK_G2b_CLb;
            break;
         } /*switch*/

         break;

      } /*switch*/

      break;


   case ARTMAP_MAP_LAY:

      switch (trgt_unit->lln) {
      case ARTMAP_SPEC_LAY:
         *weight = ARTMAP_LINK_MAP_RM;
         break;
      case ARTMAP_DELb_LAY:
         *weight = ARTMAP_LINK_MAP_DELb;
         break;
      } /*switch*/

      break;

   case ARTMAP_SPEC_LAY:

      switch (src_unit->lun) {

      case ARTMAP_D1_UNIT:

         *weight = ARTMAP_LINK_D1_RB;
         break;

      case ARTMAP_G_UNIT:

         *weight = ARTMAP_LINK_G_MAP;
         break;

      case ARTMAP_RB_UNIT:

         *weight = ARTMAP_LINK_RB_RG;
         break;

      case ARTMAP_RM_UNIT:

         *weight = ARTMAP_LINK_RM_RG;
         break;

      case ARTMAP_RG_UNIT:

         switch (trgt_unit->lun) {
         case ARTMAP_CL_UNIT:
            *weight = ARTMAP_LINK_RG_CL;
            break;
         case ARTMAP_DRHO_UNIT:
            *weight = ARTMAP_LINK_RG_DRHO;
            break;
         } /*switch*/

         break;

      case ARTMAP_RHO_UNIT:

         switch (trgt_unit->lun) {
         case ARTMAP_RB_UNIT:
            *weight = ARTMAP_LINK_RHO_RB;
            break;
         case ARTMAP_RHO_UNIT:
            *weight = ARTMAP_LINK_RHO_RHO;
            break;
         } /*switch*/

         break;

      case ARTMAP_QU_UNIT:

         *weight = ARTMAP_LINK_QU_DRHO;
         break;

      case ARTMAP_DRHO_UNIT:

         switch (trgt_unit->lln) {
         case ARTMAP_SPECa_LAY:
            *weight = ARTMAP_LINK_DRHO_RHOa;
            break;
         case ARTMAP_SPEC_LAY:
            *weight = ARTMAP_LINK_DRHO_DRHO;
            break;
         } /* switch */

         break;

      } /*switch*/

      break;

   } /*switch*/

} /* kram_set_fix_weight () */
/*___________________________________________________________________________*/

/************************************************************************************

   This file is part of SnnsCLib, a fork of the kernel and parts of the gui of 
   the Stuttgart Neural Network Simulator (SNNS), version 4.3.

   The file's original version is part of SNNS 4.3. It's source code can be found at

   http://www.ra.cs.uni-tuebingen.de/SNNS/

   SNNS 4.3 is under the license LGPL v2. We note that source code files of SNNS 4.3 
   state as version "4.2". Base of this fork is SNNS 4.3 with a reverse-applied 
   python patch (see http://developer.berlios.de/projects/snns-dev/).

   SnnsCLib was developed in 2010 by Christoph Bergmeir under supervision of 
   José M. Benítez, both affiliated to DiCITS Lab, Sci2s group, DECSAI, 
   University of Granada

   Changes done to the original code were performed with the objective to
   port it from C to C++ and to encapsulate all code in one class named SnnsCLib.

   Changes in header files mainly include:
   * removed all static keywords
   * moved initializations of variables to the constructor of SnnsCLib

   Changes in cpp code files mainly include:
   * changed file ending from .c to .cpp
   * removed all SNNS internal includes and only include SnnsCLib   
   * static variables within functions were turned into member variables of SnnsCLib
   * function declarations were changed to method declarations, i.e. "SnnsCLib::.."
     was added
   * calls to the function table are now "C++-style", using the "this"-pointer

   License of SnnsCLib:
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

************************************************************************************/


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_artmap.c,v $
  SHORTNAME      : bn_artmap
  SNNS VERSION   : 4.2

  PURPOSE        : Creates the ARTMAP-window and ARTMAP networks. 
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 15.1.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "SnnsCLib.h"


/* Definition of the Output Function */
#define OUTFUNC    const_cast<char*>("Out_Identity") 


/* Definition of the Activationfunctions */
#define ACTF_INPa const_cast<char*>("Act_Identity") 
#define ACTF_CMPa const_cast<char*>("Act_at_least_2")
#define ACTF_RECa const_cast<char*>("Act_Identity")
#define ACTF_DELa const_cast<char*>("Act_at_least_2")
#define ACTF_Da   const_cast<char*>("Act_at_least_1")
#define ACTF_RSTa const_cast<char*>("Act_at_least_1")
#define ACTF_G1a  const_cast<char*>("Act_at_least_2")
#define ACTF_RIa  const_cast<char*>("Act_Product")
#define ACTF_RCa  const_cast<char*>("Act_Identity")
#define ACTF_RGa  const_cast<char*>("Act_less_than_0")
#define ACTF_CLa  const_cast<char*>("Act_at_least_1")
#define ACTF_NCa  const_cast<char*>("Act_ARTMAP_NCa")
#define ACTF_RHOa const_cast<char*>("Act_Identity")
#define ACTF_G2a  const_cast<char*>("Act_at_most_0")


#define ACTF_INPb const_cast<char*>("Act_Identity")
#define ACTF_CMPb const_cast<char*>("Act_at_least_2")
#define ACTF_RECb const_cast<char*>("Act_Identity")
#define ACTF_DELb const_cast<char*>("Act_at_least_2")
#define ACTF_Db   const_cast<char*>("Act_at_least_1")
#define ACTF_RSTb const_cast<char*>("Act_at_least_1")
#define ACTF_G1b  const_cast<char*>("Act_at_least_2")
#define ACTF_RIb  const_cast<char*>("Act_Product")
#define ACTF_RCb  const_cast<char*>("Act_Identity")
#define ACTF_RGb  const_cast<char*>("Act_less_than_0")
#define ACTF_CLb  const_cast<char*>("Act_at_least_1")
#define ACTF_NCb  const_cast<char*>("Act_ARTMAP_NCb")
#define ACTF_RHOb const_cast<char*>("Act_Identity")
#define ACTF_G2b  const_cast<char*>("Act_at_most_0")

#define ACTF_MAP  const_cast<char*>("Act_at_least_2")
#define ACTF_D1   const_cast<char*>("Act_Identity")
#define ACTF_RB   const_cast<char*>("Act_Product")
#define ACTF_RM   const_cast<char*>("Act_Identity")
#define ACTF_RG   const_cast<char*>("Act_less_than_0")
#define ACTF_G    const_cast<char*>("Act_exactly_1")
#define ACTF_CL   const_cast<char*>("Act_at_least_2")
#define ACTF_NC   const_cast<char*>("Act_at_least_1")
#define ACTF_RHO  const_cast<char*>("Act_Identity")
#define ACTF_QU   const_cast<char*>("Act_Product")
#define ACTF_DRHO const_cast<char*>("Act_ARTMAP_DRho")



/* Site names
*/
#define SITE_NAME_RSTa_SELF    const_cast<char*>("ArtMap_Site_Rsta_Self")
#define SITE_NAME_RSTa_SIGNAL  const_cast<char*>("ArtMap_Site_Rsta_Signal")
#define SITE_NAME_REC_G1a      const_cast<char*>("ArtMap_Site_Rec_G1a")
#define SITE_NAME_INP_G1a      const_cast<char*>("ArtMap_Site_Inp_G1a")
#define SITE_NAME_INP_RIa      const_cast<char*>("ArtMap_Site_Inp_RIa")
#define SITE_NAME_RHO_RIa      const_cast<char*>("ArtMap_Site_Rho_RIa")

#define SITE_NAME_RSTb_SELF    const_cast<char*>("ArtMap_Site_Rstb_Self")
#define SITE_NAME_RSTb_SIGNAL  const_cast<char*>("ArtMap_Site_Rstb_Signal")
#define SITE_NAME_REC_G1b      const_cast<char*>("ArtMap_Site_Rec_G1b")
#define SITE_NAME_INP_G1b      const_cast<char*>("ArtMap_Site_Inp_G1b")
#define SITE_NAME_INP_RIb      const_cast<char*>("ArtMap_Site_Inp_RIb")
#define SITE_NAME_RHO_RIb      const_cast<char*>("ArtMap_Site_Rho_RIb")

#define SITE_NAME_ARTa_G       const_cast<char*>("ArtMap_Site_ARTa_G")
#define SITE_NAME_ARTb_G       const_cast<char*>("ArtMap_Site_ARTb_G")
#define SITE_NAME_B_RB         const_cast<char*>("ArtMap_Site_B_RB")
#define SITE_NAME_RHO_RB       const_cast<char*>("ArtMap_Site_Rho_RB")
#define SITE_NAME_CMPa_QU      const_cast<char*>("ArtMap_Site_Cmpa_QU")
#define SITE_NAME_INPa_QU      const_cast<char*>("ArtMap_Site_Inpa_QU")

/* Site function names
*/
#define SITE_FUNC_RSTa_SELF    const_cast<char*>("Site_WeightedSum")
#define SITE_FUNC_RSTa_SIGNAL  const_cast<char*>("Site_at_least_2")
#define SITE_FUNC_REC_G1a      const_cast<char*>("Site_at_most_0")
#define SITE_FUNC_INP_G1a      const_cast<char*>("Site_at_least_1")
#define SITE_FUNC_INP_RIa      const_cast<char*>("Site_WeightedSum")
#define SITE_FUNC_RHO_RIa      const_cast<char*>("Site_WeightedSum")

#define SITE_FUNC_RSTb_SELF    const_cast<char*>("Site_WeightedSum")
#define SITE_FUNC_RSTb_SIGNAL  const_cast<char*>("Site_at_least_2")
#define SITE_FUNC_REC_G1b      const_cast<char*>("Site_at_most_0")
#define SITE_FUNC_INP_G1b      const_cast<char*>("Site_at_least_1")
#define SITE_FUNC_INP_RIb      const_cast<char*>("Site_WeightedSum")
#define SITE_FUNC_RHO_RIb      const_cast<char*>("Site_WeightedSum")

#define SITE_FUNC_ARTa_G       const_cast<char*>("Site_at_least_1")
#define SITE_FUNC_ARTb_G       const_cast<char*>("Site_at_least_1")
#define SITE_FUNC_B_RB         const_cast<char*>("Site_WeightedSum")
#define SITE_FUNC_RHO_RB       const_cast<char*>("Site_WeightedSum")
#define SITE_FUNC_CMPa_QU      const_cast<char*>("Site_WeightedSum")
#define SITE_FUNC_INPa_QU      const_cast<char*>("Site_Reciprocal")

/* Learning function name */
#define LEARN_FUNC_NAME     const_cast<char*>("ARTMAP")

/* Update function name */
#define UPDATE_FUNC_NAME    const_cast<char*>("ARTMAP_Stable")



/* Macros
*/


/* returns 0 if x<=0 and 1 if x>0
*/
#define POS_SIGN(x)            (((x) > 0) ? 1 : 0)


#define NO_OF_SPECIAL_ab_UNITS   8   /* d1a to d3a (d1b to d3b) excluded */
#define NO_OF_SPECIAL_UNITS      10   /* including d1 unit */

/* Absolute Unit-Nummer berechnet aus ihrer relativen Nummer im Layer */
#define INPa_UNIT(y)            y
#define CMPa_UNIT(y)            INPa_UNIT(f1aUnits) + y
#define RECa_UNIT(y)            CMPa_UNIT(f1aUnits) + y
#define DELa_UNIT(y)            RECa_UNIT(f2aUnits) + y
#define RSTa_UNIT(y)            DELa_UNIT(f2aUnits) + 3 + y

#define MAP_UNIT(y)             RSTa_UNIT(f2aUnits) + NO_OF_SPECIAL_ab_UNITS + y

#define INPb_UNIT(y)            MAP_UNIT(f2bUnits) + NO_OF_SPECIAL_UNITS + y
#define CMPb_UNIT(y)            INPb_UNIT(f1bUnits) + y
#define RECb_UNIT(y)            CMPb_UNIT(f1bUnits) + y
#define DELb_UNIT(y)            RECb_UNIT(f2bUnits) + y
#define RSTb_UNIT(y)            DELb_UNIT(f2bUnits) + 3 + y

#define Da_UNIT(y)              DELa_UNIT(f2aUnits) + y
#define Db_UNIT(y)              DELb_UNIT(f2bUnits) + y
#define D1_UNIT                 MAP_UNIT(f2bUnits) + 1

#define SPECIAL_UNITS_a         RSTa_UNIT(f2aUnits) + 1
#define SPECIAL_UNITS_b         RSTb_UNIT(f2bUnits) + 1
#define SPECIAL_UNITS           D1_UNIT + 1

#define G1a_UNIT                 SPECIAL_UNITS_a
#define G1b_UNIT                 SPECIAL_UNITS_b
#define G_UNIT                   SPECIAL_UNITS

#define RIa_UNIT                 SPECIAL_UNITS_a + 1
#define RIb_UNIT                 SPECIAL_UNITS_b + 1
#define RB_UNIT                  SPECIAL_UNITS + 1

#define RCa_UNIT                 SPECIAL_UNITS_a + 2
#define RCb_UNIT                 SPECIAL_UNITS_b + 2
#define RM_UNIT                  SPECIAL_UNITS + 2

#define RGa_UNIT                 SPECIAL_UNITS_a + 3
#define RGb_UNIT                 SPECIAL_UNITS_b + 3
#define RG_UNIT                  SPECIAL_UNITS + 3

#define CLa_UNIT                 SPECIAL_UNITS_a + 4
#define CLb_UNIT                 SPECIAL_UNITS_b + 4
#define CL_UNIT                  SPECIAL_UNITS + 4

#define NCa_UNIT                 SPECIAL_UNITS_a + 5
#define NCb_UNIT                 SPECIAL_UNITS_b + 5
#define NC_UNIT                  SPECIAL_UNITS + 5

#define RHOa_UNIT                SPECIAL_UNITS_a + 6
#define RHOb_UNIT                SPECIAL_UNITS_b + 6
#define RHO_UNIT                 SPECIAL_UNITS + 6

#define G2a_UNIT                 SPECIAL_UNITS_a + 7
#define G2b_UNIT                 SPECIAL_UNITS_b + 7

#define QU_UNIT                  SPECIAL_UNITS + 7
#define DRHO_UNIT                SPECIAL_UNITS + 8


/* Calculates the number of columns when no of units and no. of rows is given
*/
#define NO_OF_COLS(units,rows)  ((int) (units/rows) + POS_SIGN(units % rows))

//already defined in cc_mac.h
//#define MAX(x,y)                ((x) > (y) ? (x) : (y))

/* No of columns for F1a, F2a, F1b, F2b */
#define F1a_COLS                 NO_OF_COLS(f1aUnits, f1aRows)
#define F2a_COLS                 NO_OF_COLS(f2aUnits, f2aRows)
#define F1b_COLS                 NO_OF_COLS(f1bUnits, f1bRows)
#define F2b_COLS                 NO_OF_COLS(f2bUnits, f2bRows)

/* Unit Positions */
#define OFFSET_X                 0
#define OFFSET_Y                 0
#define INPa_X                   OFFSET_X + 1
#define INPa_Y                   OFFSET_Y + 8
#define CMPa_X                   INPa_X + MAX(F1a_COLS,F1b_COLS) + 1
#define CMPa_Y                   INPa_Y
#define RECa_X                   CMPa_X + MAX(F1a_COLS,F1b_COLS) + 3
#define RECa_Y                   CMPa_Y
#define DELa_X                   RECa_X + MAX(F2a_COLS,F2b_COLS) + 1
#define DELa_Y                   RECa_Y
#define RSTa_X                   DELa_X + MAX(F2a_COLS,F2b_COLS) + 1
#define RSTa_Y                   DELa_Y
#define D1a_X                    DELa_X
#define D1a_Y                    DELa_Y + MAX(f1aRows,f2aRows) + 1
#define D2a_X                    (RSTa_X + DELa_X) / 2
#define D2a_Y                    D1a_Y
#define D3a_X                    RSTa_X
#define D3a_Y                    D1a_Y
#define G1a_X                    CMPa_X
#define G1a_Y                    CMPa_Y + MAX(f1aRows,f2aRows) + 1
#define RIa_X                    INPa_X
#define RIa_Y                    INPa_Y - 2
#define RCa_X                    CMPa_X
#define RCa_Y                    RIa_Y
#define RGa_X                    RCa_X
#define RGa_Y                    RCa_Y - 2
#define CLa_X                    RSTa_X + MAX(F2a_COLS,F2b_COLS) + 1
#define CLa_Y                    RSTa_Y + MAX(f1aRows, f2aRows) + 1
#define NCa_X                    CLa_X
#define NCa_Y                    CLa_Y - 2
#define RHOa_X                   RIa_X
#define RHOa_Y                   RIa_Y - 2
#define G2a_X                    RECa_X
#define G2a_Y                    G1a_Y

#define MAP_X                    RSTa_X + MAX(F2a_COLS, F2b_COLS) + 9
#define MAP_Y                    RSTa_Y + (MAX(f1aRows,f2aRows) + 6) / 2
#define D1_X                     MAP_X - 2
#define D1_Y                     RSTb_Y + f2bRows - 1
#define RB_X                     MAP_X - 2
#define RB_Y                     MAP_Y - 2
#define RM_X                     MAP_X
#define RM_Y                     RB_Y
#define RG_X                     RM_X
#define RG_Y                     RGa_Y
#define CL_X                     MAP_X - 6
#define CL_Y                     (CLa_Y + CLb_Y) / 2
#define NC_X                     CL_X + 2
#define NC_Y                     CL_Y
#define G_X                      MAP_X
#define G_Y                      MAP_Y + f2bRows + 1
#define RHO_X                    RB_X
#define RHO_Y                    RB_Y - 2
#define QU_X                     RECa_X
#define QU_Y                     RCa_Y
#define DRHO_X                   QU_X
#define DRHO_Y                   RGa_Y

#define INPb_X                   INPa_X
#define INPb_Y                   INPa_Y + MAX(f1aRows,f2aRows) + 5
#define CMPb_X                   CMPa_X
#define CMPb_Y                   INPb_Y
#define RECb_X                   RECa_X
#define RECb_Y                   CMPb_Y
#define DELb_X                   DELa_X
#define DELb_Y                   RECb_Y
#define RSTb_X                   RSTa_X
#define RSTb_Y                   DELb_Y
#define D1b_X                    D1a_X
#define D1b_Y                    DELb_Y - 2
#define D2b_X                    D2a_X
#define D2b_Y                    D1b_Y
#define D3b_X                    D3a_X
#define D3b_Y                    D2b_Y
#define G1b_X                    G1a_X
#define G1b_Y                    CMPb_Y - 2
#define RIb_X                    RIa_X
#define RIb_Y                    INPb_Y + MAX(f1bRows,f2bRows) + 1
#define RCb_X                    RCa_X
#define RCb_Y                    RIb_Y
#define RGb_X                    RGa_X
#define RGb_Y                    RCb_Y + 2
#define CLb_X                    CLa_X
#define CLb_Y                    D3b_Y
#define NCb_X                    NCa_X
#define NCb_Y                    CLb_Y + 2
#define RHOb_X                   RHOa_X
#define RHOb_Y                   RIb_Y + 2
#define G2b_X                    RECb_X
#define G2b_Y                    G1b_Y

#define STRING_LENGTH            30

/* checks whether value is not equal to KRERR_NO_ERROR and if so, returns
*/
#define CHECK_RETURN(ret_code)  if (ret_code != KRERR_NO_ERROR) \
                                   return (ret_code)



/*****************************************************************************
  FUNCTION : bn_artmap_make_layer

  PURPOSE  : make a complete layer
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/
krui_err SnnsCLib::bn_artmap_make_layer (int  units, int  rows, int  cols, 
				      int  Xbegin, int  Ybegin,
				      char *name_prefix, int  unit_ttype, 
				      char *actfuncname, char *outfuncname,
				      int  NoOfSites, char **SiteNames,
				      char **SiteFuncNames)

{
   krui_err             ret_code = KRERR_NO_ERROR;

   int                  row;
   int                  col;
   int                  count;
   int                  last_col;
   int                  unit_no;
   struct PosType       unit_pos;
   char                 *site_name, *site_func_name;
   int                  site_no;
   char                 name[100];

   unit_pos.z = 0;

   /* Create Site Table Entries */
   site_no = 1;
   while (site_no <= NoOfSites) {
      site_name= *(SiteNames+(site_no-1));
      site_func_name= *(SiteFuncNames+(site_no-1));

      if ((site_name != NULL) && (site_func_name != NULL)) {
         ret_code = krui_createSiteTableEntry(site_name, site_func_name);
         CHECK_RETURN (ret_code);
      } /*if*/

      site_no++;
   }  /*while*/

   /* Create Units */
   for (row = 1, count = 0; row <= rows; row++) {

      if ((units % rows) > 0) {
         last_col = POS_SIGN((units-count) % (rows-row+1));
      } else {
         last_col = 1;
      } /*if*/

      for (col = 1; col <= (cols-1+last_col); col++) {

         count++;

         unit_no = krui_createDefaultUnit();
         if (unit_no < 0)  CHECK_RETURN (unit_no);

         ret_code = krui_setUnitTType (unit_no, unit_ttype);
         CHECK_RETURN( ret_code );

         ret_code = krui_setUnitActFunc(unit_no, actfuncname);
         CHECK_RETURN (ret_code);

         ret_code = krui_setUnitOutFunc(unit_no, outfuncname);
         CHECK_RETURN (ret_code);

         if (units > 1) {
            sprintf (name,"%s%d",name_prefix,count);
         } else {
            sprintf (name,"%s",name_prefix);
         } /*if*/

         ret_code = krui_setUnitName (unit_no, name);
         CHECK_RETURN (ret_code);

         unit_pos.x = Xbegin + col - 1;
         unit_pos.y = Ybegin + row - 1;
         krui_setUnitPosition( unit_no, &unit_pos );

         ret_code = krui_setCurrentUnit (unit_no);
         CHECK_RETURN (ret_code);

         /* Add Sites */
         site_no = 1;
         while (site_no <= NoOfSites) {
             site_name= *(SiteNames+(site_no-1));

             if (site_name != NULL) {
                ret_code = krui_addSite (site_name);
                CHECK_RETURN (ret_code);
             } /*if*/

             site_no++;
         } /*while*/

      } /*for*/

   } /*for*/

   return (ret_code);
} 



/*****************************************************************************
  FUNCTION : bn_artmap_createNet

  PURPOSE  : Generation of the ARTMAP network using SNNS kernel functions.
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/
krui_err SnnsCLib::bn_artmap_createNet(int f1aUnits, int f1aRows, int f2aUnits, 
				    int f2aRows, int f1bUnits, int f1bRows, 
				    int f2bUnits, int f2bRows)

{
   krui_err          ret_code   = KRERR_NO_ERROR;

   int               NoOfSites;

   char              **SiteName, **SiteFunc;

   int               i, j;

   /* Allocate memory for the to pointer Arrays for site names
      and sitefunc names
   */
   SiteName      = (char **) malloc (2*sizeof(char *));
   SiteFunc      = (char **) malloc (2*sizeof(char *));

   if ((SiteName == NULL) || (SiteFunc == NULL)) {
      return (KRERR_INSUFFICIENT_MEM);
   } /*if*/
   /* Allocate memory to save Site names and Sitefunc names */
   *SiteName     = (char *)malloc (STRING_LENGTH);
   *(SiteName+1) = (char *)malloc (STRING_LENGTH);
   *SiteFunc     = (char *)malloc (STRING_LENGTH);
   *(SiteFunc+1) = (char *)malloc (STRING_LENGTH);

   if ((*SiteName == NULL) || (*(SiteName+1) == NULL) ||
       (*SiteFunc == NULL) || (*(SiteFunc+1) == NULL)
      )
   {
      return (KRERR_INSUFFICIENT_MEM);
   } /*if*/


   /********* create ARTa units ******************/

   /* create Input Layer for ARTa */
   ret_code = bn_artmap_make_layer (f1aUnits, f1aRows, F1a_COLS, INPa_X, INPa_Y, const_cast<char*>("inpa"),
                                    INPUT, ACTF_INPa, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Comparison Layer for ARTa */
   ret_code = bn_artmap_make_layer (f1aUnits, f1aRows, F1a_COLS, CMPa_X, CMPa_Y, const_cast<char*>("cmpa"),
                                    HIDDEN, ACTF_CMPa, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Recognition Layer for ARTa */
   ret_code = bn_artmap_make_layer (f2aUnits, f2aRows, F2a_COLS, RECa_X, RECa_Y, const_cast<char*>("reca"),
                                    SPECIAL, ACTF_RECa, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Delay Layer for ARTa */
   ret_code = bn_artmap_make_layer (f2aUnits, f2aRows, F2a_COLS, DELa_X, DELa_Y, const_cast<char*>("dela"),
                                    HIDDEN, ACTF_DELa, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d1a */
   ret_code = bn_artmap_make_layer (1, 1, 1, D1a_X, D1a_Y, const_cast<char*>("d1a"), HIDDEN, ACTF_Da,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d2a */
   ret_code = bn_artmap_make_layer (1, 1, 1, D2a_X, D2a_Y, const_cast<char*>("d2a"), HIDDEN, ACTF_Da,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d3a */
   ret_code = bn_artmap_make_layer (1, 1, 1, D3a_X, D3a_Y, const_cast<char*>("d3a"), HIDDEN, ACTF_Da,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create local reset layer */
   NoOfSites = 2;
   strcpy(*SiteName,     SITE_NAME_RSTa_SELF);
   strcpy(*(SiteName+1), SITE_NAME_RSTa_SIGNAL);
   strcpy(*SiteFunc,     SITE_FUNC_RSTa_SELF);
   strcpy(*(SiteFunc+1), SITE_FUNC_RSTa_SIGNAL);

   ret_code = bn_artmap_make_layer (f2aUnits, f2aRows, F2a_COLS, RSTa_X, RSTa_Y, const_cast<char*>("rsta"),
                                    HIDDEN, ACTF_RSTa, OUTFUNC, NoOfSites, SiteName,
                                    SiteFunc);
   CHECK_RETURN (ret_code);


   /* create g1a */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_REC_G1a);
   strcpy(*(SiteName+1), SITE_NAME_INP_G1a);
   strcpy(*SiteFunc    , SITE_FUNC_REC_G1a);
   strcpy(*(SiteFunc+1), SITE_FUNC_INP_G1a);

   ret_code = bn_artmap_make_layer (1, 1, 1, G1a_X, G1a_Y, const_cast<char*>("g1a"), HIDDEN, ACTF_G1a,
                                    OUTFUNC, NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create ria */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_INP_RIa);
   strcpy(*(SiteName+1), SITE_NAME_RHO_RIa);
   strcpy(*SiteFunc    , SITE_FUNC_INP_RIa);
   strcpy(*(SiteFunc+1), SITE_FUNC_RHO_RIa);

   ret_code = bn_artmap_make_layer (1, 1, 1, RIa_X, RIa_Y, const_cast<char*>("ria"), HIDDEN, ACTF_RIa,
                                    OUTFUNC, NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create rca */
   ret_code = bn_artmap_make_layer (1, 1, 1, RCa_X, RCa_Y, const_cast<char*>("rca"), HIDDEN, ACTF_RCa,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);


   /* create rga */
   ret_code = bn_artmap_make_layer (1, 1, 1, RGa_X, RGa_Y, const_cast<char*>("rga"), HIDDEN, ACTF_RGa,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create cla */
   ret_code = bn_artmap_make_layer (1, 1, 1, CLa_X, CLa_Y, const_cast<char*>("cla"), HIDDEN, ACTF_CLa,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create nca */
   ret_code = bn_artmap_make_layer (1, 1, 1, NCa_X, NCa_Y, const_cast<char*>("nca"), HIDDEN, ACTF_NCa,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create rhoa */
   ret_code = bn_artmap_make_layer (1, 1, 1, RHOa_X, RHOa_Y, const_cast<char*>("rhoa"), HIDDEN, ACTF_RHOa,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create g2a */
   ret_code = bn_artmap_make_layer (1, 1, 1, G2a_X, G2a_Y, const_cast<char*>("g2a"), HIDDEN, ACTF_G2a,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);



   /********* create MAP-Field units ******************/

   /* create map layer */
   ret_code = bn_artmap_make_layer (f2bUnits, f2bRows, F2b_COLS, MAP_X, MAP_Y, const_cast<char*>("map"), HIDDEN,
                                    ACTF_MAP, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d1 unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, D1_X, D1_Y, const_cast<char*>("d1"), HIDDEN, ACTF_D1, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create G unit */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_ARTa_G);
   strcpy(*(SiteName+1), SITE_NAME_ARTb_G);
   strcpy(*SiteFunc    , SITE_FUNC_ARTa_G);
   strcpy(*(SiteFunc+1), SITE_FUNC_ARTb_G);

   ret_code = bn_artmap_make_layer (1, 1, 1, G_X, G_Y, const_cast<char*>("G"), HIDDEN, ACTF_G, OUTFUNC,
                                    NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create rb unit */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_B_RB);
   strcpy(*(SiteName+1), SITE_NAME_RHO_RB);
   strcpy(*SiteFunc    , SITE_FUNC_B_RB);
   strcpy(*(SiteFunc+1), SITE_FUNC_RHO_RB);

   ret_code = bn_artmap_make_layer (1, 1, 1, RB_X, RB_Y, const_cast<char*>("rb"), HIDDEN, ACTF_RB, OUTFUNC,
                                    NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create rm unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, RM_X, RM_Y, const_cast<char*>("rm"), HIDDEN, ACTF_RM, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create rg unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, RG_X, RG_Y, const_cast<char*>("rg"), HIDDEN, ACTF_RG, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create cl unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, CL_X, CL_Y, const_cast<char*>("cl"), HIDDEN, ACTF_CL, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create nc unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, NC_X, NC_Y, const_cast<char*>("nc"), HIDDEN, ACTF_NC, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create rho unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, RHO_X, RHO_Y, const_cast<char*>("rho"), HIDDEN, ACTF_RHO, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create qu unit */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_INPa_QU);
   strcpy(*(SiteName+1), SITE_NAME_CMPa_QU);
   strcpy(*SiteFunc    , SITE_FUNC_INPa_QU);
   strcpy(*(SiteFunc+1), SITE_FUNC_CMPa_QU);

   ret_code = bn_artmap_make_layer (1, 1, 1, QU_X, QU_Y, const_cast<char*>("qu"), HIDDEN, ACTF_QU, OUTFUNC,
                                    NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create drho unit */
   ret_code = bn_artmap_make_layer (1, 1, 1, DRHO_X, DRHO_Y, const_cast<char*>("drho"), HIDDEN, ACTF_DRHO, OUTFUNC,
                                    0, NULL, NULL);
   CHECK_RETURN (ret_code);



   /********* create ARTb units ******************/


   /* create Input Layer for ARTb */
   ret_code = bn_artmap_make_layer (f1bUnits, f1bRows, F1b_COLS, INPb_X, INPb_Y, const_cast<char*>("inpb"),
                                    INPUT, ACTF_INPb, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Comparison Layer for ARTb */
   ret_code = bn_artmap_make_layer (f1bUnits, f1bRows, F1b_COLS, CMPb_X, CMPb_Y, const_cast<char*>("cmpb"),
                                    HIDDEN, ACTF_CMPb, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Recognition Layer for ARTb */
   ret_code = bn_artmap_make_layer (f2bUnits, f2bRows, F2b_COLS, RECb_X, RECb_Y, const_cast<char*>("recb"),
                                    SPECIAL, ACTF_RECb, OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Delay Layer for ARTa */
   ret_code = bn_artmap_make_layer (f2bUnits, f2bRows, F2b_COLS, DELb_X, DELb_Y, const_cast<char*>("delb"),
                                    HIDDEN, ACTF_DELb, OUTFUNC, 0, NULL, NULL);

   /* create d1b */
   ret_code = bn_artmap_make_layer (1, 1, 1, D1b_X, D1b_Y, const_cast<char*>("d1b"), HIDDEN, ACTF_Db,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d2b */
   ret_code = bn_artmap_make_layer (1, 1, 1, D2b_X, D2b_Y, const_cast<char*>("d2b"), HIDDEN, ACTF_Db,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create d3b */
   ret_code = bn_artmap_make_layer (1, 1, 1, D3b_X, D3b_Y, const_cast<char*>("d3b"), HIDDEN, ACTF_Db,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create local reset layer */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_RSTb_SELF);
   strcpy(*(SiteName+1), SITE_NAME_RSTb_SIGNAL);
   strcpy(*SiteFunc    , SITE_FUNC_RSTb_SELF);
   strcpy(*(SiteFunc+1), SITE_FUNC_RSTb_SIGNAL);

   ret_code = bn_artmap_make_layer (f2bUnits, f2bRows, F2b_COLS, RSTb_X, RSTb_Y, const_cast<char*>("rstb"),
                                    HIDDEN, ACTF_RSTb, OUTFUNC, NoOfSites, SiteName,
                                    SiteFunc);
   CHECK_RETURN (ret_code);


   /* create g1b */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_REC_G1b);
   strcpy(*(SiteName+1), SITE_NAME_INP_G1b);
   strcpy(*SiteFunc    , SITE_FUNC_REC_G1b);
   strcpy(*(SiteFunc+1), SITE_FUNC_INP_G1b);

   ret_code = bn_artmap_make_layer (1, 1, 1, G1b_X, G1b_Y, const_cast<char*>("g1b"), HIDDEN, ACTF_G1b,
                                    OUTFUNC, NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create rib */
   NoOfSites = 2;
   strcpy(*SiteName    , SITE_NAME_INP_RIb);
   strcpy(*(SiteName+1), SITE_NAME_RHO_RIb);
   strcpy(*SiteFunc    , SITE_FUNC_INP_RIb);
   strcpy(*(SiteFunc+1), SITE_FUNC_RHO_RIb);

   ret_code = bn_artmap_make_layer (1, 1, 1, RIb_X, RIb_Y, const_cast<char*>("rib"), HIDDEN, ACTF_RIb,
                                    OUTFUNC, NoOfSites, SiteName, SiteFunc);
   CHECK_RETURN (ret_code);

   /* create rcb */
   ret_code = bn_artmap_make_layer (1, 1, 1, RCb_X, RCb_Y, const_cast<char*>("rcb"), HIDDEN, ACTF_RCb,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);


   /* create rgb */
   ret_code = bn_artmap_make_layer (1, 1, 1, RGb_X, RGb_Y, const_cast<char*>("rgb"), HIDDEN, ACTF_RGb,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create clb */
   ret_code = bn_artmap_make_layer (1, 1, 1, CLb_X, CLb_Y, const_cast<char*>("clb"), HIDDEN, ACTF_CLb,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create ncb */
   ret_code = bn_artmap_make_layer (1, 1, 1, NCb_X, NCb_Y, const_cast<char*>("ncb"), HIDDEN, ACTF_NCb,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create rhob */
   ret_code = bn_artmap_make_layer (1, 1, 1, RHOb_X, RHOb_Y, const_cast<char*>("rhob"), HIDDEN, ACTF_RHOb,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create g2b */
   ret_code = bn_artmap_make_layer (1, 1, 1, G2b_X, G2b_Y, const_cast<char*>("g2b"), HIDDEN, ACTF_G2b,
                                    OUTFUNC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);



   /* Make Connections now */


   /********* Create Links **************************************/


   /********* Links to ARTa units ***********************/

   /* TO cmpa units */
   for (i=1; i<=f1aUnits; i++) {

      ret_code = krui_setCurrentUnit (CMPa_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM inpa units */
      ret_code = krui_createLink (INPa_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

      /* FROM g1a unit */
      ret_code = krui_createLink (G1a_UNIT, 0.0);
      CHECK_RETURN (ret_code);

      /* FROM dela units */
      for (j=1; j<=f2aUnits; j++) {
         ret_code = krui_createLink (DELa_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

   } /*for*/


   /* TO reca units */
   for (i=1; i<=f2aUnits; i++) {

      ret_code = krui_setCurrentUnit (RECa_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM cmpa units */
      for (j=1; j<=f1aUnits; j++) {
         ret_code = krui_createLink (CMPa_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

      /* FROM rga unit */
      ret_code = krui_createLink (RGa_UNIT, 0.0);
      CHECK_RETURN (ret_code);

      /* FROM rsta units */
      ret_code = krui_createLink (RSTa_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM g2a unit */
      ret_code = krui_createLink (G2a_UNIT, 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO dela units */
   for (i=1; i<=f2aUnits; i++) {

      ret_code = krui_setCurrentUnit (DELa_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM reca units */
      ret_code = krui_createLink (RECa_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO rsta units */
   for (i=1; i<=f2aUnits; i++) {

      ret_code = krui_setCurrentUnit (RSTa_UNIT(i));
      CHECK_RETURN (ret_code);

      /* first Site */
      ret_code = krui_setSite (SITE_NAME_RSTa_SELF);
      CHECK_RETURN (ret_code);

      /* FROM itself */
      ret_code = krui_createLink (RSTa_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* second Site */
      ret_code = krui_setSite (SITE_NAME_RSTa_SIGNAL);
      CHECK_RETURN (ret_code);

      /* FROM dela units */
      ret_code = krui_createLink (DELa_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM rga unit */
      ret_code = krui_createLink (RGa_UNIT, 0.0);
      CHECK_RETURN (ret_code);


   } /*for*/


   /* TO d1a unit */
   ret_code = krui_setCurrentUnit (Da_UNIT(1));
   CHECK_RETURN (ret_code);

   /* FROM dela units */
   for (j=1; j<=f2aUnits; j++) {
      ret_code = krui_createLink (DELa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO d2a unit */
   ret_code = krui_setCurrentUnit (Da_UNIT(2));
   CHECK_RETURN (ret_code);

   /* FROM d1a unit */
   ret_code = krui_createLink (Da_UNIT(1), 0.0);
   CHECK_RETURN (ret_code);


   /* TO d3a unit */
   ret_code = krui_setCurrentUnit (Da_UNIT(3));
   CHECK_RETURN (ret_code);

   /* FROM d2a unit */
   ret_code = krui_createLink (Da_UNIT(2), 0.0);
   CHECK_RETURN (ret_code);


   /* TO g1a unit */
   ret_code = krui_setCurrentUnit (G1a_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_REC_G1a);
   CHECK_RETURN (ret_code);

   /* FROM reca units */
   for (j=1; j<=f2aUnits; j++) {
      ret_code = krui_createLink (RECa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_INP_G1a);
   CHECK_RETURN (ret_code);

   /* FROM inpa units */
   for (j=1; j<=f1aUnits; j++) {
      ret_code = krui_createLink (INPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO ria unit */
   ret_code = krui_setCurrentUnit (RIa_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_INP_RIa);
   CHECK_RETURN (ret_code);

   /* FROM inpa units */
   for (j=1; j<=f1aUnits; j++) {
      ret_code = krui_createLink (INPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_RHO_RIa);
   CHECK_RETURN (ret_code);

   /* from rhoa unit */
   ret_code = krui_createLink (RHOa_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO rca unit */
   ret_code = krui_setCurrentUnit (RCa_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM cmpa units */
   for (j=1; j<=f1aUnits; j++) {
      ret_code = krui_createLink (CMPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rga unit */
   ret_code = krui_setCurrentUnit (RGa_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM ria unit */
   ret_code = krui_createLink (RIa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rca unit */
   ret_code = krui_createLink (RCa_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO cla unit */
   ret_code = krui_setCurrentUnit (CLa_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM d3a unit */
   ret_code = krui_createLink (Da_UNIT(3), 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rga unit */
   ret_code = krui_createLink (RGa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM g2a unit */
   ret_code = krui_createLink (G2a_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO nca unit */
   ret_code = krui_setCurrentUnit (NCa_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM rsta units */
   for (j=1; j<=f2aUnits; j++) {
      ret_code = krui_createLink (RSTa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* TO rhoa unit */
   ret_code = krui_setCurrentUnit (RHOa_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM itself */
   ret_code = krui_createLink (RHOa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM drho unit */
   ret_code = krui_createLink (DRHO_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO g2a unit */
   ret_code = krui_setCurrentUnit (G2a_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM inpa units */
   for (j = 1; j <= f1aUnits; j++) {
      ret_code = krui_createLink (INPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /********* Links to MAP Fiels units ***********************/

   /* TO map units */
   for (i=1; i<=f2bUnits; i++) {

      ret_code = krui_setCurrentUnit (MAP_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM dela units */
      for (j=1; j<=f2aUnits; j++) {
         ret_code = krui_createLink (DELa_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

      /* FROM delb unit */
      ret_code = krui_createLink (DELb_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM G unit */
      ret_code = krui_createLink (G_UNIT, 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO d1 unit */
   ret_code = krui_setCurrentUnit (D1_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM delb units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (DELb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO G unit */
   ret_code = krui_setCurrentUnit (G_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_ARTa_G);
   CHECK_RETURN (ret_code);

   /* FROM reca units */
   for (j=1; j<=f2aUnits; j++) {
      ret_code = krui_createLink (RECa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_ARTb_G);
   CHECK_RETURN (ret_code);

   /* FROM recb units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (RECb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rb unit */
   ret_code = krui_setCurrentUnit (RB_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_B_RB);
   CHECK_RETURN (ret_code);

   /* FROM d1 unit */
   ret_code = krui_createLink (D1_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_RHO_RB);
   CHECK_RETURN (ret_code);

   /* from rho unit */
   ret_code = krui_createLink (RHO_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO rm unit */
   ret_code = krui_setCurrentUnit (RM_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM map units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (MAP_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rg unit */
   ret_code = krui_setCurrentUnit (RG_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM rb unit */
   ret_code = krui_createLink (RB_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rm unit */
   ret_code = krui_createLink (RM_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO cl unit */
   ret_code = krui_setCurrentUnit (CL_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM cla unit */
   ret_code = krui_createLink (CLa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM clb unit */
   ret_code = krui_createLink (CLb_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rg unit */
   ret_code = krui_createLink (RG_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO nc unit */
   ret_code = krui_setCurrentUnit (NC_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM nca unit */
   ret_code = krui_createLink (NCa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM ncb unit */
   ret_code = krui_createLink (NCb_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* TO rho unit */
   ret_code = krui_setCurrentUnit (RHO_UNIT);
   CHECK_RETURN (ret_code);

   /* from itself */
   ret_code = krui_createLink (RHO_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO qu unit */
   ret_code = krui_setCurrentUnit (QU_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_INPa_QU);
   CHECK_RETURN (ret_code);

   /* FROM inpa units */
   for (j=1; j <= f1aUnits; j++) {
      ret_code = krui_createLink (INPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_CMPa_QU);
   CHECK_RETURN (ret_code);

   /* from cmpa units */
   for (j=1; j<=f1aUnits; j++) {
      ret_code = krui_createLink (CMPa_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO drho unit */
   ret_code = krui_setCurrentUnit (DRHO_UNIT);
   CHECK_RETURN (ret_code);

   /* from itself */
   ret_code = krui_createLink (DRHO_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* from rg unit */
   ret_code = krui_createLink (RG_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* from qu unit */
   ret_code = krui_createLink (QU_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* from rhoa unit */
   ret_code = krui_createLink (RHOa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* from rga unit */
   ret_code = krui_createLink (RGa_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* from clb unit */
   ret_code = krui_createLink (CLb_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /********* Links to ARTb units ***********************/

   /* TO cmpb units */
   for (i=1; i<=f1bUnits; i++) {

      ret_code = krui_setCurrentUnit (CMPb_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM inpb units */
      ret_code = krui_createLink (INPb_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

      /* FROM g1b unit */
      ret_code = krui_createLink (G1b_UNIT, 0.0);
      CHECK_RETURN (ret_code);

      /* FROM delb units */
      for (j=1; j<=f2bUnits; j++) {
         ret_code = krui_createLink (DELb_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

   } /*for*/


   /* TO recb units */
   for (i=1; i<=f2bUnits; i++) {

      ret_code = krui_setCurrentUnit (RECb_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM cmpb units */
      for (j=1; j<=f1bUnits; j++) {
         ret_code = krui_createLink (CMPb_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

      /* FROM rgb unit */
      ret_code = krui_createLink (RGb_UNIT, 0.0);
      CHECK_RETURN (ret_code);

      /* FROM rstb units */
      ret_code = krui_createLink (RSTb_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM g2b unit */
      ret_code = krui_createLink (G2b_UNIT, 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO delb units */
   for (i=1; i<=f2bUnits; i++) {

      ret_code = krui_setCurrentUnit (DELb_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM recb units */
      ret_code = krui_createLink (RECb_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM map units */
      ret_code = krui_createLink (MAP_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO rstb units */
   for (i=1; i<=f2bUnits; i++) {

      ret_code = krui_setCurrentUnit (RSTb_UNIT(i));
      CHECK_RETURN (ret_code);

      /* first Site */
      ret_code = krui_setSite (SITE_NAME_RSTb_SELF);
      CHECK_RETURN (ret_code);

      /* FROM itself */
      ret_code = krui_createLink (RSTb_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* second Site */
      ret_code = krui_setSite (SITE_NAME_RSTb_SIGNAL);
      CHECK_RETURN (ret_code);

      /* FROM delb units */
      ret_code = krui_createLink (DELb_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM rgb unit */
      ret_code = krui_createLink (RGb_UNIT, 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO d1b unit */
   ret_code = krui_setCurrentUnit (Db_UNIT(1));
   CHECK_RETURN (ret_code);

   /* FROM delb units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (DELb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO d2b unit */
   ret_code = krui_setCurrentUnit (Db_UNIT(2));
   CHECK_RETURN (ret_code);

   /* FROM d1b unit */
   ret_code = krui_createLink (Db_UNIT(1), 0.0);
   CHECK_RETURN (ret_code);


   /* TO d3b unit */
   ret_code = krui_setCurrentUnit (Db_UNIT(3));
   CHECK_RETURN (ret_code);

   /* FROM d2b unit */
   ret_code = krui_createLink (Db_UNIT(2), 0.0);
   CHECK_RETURN (ret_code);


   /* TO g1b unit */
   ret_code = krui_setCurrentUnit (G1b_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_REC_G1b);
   CHECK_RETURN (ret_code);

   /* FROM recb units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (RECb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_INP_G1b);
   CHECK_RETURN (ret_code);

   /* FROM inpb units */
   for (j=1; j<=f1bUnits; j++) {
      ret_code = krui_createLink (INPb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rib unit */
   ret_code = krui_setCurrentUnit (RIb_UNIT);
   CHECK_RETURN (ret_code);

   /* first Site */
   ret_code = krui_setSite (SITE_NAME_INP_RIb);
   CHECK_RETURN (ret_code);

   /* FROM inpb units */
   for (j=1; j<=f1bUnits; j++) {
      ret_code = krui_createLink (INPb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/

   /* second Site */
   ret_code = krui_setSite (SITE_NAME_RHO_RIb);
   CHECK_RETURN (ret_code);

   /* from rhob unit */
   ret_code = krui_createLink (RHOb_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO rcb unit */
   ret_code = krui_setCurrentUnit (RCb_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM cmpb units */
   for (j=1; j<=f1bUnits; j++) {
      ret_code = krui_createLink (CMPb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rgb unit */
   ret_code = krui_setCurrentUnit (RGb_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM rib unit */
   ret_code = krui_createLink (RIb_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rcb unit */
   ret_code = krui_createLink (RCb_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO clb unit */
   ret_code = krui_setCurrentUnit (CLb_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM d3b unit */
   ret_code = krui_createLink (Db_UNIT(3), 0.0);
   CHECK_RETURN (ret_code);

   /* FROM rgb unit */
   ret_code = krui_createLink (RGb_UNIT, 0.0);
   CHECK_RETURN (ret_code);

   /* FROM g2b unit */
   ret_code = krui_createLink (G2b_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO ncb unit */
   ret_code = krui_setCurrentUnit (NCb_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM rstb units */
   for (j=1; j<=f2bUnits; j++) {
      ret_code = krui_createLink (RSTb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


   /* TO rhob unit */
   ret_code = krui_setCurrentUnit (RHOb_UNIT);
   CHECK_RETURN (ret_code);

   /* from itself */
   ret_code = krui_createLink (RHOb_UNIT, 0.0);
   CHECK_RETURN (ret_code);


   /* TO g2b unit */
   ret_code = krui_setCurrentUnit (G2b_UNIT);
   CHECK_RETURN (ret_code);

   /* FROM inpb units */
   for (j = 1; j <= f1bUnits; j++) {
      ret_code = krui_createLink (INPb_UNIT(j), 0.0);
      CHECK_RETURN (ret_code);
   } /*for*/


  /*  set the update function  */
  ret_code = krui_setUpdateFunc (UPDATE_FUNC_NAME);
  CHECK_RETURN( ret_code );

  /* set the learning function */
  ret_code = krui_setLearnFunc (LEARN_FUNC_NAME);

   //ui_NumberOfLearnParamsChanged();
   //ui_NumberOfUpdateParamsChanged();
   return (ret_code);
}


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
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_art2.c,v $
  SHORTNAME      : bn_art2
  SNNS VERSION   : 4.2

  PURPOSE        : Creates the ART2-window and ART2 networks. 
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 15.1.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.10 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:40 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

/* begin private definition section */

/* define noise function for contrast enhancement */
/* Possible choices are:   "Out_ART2_Noise_PLin"
                           "Out_ART2_Noise_ContDiff"
*/
#define ART2_NOISE_FUNCTION  const_cast<char*>("Out_ART2_Noise_ContDiff")

/* Definition of the Activationfunctions */

/* F0-Layer */
#define ACTF_INP const_cast<char*>("Act_Identity")

/* F1-Layer */
#define ACTF_P   const_cast<char*>("Act_ART2_Identity")
#define ACTF_Q   const_cast<char*>("Act_ART2_NormP")
#define ACTF_U   const_cast<char*>("Act_ART2_NormV")
#define ACTF_V   const_cast<char*>("Act_ART2_Identity")
#define ACTF_W   const_cast<char*>("Act_ART2_Identity")
#define ACTF_X   const_cast<char*>("Act_ART2_NormW")
#define ACTF_R   const_cast<char*>("Act_ART2_NormIP")

/* F2-Layer */
#define ACTF_REC const_cast<char*>("Act_ART2_Rec")
#define ACTF_RST const_cast<char*>("Act_ART2_Rst")



/* Definition of the Output Function */

/* F0-Layer */
#define OUTF_INP const_cast<char*>("Out_Identity")

/* F1-Layer */
#define OUTF_P   const_cast<char*>("Out_Identity")
#define OUTF_Q   ART2_NOISE_FUNCTION   /* This output function has to be
                                          the same as for Units X
                                          (Noise Reduction/Contrast enhancem.)
                                       */
#define OUTF_U   const_cast<char*>("Out_Identity")
#define OUTF_V   const_cast<char*>("Out_Identity")
#define OUTF_W   const_cast<char*>("Out_Identity")
#define OUTF_X   ART2_NOISE_FUNCTION   /* This output function has to be
                                          the same as for Units Q
                                          (Noise Reduction/Contrast enhancem.)
                                       */
#define OUTF_R   const_cast<char*>("Out_Identity")

/* F2-Layer */
#define OUTF_REC const_cast<char*>("Out_Identity")
#define OUTF_RST const_cast<char*>("Out_Identity")


/* Learning function name */
#define LEARN_FUNC_NAME        const_cast<char*>("ART2")

/* Update function name */
#define UPDATE_FUNC_NAME       const_cast<char*>("ART2_Stable")




/* returns 0 if x<=0 and 1 if x>0
*/
#define POS_SIGN(x)            (((x) > 0) ? 1 : 0)

/* Calculates the number of columns when no of units and no. of rows is given
*/
#define NO_OF_COLS(units,rows)  ((int) (units/rows) + POS_SIGN(units % rows))

//already defined in cc_max
//#define MAX(x,y)                ((x) > (y) ? (x) : (y))


/* Calculating the unit numbers */

#define INP_UNIT(y)             y

#define W_UNIT(y)               INP_UNIT(f1Units) + y
#define X_UNIT(y)               W_UNIT(f1Units) + y
#define U_UNIT(y)               X_UNIT(f1Units) + y
#define V_UNIT(y)               U_UNIT(f1Units) + y
#define P_UNIT(y)               V_UNIT(f1Units) + y
#define Q_UNIT(y)               P_UNIT(f1Units) + y
#define R_UNIT(y)               Q_UNIT(f1Units) + y

#define REC_UNIT(y)             R_UNIT(f1Units) + y
#define RST_UNIT(y)             REC_UNIT(f2Units) + y



/* calculating the unit positions */

#define F1_COLS                 NO_OF_COLS(f1Units, f1Rows)
#define F2_COLS                 NO_OF_COLS(f2Units, f2Rows)

#define OFFSET_X                0
#define OFFSET_Y                0

#define INP_X                   OFFSET_X + 1
#define INP_Y                   OFFSET_Y + 4 + f1Rows

#define W_X                     INP_X + F1_COLS + 1
#define W_Y                     OFFSET_Y + 4
#define X_X                     W_X
#define X_Y                     W_Y + 1
#define U_X                     W_X + 1
#define U_Y                     W_Y
#define V_X                     U_X
#define V_Y                     X_Y
#define P_X                     U_X + 1
#define P_Y                     W_Y
#define Q_X                     P_X
#define Q_Y                     V_Y
#define R_X                     P_X
#define R_Y                     P_Y - 1

#define REC_X                   W_X + 4*F1_COLS + 1
#define REC_Y                   OFFSET_Y + 3
#define RST_X                   REC_X + F2_COLS + 1
#define RST_Y                   REC_Y


#define F1_SEP_X                3
#define F1_SEP_Y                3


#define STRING_LENGTH            30


/* checks whether value is not equal to KRERR_NO_ERROR and if so, returns
*/
#define CHECK_RETURN(ret_code)  if (ret_code != KRERR_NO_ERROR) \
                                   return (ret_code)


#include "SnnsCLib.h"


/*****************************************************************************
  FUNCTION : bn_art2_make_layer

  PURPOSE  : Generation of a complete layer
  NOTES    : 

  UPDATE   : 20.1.1993
******************************************************************************/

krui_err SnnsCLib::bn_art2_make_layer (
                            int    units,
                            int    rows,
                            int    cols,
                            int    Xbegin,
                            int    Ybegin,
                            int    Xsep,
                            int    Ysep,
                            char  *name_prefix,
                            int    unit_ttype,
                            char  *actfuncname,
                            char  *outfuncname,
                            int    NoOfSites,
                            char **SiteNames,
                            char **SiteFuncNames)
                         

{
   krui_err             ret_code = KRERR_NO_ERROR;

   int                  row;
   int                  col;
   int                  count;
   int                  last_col;
   int                  unit_no;
   struct PosType       unit_pos;
   char                 *site_name;
   int                  site_no;
   char                 name[100];

   unit_pos.z = 0;

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

         unit_pos.x = Xbegin + Xsep*(col-1);
         unit_pos.y = Ybegin + Ysep*(row-1);
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
  FUNCTION : bn_art2_createNet

  PURPOSE  : Generation of the ART2 network using SNNS kernel functions.
  NOTES    :

  UPDATE   : 20.1.1993
******************************************************************************/

krui_err SnnsCLib::bn_art2_createNet (int f1Units, int f1Rows, int f2Units, int f2Rows)

{
   krui_err          ret_code   = KRERR_NO_ERROR;

   int               i, j;


   /********* create units ******************/

   /* create Input Layer  */
   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, INP_X, INP_Y,
                          1, 1, const_cast<char*>("inp"), INPUT, ACTF_INP, OUTF_INP, 0,
                          NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create W-Units */
   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, W_X, W_Y,
                          F1_SEP_X, F1_SEP_Y, const_cast<char*>("w"),
                          HIDDEN, ACTF_W, OUTF_W, 0, NULL, NULL);
   CHECK_RETURN (ret_code);



   /* create X-Units */

   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, X_X, X_Y,
                          F1_SEP_X, F1_SEP_Y, const_cast<char*>("x"), HIDDEN, ACTF_X,
                          OUTF_X, 0, NULL, NULL);
   CHECK_RETURN (ret_code);


   /* create U-Units */

   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, U_X, U_Y,
                          F1_SEP_X, F1_SEP_Y, const_cast<char*>("u"), HIDDEN, ACTF_U,
                          OUTF_U, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create V-Units */
   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, V_X, V_Y,
                          F1_SEP_X, F1_SEP_Y, const_cast<char*>("v"),
                          HIDDEN, ACTF_V, OUTF_V, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create P-Units */
   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, P_X, P_Y,
                          F1_SEP_X, F1_SEP_Y, const_cast<char*>("p"),
                          HIDDEN, ACTF_P, OUTF_P, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Q-Units */

   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, Q_X, Q_Y,
                          F1_SEP_X, F1_SEP_Y, const_cast<char*>("q"), HIDDEN, ACTF_Q,
                          OUTF_Q, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create R-Units */

   ret_code = bn_art2_make_layer (f1Units, f1Rows, F1_COLS, R_X, R_Y,
                          F1_SEP_X, F1_SEP_Y, const_cast<char*>("r"), HIDDEN, ACTF_R,
                          OUTF_R, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   /* create Recognition Layer */
   ret_code = bn_art2_make_layer (f2Units, f2Rows, F2_COLS, REC_X, REC_Y, 1, 1,
                          const_cast<char*>("rec"), SPECIAL, ACTF_REC, OUTF_REC, 0, NULL, NULL);
   CHECK_RETURN (ret_code);

   ret_code = bn_art2_make_layer (f2Units, f2Rows, F2_COLS, RST_X, RST_Y, 1, 1,
                          const_cast<char*>("rst"), HIDDEN, ACTF_RST, OUTF_RST, 0, NULL, NULL);
   CHECK_RETURN (ret_code);




   /********* Create Links **************************************/


   /* TO w units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (W_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM inp units */
      ret_code = krui_createLink (INP_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

      /* FROM u units */
      ret_code = krui_createLink (U_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO x units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (X_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM w units */
      ret_code = krui_createLink (W_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO u units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (U_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM v units */
      ret_code = krui_createLink (V_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO v units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (V_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM x units */
      ret_code = krui_createLink (X_UNIT(i),0.0);
      CHECK_RETURN (ret_code);

      /* FROM q unit */
      ret_code = krui_createLink (Q_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO p units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (P_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM rec units */
      for (j=1; j<=f2Units; j++) {
         ret_code = krui_createLink (REC_UNIT(j),0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

      /* FROM u unit */
      ret_code = krui_createLink (U_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO q units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (Q_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM p unit */
      ret_code = krui_createLink (P_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO r units */
   for (i=1; i<=f1Units; i++) {

      ret_code = krui_setCurrentUnit (R_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM p unit */
      ret_code = krui_createLink (P_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

      /* FROM inp unit */
      ret_code = krui_createLink (INP_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO rec units */
   for (i=1; i<=f2Units; i++) {

      ret_code = krui_setCurrentUnit (REC_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM p units */
      for (j=1; j<=f1Units; j++) {
         ret_code = krui_createLink (P_UNIT(j), 0.0);
         CHECK_RETURN (ret_code);
      } /*for*/

      /* FROM rst unit */
      ret_code = krui_createLink (RST_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/


   /* TO rst units */
   for (i=1; i<=f2Units; i++) {

      ret_code = krui_setCurrentUnit (RST_UNIT(i));
      CHECK_RETURN (ret_code);

      /* FROM rec units */
      ret_code = krui_createLink (REC_UNIT(i), 0.0);
      CHECK_RETURN (ret_code);

   } /*for*/

   //printf (" Done.\n");


   /*  set the update function  */
   ret_code = krui_setUpdateFunc (UPDATE_FUNC_NAME);
   CHECK_RETURN( ret_code );

   /* set the learning function */
   ret_code = krui_setLearnFunc (LEARN_FUNC_NAME);

   return (ret_code);
}


/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_art2.ph,v $
  SHORTNAME      : bn_art2
  SNNS VERSION   : 4.2

  PURPOSE        : 
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 15.1.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:32 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_ART2_DEFINED_
#define  _BN_ART2_DEFINED_

/* begin global definition section */

//void bn_createART2 (void);

/* end global definition section */


/* begin private definition section */

/* define noise function for contrast enhancement */
/* Possible choices are:   "Out_ART2_Noise_PLin"
                           "Out_ART2_Noise_ContDiff"
*/
#define ART2_NOISE_FUNCTION  "Out_ART2_Noise_ContDiff"

/* Definition of the Activationfunctions */

/* F0-Layer */
#define ACTF_INP "Act_Identity"

/* F1-Layer */
#define ACTF_P   "Act_ART2_Identity"
#define ACTF_Q   "Act_ART2_NormP"
#define ACTF_U   "Act_ART2_NormV"
#define ACTF_V   "Act_ART2_Identity"
#define ACTF_W   "Act_ART2_Identity"
#define ACTF_X   "Act_ART2_NormW"
#define ACTF_R   "Act_ART2_NormIP"

/* F2-Layer */
#define ACTF_REC "Act_ART2_Rec"
#define ACTF_RST "Act_ART2_Rst"



/* Definition of the Output Function */

/* F0-Layer */
#define OUTF_INP "Out_Identity"

/* F1-Layer */
#define OUTF_P   "Out_Identity"
#define OUTF_Q   ART2_NOISE_FUNCTION   /* This output function has to be
                                          the same as for Units X
                                          (Noise Reduction/Contrast enhancem.)
                                       */
#define OUTF_U   "Out_Identity"
#define OUTF_V   "Out_Identity"
#define OUTF_W   "Out_Identity"
#define OUTF_X   ART2_NOISE_FUNCTION   /* This output function has to be
                                          the same as for Units Q
                                          (Noise Reduction/Contrast enhancem.)
                                       */
#define OUTF_R   "Out_Identity"

/* F2-Layer */
#define OUTF_REC "Out_Identity"
#define OUTF_RST "Out_Identity"


/* Learning function name */
#define LEARN_FUNC_NAME        "ART2"

/* Update function name */
#define UPDATE_FUNC_NAME       "ART2_Stable"




/* returns 0 if x<=0 and 1 if x>0
*/
#define POS_SIGN(x)            (((x) > 0) ? 1 : 0)

/* Calculates the number of columns when no of units and no. of rows is given
*/
#define NO_OF_COLS(units,rows)  ((int) (units/rows) + POS_SIGN(units % rows))

#define MAX(x,y)                ((x) > (y) ? (x) : (y))


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





//static int      bn_art2_open = 0;
//static Widget   baseWidget;
//static Widget   art2UnitWidget[2], art2RowWidget[2];



krui_err bn_art2_make_layer (
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
                            char **SiteFuncNames);                        
krui_err bn_art2_createNet (int f1Units, int f1Rows, int f2Units, int f2Rows);
//static void bn_art2_donePROC (void);
//static void bn_art2_createPROC (void);

/* end private definition section */

#endif

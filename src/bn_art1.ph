/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_art1.ph,v $
  SHORTNAME      : bn_art1
  SNNS VERSION   : 4.2

  PURPOSE        : 
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 15.1.1993

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:19:30 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _BN_ART1_DEFINED_
#define  _BN_ART1_DEFINED_

/* begin private definition section */

/* Definition of the Output Function */
#define ART1_OUTFUNC    "Out_Identity"


/* Definition of the Activationfunctions */
#define ART1_ACTF_INP "Act_Identity"
#define ART1_ACTF_CMP "Act_at_least_2"
#define ART1_ACTF_REC "Act_Identity"
#define ART1_ACTF_DEL "Act_at_least_2"
#define ART1_ACTF_D   "Act_at_least_1"
#define ART1_ACTF_RST "Act_at_least_1"
#define ART1_ACTF_G1  "Act_at_least_2"
#define ART1_ACTF_RI  "Act_Product"
#define ART1_ACTF_RC  "Act_Identity"
#define ART1_ACTF_RG  "Act_less_than_0"
#define ART1_ACTF_CL  "Act_at_least_1"
#define ART1_ACTF_NCL "Act_ART1_NC"
#define ART1_ACTF_RHO "Act_Identity"
#define ART1_ACTF_G2  "Act_at_most_0"


/* Site names
*/
#define ART1_SITE_NAME_RST_SELF    "Art1_Site_Rst_Self"
#define ART1_SITE_NAME_RST_SIGNAL  "Art1_Site_Rst_Signal"
#define ART1_SITE_NAME_REC_G1      "Art1_Site_Rec_G1"
#define ART1_SITE_NAME_INP_G1      "Art1_Site_Inp_G1"
#define ART1_SITE_NAME_INP_RI      "Art1_Site_Inp_RI"
#define ART1_SITE_NAME_RHO_RI      "Art1_Site_Rho_RI"

/* Site function names
*/
#define ART1_SITE_FUNC_RST_SELF    "Site_WeightedSum"
#define ART1_SITE_FUNC_RST_SIGNAL  "Site_at_least_2"
#define ART1_SITE_FUNC_REC_G1      "Site_at_most_0"
#define ART1_SITE_FUNC_INP_G1      "Site_at_least_1"
#define ART1_SITE_FUNC_INP_RI      "Site_WeightedSum"
#define ART1_SITE_FUNC_RHO_RI      "Site_WeightedSum"

/* Learning function name */
#define ART1_LEARN_FUNC_NAME     "ART1"

/* Update function name */
#define ART1_UPDATE_FUNC_NAME    "ART1_Stable"



/* Macros
*/
#define FOR_ALL_ART1_INP_UNITS(x)  for (x = 1; x <= IUnits; x++)
#define FOR_ALL_ART1_CMP_UNITS(x)  for (x = IUnits+1; x <= 2*IUnits; x++)
#define FOR_ALL_ART1_REC_UNITS(x)  for (x = 2*IUnits+1; x <= 2*IUnits + CUnits; x++)
#define FOR_ALL_ART1_DEL_UNITS(x)  for (x = 2*IUnits+CUnits+1; x <= 2*IUnits+2*CUnits; x++)
#define FOR_ALL_ART1_RST_UNITS(x)  for (x = 2*IUnits+2*CUnits+4; x <= 2*IUnits+3*CUnits+3; x++)

#define POS_SIGN(x)             (((x) > 0) ? 1 : 0)
#define NO_OF_COLS(units,rows)  ((int) (units/rows) + POS_SIGN(units % rows))
#define F1_COLS(units,rows)     NO_OF_COLS(units, rows)
#define F2_COLS(units,rows)     NO_OF_COLS(units, rows)

#define D_UNIT(y)              2*IUnits + 2*CUnits + y

#define SPECIAL_UNITS          2*IUnits + 3*CUnits + 4
#define G1_UNIT                SPECIAL_UNITS
#define RI_UNIT                SPECIAL_UNITS + 1
#define RC_UNIT                SPECIAL_UNITS + 2
#define RG_UNIT                SPECIAL_UNITS + 3
#define CL_UNIT                SPECIAL_UNITS + 4
#define NCL_UNIT               SPECIAL_UNITS + 5
#define RHO_UNIT               SPECIAL_UNITS + 6
#define G2_UNIT                SPECIAL_UNITS + 7


/* checks whether value is not equal to KRERR_NO_ERROR and if so, returns
*/
#define CHECK_RETURN(ret_code)  if (ret_code != KRERR_NO_ERROR) \
                                   return (ret_code)


krui_err bn_art1_createNet (int IUnits, int IRow, int CUnits, int CRos);

/* end private definition section */

#endif

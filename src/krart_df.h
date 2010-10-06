/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/krart_df.h,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS Kernel definitions for ART-Networks 
  NOTES          :

  AUTHOR         : Kai-Uwe Herrmann
  DATE           : 17.05.92

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:27:19 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef __KR_ART_DEF
#define __KR_ART_DEF


/* Logical Layers for ART1 networks
*/
#define ART1_INP_LAY 1     /* The ART1 Input layer */
#define ART1_CMP_LAY 2     /* The ART1 Comparison layer */
#define ART1_REC_LAY 3     /* The ART1 Recognition layer */
#define ART1_DEL_LAY 4     /* The ART1 Delay layer */
#define ART1_RST_LAY 5     /* The ART1 Reset layer */
#define ART1_SPEC_LAY 6    /* The ART1 Special unit layer */


/* Logical Layers for ART2 networks
*/
#define ART2_INP_LAY    1   /* The ART2 Input Layer */
#define ART2_W_LAY      2   /* The ART2 W Units (belonging to F1) */
#define ART2_X_LAY      3   /* The ART2 X Units (belonging to F1) */
#define ART2_U_LAY      4   /* The ART2 U Units (belonging to F1) */
#define ART2_V_LAY      5   /* The ART2 V Units (belonging to F1) */
#define ART2_P_LAY      6   /* The ART2 P Units (belonging to F1) */
#define ART2_Q_LAY      7   /* The ART2 Q Units (belonging to F1) */
#define ART2_R_LAY      8   /* The ART2 R Units (belonging to F1) */
#define ART2_REC_LAY    9   /* The ART2 Recognition Layer */
#define ART2_RST_LAY   10   /* The ART2 Reset Layer */

/* Logical Layers for ARTMAP networks
*/
#define ARTMAP_INPa_LAY    1       /* the artmap-arta input layer */
#define ARTMAP_CMPa_LAY    2       /* the artmap-arta comparison layer */
#define ARTMAP_RECa_LAY    3       /* the artmap-arta recognition layer */
#define ARTMAP_DELa_LAY    4       /* the artmap-arta delay layer */
#define ARTMAP_RSTa_LAY    5       /* the artmap-arta local reset layer */
#define ARTMAP_SPECa_LAY   6       /* the artmap-arta special unit layer */
#define ARTMAP_INPb_LAY    7       /* the artmap-artb input layer */
#define ARTMAP_CMPb_LAY    8       /* the artmap-artb comparison layer */
#define ARTMAP_RECb_LAY    9       /* the artmap-artb recognition layer */
#define ARTMAP_DELb_LAY   10       /* the artmap-artb delay layer */
#define ARTMAP_RSTb_LAY   11       /* the artmap-artb local reset layer */
#define ARTMAP_SPECb_LAY  12       /* the artmap-artb special unit layer */
#define ARTMAP_MAP_LAY    13       /* the artmap map layer */
#define ARTMAP_SPEC_LAY   14       /* the artmap special unit layer */

/* Logical unit numbers for ART1 networks
*/

  /* special units */
#define ART1_NO_OF_SPEC_UNITS   8

#define ART1_CL_UNIT    1
#define ART1_NCL_UNIT   2
#define ART1_G1_UNIT    3
#define ART1_RI_UNIT    4
#define ART1_RC_UNIT    5
#define ART1_RG_UNIT    6
#define ART1_RHO_UNIT   7
#define ART1_G2_UNIT    8

  /* delay type units */
#define ART1_DEL_REC_UNIT         1
#define ART1_D1_UNIT              2
#define ART1_D2_UNIT              3
#define ART1_D3_UNIT              4


/* Logical unit numbers for ARTMAP networks
*/

  /* special units */
#define ARTMAP_NO_OF_SPECa_UNITS   8

#define ARTMAP_CLa_UNIT   1
#define ARTMAP_NCa_UNIT   2
#define ARTMAP_G1a_UNIT   3
#define ARTMAP_RIa_UNIT   4
#define ARTMAP_RCa_UNIT   5
#define ARTMAP_RGa_UNIT   6
#define ARTMAP_RHOa_UNIT  7
#define ARTMAP_G2a_UNIT   8


#define ARTMAP_NO_OF_SPECb_UNITS   8

#define ARTMAP_CLb_UNIT   1
#define ARTMAP_NCb_UNIT   2
#define ARTMAP_G1b_UNIT   3
#define ARTMAP_RIb_UNIT   4
#define ARTMAP_RCb_UNIT   5
#define ARTMAP_RGb_UNIT   6
#define ARTMAP_RHOb_UNIT  7
#define ARTMAP_G2b_UNIT   8


#define ARTMAP_NO_OF_SPEC_UNITS   10

#define ARTMAP_D1_UNIT    1
#define ARTMAP_CL_UNIT    2
#define ARTMAP_NC_UNIT    3
#define ARTMAP_G_UNIT     4
#define ARTMAP_RB_UNIT    5
#define ARTMAP_RM_UNIT    6
#define ARTMAP_RG_UNIT    7
#define ARTMAP_RHO_UNIT   8
#define ARTMAP_QU_UNIT    9
#define ARTMAP_DRHO_UNIT 10

  /* delay type units */
#define ARTMAP_DELa_REC_UNIT   1
#define ARTMAP_D1a_UNIT        2
#define ARTMAP_D2a_UNIT        3
#define ARTMAP_D3a_UNIT        4

#define ARTMAP_DELb_REC_UNIT   1
#define ARTMAP_D1b_UNIT        2
#define ARTMAP_D2b_UNIT        3
#define ARTMAP_D3b_UNIT        4




/* Link initialization values for the links of an ART1 network.
   The last two elements of the name indicate the logical source layer
   and the logical target layer
*/

  /* fix links for ART1 */
#define ART1_LINK_INP_CMP   1.0
#define ART1_LINK_INP_G1    1.0
#define ART1_LINK_INP_RI    1.0
#define ART1_LINK_INP_G2    1.0
#define ART1_LINK_CMP_RC    1.0
#define ART1_LINK_REC_G1    1.0
#define ART1_LINK_REC_DEL   2.0
#define ART1_LINK_DEL_DEL   1.0
#define ART1_LINK_DEL_RST   1.0
#define ART1_LINK_DEL_CL    1.0
#define ART1_LINK_RST_REC   -2.0
#define ART1_LINK_RST_RST   1.0
#define ART1_LINK_RST_NCL   1.0
#define ART1_LINK_G1_CMP    1.0
#define ART1_LINK_RI_RG     -1.0
#define ART1_LINK_RC_RG     1.0
#define ART1_LINK_RG_RST    1.0
#define ART1_LINK_RG_REC    -2.0
#define ART1_LINK_RG_CL     -1.0
#define ART1_LINK_RHO_RI    1.0
#define ART1_LINK_RHO_RHO   1.0
#define ART1_LINK_G2_REC    -2.0
#define ART1_LINK_G2_CL     1.0


/* trainable links for ART1 */
  /* the initial values of the bottom up link weights should be
             0 < weight < (1 / (beta + |I|))
             where beta is a value > 0.0 and should not be too big,
                   I is the number of ones in the binary input vector.
                   gamma has to be choosen > 1.0.
  */
#define ART1_LINK_CMP_REC(beta,gamma)    (1.0 / ((beta) + (gamma) * NoOfInputUnits))
#define ART1_LINK_DEL_CMP                1.0

  /* Formulas to adjust the link weights in an ART1 network
  */
#define ART1_ADJUST_LINK_DEL_CMP(unit)                      (unit->act)
#define ART1_ADJUST_LINK_CMP_REC(unit, beta, norm) \
                                       ((unit->act)/((beta) + (norm)))




/* fix links for ART2 */
#define ART2_LINK_INP_W               1.0
#define ART2_LINK_U_W(a)              a
#define ART2_LINK_W_X                 1.0
#define ART2_LINK_V_U                 1.0
#define ART2_LINK_X_V                 1.0
#define ART2_LINK_Q_V(b)              b
#define ART2_LINK_U_P                 1.0
#define ART2_LINK_P_Q                 1.0
#define ART2_LINK_P_R(c)              c
#define ART2_LINK_INP_R               1.0

#define ART2_LINK_RST_REC(d)         -(1/(1-(d)))-10
#define ART2_LINK_REC_RST             1.0
#define ART2_LINK_RST_RST             1.0



/* trainable links for ART2 */
#define ART2_LINK_P_REC(param_d,gamma) (1.0/(gamma*(1-param_d)*sqrt((double) NoOfInputUnits)))
#define ART2_LINK_REC_P                0.0

  /* Formulas to adjust the link weights in an ART2 network
  */
#define ART2_ADJUST_LINK_REC_P(unit_u, d)       (unit_u->act / (1-d))
#define ART2_ADJUST_LINK_P_REC(unit_u, d)       (unit_u->act / (1-d))





/* fix links for ARTMAP */
#define ARTMAP_LINK_INPa_CMPa   1.0
#define ARTMAP_LINK_INPa_G1a    1.0
#define ARTMAP_LINK_INPa_RIa    1.0
#define ARTMAP_LINK_INPa_G2a    1.0
#define ARTMAP_LINK_CMPa_RCa    1.0
#define ARTMAP_LINK_RECa_G1a    1.0
#define ARTMAP_LINK_RECa_DELa   2.0
#define ARTMAP_LINK_DELa_DELa   1.0
#define ARTMAP_LINK_DELa_RSTa   1.0
#define ARTMAP_LINK_DELa_CLa    1.0
#define ARTMAP_LINK_RSTa_RECa  -2.0
#define ARTMAP_LINK_RSTa_RSTa   1.0
#define ARTMAP_LINK_RSTa_NCa    1.0
#define ARTMAP_LINK_G1a_CMPa    1.0
#define ARTMAP_LINK_RIa_RGa    -1.0
#define ARTMAP_LINK_RCa_RGa     1.0
#define ARTMAP_LINK_RGa_RSTa    1.0
#define ARTMAP_LINK_RGa_RECa   -2.0
#define ARTMAP_LINK_RGa_CLa    -1.0
#define ARTMAP_LINK_RGa_DRHO   -1.0
#define ARTMAP_LINK_RHOa_RIa    1.0
#define ARTMAP_LINK_RHOa_RHOa   1.0
#define ARTMAP_LINK_G2a_RECa   -2.0
#define ARTMAP_LINK_G2a_CLa     1.0

#define ARTMAP_LINK_INPb_CMPb   1.0
#define ARTMAP_LINK_INPb_G1b    1.0
#define ARTMAP_LINK_INPb_RIb    1.0
#define ARTMAP_LINK_INPb_G2b    1.0
#define ARTMAP_LINK_CMPb_RCb    1.0
#define ARTMAP_LINK_RECb_G1b    1.0
#define ARTMAP_LINK_RECb_DELb   2.0
#define ARTMAP_LINK_DELb_DELb   1.0
#define ARTMAP_LINK_MAP_DELb    1.0
#define ARTMAP_LINK_DELb_RSTb   1.0
#define ARTMAP_LINK_DELb_CLb    1.0
#define ARTMAP_LINK_RSTb_RECb  -2.0
#define ARTMAP_LINK_RSTb_RSTb   1.0
#define ARTMAP_LINK_RSTb_NCb    1.0
#define ARTMAP_LINK_G1b_CMPb    1.0
#define ARTMAP_LINK_RIb_RGb    -1.0
#define ARTMAP_LINK_RCb_RGb     1.0
#define ARTMAP_LINK_RGb_RSTb    1.0
#define ARTMAP_LINK_RGb_RECb   -2.0
#define ARTMAP_LINK_RGb_CLb    -1.0
#define ARTMAP_LINK_RHOb_RIb    1.0
#define ARTMAP_LINK_RHOb_RHOb   1.0
#define ARTMAP_LINK_G2b_RECb   -2.0
#define ARTMAP_LINK_G2b_CLb     1.0

#define ARTMAP_LINK_DELb_MAP    1.0
#define ARTMAP_LINK_G_MAP       1.0
#define ARTMAP_LINK_DELb_D1     1.0
#define ARTMAP_LINK_CLa_CL      1.0
#define ARTMAP_LINK_CLb_CL      1.0
#define ARTMAP_LINK_RG_CL      -2.0
#define ARTMAP_LINK_NCa_NC      1.0
#define ARTMAP_LINK_NCb_NC      1.0
#define ARTMAP_LINK_RECa_G      1.0
#define ARTMAP_LINK_RECb_G      1.0
#define ARTMAP_LINK_D1_RB       1.0
#define ARTMAP_LINK_RHO_RB      1.0
#define ARTMAP_LINK_MAP_RM      1.0
#define ARTMAP_LINK_RB_RG      -1.0
#define ARTMAP_LINK_RM_RG       1.0
#define ARTMAP_LINK_RHO_RHO     1.0
#define ARTMAP_LINK_INPa_QU     1.0
#define ARTMAP_LINK_RHO_RB      1.0
#define ARTMAP_LINK_INPa_QU     1.0
#define ARTMAP_LINK_CMPa_QU     1.0
#define ARTMAP_LINK_RHOa_DRHO  -1.0
#define ARTMAP_LINK_CLb_DRHO    1.0
#define ARTMAP_LINK_RG_DRHO     1.0
#define ARTMAP_LINK_QU_DRHO     1.0
#define ARTMAP_LINK_DRHO_DRHO  -1.0
#define ARTMAP_LINK_DRHO_RHOa   1.0


/* trainable links for ARTMAP */
  /* the initial values of the bottom up link weights should be
             0 < weight < (1 / (beta + |I|))
             where beta is a value > 0.0 and should not be too big,
                   I is the number of ones in the binary input vector.
                   gamma has to be choosen > 1.0.
  */
#define ARTMAP_LINK_CMPa_RECa(beta,gamma)    (1.0 / ((beta) + (gamma) * ArtMap_NoOfInpUnits_a))
#define ARTMAP_LINK_DELa_CMPa                1.0
#define ARTMAP_LINK_CMPb_RECb(beta,gamma)    (1.0 / ((beta) + (gamma) * ArtMap_NoOfInpUnits_b))
#define ARTMAP_LINK_DELb_CMPb                1.0
#define ARTMAP_LINK_DELa_MAP                 1.0

/* Links of ARTMAP networks are adjusted using the ART1 formulas */


/******************** Output and Activation function names ******************/

/* Definition of the Outputfunctions for ART1 */
#define ART1_OUTFUNC    "Out_Identity"

/* Definition of the Outputfunctions for ART2 */
#define ART2_OUTFUNC    "Out_Identity"

/* Definition of the Outputfunctions for ARTMAP */
#define ARTMAP_OUTFUNC    "Out_Identity"


/* Definition of the Activationfunctions for ART1 */
#define ART1_ACTF_INP            "Act_Identity"
#define ART1_ACTF_CMP            "Act_at_least_2"
#define ART1_ACTF_REC            "Act_Identity"
#define ART1_ACTF_DEL            "Act_at_least_2"
#define ART1_ACTF_D              "Act_at_least_1"
#define ART1_ACTF_RST            "Act_at_least_1"
#define ART1_ACTF_G1             "Act_at_least_2"
#define ART1_ACTF_RI             "Act_Product"
#define ART1_ACTF_RC             "Act_Identity"
#define ART1_ACTF_RG             "Act_less_than_0"
#define ART1_ACTF_CL             "Act_at_least_1"
#define ART1_ACTF_NCL            "Act_ART1_NC"
#define ART1_ACTF_RHO            "Act_Identity"
#define ART1_ACTF_G2             "Act_at_most_0"

/* Definition of the Activationfunctions for ART2 */

/* F0-Layer*/
#define ART2_ACTF_INP            "Act_Identity"

/* F1-Layer */
#define ART2_ACTF_W              "Act_ART2_Identity"
#define ART2_ACTF_X              "Act_ART2_NormW"
#define ART2_ACTF_U              "Act_ART2_NormV"
#define ART2_ACTF_V              "Act_ART2_Identity"
#define ART2_ACTF_P              "Act_ART2_Identity"
#define ART2_ACTF_Q              "Act_ART2_NormP"
#define ART2_ACTF_R              "Act_ART2_NormIP"

/* F2-Layer */
#define ART2_ACTF_REC            "Act_ART2_Rec"
#define ART2_ACTF_RST            "Act_ART2_Rst"


/* Definition of the Activationfunctions for ARTMAP */
#define ARTMAP_ACTF_INPa            "Act_Identity"
#define ARTMAP_ACTF_CMPa            "Act_at_least_2"
#define ARTMAP_ACTF_RECa            "Act_Identity"
#define ARTMAP_ACTF_DELa            "Act_at_least_2"
#define ARTMAP_ACTF_Da              "Act_at_least_1"
#define ARTMAP_ACTF_RSTa            "Act_at_least_1"
#define ARTMAP_ACTF_G1a             "Act_at_least_2"
#define ARTMAP_ACTF_RIa             "Act_Product"
#define ARTMAP_ACTF_RCa             "Act_Identity"
#define ARTMAP_ACTF_RGa             "Act_less_than_0"
#define ARTMAP_ACTF_CLa             "Act_at_least_1"
#define ARTMAP_ACTF_NCa             "Act_ARTMAP_NCa"
#define ARTMAP_ACTF_RHOa            "Act_Identity"
#define ARTMAP_ACTF_G2a             "Act_at_most_0"

#define ARTMAP_ACTF_INPb            "Act_Identity"
#define ARTMAP_ACTF_CMPb            "Act_at_least_2"
#define ARTMAP_ACTF_RECb            "Act_Identity"
#define ARTMAP_ACTF_DELb            "Act_at_least_2"
#define ARTMAP_ACTF_Db              "Act_at_least_1"
#define ARTMAP_ACTF_RSTb            "Act_at_least_1"
#define ARTMAP_ACTF_G1b             "Act_at_least_2"
#define ARTMAP_ACTF_RIb             "Act_Product"
#define ARTMAP_ACTF_RCb             "Act_Identity"
#define ARTMAP_ACTF_RGb             "Act_less_than_0"
#define ARTMAP_ACTF_CLb             "Act_at_least_1"
#define ARTMAP_ACTF_NCb             "Act_ARTMAP_NCb"
#define ARTMAP_ACTF_RHOb            "Act_Identity"
#define ARTMAP_ACTF_G2b             "Act_at_most_0"

#define ARTMAP_ACTF_MAP             "Act_at_least_2"
#define ARTMAP_ACTF_D1              "Act_Identity"
#define ARTMAP_ACTF_CL              "Act_at_least_2"
#define ARTMAP_ACTF_NC              "Act_at_least_1"
#define ARTMAP_ACTF_G               "Act_exactly_1"
#define ARTMAP_ACTF_RB              "Act_Product"
#define ARTMAP_ACTF_RM              "Act_Identity"
#define ARTMAP_ACTF_RG              "Act_less_than_0"
#define ARTMAP_ACTF_RHO             "Act_Identity"
#define ARTMAP_ACTF_QU              "Act_Product"
#define ARTMAP_ACTF_DRHO            "Act_ARTMAP_DRho"




/* Definition of the Sitefunctions for ART1 */
#define ART1_SITEF_RST_SELF      "Site_WeightedSum"
#define ART1_SITEF_RST_SIGNAL    "Site_at_least_2"
#define ART1_SITEF_REC_G1        "Site_at_most_0"
#define ART1_SITEF_INP_G1        "Site_at_least_1"
#define ART1_SITEF_RI            "Site_WeightedSum"


/* Definition of the Sitefunctions for ART2 */
#define ART2_SITEF_RST_SELF      "Site_WeightedSum"
#define ART2_SITEF_RST_SIGNAL    "Site_at_least_2"


/* Definition of the Sitefunctions for ARTMAP */
#define ARTMAP_SITEF_RSTa_SELF       "Site_WeightedSum"
#define ARTMAP_SITEF_RSTa_SIGNAL     "Site_at_least_2"
#define ARTMAP_SITEF_RECa_G1a        "Site_at_most_0"
#define ARTMAP_SITEF_INPa_G1a        "Site_at_least_1"
#define ARTMAP_SITEF_RIa             "Site_WeightedSum"

#define ARTMAP_SITEF_RSTb_SELF       "Site_WeightedSum"
#define ARTMAP_SITEF_RSTb_SIGNAL     "Site_at_least_2"
#define ARTMAP_SITEF_RECb_G1b        "Site_at_most_0"
#define ARTMAP_SITEF_INPb_G1b        "Site_at_least_1"
#define ARTMAP_SITEF_RIb             "Site_WeightedSum"

#define ARTMAP_SITEF_ARTa_G          "Site_at_least_1"
#define ARTMAP_SITEF_ARTb_G          "Site_at_least_1"
#define ARTMAP_SITEF_RB              "Site_WeightedSum"
#define ARTMAP_SITEF_INPa_QU         "Site_Reciprocal"
#define ARTMAP_SITEF_CMPa_QU         "Site_WeightedSum"



/* Global fix parameters */

/* for ART 2 */
#define ART2_PARAM_e                 0.00001




/************************** other macros ************************************/

/*  Don't call this macro unless kra1_sort() has been run
*/
#define ART1_CLASSIFIED       (Art1_cl_unit->Out.output >= 0.9)

/*  Don't call this macro unless kra1_sort() has been run
*/
#define ART1_NOT_CLASSIFIABLE (Art1_nc_unit->Out.output >= 0.9)


/*  Don't call this macro unless kra2_sort() has been run
*/
#define ART2_CLASSIFIED       kra2_classified()

/*  Don't call this macro unless kra2_sort() has been run
*/
#define ART2_NOT_CLASSIFIABLE kra2_not_classifiable()



/* Don't call this macro unless kram_sort() has been run
*/
#define ARTMAP_CLASSIFIED     (ArtMap_cl_unit->Out.output >= 0.9)

/* Don't call this macro unless kram_sort() has been run
*/
#define ARTMAP_NOT_CLASSIFIABLE  (ArtMap_nc_unit->Out.output >= 0.9)


/* Check if Sitefuncname is OK
*/
#define CHECK_SITE_FUNC(site_ptr, sitef_name)  \
                 (strcmp (krf_getFuncName ((FunctionPtr) site_ptr->site_table->site_func), \
                          sitef_name) \
                 ) == 0

/* Check if Actfuncname is OK
*/
#define CHECK_ACT_FUNC(unit_ptr, actf_name) \
                 (strcmp (krf_getFuncName ((FunctionPtr) unit_ptr->act_func), \
                          actf_name) \
                 ) == 0

/* Check if Outfuncname is OK
*/
#define CHECK_OUT_FUNC(unit_ptr, outf_name) \
                 (strcmp (krf_getFuncName ((FunctionPtr) unit_ptr->out_func), \
                           outf_name) \
                 ) == 0


/* Set topo_msg for unexpexted link
*/

#define TOPO_MSG_NO_OF_UNITS_IN_LAYER(layer_name) \
                  topo_msg.error_code = KRERR_NO_OF_UNITS_IN_LAYER; \
                  topo_msg.src_error_unit = 0; \
                  topo_msg.dest_error_unit = 0; \
                  strcpy (topo_msg.name, layer_name); \
                  return (topo_msg.error_code)

#define TOPO_MSG_UNIT_MISSING(unit_name) \
                  topo_msg.error_code = KRERR_UNIT_MISSING; \
                  topo_msg.src_error_unit = 0; \
                  topo_msg.dest_error_unit = 0; \
                  strcpy (topo_msg.name, unit_name); \
                  return (topo_msg.error_code)

#define TOPO_MSG_UNDETERMINED_UNIT(unit_ptr) \
                  topo_msg.error_code = KRERR_UNDETERMINED_UNIT; \
                  topo_msg.src_error_unit = 0; \
                  topo_msg.dest_error_unit = (unit_ptr)-unit_array

#define TOPO_MSG_ACT_FUNC(unit_ptr) \
                  topo_msg.error_code = KRERR_ACT_FUNC; \
                  topo_msg.src_error_unit = 0; \
                  topo_msg.dest_error_unit = (unit_ptr)-unit_array; \
                  return (topo_msg.error_code)

#define TOPO_MSG_OUT_FUNC(unit_ptr) \
                  topo_msg.error_code = KRERR_OUT_FUNC; \
                  topo_msg.src_error_unit = 0; \
                  topo_msg.dest_error_unit = (unit_ptr)-unit_array; \
                  return (topo_msg.error_code)

#define TOPO_MSG_SITE_FUNC(unit_ptr) \
                  topo_msg.error_code = KRERR_SITE_FUNC; \
                  topo_msg.src_error_unit = 0; \
                  topo_msg.dest_error_unit = (unit_ptr)-unit_array; \
                  return (topo_msg.error_code)

#define TOPO_MSG_UNEXPECTED_SITES(unit_ptr) \
                  topo_msg.error_code = KRERR_UNEXPECTED_SITES; \
                  topo_msg.src_error_unit = 0; \
                  topo_msg.dest_error_unit = (unit_ptr)-unit_array; \
                  return (topo_msg.error_code)

#define TOPO_MSG_SITE_MISSING(unit_ptr) \
                  topo_msg.error_code = KRERR_SITE_MISSING; \
                  topo_msg.src_error_unit = 0; \
                  topo_msg.dest_error_unit = (unit_ptr)-unit_array; \
                  return (topo_msg.error_code)

#define TOPO_MSG_UNEXPECTED_DIRECT_INPUTS(unit_ptr) \
                  topo_msg.error_code = KRERR_UNEXPECTED_DIRECT_INPUTS; \
                  topo_msg.src_error_unit = 0; \
                  topo_msg.dest_error_unit = (unit_ptr)-unit_array; \
                  return (topo_msg.error_code)

#define TOPO_MSG_UNEXPECTED_LINK(src_unit_ptr, dest_unit_ptr) \
                  topo_msg.error_code = KRERR_UNEXPECTED_LINK; \
                  topo_msg.src_error_unit = ((src_unit_ptr)-unit_array); \
                  topo_msg.dest_error_unit = ((dest_unit_ptr)-unit_array); \
                  return (topo_msg.error_code)

#define TOPO_MSG_LINK_MISSING(dest_unit_ptr) \
                  topo_msg.error_code = KRERR_LINK_MISSING; \
                  topo_msg.src_error_unit = 0; \
                  topo_msg.dest_error_unit = ((dest_unit_ptr)-unit_array); \
                  return (topo_msg.error_code)

#define TOPO_MSG_LINK_TO_WRONG_SITE(src_unit_ptr, dest_unit_ptr) \
                  topo_msg.error_code = KRERR_LINK_TO_WRONG_SITE; \
                  topo_msg.src_error_unit = ((src_unit_ptr)-unit_array); \
                  topo_msg.dest_error_unit = ((dest_unit_ptr)-unit_array); \
                  return (topo_msg.error_code)


#define ART1_RETURN_NET_ERROR(ret_code)  return ((ret_code = KRERR_TOPOLOGY))

#define ART2_RETURN_NET_ERROR(ret_code)  return ((ret_code = KRERR_TOPOLOGY))

#define ARTMAP_RETURN_NET_ERROR(ret_code) return ((ret_code = KRERR_TOPOLOGY))



#endif  /* ifndef __KR_ART_DEF */

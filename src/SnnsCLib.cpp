/************************************************************************************

   This file is part of SnnsCLib, a fork of the kernel and parts of the gui of 
   the Stuttgart Neural Network Simulator (SNNS), version 4.3.

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



//#include <R_ext/Print.h>

#include <stdlib.h>

#include "SnnsCLib.h"

SnnsCLib::SnnsCLib() {

//------------------------------------------------
// kr_ui.h
//------------------------------------------------

//(*krui_errorHandler)(int)=NULL;
rpckernel=FALSE;

UICurrentNameSearchUnitNo = 0;
UICurrentFtypeSite = NULL;
UICurrentFtypeEntry = NULL;
UICurrentNameSearchUnitSymbolPtr = NULL;
noOfStoredErrors=0;

krui_error_code = 0;
dotraining = 0; /* If True => train Network */

//NR double storedLearnErrors[NO_OF_STORED_ERRORS+1] = 0;

for(int i = 0; i < (NO_OF_STORED_ERRORS+1); i++) storedLearnErrors[i] = 0;

//NR int storedAtEpoch[NO_OF_STORED_ERRORS+1] = 0;

setIntArrayToZero(storedAtEpoch, NO_OF_STORED_ERRORS+1);

//former static variables

kr_ui_randomSeedVal=0;

krui_topo_err_msg_msg1[0] = 0;
krui_topo_err_msg_msg2[0] = 0;
krui_error_mesg[0] = 0;
krui_error_aux[0] = 0;

krui_getUnitActFuncName_act_func_name[0] = 0;
krui_getUnitOutFuncName_out_func_name[0] = 0;
krui_getFTypeActFuncName_act_func_name[0] = 0;
krui_getFTypeOutFuncName_out_func_name[0] = 0;

krui_getSiteFuncName_site_func_name[0] = 0;

krui_getUpdateFunc_updateFunc[0] = 0;
krui_getLearnFunc_learnFunc[0] = 0;
krui_getPrunFunc_prunFunc[0] = 0;
krui_getFFLearnFunc_FFLearnFunc[0] = 0;

krui_getVersion_snns_version[0] = 0;

//NR struct FuncInfoDescriptor  krui_getFuncInfo_functionDescr;
//NR struct FuncInfoDescriptor  krui_getFuncParamInfo_functionDescr;

setFuncInfoDescriptorToZero(&krui_getFuncInfo_functionDescr);
setFuncInfoDescriptorToZero(&krui_getFuncParamInfo_functionDescr);

//------------------------------------------------
// kr_newpattern.h
//------------------------------------------------

//NR int                npui_pat_sets[NO_OF_PAT_SETS];
//NR int                npui_insize[MAX_NO_OF_VAR_DIM];
//NR int                npui_outsize[MAX_NO_OF_VAR_DIM];
//NR int                npui_inpos[MAX_NO_OF_VAR_DIM];
//NR int                npui_outpos[MAX_NO_OF_VAR_DIM];
//NR int                npui_instep[MAX_NO_OF_VAR_DIM];
//NR int                npui_outstep[MAX_NO_OF_VAR_DIM];
//NR int                np_t_insize[MAX_NO_OF_VAR_DIM];
//NR int                np_t_outsize[MAX_NO_OF_VAR_DIM];
//NR int                np_t_instep[MAX_NO_OF_VAR_DIM];
//NR int                np_t_outstep[MAX_NO_OF_VAR_DIM];

setIntArrayToZero(npui_pat_sets, NO_OF_PAT_SETS);
setIntArrayToZero(npui_insize, MAX_NO_OF_VAR_DIM);
setIntArrayToZero(npui_outsize, MAX_NO_OF_VAR_DIM);
setIntArrayToZero(npui_inpos, MAX_NO_OF_VAR_DIM);
setIntArrayToZero(npui_outpos, MAX_NO_OF_VAR_DIM);
setIntArrayToZero(npui_instep, MAX_NO_OF_VAR_DIM);
setIntArrayToZero(npui_outstep, MAX_NO_OF_VAR_DIM);
setIntArrayToZero(np_t_insize, MAX_NO_OF_VAR_DIM);
setIntArrayToZero(np_t_outsize, MAX_NO_OF_VAR_DIM);
setIntArrayToZero(np_t_instep, MAX_NO_OF_VAR_DIM);
setIntArrayToZero(np_t_outstep, MAX_NO_OF_VAR_DIM);


npui_number_pat_sets             = 0;
npui_curr_pat_set                = -1;
npui_curr_pattern                = -1;
npui_show_defined                = FALSE;
npui_train_defined               = FALSE;
npui_shuffle_pattern             = FALSE;
npui_shuffle_sub_pattern         = FALSE;
np_pat_train_order              = NULL;
np_sub_pat_train_order          = NULL;
np_pat_mapping_order            = NULL;

np_pat_train_valid               = FALSE;
np_sub_pat_train_valid           = FALSE;
np_pat_mapping_valid             = FALSE;
np_pat_train_size                = 0;
np_sub_pat_train_size            = 0;
np_pat_mapping_size              = 0;
np_pat_train_number              = 0;
np_sub_pat_train_number          = 0;
np_next_train_pat                = -1;
np_next_train_sub_pat            = -1;

np_random_train_number           = 0;
np_next_train_random             = -1;
np_sub_pat_input_size            = 0;
np_sub_pat_output_size           = 0;
np_sub_pat_sizes_valid           = FALSE;
np_abs_count_No                  = 0;
np_abs_count_valid               = FALSE;
np_abs_count                    = NULL;
np_abs_count_size                = 0;
np_used_pat_set_entries       = 0;

np_pat_set_used              = NULL;
np_pat_sets                 = NULL;
np_info                      = NULL;
np_info_valid                = NULL;
np_st                       = NULL;

np_current_pattern           = NULL;
np_i_subpat                  = NULL;
np_i_subpatsize               = 0;
np_o_subpat                  = NULL;
np_o_subpatsize               = 0;

np_pages                     = NULL;

//former static variables

kr_getSubPatData_dummy_data = 0.0;

kr_np_order_pat_entries_shuffle = 0;
kr_np_order_pat_entries_c_start = 0;
kr_np_order_pat_entries_c_end = 0;

kr_np_order_sub_pat_entries_shuffle = 0;
kr_np_order_sub_pat_entries_c_start = 0;
kr_np_order_sub_pat_entries_c_end = 0;

kr_np_order_chunked_pat_entries_c_start = 0;
kr_np_order_chunked_pat_entries_c_end = 0;

//------------------------------------------------
// matrix.h
//------------------------------------------------

//------------------------------------------------
// func_tbl.h
//------------------------------------------------

//------------------------------------------------
// cc_modify.h
//------------------------------------------------

cco_ActualLayer = 0;

ccs_GroupList=NULL;
ccm_CurrentLayerHeight=0;
ccm_MissingUnitsInLayer=0;
ccs_GroupData=NULL;
ccr_ListOfChosenUnits=NULL;

//------------------------------------------------
// kernel.h
//------------------------------------------------

NetModified = FALSE;
NetInitialize = TRUE;
LearnFuncHasChanged = TRUE;
DefaultSType   = DEF_STYPE;

NoOfUnits	 = 0;
MinUnitNo	 = 0;
MaxUnitNo	 = 0;
NoOfInputUnits   = 0;
NoOfOutputUnits  = 0;
NoOfHiddenUnits  = 0;
TopoSortID       = NOT_SORTED;

unit_array       = NULL;
topo_ptr_array   = NULL;

no_of_topo_units = 0;

topo_ptr_array_size = 0;

KernelErrorCode = KRERR_NO_ERROR;

lineno = 0;

//NR struct TopologicMessages  topo_msg;

topo_msg.error_code = 0;
topo_msg.no_of_cycles = 0;
topo_msg.no_of_dead_units = 0;
topo_msg.no_of_layers = 0;
topo_msg.dest_error_unit = 0;
topo_msg.src_error_unit = 0;
topo_msg.name[0] = 0;


unitPtr = NULL;
sitePtr = NULL;
prevSitePtr = NULL;
linkPtr = NULL;
prevLinkPtr = NULL;
unitNo = 0;

specialNetworkType = NET_TYPE_GENERAL;

MinimumError = 1000000;

Temperature = 0;

#ifdef MASPAR_KERNEL
masParStatus = MASPAR_DISCONNECT;
#endif

kernel_global_topo_ptr = NULL;

DefaultSubnetNo  = DEF_SUBNET_NO;	 /*  default subnet no.  */
DefaultLayerNo   = DEF_LAYER_NO;	 /*  default layer no.	*/
DefaultPosX      = DEF_POS_X;	 /*  default x-position  */
DefaultPosY      = DEF_POS_Y;	 /*  default y-position  */

DefaultPosZ = DEF_POS_Z;	 /*  default z-position  */

    DefaultIAct      = DEF_I_ACT;	 /*  default initial activation  */
     DefaultBias      = DEF_BIAS;	 /*  default bias */

DefaultUFuncOut  = NULL; /*  default output function     */
DefaultUFuncAct  = NULL; /*  default activation function */

/*  default derivation act. function  */
DefaultUFuncActDeriv = NULL;

/*  default second derivation act. function  */
DefaultUFuncAct2Deriv = NULL;

transTable = NULL;
transTableSize = 0;


//former static variables

//#ifdef MASPAR_KERNEL
//struct NetFuncParameters  m_kernel_net_func_params;
//#endif

m_kernel_link_ptr = NULL;
m_kernel_source_unit_ptr = NULL;
m_kernel_current_unit_ptr = NULL;
m_kernel_current_site_ptr = NULL;

m_kernel_kr_CorrMatrix.rows = 0;
m_kernel_kr_CorrMatrix.columns = 0;
m_kernel_kr_CorrMatrix.field = NULL;
m_kernel_kr_CorrMatrix.r_pt = NULL;

m_kernel_activation_func[0] = 0;
m_kernel_output_func[0] = 0;

//------------------------------------------------
// kr_funcs.h
//------------------------------------------------

//netFuncInit = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
//                         FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE  };
for(int i=0; i<(NO_OF_FUNC_TYPES * NO_OF_KERNELS); i++) netFuncInit[i] = FALSE;

//NR char  CurrNetworkFunc[NO_OF_FUNC_TYPES * NO_OF_KERNELS][FUNCTION_NAME_MAX_LEN];

for(int i=0;i<(NO_OF_FUNC_TYPES * NO_OF_KERNELS);i++)
 for(int j=0;j<FUNCTION_NAME_MAX_LEN;j++)
   CurrNetworkFunc[i][j] = 0;

//former static variables

//NR struct FuncInfoDescriptor  krf_getFuncName_functionDescr;
//NR struct FuncInfoDescriptor  krf_getCurrentNetworkFunc_func_descr;

setFuncInfoDescriptorToZero(&krf_getFuncName_functionDescr);
setFuncInfoDescriptorToZero(&krf_getCurrentNetworkFunc_func_descr);

//memset(krf_getFuncName_functionDescr, 0, sizeof(FuncInfoDescriptor));

//------------------------------------------------
// scaled_conj_grad.h
//------------------------------------------------

scg_space_size = 0;
scg_gradient = NULL;

//former static variables

LEARN_SCG_OutParameter[0] = 0;
LEARN_SCG_k = 0;

LEARN_SCG_restart_scg = FALSE;
LEARN_SCG_stop_scg = FALSE;

LEARN_SCG_success = 0;
LEARN_SCG_count_under_tol = 0; 

LEARN_SCG_delta = 0;
LEARN_SCG_norm_of_p_2 = 0;
LEARN_SCG_lambda = 0;
LEARN_SCG_lambda_bar = 0;
LEARN_SCG_current_error = 0; 
LEARN_SCG_old_error = 0;
LEARN_SCG_norm_of_rk = 0;

LEARN_SCG_weights = NULL;
LEARN_SCG_old_gradient = NULL;
LEARN_SCG_p = NULL;
LEARN_SCG_r = NULL;
LEARN_SCG_old_weights = NULL;
LEARN_SCG_step = NULL;


//------------------------------------------------
// trans_f.h
//------------------------------------------------

ExtraParameter = 0;

//------------------------------------------------
// art_ui.h
//------------------------------------------------

//------------------------------------------------
// cc_prune.h
//------------------------------------------------

//------------------------------------------------
// kr_amap.h
//------------------------------------------------

ArtMap_NoOfInpUnits_a = 0;
ArtMap_NoOfInpUnits_b = 0;
ArtMap_NoOfRecUnits_a = 0;
ArtMap_NoOfRecUnits_b = 0;

ArtMap_cl_unit = NULL;  /* Pointer to cl-unit */
ArtMap_nc_unit = NULL;  /* Pointer to nc-unit */

ArtMap_map_layer = NULL; 

//------------------------------------------------
// kr_inversion.h
//------------------------------------------------

//------------------------------------------------
// learn_f.h
//------------------------------------------------

NoOfLearnedPatterns = 0;

//former static variables

LEARN_backprop_OutParameter[0] = 0;
TEST_backprop_OutParameter[0] = 0;
LEARN_backpropBatch_OutParameter[0] = 0;
LEARN_backpropMomentum_OutParameter[0] = 0;
TEST_backpropMomentum_OutParameter[0] = 0;
LEARN_backpropWeightDecay_OutParameter[0] = 0;
LEARN_backpropChunk_OutParameter[0] = 0;
LEARN_backpropJogChunk_OutParameter[0] = 0;
LEARN_backpropClassJogChunk_OutParameter[0] = 0;
LEARN_quickprop_OutParameter[0] = 0;
LEARN_CPN_OutParameter[0] = 0;
LEARN_perc_OutParameter[0] = 0;
LEARN_RBF_OutParameter[0] = 0;
LEARN_RBF_DDA_OutParameter[0] = 0;
LEARN_rprop_OutParameter[0] = 0;
LEARN_RpropMAP_OutParameter[0] = 0;
TEST_rprop_OutParameter[0] = 0;
TEST_MAP_OutParameter[0] = 0;
LEARN_BPTT_OutParameter[0] = 0;
TEST_BPTT_OutParameter[0] = 0;
LEARN_BBPTT_OutParameter[0] = 0;
LEARN_QPTT_OutParameter[0] = 0;
LEARN_kohonen_OutParameter[0] = 0;
LEARN_JE_Backprop_OutParameter[0] = 0;
TEST_JE_Backprop_OutParameter[0] = 0;
TEST_JE_BackpropMomentum_OutParameter[0] = 0;
LEARN_JE_BackpropMomentum_OutParameter[0] = 0;
LEARN_JE_Quickprop_OutParameter[0] = 0;
LEARN_JE_Rprop_OutParameter[0] = 0;
TEST_JE_Rprop_OutParameter[0] = 0;
LEARN_RM_delta_OutParameter[0] = 0;
LEARN_HEBB_OutParameter[0] = 0;

#ifdef RBF_DELTA_PROT
RbfLearnAdjustWeights_step = 0;
#endif

#ifdef RBF_LEARN_PROT
LEARN_RBF_schritt = 1;
#endif

LEARN_RpropMAP_counter=0;

//------------------------------------------------
// update_f.h
//------------------------------------------------

//former static variables

UPDATE_ART1_syncPropagate_rho = 0;
UPDATE_ART2_syncPropagate_rho = 0;
UPDATE_ART2_syncPropagate_param_a = 0;
UPDATE_ART2_syncPropagate_param_b = 0; 
UPDATE_ART2_syncPropagate_param_c = 0; 
UPDATE_ART2_syncPropagate_param_d = 0; 
UPDATE_ART2_syncPropagate_theta = 0;

UPDATE_ARTMAP_syncPropagate_rho_a = -1.0;
UPDATE_ARTMAP_syncPropagate_rho_b = -1.0;
UPDATE_ARTMAP_syncPropagate_rho   = -1.0;

//------------------------------------------------
// cc_display.h
//------------------------------------------------

cc_display_mode=Y_MAX_MODE1;
cc_LayerDistance=DEFAULT_DISTANCE_BETWEEN_LAYERS;

cc_hiddenXminPos = 0;
cc_lastFirstOutputRow = 0;

//------------------------------------------------
// dlvq_learn.h
//------------------------------------------------

newPatternsLoaded = 0;
dlvq_numberOfLearnCycles=0;

initialUnitArray= NULL;
mixupArray = NULL;
lastInsertedUnitArray = NULL;

noOfClasses=0;
oldNoOfClasses=0;
wrongClassCounter=0;
continueLearning=0;

//former static variables

dlvq_learn_cycleCounter=0;
dlvq_learn_noOfTrainingCycles=0;
dlvq_learn_learnParam1=0.03;
dlvq_learn_learnParam2=0.03;

dlvq_learn_OutParameter[0] = 0;

//------------------------------------------------
// kr_art1.h
//------------------------------------------------

Art1_NoOfRecUnits = 0;

Art1_cl_unit = NULL; /* Pointer to ART1 cl-unit */
Art1_nc_unit = NULL; /* Pointer to ART1 nc-unit */

Art1_del_layer = NULL;


//------------------------------------------------
// kr_io.h
//------------------------------------------------

file_in = NULL;
//file_out = NULL;

is_subnet_info = 0;
is_layer_info = 0;
units_have_sites = 0;

site_name_len = 0;
unit_name_len = 0;
type_name_len = 0;
site_func_len = 0;
act_func_len = 0;
out_func_len = 0;
def_act_func_len = 0;
def_out_func_len = 0;
unit_no_len = 0;
max_connects_per_line = 0;
max_subnets_per_line = 0;
max_layers_per_line = 0;
pos_no_len = 0;
subnet_no_len = 0;
layer_no_len = 0;
kr_io_NoOfLinks = 0;
NoOfSiteTypes = 0;
NoOfUnitTypes = 0;
NetfileVersion = 0;

fmt_shape1[0] = 0;
fmt_shape2[0] = 0;
fmt_shape3[0] = 0;
fmt_shape4[0] = 0;
fmt_hdr1[0] = 0;
fmt_hdr2[0] = 0;
fmt_blank[0] = 0;

//former static variables

krio_getIOVersion_getIOVersion[0] = 0;
krio_repchar_str[0] = 0;

krio_fmtShapeing_formats[0] = const_cast<char*>("site name");
krio_fmtShapeing_formats[1] = const_cast<char*>("sites");
krio_fmtShapeing_formats[2] = const_cast<char*>("unitName");
krio_fmtShapeing_formats[3] = const_cast<char*>("typeName");
krio_fmtShapeing_formats[4] = const_cast<char*>("name");
krio_fmtShapeing_formats[5] = const_cast<char*>("site function");
krio_fmtShapeing_formats[6] = const_cast<char*>("act func");
krio_fmtShapeing_formats[7] = const_cast<char*>("out func");
krio_fmtShapeing_formats[8] = const_cast<char*>("no.");
krio_fmtShapeing_formats[9] = const_cast<char*>("target");
krio_fmtShapeing_formats[10] = const_cast<char*>("act");
krio_fmtShapeing_formats[11] = const_cast<char*>("bias");
krio_fmtShapeing_formats[12] = const_cast<char*>("position");
krio_fmtShapeing_formats[13] = const_cast<char*>("site");
krio_fmtShapeing_formats[14] = const_cast<char*>(" source:weight");
krio_fmtShapeing_formats[15] = const_cast<char*>("subnet");
krio_fmtShapeing_formats[16] = const_cast<char*>("layer");
krio_fmtShapeing_formats[17] = const_cast<char*>(" unitNo.");

krio_writeUnitDefinitions_blank = const_cast<char*>("\0");

krio_getTType_ttype[0] = const_cast<char*>("-");
krio_getTType_ttype[1] = const_cast<char*>("i");
krio_getTType_ttype[2] = const_cast<char*>("o");
krio_getTType_ttype[3] = const_cast<char*>("d");
krio_getTType_ttype[4] = const_cast<char*>("h");
krio_getTType_ttype[5] = const_cast<char*>("s");
krio_getTType_ttype[6] = const_cast<char*>("si");
krio_getTType_ttype[7] = const_cast<char*>("so");
krio_getTType_ttype[8] = const_cast<char*>("sh");
krio_getTType_ttype[9] = const_cast<char*>("sd");

krio_loadNet_netname_str[0] = 0;
krio_loadNet_netfile_version_str[0] = 0;
krio_loadNet_learn_func[0] = 0;
krio_loadNet_update_func[0] = 0;
krio_loadNet_pruning_func[0] = 0;
krio_loadNet_ff_learn_func[0] = 0;

//------------------------------------------------
// stochastic_learn_f.h
//------------------------------------------------

LEARN_MonteCarlo_OutParameter[0] = 0;
TEST_MonteCarlo_OutParameter[0] = 0;
LEARN_SimulatedAnnealing_OutParameter[0] = 0;
TEST_SimulatedAnnealing_OutParameter[0] = 0;

//------------------------------------------------
// cc_glob.h
//------------------------------------------------


/*
typedef struct CC_DATA {
  struct {
    float pixelError;
    int learningFunc;
    int modification;
    int onOff;
    int pruneOnOff;
    int backfittOnOff;
    int backfittPatience;
    int pruneFunc;
    float addParameters[5]; 
    int fastmode;
  } GLOBAL;
  struct {
    double covarianceChange;
    int    candidatePatience;
    int    maxNoOfUpdateCycles;
    int    maxNoOfCandUnits;
    int    actFunc;
  } CAND;
  struct {
    double errorChange;
    int    outputPatience;
    int    maxNoOfUpdateCycles;
  } OUT;
} CASCADE_TYPE;
*/

cc_data.GLOBAL.pixelError = MAX_PIXEL_ERROR;
cc_data.GLOBAL.learningFunc = LEARNING_FUNC;
cc_data.GLOBAL.modification = MODIFICATION;
cc_data.GLOBAL.onOff = ON_OFF;
cc_data.GLOBAL.pruneOnOff = 0;
cc_data.GLOBAL.backfittOnOff = 0;
cc_data.GLOBAL.backfittPatience = 150;
cc_data.GLOBAL.pruneFunc = SBC;
cc_data.GLOBAL.addParameters[0]=0.0; 
cc_data.GLOBAL.addParameters[1]=0.0; 
cc_data.GLOBAL.addParameters[2]=0.0; 
cc_data.GLOBAL.addParameters[3]=0.0; 
cc_data.GLOBAL.addParameters[4]=0.0; 
cc_data.GLOBAL.fastmode=OFF;

cc_data.CAND.covarianceChange = MIN_COVARIANCE_CHANGE;
cc_data.CAND.candidatePatience = SPECIAL_PATIENCE;
cc_data.CAND.maxNoOfUpdateCycles = MAX_NO_OF_COVARIANCE_UPDATE_CYCLES;
cc_data.CAND.maxNoOfCandUnits = MAX_SPECIAL_UNIT_NO;
cc_data.CAND.actFunc = SPECIAL_FUNC_TYPE;

cc_data.OUT.errorChange = MIN_ERROR_CHANGE;
cc_data.OUT.outputPatience = OUT_PATIENCE;
cc_data.OUT.maxNoOfUpdateCycles = MAX_NO_OF_ERROR_UPDATE_CYCLES;

/*cc_data ={
  { MAX_PIXEL_ERROR,
    LEARNING_FUNC, MODIFICATION,
    ON_OFF, 0, 0, 150, SBC,
    { 0.0, 0.0, 0.0, 0.0, 0.0 },
    OFF},
  { MIN_COVARIANCE_CHANGE, 
    SPECIAL_PATIENCE,
    MAX_NO_OF_COVARIANCE_UPDATE_CYCLES,
    MAX_SPECIAL_UNIT_NO, 
    SPECIAL_FUNC_TYPE },
  { MIN_ERROR_CHANGE,
    OUT_PATIENCE,
    MAX_NO_OF_ERROR_UPDATE_CYCLES}
};*/

cc_end                = 0;
cc_storageFree        = 1;
cc_allButtonIsPressed = 1;

FirstInputUnitPtr = NULL;
FirstHiddenUnitPtr = NULL;
FirstOutputUnitPtr = NULL;
FirstSpecialUnitPtr = NULL;
bestSpecialUnitPtr = NULL;

OutputUnitError=NULL;
SpecialUnitAct=NULL;
CorBetweenSpecialActAndOutError=NULL;
MeanOutputUnitError=NULL;
SpecialUnitSumAct=NULL;

LastInsertedHiddenUnit = 0;

SumSqError = 0;

cc_printOnOff = 0;
cc_backfittingOnOff = 0;
cc_MaxSpecialUnitNo = 0;

cc_modification = -1;

cc_fastmode = 0;

cc_Parameter[0] = 0;
cc_Parameter[1] = 0;
cc_Parameter[2] = 0;
cc_Parameter[3] = 0;
cc_Parameter[4] = 0;

reset=0;

cc_cascade=0;
NoOfLayers = 0;             /* number hidden layers */
ListOfLayers=NULL;  /* data of layers */

SizeOfLayerlist = 0;  
cc_outputXMax = 0;

cc_actualNetSaved = FALSE;
ActOfUnit = NULL;
NetLearnAlgorithm = NO_INC_ALGORITHM;

cc_fse = 0.1;

cc_learningFunction = 0;

prot_fd = -1;

prot_stream = NULL;
DumpText[0] = 0;
cc_NetErgProtocolled = 0;
CountSpecTrains = 0;
CountOutTrains = 0;
CountLinks = 0;

filedesc = -1;

OldNoOfSpecialUnitStorage = 0;

global_topo_ptr = NULL;

//------------------------------------------------
// kr_art2.h
//------------------------------------------------

Art2_NoOfRecUnits = 0;
Art2_cl_unit = NULL;
Art2_nc_unit = NULL;

/* Global variable for parameter values */

Param_rho   = 0.8;
Param_a     = 0.0;
Param_b     = 0.0;
Param_c     = 0.0;
Param_d     = 0.0;
Param_theta = 0.5;

/* Global variables for vector norms */

NormInp     = 0.0;
NormW       = 0.0;
NormU       = 0.0;
NormV       = 0.0;
NormP       = 0.0;
NormR       = 0.0;

NoOfDelaySteps = 0;

GlobalReset     = FALSE;
f1_stable = FALSE;

//NR TopoPtrArray   topo_layer[10];

topo_layer[0] = NULL;
topo_layer[1] = NULL;
topo_layer[2] = NULL;
topo_layer[3] = NULL;
topo_layer[4] = NULL;
topo_layer[5] = NULL;
topo_layer[6] = NULL;
topo_layer[7] = NULL;
topo_layer[8] = NULL;
topo_layer[9] = NULL;


TopDownPhase = 0;

//------------------------------------------------
// kr_JordElm.h
//------------------------------------------------

topo_ptr = NULL;

//------------------------------------------------
// prun_f.h
//------------------------------------------------

pr_trainCycles = DEFAULT_TRAIN_CYCLES;
pr_retrainCycles = DEFAULT_RETRAIN_CYCLES;
pr_recreate = TRUE;
pr_refresh = FALSE;
pr_inputPruning = TRUE;
pr_hiddenPruning = TRUE;

pr_maxErrorInc = DEFAULT_ERROR_INCREASE;
pr_acceptedError = DEFAULT_ACCEPTED_ERROR;
pr_minError = DEFAULT_MIN_ERROR;
pr_obs_initParameter = DEFAULT_OBS_INIT_PARAMETER;

pr_candidateTargetUnit = NULL;
pr_candidateUnit = NULL;
pr_candidateLink = NULL;

pr_candidateSaliency = 0;
pr_candidateStddev = 0;
pr_candidateSourceUnitNo = 0;
pr_candidatePass = 0;
pr_Pass = 0;

/* variables only used by OBS */
pr_candidateLinkNo = 0;
pr_noOfLinks = 0;

//NR RbfFloatMatrix pr_inverseHessian, pr_derivVector, pr_helpHX, pr_helpXH;

pr_inverseHessian.rows = 0;
pr_inverseHessian.columns = 0;
pr_inverseHessian.field = NULL;
pr_inverseHessian.r_pt = NULL;

pr_derivVector.rows = 0;
pr_derivVector.columns = 0;
pr_derivVector.field = NULL;
pr_derivVector.r_pt = NULL;

pr_helpHX.rows = 0;
pr_helpHX.columns = 0;
pr_helpHX.field = NULL;
pr_helpHX.r_pt = NULL;

pr_helpXH.rows = 0;
pr_helpXH.columns = 0;
pr_helpXH.field = NULL;
pr_helpXH.r_pt = NULL;

//former static variables

PRUNE_Skeletonization_first = TRUE;

//------------------------------------------------
// tacoma_learn.h
//------------------------------------------------

PatternSumError = NULL;
WholeSummedError = 0;
Rij = NULL;
Nij = NULL;  
MeanYi = NULL;  /* points to SpecialUnitSumAct */
SpecialUnitData = NULL;
PrimesOfSpecialUnits = NULL;
NoOfInstalledUnits = 0;
AntiCorr = 0;
AC_Zaehler = 0;
AC_Nenner = 0;

//------------------------------------------------
// cc_learn.h
//------------------------------------------------

//former static variables

m_cc_trainOutputUnits_OutParameter[0] = 0;
m_TEST_CasCor_OutParameter[0] = 0;
m_cc_learn_GroupNo = 0;
m_cc_learn_LeTe = 0;

//------------------------------------------------
// init_f.h
//------------------------------------------------

//former static variables

m_init_f_notfirstrun = 0;

//------------------------------------------------
// kr_art.h
//------------------------------------------------

//------------------------------------------------
// kr_mem.h
//------------------------------------------------

NoOfAllocPatternPairs = 0;  /*  no. of allocated pattern pairs  */


FreeUnitIndex  = 0; /* no. of the next free unit in the unit array*/
NoOfAllocUnits = 0;  /*  no. of allocated units  */
NoOfSites	   = 0; /* no. of used sites in the network and Ftype 
				   sites (including sites for functionality 
				   type use only) */
NoOfNetSites   = 0; /* no. of used sites in the network (only unit 
				   sites)  */
NoOfAllocSites = 0;  /*  no. of allocated sites  */
NoOfLinks	   = 0;  /*  no. of links used for the network	*/
NoOfAllocLinks = 0;  /*  no. of allocated links  */
NoOfNTableEntries	    = 0; /*no. of name table entries  */
NoOfAllocNTableEntries  = 0; /*no. of allocated name table entries*/
NoOfSTableEntries	    = 0; /*no. of site table entries  */
NoOfAllocSTableEntries  = 0; /*no. of allocated site table entries*/
NoOfFTableEntries	    = 0; /*no. of functionality types  */

site_array        = NULL;  /*  pointer to first site array  */
free_site_ptr	 = NULL;  /*  pointer to first free site  */
site_block_list	 = NULL;  /*  pointer to first free site block */

link_array        = NULL;  /*  pointer to first link array  */
free_link_ptr	 = NULL;  /*  pointer to first free link  */
link_block_list	 = NULL;  /*  pointer to first free link block */


NTable_array      = NULL;  /* pointer to name table  */
free_NTable_entry = NULL;  /* ptr to first free name table entry  */
NTable_block_list = NULL;  /* ptr to first free name table block  */
curr_NTable_entry = NULL;  /*  pointer to current name table entry  */
curr_NTable_block = NULL;  /*  pointer to current name table block  */


STable_array      = NULL;  /* pointer to site table  */
free_STable_entry = NULL;  /* ptr to first free site table entry  */
STable_block_list = NULL;  /* ptr to first free site table block  */
curr_STable_entry = NULL;  /*  pointer to current name site entry  */
curr_STable_block = NULL;  /*  pointer to current name site block  */


Ftype_list_root  = NULL;  /*  pointer to root of the Ftype list  */
curr_Ftype_entry = NULL;  /*  pointer to current Ftype entry  */

//former static variables

//NR int krm_putPattern_in[MAX_NO_OF_VAR_I_DIM],krm_putPattern_out[MAX_NO_OF_VAR_I_DIM];

setIntArrayToZero(krm_putPattern_in, MAX_NO_OF_VAR_I_DIM);
setIntArrayToZero(krm_putPattern_out, MAX_NO_OF_VAR_I_DIM);

//------------------------------------------------
// kr_td.h
//------------------------------------------------

//former static variables

LEARN_TDbackprop_OutParameter[0] = 0;
TEST_TDbackprop_OutParameter[0] = 0;
LEARN_TDBP_McClelland_OutParameter[0] = 0;

//------------------------------------------------
// remap_f.h
//------------------------------------------------

//------------------------------------------------
// tbl_func.h
//------------------------------------------------

//------------------------------------------------
// kr_pat_scan.h
//------------------------------------------------

s_dotpos = NULL;
scanner_pattern_end_flag = 0;
scanner_no_class_flag = 0;
numbers_as_names = 0;

//------------------------------------------------
// kr_pat_parse.h
//------------------------------------------------

//i = 0;

//NR int	dims[MAX_NO_OF_VAR_DIM];
//NR int	max_i_dims[MAX_NO_OF_VAR_I_DIM];
//NR int	max_o_dims[MAX_NO_OF_VAR_O_DIM];

setIntArrayToZero(dims, MAX_NO_OF_VAR_DIM);
setIntArrayToZero(max_i_dims, MAX_NO_OF_VAR_I_DIM);
setIntArrayToZero(max_o_dims, MAX_NO_OF_VAR_O_DIM);

actual_dim_count = 0;
variable_input_dim = 0;
variable_output_dim = 0;
current_pattern = 0;
next_pattern_is_input = 0;
no_of_input = 0;
no_of_output = 0;
no_of_pattern = 0;
act_size = 0;

pat_mem = NULL;
pattern_set = 0;
pattern = NULL;
with_classes = 0;
this_class = NULL;
num_param = 0;
parameter = NULL;

this_set = NULL;


//------------------------------------------------
// Initialize the random number generator
//------------------------------------------------

krui_setSeedNo(u_getCurrentSeedVal());

}



//------------------------------------------------
// Destructor: delete net and pat sets
//------------------------------------------------

SnnsCLib::~SnnsCLib() {

  krui_err err = 0;

  while (err == 0) {
    err = krui_deletePatSet(0);
  }
  
  krui_deleteNet();

//Delete anything that may be left

  if (np_pat_train_order != (int *) NULL)
    free(np_pat_train_order);

  if (np_sub_pat_train_order != (int *) NULL)
    free(np_sub_pat_train_order);

  if (np_pat_mapping_order != (int *) NULL)
    free(np_pat_mapping_order);

  if (np_abs_count != (int *) NULL)
    free(np_abs_count);

  if (np_pat_set_used != (bool *) NULL)
    free(np_pat_set_used);

  if (np_pat_sets != (np_pattern_descriptor**) NULL)
    free(np_pat_sets);

  if (np_info != (np_pattern_set_info*) NULL)
    free(np_info);

  if (np_info_valid != (bool *) NULL)
    free(np_info_valid);

  if (np_st != (np_symtab**) NULL)
    free(np_st);

//DLVQ

  if (lastInsertedUnitArray != (int *) NULL)
    free(lastInsertedUnitArray);

  if (initialUnitArray != (MIX_UP*) NULL) {
    for(int j=0;j<noOfClasses;j++) {
	if(initialUnitArray[j].link != (double*) NULL)
          free(initialUnitArray[j].link);
    }
    free(initialUnitArray);
  }


  if (mixupArray != (MIX_UP**) NULL) {

    for(int i=0;i<noOfClasses;i++) {
      if (mixupArray[i] != (MIX_UP*) NULL) {
	for(int j=0;j<noOfClasses;j++) {
	    if(mixupArray[i][j].link != (double*) NULL)
              free(mixupArray[i][j].link);
	}
        free(mixupArray[i]);
      }
    }
    free(mixupArray);
  }

}

void SnnsCLib::setIntArrayToZero(int* array, int n) {

  for(int i=0;i<n;i++) array[i] = 0;
}

void SnnsCLib::setFuncInfoDescriptorToZero(FuncInfoDescriptor* f) {

  f->number = 0;
  f->func_name[0] = 0;
  f->func_type = 0;
  f->no_of_input_parameters = 0;
  f->no_of_output_parameters = 0;
  f->function = NULL;
#ifdef PARAGON_KERNEL
  f->parallelized = 0;
#endif
}


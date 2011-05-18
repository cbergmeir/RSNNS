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

#include "SnnsCLib.h"

SnnsCLib::SnnsCLib() {

//cc_modify.ph
ccs_GroupList=NULL;
ccm_CurrentLayerHeight=0;
ccm_MissingUnitsInLayer=0;
ccs_GroupData=NULL;
ccr_ListOfChosenUnits=NULL;

//kernel.ph
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

KernelErrorCode = KRERR_NO_ERROR;

lineno = 0;

unitPtr = NULL;
sitePtr = NULL;
prevSitePtr = NULL;
linkPtr = NULL;
prevLinkPtr = NULL;
unitNo = 0;

specialNetworkType = NET_TYPE_GENERAL;

MinimumError = 1000000;

#ifdef MASPAR_KERNEL
masParStatus = MASPAR_DISCONNECT;
#endif


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

//kr_funcs.ph

//netFuncInit = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
//                         FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE  };
for(int i=0; i<(NO_OF_FUNC_TYPES * NO_OF_KERNELS); i++) netFuncInit[i] = FALSE;

//--------------------------------
//kr_newpattern.ph
//--------------------------------

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

//--------------------------------
//kr_ui.ph
//--------------------------------

//(*krui_errorHandler)(int)=NULL;
rpckernel=FALSE;

UICurrentNameSearchUnitNo = 0;
UICurrentFtypeSite = NULL;
UICurrentFtypeEntry = NULL;
UICurrentNameSearchUnitSymbolPtr = NULL;
noOfStoredErrors=0;

//--------------------------------
//kr_amap.ph
//--------------------------------
ArtMap_map_layer = NULL; 

//--------------------------------
//kr_art1.ph
//--------------------------------

Art1_del_layer = NULL;

//--------------------------------
//kr_art2.ph
//--------------------------------

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


//--------------------------------
//kr_pat_parse.ph
//--------------------------------

num_param = 0;
parameter = NULL;

//--------------------------------
//cc_display.ph
//--------------------------------

cc_display_mode=Y_MAX_MODE1;
cc_LayerDistance=DEFAULT_DISTANCE_BETWEEN_LAYERS;

//--------------------------------
//dlvq_learn.ph
//--------------------------------

newPatternsLoaded = 0;
dlvq_numberOfLearnCycles=0;

noOfClasses=0;
oldNoOfClasses=0;
wrongClassCounter=0;
continueLearning=0;

// This fixes a bug which probably also is present in the original SNNS version
// that dlvq tries to free the memory of uninitialized pointers, if they happen to be
// different from NULL
initialUnitArray= NULL;
mixupArray = NULL;
lastInsertedUnitArray = NULL;


//--------------------------------
//cc_glob.ph
//--------------------------------
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



OutputUnitError=NULL;
SpecialUnitAct=NULL;
CorBetweenSpecialActAndOutError=NULL;
MeanOutputUnitError=NULL;
SpecialUnitSumAct=NULL;

cc_modification = -1;

reset=0;

cc_cascade=0;
NoOfLayers = 0;             /* number hidden layers */
ListOfLayers=NULL;  /* data of layers */

cc_actualNetSaved = FALSE;
ActOfUnit = NULL;
NetLearnAlgorithm = NO_INC_ALGORITHM;

cc_fse = 0.1;

prot_fd = -1;

filedesc = -1;

OldNoOfSpecialUnitStorage = 0;

//--------------------------------
//prun_f.ph
//--------------------------------

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

//--------------------------------
//tacoma_learn.ph
//--------------------------------

PatternSumError=NULL;
SpecialUnitData=NULL;

//--------------------------------
//kr_mem.ph
//--------------------------------

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

dlvq_learn_cycleCounter=0;
dlvq_learn_noOfTrainingCycles=0;
dlvq_learn_learnParam1=0.03;
dlvq_learn_learnParam2=0.03;

m_kernel_link_ptr = NULL;
//m_kernel_source_unit_ptr = NULL;
m_kernel_current_unit_ptr = NULL;
m_kernel_current_site_ptr = NULL;

m_kernel_kr_CorrMatrix.rows = 0;
m_kernel_kr_CorrMatrix.columns = 0;
m_kernel_kr_CorrMatrix.field = NULL;
m_kernel_kr_CorrMatrix.r_pt = NULL;

kr_ui_randomSeedVal=0;

PRUNE_Skeletonization_first = TRUE;

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


#ifdef RBF_DELTA_PROT
RbfLearnAdjustWeights_step = 0;
#endif

#ifdef RBF_LEARN_PROT
LEARN_RBF_schritt = 1;
#endif

LEARN_RpropMAP_counter=0;


LEARN_SCG_restart_scg = FALSE;
LEARN_SCG_stop_scg = FALSE;

UPDATE_ARTMAP_syncPropagate_rho_a = -1.0;
UPDATE_ARTMAP_syncPropagate_rho_b = -1.0;
UPDATE_ARTMAP_syncPropagate_rho   = -1.0;

kr_getSubPatData_dummy_data = 0.0;

krio_loadNet_netname_str[0] = 0;
krio_loadNet_netfile_version_str[0] = 0;
krio_loadNet_learn_func[0] = 0;
krio_loadNet_update_func[0] = 0;
krio_loadNet_pruning_func[0] = 0;
krio_loadNet_ff_learn_func[0] = 0;


//Initialize the random number generator
krui_setSeedNo(u_getCurrentSeedVal());

}

SnnsCLib::~SnnsCLib() {}



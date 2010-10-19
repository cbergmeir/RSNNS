#ifndef SNNS_HEADER_ALL
#define SNNS_HEADER_ALL

#include "config.h"

#include "glob_typ.h"

#include "kr_typ.h"
#include "kr_const.h"
#include "kr_def.h"
#include "kr_mac.h"

#include "ext_typ.h"
#include "krui_typ.h"
#include "art_typ.h" 
#include "krart_df.h"
 
#include "func_mac.h"

#include "cc_type.h"  
#include "cc_mac.h"
#include "dlvq_type.h"  
#include "enzo_mem_typ.h"

#include "version.h"

//#include "func_tbl.ph"

//--------------------------------
//kr_io.ph
//--------------------------------

static char *title[] = {
    "SNNS network definition file",
    "generated at",
    "network name",
    "no. of units",
    "no. of connections",
    "no. of unit types",
    "no. of site types",
    "learning function",
    "site definition section",
    "type definition section",
    "unit definition section",
    "connection definition section",
    "subnet definition section",
    "unit default section",
    "source files",
    "layer definition section",
    "update function",
    "3D translation section",
    "time delay section",
    "pruning function",
    "subordinate learning function"
/*
    "topologic definition section",
    "feedforward network"
*/
    };	 /*  20  */

static char *resHeader[] = {
    "SNNS result file %s\n",
    "generated at",
    "No. of patterns     : %d\n",
    "No. of input units  : %d\n",
    "No. of output units : %d\n",
    "startpattern        : %d\n",
    "endpattern          : %d\n",
    "input patterns included\n",
    "teaching output included\n"
    };

static char *headers[] = {
    " site name | site function",
    " name | act func | out func | sites",
    " act | bias | st | subnet | layer | act func | out func",
    " no. | typeName | unitName | act | bias | st | position | act func | out func | sites",
    " target | site | source:weight",
    " subnet | unitNo.",
    " layer | unitNo.",
    " delta x | delta y | z",
    " no. | LLN | LUN | Toff | Soff | Ctype"
    };


//--------------------------------
//cc_glob.ph
//--------------------------------

static char *cc_actFuncArray[]     ={"Act_Logistic","Act_LogSym","Act_TanH",
                              "Act_CC_Thresh","Act_Sinus","Act_Exponential",
			      "Act_Random"};
static char *cc_actFuncArray2[]     ={"Logistic"," LogSym ",
			      "  TanH  ","dummy"," Sinus  ",
			      "  Gauss "," Random "};
static char *cc_learningFuncArray[]={" Batch-BP  "," Backprop  ",
			      " Quickprop ","   Rprop   "};
static char *cc_ModificationArray[]={"   none ","   SDCC ","   LFCC ","   RLCC ",
			      "   ECC  ","   GCC  ","  Static"};
static char *cc_pruningFuncArray[] ={" SBC "," AIC ","CMSEP"};


    static char *err_message[] = {
        "Invalid error code",
        "Insufficient memory",
        "Invalid unit number",
        "Invalid unit output function",
        "Invalid unit activation function",
        "Invalid site function",
        "Creation of sites isn't permitted because unit has direct input links",
        "Creation of a link isn't permitted because there exists already a link between these units",
        "Memory allocation failed during critical operation. Have lost some pointers, but consistency of the network is guaranteed",
        "Ftype name isn't definite",
        /*10*/"Current Ftype entry isn't defined",
        "Invalid copy mode",
        "Current unit doesn't have sites",
        "Can't update unit because unit is frozen",
        "Redefinition of site name isn't permitted (site name already exists)",
        "Site name isn't defined",
        "This is not a 3D-Kernel",
        "This unit has already a site with this name",
        "Can't delete site table entry because site is in use",
        "Current Ftype site isn't defined",
        /*20*/"Ftype symbol isn't defined",
        "I/O error: ",
        "Creation of output file failed (line length limit exceeded)",
        "The network has not enough layers: ",
        "No Units defined",
        "Unexpected EOF",
        "Line length exceeded",
        "Incompatible file format",
        "Can't open file",
        "Syntax error",
        /*30*/"Memory allocation error #01",
        "Topologic type invalid",
        "Symbol pattern invalid (must match [A-Za-z][^|, ]*)",
        "Current unit doesn't have a site with this name",
        "No hidden units defined",
        "Network contains cycle(s): ",
        "Network contains dead unit(s): ",
        "Pattern file contains not the same no. of input units as the network",
        "Pattern file contains not the same no. of output units as the network",
        "No. of input units have changed",
        /*40*/"No. of output units have changed",
        "No input units defined",
        "No output units defined",
        "No patterns defined",
        "In-Core patterns incompatible with current network (remove In-Core patterns before loading a new network)",
        "Invalid pattern number",
        "Invalid learning function",
        "Invalid parameters",
        "Invalid update function",
        "Invalid initialisation function",
        /*50*/"Derivation function of the activation function doesn't exist",
        "Input unit(s) with input connection(s) to other units: ",
        "Output unit(s) with output connection(s) to other units: ",
        "Invalid topological sorting mode",
        "Learning function doesn't support sites",
        "Sites are not supported",
        "This isn't a MasPar Kernel",
        "Connection(s) between unit(s) in non-neighbour layers are not supported: ",
        "The network has too much layers: ",
        "The network layers aren't fully connected",
        /*60*/"This operation isn't allowed in the parallel kernel mode",
        "Change of network type isn't possible in parallel kernel mode",
        "No current link defined",
        "No current unit defined",
        "Current unit doesn't have any inputs",
        "Invalid parameter in topologic definition section",
        "Creation of link between these units isn't permitted",
        "MasPar don't respond",
        "This function isn't implemented yet",
        "Kernel isn't in parallel mode",
        /*70*/"MasPar ran out of memory",
        "MasPar communication error",
        "MasPar ran out of processors",
        "Missing default function (check function table)",
        "MasPar kernel doesn't support multiple unit output functions",
        "MasPar kernel doesn't support multiple unit activation functions",
        "The depth of the network doesn't fit to the learning function",
        "Wrong no of units in layer: ",
        "Unit is missing or not correctly connected: ",
        "Unit doesn't belong to a defined layer in the network: ",
        /*80*/"Unit has wrong activation function: ",
        "Unit has wrong output function: ",
        "Unexpected site function at unit: ",
        "Unit is not expected to have sites: ",
        "Unit is expected to have sites: ",
        "Site missing at unit: ",
        "Unexpected link: ",
        "Missing link(s) to unit: ",
        "Link ends at wrong site of destination unit: ",
        "This network is not fitting the required topology",
        /*90*/"Wrong beta parameter in unit bias value: ",
        "CC error: Topo_ptr_array is sorted in the wrong way", /*CC_ERROR1*/
        "CC error: There is no memory allocated", /*CC_ERROR2*/
        "CC error: Not enough memory to run Casscade", /*CC_ERROR3*/
        "Invalid error code", /*RCC_ERROR4*/
        "Invalid error code", /*RCC_ERROR5*/
        "CC error: Hidden layer is not fitting the required topology",
        "Invalid error code", /*RCC_ERROR7*/
        "Invalid error code", /*RCC_ERROR8*/
        "Invalid error code", /*RCC_ERROR9*/
        /*100*/ "Wrong update function",/*CC_ERROR10*/
        "Wrong init function",  /*CC_ERROR11*/
        "DLVQ error: There are empty classes",
        "DLVQ error: There exists a class lower than zero",
        "DLVQ error: Wrong no. of output units",
        "DLVQ error: This network is not fitting the required topology",
        "DLVQ error: There does not exist a unit for every class",
        "No more free pattern sets available",
        "No such pattern set defined",
        "No current pattern defined",
        /*110*/ "Specified sub pattern does not fit into pattern",
        "No such pattern available",
        "No current pattern set defined",
        "Pattern (sub pattern) does not fit the network",
        "No sub pattern shifting scheme defined",
        "Pattern contains no output information",
        "New pattern does not fit into existing set",
        "Paragon kernel not initialized",
        "Paragon kernel already initialized",
        "Sending a message fails",
        /*120*/ "Syntax error in received message",
        "Receive unknown command",
        "Less patterns then allocated nodes",
        "Weight update with global sum fails",
        "Learning function not parallelized",
        "Invalid error code", /* RPC-Call failed */
        "Invalid error code", /* RPC-Timeout */
        "Invalid error code", /* RPC: No Server defined */
        "Invalid error code", /* RPC: Cooperativ failed */
        "Invalid error code", /* RPC: No response from the slaves */
        /*130*/ "Algorithm needs patterns. Please press TEST first to check patterns.",
	"RBF-DDA: First input parameter out of range 0<theta_pos<=1.",
	"RBF-DDA: Second input parameter out of range 0<theta_neg<=1.",
	"RBF-DDA: Third input parameter must be >=0.",
	"RBF-DDA: More than one desired class in output pattern.",
	"RBF-DDA: Input-hidden connection pointer problem.",
	"RBF-DDA: Input-output shortcut connections are not allowed.",
	"RBF-DDA: Activation function of input units must be Act_Identity.",
	"RBF-DDA: Activation function of hidden units must be Act_RBF_Gaussian.",
	"RBF-DDA: Activation function of output units must be Act_Identity.",
        /*140*/ "CC error : Invalid additional Parameters.",
        "Activation-functions have to be Act_Threshold.",
        "Learning function must be online Backpropagation",
	"No learning possible with only one class",
	"Invalid pattern remap function",
        "Patterns don't have class information",
	"Illegal virtual class distribution",
	"Patterns can not be normalized"
    };  /* 147 error messages */

    static char *ext_messages[] = {
        "SNNS-Kernel No Errors",
        "SNNS-Kernel Error: ",
        "SNNS-Kernel Network Topologic Error: "
        };

//--------------------------------
//Topological Unit Types for bn_JordElm, cc_glob, ...
//--------------------------------
#define  UNKNOWN    0
#define  INPUT      1
#define  OUTPUT     2
#define  HIDDEN     3
#define  DUAL       4
#define  SPECIAL    5
#define  SPECIAL_I  6
#define  SPECIAL_O  7
#define  SPECIAL_H  8
#define  SPECIAL_D     9
#define  SPECIAL_X    10 /* Only used for toggle special flag */
#define  N_SPECIAL_X  11 /* Only used for toggle special flag */






class SnnsCLib {

public:

  SnnsCLib();
  ~SnnsCLib();

#include "kr_ui.ph" 

#include "bn_JordElm.h"
#include "bn_art1.ph"
#include "bn_kohonen.ph"

#include "kr_newpattern.ph"

private:

//#include "func_tbl.txx"
//the func table

struct FuncTable  kernel_func_table[277];
int  NoOfKernelFuncs;

#include "arttr_f.ph"     
#include "cc_modify.ph"   
#include "kernel.ph"   
#include "kr_funcs.ph"      
#include "scaled_conj_grad.ph"    
#include "trans_f.ph"
#include "art_ui.ph"      
#include "cc_prune.ph"    
#include "kr_amap.ph"  
#include "kr_inversion.ph"  
#include "learn_f.ph"  
#include "update_f.ph"
#include "cc_display.ph"  
#include "dlvq_learn.ph"  
#include "kr_art1.ph"  
#include "kr_io.ph"         
#include "matrix.ph"   
#include "stochastic_learn_f.ph"
#include "cc_glob.ph"     
#include "kr_art2.ph"  
#include "kr_JordElm.ph"    
//#include "kr_rand.ph"        
#include "prun_f.ph"   
#include "tacoma_learn.ph"
#include "cc_learn.ph"    
#include "init_f.ph"      
#include "kr_art.ph"   
#include "kr_mem.ph"        
#include "kr_td.ph"          
#include "remap_f.ph"  
#include "tbl_func.ph"

#include "kr_pat_scan.ph"    
#include "kr_pat_parse.ph"   


int ppparse();


//former not explicitly declared members (some were added to the .ph files)

//kernel

void clr_T_flags();
void DepthFirst1(Unit*, int);
void DepthFirst2(Unit*, int);
void DepthFirst3(Unit*, int);
krui_err kr_topoSortT();
krui_err kr_topoSortFF();
krui_err kr_topoSortIHO();
//int llncompare(const Unit**, const Unit**);
//int transTableCompare(const void*, const void*);
krui_err kr_topoSortLOG();

//kr_JordElm

void kr_recTouchContextUnits(Unit*);

//kr_io

bool matchHead(int);
krui_err krio_writeTimeDelayDefs();



//former static variables

//cc_learn
float m_cc_trainOutputUnits_OutParameter[1];
float m_TEST_CasCor_OutParameter[1];
int   m_cc_learn_GroupNo;
float m_cc_learn_LeTe;

//dlvq_learn

int dlvq_learn_cycleCounter,dlvq_learn_noOfTrainingCycles;
float dlvq_learn_learnParam1,dlvq_learn_learnParam2;
float dlvq_learn_OutParameter[1];

//init_f

char m_init_f_notfirstrun;

//kernel

struct Link  *m_kernel_link_ptr;
struct Unit  *m_kernel_source_unit_ptr,*m_kernel_current_unit_ptr;
struct Site  *m_kernel_current_site_ptr;
RbfFloatMatrix m_kernel_kr_CorrMatrix;
char  m_kernel_activation_func[FUNCTION_NAME_MAX_LEN], m_kernel_output_func[FUNCTION_NAME_MAX_LEN];

#ifdef MASPAR_KERNEL
struct NetFuncParameters  m_kernel_net_func_params;
#endif

//kr_ui

long kr_ui_randomSeedVal;

char  krui_topo_err_msg_msg1[512];
char  krui_topo_err_msg_msg2[512];

char  krui_error_mesg[512], krui_error_aux[512];

char  krui_getUnitActFuncName_act_func_name[FUNCTION_NAME_MAX_LEN];
char  krui_getUnitOutFuncName_out_func_name[FUNCTION_NAME_MAX_LEN];
char  krui_getFTypeActFuncName_act_func_name[FUNCTION_NAME_MAX_LEN];
char  krui_getFTypeOutFuncName_out_func_name[FUNCTION_NAME_MAX_LEN];

char  krui_getSiteFuncName_site_func_name[FUNCTION_NAME_MAX_LEN];

char  krui_getUpdateFunc_updateFunc[FUNCTION_NAME_MAX_LEN];
char  krui_getLearnFunc_learnFunc[FUNCTION_NAME_MAX_LEN];
char  krui_getPrunFunc_prunFunc[FUNCTION_NAME_MAX_LEN];
char  krui_getFFLearnFunc_FFLearnFunc[FUNCTION_NAME_MAX_LEN];

char  krui_getVersion_snns_version[128];

struct FuncInfoDescriptor  krui_getFuncInfo_functionDescr;
struct FuncInfoDescriptor  krui_getFuncParamInfo_functionDescr;

//prun_f

bool PRUNE_Skeletonization_first;

//kr_funcs

struct FuncInfoDescriptor  krf_getFuncName_functionDescr;
struct FuncInfoDescriptor  krf_getCurrentNetworkFunc_func_descr;

//kr_io
char  krio_getIOVersion_getIOVersion[128];
char  krio_repchar_str[180];

char  krio_loadNet_netname_str[81],
      krio_loadNet_netfile_version_str[81],
      krio_loadNet_learn_func[81],
      krio_loadNet_update_func[81],
      krio_loadNet_pruning_func[81],
      krio_loadNet_ff_learn_func[81];

char   *krio_fmtShapeing_formats[18];
char   *krio_writeUnitDefinitions_blank;

char   *krio_getTType_ttype[10];

//kr_mem

int krm_putPattern_in[MAX_NO_OF_VAR_I_DIM],krm_putPattern_out[MAX_NO_OF_VAR_I_DIM];

//kr_td

float  LEARN_TDbackprop_OutParameter[1];
float  TEST_TDbackprop_OutParameter[1];
float  LEARN_TDBP_McClelland_OutParameter[1];

//learn_f

float  LEARN_backprop_OutParameter[1];
float  TEST_backprop_OutParameter[1];
float  LEARN_backpropBatch_OutParameter[1];
float  LEARN_backpropMomentum_OutParameter[1];
float  TEST_backpropMomentum_OutParameter[1];
float  LEARN_backpropWeightDecay_OutParameter[1];
float  LEARN_backpropChunk_OutParameter[1];
float  LEARN_backpropJogChunk_OutParameter[1];
float  LEARN_backpropClassJogChunk_OutParameter[1];
float  LEARN_quickprop_OutParameter[1];
float  LEARN_CPN_OutParameter[1];
float  LEARN_perc_OutParameter[1];
float  LEARN_RBF_OutParameter[1];
float  LEARN_RBF_DDA_OutParameter[1];
float  LEARN_rprop_OutParameter[1];
float  LEARN_RpropMAP_OutParameter[1];
float  TEST_rprop_OutParameter[1];
float  TEST_MAP_OutParameter[1];
float  LEARN_BPTT_OutParameter[1];
float  TEST_BPTT_OutParameter[1];
float  LEARN_BBPTT_OutParameter[1];
float  LEARN_QPTT_OutParameter[1];
float  LEARN_kohonen_OutParameter[1];
float  LEARN_JE_Backprop_OutParameter[1];
float  TEST_JE_Backprop_OutParameter[1];
float  TEST_JE_BackpropMomentum_OutParameter[1];
float  LEARN_JE_BackpropMomentum_OutParameter[1];
float  LEARN_JE_Quickprop_OutParameter[1];
float  LEARN_JE_Rprop_OutParameter[1];
float  TEST_JE_Rprop_OutParameter[1];
float  LEARN_RM_delta_OutParameter[1];
float  LEARN_HEBB_OutParameter[1];

#ifdef RBF_DELTA_PROT
int      RbfLearnAdjustWeights_step;	/* current learning step */
#endif


#ifdef RBF_LEARN_PROT
int    LEARN_RBF_schritt;
#endif

int LEARN_RpropMAP_counter;


//scaled_conj_grad

float    LEARN_SCG_OutParameter[1];
int      LEARN_SCG_k, LEARN_SCG_restart_scg,
         LEARN_SCG_stop_scg,
         LEARN_SCG_success;
int      LEARN_SCG_count_under_tol ; 
float    LEARN_SCG_delta, LEARN_SCG_norm_of_p_2, LEARN_SCG_lambda, LEARN_SCG_lambda_bar, LEARN_SCG_current_error, 
         LEARN_SCG_old_error, LEARN_SCG_norm_of_rk ;

FlintType* *LEARN_SCG_weights ;
FlintType  *LEARN_SCG_old_gradient, *LEARN_SCG_p, *LEARN_SCG_r, *LEARN_SCG_old_weights, *LEARN_SCG_step ;

//stochastic_learn_f

float  LEARN_MonteCarlo_OutParameter[1];
float  TEST_MonteCarlo_OutParameter[1];
float  LEARN_SimulatedAnnealing_OutParameter[1];
float  TEST_SimulatedAnnealing_OutParameter[1];

//update_f

float UPDATE_ART1_syncPropagate_rho;
float UPDATE_ART2_syncPropagate_rho, 
      UPDATE_ART2_syncPropagate_param_a, 
      UPDATE_ART2_syncPropagate_param_b, 
      UPDATE_ART2_syncPropagate_param_c, 
      UPDATE_ART2_syncPropagate_param_d, 
      UPDATE_ART2_syncPropagate_theta;

float UPDATE_ARTMAP_syncPropagate_rho_a;
float UPDATE_ARTMAP_syncPropagate_rho_b;
float UPDATE_ARTMAP_syncPropagate_rho;

//kr_newpattern

float kr_getSubPatData_dummy_data;

bool kr_np_order_pat_entries_shuffle;
int kr_np_order_pat_entries_c_start;
int kr_np_order_pat_entries_c_end;

bool kr_np_order_sub_pat_entries_shuffle;
int kr_np_order_sub_pat_entries_c_start;
int kr_np_order_sub_pat_entries_c_end;

int kr_np_order_chunked_pat_entries_c_start;
int kr_np_order_chunked_pat_entries_c_end;

};

#endif

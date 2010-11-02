#ifndef SNNS_HEADER_ALL
#define SNNS_HEADER_ALL

//#include "config.h"
//#define HAVE_FCNTL_H 1
//#define HAVE_SYS_FILE_H 1
//#define HAVE_SYS_TIME_H 1
//#define HAVE_UNISTD_H 1


#ifndef MAXFLOAT
#include <float.h>
#define MAXFLOAT FLT_MAX
#endif

long snns_lrand48(void);
void snns_srand48(long seedval);
double snns_drand48(void);

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
#include "bn_art2.ph"
#include "bn_kohonen.ph"
#include "bn_artmap.ph"
#include "bn_assoz.ph"

#include "kr_newpattern.ph"

private:

//#include "func_tbl.txx"
//the func table
//struct FuncTable  kernel_func_table[277];
#include "func_tbl.ph"

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

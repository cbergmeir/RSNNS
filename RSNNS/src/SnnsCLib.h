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


#ifndef SNNS_HEADER_ALL
#define SNNS_HEADER_ALL

#ifndef MAXFLOAT
#include <float.h>
#define MAXFLOAT FLT_MAX
#endif

#if __cplusplus > 199711L
#define register      // Deprecated in C++11.
#endif  // #if __cplusplus > 199711L

#include "u_lrand48.h"

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

#include <iostream>
#include <fstream>
#include <sstream>

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

//------------------------------------------------
// kr_ui.h
//------------------------------------------------

#include "kr_ui.h" 

//former static variables

//kr_ui

long kr_ui_randomSeedVal;

char  krui_topo_err_msg_msg1[1024];
char  krui_topo_err_msg_msg2[512];

char  krui_error_mesg[1024], krui_error_aux[512];

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


//------------------------------------------------
// Big net
//------------------------------------------------
#include "bn_JordElm.h"
#include "bn_art1.h"
#include "bn_art2.h"
#include "bn_kohonen.h"
#include "bn_artmap.h"
#include "bn_assoz.h"

//------------------------------------------------
// kr_newpattern.h
//------------------------------------------------
#include "kr_newpattern.h"

//former static variables

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


//------------------------------------------------
// kr_io.h
//------------------------------------------------

#include "kr_io.h"         

//former not explicitly declared members

bool matchHead(int);
krui_err krio_writeTimeDelayDefs();

//former static variables

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

//------------------------------------------------
// art_ui.h
//------------------------------------------------

#include "art_ui.h" 


private:

//------------------------------------------------
// matrix.h
//------------------------------------------------

#include "matrix.h"  

//------------------------------------------------
// func_tbl.h
//------------------------------------------------

#include "func_tbl.h"

//------------------------------------------------
// arttr_f.h
//------------------------------------------------

#include "arttr_f.h"     

//------------------------------------------------
// cc_modify.h
//------------------------------------------------

#include "cc_modify.h"   

//------------------------------------------------
// kernel.h
//------------------------------------------------

#include "kernel.h"   

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

//former static variables

//kernel

struct Link  *m_kernel_link_ptr;
struct Unit  *m_kernel_source_unit_ptr,*m_kernel_current_unit_ptr;
struct Site  *m_kernel_current_site_ptr;
RbfFloatMatrix m_kernel_kr_CorrMatrix;
char  m_kernel_activation_func[FUNCTION_NAME_MAX_LEN], m_kernel_output_func[FUNCTION_NAME_MAX_LEN];

#ifdef MASPAR_KERNEL
struct NetFuncParameters  m_kernel_net_func_params;
#endif


//------------------------------------------------
// kr_funcs.h
//------------------------------------------------

#include "kr_funcs.h"      

//former static variables

//kr_funcs

struct FuncInfoDescriptor  krf_getFuncName_functionDescr;
struct FuncInfoDescriptor  krf_getCurrentNetworkFunc_func_descr;

//------------------------------------------------
// scaled_conj_grad.h
//------------------------------------------------

#include "scaled_conj_grad.h"    

//former static variables

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

//------------------------------------------------
// trans_f.h
//------------------------------------------------

#include "trans_f.h"

//------------------------------------------------
// cc_prune.h
//------------------------------------------------

#include "cc_prune.h"    

//------------------------------------------------
// kr_amap.h
//------------------------------------------------

#include "kr_amap.h"  

//------------------------------------------------
// kr_inversion.h
//------------------------------------------------

#include "kr_inversion.h"  

//------------------------------------------------
// learn_f.h
//------------------------------------------------

#include "learn_f.h"  

//former static variables

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

//------------------------------------------------
// update_f.h
//------------------------------------------------

#include "update_f.h"

//former static variables

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

//------------------------------------------------
// cc_display.h
//------------------------------------------------

#include "cc_display.h"  

//------------------------------------------------
// dlvq_learn.h
//------------------------------------------------

#include "dlvq_learn.h"  

//former static variables

int dlvq_learn_cycleCounter,dlvq_learn_noOfTrainingCycles;
float dlvq_learn_learnParam1,dlvq_learn_learnParam2;
float dlvq_learn_OutParameter[1];

//------------------------------------------------
// kr_art1.h
//------------------------------------------------

#include "kr_art1.h"  

//------------------------------------------------
// stochastic_learn_f.h
//------------------------------------------------

#include "stochastic_learn_f.h"

//former static variables

float  LEARN_MonteCarlo_OutParameter[1];
float  TEST_MonteCarlo_OutParameter[1];
float  LEARN_SimulatedAnnealing_OutParameter[1];
float  TEST_SimulatedAnnealing_OutParameter[1];

//------------------------------------------------
// cc_glob.h
//------------------------------------------------

#include "cc_glob.h"     

//------------------------------------------------
// kr_art2.h
//------------------------------------------------

#include "kr_art2.h"  

//------------------------------------------------
// kr_JordElm.h
//------------------------------------------------

#include "kr_JordElm.h"    

//former not explicitly declared members (some were added to the .ph files)

//kr_JordElm

void kr_recTouchContextUnits(Unit*);

public:

//------------------------------------------------
// prun_f.h
//------------------------------------------------

#include "prun_f.h"   

private:

//former static variables

bool PRUNE_Skeletonization_first;


//------------------------------------------------
// tacoma_learn.h
//------------------------------------------------

#include "tacoma_learn.h"

//------------------------------------------------
// cc_learn.h
//------------------------------------------------

#include "cc_learn.h"    

//former static variables

float m_cc_trainOutputUnits_OutParameter[1];
float m_TEST_CasCor_OutParameter[1];
int   m_cc_learn_GroupNo;
float m_cc_learn_LeTe;

//------------------------------------------------
// init_f.h
//------------------------------------------------

#include "init_f.h"      

//former static variables

char m_init_f_notfirstrun;

//------------------------------------------------
// kr_art.h
//------------------------------------------------

#include "kr_art.h"   

//------------------------------------------------
// kr_mem.h
//------------------------------------------------

#include "kr_mem.h"        

//former static variables

int krm_putPattern_in[MAX_NO_OF_VAR_I_DIM],krm_putPattern_out[MAX_NO_OF_VAR_I_DIM];

//------------------------------------------------
// kr_td.h
//------------------------------------------------

#include "kr_td.h"          

//former static variables

float  LEARN_TDbackprop_OutParameter[1];
float  TEST_TDbackprop_OutParameter[1];
float  LEARN_TDBP_McClelland_OutParameter[1];

//------------------------------------------------
// remap_f.h
//------------------------------------------------

#include "remap_f.h"  

//------------------------------------------------
// tbl_func.h
//------------------------------------------------

#include "tbl_func.h"

//------------------------------------------------
// kr_pat_scan.h
//------------------------------------------------

#include "kr_pat_scan.h"    

//------------------------------------------------
// kr_pat_parse.h
//------------------------------------------------

#include "kr_pat_parse.h"   

int ppparse();

//------------------------------------------------
// Other
//------------------------------------------------

void setIntArrayToZero(int* array, int n);
void setFuncInfoDescriptorToZero(FuncInfoDescriptor* f);

};

#endif

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
#include "kr_newpattern.ph"

private:

//#include "func_tbl.txx"
//the func table

struct FuncTable  kernel_func_table[277];
int  NoOfKernelFuncs;

#include "kr_rand.h"

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
#include "kr_rand.ph"        
#include "prun_f.ph"   
#include "tacoma_learn.ph"
#include "cc_learn.ph"    
#include "init_f.ph"      
#include "kr_art.ph"   
#include "kr_mem.ph"        
#include "kr_td.ph"          
#include "remap_f.ph"  
#include "tbl_func.ph"

//#include "kr_pat_parse.ph"   
//#include "kr_pat_scan.ph"    



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

};

#endif
